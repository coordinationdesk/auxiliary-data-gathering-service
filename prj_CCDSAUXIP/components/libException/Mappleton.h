/*
	Copyright 2009-2022 Exprivia, SPA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Exception Library $

*/

#ifndef _Mappleton_H_
#define _Mappleton_H_

#include <unordered_map>
#include <string>
#include <ThreadSafe.h>
#include <Singleton.hpp>

_ACS_BEGIN_NAMESPACE(acs)

/*! \brief A Singleton wrapping an std::map to implement a key value store */
class Mappleton final: // PRQA S 2109,2153
	public pattern::Singleton<Mappleton>
{
// friendship needed in order to construct Statistics from a singleton see Singleton doc
friend Mappleton *pattern::Singleton< Mappleton>::instance(); // PRQA S 2107

public:	

	exDECLARE_EXCEPTION(exKeyNotFoundMappletonException, exException) ;

	~Mappleton() override = default ;

	/*! \brief access to the the value of a key, returns the default or throws if not found */
	std::string getValue (const std::string& theKey, const std::string& defValue = "", bool throwIfNotFound=true) const;

	/*! \brief check the value store for the existance of a given key */
	bool hasKey (const std::string& theKey) const;

	/*! \brief associate a value to a key */
	void setValue (const std::string& theKey, const std::string& theValue);

	/*! \brief removes a key from the store */
	void removeKey (const std::string& theKey);

private:
	Mappleton() = default ; 

// private data members 

	Mutex _mapMutex = {} ;
	std::unordered_map <std::string, std::string> _theMap = {} ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(Mappleton) ;
} ;

_ACS_END_NAMESPACE


#endif //_Mappleton_H_
