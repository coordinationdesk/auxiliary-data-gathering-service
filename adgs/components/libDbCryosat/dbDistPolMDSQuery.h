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
	
	Revision 5.3  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:53  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:21  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.4  2012/11/16 13:01:12  marpas
	realignement of MAIN STREAM
	Introducing ADCS1FixedHeaderFactory class
	
	Revision 2.2  2012/02/14 12:52:45  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.1  2007/04/18 22:12:55  marpas
	Management of dbDistributionpolicies relations with respect to data sets and originators
	

*/

// This file is automatically generated by dbGenerator tool

#ifndef _dbDistPolMDSQuery_H_
#define _dbDistPolMDSQuery_H_

#include <dbGeneralQuery.h>
#include <exException.h>

namespace acs {

class dbCryosatFactory ;
class dbDistPolMDS ;

//
//
//
// class dbDistPolMDSQuery
//
//
//
class dbDistPolMDSQuery :
	public dbGeneralQuery
{
public:
	friend class dbCryosatFactory ;
	typedef dbDistPolMDS _queriedType ;
	
	dbDistPolMDSQuery(dbDistPolMDSQuery const &) ;
	dbDistPolMDSQuery &operator=(dbDistPolMDSQuery const &) ;
	
	virtual dbGeneralQueryIF *clone() const ;
	
	static  std::string staticClassName() ;
	virtual ~dbDistPolMDSQuery() throw() ;

	virtual unsigned long doQuery(dbConnection &, bool incremental=false, bool enabled = true)  ; 

	virtual dbDistPolMDS *operator[](unsigned long i) ; 
	virtual const dbDistPolMDS *operator[](unsigned long i) const ; 

protected:
	explicit dbDistPolMDSQuery(const std::string &dbkey);

private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbDistPolMDSQuery)
} ;

} // namespace

#endif // _dbDistPolMDSQuery_H_
