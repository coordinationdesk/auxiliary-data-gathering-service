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
#   $Prod: A.C.S. Owner changer procedure $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.2  2009/06/30 13:28:48  achnat
#	Implemented new security constraints.
#	
#	Revision 1.1  2009/02/05 17:56:14  achnat
#	Added changeOwner script.
#	








co_usage() {
	echo "usage: changeOwner.sh <hostname> <dbauser> <dbname> <owner> <object>"
	echo "--------------------------------------------------------------------"
	echo "objects list:"
	echo "   f     functions and trigger functions"
	echo "   s     sequences"
	echo "   t     tables"
	echo "   v     views"
	echo "--------------------------------------------------------------------"
	echo ""
	exit 1
}

check_conn() {
	echo "Checking connection on $DBNAME ..."
	psql -h $HNAME -U $DBAUSR $DBNAME -c "\l" > /dev/null
	if [ "$?" != 0 ]
	then
		echo "*** ERROR: changeOwner - Cannot connect to $DBNAME ***"
		echo ""
		exit 1
	fi
	
	echo ">> Connection done."
	echo ""
}

change_owner() {
	for i in `psql -h $HNAME -U $DBAUSR $DBNAME -v "ON_ERROR_STOP=1" -c "$2" | grep public | awk '{ print $3 }'`
	do
		echo "Changing $i owner to $OWNUSR ..."
		psql -h $HNAME -U $DBAUSR $DBNAME -v "ON_ERROR_STOP=1" -c "ALTER $1 $i OWNER TO $OWNUSR;"
		if [ "$?" != 0 ]
		then
			echo "*** ERROR: changeOwner - Cannot change owner to $i ***"
			echo ""
			exit 1
		fi
	done
}





########## MAIN ##########

echo ""
echo "PSQL CHANGE OWNER"
echo "--------------------------------------------------------------------"

# Check first option
if test "$1" = "help" || test "$1" = "--help"
then
	co_usage
fi

if test -z "$5"
then
	co_usage
fi

export HNAME=$1
export DBAUSR=$2
export DBNAME=$3
export OWNUSR=$4

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
case "$5" in
	'f' )
		#change_owner FUNCTION "\df"
		;;
	's' )
		change_owner TABLE "\ds"
		;;
	't' )
		change_owner TABLE "\dt"
		;;
	'v' )
		change_owner TABLE "\dv"
		;;
	* )
		co_usage
		;;
esac


echo "--------------------------------------------------------------------"
echo ">> Owner Changed."
echo ""
exit 0


