// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. ISM SM client library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2013/09/20 08:03:34  marpas
	new t_inventorysm schema supported
	
	Revision 5.0  2013/06/20 13:37:44  marpas
	adopting libException 5.x standards
	coding best practices applied
	qa rules
	
	Revision 1.3  2013/03/12 17:48:00  marpas
	dbCOnnectionPool is mandatory in SM interfaces and factory methods
	
	Revision 1.2  2012/02/13 17:09:49  marpas
	refactoring in progress
	
	Revision 1.1.1.1  2008/06/16 15:14:58  marpas
	Importing libIsmSMClient
	

*/


#ifndef _ISMInterfaceFactoryMethod_H_
#define _ISMInterfaceFactoryMethod_H_ 

#include <SMInterfaceFactoryMethod.h>
#include <exMacros.h>

_ACS_BEGIN_NAMESPACE(acs)

class SMInterface ;
class dbPersistent ;
class dbConnectionPool ;

class ISMInterfaceFactoryMethod: public SMInterfaceFactoryMethod // PRQA S 2109
{
public:
	explicit ISMInterfaceFactoryMethod(const std::string &smtype) ;
	ISMInterfaceFactoryMethod(const ISMInterfaceFactoryMethod &) = delete ;
	ISMInterfaceFactoryMethod &operator=(const ISMInterfaceFactoryMethod &) = delete ;

	~ISMInterfaceFactoryMethod() override = default  ;
	
protected:
	
	SMInterface *createInterface(dbPersistent::IdType smid, const dbPersistent *, dbConnectionPool &) override;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ISMInterfaceFactoryMethod) ;
} ;


_ACS_END_NAMESPACE


#endif // _ISMInterfaceFactoryMethod_H_
