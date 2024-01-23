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

#ifndef _LTAQuotaServiceConfigurer_H_
#define _LTAQuotaServiceConfigurer_H_

#include <rsResourceSet.h>

_ACS_BEGIN_NAMESPACE(acs)

class LTAQuotaServiceConfigurer
{
public:
  exDECLARE_EXCEPTION(LTAQuotaServiceConfigurerException, exException);

  enum class Services
  {
    CancelOrder,
    CheckBatchQuota,
    CheckOrderQuota,
    CheckDownloadQuotaVolume,
    CheckParallelOrderQuota,
    UpdateOrderRegistry,
    GetDownloadToken,
    UseDownloadToken,
    ReleaseDownloadToken,
    NotifyDownloadStatus
  };

private:
  static const std::string HOSTNAME_TAG;
  static const std::string PORT_TAG;
  static const std::string MAX_THREADS_TAG;
  static const std::string MAX_REQUEST_SIZE_TAG;
  static const std::string MAX_RESPONSE_SIZE_TAG;
  static const std::string TOKEN_VALIDITY_TIMEOUT_TAG;
  static const std::string TOKEN_EXPIRATION_TIMEOUT_TAG;
  static const std::string BASESERVICE_NAME_TAG;
  static const std::string RESTARTENABLED_TAG;
  static const std::string RESTARTTIME_TAG;
  static const std::map<Services, std::string> SERVICES_TAG;

public:
  /** CTORs/DTOR */
  explicit LTAQuotaServiceConfigurer(const rsResourceSet& rset);
  ~LTAQuotaServiceConfigurer() noexcept = default;
  LTAQuotaServiceConfigurer(const LTAQuotaServiceConfigurer&) = default;
  LTAQuotaServiceConfigurer& operator=(const LTAQuotaServiceConfigurer&) = default;

  /** Getters */
  std::string getHostname(bool* set = nullptr) const;
  unsigned short getPort(bool* set = nullptr) const;
  size_t getMaxThreads(bool* set = nullptr) const;
  size_t getMaxRequestSize(bool* set = nullptr) const;
  size_t getMaxResponseSize(bool* set = nullptr) const;
  unsigned long long int getTokenValidityTimeout(bool* set = nullptr) const;
  unsigned long long int getTokenExpirationTimeOut(bool* set = nullptr) const;
  std::string getBaseServiceName(bool* set = nullptr) const;
  bool getRestartEnabled() const;
  unsigned int getRestartTime() const;

  bool getServiceEnable(const Services service) const noexcept;
  std::string getServiceName(const Services service, const std::string& defaultVal) const noexcept;
  template<class T>
  T getServiceParam(const Services service, const std::string& param, bool* set = nullptr) const;

private:
  rsResourceSet rset_;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAQuotaServiceConfigurer);
};

template<class T>
T LTAQuotaServiceConfigurer::getServiceParam(const Services service, const std::string& param, bool* set) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  T val;
  std::string tag = SERVICES_TAG.at(service) + "." + param;
  bool found = rset_.hasKey(tag);
  if (found)
  {
    rset_.getValue<T>(tag, val);
  }

  if (nullptr != set)
  {
    *set = found;
  }
  else
  {
    ACS_COND_THROW(!found, LTAQuotaServiceConfigurerException("Tag " + tag + " not found on configuration."));
  }

  return val;
}

_ACS_END_NAMESPACE

#endif /* _LTAQuotaServiceConfigurer_H_ */
