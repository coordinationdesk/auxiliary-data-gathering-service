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
#   $Prod: A.C.S. Databases drop script $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.1.1.1  2009/02/05 10:38:46  achnat
#	Import psqlCreateDatabase
#	








dd_usage() {
	echo "usage: dropDb <hostname> <dbauser> <dbname>"
	echo "-------------------------------------------"
	echo ""
	exit 1
}



if test "$1" = "help" || test "$1" = "--help"
then
	dd_usage
fi

if test -z "$3"
then
	dd_usage
else
	dbname=$3
fi



echo ""
echo "PSQL DROP DATABASE"
echo "-------------------------------------------"

if test -z "$PGPASSWORD"
then
	echo -n "Insert $2 password: "
	read -s answer
	export PGPASSWORD=$answer
	echo ""
fi

echo "Dropping database $dbname ..."

dropdb -h $1 -U $2 $dbname
if [ "$?" != 0 ]
then
	echo "-------------------------------------------"
	echo "Error during drop database."
	echo ""
	exit 1
else
	echo "-------------------------------------------"
	echo "Database dropped."
	echo ""
	exit 0
fi


