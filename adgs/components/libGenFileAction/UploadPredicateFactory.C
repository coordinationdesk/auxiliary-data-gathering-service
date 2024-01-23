// PRQA S 1050 EOF
/*

	Copyright 2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Signature Algo Library $

	$Id$

	$Author$

	$Log$
	Revision 1.1  2017/11/15 16:26:23  lucio.pulvirenti
	S3EVOL-78: first issue.
	
	

*/


#include <UploadPredicateFactory.h>
#include <ByMission.h>
#include <StringUtils.h>

#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(UploadPredicateFactory)

UploadPredicateFactory::UploadPredicateFactory() :
    Singleton<UploadPredicateFactory >()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	registerUploadPredicateFactoryChainMethod("bymission", &buildByMissionInterface );

}

UploadPredicateFactory::~UploadPredicateFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ; // PRQA S 4631 2
	unregisterUploadPredicateFactoryChainMethod("bymission", &buildByMissionInterface );
}

UploadPredicate * UploadPredicateFactory::get(const std::string &predicate, const std::string & confKey) // PRQA S 4020
{
	UploadPredicate *uplPredicate = 0;
	try {
		uplPredicate = UploadPredicateFactoryChain::instance()->newObject(StringUtils::uppercase(predicate), confKey);
	}
	catch( UploadPredicateFactoryChain::FactoryType::KeyNotFoundException & )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, '\"' << uplPredicate << "\" upload predicate not found!");
	}
	
	return uplPredicate ;
}

UploadPredicate* UploadPredicateFactory::buildByMissionInterface(const string &confKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ; // PRQA S 4631
	
	return new ByMission(confKey) ;
	
}


string UploadPredicateFactory::capabilities() const
{
	string ret ;
	ret += "ByMission\n" ;
	return ret ;
}


void registerUploadPredicateFactoryChainMethod(const string & key, UploadPredicate* (*builder)(const std::string &) )
{
	UploadPredicateFactoryChain::instance()->registerFactory(StringUtils::uppercase(key), builder);
}


void unregisterUploadPredicateFactoryChainMethod(const string & key, UploadPredicate* (*builder)(const std::string &) )
{
	UploadPredicateFactoryChain::instance()->unregisterFactory(StringUtils::uppercase(key), builder);
}



_ACS_END_NAMESPACE

