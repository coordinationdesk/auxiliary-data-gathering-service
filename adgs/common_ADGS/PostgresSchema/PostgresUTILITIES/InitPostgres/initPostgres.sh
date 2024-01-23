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
#   $Prod: A.C.S. Postgres initialization and configuration script $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.1.1.1  2017/02/17 10:16:17  achnat
#	Import InitPostgres
#	






res_msg() {
	case "$1" in
		'0')
			echo_success
			echo ""
			;;
		'1')
			echo_failure
			echo ""
			;;
		'2')
			echo_warning
			echo ""
			;;
		+ )
			;;
	esac
}


exit_error() {
	echo $dashLine
	echo "*** INITIALIZATION ABORTED ***"
	echo ""
	exit 1
}






################################   MAIN   ################################
##########################################################################

# Source function library
. /etc/rc.d/init.d/functions

export dashLine="-------------------------------------------------------------------"
echo ""
echo "POSTGRES INITIALIZATION"
echo $dashLine

export EXE_DIR=`dirname $0`
cd $EXE_DIR
export EXE_DIR=`pwd`

if test "$(whoami)" != "root"
then
	echo "*** You must be root to run this script ***"
	echo ""
	exit 1
fi


echo -n "Checking for running postgres ..."
if test -z "$( ps -ef | grep postgres | grep -v grep)"
then
	res_msg 0
else
	res_msg 1
	echo "*** ERROR!!! A server postgres already running ***"
	exit_error
fi


echo -n "Checking for postgres version ..."
initScript=$(ls -1 /etc/init.d/ | grep postgres)
if test -z "$initScript"
then
	res_msg 1
	echo "*** ERROR!!! Cannot find postgres init script ***"
	exit_error
fi
if [ $(echo "$initScript" | tr -s " " "\n" | wc -l) != 1 ]
then
	res_msg 2
	echo "*** WARNING!!! There are more than on version of postgres init script ***"
	question="Do you want use the last version? (y|n): "
	while [ "$choose" != "y" ] && [ "$choose" != "n" ]
	do
		echo -n "$question"
		read choose
	done
	if [ "$choose" == "y" ]
	then
		initScript=$(echo "$initScript" | tr -s " " "\n" | tail -1)
	else
		exit_error
	fi
else
	res_msg 0
fi


echo -n "Checking for installed postgres ..."
posVersion=${initScript:(-3)}
posDir="/var/lib/pgsql/$posVersion"
if test -d $posDir
then
	if [ $(find $posDir/data -type f | wc -l) != 0 ]
	then
		res_msg 1
		echo "*** ERROR!!! an istance of postgres $posVersion is already initialized ***"
		exit_error
	fi
else
	res_msg 1
	echo "*** ERROR!!! postgres $posVersion is not installed ***"
	exit_error
fi
res_msg 0


echo -n "Initializing postgres ..."
/etc/init.d/$initScript initdb 1> /dev/null
if [ "$?" != 0 ]
then
	res_msg 1
	echo "*** ERROR!!! Cannot initialize postgres ***"
	exit_error
fi
res_msg 0


echo -n "Configuring pg_hba.conf file ..."
cp $posDir/data/pg_hba.conf $posDir/data/pg_hba.conf.original
if [ "$?" != 0 ]
then
	res_msg 1
	echo "*** ERROR!!! Cannot copy pg_hba.conf file ***"
	exit_error
fi
echo -e "\n# Modified by ACS\n" > $posDir/data/pg_hba.conf
echo "# TYPE  DATABASE        USER            ADDRESS                 METHOD" >> $posDir/data/pg_hba.conf
echo "local   all             postgres                                ident" >> $posDir/data/pg_hba.conf
echo "host    all             all             0.0.0.0/0               md5" >> $posDir/data/pg_hba.conf
echo "host    all             all             ::1/128                 md5" >> $posDir/data/pg_hba.conf
chown postgres.postgres $posDir/data/pg_hba.conf
chmod 600 $posDir/data/pg_hba.conf
res_msg 0


echo -n "Configuring postgresql.conf file ..."
cp $posDir/data/postgresql.conf $posDir/data/postgresql.conf.original
if [ "$?" != 0 ]
then
	res_msg 1
	echo "*** ERROR!!! Cannot copy postgresql.conf file ***"
	exit_error
fi
sed -i "s/listen_addresses =.*/listen_addresses = '*'/" $posDir/data/postgresql.conf
res1="$?"
sed -i "s/#listen_addresses/listen_addresses/" $posDir/data/postgresql.conf
res2="$?"
if [ "$res1" != 0 ] || [ "$res2" != 0 ]
then
	res_msg 1
	echo "*** ERROR!!! Cannot configure postgresql.conf file ***"
	exit_error
fi
chown postgres.postgres $posDir/data/postgresql.conf
chmod 600 $posDir/data/postgresql.conf
res_msg 0


echo -n "Starting postgres ..."
/etc/init.d/$initScript start 1> /dev/null
if [ "$?" != 0 ]
then
	res_msg 1
	echo "*** ERROR!!! Cannot start postgres ***"
	exit_error
fi
res_msg 0


echo -n "Creating acsdba superuser ..."
su - postgres -c "psql postgres -c \"CREATE ROLE acsdba LOGIN SUPERUSER CREATEDB CREATEROLE PASSWORD 'estestest';\"" 1> /dev/null
if [ "$?" != 0 ]
then
	res_msg 1
	echo "*** ERROR!!! Cannot create acsdba user ***"
	exit_error
fi
res_msg 0


echo $dashLine
echo ">> INITIALIZATION COMPLETED"
echo ""
exit 0




