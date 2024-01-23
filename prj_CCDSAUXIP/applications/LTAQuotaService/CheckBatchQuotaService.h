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

#ifndef _CheckBatchQuotaService_H_
#define _CheckBatchQuotaService_H_

#include <BaseService.h>
#include <dbConnectionPool.h>
#include <dbSmartPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

class CheckBatchQuotaService : public BaseService
{
public:
  explicit CheckBatchQuotaService(const std::string& service, dbConnectionPool& pool);

  /** Defaulted special functions */
  ~CheckBatchQuotaService() noexcept override = default;
  CheckBatchQuotaService(const CheckBatchQuotaService& service) = default;
  CheckBatchQuotaService& operator=(const CheckBatchQuotaService& service) = default;

  /** Deleted special functions */
  CheckBatchQuotaService() = delete;

protected:
  /** Handle a request. This function will be called when a Post or a Delete request is received. */
  void handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
                     QuotaServiceResponsePayload& responsePayload) override;

private:
  bool getUserQuota(const dbPersistent::IdType userID, long long int& size, double& interval, int& parallel) const;
  bool checkBatchUUID(const std::string& batchUUID) const;
  long long int getParallelBatchsOrders(const dbPersistent::IdType userID) const;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(CheckBatchQuotaService);
};

_ACS_END_NAMESPACE

#endif /* _CheckBatchQuotaService_H_ */
