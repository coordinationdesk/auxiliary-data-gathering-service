#!/bin/sh
#
# Copyright 1995-2005, Advanced Computer Systems , Inc.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.
# 
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
# the contents of this file may not be disclosed to third parties, copied or
# duplicated in any form, in whole or in part, without the prior written
# permission of Advanced Computer Systems, S.p.A.
#	
#
#   $Prod: A.C.S. Grant assignment procedure $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.3  2013/01/22 15:54:37  achnat
#	Added grant assignment to sequences.
#	
#	Revision 1.2  2013/01/22 13:45:22  achnat
#	Changed log message.
#	
#	Revision 1.1  2013/01/22 13:22:48  achnat
#	Added grants assignments.
#	








gu_usage() {
	echo "usage: assignGrants.sh <hostname> <dbauser> <dbname> <user> <object>"
	echo "--------------------------------------------------------------------"
	echo "objects list:"
	echo "   s     sequences"
	echo "   t     tables"
	echo "   v     views"
	echo ""
	exit 1
}

check_conn() {
	echo "Checking connection on $DBNAME ..."
	psql -h $HNAME -U $DBAUSR $DBNAME -c "\l" > /dev/null
	if [ "$?" != 0 ]
	then
		echo "*** ERROR: assignGrants - Cannot connect to $DBNAME ***"
		echo ""
		exit 1
	fi
	
	echo ">> Connection done."
	echo ""
}

assign_grant() {
	for i in `psql -h $HNAME -U $DBAUSR $DBNAME -v "ON_ERROR_STOP=1" -c "$2" | grep $DBAUSR | awk '{ print $3 }'`
	do
		echo "Assigning grants on $i to $GRNTUSR ..."
		
		case "$1" in
			'SEQUENCE' )
				psql -h $HNAME -U $DBAUSR $DBNAME -v "ON_ERROR_STOP=1" -c "GRANT USAGE, SELECT, UPDATE ON SEQUENCE $i TO $GRNTUSR;"
				res=$?
				;;
			'TABLE' )
				psql -h $HNAME -U $DBAUSR $DBNAME -v "ON_ERROR_STOP=1" -c "GRANT SELECT, INSERT, UPDATE, DELETE ON TABLE $i TO $GRNTUSR;"
				res=$?
				;;
			'VIEW' )
				psql -h $HNAME -U $DBAUSR $DBNAME -v "ON_ERROR_STOP=1" -c "GRANT SELECT ON TABLE $i TO $GRNTUSR;"
				res=$?
				;;
			* )
				res=1
				;;
		esac
		
		if [ "$res" != 0 ]
		then
			echo "*** ERROR: assignGrants - Cannot change owner to $i ***"
			echo ""
			exit 1
		fi
	done
}





########## MAIN ##########

echo ""
echo "PSQL GRANTS ASSIGNMENT"
echo "--------------------------------------------------------------------"

# Check first option
if test "$1" = "help" || test "$1" = "--help"
then
	gu_usage
fi

if test -z "$5"
then
	gu_usage
fi

export HNAME=$1
export DBAUSR=$2
export DBNAME=$3
export GRNTUSR=$4
export OBJ=$5

if test -z "$PGPASSWORD"
then
	echo -n "Insert $DBAUSR password: "
	read -s answer
	export PGPASSWORD=$answer
	echo ""
	echo ""
fi

# Check db connection
check_conn

# Check object option and go
case "$OBJ" in
	's' )
		assign_grant SEQUENCE "\ds"
		;;
	't' )
		assign_grant TABLE "\dt"
		;;
	'v' )
		assign_grant VIEW "\dv"
		;;
	* )
		gu_usage
		;;
esac

echo "--------------------------------------------------------------------"
echo ">> Grants successfully assigned."
echo ""
exit 0


