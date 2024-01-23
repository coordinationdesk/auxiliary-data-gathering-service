// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
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

*/

#ifndef _ProdTypeDescriptorSingleton_H_
#define _ProdTypeDescriptorSingleton_H_ 

#include <acs_c++config.hpp>
#include <Singleton.hpp>
#include <ProductTypeDescriptor.h>
#include <exException.h>
#include <map>

_ACS_BEGIN_NAMESPACE(acs)


using pattern::Singleton ;

class ProdTypeDescriptorSingleton : public Singleton <ProdTypeDescriptorSingleton> // PRQA S 2109, 2153
{
friend ProdTypeDescriptorSingleton* Singleton<ProdTypeDescriptorSingleton>::instance(); // PRQA S 2107

public:
	/*! class ThreadException declaration */
	exDECLARE_EXCEPTION(KeyNotFound,exException) ; // PRQA S 2131, 2502

	virtual ~ProdTypeDescriptorSingleton() throw() ;

	void registerFactory(std::string const &, ProductTypeDescriptor* (*)() );
	void unregisterFactory(std::string const &, ProductTypeDescriptor* (*)() ) ;

	ProductTypeDescriptor *newObject(std::string const &);

protected:

	ProdTypeDescriptorSingleton();
private:
    ProdTypeDescriptorSingleton(const ProdTypeDescriptorSingleton &) ; // not implemented
    ProdTypeDescriptorSingleton& operator= (const ProdTypeDescriptorSingleton &) ; // not implemented
private:

	std::map<std::string, ProductTypeDescriptor* (*)()> _map;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ProdTypeDescriptorSingleton) ;
};


_ACS_END_NAMESPACE


#endif // _ProdTypeDescriptorSingleton_H_
