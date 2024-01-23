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
	Revision 5.1  2014/05/09 11:39:55  marpas
	coding best practices applied
	qa & c++ warnings fixed
	last throwing macros for exceptions adopted
	
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
	
	Revision 2.11  2012/02/13 11:15:40  marpas
	refactoring in progress
	
	Revision 2.10  2011/09/29 14:48:10  marpas
	simplifying templates for codecheck
	
	Revision 2.9  2010/05/19 16:06:07  marpas
	Oracle spatial/locator parse completed
	
	Revision 2.8  2010/04/26 11:40:20  marpas
	work in progress Oracle11 Spatial and Locator support implementation
	
	Revision 2.7  2010/04/09 17:23:22  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.6  2009/06/26 09:48:18  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.5  2009/06/24 09:27:54  marpas
	ntersections still in progress
	
	Revision 2.4  2009/06/23 13:29:43  marpas
	isInsideGeo and intersectGeo method can be invoked on CONST objects
	
	Revision 2.3  2009/06/05 09:31:18  crivig
	 isInsideGeo and intersectGeo methods added
	
	Revision 2.2  2007/06/09 18:45:54  marpas
	bbox method added
	
	Revision 2.1  2006/03/30 10:41:58  marpas
	namespace does not need to be declared again inside
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.9  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.8  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.7  2004/05/27 12:55:13  marpas
	some cosmetic changes,
	new method unionWith in dbGeoPntList
	
	Revision 1.6  2003/09/03 13:57:43  paoscu
	Export methods are now const.
	
	Revision 1.5  2003/05/22 09:20:55  marpas
	ring interface is more homogeneous now
	
	Revision 1.4  2003/04/30 13:07:28  marpas
	using namespace std no longer in includes
	
	Revision 1.3  2003/03/17 16:42:48  marpas
	enumerant type added
	
	Revision 1.2  2002/11/29 17:48:46  danalt
	add exportXML, corrected some bugs
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/


#ifndef _dbGeoRing_H_
#define _dbGeoRing_H_

#include <dbGeoObject.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbGeoBox ;
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/  dbGeoRing     _/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

class dbGeoRing : public dbGeoObject
{

public :

	explicit dbGeoRing(const dbGeoPntList &) ;
	dbGeoRing() ;
	dbGeoRing(const dbGeoRing &) ;
	virtual ~dbGeoRing()  throw() ;
	 
	dbGeoRing &operator=(const dbGeoRing &) ;
	
	// inherited from dbGeoObject
	virtual dbGeoObject* clone() const ; // PRQA S 2502
	virtual std::ostream &getSqlString(std::ostream &, const std::string &dialectName) const ; // PRQA S 2121
	virtual void bbox(dbGeoBox &, double latmargin, double lonmargin) const ;

	virtual void importSpecific(rsResourceSet &) ;
	// rings: only the element 0 is valid
	virtual const dbGeoPntList &ring(size_t i) const ;
	virtual dbGeoPntList &ring(size_t i) ;
	
	using dbGeoObject::append ;
	
	const dbGeoPnt &operator[](size_t i) const ;
	dbGeoPnt &operator[](size_t i) ;
	
	bool intersect(const dbGeoRing &) const ;
	size_t crossing180() const ;
	bool contains(const dbGeoPnt &) const ;
	virtual bool intersectGeo(const dbGeoObject  &) const;
	virtual bool isInsideGeo(const dbGeoObject &) const;
	virtual std::string getWKTString(bool lonFirst = true) const;
	
	virtual Type type() const ;

	void parseOracle(const std::string &, int offs, int etype, int interpr) ;
protected:

	// inherited from dbGeoObject
	virtual void exportSpecific(rsResourceSet &) const;
	virtual void exportSpecificXML(XMLOstream &) const;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbGeoRing) ;
} ;

std::istream  &operator >> (std::istream &is, dbGeoRing& r) ;


_ACS_END_NAMESPACE

#endif // _dbGeoRing_H_


