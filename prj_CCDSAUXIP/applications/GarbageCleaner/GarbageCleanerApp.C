// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. GarbageCleaner $

	$Id$

	$Author$
	
	$Log$
	Revision 1.14  2016/12/16 12:28:59  francesco.avanzi
	now handles a MaxSize
	
	Revision 1.13  2016/07/08 13:21:19  marfav
	CSGACQ-84
	New project/application based log rotation management strategy enforced
	
	Revision 1.12  2016/01/29 11:47:05  marpas
	coding best practices application in progress
	
	Revision 1.11  2013/09/30 07:57:04  clanas
	removed exception notification for conf space replacement
	
	Revision 1.10  2012/02/14 15:18:15  marpas
	fixing dtor
	
	Revision 1.9  2010/06/15 09:54:12  marpas
	HAS_MISSION_INCLUDES management added
	
	Revision 1.8  2008/11/26 11:40:11  danalt
	Using ConfigurationSingleton instead of MergeConfiguration to load confspaces
	
	Revision 1.7  2007/04/20 10:49:25  ivafam
	Patch to remove libProc dependence of libGfeSatStore
	
	Revision 1.6  2006/10/03 16:36:08  davcas
	Added STANDALONE define and StandaloneApp
	
	Revision 1.5  2006/06/14 10:36:42  davcas
	Added project Harm
	
	Revision 1.4  2005/09/07 08:14:27  marpas
	Versioning fixed.
	

*/



#include <GarbageCleaner.h>
#include <GarbageCleanerApp.h>
#include <ConfigurationSingleton.h>
#include <XMLIstream.h>
#include <GCappV.h>
#include <sys/param.h> // PRQA S 1013
#include <SpawnProc.h>

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#ifndef GARBAGECLEANER_SUBSYSTEM
#define GARBAGECLEANER_SUBSYSTEM "SDF"
#endif

#ifndef GARBAGECLEANER_APPNAME
#define GARBAGECLEANER_APPNAME "GarbageCleaner"
#endif 

using namespace acs;
using namespace std;

namespace {
    GCappV version;
	Application::GenericBuilder<GarbageCleanerApp> theApplicationBuilder(GARBAGECLEANER_APPNAME, GARBAGECLEANER_SUBSYSTEM) ;
}

GarbageCleanerApp::GarbageCleanerApp (std::string const & appName, std::string const & subSystem)
    : PROJECT_APP (appName, subSystem)
{
}

GarbageCleanerApp::~GarbageCleanerApp() throw() 
{
}

void GarbageCleanerApp::init ()
{
	ACS_LOG_PRIVINFO("Loading configuration.... ") ;
    loadConfiguration(false);
	ACS_LOG_PRIVINFO("... loaded ") ;
}

const string GarbageCleanerApp::getNodeName()
{
  char machinename[MAXHOSTNAMELEN+1];
  gethostname(machinename, sizeof(machinename));
  return machinename;
}

int GarbageCleanerApp::main (int argc, char const* const* argv, char const* const* env)
{
//	Application::rotateLog() ; // open the first log
	
	/////////////////////////////////////////
	SpawnProc spawnproc("") ; // just to force library use
	/////////////////////////////////////////
	
	init();
	Application::rotateLog() ; // open the first log
	GarbageCleaner remove;
	remove.init();
	return remove.run();
}
