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

#include <CheckOrderQuotaService.h>

#include <LTACommon.h>
#include <QuotaServiceCheckOrderQuotaRequestPayload.h>
#include <QuotaServiceResponsePayload.h>
#include <dbConnection.h>
#include <SQLString.h>
#include <dbSet.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace lta;

ACS_CLASS_DEFINE_DEBUG_LEVEL(CheckOrderQuotaService);

namespace
{
constexpr char OPERATION_TYPE[] = "CREATEORDER";
constexpr size_t COST = 1UL;
}  // namespace

CheckOrderQuotaService::CheckOrderQuotaService(const std::string& service, dbConnectionPool& pool) :
  BaseService(service, pool)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  setPostEnabled(true);
}

void CheckOrderQuotaService::handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
                                           QuotaServiceResponsePayload& responsePayload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Get request body
  QuotaServiceCheckOrderQuotaRequestPayload requestPayload(request);

  // Get request data
  dbPersistent::IdType userID = requestPayload.getUserID();

  // Get user data
  long long int size = 0LL;
  double interval = 0.0;
  if (!getUserQuota(userID, size, interval))
  {
    // User not found
    responseCode = Pistache::Http::Code::Not_Found;
    responsePayload.setOperationUUID("");
    responsePayload.setGranted(false);
    std::ostringstream os;
    os << "Quota not found for UserID '" << userID << "'";
    responsePayload.setReason(os.str());
  }
  else if (getOperationQuota(dbConnectionWrapper(getPool()), userID, interval, OPERATION_TYPE) >= size)
  {
    // Too many Create Order requests
    responseCode = Pistache::Http::Code::Too_Many_Requests;
    responsePayload.setOperationUUID("");
    responsePayload.setGranted(false);
    responsePayload.setReason("Too many Create Order requests");
  }
  else
  {
    // Create a new Order
    std::string operationUUID;
    createQuotaRegistry(dbConnectionWrapper(getPool()), userID, COST, OPERATION_TYPE, operationUUID);
    responseCode = Pistache::Http::Code::Ok;
    responsePayload.setOperationUUID(operationUUID);
    responsePayload.setGranted(true);
    responsePayload.setReason("");
  }
}

bool CheckOrderQuotaService::getUserQuota(const dbPersistent::IdType userID, long long int& size,
                                          double& interval) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  const std::string& dbKey = getPool().getKey();

  // Query type - SELECT
  SQLString sqlSel(dbKey, SQLString::SELECT);
  sqlSel.addSelectList("ordersize");
  sqlSel.addSelectList(SQLString::intervalToSeconds(dbKey, "orderinterval"), "DURATION_SECONDS");

  // FROM
  sqlSel.addTableList("t_srv_users_quota");

  // WHERE
  dbQueryParameters where(dbKey);
  where.addParameter(dbParam(dbKey, "t_srv_users_quota", "user_id", dbParam::EQUAL_TO, dbConv(dbKey, userID)));
  sqlSel.addANDWhereParameter(where);

  dbSet set;
  execQuery(dbConnectionWrapper(getPool()), sqlSel, "CheckOrder_getUserQuota", set);

  // Get result
  dbRow row(dbKey);
  if (set.getNextRow(row))
  {
    row.column(0U, size);
    row.column(1U, interval);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "User " << userID << " Quota: size = " << size << "; interval = " << interval);

    return true;
  }

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Quota for User " << userID << " not found");

  return false;
}

_ACS_END_NAMESPACE
