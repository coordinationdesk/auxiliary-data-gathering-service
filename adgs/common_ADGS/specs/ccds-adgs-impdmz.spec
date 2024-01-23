#########################################################################
%define _ConfPath    /usr/local/conf/ADGS
%define _CompPath    /usr/local/components/ADGS
%define _InitPath    /usr/local/components/ADGS/initScripts
%define _DataPath    /data/ADGS
%define _StorePath   /data/ADGS
%define _ElementName ccds-adgs-impdmz
%define _Summary     The ADGS DMZ Machine services
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
Requires:    ccds-adgs-shared-libraries
Provides:    %{_ElementName} = %{_Version}

%description
%{_Summary}

#%prep

%install

%__rm -fr %{buildroot}
%__mkdir -p %{buildroot}

cd %{buildroot}

umask 022
mkdir -p %{buildroot}/etc/sudoers.d
mkdir -p %{buildroot}%{_ConfPath}

umask 002
mkdir -p %{buildroot}%{_CompPath}
mkdir -p %{buildroot}%{_CompPath}/bin
mkdir -p %{buildroot}%{_CompPath}/lib
mkdir -p %{buildroot}%{_CompPath}/scripts
mkdir -p %{buildroot}%{_CompPath}/conf
mkdir -p %{buildroot}%{_InitPath}
mkdir -p %{buildroot}%{_ConfPath}/confSpaces
mkdir -p %{buildroot}%{_ConfPath}/database
mkdir -p %{buildroot}/data
mkdir -p %{buildroot}/data/fromEUM
mkdir -p %{buildroot}/data/fromMRN
mkdir -p %{buildroot}/data/fromNASA
mkdir -p %{buildroot}/data/fromS1FOS
mkdir -p %{buildroot}/data/fromS2FOS
mkdir -p %{buildroot}/data/fromS2MCP
mkdir -p %{buildroot}/data/fromFOSMail
mkdir -p %{buildroot}/data/tmp
mkdir -p %{buildroot}%{_DataPath}
mkdir -p %{buildroot}%{_DataPath}/conf
mkdir -p %{buildroot}%{_DataPath}/conf/database
mkdir -p %{buildroot}%{_DataPath}/log
mkdir -p %{buildroot}%{_DataPath}/tmp
mkdir -p %{buildroot}%{_DataPath}/TmpExp
mkdir -p %{buildroot}%{_DataPath}/Export

# init scripts

install -m 0550 %{_topdir}/../initScripts/systemd/ADGS-ImpDmz-control                           %{buildroot}%{_InitPath}/ADGS-ImpDmz-control
install -m 0550 %{_topdir}/../initScripts/systemd/ADGS_sudo                                     %{buildroot}%{_InitPath}/ADGS_sudo
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-purgelog.service                         %{buildroot}%{_InitPath}/adgs-purgelog.service
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-purgelog.timer                           %{buildroot}%{_InitPath}/adgs-purgelog.timer
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-adfHandler.service                       %{buildroot}%{_InitPath}/adgs-adfHandler.service
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-adfHandler.timer                         %{buildroot}%{_InitPath}/adgs-adfHandler.timer
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-jiraDownloader.service                   %{buildroot}%{_InitPath}/adgs-jiraDownloader.service
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-jiraDownloader.timer                     %{buildroot}%{_InitPath}/adgs-jiraDownloader.timer
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-purgeeumfiles.service                    %{buildroot}%{_InitPath}/adgs-purgeeumfiles.service
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-purgeeumfiles.timer                      %{buildroot}%{_InitPath}/adgs-purgeeumfiles.timer
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-purgefosmailfiles.service                %{buildroot}%{_InitPath}/adgs-purgefosmailfiles.service
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-purgefosmailfiles.timer                  %{buildroot}%{_InitPath}/adgs-purgefosmailfiles.timer
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-catalogueexportgenerator.service         %{buildroot}%{_InitPath}/adgs-catalogueexportgenerator.service
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-catalogueexportgenerator.timer           %{buildroot}%{_InitPath}/adgs-catalogueexportgenerator.timer

# applications

install -m 0755 %{_topdir}/../applications/Encrypter/Encrypter                                  %{buildroot}%{_CompPath}/bin/Encrypter
install -m 0755 %{_topdir}/../components/libACS_SSL/test/RemoteConnectionsKey_Decode            %{buildroot}%{_CompPath}/bin/RemoteConnectionsKey_Decode
install -m 0755 %{_topdir}/../applications/Decrypter/Decrypter                                  %{buildroot}%{_CompPath}/bin/Decrypter

install -m 0775 %{_topdir}/../scripts/Utility/Decrypter.sh                                      %{buildroot}%{_CompPath}/bin/Decrypter.sh
install -m 0775 %{_topdir}/../scripts/Utility/Encrypter.sh                                      %{buildroot}%{_CompPath}/bin/Encrypter.sh
install -m 0755 %{_topdir}/../scripts/Utility/purgeLogFiles.sh                                  %{buildroot}%{_CompPath}/scripts/purgeLogFiles.sh
install -m 0755 %{_topdir}/../scripts/Utility/purgeEumFiles.sh                                  %{buildroot}%{_CompPath}/scripts/purgeEumFiles.sh
install -m 0755 %{_topdir}/../scripts/Utility/purgeFOSMailFiles.sh                              %{buildroot}%{_CompPath}/scripts/purgeFOSMailFiles.sh

install -m 0755 %{_topdir}/../scripts/AdfHandler/adf_handler.sh                                 %{buildroot}%{_CompPath}/scripts/adf_handler.sh
install -m 0755 %{_topdir}/../scripts/AdfHandler/adf_handler_wrapper.sh                         %{buildroot}%{_CompPath}/scripts/adf_handler_wrapper.sh
install -m 0755 %{_topdir}/../scripts/AdfHandler/sysutils                                       %{buildroot}%{_CompPath}/scripts/sysutils
install -m 0755 %{_topdir}/../scripts/AdfHandler/timeutils                                      %{buildroot}%{_CompPath}/scripts/timeutils

install -m 0755 %{_topdir}/../applications/adgs_jiraissuedownloader/ADGS_JiraIssueDownloader.py           %{buildroot}%{_CompPath}/bin/ADGS_JiraIssueDownloader.py
install -m 0755 %{_topdir}/../applications/ADGSCatalogueExport/adgs_catalogue_export.rb                   %{buildroot}%{_CompPath}/bin/adgs_catalogue_export.rb
install -m 0755 %{_topdir}/../components/RubyLibs/common_utils.rb								          %{buildroot}%{_CompPath}/lib/common_utils.rb
install -m 0755 %{_topdir}/../components/RubyLibs/job_order_handler.rb							          %{buildroot}%{_CompPath}/lib/job_order_handler.rb
install -m 0755 %{_topdir}/../components/RubyLibs/configuration_handler.rb						          %{buildroot}%{_CompPath}/lib/configuration_handler.rb
install -m 0755 %{_topdir}/../components/RubyLibs/db_handler.rb									          %{buildroot}%{_CompPath}/lib/db_handler.rb
install -m 0755 %{_topdir}/../components/RubyLibs/mtd_handler.rb								          %{buildroot}%{_CompPath}/lib/mtd_handler.rb
install -m 0755 %{_topdir}/../components/RubyLibs/rotating_logger.rb							          %{buildroot}%{_CompPath}/lib/rotating_logger.rb
install -m 0755 %{_topdir}/../components/RubyLibs/splunk_handler.rb								          %{buildroot}%{_CompPath}/lib/splunk_handler.rb

# confspaces

install -m 0664 %{_topdir}/../ADGSConf/confSpaces/MACHINE-NAME.xml                     %{buildroot}%{_ConfPath}/confSpaces/MACHINE-NAME.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/JiraDownloaderSettings.yaml          %{buildroot}%{_ConfPath}/confSpaces/JiraDownloaderSettings.yaml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/adgs_catalogue_export.conf           %{buildroot}%{_CompPath}/conf/adgs_catalogue_export.conf

# DB Conf
install -m 0644 %{_topdir}/../dbconf/dbsrv.conf                                        %{buildroot}%{_ConfPath}/database/db.conf
install -m 0644 %{_topdir}/../dbconf/smplugins.conf                                    %{buildroot}%{_ConfPath}/database/smplugins.conf

ln -s %{_ConfPath}/database/db.conf        .%{_DataPath}/conf/database/db.conf
ln -s %{_ConfPath}/database/smplugins.conf .%{_DataPath}/conf/database/smplugins.conf

%files
%dir %attr(-,root,root) /usr/local
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/bin
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/lib
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/conf
%dir %attr(-,root,%{_adgsgroup}) %{_ConfPath}
%dir %attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces
%dir %attr(-,root,%{_adgsgroup}) %{_ConfPath}/database
%dir %attr(-,root,%{_adgsgroup}) /data/fromEUM
%dir %attr(-,root,%{_adgsgroup}) /data/fromMRN
%dir %attr(-,root,%{_adgsgroup}) /data/fromNASA
%dir %attr(-,root,%{_adgsgroup}) /data/fromS1FOS
%dir %attr(-,root,%{_adgsgroup}) /data/fromS2FOS
%dir %attr(-,root,%{_adgsgroup}) /data/fromS2MCP
%dir %attr(-,root,%{_adgsgroup}) /data/fromFOSMail
%dir %attr(-,root,%{_adgsgroup}) /data/tmp
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/conf
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/conf/database
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/log
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/tmp
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/TmpExp
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/Export

%attr(-,root,%{_adgsgroup}) %{_InitPath}/ADGS-ImpDmz-control
%attr(-,root,%{_adgsgroup}) %{_InitPath}/ADGS_sudo
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-purgelog.service
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-purgelog.timer
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-adfHandler.service
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-adfHandler.timer
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-jiraDownloader.service
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-jiraDownloader.timer
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-purgeeumfiles.service
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-purgeeumfiles.timer
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-purgefosmailfiles.service
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-purgefosmailfiles.timer
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-catalogueexportgenerator.service
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-catalogueexportgenerator.timer
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/Encrypter
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/RemoteConnectionsKey_Decode
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/Decrypter
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/Decrypter.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/Encrypter.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/ADGS_JiraIssueDownloader.py
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/adgs_catalogue_export.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/common_utils.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/job_order_handler.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/configuration_handler.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/db_handler.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/mtd_handler.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/rotating_logger.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/splunk_handler.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/conf/adgs_catalogue_export.conf
%attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts/purgeFOSMailFiles.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts/purgeLogFiles.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts/purgeEumFiles.sh
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/MACHINE-NAME.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/JiraDownloaderSettings.yaml
%attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts/adf_handler.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts/adf_handler_wrapper.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts/sysutils
%attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts/timeutils
%attr(-,root,root) %{_ConfPath}/database/db.conf
%attr(-,root,root) %{_ConfPath}/database/smplugins.conf
%{_DataPath}/conf/database/db.conf
%{_DataPath}/conf/database/smplugins.conf

%post

systemctl daemon-reload

if [ "$1" -eq 1 ]; then

    sed -i "s/##ADGSDB_MACHINENAME##/$ADGSDB_MACHINENAME/g" %{_ConfPath}/database/db.conf
    sed -i "s/##SRVUSERDB_CRYPTPWD##/$SRVUSERDB_CRYPTPWD/g" %{_ConfPath}/database/db.conf

    sed -i "s/##SRVDB_BASEURL##/$SRVDB_BASEURL/g"           %{_CompPath}/conf/adgs_catalogue_export.conf
    sed -i "s/##SRVDB_BASICAUTH##/$SRVDB_BASICAUTH/g"       %{_CompPath}/conf/adgs_catalogue_export.conf

    export LD_LIBRARY_PATH=%{_CompPath}/lib

    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/MACHINE-NAME.xml %{_DataPath}/conf/$(hostname).xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/JiraDownloaderSettings.yaml %{_DataPath}/conf/JiraDownloaderSettings.yaml"

    rowexist=$(psql -X -A -h $ADGSDB_MACHINENAME -U acsdba -d adgsdb -t -c "select count (*) from t_knownhosts where host_name = '$(hostname)'")
    if [[ $rowexist -ne 1 ]]
    then
        psql -h $ADGSDB_MACHINENAME -U acsdba adgsdb -c "insert into t_knownhosts (host_name) values ('$(hostname)')"
    fi

    cp %{_InitPath}/ADGS_sudo /etc/sudoers.d/ADGS_sudo
    chmod 550 /etc/sudoers.d/ADGS_sudo

    chmod 777 /data/fromEUM
    chmod 777 /data/fromMRN

    # link the systemd Daemon Services/Timer
    services=( "adgs-purgefosmailfiles.service" "adgs-purgefosmailfiles.timer" "adgs-purgelog.service" "adgs-purgelog.timer" "adgs-purgeeumfiles.service" "adgs-purgeeumfiles.timer" "adgs-adfHandler.service" "adgs-adfHandler.timer" "adgs-jiraDownloader.service" "adgs-jiraDownloader.timer" "adgs-catalogueexportgenerator.service" "adgs-catalogueexportgenerator.timer" )

    for i in "${services[@]}"
    do
        if [ ! -h /etc/systemd/system/$i ]; then
            systemctl enable   %{_InitPath}/$i
            systemctl daemon-reload
        fi
    done

    systemctl daemon-reload
    systemctl start adgs-purgefosmailfiles.timer
    systemctl start adgs-purgelog.timer
    systemctl start adgs-purgeeumfiles.timer
    systemctl start adgs-adfHandler.timer
    systemctl start adgs-jiraDownloader.timer
    systemctl start adgs-catalogueexportgenerator.timer
fi

if [ "$1" -eq 2 ]; then

    sed -i "s/##ADGSDB_MACHINENAME##/$ADGSDB_MACHINENAME/g" %{_ConfPath}/database/db.conf
    sed -i "s/##SRVUSERDB_CRYPTPWD##/$SRVUSERDB_CRYPTPWD/g" %{_ConfPath}/database/db.conf

    sed -i "s/##SRVDB_BASEURL##/$SRVDB_BASEURL/g"           %{_CompPath}/conf/adgs_catalogue_export.conf
    sed -i "s/##SRVDB_BASICAUTH##/$SRVDB_BASICAUTH/g"       %{_CompPath}/conf/adgs_catalogue_export.conf

    # link the systemd Daemon Services/Timer
    services=( "adgs-purgefosmailfiles.service" "adgs-purgefosmailfiles.timer" "adgs-purgelog.service" "adgs-purgelog.timer" "adgs-purgeeumfiles.service" "adgs-purgeeumfiles.timer" "adgs-adfHandler.service" "adgs-adfHandler.timer" "adgs-jiraDownloader.service" "adgs-jiraDownloader.timer" "adgs-catalogueexportgenerator.service" "adgs-catalogueexportgenerator.timer" )

    for i in "${services[@]}"
    do
        if [ ! -h /etc/systemd/system/$i ]; then
            systemctl enable   %{_InitPath}/$i
            systemctl daemon-reload
        fi
    done

    systemctl daemon-reload
    systemctl start adgs-purgefosmailfiles.timer
    systemctl start adgs-purgelog.timer
    systemctl start adgs-purgeeumfiles.timer
    systemctl start adgs-adfHandler.timer
    systemctl start adgs-jiraDownloader.timer
    systemctl start adgs-catalogueexportgenerator.timer

fi

%preun
if [ "$1" -eq 0 ]; then

    systemctl stop adgs-catalogueexportgenerator.timer
    systemctl stop adgs-jiraDownloader.timer
    systemctl stop adgs-purgelog.timer
    systemctl stop adgs-adfHandler.timer
    systemctl stop adgs-purgeeumfiles.timer
    systemctl disable adgs-purgelog.timer
    systemctl disable adgs-purgelog.service
    systemctl disable adgs-purgeeumfiles.timer
    systemctl disable adgs-purgeeumfiles.service
    systemctl disable adgs-adfHandler.timer
    systemctl disable adgs-adfHandler.service
    systemctl disable adgs-jiraDownloader.timer
    systemctl disable adgs-jiraDownloader.service
    systemctl disable adgs-catalogueexportgenerator.timer
    systemctl disable adgs-catalogueexportgenerator.service
    systemctl daemon-reload

    rm /etc/sudoers.d/ADGS_sudo

    rm -f %{_DataPath}/conf/$(hostname).xml
	rm -f %{_DataPath}/conf/JiraDownloaderSettings.yaml
fi

%clean
rm -rf %{buildroot} %{rpmPkgName}
