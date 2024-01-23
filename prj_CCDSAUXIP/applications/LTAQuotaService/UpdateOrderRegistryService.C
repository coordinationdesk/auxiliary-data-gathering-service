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

#include <UpdateOrderRegistryService.h>

#include <LTACommon.h>
#include <QuotaServiceUpdateOrderRegistryRequestPayload.h>
#include <QuotaServiceResponsePayload.h>
#include <dbConnection.h>
#include <SQLString.h>
#include <dbSet.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace lta;

ACS_CLASS_DEFINE_DEBUG_LEVEL(UpdateOrderRegistryService);

namespace
{
constexpr char OPERATION_TYPE[] = "CREATEORDER";
}  // namespace

UpdateOrderRegistryService::UpdateOrderRegistryService(const std::string& service, dbConnectionPool& pool) :
  BaseService(service, pool)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  setPostEnabled(true);
}

void UpdateOrderRegistryService::handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
                                               QuotaServiceResponsePayload& responsePayload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Get request body
  QuotaServiceUpdateOrderRegistryRequestPayload requestPayload(request);

  // Get request data
  std::string registryUUID = requestPayload.getRegistryUUID();
  std::string orderUUID = requestPayload.getOrderUUID();

  // Get row to update
  dbPersistent::IdType opQuotaRegID = 0LL;
  if (!checkOrderUUID(orderUUID))
  {
    // OrderID not found
    responseCode = Pistache::Http::Code::Not_Found;
    responsePayload.setOperationUUID("");
    responsePayload.setGranted(false);
    std::ostringstream os;
    os << "OrderUUID '" << orderUUID << "' not found";
    responsePayload.setReason(os.str());
  }
  else if (!getRow(registryUUID, opQuotaRegID))
  {
    // RegistryUUID not found
    responseCode = Pistache::Http::Code::Not_Found;
    responsePayload.setOperationUUID("");
    responsePayload.setGranted(false);
    std::ostringstream os;
    os << "RegistryUUID '" << registryUUID << "' not found";
    responsePayload.setReason(os.str());
  }
  else
  {
    // Create a new Download
    updateRow(opQuotaRegID, orderUUID);
    responseCode = Pistache::Http::Code::Ok;
    responsePayload.setOperationUUID("");
    responsePayload.setGranted(true);
    responsePayload.setReason("");
  }
}

bool UpdateOrderRegistryService::checkOrderUUID(const std::string& orderUUID) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  const std::string& dbKey = getPool().getKey();

  // Query type - SELECT
  SQLString sqlSel(dbKey, SQLString::SELECT);
  sqlSel.addSelectList("COUNT(*)");

  // FROM
  sqlSel.addTableList("t_srv_ordersqueue");

  // WHERE
  dbQueryParameters where(dbKey);
  where.addParameter(dbParam(dbKey, "t_srv_ordersqueue", "uuid", dbParam::EQUAL_TO, dbConv(dbKey, orderUUID)));
  sqlSel.addANDWhereParameter(where);

  dbSet set;
  execQuery(dbConnectionWrapper(getPool()), sqlSel, "UpdateOrderReg_checkOrderUUID", set);

  // Get result
  unsigned long long int count = 0ULL;
  dbRow row(dbKey);
  if (set.getNextRow(row))
  {
    row.column(0U, count);
  }

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "OrderUUID found: " << ((1ULL == count) ? "true" : "false"));

  return 1ULL == count;
}

bool UpdateOrderRegistryService::getRow(const std::string& operationUUID, dbPersistent::IdType& opQuotaRegID) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  const std::string& dbKey = getPool().getKey();

  // Query type - SELECT
  SQLString sqlSel(dbKey, SQLString::SELECT);
  sqlSel.addSelectList("id");

  // FROM
  sqlSel.addTableList("t_srv_op_quota_registry");

  // WHERE
  const LTASrvOpTypes* opTypes = LTASrvOpTypes::instance(dbKey);
  dbQueryParameters where(dbKey);
  where.addParameter(dbParam(dbKey, "t_srv_op_quota_registry", "operationuuid", dbParam::EQUAL_TO,
                             dbConv(dbKey, operationUUID)));
  where.addParameter(dbParam(dbKey, "t_srv_op_quota_registry", "referenceuuid", dbParam::ISNULL));
  where.addParameter(dbParam(dbKey, "t_srv_op_quota_registry", "cost", dbParam::MAJOR_OF, dbConv(dbKey, 0UL)));
  where.addParameter(dbParam(dbKey, "t_srv_op_quota_registry", "operation_type_id", dbParam::EQUAL_TO,
                             dbConv(dbKey, opTypes->id(OPERATION_TYPE))));
  sqlSel.addANDWhereParameter(where);

  dbSet set;
  execQuery(dbConnectionWrapper(getPool()), sqlSel, "UpdateOrderReg_getRow", set);

  // Get result
  dbRow row(dbKey);
  if (1UL == set.size() && set.getNextRow(row))
  {
    row.column(0U, opQuotaRegID);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Registry row with operationUUID = '" << operationUUID
                                                                              << "' found: ID = " << opQuotaRegID);

    return true;
  }

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Registry row with operationUUID = '" << operationUUID << "' not found");

  return false;
}

void UpdateOrderRegistryService::updateRow(const dbPersistent::IdType opQuotaRegID,
                                           const std::string& referenceUUID) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  // Get DB connection
  dbConnectionWrapper cw(getPool());
  dbConnection& conn = cw;

  dbSmartPersistent sp("t_srv_op_quota_registry", conn);
  sp.setId(opQuotaRegID);
  sp.set("referenceuuid", referenceUUID, conn);
  sp.update(conn);
}
_ACS_END_NAMESPACE
