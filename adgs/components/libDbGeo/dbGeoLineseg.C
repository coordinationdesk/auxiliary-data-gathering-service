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
	
	Revision 5.0  2013/07/11 16:07:11  marpas
	libException 5.x standards adopted
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.15  2013/05/17 16:32:25  marpas
	stream insertion operators added
	
	Revision 2.14  2013/05/14 18:00:34  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.13  2012/02/13 11:15:39  marpas
	refactoring in progress
	
	Revision 2.12  2011/09/29 14:48:09  marpas
	simplifying templates for codecheck
	
	Revision 2.11  2010/05/19 16:06:06  marpas
	Oracle spatial/locator parse completed
	
	Revision 2.10  2010/05/12 13:22:21  marpas
	setDialectSRID and getDialectSRID static methods added to dbGeoObejct class
	getSQLString with dialect set to Oracle11 use the SRID passed to the above methods
	
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
	
	Revision 1.6  2004/06/16 09:44:09  marpas
	measurement methods added
	
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


#include <dbGeoLineseg.h>
#include <dbGeoPnt.h>
#include <dbGeoBox.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <XMLOstream.h>
#include <RegExp.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/  dbGeoLineseg  _/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbGeoLineseg)

dbGeoLineseg::dbGeoLineseg() : dbGeoObject() 
{
}


dbGeoLineseg::dbGeoLineseg(const dbGeoPnt &p1, const dbGeoPnt &p2) : dbGeoObject() 
{
	if (p1 == p2) { // null length line
		ACS_THROW(dbGeoObject::ExecuteException("cannot build a segment with null length")) ; // PRQA S 3081
	} 
	dbGeoObject::append(p1) ;
	dbGeoObject::append(p2) ;
}

dbGeoLineseg::~dbGeoLineseg() throw() { }



dbGeoLineseg::dbGeoLineseg(const dbGeoLineseg &r) : dbGeoObject(r) 
{	
}


dbGeoLineseg &dbGeoLineseg::operator=(const dbGeoLineseg &l)
{
	if(this != &l) {
		dbGeoObject::operator=(l) ;
	}

    return *this ;
}




void dbGeoLineseg::p1(const dbGeoPnt &pnt)
{

	if(0 != points()) {
		(*this)[0] = pnt ;
    }
	else {
		append(pnt) ;
    }
}

dbGeoPnt &dbGeoLineseg::p1()  
{
	while (points() < 1) { // the first point[0] could not have yet been inserted 
		append(dbGeoPnt(0,0)) ;
    }
	return (*this)[0] ;
}

const dbGeoPnt &dbGeoLineseg::p1() const
{
	return (*this)[0] ;
}



void dbGeoLineseg::p2(const dbGeoPnt &pnt)
{

	if(points() > 1) {
		(*this)[1] = pnt ;
    }
	else {
		while (points() < 2) { // the first point[0] could not have yet been inserted // PRQA S 4400
			append(pnt) ;
        }
    }
}

dbGeoPnt &dbGeoLineseg::p2()
{
	while (points() < 2) { // the first point[0] could not have yet been inserted // PRQA S 4400
		append(dbGeoPnt(0,0)) ;
    }
	return (*this)[1] ;
}

const dbGeoPnt &dbGeoLineseg::p2() const
{
	return (*this)[1] ;
}

dbGeoObject::Type dbGeoLineseg::type() const 
{
	return Lineseg ;
}


dbGeoObject* dbGeoLineseg::clone() const 
{
	return new dbGeoLineseg(*this) ;
}


bool dbGeoLineseg::intersectGeo(const dbGeoObject  & geo ) const // PRQA S 4020
{
	// this method checks if the GeoBox  is geometrically inside the given dbGeoObject argument
	// timerange not considered
	bool intersect = false;

	switch(geo.type())
	{
	case dbGeoObject::Point:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for Point shape!  ")) ; // PRQA S 3081		
		}
	case dbGeoObject::Box:
		{
			// with a box
			const dbGeoBox  * box = dynamic_cast<const dbGeoBox *> (&geo); // PRQA S 3081
			return box->intersectGeo(*this);
		}
	case dbGeoObject::Lineseg:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for LineSeg shape!  ")) ;	 // PRQA S 3081	
		}
	case dbGeoObject::Circle:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for String shape!  ")) ; // PRQA S 3081		
		}
	case dbGeoObject::String:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for String shape!  ")) ; // PRQA S 3081		

		}
	case dbGeoObject::Ring:
		{

			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for String shape!  ")) ; // PRQA S 3081		
		}
	case dbGeoObject::Polygon:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for String shape!  ")) ; // PRQA S 3081		
		}
	case dbGeoObject::Ellipse:
		{
			ACS_THROW(dbGeoObject::ExecuteException ("method not implemented for Ellipse shape!  ")) ; // PRQA S 3081		
		}

	default:
		{
			ACS_THROW(dbGeoObject::ExecuteException("type not recognized  ")) ; // PRQA S 3081		
		}
	}
	
	// defaulted to false
	return intersect;
}


bool dbGeoLineseg::isInsideGeo(const dbGeoObject &) const
{
	ACS_THROW(dbGeoObject::ExecuteException ("method not implemented")) ; // PRQA S 3081		
}





ostream &dbGeoLineseg::getSqlString(ostream &os, const string &dialectName) const // PRQA S 4020
{
	if (dialectName == "Informix9") {
	// 'GeoLineseg((val,val), (val,val),ANY,ANY)'
		os << "GeoLineseg(" ;
        size_t np =  points() ;
		for(size_t i = 0 ; i < np ; ++i) { // cannot use dbGeoPntList::getSqlString(...)
			(*this)[i].getSqlString(os, dialectName, precision()) << ", " ;
        }
		_altitudeRange.getSqlString(os) << ", " ;
		_timeRange.getSqlString(os) << ")" ;

		return os ; // PRQA S 4028
	}
    else if (dialectName == "Postgres8") {
		os << "LINESTRING(" ;
        size_t np =  points() ;
		for(size_t i = 0 ; i < np ; ++i) { // cannot use dbGeoPntList::getSqlString(...)
			if (i > 0) { os << "," ; }
            (*this)[i].getSqlString(os, dialectName, precision()) ;
        }
		os << ")" ;
        return os ; // PRQA S 4028
    }
	else if (dialectName == "Oracle11") {
	 // sdo_geometry (2001, null, null, sdo_elem_info_array (1,1,1), sdo_ordinate_array (10,5))
	 // MDSYS.SDO_GEOMETRY(2001,NULL,MDSYS.SDO_POINT_TYPE(12, 14, NULL),NULL,NULL)
		os << "MDSYS.SDO_GEOMETRY("
		   << "2002," << getDialectSRID(dialectName) << ",null,MDSYS.SDO_ELEM_INFO_ARRAY(1,2,1),"
		   << "MDSYS.SDO_ORDINATE_ARRAY("
		   << setprecision(precision()) ;
		_points.getSqlString(os, dialectName, precision()) ; // val,val,...),
		os << "))" ;

		return os ; // PRQA S 4028
	}
    else {
	    ACS_THROW (ExecuteException("Dialect unimplemented: "+ dialectName)) ; // PRQA S 3081
    }
}

std::string dbGeoLineseg::getWKTString(bool lonFirst) const {
	std::ostringstream ss;
	
	ss << std::setprecision(dbGeoObject::wktPrecision) << type2WKT(type()) << "(";
	
	size_t pointsnum = _points.points();
	for (size_t p = 0; p < pointsnum; ++p) {
		if (lonFirst) {
			ss << _points[p].lon() << " " <<  _points[p].lat();
		} else {
			ss << _points[p].lat() << " " <<  _points[p].lon();		
		}
		
		if (p < pointsnum - 1) {
			ss << ", ";
		}
	}
	
	ss << ")";
	
	return ss.str();
}

void dbGeoLineseg::bbox(dbGeoBox &bb, double latmargin, double lonmargin) const 
{
	_points.bbox(bb,latmargin,lonmargin) ;
    const TimeRange &tr = timeRange() ;
    if (tr.start().isFilled() && tr.stop().isFilled()) {
	    bb.timeRange(timeRange().wholeDay()) ;
    }
}


void dbGeoLineseg::exportSpecific(rsResourceSet &s) const
{
	rsPushNSpace nsp(s,"dbGeoLineseg") ;
	exportBase(s) ;
}

void dbGeoLineseg::exportSpecificXML(XMLOstream &xst) const
{
	XMLOstream::Tag txml(xst,"dbGeoLineseg") ;
	exportBaseXML(xst) ;
}

void dbGeoLineseg::importSpecific(rsResourceSet &s)
{
	rsPushNSpace nsp(s,"dbGeoLineseg") ;
	importBase(s) ;
}

string dbGeoLineseg::properties() const 
{
	ostringstream os ;
	os << "Length: " << fixed << setprecision(1) << distanceKm(p1(),p2()) << " [Km]" ; 
	return os.str() ;
}

void dbGeoLineseg::parseOracle(const string &is, int offs, int etype, int interpr) 
{
/* MDSYS.SDO_GEOMETRY(2003,8307,null,MDSYS.SDO_ELEM_INFO_ARRAY(1,1003,1),MDSYS.SDO_ORDINATE_ARRAY(10,10, 20,20))*/

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "is: \"" << is << "\"") ;
	ACS_COND_THROW ((etype != 2) || (interpr != 1), // PRQA S 3081, 3380
			UnrecognizedTypeException("Invalid etype/iterpr pair: "+ ((is.length() > 64) ? is.substr(0,64) : is) + " ...\"")) ; // PRQA S 3081
	
	
	RegExp recoord(".*SDO_ORDINATE_ARRAY[ \t]*[(]") ;
	regoff_t coordbeg ; // PRQA S 4101 2 
    regoff_t coordend ;
	if (!recoord.match(is,coordbeg,coordend)) {
		ACS_THROW(UnrecognizedTypeException("Unrecognized \"" + ((is.length() > 64) ? is.substr(0,64) : is) + " ...\"")) ;	// PRQA S 3081, 3380
	}
	istringstream coords(is.substr(coordend)) ; // PRQA S 3000
	coords.exceptions(ios_base::failbit) ;

	dbGeoPnt pnt1 ;
    dbGeoPnt pnt2 ;
	char comma ;  // PRQA S 4101 2 
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


std::istream &
operator >> (std::istream &is, dbGeoLineseg& l) // PRQA S 4020
{
/* GeoLineseg((1.234, 5.678), (9.876, 5.432), ANY, ANY)  */
/* LINESTRING ( 10.1  20.2 , 30.3 40.4 ) */
	if (!is.good()) { return is ; } // PRQA S 4028
    
	std::string oname ;
    bool informix = true ;
	if (!dbGeoObject::extractObjType(is,oname)) {
		is.setstate(std::ios::failbit) ;
		return is ;	 // PRQA S 4028
	}
    if (oname == "GeoLineseg(") {
        informix = true ;
    }
    else if (oname == "LINESTRING(") {
        informix = false ;
    }
    else {
		is.setstate(std::ios::failbit) ;
		return is ;	 // PRQA S 4028
    }

	dbGeoPnt p1 ;
    dbGeoPnt p2 ;
    if (informix) {
        char comma ;  // PRQA S 4101 2 
        char ell ;
	    dbGeoAltitudeRange ar ;
	    dbGeoTimeRange tr ;
	    is >> p1 >> comma >> p2 >> comma >> ar >> comma >> tr >> ell ;

	    if (!is.fail()) {
		    l.p1(p1) ;
		    l.p2(p2) ;
		    l.altitudeRange(ar) ;
		    l.timeRange(tr) ;
	    }
    }
    else {
        char comma ;  // PRQA S 4101 2 
        char ell ;
        int pr1 = 0 ;
        dbGeoPnt::parsePostgres(p1, is, pr1) ; 
        is >> comma ;
        int pr2 = 0 ;
        dbGeoPnt::parsePostgres(p2, is, pr2) ; 
        is >> ell ;
		if (!is.fail()) {
            l.p1(p1) ;
		    l.p2(p2) ;
            l.precision(max(pr1,pr2)) ;
        }
    }

	return is ; // PRQA S 4028
}

std::ostream & operator << (std::ostream &os, const dbGeoLineseg& l) 
{
   return l.getSqlString(os, "Postgres8") ;
}

exostream & operator << (exostream &os, const dbGeoLineseg& l) // PRQA S 4222
{
    ostringstream s ;
    l.getSqlString(s, "Postgres8") ;
    return os << s.str() ;
}

_ACS_END_NAMESPACE
