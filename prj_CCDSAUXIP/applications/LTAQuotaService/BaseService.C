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

#include <BaseService.h>

#include <QuotaServiceResponsePayload.h>
#include <QuotaServicePayloadsExceptions.h>
#include <dbQuery.h>
#include <LTACommon.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace lta;

ACS_CLASS_DEFINE_DEBUG_LEVEL(BaseService);

BaseService::BaseService(const std::string& service, dbConnectionPool& pool) :
  RestServerBase::RestService(service),
  _pool(pool)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void BaseService::doPost(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
  internalHandleRequest(request, response);
}

void BaseService::doDelete(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
  internalHandleRequest(request, response);
}

void BaseService::internalHandleRequest(const Pistache::Rest::Request& request,
                                        Pistache::Http::ResponseWriter& response)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Initialize response
  Pistache::Http::Code responseCode = Pistache::Http::Code::Bad_Request;
  QuotaServiceResponsePayload responsePayload;
  std::string reason = "Initialization error";

  try
  {
    dumpRequest(request);

    // Always JSON answers
    response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));

    handleRequest(request.body(), responseCode, responsePayload);
    reason = responsePayload.getReason();
  }
  catch (const QuotaServicePayloadsExceptions::RequestPayloadException& e)
  {
    // Error on request parsing
    ACS_LOG_NOTIFY_EX(e);
    responseCode = Pistache::Http::Code::Bad_Request;
    responsePayload.setOperationUUID("");
    responsePayload.setGranted(false);
    responsePayload.setReason(e.description());

    reason = "Request parsing error";
  }
  catch (const QuotaServiceResponsePayload::QuotaServiceResponsePayloadException& e)
  {
    // Error on response creation
    ACS_LOG_NOTIFY_EX(e);
    responseCode = Pistache::Http::Code::Internal_Server_Error;
    responsePayload.setOperationUUID("");
    responsePayload.setGranted(false);
    responsePayload.setReason(e.description());

    reason = "Response creation error";
  }
  catch (const std::exception& e)  // NOSONAR - Want to catch all exceptions
  {
    // Other errors
    ACS_LOG_NOTIFY_EX(e);
    responseCode = Pistache::Http::Code::Internal_Server_Error;
    responsePayload.setOperationUUID("");
    responsePayload.setGranted(false);
    responsePayload.setReason(e.what());

    reason = "Internal error";
  }

  std::ostringstream msg;
  msg << request.method() << " Response [" << request.resource() << "] - HTTP Code: " << responseCode << " - Granted: ";
  if (responsePayload.getGranted())
  {
    msg << "true\n";
  }
  else
  {
    msg << "false - Reason: " << reason << "\n";
  }

  ACS_LOG_INFO(msg.str());

  std::ostringstream os;
  os << responsePayload.toJSON() << "\n";

  response.send(responseCode, os.str());
}

void BaseService::execQuery(dbConnection& conn, const SQLString& sql, const std::string& queryName, dbSet& set)
{
  // Setup query
  dbQuery query(conn, sql, queryName);

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Running query: " << sql.getSQL());

  // Execute query
  query.execForSet(set);
}

long long int BaseService::getOperationQuota(dbConnection& conn, const dbPersistent::IdType userID,
                                             const double interval, const std::string& operation_type)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  const std::string& dbKey = conn.getKey();
  const LTASrvOpTypes* opTypes = LTASrvOpTypes::instance(dbKey);

  // Query type - SELECT
  SQLString sqlSel(dbKey, SQLString::SELECT);
  SQLDialect::SQLCast sqlCast(SQLDialect::SQL_BIGINT);
  sqlSel.addSelectList("COALESCE(SUM(t_srv_op_quota_registry.cost), 0)", sqlCast);

  // FROM
  sqlSel.addTableList("t_srv_op_quota_registry");

  // WHERE
  SQLString elapsedCommand(dbKey, SQLString::COMMAND);
  elapsedCommand.forceSQLString(SQLString::intervalToSeconds(dbKey, "operationdate", dbConv(dbKey, "now()")));

  dbQueryParameters where(dbKey);
  where.addParameter(dbParam(dbKey, "t_srv_op_quota_registry", "user_id", dbParam::EQUAL_TO, dbConv(dbKey, userID)));
  where.addParameter(dbParam(dbKey, dbParam::MINOR_OR_EQUAL_OF, elapsedCommand, dbConv(dbKey, interval)));
  where.addParameter(dbParam(dbKey, "t_srv_op_quota_registry", "operation_type_id", dbParam::EQUAL_TO,
                             dbConv(dbKey, opTypes->id(operation_type))));
  sqlSel.addANDWhereParameter(where);

  dbSet set;
  execQuery(conn, sqlSel, "getOperationQuota", set);

  // Get result
  long long int count = 0LL;
  dbRow row(dbKey);
  if (set.getNextRow(row))
  {
    row.column(0U, count);
  }

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Orders found: " << count);

  return count;
}

void BaseService::createQuotaRegistry(dbConnection& conn, const dbPersistent::IdType userID, const size_t cost,
                                      const std::string& operationType, std::string& operationUUID,
                                      const std::string* referenceUUID)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  const LTASrvOpTypes* opTypes = LTASrvOpTypes::instance(conn.getKey());

  dbSmartPersistent sp("t_srv_op_quota_registry", conn);
  sp.set("user_id", userID, conn);
  if (nullptr != referenceUUID)
  {
    sp.set("referenceuuid", *referenceUUID, conn);
  }
  sp.set("operation_type_id", opTypes->id(operationType), conn);
  sp.set("cost", cost, conn);
  sp.save(conn);
  sp.refresh(conn);

  // operationuuid has a not null constraint
  bool isNull = false;
  sp.get("operationuuid", operationUUID, isNull, conn);
}

_ACS_END_NAMESPACE
