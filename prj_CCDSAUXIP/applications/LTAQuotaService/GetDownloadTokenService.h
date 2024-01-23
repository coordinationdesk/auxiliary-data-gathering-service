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

#ifndef _GetDownloadTokenService_H_
#define _GetDownloadTokenService_H_

#include <BaseService.h>
#include <dbConnectionPool.h>
#include <dbSmartPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

class GetDownloadTokenService : public BaseService
{
public:
  explicit GetDownloadTokenService(const std::string& service, dbConnectionPool& pool);

  /** Defaulted special functions */
  ~GetDownloadTokenService() noexcept override = default;
  GetDownloadTokenService(const GetDownloadTokenService& service) = default;
  GetDownloadTokenService& operator=(const GetDownloadTokenService& service) = default;

  /** Deleted special functions */
  GetDownloadTokenService() = delete;

protected:
  /** Handle a request. This function will be called when a Post or a Delete request is received. */
  void handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
                     QuotaServiceResponsePayload& responsePayload) override;

private:
  void clearOldToken() const;
  bool getUserQuota(const dbPersistent::IdType userID, int& parallel) const;
  long long int getParallelDownloads(const dbPersistent::IdType userID) const;
  bool createNewRows(const dbPersistent::IdType userID, std::string& operationUUID, std::string& tokenUUID) const;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(GetDownloadTokenService);
};

_ACS_END_NAMESPACE

#endif /* _GetDownloadTokenService_H_ */
