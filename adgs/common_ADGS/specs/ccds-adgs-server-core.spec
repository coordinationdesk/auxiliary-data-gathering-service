#########################################################################
%define _ConfPath    /usr/local/conf/ADGS
%define _CompPath    /usr/local/components/ADGS
%define _InitPath    /usr/local/components/ADGS/initScripts
%define _DataPath    /data/ADGS
%define _StorePath   /data/ADGS
%define _ElementName ccds-adgs-server-core
%define _Summary     The ADGS Server Machine services
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
Requires:    postgresql13-server, postgresql13-contrib, ccds-adgs-db-cfg, ccds-adgs-dbclient, ccds-adgs-shared-libraries
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
mkdir -p %{buildroot}%{_ConfPath}

umask 002
mkdir -p %{buildroot}%{_CompPath}
mkdir -p %{buildroot}%{_CompPath}/bin
mkdir -p %{buildroot}%{_CompPath}/scripts
mkdir -p %{buildroot}%{_ConfPath}/confSpaces
mkdir -p %{buildroot}%{_InitPath}
mkdir -p %{buildroot}%{_DataPath}
mkdir -p %{buildroot}%{_DataPath}/log
mkdir -p %{buildroot}%{_DataPath}/debug
mkdir -p %{buildroot}%{_DataPath}/conf

# init scripts

install -m 0550 %{_topdir}/../initScripts/systemd/adgs-server.service                %{buildroot}%{_InitPath}/adgs-server.service
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-distributor-generator.service %{buildroot}%{_InitPath}/adgs-distributor-generator.service
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-order-generator.service       %{buildroot}%{_InitPath}/adgs-order-generator.service
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-packager-optimiser.service    %{buildroot}%{_InitPath}/adgs-packager-optimiser.service
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-rolling-archive.service       %{buildroot}%{_InitPath}/adgs-rolling-archive.service
install -m 0550 %{_topdir}/../initScripts/systemd/ADGS-Server-control                %{buildroot}%{_InitPath}/ADGS-Server-control
install -m 0550 %{_topdir}/../initScripts/systemd/ADGS_sudo                          %{buildroot}%{_InitPath}/ADGS_sudo
install -m 0550 %{_topdir}/../initScripts/systemd/SendVersion.sh                     %{buildroot}%{_CompPath}/scripts/SendVersion.sh

# Added in V-1.1
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-purgelog.service              %{buildroot}%{_InitPath}/adgs-purgelog.service
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-purgelog.timer                %{buildroot}%{_InitPath}/adgs-purgelog.timer

# applications
install -m 0755 %{_topdir}/../applications/DistributorGenerator/DistributorGenerator %{buildroot}%{_CompPath}/bin/DistributorGenerator
install -m 0755 %{_topdir}/../applications/PackagerOptimiser/PackagerOptimiser       %{buildroot}%{_CompPath}/bin/PackagerOptimiser
install -m 0755 %{_topdir}/../applications/Encrypter/Encrypter                       %{buildroot}%{_CompPath}/bin/Encrypter
install -m 0755 %{_topdir}/../components/libACS_SSL/test/RemoteConnectionsKey_Decode %{buildroot}%{_CompPath}/bin/RemoteConnectionsKey_Decode
install -m 0755 %{_topdir}/../applications/OrderGenerator/OrderGenerator             %{buildroot}%{_CompPath}/bin/OrderGenerator
install -m 0755 %{_topdir}/../applications/RollingArchive/RollingArchive             %{buildroot}%{_CompPath}/bin/RollingArchive

#Utility SCRIPTS
install -m 0755 %{_topdir}/../scripts/Utility/Encrypter.sh                           %{buildroot}%{_CompPath}/bin/Encrypter.sh
install -m 0755 %{_topdir}/../scripts/Utility/Decrypter.sh                           %{buildroot}%{_CompPath}/bin/Decrypter.sh
# Added in V-1.1
install -m 0755 %{_topdir}/../scripts/Utility/purgeLogFiles.sh                       %{buildroot}%{_CompPath}/scripts/purgeLogFiles.sh

#Configuration Space
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/MACHINE-NAME.xml                   %{buildroot}%{_ConfPath}/confSpaces/MACHINE-NAME.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/dbBroadcast.xml                    %{buildroot}%{_ConfPath}/confSpaces/dbBroadcast.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/DistributionGeneration.xml         %{buildroot}%{_ConfPath}/confSpaces/DistributionGeneration.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/PackagerOptimiser.xml              %{buildroot}%{_ConfPath}/confSpaces/PackagerOptimiser.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/ExplorerWrapper.xml                %{buildroot}%{_ConfPath}/confSpaces/ExplorerWrapper.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/libFtpCurl.so.xml                  %{buildroot}%{_ConfPath}/confSpaces/libFtpCurl.so.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/libGenFileAction.xml               %{buildroot}%{_ConfPath}/confSpaces/libGenFileAction.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/OrdersGeneration.xml               %{buildroot}%{_ConfPath}/confSpaces/OrdersGeneration.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/RollingArchive.xml                 %{buildroot}%{_ConfPath}/confSpaces/RollingArchive.xml
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/StationConf.xml                    %{buildroot}%{_ConfPath}/confSpaces/StationConf.xml

%files
%dir %attr(-,root,root) /usr/local

%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/bin
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/log
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/debug
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/conf

%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-server.service
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-distributor-generator.service
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-packager-optimiser.service
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-order-generator.service
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-rolling-archive.service
%attr(-,root,%{_adgsgroup}) %{_InitPath}/ADGS-Server-control
%attr(-,root,%{_adgsgroup}) %{_InitPath}/ADGS_sudo
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-purgelog.service
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-purgelog.timer
%attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts/SendVersion.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/DistributorGenerator
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/PackagerOptimiser
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/Encrypter
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/RemoteConnectionsKey_Decode
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/OrderGenerator
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/RollingArchive
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/Encrypter.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/Decrypter.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts/purgeLogFiles.sh
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/MACHINE-NAME.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/dbBroadcast.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/DistributionGeneration.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/PackagerOptimiser.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/ExplorerWrapper.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/libFtpCurl.so.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/libGenFileAction.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/OrdersGeneration.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/RollingArchive.xml
%attr(-,root,%{_adgsgroup}) %{_ConfPath}/confSpaces/StationConf.xml

%post

systemctl daemon-reload

if [ "$1" -eq 1 ]; then

    cp %{_InitPath}/ADGS_sudo /etc/sudoers.d/ADGS_sudo
    chmod 550 /etc/sudoers.d/ADGS_sudo

    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/MACHINE-NAME.xml           %{_DataPath}/conf/$(hostname).xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/dbBroadcast.xml            %{_DataPath}/conf/dbBroadcast.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/DistributionGeneration.xml %{_DataPath}/conf/DistributionGeneration.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/ExplorerWrapper.xml        %{_DataPath}/conf/ExplorerWrapper.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/libFtpCurl.so.xml          %{_DataPath}/conf/libFtpCurl.so.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/libGenFileAction.xml       %{_DataPath}/conf/libGenFileAction.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/OrdersGeneration.xml       %{_DataPath}/conf/OrdersGeneration.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/PackagerOptimiser.xml      %{_DataPath}/conf/PackagerOptimiser.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/RollingArchive.xml         %{_DataPath}/conf/RollingArchive.xml"
    su - %{_adgsuser} -c "cp %{_ConfPath}/confSpaces/StationConf.xml            %{_DataPath}/conf/StationConf.xml"

    # link the systemd Daemon Services/Timer
    services=( "adgs-server.service" "adgs-packager-optimiser.service" "adgs-distributor-generator.service" "adgs-order-generator.service" "adgs-rolling-archive.service" "adgs-purgelog.service" "adgs-purgelog.timer" )

    for i in "${services[@]}"
    do
        if [ ! -h /etc/systemd/system/$i ]; then
            systemctl enable   %{_InitPath}/$i
            systemctl daemon-reload
        fi
    done

    systemctl start adgs-purgelog.timer

    # Reload for new installations 

    systemctl restart crond
fi

if [ "$1" -eq 2 ]; then

    systemctl daemon-reload
    systemctl start adgs-purgelog.timer

fi

%preun
if [ "$1" -eq 0 ]; then
    rm /etc/sudoers.d/ADGS_sudo

    rm -f %{_DataPath}/conf/$(hostname).xml
    rm -f %{_DataPath}/conf/dbBroadcast.xml
    rm -f %{_DataPath}/conf/DistributionGeneration.xml
    rm -f %{_DataPath}/conf/ExplorerWrapper.xml
    rm -f %{_DataPath}/conf/libFtpCurl.so.xml
    rm -f %{_DataPath}/conf/libGenFileAction.xml
    rm -f %{_DataPath}/conf/OrdersGeneration.xml
    rm -f %{_DataPath}/conf/PackagerOptimiser.xml
    rm -f %{_DataPath}/conf/RollingArchive.xml
    rm -f %{_DataPath}/conf/StationConf.xml

    systemctl stop adgs-purgelog.timer

    services=( "adgs-server.service" "adgs-packager-optimiser.service" "adgs-distributor-generator.service" "adgs-order-generator.service" "adgs-rolling-archive.service" "adgs-purgelog.timer" "adgs-purgelog.service" )

    for i in "${services[@]}"
    do
        if [ ! -h /etc/systemd/system/$i ]; then
            systemctl disable   %{_InitPath}/$i
        fi
    done

    systemctl daemon-reload
fi

%clean
rm -rf %{buildroot} %{rpmPkgName}
