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





usage() {
	echo "usage: configure [postgresql_conf_path]"
	echo "-------------------------------------------------------------------"
	echo "supported pgsql versions:"
	echo "   9.0      PostgreSQL 9.0.*"
	echo "   9.1      PostgreSQL 9.1.*"
	echo "   9.2      PostgreSQL 9.2.*"
	echo "   9.3      PostgreSQL 9.3.*"
	echo "   9.4      PostgreSQL 9.4.*"
	echo "   9.5      PostgreSQL 9.5.*"
	echo "   9.6      PostgreSQL 9.6.*"
	echo "   10       PostgreSQL 10.*"
	echo "   11       PostgreSQL 11.*"
	echo "-------------------------------------------------------------------"
	echo ""
	exit 1
}


err_msg() {
	echo "-------------------------------------------------------------------"
	echo "*** ERROR!!! $1 ***"
	echo "Cannot restart Postgres"
	echo ">> CONFIGURATION WILL BE COMPLETED ONLY AFTER POSTGRES RESTART"
	echo ""
	exit 1
}


check_conf() {
	while ! test -r $CONF_FILE
	do
		echo "Cannot find postgresql.conf file or is not readable."
		echo -n "Insert postgresql.conf path: "
		read check_path
		CONF_FILE=$check_path/postgresql.conf
	done
}


change_line() {
	echo -n "  changing $1 to $2 ..."
	
	#sed -i "s/^.*$1 = \b.*/$1 = $2/" $CONF_FILE
	sed -i "s/$1 =.*/$1 = $2/" $CONF_FILE
	res1="$?"
	sed -i "s/#$1/$1/" $CONF_FILE
	res2="$?"
	
	if [ "$res1" != 0 ] || [ "$res2" != 0 ]
	then
		((err_count++))
		echo_failure
		echo ""
	else
		echo_success
		echo ""
	fi
}


restart_server() {
	echo ""
	echo "Restarting server ..."
	
	OS_RLS=$(lsb_release -r | awk '{print $2}' | awk -F '.' '{print $1}')
	if [ ! "$OS_RLS" ]; then err_msg "Cannot determine OS release"; fi
	
	if [ "$OS_RLS" -lt "7" ]
	then
		PSQL_INIT=/etc/init.d/$PSQ_SRV
		if ! test -x "$PSQL_INIT"; then	err_msg "Cannot find Postgres init file"; fi
		
		$PSQL_INIT restart
		if [ "$?" != 0 ]; then err_msg "error during server restart"; fi
	else
		systemctl restart $PSQ_SRV
		if [ "$?" != 0 ]; then err_msg "error during server restart"; fi
	fi
}





################################ MAIN ################################
######################################################################

echo ""
echo "POSTGRES SERVER CONFIGURATOR"
echo "-------------------------------------------------------------------"


# Source function library
. /etc/rc.d/init.d/functions 


# Check running user
if test "`whoami`" != "root"
then
	echo "*** You must be root to run this script ***"
	echo ""
	exit 1
fi

export in_param="$1"

# Check version and postgres path 
PSQL_MJR_VRS="$(psql --version | awk '{print $3}' | awk -F "." '{print $1}' | head -1)"
PSQL_TOT_VRS="$(psql --version | awk '{print $3}' | awk -F "." '{print $1"."$2}' | head -1)"

echo "PostgreSQL version found: $PSQL_TOT_VRS"
if [ "$PSQL_MJR_VRS" -lt "10" ]
then
	case "$PSQL_TOT_VRS" in
		'9.0' | '9.1' | '9.2' | '9.3' | '9.4' | '9.5' | '9.6')
			export CONF_DIR=/var/lib/pgsql/$PSQL_TOT_VRS/data
			export PSQ_SRV=postgresql-$PSQL_TOT_VRS
			;;
		*)
			echo -e "\n*** ERROR!!! Unsupported Postgres version ***\n"
			usage
			;;
	esac
else
	case "$PSQL_MJR_VRS" in
		'10' | '11')
			export CONF_DIR=/var/lib/pgsql/$PSQL_MJR_VRS/data
			export PSQ_SRV=postgresql-$PSQL_MJR_VRS
			;;
		*)
			echo -e "\n*** ERROR!!! Unsupported Postgres version ***\n"
			usage
			;;
	esac
fi

if [ "$in_param" ]
then
	export CONF_DIR="$in_param"
fi

export CONF_FILE=$CONF_DIR/postgresql.conf

typeset -i err_count
err_count=0


# Check configuration file
check_conf


# Check memory and set variables
TOT_MEM=$(free | grep Mem | awk '{print $2}')
echo -e "Total system memory found: $TOT_MEM \n"

# RAM greater equal to 32GB
if test "$TOT_MEM" -ge 32696908
then
	max_con=1000
	shd_buf=16GB
	tmp_buf=256MB
	wrk_mem=32MB
	mnt_wrk_mem=4GB
	wal_buf=256MB

# RAM between 16GB and 32GB
elif test "$TOT_MEM" -ge 16348454 && test "$TOT_MEM" -lt 32696908
then
	max_con=500
	shd_buf=8GB
	tmp_buf=128MB
	wrk_mem=16MB
	mnt_wrk_mem=2GB
	wal_buf=128MB

# RAM between 8GB and 16GB
elif test "$TOT_MEM" -ge 8174227 && test "$TOT_MEM" -lt 16348454
then	
	max_con=400
	shd_buf=2GB
	tmp_buf=128MB
	wrk_mem=16MB
	mnt_wrk_mem=1GB
	wal_buf=64MB

# RAM between 4GB and 8GB
elif test "$TOT_MEM" -ge 4194304 && test "$TOT_MEM" -lt 8174227
then	
	max_con=400
	shd_buf=2GB
	tmp_buf=128MB
	wrk_mem=16MB
	mnt_wrk_mem=512MB
	wal_buf=32MB

# RAM between 2GB and 4GB
elif test "$TOT_MEM" -ge 2097152 && test "$TOT_MEM" -lt 4194304
then 
	max_con=300
	shd_buf=1GB
	tmp_buf=64MB
	wrk_mem=8MB
	mnt_wrk_mem=256MB
	wal_buf=16MB

# RAM less than 2GB
else 
	max_con=200
	shd_buf=512MB
	tmp_buf=32MB
	wrk_mem=8MB
	mnt_wrk_mem=128MB
	wal_buf=8MB
fi


# Create a backup of conf file and change values
echo "Configuring postgresql.conf:"
if ! test -e "$CONF_FILE.original"
then
	cp $CONF_FILE $CONF_FILE.original
	chown postgres. $CONF_FILE.original
fi
change_line "max_connections" "$max_con"
change_line "shared_buffers" "$shd_buf"
change_line "temp_buffers" "$tmp_buf"
change_line "max_prepared_transactions" "0"
change_line "work_mem" "$wrk_mem"
change_line "maintenance_work_mem" "$mnt_wrk_mem"
change_line "autovacuum_work_mem" "-1"
change_line "wal_buffers" "$wal_buf"
change_line "max_wal_size" "4GB"  # max_wal_size = (3 * checkpoint_segments) * 16MB
change_line "checkpoint_segments" "64"
change_line "checkpoint_completion_target" "0.9"
change_line "log_min_duration_statement" "2000"
change_line "log_lock_waits" "on"
change_line "deadlock_timeout" "2s"
change_line "log_timezone" "'UTC'"
change_line "timezone" "'UTC'"

restart_server

if (( $err_count != 0 ))
then
	echo "-------------------------------------------------------------------"
	if (( $err_count == 1 ))
	then
		echo "*** $err_count ERROR is occurred ***"
	else
		echo "*** $err_count ERROR are occurred ***"
	fi
	echo ">> CONFIGURATION COMPLETED WITH ERRORS"
	echo ""
	exit 2
fi

echo "-------------------------------------------------------------------"
echo ">> SERVER SUCCESSFULLY CONFIGURED"
echo ""
exit 0


