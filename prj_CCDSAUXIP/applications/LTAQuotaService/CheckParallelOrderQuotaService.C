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

#include <CheckParallelOrderQuotaService.h>

#include <LTACommon.h>
#include <QuotaServiceCheckParallelOrderQuotaRequestPayload.h>
#include <QuotaServiceResponsePayload.h>
#include <dbConnection.h>
#include <SQLString.h>
#include <dbSet.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace lta;

ACS_CLASS_DEFINE_DEBUG_LEVEL(CheckParallelOrderQuotaService);

namespace
{
constexpr char STATUS_LTA_DOWNLOADING[] = "LTA_DOWNLOADING";
constexpr char STATUS_DISTRIBUTION[] = "DISTRIBUTION";
}  // namespace

CheckParallelOrderQuotaService::CheckParallelOrderQuotaService(const std::string& service, dbConnectionPool& pool) :
  BaseService(service, pool)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  setPostEnabled(true);
}

void CheckParallelOrderQuotaService::handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
                                                   QuotaServiceResponsePayload& responsePayload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Get request body
  QuotaServiceCheckParallelOrderQuotaRequestPayload requestPayload(request);

  // Get request data
  std::string orderUUID = requestPayload.getOrderUUID();

  // Get user list
  std::vector<dbPersistent::IdType> userIDList;
  if (!getUsersFromOrder(orderUUID, userIDList))
  {
    // OrderUUID not found
    responseCode = Pistache::Http::Code::Not_Found;
    responsePayload.setOperationUUID("");
    responsePayload.setGranted(false);
    std::ostringstream os;
    os << "Invalid OrderUUID '" << orderUUID << "'";
    responsePayload.setReason(os.str());
  }
  else if (!checkUsersParallelQuota(userIDList))
  {
    // Too many parallel Batch requests
    responseCode = Pistache::Http::Code::Too_Many_Requests;
    responsePayload.setOperationUUID("");
    responsePayload.setGranted(false);
    responsePayload.setReason("Too many parallel Order requests");
  }
  else
  {
    responseCode = Pistache::Http::Code::Ok;
    responsePayload.setOperationUUID("");
    responsePayload.setGranted(true);
    responsePayload.setReason("");
  }
}

bool CheckParallelOrderQuotaService::getUsersFromOrder(const std::string& orderUUID,
                                                       std::vector<dbPersistent::IdType>& userIDList) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  /* SQL QUERY used:
  (
    SELECT
      t_srv_request_x_user.user_id AS user_id
    FROM
      t_srv_request_x_user
      INNER JOIN t_srv_ordersqueue ON (
        t_srv_ordersqueue.id = t_srv_request_x_user.order_id
        AND t_srv_ordersqueue.uuid = orderUUID
      )
  )
  UNION
  (
    SELECT
      t_srv_bulk_orders.user_id AS user_id
    FROM
      t_srv_bulk_orders
      INNER JOIN t_srv_bulkbatches ON (
        t_srv_bulkbatches.bulk_id = t_srv_bulk_orders.id
      )
      INNER JOIN t_srv_bulkbatch_x_ordersqueue ON (
        t_srv_bulkbatch_x_ordersqueue.bulkbatch_id = t_srv_bulkbatches.id
      )
      INNER JOIN t_srv_ordersqueue ON (
        t_srv_ordersqueue.id = t_srv_bulkbatch_x_ordersqueue.order_id
        AND t_srv_ordersqueue.uuid = orderUUID
      )
  );
  */
  const std::string& dbKey = getPool().getKey();

  // Query type - UNION
  SQLString sqlUnion(dbKey, SQLString::UNION);
  // First query - order
  // Query type - SELECT
  SQLString sqlSelOrder(dbKey, SQLString::SELECT);
  sqlSelOrder.addSelectList("t_srv_request_x_user.user_id", "user_id");

  // FROM
  sqlSelOrder.addTableList("t_srv_request_x_user");

  // JOIN
  dbQueryParameters orderJoin(dbKey);
  orderJoin.addParameter(dbParam(dbKey, "t_srv_ordersqueue", "id", dbParam::EQUAL_TO,
                                 "t_srv_request_x_user.order_id"));
  orderJoin.addParameter(dbParam(dbKey, "t_srv_ordersqueue", "uuid", dbParam::EQUAL_TO, dbConv(dbKey, orderUUID)));
  sqlSelOrder.addJoinList(SQLDialect::INNER, "t_srv_ordersqueue", orderJoin);

  // Add the table to union list
  sqlUnion.addUnionList(sqlSelOrder);

  // Second query - batch
  // Query type - SELECT
  SQLString sqlSelBatch(dbKey, SQLString::SELECT);
  sqlSelBatch.addSelectList("t_srv_bulk_orders.user_id", "user_id");

  // FROM
  sqlSelBatch.addTableList("t_srv_bulk_orders");

  // JOIN 1
  dbQueryParameters batchJoin1(dbKey);
  batchJoin1.addParameter(dbParam(dbKey, "t_srv_bulkbatches", "bulk_id", dbParam::EQUAL_TO, "t_srv_bulk_orders.id"));
  sqlSelBatch.addJoinList(SQLDialect::INNER, "t_srv_bulkbatches", batchJoin1);

  // JOIN 2
  dbQueryParameters batchJoin2(dbKey);
  batchJoin2.addParameter(dbParam(dbKey, "t_srv_bulkbatch_x_ordersqueue", "bulkbatch_id", dbParam::EQUAL_TO,
                                  "t_srv_bulkbatches.id"));
  sqlSelBatch.addJoinList(SQLDialect::INNER, "t_srv_bulkbatch_x_ordersqueue", batchJoin2);

  // JOIN 3
  dbQueryParameters batchJoin3(dbKey);
  batchJoin3.addParameter(dbParam(dbKey, "t_srv_ordersqueue", "id", dbParam::EQUAL_TO,
                                  "t_srv_bulkbatch_x_ordersqueue.order_id"));
  batchJoin3.addParameter(dbParam(dbKey, "t_srv_ordersqueue", "uuid", dbParam::EQUAL_TO, dbConv(dbKey, orderUUID)));
  sqlSelBatch.addJoinList(SQLDialect::INNER, "t_srv_ordersqueue", batchJoin3);

  // Add the table to union list
  sqlUnion.addUnionList(sqlSelBatch);

  dbSet set;
  execQuery(dbConnectionWrapper(getPool()), sqlUnion, "CheckParallelOrd_UsersFromOrder", set);

  // Get result
  dbRow row(dbKey);
  userIDList.clear();
  dbPersistent::IdType userID;
  while (set.getNextRow(row))
  {
    row.column(0U, userID);
    userIDList.push_back(userID);
  }

  ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
  std::ostringstream os;
  os << "Users found: " << userIDList.size() << "; User ID List = {";
  std::copy(userIDList.begin(), userIDList.end(), std::ostream_iterator<long long>(os, ", "));
  os << "};";
  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, os.str());
  ACS_CLASS_END_DEBUG

  return (!userIDList.empty());
}

bool CheckParallelOrderQuotaService::checkUsersParallelQuota(const std::vector<dbPersistent::IdType>& userIDList) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  /* SQL QUERY used:
  SELECT
    (
      CASE
        WHEN COUNT(*) = 0 THEN (
          SELECT
            COUNT(*)
          FROM
            t_srv_users_quota
          WHERE
            t_srv_users_quota.orderparallel > 0
            AND t_srv_users_quota.user_id IN (userIDList)
        )
        ELSE (
          COUNT(
            CASE
              WHEN nested_users.order_count < t_srv_users_quota.orderparallel THEN nested_users.user_id
            END
          )
        )
      END
    ) AS count
  FROM
    (
      SELECT
        nested_orders.user_id AS user_id,
        COUNT(*) AS order_count
      FROM
        (
          (
            SELECT
              t_srv_request_x_user.user_id AS user_id,
              t_srv_ordersqueue.id AS order_id
            FROM
              t_srv_ordersqueue
              INNER JOIN t_srv_request_x_user ON (
                t_srv_request_x_user.order_id = t_srv_ordersqueue.id
                AND t_srv_request_x_user.user_id IN (userIDList)
                AND t_srv_ordersqueue.status_id IN (LTA_DOWNLOADING, DISTRIBUTION)
              )
          )
          UNION
          (
            SELECT
              t_srv_bulk_orders.user_id AS user_id,
              t_srv_ordersqueue.id AS order_id
            FROM
              t_srv_ordersqueue
              INNER JOIN t_srv_bulkbatch_x_ordersqueue ON (
                t_srv_bulkbatch_x_ordersqueue.order_id = t_srv_ordersqueue.id
                AND t_srv_ordersqueue.status_id IN (LTA_DOWNLOADING, DISTRIBUTION)
              )
              INNER JOIN t_srv_bulkbatches ON (
                t_srv_bulkbatches.id = t_srv_bulkbatch_x_ordersqueue.bulkbatch_id
              )
              INNER JOIN t_srv_bulk_orders ON (
                t_srv_bulk_orders.id = t_srv_bulkbatches.bulk_id
                AND t_srv_bulk_orders.user_id IN (userIDList)
              )
          )
        ) AS nested_orders
      GROUP BY
        nested_orders.user_id
    ) AS nested_users
    INNER JOIN t_srv_users_quota ON (
      t_srv_users_quota.user_id = nested_users.user_id
  );
  */
  const std::string& dbKey = getPool().getKey();

  // Query type - SELECT
  SQLString sqlSel(dbKey, SQLString::SELECT);

  // CASE
  SQLString::SQLCase sqlCase;

  // WHEN COUNT(nested_users.user_id) = 0
  dbQueryParameters whenCase(dbKey);
  SQLString sqlCommand(dbKey, SQLString::COMMAND);
  sqlCommand.forceSQLString("COUNT(*)");
  whenCase.addParameter(dbParam(dbKey, dbParam::EQUAL_TO, sqlCommand, dbConv(dbKey, 0LL)));

  // THEN SELECT
  SQLString sqlThenSel(dbKey, SQLString::SELECT);
  sqlThenSel.addSelectList("COUNT(*)");

  // FROM
  sqlThenSel.addTableList("t_srv_users_quota");

  // WHERE
  dbQueryParameters whereThenSel(dbKey);
  whereThenSel.addParameter(dbParam(dbKey, "t_srv_users_quota", "orderparallel", dbParam::MAJOR_OF, dbConv(dbKey, 0)));
  whereThenSel.addParameter(dbParam(dbKey, "t_srv_users_quota", "user_id", dbParam::IN, userIDList));
  sqlThenSel.addANDWhereParameter(whereThenSel);

  sqlCase.addWhen(whenCase, sqlThenSel.getSQL(true));

  // ELSE COUNT
  SQLString::SQLCase sqlCountCase;

  // WHEN nested_users.order_count < t_srv_users_quota.orderparallel
  dbQueryParameters whenElse(dbKey);
  whenElse.addParameter(dbParam(dbKey, "nested_users", "order_count", dbParam::MINOR_OF,
                                "t_srv_users_quota.orderparallel"));

  // THEN nested_users.user_id
  sqlCountCase.addWhen(whenElse, "nested_users.user_id");
  std::ostringstream os;
  os << "COUNT(" << sqlCountCase.toString(dbKey) << ")";

  sqlCase.setElse(os.str());

  sqlSel.addSelectList(sqlCase, "count");

  // FROM: nested
  // Query type - SELECT
  SQLString sqlSel_nested_users(dbKey, SQLString::SELECT);
  sqlSel_nested_users.addSelectList("nested_orders.user_id", "user_id");
  sqlSel_nested_users.addSelectList("COUNT(*)", "order_count");

  // FROM: nested
  // Query type - UNION
  SQLString sqlUnion_nested_orders(dbKey, SQLString::UNION);
  // Status list
  const LTAStatus* statuses = LTAStatus::instance(dbKey);
  const std::vector<dbPersistent::IdType> statusList = { statuses->id(STATUS_LTA_DOWNLOADING),
                                                         statuses->id(STATUS_DISTRIBUTION) };

  // First query - order
  // Query type - SELECT
  SQLString sqlSelOrder(dbKey, SQLString::SELECT);
  sqlSelOrder.addSelectList("t_srv_request_x_user.user_id", "user_id");
  sqlSelOrder.addSelectList("t_srv_ordersqueue.id", "order_id");

  // FROM
  sqlSelOrder.addTableList("t_srv_ordersqueue");

  // JOIN
  dbQueryParameters orderJoin(dbKey);
  orderJoin.addParameter(dbParam(dbKey, "t_srv_request_x_user", "order_id", dbParam::EQUAL_TO,
                                 "t_srv_ordersqueue.id"));
  orderJoin.addParameter(dbParam(dbKey, "t_srv_request_x_user", "user_id", dbParam::IN, userIDList));
  orderJoin.addParameter(dbParam(dbKey, "t_srv_ordersqueue", "status_id", dbParam::IN, statusList));
  sqlSelOrder.addJoinList(SQLDialect::INNER, "t_srv_request_x_user", orderJoin);

  // Add the table to union list
  sqlUnion_nested_orders.addUnionList(sqlSelOrder);

  // Second query - batch
  // Query type - SELECT
  SQLString sqlSelBatch(dbKey, SQLString::SELECT);
  sqlSelBatch.addSelectList("t_srv_bulk_orders.user_id", "user_id");
  sqlSelBatch.addSelectList("t_srv_ordersqueue.id", "order_id");

  // FROM
  sqlSelBatch.addTableList("t_srv_ordersqueue");

  // JOIN 1
  dbQueryParameters batchJoin1(dbKey);
  batchJoin1.addParameter(dbParam(dbKey, "t_srv_bulkbatch_x_ordersqueue", "order_id", dbParam::EQUAL_TO,
                                  "t_srv_ordersqueue.id"));
  batchJoin1.addParameter(dbParam(dbKey, "t_srv_ordersqueue", "status_id", dbParam::IN, statusList));
  sqlSelBatch.addJoinList(SQLDialect::INNER, "t_srv_bulkbatch_x_ordersqueue", batchJoin1);

  // JOIN 2
  dbQueryParameters batchJoin2(dbKey);
  batchJoin2.addParameter(dbParam(dbKey, "t_srv_bulkbatches", "id", dbParam::EQUAL_TO,
                                  "t_srv_bulkbatch_x_ordersqueue.bulkbatch_id"));
  sqlSelBatch.addJoinList(SQLDialect::INNER, "t_srv_bulkbatches", batchJoin2);

  // JOIN 3
  dbQueryParameters batchJoin3(dbKey);
  batchJoin3.addParameter(dbParam(dbKey, "t_srv_bulk_orders", "id", dbParam::EQUAL_TO,
                                  "t_srv_bulkbatches.bulk_id"));
  batchJoin3.addParameter(dbParam(dbKey, "t_srv_bulk_orders", "user_id", dbParam::IN, userIDList));
  sqlSelBatch.addJoinList(SQLDialect::INNER, "t_srv_bulk_orders", batchJoin3);

  // Add the table to union list
  sqlUnion_nested_orders.addUnionList(sqlSelBatch);

  sqlSel_nested_users.addTableList(sqlUnion_nested_orders.getSQL(true), "nested_orders");

  // GROUP BY
  sqlSel_nested_users.addGroupByList("nested_orders.user_id");

  sqlSel.addTableList(sqlSel_nested_users.getSQL(true), "nested_users");

  // JOIN
  dbQueryParameters join(dbKey);
  join.addParameter(dbParam(dbKey, "t_srv_users_quota", "user_id", dbParam::EQUAL_TO, "nested_users.user_id"));
  sqlSel.addJoinList(SQLDialect::INNER, "t_srv_users_quota", join);

  dbSet set;
  execQuery(dbConnectionWrapper(getPool()), sqlSel, "CheckParallOrder_CheckUsersQuota", set);

  // Get result
  long long int count = 0LL;
  dbRow row(dbKey);
  if (set.getNextRow(row))
  {
    row.column(0U, count);
  }

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Number of Users with less Order than Quota = " << count);

  return 0LL < count;
}

_ACS_END_NAMESPACE
