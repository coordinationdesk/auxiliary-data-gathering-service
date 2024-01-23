// PRQA S 1050 EOF
/*
 * 
 * 	Copyright 1995-2021, Advanced Computer Systems , Inc.
 * 	Via Della Bufalotta, 378 - 00139 Roma - Italy
 * 	http://www.acsys.it
 * 
 * 	All Rights Reserved.
 * 
 * 	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 * 	the contents of this file may not be disclosed to third parties, copied or
 * 	duplicated in any form, in whole or in part, without the prior written
 * 	permission of Advanced Computer Systems, Inc.
 * 
 * 	$Prod: A.C.S. PackagerOptimiser Singleton Class HEADER FILE $
 * 
 * 	$Id$
 * 
 * 	$Author$
 * 
 * 	$Log$
 * 	Revision 2.4  2014/05/13 16:03:59  lucio.pulvirenti
 * 	S2PDGS-760: New debug macros used.
 * 	
 * 	Revision 2.3  2013/02/07 15:09:12  enrcar
 * 	EC:: Updated to handle the new interface of the PackagerOptimiser
 * 	
 * 	
 */


#ifndef _PackagerOptimiserSingleton_H_
#define _PackagerOptimiserSingleton_H_

#include <PackagerOptimiser.h>
#include <Singleton.hpp>
#include <exException.h>
#include <map>
#include <deque>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;

class dbPackagerOptimiserFactory ;

void registerPackagerOptimiserFactoryMethod(std::string const & , PackagerOptimiser* (*)(PACKAGEROPT_ARGS) );
void unregisterPackagerOptimiserFactoryMethod(std::string const & , PackagerOptimiser* (*)(PACKAGEROPT_ARGS) );

class PackagerOptimiserSingleton: public Singleton<PackagerOptimiserSingleton> // PRQA S 2109, 2153
{
    friend PackagerOptimiserSingleton* Singleton<PackagerOptimiserSingleton>::instance(); // PRQA S 2107

public:
	/*! class KeyNotFound declaration */
	exDECLARE_EXCEPTION(KeyNotFound,exException) ;	// PRQA S 2131, 2502	

	void registerFactory(std::string const &key, PackagerOptimiser* (*)(PACKAGEROPT_ARGS) );
	void unregisterFactory(std::string const &key, PackagerOptimiser* (*)(PACKAGEROPT_ARGS) );
	PackagerOptimiser *newObject(std::string const &key, PACKAGEROPT_ARGS);


	virtual ~PackagerOptimiserSingleton() throw() ;
protected:
	PackagerOptimiserSingleton();
private:
	// declared but not implemented 
	PackagerOptimiserSingleton(const PackagerOptimiserSingleton &); // not implemented
	PackagerOptimiserSingleton &operator=(const PackagerOptimiserSingleton &); // not implemented

private:
	std::map<std::string, PackagerOptimiser* (*)(PACKAGEROPT_ARGS)> _mapOfOptimisers;
	std::map<std::string, std::deque<PackagerOptimiser* (*)(PACKAGEROPT_ARGS)> > _oldMapOfOptimisers;
	ACS_SMARTPTR<dbPackagerOptimiserFactory> _dbPackagerOptimiserFactory ;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(PackagerOptimiserSingleton) ;


};


_ACS_END_NAMESPACE

#endif /* _PackagerOptimiserSingleton_H_ */
