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

*/

#include <ApplicationDebug.h>

#include <Application.h>
#include <File.h>
#include <Filterables.h>
#include <rsResourceSet.h>
#include <exRTDebug.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ApplicationDebug) 

ApplicationDebug::ApplicationDebug(const string &path) : 
	_path(path)
{	
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_VLO_VERB);
}

ApplicationDebug::~ApplicationDebug() ACS_NOEXCEPT
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_VLO_VERB);
}

void ApplicationDebug::setPath(const std::string &path) // PRQA S 4121
{
	_path = path ;
}

void ApplicationDebug::updateDebug()  // PRQA S 4020
{
	// Legacy call... to be simplified

	refreshDebug() ; // refresh it now !
}

void ApplicationDebug::refreshDebug()  // PRQA S 4020, 4211
{
	try {
        bool app = Application::exists() ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "app is " << (app ? "set" : "<null>")) // PRQA S 3380
		if (!app) { // no application has been created till now .. nothing to do 
			return ;
        } 
		
		// which is the current app mode ?	
		string mode = exRTDebug::instance()->mode() ;
		if (mode != "") {
			mode += "/" ;
        }
		// build the path for the debug file
		// note that mode concur to path name 
		string file = _path + "/" + mode + Application::instance()->applicationName() + ".dbg" ;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "file is " << file) ;
		exRTDebug &rtdbg = *exRTDebug::instance() ;
		if (!File::exists(file)) { // the file has to be created ?
			try {
				string dbgInfo ;
				rtdbg.getDebugRefs(dbgInfo, false) ;
				ofstream out(file.c_str()) ; // creae it
				out << dbgInfo ; // populate it
				out << "\n" ; // last newline
				if (!out.good()) {
					ACS_THROW(exIOException("Could not write " + file,errno )) ; // PRQA S 3081
				}
				// notify the file has been written
				ACS_LOG_INFO("ApplicationDebug wrote file: \"" << file << ".") ;
			}
			catch(exception &e) {
				ACS_LOG_WARNING("ApplicationDebug could not write \"" << file << "\" exception was:" << e) ;
				return ;
			}
		}

		// read the file 	
		ifstream ifs(file.c_str()) ;
		rsResourceSet rs ;
		ifs >> rs ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "file read") ;
		
		// and set the values 
		vector<string> debugvector ;
        rtdbg.getDebugRefs(debugvector) ;
        size_t dbgvsize = debugvector.size() ;
		for (size_t i = 0; i < dbgvsize ; ++i) {
			try {
				int v ;     // PRQA S 4101
				rs.getValue(debugvector[i], v) ;
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, " " << debugvector[i] << "=" << v) ;
				rtdbg.setDebug(debugvector[i], v) ;
			}
			catch (...) {
				; // nothing to do with exception ... skip to next
			}
		}
		
		// notify the new debug set (should go in the log for further reference
		string newSet ;
		rtdbg.getDebugRefs(newSet) ;
		exostream::getDefaultOutLogger() << newSet << "\n" ;
		
	}
	catch(exception &x) {
		ACS_LOG_WARNING("ApplicationDebug exception got: " << x << "\nIgnoring and continuing - debug has not changed.") ;
	}
}

_ACS_END_NAMESPACE
