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
	Revision 5.1  2015/11/27 14:56:58  marpas
	in case of optional plugin with errors, e.g. undefined symbols, notifier is called
	
	Revision 5.0  2013/06/19 08:51:08  marpas
	adoption of libException 5.x standards
	
	Revision 2.6  2013/02/28 13:17:44  marpas
	dependency from libMessage removed definitely
	
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
	
	Revision 2.1  2011/07/08 12:18:29  marpas
	debug improved
	
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

#include <PluginLoader.h>
#include <Plugin.h>
#include <File.h>
#include <Filterables.h>
#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


PluginLoaderNotifier::PluginLoaderNotifier()
{
}

PluginLoaderNotifier::~PluginLoaderNotifier() ACS_NOEXCEPT
{}

    
void PluginLoaderNotifier::operator()(const string &plugin) const 
{ 
    ACS_LOG_ERROR("Error loading plugin: \"" << plugin << "\"") ;
}



ACS_CLASS_DEFINE_DEBUG_LEVEL(PluginLoader) 


PluginLoader::PluginLoader(const string &loadingPath) : 
	_loadingPath(loadingPath),
    _plugins(),
	_loaded(false)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}



PluginLoader::~PluginLoader() ACS_NOEXCEPT
{
// PRQA S 4631 L1
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	//destroyed in reverse order to avoid plugins dependencies.
	for (int i=(size()-1); i >= 0; i--) // PRQA S 3000, 3010, 3084
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to delete plugin number " << i) ;
		delete _plugins[i] ; // PRQA S 3000
	}
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
// PRQA L:L1
}


void PluginLoader:: setPath(const string &loadingPath) // PRQA S 4121
{
	_loadingPath=loadingPath;
}


const string &PluginLoader::getPath() const ACS_NOEXCEPT // PRQA S 4120
{
	return _loadingPath;
}

void PluginLoader::add(const string &so_file, bool optional, const string &description, int mode) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "file is \""<< so_file <<"\".") ;
	ACS_COND_THROW(_loaded,SequenceError("Plugins had already been loaded.")) ; // PRQA S 3081

	string fullPath;
	if(_loadingPath!="") { fullPath=_loadingPath+"/"; }
	fullPath+=so_file;
	
	// find duplicated and issue an exception if any
	for(size_t i=0; i<size(); i++)
	{
		if( (*this)[i].file() == fullPath)
		{
			ACS_THROW(DuplicatedSo("Shared Object \"" + fullPath +"\" has already been added.")); // PRQA S 3081
		}
	}
	
	_plugins.push_back(new Plugin(fullPath,optional,description, mode)) ;
	
}


bool PluginLoader::load(const PluginLoaderNotifier &notifier)
{
	bool res=true;
    // PRQA S 4238, 4244 2
	for(size_t i=0; i<size(); i++)
	{
		Plugin & p = (*this)[i];
	
		const bool prevent_exception = true ; 
				
		// Test if the entry is a broken symlink :		
		if ( File::entry_is_symlink(p.file(), prevent_exception) && File::entry_is_broken_symlink(p.file(), prevent_exception) )
		{
			ACS_LOG_WARNING("Unable to load symlink: \"" << p.file() << "\": skipped") ;
			notifier(p.file()) ; 
			continue ;	/* Skip this entry */
	
		} else if (not File::exists(p.file())) {
			// Test if the entry exists (if not existent, probably it was removed from the disk soon after the discover: )	
			notifier(p.file()) ;
			ACS_THROW( exIOException("Plugin not found: \"" + p.file() + "\"") ) ; // PRQA S 3081
		}
        else {
            ; // nothing
        }
		
		
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "loading " << p.file()) ;

		if( p.handle() == 0 )
		{
			if (!preLoadPlugin (p))
			{
	            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "preLoadPlugin() on " << p.file() << " returned false") ;
				res=false;
			}
			else if (!p.open())
			{
	            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "p.open() " << p.file()<< " returned false")  ;
			    notifier(p.file()) ; 
				res=false;
			}
			else
			{
				try
				{
					if (!postLoadPlugin (p))
					{
						// PostLoad error means that the plugin cannot be used. Close it.
						res=false;
						p.close();
					}
				}
				catch (exception& e)
				{
	                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "exception: " << e) ;
					p.close();
					ACS_THROW(PostLoadError(e, "Unable to execute post-load configuration of plugin [" + p.description() + "]")); // PRQA S 3081
				}
			}
		}
	}
	_loaded=true;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "returning: " << boolalpha << res) ;

	return res;
}

const Plugin & PluginLoader::operator[](size_t i) const
{
	if(i>=size())
	{
		ostringstream os;
		os<<"Index "<<i<<" is out if bound : size is "<<size() ;
		ACS_THROW(exBoundsException(os.str())); // PRQA S 3081
	}
	return *_plugins[i];
}


Plugin & PluginLoader::operator[](size_t i)
{
	if(i>=size())
	{
		ostringstream os;
		os<<"Index "<<i<<" is out if bound : size is "<<size() ;
		ACS_THROW(exBoundsException(os.str())); // PRQA S 3081
	}
	return *_plugins[i];
}


size_t  PluginLoader::size()  const
{
	return _plugins.size();
}

const Plugin & PluginLoader::failed(bool optional) const // PRQA S 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	for(size_t i=0; i<size(); i++)
	{
		Plugin const & p=(*this)[i];
		if(p.handle() == 0 ) {
			if( optional || !p.optional() ) {
				return p;
            }
        }
	}
	ACS_THROW(Nofailed("No plugin failed to load.")); // PRQA S 3081
}


Plugin & PluginLoader::failed(bool optional) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	for(size_t i=0; i<size(); i++)
	{
		Plugin & p=(*this)[i];
		if(p.handle() == 0 ) {
			if( optional || !p.optional() ) {
				return p;
            }
        }
	}
	ACS_THROW(Nofailed("No plugin failed to load.")); // PRQA S 3081
}


_ACS_END_NAMESPACE
