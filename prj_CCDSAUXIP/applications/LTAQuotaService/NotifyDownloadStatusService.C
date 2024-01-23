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

#include <NotifyDownloadStatusService.h>

#include <LTACommon.h>
#include <QuotaServiceNotifyDownloadStatusRequestPayload.h>
#include <QuotaServiceResponsePayload.h>
#include <dbConnection.h>
#include <SQLString.h>
#include <dbSet.h>
#include <dbQuery.h>
#include <dbSchemaMetaDataProvider.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace lta;
using namespace db;

ACS_CLASS_DEFINE_DEBUG_LEVEL(NotifyDownloadStatusService);

namespace
{
constexpr char STATUS[] = "IN_USE";
}  // namespace

NotifyDownloadStatusService::NotifyDownloadStatusService(const std::string& service, dbConnectionPool& pool) :
  BaseService(service, pool),
  _useAlias(dbSchemaMetaDataProvider::hasField("t_srv_users", "service_alias", dbConnectionWrapper(pool)))
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  setPostEnabled(true);
}

void NotifyDownloadStatusService::handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
                                                QuotaServiceResponsePayload& responsePayload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  try
  {
    // Get request body
    QuotaServiceNotifyDownloadStatusRequestPayload requestPayload(request);

    // Get request data
    std::string tokenUUID = requestPayload.getTokenUUID();
    std::string fileName = requestPayload.getFileName();
    bool dataSizeSet = false;
    size_t dataSize = requestPayload.getDataSize(&dataSizeSet);
    bool completed = requestPayload.getCompleted();

    dbPersistent::IdType statusID = 0;
    std::string userName = "";
    std::string productType = "";
    size_t* size = nullptr;
    if (!dataSizeSet)
    {
      size = &dataSize;
    }

    // Get Token Data
    if (!getTokenData(tokenUUID, statusID, userName))
    {
      // Token not found
      responseCode = Pistache::Http::Code::Not_Found;
      responsePayload.setOperationUUID("");
      responsePayload.setGranted(false);
      std::ostringstream os;
      os << "Token '" << tokenUUID << "' not found";
      responsePayload.setReason(os.str());
      responsePayload.setTokenUUID("");
    }
    else if (LTAStatus::instance(getPool().getKey())->id(STATUS) != statusID)
    {
      // Too many parallel data downloaded in the period
      responseCode = Pistache::Http::Code::Bad_Request;
      responsePayload.setOperationUUID("");
      responsePayload.setGranted(false);
      std::ostringstream os;
      os << "Token '" << tokenUUID << "' is not in the 'IN_USE' status";
      responsePayload.setReason(os.str());
      responsePayload.setTokenUUID("");
    }
    // Get File Data
    else if (!getFileData(fileName, productType, size))
    {
      // File not found
      responseCode = Pistache::Http::Code::Not_Found;
      responsePayload.setOperationUUID("");
      responsePayload.setGranted(false);
      std::ostringstream os;
      os << "File '" << fileName << "' not found";
      responsePayload.setReason(os.str());
      responsePayload.setTokenUUID("");
    }
    // Update the t_srv_totalmetrics table
    else if (!updateTotalMetrics(fileName, productType, userName, completed, dataSize))
    {
      // Cannot update metrics
      responseCode = Pistache::Http::Code::Internal_Server_Error;
      responsePayload.setOperationUUID("");
      responsePayload.setGranted(false);
      std::ostringstream os;
      os << "Impossible to update the metrics for file '" << fileName << "' of type '" << productType << "'";
      responsePayload.setReason(os.str());
      responsePayload.setTokenUUID("");
    }
    else
    {
      responseCode = Pistache::Http::Code::Ok;
      responsePayload.setOperationUUID("");
      responsePayload.setGranted(true);
      responsePayload.setReason("");
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

bool NotifyDownloadStatusService::getTokenData(const std::string& tokenUUID, dbPersistent::IdType& statusID,
                                               std::string& userName) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  const std::string& dbKey = getPool().getKey();

  // Query type - SELECT
  SQLString sqlSel(dbKey, SQLString::SELECT);
  sqlSel.addSelectList("t_srv_op_dltokens.status_id");
  if (_useAlias)
  {
    sqlSel.addSelectList("COALESCE(t_srv_users.service_alias, t_srv_users.name)");
  }
  else
  {
    sqlSel.addSelectList("t_srv_users.name");
  }

  // FROM
  sqlSel.addTableList("t_srv_op_dltokens");

  // JOIN
  dbQueryParameters join(dbKey);
  join.addParameter(dbParam(dbKey, "t_srv_users", "id", dbParam::EQUAL_TO, "t_srv_op_dltokens.user_id"));
  sqlSel.addJoinList(SQLDialect::INNER, "t_srv_users", join);

  // WHERE
  dbQueryParameters where(dbKey);
  where.addParameter(dbParam(dbKey, "t_srv_op_dltokens", "tokenuuid", dbParam::EQUAL_TO, dbConv(dbKey, tokenUUID)));
  sqlSel.addANDWhereParameter(where);

  dbSet set;
  execQuery(dbConnectionWrapper(getPool()), sqlSel, "NotifyDownload_getTokenStatus", set);

  // Get result
  dbRow row(dbKey);
  if (set.getNextRow(row))
  {
    row.column(0U, statusID);
    row.column(1U, userName);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Token '" << tokenUUID << "' have statusID = " << statusID);

    return true;
  }

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Token '" << tokenUUID << "' not found");

  return false;
}

bool NotifyDownloadStatusService::getFileData(const std::string& fileName, std::string& productType, size_t* size) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  const std::string& dbKey = getPool().getKey();

  // Query type - SELECT
  SQLString sqlSel(dbKey, SQLString::SELECT);
  sqlSel.addSelectList("t_filetypes.filetype");
  if (nullptr != size)
  {
    sqlSel.addSelectList("t_srv_inventory.contentlength");
  }

  // FROM
  sqlSel.addTableList("t_filetypes");

  // JOIN
  dbQueryParameters join(dbKey);
  join.addParameter(dbParam(dbKey, "t_srv_inventory", "filetype_id", dbParam::EQUAL_TO, "t_filetypes.id_filetype"));
  join.addParameter(dbParam(dbKey, "t_srv_inventory", "name", dbParam::EQUAL_TO, dbConv(dbKey, fileName)));
  sqlSel.addJoinList(SQLDialect::INNER, "t_srv_inventory", join);

  dbSet set;
  execQuery(dbConnectionWrapper(getPool()), sqlSel, "NotifyDownload_getFileData", set);

  // Get result
  dbRow row(dbKey);
  if (set.getNextRow(row))
  {
    row.column(0U, productType);
    if (nullptr != size)
    {
      row.column(1U, *size);
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File '" << fileName << "' have productType = '" << productType << "'");

    return true;
  }

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File '" << fileName << "' not found");

  return false;
}

bool NotifyDownloadStatusService::updateTotalMetrics(const std::string& fileName, const std::string& productType,
                                                     const std::string& userName, const bool completed,
                                                     const size_t size) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

  const std::string& dbKey = getPool().getKey();

  std::string type = productType.substr(3UL);
  std::string platformName = "SENTINEL-" + fileName.substr(1UL, 1UL);
  std::string platformIdentifier = fileName.substr(2UL, 1UL);
  short unsigned int completedCount = 0U;
  short unsigned int failCount = 1U;
  if (completed)
  {
    completedCount = 1U;
    failCount = 0U;
  }

  // POSTGRES QUERY:
  // INSERT INTO
  //     t_srv_totalmetrics(
  //         producttype,
  //         platformshortname,
  //         platformserialidentifier,
  //         downloadsize,
  //         downloadcompletecount,
  //         downloadfailcount,
  //         servicealias
  //     )
  // VALUES
  //     ('<type>', '<platformName>', '<platformIdentifier>', <size>, <completedCount>, <failCount>, '<userName>')
  // ON CONFLICT (producttype, platformshortname, platformserialidentifier, COALESCE(servicealias, 'DUMMY')) DO
  // UPDATE
  // SET
  //     downloadsize = t_srv_totalmetrics.downloadsize + EXCLUDED.downloadsize,
  //     downloadcompletecount = t_srv_totalmetrics.downloadcompletecount + EXCLUDED.downloadcompletecount,
  //     downloadfailcount = t_srv_totalmetrics.downloadfailcount + EXCLUDED.downloadfailcount;

  std::ostringstream os;
  os << "INSERT INTO t_srv_totalmetrics("
        "producttype, "
        "platformshortname, "
        "platformserialidentifier, "
        "downloadsize, "
        "downloadcompletecount, "
        "downloadfailcount, "
        "servicealias) "
        "VALUES ("
     << "'" << type << "', "
     << "'" << platformName << "', "
     << "'" << platformIdentifier << "', "
     << size << ", "
     << completedCount << ", "
     << failCount << ", "
     << "'" << userName << "') "
     << "ON CONFLICT (producttype, platformshortname, platformserialidentifier, COALESCE(servicealias, 'DUMMY')) DO "
        "UPDATE SET "
        "downloadsize = t_srv_totalmetrics.downloadsize + EXCLUDED.downloadsize, "
        "downloadcompletecount = t_srv_totalmetrics.downloadcompletecount + EXCLUDED.downloadcompletecount, "
        "downloadfailcount = t_srv_totalmetrics.downloadfailcount + EXCLUDED.downloadfailcount;";

  SQLString sqlString(dbKey, SQLString::COMMAND);
  sqlString.forceSQLString(os.str());

  try
  {
    // Get DB connection
    dbConnectionWrapper cw(getPool());
    dbConnection& conn = cw;

    // Setup query
    dbQuery query(conn, sqlString, "NotifyDownload_updateTotalMetrics");

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Running query: " << sqlString.getSQL());

    // Execute query
    query.execForStatus();

    return true;
  }
  catch (const std::exception& e)  // NOSONAR - Want to catch all
  {
    ACS_LOG_ERROR("Impossible to update the metrics for file '" << fileName << "' of type '" << productType
                                                                << "'. Error:\n");
    ACS_LOG_NOTIFY_EX(e);
  }

  return false;
}

_ACS_END_NAMESPACE
