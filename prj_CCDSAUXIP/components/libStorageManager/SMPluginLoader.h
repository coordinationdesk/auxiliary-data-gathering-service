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

	$Prod: A.C.S. Storage Manager Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/18 14:40:02  marpas
	adoption of libException 5.x standards
	
	Revision 2.5  2013/03/28 14:02:23  marpas
	coding best practices applied
	dbConnectionPool concept forced
	qa warnings
	
	Revision 2.4  2013/02/25 17:00:18  marpas
	coding best practices applied, useless compilation unti removed
	
	Revision 2.3  2013/02/06 16:16:57  marpas
	some coding best prctice applied
	some qa rule fixed
	
	Revision 2.2  2012/03/01 16:40:33  marpas
	default path warning emitted
	
	Revision 2.1  2012/02/13 17:08:58  marpas
	refactoring in proegress
	
	Revision 2.0  2006/02/28 09:11:13  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2006/01/17 11:07:20  marpas
	init(const std::string &path, bool ignoreifnotexists) has no longer default params
	
	Revision 1.4  2006/01/17 10:50:00  marpas
	init signature slightly changed behaviour is the same
	
	Revision 1.3  2005/07/28 14:26:43  marpas
	inline SMPluginLoader::init(void) method added. The previous
	init method has been changed in order to pass a boolean to avoid throwing
	exceptions if the plugin file was not found. In the latter case a Warning is issued
	
	Revision 1.2  2005/06/15 07:59:34  integrator
	Work in progress
	
	Revision 1.1  2005/06/07 09:56:56  integrator
	split in progress
	
	

*/


#ifndef _SMPluginLoader_H_
#define _SMPluginLoader_H_ 

#include <acs_c++config.hpp>
#include <PluginLoader.h>
#include <PluginSet.h>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)


class SMPluginLoader : public PluginLoader // PRQA S 2109, 2153
{
public:
	static void init(const std::string &path = base::defaultSMPluginsPath() + std::string("/") + base::defaultSMPluginsFileName()) ; // default implementation, see at the end of file // PRQA S 3081
	static void init(const std::string &path, bool ignoreIfNotExists) ;

	SMPluginLoader(const SMPluginLoader &) = delete ;
	SMPluginLoader &operator=(const SMPluginLoader &) = delete ;
	SMPluginLoader(SMPluginLoader &&) = delete ;
	SMPluginLoader &operator=(SMPluginLoader &&) = delete ;

	virtual ~SMPluginLoader() = default ;
private:
	SMPluginLoader() ; // only init can call ctor

	// declared but implemented
	void load(const std::string &path, bool ignoreIfNotExists) ; // PRQA S 2121
private:
	static SMPluginLoader *_instance ;
	PluginSet _pluginSet ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(SMPluginLoader) ;
} ;



_ACS_END_NAMESPACE


#endif // _SMPluginLoader_H_
