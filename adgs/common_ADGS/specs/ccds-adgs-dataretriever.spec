#########################################################################
%define		project			adgs
%define		installname		%{project}-DataRetriever
%define		installpath		/usr/local/components/ADGS
%define		daemonuser		opadgs
%define		daemongroup		ccds
%define		configclass		ConfigADGS
%define		downloadworkers 12
#########################################################################

%define rpmPkgName      ccds-adgs-dataretriever

Name: ccds-adgs-dataretriever
Version: 1.3
Release: 0
Summary: DataRetriever for satellite products remote discovery and scalable download

License: Commercial. Copyright 2020-2021 Exprivia s.p.a.
Group: Application/Networking
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
BuildArchitectures: noarch

Requires: /usr/bin/php,php-pdo,php-pgsql,php-pecl-gearman,gearmand,supervisor >= 3.4.0

%description

%install

install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/bin"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/config"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/workers"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Plugin/Data"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Plugin"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Tools"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Exception"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Gearman"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Curl"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Curl/HttpAuthentication"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Curl/Protocols"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/DB"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Log"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/Slack"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/SyslogUdp"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FingersCrossed"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/Curl"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/psr"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/psr/log"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/psr/log/Psr"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/psr/log/Psr/Log"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/psr/log/Psr/Log/Test"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/acs"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/acs/acs-php-tools"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes/Exception"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/composer"
install -d "$RPM_BUILD_ROOT/%{installpath}/%{installname}/"
install -m644 "%{_topdir}/../applications/DataRetriever/bin/data-retriever.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/bin/data-retriever.php"
install -m644 "%{_topdir}/../applications/DataRetriever/bin/data-retriever-configurator.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/bin/data-retriever-configurator.php"
install -m644 "%{_topdir}/../applications/DataRetriever/README.md" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/README.md"
install -m644 "%{_topdir}/../applications/DataRetriever/config/Config.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/config/Config.php"
install -m644 "%{_topdir}/../ADGSConf/confDataRetriever/ConfigADGS.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/config/ConfigADGS.php"
install -m644 "%{_topdir}/../applications/DataRetriever/workers/data-retriever-download-worker.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/workers/data-retriever-download-worker.php"
install -m644 "%{_topdir}/../applications/DataRetriever/workers/data-retriever-discovery-worker.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/workers/data-retriever-discovery-worker.php"
install -m644 "%{_topdir}/../applications/DataRetriever/composer.json" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/composer.json"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/PHPImportMainProcess.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/PHPImportMainProcess.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Plugin/Data/Product.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Plugin/Data/Product.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Plugin/DataRetrieverPluginOData.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Plugin/DataRetrieverPluginOData.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Plugin/DataRetrieverPluginMpip.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Plugin/DataRetrieverPluginMpip.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Plugin/DataRetrieverPluginInterface.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Plugin/DataRetrieverPluginInterface.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Plugin/DataRetrieverPlugin.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Plugin/DataRetrieverPlugin.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Tools/PluginTools.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Tools/PluginTools.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Exception/PhpImportCannotDownloadException.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Exception/PhpImportCannotDownloadException.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Exception/PhpImportFileNotFoundException.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Exception/PhpImportFileNotFoundException.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Exception/PhpImportException.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Exception/PhpImportException.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Exception/PhpImportCurlErrorException.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Exception/PhpImportCurlErrorException.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Exception/PhpImportCannotFilterException.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Exception/PhpImportCannotFilterException.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Exception/PhpImportTimeoutException.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Exception/PhpImportTimeoutException.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Exception/PhpImportBadRequestException.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Exception/PhpImportBadRequestException.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Exception/PhpImportCannotDiscoveryProductsException.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Exception/PhpImportCannotDiscoveryProductsException.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Gearman/TaskPayload.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Gearman/TaskPayload.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Gearman/PHPImportGearmanWorker.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Gearman/PHPImportGearmanWorker.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Gearman/PHPImportGearmanClient.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Gearman/PHPImportGearmanClient.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Curl/Protocols/OData.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Curl/Protocols/OData.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Curl/Protocols/ODataFactory.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Curl/Protocols/ODataFactory.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Curl/Protocols/Mpip.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Curl/Protocols/Mpip.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Curl/Protocols/MpipFactory.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Curl/Protocols/MpipFactory.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Curl/Protocols/ProtocolInterface.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Curl/Protocols/ProtocolInterface.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Curl/CurlExt.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Curl/CurlExt.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Curl/CurlDataRetriever.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Curl/CurlDataRetriever.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Curl/HttpAuthentication/HttpAuthenticationBasic.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Curl/HttpAuthentication/HttpAuthenticationBasic.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Curl/HttpAuthentication/HttpAuthenticationFactory.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Curl/HttpAuthentication/HttpAuthenticationFactory.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Curl/HttpAuthentication/HttpAuthenticationInterface.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Curl/HttpAuthentication/HttpAuthenticationInterface.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Curl/HttpAuthentication/HttpAuthenticationNone.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Curl/HttpAuthentication/HttpAuthenticationNone.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Curl/HttpAuthentication/HttpAuthenticationOAuth2.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Curl/HttpAuthentication/HttpAuthenticationOAuth2.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/DB/PHPImportDB.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/DB/PHPImportDB.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Log/PHPImportLogger.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Log/PHPImportLogger.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Log/PHPImportLoggerStdout.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Log/PHPImportLoggerStdout.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Log/PHPImportLoggerSyslog.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Log/PHPImportLoggerSyslog.php"
install -m644 "%{_topdir}/../applications/DataRetriever/classes/Service/Log/PHPImportCustomLogger.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/classes/Service/Log/PHPImportCustomLogger.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Logger.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Logger.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Formatter/FlowdockFormatter.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/FlowdockFormatter.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Formatter/FluentdFormatter.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/FluentdFormatter.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Formatter/GelfMessageFormatter.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/GelfMessageFormatter.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Formatter/WildfireFormatter.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/WildfireFormatter.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Formatter/NormalizerFormatter.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/NormalizerFormatter.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Formatter/FormatterInterface.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/FormatterInterface.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Formatter/JsonFormatter.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/JsonFormatter.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Formatter/HtmlFormatter.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/HtmlFormatter.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Formatter/LogstashFormatter.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/LogstashFormatter.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Formatter/ChromePHPFormatter.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/ChromePHPFormatter.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Formatter/LogglyFormatter.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/LogglyFormatter.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Formatter/ElasticaFormatter.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/ElasticaFormatter.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Formatter/ScalarFormatter.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/ScalarFormatter.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Formatter/LineFormatter.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/LineFormatter.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Formatter/MongoDBFormatter.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/MongoDBFormatter.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Registry.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Registry.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/ResettableInterface.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/ResettableInterface.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Utils.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Utils.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/ErrorHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/ErrorHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/SignalHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/SignalHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Processor/MercurialProcessor.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/MercurialProcessor.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Processor/IntrospectionProcessor.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/IntrospectionProcessor.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Processor/ProcessorInterface.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/ProcessorInterface.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Processor/GitProcessor.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/GitProcessor.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Processor/MemoryProcessor.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/MemoryProcessor.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Processor/PsrLogMessageProcessor.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/PsrLogMessageProcessor.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Processor/UidProcessor.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/UidProcessor.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Processor/MemoryPeakUsageProcessor.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/MemoryPeakUsageProcessor.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Processor/TagProcessor.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/TagProcessor.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Processor/ProcessIdProcessor.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/ProcessIdProcessor.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Processor/WebProcessor.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/WebProcessor.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Processor/MemoryUsageProcessor.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/MemoryUsageProcessor.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/IFTTTHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/IFTTTHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/SocketHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/SocketHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/FingersCrossedHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FingersCrossedHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/RavenHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/RavenHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/AmqpHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/AmqpHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/ZendMonitorHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/ZendMonitorHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/FormattableHandlerTrait.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FormattableHandlerTrait.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/NewRelicHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/NewRelicHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/SamplingHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/SamplingHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/HandlerInterface.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/HandlerInterface.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/Slack/SlackRecord.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/Slack/SlackRecord.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/ElasticSearchHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/ElasticSearchHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/ProcessableHandlerTrait.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/ProcessableHandlerTrait.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/FlowdockHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FlowdockHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/DeduplicationHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/DeduplicationHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/NativeMailerHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/NativeMailerHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/MailHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/MailHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/PHPConsoleHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/PHPConsoleHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/HandlerWrapper.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/HandlerWrapper.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/SyslogUdpHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/SyslogUdpHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/RotatingFileHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/RotatingFileHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/CouchDBHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/CouchDBHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/SyslogHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/SyslogHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/ChromePHPHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/ChromePHPHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/LogEntriesHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/LogEntriesHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/BrowserConsoleHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/BrowserConsoleHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/RedisHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/RedisHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/SyslogUdp/UdpSocket.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/SyslogUdp/UdpSocket.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/PushoverHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/PushoverHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/InsightOpsHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/InsightOpsHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/FingersCrossed/ErrorLevelActivationStrategy.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FingersCrossed/ErrorLevelActivationStrategy.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/FingersCrossed/ActivationStrategyInterface.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FingersCrossed/ActivationStrategyInterface.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/FingersCrossed/ChannelLevelActivationStrategy.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FingersCrossed/ChannelLevelActivationStrategy.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/ErrorLogHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/ErrorLogHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/ProcessableHandlerInterface.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/ProcessableHandlerInterface.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/FleepHookHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FleepHookHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/DynamoDbHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/DynamoDbHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/SwiftMailerHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/SwiftMailerHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/RollbarHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/RollbarHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/MongoDBHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/MongoDBHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/FilterHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FilterHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/WhatFailureGroupHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/WhatFailureGroupHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/MissingExtensionException.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/MissingExtensionException.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/FirePHPHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FirePHPHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/GelfHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/GelfHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/MandrillHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/MandrillHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/TestHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/TestHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/AbstractSyslogHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/AbstractSyslogHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/DoctrineCouchDBHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/DoctrineCouchDBHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/FormattableHandlerInterface.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FormattableHandlerInterface.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/StreamHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/StreamHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/AbstractHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/AbstractHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/AbstractProcessingHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/AbstractProcessingHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/SlackWebhookHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/SlackWebhookHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/CubeHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/CubeHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/GroupHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/GroupHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/Curl/Util.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/Curl/Util.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/BufferHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/BufferHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/LogglyHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/LogglyHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/NullHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/NullHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/SlackbotHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/SlackbotHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/SlackHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/SlackHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/HipChatHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/HipChatHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/src/Monolog/Handler/PsrHandler.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/PsrHandler.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/README.md" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/README.md"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/CHANGELOG.md" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/CHANGELOG.md"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/composer.json" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/composer.json"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/phpstan.neon.dist" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/phpstan.neon.dist"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/monolog/monolog/LICENSE" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/monolog/monolog/LICENSE"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/psr/log/README.md" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/psr/log/README.md"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/psr/log/composer.json" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/psr/log/composer.json"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/psr/log/Psr/Log/LoggerAwareInterface.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/psr/log/Psr/Log/LoggerAwareInterface.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/psr/log/Psr/Log/InvalidArgumentException.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/psr/log/Psr/Log/InvalidArgumentException.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/psr/log/Psr/Log/LogLevel.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/psr/log/Psr/Log/LogLevel.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/psr/log/Psr/Log/LoggerInterface.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/psr/log/Psr/Log/LoggerInterface.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/psr/log/Psr/Log/NullLogger.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/psr/log/Psr/Log/NullLogger.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/psr/log/Psr/Log/Test/DummyTest.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/psr/log/Psr/Log/Test/DummyTest.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/psr/log/Psr/Log/Test/LoggerInterfaceTest.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/psr/log/Psr/Log/Test/LoggerInterfaceTest.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/psr/log/Psr/Log/Test/TestLogger.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/psr/log/Psr/Log/Test/TestLogger.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/psr/log/Psr/Log/LoggerAwareTrait.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/psr/log/Psr/Log/LoggerAwareTrait.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/psr/log/Psr/Log/LoggerTrait.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/psr/log/Psr/Log/LoggerTrait.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/psr/log/Psr/Log/AbstractLogger.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/psr/log/Psr/Log/AbstractLogger.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/psr/log/LICENSE" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/psr/log/LICENSE"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/acs/acs-php-tools/README.md" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/acs/acs-php-tools/README.md"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/acs/acs-php-tools/composer.json" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/acs/acs-php-tools/composer.json"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/acs/acs-php-tools/classes/Curl.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes/Curl.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/acs/acs-php-tools/classes/CmdExec.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes/CmdExec.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/acs/acs-php-tools/classes/File.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes/File.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/acs/acs-php-tools/classes/Exception/CannotRemoveFileException.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes/Exception/CannotRemoveFileException.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/acs/acs-php-tools/classes/Exception/CannotRemoveDirectoryException.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes/Exception/CannotRemoveDirectoryException.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/acs/acs-php-tools/classes/Exception/FileException.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes/Exception/FileException.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/acs/acs-php-tools/classes/Exception/CannotCreateDirectoryException.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes/Exception/CannotCreateDirectoryException.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/acs/acs-php-tools/classes/Exception/CommandExecException.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes/Exception/CommandExecException.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/acs/acs-php-tools/classes/Directory.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes/Directory.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/autoload.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/autoload.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/composer/ClassLoader.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/composer/ClassLoader.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/composer/autoload_real.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/composer/autoload_real.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/composer/installed.json" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/composer/installed.json"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/composer/autoload_classmap.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/composer/autoload_classmap.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/composer/autoload_static.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/composer/autoload_static.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/composer/autoload_namespaces.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/composer/autoload_namespaces.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/composer/autoload_psr4.php" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/composer/autoload_psr4.php"
install -m644 "%{_topdir}/../applications/DataRetriever/vendor/composer/LICENSE" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/vendor/composer/LICENSE"
install -m644 "%{_topdir}/../applications/DataRetriever/composer.lock" "$RPM_BUILD_ROOT/%{installpath}/%{installname}/composer.lock"
mkdir -p $RPM_BUILD_ROOT/usr/lib/systemd/system
mkdir -p $RPM_BUILD_ROOT/etc/supervisord.d
mkdir -p $RPM_BUILD_ROOT/etc/php.d


function installConfFile() {
	sed -e 's|{installpath}|%{installpath}|g' \
		-e 's|{installname}|%{installname}|g' \
		-e 's|{daemonuser}|%{daemonuser}|g' \
		-e 's|{daemongroup}|%{daemongroup}|g' \
		-e 's|{project}|%{project}|g' \
		-e 's|{config}|%{configclass}|g' \
		-e 's|{downloadworkers}|%{downloadworkers}|g' \
		$1 > $RPM_BUILD_ROOT/$2
}

installConfFile %{_topdir}/../applications/DataRetriever/setup/conf/systemd/DataRetriever.service	\
				/usr/lib/systemd/system/%{project}DataRetriever.service
installConfFile %{_topdir}/../applications/DataRetriever/setup/conf/supervisor.d/data-retriever-discovery-worker.ini	\
				/etc/supervisord.d/data-retriever-discovery-worker-%{project}.ini
installConfFile %{_topdir}/../applications/DataRetriever/setup/conf/supervisor.d/data-retriever-download-worker.ini	\
				/etc/supervisord.d/data-retriever-download-worker-%{project}.ini

cp -r	%{_topdir}/../applications/DataRetriever/composer.lock \
		%{_topdir}/../applications/DataRetriever/vendor	\
		$RPM_BUILD_ROOT/%{installpath}/%{installname}


%post
#function configureGearmand() {
#	# TODO improve this
#	cat > /etc/sysconfig/gearmand <<EOF
#### Settings for gearmand
## OPTIONS=""
#OPTIONS="-q Postgres --libpq-conninfo 'postgresql://gearman:***@dbhost:5432/gearman' --libpq-table=$HOSTNAME"
#EOF
#}
#
#
#configureGearmand

systemctl daemon-reload

if [ "$1" = "1" ] ; then  # first install
	systemctl enable gearmand
	systemctl enable supervisord   
	systemctl enable %{project}DataRetriever.service
fi

if ! grep -q "gearmand" /usr/local/components/ADGS/pds2_import_adgs/setup/conf/ListPdsImportServices.txt ; then
    echo "gearmand" >> /usr/local/components/ADGS/pds2_import_adgs/setup/conf/ListPdsImportServices.txt
fi

if ! grep -q "supervisord" /usr/local/components/ADGS/pds2_import_adgs/setup/conf/ListPdsImportServices.txt ; then
    echo "supervisord" >> /usr/local/components/ADGS/pds2_import_adgs/setup/conf/ListPdsImportServices.txt
fi

if ! grep -q "%{project}DataRetriever" /usr/local/components/ADGS/pds2_import_adgs/setup/conf/ListPdsImportServices.txt ; then
    echo "%{project}DataRetriever" >> /usr/local/components/ADGS/pds2_import_adgs/setup/conf/ListPdsImportServices.txt
fi


%preun

if [ "$1" = "0" ] ; then  # Last  uninstall
	systemctl disable %{project}DataRetriever.service
	systemctl disable supervisord   
	systemctl disable gearmand
	
	systemctl daemon-reload
fi


%files
%{installpath}/%{installname}/README.md
%config %{installpath}/%{installname}/config/Config.php
%config %{installpath}/%{installname}/config/ConfigADGS.php
%{installpath}/%{installname}/workers/data-retriever-download-worker.php
%{installpath}/%{installname}/workers/data-retriever-discovery-worker.php
%{installpath}/%{installname}/composer.json
%{installpath}/%{installname}/classes/PHPImportMainProcess.php
%{installpath}/%{installname}/classes/Service/Plugin/Data/Product.php
%{installpath}/%{installname}/classes/Service/Plugin/DataRetrieverPluginOData.php
%{installpath}/%{installname}/classes/Service/Plugin/DataRetrieverPluginInterface.php
%{installpath}/%{installname}/classes/Service/Plugin/DataRetrieverPlugin.php
%{installpath}/%{installname}/classes/Service/Plugin/DataRetrieverPluginMpip.php
%{installpath}/%{installname}/classes/Service/Tools/PluginTools.php
%{installpath}/%{installname}/classes/Service/Exception/PhpImportCannotDownloadException.php
%{installpath}/%{installname}/classes/Service/Exception/PhpImportFileNotFoundException.php
%{installpath}/%{installname}/classes/Service/Exception/PhpImportException.php
%{installpath}/%{installname}/classes/Service/Exception/PhpImportCurlErrorException.php
%{installpath}/%{installname}/classes/Service/Exception/PhpImportCannotFilterException.php
%{installpath}/%{installname}/classes/Service/Exception/PhpImportTimeoutException.php
%{installpath}/%{installname}/classes/Service/Exception/PhpImportBadRequestException.php
%{installpath}/%{installname}/classes/Service/Exception/PhpImportCannotDiscoveryProductsException.php
%{installpath}/%{installname}/classes/Service/Gearman/TaskPayload.php
%{installpath}/%{installname}/classes/Service/Gearman/PHPImportGearmanWorker.php
%{installpath}/%{installname}/classes/Service/Gearman/PHPImportGearmanClient.php
%{installpath}/%{installname}/classes/Service/Curl/CurlExt.php
%{installpath}/%{installname}/classes/Service/Curl/CurlDataRetriever.php
%{installpath}/%{installname}/classes/Service/Curl/HttpAuthentication/HttpAuthenticationBasic.php
%{installpath}/%{installname}/classes/Service/Curl/HttpAuthentication/HttpAuthenticationFactory.php
%{installpath}/%{installname}/classes/Service/Curl/HttpAuthentication/HttpAuthenticationInterface.php
%{installpath}/%{installname}/classes/Service/Curl/HttpAuthentication/HttpAuthenticationNone.php
%{installpath}/%{installname}/classes/Service/Curl/HttpAuthentication/HttpAuthenticationOAuth2.php
%{installpath}/%{installname}/classes/Service/Curl/Protocols/OData.php
%{installpath}/%{installname}/classes/Service/Curl/Protocols/ODataFactory.php
%{installpath}/%{installname}/classes/Service/Curl/Protocols/Mpip.php
%{installpath}/%{installname}/classes/Service/Curl/Protocols/MpipFactory.php
%{installpath}/%{installname}/classes/Service/Curl/Protocols/ProtocolInterface.php
%{installpath}/%{installname}/classes/Service/DB/PHPImportDB.php
%{installpath}/%{installname}/classes/Service/Log/PHPImportLogger.php
%{installpath}/%{installname}/classes/Service/Log/PHPImportLoggerStdout.php
%{installpath}/%{installname}/classes/Service/Log/PHPImportLoggerSyslog.php
%{installpath}/%{installname}/classes/Service/Log/PHPImportCustomLogger.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Logger.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/FlowdockFormatter.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/FluentdFormatter.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/GelfMessageFormatter.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/WildfireFormatter.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/NormalizerFormatter.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/FormatterInterface.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/JsonFormatter.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/HtmlFormatter.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/LogstashFormatter.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/ChromePHPFormatter.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/LogglyFormatter.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/ElasticaFormatter.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/ScalarFormatter.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/LineFormatter.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Formatter/MongoDBFormatter.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Registry.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/ResettableInterface.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Utils.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/ErrorHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/SignalHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/MercurialProcessor.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/IntrospectionProcessor.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/ProcessorInterface.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/GitProcessor.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/MemoryProcessor.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/PsrLogMessageProcessor.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/UidProcessor.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/MemoryPeakUsageProcessor.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/TagProcessor.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/ProcessIdProcessor.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/WebProcessor.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Processor/MemoryUsageProcessor.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/IFTTTHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/SocketHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FingersCrossedHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/RavenHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/AmqpHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/ZendMonitorHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FormattableHandlerTrait.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/NewRelicHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/SamplingHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/HandlerInterface.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/Slack/SlackRecord.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/ElasticSearchHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/ProcessableHandlerTrait.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FlowdockHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/DeduplicationHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/NativeMailerHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/MailHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/PHPConsoleHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/HandlerWrapper.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/SyslogUdpHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/RotatingFileHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/CouchDBHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/SyslogHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/ChromePHPHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/LogEntriesHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/BrowserConsoleHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/RedisHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/SyslogUdp/UdpSocket.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/PushoverHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/InsightOpsHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FingersCrossed/ErrorLevelActivationStrategy.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FingersCrossed/ActivationStrategyInterface.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FingersCrossed/ChannelLevelActivationStrategy.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/ErrorLogHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/ProcessableHandlerInterface.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FleepHookHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/DynamoDbHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/SwiftMailerHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/RollbarHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/MongoDBHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FilterHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/WhatFailureGroupHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/MissingExtensionException.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FirePHPHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/GelfHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/MandrillHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/TestHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/AbstractSyslogHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/DoctrineCouchDBHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/FormattableHandlerInterface.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/StreamHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/AbstractHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/AbstractProcessingHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/SlackWebhookHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/CubeHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/GroupHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/Curl/Util.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/BufferHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/LogglyHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/NullHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/SlackbotHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/SlackHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/HipChatHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/src/Monolog/Handler/PsrHandler.php
%{installpath}/%{installname}/vendor/monolog/monolog/README.md
%{installpath}/%{installname}/vendor/monolog/monolog/CHANGELOG.md
%{installpath}/%{installname}/vendor/monolog/monolog/composer.json
%{installpath}/%{installname}/vendor/monolog/monolog/phpstan.neon.dist
%{installpath}/%{installname}/vendor/monolog/monolog/LICENSE
%{installpath}/%{installname}/vendor/psr/log/README.md
%{installpath}/%{installname}/vendor/psr/log/composer.json
%{installpath}/%{installname}/vendor/psr/log/Psr/Log/LoggerAwareInterface.php
%{installpath}/%{installname}/vendor/psr/log/Psr/Log/InvalidArgumentException.php
%{installpath}/%{installname}/vendor/psr/log/Psr/Log/LogLevel.php
%{installpath}/%{installname}/vendor/psr/log/Psr/Log/LoggerInterface.php
%{installpath}/%{installname}/vendor/psr/log/Psr/Log/NullLogger.php
%{installpath}/%{installname}/vendor/psr/log/Psr/Log/Test/DummyTest.php
%{installpath}/%{installname}/vendor/psr/log/Psr/Log/Test/LoggerInterfaceTest.php
%{installpath}/%{installname}/vendor/psr/log/Psr/Log/Test/TestLogger.php
%{installpath}/%{installname}/vendor/psr/log/Psr/Log/LoggerAwareTrait.php
%{installpath}/%{installname}/vendor/psr/log/Psr/Log/LoggerTrait.php
%{installpath}/%{installname}/vendor/psr/log/Psr/Log/AbstractLogger.php
%{installpath}/%{installname}/vendor/psr/log/LICENSE
%{installpath}/%{installname}/vendor/acs/acs-php-tools/README.md
%{installpath}/%{installname}/vendor/acs/acs-php-tools/composer.json
%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes/Curl.php
%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes/CmdExec.php
%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes/File.php
%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes/Exception/CannotRemoveFileException.php
%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes/Exception/CannotRemoveDirectoryException.php
%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes/Exception/FileException.php
%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes/Exception/CannotCreateDirectoryException.php
%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes/Exception/CommandExecException.php
%{installpath}/%{installname}/vendor/acs/acs-php-tools/classes/Directory.php
%{installpath}/%{installname}/vendor/autoload.php
%{installpath}/%{installname}/vendor/composer/ClassLoader.php
%{installpath}/%{installname}/vendor/composer/autoload_real.php
%{installpath}/%{installname}/vendor/composer/installed.json
%{installpath}/%{installname}/vendor/composer/autoload_classmap.php
%{installpath}/%{installname}/vendor/composer/autoload_static.php
%{installpath}/%{installname}/vendor/composer/autoload_namespaces.php
%{installpath}/%{installname}/vendor/composer/autoload_psr4.php
%{installpath}/%{installname}/vendor/composer/LICENSE
%{installpath}/%{installname}/composer.lock
%dir %{installpath}/%{installname}//bin
%dir %{installpath}/%{installname}//config
%dir %{installpath}/%{installname}//workers
%dir %{installpath}/%{installname}//classes
%dir %{installpath}/%{installname}//classes/Service
%dir %{installpath}/%{installname}//classes/Service/Plugin/Data
%dir %{installpath}/%{installname}//classes/Service/Plugin
%dir %{installpath}/%{installname}//classes/Service/Tools
%dir %{installpath}/%{installname}//classes/Service/Exception
%dir %{installpath}/%{installname}//classes/Service/Gearman
%dir %{installpath}/%{installname}//classes/Service/Curl
%dir %{installpath}/%{installname}//classes/Service/DB
%dir %{installpath}/%{installname}//classes/Service/Log
%dir %{installpath}/%{installname}//vendor
%dir %{installpath}/%{installname}//vendor/monolog
%dir %{installpath}/%{installname}//vendor/monolog/monolog
%dir %{installpath}/%{installname}//vendor/monolog/monolog/src
%dir %{installpath}/%{installname}//vendor/monolog/monolog/src/Monolog
%dir %{installpath}/%{installname}//vendor/monolog/monolog/src/Monolog/Formatter
%dir %{installpath}/%{installname}//vendor/monolog/monolog/src/Monolog/Processor
%dir %{installpath}/%{installname}//vendor/monolog/monolog/src/Monolog/Handler
%dir %{installpath}/%{installname}//vendor/monolog/monolog/src/Monolog/Handler/Slack
%dir %{installpath}/%{installname}//vendor/monolog/monolog/src/Monolog/Handler/SyslogUdp
%dir %{installpath}/%{installname}//vendor/monolog/monolog/src/Monolog/Handler/FingersCrossed
%dir %{installpath}/%{installname}//vendor/monolog/monolog/src/Monolog/Handler/Curl
%dir %{installpath}/%{installname}//vendor/psr
%dir %{installpath}/%{installname}//vendor/psr/log
%dir %{installpath}/%{installname}//vendor/psr/log/Psr
%dir %{installpath}/%{installname}//vendor/psr/log/Psr/Log
%dir %{installpath}/%{installname}//vendor/psr/log/Psr/Log/Test
%dir %{installpath}/%{installname}//vendor/acs
%dir %{installpath}/%{installname}//vendor/acs/acs-php-tools
%dir %{installpath}/%{installname}//vendor/acs/acs-php-tools/classes
%dir %{installpath}/%{installname}//vendor/acs/acs-php-tools/classes/Exception
%dir %{installpath}/%{installname}//vendor/composer
%dir %{installpath}/%{installname}/
%attr(0755, %{daemonuser}, %{daemongroup}) %{installpath}/%{installname}/bin/data-retriever.php
%attr(0755, %{daemonuser}, %{daemongroup}) %{installpath}/%{installname}/bin/data-retriever-configurator.php
%{installpath}/%{installname}/vendor
%{installpath}/%{installname}/composer.lock
/usr/lib/systemd/system/%{project}DataRetriever.service
%config /etc/supervisord.d/data-retriever-download-worker-%{project}.ini
%config /etc/supervisord.d/data-retriever-discovery-worker-%{project}.ini
