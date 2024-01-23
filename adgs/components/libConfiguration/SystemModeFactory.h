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

	$Prod: A.C.S. Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 2.4  2013/03/14 15:23:13  marpas
	interface rationalization
	coding best practices applied
	
	Revision 2.3  2012/12/01 18:47:10  marpas
	qa rules
	
	Revision 2.2  2012/02/13 11:07:01  marpas
	refactoring in progress
	
	Revision 2.1  2008/12/15 18:43:52  marpas
	SystemMode implementation in progress
	

*/


#ifndef _SystemModeFactory_H_
#define _SystemModeFactory_H_ 

#include <acs_c++config.hpp>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class SystemMode ;
//
//
//
// class SystemModeFactory
//
//
//
class SystemModeFactory // PRQA S 2109
{
public:

	SystemModeFactory();
	virtual ~SystemModeFactory() throw() ;

	static SystemMode *newItem(const std::string &app, const std::string &sub = "") ;
	static std::string commonConfigurationPath() ;

	virtual SystemMode *createItem(const std::string &app, const std::string &sub) ;
	virtual std::string configurationRepository() ;
	
	static void setConfigurationRepository(const std::string &) ;
	static const std::string &getConfigurationRepository() throw() ;
	
private:
	// declared but not implemented to prevent their use
	SystemModeFactory(const SystemModeFactory &) ;
	SystemModeFactory &operator=(const SystemModeFactory &) ;
	
private:
	static std::vector<SystemModeFactory *> _factories ;
	static SystemModeFactory *_instance ;
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SystemModeFactory) ;
	static std::string _configurationRepository ;
} ; 


_ACS_END_NAMESPACE


#endif // _SystemModeFactory_H_
