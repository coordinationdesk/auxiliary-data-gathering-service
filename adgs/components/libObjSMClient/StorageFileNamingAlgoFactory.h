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


#ifndef _StorageFileNamingAlgoFactory_H_
#define _StorageFileNamingAlgoFactory_H_ 

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbPersistent;

class StorageFileNamingAlgo;

class StorageFileNamingAlgoFactory // PRQA S 2109
{
public:

	StorageFileNamingAlgoFactory();

	~StorageFileNamingAlgoFactory() throw() ;

	static StorageFileNamingAlgo* buildStorageFileNamingAlgoIdentity(const dbPersistent *);		
	static StorageFileNamingAlgo* buildStorageFileNamingAlgoPdsLike(const dbPersistent *);				

private:
    StorageFileNamingAlgoFactory(const StorageFileNamingAlgoFactory &) ; // not implemented
    StorageFileNamingAlgoFactory& operator= (const StorageFileNamingAlgoFactory &) ; // not implemented
private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(StorageFileNamingAlgoFactory)

};

_ACS_END_NAMESPACE

#endif // _StorageFileNamingAlgoFactory_H_

