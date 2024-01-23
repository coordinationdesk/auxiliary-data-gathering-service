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
	Revision 5.5  2015/04/28 09:59:07  marpas
	fixing problem in bbox methods when timerange was not filled.
	
	Revision 5.4  2014/05/28 09:50:02  marpas
	fixed coordinate inversion regression, getSqlString out format
	
	Revision 5.3  2014/05/27 14:28:57  marpas
	regression in reading negative coordinates fixed
	generation of numeric strings fixed
	
	Revision 5.2  2014/05/09 11:39:55  marpas
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
	
	Revision 2.16  2013/05/14 18:00:35  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.15  2012/12/14 11:13:41  marpas
	coding best practices
	qa rules
	test improeved
	test coverage
	
	Revision 2.14  2012/02/13 11:15:40  marpas
	refactoring in progress
	
	Revision 2.13  2011/09/29 14:48:10  marpas
	simplifying templates for codecheck
	
	Revision 2.12  2010/05/19 16:06:07  marpas
	Oracle spatial/locator parse completed
	
	Revision 2.11  2010/05/12 13:22:22  marpas
	setDialectSRID and getDialectSRID static methods added to dbGeoObejct class
	getSQLString with dialect set to Oracle11 use the SRID passed to the above methods
	
	Revision 2.10  2010/05/04 15:59:20  marpas
	Postgres8 behaviour like Informix9
	
	Revision 2.9  2010/04/30 16:23:05  marpas
	ORACLE spatial/locator support in progress ..
	
	Revision 2.8  2010/04/26 11:40:20  marpas
	work in progress Oracle11 Spatial and Locator support implementation
	
	Revision 2.7  2010/04/09 17:23:22  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.6  2009/06/26 09:48:18  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.5  2009/06/24 09:27:54  marpas
	ntersections still in progress
	
	Revision 2.4  2009/06/23 13:29:43  marpas
	isInsideGeo and intersectGeo method can be invoked on CONST objects
	
	Revision 2.3  2009/06/22 14:39:42  crivig
	bug fixed for intersect/inside methods: only outer ring considered  and
	create a pntList for dbGeoBox
	
	Revision 2.2  2009/06/05 09:31:18  crivig
	 isInsideGeo and intersectGeo methods added
	
	Revision 2.1  2007/06/09 18:45:54  marpas
	bbox method added
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.9  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.8  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.7  2004/05/27 12:55:13  marpas
	some cosmetic changes,
	new method unionWith in dbGeoPntList
	
	Revision 1.6  2003/09/03 13:57:43  paoscu
	Export methods are now const.
	
	Revision 1.5  2003/05/22 09:20:55  marpas
	ring interface is more homogeneous now
	
	Revision 1.4  2003/04/30 13:07:28  marpas
	using namespace std no longer in includes
	
	Revision 1.3  2003/03/17 16:42:48  marpas
	enumerant type added
	
	Revision 1.2  2002/11/29 17:48:46  danalt
	add exportXML, corrected some bugs
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/


#include <dbGeoBox.h>
#include <dbGeoPnt.h>
#include <dbGeoCircle.h>
#include <dbGeoEllipse.h>
#include <dbGeoRing.h>
#include <dbGeoPolygon.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <XMLOstream.h>
#include <dbGeoPoint.h>
#include <RegExp.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/  dbGeoRing     _/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbGeoRing)

dbGeoRing::dbGeoRing() : dbGeoObject()
{
}

dbGeoRing::dbGeoRing(const dbGeoPntList &pl) :
	dbGeoObject(pl)
{
}

dbGeoRing::~dbGeoRing() throw() { }



dbGeoRing::dbGeoRing(const dbGeoRing &r) : dbGeoObject(r)
{
}



dbGeoRing &dbGeoRing::operator=(const dbGeoRing &r)
{
	if(this != &r) {
		dbGeoObject::operator=(r) ;
	}
    return *this ;
}

const dbGeoPntList &dbGeoRing::ring(size_t i) const 
{
	if (0 != i) { // only one ring allowed
		std::ostringstream os ;
		os << "dbGeoRing::ring " << i << " greater than 0" ;
		
		ACS_THROW(exBoundsException(os.str())) ; // PRQA S 3081
	}
	
	return _points ;
}

dbGeoPntList &dbGeoRing::ring(size_t i) 
{
	if (0 != i) { // only one ring allowed
		std::ostringstream os ;
		os << "dbGeoRing::ring " << i << " greater than 0" ;
		
		ACS_THROW(exBoundsException(os.str())) ; // PRQA S 3081
	}
	
	return _points ; // PRQA S 4024
	
}


bool dbGeoRing::intersectGeo(const dbGeoObject  & geo) const // PRQA S 4020
{
	// this method checks if the GeoRing is geometrically inside the given dbGeoObject argument
	// timerange not considered
	bool intersect = false;
	const dbGeoPntList PntList = ring(0);
	
	switch(geo.type())
	{
    case dbGeoObject::Point:
		{
			const dbGeoPoint * p = dynamic_cast<const dbGeoPoint *> (&geo); // PRQA S 3081
			return PntList.contains(p->point()) ; // the geo ring must contain the point
		}
    case dbGeoObject::Box:
		{
			// check for spatial intersection
			const dbGeoBox  * box = dynamic_cast<const dbGeoBox *> (&geo); // PRQA S 3081
			return box->intersectGeo(*this);
		}
    case dbGeoObject::Lineseg:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for Lineseg shape")) ; // PRQA S 3081		
		}
    case dbGeoObject::Circle:
		{
			const dbGeoCircle * circle = dynamic_cast<const dbGeoCircle *> (&geo); // PRQA S 3081
			return circle->intersectGeo(*this);
		}
    case dbGeoObject::String:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for String shape")) ; // PRQA S 3081		
		}
    case dbGeoObject::Ring:
		{
			const dbGeoRing * ring = dynamic_cast<const dbGeoRing *> (&geo); // PRQA S 3081
			const dbGeoPntList PntList2 = ring->ring(0);	


			//  first check if at least one point is inside the shape
			intersect = (PntList.containsAtLeastOne(PntList2) || PntList2.containsAtLeastOne(PntList));

			// if no points included, check for lines intersection
			if(!intersect) { intersect = PntList.intersect(PntList2, true, true); }

			return intersect;
		}
    case dbGeoObject::Polygon:
		{
			const dbGeoPolygon * pol = dynamic_cast<const dbGeoPolygon *> (&geo); // PRQA S 3081
			// loop on all polygon rings
			const dbGeoPntList PntList2 = pol->ring(0);	

			//  first check if at least one point is inside the shape
			intersect = (PntList.containsAtLeastOne(PntList2) || PntList2.containsAtLeastOne(PntList));

			// if no points included, check for lines intersection
			if(!intersect) { intersect = PntList.intersect(PntList2, true, true); }

			return intersect;
		}
    case dbGeoObject::Ellipse:
		{
			// not implemented
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for Ellipse shape")) ;  // PRQA S 3081       
		}

    default:
		{
			// unknown: not implemented
			ACS_THROW(dbGeoObject::ExecuteException ("type not recognized  ")) ;  // PRQA S 3081     
		}
	}
	return intersect;
}

std::string dbGeoRing::getWKTString(bool lonFirst) const {
	std::ostringstream ss;
	
	ss << std::setprecision(dbGeoObject::wktPrecision) << type2WKT(type()) << "((";
	
	const dbGeoPntList& pnts = _rings[0];
	size_t pointsnum = pnts.points();
	for (size_t p = 0; p < pointsnum; ++p) {
		if (lonFirst) {
			ss << pnts[p].lon() << " " <<  pnts[p].lat();
		} else {
			ss << pnts[p].lat() << " " <<  pnts[p].lon();		
		}
		
		if (p < pointsnum - 1) {
			ss << ", ";
		}
	}
	
	ss << "))";
	
	return ss.str();
}


bool dbGeoRing::isInsideGeo(const dbGeoObject & geo) const
{
	// this method checks if the GeoBox  is geometrically inside the given dbGeoObject argument
	// timerange not considered
	
	bool inside = false;
	// Get the point list for the GeoRing
	const dbGeoPntList PntList = ring(0);
	switch(geo.type())
	{
	case dbGeoObject::Point:
		{
			// a geo ring can't be inside a point
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for Point shape")) ; // PRQA S 3081
		}
		break ;
	case dbGeoObject::Box:
		{
			const dbGeoBox  * box = dynamic_cast<const dbGeoBox *> (&geo); // PRQA S 3081
			
			// Get the point list for box --> counter clockwise direction!!
			dbGeoPntList PntList2;
			PntList2.append(box->p1());	
			PntList2.append(dbGeoPnt(box->p1().lat(),box->p2().lon()));	
			PntList2.append(box->p2());	
			PntList2.append(dbGeoPnt(box->p2().lat(),box->p1().lon()));	

			inside = PntList.inside(PntList2);
		}
		break ;
	case dbGeoObject::Lineseg:
		{	// a geo ring can't be inside a Lineseg
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for Lineseg shape")) ;	 // PRQA S 3081
		}
		break ;
	case dbGeoObject::Circle:
		{
			const dbGeoCircle * circle = dynamic_cast<const dbGeoCircle *> (&geo); // PRQA S 3081

			//loop on box points
			//and check that all points are inside the circle 
			for(size_t i = 0; i< PntList.points(); i++)
			{
				// if at least one is outside then return false
				if(PntList[i].distance(circle->center()) > (circle->radius()))
				{
					inside = false;
					break;
				}
			}
		}
		break ;
	case dbGeoObject::String:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for String shape")) ; // PRQA S 3081
		}
		break ;
	case dbGeoObject::Ring:
		{
			const dbGeoRing * ring = dynamic_cast<const dbGeoRing *> (&geo); // PRQA S 3081
			const dbGeoPntList PntList2 = ring->ring(0);	
			inside = PntList.inside(PntList2);
		}
		break ;
	case dbGeoObject::Polygon:
		{
			const dbGeoPolygon * pol = dynamic_cast<const dbGeoPolygon *> (&geo); // PRQA S 3081
			const dbGeoPntList PntList2 = pol->ring(0);	
			inside = PntList.inside(PntList2);
		}
		break ;
	case dbGeoObject::Ellipse:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for ellipse shape")) ; // PRQA S 3081
		}
		break ;

	default:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("type not recognized")) ;  // PRQA S 3081
		}
		break ;
	}
	return inside;
}


const dbGeoPnt &dbGeoRing::operator[](size_t i) const 
{
	return dbGeoObject::operator[](i) ;
}

dbGeoPnt &dbGeoRing::operator[](size_t i) 
{
	return dbGeoObject::operator[](i) ;
}



dbGeoObject::Type dbGeoRing::type() const 
{
	return Ring ;
}



dbGeoObject* dbGeoRing::clone() const 
{
	return new dbGeoRing(*this) ;
}


ostream &dbGeoRing::getSqlString(ostream &os, const string &dialectName) const // PRQA S 4020
{
	if (dialectName == "Informix9") {
	// 'GeoRing(((val,val),(val,val)),ANY,ANY)'
		os << "GeoRing(" ;
		_points.getSqlString(os, dialectName, precision()) << ", " ; // ((val,val),...)
		_altitudeRange.getSqlString(os) << ", " ;
		_timeRange.getSqlString(os) << ")" ;

		return os ; // PRQA S 4028
	}
	if (dialectName == "Postgres8") {
	// 'GeoRing(((val,val),(val,val)),ANY,ANY)'
		os << "POLYGON(" ;
		_points.getSqlString(os, dialectName, true, precision()) << ")" ; // ((val,val),...)

		return os ; // PRQA S 4028
	}
	else if (dialectName == "Oracle11") {
	// sdo_geometry (2002, null, null, sdo_elem_info_array (1,2,1),sdo_ordinate_array (10,55, 15,55, 20,60, 10,60, 10,55))	}
		os << "MDSYS.SDO_GEOMETRY("
		   << "2002," << getDialectSRID(dialectName) << ",null,MDSYS.SDO_ELEM_INFO_ARRAY(1,2,1),"
		   << "MDSYS.SDO_ORDINATE_ARRAY("
		   << setprecision(precision()) ;
		_points.getSqlString(os, dialectName, precision()) ; // val,val,...),
		// rebate first point
		os << ", " ;
		_points[0].getSqlString(os, dialectName, precision()) ;
		os << "))" ;

		return os ; // PRQA S 4028
	}
    else {
    	ACS_THROW (ExecuteException("Dialect unimplemented: "+ dialectName)) ; // PRQA S 3081
    }
}


void dbGeoRing::bbox(dbGeoBox &bb, double latmargin, double lonmargin) const 
{
	_points.bbox(bb,latmargin,lonmargin) ;
    const TimeRange &tr = timeRange() ;
    if (tr.start().isFilled() && tr.stop().isFilled()) {
	    bb.timeRange(timeRange().wholeDay()) ;
    }
}


void dbGeoRing::exportSpecific(rsResourceSet &s) const
{
	rsPushNSpace nsp(s,"dbGeoRing") ;
	exportBase(s) ;
}

void dbGeoRing::exportSpecificXML(XMLOstream &xst) const
{
	XMLOstream::Tag txml(xst,"dbGeoRing") ;
	exportBaseXML(xst) ;
}

void dbGeoRing::importSpecific(rsResourceSet &s)
{
	rsPushNSpace nsp(s,"dbGeoRing") ;
	importBase(s) ;
}

bool dbGeoRing::intersect(const dbGeoRing &r) const 
{
	return _points.intersect(r._points, true, true) ;
}

size_t dbGeoRing::crossing180() const 
{
	return _points.crossing180() ;
}

bool dbGeoRing::contains(const dbGeoPnt &p) const 
{
	return _points.contains(p) ;
}

void dbGeoRing::parseOracle(const string &is, int offs, int etype, int interpr) 
{
/* MDSYS.SDO_GEOMETRY(2003,8307,null,MDSYS.SDO_ELEM_INFO_ARRAY(1,1003,1),MDSYS.SDO_ORDINATE_ARRAY(10,10, 20,20, 40,20, 30,10, 10,10))*/
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "is: \"" << is << "\"") ;
    
	ACS_COND_THROW ((etype != 2) || (interpr != 1), // PRQA S 3081, 3380
			UnrecognizedTypeException("Invalid etype/iterpr pair: "+ ((is.length() > 64) ? is.substr(0,64) : is) + " ...\"")) ;  // PRQA S 3081, 3380
	
	
	RegExp recoord(".*SDO_ORDINATE_ARRAY[ \t]*[(]") ;
	regoff_t coordbeg ; // PRQA S 4101 2  
    regoff_t coordend ;
	if (!recoord.match(is,coordbeg,coordend)) {
		ACS_THROW(UnrecognizedTypeException("Unrecognized \"" + ((is.length() > 64) ? is.substr(0,64) : is) + " ...\"")) ;	 // PRQA S 3081, 3380
	}

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "coords: \"" << is.substr(coordend) << "\"") ; // PRQA S 3000

	dbGeoPntList p ;
	p.parseOracle(is.substr(coordend)) ; // PRQA S 3000

	if (p[0].distance(p[p.points()-1]) > 1e-8) { // PRQA S 3084, 4400
		ACS_THROW(UnrecognizedTypeException("Open ring")) ; // PRQA S 3081
    }
	// erase lastpoint.
	
	p.erase(p.points()-1) ; // PRQA S 3084

	*this = dbGeoRing(p) ; // PRQA S 3081
	dbGeoAltitudeRange ar ;
	dbGeoTimeRange tr ;
	altitudeRange(ar) ;
	timeRange(tr) ;
}

/* GeoRing(((lat1, long1), (lat2, long12),...(latn, longn)),altrange, timerange) */

istream  &
operator >> (istream &is, dbGeoRing& r) // PRQA S 4020
{
/* GeoRing(((lat1, long1), (lat2, long12),...(latn, longn)),altrange, timerange) */
	if (!is.good()) { return is ; } // PRQA S 4028
	std::string oname ;
	if (!dbGeoObject::extractObjType(is,oname) || oname != "GeoRing(") {
		is.setstate(std::ios::failbit) ;
		return is ;	 // PRQA S 4028
	}

	dbGeoPntList pl ;
	dbGeoAltitudeRange ar ;
	dbGeoTimeRange tr ;
	
	char comma ; // PRQA S 4101 2
    char ell ;
	is >> pl >> comma >> ar >> comma >> tr >> ell ;

	if (!is.fail()) {
		r = dbGeoRing(pl) ; // PRQA S 3081
		r.altitudeRange(ar) ;
		r.timeRange(tr) ;
	}
	return is ; // PRQA S 4028
}

_ACS_END_NAMESPACE
