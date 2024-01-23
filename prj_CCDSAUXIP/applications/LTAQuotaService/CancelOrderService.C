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

#include <CancelOrderService.h>

#include <LTACommon.h>
#include <QuotaServiceCancelOrderRequestPayload.h>
#include <QuotaServiceResponsePayload.h>
#include <dbConnection.h>
#include <SQLString.h>
#include <dbSet.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace lta;

ACS_CLASS_DEFINE_DEBUG_LEVEL(CancelOrderService);

namespace
{
constexpr char OPERATION_TYPE_CREATEORDER[] = "CREATEORDER";
constexpr char OPERATION_TYPE_TRIGGERBATCH[] = "TRIGGERBATCH";
}  // namespace

CancelOrderService::CancelOrderService(const std::string& service, dbConnectionPool& pool) :
  BaseService(service, pool)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  setPostEnabled(true);
}

void CancelOrderService::handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
                                       QuotaServiceResponsePayload& responsePayload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Get request body
  QuotaServiceCancelOrderRequestPayload requestPayload(request);

  // Get request data
  dbPersistent::IdType userID = requestPayload.getUserID();
  std::string orderUUID = requestPayload.getOrderUUID();

  // Get row to cancel
  dbPersistent::IdType opQuotaRegID = 0LL;
  if (getRow(userID, orderUUID, opQuotaRegID))
  {
    // Row found, update row
    updateRow(opQuotaRegID, 0LL);
    responseCode = Pistache::Http::Code::Ok;
    responsePayload.setOperationUUID("");
    responsePayload.setGranted(true);
    responsePayload.setReason("");
  }
  else
  {
    // Row not found
    responseCode = Pistache::Http::Code::Not_Found;
    responsePayload.setOperationUUID("");
    responsePayload.setGranted(false);
    std::ostringstream os;
    os << "Order '" << orderUUID << "' not found";
    responsePayload.setReason(os.str());
  }
}

bool CancelOrderService::getRow(const dbPersistent::IdType userID, const std::string& referenceUUID,
                                dbPersistent::IdType& opQuotaRegID) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  const std::string& dbKey = getPool().getKey();
  const LTASrvOpTypes* opTypes = LTASrvOpTypes::instance(dbKey);

  // Query type - SELECT
  SQLString sqlSel(dbKey, SQLString::SELECT);
  sqlSel.addSelectList("id");

  // FROM
  sqlSel.addTableList("t_srv_op_quota_registry");

  // WHERE
  dbQueryParameters where(dbKey);
  where.addParameter(dbParam(dbKey, "t_srv_op_quota_registry", "referenceuuid", dbParam::EQUAL_TO,
                             dbConv(dbKey, referenceUUID)));
  where.addParameter(dbParam(dbKey, "t_srv_op_quota_registry", "user_id", dbParam::EQUAL_TO, dbConv(dbKey, userID)));
  where.addParameter(dbParam(dbKey, "t_srv_op_quota_registry", "cost", dbParam::MAJOR_OF, dbConv(dbKey, 0LL)));
  sqlSel.addANDWhereParameter(where);

  dbQueryParameters orWhere(dbKey);
  orWhere.setParamOperator(dbQueryParameters::OR);
  orWhere.addParameter(dbParam(dbKey, "t_srv_op_quota_registry", "operation_type_id", dbParam::EQUAL_TO,
                               dbConv(dbKey, opTypes->id(OPERATION_TYPE_CREATEORDER))));
  orWhere.addParameter(dbParam(dbKey, "t_srv_op_quota_registry", "operation_type_id", dbParam::EQUAL_TO,
                               dbConv(dbKey, opTypes->id(OPERATION_TYPE_TRIGGERBATCH))));
  sqlSel.addANDWhereParameter(orWhere);

  // SORT
  sqlSel.addOrderByList("operationdate", SQLString::DESC);

  // LIMIT
  sqlSel.setLimit(1UL);

  // Execute query
  dbSet set;
  execQuery(dbConnectionWrapper(getPool()), sqlSel, "CancelOrder_getRow", set);

  // Get result
  dbRow row(dbKey);
  if (1UL == set.size() && set.getNextRow(row))
  {
    row.column(0U, opQuotaRegID);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "User " << userID
                                                << " have a Order with referenceUUID '" << referenceUUID
                                                << "': ID = " << opQuotaRegID);

    return true;
  }

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "User " << userID
                                              << " don't have a Order with referenceUUID '" << referenceUUID << "'");

  return false;
}

void CancelOrderService::updateRow(const dbPersistent::IdType opQuotaRegID, const long long cost)
  const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  // Get DB connection
  dbConnectionWrapper cw(getPool());
  dbConnection& conn = cw;

  dbSmartPersistent sp("t_srv_op_quota_registry", conn);
  sp.setId(opQuotaRegID);
  sp.set("cost", cost, conn);
  sp.update(conn);
}

_ACS_END_NAMESPACE
