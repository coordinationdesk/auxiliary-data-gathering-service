// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
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
	
	Revision 2.4  2013/05/14 18:00:35  marpas
	implementig PostGRES/PostGIS support
	
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


#include <dbQLookPnt.h>

_ACS_BEGIN_NAMESPACE(acs)


dbQLookPnt::dbQLookPnt(const dbQLookPnt &p) throw() :
	_span(p._span),
	_line(p._line)
{
}

dbQLookPnt::~dbQLookPnt() throw() { }

dbQLookPnt &dbQLookPnt::operator=(const dbQLookPnt &p) throw() 
{
	if (this != &p) {
		_span = p._span ;
		_line = p._line ;
	}
	
	return *this ;
}

bool operator==(const dbQLookPnt &a, const dbQLookPnt &b) throw() // PRQA S 4020
{
    if ( ( a.line() > b.line() ) || ( a.line() < b.line() ) ) {
        return false ;
    }
    if ( ( a.span() > b.span() ) || ( a.span() < b.span() ) ) {
        return false ;
    }
	return true ;
}

bool operator!=(const dbQLookPnt &a, const dbQLookPnt &b) throw() { return not ( a == b ) ; }



_ACS_END_NAMESPACE


