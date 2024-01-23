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
	Revision 2.4  2013/05/14 18:00:35  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.3  2012/12/14 11:13:41  marpas
	coding best practices
	qa rules
	test improeved
	test coverage
	
	Revision 2.2  2010/04/09 17:23:22  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.1  2009/06/26 09:48:18  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.3  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.2  2003/04/30 13:07:28  marpas
	using namespace std no longer in includes
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/


#ifndef _dbGeoStationSet_H_
#define _dbGeoStationSet_H_

#include <dbGeoRing.h>
#include <vector>


_ACS_BEGIN_NAMESPACE(acs)

//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/  dbGeoStationSet   _/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

class rsResourceSet ;
class dbGeoStation ;

class dbGeoStationSet  // PRQA S 2109
{
public :
	dbGeoStationSet() ;
	 ~dbGeoStationSet() throw() ;
		
	void clear() ;
	void fill(const rsResourceSet &) ; // PRQA S 2502
	
	size_t size() const ;
	const dbGeoStation &operator[](size_t) const ;
	dbGeoStation &operator[](size_t) ;
	
private:
	dbGeoStationSet(const dbGeoStationSet &) ;
	dbGeoStationSet &operator=(const dbGeoStationSet & ) ;
	
private:
	std::vector<dbGeoStation> _stations ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbGeoStationSet) ;	
} ;

} // namespace 

#endif // _dbGeoStationSet_H_


