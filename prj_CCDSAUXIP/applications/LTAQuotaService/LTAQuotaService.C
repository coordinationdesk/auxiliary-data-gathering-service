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

#include <LTAQuotaService.h>

_ACS_BEGIN_NAMESPACE(acs)

namespace
{
const ltaqsV vers;  // NOSONAR - Force version's symbols linking
}

std::unique_ptr<RestServerBase> createServer(const LTAQuotaServiceConfigurer& configurer)
{
  auto server = std::make_unique<RestServerBase>(configurer.getHostname(), configurer.getPort());

  bool set = false;
  size_t val = configurer.getMaxThreads(&set);
  if (set && val != 1UL)
  {
    ACS_LOG_WARNING("It is not possible to use more than one thread in the current implementation.")
  }
  server->setMaxThreads(1UL);

  val = configurer.getMaxRequestSize(&set);
  if (set)
  {
    server->setMaxRequestSize(val);
  }

  val = configurer.getMaxResponseSize(&set);
  if (set)
  {
    server->setMaxResponseSize(val);
  }

  std::string baseName = configurer.getBaseServiceName(&set);
  if (!set)
  {
    baseName = "ltaquotaservice/";
  }

  dbConnectionPool* pool = Application::instance()->getDefaultConnectionPool();

  if (configurer.getServiceEnable(LTAQuotaServiceConfigurer::Services::CancelOrder))
  {
    const std::string name = configurer.getServiceName(LTAQuotaServiceConfigurer::Services::CancelOrder, "cancelorder");
    server->registerService(std::make_shared<CancelOrderService>(baseName + name, *pool));
  }

  if (configurer.getServiceEnable(LTAQuotaServiceConfigurer::Services::CheckBatchQuota))
  {
    const std::string name = configurer.getServiceName(LTAQuotaServiceConfigurer::Services::CheckBatchQuota,
                                                       "checkbatchquota");
    server->registerService(std::make_shared<CheckBatchQuotaService>(baseName + name, *pool));
  }

  if (configurer.getServiceEnable(LTAQuotaServiceConfigurer::Services::CheckOrderQuota))
  {
    const std::string name = configurer.getServiceName(LTAQuotaServiceConfigurer::Services::CheckOrderQuota,
                                                       "checkorderquota");
    server->registerService(std::make_shared<CheckOrderQuotaService>(baseName + name, *pool));
  }

  if (configurer.getServiceEnable(LTAQuotaServiceConfigurer::Services::CheckDownloadQuotaVolume))
  {
    const std::string name = configurer.getServiceName(LTAQuotaServiceConfigurer::Services::CheckDownloadQuotaVolume,
                                                       "checkdownloadquotavolume");
    server->registerService(std::make_shared<CheckDownloadQuotaVolumeService>(baseName + name, *pool));
  }

  if (configurer.getServiceEnable(LTAQuotaServiceConfigurer::Services::CheckParallelOrderQuota))
  {
    const std::string name = configurer.getServiceName(LTAQuotaServiceConfigurer::Services::CheckParallelOrderQuota,
                                                       "checkparallelorderquota");
    server->registerService(std::make_shared<CheckParallelOrderQuotaService>(baseName + name, *pool));
  }

  if (configurer.getServiceEnable(LTAQuotaServiceConfigurer::Services::UpdateOrderRegistry))
  {
    const std::string name = configurer.getServiceName(LTAQuotaServiceConfigurer::Services::UpdateOrderRegistry,
                                                       "updateorderregistry");
    server->registerService(std::make_shared<UpdateOrderRegistryService>(baseName + name, *pool));
  }

  if (configurer.getServiceEnable(LTAQuotaServiceConfigurer::Services::GetDownloadToken))
  {
    const std::string name = configurer.getServiceName(LTAQuotaServiceConfigurer::Services::GetDownloadToken,
                                                       "getdownloadtoken");

    server->registerService(std::make_shared<GetDownloadTokenService>(baseName + name, *pool));
  }

  if (configurer.getServiceEnable(LTAQuotaServiceConfigurer::Services::UseDownloadToken))
  {
    const std::string name = configurer.getServiceName(LTAQuotaServiceConfigurer::Services::UseDownloadToken,
                                                       "usedownloadtoken");
    server->registerService(std::make_shared<UseDownloadTokenService>(baseName + name, *pool));
  }

  if (configurer.getServiceEnable(LTAQuotaServiceConfigurer::Services::ReleaseDownloadToken))
  {
    const std::string name = configurer.getServiceName(LTAQuotaServiceConfigurer::Services::ReleaseDownloadToken,
                                                       "releasedownloadtoken");
    server->registerService(std::make_shared<ReleaseDownloadTokenService>(baseName + name, *pool));
  }

  if (configurer.getServiceEnable(LTAQuotaServiceConfigurer::Services::NotifyDownloadStatus))
  {
    const std::string name = configurer.getServiceName(LTAQuotaServiceConfigurer::Services::NotifyDownloadStatus,
                                                       "notifydownloadstatus");
    server->registerService(std::make_shared<NotifyDownloadStatusService>(baseName + name, *pool));
  }

  server->dump();

  return server;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(LTAQuotaService);

const PROJECT_APP::GenericBuilder<LTAQuotaService> theApplicationBuilder("LTAQuotaService", "LTA");

unsigned long long int LTAQuotaService::_tokenValidityTimeout = 5ULL;
unsigned long long int LTAQuotaService::_tokenExpirationTimeOut = 86400ULL;
unsigned long long int LTAQuotaService::getTokenValidityTimeout()
{
  return _tokenValidityTimeout;
}
unsigned long long int LTAQuotaService::getTokenExpirationTimeOut()
{
  return _tokenExpirationTimeOut;
}

/*
 * Default constructor
 */
LTAQuotaService::LTAQuotaService(const std::string& appname, const std::string& appsubsys) :
  PROJECT_APP(appname, appsubsys)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

std::string LTAQuotaService::getDefaultLogName() const
{
  return defaultLogPath + "/" + File::getFileName(Application::executableName()) + ".log";
}

/*
 * This is the main function
 */
int LTAQuotaService::main(int argc, char const* const* argv, char const* const* env)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  int retValue = 0;

  // Lock the application.
  ApplicationLock appLock;
  appLock.lock(true);

  daemonize();

  bool confLoaded = false;
  short retry = 0;
  while (!confLoaded && retry < 3)
  {
    try
    {
      addSpace(ConfigurationSingleton::ConfSpace("LTAQuotaService", "", false));
      loadConfiguration();
      confLoaded = true;
      break;
    }
    catch (const dbConnection::ConnectionException& e)
    {
      ACS_LOG_NOTIFY_EX(e);
      ++retry;

      waitStopOrTimeout(10U);

      if (getCurrentStopMode() == ImmediateStop)
      {
        Application::setStatus(Application::Critical);
        return 255;
      }
      else if (getCurrentStopMode() == CleanStop)
      {
        Application::setStatus(Application::Exiting);
        return 0;
      }

      if (retry <= 3)
      {
        ACS_LOG_INFO("Retry to load configuration in 10 seconds... Attempt " << retry << "/3");
      }
    }
  }

  if (!confLoaded)
  {
    ACS_LOG_ERROR("Error loading configuration. Exiting");
    Application::setStatus(Application::Critical);
    return 255;
  }

  rotateLog();

  LTAQuotaServiceConfigurer configurer(ConfigurationSingleton::instance()->get());

  // Setup Phoenix restart
  if (configurer.getRestartEnabled())
  {
    wantToRestart(configurer.getRestartTime());
    ACS_LOG_INFO("Phoenix restart every " << configurer.getRestartTime() << " seconds")
  }

  bool set;
  _tokenValidityTimeout = configurer.getTokenValidityTimeout(&set);
  if (!set)
  {
    _tokenValidityTimeout = 5ULL;  // seconds
  }

  _tokenExpirationTimeOut = configurer.getTokenExpirationTimeOut(&set);
  if (!set)
  {
    _tokenExpirationTimeOut = 86400ULL;  // seconds = 1 day
  }

  std::unique_ptr<RestServerBase> server = createServer(configurer);

  server->startServer();

  try
  {
    while (getCurrentStopMode() == NoStop)
    {
      // Check if server is down
      if (!server->isServerOnline())
      {
        Application::setStatus(Application::Critical);
        ACS_LOG_ERROR("Server is unexpectedly offline...");
        break;
      }
      rotateLog();
      waitStopOrTimeout(5000);
    }
  }
  catch (const dbConnection::ConnectionException& e)
  {
    ACS_LOG_NOTIFY_EX(e);
    ACS_LOG_ERROR("DB error");
    Application::setStatus(Application::Exiting);
  }
  catch (const std::exception& e)  // NOSONAR - Want to catch all
  {
    ACS_LOG_NOTIFY_EX(e);
    ACS_LOG_ERROR("Urecoverable error");
    Application::setStatus(Application::Critical);
  }

  // Immediate stop
  if (getCurrentStopMode() == ImmediateStop)
  {
    ACS_LOG_INFO("SIGTERM received");
    retValue = 255;
  }
  // Clean stop
  else if (getCurrentStopMode() == CleanStop)
  {
    ACS_LOG_INFO("SIGINT received");
  }
  // Phoenix restart
  else if (getCurrentStopMode() == PhoenixRestart)
  {
    ACS_LOG_INFO("Phoenix detected. Restarting...");
  }

  try
  {
    // Stop the server
    if (server->isServerOnline())
    {
      server->stopServer();
    }
  }
  catch (const dbConnection::ConnectionException& e)
  {
    ACS_LOG_NOTIFY_EX(e);
    ACS_LOG_ERROR("DB error");
    Application::setStatus(Application::Exiting);
  }
  catch (const std::exception& e)  // NOSONAR - Want to catch all
  {
    ACS_LOG_NOTIFY_EX(e);
    ACS_LOG_ERROR("Unrecoverable error");
    Application::setStatus(Application::Critical);
  }

  // Phoenix restart
  if (getCurrentStopMode() == PhoenixRestart)
  {
    ACS_LOG_INFO("Phoenix now.");
  }

  if (Application::getCurrentStatus() == Application::Critical)
  {
    retValue = 255;
  }

  ACS_LOG_PRIVINFO("Exiting with exit value " << retValue);
  return retValue;
}

_ACS_END_NAMESPACE
