// PRQA S 1050 EOF
/*
 * 
	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Storage Manager Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2013/12/11 11:53:49  marfav
	Issuing major release 6.0
	
	Revision 5.0  2013/06/18 14:40:01  marpas
	adoption of libException 5.x standards
	
	Revision 2.4  2013/03/28 14:02:23  marpas
	coding best practices applied
	dbConnectionPool concept forced
	qa warnings
	
	Revision 2.3  2013/03/12 17:45:35  marpas
	dbConnectionPool used in SM interfaces and factory methods
	
	Revision 2.2  2013/02/25 17:00:17  marpas
	coding best practices applied, useless compilation unti removed
	
	Revision 2.1  2012/02/13 17:08:58  marpas
	refactoring in proegress
	
	Revision 2.0  2006/02/28 09:11:13  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2005/06/15 07:59:34  integrator
	Work in progress
	
	Revision 1.3  2005/06/07 09:56:56  integrator
	split in progress
	
	Revision 1.2  2005/05/30 10:24:31  ivafam
	Work in progress
	
	Revision 1.1.1.1  2005/05/18 07:57:07  marpas
	Importing libStorageManager
	

*/


#ifndef _SMInterfaceFactoryMethod_H_
#define _SMInterfaceFactoryMethod_H_ 

#include <dbPersistent.h>
#include <vector>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class SMInterface ;
class dbConnectionPool ;

class SMInterfaceFactoryMethod // PRQA S 2109
{
public:
	SMInterfaceFactoryMethod() = delete ;
	SMInterfaceFactoryMethod(const SMInterfaceFactoryMethod &) = delete ;
	SMInterfaceFactoryMethod &operator=(const SMInterfaceFactoryMethod &) = delete ;
	SMInterfaceFactoryMethod(SMInterfaceFactoryMethod &&) = delete ;
	SMInterfaceFactoryMethod &operator=(SMInterfaceFactoryMethod &&) = delete ;

	virtual ~SMInterfaceFactoryMethod() noexcept ;
	const std::string &type() const ;
	
	static SMInterface *getInterface(const std::string &smtype, dbPersistent::IdType smid, const dbPersistent *, dbConnectionPool&) ;

protected:
	explicit SMInterfaceFactoryMethod(const std::string &smtype) ;

	virtual SMInterface *createInterface(dbPersistent::IdType smid, const dbPersistent *, dbConnectionPool &) = 0 ;

private:
	std::string _smtype ;
	static std::vector<SMInterfaceFactoryMethod *> _methods ;
} ;

_ACS_END_NAMESPACE


#endif // _SMInterfaceFactoryMethod_H_
