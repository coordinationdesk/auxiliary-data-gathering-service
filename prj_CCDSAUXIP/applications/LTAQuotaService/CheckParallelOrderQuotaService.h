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

#ifndef _CheckParallelOrderQuotaService_H_
#define _CheckParallelOrderQuotaService_H_

#include <BaseService.h>
#include <dbConnectionPool.h>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

class CheckParallelOrderQuotaService : public BaseService
{
public:
  explicit CheckParallelOrderQuotaService(const std::string& service, dbConnectionPool& pool);

  /** Defaulted special functions */
  ~CheckParallelOrderQuotaService() noexcept override = default;
  CheckParallelOrderQuotaService(const CheckParallelOrderQuotaService& service) = default;
  CheckParallelOrderQuotaService& operator=(const CheckParallelOrderQuotaService& service) = default;

  /** Deleted special functions */
  CheckParallelOrderQuotaService() = delete;

protected:
  /** Handle a request. This function will be called when a Post or a Delete request is received. */
  void handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
                     QuotaServiceResponsePayload& responsePayload) override;

private:
  bool getUsersFromOrder(const std::string& orderUUID, std::vector<dbPersistent::IdType>& userIDList) const;
  bool checkUsersParallelQuota(const std::vector<dbPersistent::IdType>& userIDList) const;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(CheckParallelOrderQuotaService);
};

_ACS_END_NAMESPACE

#endif /* _CheckParallelOrderQuotaService_H_ */
