// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Template StringKey Based Factory Library $

	$Id$

	$Author$

	$Log$
	Revision 1.8  2016/11/17 15:58:50  marfav
	Added support to named builders
	
	Revision 1.7  2013/07/07 17:36:31  marpas
	coding besta practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 1.6  2012/02/21 13:09:43  marpas
	refactoring
	
	Revision 1.5  2011/03/01 16:30:38  marpas
	GCC 4.4.x support
	
	Revision 1.4  2005/08/04 11:46:34  clanas
	inserted std namespace on endl
	
	Revision 1.3  2004/12/03 18:00:41  marfav
	Header fixed
	
	Revision 1.2  2004/08/24 14:39:05  marfav
	Added template support for builder parameter type
	
	Revision 1.1.1.1  2004/04/07 13:30:10  marfav
	Import libStringKeyFactory
	

*/

#ifndef _StringKeyFactory_H_
#define _StringKeyFactory_H_

#include <acs_c++config.hpp>

#include <StringKey.h>
#include <StringFactoryChain.h>

#include <ConfigurationSingleton.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <exStream.h>
#include <Filterables.h>

#include <string>
#include <vector>


_ACS_BEGIN_NAMESPACE(acs)


template <typename O, typename R=void>
class StringKeyFactory      // PRQA S 2109
{
public:
	typedef StringFactoryChain<O,R> SingletonType;

	StringKeyFactory (const std::string&, typename SingletonType::FactoryType::BuilderPointer, const std::string& = "");
	virtual ~StringKeyFactory();

private:
	void loadRegisteredKeys();
	void registerFactory();
	void unregisterFactory();

	// declared but not implemented
    StringKeyFactory() ;
	StringKeyFactory (const StringKeyFactory&);
	StringKeyFactory& operator= (const StringKeyFactory&);

private: // attributes
	std::string _rootTag;
	std::vector <StringKey> _registeredKeys;
	std::string _builderId;

	typename SingletonType::FactoryType::BuilderPointer _theBuilderPointer;

	// static attributes - keys in configuration spaces
	static char _regCountKey[];
	static char _regArrayKey[];
	static char _satKey[];
	static char _misKey[];
	static char _proKey[];
	static char _verKey[];
	static char _objKey[];
};


// ***************************************
//     Resource space navigation keys
// ***************************************

template <typename O, typename R>
char StringKeyFactory<O,R>::_regCountKey[] = "List_Of_Registrations.count.value";

template <typename O, typename R>
char StringKeyFactory<O,R>::_regArrayKey[] = "List_Of_Registrations.Registration";

// to be used inside "List_Of_Registrations.Registration[i]"
template <typename O, typename R>
char StringKeyFactory<O,R>::_satKey[] = "Satellite";

template <typename O, typename R>
char StringKeyFactory<O,R>::_misKey[] = "Mission";

template <typename O, typename R>
char StringKeyFactory<O,R>::_proKey[] = "Processor";

template <typename O, typename R>
char StringKeyFactory<O,R>::_verKey[] = "Version";

template <typename O, typename R>
char StringKeyFactory<O,R>::_objKey[] = "Object";  // Optional Key




// ***************************************
//     CTOR
// ***************************************

template <typename O, typename R>
StringKeyFactory<O,R>::StringKeyFactory (const std::string &rootTag, typename SingletonType::FactoryType::BuilderPointer pointer, const std::string& builderId) :
	_rootTag (rootTag),
    _registeredKeys(),
	_builderId (builderId),
	_theBuilderPointer (pointer)
{
	loadRegisteredKeys();
	registerFactory();
}


// ***************************************
//     DTOR
// ***************************************

template <typename O, typename R>
StringKeyFactory<O,R>::~StringKeyFactory ()
{
	unregisterFactory();
}


// ***************************************
//     Load Registered Keys
// ***************************************

template <typename O, typename R>
void StringKeyFactory<O,R>::loadRegisteredKeys()
{
	try
	{
		_registeredKeys.clear();
		rsResourceSet rs = ConfigurationSingleton::instance()->get();
		if (rs.hasKey (_rootTag))
		{
			// can load something
			rsPushNSpace pusher (rs, _rootTag);
			int count = 0 ;
			rs.getValue (_regCountKey, count);
			for (int i=0; i<count; ++i)
			{
				std::string sat ;
                std::string mis ;
                std::string pro ;
                std::string ver ;
                std::string obj ;
				std::ostringstream os;
				os << _regArrayKey << "[" << i << "]";
				rsPushNSpace regPusher (rs, os.str());
				rs.getValue (_satKey, sat);
				rs.getValue (_misKey, mis);
				rs.getValue (_proKey, pro);
				rs.getValue (_verKey, ver);
				if (rs.hasKey (_objKey)) {
					rs.getValue (_objKey, obj); // optional key
                }
				StringKey newKey(
					sat == "" ? StringKeyElement::getWildCard() : StringKeyElement(sat), // PRQA S 3081, 3380, 3385 5
					mis == "" ? StringKeyElement::getWildCard() : StringKeyElement(mis),
					pro == "" ? StringKeyElement::getWildCard() : StringKeyElement(pro),
					ver == "" ? StringKeyElement::getWildCard() : StringKeyElement(ver),
					obj == "" ? StringKeyElement::getWildCard() : StringKeyElement(obj) );
				_registeredKeys.push_back (newKey);
			}

		}
	}
	catch (std::exception& ex)
	{
		ACS_LOG_ERROR("StringKeyFactory: exception caught while initializing") ;
		ACS_LOG_NOTIFY_EX(ex);
		ACS_LOG_ERROR("StringKeyFactory: retrieved " << _registeredKeys.size() << " keys in namespace [" << _rootTag << "]") ;
	}
}


// ***************************************
//     Register Factory
// ***************************************

template <typename O, typename R>
void StringKeyFactory<O,R>::registerFactory()
{
	for (std::vector<StringKey>::iterator it = _registeredKeys.begin(); it != _registeredKeys.end(); ++it) // PRQA S 4238
	{
		SingletonType::instance() -> registerFactory(*it, _theBuilderPointer, _builderId);
	}
}

// ***************************************
//     Un-Register Factory
// ***************************************

template <typename O, typename R>
void StringKeyFactory<O,R>::unregisterFactory()
{
	for (std::vector<StringKey>::iterator it = _registeredKeys.begin(); it != _registeredKeys.end(); ++it) // PRQA S 4238
	{
		SingletonType::instance() -> unregisterFactory(*it, _theBuilderPointer);
	}
}



_ACS_END_NAMESPACE


#endif //_StringKeyFactory_H_
