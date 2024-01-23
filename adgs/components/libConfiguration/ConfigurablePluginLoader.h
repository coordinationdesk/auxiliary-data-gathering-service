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
	Revision 2.4  2013/03/14 15:23:12  marpas
	interface rationalization
	coding best practices applied
	
	Revision 2.3  2012/12/01 18:47:10  marpas
	qa rules
	
	Revision 2.2  2012/02/13 11:07:01  marpas
	refactoring in progress
	
	Revision 2.1  2008/11/14 13:44:35  marfav
	Using always the ConfigurationSingleton element to load plugin config spaces
	
	Revision 2.0  2006/02/28 08:37:08  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 17:44:36  marfav
	Header fixed
	
	Revision 1.1  2004/06/04 08:22:11  marfav
	Importing ConfigurablePluginLoader
	

*/


#ifndef _ConfigurablePluginLoader_H_
#define _ConfigurablePluginLoader_H_

#include <PluginLoader.h>
#include <Plugin.h>
#include <rsResourceSet.h>


_ACS_BEGIN_NAMESPACE(acs)

class ConfigurablePluginLoader : public PluginLoader // PRQA S 2109, 2153
{
public:
	explicit ConfigurablePluginLoader(const std::string &loadingPath = "");
	virtual ~ConfigurablePluginLoader() throw() ;

protected:
	// Try to load the configuration space named as the file opened by dlopen
	virtual bool preLoadPlugin(const Plugin& plugin);
	virtual bool postLoadPlugin(const Plugin& plugin);

private:
	// Declared but not implemented to prevent their use
	ConfigurablePluginLoader (const ConfigurablePluginLoader&);
	ConfigurablePluginLoader& operator= (const ConfigurablePluginLoader&);

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ConfigurablePluginLoader) ;
};


_ACS_END_NAMESPACE

#endif // _ConfigurablePluginLoader_H_
