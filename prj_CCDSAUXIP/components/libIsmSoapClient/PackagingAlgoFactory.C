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


#include <PackagingAlgoFactory.h>

#include <PackagingAlgoSingleton.h>
#include <TarAlgo.h>
#include <TarGzAlgo.h>

#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(PackagingAlgoFactory);

PackagingAlgoFactory::PackagingAlgoFactory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

	PackagingAlgoSingleton::instance()->registerFactory("tar",&buildTarAlgo ) ;	
	PackagingAlgoSingleton::instance()->registerFactory("targz",&buildTarGzAlgo ) ;	
	
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "End");
}


PackagingAlgoFactory::~PackagingAlgoFactory() throw()
{
    // PRQA S 4631 L1
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB); 
			
	PackagingAlgoSingleton::instance()->unregisterFactory("tar",&buildTarAlgo ) ;	
	PackagingAlgoSingleton::instance()->unregisterFactory("targz",&buildTarGzAlgo ) ;	
		
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "End");
    // PRQA L:L1
}

// *************** buildTarAlgo ***************************
PackagingAlgo* PackagingAlgoFactory::buildTarAlgo(const map<string, string> & arcOpts, const map<string, string> & xtrOpts, 
													const PackagingAlgo::FilesChangeFixStrategy & filesChangeFixStrategy)
{
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "Going to allocate object");
	
	return new TarAlgo(arcOpts, xtrOpts, filesChangeFixStrategy);
}

// *************** buildTarAlgo ***************************
PackagingAlgo* PackagingAlgoFactory::buildTarGzAlgo(const map<string, string> & arcOpts, const map<string, string> & xtrOpts, 
													const PackagingAlgo::FilesChangeFixStrategy & filesChangeFixStrategy)
{
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "Going to allocate object");
	
	return new TarGzAlgo(arcOpts, xtrOpts, filesChangeFixStrategy);
}




_ACS_END_NAMESPACE
