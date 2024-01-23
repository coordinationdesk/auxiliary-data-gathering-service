#########################################################################
%define _ConfPath    /usr/local/conf/ADGS
%define _CompPath    /usr/local/components/ADGS
%define _InitPath    /usr/local/ADGS
%define _DataPath    /data/ADGS
%define _StorePath   /data/ADGS
%define _ElementName ccds-adgs-baselinereport
%define _Summary     The ADGS Baseline Report Generator
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
Requires:    postgresql13-libs
Provides:    %{_ElementName} = %{_Version}

%description
%{_Summary}

#%prep

%install

%__rm -fr %{buildroot}
%__mkdir -p %{buildroot}

cd %{buildroot}

umask 002
mkdir -p %{buildroot}%{_CompPath}
mkdir -p %{buildroot}%{_CompPath}/bin
mkdir -p %{buildroot}%{_CompPath}/lib
mkdir -p %{buildroot}%{_CompPath}/scripts
mkdir -p %{buildroot}%{_CompPath}/conf
mkdir -p %{buildroot}%{_InitPath}
mkdir -p %{buildroot}%{_ConfPath}
mkdir -p %{buildroot}%{_ConfPath}/database
mkdir -p %{buildroot}%{_DataPath}
mkdir -p %{buildroot}%{_DataPath}/conf
mkdir -p %{buildroot}%{_DataPath}/conf/database
mkdir -p %{buildroot}%{_DataPath}/log


###### Install needed libraries
libs="ADGS_RemoteConnections_PrivateKeys ADGS_RemoteConnections_PublicKeys libException libRegExp libStringUtils libResources libTimer libThread libSignal libCRC libACS_SSL"
for lib in $libs
do
	cp $(ls %{_topdir}/../shared/${lib}* | egrep '.*\.so(\.[0-9]+){3}') %{buildroot}%{_CompPath}/lib
done

/usr/sbin/ldconfig -N -n %{buildroot}%{install_path}/lib

###### Install systemctl files
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-baselinereportgenerator.service          %{buildroot}%{_InitPath}/adgs-baselinereportgenerator.service
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-baselinereportgenerator.timer            %{buildroot}%{_InitPath}/adgs-baselinereportgenerator.timer
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-purgebaselinereport.service              %{buildroot}%{_InitPath}/adgs-purgebaselinereport.service
install -m 0550 %{_topdir}/../initScripts/systemd/adgs-purgebaselinereport.timer                %{buildroot}%{_InitPath}/adgs-purgebaselinereport.timer

###### Install needed applications
install -m 0755 %{_topdir}/../applications/Encrypter/Encrypter                                  %{buildroot}%{_CompPath}/bin/Encrypter
install -m 0755 %{_topdir}/../applications/Decrypter/Decrypter                                  %{buildroot}%{_CompPath}/bin/Decrypter

###### Install needed scripts
install -m 0775 %{_topdir}/../scripts/Utility/Decrypter.sh                                      %{buildroot}%{_CompPath}/bin/Decrypter.sh
install -m 0775 %{_topdir}/../scripts/Utility/Encrypter.sh                                      %{buildroot}%{_CompPath}/bin/Encrypter.sh
install -m 0755 %{_topdir}/../scripts/Utility/purgeBaselineReports.sh                           %{buildroot}%{_CompPath}/scripts/purgeBaselineReports.sh

###### Install Ruby files
install -m 0755 %{_topdir}/../applications/ADGSBaselineReportGenerator/adgs_baseline_report_generator.rb  %{buildroot}%{_CompPath}/bin/adgs_baseline_report_generator.rb
install -m 0755 %{_topdir}/../components/RubyLibs/common_utils.rb								          %{buildroot}%{_CompPath}/lib/common_utils.rb
install -m 0755 %{_topdir}/../components/RubyLibs/job_order_handler.rb							          %{buildroot}%{_CompPath}/lib/job_order_handler.rb
install -m 0755 %{_topdir}/../components/RubyLibs/configuration_handler.rb						          %{buildroot}%{_CompPath}/lib/configuration_handler.rb
install -m 0755 %{_topdir}/../components/RubyLibs/db_handler.rb									          %{buildroot}%{_CompPath}/lib/db_handler.rb
install -m 0755 %{_topdir}/../components/RubyLibs/mtd_handler.rb								          %{buildroot}%{_CompPath}/lib/mtd_handler.rb
install -m 0755 %{_topdir}/../components/RubyLibs/rotating_logger.rb							          %{buildroot}%{_CompPath}/lib/rotating_logger.rb
install -m 0755 %{_topdir}/../components/RubyLibs/splunk_handler.rb								          %{buildroot}%{_CompPath}/lib/splunk_handler.rb

###### Configuration files
install -m 0664 %{_topdir}/../ADGSConf/confSpaces/adgs_baseline_report_generator.conf  %{buildroot}%{_CompPath}/conf/adgs_baseline_report_generator.conf

# DB Conf
install -m 0644 %{_topdir}/../dbconf/dbsrv.conf         %{buildroot}%{_ConfPath}/database/db.conf
install -m 0644 %{_topdir}/../dbconf/smplugins.conf     %{buildroot}%{_ConfPath}/database/smplugins.conf

ln -s %{_ConfPath}/database/db.conf        .%{_DataPath}/conf/database/db.conf
ln -s %{_ConfPath}/database/smplugins.conf .%{_DataPath}/conf/database/smplugins.conf

%files
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/bin
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/lib
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/conf
%dir %attr(-,root,%{_adgsgroup}) %{_InitPath}
%dir %attr(-,root,%{_adgsgroup}) %{_ConfPath}
%dir %attr(-,root,%{_adgsgroup}) %{_ConfPath}/database
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/conf
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/conf/database
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/log

%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib

%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-baselinereportgenerator.service
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-baselinereportgenerator.timer
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-purgebaselinereport.service
%attr(-,root,%{_adgsgroup}) %{_InitPath}/adgs-purgebaselinereport.timer
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/Encrypter
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/Decrypter
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/Decrypter.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/Encrypter.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/adgs_baseline_report_generator.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/common_utils.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/job_order_handler.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/configuration_handler.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/db_handler.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/mtd_handler.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/rotating_logger.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib/splunk_handler.rb
%attr(-,root,%{_adgsgroup}) %{_CompPath}/conf/adgs_baseline_report_generator.conf
%attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts/purgeBaselineReports.sh
%attr(-,root,root) %{_ConfPath}/database/db.conf
%attr(-,root,root) %{_ConfPath}/database/smplugins.conf
%{_DataPath}/conf/database/db.conf
%{_DataPath}/conf/database/smplugins.conf

%post

systemctl daemon-reload

if [ "$1" -eq 1 ]; then
    echo "%{_CompPath}/lib" > /etc/ld.so.conf.d/acs_share.conf

    sed -i "s/##ADGSDB_MACHINENAME##/$ADGSDB_MACHINENAME/g" %{_ConfPath}/database/db.conf
    sed -i "s/##SRVUSERDB_CRYPTPWD##/$SRVUSERDB_CRYPTPWD/g" %{_ConfPath}/database/db.conf

    export LD_LIBRARY_PATH=%{_CompPath}/lib

    # link the systemd Daemon Services/Timer
    services=( "adgs-baselinereportgenerator.service" "adgs-baselinereportgenerator.timer" "adgs-purgebaselinereport.service" "adgs-purgebaselinereport.timer" )

    for i in "${services[@]}"
    do
        if [ ! -h /etc/systemd/system/$i ]; then
            systemctl enable   %{_InitPath}/$i
            systemctl daemon-reload
        fi
    done

    systemctl daemon-reload
    systemctl start adgs-baselinereportgenerator.timer
    systemctl start adgs-purgebaselinereport.timer
fi

/sbin/ldconfig -v > /dev/null

if [ "$1" -eq 2 ]; then
    sed -i "s/##ADGSDB_MACHINENAME##/$ADGSDB_MACHINENAME/g" %{_ConfPath}/database/db.conf
    sed -i "s/##SRVUSERDB_CRYPTPWD##/$SRVUSERDB_CRYPTPWD/g" %{_ConfPath}/database/db.conf

    # link the systemd Daemon Services/Timer
    services=( "adgs-baselinereportgenerator.service" "adgs-baselinereportgenerator.timer" "adgs-purgebaselinereport.service" "adgs-purgebaselinereport.timer" )

    for i in "${services[@]}"
    do
        if [ ! -h /etc/systemd/system/$i ]; then
            systemctl enable   %{_InitPath}/$i
            systemctl daemon-reload
        fi
    done

    systemctl daemon-reload
    systemctl start adgs-baselinereportgenerator.timer
    systemctl start adgs-purgebaselinereport.timer
fi

%preun

if [ "$1" -eq 0 ]; then
    systemctl stop adgs-baselinereportgenerator.timer
    systemctl stop adgs-purgebaselinereport.timer
    systemctl disable adgs-baselinereportgenerator.timer
    systemctl disable adgs-baselinereportgenerator.service
    systemctl disable adgs-purgebaselinereport.timer
    systemctl disable adgs-purgebaselinereport.service
    systemctl daemon-reload
fi

%postun
if [ "$1" -eq 0 ]; then
    rm -f /etc/ld.so.conf.d/acs_share.conf
fi
/sbin/ldconfig -v > /dev/null

%clean
rm -rf %{buildroot} %{rpmPkgName}
