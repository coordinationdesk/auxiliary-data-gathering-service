// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
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
	
	Revision 5.4  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.3  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.2  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.1  2016/03/14 16:57:28  marpas
	honor dbGeneralQuery::offset(...)
	coding best practices applied
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:59  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:27  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.1  2012/02/14 12:52:48  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/06/20 10:36:11  marpas
	gaps management added
	
		

*/


#ifndef _dbInventoryObjectWithGapsQuery_H_
#define _dbInventoryObjectWithGapsQuery_H_

#include <dbGeneralQuery.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbInventoryObjectWithGaps ;
class dbCryosatFactory;

//
//
//
// class dbInventoryObjectWithGapsQuery
//
//
//
class dbInventoryObjectWithGapsQuery : // PRQA S 2109, 2153
	public dbGeneralQuery
{
	friend class dbCryosatFactory; // PRQA S 2107
public:
	typedef dbInventoryObjectWithGaps _queriedType ;
	
	dbInventoryObjectWithGapsQuery(dbInventoryObjectWithGapsQuery const &) ;
	dbInventoryObjectWithGapsQuery &operator=(dbInventoryObjectWithGapsQuery const &) ;
	
	virtual dbGeneralQueryIF *clone() const ; // PRQA S 2502
	
	static  std::string staticClassName() ;

	virtual ~dbInventoryObjectWithGapsQuery() throw() ;

	virtual unsigned long doQuery(dbConnection &, bool incremental=false, bool enabled = true)  ; 


	virtual dbInventoryObjectWithGaps *operator[](unsigned long i) ; 
	virtual const dbInventoryObjectWithGaps *operator[](unsigned long i) const ; 

protected:
	explicit dbInventoryObjectWithGapsQuery(const std::string &dbkey);
private:
    dbInventoryObjectWithGapsQuery() ; // not implemented
private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbInventoryObjectWithGapsQuery)
} ;

_ACS_END_NAMESPACE

#endif // _dbInventoryObjectWithGapsQuery_H_
