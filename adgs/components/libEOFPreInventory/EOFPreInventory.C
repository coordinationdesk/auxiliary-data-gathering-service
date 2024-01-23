// PRQA S 1050 EOF
/*
    Copyright 1995-2021, Exprivia SpA.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.com

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Exprivia SpA.

	$Prod: A.C.S. libEOFPreInventory$

	$Id$

	$Author$

	$Log$
	Revision 1.41  2017/07/11 13:41:58  lucio.pulvirenti
	ECICMF-108: preProcess bug fixed when passing DBL file. readEOFFile: optional configuration flag introduced to read HDR files with the same structure
	of top level EOF files.
	getQualifiedValue: optional configuration flag introduced to set qualified flag different from NOT_AVAILABLE (default) if product cannot be validated.
	
	Revision 1.40  2016/04/18 13:10:15  marpas
	adopting new standard
	
	Revision 1.39  2015/11/30 11:00:42  francesco.avanzi
	qac++analisys run
	
	Revision 1.38  2015/11/27 16:49:37  francesco.avanzi
	MPM EOF files now added
	
	Revision 1.37  2015/07/17 15:28:25  lucio.pulvirenti
	S3MPM-549: DS context managed. If unique_id tag not found, exception not thrown if properly configured.
	
	Revision 1.36  2015/07/09 16:25:43  matteo.airoldi
	Versioning in the ADM context has been fixed
	XSLT is now used to change HDR and EEF contents
	
	Revision 1.35  2015/06/12 10:43:05  francesco.avanzi
	 conf/Test/libEOFPreInventory.so.xml
	
	Revision 1.34  2015/06/03 12:27:46  francesco.avanzi
	conf/Test/libEOFPreInventory.so.xml
	
	Revision 1.33  2015/05/28 14:31:42  francesco.avanzi
	best code practices applied in overrideVersionHandling
	
	Revision 1.32  2015/05/27 15:08:41  francesco.avanzi
	modified to best derivate EOFADMPreInventory
	
	Revision 1.31  2014/07/23 10:10:45  lucio.pulvirenti
	S2PDGS-738: If no schema occurs in file and NoSchemaAvailableFlag no correctly valorized, just a warning is emitted advising to check configuration and product is marked as VALID.
	
	Revision 1.30  2014/05/15 10:39:01  lucio.pulvirenti
	S2PDGS-738: getNoSchemaAvailableFlag method added to get flag corresponding to products with no schema tag
	getQualifiedValue: if validation is successfull but no schema occurs, the proper flag is searched in configuration by getNoSchemaAvailableFlag method. If no flag is found, VALID_PRODUCT is set.
	
	Revision 1.29  2014/02/11 11:29:51  marpas
	base class method signature updated
	
	Revision 1.28  2014/02/11 11:08:20  francesco.avanzi
	better code practices applied && dbConv moved to db::toSQL
	
	Revision 1.27  2013/11/29 14:06:42  lucio.pulvirenti
	readEOFFile: flag passed to XMLProgDataConsumer CTOR to neglect namespaces in nodes names, e.g. /n1:Earth_Explorer_File/n1:Earth_Explorer_Header
	becomes /Earth_Explorer_File/Earth_Explorer_Header.
	
	Revision 1.26  2013/11/13 12:53:01  lucio.pulvirenti
	Deprecated debug macros substituted to avoid compilation warnings.
	Connection key now passed to both dbQueryParamters and dbParam CTORs.
	
	Revision 1.25  2013/11/13 11:00:11  lucio.pulvirenti
	_pluginConfSpaceId static const member added to interface.
	Both validity start and stop dates and creation date checked with base class convertUncorrectDate to convert possible wrong
	dates (e.g. 9999-99-99T99:99:99) according to a configured LUT.
	
	Revision 1.24  2013/11/12 16:45:57  lucio.pulvirenti
	Bug fixed: metadata Originator no more set: System set instead.
	Bug fixed: if file class is empty, got by filename with no path (it was full path).
	convertOriginalFileClass base class method used to possibly convert file class read from HDR.
	
	Revision 1.23  2013/10/22 10:49:15  matteo.airoldi
	Patch to manage file classes "Testing" and "Routine Operations"
	
	Revision 1.22  2013/09/25 15:42:04  matteo.airoldi
	Validity_stop set to 9999-99-99T99:99:99 managed.
	In the Metadata the data is set to 9999-12-31T23:59:59
	
	Revision 1.21  2013/09/02 13:02:32  lucio.pulvirenti
	After jira issue ADC-185, a factory is used to return the proper satellite acronym and mission depending on the context.
	getSatMissionAssociatorFactoryKey method added to get the factory key from configuration corresponding to a given satellite code.
	convertMissionId2INVMission method removed.
	
	Revision 1.20  2013/04/29 16:28:33  marpas
	base class interface changed
	file type recognition interface changed
	
	Revision 1.19  2013/04/05 09:38:11  marpas
	NameIdAssociator partial refactoring
	
	Revision 1.18  2013/04/04 10:53:18  lucio.pulvirenti
	XML Validation performed if configured. Qualified flag valorized properly.
	
	Revision 1.17  2013/04/03 15:39:30  lucio.pulvirenti
	Work in progress.
	
	Revision 1.16  2013/03/13 14:16:51  lucio.pulvirenti
	File type never taken from product so far, even if stated from configuration.
	To implement in next version.
	
	Revision 1.15  2013/03/01 15:49:54  lucio.pulvirenti
	prepareMetadata: possibility to take file type either from framework or from configuration now configurable.
	File type is take from framework per default.
	
	Revision 1.14  2013/02/25 09:38:45  lucio.pulvirenti
	Base class doVersioning method re-implemented: versioning performed only if no inventory record
	with both the same filename and the same version occurs.
	
	Revision 1.13  2013/02/21 14:14:33  lucio.pulvirenti
	dbConnection added to base class fillMetadataFilename method.
	
	Revision 1.12  2013/02/20 16:54:32  lucio.pulvirenti
	Added dbConnection to preProcess, computeMetadata, prepareMetadata, getSensorIdFromSensorCode, getSatelliteIdFromSatelliteCode.
	prepareMetadata: File_Version set to 1 only if not occurring in file header.
	updateProductAndMetadata implemented tp update metadata File_Version and set metadata Quality_Info.
	getFormattedVersion method removed: using the one implemented in base class.
	overrideVersionHandling method implemented.
	qa rules.
	
	Revision 1.11  2013/01/28 15:41:37  lucio.pulvirenti
	readEOFFile: if EOF file, it can be quite large so that reading nodes of interest can be time expensive.
	For that reason nodes and values of interest are filtered and written in a temporary
	xml file that then is read by resourceSet.
	
	Revision 1.10  2012/12/12 11:49:58  lucio.pulvirenti
	preProcess: now managed the case where DATA file is the input; package extension mo more got here.
	prepareMetadata: base class fillMetadataFilename method used to write Filename field of metadata class; the new method takes into
	account if extesion must be either stripped off or not.
	
	Revision 1.9  2012/12/06 11:26:54  lucio.pulvirenti
	preProcess now query to db the expected extension of input file on t_filetypes table. EOF, HDR, DBL
	extensions now got from db. The name of the current file (no ext is removed) is returned by the method.
	doPackage modified according of the new IF of the base class.
	getUniqueId: exceptions turned into warning messages.
	convertMissionId2INVMission: now returns int.
	
	Revision 1.8  2012/11/30 16:30:01  lucio.pulvirenti
	work in progress.
	
	Revision 1.7  2012/11/29 16:59:32  lucio.pulvirenti
	work in progress.
	
	Revision 1.6  2012/11/19 14:00:38  giufor
	removed some obsolete comments
	
	Revision 1.5  2012/10/18 09:18:47  giufor
	updated from TlDefaultJobOrder to defaultJoborder
	
	Revision 1.4  2012/10/17 10:26:04  giufor
	reimplemented preProcess and doPackaging methods to consider case of HDR/DBL files
	
	Revision 1.3  2012/10/12 14:04:36  giufor
	implemented readEOFFile to read FixedHeader section of generic EahrtObservationFile) .EOF or .HDR in a structure
	
	Revision 1.2  2012/10/11 10:39:59  giufor
	created empty class
	
	Revision 1.1.1.1  2012/10/11 09:56:12  giufor
	Import libEOFPreInventory
	
        
*/ 

#include <EOFPreInventory.h>
#include <EOFPreInventoryV.h>
#include <Filterables.h>

#include <SatelliteMissionAssociatorFactory.h>
#include <SatelliteMissionAssociatorSingleton.h>

#include <ConfigurationSingleton.h>

#include <XMLIstream.h>
#include <XMLValidator.h>
#include <ftmpstream.h>
#include <XMLProgDataConsumer.h>
#include <StringUtils.h>
#include <JobOrderFactoryChain.h>
#include <DefaultJobOrder.h>

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

ACS_CLASS_DEFINE_DEBUG_LEVEL(EOFPreInventory)

namespace {
    EOFPreInventoryV _version_;
}

const string EOFPreInventory::_defaultSatMissionAssociatorFactoryKey = "STANDARD";
const string EOFPreInventory::_noSchemaAvailableFlagTag = "NoSchemaAvailableFlag";
const string EOFPreInventory::_defaultQualifiedFlagNoSchema = "VALID_PRODUCT";

EOFPreInventory::EOFPreInventory() : 
    PreInventoryBase(),
	_currentFileName(),
	_currentFileType(),
	_isHDR(),
	_datExtension(),
	_sngExtension()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	setPluginConfSpaceId("ConfigEOFPreInventory");
	setFactoryConfSpaceId("EOFPreInventoryFactory");
}

EOFPreInventory::~EOFPreInventory() throw() 
{
}

string EOFPreInventory::getClassId()
{
        //return the class name
	return "EOFPreInventory";
}
        

vector< Pair_FileName_FileType > EOFPreInventory::preProcess( const Pair_FileName_FileType& inputFile, dbConnection &conn)
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename=\"" << inputFile.first << "\", filetype=\"" << inputFile.second << "\".");

    dbFileTypeRecognition::ExtensionInfo extInfo;
    ACS_COND_THROW( !dbFileTypeRecognition::checkFNameExtension(inputFile.second, File::getFileName(inputFile.first), extInfo, conn) , //PRQA S 3081
                    exEOFPreInventoryException("\"" + inputFile.first + "\" file extension does not match the one configured on db for file type \"" + inputFile.second + "\"") ); // PRQA S 3081

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << inputFile.first  << "\" file extension matches with the one on db for given file type; extension id: " // PRQA S 3081, 3380, 3385
		    << extInfo.extId << ", number of expected extensions: " << extInfo.expectedExtNr
        << (( extInfo.expectedExtNr > 1 ) ? ("; other extension: \"" + extInfo.otherExt + "\"") : string("")))  ; // PRQA S 3081, 3380, 3385

    // Reset for the next
    _isHDR[File::getFileName(inputFile.first)] = false;

    Pair_FileName_FileType pairFnameFtype = inputFile;
    if( extInfo.extId == dbFileTypeRecognition::HDR ) {
	//_isHDR = true;
        _isHDR[File::getFileName(inputFile.first)] = true;
	_datExtension = extInfo.otherExt;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Current file is an HEADER; data extension is \"" << _datExtension << '\"') ;

    }
    else if( extInfo.extId == dbFileTypeRecognition::DAT ) {
	// _isHDR = true;
	_datExtension = File::getExt( File::getFileName(inputFile.first) );
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Current file is a DATA file with extension \"" << _datExtension << "\", HEADER extension is \"" << extInfo.otherExt << "\".");
	pairFnameFtype.first = File::addExt( File::removeExt(inputFile.first), extInfo.otherExt );
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Now setting \"" << pairFnameFtype.first << "\" as the current file.");
	 	_isHDR[File::getFileName(pairFnameFtype.first)] = true;

    }
    else 
    {
    }
	
    vector< Pair_FileName_FileType > filesToInventory;
    filesToInventory.push_back(pairFnameFtype);

    return filesToInventory;
 	
}


void EOFPreInventory::computeMetadata( const string& pathFileName, const string& fileType, Metadata& metadata, dbConnection &conn)
{
	// This is needed from isHDR
	setCurrentFileName(pathFileName ) ;

       //NB: the pathFileName passed to this method is an absolute path
 	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	try {
		ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "pathFileName= \"" << pathFileName << "\" filetype= \"" << fileType << '"' );

		_currentFileName = pathFileName;
		_currentFileType = fileType;
		
		FixedHeaderStruct fhs;
		
        readEOFFile( pathFileName, fhs);
        
		metadata = prepareMetadata( fileType, fhs, conn);
        
	}
    catch( exception& ex)
    {
        ACS_THROW(exEOFPreInventoryException( ex, "computeMetadata Exception")) ; // PRQA S 3081
    }
}      




Metadata EOFPreInventory::prepareMetadata( const string& fileType, const FixedHeaderStruct& fhs, dbConnection &conn) //PRQA S 5500, 5510
{
 	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);


	//filling Metadata
	Metadata metadata;


	// *UNIQUE_ID
	string uniqueId = getUniqueId();
    if ( !uniqueId.empty()) {
        metadata.setMetadataValue( "Unique_Id", uniqueId); 
    }
	
	bool manageSngExt = false ;
	string fileName = fillMetadataFilename( _currentFileName, fileType, metadata, conn, &manageSngExt, &_sngExtension);
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "File will be inventoried with the name \"" << fileName << '\"'
            					<< "; manage sng extension: " << boolalpha << manageSngExt << "; sng extension \"" << _sngExtension << "\".");
	
	// *FILE_VERSION
	int fileVersion = 1;
	if( !fhs.File_Version.empty() ) {
		istringstream is ( fhs.File_Version, istringstream::in);
		is >> fileVersion;
	}
	metadata.setMetadataValue( "File_Version", getFormattedVersion(fileVersion));
	
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, " File_Version=\"" << getFormattedVersion(fileVersion) << '\"');


	string system = fhs.System;
	metadata.setMetadataValue( "System", system);

	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, " System=\"" << system << '\"');

	
	// *SOURCE	
	string source = fhs.Creator;
	metadata.setMetadataValue( "Source", source);

	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, " Source=" << source);

	
	// *SOURCE_SW_VERSION
	string sourceSwVersion = fhs.Creator_Version;
	metadata.setMetadataValue( "Source_Sw_Version", sourceSwVersion);

	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, " Source_Sw_Version=" << sourceSwVersion);

		
	// *GENERATION_TIME
	string pluginConfSpaceId = getPluginConfSpaceId();
	string generationTime = convertUncorrectDate( pluginConfSpaceId, fhs.Creation_Date );
	if( !generationTime.empty()) {
		metadata.setMetadataValue( "Generation_Time", generationTime);
    }
	else
	{
		ACS_THROW(exEOFPreInventoryException("Empty Generation_Time in file " + _currentFileName)) ; // PRQA S 3081
	}

	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, " Generation_Time=" << generationTime)


	// *VALIDITY_START
	string validityStart = convertUncorrectDate( pluginConfSpaceId, fhs.Validity_Start );
	if( !validityStart.empty()) {
		metadata.setMetadataValue( "Validity_Start", validityStart);
    }
	else
	{
		ACS_THROW(exEOFPreInventoryException("Empty Validity_Start in file " + _currentFileName)) ; // PRQA S 3081
	}
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, " Validity_Start=" << validityStart);


	// *VALIDITY_STOP
	string validityStop = convertUncorrectDate( pluginConfSpaceId, fhs.Validity_Stop);
	if( !validityStop.empty()) {
		metadata.setMetadataValue( "Validity_Stop", validityStop);
    }
	else
	{
		ACS_THROW(exEOFPreInventoryException("Empty Validity_Stop in file " + _currentFileName)) ; // PRQA S 3081
	}
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, " Validity_Stop=" << validityStop);

	 
/*	
	// START_ORBIT_NUMBER
	string startOrbitNumber = "";
	metadata.setMetadataValue( "Start_Orbit_Number", StringUtils::fromString<int>( startOrbitNumber));
	exCLASS_BGN_DEBUG(10)
		ostringstream msg;
		msg << " Start_Orbit_Number=" << startOrbitNumber;
		exFDebug::notify(msg.str());
	exCLASS_END_DEBUG
		 
	// STOP_ORBIT_NUMBER
	string stopOrbitNumber = "";
	metadata.setMetadataValue( "Stop_Orbit_Number", StringUtils::fromString<int>( stopOrbitNumber));
	exCLASS_BGN_DEBUG(10)
		ostringstream msg;
		msg << " Stop_Orbit_Number=" << stopOrbitNumber;
		exFDebug::notify(msg.str());
	exCLASS_END_DEBUG
*/
	

/*	
	// GEOGRAPHIC_LOCALIZATION
	vector<Metadata::GeoPointInfo>  vGeoPntInfo;
	dbGeoObject::Type geoType;

		metadata.setGeoPoints( vGeoPntInfo);	
		metadata.setGeoType( geoType);

		// TIMERANGE of Geographic_Localization
		metadata.setFieldValue( "Start_Time", validityStart);
		metadata.setFieldValue( "Stop_Time", validityStop);
*/
	
	
/*	
	// QUALITY_INFO
	string qualityInfo = "";
	metadata.setMetadataValue( "Quality_Info", qualityInfo);
	exCLASS_BGN_DEBUG(10)
		ostringstream msg;
		msg << " Quality_Info=" << qualityInfo;
		exFDebug::notify(msg.str());
	exCLASS_END_DEBUG
*/
	
	// *VALIDITY_FLAG
	metadata.setMetadataValue( "Validity_Flag", "true");

	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, " Validity_Flag= true");
	

	// *VALIDATION_DATE
	DateTime dt;            //today datetime
	ostringstream strTime;
	strTime << "UTC=";
	dt.asISO8601( strTime, 6); // PRQA S 4400
	string creationTime = strTime.str();
	metadata.setMetadataValue( "Validation_Date", creationTime); 

	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, " Validation_Date=" << creationTime);


	// *HEADER_SIZE 
	off_t headerSize = 0;
	if( _isHDR [File::getFileName (_currentFileName)]) {
		headerSize = File::size( _currentFileName); // PRQA S 3000
    }
	
	metadata.setMetadataValue( "Header_Size", headerSize); 
	
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, " Header_Size=" << headerSize);
	

	// *DATA_SIZE prelevo la size reale
	off_t fileSize = 0;
	//if( _isHDR)
	if( _isHDR [File::getFileName (_currentFileName)]) {
		string dblFileName = File::addExt(File::removeExt(_currentFileName), _datExtension);
		ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "Going to compute the size of \"" << dblFileName << "\" data file.") ;
		fileSize = File::size( dblFileName);
	}
	else {
		fileSize = File::size( _currentFileName);
    }
	
	metadata.setMetadataValue( "Data_Size", fileSize);
	
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB," Data_Size=" << fileSize);
		
	// *FILE_TYPE
	bool getFtypeFromFramework = true;
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	try {
		rs.getValue( "PreInventory."+pluginConfSpaceId+".FiletypeFromFramework", getFtypeFromFramework);
		ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "FiletypeFromFramework from configuration is " << boolalpha << getFtypeFromFramework );
	}
	catch(rsResourceSet::NotFoundException &e) { /* do nothing */ }

	string fType = fileType;

	metadata.setMetadataValue( "File_Type", fType);

	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB," File_Type=" << fType);


	// *FILE_CLASS
	string fileClass = convertOriginalFileClass( pluginConfSpaceId, fileType, fhs.File_Class ) ;
	if( fileClass.empty())
	{
		//S1A_OPER_MPL_ORBPRE_20120601T114056_20120608T113243_0001.EOF
		//S1A_OPER_MPL_ORBRES_20120601T114056_20120608T113243_0001.EOF		
		//S1A_OPER_MPL_TLEPRE_20160306T000000_20160313T000000_0001.HDR
		//012345678901234567890
		fileClass = File::getFileName(_currentFileName).substr( 4, 4); // PRQA S 4400
	}
	metadata.setMetadataValue( "File_Class", fileClass);

	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, " File_Class=" << fileClass);

/*
	// SENSOR_ID
	string sensorId = "";
	metadata.setMetadataValue( "Sensor_Id", sensorId); 
	exCLASS_BGN_DEBUG(10)
		ostringstream msg;
		msg << " Sensor_Id=" << sensorId;
		exFDebug::notify(msg.str());
	exCLASS_END_DEBUG


	// SENSOR_MODE
	string sensorMode = "";
	metadata.setMetadataValue( "Sensor_Mode", sensorMode);
	exCLASS_BGN_DEBUG(10)
		ostringstream msg;
		msg << " Sensor_Mode=" << sensorMode;
		exFDebug::notify(msg.str());
	exCLASS_END_DEBUG
*/
	

/*
	// ACQUISITION_STATION_ID
	string acquisitionStationId = "";
	metadata.setMetadataValue( "Acquisition_Station_Id", acquisitionStationId);
	exCLASS_BGN_DEBUG(10)
		ostringstream msg;
		msg << " AcquisitionStationId=" << acquisitionStationId;
		exFDebug::notify(msg.str());
	exCLASS_END_DEBUG


	// PROCESSING_STATION_ID
	string processingStationId = "";
	metadata.setMetadataValue( "Processing_Station_Id", processingStationId);
	exCLASS_BGN_DEBUG(10)
		ostringstream msg;
		msg << " ProcessingStationId=" << processingStationId;
		exFDebug::notify(msg.str());
	exCLASS_END_DEBUG	
*/
	
	// *SATELLITE_ID 	
	string satelliteCode = fileName.substr( 0, 3);
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, " SatelliteCode=" << satelliteCode);
		
	string satFactoryKey = getSatMissionAssociatorFactoryKey( satelliteCode );
	if( satFactoryKey.empty() ) {
		satFactoryKey = _defaultSatMissionAssociatorFactoryKey;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "SatelliteMissionAssociator factory key \"" << satFactoryKey << '\"');

	SatelliteMissionAssociatorFactory factoryObject ;

	SatelliteMissionAssociator* satelliteMissionAssociator = SatelliteMissionAssociatorSingleton::instance()->newSatelliteMissionAssociator(StringUtils::uppercase(satFactoryKey));
	ACS_COND_THROW(!satelliteMissionAssociator, exEOFPreInventoryException("Can't Instantiate \"" + satFactoryKey + "\": got NULL pointer from SatelliteMissionAssociatorFactory") );	 // PRQA S 3081

	string satelliteAcronym;
	int missionNumber = 0;
	bool satelliteAndMissionFromConf = false;
	try { 
		satelliteAcronym = satelliteMissionAssociator->getSatelliteAcronym(satelliteCode);
	}
	catch( SatelliteMissionAssociator::exKeyNotFoundException &e )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Cannot get satellite acronym corresponding to \"" << satelliteCode << "\" from factory");
		try {
			satelliteAcronym = getSatelliteAcronymFromConf( satelliteCode, missionNumber);
		}
		catch( exception &ee )
		{
			ACS_LOG_ERROR("Cannot get satellite acronym from configuration with key \"" << satelliteCode << "\" :" << ee.what() );
		}
		ACS_COND_THROW( satelliteAcronym.empty(), exEOFPreInventoryException( e , "Cannot even get satellite \""+satelliteCode+"\" from configuration" ) );
		satelliteAndMissionFromConf = true;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite acronym=\"" << satelliteAcronym << '\"');
	int satelliteId = getSatelliteIdFromSatelliteCode(satelliteAcronym, conn);
	
	if( satelliteId != 0 ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite_Id=" << satelliteId);
		metadata.setMetadataValue( "Satellite_Id", satelliteId);
		
		if( not satelliteAndMissionFromConf ) {
			missionNumber = satelliteMissionAssociator->getMissionId(satelliteCode);
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Mission number=" << missionNumber);
		if( missionNumber != 0 ) {
			metadata.setMetadataValue( "Mission", missionNumber);
    	}
	}
	
	bool noSchemaAvailable = false;
	string qualified = getQualifiedValue(noSchemaAvailable);
	if( !qualified.empty() ) {
		NameIdAssociator<int> nameIdAssociator( "t_qualifiedflag", "id", "name" , conn.getKey()) ;
		nameIdAssociator.load() ;
		dbPersistent::IdType qualifiedId = 0;
		try {
			qualifiedId = nameIdAssociator.id(qualified); 
		}
		catch(exception &e)
		{
			if(not noSchemaAvailable) {
				throw;
			}
			ACS_LOG_WARNING("*** CANNOT CONVERT \"" << qualified << "\" string into the proper numeric id: product will be marked " << _defaultQualifiedFlagNoSchema << " as default value. PLEASE CHECK " 
							<< _noSchemaAvailableFlagTag << " TAG VALUE IN CONFIGURATION: it must match name column of one t_qualifiedflag table record.");
			ACS_LOG_WARNING("Error was: " << e.what());
			qualifiedId = nameIdAssociator.id(_defaultQualifiedFlagNoSchema); 
		}
		metadata.setMetadataValue( "Qualified", qualifiedId);
	}
	setFurther(metadata);

	return metadata;

}


void EOFPreInventory::updateProductAndMetadata( const std::string& newVersion, Metadata& metadata)
{
  	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
		
	metadata.setMetadataValue( "File_Version", newVersion);
	
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File_Version field updated with the value \"" << newVersion << '\"');

	string qualityInfo = "DUP";
	metadata.setMetadataValue( "Quality_Info", qualityInfo);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Quality_Info field set to \"" << qualityInfo << '\"');
			
}

string EOFPreInventory::doPackaging( Metadata& metadata, const string & firstPath, const string & filetype, const string & secondPath, dbConnection & )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	
	string packagePath;
	if( secondPath.empty() ) {
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No package to be performed");
		packagePath = firstPath;
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Package to be performed");

	// Going to create the name of package file
		packagePath = File::removeExt(firstPath);
		if( !_sngExtension.empty() ) {
			packagePath = File::addExt(packagePath, _sngExtension);
        }
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Package file path to inventory is \"" << packagePath << "\". Going to concat \"" << firstPath << "\" header file to \""
				<< secondPath << "\" data file.");
		
		//Stick HDR to the end of DBL in the new file packagePath
		File::concat( secondPath, firstPath, packagePath);

	}
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning \"" << packagePath << "\" path to inventory");
	return packagePath;
	
}


void EOFPreInventory::readEOFFile( const string& pathFileName, FixedHeaderStruct& fhs) // PRQA S 4211, 5500, 5510
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename \""  << pathFileName << '\"');
	
	//Reading data from .EOF file (xml) or .HDR file (xml)
	rsResourceSet imXML;
    XMLIstream xml( pathFileName);
	
/*
root è Earth_Explorer_Header nel caso di file .HDR mentre 
per i file .EOF il root tag e' <Earth_Explorer_File .....> 
<Earth_Explorer_Header> 
    <Fixed_Header>      
      <File_Name>S1A_OPER_MPL_ORBPRE_20120601T114056_20120608T113243_0001</File_Name>
      <File_Description>FOS Predicted Orbit File</File_Description>
      <Notes/>
      <Mission>Sentinel 1A</Mission>
      <File_Class>OPER</File_Class>
      <File_Type>MPL_ORBPRE</File_Type>
      <Validity_Period>
        <Validity_Start>UTC=2012-06-01T11:40:56</Validity_Start>
        <Validity_Stop>UTC=2012-06-08T11:32:43</Validity_Stop>
      </Validity_Period>
      <File_Version>0001</File_Version>
      <Source>
        <System>FOS</System>
        <Creator>NAPEOS</Creator>
        <Creator_Version>1.0</Creator_Version>
        <Creation_Date>UTC=2012-06-01T12:00:00</Creation_Date>
      </Source>
    </Fixed_Header>
    <Variable_Header>
      <Ref_Frame>EARTH_FIXED</Ref_Frame>
      <Time_Reference>UTC</Time_Reference>
      <Main_Product_Header>
        <Baseline>Baseline</Baseline>
      </Main_Product_Header>
    </Variable_Header>
  </Earth_Explorer_Header>
*/	
	string rootTag = "Earth_Explorer_Header.Fixed_Header";
	
	//if( _isHDR ) {
	if( extractMetadataFromHdr(pathFileName) ) {
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "file HDR");
	
		bool hdrSchemaAsTopLevel = false;
		try {
			ConfigurationSingleton::instance()->get().getValue( "PreInventory."+getPluginConfSpaceId()+".HdrSchemaAsTopLevel", hdrSchemaAsTopLevel);
			ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "HdrSchemaAsTopLevel from configuration is " << boolalpha << hdrSchemaAsTopLevel );
		}
		catch(rsResourceSet::NotFoundException &e) { /* do nothing */ }
		
		if( not hdrSchemaAsTopLevel ) {
			rootTag = "Fixed_Header";
		}
	
	// write in resource set	
		xml >> imXML;
		
		ACS_COND_THROW( not imXML.hasKey( rootTag) , exEOFPreInventoryException( "Malformed product: root tag in \"" + pathFileName + "\" file is not \"" + rootTag + '\"') );
	}
	else {
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "file EOF");
		
	// First filter interesting nodes and values
		string xmlRootTag = "Earth_Explorer_File/Earth_Explorer_Header/Fixed_Header/";
		vector<string> nodes ;
        nodes.push_back(xmlRootTag+"File_Name") ;
        nodes.push_back(xmlRootTag+"File_Description") ;
        nodes.push_back(xmlRootTag+"Notes");
        nodes.push_back(xmlRootTag+"Mission");
        nodes.push_back(xmlRootTag+"File_Class");
        nodes.push_back(xmlRootTag+"File_Type");
        nodes.push_back(xmlRootTag+"Validity_Period/Validity_Start");
        nodes.push_back(xmlRootTag+"Validity_Period/Validity_Stop");
        nodes.push_back(xmlRootTag+"File_Version");
        nodes.push_back(xmlRootTag+"Source/System");
        nodes.push_back(xmlRootTag+"Source/Creator");
        nodes.push_back(xmlRootTag+"Source/Creator_Version");
        nodes.push_back(xmlRootTag+"Source/Creation_Date");
        
        addFurtherNodes(nodes) ;
		
		bool neglectNodeNSpaces = true; // e.g. tag could be n1:Earth_Explorer_File/n1:Earth_Explorer_Header/Fixed_Header
	    XMLProgDataConsumer dc(nodes, neglectNodeNSpaces) ;
	    xml.addConsumer(&dc) ;
		
	// Write in a temporary files
		oftmpstream oftmp(".XmlFiltered_");
	    xml.saxCopy(oftmp.path()) ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Interesting nodes and values filtered in \"" << oftmp.path() << "\" file");
		
		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			oftmp.setAutoRemove(false);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Temporary file \"" << oftmp.path() << "\" will not be removed.");
		ACS_CLASS_END_DEBUG

	
	// read XML filtered file and write in resource set;
		XMLIstream xmlFilteredStream(oftmp.path());
		xmlFilteredStream >> imXML;

    

	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Root tag \"" << rootTag << '\"');
	
	string tag = rootTag + ".File_Name";
	if( imXML.hasKey( tag)) {
		imXML.getValue( tag, fhs.File_Name);
	}
		
    tag = rootTag + ".File_Description";
	if( imXML.hasKey( tag)) {
		imXML.getValue( tag, fhs.File_Description);
	}
		
    tag = rootTag + ".Notes";
	if( imXML.hasKey( tag)) {
		imXML.getValue( tag, fhs.Notes);
	}
	
    tag = rootTag + ".Mission";
	if( imXML.hasKey( tag)) {
		imXML.getValue( tag, fhs.Mission);
	}
		
	tag = rootTag + ".File_Class";
	if( imXML.hasKey( tag)) {
		imXML.getValue( tag, fhs.File_Class);
	}
		
	tag = rootTag + ".File_Type";
	if( imXML.hasKey( tag)) {
		imXML.getValue( tag, fhs.File_Type);
	}

	tag = rootTag + ".Validity_Period.Validity_Start";
	if( imXML.hasKey( tag)) {
		imXML.getValue( tag, fhs.Validity_Start);
	}
		
	tag = rootTag + ".Validity_Period.Validity_Stop";
	if( imXML.hasKey( tag)) {
		imXML.getValue( tag, fhs.Validity_Stop);
	}
		
	tag = rootTag + ".File_Version";
	if( imXML.hasKey( tag)) {
		imXML.getValue( tag, fhs.File_Version);
	}
		
	tag = rootTag + ".Source.System";
	if( imXML.hasKey( tag)) {
		imXML.getValue( tag, fhs.System);
	}
		
	tag = rootTag + ".Source.Creator";
	if( imXML.hasKey( tag)) {
		imXML.getValue( tag, fhs.Creator);
	}
		
	tag = rootTag + ".Source.Creator_Version";
	if( imXML.hasKey( tag)) {
		imXML.getValue( tag, fhs.Creator_Version);
	}
		
	tag = rootTag + ".Source.Creation_Date";
	if( imXML.hasKey( tag)) {
		imXML.getValue( tag, fhs.Creation_Date);
	}
	
    extractFurther(imXML) ;
}




string EOFPreInventory::getUniqueId()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	string uID;

	if( ( getContext().which == getContext().TL ) || ( getContext().which == getContext().DS ) )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Context is " << getContext().which );
		string pFileName = getParametersFile();
        if ( ! pFileName.empty() )
        {

            rsResourceSet imXML;
            XMLIstream xml( pFileName);
            xml >> imXML;

            
            string tag = "Unique_Id";
            if( imXML.hasKey( tag))
            {
                imXML.getValue( tag, uID);

                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB," Unique_Id=" << uID);
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
				
				ostringstream os;
				os << "NOT found Unique_Id tag in the ParametersFile \"" << pFileName << "\" for " << getContext().which;
				ACS_COND_THROW( uniqueIdMustOccurInParFile, exEOFPreInventoryException(os.str()) ); //PRQA S 3081
				ACS_LOG_WARNING(os.str());
                
            }
        }
	}
	else if( getContext().which == getContext().DIS)
	{
		ACS_LOG_WARNING("getUniqueId NOT IMPLEMENTED for DIS");

	}
	else if( getContext().which == getContext().PROD)
	{
		ACS_LOG_WARNING("getUniqueId NOT IMPLEMENTED for PROD");
	}
	else
	{
		ostringstream msg;
		msg << "getUniqueId context.which=\"" << getContext().which << "\" NOT Recognized";
		ACS_THROW(exEOFPreInventoryException( msg.str())); // PRQA S 3081
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Going to return \"" << uID << '\"');

	return uID;
	 
}


string EOFPreInventory::getParametersFile()
{
    string parFileName ("");
    ACS_SMARTPTR<JobOrder> jo;

    string satellite = getContext().metadataInfo.satelliteName;
    string mission = getContext().metadataInfo.mission;
    string procName = getContext().metadataInfo.processorName;
    string procVersion = getContext().metadataInfo.processorVersion;

    {
		//Get JobOrder from JobOrder Factory if context is TL, otherwise use default Job order
		if( getContext().which == getContext().TL  )
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TL Context");
        	StringKey key = StringKeyUtils::makeKey( satellite, mission, procName, procVersion );
        	try {
        		jo.reset ((JobOrderFactoryChain::instance())->newObject(key) );
        	} catch (exception& ex) {
                	ACS_LOG_WARNING("Unable to get a valid JobOrder plugin using the default one") ;
                	jo.reset (new DefaultJobOrder);
        	}
		}
		else if( getContext().which == getContext().DS ) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DS Context");
			jo.reset (new DefaultJobOrder);
		}
		else { // it shuold not happen
			ACS_THROW( exEOFPreInventoryException( "Unexpected context: "+ Context::toString(getContext().which)+ " (should be TL/DS)")) ; // PRQA S 3081
		}

		XMLIstream jofileXml( getContext().argvParameters["jobOrder"].as<string>());
        jofileXml >> *jo ;

		parFileName = jo->getParametersFileName();
		if( parFileName.empty())
		{
			//just notify.
			ACS_LOG_WARNING("ParametersFile not specified");
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "parameters filename =\"" << parFileName << '\"');
    }
    return parFileName;
}


int EOFPreInventory::getSatelliteIdFromSatelliteCode( const string& satCode, dbConnection &conn)
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    dbSatelliteQuery * satelliteQuery=dynamic_cast<dbSatelliteQuery*>(dbQueryFactoryChain::instance()->newObject("dbSatelliteQuery", conn.getKey())); // PRQA S 3081, 4412

    ACS_COND_THROW( !satelliteQuery, exEOFPreInventoryException( "Error in executing dbSatelliteQuery")); //PRQA S 3081

    dbGenQueryScopeGuard satelliteQueryGuard( satelliteQuery);
    const string &dbKey = conn.getKey() ;
    dbQueryParameters satParameters(dbKey) ; 
    satParameters.addParameter( dbParam(dbKey, "t_satellites", "satelliteacronym", dbParam::EQUAL_TO, dbConv(dbKey, satCode))); // PRQA S 3050, 3081
    satelliteQuery->addParameter( satParameters);
    unsigned long num = satelliteQuery->doQuery(conn);

    int satId = 0;  //0<=>false
    if (num > 0) 
    {
	satId = ((*satelliteQuery)[0])->getSatelliteId();
    }

    return satId;
}


int EOFPreInventory::getSensorIdFromSensorCode( const string& sensorCode, dbConnection &conn)
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    //Con questi valori devo andare sul db per prendere le rispettive Id 
    //query su T_Sensors
    dbSensorQuery * sensorQuery=dynamic_cast<dbSensorQuery*>(dbQueryFactoryChain::instance()->newObject("dbSensorQuery", conn.getKey())); // PRQA S 3081, 4412
    
    ACS_COND_THROW( !sensorQuery, exEOFPreInventoryException( "Error in executing dbSensorQuery")); //PRQA S 3081
    
    dbGenQueryScopeGuard sensorQueryGuard( sensorQuery);
    const string &dbKey = conn.getKey() ;
    dbQueryParameters sensParameters(dbKey) ; 
    sensParameters.addParameter( dbParam(dbKey, "t_sensors", "sensorname", dbParam::EQUAL_TO, dbConv(dbKey, sensorCode))); // PRQA S 3050, 3081
    sensorQuery->addParameter( sensParameters);
    unsigned long num = sensorQuery->doQuery( conn );

    int sensId = 0;  //0<=>false
    if (num > 0) {
        sensId = ((*sensorQuery)[0])->getSensorId();
    }

    return sensId;
}


namespace {


struct Registration { //PRQA S 2173, 2175
    Registration() : object(), doVersioning() {}
    Registration(string o, bool doV ) : object(o), doVersioning(doV) {} //PRQA S 2010, 2171

    string object ; //PRQA S 2100 2
    bool doVersioning ;
} ;

struct findFType { //PRQA S 2173, 2175, 2185
    explicit findFType(const string &f) : ftype_ (f) {} //PRQA S 2180
    bool operator()  (const Registration &r) const { return r.object == ftype_ ; }

    string ftype_ ; //PRQA S 2100
} ;

}

/*
template<>
void rsResourceSet::setValue<Registration>(const std::string &res, const Registration &v)
{
	Lock lock(_mutex) ;
	findOrInsertValue(res+".Object")->set(v.object) ;
	findOrInsertValue(res+".DoVersioning")->set(v.doVersioning) ;
}
*/

template<>
void rsResourceSet::getValue(const std::string &res, Registration &v) const 
{
    Lock lock(_mutex) ;
	getValue(res+".Object")->get(v.object) ;
	getValue(res+".DoVersioning")->get(v.doVersioning) ;
} 


bool EOFPreInventory::overrideVersionHandling(bool& doVersioning)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
			
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to check if versioning is allowed for file type \"" << _currentFileType << '\"');
	
    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
    vector<Registration> reg ;
    rs.getArray("PreInventory.PluginsRegistrationSettings." + getFactoryConfSpaceId() + ".List_Of_Registrations.Registration", reg) ;
    
    ACS_LOG_INFO("Registration entries: " << reg.size()) ;
    vector<Registration>::const_iterator r = find_if(reg.begin(), reg.end(), findFType(_currentFileType)) ; //PRQA S 3081 2
    ACS_COND_THROW(r == reg.end(),exEOFPreInventoryException("Cannot find fileType=\""+_currentFileType+"\" in configuration")) ;

    doVersioning = r->doVersioning ;
    return true ;
}


bool EOFPreInventory::doVersioning(const Metadata& metadata, std::string& newVersion, dbConnection &conn)
{

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    bool isTheVersionChanged(false);

    //Execute Query
    dbInventoryObjectQuery* dbInvObjectQuery =dynamic_cast<dbInventoryObjectQuery*>( dbQueryFactoryChain::instance()->newObject("dbInventoryObjectQuery", conn.getKey()) ); // PRQA S 3081, 4412
    ACS_COND_THROW( !dbInvObjectQuery, exEOFPreInventoryException("Error in executing dbSensorQuery") );	 // PRQA S 3081
    dbGenQueryScopeGuard stationQueryGuard(dbInvObjectQuery);
	
    //Get specific dbQueryParametes
    dbQueryParameters parameters = getProductFilenameInInventoryQueryParameters(conn, metadata);
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "About to query t_inventory record with the following condition: " << parameters.getSqlString());

    dbInvObjectQuery->addParameter( parameters );

    unsigned long int countRow = dbInvObjectQuery->doQuery(conn);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "num = dbInventoryObjectQuery->doQuery() = " << countRow ); 

    if( countRow > 0) {

        int maxVers = std::numeric_limits<int>::min();
		
		string currentVersion = metadata.getMetadataValue( "File_Version");
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Current file version from metadata: \"" << currentVersion << '\"' ); 
		
		bool found = false;
        for(unsigned long int i=0; i<countRow; i++) {

            string version = ((*dbInvObjectQuery)[i])->getFileVersion();
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "dbInvObjectQuery->getFileVersion-> version = " << version ); 
			if( version == currentVersion ) {
				found = true;
            	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "t_inventory entry with filename \"" << metadata.getGlobalFileName() << "\" and version \"" << currentVersion
						<< "\" already occurring in db") ; 
			}
			
			{
				int vers = atoi(version.c_str()) ;
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
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No t_inventory entry with filename \"" << metadata.getGlobalFileName() << "\" has version \"" << currentVersion
					<< "\": no new Version required." ); 
		}

    } else {
       ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No t_inventory entry with filename \"" << metadata.getGlobalFileName() << "\" occurs in db: no new Version required." ); 
    }

    return isTheVersionChanged;
}


string EOFPreInventory::getQualifiedValue(bool & noSchemaAvailable)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	string qualified;
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	try {
		bool validateXml = false ;
		string pluginConfSpaceId = getPluginConfSpaceId();
		rs.getValue( "PreInventory."+pluginConfSpaceId+".ExternalSchemaValidation", validateXml);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ExternalSchemaValidation from configuration is " << boolalpha << validateXml);
				
		qualified = "NOT_AVAILABLE";
		if( validateXml ) {
			string filepathToValidate = getFilepathToValidate();
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "File to validate is \"" << filepathToValidate << '\"' );
			XMLValidator validator(filepathToValidate) ;
			qualified = "PRODUCT_HAS_ERRORS";
			try {
				bool schemaExists = false;
				if (validator.validate(true, &schemaExists)) {
					qualified = "VALID_PRODUCT";
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << filepathToValidate << "\" is a valid xml file");
					
					if( !schemaExists ) {
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No xsd schema occurring in \"" << filepathToValidate << "\" xml file");
						string noSchemaAvailableValue = getNoSchemaAvailableFlag();
						if( not noSchemaAvailableValue.empty() ) {
							qualified = noSchemaAvailableValue;
							noSchemaAvailable = true;
						}
					}
				}
			}
			catch(exception &e)
			{
				ACS_LOG_ERROR("Error trying to validate \"" << filepathToValidate << "\" file:\n" << e.what());
				qualified = "VALIDATION_FAILURE";		
			}
		}
		else {
			try {
				rs.getValue( "PreInventory."+pluginConfSpaceId+".CannotValidateValue", qualified);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "CannotValidateValue from configuration is " << qualified);
			}
			catch(rsResourceSet::NotFoundException &)
			{}
		}

	}
	catch(rsResourceSet::NotFoundException &e)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ExternalSchemaValidation does not occur in configuration");
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "qualified: \"" << qualified << '\"');
	
	return qualified;

}


string EOFPreInventory::getSatMissionAssociatorFactoryKey( const string& satCode)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

	string key;
    //read the configuration file from database
    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	string localRootTag = "PreInventory."+getPluginConfSpaceId()+".SatMissionsAssociatorKeys.Pair";
    size_t elements = rs.getArraySize( localRootTag ); // PRQA S 3000, 3010, 4412
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << localRootTag << ", elements " << elements);

	bool found = false;
	for( size_t i=0; (i<elements) && !found; i++)
	{
		string tag = localRootTag;
		if( elements > 1 ) {
			ostringstream os ;
			os << "[" << i << "]" ;
			tag += os.str();
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TAG=\"" << tag << "\", satellite code to find=\"" << satCode << '\"');

		string satCodeValue;
		rs.getValue( tag + ".satelliteCode.value", satCodeValue);
		if( satCodeValue == satCode) {
			rs.getValue( tag + ".key.value", key);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "satellite code found=(\"" << satCode << "\"-->\"" << key << "\")" );
			found = true;
		}
	}
	if( !found)
	{	
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "satellite code \"" << satCode << "\" not found in config file. Returning \"" << key << '\"');
	}
		
	return key;
}


string EOFPreInventory::getNoSchemaAvailableFlag() // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	
	string noSchemaAvailableFlag;
	bool found = false;
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	const string noSchemaAvailTag = "PreInventory."+getPluginConfSpaceId()+ '.' + _noSchemaAvailableFlagTag;
	if ( rs.hasKey(noSchemaAvailTag) ) {
		found = true;
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << noSchemaAvailTag << "\" tag found in configuration");
		rs.getValue( noSchemaAvailTag, noSchemaAvailableFlag);
	}
	else {
		size_t registeredEntries = 0;
		string rootTag = "PreInventory.PluginsRegistrationSettings." + getFactoryConfSpaceId() + ".List_Of_Registrations";
		rs.getValue(rootTag+ ".count.value", registeredEntries);

		for( size_t i=0; i<registeredEntries ; i++) {
			ostringstream osTag;
			osTag << rootTag << ".Registration[" << i << "]";
			string tag = osTag.str();
			string fileType;
			rs.getValue( tag + ".Object", fileType);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << " fileType=\"" << fileType << '\"');

			if( fileType == _currentFileType) {
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "fileType \"" << _currentFileType << "\" found" );
				
				const string noSchemaAvailFlagTag = tag + "." + _noSchemaAvailableFlagTag;
				if( rs.hasKey( noSchemaAvailFlagTag ) ) {
					found = true;
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\"" << noSchemaAvailFlagTag << "\" tag found in configuration");
					rs.getValue(noSchemaAvailFlagTag, noSchemaAvailableFlag);
				}
				else {
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << _noSchemaAvailableFlagTag << "\" tag not occurring in configuration");
				}
				
				break;
			}
		}
	}
	
	if(found && noSchemaAvailableFlag.empty()) {
		ACS_LOG_WARNING(_noSchemaAvailableFlagTag << " tag occurs in configuration BUT IS NOT valorized: "
				<< " cannot mark product with a flag corresponding to no schema occurence" ) ; 
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << _noSchemaAvailableFlagTag << "\" value=\"" << noSchemaAvailableFlag << "\"");
	
	return noSchemaAvailableFlag;
}


bool EOFPreInventory::isHdr() const throw()  //PRQA S 4020
{
	std::map <std::string, bool>::const_iterator it = _isHDR.find (File::getFileName(_currentFileName));
	if (it != _isHDR.end())
	{
		return it->second;
	}
	else
	{
		return false;
	}
}

//converte il missionId nel corrispondente mission per l'inventory
string EOFPreInventory::getSatelliteAcronymFromConf( const string& satCode, int & mission)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);

    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	string localRootTag = "PreInventory."+getPluginConfSpaceId()+".SatelliteMissions.Pair";
    size_t elements = rs.getArraySize( localRootTag ); // PRQA S 3000, 3010, 4412
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << localRootTag << ", elements " << elements);

	string satKey = StringUtils::uppercase(satCode);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite code to find=\"" << satKey << '\"');
	string satellite;
	for( size_t i=0; (i<elements) && satellite.empty(); i++) {
		string tag = localRootTag;
		if( elements > 1 ) {
			ostringstream os ;
			os << "[" << i << "]" ;
			tag += os.str();
		}
		string satelliteValue;
		rs.getValue( tag + ".satellite.value", satelliteValue);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=\"" << tag << "\", satellite value: \"" << satelliteValue << '\"');
		if( StringUtils::equals(satelliteValue, satKey) ) {
			satellite = satelliteValue;
			rs.getValue( tag + ".mission.value", mission);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite found: \"" << satellite << "\" --> " << mission  );
		}
		
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite \"" << satKey << '\"' << ( satellite.empty() ? " NOT" : "" ) << " found in configuration");
		
	return satellite;
}

bool EOFPreInventory::extractMetadataFromHdr(const string & s)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	 
	return _isHDR [File::getFileName (_currentFileName)] ;
}

_ACS_END_NAMESPACE
