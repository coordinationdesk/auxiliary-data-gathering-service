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

	$Prod: libSafePreInventory $

	$Id$

	$Author$

	$Log$
	Revision 1.16  2016/04/18 13:23:57  marpas
	adopting new db I/F standards
	
	Revision 1.15  2016/03/11 09:08:04  tergem
	DefaultJobOrder dependency removed. Parameter File read from context
	
	Revision 1.14  2015/12/10 14:11:43  francesco.avanzi
	OrderId now taken into account in libPreInventoryBase
	
	Revision 1.13  2015/09/04 14:08:35  nicvac
	S3MPM-620: Metadata size is the real product size.
	
	Revision 1.12  2015/07/17 15:52:06  lucio.pulvirenti
	S3MPM-549: DS context managed. If unique_id tag not found, exception not thrown if properly configured.
	
	Revision 1.11  2015/07/15 15:57:23  lucio.pulvirenti
	S3MPM-549: versioning now enabled. Temporary work around not to throw exception if context is TL and uniqueID is non available. It will be fixed
	in the same issue.
	
	Revision 1.10  2015/06/19 14:20:22  nicvac
	S3MPM-533: Order_Id added
	
	Revision 1.9  2015/06/19 12:50:13  nicvac
	S3MPM-533: Support for S3 products. Added also Sensor id and Geolocation.
	
	Revision 1.8  2015/03/19 13:49:49  lucio.pulvirenti
	S3MPM-359: getSatMissionAssociatorFactoryKey method moved into base class.
	
	Revision 1.7  2015/01/27 16:18:29  lucio.pulvirenti
	removeSpuriousCharsFromStringDate base class method used before setting dates in Metadata struct.
	
	Revision 1.6  2013/11/13 17:24:30  lucio.pulvirenti
	Both StartValidity and StopValidity dates and creation date checked with base function convertUncorrectDate to let configure possibly a Time LUT.
	
	Revision 1.5  2013/10/04 10:17:56  matteo.airoldi
	Aux Specification V2 implemented
	
	Revision 1.4  2013/08/09 13:18:24  lucio.pulvirenti
	prepareMetadata: satelliteCode so far extracted from filename instead that from manifest.
	A factory is used to return the proper satellite acronym and mission depending on the context.
	getSatMissionAssociatorFactoryKey method added to get the factory key from configuration corresponding to
	a given satellite code.
	convertMissionId2INVMission method removed.
	
	Revision 1.3  2013/07/02 10:43:38  lucio.pulvirenti
	Fileclass now taken from Manifest and no more from configuration.
	Quality info flag written in metadata.
	getFiletypeKey developed to get key related to filetype from configuration
	getFactoryKey method developed to get key of factory on the base of satelliteid and filetype key.
	
	Revision 1.2  2013/07/01 16:42:30  lucio.pulvirenti
	Work in progress.
	
	Revision 1.1.1.1  2013/06/28 16:28:59  lucio.pulvirenti
	Import libSafePreInventory
	
        
*/ 

#include <SafePreInventory.h>
#include "SafePreInventoryV.h"

#include <ManifestDataExtractorFactory.h>
#include <ManifestDataExtractorSingleton.h>

#include <SatelliteMissionAssociatorFactory.h>
#include <SatelliteMissionAssociatorSingleton.h>

#include <Filterables.h>
#include <ConfigurationSingleton.h>

#include <XMLIstream.h>

#include <NameIdAssociator.h>

#include <dbQueryFactoryChain.h>
#include <dbGenQueryScopeGuard.h>
#include <dbInventoryObjectQuery.h>
#include <dbInventoryObject.h>

#include <dbSatelliteQuery.h>
#include <dbSatellite.h>
#include <StringUtils.h>

#include <boost/tokenizer.hpp>
#include <boost/format.hpp>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SafePreInventory)

namespace {
    SafePreInventoryV version;
}

const string SafePreInventory::_defaultSatMissionAssociatorFactoryKey = "STANDARD";

SafePreInventory::SafePreInventory() : 
    PreInventoryBase(),
	_currentFileName(),
	_currentFileType(),
	_ftypeParentTag()
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Id: " << Id);
	setPluginConfSpaceId("ConfigSafePreInventory");
	setFactoryConfSpaceId("SafePreInventoryFactory");
}

SafePreInventory::~SafePreInventory() throw() 
{
// PRQA S 4631 L1
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");
// PRQA L:L1
}

string SafePreInventory::getClassId()
{
        //return the class name
	return "SafePreInventory";
}
        

void SafePreInventory::computeMetadata( const string& pathFileName, const string& fileType, Metadata& metadata, dbConnection &conn)
{
    //NB: the pathFileName passed to this method is an absolute path
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");
	
	try {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MetadataInfo:\n"
			<< " OrderId=" << getContext().metadataInfo.orderId << '\n'
			<< "  pathFileName= " << pathFileName << '\n'
			<< "  filetype from framework= " << fileType );

		_currentFileName = pathFileName;
		_currentFileType = fileType;
		
		ManifestData manifestData;
		
		readManifest(pathFileName, fileType, manifestData);
		
		metadata = prepareMetadata( fileType, manifestData, conn);
	}
    catch( exception& ex)
    {
        ACS_THROW( exSafePreInventoryException( ex, "computeMetadata Exception"));
    }
}      




Metadata SafePreInventory::prepareMetadata( const string& fileType, const ManifestData& manifestData, dbConnection &conn)
{

	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");

	//filling Metadata
	Metadata metadata;

	// *UNIQUE_ID
	string uniqueId = getUniqueId();
    if ( !uniqueId.empty()) {
        metadata.setMetadataValue( "Unique_Id", uniqueId); 
    }
	
	string fileName = fillMetadataFilename( _currentFileName, fileType, metadata, conn);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File will be inventoried with the name \"" << fileName << "\"");

	// *FILE_TYPE
	bool getFtypeFromFramework = true;
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	string pluginConfSpaceId = getPluginConfSpaceId();
	try {
		rs.getValue( "PreInventory."+pluginConfSpaceId+".FiletypeFromFramework", getFtypeFromFramework);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FiletypeFromFramework from configuration is " << ( getFtypeFromFramework ? "true" : "false" ) );
	}
	catch(rsResourceSet::NotFoundException &e)
	{}

	string fType = fileType;
	if( !getFtypeFromFramework && !manifestData.filetype.empty() ) {
		fType = manifestData.filetype;
    }
	metadata.setMetadataValue( "File_Type", fType);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File_Type set to \"" << fType << "\"");

	// *VALIDITY_START
	{
		string tmp = "UTC=" + removeSpuriousCharsFromStringDate(manifestData.startTime);
		string validityStart = convertUncorrectDate( pluginConfSpaceId, tmp );
		metadata.setMetadataValue( "Validity_Start", validityStart);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity_Start=" << validityStart);
	}

	// *VALIDITY_STOP
	{
		string tmp = "UTC=" + removeSpuriousCharsFromStringDate(manifestData.stopTime);
		string validityStop = convertUncorrectDate( pluginConfSpaceId, tmp );
		metadata.setMetadataValue( "Validity_Stop", validityStop);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity_Stop=" << validityStop);
	}
	
	// *FILE_VERSION
	int fileVersion = 1;
	if( !manifestData.productVersion.empty() ) {
		istringstream is ( manifestData.productVersion, istringstream::in);
		is >> fileVersion;
	}
	metadata.setMetadataValue( "File_Version", getFormattedVersion(fileVersion));
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File_Version=\"" << getFormattedVersion(fileVersion) << "\"");

	// *SOURCE	
	string source = manifestData.softwareName;
	metadata.setMetadataValue( "Source", source);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Source=" << source);
	
	// *SOURCE_SW_VERSION
	string sourceSwVersion = manifestData.softwareVersion;
	metadata.setMetadataValue( "Source_Sw_Version", sourceSwVersion);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Source_Sw_Version=" << sourceSwVersion);

	// SYSTEM
	string system = manifestData.facilityName;
	if( !system.empty() ) {
		metadata.setMetadataValue( "System", system);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "System=" << system);
	}
	
	// ORDER_ID (if any according to context) Now Set in PreInventoryBase

	// *GENERATION_TIME
	{
		string generationTime = "UTC=";
		if( manifestData.creationTime.empty()) {
			DateTime dt;            //today datetime
			ostringstream strTime;
			dt.asISO8601( strTime, 6); // PRQA S 4400
			generationTime += strTime.str();
    	}
		else {
			string tmp = generationTime + removeSpuriousCharsFromStringDate(manifestData.creationTime);
			generationTime = convertUncorrectDate( pluginConfSpaceId, tmp );
		}
		metadata.setMetadataValue( "Generation_Time", generationTime);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generation_Time=" << generationTime);
	}

	
	// *VALIDITY_FLAG
	metadata.setMetadataValue( "Validity_Flag", "true");
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity_Flag= true");
	
	
	// *VALIDATION_DATE
	{
		DateTime dt;            //today datetime
		ostringstream strTime;
		strTime << "UTC=";
		dt.asISO8601( strTime, 6); // PRQA S 4400
		string creationTime = strTime.str();
		metadata.setMetadataValue( "Validation_Date", creationTime); 
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validation_Date=" << creationTime );
	}

	// *HEADER_SIZE 
	off_t headerSize = 0;
	metadata.setMetadataValue( "Header_Size", headerSize); 
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Header_Size=" << headerSize );
	

	// *DATA_SIZE 
	//Ignore size coming from Manifest. Inventory Metadata needs real product size.
	//metadata.setMetadataValue( "Data_Size", manifestData.productSize);
	off_t fileSize = File::size( _currentFileName);
	metadata.setMetadataValue( "Data_Size", fileSize);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Data_Size=" << manifestData.productSize);

	// *FILE_CLASS
	metadata.setMetadataValue( "File_Class", manifestData.fileClass);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File_Class=" << manifestData.fileClass);
			
	if( !manifestData.startOrbitNumber.empty() ) {
		metadata.setMetadataValue( "Start_Orbit_Number", manifestData.startOrbitNumber);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start orbit number=" << manifestData.startOrbitNumber);
	}

	if( !manifestData.stopOrbitNumber.empty() ) {
		metadata.setMetadataValue( "Stop_Orbit_Number", manifestData.stopOrbitNumber);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start orbit number=" << manifestData.stopOrbitNumber);
	}

	// *SATELLITE_ID 	
#ifndef OLD_VERSION
	string satelliteCode = manifestData.mission;
#else
#warning "For next version, mission to be extracted from both familyName and number tags of manifest"
	string satelliteCode;
	{
		string filename = File::getFileName(_currentFileName);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to extract satellite name from \"" << filename << "\" filename");
		size_t satelliteCodeLen = 3;
		if( filename.length() < satelliteCodeLen ) {
			ostringstream exMsg;
			exMsg << "\"" << filename << "\" length is " << filename.length() << " ( >=" << satelliteCodeLen << " expected)";
			throwException( exSafePreInventoryException(exMsg.str()) );
		}
		satelliteCode = filename.substr(0, satelliteCodeLen);
	}
#endif
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SatelliteCode=" << satelliteCode);
	
	const string parentTag("PreInventory."+pluginConfSpaceId);
	string satFactoryKey = getSatMissionAssociatorFactoryKey( satelliteCode, parentTag );
	if( satFactoryKey.empty() ) {
		satFactoryKey = _defaultSatMissionAssociatorFactoryKey;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "SatelliteMissionAssociator factory key \"" << satFactoryKey << "\"");
	
	SatelliteMissionAssociatorFactory factoryObject ;
	
	SatelliteMissionAssociator* satelliteMissionAssociator = SatelliteMissionAssociatorSingleton::instance()->newSatelliteMissionAssociator(StringUtils::uppercase(satFactoryKey));
	throwConditionedException(!satelliteMissionAssociator, exSafePreInventoryException("Can't Instantiate \"" + satFactoryKey + "\": got NULL pointer from SatelliteMissionAssociatorFactory") );	 // PRQA S 3081

	string satelliteAcronym = satelliteMissionAssociator->getSatelliteAcronym(satelliteCode);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite acronym=\"" << satelliteAcronym << "\"");
	int satelliteId = getSatelliteIdFromSatelliteCode(satelliteAcronym, conn);
	if( satelliteId != 0 ) {
		metadata.setMetadataValue( "Satellite_Id", satelliteId);
    }

	// MISSION 
	int missionNumber = satelliteMissionAssociator->getMissionId(satelliteCode);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Mission number=" << missionNumber);
	if( missionNumber != 0 ) {
		metadata.setMetadataValue( "Mission", missionNumber);
    }

	// SENSOR ID
	if ( ! manifestData.instrumentName.empty() ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Getting id for sensor name ["<<manifestData.instrumentName<<"]" );
		NameIdAssociator<int> nameIdAssociator( "t_sensors", "sensorid", "sensorname", conn.getKey() );
		nameIdAssociator.load();
		metadata.setMetadataValue( "Sensor_Id", nameIdAssociator.id( manifestData.instrumentName ) );
	}

	// QUALIFIED
	{
		NameIdAssociator<int> nameIdAssociator( "t_qualifiedflag", "id", "name" , conn.getKey()) ;
		nameIdAssociator.load() ;
		metadata.setMetadataValue( "Qualified", nameIdAssociator.id(manifestData.qualityFlag));
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Qualified=" << nameIdAssociator.id(manifestData.qualityFlag));
	}

	
	// GEOGRAPHIC_LOCALIZATION
	if ( ! manifestData.footPrintPosList.empty() ) {
		
		manageFootPrintPosList(manifestData, metadata) ;

	}
	
	// additional data
	const size_t addDataSize = manifestData.additionalData.size();
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Additional data number: " << addDataSize); 
	for( size_t i = 0; i < addDataSize; ++i ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << manifestData.additionalData[i].first << "\" -> \"" << manifestData.additionalData[i].second << '\"');
		metadata.setFieldValue( manifestData.additionalData[i].first, manifestData.additionalData[i].second );
	}
	
	setFurther(metadata, conn);
	return metadata;
}



void SafePreInventory::updateProductAndMetadata( const std::string& newVersion, Metadata& metadata)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

	metadata.setMetadataValue( "File_Version", newVersion);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File_Version field updated with the value \"" << newVersion << '\"');
			
}

string SafePreInventory::doPackaging( Metadata& metadata, const string & firstPath, const string & filetype, const string & secondPath, dbConnection & )
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");
			
	throwConditionedException( !secondPath.empty(), exSafePreInventoryException("No package foreseen: data filename should no be valorized \"" + secondPath + "\".") );

	return firstPath;
}


void SafePreInventory::readManifest( const string& productDir, const string& fileType, ManifestData& manifestData) // PRQA S 4211
{

	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");
	
	// find the manifest in the product directory
	string manifestFilename;
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	string pluginConfSpaceId = getPluginConfSpaceId();
	rs.getValue( "PreInventory."+pluginConfSpaceId+".ManifestFilename", manifestFilename);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Manifest filename from configuration is \"" << manifestFilename << "\"" );
	string filetypeKey = getFiletypeKey(fileType);
	// CCDS-41: manifestFilename can be overwritten by filetype specific configuration
	if (getManifestFilename(manifestFilename))
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Manifest filename overwritten for filetype " << fileType<< ". New filename: " << manifestFilename);
	}
	string manifestFullpath = productDir + "/" + manifestFilename;
	throwConditionedException( !File::exists(manifestFullpath), exSafePreInventoryException("\"" + manifestFullpath + "\" manifest file not occurring" ) );
	
	vector<string> vect;
	vect.push_back(productDir);
	vect.push_back(filetypeKey);
	string factoryKey = getFactoryKey(vect);
	
	ManifestDataExtractorFactory factoryObject ;
	
	ManifestDataExtractor* manifestDataExtractor = ManifestDataExtractorSingleton::instance()->newManifestDataExtractor(factoryKey);
	throwConditionedException(!manifestDataExtractor, exSafePreInventoryException("Can't Instantiate \"" + factoryKey + "\": got NULL pointer from Factory") );	 // PRQA S 3081
	
	manifestDataExtractor->setFiletype(fileType);
	
	vector< pair<string, string> > xPathsV;
	extractAdditionalManifestFieldsXPaths(xPathsV);
	manifestDataExtractor->setAdditionalDataFields(xPathsV);
	
	manifestDataExtractor->setConfSpaceId(pluginConfSpaceId);

	manifestDataExtractor->doWork(manifestFullpath, manifestData);
	
}




string SafePreInventory::getUniqueId()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

	string uID;

	if( getContext().which == Context::TL || getContext().which == Context::DS )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Context is " << getContext().which );
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
                    ACS_LOG_WARNING( "Empty Unique_Id tag in the ParametersFile \""  << pFileName << '\"');
                }
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Unique_Id=" << uID ); 
            }
            else
            {
			// exception not thrown only if explicitely configured.
				bool uniqueIdMustOccurInParFile = true;
				try {
					ConfigurationSingleton::instance()->get().getValue( "PreInventory."+getPluginConfSpaceId()+".UniqueIdMustOccurInParameterFile", uniqueIdMustOccurInParFile);
					ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "UniqueIdMustOccurInParameterFile from configuration is " << boolalpha << uniqueIdMustOccurInParFile );
				}
				catch(rsResourceSet::NotFoundException &) { /* do nothing */ }
				
                ostringstream msg;
                msg << "NOT found Unique_Id tag in the ParametersFile \""  << pFileName << "\" for " << getContext().which;
				ACS_COND_THROW( uniqueIdMustOccurInParFile, exSafePreInventoryException(msg.str()) );
				if( getContext().which == getContext().DS ) {
					ACS_LOG_WARNING(msg.str());
				}
				else { // TL
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, msg.str() ); 
				}
            }
        }
	}
	else if( getContext().which == Context::DIS)
	{
		ACS_LOG_WARNING( "getUniqueId NOT IMPLEMENTED for DIS" );

	}
	else if( getContext().which == Context::PROD)
	{
		ACS_LOG_WARNING( "getUniqueId NOT IMPLEMENTED for PROD" );
	}
	else
	{
		ostringstream msg;
		msg << "getUniqueId context.which=\"" << getContext().which << "\" NOT Recognaized!!";
		ACS_THROW( exSafePreInventoryException( msg.str() ) );
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to return \"" << uID << '\"'); 

	return uID;
	 
}


// string SafePreInventory::getParametersFile()
// {
// 	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");
// 	
// 	//apro il joborder e prelevo il parameterfile	
// 	DefaultJobOrder jo;
// 
// 	XMLIstream jofileXml( getContext().argvParameters["jobOrder"].as<string>());
//         jo << jofileXml;
// 	
// 	string parFileName = jo.getParametersFileName();
// 	if( parFileName.empty())
// 	{
// 		ACS_LOG_WARNING( "ParametersFile not specified!" );
// 	}
// 	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Parameters filename =\"" << parFileName << "\""); 
// 
// 	return parFileName;
// 	
// }


int SafePreInventory::getSatelliteIdFromSatelliteCode( const string& satCode, dbConnection &conn)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");

	dbSatelliteQuery * satelliteQuery=dynamic_cast<dbSatelliteQuery*>(dbQueryFactoryChain::instance()->newObject("dbSatelliteQuery", conn.getKey())); // PRQA S 3081, 4412
	if( !satelliteQuery)
	{
		exSafePreInventoryException e( "Error in executing dbSatelliteQuery");
		throwException(e);
	}
	dbGenQueryScopeGuard satelliteQueryGuard( satelliteQuery);
	const string &dbKey = conn.getKey() ;
	dbQueryParameters satParameters(dbKey) ; 
	satParameters.addParameter( dbParam(dbKey, "t_satellites", "satelliteacronym", dbParam::EQUAL_TO, dbConv(dbKey, satCode))); // PRQA S 3050, 3081
	satelliteQuery->addParameter( satParameters);
	unsigned long num = satelliteQuery->doQuery(conn);

	int satId = 0;  //0<=>false
	if (num > 0) {
		satId = ((*satelliteQuery)[0])->getSatelliteId();
    }

	return satId;
}


string SafePreInventory::getFactoryKey(const vector<string> & sv)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");
	
	string filename = File::getFileName(sv[0]);
	size_t satelliteCodeLen = 3;
	if( filename.length() < satelliteCodeLen ) {
		ostringstream exMsg;
		exMsg << "\"" << filename << "\" length is " << filename.length() << " ( >=" << satelliteCodeLen << " expected)";
		throwException( exSafePreInventoryException(exMsg.str()) );
	}
	string satelliteCode = File::getFileName(sv[0]).substr(0, satelliteCodeLen);
	
	string factoryKey = satelliteCode + "_" + sv[1];
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning factory key: \"" << factoryKey << "\"");
	
	return factoryKey;
	
}

bool SafePreInventory::getManifestFilename(std::string & manifestname)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	ACS_COND_THROW( _ftypeParentTag.empty(), exSafePreInventoryException("Wrong methods call sequence: file type tag not valorized!"));	 // PRQA S 3081
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Filetype parent tag \"" << _ftypeParentTag << '\"');

    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	const string manifestTag = _ftypeParentTag + ".ManifestFilename";
	bool overrideManifestName = false;
	if( rs.getValue( manifestTag, manifestname, false))
	{
		overrideManifestName = true;
	}
	return overrideManifestName;
}
string SafePreInventory::getFiletypeKey(const string & refFiletype)
{
	
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");
	
    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
    size_t registeredEntries = 0;
	
	string factoryConfigurationRootTag = "PreInventory.PluginsRegistrationSettings." + getFactoryConfSpaceId();
	rs.getValue( factoryConfigurationRootTag+".List_Of_Registrations.count.value", registeredEntries);

	bool found = false;
	string filetypeKey;
	for( size_t i=0; i<registeredEntries ; i++) {
		ostringstream osTag;
		osTag << factoryConfigurationRootTag << ".List_Of_Registrations.Registration[" << i << "]";
		string tag = osTag.str();
		string fileType;
		rs.getValue( tag + ".Object", fileType);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << " fileType=\"" << fileType << "\"");

		if( fileType == refFiletype) {
			_ftypeParentTag = tag;
			rs.getValue( tag + ".FiletypeKey", filetypeKey);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "fileType \"" << refFiletype << "\" found in configuration; Key= \"" << filetypeKey << "\"") ;
			found = true;
			break;
		}
	}
	
	throwConditionedException(!found, exSafePreInventoryException("Cannot find fileType=\""+refFiletype+"\" in configuration"));	 // PRQA S 3081
	
	return filetypeKey;
	
}

void SafePreInventory::extractAdditionalManifestFieldsXPaths(vector< pair<string, string> > &xPaths)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	ACS_COND_THROW( _ftypeParentTag.empty(), exSafePreInventoryException("Wrong methods call sequence: file type tag not valorized!"));	 // PRQA S 3081
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Filetype parent tag \"" << _ftypeParentTag << '\"');
	
	const string localRootTag = _ftypeParentTag + ".ManifestFieldsXPaths.xPath";
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
    size_t elements = rs.getArraySize( localRootTag ); // PRQA S 3000, 3010, 4412
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << localRootTag << ", elements " << elements);

	for( size_t i=0; i < elements; i++)
	{
		string tag = localRootTag;
		if( elements > 1 ) {
			ostringstream os ;
			os << "[" << i << "]" ;
			tag += os.str();
		}
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=\"" << tag << '\"');

		string name;
		rs.getValue( tag + ".name.value", name);
		string xPath;
		rs.getValue( tag, xPath);
		
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Data path: \"" << xPath << "\", name: \"" << name << '\"');
		xPaths.push_back( make_pair(xPath, name) );
		
	}
	
}


bool SafePreInventory::overrideVersionHandling(bool& doVersioning)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
			
	ACS_COND_THROW( _ftypeParentTag.empty(), exSafePreInventoryException("Wrong methods call sequence: file type tag not valorized!"));	 // PRQA S 3081
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Filetype parent tag \"" << _ftypeParentTag << '\"');

    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	const string versioningTag = _ftypeParentTag + ".DoVersioning";
	bool overrideVersionHandl = false;
	if( rs.hasKey(versioningTag) ) {
		rs.getValue( versioningTag, doVersioning);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "doVersioning=" << boolalpha << doVersioning );
		overrideVersionHandl = true;
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "doVersioning tag not found in configuration: version handled from framework" );
	}
	
	return overrideVersionHandl;
	
}



bool SafePreInventory::doVersioning(const Metadata& metadata, std::string& newVersion, dbConnection &conn)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

    bool isTheVersionChanged(false);

    //Execute Query
    dbInventoryObjectQuery* dbInvObjectQuery =dynamic_cast<dbInventoryObjectQuery*>( dbQueryFactoryChain::instance()->newObject("dbInventoryObjectQuery", conn.getKey()) ); // PRQA S 3081, 4412
	ACS_COND_THROW( !dbInvObjectQuery, exSafePreInventoryException("Null pointer returned by dbQueryFactoryChain with dbInventoryObjectQuery key") );	 // PRQA S 3081
	dbGenQueryScopeGuard squeryGuard(dbInvObjectQuery);
	
    //Get specific dbQueryParametes
    dbQueryParameters parameters = getProductFilenameInInventoryQueryParameters(conn, metadata);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "About to query t_inventory record with the following condition: " << parameters.getSqlString());

    dbInvObjectQuery->addParameter( parameters );
    unsigned long int countRow = dbInvObjectQuery->doQuery(conn);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "num = dbInventoryObjectQuery->doQuery() = " << countRow ); 
    if( countRow > 0) {

        int maxVers = std::numeric_limits<int>::min();
		
		string currentVersion = metadata.getMetadataValue( "File_Version");
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Current file version from metadata: \"" << currentVersion << "\"" ); 

#warning "The only case of decimal version managed so far is xx.x"		
		bool decimalVersion = false;
		if( StringUtils::findNoCase(currentVersion, ".") != string::npos ) {
			decimalVersion = true;
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Decimal version: " << boolalpha << decimalVersion );

		
		bool found = false;
        for(unsigned long int i=0; i<countRow; i++) {

            string version = ((*dbInvObjectQuery)[i])->getFileVersion();
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "dbInvObjectQuery->getFileVersion-> version = " << version) ; 
			if( version == currentVersion ) {
				found = true;
            	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "t_inventory entry with filename \"" << metadata.getGlobalFileName() << "\" and version \"" << currentVersion
						<< "\" already occurring in db") ; 
			}
			
			{
				string transformedVersion = StringUtils::transform(version, ".", ""); 
				if( decimalVersion && transformedVersion.size() != 3 ) {
					ACS_THROW( exSafePreInventoryException("Decimal version trasformation with size != 3 still not implemented") );
				}
				int vers;
				istringstream iss(transformedVersion);
				iss >> vers;
            	maxVers = max(vers, maxVers) ; 
			}

        }	
		
		if( found ) {
        	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "maxVers = " << maxVers) ; 

        	int newVers = maxVers+1;
			if( decimalVersion ) {
				boost::format fmt("%03d");
				fmt % newVers;
				string newVersString = fmt.str();
				newVersion = newVersString.substr(0, 2) + "." + newVersString[newVersString.size()-1];
			}
			else {
				newVersion = getFormattedVersion( newVers);
			}
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "New version string \"" << newVersion << '\"');
			
			isTheVersionChanged=true;
		}
		else {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No t_inventory entry with filename \"" << metadata.getGlobalFileName() << "\" has version \"" << currentVersion
					<< "\": no new Version required.");
		}

    } // if( countRow > 0)
	else {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No t_inventory entry with filename \"" << metadata.getGlobalFileName() << "\" occurs in db: no new Version required." ); 
    }

    return isTheVersionChanged;
}

void SafePreInventory::manageFootPrintPosList(const ManifestData& manifestData, Metadata & metadata) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Parsing footprint [" << manifestData.footPrintPosList << "]");

	vector<Metadata::GeoPointInfo>  vGeoPntInfo;

	boost::char_separator<char> sep(" ");
	boost::tokenizer< boost::char_separator<char> > tok( manifestData.footPrintPosList, sep );
	for( boost::tokenizer< boost::char_separator<char> >::iterator it=tok.begin(); it!=tok.end(); ++it) {

		Metadata::GeoPointInfo gPntInfo;
		gPntInfo.latitude = *it++;

		ACS_COND_THROW( it==tok.end(),
				exSafePreInventoryException("Geo Point List from Manifest are not well balanced. Cannot found longitude on the last point lat-lon"));

		gPntInfo.longitude = *it;

		vGeoPntInfo.push_back( gPntInfo );
	}

	metadata.setGeoPoints( vGeoPntInfo );
	metadata.setGeoType( dbGeoObject::Polygon );

	// time range of Geographic_Localization
	metadata.setFieldValue( "Start_Time", metadata.getMetadataValue("Validity_Start") );
	metadata.setFieldValue( "Stop_Time", metadata.getMetadataValue("Validity_Stop"));
	
}

SafePreInventory::ManifestData::ManifestData():
							   filename(),
							   filetype(),
							   mission(),
							   instrumentName(),
							   startTime(),
							   stopTime(),
							   productSize(),
							   productVersion(),
							   fileClass(),
							   softwareName(),
							   softwareVersion(),
							   creationTime(),
							   facilityName(),
							   qualityFlag(),
							   startOrbitNumber(),
							   stopOrbitNumber(),
							   footPrintPosList(),
							   additionalData()
{
}

SafePreInventory::ManifestData::ManifestData(SafePreInventory::ManifestData const &o):
												   filename(o.filename),
												   filetype(o.filetype),
												   mission(o.mission),
												   instrumentName(o.instrumentName),
												   startTime(o.startTime),
												   stopTime(o.stopTime),
												   productSize(o.productSize),
												   productVersion(o.productVersion),
												   fileClass(o.fileClass),
												   softwareName(o.softwareName),
												   softwareVersion(o.softwareVersion),
												   creationTime(o.creationTime),
												   facilityName(o.facilityName),
												   qualityFlag(o.qualityFlag),
												   startOrbitNumber(o.startOrbitNumber),
												   stopOrbitNumber(o.stopOrbitNumber),
												   footPrintPosList(o.footPrintPosList),
												   additionalData(o.additionalData)
{
}
												
SafePreInventory::ManifestData & SafePreInventory::ManifestData::operator = (SafePreInventory::ManifestData const &o)
{
	if( this != &o ) {
		filename=o.filename;
		filetype=o.filetype;
		mission=o.mission;
		instrumentName=o.instrumentName;
		startTime=o.startTime;
		stopTime=o.stopTime;
		productSize=o.productSize;
		productVersion=o.productVersion;
		fileClass=o.fileClass;
		softwareName=o.softwareName;
		softwareVersion=o.softwareVersion;
		creationTime=o.creationTime;
		facilityName=o.facilityName;
		qualityFlag=o.qualityFlag;
		startOrbitNumber=o.startOrbitNumber;
		stopOrbitNumber=o.stopOrbitNumber;
		footPrintPosList=o.footPrintPosList;
		additionalData=o.additionalData;
	}
	
	return (*this);
}
												
	
_ACS_END_NAMESPACE
