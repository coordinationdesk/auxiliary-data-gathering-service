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

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 2.2  2013/02/13 18:26:27  marpas
	coding best practices applied
	qa rules enforced
	new namespace model
	
	Revision 2.1  2013/02/07 17:51:33  marpas
	DistributorExecutor class added
	Factories, Creators and related classes interface changed to allow dbConnectionPoll easy handling and stop predicates
	
	Revision 2.0  2013/01/21 17:52:32  marpas
	introducing dbConnectionPool
	coding best practices
	error management improved
	qa rules
	
	Revision 1.3  2012/12/21 12:41:05  marpas
	coding best practices
	introducing if compiled without PDS_OLD_SCHEMA the distributiorule_id field saving distribution items
	qa rules
	
	Revision 1.2  2007/03/15 15:04:15  crivig
	coding and compiling...
	
	Revision 1.1.1.1  2007/03/14 13:11:12  crivig
	Imported source
	
*/


#ifndef _DistributorCreatorFactory_H_
#define _DistributorCreatorFactory_H_ 

#include <acs_c++config.hpp>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

class DistributorCreator ;

//* Base class for orders creator factory
class DistributorCreatorFactory // PRQA S 2109
{	
public:
	/*! class NotFoundException declaration */
	exDECLARE_EXCEPTION(NotFoundException,exException) ; // PRQA S 2131, 2502

public:

	DistributorCreatorFactory() ;
	virtual ~DistributorCreatorFactory() ;

	virtual DistributorCreator *newCreator(const std::string &algo) = 0 ;
	virtual void capabilities(std::vector<std::string> &) const = 0 ;
		
private:
	void registerToChain() ;
	void unregisterFromChain() ;

	// declared but not implemented	
	DistributorCreatorFactory(const DistributorCreatorFactory &) ;
	DistributorCreatorFactory &operator=(const DistributorCreatorFactory &) ; 
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(DistributorCreatorFactory) ;
} ;


_ACS_END_NAMESPACE

#endif /* _DistributorCreatorFactory_H_ */

