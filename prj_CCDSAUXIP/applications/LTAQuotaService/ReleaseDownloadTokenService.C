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

#include <ReleaseDownloadTokenService.h>

#include <LTACommon.h>
#include <QuotaServiceReleaseDownloadTokenRequestPayload.h>
#include <QuotaServiceResponsePayload.h>
#include <SQLString.h>
#include <dbSet.h>
#include <Transaction.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace lta;
using namespace db;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ReleaseDownloadTokenService);

namespace
{
constexpr char OPERATION_TYPE[] = "DELETEDLTOKEN";
constexpr size_t COST = 0UL;
}  // namespace

ReleaseDownloadTokenService::ReleaseDownloadTokenService(const std::string& service, dbConnectionPool& pool) :
  BaseService(service, pool)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  setDeleteEnabled(true);
}

void ReleaseDownloadTokenService::handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
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
    QuotaServiceReleaseDownloadTokenRequestPayload requestPayload(request);

    // Get request data
    dbPersistent::IdType userID = requestPayload.getUserID();
    std::string tokenUUID = requestPayload.getTokenUUID();

    // Check Quota
    std::string operationUUID = "";
    if (deleteToken(conn, userID, tokenUUID))
    {
      // Create a new row in registry
      createQuotaRegistry(conn, userID, COST, OPERATION_TYPE, operationUUID, &tokenUUID);
      responseCode = Pistache::Http::Code::Ok;
      responsePayload.setOperationUUID(operationUUID);
      responsePayload.setGranted(true);
      responsePayload.setReason("");
    }
    else
    {
      // Granted = true in any case
      responseCode = Pistache::Http::Code::Ok;
      responsePayload.setOperationUUID("");
      responsePayload.setGranted(true);
      responsePayload.setReason("");
    }
  }
  catch (const std::exception&)
  {
    // In case of error roll back all the changes
    myTransaction.roll_back();

    throw;
  }

  myTransaction.commit();
}

bool ReleaseDownloadTokenService::deleteToken(dbConnection& conn, const dbPersistent::IdType userID,
                                              const std::string& tokenUUID) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  const std::string& dbKey = conn.getKey();

  // Query type - DELETE
  SQLString sqlDel(dbKey, SQLString::DELETE);

  // FROM
  sqlDel.addTableList("t_srv_op_dltokens");

  // WHERE
  dbQueryParameters where(dbKey);
  where.addParameter(dbParam(dbKey, "t_srv_op_dltokens", "user_id", dbParam::EQUAL_TO, dbConv(dbKey, userID)));
  where.addParameter(dbParam(dbKey, "t_srv_op_dltokens", "tokenuuid", dbParam::EQUAL_TO, dbConv(dbKey, tokenUUID)));
  sqlDel.addANDWhereParameter(where);

  // Extract all the deleted rows for logging
  std::string sqlString = sqlDel.getSQL();
  std::string::size_type pos = sqlString.find_last_of(';');
  sqlDel.setType(SQLString::COMMAND, false);
  sqlDel.forceSQLString(sqlString.replace(pos, 1U, " RETURNING id;"));

  // Execute query
  dbSet set;
  execQuery(conn, sqlDel, "ReleaseDownToken_deleteToken", set);

  // For Postgres DB: check if a row was updated
  dbRow row(dbKey);
  dbPersistent::IdType rowID = 0LL;
  if (1UL != set.size() || !set.getNextRow(row))
  {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "User " << userID << " don't have a Token with tokenUUID = '" << tokenUUID
                                                << "'");

    return false;
  }

  bool isNull = true;
  row.column(0U, rowID, &isNull);
  if (isNull || 0LL == rowID)
  {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "User " << userID << " don't have a Token with tokenUUID = '" << tokenUUID
                                                << "'");
    return false;
  }

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "User " << userID << " has deleted his Token with tokenUUID = '" << tokenUUID
                                              << "'");

  return true;
}

_ACS_END_NAMESPACE
