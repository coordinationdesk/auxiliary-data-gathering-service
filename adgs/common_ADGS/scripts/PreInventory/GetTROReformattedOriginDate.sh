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
#   $Prod: A.C.S. GetTROReformattedOriginDate script $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	

usage() {
	echo "usage: GetTROReformattedOriginDate <JobOrderFile>"
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

	res_msg 0
	echo -e ">> new $msg configured\n" | tee -a $LOG_FILE
}

DBNAME="##ADGSDB_MACHINENAME##"
PDSUSER_PWD="##PDSUSER_DBPWD##"

 # Source function library.
. /etc/rc.d/init.d/functions 

if test -z "$1"
then
	usage
fi

export EXE_DIR=`pwd`
cd $EXE_DIR

export BIN_DIR=/usr/local/components/ADGS/bin
export LOG_FILE=$EXE_DIR/GetTROReformattedOriginDate.log
init_log

clean

# Chek if exists JobOrder file
JOB_ORD="$1"
check_file JobOrder $JOB_ORD

# Extract file name from JobOrder
log_msg "Extracting File_Name ..."
ORIG_FILE129=`grep -oPm1 "(?<=<File_Name>).*_129_[^<]+" $JOB_ORD`
if test -z "$ORIG_FILE129"; then res_msg 1 "*** ERROR!!! Cannot extract File_Name tag ***" "searching into: $JOB_ORD"; fi
echo -e ">> Origin 129 File_Name: $ORIG_FILE129\n" | tee -a $LOG_FILE
ORIGNAMENOPATH129=$(basename -- $ORIG_FILE129)
res_msg 0

ORIG_FILE130=`grep -oPm1 "(?<=<File_Name>).*_130_[^<]+" $JOB_ORD`
if test -z "$ORIG_FILE130"; then res_msg 1 "*** ERROR!!! Cannot extract File_Name tag ***" "searching into: $JOB_ORD"; fi
echo -e ">> Origin 130 File_Name: $ORIG_FILE130\n" | tee -a $LOG_FILE
ORIGNAMENOPATH130=$(basename -- $ORIG_FILE130)
res_msg 0

ORIG_FILE133=`grep -oPm1 "(?<=<File_Name>).*_133_[^<]+" $JOB_ORD`
if test -z "$ORIG_FILE133"; then res_msg 1 "*** ERROR!!! Cannot extract File_Name tag ***" "searching into: $JOB_ORD"; fi
echo -e ">> Origin 133 File_Name: $ORIG_FILE133\n" | tee -a $LOG_FILE
ORIGNAMENOPATH133=$(basename -- $ORIG_FILE133)
res_msg 0

ORIG_FILE134=`grep -oPm1 "(?<=<File_Name>).*_134_[^<]+" $JOB_ORD`
if test -z "$ORIG_FILE134"; then res_msg 1 "*** ERROR!!! Cannot extract File_Name tag ***" "searching into: $JOB_ORD"; fi
echo -e ">> Origin 134 File_Name: $ORIG_FILE134\n" | tee -a $LOG_FILE
ORIGNAMENOPATH134=$(basename -- $ORIG_FILE134)
res_msg 0

ORIG_FILE151=`grep -oPm1 "(?<=<File_Name>).*_151_[^<]+" $JOB_ORD`
if test -z "$ORIG_FILE151"; then res_msg 1 "*** ERROR!!! Cannot extract File_Name tag ***" "searching into: $JOB_ORD"; fi
echo -e ">> Origin 151 File_Name: $ORIG_FILE151\n" | tee -a $LOG_FILE
res_msg 0
ORIGNAMENOPATH151=$(basename -- $ORIG_FILE151)

# Get Origin date of the file inside the database
export PGPASSWORD=$PDSUSER_PWD
ORIGDATE=$(psql -h $DBNAME -U pdsuser adgsdb -t -c "SELECT MAX(TO_CHAR(equatorxtime,'UTC=YYYY-MM-DD\"T\"HH24:MI:ss')) FROM t_inventory WHERE filename='$ORIGNAMENOPATH129' OR filename='$ORIGNAMENOPATH130' OR filename='$ORIGNAMENOPATH133' OR filename='$ORIGNAMENOPATH134' OR filename='$ORIGNAMENOPATH151'")
echo -e ">> Origin DateTime: $ORIGDATE\n" | tee -a $LOG_FILE

# Extract file name from JobOrder
echo -e "Get Reformatted file name ..." | tee -a $LOG_FILE
INP_FILE=`ls -d *.SAFE`
if test -z "$INP_FILE"; then res_msg 1 "*** ERROR!!! Reformatted file Not Found !!!"; fi
echo -e ">> File_Name: $INP_FILE\n" | tee -a $LOG_FILE
res_msg 0

# Chek if exists the file configured into JobOrder
check_file Input $INP_FILE

# Chek if exists and if is readable the MTD file
MTD_FILE=$INP_FILE.MTD
check_file MTD $MTD_FILE

# Change the file name into new MTD
change_tag_value "Equator_X_Time" $ORIGDATE $MTD_FILE MTD

echo "-------------------------------------------------------------------" | tee -a $LOG_FILE
echo ">> ORIGIN DATE SUCCESSFULLY ADDED" | tee -a $LOG_FILE
echo "" | tee -a $LOG_FILE
clean
exit 0

