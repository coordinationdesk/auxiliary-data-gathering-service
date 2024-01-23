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

#ifndef _StorageFileNamingAlgo_H_
#define _StorageFileNamingAlgo_H_ 

#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbPersistent ;
class dbPersistentOperatorIF ;

class StorageFileNamingAlgo // PRQA S 2109
{

public:

	exDECLARE_EXCEPTION(StorageFileNamingAlgoException, exException) ; // base exception // PRQA S 2131, 2502
	
	/**
	 * \brief  Class constructor 
	 *    
	 */
	StorageFileNamingAlgo(const dbPersistent *);

	/**
	 * \brief  Class destructor 
	 */    	
	virtual ~StorageFileNamingAlgo() throw();


	/**
	 * \brief build name of file on storage
	 * \param 
	 */
	virtual void buildName( dbPersistentOperatorIF*, std::string & filename, std::string & originalname ) = 0;

protected:
	dbPersistent * getObject() const ;
	
private:
	// copy constructor and operator= defined but not implemented
	StorageFileNamingAlgo(const StorageFileNamingAlgo &); 
	StorageFileNamingAlgo &operator=(const StorageFileNamingAlgo &);


// data
private:
	dbPersistent * _object ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(StorageFileNamingAlgo)
};




_ACS_END_NAMESPACE

#endif //_StorageFileNamingAlgo_H_
