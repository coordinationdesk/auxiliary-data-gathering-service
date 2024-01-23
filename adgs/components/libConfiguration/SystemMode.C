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
	Revision 5.1  2013/11/15 13:39:35  marpas
	improving debug message
	
	Revision 5.0  2013/06/18 10:14:19  marpas
	adoption of libException 5.x standards
	
	Revision 2.11  2013/03/14 15:23:13  marpas
	interface rationalization
	coding best practices applied
	
	Revision 2.10  2013/01/24 16:55:43  marpas
	useless macros removed
	minor improvements
	
	Revision 2.9  2012/12/01 18:47:10  marpas
	qa rules
	
	Revision 2.8  2012/02/13 11:07:01  marpas
	refactoring in progress
	
	Revision 2.7  2011/02/28 17:09:33  giucas
	debug improved, if file found and read, but resource not found, the exception raised is still SystemMode::ModeExcpetion in order to have no evidence of it in the log (see Application.C)
	
	Revision 2.6  2009/08/03 18:24:22  marpas
	less connection and faster design
	
	Revision 2.5  2009/01/30 15:16:49  marpas
	ModeException thrown instead of more general IO exception
	
	Revision 2.4  2008/12/16 16:55:27  marpas
	SystemMode final implementation
	
	Revision 2.3  2008/12/16 14:51:11  marpas
	Standalone SystemMode implemented
	
	Revision 2.2  2008/12/15 18:51:06  marpas
	SystemMode implementation done
	
	Revision 2.1  2008/12/15 18:43:52  marpas
	SystemMode implementation in progress
	
		

*/


#include <SystemModeFactory.h>
#include <SystemMode.h>
#include <rsResourceSet.h>
#include <Dir.h>
#include <File.h>
#include <Filterables.h>
#include <fstream>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(SystemMode)

string SystemMode::_systemGlobal = "SystemMode" ;
// constructor
SystemMode::SystemMode(const string &app, const string &sub) :
	_app(app),
	_sub(sub)
{
}


SystemMode::SystemMode(const SystemMode &o) :
	_app(o._app),
	_sub(o._sub)
{
}

SystemMode &SystemMode:: operator=(const SystemMode &o) 
{
	if (this != &o) {
		_app = o._app ;
		_sub = o._sub ;
	}
	return *this ;
}


SystemMode::~SystemMode() throw() {}

string SystemMode::path(const string &app, const string &sub) const 
{
	return SystemModeFactory::commonConfigurationPath() + 
		   "/" +
		   buildPathWithoutExtension(app,sub) + "." + defaultExtension() ;
}

string SystemMode::buildPathWithoutExtension(const string &app, const string &sub) 
{
	return app + ( sub != "" ? "_" + sub : "") ; // PRQA S 3383, 3385
}


string SystemMode::defaultExtension()
{
	return "mode" ;
}


string SystemMode::load()
{
	string mode ;
	// fill alternative vectors sorted in 
	// their evaluation order
	vector<string> paths ;
	paths.push_back(path(_app,_sub)) ;
	paths.push_back(path(_app,"")) ;
	paths.push_back(path(systemGlobal(),_sub)) ; 
	paths.push_back(path(systemGlobal(),"")) ;

	ostringstream osex ;
    size_t pathsize = paths.size() ;
	for (size_t i=0; i < pathsize; ++i) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "trying: " << paths[i])
		osex << (i ? ", " : " ") << "\"" << paths[i] << "\"" ; // PRQA S 3054, 3380
		if (not File::exists(paths[i])) { // does not exists: go over
			continue ;
        }
		try {
			// open ... 
			ifstream file(paths[i].c_str()) ;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading file: " << paths[i])
			if (file.bad()) {
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "cannot open file: " << paths[i]) ;
				ACS_THROW (exFileOpenException("Cannot open " + paths[i] ,errno)) ; // PRQA S 3081
			}
			// ... load ... 
			rsResourceSet myrs ;
			try {
				file >> myrs ;
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "file: " << paths[i] << " loaded") ;
			}
			catch (exception &e) {
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "cannot load file: " << paths[i]) ;
				ACS_THROW(ModeException(e,"Cannot load/parse path:\"" + paths[i] +"\"")) ;
			}
			
			try {
				// ... get value if any ... (else not found exception)
				myrs.getValue("SystemMode",mode) ;
				return mode ;
			}
			catch (exception &e) {
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "cannot find \"SystemMode\" in file: " << paths[i]) ;
				osex << "(*)" ;
			}		
		}
		catch(exception &x) { // exception ... go over
			ACS_LOG_WARNING( x << "The above exception has been ignored") ;
			continue ;
		}

	}

	ACS_THROW(ModeException("Cannot load (or format corrupted/invalid) any of file(s): " + osex.str())) ;	// PRQA S 3081
}

const string &SystemMode::app() const throw()  // PRQA S 4120
{
	return _app ;
}

const string &SystemMode::sub() const throw()  // PRQA S 4120
{
	return _sub ;
}

void SystemMode::app(const string &v) // PRQA S 4121
{
	_app = v ;
}

void SystemMode::sub(const string &v)  // PRQA S 4121
{
	_sub = v ;
}

string SystemMode::systemGlobal()
{
	return _systemGlobal ;
}

_ACS_END_NAMESPACE


