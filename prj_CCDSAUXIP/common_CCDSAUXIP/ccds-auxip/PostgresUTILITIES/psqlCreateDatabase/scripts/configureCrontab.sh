#!/bin/sh
#
# Copyright 2005-2010, Advanced Computer Systems , Inc.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.
# 
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
# the contents of this file may not be disclosed to third parties, copied or
# duplicated in any form, in whole or in part, without the prior written
# permission of Advanced Computer Systems, S.p.A.
#	
#
#   $Prod: A.C.S. Crontab configuration Procedure $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.3  2013/10/29 14:46:24  achnat
#	Changed exit code.
#	
#	Revision 1.2  2013/10/07 13:01:42  achnat
#	Fixed bug on already configured crontab.
#	
#	Revision 1.1  2013/09/26 15:01:25  achnat
#	Added configureCrontab
#	








usage() {
	echo "usage: configureCrontab <hostname> <dbname> <script_file> [options]"
	echo "---------------------------------------------------------------------------"
	echo "scripts name list:"
	echo "   analyzeTables         execute ANALYZE command on all db tables"
	echo "   reindexTables         execute REINDEX command on all db tables"
	echo "   vacuumTables          execute VACUUM command on all db tables"
	echo ""
	echo "options:"
	echo "   -f | --force          Overwrite existing crontab for same application"
	echo "   -h | --help           Show this help and exit"
	echo "---------------------------------------------------------------------------"
	echo ""
	exit 1
}

check_file() {
	if ! test -r "$script_file"
	then
		echo "*** ERROR!!! Cannot find $(basename $script_file) or it is not readable ***"
		echo "searching path: $(dirname $script_file)"
		echo ""
		exit 1
	fi
}

check_cron() {
	crontab -u postgres -l 2>&1 | grep -i $script_name | grep -i $db_name > /dev/null 2>&1
	if  [ "$?" = '0' ]
	then
		echo "*** WARNING!!! $script_name for $db_name already configured ***"
		echo ""
		exit 0
	fi
}

config_cron() {
	echo "Configuring crontab for $(basename $script_file) ..."
	sleep 1
	
	crontab -u postgres -l > /tmp/tmpCron.txt 2>&1
	if test -n "$(grep crontab /tmp/tmpCron.txt)"; then cat /dev/null > /tmp/tmpCron.txt; fi	
	echo "$run_setting $script_file $h_name postgres $db_name -a &> /dev/null" >> /tmp/tmpCron.txt
	crontab -u postgres /tmp/tmpCron.txt
	if [ "$?" != 0 ]
	then
		echo "---------------------------------------------------------------------------"
		echo "*** Error during $script_name crontab configuration ***"
		echo ""
		exit 1
	fi
	
	rm -f /tmp/tmpCron.txt
}





echo ""
echo "Crontab configuration"
echo "---------------------------------------------------------------------------"

if test "`whoami`" != "root"
then
	echo "*** You must be root to run this script ***"
	echo ""
	exit 1
fi

if test -z "$3"
then
	usage
fi

if  [ "$4" = '-h' ] || [ "$4" = '--help' ]
then
	usage
fi

export EXE_DIR=`dirname $0`
cd $EXE_DIR
export EXE_DIR=`pwd`

export h_name="$1"
export db_name="$2"
export script_file="$3"
export script_name=$(basename $script_file)
export option="$4"

case $script_name in
	'analyzeTables')
		export run_setting="0 3,6,9,12,15,18,21,0 * * *"
		;;
	'reindexTables')
		export run_setting="* 2 * * *"
		;;
	'vacuumTables')
		export run_setting="* 1 * * *"
		;;
	* )
		usage
		;;
esac

check_file
check_cron
config_cron

echo "---------------------------------------------------------------------------"
echo "Crontab configuration successfully completed."
echo ""
exit 0


