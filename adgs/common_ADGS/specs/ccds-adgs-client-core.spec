#########################################################################
%define _ConfPath    /usr/local/conf/ADGS
%define _CompPath    /usr/local/components/ADGS
%define _InitPath    /usr/local/components/ADGS/initScripts
%define _DataPath    /data/ADGS
%define _StorePath   /data/ADGS
%define _ElementName ccds-adgs-client-core
%define _Summary     The ADGS Client Machine services
%define _License     ESA
%define _Group       Applications/%{_ElementName}
%define _Vendor      Exprivia
%define _BuildArch   x86_64
%define _Packager    XPR AIV TEAM
%define _adgsuser    opadgs
%define _adgsgroup   ccds
#########################################################################

%define rpmPkgName      %{_ElementName}

Name:        %{rpmPkgName}
Version:     %{_Version}
Release:     %{dist}
Summary:     %{_Summary}
License:     %{_License}
Group:       %{_Group}
BuildRoot:   %{_topdir}/BUILDROOT/%{name}
URL:         www.exprivia.com
Vendor:      %{_Vendor}
Packager:    %{_Packager}
AutoReqProv: no
Requires:    ccds-adgs-shared-libraries, ccds-adgs-dbclient
Provides:    %{_ElementName} = %{_Version}

%description
%{_Summary}

#%prep

%install

%__rm -fr %{buildroot}
%__mkdir -p %{buildroot}

cd %{buildroot}

umask 022
mkdir -p %{buildroot}/etc/cron.d
mkdir -p %{buildroot}/etc/sudoers.d
mkdir -p %{buildroot}/usr/share/nginx
mkdir -p %{buildroot}/usr/share/nginx/tmp
mkdir -p %{buildroot}%{_ConfPath}

umask 002
mkdir -p %{buildroot}%{_CompPath}
mkdir -p %{buildroot}%{_CompPath}/bin
mkdir -p %{buildroot}%{_CompPath}/lib
mkdir -p %{buildroot}%{_CompPath}/scripts
mkdir -p %{buildroot}%{_CompPath}/xsd
mkdir -p %{buildroot}%{_ConfPath}/TlDaemon
mkdir -p %{buildroot}%{_ConfPath}/tasktables
mkdir -p %{buildroot}%{_ConfPath}/xsl
mkdir -p %{buildroot}%{_ConfPath}/confSpaces
mkdir -p %{buildroot}%{_InitPath}
mkdir -p %{buildroot}%{_StorePath}
mkdir -p %{buildroot}%{_StorePath}/Inventory
mkdir -p %{buildroot}%{_StorePath}/Inventory/backup
mkdir -p %{buildroot}%{_StorePath}/Inventory/working
mkdir -p %{buildroot}%{_StorePath}/tld_productions
mkdir -p %{buildroot}/data
mkdir -p %{buildroot}/data/fromCalVal
mkdir -p %{buildroot}/data/fromECMWF
mkdir -p %{buildroot}/data/fromEUM
mkdir -p %{buildroot}/data/fromMP
mkdir -p %{buildroot}/data/fromPRIP
mkdir -p %{buildroot}/data/fromS1FOS
mkdir -p %{buildroot}/data/fromS1POD
mkdir -p %{buildroot}/data/fromS2FOS
mkdir -p %{buildroot}/data/fromS2POD
mkdir -p %{buildroot}/data/fromS3FOS
mkdir -p %{buildroot}/data/fromS3POD
mkdir -p %{buildroot}/data/fromS3OSI
mkdir -p %{buildroot}/data/S2CAMS
mkdir -p %{buildroot}/data/S2ECMWF
mkdir -p %{buildroot}/data/S2GIPP
mkdir -p %{buildroot}/data/S2UTC
mkdir -p %{buildroot}/data/S2TRO
mkdir -p %{buildroot}/data/FromExt
mkdir -p %{buildroot}/data/tmp
mkdir -p %{buildroot}%{_DataPath}
mkdir -p %{buildroot}%{_DataPath}/conf
mkdir -p %{buildroot}%{_DataPath}/conf/tasktables
mkdir -p %{buildroot}%{_DataPath}/debug
mkdir -p %{buildroot}%{_DataPath}/downloadspace
mkdir -p %{buildroot}%{_DataPath}/log
mkdir -p %{buildroot}%{_DataPath}/productions
mkdir -p %{buildroot}%{_DataPath}/ProductsPath
mkdir -p %{buildroot}%{_DataPath}/so_notifier
mkdir -p %{buildroot}%{_DataPath}/so_notifier/data
mkdir -p %{buildroot}%{_DataPath}/so_notifier/tmp
mkdir -p %{buildroot}%{_DataPath}/CAMSFC-00
mkdir -p %{buildroot}%{_DataPath}/CAMSFC-12
mkdir -p %{buildroot}%{_DataPath}/ECMWFD
mkdir -p %{buildroot}%{_DataPath}/tmp

# init scripts

install -m 0550 %{_topdir}/../initScripts/systemd/adgs-client.service                                  %{buildroot}%{_InitPath}/adgs-client.service
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-importer.service                                %{buildroot}%{_InitPath}/adgs-importer.service
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-thinlayer.service                               %{buildroot}%{_InitPath}/adgs-thinlayer.service
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-package-exporter.service                        %{buildroot}%{_InitPath}/adgs-package-exporter.service
install -m 0550 %{_topdir}/../initScripts/systemd/ADGS-Client-control                                  %{buildroot}%{_InitPath}/ADGS-Client-control
install -m 0550 %{_topdir}/../initScripts/systemd/ADGS_sudo                                            %{buildroot}%{_InitPath}/ADGS_sudo
install -m 0550 %{_topdir}/../initScripts/systemd/SendVersion.sh                                       %{buildroot}%{_CompPath}/scripts/SendVersion.sh

# Added in V-1.1
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-purgelog.service                               %{buildroot}%{_InitPath}/adgs-purgelog.service
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-purgelog.timer                                 %{buildroot}%{_InitPath}/adgs-purgelog.timer

# applications

install -m 0755 %{_topdir}/../applications/DownloadInventoryFile/DownloadInventoryFile                 %{buildroot}%{_CompPath}/bin/DownloadInventoryFile
install -m 0755 %{_topdir}/../applications/Encrypter/Encrypter                                         %{buildroot}%{_CompPath}/bin/Encrypter
install -m 0755 %{_topdir}/../components/libACS_SSL/test/RemoteConnectionsKey_Decode                   %{buildroot}%{_CompPath}/bin/RemoteConnectionsKey_Decode
install -m 0755 %{_topdir}/../applications/FileVersioningTask/FileVersioningTask                       %{buildroot}%{_CompPath}/bin/FileVersioningTask
install -m 0755 %{_topdir}/../applications/Importer/Importer                                           %{buildroot}%{_CompPath}/bin/Importer
install -m 0755 %{_topdir}/../applications/Inventory/Inventory                                         %{buildroot}%{_CompPath}/bin/Inventory
install -m 0755 %{_topdir}/../applications/MultiDownloadInventoryObject/MultiDownloadInventoryObject   %{buildroot}%{_CompPath}/bin/MultiDownloadInventoryObject
install -m 0755 %{_topdir}/../applications/MultiRemoveInventoryObject/MultiRemoveInventoryObject       %{buildroot}%{_CompPath}/bin/MultiRemoveInventoryObject
install -m 0755 %{_topdir}/../applications/RemoveInventoryObject/RemoveInventoryObject                 %{buildroot}%{_CompPath}/bin/RemoveInventoryObject
install -m 0755 %{_topdir}/../applications/Scheduler/Scheduler                                         %{buildroot}%{_CompPath}/bin/Scheduler
install -m 0755 %{_topdir}/../applications/TaskInPreInventory/TaskInPreInventory                       %{buildroot}%{_CompPath}/bin/TaskInPreInventory
install -m 0755 %{_topdir}/../applications/TlDaemon/TlDaemon                                           %{buildroot}%{_CompPath}/bin/TlDaemon
install -m 0755 %{_topdir}/../applications/TLPreInventory/TLPreInventory                               %{buildroot}%{_CompPath}/bin/TLPreInventory
install -m 0755 %{_topdir}/../applications/TTDecompress/TTDecompress                                   %{buildroot}%{_CompPath}/bin/TTDecompress
install -m 0755 %{_topdir}/../applications/WSConfigBuilder/WSConfigBuilder                             %{buildroot}%{_CompPath}/bin/WSConfigBuilder
install -m 0755 %{_topdir}/../applications/WSConfigMerger/WSConfigMerger                               %{buildroot}%{_CompPath}/bin/WSConfigMerger
install -m 0755 %{_topdir}/../applications/PackageCheckCirculatorTask/PackageCheckCirculatorTask       %{buildroot}%{_CompPath}/bin/PackageCheckCirculatorTask
install -m 0755 %{_topdir}/../applications/PackageDistCirculatorTask/PackageDistCirculatorTask         %{buildroot}%{_CompPath}/bin/PackageDistCirculatorTask
install -m 0755 %{_topdir}/../applications/PackageDistFilesystemTask/PackageDistFilesystemTask         %{buildroot}%{_CompPath}/bin/PackageDistFilesystemTask
install -m 0755 %{_topdir}/../applications/PackageExporter/PackageExporter                             %{buildroot}%{_CompPath}/bin/PackageExporter
install -m 0755 %{_topdir}/../applications/PackageFmtEOFpackTask/PackageFmtEOFpackTask                 %{buildroot}%{_CompPath}/bin/PackageFmtEOFpackTask
install -m 0755 %{_topdir}/../applications/PackageFmtEOFpackTask/Scripts/PackageFmtEOFpackTask-Zip     %{buildroot}%{_CompPath}/bin/PackageFmtEOFpackTask-Zip
install -m 0755 %{_topdir}/../applications/PackageDistCurlTask/PackageDistCurlTask                     %{buildroot}%{_CompPath}/bin/PackageDistCurlTask
install -m 0755 %{_topdir}/../applications/S2ECMWFReFormatterTask/s2_ECMWF_re_formatter_task.rb        %{buildroot}%{_CompPath}/bin/s2_ECMWF_re_formatter_task.rb
install -m 0755 %{_topdir}/../applications/S2PreInventoryReformatterTask/S2PreInventoryReformatterTask %{buildroot}%{_CompPath}/bin/S2PreInventoryReformatterTask
install -m 0755 %{_topdir}/../applications/S2CAMSReFormatterTask/s2_CAMS_re_formatter_task.rb          %{buildroot}%{_CompPath}/bin/s2_CAMS_re_formatter_task.rb

# S1 Ruby preinventory
install -m 0755 %{_topdir}/../applications/S1PreInventoryAuxTroTec/s1_pre_inventory_aux_tec.rb            %{buildroot}%{_CompPath}/bin/s1_pre_inventory_aux_tec.rb
install -m 0755 %{_topdir}/../applications/S1PreInventoryAuxTroTec/s1_pre_inventory_aux_tro.rb            %{buildroot}%{_CompPath}/bin/s1_pre_inventory_aux_tro.rb
install -m 0755 %{_topdir}/../applications/MPPreInventoryRuby/mp_pre_inventory.rb                         %{buildroot}%{_CompPath}/bin/mp_pre_inventory.rb
install -m 0755 %{_topdir}/../applications/S1PreInventoryAuxTroTec/s1_pre_inventory_input_file_handler.rb %{buildroot}%{_CompPath}/lib/s1_pre_inventory_input_file_handler.rb
install -m 0755 %{_topdir}/../components/RubyLibs/common_utils.rb										  %{buildroot}%{_CompPath}/lib/common_utils.rb
install -m 0755 %{_topdir}/../components/RubyLibs/job_order_handler.rb									  %{buildroot}%{_CompPath}/lib/job_order_handler.rb
install -m 0755 %{_topdir}/../components/RubyLibs/configuration_handler.rb								  %{buildroot}%{_CompPath}/lib/configuration_handler.rb
install -m 0755 %{_topdir}/../components/RubyLibs/db_handler.rb											  %{buildroot}%{_CompPath}/lib/db_handler.rb
install -m 0755 %{_topdir}/../components/RubyLibs/mtd_handler.rb										  %{buildroot}%{_CompPath}/lib/mtd_handler.rb
install -m 0755 %{_topdir}/../components/RubyLibs/rotating_logger.rb									  %{buildroot}%{_CompPath}/lib/rotating_logger.rb
install -m 0755 %{_topdir}/../components/RubyLibs/splunk_handler.rb										  %{buildroot}%{_CompPath}/lib/splunk_handler.rb

# S2 Ruby preinventory
install -m 0755 %{_topdir}/../applications/S2GIPPPreInventory/s2_gipp_pre_inventory.rb                 %{buildroot}%{_CompPath}/bin/s2_gipp_pre_inventory.rb
install -m 0755 %{_topdir}/../applications/OBMEMCPreInventoryRuby/obmemc_pre_inventory.rb              %{buildroot}%{_CompPath}/bin/obmemc_pre_inventory.rb
install -m 0755 %{_topdir}/../applications/ICECONCPreInventoryRuby/ice_conc_pre_inventory.rb           %{buildroot}%{_CompPath}/bin/ice_conc_pre_inventory.rb

# stylesheet
install -m 0664 %{_topdir}/../components/libExporterBase/StyleSheets/PackageExporterHtmlMail.xsl       %{buildroot}%{_ConfPath}/xsl/PackageExporterHtmlMail.xsl
install -m 0664 %{_topdir}/../components/libExporterBase/StyleSheets/PackageExporterPlainMail.xsl      %{buildroot}%{_ConfPath}/xsl/PackageExporterPlainMail.xsl
install -m 0664 %{_topdir}/../components/libExporterBase/StyleSheets/PackageExporterRsetMail.xsl       %{buildroot}%{_ConfPath}/xsl/PackageExporterRsetMail.xsl

# scripts
install -m 0775 %{_topdir}/../applications/TTDecompress/Scripts/Decompress.sh                          %{buildroot}%{_CompPath}/bin/Decompress.sh
install -m 0775 %{_topdir}/../scripts/Distribution/PackageFmtEOFpackTask-EOFzip                        %{buildroot}%{_CompPath}/bin/PackageFmtEOFpackTask-EOFzip 
install -m 0775 %{_topdir}/../scripts/Distribution/PackageFmtEOFpackTask-DBLzip                        %{buildroot}%{_CompPath}/bin/PackageFmtEOFpackTask-DBLzip 
install -m 0775 %{_topdir}/../scripts/Distribution/PackageFmtEOFpackTask-XMLzip                        %{buildroot}%{_CompPath}/bin/PackageFmtEOFpackTask-XMLzip 
install -m 0775 %{_topdir}/../scripts/Distribution/PackageFmtEOFpackTask-CSVzip                        %{buildroot}%{_CompPath}/bin/PackageFmtEOFpackTask-CSVzip 
install -m 0775 %{_topdir}/../scripts/Distribution/PackageFmtEOFpackTask-KMLzip                        %{buildroot}%{_CompPath}/bin/PackageFmtEOFpackTask-KMLzip 
install -m 0775 %{_topdir}/../scripts/InvUtility/DownloadInventoryFile.sh                              %{buildroot}%{_CompPath}/bin/DownloadInventoryFile.sh
install -m 0775 %{_topdir}/../scripts/InvUtility/MultiDownloadInventoryObject.sh                       %{buildroot}%{_CompPath}/bin/MultiDownloadInventoryObject.sh
install -m 0775 %{_topdir}/../scripts/InvUtility/MultiRemoveInventoryObject.sh                         %{buildroot}%{_CompPath}/bin/MultiRemoveInventoryObject.sh
install -m 0775 %{_topdir}/../scripts/InvUtility/RemoveInventoryObject.sh                              %{buildroot}%{_CompPath}/bin/RemoveInventoryObject.sh
install -m 0775 %{_topdir}/../scripts/PreInventory/ZipSafeFile.sh                                      %{buildroot}%{_CompPath}/bin/ZipSafeFile.sh
install -m 0775 %{_topdir}/../scripts/PreInventory/ZipReformattedSafeFile.sh                           %{buildroot}%{_CompPath}/bin/ZipReformattedSafeFile.sh
install -m 0775 %{_topdir}/../scripts/PreInventory/ZipS1ReformattedSafeFile.sh                         %{buildroot}%{_CompPath}/bin/ZipS1ReformattedSafeFile.sh
install -m 0775 %{_topdir}/../scripts/PreInventory/ZIPEOF_IngestionChain.sh                            %{buildroot}%{_CompPath}/bin/ZIPEOF_IngestionChain.sh
install -m 0775 %{_topdir}/../scripts/PreInventory/GetCamsOriginDate.sh                                %{buildroot}%{_CompPath}/bin/GetCamsOriginDate.sh
install -m 0775 %{_topdir}/../scripts/PreInventory/GetECMWFDOriginDate.sh                              %{buildroot}%{_CompPath}/bin/GetECMWFDOriginDate.sh
install -m 0775 %{_topdir}/../scripts/PreInventory/GetOriginDate.sh                                    %{buildroot}%{_CompPath}/bin/GetOriginDate.sh
install -m 0775 %{_topdir}/../scripts/PreInventory/GetReformattedOriginDate.sh                         %{buildroot}%{_CompPath}/bin/GetReformattedOriginDate.sh
install -m 0775 %{_topdir}/../scripts/PreInventory/GetS1ReformattedOriginDate.sh                       %{buildroot}%{_CompPath}/bin/GetS1ReformattedOriginDate.sh
install -m 0775 %{_topdir}/../scripts/PreInventory/GetTROReformattedOriginDate.sh                      %{buildroot}%{_CompPath}/bin/GetTROReformattedOriginDate.sh
install -m 0775 %{_topdir}/../scripts/PreInventory/RenameAUXTROOrigFiles.sh                            %{buildroot}%{_CompPath}/bin/RenameAUXTROOrigFiles.sh
install -m 0775 %{_topdir}/../scripts/PreInventory/RenameUT1UTCFile.sh                                 %{buildroot}%{_CompPath}/bin/RenameUT1UTCFile.sh
install -m 0775 %{_topdir}/../scripts/PreInventory/RenameECMWFFile.sh                                  %{buildroot}%{_CompPath}/bin/RenameECMWFFile.sh
install -m 0775 %{_topdir}/../scripts/PreInventory/RenameAUXWNDFile.sh                                 %{buildroot}%{_CompPath}/bin/RenameAUXWNDFile.sh
install -m 0775 %{_topdir}/../scripts/PreInventory/TGZ2HDRDBL_IngestionChain.sh                        %{buildroot}%{_CompPath}/bin/TGZ2HDRDBL_IngestionChain.sh
install -m 0775 %{_topdir}/../scripts/PreInventory/S2ECMWFScripts/S2ECMWFPreInventoryRename.rb         %{buildroot}%{_CompPath}/bin/S2ECMWFPreInventoryRename.rb
install -m 0775 %{_topdir}/../scripts/PreInventory/S2ECMWFScripts/libDB.rb                             %{buildroot}%{_CompPath}/lib/libDB.rb  
install -m 0775 %{_topdir}/../scripts/PreInventory/S2ECMWFScripts/libMetaData.rb                       %{buildroot}%{_CompPath}/lib/libMetaData.rb  
install -m 0775 %{_topdir}/../scripts/PreInventory/S2ECMWFScripts/libReadDBConf.rb                     %{buildroot}%{_CompPath}/lib/libReadDBConf.rb  
install -m 0775 %{_topdir}/../scripts/PreInventory/S2ECMWFScripts/libXMLHeader.rb                      %{buildroot}%{_CompPath}/lib/libXMLHeader.rb  
install -m 0775 %{_topdir}/../scripts/ProcUtility/WSConfigBuilder.sh                                   %{buildroot}%{_CompPath}/bin/WSConfigBuilder.sh
install -m 0775 %{_topdir}/../scripts/ProcUtility/WSConfigMerger.sh                                    %{buildroot}%{_CompPath}/bin/WSConfigMerger.sh
install -m 0775 %{_topdir}/../scripts/Utility/Decrypter.sh                                             %{buildroot}%{_CompPath}/bin/Decrypter.sh
install -m 0775 %{_topdir}/../scripts/Utility/Encrypter.sh                                             %{buildroot}%{_CompPath}/bin/Encrypter.sh
install -m 0775 %{_topdir}/../scripts/Utility/ExportContextScript.py                                   %{buildroot}%{_CompPath}/bin/ExportContextScript.py
# Added in V-1.1
install -m 0755 %{_topdir}/../scripts/Utility/purgeLogFiles.sh                                         %{buildroot}%{_CompPath}/scripts/purgeLogFiles.sh

# confspaces

install -m 0664 %{_topdir}/../ADGSConf/confSpaces/MACHINE-NAME.xml                                     %{buildroot}%{_ConfPath}/confSpaces/MACHINE-NAME.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/dbBroadcast.xml                                      %{buildroot}%{_ConfPath}/confSpaces/dbBroadcast.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/EmailUserNotification.xml                            %{buildroot}%{_ConfPath}/confSpaces/EmailUserNotification.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/Importer.xml                                         %{buildroot}%{_ConfPath}/confSpaces/Importer.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/libFtpCurl.so.xml                                    %{buildroot}%{_ConfPath}/confSpaces/libFtpCurl.so.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/libGenFileAction.xml                                 %{buildroot}%{_ConfPath}/confSpaces/libGenFileAction.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/libScDefaultLogGroup.so.xml                          %{buildroot}%{_ConfPath}/confSpaces/libScDefaultLogGroup.so.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/OrderTypeManager.xml                                 %{buildroot}%{_ConfPath}/confSpaces/OrderTypeManager.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/PackageEmailNotification.xml                         %{buildroot}%{_ConfPath}/confSpaces/PackageEmailNotification.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/PackageExporter.xml                                  %{buildroot}%{_ConfPath}/confSpaces/PackageExporter.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/PackageProcessing.xml                                %{buildroot}%{_ConfPath}/confSpaces/PackageProcessing.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/StationConf.xml                                      %{buildroot}%{_ConfPath}/confSpaces/StationConf.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/tlDaemon.xml                                         %{buildroot}%{_ConfPath}/confSpaces/tlDaemon.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/libOcadaJobOrder.so.xml                              %{buildroot}%{_ConfPath}/confSpaces/libOcadaJobOrder.so.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/libTlDefaultPCC.so.xml                               %{buildroot}%{_ConfPath}/confSpaces/libTlDefaultPCC.so.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/Scheduler.xml                                        %{buildroot}%{_ConfPath}/confSpaces/Scheduler.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/TaskInPreInventory.xml                               %{buildroot}%{_ConfPath}/confSpaces/TaskInPreInventory.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/PreInventory.xml                                     %{buildroot}%{_ConfPath}/confSpaces/PreInventory.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/Inventory.xml                                        %{buildroot}%{_ConfPath}/confSpaces/Inventory.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/ACSTimeConverter.xml                                 %{buildroot}%{_ConfPath}/confSpaces/ACSTimeConverter.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/ExplorerWrapper.xml                                  %{buildroot}%{_ConfPath}/confSpaces/ExplorerWrapper.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/libSafePreInventory.so.xml                           %{buildroot}%{_ConfPath}/confSpaces/libSafePreInventory.so.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/InventoryEmailNotification.xml                       %{buildroot}%{_ConfPath}/confSpaces/InventoryEmailNotification.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/libFilenamePreInventory.so.xml                       %{buildroot}%{_ConfPath}/confSpaces/libFilenamePreInventory.so.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/libEOFPreInventory.so.xml                            %{buildroot}%{_ConfPath}/confSpaces/libEOFPreInventory.so.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/libNETCDFPreInventory.so.xml                         %{buildroot}%{_ConfPath}/confSpaces/libNETCDFPreInventory.so.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/libEOFFilenamePreInventory.so.xml                    %{buildroot}%{_ConfPath}/confSpaces/libEOFFilenamePreInventory.so.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/libGRIBPreInventory.so.xml                           %{buildroot}%{_ConfPath}/confSpaces/libGRIBPreInventory.so.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/S2PreInventoryReformatterTask.xml                    %{buildroot}%{_ConfPath}/confSpaces/S2PreInventoryReformatterTask.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/TTDecompress.xml                                     %{buildroot}%{_ConfPath}/confSpaces/TTDecompress.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/s2_gipp_pre_inventory.conf                           %{buildroot}%{_ConfPath}/confSpaces/s2_gipp_pre_inventory.conf
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/obmemc_pre_inventory.conf                            %{buildroot}%{_ConfPath}/confSpaces/obmemc_pre_inventory.conf
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/ice_conc_pre_inventory.conf                          %{buildroot}%{_ConfPath}/confSpaces/ice_conc_pre_inventory.conf
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/s2_cams_reformatter00.conf                           %{buildroot}%{_ConfPath}/confSpaces/s2_cams_reformatter00.conf
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/s2_cams_reformatter12.conf                           %{buildroot}%{_ConfPath}/confSpaces/s2_cams_reformatter12.conf
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/CamsTemplate.HDR                                     %{buildroot}%{_ConfPath}/confSpaces/CamsTemplate.HDR
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/s2_ecmwfd_reformatter.conf                           %{buildroot}%{_ConfPath}/confSpaces/s2_ecmwfd_reformatter.conf
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/ECMWFDTemplate.HDR                                   %{buildroot}%{_ConfPath}/confSpaces/ECMWFDTemplate.HDR
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/misson_planning_pre_inventory.conf                   %{buildroot}%{_ConfPath}/confSpaces/misson_planning_pre_inventory.conf

# tasktables

install -m 0664 %{_topdir}/../ADGSConf/taskTables/decompressReception.xml                              %{buildroot}%{_ConfPath}/tasktables/decompressReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/decompressSafeDefaultReception.xml                   %{buildroot}%{_ConfPath}/tasktables/decompressSafeDefaultReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/defaultDelivery.xml                                  %{buildroot}%{_ConfPath}/tasktables/defaultDelivery.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/defaultDistribFormat.xml                             %{buildroot}%{_ConfPath}/tasktables/defaultDistribFormat.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/eofzipDistribFormat.xml                              %{buildroot}%{_ConfPath}/tasktables/eofzipDistribFormat.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/dblzipDistribFormat.xml                              %{buildroot}%{_ConfPath}/tasktables/dblzipDistribFormat.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/xmlzipDistribFormat.xml                              %{buildroot}%{_ConfPath}/tasktables/xmlzipDistribFormat.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/defaultReception.xml                                 %{buildroot}%{_ConfPath}/tasktables/defaultReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/safeDefaultReceptionZip.xml                          %{buildroot}%{_ConfPath}/tasktables/safeDefaultReceptionZip.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/ZipEofReception.xml                                  %{buildroot}%{_ConfPath}/tasktables/ZipEofReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/auxtecReception.xml                                  %{buildroot}%{_ConfPath}/tasktables/auxtecReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/auxtroReception.xml                                  %{buildroot}%{_ConfPath}/tasktables/auxtroReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/TGZ2HDRDBLReception.xml                              %{buildroot}%{_ConfPath}/tasktables/TGZ2HDRDBLReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/nopDeliveryCheck.xml                                 %{buildroot}%{_ConfPath}/tasktables/nopDeliveryCheck.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/ecmwfReception.xml                                   %{buildroot}%{_ConfPath}/tasktables/ecmwfReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/auxwndReception.xml                                  %{buildroot}%{_ConfPath}/tasktables/auxwndReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/eofFmtReception.xml                                  %{buildroot}%{_ConfPath}/tasktables/eofFmtReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/gippReception.xml                                    %{buildroot}%{_ConfPath}/tasktables/gippReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/obmemcReception.xml                                  %{buildroot}%{_ConfPath}/tasktables/obmemcReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/osiiceReception.xml                                  %{buildroot}%{_ConfPath}/tasktables/osiiceReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/camsReception00.xml                                  %{buildroot}%{_ConfPath}/tasktables/camsReception00.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/camsReception12.xml                                  %{buildroot}%{_ConfPath}/tasktables/camsReception12.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/CAMSTGZReception.xml                                 %{buildroot}%{_ConfPath}/tasktables/CAMSTGZReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/ECMWFDTGZReception.xml                               %{buildroot}%{_ConfPath}/tasktables/ECMWFDTGZReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/MissPlanReception.xml                                %{buildroot}%{_ConfPath}/tasktables/MissPlanReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/csvzipDistribFormat.xml                              %{buildroot}%{_ConfPath}/tasktables/csvzipDistribFormat.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/kmlzipDistribFormat.xml                              %{buildroot}%{_ConfPath}/tasktables/kmlzipDistribFormat.xml

# xsd
install -m 0664 %{_topdir}/../ADGSConf/xsd/S2GIPPValidator.xsd                                         %{buildroot}%{_CompPath}/xsd/S2GIPPValidator.xsd

ln -s %{_ConfPath}/xsl                 .%{_DataPath}/conf/xsl

%files
%dir %attr(-,root,root) /usr/local
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/bin
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/lib
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/xsd
%dir %attr(-,root,%{_adgsgroup}) %{_ConfPath}
%dir %attr(-,root,%{_adgsgroup}) %{_ConfPath}/TlDaemon
%dir %attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables
%dir %attr(-,root,%{_adgsgroup}) %{_ConfPath}/xsl
%dir %attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces
%dir %attr(-,root,%{_adgsgroup}) %{_StorePath}
%dir %attr(-,root,%{_adgsgroup}) %{_StorePath}/Inventory
%dir %attr(-,root,%{_adgsgroup}) %{_StorePath}/Inventory/backup
%dir %attr(-,root,%{_adgsgroup}) %{_StorePath}/Inventory/working
%dir %attr(-,root,%{_adgsgroup}) %{_StorePath}/tld_productions
%dir %attr(-,root,%{_adgsgroup}) /data/fromCalVal
%dir %attr(-,root,%{_adgsgroup}) /data/fromECMWF
%dir %attr(-,root,%{_adgsgroup}) /data/fromEUM
%dir %attr(-,root,%{_adgsgroup}) /data/fromMP
%dir %attr(-,root,%{_adgsgroup}) /data/fromPRIP
%dir %attr(-,root,%{_adgsgroup}) /data/fromS1FOS
%dir %attr(-,root,%{_adgsgroup}) /data/fromS1POD
%dir %attr(-,root,%{_adgsgroup}) /data/fromS2FOS
%dir %attr(-,root,%{_adgsgroup}) /data/fromS2POD
%dir %attr(-,root,%{_adgsgroup}) /data/fromS3FOS
%dir %attr(-,root,%{_adgsgroup}) /data/fromS3POD
%dir %attr(-,root,%{_adgsgroup}) /data/fromS3OSI
%dir %attr(-,root,%{_adgsgroup}) /data/S2CAMS
%dir %attr(-,root,%{_adgsgroup}) /data/S2ECMWF
%dir %attr(-,root,%{_adgsgroup}) /data/S2GIPP
%dir %attr(-,root,%{_adgsgroup}) /data/S2UTC
%dir %attr(-,root,%{_adgsgroup}) /data/S2TRO
%dir %attr(-,root,%{_adgsgroup}) /data/FromExt
%dir %attr(-,root,%{_adgsgroup}) /data/tmp
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/conf
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/conf/tasktables
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/debug
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/downloadspace
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/log
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/productions
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/ProductsPath
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/so_notifier
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/so_notifier/data
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/so_notifier/tmp
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/CAMSFC-00
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/CAMSFC-12
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/ECMWFD
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/tmp

%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-client.service
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-importer.service
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-thinlayer.service
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-package-exporter.service
%attr(-,root,%{_adgsgroup}) %{_InitPath}/ADGS-Client-control
%attr(-,root,%{_adgsgroup}) %{_InitPath}/ADGS_sudo
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-purgelog.service
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-purgelog.timer
%attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts/SendVersion.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/DownloadInventoryFile
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/Encrypter
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/RemoteConnectionsKey_Decode
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/FileVersioningTask
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/Importer
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/Inventory
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/MultiDownloadInventoryObject
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/MultiRemoveInventoryObject
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/RemoveInventoryObject
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/Scheduler
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/TaskInPreInventory
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/TlDaemon
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/TLPreInventory
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/TTDecompress
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/WSConfigBuilder
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/WSConfigMerger
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/PackageCheckCirculatorTask
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/PackageDistCirculatorTask
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/PackageDistFilesystemTask
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/PackageExporter
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/PackageFmtEOFpackTask
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/PackageFmtEOFpackTask-Zip
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/PackageDistCurlTask
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/s2_ECMWF_re_formatter_task.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/S2PreInventoryReformatterTask
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/s2_CAMS_re_formatter_task.rb

%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/s1_pre_inventory_aux_tec.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/s1_pre_inventory_aux_tro.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/mp_pre_inventory.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/s1_pre_inventory_input_file_handler.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/common_utils.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/job_order_handler.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/configuration_handler.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/db_handler.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/mtd_handler.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/rotating_logger.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/splunk_handler.rb

%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/s2_gipp_pre_inventory.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/obmemc_pre_inventory.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/ice_conc_pre_inventory.rb

%attr(-,root,%{_adgsgroup}) %{_ConfPath}/xsl/PackageExporterHtmlMail.xsl
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/xsl/PackageExporterPlainMail.xsl
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/xsl/PackageExporterRsetMail.xsl

%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/Decompress.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/PackageFmtEOFpackTask-EOFzip 
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/PackageFmtEOFpackTask-DBLzip 
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/PackageFmtEOFpackTask-XMLzip 
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/PackageFmtEOFpackTask-CSVzip 
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/PackageFmtEOFpackTask-KMLzip 
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/DownloadInventoryFile.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/MultiDownloadInventoryObject.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/MultiRemoveInventoryObject.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/RemoveInventoryObject.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/ZipSafeFile.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/ZIPEOF_IngestionChain.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/ZipReformattedSafeFile.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/ZipS1ReformattedSafeFile.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/GetOriginDate.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/GetCamsOriginDate.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/GetECMWFDOriginDate.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/GetReformattedOriginDate.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/GetS1ReformattedOriginDate.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/GetTROReformattedOriginDate.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/S2ECMWFPreInventoryRename.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/libDB.rb  
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/libMetaData.rb  
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/libReadDBConf.rb  
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/libXMLHeader.rb  
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/RenameUT1UTCFile.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/RenameECMWFFile.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/RenameAUXWNDFile.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/RenameAUXTROOrigFiles.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/TGZ2HDRDBL_IngestionChain.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/WSConfigBuilder.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/WSConfigMerger.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/Decrypter.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/Encrypter.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/ExportContextScript.py
%attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts/purgeLogFiles.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/xsd/S2GIPPValidator.xsd
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/MACHINE-NAME.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/dbBroadcast.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/EmailUserNotification.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/Importer.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/libFtpCurl.so.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/libGenFileAction.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/libScDefaultLogGroup.so.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/OrderTypeManager.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/PackageEmailNotification.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/PackageExporter.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/PackageProcessing.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/StationConf.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/tlDaemon.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/libOcadaJobOrder.so.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/libTlDefaultPCC.so.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/Scheduler.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/TaskInPreInventory.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/PreInventory.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/Inventory.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/ACSTimeConverter.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/ExplorerWrapper.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/libSafePreInventory.so.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/InventoryEmailNotification.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/libFilenamePreInventory.so.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/libEOFPreInventory.so.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/libNETCDFPreInventory.so.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/libEOFFilenamePreInventory.so.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/libGRIBPreInventory.so.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/S2PreInventoryReformatterTask.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/TTDecompress.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/s2_gipp_pre_inventory.conf
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/obmemc_pre_inventory.conf
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/ice_conc_pre_inventory.conf
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/s2_cams_reformatter00.conf
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/s2_cams_reformatter12.conf
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/CamsTemplate.HDR
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/s2_ecmwfd_reformatter.conf
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/ECMWFDTemplate.HDR
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/misson_planning_pre_inventory.conf
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/decompressReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/decompressSafeDefaultReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/defaultDelivery.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/defaultDistribFormat.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/eofzipDistribFormat.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/dblzipDistribFormat.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/xmlzipDistribFormat.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/defaultReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/safeDefaultReceptionZip.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/ZipEofReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/auxtecReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/auxtroReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/TGZ2HDRDBLReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/nopDeliveryCheck.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/ecmwfReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/auxwndReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/eofFmtReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/gippReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/obmemcReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/osiiceReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/camsReception00.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/camsReception12.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/CAMSTGZReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/ECMWFDTGZReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/MissPlanReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/csvzipDistribFormat.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/tasktables/kmlzipDistribFormat.xml

%{_DataPath}/conf/xsl

%post

systemctl daemon-reload

if [ "$1" -eq 1 ]; then

    sed -i "s/##ADGSDB_MACHINENAME##/$ADGSDB_MACHINENAME/g" %{_CompPath}/bin/GetReformattedOriginDate.sh
    sed -i "s/##PDSUSER_DBPWD##/$PDSUSER_DBPWD/g"           %{_CompPath}/bin/GetReformattedOriginDate.sh
    sed -i "s/##ADGSDB_MACHINENAME##/$ADGSDB_MACHINENAME/g" %{_CompPath}/bin/GetS1ReformattedOriginDate.sh
    sed -i "s/##PDSUSER_DBPWD##/$PDSUSER_DBPWD/g"           %{_CompPath}/bin/GetS1ReformattedOriginDate.sh
    sed -i "s/##ADGSDB_MACHINENAME##/$ADGSDB_MACHINENAME/g" %{_CompPath}/bin/GetTROReformattedOriginDate.sh
    sed -i "s/##PDSUSER_DBPWD##/$PDSUSER_DBPWD/g"           %{_CompPath}/bin/GetTROReformattedOriginDate.sh

    export LD_LIBRARY_PATH=%{_CompPath}/lib
    su - %{_adgsuser} -c "%{_CompPath}/bin/WSConfigBuilder.sh -l notexist -o %{_ConfPath}/TlDaemon/WSConfig.xml"

    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/MACHINE-NAME.xml %{_DataPath}/conf/$(hostname).xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/dbBroadcast.xml %{_DataPath}/conf/dbBroadcast.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/EmailUserNotification.xml %{_DataPath}/conf/EmailUserNotification.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/Importer.xml %{_DataPath}/conf/Importer.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/libFtpCurl.so.xml %{_DataPath}/conf/libFtpCurl.so.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/libGenFileAction.xml %{_DataPath}/conf/libGenFileAction.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/libScDefaultLogGroup.so.xml %{_DataPath}/conf/libScDefaultLogGroup.so.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/OrderTypeManager.xml %{_DataPath}/conf/OrderTypeManager.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/PackageEmailNotification.xml %{_DataPath}/conf/PackageEmailNotification.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/PackageExporter.xml %{_DataPath}/conf/PackageExporter.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/PackageProcessing.xml %{_DataPath}/conf/PackageProcessing.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/StationConf.xml %{_DataPath}/conf/StationConf.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/tlDaemon.xml %{_DataPath}/conf/tlDaemon.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/libOcadaJobOrder.so.xml %{_DataPath}/conf/libOcadaJobOrder.so.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/libTlDefaultPCC.so.xml %{_DataPath}/conf/libTlDefaultPCC.so.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/Scheduler.xml %{_DataPath}/conf/Scheduler.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/TaskInPreInventory.xml %{_DataPath}/conf/TaskInPreInventory.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/PreInventory.xml %{_DataPath}/conf/PreInventory.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/Inventory.xml %{_DataPath}/conf/Inventory.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/ACSTimeConverter.xml %{_DataPath}/conf/ACSTimeConverter.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/ExplorerWrapper.xml %{_DataPath}/conf/ExplorerWrapper.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/libSafePreInventory.so.xml %{_DataPath}/conf/libSafePreInventory.so.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/InventoryEmailNotification.xml %{_DataPath}/conf/InventoryEmailNotification.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/libFilenamePreInventory.so.xml %{_DataPath}/conf/libFilenamePreInventory.so.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/libEOFPreInventory.so.xml %{_DataPath}/conf/libEOFPreInventory.so.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/libNETCDFPreInventory.so.xml %{_DataPath}/conf/libNETCDFPreInventory.so.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/libEOFFilenamePreInventory.so.xml %{_DataPath}/conf/libEOFFilenamePreInventory.so.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/libGRIBPreInventory.so.xml %{_DataPath}/conf/libGRIBPreInventory.so.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/S2PreInventoryReformatterTask.xml %{_DataPath}/conf/S2PreInventoryReformatterTask.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/TTDecompress.xml %{_DataPath}/conf/TTDecompress.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/s2_gipp_pre_inventory.conf %{_DataPath}/conf/s2_gipp_pre_inventory.conf"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/obmemc_pre_inventory.conf %{_DataPath}/conf/obmemc_pre_inventory.conf"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/ice_conc_pre_inventory.conf %{_DataPath}/conf/ice_conc_pre_inventory.conf"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/s2_cams_reformatter00.conf %{_DataPath}/conf/s2_cams_reformatter00.conf"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/s2_cams_reformatter12.conf %{_DataPath}/conf/s2_cams_reformatter12.conf"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/CamsTemplate.HDR %{_DataPath}/conf/CamsTemplate.HDR"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/s2_ecmwfd_reformatter.conf %{_DataPath}/conf/s2_ecmwfd_reformatter.conf"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/ECMWFDTemplate.HDR %{_DataPath}/conf/ECMWFDTemplate.HDR"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/misson_planning_pre_inventory.conf %{_DataPath}/conf/misson_planning_pre_inventory.conf"

    rowexist=$(psql -X -A -h $ADGSDB_MACHINENAME -U acsdba -d adgsdb -t -c "select count (*) from t_knownhosts where host_name = '$(hostname)'")
    if [[ $rowexist -ne 1 ]]
    then
        psql -h $ADGSDB_MACHINENAME -U acsdba adgsdb -c "insert into t_knownhosts (host_name) values ('$(hostname)')"
    fi

    cp %{_InitPath}/ADGS_sudo /etc/sudoers.d/ADGS_sudo
    chmod 550 /etc/sudoers.d/ADGS_sudo

    chmod 777 /usr/share/nginx/tmp

    # link the systemd Daemon Services/Timer
    services=( "adgs-client.service" "adgs-package-exporter.service" "adgs-importer.service" "adgs-thinlayer.service" "adgs-purgelog.service" "adgs-purgelog.timer" )

    for i in "${services[@]}"
    do
        if [ ! -h /etc/systemd/system/$i ]; then
            systemctl enable   %{_InitPath}/$i
            systemctl daemon-reload
        fi
    done

	systemctl start adgs-purgelog.timer

    # Reload for new installations 
    systemctl daemon-reload
fi

if [ "$1" -eq 2 ]; then

    sed -i "s/##ADGSDB_MACHINENAME##/$ADGSDB_MACHINENAME/g" %{_CompPath}/bin/GetReformattedOriginDate.sh
    sed -i "s/##PDSUSER_DBPWD##/$PDSUSER_DBPWD/g"           %{_CompPath}/bin/GetReformattedOriginDate.sh
    sed -i "s/##ADGSDB_MACHINENAME##/$ADGSDB_MACHINENAME/g" %{_CompPath}/bin/GetS1ReformattedOriginDate.sh
    sed -i "s/##PDSUSER_DBPWD##/$PDSUSER_DBPWD/g"           %{_CompPath}/bin/GetS1ReformattedOriginDate.sh
    sed -i "s/##ADGSDB_MACHINENAME##/$ADGSDB_MACHINENAME/g" %{_CompPath}/bin/GetTROReformattedOriginDate.sh
    sed -i "s/##PDSUSER_DBPWD##/$PDSUSER_DBPWD/g"           %{_CompPath}/bin/GetTROReformattedOriginDate.sh

    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/libEOFFilenamePreInventory.so.xml %{_DataPath}/conf/libEOFFilenamePreInventory.so.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/libEOFPreInventory.so.xml %{_DataPath}/conf/libEOFPreInventory.so.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/misson_planning_pre_inventory.conf %{_DataPath}/conf/misson_planning_pre_inventory.conf"

    # link the systemd Daemon Services/Timer
    services=( "adgs-client.service" "adgs-package-exporter.service" "adgs-importer.service" "adgs-thinlayer.service" "adgs-purgelog.service" "adgs-purgelog.timer" )

    for i in "${services[@]}"
    do
        if [ ! -h /etc/systemd/system/$i ]; then
            systemctl enable   %{_InitPath}/$i
            systemctl daemon-reload
        fi
    done

	systemctl start adgs-purgelog.timer

    # Reload for new installations 
    systemctl daemon-reload

fi

if ! grep -q %{_adgsuser} /etc/cron.allow ; then
    echo "%{_adgsuser}" >> /etc/cron.allow
fi

%preun
if [ "$1" -eq 0 ]; then
    rm /etc/sudoers.d/ADGS_sudo

    rm -f %{_DataPath}/conf/$(hostname).xml
    rm -f %{_DataPath}/conf/dbBroadcast.xml
    rm -f %{_DataPath}/conf/EmailUserNotification.xml
    rm -f %{_DataPath}/conf/Importer.xml
    rm -f %{_DataPath}/conf/libFtpCurl.so.xml
    rm -f %{_DataPath}/conf/libGenFileAction.xml
    rm -f %{_DataPath}/conf/libScDefaultLogGroup.so.xml
    rm -f %{_DataPath}/conf/OrderTypeManager.xml
    rm -f %{_DataPath}/conf/PackageEmailNotification.xml
    rm -f %{_DataPath}/conf/PackageExporter.xml
    rm -f %{_DataPath}/conf/PackageProcessing.xml
    rm -f %{_DataPath}/conf/StationConf.xml
    rm -f %{_DataPath}/conf/tlDaemon.xml
    rm -f %{_DataPath}/conf/libOcadaJobOrder.so.xml
    rm -f %{_DataPath}/conf/libTlDefaultPCC.so.xml
    rm -f %{_DataPath}/conf/Scheduler.xml
    rm -f %{_DataPath}/conf/TaskInPreInventory.xml
    rm -f %{_DataPath}/conf/PreInventory.xml
    rm -f %{_DataPath}/conf/Inventory.xml
    rm -f %{_DataPath}/conf/ACSTimeConverter.xml
    rm -f %{_DataPath}/conf/ExplorerWrapper.xml
    rm -f %{_DataPath}/conf/libSafePreInventory.so.xml
    rm -f %{_DataPath}/conf/InventoryEmailNotification.xml
    rm -f %{_DataPath}/conf/libFilenamePreInventory.so.xml
    rm -f %{_DataPath}/conf/libEOFPreInventory.so.xml
    rm -f %{_DataPath}/conf/libNETCDFPreInventory.so.xml
    rm -f %{_DataPath}/conf/libEOFFilenamePreInventory.so.xml
    rm -f %{_DataPath}/conf/libGRIBPreInventory.so.xml
    rm -f %{_DataPath}/conf/S2PreInventoryReformatterTask.xml
    rm -f %{_DataPath}/conf/TTDecompress.xml
    rm -f %{_DataPath}/conf/s2_gipp_pre_inventory.conf
    rm -f %{_DataPath}/conf/obmemc_pre_inventory.conf
    rm -f %{_DataPath}/conf/ice_conc_pre_inventory.conf
    rm -f %{_DataPath}/conf/s2_cams_reformatter00.conf
    rm -f %{_DataPath}/conf/s2_cams_reformatter12.conf
    rm -f %{_DataPath}/conf/CamsTemplate.HDR
    rm -f %{_DataPath}/conf/s2_ecmwfd_reformatter.conf
    rm -f %{_DataPath}/conf/ECMWFDTemplate.HDR
    rm -f %{_DataPath}/conf/misson_planning_pre_inventory.conf

	systemctl stop adgs-purgelog.timer

    # link the systemd Daemon Services/Timer
    services=( "adgs-client.service" "adgs-package-exporter.service" "adgs-importer.service" "adgs-thinlayer.service" "adgs-purgelog.timer" "adgs-purgelog.service" )

    for i in "${services[@]}"
    do
        if [ ! -h /etc/systemd/system/$i ]; then
            systemctl disable   %{_InitPath}/$i
            systemctl daemon-reload
        fi
    done
    # Reload for new installations 
    systemctl daemon-reload
fi

%clean
rm -rf %{buildroot} %{rpmPkgName}
