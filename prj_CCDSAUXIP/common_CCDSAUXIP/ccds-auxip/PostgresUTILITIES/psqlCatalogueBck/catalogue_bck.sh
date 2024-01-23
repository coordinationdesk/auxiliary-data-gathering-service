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
#   $Prod: A.C.S. Postgres Catalogue Dump Procedure $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.3  2014/05/15 14:14:48  achnat
#	Added different users for databases
#	
#	Revision 1.2  2014/01/28 09:46:26  achnat
#	Added degreeusers
#	
#	Revision 1.1.1.1  2014/01/27 15:33:39  achnat
#	Import psqlCatalogueBck
#	





dump() {
	echo "" >> $LOG_FILE
	echo "BACKUP OF $1" >> $LOG_FILE
	echo "------------------------------" >> $LOG_FILE
	
	case $1 in
		'CNMCMDB' ) USR=cnmcm_admin ;;
		'CNMDB' ) USR=cnm_admin ;;
		'CSDB' ) USR=cnm_admin ;;
		'deegreebrim' ) USR=deegreebrim ;;
		'deegreeusers' ) USR=deegreeusers ;;
		* ) ;;
	esac
	
	echo "Connection parameters:" >> $LOG_FILE
	echo "  hostname: $HNAME" >> $LOG_FILE
	echo "  username: $USR" >> $LOG_FILE
	
	echo -n "Running dump of $1 ..."
	echo "Running dump ..." >> $LOG_FILE	
	pg_dump -c -o -h $HNAME -U $USR --format=t $1 -f $1_BkpFile_$(date +"%Y-%m-%d").tar >> $LOG_FILE 2>&1
	if [ "$?" != 0 ]
	then
		errCount=errCount+1
		echo_failure
		echo ""
	else
		echo ">> backup of $1 done" >> $LOG_FILE
		echo_success
		echo ""
	fi
}





echo ""
echo "PSQL CATALOGUE BACKUP PREOCEDURE"
echo "-------------------------------------------------------------------"

typeset -i errCount
errCount=0

. /etc/rc.d/init.d/functions 
export LOG_FILE=/tmp/catalogue_bck.log

export HNAME=cnm-cat
export PGPASSWORD=estestest

echo -e "\n\n\n" >> $LOG_FILE
echo "##################################################" >> $LOG_FILE
echo "# $(date)" >> $LOG_FILE
echo "##################################################" >> $LOG_FILE

dump CNMCMDB
dump CNMDB
dump CSDB
dump deegreebrim
dump deegreeusers

echo "-------------------------------------------------------------------"
echo -e "\n##################################################" >> $LOG_FILE
if (( "$errCount" > 0 ))
then
	echo ">> $errCount errors occurred." | tee -a $LOG_FILE
	echo "See log file $LOG_FILE"
	echo "" | tee -a $LOG_FILE
	exit 1
fi

echo ">> backup successfully completed." | tee -a $LOG_FILE
echo "" | tee -a $LOG_FILE
echo ""
exit 0


