#!/bin/sh
#
# Copyright 2020-2030, Exprivia S.p.A.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.exprivia.com
# 
#   $Prod: ZIPEOF ingestion chain script $
#

usage() {
	echo "usage: ZIPEOF_IngestionChain.sh <JobOrderFile>"
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
	echo "###  ZIPEOF INGESTION CHAIN" | tee -a $LOG_FILE
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
			echo ">> INGESTION ABORTED" | tee -a $LOG_FILE
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
	log_msg "Searchin for $1 file ..."
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


call_app() {
	APP=$BIN_DIR/"$1"
	log_msg "Executing $1 ..."
	
	if ! test -x "$APP"
	then
		res_msg 1 "*** ERROR!!! Cannot find $1 or is not executable ***" "checking for: $APP"
	fi
	
	$APP $2 $3 2>&1 | tee -a $LOG_FILE
	res=${PIPESTATUS[0]}
	if [ "$res" != 0 ] 
	then 
		res_msg 1 "*** ERROR!!! Error During $1 execution ***"  "$1 terminated with code $res"
	fi
	
	res_msg 0
	echo -e ">> $1 done\n" | tee -a $LOG_FILE
}





echo ""
echo "ZIPEOF INGESTION CHAIN"
echo "-------------------------------------------------------------------"

 # Source function library.
. /etc/rc.d/init.d/functions 

if test -z "$1"
then
	usage
fi

export EXE_DIR=`pwd`
#export EXE_DIR=`dirname $0`
cd $EXE_DIR

export BIN_DIR=/usr/local/components/ADGS/bin
export LOG_FILE=$EXE_DIR/ZIPEOF_IngestionChain.log
init_log

clean

# Chek if exists JobOrder file
JOB_ORD="$1"
check_file JobOrder $JOB_ORD

# Extract file name from JobOrder
log_msg "Extracting File_Name ..."
ZIP_FILE=`grep -oPm1 "(?<=<File_Name>)[^<]+" $JOB_ORD`
if test -z "$ZIP_FILE"; then res_msg 1 "*** ERROR!!! Cannot extract File_Name tag ***" "searching into: $JOB_ORD"; fi
echo -e ">> File_Name: $ZIP_FILE\n" | tee -a $LOG_FILE
res_msg 0

# Chek if exists the file configured into JobOrder
check_file ZIP $ZIP_FILE

# Chek if the file is a ZIP file
log_msg "Validating file extension ..."
if [ ${ZIP_FILE: -4} != ".ZIP" ] && [ ${ZIP_FILE: -4} != ".zip" ];
then
	ISZIPPED=0
	echo "extension found: ${ZIP_FILE: -4} - FILE is not Zipped" | tee -a $LOG_FILE
else
	ISZIPPED=1
	echo "extension found: ${ZIP_FILE: -4}" | tee -a $LOG_FILE
	echo -e ">> done\n" | tee -a $LOG_FILE
fi
res_msg 0

NEW_FILE=`basename $ZIP_FILE`

if [ $ISZIPPED == 1 ];
then
	# Extract ZIP
	mkdir temp
	log_msg "Extracting ZIP file ..."
	cd temp
	unzip $ZIP_FILE | tee -a $LOG_FILE 
	res=${PIPESTATUS[0]}
	if [ "$res" != 0 ] ; then res_msg 1 "*** ERROR!!! TAR: Cannot extract file ***"; fi
	echo -e ">> done\n" | tee -a $LOG_FILE
	res_msg 0
	cd ..

	# Check TAR result
	log_msg "Validating extraction result ..."
	if [ "$(ls -1 temp | wc -l)" = 0 ]; then res_msg 1 "*** ERROR!!! The ZIP archive doesn't contain any files ***"; fi
	if [ "$(ls -1 temp | wc -l)" != 1 ]; then res_msg 1 "*** ERROR!!! Too many files contained ZIP archive ***" "$(ls -1 temp)"; fi
	echo -e ">> done\n" | tee -a $LOG_FILE
	res_msg 0

	# Get the name of new file and move it into working dir
	NEW_FILE=$(ls -1 temp)
	mv temp/$NEW_FILE $EXE_DIR

	# Create new JobOrder coping the old one
	JO_NODIR=`basename $JOB_ORD`
	NEW_JOB_ORD="new"$JO_NODIR
	copy_file $JO_NODIR $NEW_JOB_ORD JobOrder

	# Change the file name into new JobOrder
	TOT_FILE=$EXE_DIR/$NEW_FILE
	change_tag_value "File_Name" $ZIP_FILE $TOT_FILE $NEW_JOB_ORD JobOrder
else
	# Create new JobOrder coping the old one
	JO_NODIR=`basename $JOB_ORD`
	NEW_JOB_ORD="new"$JO_NODIR
	copy_file $JO_NODIR $NEW_JOB_ORD JobOrder
fi

# Call external aplpications
call_app TaskInPreInventory $EXE_DIR/$NEW_JOB_ORD

if [ $ISZIPPED == 0 ];
then
	change_tag_value "File_Name" $ZIP_FILE $ZIP_FILE".zip" $JOB_ORD JobOrder
	INFILE=`basename $ZIP_FILE`
	zip $INFILE".zip" $INFILE
fi

# Chek if exists and if is readable the files generated
MTD_FILE=$NEW_FILE".MTD"
LIST_FILE=$NEW_FILE".LIST"
check_file MTD $MTD_FILE
check_file LIST $LIST_FILE

# Create new MTD coping the old one
NEW_MTD_FILE=$NEW_FILE".zip.MTD"
copy_file $MTD_FILE $NEW_MTD_FILE MTD

# Change the file name into new MTD
change_tag_value "File_Name" $NEW_FILE $NEW_FILE".zip" $NEW_MTD_FILE MTD

# Change the data size into new MTD
NEW_SIZE=$(du -sb $NEW_FILE".zip" | awk '{print $1}')
OldSize=$(grep '<Data_Size>.*</Data_Size>' $MTD_FILE | grep  -o '[0-9]*')
change_tag_value "Data_Size" $OldSize $NEW_SIZE $NEW_MTD_FILE MTD

# Call task to check if file is already in DB
call_app FileVersioningTask $NEW_MTD_FILE

# Create new LIST coping the old one
RENAMEDFILE=$LIST_FILE.OLD
NEW_LIST_FILE=$NEW_FILE".zip.LIST"
rename_file $LIST_FILE $RENAMEDFILE LIST

# Change the file name into new LIST
log_msg "Configuring new LIST file ..."
echo $NEW_FILE".zip" > $NEW_LIST_FILE
echo -e ">> done\n" | tee -a $LOG_FILE
res_msg 0

echo "-------------------------------------------------------------------" | tee -a $LOG_FILE
echo ">> INGESTION SUCCESSFULLY COMPLETED" | tee -a $LOG_FILE
echo "" | tee -a $LOG_FILE
clean
exit 0


