#########################################################################
%define _ConfPath    /usr/local/conf/ADGS
%define _CompPath    /usr/local/components/ADGS
%define _InitPath    /usr/local/ADGS
%define _DataPath    /data/ADGS
%define _StorePath   /data/ADGS
%define _ElementName ccds-adgs-shared-libraries
%define _Summary     The ADGS Shared Libraries
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
mkdir -p %{buildroot}%{_CompPath}/lib
mkdir -p %{buildroot}%{_CompPath}/plugins
mkdir -p %{buildroot}%{_CompPath}/plugins/PreInventoryPlugins
mkdir -p %{buildroot}%{_CompPath}/plugins/SchedulerPlugins
mkdir -p %{buildroot}%{_CompPath}/plugins/TlDaemonPlugins
mkdir -p %{buildroot}%{_CompPath}/plugins/TlDaemonPlugins/JobOrder
mkdir -p %{buildroot}%{_CompPath}/plugins/OrderGeneratorPlugins

cp $(ls %{_topdir}/../shared/* | egrep '.*\.so(\.[0-9]+){3}') %{buildroot}%{_CompPath}/lib
/usr/sbin/ldconfig -N -n %{buildroot}%{install_path}/lib

%files

%attr(-,root,%{_adgsgroup}) %{_CompPath}/lib
%attr(-,root,%{_adgsgroup}) %{_CompPath}/plugins/PreInventoryPlugins
%attr(-,root,%{_adgsgroup}) %{_CompPath}/plugins/SchedulerPlugins
%attr(-,root,%{_adgsgroup}) %{_CompPath}/plugins/TlDaemonPlugins
%attr(-,root,%{_adgsgroup}) %{_CompPath}/plugins/TlDaemonPlugins/JobOrder
%attr(-,root,%{_adgsgroup}) %{_CompPath}/plugins/OrderGeneratorPlugins

%post
if [ "$1" -eq 1 ]; then
    echo "%{_CompPath}/lib" > /etc/ld.so.conf.d/acs_share.conf
fi
/sbin/ldconfig -v > /dev/null

libs="libPostgres8Dialect.so libGenFileAction.so libObjSMClient.so libIsmSMClient.so libIsmSoapClient.so libacdbexppgl.so"
for lib in $libs
do
    rm -f %{_CompPath}/plugins/$lib
    p=0; uv=0
    for so in `ls %{_CompPath}/lib/${lib}.*`
    do
        name=`basename $so`
        ver=`echo $name | awk --field-separator=. '{print $6 + ($5 * 100) + ($4 * 100 * 100) + ($3 * 100 * 100 * 100 ) ; }'`
        if [ "$ver" -gt "$uv" ]; then
            uv=$ver
            p=$name
        fi
    done
    ln -s %{_CompPath}/lib/$p %{_CompPath}/plugins/$lib
done

#libs="libNETCDFPreInventory.so libGRIBPreInventory.so libEOFPreInventory.so libSafePreInventory.so libIERSPreInventory.so libFilenamePreInventory.so libPDGSExtDataPreInventory.so"
libs="libEOFFilenamePreInventory.so libNETCDFPreInventory.so libGRIBPreInventory.so libEOFPreInventory.so libSafePreInventory.so libFilenamePreInventory.so"
for lib in $libs
do
    rm -f %{_CompPath}/plugins/PreInventoryPlugins/$lib
    p=0; uv=0
    for so in `ls %{_CompPath}/lib/${lib}.*`
    do
        name=`basename $so`
        ver=`echo $name | awk --field-separator=. '{print $6 + ($5 * 100) + ($4 * 100 * 100) + ($3 * 100 * 100 * 100 ) ; }'`
        if [ "$ver" -gt "$uv" ]; then
            uv=$ver
            p=$name
        fi
    done
    ln -s %{_CompPath}/lib/$p %{_CompPath}/plugins/PreInventoryPlugins/$lib
done

libs="libTlDefaultPCC.so libOcadaJobOrder.so"
for lib in $libs
do
    rm -f %{_CompPath}/plugins/TlDaemonPlugins/$lib
    p=0; uv=0
    for so in `ls %{_CompPath}/lib/${lib}.*`
    do
        name=`basename $so`
        ver=`echo $name | awk --field-separator=. '{print $6 + ($5 * 100) + ($4 * 100 * 100) + ($3 * 100 * 100 * 100 ) ; }'`
        if [ "$ver" -gt "$uv" ]; then
            uv=$ver
            p=$name
        fi
    done
    ln -s %{_CompPath}/lib/$p %{_CompPath}/plugins/TlDaemonPlugins/$lib
done


libs="libScDefaultCommands.so libScDefaultLogGroup.so"
for lib in $libs
do
    rm -f %{_CompPath}/plugins/SchedulerPlugins/$lib
    p=0; uv=0
    for so in `ls %{_CompPath}/lib/${lib}.*`
    do
        name=`basename $so`
        ver=`echo $name | awk --field-separator=. '{print $6 + ($5 * 100) + ($4 * 100 * 100) + ($3 * 100 * 100 * 100 ) ; }'`
        if [ "$ver" -gt "$uv" ]; then
            uv=$ver
            p=$name
        fi
    done
    ln -s %{_CompPath}/lib/$p %{_CompPath}/plugins/SchedulerPlugins/$lib
done


%preun

if [ "$1" -eq 0 ]; then

    libs="libPostgres8Dialect.so libGenFileAction.so libObjSMClient.so libIsmSMClient.so libIsmSoapClient.so libacdbexppgl.so"
    for lib in $libs
    do
        rm -f %{_CompPath}/plugins/$lib
    done

    #libs="libNETCDFPreInventory.so libGRIBPreInventory.so libEOFPreInventory.so libSafePreInventory.so libIERSPreInventory.so libFilenamePreInventory.so libPDGSExtDataPreInventory.so"
    libs="libEOFFilenamePreInventory.so libNETCDFPreInventory.so libGRIBPreInventory.so libEOFPreInventory.so libSafePreInventory.so libFilenamePreInventory.so"
    for lib in $libs
    do
        rm -f %{_CompPath}/plugins/PreInventoryPlugins/$lib
    done

    libs="libTlDefaultPCC.so libOcadaJobOrder.so"
    for lib in $libs
    do
        rm -f %{_CompPath}/plugins/TlDaemonPlugins/$lib
    done

    libs="libScDefaultCommands.so  libScDefaultLogGroup.so"
    for lib in $libs
    do
        rm -f %{_CompPath}/plugins/SchedulerPlugins/$lib
    done
fi

%postun
if [ "$1" -eq 0 ]; then
    rm -f /etc/ld.so.conf.d/acs_share.conf
fi
/sbin/ldconfig -v > /dev/null

%clean
rm -rf %{buildroot} %{rpmPkgName}
