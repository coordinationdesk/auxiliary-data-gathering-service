#!/bin/bash
#---------------------------------------------------------------------
# ADGS Project
#---------------------------------------------------------------------
# Description
#
# the script handles the ADF received from MMDS@EUMETSAT and filters them before moving
# into the ADGS ingestion basket
#
#---------------------------------------------------------------------
function __print_usage {
	LogError "Usage: $0 <SOURCEFOLDER> <DESTFOLDER>"
}

function __handle_ADF_unformatted {
        local EC=$S3_TB_OK
        local ADF=$1
        local ADFNAME=$2
        local ADFTYPE=$3
        local ADFCENTRE=$4
        local DESTFOLDER=$5

        case $ADFCENTRE in
        * )
                case $ADFTYPE in
                * )
                        LogMsg "Moving  $ADF to ${DESTFOLDER}"
                        mv $ADF ${DESTFOLDER}
                        EC=$?
                ;;
                esac;
        ;;
        esac;
        return $EC
}

function __handle_ADF_formatted {
	local EC=$S3_TB_OK
	local ADF=$1
	local ADFNAME=$2
	local ADFTYPE=$3
	local ADFCENTRE=$4
	local DESTFOLDER=$5
	
	case $ADFCENTRE in 
	* )
		case $ADFTYPE in
        	* )
                	LogMsg "Moving  $ADF to ${DESTFOLDER}"
                	mv $ADF ${DESTFOLDER}
                	EC=$?
        	;;
		esac;
	;;
	esac;
	return $EC
}

function __check_binary {
	local EC=$S3_TB_OK
	local ADF=$1
	local BINFILE=$2
	local SIZE=$3
	local CKSUMNAME=$4
	local CKSUMVALUE=$5

	if [ ! -e ${ADF}/${BINFILE} ]
        then
 		LogError "${ADF}/${BINFILE} not found"
		return $S3_TB_ERROR
	else
		LogMsg "${ADF}/${BINFILE} found"
        fi

	computedsize=$(du -b ${ADF}/${BINFILE} | awk '{print $1}')
	if [[ $computedsize != $SIZE ]]
	then
		LogError "${ADF}/${BINFILE} size mismatch (computed $computedsize expected $SIZE)"
                return $S3_TB_ERROR
	else
		LogMsg "${ADF}/${BINFILE} size match ($SIZE)"
        fi

	case $CKSUMNAME in
	"md5sum" | "MD5SUM" | "MD5" )
		computedmd5sum=$(md5sum ${ADF}/${BINFILE} | awk '{print $1}')
		if [[ $computedmd5sum != $CKSUMVALUE ]]
        	then
			LogError "${ADF}/${BINFILE} md5sum mismatch (computed $computedmd5sum expected $CKSUMVALUE)"
                	return $S3_TB_ERROR
		else
			LogMsg "${ADF}/${BINFILE} md5sum match ($CKSUMVALUE)"
       		fi
	;;
	* )
		LogMsg "checksum reported as $CKSUMNAME $CKSUMVALUE, check skipped as not implemented yet..."
	;;
	esac;

	return $EC
}

function __check_binary_files {
	local EC=$S3_TB_OK
	local XFDUFILE=$1
	local ADF=$2
	local MY_ARRAY=""
	local howmany=0
	local counter=0
	local loopspec="binary files"
	local line=0

	MY_ARRAY=($(cat $XFDUFILE))
	howmany=${#MY_ARRAY[@]}; counter=1
	if [[ $howmany -gt 0 ]]
	then
		LoopBegin "$loopspec" $howmany
		for line in "${MY_ARRAY[@]}"
		do
			file__name=$(echo $line | awk -F "|" '{print $1}')	
			file__size=$(echo $line | awk -F "|" '{print $2}')
			cksum_name=$(echo $line | awk -F "|" '{print $3}')
			cksum__val=$(echo $line | awk -F "|" '{print $4}')
			LoopMsg "$loopspec" $howmany $counter "binary file=$file__name"; let "counter += 1"
			__check_binary ${ADF} $file__name $file__size $cksum_name $cksum__val
			EC=$?
			if [ $EC -eq $S3_TB_OK ]
			then
				LogMsg "Binary file $file__name checked"
			else
				break			
			fi
		done
		LoopEnd "$loopspec"
	else
		LogError "No binary file referenced by the xfdu manifest"
                return $S3_TB_ERROR
	fi
	return $EC
}

function __check_if_completed {
	local EC=$S3_TB_OK
	local ADF=$1
	local TMPDIR=$(create_tmpdir __check_if_completed)
	local XSLTFILE=$(create_tmpfile $TMPDIR xsltfile.xsl)
	local TMPFILE=$(create_tmpfile $TMPDIR xfdu_binary.txt)

	LogMsg "Checking if $ADF transfer is completed..."
	# check if manifest exists
	if [ ! -e ${ADF}/${S3_MANIFEST} ]
	then
		LogError "${ADF}/${S3_MANIFEST} not found"
		return $S3_TB_ERROR
	else
		LogMsg "${ADF}/${S3_MANIFEST} found"
	fi

	cat > ${XSLTFILE} << EOF 
<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
	xmlns:xfdu="urn:ccsds:schema:xfdu:1" version="1.0">
	<xsl:output version="1.0" method="text" indent="yes" />
	<xsl:template match="/">
		<xsl:for-each select="/xfdu:XFDU/dataObjectSection/dataObject/byteStream[*]">
			<xsl:value-of select="fileLocation/@href" />
			<xsl:text>|</xsl:text>
			<xsl:value-of select="@size" />
			<xsl:text>|</xsl:text>
			<xsl:value-of select="checksum/@checksumName" />
			<xsl:text>|</xsl:text>
			<xsl:value-of select="checksum" />
			<xsl:text>&#xa;</xsl:text>
		</xsl:for-each>
	</xsl:template>
</xsl:stylesheet>
EOF

	xsltproc ${XSLTFILE} ${ADF}/${S3_MANIFEST} > ${TMPFILE}
	__check_binary_files ${TMPFILE} ${ADF}
	EC=$?
	if [ $EC -eq $S3_TB_OK ]
	then 
		LogMsg "$ADF is complete, it can be moved"
	fi

	destroy_tmpfile $TMPFILE
	destroy_tmpfile $XSLTFILE
	destroy_tmpdir $TMPDIR
	return $EC
}

function __scan_source {
	local EC=$S3_TB_OK
	local SATELL=$1
	local SOURCEFOLDER=$2
	local DESTFOLDER=$3

	local ADF=""
	local ADFNAME=""
	local MY_ARRAY=""
	local howmany=0
	local counter=0
	local loopspec="adf"

	LogMsg "Scanning $SOURCEFOLDER for formatted ADF (folders with extension ${S3_PROD_EXT})"
	cd $SOURCEFOLDER
	MY_ARRAY=($(ls -1d ${SATELL}*.${S3_PROD_EXT}))
	howmany=${#MY_ARRAY[@]}; counter=1
	if [[ $howmany -gt 0 ]]
	then
		LoopBegin "$loopspec" $howmany
		for ADF in "${MY_ARRAY[@]}"
		do
			ADFNAME=$(basename $ADF)
			ADFTYPE=$(get_type_from_name $ADFNAME)	
			ADFCENTRE=$(get_centre_from_name $ADFNAME)
			LoopMsg "$loopspec" $howmany $counter "adftype=$ADFTYPE adf=$ADFNAME centre=$ADFCENTRE"; let "counter += 1"
			__check_if_completed $ADF
			EC=$?
			if [ $EC -eq $S3_TB_OK ]
			then 
				__handle_ADF_formatted $ADF $ADFNAME $ADFTYPE $ADFCENTRE $DESTFOLDER
				EC=$?
			else
				LogMsg "$ADF not complete yet, skipped"
			fi
		done
		LoopEnd "$loopspec"
	fi

    LogMsg "Scanning $SOURCEFOLDER for unformatted ADF (files with extension EOF)"
    cd $SOURCEFOLDER
    MY_ARRAY=($(ls -1d ${SATELL}*.EOF))
    howmany=${#MY_ARRAY[@]}; counter=1
    if [[ $howmany -gt 0 ]]
    then
            LoopBegin "$loopspec" $howmany
            for ADF in "${MY_ARRAY[@]}"
            do
                    ADFNAME=$(basename $ADF)
                    ADFTYPE=$(get_type_from_name $ADFNAME)
                    ADFCENTRE="N/A"
                    LoopMsg "$loopspec" $howmany $counter "adftype=$ADFTYPE adf=$ADFNAME centre=$ADFCENTRE"; let "counter += 1"
                    __handle_ADF_unformatted $ADF $ADFNAME $ADFTYPE $ADFCENTRE $DESTFOLDER
                    EC=$?
            done
            LoopEnd "$loopspec"
    fi

	return $EC
}

function checkifpreviousrunning
{
	LogMsg "Checking if previous instance is still running..."
        if [[ $(pgrep -f $1) != "$$" ]]
        then
		LogMsg "Another instance of $1 still running; exiting..."
                exit 0
        fi
}

# Environment settings
TB_SCRIPTS=$(dirname $0)
EXE_FILE=$(basename $0)
CENTRE_ID=PDMC
SATELL="S3"

source $TB_SCRIPTS/sysutils
source $TB_SCRIPTS/timeutils
LogBegin $0
EC=$S3_TB_OK

allowed_centre_id PDMC
case $# in
2 )
	SOURCEFOLDER=$1
	DESTFOLDER=$2
;;
* )
	EC=$S3_TB_ERROR
	__print_usage
	LogEnd $0
	exit $EC
;;
esac;

#checkifpreviousrunning $EXE_FILE
LogMsg "Checking if previous instance is still running..."
if pidof -x "$1"-o $$ >/dev/null;then
   LogMsg "Another instance of $1 still running; exiting..."
   exit 0
fi

LogMsg "Going to scan $SOURCEFOLDER..."
__scan_source $SATELL $SOURCEFOLDER $DESTFOLDER
EC=$?

LogEnd $0
exit $EC

