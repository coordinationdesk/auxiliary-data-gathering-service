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

	$Prod: A.C.S. Plugin Library $

	$Id$

	$Author$

	$Log$
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
	
	Revision 1.4  2004/12/03 17:48:44  marfav
	Header fixed
	
	Revision 1.3  2003/07/11 12:59:19  paoscu
	exException::clone implemented
	
	Revision 1.2  2003/04/30 16:41:31  paoscu
	using namespace std no longer into acs includes
	
	Revision 1.1.1.1  2002/10/29 18:24:47  paoscu
	Import libPlugin
	
	

*/

#ifndef _Plugin_H_
#define _Plugin_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <dlfcn.h>


_ACS_BEGIN_NAMESPACE(acs)

/*! \class Plugin
	
*/
class Plugin // PRQA S 2109
{
public:
	/*! class DlOpenException declaration thrown each time a shared object fails loading.*/
	exDECLARE_EXCEPTION(DlOpenException,exException) ; // PRQA S 2131, 2502
    
public:


	explicit Plugin(const std::string & so_file, bool optional = false, const std::string &description="", int mode = RTLD_LAZY|RTLD_GLOBAL) ;
	virtual ~Plugin() ACS_NOEXCEPT ;
	
	/*!
	 * returns the handle (see dlopen).
	 * In case of failure, if the plugin was optional, a null pointer will be returned
	 * else the DlOpenException will be raised.
	 * \exception DlOpenException A mandatory plugin could not be loaded.
	 */
	void *open() ;
	
	/*!
	 * closes the shared object handle if it was previously opened.
	 */
	 
	void close() ; // PRQA S 2502
	
	/*!
	 * returns the shared object handle. Useful to use dlsym function.
	 */
	void *handle() const ;
	
	/*!
	 * returns if the shared object can be optionally loaded.
	 */
	bool optional() const ;
	
	/*!
	 * returns the shared object passed to dlopen 
	 */
	const std::string &file() const ;

	/*!
	 * returns the plugin description. 
	 */
	const std::string &description() const ;


	/*!
	 * set the plugin description. 
	 */
	void description(const std::string &) ;

	/*!
	 * If the plugin is optional and it couldn't be loaded, then this method returns the error description. 
	 */
	const std::string &openError() const ;	
private:
	Plugin() ;
	Plugin(const Plugin & ) ;
	Plugin &operator=(const Plugin & ) ;	
	
private:	
	std::string _so_file ;
	void * _handle ;
	bool _optional ;
	int _mode; 
	std::string _description ;
	std::string _openError ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(Plugin) ;
} ;



_ACS_END_NAMESPACE

#endif // _Plugin_H_
