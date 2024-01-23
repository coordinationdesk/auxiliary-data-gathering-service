// PRQA S 1050 EOF
/*

	Copyright 1995-2017, Advanced Computer Systems , Inc.
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
	Revision 5.1  2017/09/25 15:36:15  lucio.pulvirenti
	S2PDGS-1843: Passed optional boolean parameter to be passed to ResourceSpace CTOR.
	
	Revision 5.0  2013/06/18 10:14:19  marpas
	adoption of libException 5.x standards
	
	Revision 2.5  2013/03/14 15:23:13  marpas
	interface rationalization
	coding best practices applied
	
	Revision 2.4  2013/02/25 13:56:57  marpas
	missing includes added
	
	Revision 2.3  2012/12/01 18:47:10  marpas
	qa rules
	
	Revision 2.2  2012/02/13 11:07:01  marpas
	refactoring in progress
	
	Revision 2.1  2009/08/03 18:24:22  marpas
	less connection and faster design
	
	Revision 2.0  2006/02/28 08:37:08  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2004/12/03 17:44:36  marfav
	Header fixed
	
	Revision 1.3  2003/04/30 17:00:26  marpas
	using namespace acs removed
	
	Revision 1.2  2003/04/30 10:20:24  marpas
	using namespace std was removed from includes
	
	Revision 1.1.1.1  2002/10/10 12:06:14  marpas
	libConfiguration import
		

*/


#ifndef _ResourceSpaceFactory_H_
#define _ResourceSpaceFactory_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <vector>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class ResourceSpace ;
//
//
//
// class ResourceSpaceFactory
//
//
//
class ResourceSpaceFactory // PRQA S 2109
{
public:

	ResourceSpaceFactory();
	virtual ~ResourceSpaceFactory() throw() ;

	static ResourceSpace *newItem(const std::string &space, const std::string &version = "", bool absPath = false) ;
	static std::string commonConfigurationPath() ;
	static std::string optionalConfigurationPath() ;

	virtual ResourceSpace *createItem(const std::string &space, const std::string &version, bool absPath = false) ;
	virtual std::string configurationRepository() const ;
    virtual std::string optConfigurationRepository() const ;
    	
	static void setConfigurationRepository(const std::string &) ;
	static const std::string &getConfigurationRepository() throw() ;
	
	static void setOptConfigurationRepository(const std::string &) ;
	static const std::string &getOptConfigurationRepository() throw() ;
	
    /*! calls hasOptionalConf of the registered factory.
        as collateral effect, if no factory has been registered will create an istance of 
        this base class
     */
    static bool hasOptionalConfDir() throw() ;
    
    /*! returns true if an optional conf path exists and it is different from the default conf path
        to be reimplemented if the factory does not allow optional conf
     */
    virtual bool hasOptionalConf() const ; 
    
private:
	// declared but not implemented to prevent their use
	ResourceSpaceFactory(const ResourceSpaceFactory &) ;
	ResourceSpaceFactory &operator=(const ResourceSpaceFactory &) ;
	
private:
	static std::vector<ResourceSpaceFactory *> _factories ;
	static ResourceSpaceFactory *_instance ;
	
private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(ResourceSpaceFactory) ;
	static std::string _configurationRepository ;
	static std::string _optConfigurationRepository ;
} ; 


_ACS_END_NAMESPACE

#endif // _ResourceSpaceFactory_H_
