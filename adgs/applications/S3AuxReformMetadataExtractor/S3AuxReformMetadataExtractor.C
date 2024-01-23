/*

	Copyright 1995-2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Sentinel3 Auxiliary Reformatter application $

	$Id$

	$Author$
	
	$Log$
	Revision 1.4  2013/10/04 14:22:21  matteo.airoldi
	Added Qualified Id inside the generated metadata file
	
	Revision 1.3  2013/09/13 13:59:12  marfav
	Added Satellite Id and Mission Number to the extracted metadata
	
	Revision 1.2  2013/06/13 17:17:36  marfav
	First issue completed
	
	Revision 1.1.1.1  2013/06/13 10:06:40  marfav
	Importing S3AuxReformMetadataExtractor
	


*/

#include <S3AuxReformMetadataExtractor.h>

#include <exMacros.h>
#include <Filterables.h>
#include <OcadaJobOrder.h>

#include <XMLIstream.h>

#include <dbPersistentFactoryChain.h>
#include <dbInventoryObject.h>
#include <dbConnectionPool.h>
#include <SatelliteName.h>

#include <XMLOstream.h>

#include <S3AuxRefMEV.h>

using namespace acs;
using namespace std;

namespace {
	const string taskName_ ("METADATA_EXTRACTOR");
	const string triggerArrayKey_ ("List_of_TriggeringFiles.TriggeringFile");
	const string fileNameKey_ ("FileName");
	const string fileVersionKey_ ("FileVersion");

}


ACS_CLASS_DEFINE_DEBUG_LEVEL(S3AuxReformMetadataExtractor)

static S3AuxRefMEV Version;

Application::GenericBuilder<S3AuxReformMetadataExtractor> theApplicationBuilder("S3AuxReformMetadataExtractorCore", "ADC");


//Class constructor
S3AuxReformMetadataExtractor::S3AuxReformMetadataExtractor(const string& appName, const string& subSys) :	
#if   defined (HAS_MISSION_INCLUDES)
PROJECT_APP(appName, subSys)
#else
StandaloneApp(appName, subSys)
#endif
{
	// exCLASS_INIT_DEBUG(S3AuxReformMetadataExtractor_DEBUG) ;
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"S3AuxReformMetadataExtractor::" << __LINE__ << " Id: " << Id) ;
} 

//Class destructor
S3AuxReformMetadataExtractor::~S3AuxReformMetadataExtractor() throw() 
{  
}

void S3AuxReformMetadataExtractor::usage(int argc,  char const * const *argv) const
{
	excerr << "usage: " << argv[0] << " <JobOrder file> \n";
}

int S3AuxReformMetadataExtractor::main(int argc, char const * const * argv, char const * const * env)		
{																								
	if (argc < 2) 
	{ 
		usage(argc, argv) ;
		return 1 ;
	}
	
	// This application does not use logfiles
	int returnCode = 0;
	try
	{
		string joFileName(argv[1]);
		returnCode = execute(joFileName) ;
	}
	catch(exception& e)
	{
		ACS_LOG_ERROR("Error detected during the processing. Exiting in ERROR.");
		ACS_LOG_NOTIFY_EX(e);

		returnCode = 255;

	} 

	ACS_LOG_INFO ("Application exiting with code [" << returnCode << "]");
	return 0;								
}

/////////////////////////////////////////////////////////////////////////////////////////
// Private methods 
/////////////////////////////////////////////////////////////////////////////////////////


int S3AuxReformMetadataExtractor::execute(const string & jobOrderFileName)
{

	ACS_LOG_INFO ("Application start. Initializing.");
	ACS_LOG_INFO ("JobOrder file is [" << jobOrderFileName << "]");

	// Read the jobOrder
	OcadaJobOrder theJobOrder;

	try
	{
		XMLIstream is (jobOrderFileName);
		is >> theJobOrder ;
	}
	catch (exception& e)
	{
		ostringstream os;
		os <<  "Unable to load JobOrder from file  \"" << jobOrderFileName << "\"";
		throwException (S3AuxReformMetadataExtractorException (e, os.str() ));
	}

	// extract the relevant task from the jobOrder
	Task task (theJobOrder.getTask(taskName_));
	
	// Check that exactly one input has been provided
	vector<InputDescriptor> idl;
	task.getInputList (idl);

	if (1 != idl.size())
	{
		ostringstream os;
		os <<  "Too many inputs [" << idl.size() << "] in task [" << taskName_ << "] - JobOrder is \"" << jobOrderFileName << "\"";
		throwException (S3AuxReformMetadataExtractorException (os.str() ));
	}
	
	// Get the unique alternative and extract the input file name
	InputAlternative alt ( *(static_cast<InputAlternative*> ((idl.at(0))[0])) );

	vector<string> inputList;
    alt.getPathNames(inputList);

	// Just one input is expected now
	if (1 != inputList.size())
	{
		ostringstream os;
		os <<  "Too many inputs [" << idl.size() << "] in task [" << taskName_ << "] - JobOrder is \"" << jobOrderFileName << "\"";
		throwException (S3AuxReformMetadataExtractorException (os.str() ));
	}

	string inputName (inputList.at(0));

	// The input file name is now known
	ACS_LOG_INFO ("Input file name is [" << inputName << "]");

	
	// Try to retrieve also the triggering file information to create the metadata file
	TriggeringFile theTrigger;
	try
	{
		theTrigger = getTriggerFile (theJobOrder.getParametersFileName());
	}
	catch (exception& e)
	{
		ostringstream os;
		os <<  "Failed to gather information on the triggering file";
		throwException (S3AuxReformMetadataExtractorException (e, os.str() ));
	}

	try
	{
		createMetadata (inputName, theTrigger);
	}
	catch (exception& e)
	{
		ostringstream os;
		os <<  "Failed to create the dbInventory metadata file";
		throwException (S3AuxReformMetadataExtractorException (e, os.str() ));
	}


	// If here no problems occurred. So return 0.
	return 0;

}

/**
The medatata structure to provide is the following (used by S3AuxReformatterCore)
<?xml version ="1.0" encoding="UTF-8"?>
<Root>
	<ObjectMetadata>
		<FileName>S3A_TEST_MPL_ORBPRE_20130330T222957_20130401T231834_0011.EOF</FileName>
		<FileVersion>0011</FileVersion>
		<FileType>MPL_ORBPRE</FileType>
		<FileClass>OPER</FileClass>
		<ValidityStart>2013-03-30T22:29:57.000000</ValidityStart>
		<ValidityStop>2013-04-01T23:18:34.000000</ValidityStop>
		<SatName>Sentinel-3</SatName>
		<SatId>40</SatId>
		<Mission>2</Mission>
		<QualifiedId>2</QualifiedId>
		<QualityInfoValue>DEGRADED</QualityInfoValue>  PASSED , DEGRADED or FAILED
	</ObjectMetadata>
</Root>
**/

void
S3AuxReformMetadataExtractor::createMetadata (const string& inputName, const TriggeringFile& trigger)
{
	dbConnectionPool &pool = *Application::instance()->getDefaultConnectionPool();
	dbConnectionWrapper theWrapper (pool);
	dbConnection& conn (theWrapper);

	string metadataName (inputName+".metadata");
	ACS_LOG_INFO ("Preparing metadata file [" << metadataName << "]");
	// Load the inventory record and use it to create the metadata file
	ACS_SMARTPTR <dbInventoryObject> obj( dynamic_cast<dbInventoryObject*>(dbPersistentFactoryChain::instance()->newObject("dbInventoryObject", conn.getKey())));

	// Grap a connection and keep it till the end
	obj->setFileName (trigger.fileName);
	obj->setFileVersion (trigger.fileVersion);
	if (!obj->load (conn))
	{
		// The object does not exists in the database
		ostringstream os;
		os << "Record on t_inventory not found [" << trigger.fileName << ", " << trigger.fileVersion << "]";
		throwException (S3AuxReformMetadataExtractorException (os.str() ));
	}

	// Let's start collecting data... what do we need now?
	DateTime valStart (obj->getValidityStart());
	DateTime valStop (obj->getValidityStop());
	string filetype (obj->getFileType());
	string fileclass (obj->getFileClass());

	ostringstream osvalstart, osvalstop;
	valStart.asISO8601(osvalstart, 6);
	valStop.asISO8601(osvalstop, 6);

	// Get the satellite name
	bool satIdSet;
	int satId = obj->getSatelliteId(&satIdSet);
	string satName (satIdSet ? SatelliteName::instance("")->name (satId) : "");
	ostringstream ossatid;
	if (satIdSet)
	{
		// In case the satellite is set populate ossatid
		ossatid << satId;
	}

	bool missionSet;
	int missionNumber = obj->getMission(&missionSet);
	ostringstream osmission;
	if (missionSet)
	{
		// In case the mission is set populate osmission
		osmission << missionNumber;
	}

	// Get the association between qualified flag and qualified values
	NameIdAssociator<int> qualifyAssociation ("t_qualifiedflag", "id", "name", "");
	qualifyAssociation.load();
	string qualifyValue (qualifyAssociation.name (obj->getQualified()));

	ofstream ofs (metadataName.c_str());
	XMLOstream os (ofs, "Root");
	XMLOstream::Tag metadataTag (os, "ObjectMetadata");
	{ XMLOstream::Tag innerTag (os, "FileName", trigger.fileName, "", true); }
	{ XMLOstream::Tag innerTag (os, "FileVersion", trigger.fileVersion, "", true); }
	{ XMLOstream::Tag innerTag (os, "FileType", filetype, "", true); }
	{ XMLOstream::Tag innerTag (os, "FileClass", fileclass, "", true); }
	{ XMLOstream::Tag innerTag (os, "ValidityStart", osvalstart.str(), "", true); }
	{ XMLOstream::Tag innerTag (os, "ValidityStop", osvalstop.str(), "", true); }
	{ XMLOstream::Tag innerTag (os, "SatName", satName, "", true); }
	{ XMLOstream::Tag innerTag (os, "SatId", ossatid.str(), "", true); }
	{ XMLOstream::Tag innerTag (os, "Mission", osmission.str(), "", true); }
	{ XMLOstream::Tag innerTag (os, "QualifiedId", obj->getQualified(), "", true); }
	{ XMLOstream::Tag innerTag (os, "QualityInfoValue", qualifyValue, "", true); }

	ACS_LOG_INFO ("Metadata file generation completed.");
	
	
}

// The parameters file format is the following
/**
<?xml version ="1.0" encoding="UTF-8"?>
<Parameter_File>
	<List_of_TriggeringFiles count="3">
		<TriggeringFile>
			<FileName>fname1</FileName>
			<FileVersion>fversion1</FileVersion>
			<GeneratingOrder>1</GeneratingOrder>
			<TriggeringUserDistribution>user</TriggeringUserDistribution>
		</TriggeringFile>
		<TriggeringFile>
			<FileName>fname2</FileName>
			<FileVersion>fversion2</FileVersion>
			<GeneratingOrder>2</GeneratingOrder>
			<TriggeringUserDistribution>user</TriggeringUserDistribution>
		</TriggeringFile>
		<TriggeringFile>
			<FileName>fname3</FileName>
			<FileVersion>fversion3</FileVersion>
			<GeneratingOrder>3</GeneratingOrder>
			<TriggeringUserDistribution>user</TriggeringUserDistribution>
		</TriggeringFile>
	</List_of_TriggeringFiles>
</Parameter_File>
**/

S3AuxReformMetadataExtractor::TriggeringFile 
S3AuxReformMetadataExtractor::getTriggerFile (const string& paramFile) const
{
	// Only one file is expected to trigger the order
	// if more than one do not scream and consider just the first in the list
	ACS_LOG_INFO ("Parsing parameter file for triggering file [" << paramFile << "]");

	XMLIstream is (paramFile);
	rsResourceSet rs;

	is >> rs;

	// How many files are actually listed
	size_t triggers ( rs.getArraySize (triggerArrayKey_) );
	if (1 != triggers)
	{
		ACS_LOG_WARNING ("Found [" << triggers << "] triggering files instead of the 1 expected. Try to continue with the first one.");
	}

	TriggeringFile theFile;
	rs.getValue (triggerArrayKey_ + "[0]." + fileNameKey_, theFile.fileName);
	rs.getValue (triggerArrayKey_ + "[0]." + fileVersionKey_, theFile.fileVersion);

	ACS_LOG_INFO ("Triggering file in DB is [" << theFile.fileName << ", " << theFile.fileVersion << "]");

	return theFile;
}


