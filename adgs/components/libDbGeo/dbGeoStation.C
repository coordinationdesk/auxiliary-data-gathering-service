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
	Revision 5.0  2013/07/11 16:07:12  marpas
	libException 5.x standards adopted
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.5  2013/05/14 18:00:35  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.4  2013/03/29 18:03:21  marpas
	libGraphicAttributes deprecated
	
	Revision 2.3  2012/02/13 11:15:40  marpas
	refactoring in progress
	
	Revision 2.2  2010/04/09 17:23:22  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.1  2009/06/26 09:48:18  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.3  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.2  2003/04/30 17:21:18  marpas
	using namespace std no longer into acs includes
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/


#include <dbGeoStation.h>
#include <dbGeoPnt.h>

#include <rsResourceSet.h>
#include <rsPushNSpace.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/  dbGeoStation   _/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbGeoStation)

dbGeoStation::dbGeoStation(const string &nm) :
	_ring(),
    _enabled(false),
	_name(nm)
{
}

dbGeoStation::dbGeoStation(const dbGeoStation &s) :
	_ring(s._ring),
	_enabled(s._enabled),
	_name(s._name)
{
	// empty
}

dbGeoStation & dbGeoStation::operator=(const dbGeoStation &s) 
{
	if (this != &s) { // prevent a = a 
		_ring = s._ring ;
		_name = s._name ;
		_enabled = s._enabled ;
	}
	
	return *this ;
}

dbGeoStation::~dbGeoStation() throw() 
{
}

const string &dbGeoStation::name() const throw() 
{
	return *this ; // PRQA S 3050
}

const dbGeoObject &dbGeoStation::obj() const throw()  // PRQA S 4120
{
	return _ring ;
}

void dbGeoStation::load(const string &file) 
{
	ifstream in(file.c_str()) ;
	
	int num ; // load coordinates number // PRQA S 4101
	in >> num ;
	
	for (long i=0; i < num; i++) { // load each coord couple
		double lat ; // PRQA S 4101 2
        double lon ;
		in >> lon >> lat ;
		if (in.fail()) {
			ACS_THROW(exIOException("Reading: " + file, errno)) ; // PRQA S 3081
		}
		
		dbGeoPnt p(lat,lon) ;
		_ring.append(p) ;
	}
}

void dbGeoStation::enable() throw() 
{
	_enabled = true ;
}
 
void dbGeoStation::disable() throw() 
{
	_enabled = false ;
}

bool dbGeoStation::isEnabled() const throw() // PRQA S 4120
{
	return _enabled ;
}

dbGeoStation::operator const string & () const throw() 
{
	return _name ;
}


_ACS_END_NAMESPACE
	
	


