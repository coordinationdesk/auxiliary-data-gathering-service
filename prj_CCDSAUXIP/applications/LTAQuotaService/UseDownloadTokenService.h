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

#ifndef _UseDownloadTokenService_H_
#define _UseDownloadTokenService_H_

#include <BaseService.h>
#include <dbConnectionPool.h>
#include <dbSmartPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

class UseDownloadTokenService : public BaseService
{
public:
  exDECLARE_EXCEPTION(UseDownloadTokenServiceException, exException);

  explicit UseDownloadTokenService(const std::string& service, dbConnectionPool& pool);

  /** Defaulted special functions */
  ~UseDownloadTokenService() noexcept override = default;
  UseDownloadTokenService(const UseDownloadTokenService& service) = default;
  UseDownloadTokenService& operator=(const UseDownloadTokenService& service) = default;

  /** Deleted special functions */
  UseDownloadTokenService() = delete;

protected:
  /** Handle a request. This function will be called when a Post or a Delete request is received. */
  void handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
                     QuotaServiceResponsePayload& responsePayload) override;

private:
  bool useToken(dbConnection& conn, const dbPersistent::IdType userID, const std::string& tokenUUID) const;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(UseDownloadTokenService);
};

_ACS_END_NAMESPACE

#endif /* _UseDownloadTokenService_H_ */
