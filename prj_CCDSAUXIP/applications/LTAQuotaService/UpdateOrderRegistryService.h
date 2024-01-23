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

#ifndef _UpdateOrderRegistryService_H_
#define _UpdateOrderRegistryService_H_

#include <BaseService.h>
#include <dbConnectionPool.h>
#include <dbSmartPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

class UpdateOrderRegistryService : public BaseService
{
public:
  explicit UpdateOrderRegistryService(const std::string& service, dbConnectionPool& pool);

  /** Defaulted special functions */
  ~UpdateOrderRegistryService() noexcept override = default;
  UpdateOrderRegistryService(const UpdateOrderRegistryService& service) = default;
  UpdateOrderRegistryService& operator=(const UpdateOrderRegistryService& service) = default;

  /** Deleted special functions */
  UpdateOrderRegistryService() = delete;

protected:
  /** Handle a request. This function will be called when a Post or a Delete request is received. */
  void handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
                     QuotaServiceResponsePayload& responsePayload) override;

private:
  bool checkOrderUUID(const std::string& orderUUID) const;
  bool getRow(const std::string& operationUUID, dbPersistent::IdType& opQuotaRegID) const;
  void updateRow(const dbPersistent::IdType opQuotaRegID, const std::string& referenceUUID) const;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(UpdateOrderRegistryService);
};

_ACS_END_NAMESPACE

#endif /* _UpdateOrderRegistryService_H_ */
