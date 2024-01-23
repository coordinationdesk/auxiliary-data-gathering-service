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
	Revision 5.4  2014/05/28 09:50:02  marpas
	fixed coordinate inversion regression, getSqlString out format
	
	Revision 5.3  2014/05/27 14:28:57  marpas
	regression in reading negative coordinates fixed
	generation of numeric strings fixed
	
	Revision 5.2  2014/05/09 11:39:54  marpas
	coding best practices applied
	qa & c++ warnings fixed
	last throwing macros for exceptions adopted
	
	Revision 5.1  2014/05/06 12:25:58  marpas
	precision in getting sql string managed
	dbGeoObject interface has now methods to set/get precision and a defaultPrecision static constant
	precision must be greater than 2, note that precision will affect lenght of string that will be used in queries, limiting the maximum number of points as the query string has limited size
	
	Revision 5.0  2013/07/11 16:07:11  marpas
	libException 5.x standards adopted
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.13  2013/05/14 18:00:34  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.12  2012/02/13 11:15:39  marpas
	refactoring in progress
	
	Revision 2.11  2011/09/29 14:48:09  marpas
	simplifying templates for codecheck
	
	Revision 2.10  2010/05/04 15:59:20  marpas
	Postgres8 behaviour like Informix9
	
	Revision 2.9  2010/04/30 16:23:05  marpas
	ORACLE spatial/locator support in progress ..
	
	Revision 2.8  2010/04/26 11:40:20  marpas
	work in progress Oracle11 Spatial and Locator support implementation
	
	Revision 2.7  2010/04/09 17:23:21  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.6  2009/06/26 09:48:17  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.5  2009/06/24 09:27:53  marpas
	ntersections still in progress
	
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
	
	Revision 1.9  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.8  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.7  2004/06/16 09:44:09  marpas
	measurement methods added
	
	Revision 1.6  2003/09/03 13:57:43  paoscu
	Export methods are now const.
	
	Revision 1.5  2003/04/30 13:07:28  marpas
	using namespace std no longer in includes
	
	Revision 1.4  2003/04/15 18:35:04  marpas
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
#include <dbGeoCircle.h>
#include <dbGeoEllipse.h>
#include <dbGeoRing.h>
#include <dbGeoPolygon.h>
#include <dbGeoPoint.h>
#include <iomanip>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <XMLOstream.h>
#include <sstream>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/  dbGeoCircle _/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbGeoCircle)

dbGeoCircle::dbGeoCircle() :
    dbGeoObject(),
	_radius(0)
{
}


dbGeoCircle::dbGeoCircle(const dbGeoPnt &c, double r) :
    dbGeoObject(),
	_radius(r)
{
	if (r == 0) { // not allowed with radius 0 // PRQA S 3270
		ACS_THROW(dbGeoObject::ExecuteException("cannot build a circle with radius equal to 0")) ; // PRQA S 3081
	}
	
	dbGeoObject::append(c) ; // append the centre to base class points
}



dbGeoCircle::~dbGeoCircle() throw() { }





dbGeoCircle::dbGeoCircle(const dbGeoCircle	&c) :
	dbGeoObject(c),
	_radius(c._radius)

{ 
    // empty
}




dbGeoCircle &dbGeoCircle::operator=(const dbGeoCircle &c)
{
	if(this != &c) { // check a = a 
		dbGeoObject::operator=(c) ; // set base data
		_radius = c._radius ; // and the radius
	}
    return *this ;
}



dbGeoObject::Type dbGeoCircle::type() const 
{
	return Circle ;
}



dbGeoObject* dbGeoCircle::clone() const 
{
	return new dbGeoCircle(*this) ;
}





void dbGeoCircle::center(const dbGeoPnt &p)
{

	if(0 != points()) {
		operator[](0) = p ;
    }
	else {
		append(p) ;
    }
}



const dbGeoPnt &dbGeoCircle::center() const 
{
	return (*this)[0] ;
}

dbGeoPnt &dbGeoCircle::center()  
{
	while (points() < 1) { // the centre could not have yet been inserted 
		append(dbGeoPnt(0,0)) ;
    }
	return (*this)[0] ;
}

void dbGeoCircle::radius(double v) // PRQA S 4121
{
	// MARCO check se il raggio e' > di 2*pi*earthRadius
	_radius = v ;
}

ostream &dbGeoCircle::getSqlString(ostream &os, const string &dialectName) const 
{
	if (dialectName == "Informix9") {
	// 'GeoCircle((val,val),radius,ANY,ANY)'
		os << "GeoCircle(" ;
		(*this)[0].getSqlString(os, dialectName, precision()) << ", " ;
    	os << setprecision(precision()) << _radius << ", " ; 
		_altitudeRange.getSqlString(os) << ", " ;
		_timeRange.getSqlString(os) << ")" ;

		return os ; // PRQA S 4028
	}
	else if (dialectName == "Oracle11") {
		ACS_THROW (ExecuteException("Oracle does not support geodetic arcs or circles")) ; // PRQA S 3081
	}
	else if (dialectName == "Postgres8") {
		ACS_THROW (ExecuteException("Postgres (geographic) does not support geodetic arcs or circles")) ; // PRQA S 3081
	}
    else {
	    ACS_THROW (ExecuteException("Dialect unimplemented: "+ dialectName)) ; // PRQA S 3081
    }
}

std::string dbGeoCircle::getWKTString(bool lonFirst) const {
	ACS_THROW(exNotImplemented("Well-Known-Text representation for type Circle is not supported."));
	return "";
}


void dbGeoCircle::bbox(dbGeoBox &bb, double latmargin, double lonmargin) const 
{
	ACS_THROW(ExecuteException("not implemented")) ; // PRQA S 3081
}

void dbGeoCircle::exportSpecific(rsResourceSet &s) const
{
	rsPushNSpace nsp(s,"dbGeoCircle") ;
	
	s.setValue("_radius",_radius) ;
	exportBase(s) ;
}

void dbGeoCircle::exportSpecificXML(XMLOstream &xst) const
{
	XMLOstream::Tag txml(xst,"dbGeoCircle") ;

	{
		ostringstream os;
			
		os << fixed << setprecision(17) << _radius ; // the radius with max precision allowed // PRQA S 4400
	
		XMLOstream::Tag txml2(xst,"_radius", os.str()) ; // the radius tag
	}

	exportBaseXML(xst) ;
}

void dbGeoCircle::importSpecific(rsResourceSet &s)
{
	rsPushNSpace nsp(s,"dbGeoCircle") ;
	s.getValue("_radius",_radius) ;
	importBase(s) ;
}

string dbGeoCircle::properties() const 
{
	ostringstream os ;
	os << "Radius: " << fixed << setprecision(1) << _radius/1000 << " [Km]" ;  // PRQA S 4400
	return os.str() ;
}

bool dbGeoCircle::intersectGeo(const dbGeoObject  & geo) const // PRQA S 4020
{
	// this method checks if the GeoCircle  is geometrically inside the given dbGeoObject argument
	// timerange not considered
	bool intersect = false;
	
	switch(geo.type())
	{
	case dbGeoObject::Point:
		{
			const dbGeoPoint * pnt = dynamic_cast<const dbGeoPoint *> (&geo); // PRQA S 3081
			return pnt->intersectGeo(*this);
		}
	case dbGeoObject::Box:
		{
			// check for spatial intersection
			const dbGeoBox  * box = dynamic_cast<const dbGeoBox *> (&geo); // PRQA S 3081
			return box->intersectGeo(*this);
		}
	case dbGeoObject::Lineseg:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for Lineseg shape")) ;	 // PRQA S 3081	
		}
	case dbGeoObject::Circle:
		{
			const dbGeoCircle * circle = dynamic_cast<const dbGeoCircle *> (&geo); // PRQA S 3081

			// if the distance between the two circle center 
			// is less than equal the two radius sum->intersect
			return (center().distance(circle->center()) <= ((circle->radius() + radius())));
		}
	case dbGeoObject::String:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for String shape")) ; // PRQA S 3081		
		}
	case dbGeoObject::Ring:
		{
			const dbGeoRing * ring = dynamic_cast<const dbGeoRing *> (&geo); // PRQA S 3081
			const dbGeoPntList PntList = ring->ring(0);	
			// if circle center is inside ring -> intersects
			if (PntList.contains(center())) {
                return true ;
            }
			//loop on ring points
			//if at least one point is inside the circle then they intersect 
			for(size_t i = 0; i< PntList.points(); i++)
			{
				// if the distance between the circle center and the point 
				// is less than the radius ->intersect
				if(PntList[i].distance(center()) <= (radius()))
				{
					return true;
				}
			}
			// if here check for intersection
			return  PntList.intersectCircle(radius(),center(), true);
		}
	case dbGeoObject::Polygon:
		{
			const dbGeoPolygon * pol = dynamic_cast<const dbGeoPolygon *> (&geo); // PRQA S 3081
			const dbGeoPntList PntList = pol->ring(0);	
			// if circle center is inside ring -> intersects
			if (PntList.contains(center())) {
                return true ;
            }
			//loop on ring points
			//if at least one point is inside the circle then they intersect 
			for(size_t i = 0; i< PntList.points(); i++)
			{
				if(PntList[i].distance(center()) <= (radius()))
				{
					return true;
				}
			}
			// if here check for intersection
			return  PntList.intersectCircle(radius(),center(), true); // PRQA S 3081
		}
	case dbGeoObject::Ellipse:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for Ellipse shape")) ; // PRQA S 3081		
		}

	default:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("type not recognized  ")) ; // PRQA S 3081		
		}
	}
	return intersect;
}
bool dbGeoCircle::isInsideGeo(const dbGeoObject & geo) const
{
	// this method checks if the GeoBox  is geometrically inside the given dbGeoObject argument
	// timerange not considered
	ACS_THROW(dbGeoObject::ExecuteException ("method not implemented ")) ;	// PRQA S 3081
}


double dbGeoCircle::radius() const { return _radius ; } // PRQA S 4120


istream &
operator >> (istream &is, dbGeoCircle& c) // PRQA S 4020
{
/* GeoCircle((12.3, 45.6), 12000, ANY, ANY) */
	if (!is.good()) { return is ; } // PRQA S 4028
	std::string oname ;
	if (!dbGeoObject::extractObjType(is,oname) || oname != "GeoCircle(") {
		is.setstate(std::ios::failbit) ;
		return is ;	 // PRQA S 4028
	}
	dbGeoPnt cen ;
	double r ; // PRQA S 4101 3
	char comma ; 
    char ell ;
	dbGeoAltitudeRange ar ;
	dbGeoTimeRange tr ;
	is >> cen >> comma >> r >> comma >> ar >> comma >> tr >> ell ;

	if (!is.fail()) {
		c.center(cen) ;
		c.radius(r) ;
		c.altitudeRange(ar) ;
		c.timeRange(tr) ;
	}
	return is ; // PRQA S 4028
}

_ACS_END_NAMESPACE
