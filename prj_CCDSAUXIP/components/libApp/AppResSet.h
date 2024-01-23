// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Application Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2013/07/05 12:29:55  marpas
	adopting new libException 5.x standards
	using acs::base::defaultLockPath() to define application lock path
	
	Revision 5.0  2013/06/06 08:37:35  marpas
	new ibException 5.0 nterfaces
	
	Revision 4.4  2013/02/07 09:11:46  marpas
	namespaces enforced
	some interfaces modification in singleton's classes
	
	Revision 4.3  2012/11/28 17:19:24  marpas
	qa rules and robustness
	
	Revision 4.2  2012/03/01 15:48:09  marpas
	using acs::base for default satic paths
	
	Revision 4.1  2012/02/14 14:17:42  marpas
	refactoring
	
	Revision 4.0  2011/10/18 12:43:58  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/08 10:48:56  marpas
	uses new Statistics from libException 3.x
	
	Revision 2.0  2006/02/28 08:35:39  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2005/08/05 09:22:12  marpas
	documentation fix
	
	Revision 1.7  2004/12/14 15:10:49  paoscu
	headers fixed
	
	Revision 1.6  2003/08/05 11:16:11  paoscu
	save method now takes a rsResourceSet as argument.
	
	Revision 1.5  2003/06/26 10:13:31  paoscu
	lock implemented
	
	Revision 1.4  2003/04/30 16:10:45  paoscu
	using namespace std no longer into acs include
	
	Revision 1.3  2003/02/18 18:41:11  marpas
	i/f changed
	
	Revision 1.2  2003/01/31 13:56:22  paoscu
	application name from Application class. No longer in AppResSet class.
	
	Revision 1.1.1.1  2002/11/27 10:23:36  paoscu
	Import libApp
	

*/

#ifndef _AppResSet_H_
#define _AppResSet_H_ 

#include <acs_c++config.hpp>
#include <rsResourceSet.h>
#include <exException.h>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

//
//
// class AppResSet
//
//
// AppResSet class 
// 
// 
// 
//


/**
 * Class for managing application preferences
 * The class uses several different configuration files.
 * 1) User configuration file = $HOME/.ApplicationName/conf/ApplicationName.conf
 * 2) Default File = <base::defaultConfPath()>/ApplicationName.conf
 * 3) Optional file = <base::defaultConfPath()>/ApplicationNameOpt.conf
 * 4) Dcw configuration file = <base::defaultConfPath()>/HyBroDcw.con
 */

class AppResSet                 // PRQA S 2109
{
public:



	/*! class exAppResSetInvalidFilename declaration */
	exDECLARE_EXCEPTION(exAppResSetInvalidFilename,exException) ;       // PRQA S 2131, 2502
		
	
	/**
	 * Build the object, load Optional Settings, and execute revertToSaved().
	 * \param loadConf revertToSaved() will load default configuration file.
	 * \param mergeDcw revertToSaved() will load Dcw configuration file.
	 * \param userEnv Init applications directories (log, work, conf) in home directory.
	 * \throw exFileOpenException Couldn't make a directory in user's home.
	 */
	explicit AppResSet(bool loadConf = true, bool mergeDcw = true, bool userEnv = true);		// Constructor. Need the configuration file name

	~AppResSet() ACS_NOEXCEPT ;						
	AppResSet(const AppResSet &) ; 		// Copy Constructor
	AppResSet &operator=(const AppResSet &) ; 

	/// \return User configuration resource set. 
	rsResourceSet &resourceSet() ; 

	/// \return Optional configuration resource set. 
	rsResourceSet &optResourceSet() ; 
	
	/**
	 * Load user configuration file (if present) , default configuration file and Dcw configuration file.
	 */
	void revertToSaved(); 
	
	/// Save user configuration file.
	void save(rsResourceSet *inrs = 0) ; 

	/// \return optional configuration file path.
	std::string optConfFileName() ; 
	
	/// \return User configuration file path.
	std::string confFileName() ; 
	
	/// \return Default configuration file path.
	std::string defaultConfFileName() ; 
	
	/// \return Dcw configuration file path.
	std::string dcwConfFileName() ; 

	// the following two methods can be called before any reference
	// to the singleton
	
	/**
	 * \return home path.
	 */
	const std::string &homeDir() ; 

private:
	// init applications directories (log, work, conf) in home directory.
	void initUserEnv() ; 
	
	// loads file called optional configuration file (if present).
	void loadOptSet() ;
	void createDir(const std::string &path) ;

private:
	static std::string _homeDir ; 
	rsResourceSet	_rset ;
	// read-only, should be used for external applications, help ...
	// users should not change it, because it is a system-level configuration
	// default file name is given by optConfFileName()
	// and is computed as <base::defaultConfPath()> + /"_applicationName" + "Opt.conf"
	// so for HyBro application the above rule would lead to <base::defaultConfPath()> + /HyBroOpt.conf.
	rsResourceSet	_optSet ; 
	bool			_mergeDcw ;
	bool			_loadConf ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(AppResSet) ;
} ; 

//
//
// class AppResSetSingleton
//
//

class AppResSetSingleton
{
public:
	static AppResSet* instance() ; 
	virtual ~AppResSetSingleton() ACS_NOEXCEPT ; 
	
	static void save(rsResourceSet *rs = 0) ; 
	// call this passing true if you want to merge dcw.conf
	static void mergeDcw(bool) ; 
	// call this passing false if you want to load app.conf
	static void loadConf(bool) ; 
	// call this passing false if you do not want to create ~/.<app> tree
	static void userEnv(bool) ; 
	
protected:
	AppResSetSingleton() ACS_NOEXCEPT ; 
	
private:
	AppResSetSingleton &operator=(const AppResSetSingleton &) ; 	// declared but not implemented
	AppResSetSingleton (const AppResSetSingleton &) ; 				// declared but not implemented
	static void release() ;
	
	static AppResSet*	_instance ; 
	static bool _mergeDcw ; // defaulted to false
	static bool _loadConf ; // defaulted to false
	static bool _userEnv ; // defaulted to true
} ; 

_ACS_END_NAMESPACE

#endif // _AppResSet_H_

