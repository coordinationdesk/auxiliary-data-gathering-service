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
#   $Prod: A.C.S. ZipSafeFile script $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	

usage() {
	echo "usage: ZipSafeFile.sh <JobOrderFile>"
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
	echo "" | tee -a $LOG_FILE
	echo "############################################################" | tee -a $LOG_FILE
	echo "###  ZIP SAFE FILE" | tee -a $LOG_FILE
	echo "###  $(date)" | tee -a $LOG_FILE
	echo "############################################################" | tee -a $LOG_FILE
}


log_msg() {
	echo -n "$1"
	echo $(date +"%H:%M:%S") "- $1" | tee -a $LOG_FILE
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
			echo ">> CANNOT ZIP FILE" | tee -a $LOG_FILE
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
	log_msg "Searching for $1 file ..."
	if ! test -r $2
	then
		res_msg 1 "*** ERROR!!! Cannot find $1 file or is not readable ***" "checking for: $2"
	fi
	
	res_msg 0
	echo -e ">> $1 file: $2\n" | tee -a $LOG_FILE
}


copy_file() {
	log_msg "Creating new $3 file ..."
	cp $1 $2 2>&1 | tee -a $LOG_FILE

	res=${PIPESTATUS[0]}
	if [ "$res" != 0 ] 
	then 
		res_msg 1 "*** ERROR!!! Cannot create new $3 file ***"
	fi
	
	res_msg 0
	echo -e ">> new $3 created: $2\n" | tee -a $LOG_FILE
}


rename_file() {
	log_msg "Rename file $1 to avoid unwanted ingestion ..."
	cp $1 $2 2>&1 | tee -a $LOG_FILE
	res=${PIPESTATUS[0]}
	if [ "$res" != 0 ] 
	then 
		res_msg 1 "*** ERROR!!! Cannot rename $3 file ***"
	fi

	rm -f $1 2>&1 | tee -a $LOG_FILE

	res=${PIPESTATUS[0]}
	if [ "$res" != 0 ] 
	then 
		res_msg 1 "*** ERROR!!! Cannot rename $3 file ***"
	fi

	res_msg 0
	echo -e ">> file $1 renamed: $2\n" | tee -a $LOG_FILE
}


change_tag_value() {
	tag_name="$1"
	old_value="$2"
	tag_value="$3"
	file_to_modify="$4"
	msg="$5"

	log_msg "Configuring new $msg file ..."
	echo "TagName: $tag_name" | tee -a $LOG_FILE
	echo "Value: $tag_value" | tee -a $LOG_FILE
	
	sed -i "{s|\(<$tag_name>\)$old_value\(</$tag_name>\)|\1${tag_value}\2|}" $file_to_modify
	if [ "$?" != 0 ]
	then
		res_msg 1 "*** ERROR!!! Cannot configure new $msg file ***"
	fi
	
	res_msg 0
	echo -e ">> new $msg configured\n" | tee -a $LOG_FILE
}


echo ""
echo "ZIP SAFE FILE"
echo "-------------------------------------------------------------------"

 # Source function library.
. /etc/rc.d/init.d/functions 

if test -z "$1"
then
	usage
fi

export EXE_DIR=`pwd`
cd $EXE_DIR

export BIN_DIR=/usr/local/components/ADGS/bin
export LOG_FILE=$EXE_DIR/ZipSafeFile.log
init_log

clean

# Chek if exists JobOrder file
JOB_ORD="$1"
check_file JobOrder $JOB_ORD

# Extract file name from JobOrder
log_msg "Extracting File_Name ..."
SAFE_FILE=`grep -oPm1 "(?<=<File_Name>)[^<]+" $JOB_ORD`
if test -z "$SAFE_FILE"; then res_msg 1 "*** ERROR!!! Cannot extract File_Name tag ***" "searching into: $JOB_ORD"; fi
echo -e ">> File_Name: $SAFE_FILE\n" | tee -a $LOG_FILE
res_msg 0

# Chek if exists the file configured into JobOrder
check_file DIR $SAFE_FILE

# Chek if the file is a directory
log_msg "Check if the input is a directory ..."
if [ ! -d $SAFE_FILE ]; then res_msg 1 "*** ERROR!!! Input $(basename $SAFE_FILE) in not a directory ***" ; fi
echo -e ">> done\n" | tee -a $LOG_FILE
res_msg 0

NAMENOPATH=$(basename -- $SAFE_FILE)
SEN3NAMEZIP="${NAMENOPATH}.zip"

zip -r $SEN3NAMEZIP $NAMENOPATH

if [ ! -f $EXE_DIR/$SEN3NAMEZIP ]; then res_msg 1 "*** ERROR!!! Cannot create ZIP file ***"; fi

# Create new JobOrder coping the old one
JO_NODIR=`basename $JOB_ORD`
NEW_JOB_ORD="new"$JO_NODIR
copy_file $JO_NODIR $NEW_JOB_ORD JobOrder

# Change the file name into new JobOrder
TOT_FILE=$EXE_DIR/$SEN3NAMEZIP

change_tag_value "File_Name" $SAFE_FILE $TOT_FILE $NEW_JOB_ORD JobOrder

# Generate List File Name
OrderId=$(grep '<Order_ID>.*</Order_ID>' $JOB_ORD | grep  -o '[0-9]*')

# Chek if exists and if is readable the files generated
MTD_FILE=$SAFE_FILE.MTD
LIST_FILE=$NAMENOPATH.LIST
check_file MTD $MTD_FILE
check_file LIST $LIST_FILE

# Create new MTD coping the old one
NEW_MTD_FILE=$TOT_FILE.MTD
copy_file $MTD_FILE $NEW_MTD_FILE MTD

# Change the file name into new MTD
change_tag_value "File_Name" $(basename $SAFE_FILE) $(basename $TOT_FILE) $NEW_MTD_FILE MTD

# Change the data size into new MTD
NEW_SIZE=$(du -sb $TOT_FILE | awk '{print $1}')
OldSize=$(grep '<Data_Size>.*</Data_Size>' $MTD_FILE | grep  -o '[0-9]*')
change_tag_value "Data_Size" $OldSize $NEW_SIZE $NEW_MTD_FILE MTD

# Create new LIST coping the old one
RENAMEDFILE=$LIST_FILE.OLD
NEW_LIST_FILE=$TOT_FILE.LIST
rename_file $LIST_FILE $RENAMEDFILE LIST

# Change the file name into new LIST
log_msg "Configuring new LIST file ..."
echo $SEN3NAMEZIP > $NEW_LIST_FILE
echo -e ">> done\n" | tee -a $LOG_FILE
res_msg 0

echo "-------------------------------------------------------------------" | tee -a $LOG_FILE
echo ">> ZIP SUCCESSFULLY COMPLETED" | tee -a $LOG_FILE
echo "" | tee -a $LOG_FILE
clean
exit 0


