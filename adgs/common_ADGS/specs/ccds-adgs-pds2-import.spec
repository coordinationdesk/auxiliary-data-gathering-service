Name: ccds-adgs-pds2_import_adgs
Version: 1.4
Release: 1
Summary: Summary

License: Commercial. Copyright 1995-2021 Exprivia Spa
Group: Application/Networking
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
BuildArchitectures: noarch

Requires: ccds-adgs-acsphplib-lite,php56-php-cli,php56-php-pgsql,php56-php-pecl-ssh2

Provides: pds2_import_adgs

%description

%install

install -d "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/"
install -d "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/config"
install -d "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/engine"
install -d "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/engine/plugins"
install -d "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/engine/tools"
install -d "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/setup"
install -d "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/setup/conf"

install -m644 "%{_topdir}/../applications/pds2_import_adgs/Makefile"                                 "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/Makefile"

install -m644 "%{_topdir}/../applications/pds2_import_adgs/config/ext_pds2_import.ini"               "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/config/ext_pds2_import.ini"

install -m644 "%{_topdir}/../applications/pds2_import_adgs/engine/pds2_import_adgs.php"              "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/engine/pds2_import_adgs.php"

install -m644 "%{_topdir}/../applications/pds2_import_adgs/engine/plugins/ftp_defines.php"           "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/engine/plugins/ftp_defines.php"
install -m644 "%{_topdir}/../applications/pds2_import_adgs/engine/plugins/ftp_imp_pl_adgs_date.php"  "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/engine/plugins/ftp_imp_pl_adgs_date.php"
install -m644 "%{_topdir}/../applications/pds2_import_adgs/engine/plugins/ftp_imp_pl_adgs_newer.php" "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/engine/plugins/ftp_imp_pl_adgs_newer.php"
install -m644 "%{_topdir}/../applications/pds2_import_adgs/engine/plugins/ftp_imp_pl_adgs.php"       "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/engine/plugins/ftp_imp_pl_adgs.php"
install -m644 "%{_topdir}/../applications/pds2_import_adgs/engine/plugins/ftp_special_rules.php"     "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/engine/plugins/ftp_special_rules.php"
install -m644 "%{_topdir}/../applications/pds2_import_adgs/engine/plugins/ftp_tools.php"             "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/engine/plugins/ftp_tools.php"
install -m644 "%{_topdir}/../applications/pds2_import_adgs/engine/plugins/nop_imp.php"               "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/engine/plugins/nop_imp.php"

install -m644 "%{_topdir}/../applications/pds2_import_adgs/engine/tools/preimp_ftp_curl_new.php"     "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/engine/tools/preimp_ftp_curl_new.php"

install -m644 "%{_topdir}/../applications/pds2_import_adgs/setup/conf/pds2_import_php.ini"           "$RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/setup/conf/pds2_import_php.ini"

mkdir -p $RPM_BUILD_ROOT/var/run/pds2_import
mkdir -p $RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/config
mkdir -p $RPM_BUILD_ROOT/etc/php.d
mkdir -p $RPM_BUILD_ROOT/opt/remi/php56/root/etc/php.d
mkdir -p $RPM_BUILD_ROOT/usr/lib/systemd/system/

ln -s /usr/local/components/ADGS/pds2_import/config/pds2_import.ini               $RPM_BUILD_ROOT/usr/local/components/ADGS/pds2_import_adgs/config/pds2_import.ini
ln -sf /usr/local/components/ADGS/pds2_import_adgs/setup/conf/pds2_import_php.ini $RPM_BUILD_ROOT/etc/php.d/pds2_import_php.ini
ln -sf /usr/local/components/ADGS/pds2_import_adgs/setup/conf/pds2_import_php.ini $RPM_BUILD_ROOT/opt/remi/php56/root/etc/php.d/pds2_import_php.ini

# embed pds2_import
find %{_topdir}/../applications/pds2_import_adgs/embed/ \
    -type d \
    -mindepth 1 -maxdepth 1 \
    -exec cp -r {} $RPM_BUILD_ROOT/usr/local/components/ADGS \;

%post

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

/usr/local/components/ADGS/pds2_import_adgs/Makefile

/usr/local/components/ADGS/pds2_import_adgs/config/ext_pds2_import.ini

/usr/local/components/ADGS/pds2_import_adgs/engine/pds2_import_adgs.php
/usr/local/components/ADGS/pds2_import_adgs/engine/plugins/ftp_defines.php
/usr/local/components/ADGS/pds2_import_adgs/engine/plugins/ftp_imp_pl_adgs_date.php
/usr/local/components/ADGS/pds2_import_adgs/engine/plugins/ftp_imp_pl_adgs_newer.php
/usr/local/components/ADGS/pds2_import_adgs/engine/plugins/ftp_imp_pl_adgs.php
/usr/local/components/ADGS/pds2_import_adgs/engine/plugins/ftp_special_rules.php
/usr/local/components/ADGS/pds2_import_adgs/engine/plugins/ftp_tools.php
/usr/local/components/ADGS/pds2_import_adgs/engine/plugins/nop_imp.php

/usr/local/components/ADGS/pds2_import_adgs/engine/tools/preimp_ftp_curl_new.php

/usr/local/components/ADGS/pds2_import_adgs/setup/conf/pds2_import_php.ini

# this is now done by systemd
# %dir %attr(755, ops3pdgs, S3appladm) /var/run/pds2_import
/etc/php.d/pds2_import_php.ini
/opt/remi/php56/root/etc/php.d/pds2_import_php.ini
/usr/local/components/ADGS/pds2_import_adgs/config/pds2_import.ini

# embedded version of pds2_import
/usr/local/components/ADGS/pds2_import

