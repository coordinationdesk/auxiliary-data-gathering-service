// PRQA S 1050 EOF
/*
	Copyright 2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA, Inc.
        
*/

#ifndef _IsmServerVersionManagerSingleton_H_
#define _IsmServerVersionManagerSingleton_H_

#include <Singleton.hpp>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;

class IsmServerVersionManagerSingleton : public Singleton <IsmServerVersionManagerSingleton>  // PRQA S 2109, 2153
{ 
    friend IsmServerVersionManagerSingleton* Singleton<IsmServerVersionManagerSingleton>::instance(); // PRQA S 2107

public:

	 /** PackagingAlgoSingleton Exception */
    exDECLARE_EXCEPTION(exIsmServerVersionManagerSingletonException, exException) ; // PRQA S 2131, 2153, 2502 

	/**
	* \brief  Class destructor 
	*/
	virtual ~IsmServerVersionManagerSingleton() throw() ;
	
	/**
	* \brief  get version for serverName, if occurring in map. -1 if not occurring
	* \param serverName The server name
	* \retval version number
	*/
	int getVersion( const std::string & serverName );
	
	/**
	* \brief  set version for serverName
	* \param serverName The server name
	* \param version The server version
	*/
	void setVersion( const std::string & serverName, int version );

protected:
	/**
	* \brief  Class constructor 
	*/
	IsmServerVersionManagerSingleton() ;
	
private:
	/**
	* \brief  Class copy constructor 
	*    
	*/
	IsmServerVersionManagerSingleton(IsmServerVersionManagerSingleton const &) ;
	/**
	* \brief  Class operator = 
	*    
	*/
	IsmServerVersionManagerSingleton &operator=(IsmServerVersionManagerSingleton const &) ;

// data
private:
		
	std::map< std::string, int > _serversMap; // server name -> version
	static Mutex _mapMutex;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(IsmServerVersionManagerSingleton)

};
	
_ACS_END_NAMESPACE
	
#endif //_IsmServerVersionManagerSingleton_H_

