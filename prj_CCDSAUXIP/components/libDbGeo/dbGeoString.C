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
	
	Revision 2.16  2013/05/17 16:32:25  marpas
	stream insertion operators added
	
	Revision 2.15  2013/05/14 18:00:35  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.14  2012/12/14 11:13:41  marpas
	coding best practices
	qa rules
	test improeved
	test coverage
	
	Revision 2.13  2012/02/13 11:15:40  marpas
	refactoring in progress
	
	Revision 2.12  2011/09/29 14:48:10  marpas
	simplifying templates for codecheck
	
	Revision 2.11  2010/05/19 16:06:07  marpas
	Oracle spatial/locator parse completed
	
	Revision 2.10  2010/05/12 13:22:22  marpas
	setDialectSRID and getDialectSRID static methods added to dbGeoObejct class
	getSQLString with dialect set to Oracle11 use the SRID passed to the above methods
	
	Revision 2.9  2010/05/04 15:59:20  marpas
	Postgres8 behaviour like Informix9
	
	Revision 2.8  2010/04/30 16:23:05  marpas
	ORACLE spatial/locator support in progress ..
	
	Revision 2.7  2010/04/26 11:40:20  marpas
	work in progress Oracle11 Spatial and Locator support implementation
	
	Revision 2.6  2010/04/09 17:23:22  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.5  2009/06/26 09:48:18  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.4  2009/06/24 09:27:54  marpas
	ntersections still in progress
	
	Revision 2.3  2009/06/23 13:29:43  marpas
	isInsideGeo and intersectGeo method can be invoked on CONST objects
	
	Revision 2.2  2009/06/05 09:31:18  crivig
	 isInsideGeo and intersectGeo methods added
	
	Revision 2.1  2007/06/09 18:45:54  marpas
	bbox method added
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.11  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.10  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.9  2004/06/16 09:44:09  marpas
	measurement methods added
	
	Revision 1.8  2003/09/03 13:57:43  paoscu
	Export methods are now const.
	
	Revision 1.7  2003/07/11 13:06:06  marpas
	exException::clone implemented
	
	Revision 1.6  2003/07/07 15:10:28  marpas
	sorry ...
	
	Revision 1.5  2003/07/07 15:05:25  marpas
	consistency check improved
	
	Revision 1.4  2003/04/30 13:07:28  marpas
	using namespace std no longer in includes
	
	Revision 1.3  2003/03/17 16:42:48  marpas
	enumerant type added
	
	Revision 1.2  2002/11/29 17:48:46  danalt
	add exportXML, corrected some bugs
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/


#include <dbGeoString.h>
#include <dbGeoPnt.h>
#include <dbGeoBox.h>

#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <XMLOstream.h>
#include <RegExp.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/  dbGeoString   _/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbGeoString)

dbGeoString::dbGeoString() : dbGeoObject()
{
}

dbGeoString::dbGeoString(const dbGeoPntList &pl) : 
	dbGeoObject(pl) 
{
}


dbGeoString::~dbGeoString() throw() {}



dbGeoObject::Type dbGeoString::type() const 
{
	return String ;
}



dbGeoObject* dbGeoString::clone() const 
{
	return new dbGeoString(*this) ;
}


dbGeoString::dbGeoString(const dbGeoString	&r) : dbGeoObject(r) 
{	
}


dbGeoString &dbGeoString::operator=(const dbGeoString &gs)
{
	if(this != &gs) {
		dbGeoObject::operator=(gs) ;
	}
    return *this ;
}


const dbGeoPnt &dbGeoString::operator[](size_t i) const 
{
	return dbGeoObject::operator[](i) ;
}

dbGeoPnt &dbGeoString::operator[](size_t i) 
{
	return dbGeoObject::operator[](i) ;
}

const dbGeoPntList &dbGeoString::pointList() const 
{
	return dbGeoObject::pointList() ;
}

std::string dbGeoString::getWKTString(bool lonFirst) const {
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


ostream &dbGeoString::getSqlString(ostream &os, const string &dialectName) const // PRQA S 4020 
{
	ACS_COND_THROW(_points.points() < 2, // PRQA S 3081
		GeoStringException(string("Too few points for a geo-string"))) ;
	if (dialectName == "Informix9") {
	// 'GeoString(((val,val), .... ),ANY,ANY)'
		os << "GeoString(" ;
		_points.getSqlString(os, dialectName, precision()) << ", "; // ((val,val),...),
		_altitudeRange.getSqlString(os) << ", " ;
		_timeRange.getSqlString(os) << ")" ;

		return os ; // PRQA S 4028
	}
    else if (dialectName == "Postgres8") {
		os << "LINESTRING(" ;
        size_t pts = points() ;
		for(size_t i = 0 ; i < pts ; i++) { // cannot use dbGeoPntList::getSqlString(...)
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
		   << "2002," << getDialectSRID(dialectName) <<",null,MDSYS.SDO_ELEM_INFO_ARRAY(1,2,1),"
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

void dbGeoString::bbox(dbGeoBox &bb, double latmargin, double lonmargin) const 
{
	_points.bbox(bb,latmargin,lonmargin) ;
    const TimeRange &tr = timeRange() ;
    if (tr.start().isFilled() && tr.stop().isFilled()) {
	    bb.timeRange(timeRange().wholeDay()) ;
    }
}


void dbGeoString::exportSpecific(rsResourceSet &s) const
{
	rsPushNSpace nsp(s,"dbGeoString") ;
	exportBase(s) ;
}

void dbGeoString::exportSpecificXML(XMLOstream &xst) const
{
	XMLOstream::Tag txml(xst,"dbGeoString") ;
	exportBaseXML(xst) ;
}

void dbGeoString::importSpecific(rsResourceSet &s)
{
	rsPushNSpace nsp(s,"dbGeoString") ;
	importBase(s) ;
}

string dbGeoString::properties() const 
{
	ostringstream os ;
	os << "Length: " << fixed << setprecision(1) << _points.lengthKm() << " [Km]" ; 
	return os.str() ;
}


bool dbGeoString::intersectGeo(const dbGeoObject  & geo) const
{
	ACS_THROW(dbGeoObject::ExecuteException ("method not implemented ")) ;	// PRQA S 3081	

}

bool dbGeoString::isInsideGeo(const dbGeoObject &) const
{
	ACS_THROW(dbGeoObject::ExecuteException ("method not implemented ")) ; // PRQA S 3081		
}

void dbGeoString::parseOracle(const string &is, int offs, int etype, int interpr) 
{
/* MDSYS.SDO_GEOMETRY(2003,8307,null,MDSYS.SDO_ELEM_INFO_ARRAY(1,1003,1),MDSYS.SDO_ORDINATE_ARRAY(10,10, 20,20, 40,20, 30,10))*/
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "is: \"" << is << "\"") ;
	ACS_COND_THROW ((etype != 2) || (interpr != 1), // PRQA S 3081, 3380, 4400
			UnrecognizedTypeException("Invalid etype/iterpr pair: "+ ((is.length() > 64) ? is.substr(0,64) : is) + " ...\"")) ; // PRQA S 3081, 3380
	
	
	RegExp recoord(".*SDO_ORDINATE_ARRAY[ \t]*[(]") ;
	regoff_t coordbeg ; // PRQA S 4101 2
    regoff_t coordend ;
	if (!recoord.match(is,coordbeg,coordend)) {
		ACS_THROW(UnrecognizedTypeException("Unrecognized \"" + ((is.length() > 64) ? is.substr(0,64) : is) + " ...\"")) ; // PRQA S 3081, 3380
	}

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "coords: \"" << is.substr(coordend) << "\"") ; // PRQA S 3000

	dbGeoPntList p ;
	p.parseOracle(is.substr(coordend)) ; // PRQA S 3000

	// erase lastpoint.
	*this = dbGeoString(p) ; // PRQA S 3081
	dbGeoAltitudeRange ar ;
	dbGeoTimeRange tr ;
	altitudeRange(ar) ;
	timeRange(tr) ;
}


std::istream &
operator >> (std::istream &is , dbGeoString & l) // PRQA S 4020
{
/* GeoString(((55.7348,-161.727),(53.9313,-162.04),(52.1199,-162.338),(50.3076,-162.623),(48.4945,-162.896),(46.6806,-163.159),
   (44.8659,-163.413),(43.0505,-163.66),(41.2343,-163.899),(39.4174,-164.132),(37.5998,-164.36),(36.6317,-164.479)),
   ANY,(2004-07-04 10:49:25,2004-07-04 10:54:41)) */
/* LINESTRING(0 0, 1 1, 2 1, 2 2) */
	if (!is.good()) { return is ;} // PRQA S 4028
	std::string oname ;
    bool informix = true ;
	if (!dbGeoObject::extractObjType(is,oname)) {
		is.setstate(std::ios::failbit) ;
		return is ;	 // PRQA S 4028
	}

    if (oname == "GeoString(") {
        informix = true ;
    }
    else if (oname == "LINESTRING(") {
        informix = false ;
    }
    else {
		is.setstate(std::ios::failbit) ;
		return is ;	 // PRQA S 4028
    }

	dbGeoPntList pl ;
    
    if (informix) {
	    dbGeoAltitudeRange ar ;
	    dbGeoTimeRange tr ;

	    char comma ; // PRQA S 4101 2
        char ell ;
	    is >> pl >> comma >> ar >> comma >> tr >> ell ;

	    if (!is.fail()) {
		    l = dbGeoString(pl) ; // PRQA S 3081
		    l.altitudeRange(ar) ;
		    l.timeRange(tr) ;
	    }
    }
    else {
        int prec = 0 ;
        dbGeoPntList::parsePostgres(pl, is, prec) ; 
        if (!is.fail()) {
		    l = dbGeoString(pl) ; // PRQA S 3081
            l.precision(prec) ;
	    }
    }
	return is ; // PRQA S 4028
}



std::ostream & operator << (std::ostream &os, const dbGeoString& gs) 
{
   return gs.getSqlString(os, "Postgres8") ;
}

exostream & operator << (exostream &os, const dbGeoString& gs) // PRQA S 4222
{
    ostringstream s ;
    gs.getSqlString(s, "Postgres8") ;
    return os << s.str() ;
}
_ACS_END_NAMESPACE

