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

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/19 08:51:08  marpas
	adoption of libException 5.x standards
	
	Revision 2.8  2013/06/04 09:58:09  marpas
	debug improved
	
	Revision 2.7  2013/06/03 12:47:25  marpas
	notification on closing error added
	
	Revision 2.6  2013/05/29 17:54:05  marpas
	debug improved
	
	Revision 2.5  2013/02/19 14:35:03  marpas
	coding best practices applied
	PluginLoaderNotifier class introduced
	qa rules
	
	Revision 2.4  2012/12/01 19:43:00  marpas
	qa rules
	
	Revision 2.3  2012/02/13 11:00:26  marpas
	refactoring in progress
	
	Revision 2.2  2012/01/31 17:33:57  marpas
	 removing ompiler warnings
	
	Revision 2.1  2008/02/26 14:44:49  clanas
	inserted debug string of plugin name opened
	
	Revision 2.0  2006/02/28 08:36:44  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2006/01/13 11:13:26  giucas
	Added DEBUG information
	
	Revision 1.5  2004/12/03 17:48:44  marfav
	Header fixed
	
	Revision 1.4  2003/07/11 12:59:19  paoscu
	exException::clone implemented
	
	Revision 1.3  2003/04/30 16:41:31  paoscu
	using namespace std no longer into acs includes
	
	Revision 1.2  2002/11/25 18:32:38  paoscu
	More debug messages.
	
	Revision 1.1.1.1  2002/10/29 18:24:47  paoscu
	Import libPlugin
	
	

*/

#include <Plugin.h>
#include <Filterables.h>
#include <plV.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

namespace {
    plV version ;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(Plugin) ;


Plugin::Plugin(const string &so_file, bool optional, const string &description, int mode) : 
	_so_file(so_file),
	_handle(0),
	_optional(optional),
	_mode(mode),
	_description(description),
    _openError()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "so_file: " << _so_file << " optional: " << boolalpha << _optional) ; ;
}

Plugin::~Plugin() ACS_NOEXCEPT 
{
// PRQA S 4631 L1
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "so_file: " << _so_file) ;

    try {
    	close();
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
// PRQA L:L1
}


void *Plugin::open() // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "so_file: " << _so_file) ;
	close() ;
	_handle = dlopen(_so_file.c_str(),_mode) ;
	if (!_handle)
	{
		char *err = dlerror() ;
		if (!_optional)
		{
			
			ACS_THROW(DlOpenException("Cannot load \"" + _so_file + "\": "  + (err ? err : ""))) ; // PRQA S 3081, 3380
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "can't load optional plugin") ;
			_openError = (err!=0) ? err : "Unknown error." ; // PRQA S 3380
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "can't load optional plugin open error: " << _openError) ;
		}
		return 0;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_handle: " << _handle) ;
	return _handle;
}


void Plugin::close()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "so_file: " << _so_file << " _handle: "  << _handle) ;
	// MARCO: probably a more robust check should be added.
	// e.g. if the dlopen mode contained RTLD_GLOBAL, because 
	// the relocation of the symbols, another shared object could refer 
	// to them. Until it's true, probably the library will not be definitively closed.
	// anyway the mentioned behaviour is implementation dependent.
	if (_handle !=0 )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to call dlclose(" << _handle << ")") ;
		int err = dlclose(_handle);
        if (err != 0) {
            int loc_errno = errno ;
            char *dlerr = dlerror() ;
            string err_human = dlerr ? dlerr : "" ; // PRQA S 3384
            ACS_LOG_TRYCATCHNOTIFY_EX(exSystemException(err_human + " closing " + _so_file, loc_errno)) ;
        }
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "closed") ;
	}
	_handle = 0 ; 
	_openError = "" ;
}



void * Plugin::handle() const // PRQA S 4120
{
	return _handle ; // PRQA S 4628
}



bool Plugin::optional() const // PRQA S 4120
{
	return _optional ;
}



const string &Plugin::file() const // PRQA S 4120
{
	return _so_file ;
}

const string &Plugin::description() const // PRQA S 4120
{
	return _description ;
}


void Plugin::description(const string & d) // PRQA S 4121
{
	_description = d ;
}



const string &Plugin::openError() const// PRQA S 4120
{
	return _openError ;
}



_ACS_END_NAMESPACE


