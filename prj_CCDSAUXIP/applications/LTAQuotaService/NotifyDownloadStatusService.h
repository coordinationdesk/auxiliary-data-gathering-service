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

#ifndef _NotifyDownloadStatusService_H_
#define _NotifyDownloadStatusService_H_

#include <BaseService.h>
#include <dbConnectionPool.h>
#include <dbSmartPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

class NotifyDownloadStatusService : public BaseService
{
public:
  explicit NotifyDownloadStatusService(const std::string& service, dbConnectionPool& pool);

  /** Defaulted special functions */
  ~NotifyDownloadStatusService() noexcept override = default;
  NotifyDownloadStatusService(const NotifyDownloadStatusService& service) = default;
  NotifyDownloadStatusService& operator=(const NotifyDownloadStatusService& service) = default;

  /** Deleted special functions */
  NotifyDownloadStatusService() = delete;

protected:
  /** Handle a request. This function will be called when a Post or a Delete request is received. */
  void handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
                     QuotaServiceResponsePayload& responsePayload) override;

private:
  bool getTokenData(const std::string& tokenUUID, dbPersistent::IdType& statusID, std::string& userName) const;
  bool getFileData(const std::string& fileName, std::string& productType, size_t* size = nullptr) const;
  bool updateTotalMetrics(const std::string& fileName, const std::string& productType, const std::string& userName,
                          const bool completed, const size_t size) const;

  bool _useAlias;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(NotifyDownloadStatusService);
};

_ACS_END_NAMESPACE

#endif /* _NotifyDownloadStatusService_H_ */
