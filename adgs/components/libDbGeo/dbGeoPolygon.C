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
	Revision 5.9  2018/06/28 14:10:03  davide.tiriticco
	Fixed bug in getWKTString() that returned an open polygon representation.
	
	Revision 5.8  2017/11/21 15:14:55  davide.tiriticco
	Implemented functionality getWKTString
	
	Revision 5.7  2015/09/22 14:09:47  marfav
	Polygon closure checked
	
	Revision 5.6  2015/04/28 09:59:07  marpas
	fixing problem in bbox methods when timerange was not filled.
	
	Revision 5.5  2014/05/28 09:50:02  marpas
	fixed coordinate inversion regression, getSqlString out format
	
	Revision 5.4  2014/05/27 14:28:57  marpas
	regression in reading negative coordinates fixed
	generation of numeric strings fixed
	
	Revision 5.3  2014/05/27 10:40:45  marpas
	precision handling when reading data from postgres SQL result
	
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
	
	Revision 2.17  2013/05/14 18:00:35  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.16  2012/02/13 11:15:40  marpas
	refactoring in progress
	
	Revision 2.15  2011/09/29 14:48:10  marpas
	simplifying templates for codecheck
	
	Revision 2.14  2010/09/20 15:18:55  marpas
	autointersection exception notifies interecting segments along with their coordinates
	
	Revision 2.13  2010/05/19 16:06:06  marpas
	Oracle spatial/locator parse completed
	
	Revision 2.12  2010/05/12 13:22:22  marpas
	setDialectSRID and getDialectSRID static methods added to dbGeoObejct class
	getSQLString with dialect set to Oracle11 use the SRID passed to the above methods
	
	Revision 2.11  2010/05/04 15:59:20  marpas
	Postgres8 behaviour like Informix9
	
	Revision 2.10  2010/04/30 16:23:05  marpas
	ORACLE spatial/locator support in progress ..
	
	Revision 2.9  2010/04/26 11:40:20  marpas
	work in progress Oracle11 Spatial and Locator support implementation
	
	Revision 2.8  2010/04/09 17:23:22  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.7  2009/06/26 09:48:18  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.6  2009/06/24 09:27:54  marpas
	ntersections still in progress
	
	Revision 2.5  2009/06/23 13:29:43  marpas
	isInsideGeo and intersectGeo method can be invoked on CONST objects
	
	Revision 2.4  2009/06/22 14:39:42  crivig
	bug fixed for intersect/inside methods: only outer ring considered  and
	create a pntList for dbGeoBox
	
	Revision 2.3  2009/06/05 09:31:18  crivig
	 isInsideGeo and intersectGeo methods added
	
	Revision 2.2  2007/06/09 18:45:54  marpas
	bbox method added
	
	Revision 2.1  2006/05/12 16:32:07  marpas
	Bug fixed when north pole is inside the polygon
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.13  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.12  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.11  2004/06/16 09:44:09  marpas
	measurement methods added
	
	Revision 1.10  2003/09/03 13:57:43  paoscu
	Export methods are now const.
	
	Revision 1.9  2003/08/08 19:42:14  marpas
	a wrap can be passed to signal the polygon wraps around the dateline
	in this case no auto intersection check is done and the polygon
	orientation is left unchanged and never forced
	
	Revision 1.8  2003/07/11 13:06:06  marpas
	exException::clone implemented
	
	Revision 1.7  2003/07/07 15:05:25  marpas
	consistency check improved
	
	Revision 1.6  2003/04/30 13:07:28  marpas
	using namespace std no longer in includes
	
	Revision 1.5  2003/03/17 16:42:48  marpas
	enumerant type added
	
	Revision 1.4  2003/02/04 10:26:36  marpas
	comments changed
	
	Revision 1.3  2003/02/03 18:58:59  marpas
	Exceptions hierarchy tree changed to hilight db exceptions
	
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
#include <dbGeoPoint.h>
#include <dbGeoPolygon.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <XMLOstream.h>
#include <RegExp.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/  dbGeoPolygon  _/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbGeoPolygon)

dbGeoPolygon::dbGeoPolygon() : dbGeoObject()
{
}


dbGeoPolygon::dbGeoPolygon(const dbGeoPntList &pl, bool wrap) : dbGeoObject()
{
	addring(pl, wrap) ;
}

dbGeoPolygon::~dbGeoPolygon() throw() {}


dbGeoPolygon::dbGeoPolygon(const dbGeoPolygon &p) : dbGeoObject(p)
{	
}


dbGeoPolygon &dbGeoPolygon::operator=(const dbGeoPolygon &p)
{
	if(this != &p) {
		dbGeoObject::operator=(p) ;
	}
    return *this ;
}


void dbGeoPolygon::addring(const dbGeoPntList &ring, bool wrap) 
{
	
	if (ring.points() < 3) { // too few points in the adding ring // PRQA S 4400
		ACS_THROW(GeoPolygonException("Too few points for a polygon ring")) ; // PRQA S 3081
	}
	
	if (!wrap) {
		size_t s1 ; // PRQA S 4101 2 
        size_t s2 ;
	bool isClosed = (ring[0] == ring[ring.points()-1]);
		if (ring.autoIntersect(isClosed, &s1, &s2)) { // check if the adding ring intersects itself
			ostringstream os ;
			ring.getSqlString(os, "Postgres8", 13) ; // just for the string // PRQA S 4400
			size_t np = ring.points() ;
			os << " intersection found between segments: " 
				   << s1 << " (" 
				   		<< ring[s1%np].lat() << "," << ring[s1%np].lon()  // PRQA S 3084 8
						<< " - "
				   		<< ring[(s1+1)%np].lat() << "," << ring[(s1+1)%np].lon() 
				   << ") and "
				   << s2  << " (" 
				   		<< ring[s2%np].lat() << "," << ring[s2%np].lon() 
						<< " - "
				   		<< ring[(s2+1)%np].lat() << "," << ring[(s2+1)%np].lon() << ")" ;
			ACS_THROW(GeoPolygonException("The ring intersect itself - poly is: " + os.str())) ; // PRQA S 3081
		}

		if (ring.isCCW() == dbGeoPntList::CounterClockWise) {
			append(ring) ; // in ccw 
        }
		else {
			append(ring.ccw()) ; // converts to ccw 
        }
	}
	else { // can wrap: no check about geometry
		append(ring) ;
    }
}


bool dbGeoPolygon::intersectGeo(const dbGeoObject  & geo) const // PRQA S 4020
{
	// this method checks if the GeoPolygon intersects the given dbGeoObject argument
	// timerange not considered
	
	bool intersect = false;
	
	switch(geo.type())
	{
	case dbGeoObject::Point:
		{
			const dbGeoPoint * pnt = dynamic_cast<const dbGeoPoint *> (&geo); // PRQA S 3081
			return pnt->intersectGeo(*this);
		}
		break ;
	case dbGeoObject::Box:
		{
			return geo.intersectGeo(*this);
		}
	case dbGeoObject::Lineseg:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for Lineseg shape!  ")) ;	 // PRQA S 3081	
		}
		break ;
	case dbGeoObject::Circle:
		{
			const dbGeoCircle * circle = dynamic_cast<const dbGeoCircle *> (&geo); // PRQA S 3081
			return circle->intersectGeo(*this);
		}
		break ;
	case dbGeoObject::String:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for String shape!  ")) ; // PRQA S 3081
		}
		break ;
	case dbGeoObject::Ring:
		{
			const dbGeoRing * ring = dynamic_cast<const dbGeoRing *> (&geo); // PRQA S 3081
			return ring->intersectGeo(*this);
		}
		break ;
	case dbGeoObject::Polygon:
		{
			const dbGeoPolygon * pol = dynamic_cast<const dbGeoPolygon *> (&geo); // PRQA S 3081
			const dbGeoPntList PntList = ring(0);
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
				ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for Ellipse shape!  ")) ;	 // PRQA S 3081	
			}

	default:
			{
				ACS_THROW(dbGeoObject::ExecuteException ("type not recognized  ")) ; // PRQA S 3081
			}
	}
	return intersect ;
}

bool dbGeoPolygon::isInsideGeo(const dbGeoObject & geo) const 
{
	// this method checks if the GeoPolygon  is geometrically inside the given dbGeoObject argument
	// timerange not considered
	
	bool inside = false;
	// Get the point list for the GeoPolygon
	const dbGeoPntList PntList = ring(0);
	switch(geo.type())
	{
	case dbGeoObject::Point:
		{
			// polygon cannot be inside a point
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for Point shape!  ")) ; // PRQA S 3081		
		}
		break ;
	case dbGeoObject::Box:
		{
			const dbGeoBox  * box = dynamic_cast<const dbGeoBox *> (&geo); // PRQA S 3081
			// Get the point list for box  --> counter clockwise direction!!
			dbGeoPntList PntList2;
			PntList2.append(box->p1());	
			PntList2.append(dbGeoPnt(box->p1().lat(),box->p2().lon()));	
			PntList2.append(box->p2());	
			PntList2.append(dbGeoPnt(box->p2().lat(),box->p1().lon()));	
			// Check that outer geoPolygon ring is inside GeoBox	
			inside = PntList.inside(PntList2);
		}
		break ;
	case dbGeoObject::Lineseg:
		{
			// polygon cannot be inside a segment
			ACS_THROW(dbGeoObject::ExecuteException ("dbGeoPolygon::inside: method not implemented for Lineseg shape!  ")) ; // PRQA S 3081		
		}
		break ;
	case dbGeoObject::Circle:
		{
			const dbGeoCircle * circle = dynamic_cast<const dbGeoCircle *> (&geo); // PRQA S 3081

			// Check that outer geoPolygon ring is inside GeoCircle	
			for(size_t i = 0; i< PntList.points(); i++)
			{
				// if at least one is outside then return false
				if(PntList[i].distance(circle->center()) > (circle->radius()))
				{
					inside = false;
					break;
				}
				inside = true;
			}
		}
		break ;
	case dbGeoObject::String:
		{
			// polygon cannot be inside a geo string
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for String shape!  ")) ; // PRQA S 3081		
		}
		break ;
	case dbGeoObject::Ring:
		{
			// Check that outer geoPolygon ring is inside GeoCircle	
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
			// unimplemented
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for ellipse shape!  ")) ; // PRQA S 3081		
		}
		break ;

	default:
		{
			dbGeoObject::ExecuteException rx(string("type not recognized  ")) ; // PRQA S 3081
			ACS_THROW(rx) ;		
		}
		break ;
	}
	return inside;
}




std::string dbGeoPolygon::getWKTString(bool lonFirst) const {
	std::ostringstream ss;
	ss << std::setprecision(dbGeoObject::wktPrecision) << type2WKT(type()) << "((";
	
	dbGeoPntList pnts = _rings[0];
	size_t pointsnum = pnts.points();
	
	// If it is an open ring, then in WKT the last point must be equal to the first one
	if (pnts[0].distance(pnts[pointsnum - 1]) > 1e-12) {
		pnts.append(pnts[0]);
		++pointsnum;
	}
	
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




const dbGeoPntList &dbGeoPolygon::ring(size_t i) const 
{
	return dbGeoObject::ring(i) ;
}

dbGeoPntList &dbGeoPolygon::ring(size_t i) 
{
	return dbGeoObject::ring(i) ;
}


dbGeoObject::Type dbGeoPolygon::type() const 
{
	return Polygon ;
}



dbGeoObject* dbGeoPolygon::clone() const 
{
	return new dbGeoPolygon(*this) ;
}

ostream &dbGeoPolygon::getSqlString(ostream &os, const string &dialectName) const // PRQA S 4020
{
#warning polygon with holes are currently unsupported for ORACLE
	size_t rngs =  rings() ;

	ACS_COND_THROW(rngs < 1, GeoPolygonException("No rings in polygon")) ; // PRQA S 3081

	if (dialectName == "Informix9") {
	// // 'GeoPolygon((((val,val),(val,val), .....),((val,val), ....)),ANY,ANY)'

		os << "GeoPolygon((" ;
		for(size_t i = 0 ; i < rngs; i++) {
			ring(i).getSqlString(os, dialectName, precision()) << ((i != rngs-1) ? "," : "),") ; // ((val,val),(val,val), .....) -- , or ")," at last ring // PRQA S 3084, 3380
        }
		_altitudeRange.getSqlString(os) << ", " ;
		_timeRange.getSqlString(os) << ")" ;

		return os ; // PRQA S 4028
	}
	else if (dialectName == "Oracle11") {
		ACS_COND_THROW(rngs > 1, // PRQA S 3081
			GeoPolygonException("Polygon with holes not implemented for dialectName: "+dialectName)) ;
		// sdo_geometry (2003, null, null, sdo_elem_info_array (1,1003,1),sdo_ordinate_array (10,105, 15,105, 20,110, 10,110, 10,105))
		os << "MDSYS.SDO_GEOMETRY("
		   << "2003," << getDialectSRID(dialectName) << ",null,MDSYS.SDO_ELEM_INFO_ARRAY(1,1003,1),"
		   << "MDSYS.SDO_ORDINATE_ARRAY("
		   << setprecision(precision()) ; // PRQA S 4400
		ring(0).getSqlString(os, dialectName, precision()) ; // val,val,val,val, .....
		os << "))" ;

		return os ; // PRQA S 4028
	}
	else if (dialectName == "Postgres8") {
		// POLYGON((0 0, 1 0, 1 1, 0 1, 0 0))
		os << "POLYGON(" ;
		for(size_t i = 0 ; i < rngs; i++) {
			ring(i).getSqlString(os, dialectName, true, precision()) << ((i != rngs-1) ? "," : "") ; // ((val,val),(val,val), .....) -- , or ")," at last ring // PRQA S 3084, 3380
        }
		os << ")" ;

		return os ; // PRQA S 4028
	}
    else {
	    ACS_THROW (ExecuteException("Dialect unimplemented: "+ dialectName)) ; // PRQA S 3081
    }
}


void dbGeoPolygon::bbox(dbGeoBox &bb, double latmargin, double lonmargin) const 
{
	ring(0).bbox(bb,latmargin,lonmargin) ;
    const TimeRange &tr = timeRange() ;
    if (tr.start().isFilled() && tr.stop().isFilled()) {
	    bb.timeRange(timeRange().wholeDay()) ;
    }
}

void dbGeoPolygon::exportSpecific(rsResourceSet &s) const
{
	rsPushNSpace nsp(s,"dbGeoPolygon") ;
	exportBase(s) ;
}

void dbGeoPolygon::exportSpecificXML(XMLOstream &xst) const
{
	XMLOstream::Tag txml(xst,"dbGeoPolygon") ;
	exportBaseXML(xst) ;
}

void dbGeoPolygon::importSpecific(rsResourceSet &s)
{
	rsPushNSpace nsp(s,"dbGeoPolygon") ;
	importBase(s) ;
}

string dbGeoPolygon::properties() const 
{
	ostringstream os ;
	os << "Length: " << fixed << setprecision(1) << ring(0).lengthKm() << " [Km]" ; 
	return os.str() ;
}


void dbGeoPolygon::parseOracle(const string &is, int offs, int etype, int interpr) 
{
/* MDSYS.SDO_GEOMETRY(2003,8307,null,MDSYS.SDO_ELEM_INFO_ARRAY(1,1003,1),MDSYS.SDO_ORDINATE_ARRAY(10,10, 20,20, 40,20, 30,10))*/
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "is: \"" << is << "\"") ;
	ACS_COND_THROW ((etype != 1003) || (interpr != 1), // PRQA S 3081, 3380
			UnrecognizedTypeException("Invalid etype/iterpr pair: "+ ((is.length() > 64) ? is.substr(0,64) : is) + " ...\"")) ;
	
	
	RegExp recoord(".*SDO_ORDINATE_ARRAY[ \t]*[(]") ;
	regoff_t coordbeg ; // PRQA S 4101 2
    regoff_t coordend ;
	if (!recoord.match(is,coordbeg,coordend)) {
		ACS_THROW(UnrecognizedTypeException("Unrecognized \"" + ((is.length() > 64) ? is.substr(0,64) : is) + " ...\"")) ; // PRQA S 3081, 3380
	}

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "coords: \"" << is.substr(coordend) << "\"") ; // PRQA S 3000

	dbGeoPntList p ;
	p.parseOracle(is.substr(coordend)) ; // PRQA S 3000
#warning No holes in Oracle Polygons
	*this = dbGeoPolygon(p) ; // PRQA S 3081
	dbGeoAltitudeRange ar ;
	dbGeoTimeRange tr ;
	altitudeRange(ar) ;
	timeRange(tr) ;
}


std::istream &
operator >> (std::istream &is, dbGeoPolygon& polygon) // PRQA S 4020
{
 /* GeoPolygon((((lat11, long11), (lat21, long21), ... (latn1, longn1)),
((lat12, long12), (lat22, long22), ... (latn2, longn2)), ...
((lat1N, long1N), (lat2N, long2N), ... (latnN, longnN))),
altrange, timerange) */
/* POLYGON((10 10, 20 20, 20 40, 10 30, 10 10), (40 10, 40 20, 40 30, 40 40, 40 10)) */
	if (!is.good()) { return is ; } // PRQA S 4028
	std::string oname ;
    bool informix = true ;
	if (!dbGeoObject::extractObjType(is,oname)) {
		is.setstate(std::ios::failbit) ;
		return is ;	 // PRQA S 4028
	}


    if (oname == "GeoPolygon(") {
        informix = true ;
    }
    else if (oname == "POLYGON(") {
        informix = false ;
    }
    else {
		is.setstate(std::ios::failbit) ;
		return is ;	 // PRQA S 4028
    }
	
	char ell ; // PRQA S 4101 2
    char comma ;

    if (informix) {
	    is >> ell ;
	    dbGeoPolygon pol ;
	    for (int i=0; !is.fail();i++) { // PRQA S 4238
		    dbGeoPntList p ;
		    is >> p ;
		    if (is.fail()) { return is ; } // PRQA S 4028
		    if (0 == i) {
			    pol = dbGeoPolygon(p) ; // PRQA S 3081
            }
		    else {
			    pol.addring(p) ;
            }
		    is >> comma ;
		    if (comma == ',') { continue ; }
		    if (comma == ')') { break ; }
	    }

	    dbGeoAltitudeRange ar ;
	    dbGeoTimeRange tr ;
	    is >> comma >> ar >> comma >> tr >> ell ;
	    if (!is.fail()) {
		    polygon = pol ;
		    polygon.altitudeRange(ar) ;
		    polygon.timeRange(tr) ;
	    }
    }
    else {
	    dbGeoPolygon pol ;
	    for (int i=0; !is.fail();i++) { // PRQA S 4238
	        is >> ell ;
		    dbGeoPntList p ;
            int precision = 0 ;
		    dbGeoPntList::parsePostgres(p, is, precision) ;
		    if (is.fail()) { return is ; } // PRQA S 4028
            // remove last point (is the closure)
            p.erase(p.points()-1) ; // PRQA S 3084
		    if (0 == i) {
			    pol = dbGeoPolygon(p) ; // PRQA S 3081
                pol.precision(precision) ;
            }
		    else {
			    pol.addring(p) ;
                pol.precision(max(precision, pol.precision())) ;
            }
		    is >> comma ;
		    if (comma == ',') { continue ; }
		    if (comma == ')') { break ; }
	    }
	    if (!is.fail()) {
		    polygon = pol ;
	    }
    }

	return is ; // PRQA S 4028
}



std::ostream & operator << (std::ostream &os, const dbGeoPolygon& gs) 
{
   return gs.getSqlString(os, "Postgres8") ;
}

exostream & operator << (exostream &os, const dbGeoPolygon& gs) // PRQA S 4222
{
    ostringstream s ;
    gs.getSqlString(s, "Postgres8") ;
    return os << s.str() ;
}

_ACS_END_NAMESPACE

