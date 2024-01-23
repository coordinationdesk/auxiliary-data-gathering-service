#!/bin/bash
#
# Copyright 2010-2020, Advanced Computer Systems , Inc.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.
# 
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
# the contents of this file may not be disclosed to third parties, copied or
# duplicated in any form, in whole or in part, without the prior written
# permission of Advanced Computer Systems, S.p.A.








usage() {
	echo "usage: createTablespace <hostname> <dbauser> <tbs_name> <tbs_path>"
	echo "------------------------------------------------------------------"
	echo ""
	exit 1
}



echo ""
echo "PSQL TABLESPACE CREATION"
echo "------------------------------------------------------------------"

if test "$1" = "help" || test "$1" = "--help"; then	usage; fi
if test -z "$4"; then usage; fi

h_name="$1"
s_user="$2"
t_name="$3"
t_path="$4"

if test -z "$PGPASSWORD"
then
	echo -n "Insert $s_user password: "
	read -s answer
	export PGPASSWORD=$answer
	echo ""
fi

echo "Creating tablespace $t_name in $t_path ..."

if ! test -z "$(psql -h $1 -U $2 template1 -c "SELECT * FROM pg_tablespace ;" | grep $t_name)"
then
	echo "------------------------------------------------------------------"
	echo ">> Tablespace $t_name already exist."
	echo ""
	exit 0
fi

psql -h $1 -U $2 -v "ON_ERROR_STOP=1" -c "CREATE TABLESPACE $t_name LOCATION '$t_path'; " template1
if [ "$?" != 0 ]
then
	echo "------------------------------------------------------------------"
	echo ">> Error during tablespace creation."
	echo ""
	exit 1
else
	echo "------------------------------------------------------------------"
	echo ">> Tablespace created."
	echo ""
	exit 0
fi


