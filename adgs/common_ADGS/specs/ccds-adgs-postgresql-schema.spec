#########################################################################
%define _ConfPath    /usr/local/conf/ADGS
%define _CompPath    /usr/local/components/ADGS
%define _InitPath    /usr/local/ADGS
%define _DataPath    /data/ADGS
%define _StorePath   /data/ADGS
%define _ElementName ccds-adgs-postgresql-schema
%define _Summary     The ADGS Schema for PostgreSQL
%define _License     ESA
%define _Group       Applications/%{_ElementName}
%define _Vendor      Exprivia
%define _BuildArch   noarch
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

%prep
%build

%install

umask 022
mkdir -p %{buildroot}%{_ConfPath}

umask 002
mkdir -p %{buildroot}%{_ConfPath}/PgsqlConfiguration
mkdir -p %{buildroot}%{_ConfPath}/PgsqlConfiguration/schema
mkdir -p %{buildroot}%{_ConfPath}/PgsqlConfiguration/schema/PostgresUTILITIES

cp -r %{_topdir}/../PostgresSchema/PostgresUTILITIES %{buildroot}%{_ConfPath}/PgsqlConfiguration/schema

%files
%attr(-,root,%{_adgsgroup}) %{_ConfPath}

%post

sed -i "s/^PGDATA=\/var\/lib\/pgsql.*/PGDATA=\/data\/databases\/pgsql13\/data   # Postgres Data directory path/g"  %{_ConfPath}/PgsqlConfiguration/schema/PostgresUTILITIES/psqlUtilitiesConf/adgs.conf
sed -i "s/^PGDATA=\/var\/lib\/pgsql.*/PGDATA=\/data\/databases\/pgsql13\/data   # Postgres Data directory path/g"  %{_ConfPath}/PgsqlConfiguration/schema/PostgresUTILITIES/psqlUtilitiesConf/kipp.conf

if [ "$1" -eq 1 ]; then

    /usr/local/conf/ADGS/PgsqlConfiguration/schema/PostgresUTILITIES/psqlCreateDatabase/createDatabase adgs adgsdb
#    psql -h localhost -U acsdba adgsdb -c "\i /usr/local/conf/ADGS/PgsqlConfiguration/schema/PostgresUTILITIES/psqlSchemasHistory/PDS/pds_v14.00.00_2021-09-29.sql"
    /usr/local/conf/ADGS/PgsqlConfiguration/schema/PostgresUTILITIES/psqlCreateDatabase/createDatabase kipp django
fi

%clean
rm -rf %{buildroot} %{rpmPkgName}

