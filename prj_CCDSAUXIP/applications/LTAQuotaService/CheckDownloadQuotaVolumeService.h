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

#ifndef _CheckDownloadQuotaVolumeService_H_
#define _CheckDownloadQuotaVolumeService_H_

#include <BaseService.h>
#include <dbConnectionPool.h>
#include <dbSmartPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

class CheckDownloadQuotaVolumeService : public BaseService
{
public:
  exDECLARE_EXCEPTION(CheckDownloadQuotaVolumeServiceException, exException);

  enum class Exits
  {
    OK,
    InvalidUserID,
    QuotaExceeded
  };
  static Exits CheckDownloadQuotaVolume(dbConnection& conn, dbPersistent::IdType userID, size_t dataSize,
                                        std::string& operationUUID);

private:
  static bool getUserQuota(dbConnection& conn, const dbPersistent::IdType userID, size_t& size, double& interval);
  static long long int getDownloadsSize(dbConnection& conn, const dbPersistent::IdType userID, const double interval);

public:
  explicit CheckDownloadQuotaVolumeService(const std::string& service, dbConnectionPool& pool);

  /** Defaulted special functions */
  ~CheckDownloadQuotaVolumeService() noexcept override = default;
  CheckDownloadQuotaVolumeService(const CheckDownloadQuotaVolumeService& service) = default;
  CheckDownloadQuotaVolumeService& operator=(const CheckDownloadQuotaVolumeService& service) = default;

  /** Deleted special functions */
  CheckDownloadQuotaVolumeService() = delete;

protected:
  /** Handle a request. This function will be called when a Post or a Delete request is received. */
  void handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
                     QuotaServiceResponsePayload& responsePayload) override;

private:
  ACS_CLASS_DECLARE_DEBUG_LEVEL(CheckDownloadQuotaVolumeService);
};

_ACS_END_NAMESPACE

#endif /* _CheckDownloadQuotaVolumeService_H_ */
