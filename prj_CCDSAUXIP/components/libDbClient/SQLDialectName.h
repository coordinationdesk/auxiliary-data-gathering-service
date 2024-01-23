// PRQA S 1050 EOF
/*

	Copyright 1995-2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:  $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2012/12/11 17:30:10  marpas
	qa rules
	
	Revision 1.1  2012/08/02 14:16:59  chicas
	 libDbClient REFACTORY; just added
	
	
*/

#ifndef _SQLDialectName_H_
#define _SQLDialectName_H_

//_ACS_BEGIN_NAMESPACE(acs)
_ACS_BEGIN_NESTED_NAMESPACE(acs,db)

static const char * INFORMIX = "Informix9";
static const char * POSTGRESS = "Postgres8";
static const char * ORACLE = "Oracle11";
static const char * GENERICKEY = "All";

_ACS_END_NESTED_NAMESPACE
//_ACS_END_NAMESPACE

#endif


