// PRQA S 1050 EOF
/*

	Copyright 2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: libDistributorCreator $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2013/02/07 17:51:33  marpas
	DistributorExecutor class added
	Factories, Creators and related classes interface changed to allow dbConnectionPoll easy handling and stop predicates
	
	
*/



#ifndef _DistributorExecutor_H_
#define _DistributorExecutor_H_ 

#include <acs_c++config.hpp>
#include <ConfigurationSingleton.h>
#include <exMacros.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbConnectionPool ;
class dbConnection ;
class dbSet ;

class DistributorExecutor // PRQA S 2109
{		
public:

	DistributorExecutor();
	virtual ~DistributorExecutor() throw() ;
    void distribute(dbConnectionPool & pool, ConfigurationSingleton::StopPredicate &) ; // PRQA S 4400

private:
	// declared but not implemented 
	DistributorExecutor(const DistributorExecutor&);
	DistributorExecutor & operator=(const DistributorExecutor&);	

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(DistributorExecutor);
};


_ACS_END_NAMESPACE

#endif /* _DistributorExecutor_H_ */
