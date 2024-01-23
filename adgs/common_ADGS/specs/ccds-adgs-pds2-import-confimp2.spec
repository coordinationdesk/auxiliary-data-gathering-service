Name: ccds-adgs-pds2_import_confimp2
Version: 1.1
Release: 0
Summary: Summary

License: Commercial. Copyright 1995-2021 Exprivia Spa
Group: Application/Networking
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
BuildArchitectures: noarch

Requires: pds2_import_adgs

Provides: pds2_import_adgs-confimp2

%description

%install

install -d "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/"
install -d "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/config"
install -d "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/engine"
install -d "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/engine/plugins"
install -d "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/engine/tools"
install -d "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/setup"
install -d "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/setup/conf"

install -m644 "%{_topdir}/../ADGSConf/confPdsImport/import_pds-imp2-1.ini"          "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/config/import_pds-imp2-1.ini"
install -m644 "%{_topdir}/../ADGSConf/confPdsImport/import_pds-imp2-2.ini"          "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/config/import_pds-imp2-2.ini"

install -m644 "%{_topdir}/../ADGSConf/confPdsImport/pds2_import_pds-imp2-1.service"   "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/setup/conf/pds2_import_pds-imp2-1.service"
install -m644 "%{_topdir}/../ADGSConf/confPdsImport/pds2_import_pds-imp2-1.timer"     "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/setup/conf/pds2_import_pds-imp2-1.timer"
install -m644 "%{_topdir}/../ADGSConf/confPdsImport/pds2_import_pds-imp2-2.service"   "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/setup/conf/pds2_import_pds-imp2-2.service"
install -m644 "%{_topdir}/../ADGSConf/confPdsImport/pds2_import_pds-imp2-2.timer"     "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/setup/conf/pds2_import_pds-imp2-2.timer"

mkdir -p $RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/config
mkdir -p $RPM_BUILD_ROOT/usr/lib/systemd/system/

cp $RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/setup/conf/pds2_import_pds-imp2-1.service $RPM_BUILD_ROOT/usr/lib/systemd/system/
cp $RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/setup/conf/pds2_import_pds-imp2-1.timer   $RPM_BUILD_ROOT/usr/lib/systemd/system/
cp $RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/setup/conf/pds2_import_pds-imp2-2.service $RPM_BUILD_ROOT/usr/lib/systemd/system/
cp $RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/setup/conf/pds2_import_pds-imp2-2.timer   $RPM_BUILD_ROOT/usr/lib/systemd/system/

%post

if ! grep -q "pds2_import_pds-imp2-1.timer" /usr/local/components/ADGS/pds2_import_adgs/setup/conf/ListPdsImportServices.txt ; then
    echo "pds2_import_pds-imp2-1.timer" >> /usr/local/components/ADGS/pds2_import_adgs/setup/conf/ListPdsImportServices.txt
fi

if ! grep -q "pds2_import_pds-imp2-2.timer" /usr/local/components/ADGS/pds2_import_adgs/setup/conf/ListPdsImportServices.txt ; then
    echo "pds2_import_pds-imp2-2.timer" >> /usr/local/components/ADGS/pds2_import_adgs/setup/conf/ListPdsImportServices.txt
fi

systemctl daemon-reload

%postun
systemctl daemon-reload

%files
%dir /usr/local/components/ADGS/pds2_import_adgs/
%dir /usr/local/components/ADGS/pds2_import_adgs/config
%dir /usr/local/components/ADGS/pds2_import_adgs/engine
%dir /usr/local/components/ADGS/pds2_import_adgs/engine/plugins
%dir /usr/local/components/ADGS/pds2_import_adgs/engine/tools
%dir /usr/local/components/ADGS/pds2_import_adgs/setup
%dir /usr/local/components/ADGS/pds2_import_adgs/setup/conf

/usr/local/components/ADGS/pds2_import_adgs/setup/conf/pds2_import_pds-imp2-1.service
/usr/local/components/ADGS/pds2_import_adgs/setup/conf/pds2_import_pds-imp2-1.timer
/usr/local/components/ADGS/pds2_import_adgs/setup/conf/pds2_import_pds-imp2-2.service
/usr/local/components/ADGS/pds2_import_adgs/setup/conf/pds2_import_pds-imp2-2.timer

# this is now done by systemd
# %dir %attr(755, ops3pdgs, S3appladm) /var/run/pds2_import
/usr/local/components/ADGS/pds2_import_adgs/config/import_pds-imp2-1.ini
/usr/local/components/ADGS/pds2_import_adgs/config/import_pds-imp2-2.ini
/usr/lib/systemd/system/pds2_import_pds-imp2-1.service
/usr/lib/systemd/system/pds2_import_pds-imp2-1.timer
/usr/lib/systemd/system/pds2_import_pds-imp2-2.service
/usr/lib/systemd/system/pds2_import_pds-imp2-2.timer

