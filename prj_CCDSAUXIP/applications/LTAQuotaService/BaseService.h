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

#ifndef _BaseService_H_
#define _BaseService_H_

#include <RestServerBase.h>
#include <dbConnectionPool.h>
#include <dbSmartPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

class QuotaServiceResponsePayload;
class SQLString;
class dbSet;

class BaseService : public RestServerBase::RestService
{
public:
  explicit BaseService(const std::string& service, dbConnectionPool& pool);

  /** Defaulted special functions */
  ~BaseService() noexcept override = default;
  BaseService(const BaseService& service) = default;
  BaseService& operator=(const BaseService& service) = default;

  /** Deleted special functions */
  BaseService() = delete;

protected:
  /** Generic POST handler function. */
  void doPost(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) final;
  /** Generic DELETE handler function. */
  void doDelete(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) final;

  /** Handle a request. This function will be called when a Post or a Delete request is received. */
  virtual void handleRequest(const std::string& request, Pistache::Http::Code& responseCode,
                             QuotaServiceResponsePayload& responsePayload) = 0;

  inline dbConnectionPool& getPool() const noexcept
  {
    return _pool;
  };

  static void execQuery(dbConnection& conn, const SQLString& sql, const std::string& queryName, dbSet& set);
  static long long int getOperationQuota(dbConnection& conn, const dbPersistent::IdType userID,
                                         const double interval, const std::string& operation_type);
  static void createQuotaRegistry(dbConnection& conn, const dbPersistent::IdType userID, const size_t cost,
                                  const std::string& operationType, std::string& operationUUID,
                                  const std::string* referenceUUID = nullptr);

private:
  void internalHandleRequest(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter& response);

  dbConnectionPool& _pool;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(BaseService);
};

_ACS_END_NAMESPACE

#endif /* _BaseService_H_ */
