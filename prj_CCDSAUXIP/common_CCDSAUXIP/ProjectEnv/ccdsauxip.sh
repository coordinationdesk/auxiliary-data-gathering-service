#
#	Copyright 1995-2022, Exprivia SPA - DFDA-AS
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.exprivia.it
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia SPA.
#

if [ "$0" == "$BASH_SOURCE" ]
then
	echo "Must source and not execute this script."
	exit 1
fi

project=ccdsauxip

# the PROJECT env var is used inside Makefile for specific project purposes
PROJECT=${project^^}
export PROJECT

myself=$BASH_SOURCE

# The main entry level for the whole project
PRJROOTTREE=$(readlink -f ${myself} | rev | cut -d '/' -f4- | rev)
export PRJROOTTREE


# the setting needed from the makefiles
LIB_INSTALL_PATH=${PRJROOTTREE}
LIB_INTEGRATION_PATH=${LIB_INSTALL_PATH}
EXTCOMP_PATH=${LIB_INSTALL_PATH}
EXE_INTEGRATION_PATH=${LIB_INSTALL_PATH}/bin

export LIB_INSTALL_PATH LIB_INTEGRATION_PATH EXE_INTEGRATION_PATH EXTCOMP_PATH

#PATH setting
PATH=${PRJROOTTREE}/tools:${PATH}
export PATH

# the umask value is default to 002 instead that 022 
# to allow everyone on the same group to edit files
umask 002

# an alias to get the right dev start point
alias ${project}="cd ${PRJROOTTREE}"
alias ${project}c="cd ${PRJROOTTREE}/components"
alias ${project}a="cd ${PRJROOTTREE}/applications"




