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

#include <GetDownloadTokenService.h>

#include <LTACommon.h>
#include <QuotaServiceGetDownloadTokenRequestPayload.h>
#include <QuotaServiceResponsePayload.h>
#include <dbConnection.h>
#include <SQLString.h>
#include <dbSet.h>
#include <Transaction.h>

#include <LTAQuotaService.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace lta;
using namespace db;

ACS_CLASS_DEFINE_DEBUG_LEVEL(GetDownloadTokenService);

namespace
{
constexpr char OPERATION_TYPE[] = "CREATEDLTOKEN";
constexpr char STATUS[] = "VALID";
constexpr size_t COST = 1UL;
}  // namespace

GetDownloadTokenService::GetDownloadTokenService(const std::string& service, dbConnectionPool& pool) :
  BaseService(service, pool)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  setPostEnabled(true);
}

void GetDownloadTokenService::handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
                                            QuotaServiceResponsePayload& responsePayload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  try
  {
    responsePayload.setTokenUUID("");

    // Clear old Token
    clearOldToken();

    // Get request body
    QuotaServiceGetDownloadTokenRequestPayload requestPayload(request);

    // Get request data
    dbPersistent::IdType userID = requestPayload.getUserID();

    // Get user data
    int parallel = 0;
    std::string operationUUID;
    std::string tokenUUID;
    if (!getUserQuota(userID, parallel))
    {
      // User not found
      responseCode = Pistache::Http::Code::Not_Found;
      responsePayload.setOperationUUID("");
      responsePayload.setGranted(false);
      std::ostringstream os;
      os << "Quota not found for UserID '" << userID << "'";
      responsePayload.setReason(os.str());
      responsePayload.setTokenUUID("");
    }
    else if (getParallelDownloads(userID) >= parallel)
    {
      // Too many parallel data downloaded in the period
      responseCode = Pistache::Http::Code::Too_Many_Requests;
      responsePayload.setOperationUUID("");
      responsePayload.setGranted(false);
      responsePayload.setReason("Too many parallel data downloaded in the period");
      responsePayload.setTokenUUID("");
    }
    else if (createNewRows(userID, operationUUID, tokenUUID))
    {
      // Create a new Download
      responseCode = Pistache::Http::Code::Ok;
      responsePayload.setOperationUUID(operationUUID);
      responsePayload.setGranted(true);
      responsePayload.setReason("");
      responsePayload.setTokenUUID(tokenUUID);
    }
    else
    {
      // Cannot create a new Download
      responseCode = Pistache::Http::Code::Internal_Server_Error;
      responsePayload.setOperationUUID("");
      responsePayload.setGranted(false);
      responsePayload.setReason("Cannot create a new download");
      responsePayload.setTokenUUID("");
    }
  }
  catch (const std::exception&)
  {
    // In case of error reset the token
    responsePayload.setTokenUUID("");

    throw;
  }
}

void GetDownloadTokenService::clearOldToken() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  const std::string& dbKey = getPool().getKey();
  const LTAStatus* statuses = LTAStatus::instance(dbKey);

  SQLString elapsedCommand(dbKey, SQLString::COMMAND);
  elapsedCommand.forceSQLString(SQLString::intervalToSeconds(dbKey, "creationtime", dbConv(dbKey, "now()")));

  // Query type - DELETE
  SQLString sqlDel(dbKey, SQLString::DELETE);

  // FROM
  sqlDel.addTableList("t_srv_op_dltokens");

  // WHERE 1: 'status == valid' and 'now - creationtime > _tokenValidityTimeout'
  dbQueryParameters where(dbKey);
  where.addParameter(dbParam(dbKey, "t_srv_op_dltokens", "status_id", dbParam::EQUAL_TO,
                             dbConv(dbKey, statuses->id(STATUS))));
  where.addParameter(dbParam(dbKey, dbParam::MAJOR_OF, elapsedCommand,
                             dbConv(dbKey, LTAQuotaService::getTokenValidityTimeout())));
  sqlDel.addANDWhereParameter(where);

  // OR WHERE 2: 'now - creationtime > _tokenExpirationTimeOut'
  dbQueryParameters orWhere(dbKey);
  orWhere.addParameter(dbParam(dbKey, dbParam::MAJOR_OF, elapsedCommand,
                               dbConv(dbKey, LTAQuotaService::getTokenExpirationTimeOut())));
  sqlDel.addORWhereParameter(orWhere);

  // In debug, extract all the deleted rows for logging
  ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
  std::string sqlString = sqlDel.getSQL();
  std::string::size_type pos = sqlString.find_last_of(';');
  sqlDel.setType(SQLString::COMMAND, false);
  sqlDel.forceSQLString(sqlString.replace(pos, 1U, " RETURNING *;"));
  ACS_CLASS_END_DEBUG

  dbSet set;
  execQuery(dbConnectionWrapper(getPool()), sqlDel, "GetDownloadToken_clearOltToken", set);

  // In debug, log all the deleted rows
  ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)

  dbRow row(dbKey);
  dbPersistent::IdType rowID = 0;
  dbPersistent::IdType userID = 0;
  std::string tokenUUID = "";
  long long int statusID = 0;
  std::string creationTime = "";
  bool isNull;
  while (set.getNextRow(row))
  {
    std::ostringstream os;
    os << "Row deleted from table 't_srv_op_dltokens': { ";

    row.column(0U, rowID, &isNull);
    if (isNull)
    {
      os << "id: NULL; ";
    }
    else
    {
      os << "id: " << rowID << "; ";
    }

    row.column(1U, userID, &isNull);
    if (isNull)
    {
      os << "user_id: NULL; ";
    }
    else
    {
      os << "user_id: " << userID << "; ";
    }

    row.column(2U, tokenUUID, &isNull);
    if (isNull)
    {
      os << "tokenuuid: NULL; ";
    }
    else
    {
      os << "tokenuuid: '" << tokenUUID << "'; ";
    }

    row.column(3U, statusID, &isNull);
    if (isNull)
    {
      os << "status_id: NULL; ";
    }
    else
    {
      os << "status_id: " << statusID << "; ";
    }

    row.column(4U, creationTime, &isNull);
    if (isNull)
    {
      os << "creationtime: NULL; ";
    }
    else
    {
      os << "creationtime: '" << creationTime << "'; ";
    }

    os << "}";
    ACS_LOG_DEBUG(os.str());
  }

  ACS_CLASS_END_DEBUG
}

bool GetDownloadTokenService::getUserQuota(const dbPersistent::IdType userID, int& parallel) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  const std::string& dbKey = getPool().getKey();

  // Query type - SELECT
  SQLString sqlSel(dbKey, SQLString::SELECT);
  sqlSel.addSelectList("downloadparallel");

  // FROM
  sqlSel.addTableList("t_srv_users_quota");

  // WHERE
  dbQueryParameters where(dbKey);
  where.addParameter(dbParam(dbKey, "t_srv_users_quota", "user_id", dbParam::EQUAL_TO, dbConv(dbKey, userID)));
  sqlSel.addANDWhereParameter(where);

  dbSet set;
  execQuery(dbConnectionWrapper(getPool()), sqlSel, "GetDownloadToken_getUserQuota", set);

  // Get result
  dbRow row(dbKey);
  if (set.getNextRow(row))
  {
    row.column(0U, parallel);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "User " << userID << " Quota: parallel = " << parallel);

    return true;
  }

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Quota for User " << userID << " not found");

  return false;
}

long long int GetDownloadTokenService::getParallelDownloads(const dbPersistent::IdType userID) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  const std::string& dbKey = getPool().getKey();

  // Query type - SELECT
  SQLString sqlSel(dbKey, SQLString::SELECT);
  sqlSel.addSelectList("COUNT(*)");

  // FROM
  sqlSel.addTableList("t_srv_op_dltokens");

  // WHERE
  dbQueryParameters qp(dbKey);
  qp.addParameter(dbParam(dbKey, "t_srv_op_dltokens", "user_id", dbParam::EQUAL_TO, dbConv(dbKey, userID)));
  sqlSel.addANDWhereParameter(qp);

  dbSet set;
  execQuery(dbConnectionWrapper(getPool()), sqlSel, "GetDownloadToken_getParallDown", set);

  // Get result
  long long int count = 0LL;
  dbRow row(dbKey);
  if (set.getNextRow(row))
  {
    row.column(0U, count);
  }

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Parallel Download found: " << count);

  return count;
}

bool GetDownloadTokenService::createNewRows(const dbPersistent::IdType userID, std::string& operationUUID,
                                            std::string& tokenUUID) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  // Get DB connection
  const std::string& dbKey = getPool().getKey();
  dbConnectionWrapper cw(getPool());
  dbConnection& conn = cw;

  Transaction myTransaction(conn);
  myTransaction.begin_work("GetDownloadTokenService_createNewRows");

  try
  {
    // Add a new row in t_srv_op_dltokens
    const LTAStatus* statuses = LTAStatus::instance(dbKey);

    dbSmartPersistent spToken("t_srv_op_dltokens", conn);
    spToken.set("user_id", userID, conn);
    spToken.set("status_id", statuses->id(STATUS), conn);
    spToken.save(conn);
    spToken.refresh(conn);

    // tokenUUID has a not null constraint
    bool isNull = false;
    spToken.get("tokenuuid", tokenUUID, isNull, conn);

    // Add a new row in t_srv_op_quota_registry
    createQuotaRegistry(conn, userID, COST, OPERATION_TYPE, operationUUID, &tokenUUID);
  }
  catch (const std::exception& e)  // NOSONAR - Want to catch all for roll back the transation
  {
    myTransaction.roll_back();

    ACS_LOG_NOTIFY_EX(e);

    return false;
  }

  myTransaction.commit();
  return true;
}

_ACS_END_NAMESPACE
