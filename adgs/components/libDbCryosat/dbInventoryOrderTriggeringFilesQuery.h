// PRQA S 1050 EOF
/*

	Copyright 2012-2016, Advanced Computer Systems , Inc.
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
	
	Revision 5.4  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.3  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:26  marfav
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
	
	Revision 2.1  2012/03/07 17:30:46  marpas
	new special query: dbInventoryOrderTriggeringFilesQuery added
	
		

*/

#ifndef _dbInventoryOrderTriggeringFilesQuery_H_
#define _dbInventoryOrderTriggeringFilesQuery_H_

#include <dbInventoryObjectSpecialQuery.h>


_ACS_BEGIN_NAMESPACE(acs)

class DateTime;
class dbConnection;

class dbInventoryOrderTriggeringFilesQuery : // PRQA S 2109, 2153
	public dbInventoryObjectSpecialQuery
{
	friend class dbCryosatFactory ; // PRQA S 2107
public:

	dbInventoryOrderTriggeringFilesQuery(dbInventoryOrderTriggeringFilesQuery const &) ;
	dbInventoryOrderTriggeringFilesQuery &operator=(dbInventoryOrderTriggeringFilesQuery const &) ;

	virtual dbGeneralQueryIF *clone() const ;// PRQA S 2502	


	static  std::string staticClassName() ;

	virtual ~dbInventoryOrderTriggeringFilesQuery() throw() ;

    virtual unsigned long doQuery(dbConnection &, bool incremental=false, bool enabled = true)  ; 


	virtual void resetParameters() ; 

private: 
	explicit dbInventoryOrderTriggeringFilesQuery(const std::string &dbkey) ;
private: 
	dbInventoryOrderTriggeringFilesQuery() ; // not implemented

private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbInventoryOrderTriggeringFilesQuery)

};


_ACS_END_NAMESPACE

#endif // _dbInventoryOrderTriggeringFilesQuery_H_
