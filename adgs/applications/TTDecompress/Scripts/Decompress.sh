#!/bin/bash
#
# Copyright 1995-2009, Advanced Computer Systems , Inc.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.acsys.it
#
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
# the contents of this file may not be disclosed to third parties, copied or
# duplicated in any form, in whole or in part, without the prior written
# permission of Advanced Computer Systems, Inc.
#
# Written by giapas
#
# $Prod: A.C.S. Decompress.sh bash-script $
#
# $Id$
# 
# $Author$
#
#
# USAGE: <script_name> ARCHIVE_FILE [DESTINATION_DIR]
#


# User-defined function: convert UPPER to lower
function tolower { tr '[A-Z]' '[a-z]'; }


#Check if input parameters provided
if [ $# -lt 1 ]
then
	echo "Usage: $0  Source_ARCHIVE [Destination_PATH]"
	exit 1
fi


EXTENSION=""

tar_PAR=-xf
tar_pipe_PAR=-x
gzip_tar_PAR=-z
bzip2_tar_PAR="--use-compress-program bzip2"
gzip_PAR=-dc
bzip2_PAR=-dc
Z_PAR=-c
zip_PAR=-qo
zip_tar_PAR=-qop
sevenz_PAR=x 
sevenz_tar_PAR="x -so" 
sevenz_EXECUTABLE=7za

FILENAME_LOWER=$( echo "$1" | tolower )


# CHECK IF ARCHIVE_NAME is passed as 1st argument. OTHERWISE ERROR
if [ ! -f "$1" ]
then
	exit 1
fi


# CHECK IF $DIR IS GIVEN AS ARGUMENT. OTHERWISE, is taken from the path of archive
if [ "$2" = "" ]
then
	DIR=`dirname $1`
else
	DIR=$2
fi


# CHECK IF $DIR IS EXECUTABLE AND WRITABLE
if [ -d "$DIR" ]
then
	if [ ! -x $DIR -o ! -w $DIR ]
	then
		exit 1
	fi
else	
	mkdir -p $DIR
	if [ $? != 0 ];
	then
		exit 1
	fi
fi


# CHECK FOR EXTENSION:
NONTAR="N"
if [ `expr match $FILENAME_LOWER '.*\.tar$'` -ne 0 ]
then
	EXTENSION="tar"
fi

if [ `expr match $FILENAME_LOWER '.*\.z$'` -ne 0 ]
then
	EXTENSION="Z"; NONTAR="Y"
fi

if [ `expr match $FILENAME_LOWER '.*\.gz$'` -ne 0 ]
then
	EXTENSION="gz"; NONTAR="Y"
fi

if [ `expr match $FILENAME_LOWER '.*\.bz$'` -ne 0 ]
then
	EXTENSION="bz"; NONTAR="Y"
fi

if [ `expr match $FILENAME_LOWER '.*\.bz2$'` -ne 0 ]
then
	EXTENSION="bz2"; NONTAR="Y"
fi

if [ `expr match $FILENAME_LOWER '.*\.zip$'` -ne 0 ]
then
	EXTENSION="zip"; NONTAR="Y"
fi

if [ `expr match $FILENAME_LOWER '.*\.7z$'` -ne 0 ]
then
	EXTENSION="7z"; NONTAR="Y"
fi

if [ `expr match $FILENAME_LOWER '.*\.tar\.z$'` -ne 0 -o `expr match $FILENAME_LOWER '.*\.tz$'` -ne 0 ]
then
	EXTENSION="tar.Z"
fi

if [ `expr match $FILENAME_LOWER '.*\.tar\.gz$'` -ne 0 -o `expr match $FILENAME_LOWER '.*\.tgz$'` -ne 0 ]
then
	EXTENSION="tar.gz"
fi

if [ `expr match $FILENAME_LOWER '.*\.tar\.bz$'` -ne 0 -o `expr match $FILENAME_LOWER '.*\.tbz$'` -ne 0 ]
then
	EXTENSION="tar.bz"
fi

if [ `expr match $FILENAME_LOWER '.*\.tar\.bz2$'` -ne 0 -o `expr match $FILENAME_LOWER '.*\.tbz2$'` -ne 0 ]
then
	EXTENSION="tar.bz2"
fi

if [ `expr match $FILENAME_LOWER '.*\.tar\.zip$'` -ne 0 -o `expr match $FILENAME_LOWER '.*\.tzip$'` -ne 0 ]
then
	EXTENSION="tar.zip"
fi

if [ `expr match $FILENAME_LOWER '.*\.tar\.7z$'` -ne 0 -o `expr match $FILENAME_LOWER '.*\.t7z$'` -ne 0 ]
then
	EXTENSION="tar.7z"
fi

# Remove both path and archive extension from filename
# (Used only with non-tar archives)
if [ "$NONTAR" = "Y" ]; then
	BASE=`basename $1`
	EXPR=$(( `expr length $BASE` - `expr length $EXTENSION` - 1 ))
	FILTERED_FILENAME=`expr substr $BASE 1 $EXPR`
fi

RETCODE=0;

case  "$EXTENSION" in
	"tar"		) tar $tar_PAR $1 -C $DIR; RETCODE="$?"
			;;
	"Z"			) gzip $gzip_PAR $1 > $DIR"/"$FILTERED_FILENAME; RETCODE="$?"
			;;	# Z: since "compress" no more provided. gzip is also compress-compatible
	"gz"		) gzip $gzip_PAR $1 > $DIR"/"$FILTERED_FILENAME; RETCODE="$?"
			;;
	"bz"		) bzip2 $bzip2_PAR $1 > $DIR"/"$FILTERED_FILENAME; RETCODE="$?"
			;; # bz untested
	"bz2"		) bzip2 $bzip2_PAR $1 > $DIR"/"$FILTERED_FILENAME; RETCODE="$?"
			;;
	"zip"		) unzip $zip_PAR $1 -d $DIR; RETCODE="$?"
			;;
    "7z"		) $sevenz_EXECUTABLE $sevenz_PAR $1 -o$DIR; RETCODE="$?"
			;;
	"tar.Z"		) gzip $gzip_PAR $1 | tar $tar_pipe_PAR -C $DIR; RETCODE="$?"
			;;
	"tar.gz"	) tar $gzip_tar_PAR $tar_PAR $1 -C $DIR; RETCODE="$?"
			;;
	"tar.bz"	) tar $bzip2_tar_PAR $tar_PAR $1 -C $DIR; RETCODE="$?"
			;; 
	"tar.bz2"	) tar $bzip2_tar_PAR $tar_PAR $1 -C $DIR; RETCODE="$?"
			;;
	"tar.zip"	) unzip $zip_tar_PAR $1 | tar $tar_pipe_PAR -C $DIR; RETCODE="$?"
			;;
	"tar.7z"	) $sevenz_EXECUTABLE $sevenz_tar_PAR $1 | tar $tar_pipe_PAR -C $DIR; RETCODE="$?"
			;;
	*		) RETCODE="1"
				# CASE ELSE
			;;
esac


if [ "$RETCODE" -ne 0 ]
then
	echo "Error running command"
	exit 1
fi


# exit script

exit 0 ;
# THE END
