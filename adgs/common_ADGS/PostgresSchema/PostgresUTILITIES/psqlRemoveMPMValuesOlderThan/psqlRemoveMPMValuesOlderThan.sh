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
#	
#
#   $Prod: A.C.S. Postgres: remove from db MPM values older than $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.2  2018/06/12 10:09:55  achnat
#	Added -r option
#	
#	Revision 1.1.1.1  2018/06/08 15:45:43  achnat
#	Import psqlRemoveMPMValuesOlderThan
#	








usage() {
	echo "usage: $(basename $0)  <conn_param> [[options]..]"
	echo $dashLine
	echo "connection parameters:"
	echo "   -p <project>   Use connection parameters of a specific project"
	echo "   -z             No project. Insert connection parameters manually"
	echo "                  when requested"
	echo "options:"
	echo "   -d <days>      Days from which the cleaning will starts. Default 180"
	echo "                  (eg. 30: will be removed items older than one month)"
	echo "   -f             Starts cleaning after search without prompt"
	echo "   -r <rows>      Number of rows deleted simultaneously for each loop"
	echo "                  (min 10; max 100; default 50)"
	echo "   -s <file>      Save the search query output into a file"
	echo "   -w <seconds>   Wait for defined seconds before next delete query"
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
	if test -e $RES_FILE; then rm -f $RES_FILE; fi
}


exit_error() {
	echo "" >> $LOG_FILE
	echo $dashLine | tee -a $LOG_FILE
	echo "*** REMOTION ABORTED ***" | tee -a $LOG_FILE
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
		export PDSUSER
		export PDSPASSWORD
		
		export PGPASSWORD=$PDSPASSWORD
	else
		echo "*** Cannot find CONF file or it is not readable ***" | tee -a $LOG_FILE
		echo "expected file: $CONF_FILE" | tee -a $LOG_FILE
		exit_error
	fi
}


check_insert() {
	toAnalize="$2"
	
	while test -z "$toAnalize"
	do
		echo -n "Insert $1 name: "
		read toAnalize
	done
	
	export validated="$toAnalize"
}


insert_param() {
	case $1 in
		'srv' )
			echo -n "Insert host name: "
			read val
			check_insert host $val
			export DBHOSTNAME="$validated"
			;;
		'usr' )
			echo -n "Insert user name: "
			read val
			check_insert user $val
			export PDSUSER="$validated"
			;;
		'psw' )
			echo -n "Insert user password: "
			read val
			check_insert password $val
			export PGPASSWORD="$validated"
			;;
		'db' )
			echo -n "Insert database name: "
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
	
	psql -h $DBHOSTNAME -U $PDSUSER $DBNAME -c "\l" 2>> $LOG_FILE 1> /dev/null
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
	logger -B "  executing $1 query $2..."
	
	if [ $1 = "select" ]
	then
		psql -h $DBHOSTNAME -U $PDSUSER $DBNAME -v "ON_ERROR_STOP=1" -t -c "$SQL_CMD" 2>> $LOG_FILE 1> $RES_FILE
		res="$?"
	else
		psql -h $DBHOSTNAME -U $PDSUSER $DBNAME -v "ON_ERROR_STOP=1" -t -c "$SQL_CMD" 2>> $LOG_FILE 1> /dev/null
		res="$?"
	fi
	
	if [ "$res" != 0 ]
	then
		res_msg 1
		echo "*** ERROR!!! Error during query execution ***" | tee -a $LOG_FILE
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
echo "POSTGRES REMOVE MPM VALUES OLDER THAN"
echo $dashLine

export EXE_DIR=`dirname $0`
cd $EXE_DIR
export EXE_DIR=`pwd`

cd ..
export BASE_DIR=`pwd`
cd $EXE_DIR

export CONF_DIR=$BASE_DIR/psqlUtilitiesConf
export LOG_FILE=/tmp/$(basename ${0%.*}).log
export RES_FILE="/tmp/$(basename ${0%.*})_"temp_result.txt

export d_flag="F"
export f_flag="F"
export p_flag="F"
export r_flag="F"
export s_flag="F"
export t_flag="F"
export w_flag="F"
export z_flag="F"

while getopts ":d:r::s:p:w:fhz" opt 
do 
	case "${opt}" in 
		d) 
			export d_flag="T"
			export day_num=${OPTARG}
			;;
		f) 
			export f_flag="T"
			;;
		p) 
			export p_flag="T"
			export prj=${OPTARG}
			;;
		r) 
			export r_flag="T"
			export del_rows=${OPTARG}
			;;
		s) 
			export s_flag="T"
			export save_file=${OPTARG}
			;;
		w) 
			export w_flag="T"
			export wait_sec=${OPTARG}
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

if [ "$p_flag" = "F" ] && [ "$z_flag" = "F" ]; then usage; fi
if [ "$p_flag" = "T" ] && [ "$z_flag" = "T" ]; then usage; fi
if [ "$d_flag" = "F" ]; then export day_num=180 ; fi
if [ "$d_flag" = "T" ] && ! [[ "$day_num" =~ ^[0-9]+$ ]]
then
	echo "*** ERROR!!! Invalid input value ***"
	echo "When you use the \"-d\" option the values admitted are only integers"
	echo ""
	exit 1
fi
if [ "$w_flag" = "T" ] && ! [[ "$wait_sec" =~ ^[0-9]+$ ]]
then
	echo "*** ERROR!!! Invalid input value ***"
	echo "When you use the \"-w\" option the values admitted are only integers"
	echo ""
	exit 1
fi
if [ "$r_flag" = "T" ] && ! [[ "$del_rows" =~ ^[0-9]+$ ]]
then
	echo "*** ERROR!!! Invalid input value ***"
	echo "When you use the \"-r\" option the values admitted are only integers"
	echo ""
	exit 1
fi
if [ "$r_flag" = "T" ] && ! [[ "$del_rows" =~ ^[1-9][0-9]?$|^100$ ]]
then
	echo "*** ERROR!!! Invalid input value ***"
	echo "Range admitted for deleting rows is 1 to 100"
	echo ""
	exit 1
fi
if [ "$r_flag" = "F" ]; then export $del_rows=50 ; fi


clean

init_file log $LOG_FILE
init_file res $RES_FILE


if [ "$p_flag" = "T" ]
then
	export CONF_FILE=$CONF_DIR/$prj.conf
	set_conf
else
	insert_param srv
	insert_param db
	insert_param usr
	insert_param psw
fi

if test -z "$PGPASSWORD"
then
	echo -n "Insert $PDSUSER password: "
	read -s answer
	export PGPASSWORD=$answer
	echo ""
fi


check_conn


### SEARCHING OPERATIONS
echo "Searching for items older than $day_num days in $tbl_name" | tee -a $LOG_FILE
SQL_CMD="SELECT id_value FROM t_mpm_values WHERE creationtime < (now() - INTERVAL '$day_num' DAY)"
exe_qry "select"
if [ "$s_flag" = "T" ]
then
	echo "$(date)" > "$save_file"
	echo "Items older than $day_num days" >> "$save_file"
	echo "------------------------------" >> "$save_file"
	cat $RES_FILE >> "$save_file"
fi


### CHECKING OPERATIONS
ite_founds=`expr $(wc -l < $RES_FILE) - 1`
if [ "$ite_founds" = "0" ]
then
	echo $dashLine | tee -a $LOG_FILE
	echo ">> No items to remove found" | tee -a $LOG_FILE
	echo "" | tee -a $LOG_FILE
	clean
	exit 0
fi
echo ">> Found $ite_founds items to remove" | tee -a $LOG_FILE
if [ "$f_flag" = "F" ]
then
	choose=bho
	while [ $choose != "n" ] && [ $choose != "y" ]
	do
		echo -n "Do you wanto to continue with items remotion? (y/n): "
		read choose
	done
	if [ "$choose" = "n" ]
	then
		echo $dashLine | tee -a $LOG_FILE
		echo ">> Remotion aborted by user" | tee -a $LOG_FILE
		echo "" | tee -a $LOG_FILE
		clean
		exit 0
	fi
fi


### CLEANING OPERATIONS
while mapfile -t -n $del_rows ary && ((${#ary[@]}))
do
	ids=$(printf '%s,' "${ary[@]}" | tr -d '[:space:]' | sed -e "s/,,//g")
	SQL_CMD=`echo "DELETE FROM t_mpm_values WHERE id_value IN ($ids); DELETE FROM t_mpm_cross WHERE value_id IN ($ids); " | sed -e 's/,)/)/g'`
	exe_qry "delete"
	if [ "$w_flag" = "T" ]
	then
		sleep $wait_sec
	fi
done < $RES_FILE


echo $dashLine | tee -a $LOG_FILE
echo ">> REMOTION COMPLETED" | tee -a $LOG_FILE
echo "" | tee -a $LOG_FILE
clean
exit 0




