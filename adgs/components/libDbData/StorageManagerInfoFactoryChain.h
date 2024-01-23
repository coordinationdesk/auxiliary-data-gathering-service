// PRQA S 1050 EOF
/*
	Copyright 1995-2017, Advanced Computer Systems , Inc.
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


#ifndef _StorageManagerInfoFactoryChain_H_
#define _StorageManagerInfoFactoryChain_H_ 

#include <acs_c++config.hpp>
#include <Singleton.hpp>
#include <exException.h>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;



class StorageManagerInfoFactory ;
class StorageManagerInfo ;
class dbConnectionPool ;

class StorageManagerInfoFactoryChain : public Singleton<StorageManagerInfoFactoryChain > // PRQA S 2109, 2153
{
	friend StorageManagerInfoFactoryChain* Singleton< StorageManagerInfoFactoryChain >::instance(); // PRQA S 2107
public:	
	virtual ~StorageManagerInfoFactoryChain() throw() ;

	// registers a new factory
	void registerFactory(StorageManagerInfoFactory *) ;
	// unregisters a new factory
	void unregisterFactory(StorageManagerInfoFactory *) throw() ;


	StorageManagerInfo *newStorageManagerInfo(dbConnectionPool &);
	std::string capabilities() const;
	
protected:
	StorageManagerInfoFactoryChain() ;

private:
	// declared but not implemented	
	StorageManagerInfoFactoryChain(const StorageManagerInfoFactoryChain &) ;
	StorageManagerInfoFactoryChain &operator=(const StorageManagerInfoFactoryChain &) ; 

private:
	std::vector<StorageManagerInfoFactory *> _factories;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(StorageManagerInfoFactoryChain)
} ;


_ACS_END_NAMESPACE

#endif
