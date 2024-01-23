// PRQA S 1050 EOF
/*
	Copyright 2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 4.3  2013/05/24 11:09:53  marpas
	qa rules
	
	Revision 4.2  2013/04/09 07:53:08  marpas
	./NameIdAssociator refactoring
	new statistics
	
	Revision 4.1  2013/04/05 12:45:53  marpas
	Statistics are now grouped for db key
	

*/

#ifndef _dbStat_H_
#define _dbStat_H_

#include <Statistics.h>


_ACS_BEGIN_NESTED_NAMESPACE(acs, db)

inline
acs::math::StatNode &stat(const std::string &key) { return (*acs::math::Statistics::instance())()["DBK: \""+key+ "\""] ; } // PRQA S 2134, 2502

_ACS_END_NESTED_NAMESPACE

#endif // _dbStat_H_
