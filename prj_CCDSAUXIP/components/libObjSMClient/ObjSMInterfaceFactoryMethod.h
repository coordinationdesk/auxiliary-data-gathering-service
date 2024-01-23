// PRQA S 1050 EOF
/*

	Copyright 2020-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

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
	ObjSMInterfaceFactoryMethod(const ObjSMInterfaceFactoryMethod &) = delete ;
	ObjSMInterfaceFactoryMethod &operator=(const ObjSMInterfaceFactoryMethod &) = delete;

	~ObjSMInterfaceFactoryMethod() override = default ;
	
protected:
	
	SMInterface *createInterface(dbPersistent::IdType smid, const dbPersistent *, dbConnectionPool &) override;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ObjSMInterfaceFactoryMethod) ;
} ;


_ACS_END_NAMESPACE


#endif // _ObjSMInterfaceFactoryMethod_H_
