/*
	Copyright 1995-2020, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	$Prod: A.C.S. libSRVImporterApp$
        
*/ 

#include <SRVImporterApp.h>
#include "SRVImporterAppV.h"
#include <Filterables.h>

#include <ImporterTableName.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(SRVImporterApp)

static SRVImporterAppV version;


// PRQA S 1020 S_Macros
#ifndef IMPORTER_SUBSYSTEM
#define IMPORTER_SUBSYSTEM "SRV"
#endif

#ifndef IMPORTER_APPNAME
#define IMPORTER_APPNAME "SRVImporter"
#endif
// PRQA L:S_Macros
namespace {
    Application::GenericBuilder<SRVImporterApp> theApplicationBuilder(IMPORTER_APPNAME, IMPORTER_SUBSYSTEM) ; // PRQA S 1021
}


SRVImporterApp::SRVImporterApp(const string &appName, const string &subSystem):
	Importer(appName, subSystem)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	//Customize the table names for this Import specialization
	ImporterTableName::instance()->set( ImporterTableName::t_receptionrules, "t_srv_receptionrules" );
}


SRVImporterApp::~SRVImporterApp() ACS_NOEXCEPT {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}


void SRVImporterApp::getConfigurationSpaceInfo( string & mainNamespace, vector<string> & confSpaceList ) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	mainNamespace = "SRVImporter";

	confSpaceList = { mainNamespace };

}


_ACS_END_NAMESPACE

