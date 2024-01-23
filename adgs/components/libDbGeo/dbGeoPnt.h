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
	Revision 5.4  2014/05/09 11:39:54  marpas
	coding best practices applied
	qa & c++ warnings fixed
	last throwing macros for exceptions adopted
	
	Revision 5.3  2014/05/06 12:25:58  marpas
	precision in getting sql string managed
	dbGeoObject interface has now methods to set/get precision and a defaultPrecision static constant
	precision must be greater than 2, note that precision will affect lenght of string that will be used in queries, limiting the maximum number of points as the query string has limited size
	
	Revision 5.2  2013/09/17 10:31:01  enrcar
	EC:: method intersect was unreliable if one of the poligons crossed 180. Fixed.
	
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
	
	Revision 2.13  2012/12/14 11:13:41  marpas
	coding best practices
	qa rules
	test improeved
	test coverage
	
	Revision 2.12  2012/02/13 11:15:39  marpas
	refactoring in progress
	
	Revision 2.11  2011/09/29 14:48:10  marpas
	simplifying templates for codecheck
	
	Revision 2.10  2010/04/26 14:32:26  marpas
	implementing Oracle spatial/locator syntax. Work in progress
	
	Revision 2.9  2010/04/26 11:40:20  marpas
	work in progress Oracle11 Spatial and Locator support implementation
	
	Revision 2.8  2010/04/09 17:23:21  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.7  2009/06/26 09:48:17  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.6  2009/06/24 09:27:53  marpas
	ntersections still in progress
	
	Revision 2.5  2009/06/23 13:29:43  marpas
	isInsideGeo and intersectGeo method can be invoked on CONST objects
	
	Revision 2.4  2007/07/03 10:32:12  marpas
	toString now has argument for decimal precision
	
	Revision 2.3  2007/06/11 13:21:42  marpas
	toHtmlString methods added
	
	Revision 2.2  2006/05/12 16:32:07  marpas
	Bug fixed when north pole is inside the polygon
	
	Revision 2.1  2006/03/30 10:41:58  marpas
	namespace does not need to be declared again inside
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.7  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.6  2004/02/04 10:17:50  paoscu
	Better doxygen comments.
	
	Revision 1.5  2003/09/03 13:57:43  paoscu
	Export methods are now const.
	
	Revision 1.4  2003/04/30 13:07:28  marpas
	using namespace std no longer in includes
	
	Revision 1.3  2002/12/16 11:13:50  paoscu
	Export and import methods modified.
	
	Revision 1.2  2002/11/29 17:48:46  danalt
	add exportXML, corrected some bugs
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/




#ifndef _dbGeoPnt_H_
#define _dbGeoPnt_H_ 

#include <acs_c++config.hpp>
#include <exMacros.h>

#include <string>
#include <iostream>

//
//
//
//	class dbGeoPnt
//
//
//

_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ;
class XMLOstream ;
class exostream ;

class dbGeoPnt // PRQA S 2109
{
public:
	enum Intersection {
		IntersectNone = 0,
		Intersect1st = 1,
		Intersect2nd = 2,
		IntersectBoth = 3
	} ;
	
	enum Type { Latitude, Longitude} ;

public:
	explicit dbGeoPnt(double lat=0, double lon=0) ;
	dbGeoPnt(const dbGeoPnt &) ;
	~dbGeoPnt()  throw() ;

	dbGeoPnt &operator=(const dbGeoPnt &) ;
	
	std::ostream &getSqlString(std::ostream &, const std::string &dialectName, int precision) const ;


	bool cross180(const dbGeoPnt &) const ;

	/** 
	 * Computes the signed area multiplied by two of the
	 * triangle oriented o-a-b, where o is the current point
	 */
	double area2(const dbGeoPnt &a, const dbGeoPnt &b) const throw() 
    { 
        return _lon*(a._lat-b._lat)+a._lon*(b._lat-_lat)+b._lon*(_lat-a._lat) ; 
    }
	
	// scalar product
	double scalar(const dbGeoPnt &a, const dbGeoPnt &b) const throw() 
    { 
        return (b._lat-_lat)*(a._lat-_lat)+(b._lon-_lon)*(a._lon-_lon) ; 
    }

	
	double lat() const throw() { return _lat ; }
	double lon() const throw() { return _lon ; }

	double &lat() throw() { return _lat ; } // PRQA S 4024 2
	double &lon() throw() { return _lon  ; }
	
	void lon(double v) throw() { _lon = v ; }
	void lat(double v) throw() { _lat = v ; }
	
	double distance(const dbGeoPnt &) const ; // PRQA S 2502
	// inherited from dbExportable
	void importObj(rsResourceSet &) ;
	void exportObj(rsResourceSet &) const ;
	void exportXML(XMLOstream &) const ;
	std::string to2LineString() const ;
	std::string toString(unsigned short decimal = 2) const ; // PRQA S 2502
	std::string toHtmlString(bool letterAtEnd=false, unsigned short secDec=0) const ;
	
	static std::string toString(double, Type, unsigned short decimal = 2) ; // PRQA S 2502, 4400
	static void D2DMS(double, int &, int &, double &) ;
	static std::string toHtmlString(double, Type, bool letterAtEnd=false, unsigned short secDec=0) ;
	static bool cross180(const dbGeoPnt &, const dbGeoPnt &) ;

	static Intersection intersect(const dbGeoPnt &, const dbGeoPnt &, 
								  const dbGeoPnt &, const dbGeoPnt &, 
								  dbGeoPnt * = 0) ;
	// checks if the point c is inside the segment [a,b]
	static bool belongsToSegment(const dbGeoPnt &a, const dbGeoPnt &b, const dbGeoPnt &c) ;
	// normalizes longitude between -180,360
	static double normLon(double lon) throw() 
    {
		return (lon <= -180.0) ? (lon + 360.0) : lon ; // PRQA S 3382, 4400
    }
	
	// calculate the coordinates of the perpendicular projection on given lineseg
	void perpendicularPos(const dbGeoPnt & , const dbGeoPnt & , dbGeoPnt & ) const;
	static std::istream & parseOracle(dbGeoPnt &, std::istream &) ;
	static std::istream & parsePostgres(dbGeoPnt &, std::istream &, int& precision) ;


// data section
private:
	double _lat ;
	double _lon ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbGeoPnt) ;
} ;


std::istream &
operator >> (std::istream &is, dbGeoPnt& p); 

exostream & operator << (exostream &, const dbGeoPnt&) ; // PRQA S 2072

std::string toString(dbGeoPnt::Intersection) ;

std::ostream & operator << (std::ostream &, dbGeoPnt::Intersection) ;
exostream & operator << (exostream &, dbGeoPnt::Intersection) ; // PRQA S 2072
      
      bool operator==(const dbGeoPnt &, const dbGeoPnt &) throw() ; 
inline bool operator!=(const dbGeoPnt &a, const dbGeoPnt &b) throw() { return not (a == b) ; } // PRQA S 2134

_ACS_END_NAMESPACE


#endif // _dbGeoPnt_H_

