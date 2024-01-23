// PRQA S 1050 EOF
/*
	Copyright 2009-2020 Exprivia, SPA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Exception Library $

*/

#ifndef _Mappleton_H_
#define _Mappleton_H_

#include <map>
#include <string>
#include <ThreadSafe.h>
#include <Singleton.hpp>

_ACS_BEGIN_NAMESPACE(acs)

/*! \brief A Singleton wrapping an std::map to implement a key value store */
class Mappleton : // PRQA S 2109,2153
	public pattern::Singleton<Mappleton>
{
// friendship needed in order to construct Statistics from a singleton see Singleton doc
friend Mappleton *pattern::Singleton< Mappleton>::instance(); // PRQA S 2107

public:	

	exDECLARE_EXCEPTION(exKeyNotFoundMappletonException, exException) ;

	virtual ~Mappleton() ACS_NOEXCEPT ;

	// No copy.. no move.. no party
	Mappleton(const Mappleton&) = delete;
	Mappleton(const Mappleton&&) = delete;
	Mappleton& operator= (const Mappleton&) = delete;
	Mappleton& operator= (const Mappleton&&) = delete;

	/*! \brief access to the the value of a key, returns the default or throws if not found */
	std::string getValue (const std::string& theKey, const std::string& defValue = "", bool throwIfNotFound=true) const;

	/*! \brief check the value store for the existance of a given key */
	bool hasKey (const std::string& theKey) const;

	/*! \brief associate a value to a key */
	void setValue (const std::string& theKey, const std::string& theValue);

	/*! \brief removes a key from the store */
	void removeKey (const std::string& theKey);

protected:
	Mappleton() ;

    
private:
	Mutex _mapMutex;
	std::map <std::string, std::string> _theMap;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(Mappleton) ;
} ;

_ACS_END_NAMESPACE


#endif //_Mappleton_H_
