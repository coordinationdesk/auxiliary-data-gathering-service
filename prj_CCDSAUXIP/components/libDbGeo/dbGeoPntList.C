// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
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
	Revision 5.6  2015/05/15 12:38:19  marfav
	autoIntersect check can be disabled using a define
	
	Revision 5.5  2014/05/28 09:50:02  marpas
	fixed coordinate inversion regression, getSqlString out format
	
	Revision 5.4  2014/05/27 10:40:45  marpas
	precision handling when reading data from postgres SQL result
	
	Revision 5.3  2014/05/09 11:39:54  marpas
	coding best practices applied
	qa & c++ warnings fixed
	last throwing macros for exceptions adopted
	
	Revision 5.2  2014/05/06 12:25:58  marpas
	precision in getting sql string managed
	dbGeoObject interface has now methods to set/get precision and a defaultPrecision static constant
	precision must be greater than 2, note that precision will affect lenght of string that will be used in queries, limiting the maximum number of points as the query string has limited size
	
	Revision 5.1  2013/09/02 11:49:30  marpas
	wrong longitude normalization caused wrong response on dbGeoPnt intersect - fixed
	Actually the function sums to lognitude 360 if and only if longitude is <= -180 degrees
	
	Revision 5.0  2013/07/11 16:07:12  marpas
	libException 5.x standards adopted
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.14  2013/05/14 18:00:35  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.13  2012/02/13 11:15:39  marpas
	refactoring in progress
	
	Revision 2.12  2011/09/29 14:48:10  marpas
	simplifying templates for codecheck
	
	Revision 2.11  2010/09/20 15:18:55  marpas
	autointersection exception notifies interecting segments along with their coordinates
	
	Revision 2.10  2010/05/19 16:06:06  marpas
	Oracle spatial/locator parse completed
	
	Revision 2.9  2010/05/04 15:59:20  marpas
	Postgres8 behaviour like Informix9
	
	Revision 2.8  2010/04/30 16:23:05  marpas
	ORACLE spatial/locator support in progress ..
	
	Revision 2.7  2010/04/26 11:40:20  marpas
	work in progress Oracle11 Spatial and Locator support implementation
	
	Revision 2.6  2010/04/09 17:23:21  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.5  2009/06/26 09:48:17  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.4  2009/06/23 16:26:40  marpas
	intersect in dbGeoBox fixed
	
	Revision 2.3  2007/12/03 16:43:37  marpas
	polygon comparison fixed
	
	Revision 2.2  2007/06/09 18:45:54  marpas
	bbox method added
	
	Revision 2.1  2006/05/12 16:32:07  marpas
	Bug fixed when north pole is inside the polygon
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.10  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.9  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.8  2004/06/16 09:44:09  marpas
	measurement methods added
	
	Revision 1.7  2004/05/27 12:55:13  marpas
	some cosmetic changes,
	new method unionWith in dbGeoPntList
	
	Revision 1.6  2003/09/03 13:57:43  paoscu
	Export methods are now const.
	
	Revision 1.5  2003/04/30 13:07:28  marpas
	using namespace std no longer in includes
	
	Revision 1.4  2003/04/15 18:35:04  marpas
	GCC3.2
	
	Revision 1.3  2002/12/16 11:13:50  paoscu
	Export and import methods modified.
	
	Revision 1.2  2002/11/29 17:48:46  danalt
	add exportXML, corrected some bugs
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/


#include <dbGeoPntList.h>
#include <dbGeoPnt.h>
#include <dbGeoObject.h>
#include <dbGeoBox.h>

#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <XMLOstream.h>
#include <Filterables.h>
#include <algorithm>
#include <sstream>

#include <cmath>
#include <values.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

// 
// 
// 		dbGeoPntList
// 		
// 		

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbGeoPntList)


dbGeoPntList::dbGeoPntList() :
	_isCCW(Undefined),
    _points()
{
}

dbGeoPntList::~dbGeoPntList() throw() { }

dbGeoPntList::dbGeoPntList(const dbGeoPntList &list) :
    _isCCW(list._isCCW),
    _points(list._points)

{
	// empty
}

dbGeoPntList &dbGeoPntList::operator=(const dbGeoPntList &list)
{
	if(this != &list) {
		_points = list._points ; // copy the points 

		_isCCW = list._isCCW ; // and the sense
	}
    return *this ;
}


bool operator==(const dbGeoPntList &a, const dbGeoPntList &b) // PRQA S 4020
{
	if(a.isCCW() != b.isCCW()) { // same sense ?
		return false;
    }
	if(a.getPoints() != b.getPoints()) { // same points ?
		return false;
    }
	return true;
}

bool operator!=(const dbGeoPntList &a, const dbGeoPntList &b)
{
	
	return not (a == b);
}


dbGeoPntList &dbGeoPntList::operator+=(const dbGeoPntList &list)
{
	_points.insert(_points.end(), list._points.begin(), list._points.end());
	_isCCW = Undefined ;
    return *this ;
}

dbGeoPntList &dbGeoPntList::operator+=(const dbGeoPnt &pnt)
{
    append(pnt) ;
	_isCCW = Undefined ;
	return *this ;
}


const dbGeoPnt &dbGeoPntList::operator[](size_t i) const 
{
	return _points[i] ;
}


dbGeoPnt &dbGeoPntList::operator[](size_t i) 
{
	return _points[i] ;
}



size_t dbGeoPntList::points() const 
{ 
	return _points.size() ; 
}


void dbGeoPntList::append(const dbGeoPnt &point)
{	
	_points.push_back(point) ;
	_isCCW = Undefined ;
}

void dbGeoPntList::append(double lat, double lon)
{	
	append(dbGeoPnt(lat,lon)) ;
}


void dbGeoPntList::erase(size_t i) 
{
	_points.erase(_points.begin()+i) ; // PRQA S 3000
	_isCCW = Undefined ;
}
 
void dbGeoPntList::erase(const dbGeoPnt &p)
{
	vector<dbGeoPnt>::iterator index = find(_points.begin(), _points.end(),p) ;
	if (index != _points.end()) {
		_points.erase(index) ;
    }
}

ostream &dbGeoPntList::getSqlString(ostream &os, const string &dialectName, int precision) const 
{
    return getSqlString(os, dialectName, false, precision) ;
}

ostream &dbGeoPntList::getSqlString(ostream &os, const string &dialectName, bool closed, int precision) const // PRQA S 4020
{
	if (dialectName == "Informix9" || dialectName == "Postgres8") {
	// ((val,val), .... )
		os << '(' ;
		size_t pts =  points() ;
        size_t sizep = pts ;
        if (closed) { ++pts ; }
		for(size_t i = 0 ; i < pts ; i++) {
			(*this)[i%sizep].getSqlString(os, dialectName, precision) << ((i != pts-1) ? ',' : ')') ; // check last  // PRQA S 3084, 3380
        }
		return os ;	// PRQA S 4028
	}
	if (dialectName == "Oracle11") {
		size_t pts =  points() ;
		for(size_t i = 0 ; i < pts ; i++) {
			if (0 != i) {
				os << ", " ;
            }
			(*this)[i].getSqlString(os, dialectName, precision) ; 
		}
		return os ; // PRQA S 4028
	}
	
	ACS_THROW (dbGeoObject::ExecuteException("Dialect unimplemented: "+ dialectName)) ; // PRQA S 3081

}


void dbGeoPntList::bbox(dbGeoBox &bb, double latmargin, double lonmargin) const 
{
	double minlat = MAXFLOAT ;
	double minlon = MAXFLOAT ; 
	double maxlat = -MAXFLOAT ; 
	double maxlon = -MAXFLOAT ;
	for (size_t i=0; i <  points(); i++) { // enlarge the bbox
		minlat = min(minlat,(*this)[i].lat()) ;
		minlon = min(minlon,(*this)[i].lon()) ;
		maxlat = max(maxlat,(*this)[i].lat()) ;
		maxlon = max(maxlon,(*this)[i].lon()) ;
	}
	
	// build the bbox
	bb.p1(dbGeoPnt(minlat-latmargin,minlon-lonmargin)) ;
	bb.p2(dbGeoPnt(maxlat+latmargin,maxlon+lonmargin)) ;

}

dbGeoPntList &dbGeoPntList::ccw()
{
	if (_isCCW != CounterClockWise) {
		switch(_isCCW) {
		case Undefined: // may falltrough // PRQA S 4011
			{
				RotationSense rs = computeRS(*this) ;
				if (rs == CounterClockWise) {
					break ; // ok it was undefined, but is in counterclockwise sense
                }
				else if (rs == Undefined) {
					break ; // nothing to do, it is still undefined (points aligned)
                }
                else { /* nothing */ }
			}
			// if here, it is in clockwise rotation sense,
			// so no break required , just a revert will fit
		case ClockWise:
            {
			    revert() ; // just revert
			    break ;
            }
		default:
			{
                break ;
            }
		}
	}
	
	return *this ;
}


dbGeoPntList &dbGeoPntList::cw()
{
	if (_isCCW != ClockWise) {
		switch(_isCCW) {
		case Undefined: // may fallthrough // PRQA S 4011
			{
				RotationSense rs = computeRS(*this) ;
				if (rs == ClockWise) {
					break ; // ok it was undefined, but is in clockwise sense
                }
				else if (rs == Undefined) {
					break ; // nothing to do, it is still undefined (points aligned)
                }
                else { /* nothing */ }
				// if here, it is in counterclockwise rotation sense,
				// so no break required , just a revert will fit
			}
		case CounterClockWise:
            {
			    revert() ; // just revert
			    break ;
            }
		default:
            {
			    break ;
            }
		}
	}
	
	return *this ;
}



dbGeoPntList dbGeoPntList::ccw() const
{
	dbGeoPntList npl(*this) ;
	return npl.ccw() ;
}


dbGeoPntList dbGeoPntList::cw() const
{
	dbGeoPntList npl(*this) ;
	return npl.cw() ;
}



dbGeoPntList::RotationSense dbGeoPntList::computeRS(dbGeoPntList &pl) // PRQA S 4020
{
	if (pl.points() < 3) { // PRQA S 4400
		return pl._isCCW = Undefined ;
    }
	
	
	// makes a triangle fan and compute the sum of the signed areas
	size_t pts = pl.points() ;
 	const dbGeoPnt &p0 = pl[0] ;
	double area = 0 ;
	for (size_t i=1; i < pts-1; i++) { // PRQA S 3084 2
		area+=p0.area2(pl[i],pl[i+1]) ;
    }
		
	if (area < 0) { // clocwise
		return pl._isCCW = ClockWise ;
    }
	else if (area > 0) { // counterclockwise
		return pl._isCCW = CounterClockWise ;
    }
    else {
	    return pl._isCCW = Undefined ;
    }
}


void dbGeoPntList::revert()
{
	size_t pts = points() ;
	// reverts the points
	// p[0] <--> p[n-1] ...  p[i] <--> p[n-i-1]
	for (size_t i=0; i < pts/2; i++) { // PRQA S 3084, 4400
		dbGeoPnt tmp = (*this)[i] ;
		(*this)[i] = (*this)[pts-i-1] ; // PRQA S 3084, 4400 2
		(*this)[pts-i-1] = tmp ;
	}
	
	
	// leave the Undefined flag
	switch(_isCCW) {
	case ClockWise:
        {
		    _isCCW = CounterClockWise ;
		    break ;
        }
	case CounterClockWise:
        {
		    _isCCW = ClockWise ;
		    break ;
        }
	default:
        {
		    break ;
        }
	}
}

bool dbGeoPntList::checkIntersection(size_t s1, size_t s2) const // PRQA S 4020
{
	size_t np = points() ;
	
	dbGeoPnt::Intersection inter = dbGeoPnt::intersect((*this)[s1%np], // PRQA S 3084 4
													   (*this)[(s1+1)%np], 
													   (*this)[s2%np], 
													   (*this)[(s2+1)%np]) ;
	if (inter == dbGeoPnt::IntersectBoth) {
		ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
			// debug to know when and where bot the pntlists intersect each other
			dbGeoPnt p ;
			dbGeoPnt::Intersection inter1 = dbGeoPnt::intersect((*this)[s1%np], // PRQA S 3084 4
															    (*this)[(s1+1)%np], 
															    (*this)[s2%np], 
															    (*this)[(s2+1)%np],
															    &p) ;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "intersection found between segments: " // PRQA S 3081, 3084 11
				   << s1 << " (" 
				   		<< (*this)[s1%np].lat() << "," << (*this)[s1%np].lon() 
						<< " - "
				   		<< (*this)[(s1+1)%np].lat() << "," << (*this)[(s1+1)%np].lon() 
				   << ") and "
				   << s2  << " (" 
				   		<< (*this)[s2%np].lat() << "," << (*this)[s2%np].lon() 
						<< " - "
				   		<< (*this)[(s2+1)%np].lat() << "," << (*this)[(s2+1)%np].lon() 
				   << " at " << p.lat() << "," << p.lon() << " (" << int(inter1) << ")") ;
			// end of debug
		ACS_CLASS_END_DEBUG
		return true ; // anyway if the intersection is found on both they DO intersect
	}
	return false ; // they DON'T
}

bool dbGeoPntList::intersectCircle(double radius,const dbGeoPnt & center, bool closed) const // PRQA S 4020
{
	// checks if the given circle intersects
	// any segments of the pntList-> to be used after points inclusion check
	// has been performed!
	size_t nsegs = points() ;
	if (closed && (nsegs < 2)) { // no segments in this one // PRQA S 4400
		return false ;
    }
	// loop on pntList segments	
	dbGeoPnt perp;
    int corr = (closed ? 0 : 1) ; // PRQA S 3384
	for (size_t s=0; s < nsegs-corr; s++) { // PRQA S 3000, 3084
		// for each segment calculate center projection on segment
		center.perpendicularPos((*this)[s%nsegs], (*this)[(s+1)%nsegs], perp); // PRQA S 3084
		
		if ((center.distance(perp) < radius) && dbGeoPnt::belongsToSegment((*this)[s%nsegs], (*this)[(s+1)%nsegs], perp)) { // PRQA S 3084
			return true;
        }
	}
	return false;		
}


bool dbGeoPntList::intersect(const dbGeoPnt &p1, const dbGeoPnt &p2, bool closed) const  // PRQA S 4020
{
	size_t nsegs = points() ;
	if (closed && (nsegs < 2)) { // no segments in this one // PRQA S 4400
		return false ;
    }
	int corr = (closed ? 0 : 1 ) ; // PRQA S 3384
	for (size_t s=0; s < nsegs-corr; s++) { // PRQA S 3000, 3084
		dbGeoPnt::Intersection inter = dbGeoPnt::intersect((*this)[s%nsegs], // PRQA S 3084 4
							   (*this)[(s+1)%nsegs], 
							   p1, 
							   p2) ;
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, '[' << s%nsegs << "]" << (*this)[s%nsegs] 
                                        << ", [" << (s+1)%nsegs << "]" << (*this)[(s+1)%nsegs] 
                                        <<  " -> " << inter) ;
		if (inter == dbGeoPnt::IntersectBoth) {
			return true ;
        }
	}
	
	return false ;
}


bool dbGeoPntList::intersect(const dbGeoPntList &ol, bool iAmClosed, bool oLclosed) const  // PRQA S 4020
{
	if (oLclosed && (ol.points() < 2)) { // no segments in the other polyline // PRQA S 4400
		return false ;
    }
		
	size_t nsegs = points() ;
	if (iAmClosed && (nsegs < 2)) { // no segments in this one // PRQA S 4400
		return false ;
    }
	int corr = iAmClosed ? 0 : 1 ; // PRQA S 3384
	for (size_t s=0; s < nsegs - corr; s++) { // PRQA S 3000, 3084
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, '[' << s%nsegs << "]" << (*this)[s%nsegs] // PRQA S 3084
                                        << ", [" << (s+1)%nsegs << "]" << (*this)[(s+1)%nsegs]) ;
		if (ol.intersect((*this)[s%nsegs], (*this)[(s+1)%nsegs], oLclosed)) { // PRQA S 3084
			return true ;
        }
	}
	
	return false ;
}

bool dbGeoPntList::inside(const dbGeoPntList &ol) const // PRQA S 4020
{
	if (ol.points() < 2) { // no segments in the other polyline // PRQA S 4400
		return false ;
    }
	
	bool in = true ;
    size_t pts = points();
	for (size_t p=0; p < pts; ++p) {
		if (!ol.contains((*this)[p])) {
			// at least one point outside
			in = false ;
			break ;
		}
	}
	
	return in ;
}

bool dbGeoPntList::containsAtLeastOne(const dbGeoPntList &ol) const // PRQA S 4020
{
	if (ol.points() < 2) { // no segments in the other polyline // PRQA S 4400
		return false ;
    }
	
    size_t pts = points();
	for (size_t p=0; p < pts; ++p) {
		if (ol.contains((*this)[p])) {
			return true ;
		}
	}
	
	return false ;
}

size_t dbGeoPntList::crossing180() const // PRQA S 4020
{
	size_t nsegs = points() ;
	if (nsegs < 2) { // no segments in this one // PRQA S 4400
		return 0 ;
    }
	
	if (nsegs == 2) { // just one segment // PRQA S 4400
		return (*this)[0].cross180((*this)[1]) ? 1 : 0 ; // PRQA S 3000, 3382
    }
	
	size_t nint = 0 ;
	for (size_t s=0; s < nsegs; s++) {
		nint = (*this)[s%nsegs].cross180((*this)[(s+1)%nsegs]) ? 1 : 0 ; // PRQA S 3000, 3084, 3380
    }
	return nint ;
}

bool dbGeoPntList::autoIntersect(bool closed, size_t *seg1, size_t *seg2) const // PRQA S 4020
{
#define DISABLE_GEO_AUTOINTERSECT_CHECK
#ifndef DISABLE_GEO_AUTOINTERSECT_CHECK
	size_t nsegs = points() - (closed ? 1 : 0) ; // PRQA S 3000, 3084, 3380
	if (nsegs < 3) { // PRQA S 4400
		return false ;
    }
	for (size_t s=0; s < nsegs; ++s) {
		for (size_t s1 = s+2; s1 < nsegs; ++s1) { // PRQA S 3084 2
			if ((0 == s) && (s1 == nsegs-1)) { // do not check first and last together 
				continue ;
            }
			if (checkIntersection(s,s1)) {
				if (seg1) { *seg1 = s ; }
				if (seg2) { *seg2 = s1 ; }
				return true ;
			}
		}
	}
	
#else //DISABLE_GEO_AUTOINTERSECT_CHECK
#warning dbGeoPoint autointersect check is disabled
#endif //DISABLE_GEO_AUTOINTERSECT_CHECK
	return false ;
}

void dbGeoPntList::exportObj(rsResourceSet &s) const
{
	rsPushNSpace nsp(s,"dbGeoPntList") ;
	
	exportPoints(s) ;
}

void dbGeoPntList::importObj(rsResourceSet &s)
{
	rsPushNSpace nsp(s,"dbGeoPntList") ;
	importPoints(s) ;
}

void dbGeoPntList::exportXML(XMLOstream &xst) const
{
	XMLOstream::Tag txml(xst,"dbGeoPntList") ;
	
	exportPointsXML(xst) ;
}

void dbGeoPntList::exportPoints(rsResourceSet &s) const
{
	s.setValue("Coordinates",points()) ; // the number of points
	{
		rsPushNSpace nsp(s,"_points") ;
        size_t pts = points() ;
		for (size_t i=0; i < pts; ++i) // for each point 
		{
			ostringstream os;
			os << "dbGeoPnt[" << i << "]";
			rsPushNSpace nsp2(s,os.str()) ;
			(*this)[i].exportObj(s) ; // export it 
		}
	}
	
	int ccw = _isCCW ;
	s.setValue("CCW",ccw) ; // and the rotation sense too
}

void dbGeoPntList::importPoints(rsResourceSet &s)
{
	size_t pts ; // PRQA S 4101
	s.getValue("Coordinates",pts) ; // how many points ?
	{
		rsPushNSpace nsp(s,"_points") ;
		_isCCW = Undefined ; _points.clear() ; // reset 
		for (size_t i=0; i < pts; ++i) { // for each point 
			append(dbGeoPnt(0,0)) ;
			ostringstream os;
			os << "dbGeoPnt[" << i << "]";
			rsPushNSpace nsp2(s,os.str()) ;
			(*this)[i].importObj(s) ; // import it 
		}
	}
	int ccw ; // PRQA S 4101
	s.getValue("CCW",ccw) ;
	_isCCW = RotationSense(ccw) ; // the rotation sense too // PRQA S 3013, 3081
}

void dbGeoPntList::exportPointsXML(XMLOstream &xst) const
{
	ostringstream os ;
	os << points();
	{
		// coord tag
		XMLOstream::Tag txml(xst, "Coordinates", os.str()) ;
	}
	{
		XMLOstream::Tag txml(xst, "_points", "", "count=\""+os.str()+"\"") ;
		size_t pts = points() ;
        for (size_t i=0; i < pts; ++i)
		{
			// a tag for each point 
			XMLOstream::Tag txml1(xst, "dbGeoPnt") ;
			(*this)[i].exportXML(xst) ;
		}
	}
	
	int ccw = _isCCW ;
	ostringstream os2 ;
	os2 << ccw;
	{
		// the sense tag
		XMLOstream::Tag txml(xst, "CCW", os2.str()) ;
	}
}

bool dbGeoPntList::contains(const dbGeoPnt &p) const // PRQA S 4020
{
	size_t nsegs ; // PRQA S 4101
	if ((nsegs = points()) < 3) { // PRQA S 4400
		return false ;
    }

	double angle = 0 ;
	
	// makes a triangle fan from the given point and 
	// compute the sum of the signed areas
	for (size_t i=0; i < nsegs; ++i) {
		const dbGeoPnt &tmp = (*this)[i%nsegs]; // PRQA S 3084 2
		const dbGeoPnt &tmp1 = (*this)[(i+1)%nsegs];
		double dist = p.distance(tmp)*p.distance(tmp1);
		angle += atan2(p.area2(tmp, tmp1)/dist, p.scalar(tmp, tmp1)/dist);
	}	

	if (fabs(angle)>0.0000001) { // PRQA S 4400
		return true ;
    }

	return false ;
}



double dbGeoPntList::lengthKm(bool closed) const // PRQA S 4020
{
	if (points() < 2) { // PRQA S 4400
		return 0 ;
    }
	size_t nsegs = points()-1 ; // PRQA S 3084
	double len = 0 ;
	for (size_t i=0; i < nsegs; ++i) {
		len += dbGeoObject::distanceKm((*this)[i],(*this)[i+1]) ; // PRQA S 3084
	}
	if (closed) {
		len += dbGeoObject::distanceKm((*this)[0],(*this)[nsegs]) ;
	}
	return len ;
}



bool dbGeoPntList::intersect(const dbGeoPnt &p1, const dbGeoPnt &p2, const dbGeoPntList &pl, dbGeoPnt &intersection, size_t &seg, bool closed) // PRQA S 4020
{
	size_t nsegs = pl.points() - (closed ? 0 : 1) ; // PRQA S 3000, 3084, 3380
	for(size_t c2=0; c2 < nsegs; c2++) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "checking " << (c2+seg)%nsegs << "(" << &pl << ")") ; // PRQA S 3084
	
		if ((dbGeoPnt::intersect(p1, p2, 
								 pl[(c2+seg)%nsegs], pl[(c2+seg+1)%nsegs], &intersection) == dbGeoPnt::IntersectBoth) && (intersection.distance(p1) > 1e-14) ) { // PRQA S 3084, 4400 
			// here if the segment pl[s2%nsegs2], pl[(s2+1)%nsegs2] intersects the current in 1
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "FOUND inters at " << intersection.lat() << " " << intersection.lon()  // PRQA S 3084
					   << " in " << (c2+seg) %nsegs << "(" <<  &pl << ")") ;
			seg = (c2+seg)%nsegs ; // PRQA S 3084
			return true ;
		}
	}
	return false ;
}

// runs on p1 from seg seg.
// stop condition is when the second point of the current segment is the same of the stop one, hence we reached 
// the very beginning of the resulting shape.
bool dbGeoPntList::followUntilIntersection( // PRQA S 4020
	dbGeoPnt &pin, 
	const dbGeoPntList &pl1, 
	const dbGeoPntList &pl2, 
	size_t &seg1, 
	size_t &seg2, 
	vector<dbGeoPnt> &points, 
	const dbGeoPnt &stop, 
	bool closed
) 
{
	size_t nsegs1 = pl1.points() ;
	for (size_t c1=0; c1 < nsegs1;c1++) {
	
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "checking " << (c1+seg1)%nsegs1 << "(" << &pl1 << ")") ; // PRQA S 3084
		dbGeoPnt p1 = pl1[(c1+seg1)%nsegs1] ; // PRQA S 3084 2
		dbGeoPnt p2 = pl1[(c1+seg1+1)%nsegs1] ;
		if (0 == c1) { // first 
			p1 = pin ;
		}
		
		points.push_back(p1) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Pushing " << p1.lat() << " " << p1.lon()) ; 
		if (intersect(p1, p2, pl2, pin, seg2, closed)) {
			return true ;
		}
		
		if (&stop == &pl1[(c1+seg1+1)%nsegs1]) { // PRQA S 3084
			// here the stop condition is reached, to signal this outside the function, seg1 is changed to assume the 
			// value that indexes exactly the stop point in the current polyline.
			seg1 = (c1+seg1+1)%nsegs1 ; // PRQA S 3084
			return false ;
		}
	}
	
	return false ;
}



void dbGeoPntList::unionWith(dbGeoPntList &pl) // PRQA S 4020
{
	// check the same list was not passed twice
	if (*this == pl) {
		return ;
    }

	// check against the number of points that shall reach the 
	// minimum required to close a polyline for both the lists
	if ((points() < 3) || (pl.points() < 3)) { // PRQA S 4400
		ACS_THROW(dbGeoObject::ExecuteException("One list cannot be closed: points number < 3")) ; // PRQA S 3081
	}
	
	// check against no intersection exists between the two lists
	if (!intersect(pl, true, true)) {
		ACS_THROW(dbGeoObject::ExecuteException("The lists do not intersects")) ; // PRQA S 3081
	}
	
	// set both to ccw rotation sense
	ccw() ;
	pl.ccw() ;
	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
		for (size_t i=0; i < points(); i++) {
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "1 [" << i << "] " << (*this)[i].lat() << " " << (*this)[i].lon());

		}
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"") ;
		for (size_t i=0; i < pl.points(); i++) {
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "2 [" << i << "] " << pl[i].lat() << " " << pl[i].lon())  ;

		}
	ACS_CLASS_END_DEBUG
	

	// check the second one is completely inside the first.
	// in such case no change, the first list is already the union
	if (pl.inside(*this)) {
		return ;
    }
	
	// check the first one is completely inside the second.
	// the list is then copied from the second
	if (inside(pl)) {
		*this = pl ;
		return ;
	}
	
	// find the first point not inside the other polygon
	size_t beginP1 = -1 ; // PRQA S 2890
	for (size_t po=0; po < points(); po++) {
		if (!pl.contains((*this)[po])) {
			beginP1 = po ;
			break ;
		}
	}

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "First point outside is # " << beginP1) ;
	vector<dbGeoPnt> points ;
	
	bool done = false ;
	dbGeoPntList *p1 = this ;
	dbGeoPntList *p2 = &pl ;
	size_t seg1 = 0 ;
	size_t seg2 = 0 ;
	dbGeoPnt ip = (*this)[beginP1];
	do {
		if (followUntilIntersection(ip, *p1, *p2, seg1, seg2, points, (*this)[beginP1], true)) {
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Swapping \n") ;
			dbGeoPntList *t = p1 ;
			p1 = p2 ;
			p2 = t ;
			size_t t1 = seg1 ;
			seg1 = seg2 ;
			seg2 = t1 ;
		}
		else {
			// note the stop condition is forced
			if (&(*this)[beginP1] == &(*p1)[seg1]) {
				done = true ;
            }
			else {
				points.push_back((*p1)[(seg1+1)%p1->points()]) ; // PRQA S 3084
				seg1++ ;
			}
		}
	} while (!done) ;

	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
        size_t pts = points.size() ;
		for (size_t i=0; i < pts; ++i) {
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "[" << i << "] " << points[i].lat() << " " << points[i].lon()) ;

		}
	ACS_CLASS_END_DEBUG
}

dbGeoPntList::RotationSense dbGeoPntList::isCCW() const { return _isCCW ; } // PRQA S 4120


void dbGeoPntList::parseOracle(const string &coords) 
{
	istringstream is(coords) ;
	dbGeoPntList pl ;
	char comma ; // PRQA S 4101
	for (int i=0; !is.fail();i++) { // PRQA S 4238
		dbGeoPnt p ;
		dbGeoPnt::parseOracle(p, is) ;
		ACS_COND_THROW (is.fail(), exIOException("Unrecognized pnt")) ; // PRQA S 3081
		pl.append(p) ;
		is >> comma ;
		if (comma == ',') { continue ; }
		if (comma == ')') { break ; }
	}
	*this = pl ;
}

std::istream & dbGeoPntList::parsePostgres(dbGeoPntList &pl, std::istream &is, int &precision) 
{
	dbGeoPntList plint ;
	char comma ; // PRQA S 4101
    precision = 0 ;
	for (int i=0; !is.fail();i++) { // PRQA S 4238
		dbGeoPnt p ;
        int precp = 0 ;
		dbGeoPnt::parsePostgres(p, is, precp) ;
        precision = max(precp, precision) ;
		ACS_COND_THROW (is.fail(), exIOException("Unrecognized pnt")) ; // PRQA S 3081
		plint.append(p) ;
		is >> comma ;
		if (comma == ',') { continue ; }
		if (comma == ')') { break ; }
	}
    pl = plint ;
    return is ; // PRQA S 4028
}



std::istream &
operator >> (std::istream &is, dbGeoPntList& l) // PRQA S 4020
{
/* ( (lat,lon), ... ) */
	if (!is.good()) { return is ; } // PRQA S 4028
	char ell ; // PRQA S 4101 2 
    char comma ;
	is >> ell ;
	dbGeoPntList pl ;
	for (int i=0; !is.fail();i++) { // PRQA S 4238
		dbGeoPnt p ;
		is >> p ;
		if (is.fail()) { return is ; } // PRQA S 4028
		pl.append(p) ;
		is >> comma ;
		if (comma == ',') { continue ; }
		if (comma == ')') { break ; }
	}

	if (!is.fail()) {
		l = pl ;
	}
	return is ; // PRQA S 4028
}



_ACS_END_NAMESPACE

