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
#   $Prod: A.C.S. Tables initialization procedure $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.1.1.1  2009/02/05 10:38:46  achnat
#	Import psqlCreateDatabase
#	








it_usage() {
	echo ""
	echo "PSQL LOAD TABLE"
	echo "-----------------------------------------------------------"
	echo "usage: loadTable <hostname> <dbauser> <dbname> <table_name>"
	echo "-----------------------------------------------------------"
	echo ""
	exit 1
}



if test "$1" = "help" || test "$1" = "--help"
then
	it_usage
fi

if test -z "$4"
then
	it_usage
fi



echo ""
echo "Loading $4 on database $3 ..."

psql -h $1 -U $2 $3 -v "ON_ERROR_STOP=1" -c "\i $4"
if [ "$?" != 0 ]
then
	echo "Error during $4 load."
	echo ""
	exit 1
else
	echo "$4 loaded."
	echo ""
	exit 0
fi


