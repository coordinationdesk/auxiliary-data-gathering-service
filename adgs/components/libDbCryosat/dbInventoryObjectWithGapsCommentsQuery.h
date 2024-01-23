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
	
	Revision 1.7  2004/12/30 22:47:59  paoscu
	headers fixed
	
	Revision 1.6  2004/08/30 17:51:13  paoscu
	doQuery abstract I/F changed
	
	Revision 1.5  2004/07/13 15:15:44  paoscu
	using namespace std no longer in includes
	exDbPersistent is an abstract class
	
	Revision 1.4  2004/04/28 13:08:40  paoscu
	Incremental query support.
	
	Revision 1.3  2004/04/13 16:51:54  paoscu
	_queriedType added.
	Copy constructor and operator= added
	clone() method added.
	
	Revision 1.2  2003/10/21 13:05:24  paoscu
	staticClassName method added.
	
	Revision 1.1  2003/05/02 16:05:43  andcol
	imported
	
	Revision 1.7  2003/02/03 13:30:21  danalt
	aligned with DB mandatory fields and lengths + loadRow + output null if not set
		

*/

// This file is automatically generated by dbGenerator tool

#ifndef _dbInventoryObjectWithGapsCommentsQuery_H_
#define _dbInventoryObjectWithGapsCommentsQuery_H_

#include <dbGeneralQuery.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbInventoryObjectWithGapsComments ;
class dbCryosatFactory;

//
//
//
// class dbInventoryObjectWithGapsCommentsQuery
//
//
//
class dbInventoryObjectWithGapsCommentsQuery : // PRQA S 2109, 2153
	public dbGeneralQuery
{
	friend class dbCryosatFactory; // PRQA S 2107
public:
	typedef dbInventoryObjectWithGapsComments _queriedType ;
	
	dbInventoryObjectWithGapsCommentsQuery(dbInventoryObjectWithGapsCommentsQuery const &) ;
	dbInventoryObjectWithGapsCommentsQuery &operator=(dbInventoryObjectWithGapsCommentsQuery const &) ;
	
	virtual dbGeneralQueryIF *clone() const ; // PRQA S 2502
	
	static  std::string staticClassName() ;

	virtual ~dbInventoryObjectWithGapsCommentsQuery() throw() ;

	virtual unsigned long doQuery(dbConnection &, bool incremental=false, bool enabled = true)  ; 


	virtual dbInventoryObjectWithGapsComments *operator[](unsigned long i) ; 
	virtual const dbInventoryObjectWithGapsComments *operator[](unsigned long i) const ; 

protected:
	explicit dbInventoryObjectWithGapsCommentsQuery(const std::string &dbkey);

private:
    dbInventoryObjectWithGapsCommentsQuery() ; // not implemented
private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbInventoryObjectWithGapsCommentsQuery)
} ;

_ACS_END_NAMESPACE

#endif // _dbInventoryObjectWithGapsCommentsQuery_H_
