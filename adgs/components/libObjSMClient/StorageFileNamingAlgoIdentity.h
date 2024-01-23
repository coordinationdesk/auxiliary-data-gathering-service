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

#ifndef _StorageFileNamingAlgoIdentity_H_
#define _StorageFileNamingAlgoIdentity_H_ 

#include <StorageFileNamingAlgo.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbPersistentOperatorIF ;

class StorageFileNamingAlgoIdentity :  public StorageFileNamingAlgo // PRQA S 2109
{

public:

	/**
	 * \brief  Class constructor 
	 *    
	 */
	StorageFileNamingAlgoIdentity(const dbPersistent *);

	/**
	 * \brief  Class destructor 
	 */    	
	virtual ~StorageFileNamingAlgoIdentity() throw();
	
	virtual void buildName( dbPersistentOperatorIF*, std::string & filename, std::string & originalname ) ;

private:
	// copy constructor and operator= defined but not implemented
	StorageFileNamingAlgoIdentity(const StorageFileNamingAlgoIdentity &); 
	StorageFileNamingAlgoIdentity &operator=(const StorageFileNamingAlgoIdentity &);

// data
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(StorageFileNamingAlgoIdentity)
};


_ACS_END_NAMESPACE

#endif //_StorageFileNamingAlgoIdentity_H_
