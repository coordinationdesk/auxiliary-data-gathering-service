/*

	Copyright 1995-2002, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2014/02/03 13:17:12  giucas
	New macros adopted
	
	Revision 2.0  2006/02/28 09:36:19  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/11/02 17:52:40  giucas
	First release
	

*/

#ifndef _OBTConverterSingleton_H_
#define _OBTConverterSingleton_H_ 

#include <acs_c++config.hpp>
#include <Singleton.hpp>
#include <OBTConverter.h>
#include <exException.h>
#include <map>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;

class OBTConverterSingleton : public Singleton <OBTConverterSingleton> // PRQA S 2109, 2153
{
	friend OBTConverterSingleton* Singleton<OBTConverterSingleton>::instance(); // PRQA S 2107
		
public:
	virtual ~OBTConverterSingleton() throw() ;

	/*! class KeyNotFound declaration */
	exDECLARE_EXCEPTION(KeyNotFound, exException) ; // PRQA S 2131, 2502

	void registerFactory(std::string const &, OBTConverter* (*)() );
	void unregisterFactory(std::string const &, OBTConverter* (*)() );

////////////////////////////////////////////////////////////////////////////////////////////
// Specific Factory Methods
////////////////////////////////////////////////////////////////////////////////////////////

	/**
	  * \brief Return the object pointer associated the the key in input
	  * \param The key must be the Satellite Name
	  */										
	OBTConverter* newObtConverter(std::string const &);

protected:
	OBTConverterSingleton();
private:
	OBTConverterSingleton(const OBTConverterSingleton &); // not implemented 
	OBTConverterSingleton& operator= (const OBTConverterSingleton &); // not implemented 
	OBTConverter *newObject(std::string const &);

private:

	std::map<std::string, OBTConverter* (*)()> _map;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(OBTConverterSingleton) ;
};


_ACS_END_NAMESPACE


#endif // _OBTConverterSingleton_H_
