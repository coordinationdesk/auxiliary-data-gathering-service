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
	
	
*/


#ifndef _StorageManagerInfoFactory_H_
#define _StorageManagerInfoFactory_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)

class StorageManagerInfo ;
class dbConnectionPool ;


class StorageManagerInfoFactory // PRQA S 2109
{	
public:
	/*! class NotFoundException declaration 
	 *
	 */
	exDECLARE_EXCEPTION(NotFoundException,exException) ; // PRQA S 2131, 2502

public:
	StorageManagerInfoFactory() ;
	StorageManagerInfoFactory(const StorageManagerInfoFactory &) = delete ;
	StorageManagerInfoFactory &operator=(const StorageManagerInfoFactory &) = delete ;	
	StorageManagerInfoFactory( StorageManagerInfoFactory &&) = delete ;
	StorageManagerInfoFactory &operator=( StorageManagerInfoFactory &&) = delete ;	
	virtual ~StorageManagerInfoFactory() noexcept ;

	virtual StorageManagerInfo *newStorageManagerInfo(dbConnectionPool &) = 0;
	virtual std::string capabilities() const = 0 ;
		
private:
	void registerToChain() ;
	void unregisterFromChain() noexcept ;
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(StorageManagerInfoFactory)
} ;


_ACS_END_NAMESPACE

#endif /* _StorageManagerInfoFactory_H_ */

