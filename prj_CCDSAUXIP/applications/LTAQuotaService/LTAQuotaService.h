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

#ifndef _LTAQuotaService_H_
#define _LTAQuotaService_H_

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#include <ProjectAppIncludes>

#include <acs_c++config.hpp>

#include <ApplicationLock.h>
#include <Transaction.h>
#include <RestServerBase.h>
#include <dbConnection.h>
#include <exMacros.h>
#include <dbAppResSet.h>
#include <SMPluginLoader.h>

#include <LTAQuotaServiceConfigurer.h>
#include <CancelOrderService.h>
#include <CheckBatchQuotaService.h>
#include <CheckOrderQuotaService.h>
#include <CheckDownloadQuotaVolumeService.h>
#include <CheckParallelOrderQuotaService.h>
#include <UpdateOrderRegistryService.h>
#include <GetDownloadTokenService.h>
#include <UseDownloadTokenService.h>
#include <ReleaseDownloadTokenService.h>
#include <NotifyDownloadStatusService.h>
#include <ltaqsV.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbConnection;

class LTAQuotaService : public PROJECT_APP
{
public:
  static unsigned long long int getTokenValidityTimeout();
  static unsigned long long int getTokenExpirationTimeOut();

private:
  static unsigned long long int _tokenValidityTimeout;
  static unsigned long long int _tokenExpirationTimeOut;

public:
  /**
   * Default constructor
   */
  LTAQuotaService(const std::string& appname, const std::string& appsubsys);

  /**
  * Destructor
  */
  ~LTAQuotaService() noexcept override = default;

  /**
  * The main function
  */
  int main(int argc, char const* const* argv, char const* const* env) override;

  std::string getDefaultLogName() const override;

private:
  // Defined but not implemented
  LTAQuotaService() = delete;
  LTAQuotaService(const LTAQuotaService&) = delete;
  LTAQuotaService& operator=(const LTAQuotaService&) = delete;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAQuotaService);
};

_ACS_END_NAMESPACE

#endif /* _LTAQuotaService_H_ */
