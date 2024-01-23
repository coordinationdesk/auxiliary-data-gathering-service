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
	
	Revision 2.11  2013/05/14 18:00:34  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.10  2012/02/13 11:15:39  marpas
	refactoring in progress
	
	Revision 2.9  2011/09/29 14:48:09  marpas
	simplifying templates for codecheck
	
	Revision 2.8  2010/05/04 15:59:20  marpas
	Postgres8 behaviour like Informix9
	
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
	
	Revision 1.8  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.7  2004/12/30 21:07:35  marpas
	header fixed
	
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


#include <dbGeoEllipse.h>
#include <dbGeoPnt.h>
#include <iomanip>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <XMLOstream.h>
#include <sstream>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/ dbGeoEllipse   _/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbGeoEllipse)

dbGeoEllipse::dbGeoEllipse() :
    dbGeoObject(),
	_major(0),
	_minor(0),
	_azimuth(0)
{
}


dbGeoEllipse::dbGeoEllipse(const dbGeoPnt &c, double ma, double mi, double az) :
    dbGeoObject(),
	_major(ma),
	_minor(mi),
	_azimuth(az)
{
	dbGeoObject::append(c) ;
}


dbGeoEllipse::~dbGeoEllipse() throw() { }



dbGeoEllipse::dbGeoEllipse(const dbGeoEllipse &e) : 
    dbGeoObject(e),
	_major(e._major),
	_minor(e._minor),
	_azimuth(e._azimuth)
{
    // empty 
}



dbGeoEllipse &dbGeoEllipse::operator=(const dbGeoEllipse &e)
{
	if(this != &e) {
		dbGeoObject::operator=(e) ; // copy the base
		_major = e._major ; // .. and the pvt data
		_minor = e._minor ;
		_azimuth = e._azimuth ;
	}
    return *this ;
}





void dbGeoEllipse::center(const dbGeoPnt &p)
{

	if(0 != points()) {
		operator[](0) = p ;
    }
	else {
		append(p) ;
    }
}



const dbGeoPnt &dbGeoEllipse::center() const 
{
	return (*this)[0] ;
}


dbGeoObject::Type dbGeoEllipse::type() const 
{
	return Ellipse ;
}


dbGeoObject* dbGeoEllipse::clone() const 
{
	return new dbGeoEllipse(*this) ;
}



dbGeoPnt &dbGeoEllipse::center() 
{
	while (points() < 1) { // the centre could not have yet been inserted 
		append(dbGeoPnt(0,0)) ;
    }
	return (*this)[0] ;
}

void dbGeoEllipse::majorAxis(double r) // PRQA S 4121
{
	_major = r ;
}

void dbGeoEllipse::minorAxis(double r) // PRQA S 4121
{
	_minor = r ;
}


void dbGeoEllipse::azimuth(double r) // PRQA S 4121
{
	_azimuth = r ;
}

ostream &dbGeoEllipse::getSqlString(ostream &os, const string &dialectName) const 
{
	if (dialectName == "Informix9") {
	// 'GeoEllipse((val,val), _major,_minor,_azimuth,ANY,ANY)'
		os << "GeoEllipse(" ;
		(*this)[0].getSqlString(os, dialectName, precision()) << ", " ;
    	os << setprecision(precision()) << _major << ", " <<  _minor << ", " << _azimuth << ", " ; 
		_altitudeRange.getSqlString(os) << ", " ;
		_timeRange.getSqlString(os) << ")" ;

		return os ; // PRQA S 4028
	}
	else if (dialectName == "Oracle11") {
		ACS_THROW (ExecuteException("Oracle does not support geodetic arcs or circles")) ; // PRQA S 3081
	}
	else if (dialectName == "Postgres8") {
		ACS_THROW (ExecuteException("Postgres does not support geodetic arcs or circles")) ; // PRQA S 3081
	}
    else {
    	ACS_THROW (ExecuteException("Dialect unimplemented: "+ dialectName)) ; // PRQA S 3081
    }
}

std::string dbGeoEllipse::getWKTString(bool lonFirst) const {
	ACS_THROW(exNotImplemented("Well-Known-Text representation for type Ellipse is not supported."));
	return "";
}


void dbGeoEllipse::bbox(dbGeoBox &bb, double latmargin, double lonmargin) const 
{
	ACS_THROW(ExecuteException("dbGeoEllipse::bbox not implemented")) ; // PRQA S 3081
}

void dbGeoEllipse::exportSpecific(rsResourceSet &s) const
{
	rsPushNSpace nsp(s,"dbGeoEllipse") ;
	
	s.setValue("_major",_major) ; // export major axis
	s.setValue("_minor",_minor) ; // export minor axis ..
	s.setValue("_azimuth",_azimuth) ; // .. and the azimuth angle
	exportBase(s) ;
}

void dbGeoEllipse::exportSpecificXML(XMLOstream &xst) const
{
	XMLOstream::Tag txml(xst,"dbGeoEllipse") ;

	{ // major axis
		ostringstream os;
		os << setprecision(17) << _major ; // PRQA S 4400
		XMLOstream::Tag txml2(xst,"_major", os.str()) ;
	}

	{ // minor axis
		ostringstream os;
		os << setprecision(17) << _minor ; // PRQA S 4400
		XMLOstream::Tag txml2(xst,"_minor", os.str()) ;
	}

	{ // azimuth angle
		ostringstream os;
		os << setprecision(17) << _azimuth ; // PRQA S 4400
		XMLOstream::Tag txml2(xst,"_azimuth", os.str()) ;
	}

	exportBaseXML(xst) ;
}

void dbGeoEllipse::importSpecific(rsResourceSet &s)
{
	rsPushNSpace nsp(s,"dbGeoEllipse") ;
	s.getValue("_major",_major) ; // import the major axis
	s.getValue("_minor",_minor) ; // the minor one 
	s.getValue("_azimuth",_azimuth) ; // and the azimuth
	importBase(s) ;
}

bool dbGeoEllipse::intersectGeo(const dbGeoObject  & geo) const
{
	ACS_THROW(dbGeoObject::ExecuteException ("method not implemented !")) ; // PRQA S 3081		

}

bool dbGeoEllipse::isInsideGeo(const dbGeoObject &) const 
{
	ACS_THROW(dbGeoObject::ExecuteException ("method not implemented !")) ;	 // PRQA S 3081	
}


double dbGeoEllipse::majorAxis() const { return _major ; } // PRQA S 4120 3
double dbGeoEllipse::minorAxis() const { return _minor ; }
double dbGeoEllipse::azimuth() const { return _azimuth ; }



std::istream  &
operator >> (std::istream &is, dbGeoEllipse& e) // PRQA S 4020
{
/* GeoEllipse((lat, long), semi_maj, semi_min, az, altrange, timerange) */
	if (!is.good()) { return is ; } // PRQA S 4028
	std::string oname ;
	if (!dbGeoObject::extractObjType(is,oname) || oname != "GeoEllipse(") {
		is.setstate(std::ios::failbit) ;
		return is ;	 // PRQA S 4028
	}
	dbGeoPnt cen ;
	double M ; // PRQA S 4101 5
    double m ;
    double az ;
	char comma; 
    char ell ;
	dbGeoAltitudeRange ar ;
	dbGeoTimeRange tr ;
	is >> cen >> comma >> M >> comma >> m >> comma >> az >> comma >> ar >> comma >> tr >> ell ;

	if (!is.fail()) {
		e.center(cen) ;
		e.majorAxis(M) ;
		e.minorAxis(m) ;
		e.azimuth(az) ;
		e.altitudeRange(ar) ;
		e.timeRange(tr) ;
	}
	return is ; // PRQA S 4028
}


_ACS_END_NAMESPACE
