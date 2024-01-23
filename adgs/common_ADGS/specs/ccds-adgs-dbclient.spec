#########################################################################
%define _ConfPath    /usr/local/conf/ADGS
%define _CompPath    /usr/local/components/ADGS
%define _InitPath    /usr/local/ADGS
%define _DataPath    /data/ADGS
%define _StorePath   /data/ADGS
%define _ElementName ccds-adgs-dbclient
%define _Summary     The ADGS Database Client Configuration
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
Requires:    postgresql13
Provides:    %{_ElementName} = %{_Version}

%description
%{_Summary}

#%prep

%install

%__rm -fr %{buildroot}
%__mkdir -p %{buildroot}

umask 022
mkdir -p %{buildroot}/data
mkdir -p %{buildroot}%{_ConfPath}

umask 002
mkdir -p %{buildroot}%{_ConfPath}/database
mkdir -p %{buildroot}%{_DataPath}
mkdir -p %{buildroot}%{_DataPath}/conf
mkdir -p %{buildroot}%{_DataPath}/conf/database

cd %{buildroot}

install -m 0644 %{_topdir}/../dbconf/db.conf        %{buildroot}%{_ConfPath}/database/db.conf
install -m 0644 %{_topdir}/../dbconf/smplugins.conf %{buildroot}%{_ConfPath}/database/smplugins.conf

ln -s %{_ConfPath}/database/db.conf        .%{_DataPath}/conf/database/db.conf
ln -s %{_ConfPath}/database/smplugins.conf .%{_DataPath}/conf/database/smplugins.conf

%files
%dir %attr(-,root,root) /usr/local
%dir %attr(-,root,%{_adgsgroup}) %{_ConfPath}
%dir %attr(-,root,%{_adgsgroup}) %{_ConfPath}/database
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/conf
%dir %attr(-,root,%{_adgsgroup}) %{_DataPath}/conf/database

%attr(-,root,root) %{_ConfPath}/database/db.conf
%attr(-,root,root) %{_ConfPath}/database/smplugins.conf
%{_DataPath}/conf/database/db.conf
%{_DataPath}/conf/database/smplugins.conf

%post
    sed -i "s/##ADGSDB_MACHINENAME##/$ADGSDB_MACHINENAME/g" %{_ConfPath}/database/db.conf
    sed -i "s/##PDSUSERDB_PWD##/$PDSUSER_USERPWD/g"         %{_ConfPath}/database/db.conf

%clean
rm -rf %{buildroot} %{rpmPkgName}
