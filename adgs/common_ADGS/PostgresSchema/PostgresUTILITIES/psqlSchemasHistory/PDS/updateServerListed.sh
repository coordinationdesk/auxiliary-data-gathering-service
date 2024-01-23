#!/bin/sh
#
# Copyright 2010-2020, Advanced Computer Systems , Inc.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.
# 
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
# the contents of this file may not be disclosed to third parties, copied or
# duplicated in any form, in whole or in part, without the prior written
# permission of Advanced Computer Systems, S.p.A.
#	
#
#   $Prod: A.C.S. Update servers db schema script $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.2  2014/04/29 13:02:16  achnat
#	Added new databases
#	
#	Revision 1.1  2013/09/10 10:40:06  achnat
#	Created script to install new delta scripts on all listed servers.
#	





usage() {
	echo "usage: updateServersListed <file_sql>"
	echo "-------------------------------------------------------------------"
	echo ""
	exit 1
}

result_message() {
	if [ "$1" != 0 ] 
	then 
		echo_failure
	else
		echo_success
	fi
	
	echo ""
}

checkConn() {
	psql -h $1 -U $DBAUSR $2 -c "\l" >> /dev/null 2>&1
	if [ "$?" != 0 ]
	then
		return 1
	fi
	return 0
}

runSql() {
	echo -n "Executing sql script on $2@$1 ..."
	checkConn $1 $2
	if [ "$?" != 0 ]
	then
		result_message 1
	else
		psql -h $1 -U $DBAUSR $2 -c "\i $SQL_CMD" >> /dev/null 2>&1
		if [ "$?" != 0 ]
		then
			result_message 1
		else
			result_message 0
		fi
	fi
}


echo ""
echo "UPDATE SERVERS LISTED"
echo "-------------------------------------------------------------------"

# Source function library
. /etc/rc.d/init.d/functions 

if test -z "$1"
then
	usage
fi

if ! test -r "$1"
then
	echo "*** ERROR!!! Cannot find input file or is not readable ***"
	echo ""
	exit 1
fi

export DBAUSR=acsdba
export PGPASSWORD=estestest
export SQL_CMD=$1

#runSql adc-dev adc
runSql s2pdgs-dev circulationdb
runSql s2pdgs-dev datastoragedb
#runSql s2pdgs-dev mccdb
runSql sentinel-sl6 sentinel1

echo "-------------------------------------------------------------------"
echo "UPDATE COMPLETED"
echo ""
exit 0


