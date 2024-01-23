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
	
	Revision 2.3  2012/02/13 11:15:39  marpas
	refactoring in progress
	
	Revision 2.2  2010/04/09 17:23:21  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.1  2009/06/26 09:48:17  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.5  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.4  2004/05/27 12:55:13  marpas
	some cosmetic changes,
	new method unionWith in dbGeoPntList
	
	Revision 1.3  2003/04/30 13:07:28  marpas
	using namespace std no longer in includes
	
	Revision 1.2  2003/04/15 18:35:04  marpas
	GCC3.2
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/


#include <dbGeoPointSortedList.h>
#include <dbGeoPoint.h>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


// 
// 
// 		dbGeoPointSortedList
// 		
// 		

dbGeoPointSortedList::dbGeoPointSortedList() : 
	_points(),
	_sorted()
{
}

dbGeoPointSortedList::~dbGeoPointSortedList() throw() 
{
}

dbGeoPointSortedList::dbGeoPointSortedList(const dbGeoPointSortedList &list) :
	_points(list._points ),
	_sorted(list._sorted )
{
	// empty
}

dbGeoPointSortedList &dbGeoPointSortedList::operator=(const dbGeoPointSortedList &list)
{
	if(this != &list) {
	
		_points = list._points ;
		_sorted = list._sorted ;
	}
    return *this ;
}

dbGeoPointSortedList &dbGeoPointSortedList::operator+=(const dbGeoPointSortedList &list)
{
	_points.insert(_points.end(), list._points.begin(), list._points.end());
	_sorted = false ;

    return *this ;
}

dbGeoPointSortedList &dbGeoPointSortedList::operator+=(const dbGeoPoint &pnt)
{
    append(pnt) ;
	return *this ;
}


const dbGeoPoint &dbGeoPointSortedList::operator[](size_t i) const 
{
	if (!_sorted) {
		const_cast<dbGeoPointSortedList *>(this)->sort() ; // PRQA S 3081
    }
	return _points[i] ;
}


void dbGeoPointSortedList::append(const dbGeoPoint &point) // PRQA S 4211
{	
	_points.push_back(point) ;
	_sorted = false ;
}


void dbGeoPointSortedList::erase(size_t i) 
{
	_points.erase(_points.begin()+i) ; // PRQA S 3000
}
 
void dbGeoPointSortedList::erase(const dbGeoPoint &p)
{
	vector<dbGeoPoint>::iterator index = find(_points.begin(), _points.end(),p) ;
	if (index != _points.end()) {
		_points.erase(index) ;
    }
}

void dbGeoPointSortedList::range(const DateTime &t, size_t &i1,size_t &i2) const 
{
	if (!_sorted) {
		sort() ;
    }
	i1 = 0 ;
	i2 = 1 ;
	
	size_t pts = points() ;
	
	for (long i=pts-1; i >=0 ; i--) { // PRQA S 3000, 3084
		if (t >= (*this)[i].timeRange().start()) { // stop the search until the given time is less than the point // PRQA S 3000, 4
			i1 = i ; // the first point whose time is less is returned  // PRQA S 3000
			
			i2 = i+1 ; // the next one is returned // PRQA S 3000
			break ;
		}
	}
	
	if (i1 == pts - 1) {// was the last, reduce all by 1 // PRQA S 3084
		i1-- ;
		i2-- ;
	}
}


void dbGeoPointSortedList::range(double lat, size_t &i1,size_t &i2) const 
{
	if (!_sorted) {
		sort() ;
    }

	size_t pts = points() ;
	
	if ((*this)[0].lat() > (*this)[pts-1].lat()) { // descending order lat[0] > lat[last] // PRQA S 3084
		i1 = pts-1 ; // PRQA S 3084 2
		i2 = pts-2 ; // PRQA S 4400

		for (size_t i=0; i <pts ; i++) {
			if (lat >= (*this)[i].lat()) { // the i-th point has a lat <= given one
				i1 = i ;

				i2 = i-1 ; // PRQA S 3084
				break ;
			}
		}
		if (i1 == 0) { // was the first so start with the next one
			i1++ ;
			i2++ ;
		}

	}
	else { // ascending order: lat[0] < lat[last]
		i1 = 0 ;
		i2 = 1 ;

		for (long i=pts-1; i >=0 ; i--) { // PRQA S 3000, 3084
			if (lat >= (*this)[i].lat()) {  // if the point has a lat less than the given one // PRQA S 3000, 4
				i1 = i ; // PRQA S 3000 3

				i2 = i+1 ; // PRQA S 3084
				break ;
			}
		}
		
		if (i1 == pts - 1) { // was the last, use the previous one // PRQA S 3084
			i1-- ;
			i2-- ;
		}
		
	}
}


void dbGeoPointSortedList::sort() const
{
	std::sort(_points.begin() , _points.end() );
}

size_t dbGeoPointSortedList::points() const { return _points.size() ; }

_ACS_END_NAMESPACE
