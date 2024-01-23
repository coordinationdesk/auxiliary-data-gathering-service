#!/bin/sh
#
# Copyright 1995-2005, Advanced Computer Systems , Inc.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.
# 
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
# the contents of this file may not be disclosed to third parties, copied or
# duplicated in any form, in whole or in part, without the prior written
# permission of Advanced Computer Systems, S.p.A.
#	
#
#   $Prod: A.C.S. Language loader script $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.1.1.1  2009/02/05 10:38:46  achnat
#	Import psqlCreateDatabase
#	








ll_usage() {
	echo "usage: loadLang <hostname> <dbauser> <dbname> <language_type>"
	echo "-------------------------------------------------------------"
	echo "language types:"
	echo "   plpgsql"
	echo "   plpythonu"
	echo "-------------------------------------------------------------"
	echo ""
	exit 1
}



echo ""
echo "PSQL LANGUAGE LOADER"
echo "-------------------------------------------------------------"

if test "$1" = "help" || test "$1" = "--help"
then
	ll_usage
fi

if test -z "$4"
then
	ll_usage
fi

if test -z "$PGPASSWORD"
then
	echo -n "Insert $2 password: "
	read -s answer
	export PGPASSWORD=$answer
	echo ""
fi

echo "Loading language $4 on database $3 ..."

psql -h $1 -U $2 $3 -v "ON_ERROR_STOP=1" -c "CREATE LANGUAGE $4"
if [ "$?" != 0 ]
then
	echo "-------------------------------------------------------------"
	echo "Error in loading language."
	echo ""
	exit 1
else
	echo "-------------------------------------------------------------"
	echo "Language loaded."
	echo ""
	exit 0
fi


