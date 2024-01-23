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
	Revision 5.0  2013/06/19 08:51:08  marpas
	adoption of libException 5.x standards
	
	Revision 2.7  2013/02/19 14:35:03  marpas
	coding best practices applied
	PluginLoaderNotifier class introduced
	qa rules
	
	Revision 2.6  2012/12/01 19:43:00  marpas
	qa rules
	
	Revision 2.5  2012/02/13 11:00:26  marpas
	refactoring in progress
	
	Revision 2.4  2012/01/31 17:33:57  marpas
	 removing ompiler warnings
	
	Revision 2.3  2011/03/01 11:55:11  marpas
	GCC 4.4.x support
	
	Revision 2.2  2010/08/03 14:17:56  nicvac
	Multiple paths handling.
	
	Revision 2.1  2008/11/24 11:16:06  marpas
	getPluginList method added
	
	Revision 2.0  2006/02/28 08:36:44  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 17:48:44  marfav
	Header fixed
	
	Revision 1.1  2004/06/04 08:19:14  marfav
	Importing DirPluginSet
	

*/


#include <DirPluginSet.h>

#include <Dir.h>
#include <RegExp.h>
#include <Filterables.h>
#include <algorithm>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(DirPluginSet)



// ************ STL HELPER CLASS ************

class PluginAdder
{
	public:
		PluginAdder (PluginLoader& loader, bool optional, int mode) :
			_loader(loader), // PRQA S 2528
			_optional(optional),
			_mode(mode)
		{}

		void operator() (const string& plugin)
		{
			_loader.add (plugin, _optional, plugin, _mode);
		}

	private:
        PluginAdder() ; // not implemented
		PluginLoader& _loader;
		bool _optional;
		int _mode;
};

// ************ STL HELPER CLASS ************


DirPluginSet::DirPluginSet(const vector<string> &basePaths, const string &regExp) :
		PluginSet(),
		_basePaths(basePaths),
		_currentScannedPath(""),
		_regExp(regExp),
        _theList()
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}


DirPluginSet::DirPluginSet(const string &basePath,  const string &regExp) :
		PluginSet(),
		_basePaths(1,basePath),
		_currentScannedPath(""),
		_regExp(regExp),
        _theList()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}

DirPluginSet::~DirPluginSet() ACS_NOEXCEPT 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ; // PRQA S 4631
}


bool
DirPluginSet::scanDir (vector<string>& pluginList)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "begin" ) ;

	pluginList.clear();
	bool result = true;

	try
	{
        // PRQA S 4238 2
		for ( vector<string>::const_iterator it=_basePaths.begin(); it!=_basePaths.end(); ++it  ) 
		{

			_currentScannedPath = *it;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "scanning dir [" << *it << "]") ;

			Dir dir(*it);
			RegExp reg(_regExp);
			for (unsigned long i=0; i<dir.size(); i++)
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "matching file [" << dir[i] << "]") ;

				if (reg.match(dir[i]))
				{
					pluginList.push_back ( *it+"/"+dir[i]);
					ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "plugin [" << pluginList.back() << "] added to list") ;
				}
			}
		}
		
	}
	catch (exception& ex)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "exception caught: " << ex) ;
		result = false;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "end - returning " << boolalpha << result) ;

	return result;
}


void
DirPluginSet::load (PluginLoader& loader, bool optional, int mode)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "begin" ) ;

	if ( !scanDir(_theList) )
	{
		// Cannot scan the directory
		ACS_THROW(BadResourceFile("Unable to scan path [" + _currentScannedPath + "]")); // PRQA S 3081 
	}

	for_each (_theList.begin(), _theList.end(), PluginAdder (loader, optional, mode));

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "end") ;

}

const vector<string> &DirPluginSet::getPluginList() const  // PRQA S 4120
{
	return _theList ;
}


_ACS_END_NAMESPACE

