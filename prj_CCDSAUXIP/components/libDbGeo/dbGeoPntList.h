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
	
	Revision 2.13  2013/05/14 18:00:35  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.12  2012/12/14 11:13:41  marpas
	coding best practices
	qa rules
	test improeved
	test coverage
	
	Revision 2.11  2012/02/13 11:15:39  marpas
	refactoring in progress
	
	Revision 2.10  2011/09/29 14:48:10  marpas
	simplifying templates for codecheck
	
	Revision 2.9  2010/09/20 15:18:55  marpas
	autointersection exception notifies interecting segments along with their coordinates
	
	Revision 2.8  2010/05/19 16:06:06  marpas
	Oracle spatial/locator parse completed
	
	Revision 2.7  2010/04/26 11:40:20  marpas
	work in progress Oracle11 Spatial and Locator support implementation
	
	Revision 2.6  2010/04/09 17:23:21  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.5  2009/06/26 09:48:17  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.4  2009/06/23 16:26:40  marpas
	intersect in dbGeoBox fixed
	
	Revision 2.3  2007/12/03 16:43:37  marpas
	polygon comparison fixed
	
	Revision 2.2  2007/06/09 18:45:54  marpas
	bbox method added
	
	Revision 2.1  2006/03/30 10:41:58  marpas
	namespace does not need to be declared again inside
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.7  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.6  2004/06/16 09:44:09  marpas
	measurement methods added
	
	Revision 1.5  2004/05/27 12:55:13  marpas
	some cosmetic changes,
	new method unionWith in dbGeoPntList
	
	Revision 1.4  2003/09/03 13:57:43  paoscu
	Export methods are now const.
	
	Revision 1.3  2003/04/30 13:07:28  marpas
	using namespace std no longer in includes
	
	Revision 1.2  2002/11/29 17:48:46  danalt
	add exportXML, corrected some bugs
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/




#ifndef _dbGeoPntList_H_
#define _dbGeoPntList_H_ 

#include <exException.h>
#include <vector>
#include <iostream>


_ACS_BEGIN_NAMESPACE(acs)

class dbGeoPnt ;
class rsResourceSet ;
class XMLOstream ;
class dbGeoBox ;

/*!
 * \class dbGeoPntList
 * Represents a sorted list of points describing a polyline or a polygon.	
 */
class dbGeoPntList // PRQA S 2109
{
public:

	enum RotationSense {
		Undefined,
		ClockWise,
		CounterClockWise
	} ;


	dbGeoPntList() ;
	dbGeoPntList(const dbGeoPntList&) ;
	virtual ~dbGeoPntList()  throw() ;
	

	dbGeoPntList &operator=(const dbGeoPntList &) ;
	dbGeoPntList &operator+=(const dbGeoPntList &) ;
	dbGeoPntList &operator+=(const dbGeoPnt &) ;
	const dbGeoPnt &operator[](size_t i) const ;
	dbGeoPnt &operator[](size_t i) ;

	std::ostream &getSqlString(std::ostream &, const std::string &dialectName, int precision) const ;
	std::ostream &getSqlString(std::ostream &, const std::string &dialectName, bool closed, int precision) const ;
	virtual void bbox(dbGeoBox &, double latmargin, double lonmargin) const ;

	size_t points() const ; // return the number of points
	void append(const dbGeoPnt &) ;
	void append(double, double) ;
	
	virtual void erase(size_t) ; 
	virtual void erase(const dbGeoPnt &) ;
	
	// sets ring coordinates in counter clock wise
	dbGeoPntList &ccw() ;
	dbGeoPntList &cw() ;
	dbGeoPntList ccw() const ;
	dbGeoPntList cw() const ;
	
	RotationSense isCCW() const ;
	bool autoIntersect(bool closed, size_t *seg1 = 0, size_t *seg2 =0) const ;

	void exportObj(rsResourceSet &) const ;
	void importObj(rsResourceSet &) ;
	void exportXML(XMLOstream &) const ;
	bool intersect(const dbGeoPntList &, bool iAmClosed, bool oLclosed) const ;
	bool inside(const dbGeoPntList &) const ;
	bool intersect(const dbGeoPnt &, const dbGeoPnt &, bool closed) const ;
	size_t  crossing180() const ;
	bool contains(const dbGeoPnt &) const ;
	bool containsAtLeastOne(const dbGeoPntList &) const ;
	double lengthKm(bool closed = false) const ;

	/**
	 * Join the list with the given one describing a new area.
	 * \exception ExecuteException if the given one does not intesects the current one.
	 */	
	void unionWith(dbGeoPntList &) ;
	bool intersectCircle(double,const dbGeoPnt & center, bool closed) const;
	void parseOracle(const std::string &) ;
	static std::istream & parsePostgres(dbGeoPntList &, std::istream &, int &precision) ;
	const std::vector<dbGeoPnt> &getPoints() const throw() { return _points ; }

	
protected:
	bool checkIntersection(size_t, size_t) const ;
	
	void importPoints(rsResourceSet &s) ; 
	void exportPoints(rsResourceSet &s) const; 
	void exportPointsXML(XMLOstream &xst) const; 

private:
	static RotationSense computeRS(dbGeoPntList &) ;
	void revert() ;
	static bool followUntilIntersection(dbGeoPnt &pin, 
									    const dbGeoPntList &pl1, 
										const dbGeoPntList &pl2, 
										size_t &seg1, 
										size_t &seg2, 
										std::vector<dbGeoPnt> &, 
										const dbGeoPnt & stop,
										bool closed) ;
	
	static bool intersect(const dbGeoPnt &p1, const dbGeoPnt &p2, const dbGeoPntList &pl, dbGeoPnt &intersection, size_t &seg, bool closed) ;

private:
	static const int _defaultPointsCapacity = 100 ;
	RotationSense _isCCW ; 
	std::vector<dbGeoPnt> _points ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbGeoPntList) ;
} ;


std::istream &operator >> (std::istream &is, dbGeoPntList& l) ;

bool operator==(const dbGeoPntList &, const dbGeoPntList &) ; 
bool operator!=(const dbGeoPntList &, const dbGeoPntList &) ;


_ACS_END_NAMESPACE




#endif // _dbGeoPntList_H_

