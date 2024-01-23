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

#include <CheckDownloadQuotaVolumeService.h>

#include <LTACommon.h>
#include <QuotaServiceCheckDownloadQuotaVolumeRequestPayload.h>
#include <QuotaServiceResponsePayload.h>
#include <dbConnection.h>
#include <SQLString.h>
#include <dbSet.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace lta;

ACS_CLASS_DEFINE_DEBUG_LEVEL(CheckDownloadQuotaVolumeService);

namespace
{
constexpr char OPERATION_TYPE[] = "DOWNLOADDATA";
}  // namespace

CheckDownloadQuotaVolumeService::CheckDownloadQuotaVolumeService(const std::string& service, dbConnectionPool& pool) :
  BaseService(service, pool)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  setPostEnabled(true);
}

void CheckDownloadQuotaVolumeService::handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
                                                    QuotaServiceResponsePayload& responsePayload)
{
  // Get request body
  QuotaServiceCheckDownloadQuotaVolumeRequestPayload requestPayload(request);

  // Get request data
  dbPersistent::IdType userID = requestPayload.getUserID();
  size_t dataSize = requestPayload.getDataSize();

  // Get DB connection
  dbConnectionWrapper cw(getPool());
  dbConnection& conn = cw;

  // Check Quota
  std::string operationUUID = "";
  switch (CheckDownloadQuotaVolume(conn, userID, dataSize, operationUUID))
  {
    case Exits::InvalidUserID:
      // User not found
      responseCode = Pistache::Http::Code::Not_Found;
      responsePayload.setOperationUUID("");
      responsePayload.setGranted(false);
      responsePayload.setReason("Quota not found for UserID '" + std::to_string(userID) + "'");
      break;

    case Exits::QuotaExceeded:
      // Too many data downloaded in the period
      responseCode = Pistache::Http::Code::Too_Many_Requests;
      responsePayload.setOperationUUID("");
      responsePayload.setGranted(false);
      responsePayload.setReason("Too many data downloaded in the period");
      break;

    case Exits::OK:
      // Create a new Download
      responseCode = Pistache::Http::Code::Ok;
      responsePayload.setOperationUUID(operationUUID);
      responsePayload.setGranted(true);
      responsePayload.setReason("");
      break;

    default:
      ACS_THROW(CheckDownloadQuotaVolumeServiceException("Unknow CheckDownloadQuotaVolume exit code"))
  }
}

CheckDownloadQuotaVolumeService::Exits CheckDownloadQuotaVolumeService::CheckDownloadQuotaVolume(
  dbConnection& conn, dbPersistent::IdType userID, size_t dataSize, std::string& operationUUID)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  // Get user data
  size_t size = 0UL;
  double interval = 0.0;

  if (!getUserQuota(conn, userID, size, interval))
  {
    // User not found
    return Exits::InvalidUserID;
  }
  else if (getOperationQuota(conn, userID, interval, OPERATION_TYPE) + dataSize > size)
  {
    // Too many data downloaded in the period
    return Exits::QuotaExceeded;
  }
  else
  {
    // Create a new Download
    createQuotaRegistry(conn, userID, dataSize, OPERATION_TYPE, operationUUID);
    return Exits::OK;
  }
}

bool CheckDownloadQuotaVolumeService::getUserQuota(dbConnection& conn, const dbPersistent::IdType userID, size_t& size,
                                                   double& interval)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  const std::string& dbKey = conn.getKey();

  // Query type - SELECT
  SQLString sqlSel(dbKey, SQLString::SELECT);
  sqlSel.addSelectList("downloadsize");
  sqlSel.addSelectList(SQLString::intervalToSeconds(dbKey, "downloadinterval"), "DURATION_SECONDS");

  // FROM
  sqlSel.addTableList("t_srv_users_quota");

  // WHERE
  dbQueryParameters where(dbKey);
  where.addParameter(dbParam(dbKey, "t_srv_users_quota", "user_id", dbParam::EQUAL_TO, dbConv(dbKey, userID)));
  sqlSel.addANDWhereParameter(where);

  // Execute query
  dbSet set;
  execQuery(conn, sqlSel, "CheckDownload_getUserQuota", set);

  // Get result
  dbRow row(dbKey);
  if (set.getNextRow(row))
  {
    row.column(0U, size);
    row.column(1U, interval);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "User " << userID << " Quota: size = " << size << "; interval = " << interval
                                                << ";");

    return true;
  }

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Quota for User " << userID << " not found");

  return false;
}

_ACS_END_NAMESPACE
