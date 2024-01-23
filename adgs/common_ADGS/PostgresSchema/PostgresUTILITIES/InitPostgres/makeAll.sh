#!/bin/sh
#
# Copyright 2010-2020, Advanced Computer Systems , Inc.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.
# 
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
# the contents of this file may not be disclosed to third parties, copied or
# duplicated in any form, in whole or in part, without the prior written
# permission of Advanced Computer Systems, S.p.A.
#	
#
#   $Prod: A.C.S. Script to install and configure Postgres and create project database$
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.1  2017/02/20 10:54:17  achnat
#	Added makeAll.sh
#	








usage() {
	echo -e "\nusage: $(basename $0)  <project_name>"
	echo ""
}


export EXE_DIR=`dirname $0`
cd $EXE_DIR
export EXE_DIR=$(pwd)
cd ..
export BASE_DIR=$(pwd)

export prj=$1
export PGPASSWORD=estestest
export CONF_FILE=$BASE_DIR/psqlUtilitiesConf/$prj.conf


if test -z "$prj"
then
	usage
	exit 1
fi

if test "$(whoami)" != "root"
then
	echo "*** You must be root to run this script ***"
	echo ""
	exit 1
fi

if ! test -r "$CONF_FILE"
then
	echo "*** Cannot find CONF file or it is not readable ***" | tee -a $LOG_FILE
	echo "expected file: $CONF_FILE"
	exit 1
fi


$EXE_DIR/initPostgres.sh
if [ "$?" != 0 ]; then exit 1; fi

$BASE_DIR/psqlServerConfiguration/configure.sh
if [ "$?" != 0 ]; then exit 1; fi

$BASE_DIR/psqlCreateDatabase/createDatabase $prj
if [ "$?" != 0 ]; then exit 1; fi

/etc/init.d/postgresql-9.5 stop 1> /dev/null
if [ "$?" != 0 ]; then exit 1; fi

exit 0




