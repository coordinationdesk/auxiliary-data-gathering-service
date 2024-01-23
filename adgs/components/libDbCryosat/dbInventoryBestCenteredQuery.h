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
	
	Revision 5.5  2016/07/04 15:06:08  damdec
	CSGACQ-82: compatibility with Oracle introduced.
	
	Revision 5.4  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.3  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:56  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:24  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.1  2012/02/14 12:52:47  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2004/12/30 22:47:59  paoscu
	headers fixed
	
	Revision 1.5  2004/08/30 17:51:13  paoscu
	doQuery abstract I/F changed
	
	Revision 1.4  2004/04/28 13:08:40  paoscu
	Incremental query support.
	
	Revision 1.3  2004/04/13 16:51:54  paoscu
	_queriedType added.
	Copy constructor and operator= added
	clone() method added.
	
	Revision 1.2  2003/10/21 13:05:24  paoscu
	staticClassName method added.
	
	Revision 1.1  2003/04/17 15:43:33  paoscu
	dbInventoryBestCenteredQuery added.
	
		

*/

#ifndef _dbInventoryBestCenteredQuery_H_
#define _dbInventoryBestCenteredQuery_H_

#include <dbInventoryObjectSpecialQuery.h>


_ACS_BEGIN_NAMESPACE(acs)

class DateTime;
class dbConnection;

class dbInventoryBestCenteredQuery : // PRQA S 2109, 2153
	public dbInventoryObjectSpecialQuery
{
	friend class dbCryosatFactory ; // PRQA S 2107
public:

	dbInventoryBestCenteredQuery(dbInventoryBestCenteredQuery const &) ;
	dbInventoryBestCenteredQuery &operator=(dbInventoryBestCenteredQuery const &) ;

	virtual dbGeneralQueryIF *clone() const ;	// PRQA S 2502


	static  std::string staticClassName() ;

	virtual ~dbInventoryBestCenteredQuery() throw() ;

	virtual unsigned long doQuery(dbConnection &, bool incremental=false, bool enabled = true)  ; 

protected:
	explicit dbInventoryBestCenteredQuery(const std::string &dbkey);

private:
    dbInventoryBestCenteredQuery() ; // not implemented
	
	std::string sql_extract(const std::string & what, const std::string & from);

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbInventoryBestCenteredQuery)

};


_ACS_END_NAMESPACE

#endif // _dbInventoryBestCenteredQuery_H_
