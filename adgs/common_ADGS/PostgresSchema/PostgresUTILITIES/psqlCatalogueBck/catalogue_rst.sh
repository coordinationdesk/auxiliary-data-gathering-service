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
#   $Prod: A.C.S. Postgres Catalogue Restore Procedure $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.2  2014/01/28 09:46:26  achnat
#	Added degreeusers
#	
#	Revision 1.1  2014/01/27 16:45:16  achnat
#	Added script for restore
#	





crt_db() {
	echo -n "   Running creation of $1 ..."
	echo "create database:" >> $LOG_FILE	
	last_one=`ls -1 | grep $1 | tail -1`
	createdb -h $HNAME -U $USR $1 >> $LOG_FILE 2>&1
	if [ "$?" != 0 ]
	then
		errCount=errCount+1
		echo_failure
		echo ""
		return 1
	else
		echo ">> creation of $1 done" >> $LOG_FILE
		echo_success
		echo ""
		return 0
	fi
}	


rest() {
	echo -n "   Running load of dump for $1 ..."
	last_one=`ls -1 | grep $1 | tail -1`
	echo "load dump $last_one:" >> $LOG_FILE	
	pg_restore -h $HNAME -U $USR -d $1 $last_one >> $LOG_FILE 2>&1
	if [ "$?" != 0 ]
	then
		errCount=errCount+1
		echo_failure
		echo ""
	else
		echo ">> restore of $1 done" >> $LOG_FILE
		echo_success
		echo ""
	fi
}





echo ""
echo "PSQL CATALOGUE RESTORE PREOCEDURE"
echo "-------------------------------------------------------------------"

typeset -i errCount
errCount=0

. /etc/rc.d/init.d/functions 
export LOG_FILE=/tmp/catalogue_rst.log

export HNAME=cnm-cat
export PGPASSWORD=estestest

echo -e "\n\n\n" >> $LOG_FILE
echo "##################################################" >> $LOG_FILE
echo "# $(date)" >> $LOG_FILE
echo "##################################################" >> $LOG_FILE

for i in CNMDB CNMCMDB CSDB deegreebrim deegreeusers
do
	case $i in
		'CNMCMDB' ) USR=cnmcm_admin ;;
		'CNMDB' ) USR=cnm_admin ;;
		'CSDB' ) USR=cnm_admin ;;
		'deegreebrim' ) USR=deegreebrim ;;
		'deegreeusers' ) USR=deegreeusers ;;
		* ) ;;
	esac
	
	echo "" >> $LOG_FILE
	echo "Restore of $i:" | tee -a $LOG_FILE
	echo "------------------------------" >> $LOG_FILE
	echo "Connection parameters:" >> $LOG_FILE
	echo "  hostname: $HNAME" >> $LOG_FILE
	echo "  username: $USR" >> $LOG_FILE
	
	crt_db $i
	if [ "$?" != 1 ]
	then
		rest $i
	fi
done

echo "-------------------------------------------------------------------"
echo -e "\n##################################################" >> $LOG_FILE
if (( "$errCount" > 0 ))
then
	echo ">> $errCount errors occurred." | tee -a $LOG_FILE
	echo "See log file $LOG_FILE"
	echo "" | tee -a $LOG_FILE
	exit 1
fi

echo ">> restore successfully completed." | tee -a $LOG_FILE
echo "" | tee -a $LOG_FILE
echo ""
exit 0


