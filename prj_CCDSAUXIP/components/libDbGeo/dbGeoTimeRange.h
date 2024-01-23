// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Database Geo Object Library $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2015/04/28 09:59:07  marpas
	fixing problem in bbox methods when timerange was not filled.
	
	Revision 5.2  2014/05/09 11:39:55  marpas
	coding best practices applied
	qa & c++ warnings fixed
	last throwing macros for exceptions adopted
	
	Revision 5.1  2014/04/08 16:47:21  marpas
	adopting libDateTime 5.13
	
	Revision 5.0  2013/07/11 16:07:13  marpas
	libException 5.x standards adopted
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.8  2013/05/14 18:00:35  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.7  2012/12/14 11:13:41  marpas
	coding best practices
	qa rules
	test improeved
	test coverage
	
	Revision 2.6  2012/02/13 11:15:40  marpas
	refactoring in progress
	
	Revision 2.5  2011/09/29 14:48:10  marpas
	simplifying templates for codecheck
	
	Revision 2.4  2010/05/05 10:21:33  marpas
	DEBUG added
	bug fixed in extraction operator
	
	Revision 2.3  2010/04/09 17:23:22  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.2  2009/06/26 09:48:18  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.1  2006/03/30 10:41:58  marpas
	namespace does not need to be declared again inside
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.11  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.10  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.9  2004/08/03 13:49:57  marfav
	dbGeoTimeRange now inherits from libDateTime TimeRange class
	
	Revision 1.8  2004/05/11 08:19:34  marpas
	isFilled implemented
	
	Revision 1.7  2004/05/10 17:15:32  ivafam
	Method isFilled throw Exception.
	
	Revision 1.6  2004/02/04 10:17:50  paoscu
	Better doxygen comments.
	
	Revision 1.5  2003/09/03 13:57:43  paoscu
	Export methods are now const.
	
	Revision 1.4  2003/04/30 13:07:28  marpas
	using namespace std no longer in includes
	
	Revision 1.3  2003/02/13 14:28:00  marpas
	contains method added
	
	Revision 1.2  2002/11/29 17:48:46  danalt
	add exportXML, corrected some bugs
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/


#ifndef _dbGeoTimeRange_H_
#define _dbGeoTimeRange_H_

#include <DateTime.h>
#include <TimeRange.h>
#include <exException.h>
#include <iostream>


_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ;
class XMLOstream ;

class dbGeoTimeRange : public TimeRange // PRQA S 2109, 2153
{

public:

	dbGeoTimeRange() ;
	dbGeoTimeRange(const TimeRange &) ; // PRQA S 2180
	dbGeoTimeRange(const DateTime &, const DateTime &) ;
	virtual ~dbGeoTimeRange() throw() ;
	dbGeoTimeRange(const dbGeoTimeRange&) ;
	dbGeoTimeRange &operator=(const dbGeoTimeRange&) ;
    
	void exportObj(rsResourceSet &) const;
	void importObj(rsResourceSet &) ;
	void exportXML(XMLOstream &) const;
public:
	static int getDebugLevel() throw() { return exCDebugLevel.getDebug() ; }
private:
    friend std::istream &operator >> (std::istream &is, dbGeoTimeRange& b) ; // PRQA S 2107
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbGeoTimeRange) ;

} ;

inline bool operator==(const dbGeoTimeRange & a, const dbGeoTimeRange & b) // PRQA S 2134
{ const TimeRange &at = a ; const TimeRange &bt = b ; return (at == bt) ; }

inline bool operator!=(const dbGeoTimeRange & a, const dbGeoTimeRange & b) { return not ( a == b ) ; }  // PRQA S 2134

std::istream &operator >> (std::istream &is, dbGeoTimeRange& b) ;


_ACS_END_NAMESPACE




#endif

