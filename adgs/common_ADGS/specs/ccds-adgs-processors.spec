#########################################################################
%define _ConfPath    /usr/local/conf/ADGS
%define _CompPath    /usr/local/components/ADGS
%define _EscCompPath \/usr\/local\/components\/ADGS
%define _InitPath    /usr/local/ADGS
%define _DataPath    /data/ADGS
%define _StorePath   /data/ADGS
%define _ElementName ccds-adgs-processors
%define _Summary     The ADGS Processor installation
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
Requires:    ccds-adgs-client-core
Provides:    %{_ElementName} = %{_Version}

%description
%{_Summary}

#%prep

%install

%__rm -fr %{buildroot}
%__mkdir -p %{buildroot}

cd %{buildroot}

#umask 022
#mkdir -p %{buildroot}/usr/local

umask 002
mkdir -p %{buildroot}%{_CompPath}
mkdir -p %{buildroot}%{_CompPath}/bin
mkdir -p %{buildroot}%{_CompPath}/conf
mkdir -p %{buildroot}%{_CompPath}/conf/tasktables
mkdir -p %{buildroot}%{_CompPath}/conf/ICE
mkdir -p %{buildroot}%{_CompPath}/conf/WAV
mkdir -p %{buildroot}%{_CompPath}/conf/WND
mkdir -p %{buildroot}%{_CompPath}/conf/TEC
mkdir -p %{buildroot}%{_CompPath}/conf/TRO
mkdir -p %{buildroot}%{_CompPath}/scripts
mkdir -p %{buildroot}%{_CompPath}/processors
mkdir -p %{buildroot}%{_CompPath}/processors/S3AuxReformatter
mkdir -p %{buildroot}%{_CompPath}/processors/S3AuxReformatter/conf
mkdir -p %{buildroot}%{_CompPath}/processors/S3AuxReformatter/tasktables
mkdir -p %{buildroot}%{_CompPath}/processors/S1AuxReformatter
mkdir -p %{buildroot}%{_CompPath}/processors/S1AuxReformatter/conf
mkdir -p %{buildroot}%{_CompPath}/processors/S1AuxReformatter/tasktables
mkdir -p %{buildroot}%{_CompPath}/processors/S1AuxReformatter/conf/ICE
mkdir -p %{buildroot}%{_CompPath}/processors/S1AuxReformatter/conf/WAV
mkdir -p %{buildroot}%{_CompPath}/processors/S1AuxReformatter/conf/WND
mkdir -p %{buildroot}%{_CompPath}/processors/S1AuxReformatter/conf/TEC
mkdir -p %{buildroot}%{_CompPath}/processors/S1AuxReformatter/conf/TRO

# Script to escape password strings
install -m 0755 %{_topdir}/../scripts/Utility/escPaswdString.sh                                               %{buildroot}%{_CompPath}/scripts/escPaswdString.sh

# S3 Reformatting processor
install -m 0775 %{_topdir}/../applications/S3AuxReformatterCore/S3AuxReformatterCore                          %{buildroot}%{_CompPath}/bin/S3AuxReformatterCore
install -m 0775 %{_topdir}/../applications/S3AuxReformMetadataExtractor/S3AuxReformMetadataExtractor          %{buildroot}%{_CompPath}/bin/S3AuxReformMetadataExtractor
install -m 0775 %{_topdir}/../ADGSConf/confSpaces/S3AuxReformatterCoreConf.xml                                %{buildroot}%{_CompPath}/processors/S3AuxReformatter/conf/S3AuxReformatterCoreConf.xml
install -m 0775 %{_topdir}/../ADGSConf/taskTables/TaskTable.S3_AUX_REFORMATTER.xml                            %{buildroot}%{_CompPath}/processors/S3AuxReformatter/tasktables/TaskTable.S3_AUX_REFORMATTER.xml
install -m 0775 %{_topdir}/../applications/S3AuxReformatterCore/conf/WSConfig/WSConfig.xml                    %{buildroot}%{_CompPath}/processors/S3AuxReformatter/conf/WSConfig.xml

# S1 Reformatting processor
install -m 0775 %{_topdir}/../applications/S1AuxReformatter/S1AuxReformatter                                  %{buildroot}%{_CompPath}/bin/S1AuxReformatter

install -m 0775 %{_topdir}/../applications/S1AuxReformatter/conf/xml/ProductNaming.xml                        %{buildroot}%{_CompPath}/processors/S1AuxReformatter/conf/ProductNaming.xml
install -m 0775 %{_topdir}/../applications/S1AuxReformatter/conf/xml/S1AuxReformatter.xml                     %{buildroot}%{_CompPath}/processors/S1AuxReformatter/conf/S1AuxReformatter.xml
install -m 0775 %{_topdir}/../applications/S1AuxReformatter/conf/xml/S1_AUX_RFMT_CONF_01.xml                  %{buildroot}%{_CompPath}/processors/S1AuxReformatter/conf/S1_AUX_RFMT_CONF_01.xml
install -m 0775 %{_topdir}/../applications/S1AuxReformatter/conf/xml/TaskTables/TaskTable.S1_AUX_ICE_RFMT.xml %{buildroot}%{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_ICE_RFMT.xml
install -m 0775 %{_topdir}/../applications/S1AuxReformatter/conf/xml/TaskTables/TaskTable.S1_AUX_WAV_RFMT.xml %{buildroot}%{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_WAV_RFMT.xml
install -m 0775 %{_topdir}/../applications/S1AuxReformatter/conf/xml/TaskTables/TaskTable.S1_AUX_WND_RFMT.xml %{buildroot}%{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_WND_RFMT.xml
install -m 0775 %{_topdir}/../applications/S1AuxReformatter/conf/xml/TaskTables/TaskTable.S1_AUX_TEC_RFMT.xml %{buildroot}%{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_TEC_RFMT.xml
install -m 0775 %{_topdir}/../applications/S1AuxReformatter/conf/xml/TaskTables/TaskTable.S1_AUX_TRO_RFMT.xml %{buildroot}%{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_TRO_RFMT.xml
install -m 0775 %{_topdir}/../applications/S1AuxReformatter/conf/xml/ICE/SafeFormatter.xml                    %{buildroot}%{_CompPath}/processors/S1AuxReformatter/conf/ICE/SafeFormatter.xml
install -m 0775 %{_topdir}/../applications/S1AuxReformatter/conf/xml/WAV/SafeFormatter.xml                    %{buildroot}%{_CompPath}/processors/S1AuxReformatter/conf/WAV/SafeFormatter.xml
install -m 0775 %{_topdir}/../applications/S1AuxReformatter/conf/xml/WND/SafeFormatter.xml                    %{buildroot}%{_CompPath}/processors/S1AuxReformatter/conf/WND/SafeFormatter.xml
install -m 0775 %{_topdir}/../applications/S1AuxReformatter/conf/xml/TEC/SafeFormatter.xml                    %{buildroot}%{_CompPath}/processors/S1AuxReformatter/conf/TEC/SafeFormatter.xml
install -m 0775 %{_topdir}/../applications/S1AuxReformatter/conf/xml/TRO/SafeFormatter.xml                    %{buildroot}%{_CompPath}/processors/S1AuxReformatter/conf/TRO/SafeFormatter.xml
install -m 0775 %{_topdir}/../applications/S1AuxReformatter/conf/WSConfig/WSConfig.xml                        %{buildroot}%{_CompPath}/processors/S1AuxReformatter/conf/WSConfig.xml

ln -s %{_CompPath}/processors/S3AuxReformatter/conf/S3AuxReformatterCoreConf.xml           .%{_CompPath}/conf/S3AuxReformatterCoreConf.xml
ln -s %{_CompPath}/processors/S3AuxReformatter/tasktables/TaskTable.S3_AUX_REFORMATTER.xml .%{_CompPath}/conf/tasktables/TaskTable.S3_AUX_REFORMATTER.xml

ln -s %{_CompPath}/processors/S1AuxReformatter/conf/ProductNaming.xml                      .%{_CompPath}/conf/ProductNaming.xml
ln -s %{_CompPath}/processors/S1AuxReformatter/conf/S1AuxReformatter.xml                   .%{_CompPath}/conf/S1AuxReformatter.xml
ln -s %{_CompPath}/processors/S1AuxReformatter/conf/S1_AUX_RFMT_CONF_01.xml                .%{_CompPath}/conf/S1_AUX_RFMT_CONF_01.xml
ln -s %{_CompPath}/processors/S1AuxReformatter/conf/ICE/SafeFormatter.xml                  .%{_CompPath}/conf/ICE/SafeFormatter.xml
ln -s %{_CompPath}/processors/S1AuxReformatter/conf/WAV/SafeFormatter.xml                  .%{_CompPath}/conf/WAV/SafeFormatter.xml
ln -s %{_CompPath}/processors/S1AuxReformatter/conf/WND/SafeFormatter.xml                  .%{_CompPath}/conf/WND/SafeFormatter.xml
ln -s %{_CompPath}/processors/S1AuxReformatter/conf/TEC/SafeFormatter.xml                  .%{_CompPath}/conf/TEC/SafeFormatter.xml
ln -s %{_CompPath}/processors/S1AuxReformatter/conf/TRO/SafeFormatter.xml                  .%{_CompPath}/conf/TRO/SafeFormatter.xml
ln -s %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_ICE_RFMT.xml    .%{_CompPath}/conf/tasktables/TaskTable.S1_AUX_ICE_RFMT.xml
ln -s %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_WAV_RFMT.xml    .%{_CompPath}/conf/tasktables/TaskTable.S1_AUX_WAV_RFMT.xml
ln -s %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_WND_RFMT.xml    .%{_CompPath}/conf/tasktables/TaskTable.S1_AUX_WND_RFMT.xml 
ln -s %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_TEC_RFMT.xml    .%{_CompPath}/conf/tasktables/TaskTable.S1_AUX_TEC_RFMT.xml 
ln -s %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_TRO_RFMT.xml    .%{_CompPath}/conf/tasktables/TaskTable.S1_AUX_TRO_RFMT.xml 


%files
#%dir %attr(-,root,root) /usr/local
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/bin
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/conf
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/conf/tasktables
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/conf/ICE
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/conf/WAV
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/conf/WND
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/conf/TEC
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/conf/TRO
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/processors
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S3AuxReformatter
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S3AuxReformatter/conf
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S3AuxReformatter/tasktables
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/conf
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/tasktables
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/conf/ICE
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/conf/WAV
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/conf/WND
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/conf/TEC
%dir %attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/conf/TRO

%attr(-,root,%{_adgsgroup}) %{_CompPath}/scripts/escPaswdString.sh
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/S3AuxReformatterCore
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/S3AuxReformMetadataExtractor
%attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S3AuxReformatter/conf/S3AuxReformatterCoreConf.xml
%attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S3AuxReformatter/tasktables/TaskTable.S3_AUX_REFORMATTER.xml
%attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S3AuxReformatter/conf/WSConfig.xml
%attr(-,root,%{_adgsgroup}) %{_CompPath}/bin/S1AuxReformatter
%attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/conf/ProductNaming.xml
%attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/conf/S1AuxReformatter.xml
%attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/conf/S1_AUX_RFMT_CONF_01.xml
%attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_ICE_RFMT.xml
%attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_WAV_RFMT.xml
%attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_WND_RFMT.xml
%attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_TEC_RFMT.xml
%attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_TRO_RFMT.xml
%attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/conf/ICE/SafeFormatter.xml
%attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/conf/WAV/SafeFormatter.xml
%attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/conf/WND/SafeFormatter.xml
%attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/conf/TEC/SafeFormatter.xml
%attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/conf/TRO/SafeFormatter.xml
%attr(-,root,%{_adgsgroup}) %{_CompPath}/processors/S1AuxReformatter/conf/WSConfig.xml

%{_CompPath}/conf/S3AuxReformatterCoreConf.xml
%{_CompPath}/conf/tasktables/TaskTable.S3_AUX_REFORMATTER.xml
%{_CompPath}/conf/ProductNaming.xml
%{_CompPath}/conf/S1AuxReformatter.xml
%{_CompPath}/conf/S1_AUX_RFMT_CONF_01.xml
%{_CompPath}/conf/ICE/SafeFormatter.xml
%{_CompPath}/conf/WAV/SafeFormatter.xml
%{_CompPath}/conf/WND/SafeFormatter.xml
%{_CompPath}/conf/TEC/SafeFormatter.xml
%{_CompPath}/conf/TRO/SafeFormatter.xml
%{_CompPath}/conf/tasktables/TaskTable.S1_AUX_ICE_RFMT.xml
%{_CompPath}/conf/tasktables/TaskTable.S1_AUX_WAV_RFMT.xml
%{_CompPath}/conf/tasktables/TaskTable.S1_AUX_WND_RFMT.xml 
%{_CompPath}/conf/tasktables/TaskTable.S1_AUX_TEC_RFMT.xml 
%{_CompPath}/conf/tasktables/TaskTable.S1_AUX_TRO_RFMT.xml 

%post
if [ "$1" -eq 1 ]; then
    escPath=$(%{_CompPath}/scripts/escPaswdString.sh %{_CompPath})
    sed -i "s/{homeconf}/$escPath/g" %{_CompPath}/processors/S3AuxReformatter/conf/WSConfig.xml
    sed -i "s/{home}/$escPath/g"     %{_CompPath}/processors/S3AuxReformatter/tasktables/TaskTable.S3_AUX_REFORMATTER.xml

    su - %{_adgsuser} -c "%{_CompPath}/bin/WSConfigMerger.sh -a %{_CompPath}/processors/S3AuxReformatter/conf/WSConfig.xml"
    if [ "$?" -ne 0 ]; then exit 1; fi

    sed -i "s/{homeconf}/$escPath/g" %{_CompPath}/processors/S1AuxReformatter/conf/WSConfig.xml
    sed -i "s/{homeconf}/$escPath/g" %{_CompPath}/processors/S1AuxReformatter/conf/S1_AUX_RFMT_CONF_01.xml
    sed -i "s/{homeconf}/$escPath/g" %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_ICE_RFMT.xml
    sed -i "s/{homeconf}/$escPath/g" %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_WAV_RFMT.xml
    sed -i "s/{homeconf}/$escPath/g" %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_WND_RFMT.xml
    sed -i "s/{homeconf}/$escPath/g" %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_TEC_RFMT.xml
    sed -i "s/{homeconf}/$escPath/g" %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_TRO_RFMT.xml
    sed -i "s/{home}/$escPath/g"     %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_ICE_RFMT.xml
    sed -i "s/{home}/$escPath/g"     %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_WAV_RFMT.xml
    sed -i "s/{home}/$escPath/g"     %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_WND_RFMT.xml
    sed -i "s/{home}/$escPath/g"     %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_TEC_RFMT.xml
    sed -i "s/{home}/$escPath/g"     %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_TRO_RFMT.xml

    su - %{_adgsuser} -c "%{_CompPath}/bin/WSConfigMerger.sh -a %{_CompPath}/processors/S1AuxReformatter/conf/WSConfig.xml"
    if [ "$?" -ne 0 ]; then exit 1; fi

fi

if [ "$1" -eq 2 ]; then
    escPath=$(%{_CompPath}/scripts/escPaswdString.sh %{_CompPath})
    sed -i "s/{homeconf}/$escPath/g" %{_CompPath}/processors/S3AuxReformatter/conf/WSConfig.xml
    sed -i "s/{home}/$escPath/g"     %{_CompPath}/processors/S3AuxReformatter/tasktables/TaskTable.S3_AUX_REFORMATTER.xml

    sed -i "s/{homeconf}/$escPath/g" %{_CompPath}/processors/S1AuxReformatter/conf/WSConfig.xml
    sed -i "s/{homeconf}/$escPath/g" %{_CompPath}/processors/S1AuxReformatter/conf/S1_AUX_RFMT_CONF_01.xml
    sed -i "s/{homeconf}/$escPath/g" %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_ICE_RFMT.xml
    sed -i "s/{homeconf}/$escPath/g" %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_WAV_RFMT.xml
    sed -i "s/{homeconf}/$escPath/g" %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_WND_RFMT.xml
    sed -i "s/{homeconf}/$escPath/g" %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_TEC_RFMT.xml
    sed -i "s/{homeconf}/$escPath/g" %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_TRO_RFMT.xml
    sed -i "s/{home}/$escPath/g"     %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_ICE_RFMT.xml
    sed -i "s/{home}/$escPath/g"     %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_WAV_RFMT.xml
    sed -i "s/{home}/$escPath/g"     %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_WND_RFMT.xml
    sed -i "s/{home}/$escPath/g"     %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_TEC_RFMT.xml
    sed -i "s/{home}/$escPath/g"     %{_CompPath}/processors/S1AuxReformatter/tasktables/TaskTable.S1_AUX_TRO_RFMT.xml

fi

%preun
if [ "$1" -eq 0 ]; then
    su - %{_adgsuser} -c "%{_CompPath}/bin/WSConfigMerger.sh -d %{_CompPath}/processors/S3AuxReformatter/conf/WSConfig.xml"
    if [ "$?" -ne 0 ]; then exit 1; fi
    su - %{_adgsuser} -c "%{_CompPath}/bin/WSConfigMerger.sh -d %{_CompPath}/processors/S1AuxReformatter/conf/WSConfig.xml"
    if [ "$?" -ne 0 ]; then exit 1; fi
fi

%clean
rm -rf %{buildroot} %{rpmPkgName}
