// PRQA S 1050 EOF
/*

	Copyright 2017-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2017/11/07 16:18:07  lucio.pulvirenti
	S2PDGS-1843: classes added.
	
	
		
*/


#ifndef _StorageManagerInfo_H_
#define _StorageManagerInfo_H_ 

#include <acs_c++config.hpp>
#include <exException.h>



_ACS_BEGIN_NAMESPACE(acs)

class dbConnectionPool;

//
//
//
// class StorageManagerInfo
//
//
//
class StorageManagerInfo // PRQA S 2109 
{
public:	

	/*! class SaveException declaration 
	 *
	 * This exception is thrown by uploading() if there was an error
	 * while saving record in database.
	 */
	exDECLARE_EXCEPTION(StorageManagerInfoExc,exException) ; // PRQA S 2131, 2153, 2502

	
	explicit StorageManagerInfo(dbConnectionPool &);
	StorageManagerInfo() = delete ;
	StorageManagerInfo(const StorageManagerInfo &) = delete ;
	StorageManagerInfo &operator=(const StorageManagerInfo &) = delete ;

	virtual ~StorageManagerInfo() noexcept ;

	virtual void getAvailableStorage( std::map<std::string, std::vector<std::pair<std::string, long long int> > > &) = 0;
	
protected:
    dbConnectionPool &getPool() ;
	
private:
	dbConnectionPool &_pool ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(StorageManagerInfo)
	
} ; 

_ACS_END_NAMESPACE

#endif // _StorageManagerInfo_H_
