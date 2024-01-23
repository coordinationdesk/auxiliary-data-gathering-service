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

#include <LTAQuotaServiceConfigurer.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(LTAQuotaServiceConfigurer);

const std::string LTAQuotaServiceConfigurer::HOSTNAME_TAG = "LTAQuotaService.HostName";
const std::string LTAQuotaServiceConfigurer::PORT_TAG = "LTAQuotaService.Port";
const std::string LTAQuotaServiceConfigurer::MAX_THREADS_TAG = "LTAQuotaService.MaxServerThreads";
const std::string LTAQuotaServiceConfigurer::MAX_REQUEST_SIZE_TAG = "LTAQuotaService.MaxRequestSize";
const std::string LTAQuotaServiceConfigurer::MAX_RESPONSE_SIZE_TAG = "LTAQuotaService.MaxResponseSize";
const std::string LTAQuotaServiceConfigurer::TOKEN_VALIDITY_TIMEOUT_TAG = "LTAQuotaService.TokenValidityTimeout";
const std::string LTAQuotaServiceConfigurer::TOKEN_EXPIRATION_TIMEOUT_TAG = "LTAQuotaService.TokenExpirationTimeOut";
const std::string LTAQuotaServiceConfigurer::BASESERVICE_NAME_TAG = "LTAQuotaService.BaseServiceName";
const std::string LTAQuotaServiceConfigurer::RESTARTENABLED_TAG = "LTAQuotaService.RestartEnabled";
const std::string LTAQuotaServiceConfigurer::RESTARTTIME_TAG = "LTAQuotaService.RestartTime";

const std::map<LTAQuotaServiceConfigurer::Services, std::string> LTAQuotaServiceConfigurer::SERVICES_TAG = {
  { Services::CancelOrder, "LTAQuotaService.CancelOrderService" },
  { Services::CheckBatchQuota, "LTAQuotaService.CheckBatchQuotaService" },
  { Services::CheckOrderQuota, "LTAQuotaService.CheckOrderQuotaService" },
  { Services::CheckDownloadQuotaVolume, "LTAQuotaService.CheckDownloadQuotaVolumeService" },
  { Services::CheckParallelOrderQuota, "LTAQuotaService.CheckParallelOrderQuotaService" },
  { Services::UpdateOrderRegistry, "LTAQuotaService.UpdateOrderRegistryService" },
  { Services::GetDownloadToken, "LTAQuotaService.GetDownloadTokenService" },
  { Services::UseDownloadToken, "LTAQuotaService.UseDownloadTokenService" },
  { Services::ReleaseDownloadToken, "LTAQuotaService.ReleaseDownloadTokenService" },
  { Services::NotifyDownloadStatus, "LTAQuotaService.NotifyDownloadStatusService" }
};

/* Special functions */
LTAQuotaServiceConfigurer::LTAQuotaServiceConfigurer(const rsResourceSet& rset) :
  rset_(rset)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

/* Getters */
std::string LTAQuotaServiceConfigurer::getHostname(bool* set) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  std::string val;
  bool found = rset_.hasKey(HOSTNAME_TAG);
  if (found)
  {
    rset_.getValue<std::string>(HOSTNAME_TAG, val);
  }

  if (nullptr != set)
  {
    *set = found;
  }
  else
  {
    ACS_COND_THROW(!found, LTAQuotaServiceConfigurerException() << "Tag " << HOSTNAME_TAG
                                                                << " not found on configuration.");
  }

  return val;
}

unsigned short LTAQuotaServiceConfigurer::getPort(bool* set) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  unsigned short val = 0U;
  bool found = rset_.hasKey(PORT_TAG);
  if (found)
  {
    rset_.getValue<unsigned short>(PORT_TAG, val);
  }

  if (nullptr != set)
  {
    *set = found;
  }
  else
  {
    ACS_COND_THROW(!found, LTAQuotaServiceConfigurerException() << "Tag " << PORT_TAG
                                                                << " not found on configuration.");
  }

  return val;
}

size_t LTAQuotaServiceConfigurer::getMaxThreads(bool* set) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  size_t val = 0UL;
  bool found = rset_.hasKey(MAX_THREADS_TAG);
  if (found)
  {
    rset_.getValue<size_t>(MAX_THREADS_TAG, val);
  }

  if (nullptr != set)
  {
    *set = found;
  }
  else
  {
    ACS_COND_THROW(!found, LTAQuotaServiceConfigurerException() << "Tag " << MAX_THREADS_TAG
                                                                << " not found on configuration.");
  }

  return val;
}

size_t LTAQuotaServiceConfigurer::getMaxRequestSize(bool* set) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  size_t val = 0UL;
  bool found = rset_.hasKey(MAX_REQUEST_SIZE_TAG);
  if (found)
  {
    rset_.getValue<size_t>(MAX_REQUEST_SIZE_TAG, val);
  }

  if (nullptr != set)
  {
    *set = found;
  }
  else
  {
    ACS_COND_THROW(!found, LTAQuotaServiceConfigurerException() << "Tag " << MAX_REQUEST_SIZE_TAG
                                                                << " not found on configuration.");
  }

  return val;
}

size_t LTAQuotaServiceConfigurer::getMaxResponseSize(bool* set) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  size_t val = 0UL;
  bool found = rset_.hasKey(MAX_RESPONSE_SIZE_TAG);
  if (found)
  {
    rset_.getValue<size_t>(MAX_RESPONSE_SIZE_TAG, val);
  }

  if (nullptr != set)
  {
    *set = found;
  }
  else
  {
    ACS_COND_THROW(!found, LTAQuotaServiceConfigurerException() << "Tag " << MAX_RESPONSE_SIZE_TAG
                                                                << " not found on configuration.");
  }

  return val;
}

unsigned long long int LTAQuotaServiceConfigurer::getTokenValidityTimeout(bool* set) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  unsigned long long int val = 0ULL;
  bool found = rset_.hasKey(TOKEN_VALIDITY_TIMEOUT_TAG);
  if (found)
  {
    rset_.getValue<unsigned long long int>(TOKEN_VALIDITY_TIMEOUT_TAG, val);
  }

  if (nullptr != set)
  {
    *set = found;
  }
  else
  {
    ACS_COND_THROW(!found, LTAQuotaServiceConfigurerException() << "Tag " << TOKEN_VALIDITY_TIMEOUT_TAG
                                                                << " not found on configuration.");
  }

  return val;
}

unsigned long long int LTAQuotaServiceConfigurer::getTokenExpirationTimeOut(bool* set) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  unsigned long long int val = 0ULL;
  bool found = rset_.hasKey(TOKEN_EXPIRATION_TIMEOUT_TAG);
  if (found)
  {
    rset_.getValue<unsigned long long int>(TOKEN_EXPIRATION_TIMEOUT_TAG, val);
  }

  if (nullptr != set)
  {
    *set = found;
  }
  else
  {
    ACS_COND_THROW(!found, LTAQuotaServiceConfigurerException() << "Tag " << TOKEN_EXPIRATION_TIMEOUT_TAG
                                                                << " not found on configuration.");
  }

  return val;
}

std::string LTAQuotaServiceConfigurer::getBaseServiceName(bool* set) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  std::string val;
  bool found = rset_.hasKey(BASESERVICE_NAME_TAG);
  if (found)
  {
    rset_.getValue<std::string>(BASESERVICE_NAME_TAG, val);
  }

  if (nullptr != set)
  {
    *set = found;
  }
  else
  {
    ACS_COND_THROW(!found, LTAQuotaServiceConfigurerException() << "Tag " << BASESERVICE_NAME_TAG
                                                                << " not found on configuration.");
  }

  return val;
}

bool LTAQuotaServiceConfigurer::getRestartEnabled() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  bool val;
  if (rset_.hasKey(RESTARTENABLED_TAG))
  {
    rset_.getValue<bool>(RESTARTENABLED_TAG, val);
  }
  else
  {
    val = true;
  }

  return val;
}

unsigned int LTAQuotaServiceConfigurer::getRestartTime() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  unsigned int val;
  if (rset_.hasKey(RESTARTTIME_TAG))
  {
    rset_.getValue<unsigned int>(RESTARTTIME_TAG, val);
  }
  else
  {
    val = 86400U;
  }

  return val;
}

bool LTAQuotaServiceConfigurer::getServiceEnable(const Services service) const noexcept
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  bool val = true;
  const std::string tag = SERVICES_TAG.at(service) + ".Enable";
  if (rset_.hasKey(tag))
  {
    rset_.getValue<bool>(tag, val);
  }

  return val;
}

std::string LTAQuotaServiceConfigurer::getServiceName(const Services service,
                                                      const std::string& defaultVal) const noexcept
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  const std::string tag = SERVICES_TAG.at(service) + ".Name";
  if (rset_.hasKey(tag))
  {
    std::string val;
    rset_.getValue<std::string>(tag, val);
    return val;
  }
  else
  {
    return defaultVal;
  }
}

_ACS_END_NAMESPACE
