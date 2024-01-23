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

	$Prod: A.C.S. PluginLoader Library $

	$Id$

	$Author$

	$Log$
	Revision 2.4  2013/02/28 13:17:44  marpas
	dependency from libMessage removed definitely
	
	Revision 2.3  2013/02/19 14:35:03  marpas
	coding best practices applied
	PluginLoaderNotifier class introduced
	qa rules
	
	Revision 2.2  2012/12/01 19:43:00  marpas
	qa rules
	
	Revision 2.1  2012/02/13 11:00:26  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 08:36:44  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2004/12/03 17:48:44  marfav
	Header fixed
	
	Revision 1.5  2004/06/04 08:20:22  marfav
	Added virtual PreLoad and PostLoad plugin methods
	
	Revision 1.4  2003/07/11 12:59:19  paoscu
	exException::clone implemented
	
	Revision 1.3  2003/04/30 16:41:31  paoscu
	using namespace std no longer into acs includes
	
	Revision 1.2  2002/11/25 17:42:49  paoscu
	Methods setPath and getPath added.
	
	Revision 1.1.1.1  2002/10/29 18:24:47  paoscu
	Import libPlugin
	
	

*/

#ifndef _PluginLoader_H_
#define _PluginLoader_H_ 

#include <exException.h>
#include <vector>
#include <dlfcn.h>


_ACS_BEGIN_NAMESPACE(acs)

class Plugin ;

class PluginLoaderNotifier
{
public:
    PluginLoaderNotifier() ;
    PluginLoaderNotifier(const PluginLoaderNotifier &) ACS_NOEXCEPT {} 
    virtual ~PluginLoaderNotifier() ACS_NOEXCEPT ; // PRQA S 2131
    PluginLoaderNotifier& operator=(const PluginLoaderNotifier &) throw() { return *this ;} // PRQA S 4072
    
    virtual void operator()(const std::string &plugin) const ;
} ;


/** 
 * \class PluginLoader
 * This class allow the user to specify	a collection of plugins and load them.
 * Plugins are added with the add() method, and then loaded all together with the load() method.
 */
class PluginLoader // PRQA S 2109
{
public:
	/*! class DuplicatedSo declaration issued trying to add multiple reference to the same shared object in PluginLoader::add().*/
	exDECLARE_EXCEPTION(DuplicatedSo,exException) ; // PRQA S 2131, 2502

	
	/*! class SequenceError declaration issued if the user tries a wrong sequence, e.g. trying to add another
	 * shared object after the invocation of the load method.
	 */
	exDECLARE_EXCEPTION(SequenceError,exException) ; // PRQA S 2131, 2502

	/*! class Nofailed declaration issued if failed() can't find a plugin that failed to load.
	 */
	exDECLARE_EXCEPTION(Nofailed,exException) ; // PRQA S 2131, 2502

	/*! class PreLoadError declaration issued if a preload of a no optional plugin fails
	 */
	exDECLARE_EXCEPTION(PreLoadError,exException) ; // PRQA S 2131, 2502

	/*! class PostLoadError issued if a postload of a no optional plugin fails
	 */
	exDECLARE_EXCEPTION(PostLoadError,exException) ; // PRQA S 2131, 2502

		
public:

	explicit PluginLoader(const std::string &loadingPath = "") ;
	virtual ~PluginLoader() ACS_NOEXCEPT ;	
	
	/**
	 * Set path. 
	 */
	void setPath(const std::string &loadingPath) ;
	
	/**
	 * Get path. 
	 */
	const std::string &getPath() const ACS_NOEXCEPT ;
	
	
	/**
	 * Adds shared objects to the list.
	 * The loading sequence will reflect the order of the add invocation.
	 * \exception PluginLoader::SequenceError load() has already been called.
	 * \exception PluginLoader::DuplicatedSo The plugin has already been added.
	 */
	void add(const std::string &so_file, bool optional, const std::string &description="" , int mode = RTLD_LAZY|RTLD_GLOBAL) ;

	
	/**
	 * The main function to load all plugins.
	 * It returns true if all plugins are correctly loaded, false if any of the optional ones
	 * had loading problems.
	 * \see Plugin::open()
	 * After loading, no other shared objects can be added.
	 * \see add().
	 * \exception Plugin::DlOpenException A mandatory plugin could not be loaded.
	 * \exception PluginLoader::PostLoadError Post-load configuration of mandatory plugin failed.
	 * \exception PluginLoader::PreLoadError Pre-load configuration of mandatory plugin failed.
	 */
	bool load(const PluginLoaderNotifier &notifier=PluginLoaderNotifier()) ;
	
	const Plugin & operator[](size_t) const ;
	Plugin & operator[](size_t) ;
	size_t size() const ;

	/**
	 * Return the first plugin in that failed to load.
	 * \param optional Check also optional plugins.
	 * \exception Nofailed No plugin failed to load.
	 */
	const Plugin & failed(bool optional=false) const; 

	/**
	 * Return the first plugin in that failed to load.
	 * \param optional Check also optional plugins.
	 * \exception Nofailed No plugin failed to load.
	 */
	Plugin & failed(bool optional=false) ;

protected:
	/**
	 * Virtual class to be re-defined in derived classes.
	 * Performs pre-load configuration of the plugin
	 * \param plugin the reference of the plugin to configure
	 */
	virtual bool preLoadPlugin(const Plugin& plugin) {return true;} // PRQA S 2131

	/**
	 * Virtual class to be re-defined in derived classes.
	 * Performs post-load configuration of the plugin
	 * \param plugin the reference of the plugin to configure
	 */
	virtual bool postLoadPlugin(const Plugin& plugin) {return true;} // PRQA S 2131
	
private:
	PluginLoader(const PluginLoader & ) ;
	PluginLoader &operator=(const PluginLoader & ) ;
	
private:	
	std::string _loadingPath ;
	std::vector<Plugin *> _plugins ;
	bool _loaded ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PluginLoader) ;
} ;


_ACS_END_NAMESPACE

#endif // _PluginLoader_H_
