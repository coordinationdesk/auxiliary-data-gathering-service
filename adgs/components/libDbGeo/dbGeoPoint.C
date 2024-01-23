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
	Revision 5.6  2015/04/28 09:59:07  marpas
	fixing problem in bbox methods when timerange was not filled.
	
	Revision 5.5  2014/05/28 09:50:02  marpas
	fixed coordinate inversion regression, getSqlString out format
	
	Revision 5.4  2014/05/27 14:28:57  marpas
	regression in reading negative coordinates fixed
	generation of numeric strings fixed
	
	Revision 5.3  2014/05/27 10:40:45  marpas
	precision handling when reading data from postgres SQL result
	
	Revision 5.2  2014/05/09 11:39:54  marpas
	coding best practices applied
	qa & c++ warnings fixed
	last throwing macros for exceptions adopted
	
	Revision 5.1  2014/05/06 12:25:58  marpas
	precision in getting sql string managed
	dbGeoObject interface has now methods to set/get precision and a defaultPrecision static constant
	precision must be greater than 2, note that precision will affect lenght of string that will be used in queries, limiting the maximum number of points as the query string has limited size
	
	Revision 5.0  2013/07/11 16:07:12  marpas
	libException 5.x standards adopted
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.16  2013/05/17 16:32:25  marpas
	stream insertion operators added
	
	Revision 2.15  2013/05/14 18:00:35  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.14  2012/02/13 11:15:39  marpas
	refactoring in progress
	
	Revision 2.13  2011/09/29 14:48:10  marpas
	simplifying templates for codecheck
	
	Revision 2.12  2011/03/31 15:16:15  marpas
	Oracle11 has the lon, lat convention in the strings
	
	Revision 2.11  2010/05/12 13:22:22  marpas
	setDialectSRID and getDialectSRID static methods added to dbGeoObejct class
	getSQLString with dialect set to Oracle11 use the SRID passed to the above methods
	
	Revision 2.10  2010/05/04 15:59:20  marpas
	Postgres8 behaviour like Informix9
	
	Revision 2.9  2010/04/27 11:42:26  marpas
	still implementing ORACLE spatial/locator support.
	
	Revision 2.8  2010/04/26 14:32:26  marpas
	implementing Oracle spatial/locator syntax. Work in progress
	
	Revision 2.7  2010/04/26 11:40:20  marpas
	work in progress Oracle11 Spatial and Locator support implementation
	
	Revision 2.6  2010/04/09 17:23:21  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.5  2009/06/26 09:48:17  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.4  2009/06/24 09:27:53  marpas
	ntersections still in progress
	
	Revision 2.3  2009/06/23 13:29:43  marpas
	isInsideGeo and intersectGeo method can be invoked on CONST objects
	
	Revision 2.2  2009/06/05 09:31:18  crivig
	 isInsideGeo and intersectGeo methods added
	
	Revision 2.1  2007/06/09 18:45:54  marpas
	bbox method added
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.7  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.6  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.5  2003/09/03 13:57:43  paoscu
	Export methods are now const.
	
	Revision 1.4  2003/04/30 13:07:28  marpas
	using namespace std no longer in includes
	
	Revision 1.3  2003/03/17 16:42:48  marpas
	enumerant type added
	
	Revision 1.2  2002/11/29 17:48:46  danalt
	add exportXML, corrected some bugs
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/


#include <dbGeoPoint.h>
#include <dbGeoPnt.h>
#include <dbGeoBox.h>
#include <dbGeoRing.h>
#include <dbGeoCircle.h>
#include <dbGeoPolygon.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <XMLOstream.h>
#include <RegExp.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/  dbGeoPoint  _/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbGeoPoint)

dbGeoPoint::dbGeoPoint() : dbGeoObject() 
{
}

dbGeoPoint::dbGeoPoint(const dbGeoPnt &p) : dbGeoObject()
{
	dbGeoObject::append(p) ;
}



dbGeoPoint::~dbGeoPoint() throw() { }




dbGeoPoint::dbGeoPoint(const dbGeoPoint	&r) : dbGeoObject(r) 
{
}


dbGeoPoint &dbGeoPoint::operator=(const dbGeoPoint &p)
{
	if(this != &p) {
		dbGeoObject::operator=(p) ;
	}
    return *this ;
}

dbGeoObject* dbGeoPoint::clone() const 
{
	return new dbGeoPoint(*this) ;
}



void dbGeoPoint::point(const dbGeoPnt &pnt)
{
	if (0 != points()) { // almost one point
		(*this)[0] = pnt ;
    }
	else { // empty
		append(pnt) ;
    }
}



const dbGeoPnt &dbGeoPoint::point() const 
{
	return (*this)[0] ;
}

dbGeoPnt &dbGeoPoint::point() 
{
	while (points() < 1) { // the first point[0] could not have yet been inserted 
		append(dbGeoPnt(0,0)) ;
    }
	return (*this)[0] ;
}


double dbGeoPoint::lat() const 
{
	return (*this)[0].lat() ;
}

double dbGeoPoint::lon() const 
{
	return (*this)[0].lon() ;
}

void dbGeoPoint::lat(double v) 
{
	while (points() < 1) { // the first point[0] could not have yet been inserted 
		append(dbGeoPnt(0,0)) ;
    }
	(*this)[0].lat(v) ;
}

void dbGeoPoint::lon(double v) 
{
	while (points() < 1) { // the first point[0] could not have yet been inserted 
		append(dbGeoPnt(0,0)) ;
    }
	(*this)[0].lon(v) ;
}

dbGeoObject::Type dbGeoPoint::type() const 
{
	return Point ;
}

std::string dbGeoPoint::getWKTString(bool lonFirst) const {
	std::ostringstream ss;
	
	ss << std::setprecision(dbGeoObject::wktPrecision) << dbGeoObject::type2WKT(type()) << "(";
	
	if (lonFirst) {
		ss << lon() << " " << lat(); 
	} else {
		ss << lat() << " " << lon(); 	
	}
	
	ss << ")";
	
	return ss.str();
}

ostream &dbGeoPoint::getSqlString(ostream &os, const string &dialectName) const // PRQA S 4020 
{
	if (dialectName == "Informix9") {
	// 'GeoPoint((val,val),ANY,ANY)'
		os << "GeoPoint(" ;
		(*this)[0].getSqlString(os, dialectName, precision()) << ", "  ;
		_altitudeRange.getSqlString(os) << ", " ;
		_timeRange.getSqlString(os) << ")" ;
		return os ; // PRQA S 4028
	} 
    else if (dialectName == "Postgres8") {
	// 'POINT(0 0)'
		os << "POINT(" ;
		(*this)[0].getSqlString(os, dialectName, precision()) << ")" ;
		return os ; // PRQA S 4028
    }
	else if (dialectName == "Oracle11") {
	 // sdo_geometry (2001, null, null, sdo_elem_info_array (1,1,1), sdo_ordinate_array (10,5))
	 // MDSYS.SDO_GEOMETRY(2001,NULL,MDSYS.SDO_POINT_TYPE(12, 14, NULL),NULL,NULL)
		os << "MDSYS.SDO_GEOMETRY("
		   << "2001," << getDialectSRID(dialectName) << ",null,MDSYS.SDO_ELEM_INFO_ARRAY(1,1,1),"
		   << "MDSYS.SDO_ORDINATE_ARRAY("
		   << setprecision( precision() ) 
		   << (*this)[0].lon() << "," << (*this)[0].lat()
		   << "))" ;

		return os ; // PRQA S 4028
	}
    else {
	    ACS_THROW (ExecuteException("Dialect unimplemented: "+ dialectName)) ; // PRQA S 3081
    }
}


void dbGeoPoint::bbox(dbGeoBox &bb, double latmargin, double lonmargin) const 
{
	bb.p1(dbGeoPnt((*this)[0].lat()-latmargin, (*this)[0].lon()-lonmargin)) ;
	bb.p2(dbGeoPnt((*this)[0].lat()+latmargin, (*this)[0].lon()+lonmargin)) ;
    const TimeRange &tr = timeRange() ;
    if (tr.start().isFilled() && tr.stop().isFilled()) {
	    bb.timeRange(timeRange().wholeDay()) ;
    }
}



bool operator<(const dbGeoPoint &a, const dbGeoPoint &b) 
{	
	return a.timeRange().start() < b.timeRange().start() ; 
}

bool operator>=(const dbGeoPoint &a, const dbGeoPoint &b) { return not (a < b) ; } 


bool operator>(const dbGeoPoint &a, const dbGeoPoint &b)  
{	
	return a.timeRange().start() > b.timeRange().start() ; 
}

bool operator<=(const dbGeoPoint &a, const dbGeoPoint &b) { return not (a > b) ; } 



void dbGeoPoint::exportSpecific(rsResourceSet &s) const
{
	rsPushNSpace nsp(s,"dbGeoPoint") ;
	exportBase(s) ;
}

void dbGeoPoint::exportSpecificXML(XMLOstream &xst) const
{
	XMLOstream::Tag txml(xst,"dbGeoPoint") ;
	exportBaseXML(xst) ;
}

void dbGeoPoint::importSpecific(rsResourceSet &s)
{
	rsPushNSpace nsp(s,"dbGeoPoint") ;
	importBase(s) ;
}

bool dbGeoPoint::intersectGeo(const dbGeoObject & geo) const // PRQA S 4020
{
	ACS_THROW(dbGeoObject::ExecuteException ("method not implemented !  ")) ; // PRQA S 3081		
	// this method checks if the GeoBox  is geometrically inside the given dbGeoObject argument
	// timerange not considered
	bool intersect = false;

	switch(geo.type())
	{
	case dbGeoObject::Point:
		{
			const dbGeoPoint  * p = dynamic_cast<const dbGeoPoint *> (&geo); // PRQA S 3081
			return p->point() == point() ; // same coordinates ?
		}

	case dbGeoObject::Box:
		{
			// create pntList
			const dbGeoBox  * box = dynamic_cast<const dbGeoBox *> (&geo); // PRQA S 3081
			return box->intersectGeo(*this);
		}
	case dbGeoObject::Lineseg:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for LineSeg shape!  ")) ; // PRQA S 3081	
		}
	case dbGeoObject::Circle:
		{
			const dbGeoCircle * circle = dynamic_cast<const dbGeoCircle *> (&geo); // PRQA S 3081
			// is less than the radius ->intersect
			return (point().distance(circle->center()) <= circle->radius()) ;
		}
	case dbGeoObject::String:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for String shape!  ")) ; // PRQA S 3081	

		}
	case dbGeoObject::Ring:
		{

			const dbGeoRing * ring = dynamic_cast<const dbGeoRing *> (&geo); // PRQA S 3081
			const dbGeoPntList PntList = ring->ring(0);	
			return PntList.contains(point()) ;
		}
	case dbGeoObject::Polygon:
		{
			const dbGeoPolygon * pol = dynamic_cast<const dbGeoPolygon *> (&geo); // PRQA S 3081
			const dbGeoPntList PntList = pol->ring(0);	
			return PntList.contains(point()) ;
		}
	case dbGeoObject::Ellipse:
		{
			// not implemented
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for Ellipse shape!  ")) ; // PRQA S 3081
		}

	default:
		{
			// unknown
			ACS_THROW(dbGeoObject::ExecuteException("type not recognized  ")) ; // PRQA S 3081		
		}
	}
	return intersect;
}

bool dbGeoPoint::isInsideGeo(const dbGeoObject &) const
{
	ACS_THROW(dbGeoObject::ExecuteException ("method not implemented !  ")) ; // PRQA S 3081
}

void dbGeoPoint::parseOracle(const string &is) 
{
/* MDSYS.SDO_GEOMETRY(2001,null,MDSYS.SDO_POINT_TYPE(15,12,null),null,null) */
// or sdo_geometry (2001, null, null, sdo_elem_info_array (1,1,1), sdo_ordinate_array (10,5))

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "is: \"" << is << "\"") ;
	
	RegExp recoord(".*SDO_POINT_TYPE[ \t]*[(]") ;
	regoff_t coordbeg ; // PRQA S 4101 2
    regoff_t coordend ;
	if (!recoord.match(is,coordbeg,coordend)) {
		// try second alternative:
		recoord.setReg(".*SDO_ORDINATE_ARRAY[ \t]*[(]") ;
		if (!recoord.match(is,coordbeg,coordend)) {
			ACS_THROW(UnrecognizedTypeException("Unrecognized \"" + ((is.length() > 64) ? is.substr(0,64) : is) + " ...\"")) ;	// PRQA S 3081, 3380
		}
	}
	istringstream coords(is.substr(coordend)) ; // PRQA S 3000
	coords.exceptions(ios_base::failbit) ;

	dbGeoPnt pnt ;
	dbGeoPnt::parseOracle(pnt, coords) ;
	point(pnt) ;
	dbGeoAltitudeRange ar ;
	dbGeoTimeRange tr ;
	altitudeRange(ar) ;
	timeRange(tr) ;
}


istream &
operator >> (istream &is, dbGeoPoint& p) // PRQA S 4020
{
/* GeoPoint((lat, long), altrange, timerange)) */
/* POINT(lat, long) */
	if (!is.good()) { return is ; } // PRQA S 4028

	std::string oname ;
    bool informix = true ;
	if (!dbGeoObject::extractObjType(is,oname)) {
		is.setstate(std::ios::failbit) ;
		return is ;	 // PRQA S 4028
	}
    if (oname == "GeoPoint(") {
        informix = true ;
    }
    else if (oname == "POINT(") {
        informix = false ;
    }
    else {
		is.setstate(std::ios::failbit) ;
		return is ;	 // PRQA S 4028
    }

	dbGeoPnt pnt ;

    if (informix) {
	    dbGeoAltitudeRange ar ;
	    dbGeoTimeRange tr ;
	    char comma ; // PRQA S 4101 2 
        char ell ;
	    is >> pnt ;
        is >> comma >> ar >> comma >> tr >> ell ;
    
	    if (!is.fail()) {
		    p.point(pnt) ; 
	        p.altitudeRange(ar) ;
	        p.timeRange(tr) ;
	    }
    }
    else {
        int precision = 0 ;
        dbGeoPnt::parsePostgres(pnt, is, precision) ; 
        char ell ; // PRQA S 4101 
        is >> ell ;
        if (!is.fail()) {
            p.point(pnt) ; 
            p.precision(precision) ;
        }
    }
	return is ; // PRQA S 4028
}


std::ostream & operator << (std::ostream &os, const dbGeoPoint& p) 
{
   return p.getSqlString(os, "Postgres8") ;
}

exostream & operator << (exostream &os, const dbGeoPoint& p) // PRQA S 4222
{
    ostringstream s ;
    p.getSqlString(s, "Postgres8") ;
    return os << s.str() ;
}

_ACS_END_NAMESPACE


