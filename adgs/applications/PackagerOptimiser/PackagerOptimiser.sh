#!/bin/bash
#
# Copyright 1995-2002, Advanced Computer Systems , Inc.
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
# Written by enrcar
#
# $Prod: A.C.S. PackagerOptimiser.sh bash-script $
#
# $Id$
# 
# $Author$
#


########################## CHECK PARAMETERS BELOW:
# APPLICATION:
# 	Application name, for example:
#	APPLICATION="/usr/bin/MyApp"
APPLICATION="PackagerOptimiser"
# USER:
#	User who wants to run the application, for example:
#	USER="guest"
USER="opCRYOSAT"
#
##################################################


# User-defined function: convert UPPER to lower
function tolower { tr '[A-Z]' '[a-z]'; }


#Check if input parameters provided
if [ $# -ne 1 ]
then
	echo "Usage: $0  start stop abort kill"
	exit 1
fi

# Get command in lowercase:
COMMAND_LOWER=$( echo "$1" | tolower )


# start
if [ "$COMMAND_LOWER" = "start" ]; then
	su - "$USER" -c	"exec $APPLICATION"
fi

# stop
if [ "$COMMAND_LOWER" = "stop" ]; then
	su - "$USER" -c "killall -s INT $APPLICATION"
fi

# abort 
if [ "$COMMAND_LOWER" = "abort" ]; then
	su - "$USER" -c	"killall -s TERM $APPLICATION"
fi

# kill
if [ "$COMMAND_LOWER" = "kill" ]; then
	su - "$USER" -c "killall -s KILL $APPLICATION"
fi

