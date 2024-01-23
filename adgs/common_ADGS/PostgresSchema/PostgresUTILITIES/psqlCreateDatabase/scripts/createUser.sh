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
	echo "usage: createUser <hostname> <dbauser> <user_name> <user_passwd> [PSQL_VRS]"
	echo "---------------------------------------------------------------------------"
	echo ""
	exit 1
}



echo ""
echo "PSQL USER CREATION"
echo "---------------------------------------------------------------------------"

if test "$1" = "help" || test "$1" = "--help"; then usage; fi
if test -z "$4"; then usage; fi

export upwd="$4"

if test -z "$PGPASSWORD"
then
	echo -n "Insert $2 password: "
	read -s answer
	export PGPASSWORD=$answer
	echo ""
fi

echo "Creating user $3 ..."

if ! test -z "$(psql -h $1 -U $2 template1 -c "\du" | grep $3)"
then
	echo "---------------------------------------------------------------------------"
	echo ">> User $3 already exist."
	echo ""
	exit 0
fi

if [ "${upwd:0:3}" = "md5" ]
then
	psql -h $1 -U $2 -v "ON_ERROR_STOP=1" -d postgres -c "CREATE USER $3 WITH LOGIN NOSUPERUSER NOCREATEDB NOCREATEROLE ENCRYPTED PASSWORD '$upwd'"
	res="$?"
else
	psql -h $1 -U $2 -v "ON_ERROR_STOP=1" -d postgres -c "CREATE USER $3 WITH LOGIN NOSUPERUSER NOCREATEDB NOCREATEROLE PASSWORD '$upwd'"
	res="$?"
fi

if [ "$res" != 0 ]
then
	echo "---------------------------------------------------------------------------"
	echo ">> Error during user creation."
	echo ""
	exit 1
fi

if [ "${PSQL_VRS/.*}" -ge "11" ]
then
	psql -h $1 -U $2 -v "ON_ERROR_STOP=1" -d postgres -c "GRANT pg_read_server_files TO $3; GRANT pg_write_server_files TO $3;"
	if [ "$?" != 0 ]
	then
		echo "---------------------------------------------------------------------------"
		echo ">> Error during user grant assignment."
		echo ""
		exit 1
	fi
fi

echo "---------------------------------------------------------------------------"
echo ">> User created."
echo ""
exit 0

