#!/bin/bash
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
	echo "usage: createDatabase <hostname> <dbauser> <dbname> [owner] [dbspace]"
	echo "---------------------------------------------------------------------"
	echo ""
	exit 1
}



echo ""
echo "PSQL DATABASE CREATION"
echo "---------------------------------------------------------------------"

if test "$1" = "help" || test "$1" = "--help"; then usage; fi
if test -z "$3"; then usage; fi

h_name="$1"
su_name="$2"
db_name="$3"

if test -z "$PGPASSWORD"
then
	echo -n "Insert $su_name password: "
	read -s answer
	export PGPASSWORD=$answer
	echo ""
fi

if [ -n "$5" ]
then
	COMMAND="createdb -h $h_name -U $su_name -O $4 -D $5 $db_name"
elif [ -n "$4" ]
then
	COMMAND="createdb -h $h_name -U $su_name -O $4 $db_name"
else
	COMMAND="createdb -h $h_name -U $su_name $db_name"
fi

echo "Running creation of database $db_name ..."
echo "command is: $COMMAND"
eval "${COMMAND[@]}"
if [ "$?" != 0 ]
then
	echo "---------------------------------------------------------------------"
	echo "Error during database creation."
	echo ""
	exit 1
else
	echo "---------------------------------------------------------------------"
	echo "Database creation completed."
	echo ""
	exit 0
fi


