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
#   $Prod: A.C.S. Databases Schema creation script $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.5  2016/05/13 12:35:18  achnat
#	Added extensions creation
#	
#	Revision 1.4  2014/05/05 13:24:06  achnat
#	Added comment installation.
#	
#	Revision 1.3  2011/03/31 09:29:45  achnat
#	Commented primarykeys creation.
#	
#	Revision 1.2  2009/06/30 13:28:48  achnat
#	Implemented new security constraints.
#	
#	Revision 1.1.1.1  2009/02/05 10:38:46  achnat
#	Import psqlCreateDatabase
#	








cs_usage() {
	echo "usage: createSchema <hostname> <dbauser> <dbname> <objec_type>"
	echo "--------------------------------------------------------------"
	echo "object types:"
	echo "   alterations"
	echo "   comments"
	echo "   constraints"
	echo "   extensions"
	echo "   foreignkeys"
	echo "   functions"
	echo "   indexes"
	#echo "   primaryKeys"
	echo "   partitions"
	echo "   procedures"
	echo "   tables"
	echo "   triggers"
	echo "   triggersFunctions"
	echo "   viewes"
	echo "--------------------------------------------------------------"
	echo ""
	exit 1
}



echo ""
echo "PSQL DATABASE SCHEMA CREATOR"
echo "--------------------------------------------------------------"

if test "$1" = "help" || test "$1" = "--help"
then
	cs_usage
fi

if test -z "$4"
then
	cs_usage
fi

case "$4" in
	'alterations') ;;
	'comments') ;;
	'constraints') ;;
	'extensions') ;;
	'foreignkeys') ;;
	'functions') ;;
	'indexes') ;;
	#'primaryKeys') ;;
	'partitions') ;;
	'procedures') ;;
	'tables') ;;
	'triggersFunctions') ;;
	'triggers') ;;
	'viewes') ;;
	*)
		cs_usage ;;
esac

if test -z "$PGPASSWORD"
then
	echo -n "Insert $2 password: "
	read -s answer
	export PGPASSWORD=$answer
	echo ""
fi

echo "Creating $4 on database $3 ..."

psql -h $1 -U $2 $3 -v "ON_ERROR_STOP=1" -c "\i $DATABASE_DIR/schema/$4.sql"
if [ "$?" != 0 ]
then
	echo "--------------------------------------------------------------"
	echo "Error during $4 creation."
	echo ""
	exit 1
else
	echo "--------------------------------------------------------------"
	echo "$4 creation completed."
	echo ""
	exit 0
fi


