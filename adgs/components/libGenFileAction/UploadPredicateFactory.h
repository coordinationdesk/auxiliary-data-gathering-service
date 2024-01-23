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
	Revision 1.2  2017/11/16 15:03:55  lucio.pulvirenti
	S3EVOL-78: get method has the conf key with default empty string.
	
	Revision 1.1  2017/11/15 16:26:23  lucio.pulvirenti
	S3EVOL-78: first issue.
	
	

*/


#ifndef _UploadPredicateFactory_H_
#define _UploadPredicateFactory_H_ 

#include <AbstractFactoryChainSingleton.h>
#include <string>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class UploadPredicate ;


class UploadPredicateFactory : public Singleton<UploadPredicateFactory > // PRQA S 2109, 2153
{
	friend UploadPredicateFactory* Singleton< UploadPredicateFactory >::instance(); // PRQA S 2107

public:
	/*! class NotFoundException declaration */
    
	exDECLARE_EXCEPTION(NotFoundException,exException) ; // PRQA S 2131, 2502

public:
    
	virtual ~UploadPredicateFactory() throw() ;
	
    
	UploadPredicate *get(const std::string &predicate, const std::string & confKey = std::string() );
	
	static UploadPredicate* buildByMissionInterface(const std::string &);	

    
	std::string capabilities() const ;

protected:
    
	UploadPredicateFactory() ;
    
 	
private:
	UploadPredicateFactory(const UploadPredicateFactory &) ; // not implemented
	UploadPredicateFactory &operator=(const UploadPredicateFactory &) ;	 // not implemented

private:
    
	ACS_CLASS_DECLARE_DEBUG_LEVEL(UploadPredicateFactory)
} ;


typedef AbstractFactoryChainSingleton <UploadPredicate, std::string, DefaultAbstractFactoryChainPredicate<std::string>, std::string> UploadPredicateFactoryChain; 


void registerUploadPredicateFactoryChainMethod(const std::string &, UploadPredicate* (*)(const std::string &) );
void unregisterUploadPredicateFactoryChainMethod(const std::string &, UploadPredicate* (*)(const std::string &) );

_ACS_END_NAMESPACE

#endif /* _UploadPredicateFactory_H_ */

