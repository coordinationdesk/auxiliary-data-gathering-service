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
	
	Revision 5.2  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.1  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.0  2015/02/17 11:25:26  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:31:02  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:30  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.2  2012/02/14 12:52:50  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.1  2008/10/29 11:57:49  marpas
	dbOrderValidator added
	

*/

#ifndef _dbOrderValidator_H_
#define _dbOrderValidator_H_ 

#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)
class dbOrder ; 
class dbOrderValidator
{		
public:
	dbOrderValidator(const dbOrder &);
	~dbOrderValidator() throw() ;
	void validate() ;
private:
	dbOrderValidator(const dbOrderValidator &) ;
	dbOrderValidator & operator=(const dbOrderValidator &) ;

private:
	const dbOrder & _theOrder ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbOrderValidator)
};

_ACS_END_NAMESPACE

#endif /* _dbOrderValidator_H_ */
