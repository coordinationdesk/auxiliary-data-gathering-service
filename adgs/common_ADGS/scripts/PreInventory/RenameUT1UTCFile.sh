#!/bin/sh
#
# Copyright 2020-2030, Exprivia S.p.A.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.exprivia.com
# 
#   $Prod: Rename UT1UTC File script $
#


usage() {
	echo "usage: RenameUT1UTCFile <JobOrderFile>"
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
	echo "###  Rename UT1UTC File " | tee -a $LOG_FILE
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
	echo "OldValue: $old_value" | tee -a $LOG_FILE
	echo "NewValue: $tag_value" | tee -a $LOG_FILE
	
	sed -i "{s|\(<$tag_name>\)$old_value\(</$tag_name>\)|\1${tag_value}\2|}" $file_to_modify
	if [ "$?" != 0 ]
	then
		res_msg 1 "*** ERROR!!! Cannot configure new $msg file ***"
	fi
	
	res_msg 0
	echo -e ">> new $msg configured\n" | tee -a $LOG_FILE
}

getDates() {
	local filename=$1
	local crdate=false
	local lastline=""
	local extrDate=""
	local valsection=false
	local valsectionend=false
	local valstartdate=false

	log_msg "Extract Creation and Validity dates from the input file"

	while read line; do

		if [ $crdate == false ]
		then
			if [[ $line =~ [0-9]{1,2}.*[January|February|March|April|May|June|July|August|September|October|November|December].*[0-9]{4}.*Vol ]]
			then
				log_msg "Line containing Creation date = \"$line\""
				crdate=true
				extrDate=`echo $line | grep -oPm1 '[0-9]{1,2}.*[January|February|March|April|May|June|July|August|September|October|November|December].*[0-9]{4}'`
				CREATIONTIME=$(date -d "$extrDate" +'%Y%m%d')"T000000"
				if [ "$?" != 0 ]
				then
					res_msg 1 "*** ERROR!!! Invalid Creation Date -> $extrDate ***"
				fi
			fi
		fi

		if [ $valsection == false ]
		then
			if [[ $line =~ MJD.*x\(arcsec\).*y\(arcsec\).*UT1-UTC\(sec\) ]]
			then
				log_msg "Starting Line of Validity Period = \"$line\""
				valsection=true
			fi
		else
			if [[ $valstartdate == false ]]
			then
				log_msg "Line containing Validity Start = \"$line\""
				extrDate=`echo $line | grep -oPm1 '[0-9]{4}[ ]{1,4}[0-9]{1,2}[ ]{1,4}[0-9]{1,2}'`
				IFS=' '
				read -a strarr <<< "$extrDate"
				trimDate=`printf %04d-%02d-%02d  ${strarr[0]} ${strarr[1]} ${strarr[2]}`

				VALSTARTTIME=$(date -d "$trimDate" +'%Y%m%d')"T000000"
				if [ "$?" != 0 ]
				then
					res_msg 1 "*** ERROR!!! Invalid Validity Start Date -> $extrDate ***"
				fi
				valstartdate=true
			else
				if [[ $valsectionend == false ]]
				then
					if [[ $line =~ These.*predictions.*are.*based.*on.*all.*announced.*leap.*seconds ]]
					then
						log_msg "Line containing Validity End = \"$lastline\""
						valsectionend=true
						extrDate=`echo $lastline | grep -oPm1 '[0-9]{4}[ ]{1,4}[0-9]{1,2}[ ]{1,4}[0-9]{1,2}'`
						IFS=' '
						read -a strarr <<< "$extrDate"
						trimDate=`printf %04d-%02d-%02d  ${strarr[0]} ${strarr[1]} ${strarr[2]}`

						VALSTOPTIME=$(date -d "$trimDate" +'%Y%m%d')"T000000"
						if [ "$?" != 0 ]
						then
							res_msg 1 "*** ERROR!!! Invalid Validity End Date -> $extrDate ***"
						fi
						return
					fi
				fi
			fi
		fi

		lastline=$line
	done < $filename	

	res_msg 0
} 


echo ""
echo "RENAME UT1UTC FILE"
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
export LOG_FILE=$EXE_DIR/RenameUT1UTCFile.log
init_log

clean

# Chek if exists JobOrder file
JOB_ORD="$1"
check_file JobOrder $JOB_ORD

# Extract file name from JobOrder
log_msg "Extracting File_Name ..."
SER7_FILE=`grep -oPm1 "(?<=<File_Name>)[^<]+" $JOB_ORD`
if test -z "$SER7_FILE"; then res_msg 1 "*** ERROR!!! Cannot extract File_Name tag ***" "searching into: $JOB_ORD"; fi
echo -e ">> File_Name: $SER7_FILE\n" | tee -a $LOG_FILE
res_msg 0

# Chek if exists the file configured into JobOrder
check_file TXT $SER7_FILE

MODDATE=$(date -r $SER7_FILE)
CREATIONTIME=$(date -d "$MODDATE" +'%Y%m%d')"T000000"
VALSTARTTIME=$(date -d "$MODDATE + 1 days" +'%Y%m%d')"T000000"
VALSTOPTIME=$(date -d "$MODDATE + 1 year" +'%Y%m%d')"T000000"

getDates $SER7_FILE

NEWFILENAME="S2__OPER_AUX_UT1UTC_ADG__"$CREATIONTIME"_V"$VALSTARTTIME"_"$VALSTOPTIME".txt"
echo -e ">> New File_Name: $NEWFILENAME\n" | tee -a $LOG_FILE

mv $SER7_FILE $NEWFILENAME

# Create new JobOrder coping the old one
JO_NODIR=`basename $JOB_ORD`
NEW_JOB_ORD="new"$JO_NODIR
OLD_JOB_ORD=$JO_NODIR".old"
copy_file $JO_NODIR $NEW_JOB_ORD JobOrder

# Change the file name into new JobOrder
TOT_FILE=$EXE_DIR/$(basename $NEWFILENAME)
change_tag_value "File_Name" $SER7_FILE $TOT_FILE $NEW_JOB_ORD JobOrder
OldFileType=$(grep '<File_Type>.*</File_Type>' $JOB_ORD | cut -d '>' -f 2 | cut -d '<' -f 1)
NewFileType="AUX_UT1UTC"
change_tag_value "File_Type" $OldFileType $NewFileType $NEW_JOB_ORD JobOrder

rename_file $JO_NODIR $OLD_JOB_ORD JobOrder
rename_file $NEW_JOB_ORD $JO_NODIR JobOrder

echo "-------------------------------------------------------------------" | tee -a $LOG_FILE
echo ">> RENAMING SUCCESSFULLY COMPLETED" | tee -a $LOG_FILE
echo "" | tee -a $LOG_FILE
clean
exit 0


