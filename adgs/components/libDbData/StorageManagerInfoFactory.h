// PRQA S 1050 EOF
/*
	Copyright 2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

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
	virtual ~StorageManagerInfoFactory() throw() ;

	virtual StorageManagerInfo *newStorageManagerInfo(dbConnectionPool &) = 0;
	virtual std::string capabilities() const = 0 ;
		
private:
	void registerToChain() ;
	void unregisterFromChain() throw() ;
	
	// declared but not implemented	
	StorageManagerInfoFactory(const StorageManagerInfoFactory &) ;
	StorageManagerInfoFactory &operator=(const StorageManagerInfoFactory &) ;	
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(StorageManagerInfoFactory)
} ;


_ACS_END_NAMESPACE

#endif /* _StorageManagerInfoFactory_H_ */

