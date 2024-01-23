#!/bin/sh
#
# Copyright 2015-2020, Advanced Computer Systems , Inc.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.
# 
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
# the contents of this file may not be disclosed to third parties, copied or
# duplicated in any form, in whole or in part, without the prior written
# permission of Advanced Computer Systems, S.p.A.









export EXE_DIR=`dirname $0`
cd $EXE_DIR
export EXE_DIR=`pwd`

usage() {
	echo "usage: registerPostGis <hostname> <dbauser> <dbname> <postgis_version> <pgsql_version>"
	echo $dash_line
	echo "postgis versions:"
	echo "   1.5      PostGIS 1.5"
	echo "   2.0      PostGIS 2.0"
	echo "   2.1      PostGIS 2.1"
	echo "   2.2      PostGIS 2.2"
	echo "   2.3      PostGIS 2.3"
	echo "   2.4      PostGIS 2.4"
	echo "   2.5      PostGIS 2.5"
	echo "   3.0      PostGIS 3.0"
	echo "   3.1      PostGIS 3.1"
	echo "pgsql versions:"
	echo "   9.0      PostgreSQL 9.0"
	echo "   9.1      PostgreSQL 9.1"
	echo "   9.2      PostgreSQL 9.2"
	echo "   9.3      PostgreSQL 9.3"
	echo "   9.4      PostgreSQL 9.4"
	echo "   9.5      PostgreSQL 9.5"
	echo "   9.6      PostgreSQL 9.6"
	echo "   10       PostgreSQL 10"
	echo "   11       PostgreSQL 11"
	echo "   12       PostgreSQL 12"
	echo "   13       PostgreSQL 13"
	echo $dash_line
	echo ""
	exit 1
}

check_file() {
	if ! test -r "$PGIS_DIR/$1"
	then
		echo "*** ERROR!!! Cannot find $1 or it is not readable ***" | tee -a $LOG_FILE
		echo "expected path: $PGIS_DIR" | tee -a $LOG_FILE
		echo ""
		exit 1
	fi
}

exe_qry() {
	if [ "$1" = "-f" ]
	then
		psql  -h $h_name -U $dba_usr $db_name -v "ON_ERROR_STOP=1" -c "\i $PGIS_DIR/$2"
		res="$?"
	else
		psql  -h $h_name -U $dba_usr $db_name -v "ON_ERROR_STOP=1" -c "$2"
		res="$?"
	fi
	
	if [ "$res" != 0 ]
	then
		echo $dash_line
		echo "*** Error during $1 load ***"
		echo ""
	exit 1
	fi
}





echo ""
echo "PostGIS DataBlade Registration"
export dash_line="--------------------------------------------------------------------------------------"
echo $dash_line

if test "$1" = "-h" || test "$1" = "--help"
then
	usage
fi

export h_name="$1"
export dba_usr="$2"
export db_name="$3"
export postgis_vrsn="$4"
export pgsql_vrsn="$5"

# Chek for PostGIS version
case "$postgis_vrsn" in
	'1.5') ;; 
	'2.0') ;;
	'2.1') ;;
	'2.2') ;;
	'2.3') ;;
	'2.4') ;;
	'2.5') ;;
	'3.0') ;;
	'3.1') ;;
	*) usage ;;
esac

# Chek for Postgres version
case "$pgsql_vrsn" in
	'9.0') ;; 
	'9.1') ;;
	'9.2') ;;
	'9.3') ;;
	'9.4') ;;
	'9.5') ;;
	'9.6') ;;
	'10') ;;
	'11') ;;
	'12') ;;
	'13') ;;
	*) usage ;;
esac

# Check for admin password
if test -z "$PGPASSWORD"
then
	echo -n "Insert $dba_usr password: "
	read -s answer
	export PGPASSWORD=$answer
	echo ""
fi

if (( $(echo "$pgsql_vrsn > 9.0" | bc -l) ))
then
	echo "Creating postgis extension ..."
	sleep 1
	exe_qry -c "CREATE EXTENSION postgis ;"
else
	# Set PostGIS directory
	export PGIS_DIR=/usr/pgsql-$pgsql_vrsn/share/contrib/postgis-$postgis_vrsn

	# Check for PostGIS sql files
	check_file postgis.sql
	check_file spatial_ref_sys.sql
	
	echo "Loading postgis.sql ..."
	sleep 1
	exe_qry -f postgis.sql
	
	echo "Loading spatial_ref_sys.sql ..."
	sleep 1
	exe_qry -f spatial_ref_sys.sql
fi

echo $dash_line
echo "PostGIS DataBlade registration successfully completed."
echo ""
exit 0


