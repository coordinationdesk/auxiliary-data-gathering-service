// PRQA S 1050 EOF
/*
	Copyright 2020, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.
	
*/ 


#ifndef _StorageFileNamingAlgoSingleton_H_
#define _StorageFileNamingAlgoSingleton_H_

#include <StorageFileNamingAlgo.h>
#include <Singleton.hpp>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;

class dbPersistent ;

class StorageFileNamingAlgoSingleton : public Singleton<StorageFileNamingAlgoSingleton> // PRQA S 2109, 2153
{
    friend StorageFileNamingAlgoSingleton* Singleton<StorageFileNamingAlgoSingleton>::instance(); // PRQA S 2107
public:
	/*! class KeyNotFound declaration */
	exDECLARE_EXCEPTION(KeyNotFound,exException) ; // PRQA S 2131, 2502 
	 /** StorageFileNamingAlgoSingleton Exception */
    exDECLARE_EXCEPTION(exStorageFileNamingAlgoSingletonException, exException) ; // PRQA S 2131, 2153, 2502 

	void registerFactory(std::string const &, StorageFileNamingAlgo* (*)(const dbPersistent *) );

	void unregisterFactory(std::string const &, StorageFileNamingAlgo* (*)(const dbPersistent *) );
	
	/**
	 * \brief  Return an object derived from SatelliteMissionAssociator class
	 * \param The map key
	 */    	
	StorageFileNamingAlgo *newStorageFileNamingAlgo(std::string const &, const dbPersistent *) const;
    virtual ~StorageFileNamingAlgoSingleton() throw() ;
protected:

	StorageFileNamingAlgoSingleton();

private:

	StorageFileNamingAlgoSingleton(const StorageFileNamingAlgoSingleton &); // not implemented

	StorageFileNamingAlgoSingleton& operator=(const StorageFileNamingAlgoSingleton &); // not implemented

// data
private:
	std::map<std::string, StorageFileNamingAlgo* (*)(const dbPersistent *)> _map;


	ACS_CLASS_DECLARE_DEBUG_LEVEL(StorageFileNamingAlgoSingleton)

};

_ACS_END_NAMESPACE

#endif //_StorageFileNamingAlgoSingleton_H_
