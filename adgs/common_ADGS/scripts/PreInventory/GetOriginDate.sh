#!/bin/sh
#
# Copyright 2005-2021, Advanced Computer Systems , Inc.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.
# 
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
# the contents of this file may not be disclosed to third parties, copied or
# duplicated in any form, in whole or in part, without the prior written
# permission of Advanced Computer Systems, S.p.A.
#	
#
#   $Prod: A.C.S. GetOriginDate script $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	

usage() {
	echo "usage: GetOriginDate.sh <JobOrderFile>"
	echo "-------------------------------------------------------------------"
	echo ""
	exit 1
}


init_log() {
	if ! test -e $LOG_FILE
	then
		touch $LOG_FILE
		if [ "$?" != 0 ]
		then
			echo "*** ERROR!!! Cannot create log file ***"
			echo "trying to write: $LOG_FILE"
			echo ""
			exit 1
		fi
	fi
	
	echo "" | tee -a $LOG_FILE
	echo "############################################################" | tee -a $LOG_FILE
	echo "###  GET ORIGIN DATE" | tee -a $LOG_FILE
	echo "###  $(date)" | tee -a $LOG_FILE
	echo "############################################################" | tee -a $LOG_FILE
}


clean() {
	if test -d temp; then rm -fr temp; fi
}


res_msg() {
	case $1 in
		'0' ) 
			echo_success
			echo ""
			;;
		'1' ) 
			echo_failure
			echo ""
			if test -n "$2"; then echo -e "$2" | tee -a $LOG_FILE; fi
			if test -n "$3"; then echo -e "$3" | tee -a $LOG_FILE; fi
			echo "-------------------------------------------------------------------" | tee -a $LOG_FILE
			echo ">> CANNOT GET ORIGIN DATE" | tee -a $LOG_FILE
			echo "See log file $LOG_FILE"
			echo ""
			clean
			exit 1
			;;
		'2' ) 
			echo_warning
			echo ""
			;;
		*) 
			echo ""
			;;
	esac	
}


check_file() {
	echo -e "Searching for $1 file ..." | tee -a $LOG_FILE
	if ! test -r $2
	then
		res_msg 1 "*** ERROR!!! Cannot find $1 file or is not readable ***" "checking for: $2"
	fi
	
	res_msg 0
	echo -e ">> $1 file: $2\n" | tee -a $LOG_FILE
}


change_tag_value() {
	tag_name="$1"
	tag_value="$2"
	file_to_modify="$3"
	msg="$4"

	echo -e "Configuring new $msg file ..." | tee -a $LOG_FILE
	echo "TagName: $tag_name" | tee -a $LOG_FILE
	echo "Value: $tag_value" | tee -a $LOG_FILE

	INP_FILE=`grep -oPm1 "<$tag_name\/>" $file_to_modify`
	if [ "$?" == 0 ]
	then
		sed -i "s/<$tag_name\/>/<$tag_name>${tag_value}<\/$tag_name>/g" $file_to_modify
		if [ "$?" != 0 ]
		then
			res_msg 1 "*** ERROR!!! Cannot configure new $msg file (UPDATE)***"
		fi
	else
		INP_FILE=`grep -oPm1 "<$tag_name>" $file_to_modify`
		if [ "$?" != 0 ]
		then
			sed -i "/<\/Inventory_Metadata>/i<$tag_name>${tag_value}<\/$tag_name>" $file_to_modify
			if [ "$?" != 0 ]
			then
				res_msg 1 "*** ERROR!!! Cannot configure new $msg file (ADD)***"
			fi
		else
			sed -i "{s|\(<$tag_name>\).*\(</$tag_name>\)|\1${tag_value}\2|}" $file_to_modify
			if [ "$?" != 0 ]
			then
				res_msg 1 "*** ERROR!!! Cannot configure new $msg file (UPDATE)***"
			fi
		fi
	fi

	res_msg 0
	echo -e ">> new $msg configured\n" | tee -a $LOG_FILE
}


 # Source function library.
. /etc/rc.d/init.d/functions 

if test -z "$1"
then
	usage
fi

export EXE_DIR=`pwd`
cd $EXE_DIR

export BIN_DIR=/usr/local/components/ADGS/bin
export LOG_FILE=$EXE_DIR/GetOriginDate.log
init_log

clean

# Chek if exists JobOrder file
JOB_ORD="$1"
check_file JobOrder $JOB_ORD

# Extract file name from JobOrder
echo -e "Extracting File_Name ..." | tee -a $LOG_FILE
INP_FILE=`grep -oPm1 "(?<=<File_Name>)[^<]+" $JOB_ORD`
if test -z "$INP_FILE"; then res_msg 1 "*** ERROR!!! Cannot extract File_Name tag ***" "searching into: $JOB_ORD"; fi
echo -e ">> File_Name: $INP_FILE\n" | tee -a $LOG_FILE
res_msg 0

# Chek if exists the file configured into JobOrder
check_file Input $INP_FILE

# Get modification date of the input file
echo -e "Get the modification date of the input file ..." | tee -a $LOG_FILE
ORDATE=`date -r $INP_FILE +"UTC=%Y-%m-%dT%H:%M:%S.000000"`
echo -e ">> OriginDate: $ORDATE\n" | tee -a $LOG_FILE

# Chek if exists and if is readable the MTD file
MTD_FILE=$INP_FILE.MTD
check_file MTD $MTD_FILE

# Change the file name into new MTD
change_tag_value "Equator_X_Time" $ORDATE $MTD_FILE MTD

echo "-------------------------------------------------------------------" | tee -a $LOG_FILE
echo ">> ORIGIN DATE SUCCESSFULLY ADDED" | tee -a $LOG_FILE
echo "" | tee -a $LOG_FILE
clean
exit 0

