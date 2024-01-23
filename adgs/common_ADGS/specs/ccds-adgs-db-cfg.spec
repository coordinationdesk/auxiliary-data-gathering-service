#########################################################################
%define _ConfPath    /usr/local/conf/ADGS
%define _CompPath    /usr/local/components/ADGS
%define _InitPath    /usr/local/ADGS
%define _DataPath    /data/ADGS
%define _StorePath   /data/ADGS
%define _ElementName ccds-adgs-db-cfg
%define _Summary     The ADGS Database Configuration
%define _License     ESA
%define _Group       Applications/%{_ElementName}
%define _Vendor      Exprivia
%define _BuildArch   x86_64
%define _Packager    XPR AIV TEAM
%define _adgsuser    opadgs
%define _adgsgroup   ccds
#########################################################################

%define rpmPkgName %{_ElementName}

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
Requires:    postgresql13-server, postgresql13-contrib, ruby, rubygem-pg, ccds-adgs-postgresql-schema
Provides:    %{_ElementName} = %{_Version}

%description
%{_Summary}

#%prep

%install

%__rm -fr %{buildroot}
%__mkdir -p %{buildroot}

cd %{buildroot}

umask 022
mkdir -p %{buildroot}%{_ConfPath}

umask 002
mkdir -p %{buildroot}%{_CompPath}
mkdir -p %{buildroot}%{_CompPath}/RubyScripts
mkdir -p %{buildroot}%{_CompPath}/scripts
mkdir -p %{buildroot}%{_ConfPath}/DbConfiguration
mkdir -p %{buildroot}%{_ConfPath}/DbConfiguration
mkdir -p %{buildroot}%{_ConfPath}/DbConfiguration/csv
mkdir -p %{buildroot}%{_ConfPath}/DbConfiguration/query
mkdir -p %{buildroot}%{_ConfPath}/DbConfiguration/tasktables


### Ruby Tool
install -m 0755 %{_topdir}/../scripts/Ruby/LoadDbConfSpace.rb                     %{buildroot}%{_CompPath}/RubyScripts/LoadDbConfSpace.rb
install -m 0755 %{_topdir}/../scripts/Ruby/LoadDbParameters.rb                    %{buildroot}%{_CompPath}/RubyScripts/LoadDbParameters.rb
install -m 0755 %{_topdir}/../scripts/Utility/escPaswdString.sh                   %{buildroot}%{_CompPath}/scripts/escPaswdString.sh
install -m 0755 %{_topdir}/../scripts/Utility/exequteSql.sh                       %{buildroot}%{_CompPath}/scripts/exequteSql.sh

### CSV
# DB Parameters
install -m 0664 %{_topdir}/../ADGSConf/csv/t_dbstatus.csv                         %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_dbstatus.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_discoveryplugins.csv                 %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_discoveryplugins.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_fileclasses.csv                      %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_fileclasses.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_ordertypes.csv                       %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_ordertypes.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_originators.csv                      %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_originators.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_usertypes.csv                        %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_usertypes.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_rollingpolicyactions.csv             %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_rollingpolicyactions.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_bands.csv                            %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_bands.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_satrelations.csv                     %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_satrelations.csv

# V-1.4
install -m 0664 %{_topdir}/../ADGSConf/csv/t_originators_Add-V1.4.csv             %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_originators_Add-V1.4.csv

# V-1.6
install -m 0664 %{_topdir}/../ADGSConf/csv/t_originators_Add-V1.6.csv             %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_originators_Add-V1.6.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_discoveryplugins_Add-V1.6.csv        %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_discoveryplugins_Add-V1.6.csv

# DB Parameters for extra packages
# V-1.0
install -m 0664 %{_topdir}/../ADGSConf/csv/t_filetypes.csv                        %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_filetypes.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_filetypescomp.csv                    %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_filetypescomp.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_users.csv                            %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_users.csv

# V-1.1
install -m 0664 %{_topdir}/../ADGSConf/csv/t_filetypes_Add-V1.1.csv               %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_filetypes_Add-V1.1.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_filetypescomp_Add-V1.1.csv           %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_filetypescomp_Add-V1.1.csv

# V-1.2
install -m 0664 %{_topdir}/../ADGSConf/csv/t_filetypes_Add-V1.2.csv               %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_filetypes_Add-V1.2.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_filetypescomp_Add-V1.2.csv           %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_filetypescomp_Add-V1.2.csv

# V-1.3
install -m 0664 %{_topdir}/../ADGSConf/csv/t_filetypes_Add-V1.3.csv               %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_filetypes_Add-V1.3.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_filetypescomp_Add-V1.3.csv           %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_filetypescomp_Add-V1.3.csv

# V-1.4
install -m 0664 %{_topdir}/../ADGSConf/csv/t_filetypes_Add-V1.4.csv               %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_filetypes_Add-V1.4.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_filetypescomp_Add-V1.4.csv           %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_filetypescomp_Add-V1.4.csv

# V-1.5.5
install -m 0664 %{_topdir}/../ADGSConf/csv/t_filetypes_Add-V1.5.5.csv             %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_filetypes_Add-V1.5.5.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_filetypescomp_Add-V1.5.5.csv         %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_filetypescomp_Add-V1.5.5.csv

# V-1.6
install -m 0664 %{_topdir}/../ADGSConf/csv/t_filetypes_Add-V1.6.csv             %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_filetypes_Add-V1.6.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_filetypescomp_Add-V1.6.csv         %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_filetypescomp_Add-V1.6.csv

# X Circulation
# V-1.0
install -m 0664 %{_topdir}/../ADGSConf/csv/t_knownhosts.csv                       %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_knownhosts.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_securityzones.csv                    %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_securityzones.csv

# V-1.1
install -m 0664 %{_topdir}/../ADGSConf/csv/t_knownhosts_Add-V1.1.csv              %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_knownhosts_Add-V1.1.csv

# V-1.3
install -m 0664 %{_topdir}/../ADGSConf/csv/t_knownhosts_Add-V1.3.csv              %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_knownhosts_Add-V1.3.csv

# V-1.6
install -m 0664 %{_topdir}/../ADGSConf/csv/t_knownhosts_Add-V1.6.csv              %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_knownhosts_Add-V1.6.csv

# X TLDaemon
install -m 0664 %{_topdir}/../ADGSConf/csv/t_ordersgeneration.csv                 %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_ordersgeneration.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_algorithm.csv                        %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_algorithm.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_processors.csv                       %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_processors.csv

# V-1.4
install -m 0664 %{_topdir}/../ADGSConf/csv/t_ordersgeneration_Add-V1.4.csv        %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_ordersgeneration_Add-V1.4.csv

# X Import/export
install -m 0664 %{_topdir}/../ADGSConf/csv/t_datastoragetypes.csv                 %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_datastoragetypes.csv

# Task Tables
# V-1.0
install -m 0664 %{_topdir}/../ADGSConf/csv/t_smtypeAddOn.csv                      %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_smtypeAddOn.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_storagemanager.csv                   %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_storagemanager.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_tasktables.csv                       %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_tasktables.csv

# V-1.3
install -m 0664 %{_topdir}/../ADGSConf/csv/t_tasktables-V1.3.csv                  %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_tasktables-V1.3.csv

# V-1.4
install -m 0664 %{_topdir}/../ADGSConf/csv/t_tasktables-V1.4.csv                  %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_tasktables-V1.4.csv

# V-1.6
install -m 0664 %{_topdir}/../ADGSConf/csv/t_tasktables-V1.6.csv                  %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_tasktables-V1.6.csv

# Rolling policies
install -m 0664 %{_topdir}/../ADGSConf/csv/t_invrollingpolicies.csv               %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_invrollingpolicies.csv

# Import tables
# V-1.0
install -m 0664 %{_topdir}/../ADGSConf/csv/t_repositories.csv                     %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_repositories.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_receptionrules.csv                   %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_receptionrules.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_distributionrules.csv                %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_distributionrules.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_receptionruleshist.csv               %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_receptionruleshist.csv

# V-1.1
install -m 0664 %{_topdir}/../ADGSConf/csv/t_receptionrules_Add-V1.1.csv          %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_receptionrules_Add-V1.1.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_repositories_Add-V1.1.csv            %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.1.csv

# V-1.2
install -m 0664 %{_topdir}/../ADGSConf/csv/t_receptionrules_Add-V1.2.csv          %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_receptionrules_Add-V1.2.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_repositories_Add-V1.2.csv            %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.2.csv

# V-1.3
install -m 0664 %{_topdir}/../ADGSConf/csv/t_receptionrules_Add-V1.3.csv           %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_receptionrules_Add-V1.3.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_repositories_Add-V1.3.csv             %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.3.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_distributionrules_Add-V1.3.csv        %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_distributionrules_Add-V1.3.csv

# V-1.4
install -m 0664 %{_topdir}/../ADGSConf/csv/t_receptionrules_Add-V1.4.csv           %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_receptionrules_Add-V1.4.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_repositories_Add-V1.4.csv             %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.4.csv

# V-1.5
install -m 0664 %{_topdir}/../ADGSConf/csv/t_receptionrules_Add-V1.5.csv           %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_receptionrules_Add-V1.5.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_repositories_Add-V1.5.csv             %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.5.csv

# V-1.6
install -m 0664 %{_topdir}/../ADGSConf/csv/t_receptionrules_Add-V1.6.csv           %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_receptionrules_Add-V1.6.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_repositories_Add-V1.6.csv             %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.6.csv
install -m 0664 %{_topdir}/../ADGSConf/csv/t_distributionrules_Add-V1.6.csv        %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_distributionrules_Add-V1.6.csv

#SRV Eviction
install -m 0664 %{_topdir}/../ADGSConf/csv/t_srv_filetype_x_eviction.csv           %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_srv_filetype_x_eviction.csv

# V-1.4
install -m 0664 %{_topdir}/../ADGSConf/csv/t_srv_filetype_x_eviction_Add-V1.4.csv  %{buildroot}%{_ConfPath}/DbConfiguration/csv/t_srv_filetype_x_eviction_Add-V1.4.csv

# Updating DB files
# V-1.0
install -m 0664 %{_topdir}/../ADGSConf/query/updateTRepositories.sql               %{buildroot}%{_ConfPath}/DbConfiguration/query/updateTRepositories.sql

# V-1.1
install -m 0664 %{_topdir}/../ADGSConf/query/updateTReceptionRules-V1.1.sql        %{buildroot}%{_ConfPath}/DbConfiguration/query/updateTReceptionRules-V1.1.sql
install -m 0664 %{_topdir}/../ADGSConf/query/updateTRepositories-V1.1.sql          %{buildroot}%{_ConfPath}/DbConfiguration/query/updateTRepositories-V1.1.sql

# V-1.3
install -m 0664 %{_topdir}/../ADGSConf/query/updateTRepositories-V1.3.sql          %{buildroot}%{_ConfPath}/DbConfiguration/query/updateTRepositories-V1.3.sql
install -m 0664 %{_topdir}/../ADGSConf/query/updateTFiletypes-V1.3.sql             %{buildroot}%{_ConfPath}/DbConfiguration/query/updateTFiletypes-V1.3.sql
install -m 0664 %{_topdir}/../ADGSConf/query/updateTFiletypesComps-V1.3.sql        %{buildroot}%{_ConfPath}/DbConfiguration/query/updateTFiletypesComps-V1.3.sql
install -m 0664 %{_topdir}/../ADGSConf/query/updateTReceptionRules-V1.3.sql        %{buildroot}%{_ConfPath}/DbConfiguration/query/updateTReceptionRules-V1.3.sql

# V-1.4
install -m 0664 %{_topdir}/../ADGSConf/query/updateTFiletypes-V1.4.sql             %{buildroot}%{_ConfPath}/DbConfiguration/query/updateTFiletypes-V1.4.sql
install -m 0664 %{_topdir}/../ADGSConf/query/updateTReceptionRules-V1.4.sql        %{buildroot}%{_ConfPath}/DbConfiguration/query/updateTReceptionRules-V1.4.sql
install -m 0664 %{_topdir}/../ADGSConf/query/updateTTaskTable-V1.4.sql             %{buildroot}%{_ConfPath}/DbConfiguration/query/updateTTaskTable-V1.4.sql
install -m 0664 %{_topdir}/../ADGSConf/query/updateTRepositories-V1.4.sql          %{buildroot}%{_ConfPath}/DbConfiguration/query/updateTRepositories-V1.4.sql

# V-1.5
install -m 0664 %{_topdir}/../ADGSConf/query/updateTReceptionRules-V1.5.sql        %{buildroot}%{_ConfPath}/DbConfiguration/query/updateTReceptionRules-V1.5.sql
install -m 0664 %{_topdir}/../ADGSConf/query/updateTRepositories-V1.5.sql          %{buildroot}%{_ConfPath}/DbConfiguration/query/updateTRepositories-V1.5.sql

# V-1.6
install -m 0664 %{_topdir}/../ADGSConf/query/updateTRepositories-V1.6.sql          %{buildroot}%{_ConfPath}/DbConfiguration/query/updateTRepositories-V1.6.sql
# BLOB Configs
install -m 0664 %{_topdir}/../ADGSConf/blobs/StorageManager.xml                    %{buildroot}%{_ConfPath}/DbConfiguration/csv/StorageManager.xml

# TaskTables
install -m 0664 %{_topdir}/../ADGSConf/taskTables/decompressReception.xml            %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/decompressReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/decompressSafeDefaultReception.xml %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/decompressSafeDefaultReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/defaultDelivery.xml                %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/defaultDelivery.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/defaultDistribFormat.xml           %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/defaultDistribFormat.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/eofzipDistribFormat.xml            %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/eofzipDistribFormat.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/dblzipDistribFormat.xml            %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/dblzipDistribFormat.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/xmlzipDistribFormat.xml            %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/xmlzipDistribFormat.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/defaultReception.xml               %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/defaultReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/safeDefaultReceptionZip.xml        %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/safeDefaultReceptionZip.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/ZipEofReception.xml                %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/ZipEofReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/auxtecReception.xml                %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/auxtecReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/auxtroReception.xml                %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/auxtroReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/TGZ2HDRDBLReception.xml            %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/TGZ2HDRDBLReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/nopDeliveryCheck.xml               %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/nopDeliveryCheck.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/ecmwfReception.xml                 %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/ecmwfReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/auxwndReception.xml                %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/auxwndReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/eofFmtReception.xml                %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/eofFmtReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/gippReception.xml                  %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/gippReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/obmemcReception.xml                %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/obmemcReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/osiiceReception.xml                %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/osiiceReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/camsReception00.xml                %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/camsReception00.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/camsReception12.xml                %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/camsReception12.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/CAMSTGZReception.xml               %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/CAMSTGZReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/ECMWFDTGZReception.xml             %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/ECMWFDTGZReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/MissPlanReception.xml              %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/MissPlanReception.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/csvzipDistribFormat.xml            %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/csvzipDistribFormat.xml
install -m 0664 %{_topdir}/../ADGSConf/taskTables/kmlzipDistribFormat.xml            %{buildroot}%{_ConfPath}/DbConfiguration/tasktables/kmlzipDistribFormat.xml

%files
%dir %attr(-,root,root) /usr/local
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/RubyScripts
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts
%dir %attr(-,root,%{_adgsgroup}) %{_ConfPath}
%dir %attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration
%dir %attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv
%dir %attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/query
%dir %attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables

%attr(-,root,%{_adgsgroup}) %{_CompPath}/RubyScripts/LoadDbConfSpace.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/RubyScripts/LoadDbParameters.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts/escPaswdString.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts/exequteSql.sh
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_dbstatus.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_discoveryplugins.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_fileclasses.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_ordertypes.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_originators.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_usertypes.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_rollingpolicyactions.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_bands.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_satrelations.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_originators_Add-V1.4.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_originators_Add-V1.6.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_discoveryplugins_Add-V1.6.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_filetypes.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_filetypescomp.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_users.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_filetypes_Add-V1.1.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_filetypescomp_Add-V1.1.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_filetypes_Add-V1.2.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_filetypescomp_Add-V1.2.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_filetypes_Add-V1.3.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_filetypescomp_Add-V1.3.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_filetypes_Add-V1.4.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_filetypescomp_Add-V1.4.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_filetypes_Add-V1.5.5.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_filetypescomp_Add-V1.5.5.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_filetypes_Add-V1.6.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_filetypescomp_Add-V1.6.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_knownhosts.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_securityzones.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_knownhosts_Add-V1.1.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_knownhosts_Add-V1.3.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_knownhosts_Add-V1.6.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_ordersgeneration.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_algorithm.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_processors.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_ordersgeneration_Add-V1.4.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_datastoragetypes.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_smtypeAddOn.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_storagemanager.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_tasktables.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_tasktables-V1.3.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_tasktables-V1.4.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_tasktables-V1.6.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_invrollingpolicies.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_repositories.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_receptionrules.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_distributionrules.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_receptionruleshist.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_receptionrules_Add-V1.1.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.1.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_receptionrules_Add-V1.2.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.2.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_receptionrules_Add-V1.3.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.3.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_distributionrules_Add-V1.3.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_receptionrules_Add-V1.4.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_receptionrules_Add-V1.5.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.4.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.5.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_receptionrules_Add-V1.6.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.6.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_distributionrules_Add-V1.6.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_srv_filetype_x_eviction.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/t_srv_filetype_x_eviction_Add-V1.4.csv
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/query/updateTRepositories.sql
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/query/updateTReceptionRules-V1.1.sql
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/query/updateTReceptionRules-V1.3.sql
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/query/updateTReceptionRules-V1.4.sql
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/query/updateTReceptionRules-V1.5.sql
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/query/updateTRepositories-V1.1.sql
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/query/updateTRepositories-V1.3.sql
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/query/updateTRepositories-V1.4.sql
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/query/updateTRepositories-V1.5.sql
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/query/updateTRepositories-V1.6.sql
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/query/updateTFiletypes-V1.3.sql
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/query/updateTFiletypes-V1.4.sql
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/query/updateTFiletypesComps-V1.3.sql
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/query/updateTTaskTable-V1.4.sql
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/csv/StorageManager.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/decompressReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/decompressSafeDefaultReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/defaultDelivery.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/defaultDistribFormat.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/eofzipDistribFormat.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/dblzipDistribFormat.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/xmlzipDistribFormat.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/defaultReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/safeDefaultReceptionZip.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/ZipEofReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/auxtecReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/auxtroReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/TGZ2HDRDBLReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/nopDeliveryCheck.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/ecmwfReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/auxwndReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/eofFmtReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/gippReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/obmemcReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/osiiceReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/camsReception00.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/camsReception12.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/CAMSTGZReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/ECMWFDTGZReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/MissPlanReception.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/csvzipDistribFormat.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/DbConfiguration/tasktables/kmlzipDistribFormat.xml

%post

if [ "$1" -eq 1 ]; then

# Database parameters

    sed -i "s/##OBJSMDB_MACHINENAME##/$OBJSMDB_MACHINENAME/g" %{_ConfPath}/DbConfiguration/csv/StorageManager.xml

    %{_CompPath}/RubyScripts/LoadDbParameters.rb -a adgsdb t_smtype               %{_ConfPath}/DbConfiguration/csv/t_smtypeAddOn.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_storagemanager       %{_ConfPath}/DbConfiguration/csv/t_storagemanager.csv

    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_tasktables           %{_ConfPath}/DbConfiguration/csv/t_tasktables.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb -a adgsdb t_discoveryplugins     %{_ConfPath}/DbConfiguration/csv/t_discoveryplugins.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_originators          %{_ConfPath}/DbConfiguration/csv/t_originators.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_fileclasses          %{_ConfPath}/DbConfiguration/csv/t_fileclasses.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_dbstatus             %{_ConfPath}/DbConfiguration/csv/t_dbstatus.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_ordertypes           %{_ConfPath}/DbConfiguration/csv/t_ordertypes.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_usertypes            %{_ConfPath}/DbConfiguration/csv/t_usertypes.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_rollingpolicyactions %{_ConfPath}/DbConfiguration/csv/t_rollingpolicyactions.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_bands                %{_ConfPath}/DbConfiguration/csv/t_bands.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb -a adgsdb t_satrelations         %{_ConfPath}/DbConfiguration/csv/t_satrelations.csv

    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_knownhosts           %{_ConfPath}/DbConfiguration/csv/t_knownhosts.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_filetypes            %{_ConfPath}/DbConfiguration/csv/t_filetypes.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_filetypescomps       %{_ConfPath}/DbConfiguration/csv/t_filetypescomp.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_users                %{_ConfPath}/DbConfiguration/csv/t_users.csv

    ipaddr=$(hostname -I | awk '{print $1}')
    echo "'""$ipaddr"".0/24',1,'Installation node Security Zone'" >> %{_ConfPath}/DbConfiguration/csv/t_securityzones.csv

    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_securityzones        %{_ConfPath}/DbConfiguration/csv/t_securityzones.csv

    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_processors           %{_ConfPath}/DbConfiguration/csv/t_processors.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_algorithm            %{_ConfPath}/DbConfiguration/csv/t_algorithm.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_ordersgeneration     %{_ConfPath}/DbConfiguration/csv/t_ordersgeneration.csv

    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_datastoragetypes     %{_ConfPath}/DbConfiguration/csv/t_datastoragetypes.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_invrollingpolicies   %{_ConfPath}/DbConfiguration/csv/t_invrollingpolicies.csv

    escPwd=$(%{_CompPath}/scripts/escPaswdString.sh $OPADGS_CRYPTPWD)
    sed -i "s/##OPADGS_CRYPT_PWD##/$escPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv
    sed -i "s/##OPADGS_CRYPT_PWD##/$escPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.1.csv
    sed -i "s/##OPADGS_CRYPT_PWD##/$escPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.2.csv

    ecmwfPwd=$(%{_CompPath}/scripts/escPaswdString.sh $ECMWF_ESACDC_PWD)
    sed -i "s/##ECMWF_ESACDC_PWD##/$ecmwfPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    podPwd=$(%{_CompPath}/scripts/escPaswdString.sh $POD_ADGSUSER_PWD)
    sed -i "s/##POD_ADGSUSER_PWD##/$podPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    s2mpcPwd=$(%{_CompPath}/scripts/escPaswdString.sh $S2MPC_CRYPT_PWD)
    sed -i "s/##S2MPC_CRYPT_PWD##/$s2mpcPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    s1mpcPwd=$(%{_CompPath}/scripts/escPaswdString.sh $S1MCPIP_ADGSUSER_PWD)
    sed -i "s/##S1MCPIP_ADGSUSER_PWD##/$s1mpcPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    s1pripPwd=$(%{_CompPath}/scripts/escPaswdString.sh $S1PRIP_TPZUSR_PWD)
    sed -i "s/##S1PRIP_TPZUSR_PWD##/$s1pripPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    s2pripPwd=$(%{_CompPath}/scripts/escPaswdString.sh $S2PRIP_ADGUSR_PWD)
    sed -i "s/##S2PRIP_ADGUSR_PWD##/$s2pripPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    s2mcpipPwd=$(%{_CompPath}/scripts/escPaswdString.sh $S2MCPIP_ADGSUSER_PWD)
    sed -i "s/##S2MCPIP_ADGSUSER_PWD##/$s2mcpipPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    s2camsPwd=$(%{_CompPath}/scripts/escPaswdString.sh $ECMWF_CAMS_PWD)
    sed -i "s/##ECMWF_CAMS_PWD##/$s2camsPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    ops3pdgsPwd=$(%{_CompPath}/scripts/escPaswdString.sh $OPS3PDGS_PWD)
    sed -i "s/##OPS3PDGS_PWD##/$ops3pdgsPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    s1adlrPripPwd=$(%{_CompPath}/scripts/escPaswdString.sh $S1ADLRPRIP_CRYPTPWD)
    sed -i "s/##S1ADLRPRIP_CRYPTPWD##/$s1adlrPripPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv
    sed -i "s/##S1ADLRPRIP_CRYPTPWD##/$s1adlrPripPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.3.csv

    s1bdlrPripPwd=$(%{_CompPath}/scripts/escPaswdString.sh $S1BDLRPRIP_CRYPTPWD)
    sed -i "s/##S1BDLRPRIP_CRYPTPWD##/$s1bdlrPripPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv
    sed -i "s/##S1BDLRPRIP_CRYPTPWD##/$s1bdlrPripPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.3.csv

    s1aSercoPwd=$(%{_CompPath}/scripts/escPaswdString.sh $S1PRIP_SERCO_PWD)
    sed -i "s/##S1PRIP_SERCO_PWD##/$s1aSercoPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    mpipPwd=$(%{_CompPath}/scripts/escPaswdString.sh $MPIP_ADGSUSER_PWD)
    sed -i "s/##MPIP_ADGSUSER_PWD##/$mpipPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv
    sed -i "s/##MPIP_ADGSUSER_PWD##/$mpipPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.6.csv

    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_repositories         %{_ConfPath}/DbConfiguration/csv/t_repositories.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_receptionrules       %{_ConfPath}/DbConfiguration/csv/t_receptionrules.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb    adgsdb t_distributionrules    %{_ConfPath}/DbConfiguration/csv/t_distributionrules.csv

    %{_CompPath}/scripts/exequteSql.sh localhost %{_ConfPath}/DbConfiguration/query/updateTRepositories.sql adgsdb
fi 

if [ "$1" -eq 2 ]; then
    sed -i "s/##OBJSMDB_MACHINENAME##/$OBJSMDB_MACHINENAME/g" %{_ConfPath}/DbConfiguration/csv/StorageManager.xml

    ipaddr=$(hostname -I | awk '{print $1}')
    echo "'""$ipaddr"".0/24',1,'Installation node Security Zone'" >> %{_ConfPath}/DbConfiguration/csv/t_securityzones.csv

    escPwd=$(%{_CompPath}/scripts/escPaswdString.sh $OPADGS_CRYPTPWD)
    sed -i "s/##OPADGS_CRYPT_PWD##/$escPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv
    sed -i "s/##OPADGS_CRYPT_PWD##/$escPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.1.csv
    sed -i "s/##OPADGS_CRYPT_PWD##/$escPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.2.csv
    sed -i "s/##OPADGS_CRYPT_PWD##/$escPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.4.csv
    sed -i "s/##OPADGS_CRYPT_PWD##/$escPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.5.csv
    sed -i "s/##OPADGS_CRYPT_PWD##/$escPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.6.csv

    ecmwfPwd=$(%{_CompPath}/scripts/escPaswdString.sh $ECMWF_ESACDC_PWD)
    sed -i "s/##ECMWF_ESACDC_PWD##/$ecmwfPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    podPwd=$(%{_CompPath}/scripts/escPaswdString.sh $POD_ADGSUSER_PWD)
    sed -i "s/##POD_ADGSUSER_PWD##/$podPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    s2mpcPwd=$(%{_CompPath}/scripts/escPaswdString.sh $S2MPC_CRYPT_PWD)
    sed -i "s/##S2MPC_CRYPT_PWD##/$s2mpcPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    s1mpcPwd=$(%{_CompPath}/scripts/escPaswdString.sh $S1MCPIP_ADGSUSER_PWD)
    sed -i "s/##S1MCPIP_ADGSUSER_PWD##/$s1mpcPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    s1pripPwd=$(%{_CompPath}/scripts/escPaswdString.sh $S1PRIP_TPZUSR_PWD)
    sed -i "s/##S1PRIP_TPZUSR_PWD##/$s1pripPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    s2pripPwd=$(%{_CompPath}/scripts/escPaswdString.sh $S2PRIP_ADGUSR_PWD)
    sed -i "s/##S2PRIP_ADGUSR_PWD##/$s2pripPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    s2mcpipPwd=$(%{_CompPath}/scripts/escPaswdString.sh $S2MCPIP_ADGSUSER_PWD)
    sed -i "s/##S2MCPIP_ADGSUSER_PWD##/$s2mcpipPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    s2camsPwd=$(%{_CompPath}/scripts/escPaswdString.sh $ECMWF_CAMS_PWD)
    sed -i "s/##ECMWF_CAMS_PWD##/$s2camsPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    ops3pdgsPwd=$(%{_CompPath}/scripts/escPaswdString.sh $OPS3PDGS_PWD)
    sed -i "s/##OPS3PDGS_PWD##/$ops3pdgsPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    s1adlrPripPwd=$(%{_CompPath}/scripts/escPaswdString.sh $S1ADLRPRIP_CRYPTPWD)
    sed -i "s/##S1ADLRPRIP_CRYPTPWD##/$s1adlrPripPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv
    sed -i "s/##S1ADLRPRIP_CRYPTPWD##/$s1adlrPripPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.3.csv

    s1bdlrPripPwd=$(%{_CompPath}/scripts/escPaswdString.sh $S1BDLRPRIP_CRYPTPWD)
    sed -i "s/##S1BDLRPRIP_CRYPTPWD##/$s1bdlrPripPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv
    sed -i "s/##S1BDLRPRIP_CRYPTPWD##/$s1bdlrPripPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.3.csv

    s1aSercoPwd=$(%{_CompPath}/scripts/escPaswdString.sh $S1PRIP_SERCO_PWD)
    sed -i "s/##S1PRIP_SERCO_PWD##/$s1aSercoPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv

    mpipPwd=$(%{_CompPath}/scripts/escPaswdString.sh $MPIP_ADGSUSER_PWD)
    sed -i "s/##MPIP_ADGSUSER_PWD##/$mpipPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories.csv
    sed -i "s/##MPIP_ADGSUSER_PWD##/$mpipPwd/g" %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.6.csv

    %{_CompPath}/RubyScripts/LoadDbParameters.rb -a adgsdb t_satrelations                 %{_ConfPath}/DbConfiguration/csv/t_satrelations.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb -a adgsdb t_discoveryplugins             %{_ConfPath}/DbConfiguration/csv/t_discoveryplugins_Add-V1.6.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb -a adgsdb t_knownhosts                   %{_ConfPath}/DbConfiguration/csv/t_knownhosts_Add-V1.6.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb -a adgsdb t_originators                  %{_ConfPath}/DbConfiguration/csv/t_originators_Add-V1.6.csv
	%{_CompPath}/RubyScripts/LoadDbParameters.rb -a adgsdb t_tasktables                   %{_ConfPath}/DbConfiguration/csv/t_tasktables-V1.6.csv

    %{_CompPath}/RubyScripts/LoadDbParameters.rb -a adgsdb t_filetypes                    %{_ConfPath}/DbConfiguration/csv/t_filetypes_Add-V1.6.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb -a adgsdb t_filetypescomps               %{_ConfPath}/DbConfiguration/csv/t_filetypescomp_Add-V1.6.csv
	
    %{_CompPath}/RubyScripts/LoadDbParameters.rb -a adgsdb t_repositories                 %{_ConfPath}/DbConfiguration/csv/t_repositories_Add-V1.6.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb -a adgsdb t_receptionrules               %{_ConfPath}/DbConfiguration/csv/t_receptionrules_Add-V1.6.csv
    %{_CompPath}/RubyScripts/LoadDbParameters.rb -a adgsdb t_distributionrules            %{_ConfPath}/DbConfiguration/csv/t_distributionrules_Add-V1.6.csv

    %{_CompPath}/scripts/exequteSql.sh localhost %{_ConfPath}/DbConfiguration/query/updateTRepositories-V1.6.sql adgsdb

fi 

%clean
rm -rf %{buildroot} %{rpmPkgName}
