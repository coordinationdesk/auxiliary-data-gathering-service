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

#ifndef _StorageFileNamingAlgoPdsLike_H_
#define _StorageFileNamingAlgoPdsLike_H_ 

#include <StorageFileNamingAlgo.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbPersistentOperatorIF ;

class StorageFileNamingAlgoPdsLike :  public StorageFileNamingAlgo // PRQA S 2109
{

public:

	/**
	 * \brief  Class constructor 
	 *    
	 */
	StorageFileNamingAlgoPdsLike(const dbPersistent *);

	/**
	 * \brief  Class destructor 
	 */    	
	virtual ~StorageFileNamingAlgoPdsLike() throw();
	
	virtual void buildName( dbPersistentOperatorIF*, std::string & filename, std::string & originalname ) ;

private:
	// copy constructor and operator= defined but not implemented
	StorageFileNamingAlgoPdsLike(const StorageFileNamingAlgoPdsLike &); 
	StorageFileNamingAlgoPdsLike &operator=(const StorageFileNamingAlgoPdsLike &);

// data
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(StorageFileNamingAlgoPdsLike)
};


_ACS_END_NAMESPACE

#endif //_StorageFileNamingAlgoPdsLike_H_
