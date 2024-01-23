// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.3  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:55  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:22  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.1  2012/02/14 12:52:46  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.7  2004/08/30 17:51:13  paoscu
	doQuery abstract I/F changed
	
	Revision 1.6  2004/04/28 13:08:40  paoscu
	Incremental query support.
	
	Revision 1.5  2004/04/13 16:51:54  paoscu
	_queriedType added.
	Copy constructor and operator= added
	clone() method added.
	
	Revision 1.4  2004/02/04 10:12:35  paoscu
	Library generated again. dbPersistent interface changed.
		

*/

// This file is automatically generated by dbGenerator tool

#ifndef _dbDistributionViewQuery_H_
#define _dbDistributionViewQuery_H_

#include <dbGeneralQuery.h>
#include <exException.h>

namespace acs {

class dbCryosatFactory ;
class dbDistributionView ;

//
//
//
// class dbDistributionViewQuery
//
//
//
class dbDistributionViewQuery :
	public dbGeneralQuery
{
public:
	friend class dbCryosatFactory ;
	typedef dbDistributionView _queriedType ;
	
	dbDistributionViewQuery(dbDistributionViewQuery const &) ;
	dbDistributionViewQuery &operator=(dbDistributionViewQuery const &) ;
	
	virtual dbGeneralQueryIF *clone() const ;
	
	static  std::string staticClassName() ;
	virtual ~dbDistributionViewQuery() throw() ;

	virtual unsigned long doQuery(dbConnection &, bool incremental=false, bool enabled = true)  ; 

	virtual dbDistributionView *operator[](unsigned long i) ; 
	virtual const dbDistributionView *operator[](unsigned long i) const ; 

protected:
	explicit dbDistributionViewQuery(const std::string &dbkey);

private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbDistributionViewQuery)
} ;

} // namespace

#endif // _dbDistributionViewQuery_H_
