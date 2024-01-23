// PRQA S 1050 EOF
/*

	Copyright 2020, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

*/


#ifndef _ObjSMInterfaceFactoryMethod_H_
#define _ObjSMInterfaceFactoryMethod_H_ 

#include <SMInterfaceFactoryMethod.h>
#include <exMacros.h>

_ACS_BEGIN_NAMESPACE(acs)

class SMInterface ;
class dbPersistent ;
class dbConnectionPool ;

class ObjSMInterfaceFactoryMethod: public SMInterfaceFactoryMethod // PRQA S 2109
{
public:
	explicit ObjSMInterfaceFactoryMethod(const std::string &smtype) ;

	virtual ~ObjSMInterfaceFactoryMethod() throw() ;
	
protected:
	
	virtual SMInterface *createInterface(dbPersistent::IdType smid, const dbPersistent *, dbConnectionPool &) ;

private:
	// declared but implemented
	ObjSMInterfaceFactoryMethod() ;
	ObjSMInterfaceFactoryMethod(const ObjSMInterfaceFactoryMethod &) ;
	ObjSMInterfaceFactoryMethod &operator=(const ObjSMInterfaceFactoryMethod &) ;

private:
	static ObjSMInterfaceFactoryMethod _theMethod ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ObjSMInterfaceFactoryMethod) ;
} ;


_ACS_END_NAMESPACE


#endif // _ObjSMInterfaceFactoryMethod_H_
