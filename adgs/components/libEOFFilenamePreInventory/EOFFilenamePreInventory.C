// PRQA S 1050 EOF
/*
	Copyright 1995-2021 Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: libEOFFilenamePreInventory $

	$Id$

	$Author$

	$Log$
	Revision 1.9  2016/03/11 09:08:44  tergem
	DefaultJobOrder dependency removed. Parameter File read from context
	
	Revision 1.8  2015/07/17 16:26:05  lucio.pulvirenti
	S3MPM-549: DS context managed. If unique_id tag not found, exception not thrown if properly configured.
	
	Revision 1.7  2014/07/24 14:46:31  lucio.pulvirenti
	S2PDGS-850: Flag added to configuration on filetype base to get filetype either from framework (default) or from filename.
	
	Revision 1.6  2014/02/11 11:10:08  francesco.avanzi
	better code practices applied && dbConv moved to db::toSQL
	
	Revision 1.5  2013/04/29 17:43:09  marpas
	base class interface changed
	
	Revision 1.4  2013/03/19 17:50:36  lucio.pulvirenti
	Implementation is over. Test performed.
	
	Revision 1.3  2013/03/18 17:46:26  lucio.pulvirenti
	work in progress.
	
	Revision 1.2  2013/03/15 18:09:51  lucio.pulvirenti
	Work in progress.
	
	Revision 1.1.1.1  2013/03/15 18:04:38  lucio.pulvirenti
	Import libEOFFilenamePreInventory
	
        
*/ 

#include <EOFFilenamePreInventory.h>
#include "EOFFilenamePreInventoryV.h"
#include <Filterables.h>

#include <ConfigurationSingleton.h>

#include <XMLIstream.h>
#include <ftmpstream.h>
#include <XMLProgDataConsumer.h>
#include <StringUtils.h>

#include <NameIdAssociator.h>

#include <dbQueryFactoryChain.h>
#include <dbGenQueryScopeGuard.h>
#include <dbInventoryObjectQuery.h>
#include <dbInventoryObject.h>
#include <dbSensorQuery.h>
#include <dbSatelliteQuery.h>
#include <dbFileTypeQuery.h>
#include <dbFileType.h>
#include <dbSensor.h>
#include <dbSatellite.h>
#include <dbFileTypeRecognition.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(EOFFilenamePreInventory)

namespace {
    EOFFilenamePreInventoryV version;
}

string const EOFFilenamePreInventory::_InventoryTimeForMissionBegin	="19000101T000000";
string const EOFFilenamePreInventory::_InventoryTimeForMissionEnd	="99991231T235959";

EOFFilenamePreInventory::EOFFilenamePreInventory() : 
    PreInventoryBase(),
	_currentFileName(),
	_configRegistrationNode(),
	_pdgsTailored()

{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: "<< Id );
	
}

EOFFilenamePreInventory::~EOFFilenamePreInventory() throw() 
{
// PRQA S 4631 L1
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
// PRQA L:L1
}

string EOFFilenamePreInventory::getClassId()
{
        //return the class name
	return "EOFFilenamePreInventory";
}
        

void EOFFilenamePreInventory::computeMetadata( const string& pathFileName, const string& fileType, Metadata& metadata, dbConnection &conn)
{
    //NB: the pathFileName passed to this method is an absolute path
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Method called");
	
	try 
    {
		{
			ostringstream msg;
			msg << "MetadataInfo:\n";
			msg << "  OrderId=" << getContext().metadataInfo.orderId << '\n';
			msg << "  pathFileName= " << pathFileName << '\n';
			msg << "  filetype from framework= " << fileType << '\n';
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, msg.str());
        }

		_currentFileName = pathFileName;
		
		FilenameElements fne;
		
		parseFilename(pathFileName, fileType, fne);
		
		metadata = prepareMetadata( fileType, fne, conn);
		
		          
 
	}
    catch( exception& ex)
    {
        ACS_THROW(exEOFFilenamePreInventoryException( ex, "computeMetadata Exception"));
    }
}      




Metadata EOFFilenamePreInventory::prepareMetadata( const string& fileType, const FilenameElements& fe, dbConnection &conn)
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Method called");

	//filling Metadata
	Metadata metadata;

	// *UNIQUE_ID
	string uniqueId = getUniqueId();
    if ( !uniqueId.empty()) {
        metadata.setMetadataValue( "Unique_Id", uniqueId); 
    }
	
	string fileName = fillMetadataFilename( _currentFileName, fileType, metadata, conn);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File will be inventoried with the name \"" << fileName << '\"');
	
	// *FILE_VERSION
	int fileVersion = 1;
	if( !fe.File_Version.empty() ) {
		istringstream is ( fe.File_Version, istringstream::in);
		is >> fileVersion;
	}
	metadata.setMetadataValue( "File_Version", getFormattedVersion(fileVersion));
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File_Version=\"" << getFormattedVersion(fileVersion) << '\"');

	// ORIGINATOR (OSS: System e' una stringa mentre Originator e' integer nella t_inventory: sara' l'inventory a trasformare la stringa system nel giusto Id
	string system = fe.Site_Centre;
	if( !system.empty() ) {
		metadata.setMetadataValue( "Originator", system);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "System=" << system);
	}

		
	// *GENERATION_TIME
	string generationTime;
	{
		if( fe.Creation_Date.empty()) {
			DateTime dt;            //today datetime
			generationTime = dt.YYYYMMDDThhmmss();
    	}
		else {
			generationTime = fe.Creation_Date;
		}
		string InvGenerationTime = "UTC=" + generationTime;
		metadata.setMetadataValue( "Generation_Time", InvGenerationTime);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generation_Time=" << InvGenerationTime );
	}

	// *VALIDITY_START
	{
		string validityStart = "UTC=";
		if( fe.Validity_Start.empty()) {
			validityStart += generationTime;
    	}
		else if( fe.Validity_Start == "00000000T000000" ) {
			validityStart += _InventoryTimeForMissionBegin;
		}
		else {
			validityStart += fe.Validity_Start;
		}	
		
		metadata.setMetadataValue( "Validity_Start", validityStart);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity_Start=" << validityStart);
	}

	// *VALIDITY_STOP
	{
		string validityStop = "UTC=";
		if( fe.Validity_Stop.empty() || fe.Validity_Stop == "99999999T999999" ) {
			validityStop += _InventoryTimeForMissionEnd;
    	}
		else {
			validityStop += fe.Validity_Stop;
		}
		metadata.setMetadataValue( "Validity_Stop", validityStop);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity_Stop=" << validityStop);
	}

	for( map<string, string>::const_iterator it = fe.additionalInfo.begin(); it != fe.additionalInfo.end(); ++it ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to insert in metadata \"" << it->first << "\" -> \"" << it->second << '\"');
		metadata.setMetadataValue( it->first, it->second);
	}
	
	// *VALIDITY_FLAG
	metadata.setMetadataValue( "Validity_Flag", "true");

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity_Flag= true");
	

	// *VALIDATION_DATE
	{
		DateTime dt;
		string creationTime = "UTC=" + dt.YYYYMMDDThhmmss();
		metadata.setMetadataValue( "Validation_Date", creationTime); 
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validation_Date=" << creationTime);
	}

	// *HEADER_SIZE 
	off_t headerSize = 0;
	metadata.setMetadataValue( "Header_Size", headerSize); 
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Header_Size=" << headerSize);
	

	// *DATA_SIZE prelevo la size reale
	off_t fileSize = File::size( _currentFileName);
	metadata.setMetadataValue( "Data_Size", fileSize);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Data_Size=" << fileSize);
		
	// *FILE_TYPE
	string metadataFiletype = fileType;
	bool getFtypeFromFramework = true;
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	try {
		rs.getValue( _configRegistrationNode + ".FiletypeFromFramework", getFtypeFromFramework);
		ACS_CLASS_WRITE_DEBUG_TH( ACS_LOW_VERB, "FiletypeFromFramework from " << fileType << " filetype configuration is " << boolalpha << getFtypeFromFramework );
	}
	catch(rsResourceSet::NotFoundException &e)
	{
		ACS_CLASS_WRITE_DEBUG_TH( ACS_LOW_VERB, "FiletypeFromFramework not occurring in " << fileType << " filetype configuration" );
	}

	if( !getFtypeFromFramework ) {
		if( fe.File_Type.empty() ) {
			ACS_LOG_WARNING("****FILETYPE FROM FILENAME IS EMPTY: check configuration. Going to set filetype from framework: " << metadataFiletype);
		}
		else {
			metadataFiletype = fe.File_Type;
		}
    }
	metadata.setMetadataValue( "File_Type", metadataFiletype);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File_Type=" << metadataFiletype);

	// *FILE_CLASS
	metadata.setMetadataValue( "File_Class", fe.File_Class);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File_Class=" << fe.File_Class);
	
	// *SATELLITE_ID 	
	string satelliteCode = fe.Mission;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SatelliteCode=" << satelliteCode );
	int satelliteId = getSatelliteIdFromSatelliteCode(satelliteCode, conn);
	if( satelliteId != 0 ) {
		metadata.setMetadataValue( "Satellite_Id", satelliteId);
    }

	// MISSION 
	string missionId = fe.Mission;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Mission_Id=" << missionId);
    int missionNumber = convertMissionId2INVMission( missionId);
	if( missionNumber != 0 ) {
		metadata.setMetadataValue( "Mission", missionNumber);
    }
	
	
	return metadata;
}







void EOFFilenamePreInventory::updateProductAndMetadata( const std::string& newVersion, Metadata& metadata)
{
   	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called");

// DO NOTHING
#if(0)		
	metadata.setMetadataValue( "File_Version", newVersion);
	
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File_Version field updated with the value \"" << newVersion << '\"');
			
	// QUALITY_INFO
	string qualityInfo = "DUP";
	metadata.setMetadataValue( "Quality_Info", qualityInfo);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Quality_Info field set to \"" << qualityInfo <<'\"');
	
#endif
			
}

string EOFFilenamePreInventory::doPackaging( Metadata& metadata, const string & firstPath, const string & filetype, const string & secondPath, dbConnection & )
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called");
			
	ACS_COND_THROW( !secondPath.empty(), exEOFFilenamePreInventoryException("No package foreseen: data filename should no be valorized \"" + secondPath + "\".") );

	return firstPath;
}


void EOFFilenamePreInventory::parseFilename( const string& pathFileName,const string& fileType, FilenameElements& fne) // PRQA S 4211
{

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called");

	string filename = File::getFileName(pathFileName);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename=\""  << filename << '\"');
	
	fne.File_Name = filename;
	
	int fieldStart;
	int fieldLen;
	getFilenameFieldInfo( "PreInventory.ConfigEOFFilenamePreInventory.MissionField.Start", "PreInventory.ConfigEOFFilenamePreInventory.MissionField.Length", 
								fieldStart, fieldLen);
	fne.Mission = filename.substr(fieldStart, fieldLen);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Mission=\"" << fne.Mission << '\"');

	getFilenameFieldInfo( "PreInventory.ConfigEOFFilenamePreInventory.ClassField.Start", "PreInventory.ConfigEOFFilenamePreInventory.ClassField.Length", 
								fieldStart, fieldLen);
	fne.File_Class = filename.substr(fieldStart, fieldLen);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File Class=\"" << fne.File_Class << '\"');


	getFilenameFieldInfo( "PreInventory.ConfigEOFFilenamePreInventory.FtypeField.Start", "PreInventory.ConfigEOFFilenamePreInventory.FtypeField.Length", 
								fieldStart, fieldLen);
	fne.File_Type = filename.substr(fieldStart, fieldLen);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File type=\"" << fne.File_Type << '\"');
	
	setConfigRegistrationNode(fileType);
	
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	rs.getValue( _configRegistrationNode + ".PdgsTailored", _pdgsTailored);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PdgsTailored=" << boolalpha << _pdgsTailored );
			
	if( _pdgsTailored ) {
		
		getFilenameFieldInfo( "PreInventory.ConfigEOFFilenamePreInventory.PDGS_EOFFS.SiteCenterField.Start", 
									"PreInventory.ConfigEOFFilenamePreInventory.PDGS_EOFFS.SiteCenterField.Length",  fieldStart, fieldLen);
		fne.Site_Centre = filename.substr(fieldStart, fieldLen);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Site Centre=\"" << fne.Site_Centre << '\"');
		
		getFilenameFieldInfo( "PreInventory.ConfigEOFFilenamePreInventory.PDGS_EOFFS.CreationDateField.Start",
									"PreInventory.ConfigEOFFilenamePreInventory.PDGS_EOFFS.CreationDateField.Length",  fieldStart, fieldLen);
		fne.Creation_Date = filename.substr(fieldStart, fieldLen);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creation Date=\"" << fne.Creation_Date << '\"');
				
// check the next character
		fieldStart += (fieldLen + 1);
// check if string is over
		if( fieldStart < int(filename.size()) ) {
			
			string suffix = filename.substr(fieldStart, 1);
			if( "S" == suffix || "V" == suffix ) {
								
				getFilenameFieldInfo( "PreInventory.ConfigEOFFilenamePreInventory.PDGS_EOFFS.ValidityStartField.Start", 
											"PreInventory.ConfigEOFFilenamePreInventory.PDGS_EOFFS.ValidityStartField.Length", fieldStart, fieldLen);
				fne.Validity_Start = filename.substr(fieldStart, fieldLen);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity Start Time=\"" << fne.Validity_Start << '\"');
						
				if( "V" == suffix ) {
					
					getFilenameFieldInfo( "PreInventory.ConfigEOFFilenamePreInventory.PDGS_EOFFS.ValidityStopField.Start", 
												"PreInventory.ConfigEOFFilenamePreInventory.PDGS_EOFFS.ValidityStopField.Length", fieldStart, fieldLen);
					fne.Validity_Stop = filename.substr(fieldStart, fieldLen);
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity Stop Time=\"" << fne.Validity_Stop << '\"');
					
				}
			}
		}
	} // if( _pdgsTailored ) 
	else {

		try {
			getFilenameFieldInfo( _configRegistrationNode + ".CreationDateField.Start", 
										_configRegistrationNode + ".CreationDateField.Length",  fieldStart, fieldLen);
			fne.Creation_Date = filename.substr(fieldStart, fieldLen);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creation Date=\"" << fne.Creation_Date << '\"');
			
		}
		catch(rsResourceSet::NotFoundException &e)
		{}

		try {
			getFilenameFieldInfo( _configRegistrationNode + ".ValidityStartField.Start", 
										_configRegistrationNode + ".ValidityStartField.Length",  fieldStart, fieldLen);
			fne.Validity_Start = filename.substr(fieldStart, fieldLen);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity Start Time=\"" << fne.Validity_Start << '\"');

			getFilenameFieldInfo( _configRegistrationNode + ".ValidityStopField.Start", 
										_configRegistrationNode + ".ValidityStopField.Length",  fieldStart, fieldLen);
			fne.Validity_Stop = filename.substr(fieldStart, fieldLen);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity Stop Time=\"" << fne.Validity_Stop << '\"');
			
		}
		catch(rsResourceSet::NotFoundException &e)
		{}

		try {
			getFilenameFieldInfo( _configRegistrationNode + ".VersionField.Start", 
										_configRegistrationNode + ".VersionField.Length",  fieldStart, fieldLen);
			fne.File_Version = filename.substr(fieldStart, fieldLen);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File Version=\"" << fne.File_Version << '\"');
		}
		catch(rsResourceSet::NotFoundException &e)
		{}
		
	}

	try {
		size_t entries = 0;
		rs.getValue( _configRegistrationNode + ".List_Of_AdditionalInfo.count.value", entries);
		for( size_t i=0; i<entries ; ++i) {
			ostringstream osTag;
			osTag << _configRegistrationNode + ".List_Of_AdditionalInfo.AdditionalInfo[" << i << "]";
			string tag = osTag.str();
			
			string field;
			rs.getValue( tag + ".Field", field);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Field=\"" << field << '\"');
			
			int fStart;
			int fLength;
			getFilenameFieldInfo( tag + ".Start", tag + ".Length",  fStart, fLength);
			string value = filename.substr(fStart, fLength);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value=" << value );
					
			fne.additionalInfo[field] = value;
		}

	}
	catch(rsResourceSet::NotFoundException &e)
	{}

	
}




string EOFFilenamePreInventory::getUniqueId()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "EOFFilenamePreInventory::getUniqueId");

	string uID ;

	if( getContext().which == Context::TL || getContext().which == Context::DS)
	{
		string pFileName = getContext().parameterFile;
        if ( ! pFileName.empty() )
        {

            rsResourceSet imXML;
                XMLIstream xml( pFileName);
                xml >> imXML;

            
            string tag = "Unique_Id";
            if( imXML.hasKey( tag))
            {
                imXML.getValue( tag, uID);
                if( uID.empty())
                {
                    ostringstream msg;
                        msg << "Empty Unique_Id tag in the ParametersFile \""  << pFileName << '\"';
                }

                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Unique_Id=" << uID);
            }
            else
            {
                ostringstream msg;
                msg << "NOT found Unique_Id tag in the ParametersFile \""  << pFileName << '\"';
                ACS_THROW(exEOFFilenamePreInventoryException( msg.str()) );
            }
        }
	}
	else if( getContext().which == Context::DIS)
	{
		ACS_LOG_WARNING("getUniqueId NOT IMPLEMENTED for DIS");

	}
	else if( getContext().which == Context::PROD)
	{
		ACS_LOG_WARNING("getUniqueId NOT IMPLEMENTED for PROD");
	}
	else
	{
		ostringstream msg;
		msg << "getUniqueId context.which=\"" << getContext().which << "\" NOT Recognaized!!";
		ACS_THROW(exEOFFilenamePreInventoryException( msg.str()));
	}

	 ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to return \"" << uID << '\"');

	return uID;
	 
}


// string EOFFilenamePreInventory::getParametersFile()
// {
// 	//apro il joborder e prelevo il parameterfile	
// 	DefaultJobOrder jo;
// 
// 	XMLIstream jofileXml( getContext().argvParameters["jobOrder"].as<string>());
//         jo << jofileXml;
// 	
// 	string parFileName = jo.getParametersFileName();
// 	if( parFileName.empty())
// 	{
// 		//just notify.			
//         ACS_LOG_WARNING("ParametersFile not specified!");
// 	}
// 	 ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "parameters filename =\"" << parFileName << '\"');
// 
// 	return parFileName;
// 	
// }


int EOFFilenamePreInventory::getSatelliteIdFromSatelliteCode( const string& satCode, dbConnection &conn)
{
	 ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "EOFFilenamePreInventory::getSatelliteIdFromSatelliteCode");	

    const string &cKey = conn.getKey();
    
	dbSatelliteQuery * satelliteQuery=dynamic_cast<dbSatelliteQuery*>(dbQueryFactoryChain::instance()->newObject("dbSatelliteQuery", cKey)); // PRQA S 3081, 4412
	
    if( !satelliteQuery)
	{
		ACS_THROW(exEOFFilenamePreInventoryException( "Error in executing dbSatelliteQuery"));
	}
	dbGenQueryScopeGuard satelliteQueryGuard( satelliteQuery);
    dbQueryParameters satParameters(cKey); 
	satParameters.addParameter( dbParam(cKey,"t_satellites", "satelliteacronym", dbParam::EQUAL_TO, dbConv(cKey, satCode))); // PRQA S 3050, 3081
	satelliteQuery->addParameter( satParameters);
	unsigned long num = satelliteQuery->doQuery(conn);

	int satId = 0;  //0<=>false
	if (num > 0) {
		satId = ((*satelliteQuery)[0])->getSatelliteId();
    }

	return satId;
}



//converte il missionId nel corrispondente mission per l'inventory
int EOFFilenamePreInventory::convertMissionId2INVMission( const string& missionId)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "EOFFilenamePreInventory::convertIIFMissionId2INVMission");	

	int invMission = 0;
	try{
        //read the configuration file from database
        rsResourceSet& rs = ConfigurationSingleton::instance()->get();
		//get the number of files that can be inventoried
        int nMissions = rs.getArraySize( "PreInventory.ConfigEOFFilenamePreInventory.Missions.Pair"); // PRQA S 3000, 3010, 4412

		bool found = false;
		for( int i=0; (i<nMissions) && !found; ++i)
		{
			ostringstream osTag;
			osTag << "PreInventory.ConfigEOFFilenamePreInventory.Missions.Pair[" << i << "]";
			string tag = osTag.str();
			string missionIdValue("");
			rs.getValue( tag + ".missionId.value", missionIdValue);
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TAG=" << tag );
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "missionId to find=" << missionId);


			if( missionIdValue == missionId)
			{
				rs.getValue( tag + ".missionInventory.value", invMission);

				 ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "missionId found=(" << missionIdValue << "-->" << invMission << ')');
				found = true;
			}
		}
		if( !found)
		{	
			ACS_LOG_WARNING("missionId =" << missionId << " Not found in config file!!! Using " << invMission);
		}
		
   }
   catch( exException& ex)
   {
                ACS_THROW(exEOFFilenamePreInventoryException( ex, "convertMissionId2INVMission Exception"));
   }
		
	return invMission;
}

#if(0)			

bool EOFFilenamePreInventory::overrideVersionHandling(bool& doVersioning)
{
	
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called");

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to check if versioning is allowed for file type \"" << _currentFileType << '\"');
	
    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
    size_t registeredEntries = 0;
	rs.getValue( "PreInventory.PluginsRegistrationSettings.EOFFilenamePreInventoryFactory.List_Of_Registrations.count.value", registeredEntries);

	bool found = false;
	doVersioning = false;
	for( size_t i=0; i<registeredEntries ; ++i) {
		ostringstream osTag;
		osTag << "PreInventory.PluginsRegistrationSettings.EOFFilenamePreInventoryFactory.List_Of_Registrations.Registration[" << i << "]";
		string tag = osTag.str();
		string fileType;
		rs.getValue( tag + ".Object", fileType);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TAG=" << tag << " fileType=\"" << fileType << '\"');

		if( fileType == _currentFileType) {
			rs.getValue( tag + ".DoVersioning", doVersioning);

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "fileType \"" << _currentFileType << "\" found in configuration; doVersioning " << boolalpha << doVersioning);
			found = true;
			break;
		}
	}
	
	ACS_COND_THROW(!found, exEOFFilenamePreInventoryException("Cannot find fileType=\""+_currentFileType+"\" in configuration"));	 // PRQA S 3081
	
	return true;
	
}


bool EOFFilenamePreInventory::doVersioning(const Metadata& metadata, std::string& newVersion, dbConnection &conn)
{

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called");

    bool isTheVersionChanged(false);

    //Execute Query
    dbInventoryObjectQuery* dbInvObjectQuery =dynamic_cast<dbInventoryObjectQuery*>( dbQueryFactoryChain::instance()->newObject("dbInventoryObjectQuery") ); // PRQA S 3081, 4412
	ACS_COND_THROW( !dbInvObjectQuery, exEOFFilenamePreInventoryException("Error in executing dbSensorQuery") );	 // PRQA S 3081
	dbGenQueryScopeGuard stationQueryGuard(dbInvObjectQuery);
	
    //Get specific dbQueryParametes
    dbQueryParameters parameters = getProductFilenameInInventoryQueryParameters(metadata);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "About to query t_inventory record with the following condition: " << parameters.getSqlString());

    dbInvObjectQuery->addParameter( parameters );

    unsigned long int countRow = dbInvObjectQuery->doQuery(&conn);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "num = dbInventoryObjectQuery->doQuery() = " << countRow );

    if( countRow > 0) {

        int maxVers = std::numeric_limits<int>::min();
		
		string currentVersion = metadata.getMetadataValue( "File_Version");
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Current file version from metadata: \"" << currentVersion << '\"' );
		
		bool found = false;
        for(unsigned long int i=0; i<countRow; ++i) {

            string version = ((*dbInvObjectQuery)[i])->getFileVersion();
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "dbInvObjectQuery->getFileVersion-> version = " << version );
			if( version == currentVersion ) {
				found = true;
            	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "t_inventory entry with filename \"" << metadata.getGlobalFileName() << 
                                                    "\" and version \"" << currentVersion<< "\" already occurring in db" );
			}
			
			{
				int vers;
				istringstream iss(version);
				iss >> vers;
            	maxVers = max(vers, maxVers) ; 
			}

        }	
		
		if( found ) {
        	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "maxVers = " << maxVers );

        	int newVers = maxVers+1;
			newVersion = getFormattedVersion( newVers);
			
			isTheVersionChanged=true;
		}
		else {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No t_inventory entry with filename \"" << metadata.getGlobalFileName() << 
                                                "\" has version \"" << currentVersion << "\": no new Version required." );
		}

    } else {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "No t_inventory entry with filename \"" << metadata.getGlobalFileName() << "\" occurs in db: no new Version required.");

    return isTheVersionChanged;
}

#endif

void EOFFilenamePreInventory::setConfigRegistrationNode(const string & ftype)
{

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called");

	size_t registeredEntries = 0;
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	rs.getValue( "PreInventory.PluginsRegistrationSettings.EOFFilenamePreInventoryFactory.List_Of_Registrations.count.value", registeredEntries);
	bool found = false;
	for( size_t i=0; i<registeredEntries ; ++i) {
		ostringstream osTag;
		osTag << "PreInventory.PluginsRegistrationSettings.EOFFilenamePreInventoryFactory.List_Of_Registrations.Registration[" << i << "]";
		string tag = osTag.str();
		string fileType;
		rs.getValue( tag + ".Object", fileType);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TAG=" << tag << " fileType=\"" << fileType << '\"');

		if( fileType == ftype) {
			found = true;
			_configRegistrationNode = tag;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Configuration Registration Node for fileType=\"" << ftype << "\" set to \"" << _configRegistrationNode << '\"');
			break;
		}
	}
	
	ACS_COND_THROW( !found, exEOFFilenamePreInventoryException("Cannot find fileType=\""+ftype+"\" in configuration") );	 // PRQA S 3081

}


void EOFFilenamePreInventory::getFilenameFieldInfo( const string & tagStart, const string & tagLen, int & fieldStart, int & fieldLen)
{
	
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method called");
	
	
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	
	rs.getValue( tagStart, fieldStart);
	if( fieldStart < 0 ) {
		ostringstream exMsg;
		exMsg << '\"' << tagStart << "\"=" << fieldStart << " ( >=0 expected)";
		ACS_THROW(exEOFFilenamePreInventoryException(exMsg.str()));
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << tagStart << "\"=" << fieldStart);

	rs.getValue( tagLen, fieldLen);
	if( fieldLen <= 0 ) {
		ostringstream exMsg;
		exMsg << '\"' << tagLen << "\"=" << fieldLen << " ( >0 expected)";
		ACS_THROW(exEOFFilenamePreInventoryException(exMsg.str()));
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << tagLen << "\"=" << fieldLen);
	
}
		
_ACS_END_NAMESPACE
