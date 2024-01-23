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
	
	Revision 2.1  2008/11/13 12:38:26  marpas
	New configuration approach using ConfigurationSingleton instead of MergeConfiguration
	
	Revision 2.0  2006/02/28 08:37:08  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 17:44:36  marfav
	Header fixed
	
	Revision 1.1  2004/05/25 10:25:02  marpas
	ConfigurablePlugin class added
	documentation created
	
	Revision 1.5  2003/07/11 13:00:00  marpas
	exException::clone implemented
	
	Revision 1.4  2003/04/30 17:00:26  marpas
	using namespace acs removed
	
	Revision 1.3  2003/04/30 10:20:24  marpas
	using namespace std was removed from includes
	
	Revision 1.2  2003/02/03 18:52:00  marpas
	Exception handling improved to distinguish parse errors
	
	Revision 1.1.1.1  2002/10/10 12:06:14  marpas
	libConfiguration import
		

*/


#ifndef _ConfigurablePlugin_H_
#define _ConfigurablePlugin_H_ 

#include <acs_c++config.hpp>
#include <ConfigMerger.h>

_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ;
class PluginLoader ;
/** 
 * \class ConfigurablePlugin
 * This class inherits from ConfigMerger and allows to uses plugin loading the needed conf spaces.
 * It is a convenience class to select pluginset specified into configuration spaces.
 */
class ConfigurablePlugin : public ConfigMerger // PRQA S 2109, 2153
{
public:
	explicit ConfigurablePlugin(const std::vector<std::pair<std::string, std::string> > &);
	ConfigurablePlugin();
	virtual ~ConfigurablePlugin() throw() ;
	
	/**
	 * The function fills the given PluginLoader using the plugNSpace sas namespace for plugin resources.
	 * Optionally merge the loaded configuration spaces into the given resource set if not null.
	 * \param loader the PluginLoader that will be used, it can also be partially filled.
	 * \param plugNSpace install the namespace before calling PluginSet::load
	 * \param set an optional resource set that will be filled with the loaded configuration spaces.
	 * \see PluginLoader
	 * \see PluginSet
	 * \exception Plugin::DlOpenException A mandatory plugin could not be loaded.
	 */
	void fillLoader(PluginLoader &loader, const std::string &plugNSpace="", rsResourceSet *set=0) ; 
	/**
	 * The function calls fillLoader and then invoke the method load of the given PluginLoader.
	 * \see fillLoader
	 */
	void load(PluginLoader &loader, const std::string &plugNSpace="", rsResourceSet *set=0) ;

private:
	// declared but not implemented	
	ConfigurablePlugin(const ConfigurablePlugin &) ;
	ConfigurablePlugin &operator=(const ConfigurablePlugin &) ;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ConfigurablePlugin) ;
} ; 


_ACS_END_NAMESPACE

#endif // _ConfigurablePlugin_H_
