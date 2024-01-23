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
	Revision 5.0  2013/07/11 16:07:13  marpas
	libException 5.x standards adopted
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.5  2013/05/14 18:00:35  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.4  2012/12/14 11:13:41  marpas
	coding best practices
	qa rules
	test improeved
	test coverage
	
	Revision 2.3  2012/02/13 11:15:40  marpas
	refactoring in progress
	
	Revision 2.2  2010/04/09 17:23:22  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.1  2009/06/26 09:48:18  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.2  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/




#ifndef _dbScenePnt_H_
#define _dbScenePnt_H_ 

#include <acs_c++config.hpp>

_ACS_BEGIN_NAMESPACE(acs)

//
//
//
//	class dbScenePnt
//
//
//

class dbScenePnt
{
public:
	explicit dbScenePnt(double p=0, double l=0) throw() :
	    _pixel(p),
	    _line(l)
    {
    }
	dbScenePnt(const dbScenePnt &) throw() ;
	~dbScenePnt() throw() ;

	dbScenePnt &operator=(const dbScenePnt &) throw() ;
		
	double pixel() const throw() { return _pixel ; } 
	double line() const throw() { return _line ; } 
	
	void pixel(double v ) throw() { _pixel = v ; }
	void line(double v ) throw() { _line = v ; }

private:
	double _pixel ;
	double _line ;
} ;

bool operator==(const dbScenePnt &, const dbScenePnt &) throw() ;
bool operator!=(const dbScenePnt &, const dbScenePnt &) throw() ;


_ACS_END_NAMESPACE

#endif // _dbScenePnt_H_

