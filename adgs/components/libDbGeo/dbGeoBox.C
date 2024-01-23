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
	
	Revision 5.2  2014/05/09 11:39:54  marpas
	coding best practices applied
	qa & c++ warnings fixed
	last throwing macros for exceptions adopted
	
	Revision 5.1  2014/05/06 12:25:57  marpas
	precision in getting sql string managed
	dbGeoObject interface has now methods to set/get precision and a defaultPrecision static constant
	precision must be greater than 2, note that precision will affect lenght of string that will be used in queries, limiting the maximum number of points as the query string has limited size
	
	Revision 5.0  2013/07/11 16:07:11  marpas
	libException 5.x standards adopted
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.20  2013/05/17 16:32:25  marpas
	stream insertion operators added
	
	Revision 2.19  2013/05/14 18:00:34  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.18  2012/02/13 11:15:39  marpas
	refactoring in progress
	
	Revision 2.17  2011/09/29 14:48:09  marpas
	simplifying templates for codecheck
	
	Revision 2.16  2011/03/31 15:16:15  marpas
	Oracle11 has the lon, lat convention in the strings
	
	Revision 2.15  2010/05/12 13:22:21  marpas
	setDialectSRID and getDialectSRID static methods added to dbGeoObejct class
	getSQLString with dialect set to Oracle11 use the SRID passed to the above methods
	
	Revision 2.14  2010/05/04 15:59:20  marpas
	Postgres8 behaviour like Informix9
	
	Revision 2.13  2010/04/30 16:23:05  marpas
	ORACLE spatial/locator support in progress ..
	
	Revision 2.12  2010/04/27 11:42:26  marpas
	still implementing ORACLE spatial/locator support.
	
	Revision 2.11  2010/04/26 14:32:26  marpas
	implementing Oracle spatial/locator syntax. Work in progress
	
	Revision 2.10  2010/04/26 11:40:19  marpas
	work in progress Oracle11 Spatial and Locator support implementation
	
	Revision 2.9  2010/04/09 17:23:21  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.8  2010/04/08 10:02:43  crivig
	return added in switch
	
	Revision 2.7  2009/06/26 09:48:17  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.6  2009/06/24 09:27:53  marpas
	ntersections still in progress
	
	Revision 2.5  2009/06/23 16:26:40  marpas
	intersect in dbGeoBox fixed
	
	Revision 2.4  2009/06/23 13:29:43  marpas
	isInsideGeo and intersectGeo method can be invoked on CONST objects
	
	Revision 2.3  2009/06/22 14:39:42  crivig
	bug fixed for intersect/inside methods: only outer ring considered  and
	create a pntList for dbGeoBox
	
	Revision 2.2  2009/06/05 09:31:18  crivig
	 isInsideGeo and intersectGeo methods added
	
	Revision 2.1  2007/06/09 18:45:53  marpas
	bbox method added
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.7  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.6  2003/09/03 13:57:43  paoscu
	Export methods are now const.
	
	Revision 1.5  2003/04/30 13:07:28  marpas
	using namespace std no longer in includes
	
	Revision 1.4  2003/04/15 20:40:43  marpas
	GCC3.2
	
	Revision 1.3  2003/03/17 16:42:48  marpas
	enumerant type added
	
	Revision 1.2  2002/11/29 17:48:46  danalt
	add exportXML, corrected some bugs
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/


#include <dbGeoBox.h>
#include <dbGeoPnt.h>
#include <dbGeoPoint.h>
#include <dbGeoCircle.h>
#include <dbGeoEllipse.h>
#include <dbGeoRing.h>
#include <dbGeoString.h>
#include <dbGeoLineseg.h>
#include <dbGeoPolygon.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <XMLOstream.h>
#include <RegExp.h>
#include <Filterables.h>

#include <iomanip>


//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/    dbGeoBox    _/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/



_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbGeoBox)

dbGeoBox::dbGeoBox() : dbGeoObject()
{
}

dbGeoBox::~dbGeoBox() throw() { }

dbGeoBox::dbGeoBox(const dbGeoPnt &p1, const dbGeoPnt &p2) : dbGeoObject()
{
	if (p1 ==  p2) { // null area box 
		ACS_THROW(dbGeoObject::ExecuteException("cannot build a box with null area")) ; // PRQA S 3081		
	} 
	dbGeoObject::append(p1) ;
	dbGeoObject::append(p2) ;
}



dbGeoBox::dbGeoBox(const dbGeoBox &r) : dbGeoObject(r)
{	
}



dbGeoBox &dbGeoBox::operator=(const dbGeoBox &b)
{
	if(this != &b) {
		dbGeoObject::operator=(b) ;
	}
    return *this ;
}




void dbGeoBox::p1(const dbGeoPnt &pnt)
{

	if(0 != points()) {
		(*this)[0] = pnt ;
    }
	else {
		append(pnt) ;
    }
}

dbGeoPnt &dbGeoBox::p1()  
{
	while (points() < 1) { // the first point[0] could not have yet been inserted 
		append(dbGeoPnt(0,0)) ;
    }
	return (*this)[0] ;
}

const dbGeoPnt &dbGeoBox::p1() const
{
	return (*this)[0] ;
}

void dbGeoBox::p2(const dbGeoPnt &pnt)
{

	if(points() > 1) {
		(*this)[1] = pnt ;
    }
	else {
		while (points() < 2) { // the first point[0] could not have yet been inserted  // PRQA S 4400
			append(pnt) ;
        }
    }
}

dbGeoPnt &dbGeoBox::p2()
{
	while (points() < 2) { // the first point[0] could not have yet been inserted  // PRQA S 4400
		append(dbGeoPnt(0,0)) ;
    }
	return (*this)[1] ;
}

const dbGeoPnt &dbGeoBox::p2() const
{
	return (*this)[1] ;
}

dbGeoObject::Type dbGeoBox::type() const 
{
	return Box ;
}

dbGeoObject* dbGeoBox::clone() const 
{
	return new dbGeoBox(*this) ;
}

ostream &dbGeoBox::getSqlString(ostream &os, const string &dialectName) const // PRQA S 4020
{
	if (dialectName == "Informix9") {
	// 'GeoBox((val,val), (val,val),ANY,ANY)'
		os << "GeoBox(" ;

		os << "("
		   << setprecision(precision()) 
		   << min((*this)[0].lat(), (*this)[1].lat()) //sw
		   << ", "
		   << min((*this)[0].lon(), (*this)[1].lon())
		   << "), " ;

		os << "("
		   << max((*this)[0].lat(), (*this)[1].lat()) // ne
		   << ", "
		   << max((*this)[0].lon(), (*this)[1].lon())
		   << "), " ;

		_altitudeRange.getSqlString(os) << ", " ;
		_timeRange.getSqlString(os) << ")" ;

		return os ; // PRQA S 4028
	}
    else if (dialectName == "Postgres8") {
        os << "POLYGON(("
           << setprecision(precision())
           << (*this)[0].lon() << ' ' << (*this)[0].lat() 
           << ", "
           << (*this)[1].lon() << ' ' << (*this)[0].lat() 
           << ", "
           << (*this)[1].lon() << ' ' << (*this)[1].lat() 
           << ", "
           << (*this)[0].lon() << ' ' << (*this)[1].lat() 
           << ", "
           << (*this)[0].lon() << ' ' << (*this)[0].lat() 
           << "))" ;
        return os ; // PRQA S 4028
    }
	else if (dialectName == "Oracle11") {
		os << "MDSYS.SDO_GEOMETRY(" ;
		os << "2003," << getDialectSRID(dialectName) << ",null,MDSYS.SDO_ELEM_INFO_ARRAY(1,1003,3),"
		   << "MDSYS.SDO_ORDINATE_ARRAY("
		   << setprecision(precision()) 
		   << min((*this)[0].lon(), (*this)[1].lon())
		   << ","
		   << min((*this)[0].lat(), (*this)[1].lat()) //sw
		   << ", " ;
		os << max((*this)[0].lon(), (*this)[1].lon())
		   << ","
		   << max((*this)[0].lat(), (*this)[1].lat()) // ne
		   << "))" ;

		return os ; // PRQA S 4028
	}
    else {
	    ACS_THROW (ExecuteException("Dialect unimplemented: "+ dialectName)) ; // PRQA S 3081
    }
}


void dbGeoBox::bbox(dbGeoBox &bb, double latmargin, double lonmargin) const 
{
	bb.p1() = dbGeoPnt(min((*this)[0].lat(), (*this)[1].lat())-latmargin, min((*this)[0].lon(), (*this)[1].lon())-lonmargin) ;
	bb.p2() = dbGeoPnt(max((*this)[0].lat(), (*this)[1].lat())+latmargin, max((*this)[0].lon(), (*this)[1].lon())+lonmargin) ;
    const TimeRange &tr = timeRange() ;
    if (tr.start().isFilled() && tr.stop().isFilled()) {
	    bb.timeRange(timeRange().wholeDay()) ;
    }
}


void dbGeoBox::createPntList(const dbGeoPnt &  pnt1,const dbGeoPnt & pnt2, dbGeoPntList & boxPntList) const
{
	// Get the point list for Geobox --> counter clockwise direction!!
	boxPntList.append(pnt1); // SW point	
	boxPntList.append(dbGeoPnt(pnt1.lat(), pnt2.lon()));
	boxPntList.append(pnt2); // NE point	
	boxPntList.append(dbGeoPnt(pnt2.lat(), pnt1.lon()));

}


bool dbGeoBox::intersectGeo(const dbGeoObject  & geo) const // PRQA S 4020
{
	// this method checks if the GeoBox  is geometrically inside the given dbGeoObject argument
	// timerange not considered
	bool intersect = false;
	
	// get the pntList from the GeoBox
	dbGeoPntList boxPntList;
	createPntList(p1(),p2(), boxPntList);

	switch(geo.type())
	{
	case dbGeoObject::Point:
		{
			const dbGeoPoint * p = dynamic_cast<const dbGeoPoint *> (&geo); // PRQA S 3081
			return boxPntList.contains(p->point()) ;
		}
	case dbGeoObject::Box:
		{
			// create pntList
			const dbGeoBox  * box = dynamic_cast<const dbGeoBox *> (&geo); // PRQA S 3081
			dbGeoPntList PntList2;
			createPntList(box->p1(),box->p2(), PntList2);

			//  first check if at least one point is inside the shape
			intersect = (boxPntList.containsAtLeastOne(PntList2) || PntList2.containsAtLeastOne(boxPntList));

			// if no points included, check for lines intersection
			if(!intersect) { intersect = boxPntList.intersect(PntList2, true, true); }

			return intersect;
		}
	case dbGeoObject::Lineseg:
		{
			const dbGeoLineseg * l = dynamic_cast<const dbGeoLineseg *> (&geo); // PRQA S 3081
			dbGeoPntList lpntlist ;
			lpntlist.append(l->p1()) ;
			lpntlist.append(l->p2()) ;

			return boxPntList.containsAtLeastOne(lpntlist) || boxPntList.intersect(lpntlist, true, false) ;
		}
	case dbGeoObject::Circle:
		{
			const dbGeoCircle * circle = dynamic_cast<const dbGeoCircle *> (&geo); // PRQA S 3081
			if (boxPntList.contains(circle->center())) {
				return true ;
            }
			//loop on box points
			//if at least one point is inside the circle then they intersect
            size_t pnts =  boxPntList.points() ;
			for(size_t i = 0; i< pnts; ++i)
			{
				// if the distance between the circle center and the point 
				// is less than the radius ->intersect
				if(boxPntList[i].distance(circle->center()) <= (circle->radius()))
				{
					return true;
				}
			}
			// if here check for intersection
			return  boxPntList.intersectCircle(circle->radius(),circle->center(), true);
		}
	case dbGeoObject::String:
		{
			const dbGeoString * s = dynamic_cast<const dbGeoString *> (&geo); // PRQA S 3081

			return boxPntList.containsAtLeastOne(s->pointList()) || boxPntList.intersect(s->pointList(), true, false) ;

		}
	case dbGeoObject::Ring:
		{
			const dbGeoRing * ring = dynamic_cast<const dbGeoRing *> (&geo); // PRQA S 3081
			const dbGeoPntList PntList2 = ring->ring(0);	


			//  first check if at least one point is inside the shape
			intersect = (boxPntList.containsAtLeastOne(PntList2) || PntList2.containsAtLeastOne(boxPntList));

			// if no points included, check for lines intersection
			if(!intersect) { intersect = boxPntList.intersect(PntList2, true, true); }

			return intersect;

		}
	case dbGeoObject::Polygon:
		{
			const dbGeoPolygon * pol = dynamic_cast<const dbGeoPolygon *> (&geo); // PRQA S 3081
			const dbGeoPntList PntList2 = pol->ring(0);	

			//  first check if at least one point is inside the shape
			intersect = (boxPntList.containsAtLeastOne(PntList2) || PntList2.containsAtLeastOne(boxPntList));

			// if no points included, check for lines intersection
			if(!intersect) { intersect = boxPntList.intersect(PntList2, true, true); }
			return intersect;
		}
	case dbGeoObject::Ellipse:
		{
			ACS_THROW(dbGeoObject::ExecuteException("method not implemented for Ellipse shape!  ")) ; // PRQA S 3081		
		}
	default:
		{
			ACS_THROW(dbGeoObject::ExecuteException("type not recognized")) ; // PRQA S 3081
		}
	}
	return intersect;
}


bool dbGeoBox::isInsideGeo(const dbGeoObject & geo) const
{
	// this method checks if the GeoBox  is geometrically inside the given dbGeoObject argument
	// timerange not considered
	
	bool inside = false;

	// get the pntList from the GeoBox
	dbGeoPntList boxPntList;
	createPntList(p1(),p2(), boxPntList);
	
	switch(geo.type())
	{
	case dbGeoObject::Point:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for Point shape!  ")) ; // PRQA S 3081		
		}
	case dbGeoObject::Box:
		{
			const dbGeoBox  * box = dynamic_cast<const dbGeoBox *> (&geo); // PRQA S 3081
			// Get the point list for box
			dbGeoPntList PntList2;
			createPntList(box->p1(),box->p2(), PntList2);
				
			inside = boxPntList.inside(PntList2);
		}
		break ;
	case dbGeoObject::Lineseg:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for Lineseg shape!  ")) ;	 // PRQA S 3081	
		}
	case dbGeoObject::Circle:
		{
			const dbGeoCircle * circle = dynamic_cast<const dbGeoCircle *> (&geo); // PRQA S 3081

			//loop on box points
			//and check that all points are inside the circle 
            size_t pnts = boxPntList.points() ;
			for(size_t i = 0; i< pnts; ++i) 
			{
				// if at least one is outside then return false
				if(boxPntList[i].distance(circle->center()) > (circle->radius()))
				{
					inside = false;
					break;
				}
			}
		    break ;
		}
	case dbGeoObject::String:
		{
			// unimplemented
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for String shape!  ")) ;	 // PRQA S 3081	
		}
	case dbGeoObject::Ring:
		{
			const dbGeoRing * ring = dynamic_cast<const dbGeoRing *> (&geo); // PRQA S 3081
			const dbGeoPntList PntList2 = ring->ring(0);	
			inside = boxPntList.inside(PntList2);
		    break ;
		}
	case dbGeoObject::Polygon:
		{
			const dbGeoPolygon * pol = dynamic_cast<const dbGeoPolygon *> (&geo); // PRQA S 3081
			const dbGeoPntList PntList2 = pol->ring(0);	
			inside = boxPntList.inside(PntList2);
		    break ;
		}
	case dbGeoObject::Ellipse:
		{
			// not implemented
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for ellipse shape!  ")) ; // PRQA S 3081		
		}

	default:
		{
			// unknown: exception
			ACS_THROW(dbGeoObject::ExecuteException ("type not recognized  ")) ; // PRQA S 3081
		}
	}
	return inside;
}

void dbGeoBox::exportSpecific(rsResourceSet &s) const
{
	rsPushNSpace nsp(s,"dbGeoBox") ;	
	exportBase(s) ;
}

void dbGeoBox::exportSpecificXML(XMLOstream &xst) const
{
	XMLOstream::Tag txml(xst,"dbGeoBox") ;	
	exportBaseXML(xst) ;
}

void dbGeoBox::importSpecific(rsResourceSet &s)
{
	rsPushNSpace nsp(s,"dbGeoBox") ;
	importBase(s) ;
}




void dbGeoBox::parseOracle(const string &is, int offs, int etype, int interpr) 
{
/* MDSYS.SDO_GEOMETRY(2003,null,null,MDSYS.SDO_ELEM_INFO_ARRAY(1,1003,3),MDSYS.SDO_ORDINATE_ARRAY(34.25,6.4096,43.752,26.5419))*/

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "is: \"" << is << "\"") ;
	ACS_COND_THROW ((etype != 1003) || (interpr != 3), // PRQA S 3081, 3380, 4400
			UnrecognizedTypeException("Invalid etype/iterpr pair: "+ ((is.length() > 64) ? is.substr(0,64) : is) + " ...\"")) ;
	
	
	RegExp recoord(".*SDO_ORDINATE_ARRAY[ \t]*[(]") ;
	regoff_t coordbeg ; // PRQA S 4101 2
    regoff_t coordend ;
	if (!recoord.match(is,coordbeg,coordend)) {
		ACS_THROW(UnrecognizedTypeException("Unrecognized \"" + ((is.length() > 64) ? is.substr(0,64) : is) + " ...\"")) ; // PRQA S 3081, 3380
	}
	istringstream coords(is.substr(coordend)) ; // PRQA S 3000
	coords.exceptions(ios_base::failbit) ;

	dbGeoPnt pnt1 ; 
    dbGeoPnt pnt2 ;
	char comma ; // PRQA S 4101 2
    char ell ;
	dbGeoPnt::parseOracle(pnt1, coords) ;
	coords >> comma ;
	dbGeoPnt::parseOracle(pnt2, coords) ;
	coords >> ell >> ell ;
	p1(pnt1) ;
	p2(pnt2) ;
	dbGeoAltitudeRange ar ;
	dbGeoTimeRange tr ;
	altitudeRange(ar) ;
	timeRange(tr) ;
}

std::string dbGeoBox::getWKTString(bool lonFirst) const {
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

std::istream &
operator >> (std::istream &is, dbGeoBox& b) // PRQA S 4020
{
/* GeoBox((10.1, 20.2), (30.3, 40.4), ANY, ANY)  */
	if (!is.good()) { return is ; } // PRQA S 4028
	std::string oname ;
	if (!dbGeoObject::extractObjType(is,oname) || oname != "GeoBox(") {
		is.setstate(std::ios::failbit) ;
		return is ;	 // PRQA S 4028
	}
	dbGeoPnt p1 ; 
    dbGeoPnt p2 ;
	char comma ; // PRQA S 4101 2 
    char ell ;
	dbGeoAltitudeRange ar ;
	dbGeoTimeRange tr ;
	is >> p1 >> comma >> p2 >> comma >> ar >> comma >> tr >> ell ;

	if (!is.fail()) {
		b.p1(p1) ;
		b.p2(p2) ;
		b.altitudeRange(ar) ;
		b.timeRange(tr) ;
	}
	return is ; // PRQA S 4028
}


std::ostream & operator << (std::ostream &os, const dbGeoBox& b) 
{
   return b.getSqlString(os, "Postgres8") ;
}

exostream & operator << (exostream &os, const dbGeoBox& b) // PRQA S 4222
{
    ostringstream s ;
    b.getSqlString(s, "Postgres8") ;
    return os << s.str() ;
}

_ACS_END_NAMESPACE
