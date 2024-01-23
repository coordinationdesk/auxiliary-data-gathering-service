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

	$Prod: A.C.S. libGRIBPreInventory$

	$Id$

	$Author$

	$Log$
	Revision 1.22  2016/03/11 09:09:15  tergem
	DefaultJobOrder dependency removed. Parameter File read from context
	
	Revision 1.21  2015/09/10 15:35:14  damdec
	ADC-315: System field set from configuration (not mandatory)
	
	Revision 1.20  2015/08/26 15:19:47  marfav
	ADC-316 added configuration for start validity offset
	
	Revision 1.19  2015/07/20 09:57:47  lucio.pulvirenti
	S3MPM-549: getUniqueId: DS context now managed.
	
	Revision 1.18  2014/09/08 10:25:42  lucio.pulvirenti
	S1PDGS-31532: experimentVersionNumber key value no used: GRIBStruct ExperimVerNumber element removed.
	
	Revision 1.17  2014/06/27 16:54:30  marpas
	coding best practices application in progress
	
	Revision 1.16  2013/10/21 15:00:03  lucio.pulvirenti
	getFormattedVersion method removed: the one pf base class used instead.
	overrideVersionHandling method implemented getting flag from configuration.
	updateProductAndMetadata method implemented to update QualityInfo.
	
	Revision 1.15  2013/09/02 15:45:07  lucio.pulvirenti
	ADC-185
	prepareMetadata: Satellitename from context is converted now into satellite code by static map.
	a factory is used to return the proper satellite acronym and mission depending on the context.
	getSatMissionAssociatorFactoryKey method added to get the factory key from configuration corresponding to a given satellite code.
	getSatelliteIdFromSatelliteName method replaced by getSatelliteIdFromSatelliteCode.
	
	Revision 1.14  2013/06/21 08:03:00  lucio.pulvirenti
	After Carla Santella's mail (Matteo Airoldi comment on jira issue ADC-180 19/Jun/13),
	changed criterium to recognize AX___MFA_AX  files (ECMWF_MFA internal type): delta between the second
	and the first date must be (6h, 12h) instead of (6h, 1min).
	
	Revision 1.13  2013/06/13 15:21:41  lucio.pulvirenti
	File type changed after request on Matteo Airoldi, jira issue ADC-180.
	
	Revision 1.12  2013/05/30 09:05:22  lucio.pulvirenti
	getFtypeFromFramework flag moved under Registrion node to manage different cases with the same configuration.
	getFtypeFromFramework method implemented at this respect.
	
	Revision 1.11  2013/05/29 15:53:08  lucio.pulvirenti
	readGRIBFile: modified reading of dataTime to handle 0000 time too.
	prepareMetadata: bug fixed when computing validity start time across midnight of 31 Dec.
	prepareMetadata: added possibility to compute filetype from filename (see jira issue ADC-178) from configuration.
	getFtypeFromFilename private method to compute filetype according to jira issue ADC-178.
	getSatelliteIdFromSatelliteName: regression fixed.
	
	Revision 1.10  2013/04/29 16:29:10  marpas
	base class interface changed
	
	Revision 1.9  2013/04/05 09:38:21  marpas
	NameIdAssociator partial refactoring
	
	Revision 1.8  2013/02/21 16:46:45  lucio.pulvirenti
	dbConnection parameter added to prepareMetadata,getSatelliteIdFromSatelliteName,computeMetadata methods.
	
	Revision 1.7  2012/12/14 14:16:57  lucio.pulvirenti
	Metadata File_Name field now filled by fillMetadataFilename method of base class.
	
	Revision 1.6  2012/12/11 14:46:45  chicas
	Removed StringUtils:toString; UniqueId Satellite, Mission no more mandatory; Parameter file could be empty or not specified
	
	Revision 1.5  2012/12/07 17:11:36  lucio.pulvirenti
	FilenameUtils substitued by File class ;
	doPackaging interface changed : now returning the path on disk of file to inventory ;
	
	Revision 1.4  2012/10/29 13:46:21  giufor
	updated prepareMetadata: creation of geo box
	
	Revision 1.3  2012/10/23 08:55:50  giufor
	updated prepareMetadata method
	
	Revision 1.2  2012/10/22 15:19:23  giufor
	implemented readGRIBFile method
	implemented computeMetadata method
	
	Revision 1.1.1.1  2012/10/18 15:07:33  giufor
	Import libGRIBPreInventory
	
        
*/ 

#include <GRIBPreInventory.h>
#include "GRIBPreInventoryV.h"

#include <Filterables.h>

#include <SatelliteMissionAssociatorFactory.h>
#include <SatelliteMissionAssociatorSingleton.h>

#include <ConfigurationSingleton.h>

#include <File.h>
#include <XMLIstream.h>
#include <StringUtils.h>

#include <dbQueryFactoryChain.h>
#include <dbGenQueryScopeGuard.h>

#include <dbSatelliteQuery.h>
#include <dbSatellite.h>

#include <boost/format.hpp> // PRQA S 1013
#include <grib_api.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(GRIBPreInventory)

namespace {
    GRIBPreInventoryV version;
}

const map< string, string > GRIBPreInventory::_satellitesIdMap = GRIBPreInventory::createSatellitesIdMap();

const string GRIBPreInventory::_defaultSatMissionAssociatorFactoryKey = "STANDARD";

GRIBPreInventory::GRIBPreInventory() : 
    PreInventoryBase(),
	_currentFileName(),
	_currentFileType()
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: "<< Id );
}

GRIBPreInventory::~GRIBPreInventory() throw() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Destructor called." ); // PRQA S 4631
}

string GRIBPreInventory::getClassId()
{
        //return the class name
	return "GRIBPreInventory";
}
        

/*
vector< Pair_FileName_FileType > GRIBPreInventory::preProcess( const Pair_FileName_FileType& inputFile)
{
        exCLASS_BGN_DEBUG(10)
                exFMessage::notify( "GRIBPreInventory::preProcess");
        exCLASS_END_DEBUG
	
	//Default Beaviour: the returned vector has only the inputFile passed.
        vector< Pair_FileName_FileType > filesToInventory( 1, inputFile);
        return filesToInventory;
}
*/


void GRIBPreInventory::computeMetadata( const string& pathFileName, const string& fileType, Metadata& metadata, dbConnection &conn)
{
        //NB: the pathFileName passed to this method is an absolute path
        ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	try
        {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MetadataInfo:" << "  OrderId=" << getContext().metadataInfo.orderId 
                                         << "  pathFileName= " << pathFileName << "  filetype= " << fileType );
		_currentFileName = pathFileName;
		_currentFileType = fileType;
		
		GRIBStruct gribs;
		readGRIBFile( pathFileName, gribs);

		metadata = prepareMetadata( fileType, gribs, conn);
        }
        catch( exception& ex)
        {
                ACS_THROW( exGRIBPreInventoryException( ex, "computeMetadata Exception"));
        }
}      




Metadata GRIBPreInventory::prepareMetadata( const string& fileType, const GRIBStruct& gribs, dbConnection &conn)
{
        ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);


	//filling Metadata
	Metadata metadata;

	// *UNIQUE_ID
	string uniqueId = getUniqueId();    
    if (!uniqueId.empty()) { 
        metadata.setMetadataValue( "Unique_Id", uniqueId); 
    }
	
	
	// *FILE_NAME  NB:e' il nome del file grib, cioe' il file da inventoriare;
	string fileName = fillMetadataFilename( _currentFileName, fileType, metadata, conn);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File_Name=" << fileName);

	
	// *FILE_VERSION
	int fileVersion = 1;
	metadata.setMetadataValue( "File_Version", getFormattedVersion(fileVersion));
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File_Version=\"" << getFormattedVersion(fileVersion) << '\"');


	// ORIGINATOR 
	string generatingCentre = getGeneratingCentre();
	
	if( !generatingCentre.empty()) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generating Centre got from configuration");
		metadata.setMetadataValue( "System", generatingCentre);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " System=" << generatingCentre);
		
    }

	
	
	// *SOURCE  (lo prendo dal context)
	string source = getContext().metadataInfo.processorName;
	metadata.setMetadataValue( "Source", source);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Source=" << source);

	
	// *SOURCE_SW_VERSION (lo prendo dal context)
	string sourceSwVersion = getContext().metadataInfo.processorVersion;
	metadata.setMetadataValue( "Source_Sw_Version", sourceSwVersion);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Source_Sw_Version=" << sourceSwVersion);

		
	// *GENERATION_TIME
	//es dataDate="20040723" dataTime="1200" => genTime="UTC=2004-07-23T12:00:00" 
	string generationTime = "";
	if( !gribs.DataDate.empty() && !gribs.DataTime.empty())
	{

		generationTime = "UTC=";
		generationTime += gribs.DataDate.substr( 0, 4) + "-"; //Year // PRQA S 4400 5
		generationTime += gribs.DataDate.substr( 4, 2) + "-"; //Mounth
		generationTime += gribs.DataDate.substr( 6, 2) + "T"; //Day
		generationTime += gribs.DataTime.substr( 0, 2) + ":"; //hour
		generationTime += gribs.DataTime.substr( 2, 2) + ":"; //min
		generationTime += "00";                               //sec
	
		metadata.setMetadataValue( "Generation_Time", generationTime);
	}
	else
	{
        ACS_THROW(exGRIBPreInventoryException( "Empty DataDate and DataTime!! in file " + _currentFileName)); // PRQA S 3081
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generation_Time=" << generationTime);
	
// The following code is for data  across midnight of 31 Dec w.r.t. generation time and validity start time
	unsigned short generationMonth = 0;
	{
		istringstream is(gribs.DataDate.substr( 4, 2)); // PRQA S 4400 
		is >> generationMonth;
	}
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Generation month " << generationMonth);
			
	unsigned short startValidityMonth = 0;
	{
		istringstream is(fileName.substr( 11, 2)); // PRQA S 4400 
		is >> startValidityMonth;
	}
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Start validity month " << startValidityMonth);
	
	string year = gribs.DataDate.substr( 0, 4); // PRQA S 4400 
	if( startValidityMonth < generationMonth ) {
		
		unsigned short yearInt = 0 ;
		istringstream is(year);
		is >> yearInt;
		yearInt++;
		
		ostringstream os;
		os << yearInt;
		year = os.str();
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Start validity month smaller than generation month: validity year set to " << year);
		
	}
	
	// *VALIDITY_START
	//from filename ccSMMDDHHIImmddhhiiE.grb the vlaidity start is mmddhhii
	//              01234567890123456780
	
	string declaredValidityStart = "UTC=";
	declaredValidityStart += year + "-"; 						//Year
	declaredValidityStart += fileName.substr( 11, 2) + "-";      //Mounth // PRQA S 4400 4
	declaredValidityStart += fileName.substr( 13, 2) + "T";      //Day
	declaredValidityStart += fileName.substr( 15, 2) + ":";      //hour
	declaredValidityStart += fileName.substr( 17, 2) + ":";      //min
	declaredValidityStart += "00";

	// The start time can be shifted in the past 
	// of a configurable number of hours
	DateTime startTime (declaredValidityStart);
	startTime -= getStartOffest()/24.0; // PRQA S 3011

	string validityStartString = string("UTC=") + startTime.asISO8601(0);

	metadata.setMetadataValue( "Validity_Start", validityStartString);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Validity_Start=" << validityStartString);

	// *VALIDITY_STOP (configurabile)
	//OSS: il validity stop non esiste pero' osservo 3.1 CFI40
	//Generation Frequency: 2 runs per day (22 files for each run) 
	DateTime stopTime (startTime);
    	stopTime += getDeltaValidity()/24.0; // PRQA S 3050, 3081
	string validityStopString = "UTC=" + stopTime.asISO8601(0) ;
	metadata.setMetadataValue( "Validity_Stop", validityStopString);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Validity_Stop=" << validityStopString);
	
	// GEOGRAPHIC_LOCALIZATION
	vector<Metadata::GeoPointInfo>  vGeoPntInfo;
	
	//suppongo senso antiorario a partire da A->B->C->D
	Metadata::GeoPointInfo gPntInfoA;
	gPntInfoA.latitude = gribs.LatOfFirst;
	gPntInfoA.longitude = gribs.LonOfFirst;
	
	Metadata::GeoPointInfo gPntInfoC;
	gPntInfoC.latitude = gribs.LatOfLast;
	gPntInfoC.longitude = gribs.LonOfLast;
	
	// CCDS-44 - when the polygon is bigger that 120 degrees in longitude, the box is replaced by the usage of the polygon
	// convert string to float
	double latA = atof(gPntInfoA.latitude.c_str());
	double latC = atof(gPntInfoC.latitude.c_str());
	double lonA = atof(gPntInfoA.longitude.c_str());
	double lonC = atof(gPntInfoC.longitude.c_str());
	if (abs(lonA-lonC) > 120)
	{

		Metadata::GeoPointInfo gPntInfoB;
		gPntInfoB.latitude = gPntInfoA.latitude;
		gPntInfoB.longitude = gPntInfoC.longitude;

		Metadata::GeoPointInfo gPntInfoD;
		gPntInfoD.latitude = gPntInfoC.latitude;
		gPntInfoD.longitude = gPntInfoA.longitude;
	
		double latB = atof(gPntInfoB.latitude.c_str());
		double latD = atof(gPntInfoD.latitude.c_str());
		double lonB = atof(gPntInfoB.longitude.c_str());
		double lonD = atof(gPntInfoD.longitude.c_str());

		ostringstream os;
		if (lonA>180)
			lonA -=360;
		os << lonA;
		gPntInfoA.longitude = os.str();
		vGeoPntInfo.push_back(gPntInfoA);

		vector< pair<double,double> > A_B_geo_i;
		// the sequence is A-B-C-D
		//compute additional points along longitude between A and B
		if (lonC>lonA)
		{
			for (uint i=120; i<lonC; i=i+120)
			{
				double lon_i = lonA + i;
				pair<double,double> tmp_i;
				tmp_i.first = latA;
				tmp_i.second = lon_i;
				A_B_geo_i.push_back(tmp_i);
			}
		}
		else
		{
			for (uint i=120; i>lonA; i=i-120)
			{
				double lon_i = lonA - i;
				pair<double,double> tmp_i;
				tmp_i.first = latA;
				tmp_i.second = lon_i;
				A_B_geo_i.push_back(tmp_i);
			}
		}
		
		for (uint i=0; i<A_B_geo_i.size();++i)
		{
			ostringstream oslat, oslon;
			if (A_B_geo_i[i].second>180)
				A_B_geo_i[i].second -=360;
			oslon << A_B_geo_i[i].second;
			oslat << A_B_geo_i[i].first;
			Metadata::GeoPointInfo gPntInfo_i;
			gPntInfo_i.latitude = oslat.str();
			gPntInfo_i.longitude = oslon.str();
			vGeoPntInfo.push_back(gPntInfo_i);
		}
		ostringstream osB;
		if (lonB>180)
			lonB -=360;
		osB << lonB;
		gPntInfoB.longitude = osB.str();
		vGeoPntInfo.push_back(gPntInfoB);

		if (abs(latA-latC) > 120)
		{
			Metadata::GeoPointInfo gPntInfo_i;
			gPntInfo_i.latitude = "0.0";
			ostringstream oslat, oslon;
			if (lonC>180)
				lonC -=360;
			oslon << lonC;
			gPntInfo_i.longitude = oslon.str();
			vGeoPntInfo.push_back(gPntInfo_i);
		}
		//here you have reached the point C, close the polygon
		ostringstream osC;
		if (lonC>180)
			lonC -=360;
		osC << lonC;
		gPntInfoC.longitude = osC.str();
		vGeoPntInfo.push_back(gPntInfoC);

		std::reverse(A_B_geo_i.begin(),A_B_geo_i.end()); // change sign to latitude, longitude is the same
		for (uint i=0; i<A_B_geo_i.size();++i)
		{
			ostringstream oslat, oslon;
			if (A_B_geo_i[i].second>180)
				A_B_geo_i[i].second -=360;
			oslon << A_B_geo_i[i].second;
			A_B_geo_i[i].first = latC; //change to the new latitude
			oslat << A_B_geo_i[i].first;
			Metadata::GeoPointInfo gPntInfo_i;
			gPntInfo_i.latitude = oslat.str();
			gPntInfo_i.longitude = oslon.str();
			vGeoPntInfo.push_back(gPntInfo_i);
		}
		
		ostringstream osD;
		if (lonD>180)
			lonD -=360;
		osD << lonD;
		gPntInfoD.longitude = osD.str();
		vGeoPntInfo.push_back(gPntInfoD);
		
		if (abs(latA-latC) > 120)
		{
			Metadata::GeoPointInfo gPntInfo_i;
			gPntInfo_i.latitude = "0.0";
			ostringstream oslat, oslon;
			if (lonD>180)
				lonD -=360;
			oslon << lonD;
			gPntInfo_i.longitude = oslon.str();
			vGeoPntInfo.push_back(gPntInfo_i);
		}
		
		metadata.setGeoPoints( vGeoPntInfo);
    		metadata.setGeoType( dbGeoObject::Polygon);
	}
	else
	{
		vGeoPntInfo.push_back( gPntInfoA);
		vGeoPntInfo.push_back( gPntInfoC);
	
		metadata.setGeoPoints( vGeoPntInfo);
    		metadata.setGeoType( dbGeoObject::Box);
	}

	

// TIMERANGE of Geographic_Localization
    metadata.setFieldValue( "Start_Time", validityStartString);
    metadata.setFieldValue( "Stop_Time", validityStopString);
	
	//lanciare il warning
    ACS_LOG_WARNING("GeographicLocalization: Consider a box type with the sides parallel to the meridians and parallels!");
	
	

	// *VALIDITY_FLAG
	metadata.setMetadataValue( "Validity_Flag", "true");
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Validity_Flag= true");
	

	// *VALIDATION_DATE
	string creationTime = "UTC=" + DateTime().asISO8601(6) ; 
	metadata.setMetadataValue( "Validation_Date", creationTime); 

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Validation_Date=" << creationTime); 



	// *HEADER_SIZE 
	long long unsigned int headerSize = 0;
	metadata.setMetadataValue( "Header_Size", headerSize); 
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Header_Size=" << headerSize); 
	

	// *DATA_SIZE prelevo la size reale
	long long unsigned int fileSize = File::size( _currentFileName); // PRQA S 3000
	metadata.setMetadataValue( "Data_Size", fileSize);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Data_Size=" << fileSize);


	// *FILE_TYPE
	bool getFiletypeFromFramework = getFtypeFromFramework(fileType);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"File type must " << (getFiletypeFromFramework ? "": "NOT ") << "be got from framework"); // PRQA S 3380

	string fType = fileType;
	if( !getFiletypeFromFramework ) {
		
		istringstream is( gribs.DataDate.substr( 0, 4) ); // PRQA S 4400
		unsigned short yearUs = 0 ;
		is >> yearUs;
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Going to compute filetype from filename; year=" << yearUs);
		
		fType = getFtypeFromFilename(fileName, yearUs);
    }

	// *FILE_TYPE
	metadata.setMetadataValue( "File_Type", fType);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"File_Type=" << fType);
	
	
	// *FILE_CLASS
	string fileClass = "OPER";
	metadata.setMetadataValue( "File_Class", fileClass);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"File_Class=" << fileClass);

	// *SATELLITE_ID (lo prendo dal context)
	string satelliteName = getContext().metadataInfo.satelliteName; 
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"SatelliteName=" << satelliteName);
    
    if( !satelliteName.empty()) { 
		map<string, string>::const_iterator it = _satellitesIdMap.find(StringUtils::uppercase(satelliteName));
		ACS_COND_THROW(it == _satellitesIdMap.end(), exGRIBPreInventoryException("Can't find \"" + StringUtils::uppercase(satelliteName) + "\" key in satellites map"));	 // PRQA S 3081
		string satelliteCode = it->second;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Satellite code=\"" << satelliteCode << '\"');
		
		string satFactoryKey = getSatMissionAssociatorFactoryKey( satelliteCode );
		if( satFactoryKey.empty() ) {
			satFactoryKey = _defaultSatMissionAssociatorFactoryKey;
		}
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "SatelliteMissionAssociator factory key \"" << satFactoryKey << '\"');

		SatelliteMissionAssociatorFactory factoryObject ;

		SatelliteMissionAssociator* satelliteMissionAssociator = SatelliteMissionAssociatorSingleton::instance()->newSatelliteMissionAssociator(StringUtils::uppercase(satFactoryKey));
		ACS_COND_THROW(!satelliteMissionAssociator, exGRIBPreInventoryException("Can't Instantiate \"" + satFactoryKey + "\": got NULL pointer from SatelliteMissionAssociatorFactory") );	 // PRQA S 3081

		string satelliteAcronym = satelliteMissionAssociator->getSatelliteAcronym(satelliteCode);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite acronym=\"" << satelliteAcronym << '\"');
		int satelliteId = getSatelliteIdFromSatelliteCode(satelliteAcronym, conn);
		if( satelliteId != 0 ) {
			metadata.setMetadataValue( "Satellite_Id", satelliteId);
			
			int missionNumber = satelliteMissionAssociator->getMissionId(satelliteCode);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Mission number=" << missionNumber);
			if( missionNumber != 0 ) {
				metadata.setMetadataValue( "Mission", missionNumber);
    		}

    	}

	}

	
	return metadata;
}

bool GRIBPreInventory::overrideVersionHandling(bool& doVersioning)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to check if versioning is allowed for file type \"" << _currentFileType << '\"');
	
	string localRootTag = "PreInventory.PluginsRegistrationSettings.GRIBPreInventoryFactory.List_Of_Registrations.Registration";
    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
    size_t registeredEntries = rs.getArraySize(localRootTag);

	bool found = false;
	doVersioning = false;
	for( size_t i=0; i<registeredEntries ; i++) {
		
		string tag = localRootTag;
		{
			ostringstream os ;
			os << "[" << i << "]" ;
			tag += os.str();
		}
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "parentTag=\"" << tag << '\"') ;
		
		string regFileType;
		rs.getValue( tag + ".Object", regFileType);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << " fileType=\"" << regFileType << '\"');

		if( regFileType == _currentFileType) {
			
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "fileType \"" << _currentFileType << "\" found in configuration" ) ;
			try {
				rs.getValue( tag + ".DoVersioning", doVersioning);
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "DoVersioning " << boolalpha << doVersioning) ;
			}
			catch(rsResourceSet::NotFoundException &e)
			{}
			
			found = true;
			break;

		}
	}
	
	ACS_COND_THROW(!found, exGRIBPreInventoryException("Cannot find fileType=\""+_currentFileType+"\" in configuration"));	 // PRQA S 3081
	
	return true;
	
}

void GRIBPreInventory::updateProductAndMetadata( const std::string& newVersion, Metadata& metadata)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	
	
	metadata.setMetadataValue( "File_Version", newVersion);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File_Version field updated with the value \"" << newVersion << '\"');
			
	// QUALITY_INFO
	string qualityInfo = "DUP";
	metadata.setMetadataValue( "Quality_Info", qualityInfo);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Quality_Info field set to \"" << qualityInfo << '\"');

}


string GRIBPreInventory::doPackaging( Metadata& metadata, const string & firstPath, const string & filetype, const string & secondPath, dbConnection & )
{

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	ACS_COND_THROW( !secondPath.empty(), exGRIBPreInventoryException("No package foreseen: data filename should no be valorized \"" + secondPath + "\".") ); // PRQA S 3081

	return firstPath;
}


string GRIBPreInventory::getUniqueId()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	string uID ;

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
                    ACS_LOG_WARNING("Empty Unique_Id tag in the ParametersFile \""  << pFileName << '\"');
                }

                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Unique_Id=" << uID);
            }
            else
            {
                ACS_LOG_WARNING("NOT found Unique_Id tag in the ParametersFile \""  << pFileName << '\"');
            }
        }
	}
	else if( getContext().which == Context::DIS)
	{
	//	exGRIBPreInventoryException e( "getUniqueId NOT IMPLEMENTED for DIS");
	//	ACS_THROW(e);
	}
	else if( getContext().which == Context::PROD)
	{
	//	exGRIBPreInventoryException e( "getUniqueId NOT IMPLEMENTED for PROD");
	//	ACS_THROW(e);
	}
	else
	{
		ostringstream msg;
		msg << "getUniqueId context.wich=\"" << int(getContext().which) << "\" NOT Recognaized!!"; // PRQA S 3081 2
		ACS_THROW(exGRIBPreInventoryException( msg.str()));
	}

	return uID;
	 
}


// string GRIBPreInventory::getParametersFile()
// {
// 	//apro il joborder e prelevo il parameterfile	
// 	DefaultJobOrder jo;
// 
// 	XMLIstream jofileXml( getContext().argvParameters["jobOrder"].as<string>());
//         jo << jofileXml;
// 	
// 	string parFileName = jo.getParametersFileName();
//     //it could be emnpty!
// 
// /*	if( parFileName.empty())
// 	{
// 		exGRIBPreInventoryException e( "Parameters_File.File_Name is EMPTY!");
// 		ACS_THROW(e);
// 	}*/
// 	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "parameters filename =\"" << parFileName << '\"');
// 
// 	return parFileName;
// 	
// }




void GRIBPreInventory::readGRIBFile( const string& pathFileName, GRIBStruct& gribs)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "GRIBPreInventory::readEOFFile:\""  << pathFileName << '\"');
	
	//Reading data from .GRB file
	
	FILE* in = 0;
    int err = 0;
    grib_handle* h = 0;
    in = fopen( pathFileName.c_str(), "r");

    ACS_COND_THROW( !in, exGRIBPreInventoryException( "readGRIBFile: unable to open grib file " + pathFileName)); // PRQA S 3081

    h = grib_handle_new_from_file( 0, in, &err);
    ACS_COND_THROW( h==NULL, exGRIBPreInventoryException("readGRIBFile: unable to create handle from file " + pathFileName)); // PRQA S 3081 
	

	long date = 0 ;
    long time = 0 ;
	
	//Reading  dataDate = 20040723;
    {
        GRIB_CHECK( grib_get_long( h, "dataDate", &date), 0); 
        ostringstream os;
        os << date;
	    gribs.DataDate =  os.str();
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "dataDate=\"" << gribs.DataDate << '\"');
    }
    {
	//Reading dataTime = 1200;
		GRIB_CHECK( grib_get_long( h, "dataTime", &time), 0);  
		
		boost::format timeFmt("%04d");
		timeFmt % int(time); // PRQA S 3081
    	gribs.DataTime = timeFmt.str();
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "dataTime=\"" << gribs.DataTime << '\"');

    }
    {
	    //Reading centre = 98;
	    long centre(0);
	    GRIB_CHECK( grib_get_long( h, "centre", &centre), 0);
        ostringstream os;
        os << centre;
        gribs.Centre = os.str();
    }
    {
	//Reading generatingProcessIdentifier = 123;
	    long genProces(0);
	    GRIB_CHECK( grib_get_long( h, "generatingProcessIdentifier", &genProces), 0);    
        ostringstream os;
        os << genProces;
        gribs.GenProcessIdentifier = os.str();
    }

// experimentVersionNumber is no used and moreover it may not occur (S1PDGS-31532). Remove next version	
// 	//Reading experimentVersionNumber = 0001;
// 	size_t len = 5;
// 	char expVer[len];
// 	
// 	GRIB_CHECK( grib_get_string( h, "experimentVersionNumber", expVer, &len), 0);
//     gribs.ExperimVerNumber = string(expVer); // PRQA S 3081

/*
latitudeOfFirstGridPointInDegrees = 90;
longitudeOfFirstGridPointInDegrees = 0;
latitudeOfLastGridPointInDegrees = -90;
longitudeOfLastGridPointInDegrees = 359.875;
*/
	double latFirst = 0 ;
  	double lonFirst = 0 ;
  	double latLast = 0 ;
  	double lonLast = 0 ;
	
 	GRIB_CHECK( grib_get_double( h, "latitudeOfFirstGridPointInDegrees", &latFirst), 0);
  	GRIB_CHECK( grib_get_double( h, "longitudeOfFirstGridPointInDegrees", &lonFirst), 0);
  	GRIB_CHECK( grib_get_double( h, "latitudeOfLastGridPointInDegrees", &latLast), 0);
  	GRIB_CHECK( grib_get_double( h, "longitudeOfLastGridPointInDegrees", &lonLast), 0);
    
    ostringstream os1;
    os1 << latFirst;
	gribs.LatOfFirst = os1.str();

    ostringstream os2;
    os2 << lonFirst;
	gribs.LonOfFirst = os2.str();

    ostringstream os3;
    os3 << latLast;
	gribs.LatOfLast = os3.str();

    ostringstream os4;
    os4 << lonLast;
	gribs.LonOfLast = os4.str();

    grib_handle_delete(h);
    fclose(in);

	        		
}


int GRIBPreInventory::getSatelliteIdFromSatelliteCode( const string& satCode, dbConnection &conn)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

    const string &cKey = conn.getKey();
	dbSatelliteQuery * satelliteQuery=dynamic_cast<dbSatelliteQuery*>(dbQueryFactoryChain::instance()->newObject("dbSatelliteQuery", cKey)); // PRQA S 3081, 4412
	if( !satelliteQuery)
	{
		
		ACS_THROW(exGRIBPreInventoryException( "Error in executing dbSatelliteQuery")); // PRQA S 3081
	}
	dbGenQueryScopeGuard satelliteQueryGuard( satelliteQuery);
	dbQueryParameters satParameters(cKey); 
	satParameters.addParameter( dbParam(cKey, "t_satellites", "satelliteacronym", dbParam::EQUAL_TO, db::toSQL(cKey, satCode))); // PRQA S 3050, 3081
	satelliteQuery->addParameter( satParameters);
	unsigned long num = satelliteQuery->doQuery(conn);

	int satId = 0;  //0<=>false
	if (num > 0) {
		satId = ((*satelliteQuery)[0])->getSatelliteId();
    }

	return satId;
}


double GRIBPreInventory::getDeltaValidity()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

	// Default is 24 hours
	double delta = 24 ;

    //read the configuration file from database
    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
    //get the number of files that can be inventoried
    static const string tag ("PreInventory.ConfigGRIBPreInventory.DeltaValidity");
	
	rs.getValue( tag, delta, false); // Not mandatory in configuration

	return delta;
}

string GRIBPreInventory::getGeneratingCentre()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

	string centre("");

    //read the configuration file from database
    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
    //get the number of files that can be inventoried
    static const string tag ("PreInventory.ConfigGRIBPreInventory.GeneratingCentre");
	
	rs.getValue( tag, centre, false); // Not mandatory in configuration

	return centre;
}
double GRIBPreInventory::getStartOffest()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

	// Default is 24 hours
	double delta = 0 ;

    //read the configuration file from database
    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
    //get the number of files that can be inventoried
    static const string tag ("PreInventory.ConfigGRIBPreInventory.ValidityStartOffset");
	
	rs.getValue( tag, delta, false); // Not mandatory in configuration

	return delta;
}

string GRIBPreInventory::getFtypeFromFilename(const string & fileName, unsigned short year) const
{

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called");
	string gribType = fileName.substr( 1, 1);

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Grib type \"" << gribType << '\"');
	
	string firstDateFromName = fileName.substr(3, 8);
	string secondDateFromName = fileName.substr(11, 8);
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "First date from name \"" << firstDateFromName << "\", second date from name \"" << secondDateFromName << '\"');
	
    string filetype;
	if( firstDateFromName == secondDateFromName ) {
		 filetype = "ECMWF_MA" + gribType;
	}
	else {
		unsigned short firstMonth = 0 ;
		{
			istringstream is( firstDateFromName.substr(0, 2) ); // PRQA S 4400
			is >> firstMonth;
		}
		unsigned short firstDay = 0 ;
		{
			istringstream is( firstDateFromName.substr(2, 2) ); // PRQA S 4400
			is >> firstDay;
		}
		int firstHour = 0 ;
		{
			istringstream is( firstDateFromName.substr(4, 2) ); // PRQA S 4400
			is >> firstHour;
		}
		int firstMin = 0 ;
		{
			istringstream is( firstDateFromName.substr(6, 2) ); // PRQA S 4400
			is >> firstMin;
		}
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Month, day, hour, min extracted from the first date: " << firstMonth << ", " 
                                                << firstDay << ", " << firstHour<< ", " << firstMin);

// minutes from year beginning extracted from the first date				
		int firstMinutesOfYear = (DateTime::UTCD::dayOfYear(firstDay, firstMonth, year) - 1) * (24 * 60) + firstHour * 60 + firstMin;
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Minutes from year beginning extracted from the first date " << firstMinutesOfYear);

		unsigned short secondMonth = 0 ;
		{
			istringstream is( secondDateFromName.substr(0, 2) ); // PRQA S 4400
			is >> secondMonth;
		}
		unsigned short secondDay = 0 ;
		{
			istringstream is( secondDateFromName.substr(2, 2) ); // PRQA S 4400
			is >> secondDay;
		}
		int secondHour = 0 ;
		{
			istringstream is( secondDateFromName.substr(4, 2) ); // PRQA S 4400
			is >> secondHour;
		}
		int secondMin = 0 ;
		{
			istringstream is( secondDateFromName.substr(6, 2) ); // PRQA S 4400
			is >> secondMin;
		}
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Month, day, hour, min extracted from the second date " << secondMonth << ", " 
                                               << secondDay << ", " << secondHour << ", " << secondMin);

// minutes from year beginning extracted from the second date				
		int secondMinutesOfYear = (DateTime::UTCD::dayOfYear(secondDay, secondMonth, year) - 1) * (24 * 60) + secondHour * 60 + secondMin;
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Minutes from year beginning extracted from the second date " << secondMinutesOfYear);

		if( secondMinutesOfYear < firstMinutesOfYear ) {
// going over midnight of 31 Dec
			secondMinutesOfYear += DateTime::UTCD::dayOfYear(31, 12, year) * (24 * 60); // PRQA S 4400
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Going over midnight of year " << year << "; minutes from year beginning extracted from the second date recomputed: " << secondMinutesOfYear);
		}
		
		int deltaMinutes = secondMinutesOfYear - firstMinutesOfYear;
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Minutes delta between dates is " << deltaMinutes << " ( " << double(deltaMinutes/60.) << " hours )"); // PRQA S 3011, 3081
		
        switch(deltaMinutes) {
			case (60*12):	// 12h: // PRQA S 4402 L1
			case (60*6):	// 6h
            {
				filetype = "ECMWF_MFA";
			    break;
            }
			case (60*9): 	// 9h
			case (60*15): 	// 15h
			case (60*21): 	// 21h
			case (60*27): 	// 27h  // PRQA L:L1
            {
				filetype = "ECMWF_MF" + gribType ;
			    break;
            }
			default:
            {
				ostringstream exMsg;
				exMsg << "Unexpected delta between dates: " << deltaMinutes << " minutes";
            	ACS_THROW( exGRIBPreInventoryException (exMsg.str()) ); // PRQA S 3081
			    break;
            }
		}
		
	}
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Returning filetype \"" << filetype << '\"');

	return filetype;
}


bool GRIBPreInventory::getFtypeFromFramework(const string& currentFileType) const // PRQA S 4214
{
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Method called")
			
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Going to check if filetype must be got from framework for file type \"" << currentFileType << '\"');

	
    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
    size_t registeredEntries = 0;
	rs.getValue( "PreInventory.PluginsRegistrationSettings.GRIBPreInventoryFactory.List_Of_Registrations.count.value", registeredEntries);

	bool found = false;
	bool FtypeFromFramework = true;
	for( size_t i=0; i<registeredEntries ; i++) {
		ostringstream osTag;
		osTag << "PreInventory.PluginsRegistrationSettings.GRIBPreInventoryFactory.List_Of_Registrations.Registration[" << i << "]";
		string tag = osTag.str();
		string regFileType;
		rs.getValue( tag + ".Object", regFileType);
		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "TAG=" << tag << " fileType=\"" << regFileType << '\"');

		if( regFileType == currentFileType) {
			
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "File type \"" << currentFileType << "\" found in configuration");
			try {
				rs.getValue( tag + ".FiletypeFromFramework", FtypeFromFramework);
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "FiletypeFromFramework " << boolalpha << FtypeFromFramework );
			}
			catch(rsResourceSet::NotFoundException &e)
			{}

			found = true;
			break;
		}
	}
	
	ACS_COND_THROW(!found, exGRIBPreInventoryException("Cannot find fileType=\""+currentFileType+"\" in configuration"));	 // PRQA S 3081
	
	return FtypeFromFramework;
	
}

string GRIBPreInventory::getSatMissionAssociatorFactoryKey( const string& satCode)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

	string key;
    //read the configuration file from database
    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	string localRootTag = "PreInventory.ConfigGRIBPreInventory.SatMissionsAssociatorKeys.Pair";
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
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "satellite code \"" << satCode << "\" not found in config file!!! Returning \"" << key << '\"');
	}
		
	return key;
}


_ACS_END_NAMESPACE
