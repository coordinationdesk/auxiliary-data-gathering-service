// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Database Geo Object Library $

	$Id$

	$Author$

	$Log$
	Revision 2.4  2013/05/14 18:00:34  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.3  2012/12/14 11:13:41  marpas
	coding best practices
	qa rules
	test improeved
	test coverage
	
	Revision 2.2  2010/04/09 17:23:21  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.1  2009/06/26 09:48:17  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.2  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.1  2003/05/22 09:52:32  marpas
	new class added
	
	

*/


#ifndef _dbGeoObjectGuard_H_
#define _dbGeoObjectGuard_H_

#include <acs_c++config.hpp>


_ACS_BEGIN_NAMESPACE(acs)

class dbGeoObject ;
	
class dbGeoObjectGuard // PRQA S 2109
{

public :
	explicit dbGeoObjectGuard(const dbGeoObject *) ;
	virtual ~dbGeoObjectGuard()  throw() ;
private:
// declared but not defined
	dbGeoObjectGuard() ;
	dbGeoObjectGuard(const dbGeoObjectGuard &) ;
	dbGeoObjectGuard &operator=(const dbGeoObjectGuard &) ;
private:
	const dbGeoObject *_guarded ;
} ;

_ACS_END_NAMESPACE




#endif //_dbGeoObjectGuard_H_ 

