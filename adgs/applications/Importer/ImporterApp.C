/*
	Copyright 1995-2020, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	$Prod: A.C.S. Importer$
        
*/ 

#include <ImporterApp.h>

#include <ImporterVersion.h>

#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(ImporterApp)

namespace { // unnamed
    // version management
    ImporterVersion version ;
}


// PRQA S 1020 S_Macros
#ifndef IMPORTER_SUBSYSTEM
#define IMPORTER_SUBSYSTEM "SDF"
#endif

#ifndef IMPORTER_APPNAME
#define IMPORTER_APPNAME "Importer"
#endif
// PRQA L:S_Macros
namespace {
    Application::GenericBuilder<ImporterApp> theApplicationBuilder(IMPORTER_APPNAME, IMPORTER_SUBSYSTEM) ; // PRQA S 1021
}


ImporterApp::ImporterApp(const string &appName, const string &subSystem):
	Importer(appName, subSystem)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

ImporterApp::~ImporterApp() ACS_NOEXCEPT {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void ImporterApp::getConfigurationSpaceInfo( string & mainNamespace, vector<string> & confSpaceList ) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	mainNamespace = "Importer";

	confSpaceList = { mainNamespace, "EmailUserNotification" };
}


_ACS_END_NAMESPACE

