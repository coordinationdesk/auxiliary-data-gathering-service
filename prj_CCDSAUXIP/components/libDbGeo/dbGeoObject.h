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
	Revision 5.3  2016/07/25 15:40:29  damdec
	Bug fixed in getDialectSRID
	
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
	
	Revision 2.17  2013/05/14 18:00:34  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.16  2012/12/14 11:13:41  marpas
	coding best practices
	qa rules
	test improeved
	test coverage
	
	Revision 2.15  2012/04/24 16:30:03  marpas
	no more dependencies from dbException
	
	Revision 2.14  2012/02/13 11:15:39  marpas
	refactoring in progress
	
	Revision 2.13  2011/09/29 14:48:10  marpas
	simplifying templates for codecheck
	
	Revision 2.12  2010/05/12 13:22:22  marpas
	setDialectSRID and getDialectSRID static methods added to dbGeoObejct class
	getSQLString with dialect set to Oracle11 use the SRID passed to the above methods
	
	Revision 2.11  2010/04/26 14:32:26  marpas
	implementing Oracle spatial/locator syntax. Work in progress
	
	Revision 2.10  2010/04/26 11:40:20  marpas
	work in progress Oracle11 Spatial and Locator support implementation
	
	Revision 2.9  2010/04/09 17:23:21  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.8  2009/06/26 09:48:17  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.7  2009/06/24 09:27:53  marpas
	ntersections still in progress
	
	Revision 2.6  2009/06/23 16:26:40  marpas
	intersect in dbGeoBox fixed
	
	Revision 2.5  2009/06/23 13:29:43  marpas
	isInsideGeo and intersectGeo method can be invoked on CONST objects
	
	Revision 2.4  2009/06/05 09:31:18  crivig
	 isInsideGeo and intersectGeo methods added
	
	Revision 2.3  2008/10/10 10:01:51  crivig
	porting to postgres8
	
	Revision 2.2  2007/07/03 09:54:15  marpas
	comparison operators are now public
	
	Revision 2.1  2007/06/09 18:45:54  marpas
	bbox method added
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.18  2006/02/21 15:29:21  nicvac
	libStringUtils dependency removed.
	
	Revision 1.17  2006/02/21 13:37:20  nicvac
	Method string2Type (case insensitive) added.
	
	Revision 1.16  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.15  2005/10/21 18:58:20  marpas
	DBEXPRESS compatibility
	
	Revision 1.14  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.13  2004/06/16 09:44:09  marpas
	measurement methods added
	
	Revision 1.12  2004/05/27 12:55:13  marpas
	some cosmetic changes,
	new method unionWith in dbGeoPntList
	
	Revision 1.11  2004/02/04 10:17:50  paoscu
	Better doxygen comments.
	
	Revision 1.10  2003/09/03 13:57:43  paoscu
	Export methods are now const.
	
	Revision 1.9  2003/07/11 13:06:06  marpas
	exException::clone implemented
	
	Revision 1.8  2003/06/09 17:13:01  marpas
	new type2String method
	
	Revision 1.7  2003/05/22 09:20:55  marpas
	ring interface is more homogeneous now
	
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


#ifndef _dbGeoObject_H_
#define _dbGeoObject_H_

#include <acs_c++config.hpp>
#include <dbGeoPntList.h>
#include <dbGeoTimeRange.h>
#include <dbGeoAltitudeRange.h>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbGeoPnt ;
class rsResourceSet ;
class XMLOstream ;
class dbGeoBox ;

class dbGeoObject // PRQA S 2109
{

public :

	enum Type {
		Box,
		Circle,
		Ellipse,
		Lineseg,
		Point,
		Polygon,
		Ring,
		String
	} ;

    /** Convert a Type into a String */
    static std::string type2String(Type) ;
    /** Convert a Case insensitive String into a Type */
    static Type string2Type(const std::string& t);
    /** Convert a type into a WKT string */
    static std::string type2WKT(Type);

    static const int defaultPrecision = 8 ; // PRQA S 4400
    
    static const int wktPrecision = 8;
    
	/*! class ExecuteException declaration */
	exDECLARE_EXCEPTION(ExecuteException,exException) ; // PRQA S 2131, 2502

	/*! class UnrecognizedTypeException declaration */
	exDECLARE_EXCEPTION(UnrecognizedTypeException,exException) ; // PRQA S 2131, 2502

		
	/** 
	 * The following function is a creational one
	 * to create the right dbGeoObject starting from a datum
	 */
	static dbGeoObject *newDbGeo(const std::string &sqlstring) ;
	virtual ~dbGeoObject()  throw() ;
	virtual dbGeoObject* clone() const = 0 ; // PRQA S 2502

	size_t points() const throw() { return _points.points() ; }  // return the number of points
	size_t rings() const throw() { return _rings.size() ; } // return the number of points
	
	virtual std::ostream &getSqlString(std::ostream &, const std::string &dialectName) const = 0 ;
	std::string getSqlString(const std::string &dialectName) const ;
	virtual std::string getWKTString(bool lonFirst = true) const = 0;
	virtual std::string properties() const ;
	virtual void bbox(dbGeoBox &, double latmargin, double lonmargin) const = 0;
	virtual bool intersectGeo(const dbGeoObject &) const = 0;
	virtual bool isInsideGeo(const dbGeoObject &) const = 0;

	static double distanceKm(const dbGeoPnt &,const dbGeoPnt &) ;	


	// every dbGeoObject has a timeRange and an altitude range
	const dbGeoTimeRange &timeRange() const throw() { return _timeRange; }
	const dbGeoAltitudeRange &altitudeRange() const throw() { return _altitudeRange ; }
	dbGeoTimeRange &timeRange() throw() { return _timeRange; } // PRQA S 4024 2
	dbGeoAltitudeRange &altitudeRange() throw() { return _altitudeRange ; }
	void timeRange(const dbGeoTimeRange &tr) throw() { _timeRange = tr; }
	void altitudeRange(const dbGeoAltitudeRange &ar) throw() {  _altitudeRange = ar ; }

	// inherited from dbExportable
	void exportObj(rsResourceSet &s) const ; // export the class to stream
	void importObj(rsResourceSet &s) ; // import the class from stream
	void exportXML(XMLOstream &xst) const ; // export the class to XML stream
	
	static dbGeoObject *importObject(rsResourceSet &s) ; 
	
	virtual Type type() const = 0 ;
	
	static bool extractObjType(std::istream &is, std::string &objtype) ;


	static void setDialectSRID(const std::string &dialect, const std::string &srid) ; // only ORACLE, PostGRES
	static std::string getDialectSRID(const std::string &dialect) ; // only ORACLE , PostGRES

    int precision() const throw() { return precision_ ; } 
    void precision(int) ;
	virtual const dbGeoPntList &pointList() const ;
	const std::vector<dbGeoPntList> &getRings() const throw() { return _rings ; }

protected :
	dbGeoObject() ;
	dbGeoObject(const dbGeoObject &) ;
	// aim of the operator= is to copy
	// only the protected data members
	dbGeoObject &operator=(const dbGeoObject &) ;

	virtual void exportSpecific(rsResourceSet &s) const ; // export the class to stream
	virtual void importSpecific(rsResourceSet &s) ; 
	virtual void exportSpecificXML(XMLOstream &xst) const ; // export the class to XML stream
	
	void importBase(rsResourceSet &s) ; 
	void exportBase(rsResourceSet &s) const ; 
	void exportBaseXML(XMLOstream &xst) const ; 
	void importRings(rsResourceSet &s) ; 
	void exportRings(rsResourceSet &s) const ; 
	void exportRingsXML(XMLOstream &xst) const ; 

	explicit dbGeoObject(const dbGeoPntList &) ;
	
	// points related method
	virtual void append(const dbGeoPnt &) ;
	const dbGeoPnt *find(const dbGeoPnt &) const ; // PRQA S 2502
	virtual void append(const double, const double) ;
	const dbGeoPnt &operator[](size_t i) const ;
	dbGeoPnt &operator[](size_t i) ;
	virtual void append(const dbGeoPntList &) ;

	// rings
	virtual dbGeoPntList &ring(size_t i) ;
	virtual const dbGeoPntList &ring(size_t i) const ;

private:
	static dbGeoObject *newDbGeoIfx(const std::string &type, const std::string &sqlstring) ;
	static dbGeoObject *newDbGeoPgres(const std::string &type, const std::string &sqlstring) ;
    static void fillDialectSRIDs() ;

public:
	static const double earthRadiusKm ; // nominal= 6366.70702 // PRQA S 2100

protected:
    // PRQA S 2101 5
	dbGeoTimeRange	_timeRange ;
	dbGeoAltitudeRange _altitudeRange ;
	dbGeoPntList _points ;
	std::vector<dbGeoPntList> _rings ;
private:
    int precision_ ;
    
    static Mutex _SRIDs_mutex ;
	static std::map<std::string, std::string> _dialectSRIDs ;
    //compare strings ignoring case.
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbGeoObject) ;
} ;


bool operator==(const dbGeoObject &, const dbGeoObject &) ;
bool operator!=(const dbGeoObject &, const dbGeoObject &) ;

// note: cannot implement the extractor (>>) operator because it need more than just the abstract base class type
std::ostream & operator << (std::ostream &, const dbGeoObject&) ;
exostream & operator << (exostream &, const dbGeoObject&) ; // PRQA S 2072

_ACS_END_NAMESPACE



#endif //_dbGeoObject_H_ 

