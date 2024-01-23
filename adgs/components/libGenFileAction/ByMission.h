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
	Revision 7.3  2017/11/16 14:55:44  lucio.pulvirenti
	S3EVOL-78: CTOR default input now empty string.
	
	Revision 7.2  2017/11/15 16:24:15  lucio.pulvirenti
	S3EVOL-78: minor changes.
	
	Revision 7.1  2017/11/14 16:35:01  lucio.pulvirenti
	S3EVOL-78: work in progress.
	
	
		
*/


#ifndef _ByMission_H_
#define _ByMission_H_ 

#include <UploadPredicate.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbInventoryObject;
class dbConnection;

class ByMission : public UploadPredicate // PRQA S 2109
{
public:
    
	explicit ByMission(const std::string &confTag = std::string()) ;
    
	virtual ~ByMission() throw() ;
	ByMission(const ByMission &) ; // not implemented
	ByMission& operator=(const ByMission &) ; // not implemented
    
	virtual bool pass(dbPersistent::IdType, const dbInventoryObject &, dbConnection &);
	
private:
		
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ByMission) ;
	
} ; 

_ACS_END_NAMESPACE


#endif // _SignatureAlgo_H_
