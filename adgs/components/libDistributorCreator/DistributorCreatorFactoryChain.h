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

	$Prod:$

	$Id$

	$Author$

	$Log$
	Revision 2.2  2013/02/07 17:51:33  marpas
	DistributorExecutor class added
	Factories, Creators and related classes interface changed to allow dbConnectionPoll easy handling and stop predicates
	
	Revision 2.1  2013/01/25 12:16:43  marpas
	using pattern::Singleton from libException
	
	Revision 2.0  2013/01/21 17:52:32  marpas
	introducing dbConnectionPool
	coding best practices
	error management improved
	qa rules
	
	Revision 1.2  2012/12/21 12:41:05  marpas
	coding best practices
	introducing if compiled without PDS_OLD_SCHEMA the distributiorule_id field saving distribution items
	qa rules
	
	Revision 1.1.1.1  2007/03/14 13:11:12  crivig
	Imported source
	
*/


#ifndef _DistributorCreatorFactoryChain_H_
#define _DistributorCreatorFactoryChain_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <Singleton.hpp>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)

class DistributorCreatorFactory ;
class DistributorCreator ;

using pattern::Singleton ;


class DistributorCreatorFactoryChain : public Singleton<DistributorCreatorFactoryChain > // PRQA S 2109, 2153
{
	friend DistributorCreatorFactoryChain* Singleton< DistributorCreatorFactoryChain >::instance(); // PRQA S 2107

public:	
	virtual ~DistributorCreatorFactoryChain() throw() ;

	// registers a new factory
	void registerFactory(DistributorCreatorFactory *) ;
	// unregisters a new factory
	void unregisterFactory(DistributorCreatorFactory *) ;
	
	DistributorCreator *newCreator(const std::string &algo) ;
	std::string capabilities() const ;
	
protected:
	DistributorCreatorFactoryChain() ;


private:
	// declared but not implemented	
	DistributorCreatorFactoryChain(const DistributorCreatorFactoryChain &) ;
	DistributorCreatorFactoryChain &operator=(const DistributorCreatorFactoryChain &) ; 

private:
	std::vector<DistributorCreatorFactory *> _factories;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(DistributorCreatorFactoryChain) ;
} ;

_ACS_END_NAMESPACE

#endif

