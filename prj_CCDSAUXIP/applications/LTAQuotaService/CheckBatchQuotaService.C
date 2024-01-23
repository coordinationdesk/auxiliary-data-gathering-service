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

#include <CheckBatchQuotaService.h>

#include <LTACommon.h>
#include <QuotaServiceCheckBatchQuotaRequestPayload.h>
#include <QuotaServiceResponsePayload.h>
#include <dbConnection.h>
#include <SQLString.h>
#include <dbSet.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace lta;

ACS_CLASS_DEFINE_DEBUG_LEVEL(CheckBatchQuotaService);

namespace
{
constexpr char OPERATION_TYPE[] = "TRIGGERBATCH";
constexpr char STATUS[] = "IN_PROGRESS";
constexpr size_t COST = 1UL;
}  // namespace

CheckBatchQuotaService::CheckBatchQuotaService(const std::string& service, dbConnectionPool& pool) :
  BaseService(service, pool)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  setPostEnabled(true);
}

void CheckBatchQuotaService::handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
                                           QuotaServiceResponsePayload& responsePayload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Get request body
  QuotaServiceCheckBatchQuotaRequestPayload requestPayload(request);

  // Get request data
  dbPersistent::IdType userID = requestPayload.getUserID();
  std::string batchUUID = requestPayload.getBatchUUID();

  // Get user data
  long long int size = 0LL;
  double interval = 0.0;
  int parallel = 0;
  if (!getUserQuota(userID, size, interval, parallel))
  {
    // User not found
    responseCode = Pistache::Http::Code::Not_Found;
    responsePayload.setOperationUUID("");
    responsePayload.setGranted(false);
    std::ostringstream os;
    os << "Quota not found for UserID '" << userID << "'";
    responsePayload.setReason(os.str());
  }
  else if (!checkBatchUUID(batchUUID))
  {
    // User not found
    responseCode = Pistache::Http::Code::Not_Found;
    responsePayload.setOperationUUID("");
    responsePayload.setGranted(false);
    std::ostringstream os;
    os << "Invalid BatchUUID '" << batchUUID << "'";
    responsePayload.setReason(os.str());
  }
  else if (getOperationQuota(dbConnectionWrapper(getPool()), userID, interval, OPERATION_TYPE) >= size)
  {
    // Too many Batch requests
    responseCode = Pistache::Http::Code::Too_Many_Requests;
    responsePayload.setOperationUUID("");
    responsePayload.setGranted(false);
    responsePayload.setReason("Too many Batch requests");
  }
  else if (getParallelBatchsOrders(userID) >= parallel)
  {
    // Too many parallel Batch requests
    responseCode = Pistache::Http::Code::Too_Many_Requests;
    responsePayload.setOperationUUID("");
    responsePayload.setGranted(false);
    responsePayload.setReason("Too many parallel Batch requests");
  }
  else
  {
    // Crate a new Bath Order
    std::string operationUUID;
    createQuotaRegistry(dbConnectionWrapper(getPool()), userID, COST, OPERATION_TYPE, operationUUID, &batchUUID);
    responseCode = Pistache::Http::Code::Ok;
    responsePayload.setOperationUUID(operationUUID);
    responsePayload.setGranted(true);
    responsePayload.setReason("");
  }
}

bool CheckBatchQuotaService::getUserQuota(const dbPersistent::IdType userID, long long int& size, double& interval,
                                          int& parallel) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  const std::string& dbKey = getPool().getKey();

  // Query type - SELECT
  SQLString sqlSel(dbKey, SQLString::SELECT);
  sqlSel.addSelectList("batchsize");
  sqlSel.addSelectList(SQLString::intervalToSeconds(dbKey, "batchinterval"), "DURATION_SECONDS");
  sqlSel.addSelectList("batchparallel");

  // FROM
  sqlSel.addTableList("t_srv_users_quota");

  // WHERE
  dbQueryParameters where(dbKey);
  where.addParameter(dbParam(dbKey, "t_srv_users_quota", "user_id", dbParam::EQUAL_TO, dbConv(dbKey, userID)));
  sqlSel.addANDWhereParameter(where);

  dbSet set;
  execQuery(dbConnectionWrapper(getPool()), sqlSel, "CheckBatch_getUserQuotaQuery", set);

  // Get result
  dbRow row(dbKey);
  if (set.getNextRow(row))
  {
    row.column(0U, size);
    row.column(1U, interval);
    row.column(2U, parallel);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "User " << userID << " Quota: size = " << size << "; interval = " << interval
                                                << "; parallel = " << parallel);

    return true;
  }

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Quota for User " << userID << " not found");

  return false;
}

bool CheckBatchQuotaService::checkBatchUUID(const std::string& batchUUID) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  const std::string& dbKey = getPool().getKey();

  // Query type - SELECT
  SQLString sqlSel(dbKey, SQLString::SELECT);
  sqlSel.addSelectList("COUNT(*)");

  // FROM
  sqlSel.addTableList("t_srv_bulkbatches");

  // WHERE
  dbQueryParameters where(dbKey);
  where.addParameter(dbParam(dbKey, "t_srv_bulkbatches", "uuid", dbParam::EQUAL_TO, dbConv(dbKey, batchUUID)));
  sqlSel.addANDWhereParameter(where);

  dbSet set;
  execQuery(dbConnectionWrapper(getPool()), sqlSel, "CheckBatch_checkBatchUUID", set);

  // Get result
  unsigned long long int count = 0ULL;
  dbRow row(dbKey);
  if (set.getNextRow(row))
  {
    row.column(0U, count);
  }

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "BatchUUID found: " << ((1ULL == count) ? "true" : "false"));

  return 1ULL == count;
}

long long int CheckBatchQuotaService::getParallelBatchsOrders(const dbPersistent::IdType userID)
  const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  const std::string& dbKey = getPool().getKey();

  // Query type - SELECT
  SQLString sqlSel(dbKey, SQLString::SELECT);
  SQLDialect::SQLCast sqlCast(SQLDialect::SQL_BIGINT);
  sqlSel.addSelectList("COALESCE(SUM(t_srv_op_quota_registry.cost), 0)", sqlCast);

  // FROM
  sqlSel.addTableList("t_srv_op_quota_registry");

  // JOIN
  dbQueryParameters join(dbKey);
  join.addParameter(dbParam(dbKey, "t_srv_bulkbatches", "uuid", dbParam::EQUAL_TO,
                            "t_srv_op_quota_registry.referenceuuid"));
  sqlSel.addJoinList(SQLDialect::INNER, "t_srv_bulkbatches", join);

  // WHERE
  const LTASrvOpTypes* opTypes = LTASrvOpTypes::instance(dbKey);
  const LTAStatus* statuses = LTAStatus::instance(dbKey);
  dbQueryParameters where(dbKey);
  where.addParameter(dbParam(dbKey, "t_srv_op_quota_registry", "user_id", dbParam::EQUAL_TO, dbConv(dbKey, userID)));
  where.addParameter(dbParam(dbKey, "t_srv_op_quota_registry", "operation_type_id", dbParam::EQUAL_TO,
                               dbConv(dbKey, opTypes->id(OPERATION_TYPE))));
  where.addParameter(dbParam(dbKey, "t_srv_bulkbatches", "status_id", dbParam::EQUAL_TO,
                               dbConv(dbKey, statuses->id(STATUS))));
  sqlSel.addANDWhereParameter(where);

  dbSet set;
  execQuery(dbConnectionWrapper(getPool()), sqlSel, "CheckBatch_getBatchsParallOrders", set);

  // Get result
  long long int count = 0LL;
  dbRow row(dbKey);
  if (set.getNextRow(row))
  {
    row.column(0U, count);
  }

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Parallel Batchs Orders found: " << count);

  return count;
}

_ACS_END_NAMESPACE
