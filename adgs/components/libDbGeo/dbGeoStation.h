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
	Revision 2.5  2013/03/29 18:03:21  marpas
	libGraphicAttributes deprecated
	
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
	
	Revision 1.5  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.4  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.3  2003/04/30 17:21:18  marpas
	using namespace std no longer into acs includes
	
	Revision 1.2  2003/04/30 13:07:28  marpas
	using namespace std no longer in includes
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/


#ifndef _dbGeoStation_H_
#define _dbGeoStation_H_

#include <dbGeoRing.h>

_ACS_BEGIN_NAMESPACE(acs)

//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/  dbGeoStation   _/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

class dbGeoStation // PRQA S 2109
{

public :
	explicit dbGeoStation(const std::string &) ;
	dbGeoStation(const dbGeoStation &) ;
	dbGeoStation &operator=(const dbGeoStation & ) ;
	 ~dbGeoStation()  throw() ;
	
	const std::string &name() const throw() ;
	const dbGeoObject &obj() const throw() ;
	
	void load(const std::string &) ;
	
	void enable() throw() ; 
	void disable() throw() ;
	bool isEnabled() const throw() ;
	
	operator const std::string&() const throw() ; // PRQA S 2181
private:
	// default ctor declared but not implemented
	dbGeoStation() ;	
	
private:

	dbGeoRing _ring ;
	bool _enabled ;
	std::string _name ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbGeoStation) ;
} ;

_ACS_END_NAMESPACE

#endif // _dbGeoStation_H_


