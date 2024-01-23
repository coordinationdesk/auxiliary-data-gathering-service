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
	StorageManagerInfoFactoryChain(const StorageManagerInfoFactoryChain &) = delete ;
	StorageManagerInfoFactoryChain &operator=(const StorageManagerInfoFactoryChain &) = delete ;
	StorageManagerInfoFactoryChain( StorageManagerInfoFactoryChain &&) = delete ;
	StorageManagerInfoFactoryChain &operator=( StorageManagerInfoFactoryChain &&) = delete ;
	virtual ~StorageManagerInfoFactoryChain() noexcept ;

	// registers a new factory
	void registerFactory(StorageManagerInfoFactory *) ;
	// unregisters a new factory
	void unregisterFactory(StorageManagerInfoFactory *) noexcept ;


	StorageManagerInfo *newStorageManagerInfo(dbConnectionPool &);
	std::string capabilities() const;
	
protected:
	StorageManagerInfoFactoryChain() ;

private:
	std::vector<StorageManagerInfoFactory *> _factories;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(StorageManagerInfoFactoryChain)
} ;


_ACS_END_NAMESPACE

#endif
