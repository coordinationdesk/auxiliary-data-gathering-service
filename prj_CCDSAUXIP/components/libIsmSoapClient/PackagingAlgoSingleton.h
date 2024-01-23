// PRQA S 1050 EOF
/*
	Copyright 2020-2022, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA, Inc.
	
*/ 


#ifndef _PackagingAlgoSingleton_H_
#define _PackagingAlgoSingleton_H_

#include <PackagingAlgo.h>
#include <Singleton.hpp>
#include <exException.h>
#include <map>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;


class PackagingAlgoSingleton : public Singleton<PackagingAlgoSingleton> // PRQA S 2109, 2153
{
    friend PackagingAlgoSingleton* Singleton<PackagingAlgoSingleton>::instance(); // PRQA S 2107
public:
	/*! class KeyNotFound declaration */
	exDECLARE_EXCEPTION(KeyNotFound,exException) ; // PRQA S 2131, 2502 
	 /** PackagingAlgoSingleton Exception */
    exDECLARE_EXCEPTION(exPackagingAlgoSingletonException, exException) ; // PRQA S 2131, 2153, 2502 

	void registerFactory(std::string const &, PackagingAlgo* (*)(const std::map<std::string, std::string> &, const std::map<std::string, std::string> &, const PackagingAlgo::FilesChangeFixStrategy &) );

	void unregisterFactory(std::string const &, PackagingAlgo* (*)(const std::map<std::string, std::string> &, const std::map<std::string, std::string> &, const PackagingAlgo::FilesChangeFixStrategy &) );

	/**
	 * \brief  Return an object derived from SatelliteMissionAssociator class
	 * \param The map key
	 */    	
	PackagingAlgo *newPackagingAlgo(std::string const &, const std::map<std::string, std::string> & arcOpts, const std::map<std::string, std::string> & xtrOpts, const PackagingAlgo::FilesChangeFixStrategy & ) const;
    virtual ~PackagingAlgoSingleton() throw() ;
protected:

	PackagingAlgoSingleton();

private:

	PackagingAlgoSingleton(const PackagingAlgoSingleton &); // not implemented

	PackagingAlgoSingleton& operator=(const PackagingAlgoSingleton &); // not implemented

// data
private:
	std::map<std::string, PackagingAlgo* (*)(const std::map<std::string, std::string> &, const std::map<std::string, std::string> &, const PackagingAlgo::FilesChangeFixStrategy &)> _map;


	ACS_CLASS_DECLARE_DEBUG_LEVEL(PackagingAlgoSingleton)

};

_ACS_END_NAMESPACE

#endif //_PackagingAlgoSingleton_H_
