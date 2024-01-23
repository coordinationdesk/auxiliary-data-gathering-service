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

#ifndef _ReleaseDownloadTokenService_H_
#define _ReleaseDownloadTokenService_H_

#include <BaseService.h>
#include <dbConnectionPool.h>
#include <dbConnection.h>
#include <dbSmartPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

class ReleaseDownloadTokenService : public BaseService
{
public:
  explicit ReleaseDownloadTokenService(const std::string& service, dbConnectionPool& pool);

  /** Defaulted special functions */
  ~ReleaseDownloadTokenService() noexcept override = default;
  ReleaseDownloadTokenService(const ReleaseDownloadTokenService& service) = default;
  ReleaseDownloadTokenService& operator=(const ReleaseDownloadTokenService& service) = default;

  /** Deleted special functions */
  ReleaseDownloadTokenService() = delete;

protected:
  /** Handle a request. This function will be called when a Post or a Delete request is received. */
  void handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
                     QuotaServiceResponsePayload& responsePayload) override;

private:
  bool deleteToken(dbConnection& conn, const dbPersistent::IdType userID, const std::string& tokenUUID) const;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(ReleaseDownloadTokenService);
};

_ACS_END_NAMESPACE

#endif /* _ReleaseDownloadTokenService_H_ */
