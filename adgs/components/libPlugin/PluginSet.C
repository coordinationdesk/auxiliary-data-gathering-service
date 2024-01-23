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
	
	Revision 2.6  2013/02/19 14:35:03  marpas
	coding best practices applied
	PluginLoaderNotifier class introduced
	qa rules
	
	Revision 2.5  2012/12/01 19:43:00  marpas
	qa rules
	
	Revision 2.4  2012/02/13 11:00:26  marpas
	refactoring in progress
	
	Revision 2.3  2011/07/08 12:18:29  marpas
	debug improved
	
	Revision 2.2  2006/06/23 08:52:08  ivafam
	StringUtils::compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 2.1  2006/06/20 17:02:30  ivafam
	Method compareNocase replaced with StringUtils::equalsNoCase
	
	Revision 2.0  2006/02/28 08:36:44  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.10  2005/06/10 12:30:30  integrator
	interface slightly changed
	
	Revision 1.9  2004/12/03 17:48:44  marfav
	Header fixed
	
	Revision 1.8  2003/07/11 12:59:19  paoscu
	exException::clone implemented
	
	Revision 1.7  2003/04/30 16:41:31  paoscu
	using namespace std no longer into acs includes
	
	Revision 1.6  2003/03/26 10:09:36  paoscu
	Bug fixed in resource analysis.
	
	Revision 1.5  2003/03/25 18:11:01  paoscu
	Newer layout for rsResourceSet.
	
	Revision 1.4  2003/03/25 18:00:06  paoscu
	New layout for rsResourceSet.
	
	Revision 1.3  2003/02/20 18:16:20  marpas
	StringUtils::equalsNoCase fixed for empty strings
	
	Revision 1.2  2002/11/25 17:43:53  paoscu
	Now load method takes a PluginLoader reference.
	
	Revision 1.1.1.1  2002/10/29 18:24:47  paoscu
	Import libPlugin
	
	

*/

#include <PluginSet.h>
#include <rsPushNSpace.h>
#include <StringUtils.h>
#include <Filterables.h>
#include <sstream>
#include <dlfcn.h>
#include <cctype>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(PluginSet) 


PluginSet::PluginSet()  
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}

PluginSet::~PluginSet() ACS_NOEXCEPT 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;// PRQA S 4631 
}

void PluginSet::getPluginParameters(rsResourceSet & rs, PluginLoader &loader, size_t i)
{
	ostringstream os;
	os<<"plugins.plugin["<<i<<"]";
	string prefix=os.str();
	rsPushNSpace nspace(rs,prefix);

	string file, description; // PRQA S 4107
	bool optional = false ;
	int mode=0; //initial value is 0
	rs.getValue("file" , file);
	rs.getValue("description" , description);
	rs.getValue("optional" , optional);

	bool RTLD_LAZY_declared=false;
	bool RTLD_NOW_declared=false;
	vector<string> modes ;
	try
	{
		rs.getArray("mode", modes);
		// PRQA S 4238, 4244 2
        for (size_t m=0; m< modes.size() ; m++) {
			if (StringUtils::equalsNoCase(modes[m],"RTLD_GLOBAL"))
			{
				mode|=RTLD_GLOBAL; // PRQA S 3003
			}
			else if(StringUtils::equalsNoCase(modes[m],"RTLD_LAZY"))
			{
				ACS_COND_THROW(RTLD_NOW_declared,BadResourceFile("Error in configuration : You can't use RTLD_LAZY and RTLD_NOW togheter.")) ; // PRQA S 3081

				RTLD_LAZY_declared=true;
				mode|=RTLD_LAZY; // PRQA S 3003
			}
			else if(StringUtils::equalsNoCase(modes[m],"RTLD_NOW"))
			{
				ACS_COND_THROW(RTLD_LAZY_declared,BadResourceFile("Error in configuration : You can't use RTLD_LAZY and RTLD_NOW togheter.")) ; // PRQA S 3081

				RTLD_NOW_declared=true;
				mode|=RTLD_NOW; // PRQA S 3003
			}
            else {
                // nothing
            }			
		}
	}
	catch(rsResourceSet::NotFoundException &)
	{
		mode=RTLD_LAZY|RTLD_GLOBAL;
	}

	loader.add(file,optional,description,mode);
}

size_t PluginSet::load(rsResourceSet & rs, PluginLoader &loader) // PRQA S 4020
{
	if(not rs.hasKey("plugins") ) { //nothing to load
		return 0 ;
	}

	string path;

	try
	{
		rs.getValue("pluginsPath",path);
		loader.setPath(path) ;
	}
	catch(rsResourceSet::NotFoundException &ex)
	{
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "exception got (ignored) " << ex) ;
	}
			

	size_t plugins = 0 ;
	try {	
		plugins=rs.getArraySize("plugins.plugin");
	}
	catch(rsResourceSet::NotFoundException &ex)
	{
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "exception got (ignored) " << ex 
                << "returning: " << plugins) ;
		return plugins;
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "there are " << plugins << " plugins.") ;
		
		
	for(size_t i=0; i<plugins; i++)
	{
		try
		{
            getPluginParameters(rs, loader, i) ;
		}//try
		catch(exception &e)
		{
			ostringstream errorStream;
			errorStream<<"Error in plugins["<<i<<"]";
			ACS_THROW(BadResourceFile(e, errorStream.str() ));
		}
	}//for(size_t i=0; i<N; i++)

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "returning: " << plugins) ;
	return plugins;
}


_ACS_END_NAMESPACE

