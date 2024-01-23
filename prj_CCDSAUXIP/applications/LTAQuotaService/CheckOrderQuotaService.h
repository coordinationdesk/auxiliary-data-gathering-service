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

#ifndef _CheckOrderQuotaService_H_
#define _CheckOrderQuotaService_H_

#include <BaseService.h>
#include <dbConnectionPool.h>
#include <dbSmartPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

class CheckOrderQuotaService : public BaseService
{
public:
  explicit CheckOrderQuotaService(const std::string& service, dbConnectionPool& pool);

  /** Defaulted special functions */
  ~CheckOrderQuotaService() noexcept override = default;
  CheckOrderQuotaService(const CheckOrderQuotaService& service) = default;
  CheckOrderQuotaService& operator=(const CheckOrderQuotaService& service) = default;

  /** Deleted special functions */
  CheckOrderQuotaService() = delete;

protected:
  /** Handle a request. This function will be called when a Post or a Delete request is received. */
  void handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
                     QuotaServiceResponsePayload& responsePayload) override;

private:
  bool getUserQuota(const dbPersistent::IdType userID, long long int& size, double& interval) const;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(CheckOrderQuotaService);
};

_ACS_END_NAMESPACE

#endif /* _CheckOrderQuotaService_H_ */
