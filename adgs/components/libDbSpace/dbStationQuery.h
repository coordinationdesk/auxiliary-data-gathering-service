/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Space Library $

	$Id$

	$Author$

	$Log$
	Revision 1.19  2016/03/26 21:44:07  marpas
	using new db I/F
	
	Revision 1.18  2012/11/19 14:16:56  marpas
	HEAD REALIGNEMENT
	
	Revision 1.16  2012/02/13 16:50:12  marpas
	refactoring in progress
	
	Revision 1.15  2004/08/30 17:49:36  marpas
	doQuery abstract method changed in libDbData
	
	Revision 1.14  2004/04/28 13:09:10  paoscu
	Incremental query support.
	
	Revision 1.13  2004/04/13 15:05:16  marpas
	ace dependencies removed
	dbPersistent::clone method implemented
	dbGeneralQuery::clone method implemented
	XXXQuery::operator= implemented
	
	Revision 1.12  2004/02/04 10:13:22  paoscu
	dbPersistent interface changed.
		

*/

// This file is automatically generated by dbGenerator tool

#ifndef _dbStationQuery_H_
#define _dbStationQuery_H_

#include <dbGeneralQuery.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbStation ;

//
//
//
// class dbStationQuery
//
//
//
class dbStationQuery : // PRQA S 2109
	public dbGeneralQuery
{
public:
	friend class dbSpaceFactory ; // PRQA S 2107
	typedef dbStation _queriedType ;
	
	virtual dbGeneralQueryIF *clone() const ; // PRQA S 2502
	
	static  std::string staticClassName() ;
	virtual ~dbStationQuery() throw() ;

	virtual unsigned long doQuery(dbConnection &, bool incremental=false, bool enabled = true)  ; 

	virtual dbStation *operator[](unsigned long i) ; 
	virtual const dbStation *operator[](unsigned long i) const ; 

protected:
	explicit dbStationQuery(const std::string &dbkey);
	dbStationQuery(const dbStationQuery &) ;
	dbStationQuery &operator=(const dbStationQuery &) ;  // operator =
private:
   	dbStationQuery() ; // not implemented

private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbStationQuery)
} ;

_ACS_END_NAMESPACE

#endif // _dbStationQuery_H_
