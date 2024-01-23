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
#   $Prod: A.C.S. Postgres: Refresh Materialized Views script $
#
#	$Id$
#
#	$Author$
#
#	$Log$





usage() {
	echo "usage: $(basename $0)  <conn_param> <view_name> [[options]..]"
	echo $dashLine
	echo "connection parameters:"
	echo "   -p <project>   Use connection parameters of a specific project"
	echo "   -z             No project. Insert connection parameters manually"
	echo "                  when requested"
	echo "views:"
	echo "   -v <view_name> mv_tableddname" 
	echo ""
	echo "options:"
	echo "   -h             Show this help and exit"
	echo $dashLine
	echo ""
	exit 1
}


res_msg() {
	case "$1" in
		'0')
			echo_success
			echo ""
			;;
		'1')
			echo_failure
			echo ""
			;;
		'2')
			echo_warning
			echo ""
			;;
		+ )
			;;
	esac
}


clean() {
	if test -e $SQL_FILE; then rm -f $SQL_FILE; fi
	if test -e $RES_FILE; then rm -f $RES_FILE; fi
}


exit_error() {
	echo "" >> $LOG_FILE
	echo $dashLine | tee -a $LOG_FILE
	echo "*** REFRESH ABORTED ***" | tee -a $LOG_FILE
	echo "See log file $LOG_FILE"
	echo "" | tee -a $LOG_FILE
	clean
	exit 1
}


logger() {
	case $1 in
		'-B')
			echo -n "$2"
			echo "$(date +'%Y-%m-%d %H:%M:%S.%N') $2" >> $LOG_FILE
			;;
		'-L')
			echo "$(date +'%Y-%m-%d %H:%M:%S.%N') $2" >> $LOG_FILE
			;;
		* )
			;;
	esac
}


init_file() {
	if ! test -e $2
	then
		touch $2
		if [ "$?" != 0 ]
		then
			echo "*** ERROR!!! init_file() - Cannot create $1 file ***"
			echo "trying to write: $2"
			echo ""
			exit 1
		fi
		chmod a+w $2
	fi
	
	case $1 in
		'log')
			echo "" >> $2
			echo "" >> $2
			echo "###################################################################" >> $2
			echo "# $(date)" >> $2
			echo "###################################################################" >> $2
			;;
		* )
			echo "" > $2
			;;
	esac
}


set_conf() {
	if test -r "$CONF_FILE"
	then
		. $CONF_FILE

		export DBHOSTNAME
		export DBNAME
		export DBAUSER
	else
		echo "*** Cannot find CONF file or it is not readable ***" | tee -a $LOG_FILE
		echo "expected file: $CONF_FILE"
		exit_error
	fi
}


check_insert() {
	toAnalize="$2"
	
	while test -z "$toAnalize"
	do
		echo -n "Insert postgres $1 name: "
		read toAnalize
	done
	
	export validated="$toAnalize"
}


insert_param() {
	case $1 in
		'srv' )
			echo -n "Insert postgres host name: "
			read val
			check_insert host $val
			export DBHOSTNAME="$validated"
			;;
		'usr' )
			echo -n "Insert postgres superuser name: "
			read val
			check_insert user $val
			export DBAUSER="$validated"
			;;
		'db' )
			echo -n "Insert postgres database name: "
			read val
			check_insert database $val
			export DBNAME="$validated"
			;;
		* ) 
			;;
	esac
}


check_conn() {
	logger -B "Checking connection on $DBNAME ..."
	
	psql -h $DBHOSTNAME -U $DBAUSER $DBNAME -c "\l" 2>> $LOG_FILE 1> /dev/null
	if [ "$?" != 0 ]
	then
		res_msg 1
		echo "*** ERROR!!! Cannot connect to $DBNAME ***" | tee -a $LOG_FILE
		exit_error
	fi
	
	logger -L ">> connection done" >> $LOG_FILE
	res_msg 0
}


exe_qry() {
	logger -B "Executing query ..."
	
	psql -h $DBHOSTNAME -U $DBAUSER $DBNAME -v "ON_ERROR_STOP=1" -t -c "$1" 2>> $LOG_FILE 1> $RES_FILE
	if [ "$?" != 0 ]
	then
		res_msg 1
		echo "*** ERROR!!! Error during query execution ***" | tee -a $LOG_FILE
		echo -e "query is: \n$(echo $1)" >> $LOG_FILE
		exit_error
	fi
	
	logger -L "  >> query executed successfully"
	res_msg 0
}





################################   MAIN   ################################
##########################################################################

# Source function library
. /etc/rc.d/init.d/functions

export dashLine="-------------------------------------------------------------------"
echo ""
echo "POSTGRES REFRESH MATERIALIZED VIEWS"
echo $dashLine

export EXE_DIR=`dirname $0`
cd $EXE_DIR
export EXE_DIR=`pwd`

cd ..
export BASE_DIR=`pwd`
cd $EXE_DIR

export CONF_DIR=$BASE_DIR/psqlUtilitiesConf

export LOG_FILE=/tmp/$(basename $0).log
export SQL_FILE=/tmp/$(basename $0)_tempCmd.sql
export RES_FILE=/tmp/$(basename $0)_tempResult.res

export p_flag="F"
export v_flag="F"
export z_flag="F"

while getopts ":p:v:hz" opt
do 
	case "${opt}" in 
		p) 
			export p_flag="T"
			export prj=${OPTARG}
			;;
		v) 
			export v_flag="T"
			export mv_name=${OPTARG}
			;;
		z) 
			export z_flag="T"
			;;
		h) 
			usage
			;;
		*) 
			usage 
			;;
	esac 
done

if [ "$v_flag" = "F" ]; then usage; fi
if [ "$p_flag" = "F" ] && [ "$z_flag" = "F" ]; then usage; fi
if [ "$p_flag" = "T" ] && [ "$z_flag" = "T" ]; then usage; fi

case "$mv_name" in
	'mv_tableddname' ) ;;
	*) usage ;;
esac

clean

init_file log $LOG_FILE

if [ "$p_flag" = "T" ]
then
	export CONF_FILE=$CONF_DIR/$prj.conf
	set_conf
else
	insert_param srv
	insert_param db
	insert_param usr
fi

if test -z "$PGPASSWORD"
then
	echo -n "Insert $DBAUSER password: "
	read -s answer
	export PGPASSWORD=$answer
	echo ""
fi

check_conn

exe_qry "REFRESH MATERIALIZED VIEW $mv_name ;"

echo $dashLine | tee -a $LOG_FILE
echo ">> REFRESH COMPLETED" | tee -a $LOG_FILE
echo "" | tee -a $LOG_FILE
clean
exit 0




