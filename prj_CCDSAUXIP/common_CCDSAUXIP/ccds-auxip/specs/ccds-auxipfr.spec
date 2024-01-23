#########################################################################
%define _Prefix         /opt/exprivia
%define _ElementName    auxipfr
%define _Project        ccdsauxip
%define _Summary        The CCDS AUXIP element package
%define _License        Proprietary
%define _Group          Applications/%{_ElementName}
%define _Vendor         Exprivia
%define _Distribution   ccds
%define _BuildArch      x86_64
%define _Packager       Exprivia AIV TEAM
%define _juser          opadgs
%define _jgroup         ccds
#########################################################################

%define main_elem       %{_Distribution}-%{_ElementName}
%define rpmPkgName      %{main_elem}-%{_Version}
%define install_dir     %{prefix}
%define install_path    %install_dir/%{_ElementName}-%{version}
%define vinstall_path   %install_dir/%{_ElementName}

Name:           %{rpmPkgName}
Version:        %{_Version}
Release:        1%{dist}
Summary:        %{_Summary}

License:          %{_License}
Group:            %{_Group}
Source:         file://%{_sourcedir}/%{rpmPkgName}.tgz



BuildRoot:        %{_tmppath}/%{rpmPkgName}-root-%(%{__id_u} -n)
URL:              www.exprivia.com
Vendor:           %{_Vendor}
Distribution:     %{_Distribution}
Prefix:           %{_Prefix}
Packager:         %{_Packager}

AutoReqProv:      no

#Requires:         SRV-common
Requires:         xerces-c
Requires:         boost169
Requires:         bc
Requires:         ruby
Requires:         rubygem-pg
Requires:         rubygem-json
Requires:         unzip
Requires:         p7zip
Requires:         p7zip-plugins
Requires:         perl-Image-ExifTool
Requires:         xxhash
Requires:         aws-sdk-cpp
Requires:         postgresql13
Requires:         libpq5
Requires:         netcdf_c++4_1
Requires:         gsoap
Requires:         pistache-devel
Requires:         mosquitto

Provides:         %{main_elem} = %{_Version}

%description
%{_Summary}

%prep
%__rm -fr %{_builddir}
%__mkdir -p %{_builddir}
%setup -n %{rpmPkgName}

%build
umask 0002
export project=$(echo %{_Project} | tr '[:upper:]' '[:lower:]')
export PROJECT=$(echo %{_Project} | tr '[:lower:]' '[:upper:]')
export PRJROOTTREE=`pwd`
export LIB_INSTALL_PATH=$PRJROOTTREE
export LIB_INTEGRATION_PATH=$PRJROOTTREE
export PATH=$PATH:$PRJROOTTREE/tools
export EXE_INTEGRATION_PATH=$PRJROOTTREE/bin
export LDOPTS_DEPLOY_RPATH=-Wl,-rpath,%{install_path}/lib
export ENABLE_SEMANTIC=1

cd components
make -s  abortOnError -j4
cd ../applications
make -s  abortOnError -j4

%install
cd %{_builddir}/%{rpmPkgName}
%__mkdir -p %{buildroot}%{install_path}/{lib,bin,plugins,conf,dbschema,systemd,plugins/PreInventoryPlugins,plugins/SchedulerPlugins}

cp $(ls shared/* | egrep '.*\.so(\.[0-9]+){3}') %{buildroot}%{install_path}/lib
rm %{buildroot}%{install_path}/lib/*.defsym
/sbin/ldconfig -N -n                                                     %{buildroot}%{install_path}/lib

install -m755 bin/Encrypter                                              %{buildroot}%{install_path}/bin
install -m755 bin/Decrypter                                              %{buildroot}%{install_path}/bin
install -m755 bin/SRVNotificationDispatcher                              %{buildroot}%{install_path}/bin
install -m755 bin/LTAQuotaService                                        %{buildroot}%{install_path}/bin
install -m755 bin/GarbageCleaner                                         %{buildroot}%{install_path}/bin
cp -r conf                                                               %{buildroot}%{install_path}
cp -r PostgresUTILITIES                                                  %{buildroot}%{install_path}
install -m644 systemd/ccds-auxipfrontend.service                         %{buildroot}%{install_path}/systemd
install -m644 systemd/ccds-srvnotificationdispatcher.service             %{buildroot}%{install_path}/systemd
install -m644 systemd/ccds-ltaquotaservice.service.service               %{buildroot}%{install_path}/systemd
install -m644 systemd/ccds-garbagecleaner.service                        %{buildroot}%{install_path}/systemd
install -m644 systemd/ccds-garbagecleaner.timer                          %{buildroot}%{install_path}/systemd

cp -r scripts                                                            %{buildroot}%{install_path}
cp -r csv                                                                %{buildroot}%{install_path}
cp -r tasktable                                                          %{buildroot}%{install_path}

%files
%attr(755,root,root)
%{install_path}/bin/*
%{install_path}/plugins
%{install_path}/lib/*
%{install_path}/systemd/*
%{install_path}/conf/*
%{install_path}/scripts/*
%{install_path}/csv/*
%{install_path}/tasktable/*
%{install_path}/PostgresUTILITIES

%post
#create user and group if not already available
groupadd -g 400 -o -r ccds >/dev/null 2>&1 || :
useradd -m -g ccds -o -r  -s /bin/bash \
    -c "CCDS Server User" -u 500 opadgs >/dev/null 2>&1 || :

#First install automatic activation BIG link
if [ ! -h "%{vinstall_path}" ];               then ln -s %{install_path} %{vinstall_path}; fi

#Enable conf locking
chmod 775   %{install_path}/conf ; chgrp %{_jgroup} %{install_path}/conf

#Enable debug directory
mkdir  -m775 %{install_path}/debug; chgrp %{_jgroup} %{install_path}/debug

#Set Localhost conf file
if [ ! -h "%{install_path}/conf/"$HOSTNAME.xml ]; then
    ln -s %{install_path}/conf/Localhost.xml    %{install_path}/conf/$HOSTNAME.xml
fi


#LOG Links
if [ ! -d /data/%{_ElementName}/log ]; then mkdir -p -m775 /data/%{_ElementName}/log; chgrp %{_jgroup} /data/%{_ElementName}/log; fi
ln -s /data/%{_ElementName}/log %{install_path}/log

#Plugins Links
libs="libPostgres8Dialect.so libLTAFileAction.so libacdbexppgl.so libObjSMClient.so"
for lib in $libs
do
        rm -f  %{install_path}/plugins/$lib
        p=0; uv=0
        for so in `ls  %{install_path}/lib/${lib}.*`
        do
                name=`basename $so`
                ver=`echo $name | awk --field-separator=. '{print $6 + ($5 * 100) + ($4 * 100 * 100) + ($3 * 100 * 100 * 100 ) ; }'`
                if [ "$ver" -gt "$uv" ]; then
                        uv=$ver
                        p=$name
                fi
        done
        ln -s  %{install_path}/lib/$p  %{install_path}/plugins/$lib
done

# Scheduler Plugins Links
libs="libScDefaultCommands.so libScDefaultLogGroup.so"
for lib in $libs
do
        rm -f  %{install_path}/plugins/PreInventoryPlugins/$lib
        p=0; uv=0
        for so in `ls  %{install_path}/lib/${lib}.*`
        do
                name=`basename $so`
                ver=`echo $name | awk --field-separator=. '{print $6 + ($5 * 100) + ($4 * 100 * 100) + ($3 * 100 * 100 * 100 ) ; }'`
                if [ "$ver" -gt "$uv" ]; then
                        uv=$ver
                        p=$name
                fi
        done
        ln -s  %{install_path}/lib/$p  %{install_path}/plugins/SchedulerPlugins/$lib
done

#customize config files

sed -i 's|#INSTALL_PATH#|%{install_path}|g'    %{install_path}/conf/database/db.conf
sed -i 's|#INSTALL_PATH#|%{install_path}|g'    %{install_path}/conf/database/smplugins.conf

sed -i 's/#LTAUSER#/%{_juser}/g'    %{install_path}/conf/libFtpCurl.so.xml
sed -i 's/#LTAGROUP#/%{_jgroup}/g'  %{install_path}/conf/libFtpCurl.so.xml

sed -i 's/#QSPORT#/10020/g'                    %{install_path}/conf/LTAQuotaService.xml
sed -i 's/#NSPORT#/10000/g'                    %{install_path}/conf/SRVNotificationDispatcher.xml

sed -i 's/#LTAUSER#/%{_juser}/g'                    %{install_path}/systemd/ccds-ltaquotaservice.service.service
sed -i 's/#LTAGROUP#/%{_jgroup}/g'                  %{install_path}/systemd/ccds-ltaquotaservice.service.service
sed -i 's|#VINSTALL_PATH#|%{vinstall_path}|g'       %{install_path}/systemd/ccds-ltaquotaservice.service.service

sed -i 's/#LTAUSER#/%{_juser}/g'                    %{install_path}/systemd/ccds-srvnotificationdispatcher.service
sed -i 's/#LTAGROUP#/%{_jgroup}/g'                  %{install_path}/systemd/ccds-srvnotificationdispatcher.service
sed -i 's|#VINSTALL_PATH#|%{vinstall_path}|g'       %{install_path}/systemd/ccds-srvnotificationdispatcher.service

sed -i 's/#LTAUSER#/%{_juser}/g'                    %{install_path}/systemd/ccds-garbagecleaner.service
sed -i 's/#LTAGROUP#/%{_jgroup}/g'                  %{install_path}/systemd/ccds-garbagecleaner.service
sed -i 's|#VINSTALL_PATH#|%{vinstall_path}|g'       %{install_path}/systemd/ccds-garbagecleaner.service

if [ -f ~/.pgpass ]; then
    DBHOSTNAME=$(cat ~/.pgpass | cut -f1 -d:)
    #DB Host configuration
    sed -i 's|#DBHOST#|'$DBHOSTNAME'|g'     %{install_path}/PostgresUTILITIES/psqlUtilitiesConf/lta.conf
    sed -i 's|#DBHOST#|'$DBHOSTNAME'|g'     %{install_path}/conf/database/db.conf
else
    echo "WARNING!  .pgpass does not exist DB is assumed already configured";
    echo "Please configure properly the DB hostname #DBHOST# and variable on  %{install_path}/conf/database/db.conf"
fi

# link the systemd Daemon Services/Timer
services=( "ccds-srvnotificationdispatcher.service" "ccds-ltaquotaservice.service.service" "ccds-garbagecleaner.service" "ccds-garbagecleaner.timer" )

for i in "${services[@]}"
do
    if [ ! -h /etc/systemd/system/$i ]; then
        systemctl link   %{vinstall_path}/systemd/$i
        systemctl daemon-reload
    fi
done

# Enable the systemd Daemon Services
services=( "ccds-auxipfrontend.service" )

for i in "${services[@]}"
do
    if [ ! -h /etc/systemd/system/$i ]; then
        systemctl enable   %{vinstall_path}/systemd/$i
        systemctl daemon-reload
    fi
done

systemctl daemon-reload

%preun
#check if this version is the active one and exit on error
active_version=$(readlink -n %{vinstall_path})

if [ -n "$active_version" ] && [ "$active_version" == "%{install_path}" ]
then
    echo "Warning this is the active version cannot be removed!"
    exit 1
fi

%postun
#remove dirty directory
rm -rf %{install_path}

%clean
rm -rf $RPM_BUILD_ROOT

%changelog
* Tue Oct 26 2021 Claudio Nasuti 0.1
- First Release
