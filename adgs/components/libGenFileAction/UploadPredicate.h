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
	Revision 7.3  2017/11/16 14:41:59  lucio.pulvirenti
	S3EVOL-78: getConfigurationNode method implementation moved in .C file. setConfigurationTag method add to IF.
	
	Revision 7.2  2017/11/15 16:23:00  lucio.pulvirenti
	S3EVOL-78: minor changes and UploadPredicateScopeGuard typedef added.
	
	Revision 7.1  2017/11/14 16:35:01  lucio.pulvirenti
	S3EVOL-78: work in progress.
	
		
*/


#ifndef _UploadPredicate_H_
#define _UploadPredicate_H_ 

#include <dbPersistent.h>
#include <acs_c++config.hpp>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)

class dbInventoryObject;
class dbConnection;

//##Documentation
//## This class represents the common interface for all predicate checking if upload to given storage manager is possible
class UploadPredicate  // PRQA S 2109
{
public:
	
	exDECLARE_EXCEPTION(UploadPredicateException,exException) ; // PRQA S 2131, 2153, 2502
    
	explicit UploadPredicate(const std::string &confTag) ;
	virtual ~UploadPredicate() throw() ;
    
	virtual bool pass(dbPersistent::IdType, const dbInventoryObject &, dbConnection &) = 0 ;
	
	std::string getConfigurationNode() const ;
	void setConfigurationTag(const std::string & v ) { _confTag = v; }
protected:	
	UploadPredicate(const UploadPredicate &) ;
	UploadPredicate &operator=(const UploadPredicate &) ;
private:  
	UploadPredicate(); // not implemented
	
private:
	std::string _confTag;

} ; 

typedef ACS_SMARTPTR<UploadPredicate> UploadPredicateScopeGuard ;


_ACS_END_NAMESPACE

#endif // _UploadPredicate_H_
