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









usage() {
	echo "usage: loadContr <hostname> <dbauser> <dbname> <contrib> [pgsql version]"
	echo $dash_line
	echo ""
	echo "contrib:      |  functions included           |"
	echo "--------------|-------------------------------|"
	echo "   lo         |  lo_oid(lo)                   |"
	echo "              |  lo_manage()                  |"
	echo "   uuid-ossp  |  uuid_generate_v1()           |"
	echo "              |  uuid_generate_v1mc()         |"
	echo "              |  uuid_generate_v3(uuid, text) |"
	echo "              |  uuid_generate_v4()           |"
	echo "              |  uuid_generate_v5(uuid, text) |"
	echo "--------------|-------------------------------|"
	echo ""
	echo "pgsql versions:"
	echo "   8.3      PostgreSQL 8.3"
	echo "   8.4      PostgreSQL 8.4"
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




echo ""
echo "PSQL LOAD CONTRIBUTOR"
export dash_line="------------------------------------------------------------------------"
echo $dash_line

export h_name="$1"
export dba_usr="$2"
export db_name="$3"
export cntr_name="$4"
export pgsql_vrsn="$5"

if test "$1" = "help" || test "$1" = "--help"
then
	usage
fi

case "$cntr_name" in
	'lo')
		export module=$cntr_name ;;
	'uuid-ossp')
		export module=$cntr_name ;;
	*)
		usage ;;
esac

case "$pgsql_vrsn" in
	'9.0')
		export CONTR_DIR=/usr/pgsql-9.0/share/contrib
		export contr="$CONTR_DIR/$module" 
		export execution="load"
		;; 
	'9.1' | '9.2' | '9.3' | '9.4' | '9.5' | '9.6' | '10' | '11' | '12' | '13')
		export CONTR_DIR=/usr/pgsql-$pgsql_vrsn/share/extension
		export execution="command"
		;;
	*)
		export CONTR_DIR=/usr/share/pgsql/contrib
		export contr="$CONTR_DIR/$module"
		export execution="load"
		;;
esac


if test -z "$PGPASSWORD"
then
	echo -n "Insert $dba_usr password: "
	read -s answer
	export PGPASSWORD=$answer
	echo ""
fi

echo "Loading contributor $module on database $db_name ..."

if test $execution == "load"
then
	psql -h $h_name -U $dba_usr $db_name -v "ON_ERROR_STOP=1" -c "\i $contr.sql"
	psql_result="$?"
else
	psql -h $h_name -U $dba_usr $db_name -v "ON_ERROR_STOP=1" -c "CREATE EXTENSION IF NOT EXISTS \"$module\" ;"
	psql_result="$?"
fi

if [ "$psql_result" != 0 ]
then
	echo $dash_line
	echo "Error during contributor load."
	echo ""
	exit 1
fi

echo $dash_line
echo "Contributor loaded."
echo ""
exit 0



