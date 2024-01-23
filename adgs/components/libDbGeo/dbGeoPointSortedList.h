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
	Revision 2.6  2013/05/14 18:00:35  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.5  2012/12/14 11:13:41  marpas
	coding best practices
	qa rules
	test improeved
	test coverage
	
	Revision 2.4  2012/05/21 13:22:36  matteo.airoldi
	Added include of cstdio
	
	Revision 2.3  2012/02/13 11:15:39  marpas
	refactoring in progress
	
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




#ifndef _dbGeoPointSortedList_H_
#define _dbGeoPointSortedList_H_ 

#include <acs_c++config.hpp>
#include <cstdio>
#include <vector>


_ACS_BEGIN_NAMESPACE(acs)

class dbGeoPoint ;
class DateTime ;


class dbGeoPointSortedList // PRQA S 2109
{
public:

	dbGeoPointSortedList() ;
	dbGeoPointSortedList(const dbGeoPointSortedList&) ;
	virtual ~dbGeoPointSortedList()  throw() ;
	
	dbGeoPointSortedList &operator=(const dbGeoPointSortedList &) ;
	dbGeoPointSortedList &operator+=(const dbGeoPointSortedList &) ;
	dbGeoPointSortedList &operator+=(const dbGeoPoint &) ;
	const dbGeoPoint &operator[](size_t i) const ; // PRQA S 2141

	size_t points() const ; // return the number of points
	void append(const dbGeoPoint &) ;
	
	virtual void erase(size_t) ; 
	virtual void erase(const dbGeoPoint &) ;
	
	void range(const DateTime &, size_t &,size_t &) const ; // search for time
	void range(double, size_t &,size_t &) const ; // search for latitude
private:
	void sort() const ; // note, this change mutable private members 
private:
	mutable std::vector<dbGeoPoint> _points ;
	mutable bool _sorted ;
} ;


_ACS_END_NAMESPACE

#endif // _dbGeoPointSortedList_H_

