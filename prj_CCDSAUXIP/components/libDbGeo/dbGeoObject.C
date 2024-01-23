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
	Revision 5.6  2017/11/21 15:14:55  davide.tiriticco
	Implemented functionality getWKTString
	
	Revision 5.5  2016/07/25 15:40:29  damdec
	Bug fixed in getDialectSRID
	
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
	
	Revision 5.1  2013/07/11 16:31:47  marpas
	macro usage fixed
	
	Revision 5.0  2013/07/11 16:07:11  marpas
	libException 5.x standards adopted
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.19  2013/05/14 18:00:34  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.18  2012/12/14 11:13:41  marpas
	coding best practices
	qa rules
	test improeved
	test coverage
	
	Revision 2.17  2012/02/13 11:15:39  marpas
	refactoring in progress
	
	Revision 2.16  2011/09/29 14:48:09  marpas
	simplifying templates for codecheck
	
	Revision 2.15  2010/05/19 16:06:06  marpas
	Oracle spatial/locator parse completed
	
	Revision 2.14  2010/05/12 13:31:28  marpas
	debug improved
	
	Revision 2.13  2010/05/12 13:22:21  marpas
	setDialectSRID and getDialectSRID static methods added to dbGeoObejct class
	getSQLString with dialect set to Oracle11 use the SRID passed to the above methods
	
	Revision 2.12  2010/05/07 15:02:35  marpas
	bug fixed (if missing)
	
	Revision 2.11  2010/04/27 11:42:26  marpas
	still implementing ORACLE spatial/locator support.
	
	Revision 2.10  2010/04/26 14:32:26  marpas
	implementing Oracle spatial/locator syntax. Work in progress
	
	Revision 2.9  2010/04/09 17:23:21  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.8  2009/06/26 09:48:17  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.7  2009/06/24 09:27:53  marpas
	ntersections still in progress
	
	Revision 2.6  2009/06/23 16:26:40  marpas
	intersect in dbGeoBox fixed
	
	Revision 2.5  2009/06/05 09:31:18  crivig
	 isInsideGeo and intersectGeo methods added
	
	Revision 2.4  2008/10/10 10:01:51  crivig
	porting to postgres8
	
	Revision 2.3  2007/12/03 16:43:37  marpas
	polygon comparison fixed
	
	Revision 2.2  2007/07/03 09:54:15  marpas
	comparison operators are now public
	
	Revision 2.1  2006/03/06 13:09:46  marpas
	importRings method fixed according the format for the XM & RS export
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.23  2006/02/21 15:29:21  nicvac
	libStringUtils dependency removed.
	
	Revision 1.22  2006/02/21 13:37:20  nicvac
	Method string2Type (case insensitive) added.
	
	Revision 1.21  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.20  2005/09/20 16:06:55  marpas
	uses DBTHREADMUTEX macros
	
	Revision 1.19  2005/07/06 14:40:13  enrcar
	DBEXPRESS option added
	
	Revision 1.18  2005/01/04 00:38:08  marpas
	geoObject xml import export fixed
	
	Revision 1.17  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.16  2004/06/16 09:44:09  marpas
	measurement methods added
	
	Revision 1.15  2004/05/31 13:25:02  marpas
	dbLockGuard revived
	
	Revision 1.14  2004/05/27 12:55:13  marpas
	some cosmetic changes,
	new method unionWith in dbGeoPntList
	
	Revision 1.13  2004/05/18 15:55:35  marpas
	dbLockGuard removed from libDbBasic
	
	Revision 1.12  2003/09/03 13:57:43  paoscu
	Export methods are now const.
	
	Revision 1.11  2003/07/11 13:06:06  marpas
	exException::clone implemented
	
	Revision 1.10  2003/06/09 17:13:01  marpas
	new type2String method
	
	Revision 1.9  2003/04/30 13:07:28  marpas
	using namespace std no longer in includes
	
	Revision 1.8  2003/04/30 10:41:43  paoscu
	New STL include style.
	
	Revision 1.7  2003/04/15 18:35:04  marpas
	GCC3.2
	
	Revision 1.6  2003/02/04 10:26:36  marpas
	comments changed
	
	Revision 1.5  2003/02/03 18:58:59  marpas
	Exceptions hierarchy tree changed to hilight db exceptions
	
	Revision 1.4  2003/01/31 15:14:04  paoscu
	DB lock added
	
	Revision 1.3  2002/12/02 17:49:08  paoscu
	Fixed a bug in exportXML
	
	Revision 1.2  2002/11/29 17:48:46  danalt
	add exportXML, corrected some bugs
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/

#include <dbGeoObject.h>

#include <dbGeoPnt.h>
#include <dbGeoPntList.h>
#include <dbGeoPoint.h>
#include <dbGeoCircle.h>
#include <dbGeoBox.h>
#include <dbGeoEllipse.h>
#include <dbGeoLineseg.h>
#include <dbGeoString.h>
#include <dbGeoRing.h>
#include <dbGeoPolygon.h>


#include <dbgV.h>

#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <XMLOstream.h>
#include <StringUtils.h>
#include <RegExp.h>
#include <Filterables.h>


#include <sstream>

#include <iomanip>
#include <cmath>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

namespace {
    dbgV Version;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbGeoObject)

const double dbGeoObject::earthRadiusKm = 6366.70702 ; // nominal
Mutex dbGeoObject::_SRIDs_mutex ;
std::map<std::string, std::string> dbGeoObject::_dialectSRIDs ;

void dbGeoObject::fillDialectSRIDs()  {
    Lock lock(_SRIDs_mutex) ;
    if (_dialectSRIDs.find("Oracle11") == _dialectSRIDs.end()){
        _dialectSRIDs["Oracle11"] = "8307" ; // Oracle Spatial WGS84
    }
    if (_dialectSRIDs.find("Postgres8") == _dialectSRIDs.end()){
        _dialectSRIDs["Postgres8"] = "4326" ; // Oracle Spatial WGS84
    }
}


dbGeoObject::Type dbGeoObject::string2Type(const string& t) // PRQA S 4020, 5520
{
    if      ( StringUtils::equalsNoCase( t, "Box") )   { return Box; } 
    else if ( StringUtils::equalsNoCase( t, "Circle") )  { return Circle; } 
    else if ( StringUtils::equalsNoCase( t, "Ellipse") ) { return Ellipse; } 
    else if ( StringUtils::equalsNoCase( t, "Lineseg") ) { return Lineseg; } 
    else if ( StringUtils::equalsNoCase( t, "Point") )   { return Point; } 
    else if ( StringUtils::equalsNoCase( t, "Polygon") ) { return Polygon; } 
    else if ( StringUtils::equalsNoCase( t, "Ring") )    { return Ring; } 
    else if ( StringUtils::equalsNoCase( t, "String") )  { return String; } 
    else { // unrecognized
        ACS_THROW(UnrecognizedTypeException("string2Type: Unknown type : \""+t+"\"")) ; // PRQA S 3081
    }
}


string dbGeoObject::type2String(Type t) // PRQA S 4020
{
	switch(t) {
	case Box: // box
        {
		    return "Box" ;
        }
	case Circle: // circle
        {
		    return "Circle" ;
        }
	case Ellipse:
        {
		    return "Ellipse" ;
        }
	case Lineseg:
        {
		    return "Lineseg" ;
        }
	case Point:
        {
		    return "Point" ;
        }
	case Polygon:
        {
		    return "Polygon" ;
        }
	case Ring:
        {
		    return "Ring" ;
        }
	case String:
        {
		    return "String" ;
        }
	default: // ??
        {
		    return "Unrecognized" ;
        }
	}
}

string dbGeoObject::type2WKT(Type t) // PRQA S 4020
{
	switch(t) {
	case Box: // box
        {
		    return "Polygon" ;
        }
	case Circle: // circle
        {
		    ACS_THROW(exNotImplemented("Well-Known-Text representation for type Circle is not supported."));
        }
	case Ellipse:
        {
		    ACS_THROW(exNotImplemented("Well-Known-Text representation for type Ellipse is not supported."));
        }
	case Lineseg:
        {
		    return "LineString" ;
        }
	case Point:
        {
		    return "Point" ;
        }
	case Polygon:
        {
		    return "Polygon" ;
        }
	case Ring:
        {
		    return "Polygon" ;
        }
	case String:
        {
		    return "LineString" ;
        }
	default: // ??
        {
		    ACS_THROW(exNotImplemented("Unrecognized geo object type."));
        }
	}
}

	
dbGeoObject::dbGeoObject() :
	_timeRange(),
	_altitudeRange(),
	_points(),
	_rings(),
    precision_(defaultPrecision)
{
    fillDialectSRIDs() ;
}


dbGeoObject::dbGeoObject(const dbGeoPntList &pl) :
	_timeRange(),
	_altitudeRange(),
	_points(pl),
	_rings(),
    precision_(defaultPrecision)
{
    fillDialectSRIDs() ;
}





dbGeoObject::~dbGeoObject() throw() { }



void dbGeoObject::append(const dbGeoPnt &point)
{	
	_points.append(point) ;
}


void dbGeoObject::append(const double lat, const double lon)
{	
	append(dbGeoPnt(lat,lon)) ;
}

void dbGeoObject::append(const dbGeoPntList &ring)
{	
	_rings.push_back(ring) ;
}


const dbGeoPnt &dbGeoObject::operator[](size_t i) const 
{
	return _points[i] ;
}


dbGeoPnt &dbGeoObject::operator[](size_t i) 
{
	return _points[i] ;
}

const dbGeoPntList &dbGeoObject::pointList() const 
{
	return _points ;
}


const dbGeoPntList &dbGeoObject::ring(size_t i) const 
{
	if (i >= rings()) { // out of bound ... 
		ostringstream os ;
		os << "dbGeoObject::ring " << i << " greater than " << rings()-1 ; // PRQA S 3084
		
		// .. throw an exception 	
		ACS_THROW(exBoundsException(os.str())) ; // PRQA S 3081
	}
	
	return _rings[i] ;
	
}


dbGeoPntList &dbGeoObject::ring(size_t i) 
{
	if (i >= rings()) { // out of bound ... 
		ostringstream os ;
		os << "dbGeoObject::ring " << i << " greater than " << rings()-1 ; // PRQA S 3084
		
		// .. throw an exception 	
		ACS_THROW(exBoundsException(os.str())) ; // PRQA S 3081
	}
	
	return _rings[i] ;
}


dbGeoObject::dbGeoObject(const dbGeoObject &rhs) :
	_timeRange(rhs._timeRange),
	_altitudeRange(rhs._altitudeRange),
	_points(rhs._points),
    _rings(rhs._rings),
    precision_(rhs.precision_)
{
}

dbGeoObject &dbGeoObject::operator=(const dbGeoObject &dbg)
{
	if(this != &dbg) {
		_timeRange	   = dbg._timeRange ;
		_altitudeRange = dbg._altitudeRange ;
		_points = dbg._points ;
        _rings = dbg._rings ;
        precision_ = dbg.precision_ ;
	}
    return *this ;
}



dbGeoObject *dbGeoObject::importObject(rsResourceSet &s) // PRQA S 4020, 5520
{
	rsPushNSpace ns(s,"dbGeoObject") ;
	string type ;
	s.getValue("classtype", type) ;

	if (type == "dbGeoPolygon") // is a geopolygon ? 
	{
		dbGeoPolygon p ;
		p.importSpecific(s) ;
		return p.clone() ;
	}
	else if (type == "dbGeoString") // is a geostring ? 
	{
		dbGeoString p ;
		p.importSpecific(s) ;
		return p.clone() ;
	}
	else if (type == "dbGeoBox") // is a geobox ? 
	{
		dbGeoBox p ;
		p.importSpecific(s) ;
		return p.clone() ;
	}
	else if (type == "dbGeoCircle") // is a geocircle ? 
	{
		dbGeoCircle p ;
		p.importSpecific(s) ;
		return p.clone() ;
	}
	else if (type == "dbGeoEllipse")
	{
		dbGeoEllipse p ;
		p.importSpecific(s) ;
		return p.clone() ;
	}
	else if (type == "dbGeoLineseg")
	{
		dbGeoLineseg p ;
		p.importSpecific(s) ;
		return p.clone() ;
	}
	else if (type == "dbGeoPoint")
	{
		dbGeoPoint p ;
		p.importSpecific(s) ;
		return p.clone() ;
	}
	else if (type == "dbGeoRing")
	{
		dbGeoRing p ;
		p.importSpecific(s) ;
		return p.clone() ;
	}
	else // is nothing knonw ... 
	{
		ACS_THROW (UnrecognizedTypeException("Unknown obj type : \""+type+"\"")) ; // PRQA S 3081
	}
}

void dbGeoObject::exportObj(rsResourceSet &s) const // PRQA S 5520
{
	rsPushNSpace ns(s,"dbGeoObject") ;
	string type ; 
	
	if (dynamic_cast<dbGeoPolygon const *>(this)) { // is a dbGeoPolygon // PRQA S 3081
		type = "dbGeoPolygon" ;
    }
	else if (dynamic_cast<dbGeoString const *>(this)) { // is a dbGeoString // PRQA S 3081
		type = "dbGeoString" ;
    }
	else if (dynamic_cast<dbGeoBox const *>(this)) {  // PRQA S 3081
		type = "dbGeoBox" ;
    }
	else if (dynamic_cast<dbGeoCircle const *>(this)) {  // PRQA S 3081
		type = "dbGeoCircle" ;
    }
	else if (dynamic_cast<dbGeoEllipse const *>(this)) {  // PRQA S 3081
		type = "dbGeoEllipse" ;
    }
	else if (dynamic_cast<dbGeoLineseg const *>(this)) {  // PRQA S 3081
		type = "dbGeoLineseg" ;
    }
	else if (dynamic_cast<dbGeoPoint const *>(this)) {  // PRQA S 3081
		type = "dbGeoPoint" ;
    }
	else if (dynamic_cast<dbGeoRing const *>(this)) { // is a dbGeoRing // PRQA S 3081
		type = "dbGeoRing" ;
    }
	else // unrecognized
	{
		ACS_THROW(UnrecognizedTypeException("Unrecognized obj")) ; // PRQA S 3081
	}

	s.setValue("classtype", type) ;
	exportSpecific(s) ;
}

void dbGeoObject::importObj(rsResourceSet &s)
{
	ACS_THROW(ExecuteException ("importObj: not implemented") ); // PRQA S 3081
}


void dbGeoObject::exportXML(XMLOstream &xst) const // PRQA S 5520
{
	XMLOstream::Tag txml(xst,"dbGeoObject") ;
	string type ; 
	
	if (dynamic_cast<dbGeoPolygon const *>(this)) { // is a dbGeoPolygon // PRQA S 3081
		type = "dbGeoPolygon" ;
	}
	else if (dynamic_cast<dbGeoString const *>(this)) { // is a dbGeoString // PRQA S 3081
		type = "dbGeoString" ;
	}
	else if (dynamic_cast<dbGeoBox const *>(this)) { // ... // PRQA S 3081
		type = "dbGeoBox" ;
	}
	else if (dynamic_cast<dbGeoCircle const *>(this)) {  // PRQA S 3081
		type = "dbGeoCircle" ;
	}
	else if (dynamic_cast<dbGeoEllipse const *>(this)) {   // PRQA S 3081
		type = "dbGeoEllipse" ;
	}
	else if (dynamic_cast<dbGeoLineseg const *>(this)) {  // PRQA S 3081
		type = "dbGeoLineseg" ;
	}
	else if (dynamic_cast<dbGeoPoint const *>(this)) { // ... // PRQA S 3081
		type = "dbGeoPoint" ;
	}
	else if (dynamic_cast<dbGeoRing const *>(this)) { // is a dbGeoRing // PRQA S 3081
		type = "dbGeoRing" ;
	}
	else // unrecognied
	{
		ACS_THROW(UnrecognizedTypeException("Unrecognized obj")) ; // PRQA S 3081
	}

	XMLOstream::Tag txml2(xst,"classtype", type) ;
	txml2.close();
	exportSpecificXML(xst) ;
}

dbGeoObject *dbGeoObject::newDbGeo(const string &sqlstring) // PRQA S 4020
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	// find type 
	RegExp reobj("[A-Za-z_]+") ;
	regoff_t objbeg ; // PRQA S 4101 2
    regoff_t objend ;
	if (!reobj.match(sqlstring,objbeg,objend)) {
		ACS_THROW(UnrecognizedTypeException("Unrecognized \"" + (( sqlstring.length() > 10 ) ? sqlstring.substr(0,10) : sqlstring) + " ...\"")) ; // PRQA S 3081, 3380
	}
	
	string type = sqlstring.substr(objbeg, objend-objbeg) ; // PRQA S 3000
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "\""  << type << "\"") ;
	
	if (type == "MDSYS" || type == "SDO_GEOMETRY") {
		RegExp repre("[^0-9]*") ;
		regoff_t prebeg ; // PRQA S 4101 2
        regoff_t preend ;
		if (!repre.match(sqlstring,prebeg,preend)) {
			ACS_THROW(UnrecognizedTypeException("Unrecognized \"" + ((sqlstring.length() > 32) ? sqlstring.substr(0,32) : sqlstring) + " ...\"")) ;	// PRQA S 3081, 3380
		}
	
		string sdotype = sqlstring.substr(preend, 4) ; // PRQA S 3000, 4400
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "[" << preend << "," << preend+4 << "] "
                << " \""  << sdotype << "\"") ;
		
		ACS_COND_THROW(sdotype[0] != '2', // PRQA S 3081, 3380 
			UnrecognizedTypeException("Unrecognized geo object: only 2 dimensions supported (\"" + ((sqlstring.length() > 32) ? sqlstring.substr(0,32) : sqlstring) + " ...\")")
		) ;	
		
		string tt = sdotype.substr(2,2) ;
		
		if ( tt == "01") { // punto
			dbGeoPoint *p = new dbGeoPoint ;
			try {
				p->parseOracle(sqlstring) ;
				return p ;
			}
			catch(exception &x) {
				delete p ;
				ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
					ACS_LOG_NOTIFY_EX(x) ;
				ACS_CLASS_END_DEBUG
			}
		}
		
		int sdo_startingoffset ;  // PRQA S 4101 3
        int sdo_etype ; 
        int sdo_interpr ;
		{
			RegExp reinfo(".*SDO_ELEM_INFO_ARRAY[ \t]*[(]") ;
			regoff_t einfobeg ;  // PRQA S 4101 3 
            regoff_t einfoend ;
			if (!reinfo.match(sqlstring,einfobeg,einfoend)) {
				ACS_THROW(UnrecognizedTypeException("Unrecognized \"" + ((sqlstring.length() > 64) ? sqlstring.substr(0,64) : sqlstring) + " ...\"")) ;// PRQA S 3081, 3380
			}
			istringstream eleminfo(sqlstring.substr(einfoend)) ; // PRQA S 3000
			eleminfo.exceptions(ios_base::failbit) ;
			
			char comma ; // PRQA S 4101
			try {
				eleminfo >> sdo_startingoffset >> comma >> sdo_etype >> comma >> sdo_interpr ;
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "sdo_startingoffset: " << sdo_startingoffset 
                        << ", sdo_etype: " << sdo_etype << ", sdo_interpr: " << sdo_interpr) ;
			}
			catch(exception &x) {
				ACS_THROW(UnrecognizedTypeException(x, "Unrecognized \"" + ((sqlstring.length() > 64) ? sqlstring.substr(0,64) : sqlstring) + " ...\"")) ; // PRQA S 3081, 3380	
			}
			bool supported = true ;
			switch (sdo_etype) {
			case 1: { break ; }
			case 2: // lines // PRQA S 4402
                {
				    if (sdo_interpr == 2) { // PRQA S 4400
					    supported = false ;
                    }
				    break ;
                }
			case 3: // PRQA S 4402 2
			case 4: // arcs
                {
				    supported = false ;
				    break ;
                }
			case 1003: { break ; } // polygon, circle  // PRQA S 4402 2
			default:
                {
				    supported = false ;
				    break ;
                }
			}

			if (!supported)
			{
				ostringstream os;
				os << "Unsupported SDO_ETYPE/SDO_INTERPR: " << sdo_etype << "/" << sdo_interpr
				   << " \"" << ((sqlstring.length() > 64) ? sqlstring.substr(0,64) : sqlstring) << " ...\"" ; // PRQA S 3380, 4400
				ACS_THROW(UnrecognizedTypeException(os.str())) ; // PRQA S 3081
			}
		}

		if (tt == "03") { // polygon 
			dbGeoBox *b  = new dbGeoBox ;
			try {
				b->parseOracle(sqlstring,sdo_startingoffset,sdo_etype,sdo_interpr ) ;
				return b ;
			}
			catch(exception &x) {
				delete b ;
				ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
					ACS_LOG_NOTIFY_EX(x) ;
				ACS_CLASS_END_DEBUG
			}
			dbGeoPolygon *p  = new dbGeoPolygon ;
			try {
				p->parseOracle(sqlstring,sdo_startingoffset,sdo_etype,sdo_interpr ) ;
				return p ;
			}
			catch(exception &x) {
				delete p ;
				ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
					ACS_LOG_NOTIFY_EX(x) ;
				ACS_CLASS_END_DEBUG
			}
			
			// polygon	
			
		}
		else if (tt == "02") { // monodimensional ... 
			// if closed is a ring 
			dbGeoRing *r  = new dbGeoRing ;
			try {
				r->parseOracle(sqlstring,sdo_startingoffset,sdo_etype,sdo_interpr ) ;
				return r ;
			}
			catch(exception &x) {
				delete r ;
				ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
					ACS_LOG_NOTIFY_EX(x) ;
				ACS_CLASS_END_DEBUG
			}
			
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "trying GeoString") ;
			// if open and npoints > 2 is a string 
			// else is a line...
			dbGeoString *s  = new dbGeoString ;
			try {
				s->parseOracle(sqlstring,sdo_startingoffset,sdo_etype,sdo_interpr ) ;
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "s points: " << s->pointList().points()) ;
				
				if (s->pointList().points() < 3) { // PRQA S 4400
					delete s ;
				}
				else {
					return s ;
                }
			}
			catch(exception &x) {
				delete s ;
				ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
					ACS_LOG_NOTIFY_EX(x) ;
				ACS_CLASS_END_DEBUG
			}
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "trying GeoLineseg") ;
			// if open and npoints > 2 is a string 
			// else is a line...
			dbGeoLineseg *l  = new dbGeoLineseg ;
			try {
				l->parseOracle(sqlstring,sdo_startingoffset,sdo_etype,sdo_interpr ) ;
				return l ;
			}
			catch(exception &x) {
				delete l ;
				ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
					ACS_LOG_NOTIFY_EX(x) ;
				ACS_CLASS_END_DEBUG
			}
		}
        else {
            ACS_THROW(UnrecognizedTypeException("Unrecognized geo object \"" + ((sqlstring.length() > 32) ? sqlstring.substr(0,32) : sqlstring) + " ...\"")) ; // PRQA S 3081, 3380
        }
	}
    dbGeoObject *obj=newDbGeoPgres(type, sqlstring) ; 
    if (obj) { return obj ; }
	obj =  newDbGeoIfx(type, sqlstring) ;
    if (obj) { return obj ; }


	ACS_THROW(UnrecognizedTypeException("Unrecognized geo object \"" + ((sqlstring.length() > 10) ? sqlstring.substr(0,10) : sqlstring) + " ...\"")) ; // PRQA S 3081, 3380
}


dbGeoObject *dbGeoObject::newDbGeoPgres(const string &type, const string &sqlstring) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "type: \"" << type << "\" - \"" <<  sqlstring << "\"") ;
	istringstream objstream(sqlstring) ; // prepare a stream to feed the creating object

	if (type == "POINT") { // create a dbGeoPoint
		dbGeoPoint *b  = new dbGeoPoint ;
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "b->precision(): " << b->precision()) ;
        
		objstream >> *b ; // feed the stream 
		if (objstream.fail()) {
			ACS_THROW(UnrecognizedTypeException("Unrecognized POINT \"" + ((sqlstring.length() > 10) ? sqlstring.substr(0,10) : sqlstring) + " ...\"")) ; // PRQA S 3081, 3380
		}
		return b ;
	}
	
	if (type == "LINESTRING") { // create a dbGeoString
		dbGeoString *b  = new dbGeoString ;
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "b->precision(): " << b->precision()) ;
		objstream >> *b ;// feed the stream 
		if (objstream.fail()) {
			ACS_THROW(UnrecognizedTypeException("Unrecognized LINESTRING \"" + ((sqlstring.length() > 10) ? sqlstring.substr(0,10) : sqlstring) + " ...\"")) ; // PRQA S 3081, 3380
		}
		return b ;
	}
	
	if (type == "POLYGON") { // create a dbGeoPolygon
		dbGeoPolygon *b  = new dbGeoPolygon ;
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "b->precision(): " << b->precision()) ;
		objstream >> *b ;// feed the stream 
		if (objstream.fail()) {
			ACS_THROW(UnrecognizedTypeException("Unrecognized POLYGON \"" + ((sqlstring.length() > 10) ? sqlstring.substr(0,10) : sqlstring) + " ...\"")) ; // PRQA S 3081, 3380
		}
		return b ;
	}


	return 0 ;

}


dbGeoObject *dbGeoObject::newDbGeoIfx(const string &type, const string &sqlstring) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "type: \"" << type << "\" - \"" <<  sqlstring << "\"") ;

	istringstream objstream(sqlstring) ; // prepare a stream to feed the creating object

	if (type == "GeoPoint") { // create a dbGeoPoint
		dbGeoPoint *b  = new dbGeoPoint ;
		objstream >> *b ; // feed the stream 
		if (objstream.fail()) {
			ACS_THROW(UnrecognizedTypeException("Unrecognized GeoPoint \"" + ((sqlstring.length() > 10) ? sqlstring.substr(0,10) : sqlstring) + " ...\"")) ; // PRQA S 3081, 3380
		}
		return b ;
	}
	
	if (type == "GeoString") { // create a dbGeoString
		dbGeoString *b  = new dbGeoString ;
		objstream >> *b ;// feed the stream 
		if (objstream.fail()) {
			ACS_THROW(UnrecognizedTypeException("Unrecognized GeoString \"" + ((sqlstring.length() > 10) ? sqlstring.substr(0,10) : sqlstring) + " ...\"")) ; // PRQA S 3081, 3380
		}
		return b ;
	}
	

	if (type == "GeoBox") { // create a dbGeoBox
		dbGeoBox *b  = new dbGeoBox ;
		objstream >> *b ;// feed the stream 
		if (objstream.fail()) {
			ACS_THROW(UnrecognizedTypeException("Unrecognized GeoBox \"" + ((sqlstring.length() > 10) ? sqlstring.substr(0,10) : sqlstring) + " ...\"")) ; // PRQA S 3081, 3380
		}
		return b ;
	}
	
	if (type == "GeoPolygon") {
		dbGeoPolygon *b  = new dbGeoPolygon ;
		objstream >> *b ;// feed the stream 
		if (objstream.fail()) {
			ACS_THROW(UnrecognizedTypeException("Unrecognized GeoPolygon \"" + ((sqlstring.length() > 10) ? sqlstring.substr(0,10) : sqlstring) + " ...\"")) ; // PRQA S 3081, 3380
		}
		return b ;
	}
	
	if (type == "GeoRing") { // create a dbGeoRing
		dbGeoRing *b  = new dbGeoRing ;
		objstream >> *b ;// feed the stream 
		if (objstream.fail()) {
			ACS_THROW(UnrecognizedTypeException("Unrecognized GeoRing \"" + ((sqlstring.length() > 10) ? sqlstring.substr(0,10) : sqlstring) + " ...\"")) ; // PRQA S 3081, 3380
		}
		return b ;
	}
	
	if (type == "GeoCircle") { // create a dbGeoCircle
		dbGeoCircle *b  = new dbGeoCircle ;
		objstream >> *b ;// feed the stream 
		if (objstream.fail()) {
			ACS_THROW(UnrecognizedTypeException("Unrecognized GeoCircle \"" + ((sqlstring.length() > 10) ? sqlstring.substr(0,10) : sqlstring) + " ...\"")) ; // PRQA S 3081, 3380
		}
		return b ;
	}
	
	if (type == "GeoEllipse") { // create a dbGeoEllipse
		dbGeoEllipse *b  = new dbGeoEllipse ;
		objstream >> *b ;// feed the stream 
		if (objstream.fail()) {
			ACS_THROW(UnrecognizedTypeException("Unrecognized GeoEllipse \"" + ((sqlstring.length() > 10) ? sqlstring.substr(0,10) : sqlstring) + " ...\"")) ; // PRQA S 3081, 3380
		}
		return b ;
	}
	
	if (type == "GeoLineseg") { // create a dbGeoLineseg
		dbGeoLineseg *b  = new dbGeoLineseg ;
		objstream >> *b ;// feed the stream 
		if (objstream.fail()) {
			ACS_THROW(UnrecognizedTypeException("Unrecognized GeoLineseg \"" + ((sqlstring.length() > 10) ? sqlstring.substr(0,10) : sqlstring) + " ...\"")) ; // PRQA S 3081, 3380
		}
		return b ;
	}
	
	return 0 ;

}

void dbGeoObject::importBase(rsResourceSet &s) 
{
	{ // time range 
		rsPushNSpace ns(s,"_timeRange") ;
		_timeRange.importObj(s) ;
	}
	{ // altitude 
		rsPushNSpace ns(s,"_altitudeRange") ;
		_altitudeRange.importObj(s) ;
	}
	{ // points
		rsPushNSpace ns(s,"_points") ;
		_points.importObj(s) ;
	}
	{ // rings
		rsPushNSpace ns(s,"_rings") ;
		importRings(s) ;
	}
}


void dbGeoObject::exportBase(rsResourceSet &s)  const
{
	{ // time range 
		rsPushNSpace ns(s,"_timeRange") ;
		_timeRange.exportObj(s) ;
	}
	{ // altitude 
		rsPushNSpace ns(s,"_altitudeRange") ;
		_altitudeRange.exportObj(s) ;
	}
	{ // points 
		rsPushNSpace ns(s,"_points") ;
		_points.exportObj(s) ;
	}
	{ // rings
		rsPushNSpace ns(s,"_rings") ;
		exportRings(s) ;
	}
}

void dbGeoObject::exportBaseXML(XMLOstream &xst)  const
{
	{ // time range 
		XMLOstream::Tag txml(xst,"_timeRange") ;
		_timeRange.exportXML(xst) ;
	}
	{ // altitude 
		XMLOstream::Tag txml(xst,"_altitudeRange") ;
		_altitudeRange.exportXML(xst) ;
	}
	{ // points 
		XMLOstream::Tag txml(xst,"_points") ;
		_points.exportXML(xst) ;
	}
	{ // rings
		XMLOstream::Tag txml(xst,"_rings") ;
		exportRingsXML(xst) ;
	}
}


void dbGeoObject::exportRings(rsResourceSet &s) const
{
	s.setValue("Rings",rings()) ;
    size_t rngs = rings() ;
	for (size_t i=0; i < rngs; ++i) { // each ring 
		{
			ostringstream os ;

			os << "ring_" << i ;


			string r(os.str()) ;

			rsPushNSpace ns(s,r) ;
			ring(i).exportObj(s) ; // export it 
		}
	}
}

void dbGeoObject::importRings(rsResourceSet &s)
{
	size_t rngs ; // PRQA S 4101
	s.getValue("Rings",rngs) ;
	_rings.clear() ;
	for (size_t i=0; i < rngs; ++i) { // each ring 
		{
			ostringstream os ;

			os << "ring_" << i ;

			string r(os.str()) ;

			rsPushNSpace ns(s,r) ;
			_rings.push_back(dbGeoPntList()) ;
			ring(i).importObj(s) ; // import it 
		}
	}
}

void dbGeoObject::exportRingsXML(XMLOstream &xst) const
{
	ostringstream os2 ;

	os2 << rings();

	{

		XMLOstream::Tag txml(xst, "Rings", os2.str()) ;
	}
    size_t rngs = rings() ;
	for (size_t i=0; i < rngs; ++i) { // each ring
		{
			ostringstream os ;

			os << "ring_" << i  ;


			XMLOstream::Tag txml2(xst,os.str()) ; // export it 

			ring(i).exportXML(xst) ;
		}
	}
}

void dbGeoObject::exportSpecific(rsResourceSet &) const
{
	ACS_THROW(ExecuteException("Invalid virtual call")) ; // PRQA S 3081
}

void dbGeoObject::importSpecific(rsResourceSet &)
{
	ACS_THROW(ExecuteException("Invalid virtual call")) ; // PRQA S 3081
}

void dbGeoObject::exportSpecificXML(XMLOstream &) const
{
	ACS_THROW(ExecuteException("Invalid virtual call")) ; // PRQA S 3081
}

double dbGeoObject::distanceKm( // PRQA S 4020
    const dbGeoPnt &p1,
	const dbGeoPnt &p2 
)
{
	double lat1 = p1.lat()*M_PI/180. ;
	double lat2 = p2.lat()*M_PI/180. ;
	double lon1 = p1.lon()*M_PI/180. ;
	double lon2 = p2.lon()*M_PI/180. ;
	double arg = cos(lat1)*cos(lat2)*(cos(lon1)*cos(lon2)+
				 sin(lon1)*sin(lon2))+sin(lat1)*sin(lat2) ;
				 
	if (arg>=1.0) { // do not allow acos to be grater than 1 it can't
		return 0 ;
    }
	double distAngle=acos(arg); // angle in radians 
	return earthRadiusKm*distAngle ; // radius by radians ... 
}


string dbGeoObject::properties() const
{
	return "" ;
}

void dbGeoObject::setDialectSRID(const std::string &dialect, const std::string &srid)
{
    Lock lock(_SRIDs_mutex) ;
	_dialectSRIDs[dialect] = (not srid.empty() ? srid: string("null")) ; // PRQA S 3081, 3380, 3385
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "dialect: " << dialect
            << "SRID: \""  << _dialectSRIDs[dialect] << "\"") ;
}

std::string dbGeoObject::getDialectSRID(const std::string &dialect) 
{
	Lock lock(_SRIDs_mutex) ;
	string retval ("");
	if (_dialectSRIDs.find (dialect) != _dialectSRIDs.end())
	{
		retval = _dialectSRIDs[dialect];
	}
	return retval;
}


bool
dbGeoObject::extractObjType(istream &is, std::string &objtype) // PRQA S 4020
{
	if (!is.good()) { return ! is.fail() ; } // PRQA S 3050
	char buffer[32] ; // PRQA S 4403
	for (size_t i=0; !is.fail();) { // PRQA S 4238
		if (i > (sizeof(buffer)/sizeof(buffer[0]) -1) ) { // PRQA S 3084
			return false ;
		}
		char c ; // PRQA S 4101
		is.get(c) ;
		if (!is.fail()) {
			if (0 == isspace(c)) {
				buffer[i++] = c ;
				if (c == '(') {
					buffer[i++] = '\0' ;
					objtype = buffer ;
					return true ;
				}
			}
		}
	}

	return false ;
}

string dbGeoObject::getSqlString(const string &dialectName) const
{
    ostringstream os ;
    getSqlString(os,dialectName) ;
    return os.str() ;
}

void dbGeoObject::precision(int p) 
{
    ACS_COND_THROW(p <= 2, exIllegalValueException("precision must be greater than 2")) ; // PRQA S 3081
    precision_ = p ;
}

bool operator==(const dbGeoObject &a, const dbGeoObject &b)
{
	return (a.timeRange() == b.timeRange()) && (a.altitudeRange() == b.altitudeRange()) && 
           (a.pointList() == b.pointList()) && (a.getRings() == b.getRings()) ;
}


bool operator!=(const dbGeoObject &a, const dbGeoObject &b)
{
	return not (a == b ) ;
}


std::ostream & operator << (std::ostream &os, const dbGeoObject& o) 
{
   return o.getSqlString(os, "Postgres8") ;
}

exostream & operator << (exostream &os, const dbGeoObject& o) // PRQA S 4222
{
    ostringstream s ;
    o.getSqlString(s, "Postgres8") ;
    return os << s.str() ;
}

_ACS_END_NAMESPACE



