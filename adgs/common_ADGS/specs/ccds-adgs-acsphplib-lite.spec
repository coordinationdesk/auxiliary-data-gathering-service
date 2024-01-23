Name: ccds-adgs-acsphplib-lite
Version: 1.0
Release: 0
Summary: Summary

License: Commercial. Copyright 1995-2021 Exprivia Spa
Group: Application/Networking
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

BuildArchitectures: noarch

Requires: php56-php-common, php-common

Provides: ccds-adgs-acsphplib-lite

%description

%install

install -d "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/interfaces"
install -d "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras"
install -d "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi"
install -d "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Tests"
install -d "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Tests/Communication"
install -d "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes"
install -d "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Communication"
install -d "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Thread"
install -d "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Examples"
install -d "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/sqltokenizer"
install -d "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/"

install -m644 "%{_topdir}/../applications/ACSPhpLibLite/acs_workflow.php"                                                "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/acs_workflow.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/interfaces/acs_idb.php"                                          "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/interfaces/acs_idb.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/interfaces/acs_ilog.php"                                         "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/interfaces/acs_ilog.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/acs_log.php"                                                     "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/acs_log.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/acs_daemon.php"                                                  "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/acs_daemon.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/acs_mdb2.php"                                                    "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/acs_mdb2.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/acs_servlet_ext.php"                                             "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/acs_servlet_ext.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/acs_cmdexec.php"                                                 "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/acs_cmdexec.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/acs_geoserver_tools.php"                                         "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/acs_geoserver_tools.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/acs_tools.php"                                                   "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/acs_tools.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/acs_app.php"                                                     "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/acs_app.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/acs_wfoperation.php"                                             "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/acs_wfoperation.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/Makefile"                                                        "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/Makefile"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/acs_servlet.php"                                                 "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/acs_servlet.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/README.md"                                                       "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/README.md"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/acs_log_utils.php"                                               "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/acs_log_utils.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/acs_exception.php"                                               "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/acs_exception.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/acs_sql.php"                                                     "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/acs_sql.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/acs_file.php"                                                    "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/acs_file.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/extras/threadi/Tests/Communication/SharedMemory_Test.php"        "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Tests/Communication/SharedMemory_Test.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/extras/threadi/Loader.php"                                       "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Loader.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/extras/threadi/Classes/Communication/CommunicationInterface.php" "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Communication/CommunicationInterface.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/extras/threadi/Classes/Communication/SharedMemory.php"           "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Communication/SharedMemory.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/extras/threadi/Classes/ReadyAskableInterface.php"                "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/ReadyAskableInterface.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/extras/threadi/Classes/Thread/ReturnableThreadInterface.php"     "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Thread/ReturnableThreadInterface.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/extras/threadi/Classes/Thread/ThreadInterface.php"               "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Thread/ThreadInterface.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/extras/threadi/Classes/Thread/PHPThread.php"                     "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Thread/PHPThread.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/extras/threadi/Classes/Thread/AbstractThread.php"                "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Thread/AbstractThread.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/extras/threadi/Classes/Thread/PHPReturnableThread.php"           "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Thread/PHPReturnableThread.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/extras/threadi/Classes/Thread/NonThread.php"                     "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Thread/NonThread.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/extras/threadi/Classes/ThreadService.php"                        "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/ThreadService.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/extras/threadi/Classes/JoinPoint.php"                            "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/JoinPoint.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/extras/threadi/Classes/ThreadFactory.php"                        "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/ThreadFactory.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/extras/threadi/README"                                           "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/README"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/extras/threadi/Examples/Parallel-Returnable.php"                 "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Examples/Parallel-Returnable.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/extras/threadi/Examples/Parallel-Simple.php"                     "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Examples/Parallel-Simple.php"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/extras/threadi/TODO"                                             "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/TODO"
install -m644 "%{_topdir}/../applications/ACSPhpLibLite/extras/sqltokenizer/SqlParser.php"                               "$RPM_BUILD_ROOT/opt/ACSPhpLibLite/ACSPhpLib/extras/sqltokenizer/SqlParser.php"

mkdir -p $RPM_BUILD_ROOT/var/www/html/logs/
mkdir -p $RPM_BUILD_ROOT/var/www/html/runpid/
mkdir -p $RPM_BUILD_ROOT/etc/php.d/
mkdir -p $RPM_BUILD_ROOT/opt/remi/php56/root/etc/php.d/
mkdir -p $RPM_BUILD_ROOT/opt/ACSPhpLibLite/pear

cp -r %{_topdir}/../applications/ACSPhpLibLite/pear $RPM_BUILD_ROOT/opt/ACSPhpLibLite/

cat - > $RPM_BUILD_ROOT/etc/php.d/000-acsphplib-php7.ini <<EOF
include_path=.:/opt/ACSPhpLibLite:/opt/ACSPhpLibLite/pear
EOF

cat - > $RPM_BUILD_ROOT/opt/remi/php56/root/etc/php.d/000-acsphplib-php7.ini <<EOF
include_path=.:/opt/ACSPhpLibLite:/opt/ACSPhpLibLite/pear
EOF

%files
/opt/ACSPhpLibLite/ACSPhpLib/acs_workflow.php
/opt/ACSPhpLibLite/ACSPhpLib/interfaces/acs_idb.php
/opt/ACSPhpLibLite/ACSPhpLib/interfaces/acs_ilog.php
/opt/ACSPhpLibLite/ACSPhpLib/acs_log.php
/opt/ACSPhpLibLite/ACSPhpLib/acs_daemon.php
/opt/ACSPhpLibLite/ACSPhpLib/acs_mdb2.php
/opt/ACSPhpLibLite/ACSPhpLib/acs_servlet_ext.php
/opt/ACSPhpLibLite/ACSPhpLib/acs_cmdexec.php
/opt/ACSPhpLibLite/ACSPhpLib/acs_geoserver_tools.php
/opt/ACSPhpLibLite/ACSPhpLib/acs_tools.php
/opt/ACSPhpLibLite/ACSPhpLib/acs_app.php
/opt/ACSPhpLibLite/ACSPhpLib/acs_wfoperation.php
/opt/ACSPhpLibLite/ACSPhpLib/Makefile
/opt/ACSPhpLibLite/ACSPhpLib/acs_servlet.php
/opt/ACSPhpLibLite/ACSPhpLib/README.md
/opt/ACSPhpLibLite/ACSPhpLib/acs_log_utils.php
/opt/ACSPhpLibLite/ACSPhpLib/acs_exception.php
/opt/ACSPhpLibLite/ACSPhpLib/acs_sql.php
/opt/ACSPhpLibLite/ACSPhpLib/acs_file.php
/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Tests/Communication/SharedMemory_Test.php
/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Loader.php
/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Communication/CommunicationInterface.php
/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Communication/SharedMemory.php
/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/ReadyAskableInterface.php
/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Thread/ReturnableThreadInterface.php
/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Thread/ThreadInterface.php
/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Thread/PHPThread.php
/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Thread/AbstractThread.php
/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Thread/PHPReturnableThread.php
/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Thread/NonThread.php
/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/ThreadService.php
/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/JoinPoint.php
/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/ThreadFactory.php
/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/README
/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Examples/Parallel-Returnable.php
/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Examples/Parallel-Simple.php
/opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/TODO
/opt/ACSPhpLibLite/ACSPhpLib/extras/sqltokenizer/SqlParser.php

%dir /opt/ACSPhpLibLite/ACSPhpLib/interfaces
%dir /opt/ACSPhpLibLite/ACSPhpLib/extras
%dir /opt/ACSPhpLibLite/ACSPhpLib/extras/threadi
%dir /opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Tests
%dir /opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Tests/Communication
%dir /opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes
%dir /opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Communication
%dir /opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Classes/Thread
%dir /opt/ACSPhpLibLite/ACSPhpLib/extras/threadi/Examples
%dir /opt/ACSPhpLibLite/ACSPhpLib/extras/sqltokenizer
%dir /opt/ACSPhpLibLite/ACSPhpLib/

%dir %attr(0777,root,root) /var/www/html/logs/
%dir %attr(0777,root,root) /var/www/html/runpid/
/etc/php.d/000-acsphplib-php7.ini
/opt/remi/php56/root/etc/php.d/000-acsphplib-php7.ini
/opt/ACSPhpLibLite/pear

