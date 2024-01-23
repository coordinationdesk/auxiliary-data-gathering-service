/*

    Copyright 1995-2022, Exprivia SpA.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.com

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Exprivia SpA.

    $Prod: LTAQuotaService $

*/

#include <UseDownloadTokenService.h>

#include <LTACommon.h>
#include <QuotaServiceUseDownloadTokenRequestPayload.h>
#include <QuotaServiceResponsePayload.h>
#include <dbConnection.h>
#include <SQLString.h>
#include <dbSet.h>
#include <Transaction.h>

#include <LTAQuotaService.h>
#include <CheckDownloadQuotaVolumeService.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace lta;
using namespace db;

ACS_CLASS_DEFINE_DEBUG_LEVEL(UseDownloadTokenService);

namespace
{
constexpr char OPERATION_TYPE[] = "USEDLTOKEN";
constexpr char INITIAL_STATUS[] = "VALID";
constexpr char FINAL_STATUS[] = "IN_USE";
constexpr size_t COST = 1UL;
}  // namespace

UseDownloadTokenService::UseDownloadTokenService(const std::string& service, dbConnectionPool& pool) :
  BaseService(service, pool)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  setPostEnabled(true);
}

void UseDownloadTokenService::handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
                                            QuotaServiceResponsePayload& responsePayload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Start transaction
  dbConnectionWrapper cw(getPool());
  dbConnection& conn = cw;
  Transaction myTransaction(conn);
  myTransaction.begin_work("UseDownloadTokenService");

  try
  {
    // Get request body
    QuotaServiceUseDownloadTokenRequestPayload requestPayload(request);

    // Get request data
    dbPersistent::IdType userID = requestPayload.getUserID();
    std::string tokenUUID = requestPayload.getTokenUUID();
    size_t dataSize = requestPayload.getDataSize();

    // Check Quota
    std::string operationUUID = "";
    if (useToken(conn, userID, tokenUUID))
    {
      switch (CheckDownloadQuotaVolumeService::CheckDownloadQuotaVolume(conn, userID, dataSize, operationUUID))
      {
        case CheckDownloadQuotaVolumeService::Exits::InvalidUserID:
          // User not found
          myTransaction.roll_back();
          responseCode = Pistache::Http::Code::Not_Found;
          responsePayload.setOperationUUID("");
          responsePayload.setGranted(false);
          responsePayload.setReason("Quota not found for UserID '" + std::to_string(userID) + "'");
          break;
        case CheckDownloadQuotaVolumeService::Exits::QuotaExceeded:
          // Too many data downloaded in the period
          myTransaction.roll_back();
          responseCode = Pistache::Http::Code::Too_Many_Requests;
          responsePayload.setOperationUUID("");
          responsePayload.setGranted(false);
          responsePayload.setReason("Too many data downloaded in the period");
          break;
        case CheckDownloadQuotaVolumeService::Exits::OK:
          // Create a new Download
          operationUUID = "";
          createQuotaRegistry(conn, userID, COST, OPERATION_TYPE, operationUUID, &tokenUUID);
          responseCode = Pistache::Http::Code::Ok;
          responsePayload.setOperationUUID(operationUUID);
          responsePayload.setGranted(true);
          responsePayload.setReason("");

          break;
        default:
          ACS_THROW(UseDownloadTokenServiceException("Unknow CheckDownloadQuotaVolume exit code"))
      }
    }
    else
    {
      // Token not found
      myTransaction.roll_back();
      responseCode = Pistache::Http::Code::Not_Found;
      responsePayload.setOperationUUID("");
      responsePayload.setGranted(false);
      std::ostringstream os;
      os << "Invalid TokenUUID '" << tokenUUID << "' for user UserID '" << userID << "'";
      responsePayload.setReason(os.str());
    }
  }
  catch (const std::exception&)
  {
    // Roll back in case of error
    myTransaction.roll_back();

    throw;
  }

  myTransaction.commit();
}

bool UseDownloadTokenService::useToken(dbConnection& conn, const dbPersistent::IdType userID,
                                       const std::string& tokenUUID) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  const std::string& dbKey = conn.getKey();

  // Query type - UPDATE
  SQLString sqlUpdate(dbKey, SQLString::UPDATE);
  const LTAStatus* statuses = LTAStatus::instance(dbKey);
  sqlUpdate.addSetList("status_id", dbConv(dbKey, statuses->id(FINAL_STATUS)));

  // FROM
  sqlUpdate.addTableList("t_srv_op_dltokens");

  // WHERE
  SQLString elapsedCommand(dbKey, SQLString::COMMAND);
  elapsedCommand.forceSQLString(SQLString::intervalToSeconds(dbKey, "creationtime", dbConv(dbKey, "now()")));

  dbQueryParameters where(dbKey);
  where.addParameter(dbParam(dbKey, "t_srv_op_dltokens", "user_id", dbParam::EQUAL_TO, dbConv(dbKey, userID)));
  where.addParameter(dbParam(dbKey, "t_srv_op_dltokens", "tokenuuid", dbParam::EQUAL_TO, dbConv(dbKey, tokenUUID)));
  where.addParameter(dbParam(dbKey, "t_srv_op_dltokens", "status_id", dbParam::EQUAL_TO,
                             dbConv(dbKey, statuses->id(INITIAL_STATUS))));
  where.addParameter(dbParam(dbKey, dbParam::MINOR_OF, elapsedCommand,
                             dbConv(dbKey, LTAQuotaService::getTokenValidityTimeout())));
  sqlUpdate.addANDWhereParameter(where);

  // For Postgres DB: add a RETURNING id at the end of the sql query
  std::string sqlString = sqlUpdate.getSQL();
  std::string::size_type pos = sqlString.find_last_of(';');
  sqlUpdate.setType(SQLString::COMMAND, false);
  sqlUpdate.forceSQLString(sqlString.replace(pos, 1U, " RETURNING id;"));

  // Execute query
  dbSet set;
  execQuery(conn, sqlUpdate, "UseDownloadToken_useToken", set);

  // Check if a row was updated
  dbRow row(dbKey);
  dbPersistent::IdType rowID = 0LL;
  bool isNull = true;
  if (1UL != set.size() || !set.getNextRow(row))
  {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "User " << userID << " don't have a Token with tokenUUID = '" << tokenUUID
                                                << "'");

    return false;
  }
  else
  {
    row.column(0U, rowID, &isNull);
  }

  if (isNull || 0LL == rowID)
  {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "User " << userID << " don't have a Token with tokenUUID = '" << tokenUUID
                                                << "'");

    return false;
  }

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "User " << userID << " have a Token with tokenUUID = '" << tokenUUID << "'");

  return true;
}

_ACS_END_NAMESPACE
