#!/bin/sh
#
# Copyright 2020-2030, Exprivia S.p.A.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.exprivia.com
# 
#   $Prod: Rename ECMWF AUX TRO Files script $
#


usage() {
	echo "usage: RenameAUXTROOrigFiles <JobOrderFile>"
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
	echo "###  Rename ECMWF AUX TRO File " | tee -a $LOG_FILE
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



echo ""
echo "RENAME ECMWF AUX TRO FILE"
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
export LOG_FILE=$EXE_DIR/RenameAUXTROOrigFiles.log
init_log

clean

# Chek if exists JobOrder file
JOB_ORD="$1"
check_file JobOrder $JOB_ORD

# Extract file name from JobOrder
log_msg "Extracting File_Name ..."
DFILE=`grep -oPm1 "(?<=<File_Name>)[^<]+" $JOB_ORD`
if test -z "$DFILE"; then res_msg 1 "*** ERROR!!! Cannot extract File_Name tag ***" "searching into: $JOB_ORD"; fi
echo -e ">> File_Name: $DFILE\n" | tee -a $LOG_FILE
res_msg 0

# Chek if exists the file configured into JobOrder
check_file GRIB $DFILE

DFILE_NODIR=`basename $DFILE`
DIR_DFILE=`dirname $DFILE`

#### Naming convention is
#### T[12345][SD]MMDDHHIImmddhhiiE
FirstChar=${DFILE_NODIR:0:2}
filedate=${DFILE_NODIR:3:4}
filetime=${DFILE_NODIR:7:4}
curryear=$(date "+%Y")

#### The follwing steps fix the bug of the new year transition
#### Last two files of old year are published in the new year
#### In this case we cannot use the current year as validity
if [ $filedate == "1231" ]
then
        echo -e "Get the modification date of the input file (month is enough) ..." | tee -a $LOG_FILE
        ORDATE=$(date -r $DFILE +"%m")
        echo -e ">> Publication Month: $ORDATE\n" | tee -a $LOG_FILE

        if [ $ORDATE == "01" ]
        then
                curryear=$(date "+%Y" -d '1 month ago')
        fi
fi
SFILE=""
NewFileType=""

###############################################################
# The files shall be renamed as following:
# T1 -> ECMWF_OPER_ML00_06H_129_GP_N640_[0-9]{8}T[0-9]{6}
# T2 -> ECMWF_OPER_ML00_06H_130_GP_N640_[0-9]{8}T[0-9]{6}
# T3 -> ECMWF_OPER_ML00_06H_133_GP_N640_[0-9]{8}T[0-9]{6}
# T4 -> ECMWF_OPER_ML00_06H_134_GP_N640_[0-9]{8}T[0-9]{6}
# T5 -> ECMWF_OPER_ML00_06H_151_GP_N640_[0-9]{8}T[0-9]{6}
###############################################################

NEWDFILE=$DFILE
if [ $FirstChar == "T1" ]
then
	SFILE="ECMWF_OPER_ML00_06H_129_GP_N640_"$curryear$filedate"T"$filetime"00"
	NewFileType="AUX_TRO_129"
elif [ $FirstChar == "T2" ]
then
	NEWDFILE=$DFILE"_Sorted"
	grib_copy -B'level:i asc' $DFILE $NEWDFILE
	SFILE="ECMWF_OPER_ML00_06H_130_GP_N640_"$curryear$filedate"T"$filetime"00"
	NewFileType="AUX_TRO_130"
elif [ $FirstChar == "T3" ]
then
	NEWDFILE=$DFILE"_Sorted"
	grib_copy -B'level:i asc' $DFILE $NEWDFILE
	SFILE="ECMWF_OPER_ML00_06H_133_GP_N640_"$curryear$filedate"T"$filetime"00"
	NewFileType="AUX_TRO_133"
elif [ $FirstChar == "T4" ]
then
	SFILE="ECMWF_OPER_ML00_06H_134_GP_N640_"$curryear$filedate"T"$filetime"00"
	NewFileType="AUX_TRO_134"
elif [ $FirstChar == "T5" ]
then
	SFILE="ECMWF_OPER_ML00_06H_151_GP_N640_"$curryear$filedate"T"$filetime"00"
	NewFileType="AUX_TRO_151"
else
		res_msg 1 "*** ERROR!!! File $DFILE_NODIR not recognized ***"
fi

NEWFILENAME=$DIR_DFILE/$SFILE
echo -e ">> New File_Name: $NEWFILENAME\n" | tee -a $LOG_FILE

mv $NEWDFILE $NEWFILENAME

# Create new JobOrder coping the old one
JO_NODIR=`basename $JOB_ORD`
NEW_JOB_ORD="new"$JO_NODIR
OLD_JOB_ORD=$JO_NODIR".old"
copy_file $JO_NODIR $NEW_JOB_ORD JobOrder

# Change the file name into new JobOrder
TOT_FILE=$EXE_DIR/$(basename $NEWFILENAME)
change_tag_value "File_Name" $DFILE $TOT_FILE $NEW_JOB_ORD JobOrder
OldFileType=$(grep '<File_Type>.*</File_Type>' $JOB_ORD | cut -d '>' -f 2 | cut -d '<' -f 1)
change_tag_value "File_Type" $OldFileType $NewFileType $NEW_JOB_ORD JobOrder

rename_file $JO_NODIR $OLD_JOB_ORD JobOrder
rename_file $NEW_JOB_ORD $JO_NODIR JobOrder

echo "-------------------------------------------------------------------" | tee -a $LOG_FILE
echo ">> RENAMING SUCCESSFULLY COMPLETED" | tee -a $LOG_FILE
echo "" | tee -a $LOG_FILE
clean
exit 0


