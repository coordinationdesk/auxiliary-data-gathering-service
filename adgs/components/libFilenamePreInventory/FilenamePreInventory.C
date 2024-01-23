// PRQA S 1050 EOF
/*
	Copyright 2013-2022, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: libFilenamePreInventory $

	$Id$

	$Author$

	$Log$
	Revision 1.18  2016/09/20 09:38:42  paocas
	added a default for endOfMission
	
	Revision 1.17  2016/05/18 08:31:42  nicvac
	Unique id is passed in parameter file only in DS case. Also protected to be overridden by subclasses.
	
	Revision 1.16  2016/05/17 15:09:55  massimo.ricci
	Force to set mission number if it is empty from preinventory parsing
	
	Revision 1.15  2016/04/15 08:13:54  lucio.pulvirenti
	S3MPM-695: getBeginOfMission and getEndOfMission methods added to get BeginOfMission and EndOfMission from configuration if start/stop time not occurring in product name.
	
	Revision 1.14  2016/04/04 15:30:02  francesco.avanzi
	minor issue involving date changed
	
	Revision 1.13  2016/03/26 21:23:00  marpas
	using new db I/F
	
	Revision 1.12  2016/03/11 09:10:33  tergem
	DefaultJobOrder dependency removed. Parameter File read from context
	
	Revision 1.11  2015/07/20 09:36:59  lucio.pulvirenti
	S3MPM-549: getUniqueId: DS context now managed.
	
	Revision 1.10  2015/06/19 10:02:25  lucio.pulvirenti
	S3MPM-528: If FiletypeFromPlugin tag occurring in configuration and is not valorized. take filetype from FilenameElements class.
	
	Revision 1.9  2015/06/05 13:53:24  lucio.pulvirenti
	S3MPM-504: getPluginConfSpaceId method introduced to return plugin node in configuration space.
	
	Revision 1.8  2015/03/19 11:38:20  lucio.pulvirenti
	S3MPM-359: Base class getSatMissionAssociatorFactoryKey used to check if SatMissionsAssociatorKeys tag occurs in configuration. In this case, satellite and mission values
	used are the one returnde by the proper factory object.
	
	Revision 1.7  2015/01/21 14:22:04  lucio.pulvirenti
	S3GEOCAL-11: System tag in metadata filled if fe.GeneratingCentre not empty only.
	
	Revision 1.6  2015/01/15 16:33:49  lucio.pulvirenti
	S3GEOCAL-11: FileClass from filename taken only if not configured.
	
	Revision 1.5  2014/03/12 11:13:34  lucio.pulvirenti
	New debug macros used.
	getSatelliteId virtual method used to be re-implemented by derived classes if needed.
	If FilenameElements::orbitnumber not empty, write it into metadata.
	getProductFilenameInInventoryQueryParameters now get connection as parameter.
	
	Revision 1.4  2014/03/11 17:02:14  lucio.pulvirenti
	Work in progress.
	
	Revision 1.3  2014/03/10 16:49:07  lucio.pulvirenti
	Work in progress.
	
	Revision 1.2  2013/09/20 11:40:40  lucio.pulvirenti
	GeoCoords vector added to FilenameElements struct;
	doVersioning method added.
	If GeoCoords elements valorized, metadata properly filled.
	
	Revision 1.1.1.1  2013/09/19 17:39:45  lucio.pulvirenti
	Import libFilenamePreInventory
	
        
*/ 

#include <FilenamePreInventory.h>
#include "FilenamePreInventoryV.h"

#include <SatelliteMissionAssociatorFactory.h>
#include <SatelliteMissionAssociatorSingleton.h>

#include <ConfigurationSingleton.h>

#include <XMLIstream.h>

#include <dbQueryFactoryChain.h>
#include <dbGenQueryScopeGuard.h>
#include <dbInventoryObjectQuery.h>
#include <dbInventoryObject.h>

#include <StringUtils.h>
#include <Filterables.h>
#include <boost/format.hpp>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(FilenamePreInventory)

namespace {
    FilenamePreInventoryV version;
	const string pluginConfSpaceId = "ConfigFilenamePreInventory";
	const string beginOfMissionTag = "BeginOfMission";
	const string endOfMissionTag = "EndOfMission";
}


FilenamePreInventory::FilenamePreInventory() : 
    PreInventoryBase(),
	_currentFileName(),
	_currentFileType(),
	_configurationRootTag(),
	_ftypeParentTag()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

FilenamePreInventory::~FilenamePreInventory() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}


void FilenamePreInventory::computeMetadata( const string& pathFileName, const string& fileType, Metadata& metadata, dbConnection &conn)
{
    //NB: the pathFileName passed to this method is an absolute path
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	
	try {
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MetadataInfo:\n" 
			<< " OrderId=" << getContext().metadataInfo.orderId << '\n'
			<< "  pathFileName= " << pathFileName << '\n'
			<< "  filetype from framework= " << fileType);

		_currentFileName = pathFileName;
		_currentFileType = fileType;
		_ftypeParentTag=""; // needed if more than one file type occur on the same joborder (e.g. TLPreInventory )

		FilenameElements fne;
		parseFilename(pathFileName, fileType, fne);
		
		metadata = prepareMetadata( fileType, fne, conn);
	}
    catch( exception& ex)
    {
        exFilenamePreInventoryException e( ex, "computeMetadata Exception");
        throwException( e);
    }
}      



Metadata FilenamePreInventory::prepareMetadata( const string& fileType, const FilenameElements &fe, dbConnection &conn)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);

	//filling Metadata
	Metadata metadata;

	// *UNIQUE_ID
	string uniqueId = getUniqueId();
    if ( !uniqueId.empty()) {
        metadata.setMetadataValue( "Unique_Id", uniqueId); 
    }
	
	string fileName = fillMetadataFilename( _currentFileName, fileType, metadata, conn);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File will be inventoried with the name \"" << fileName << "\"");
	
	
	string satelliteCode = fe.Satellite;
	if( not fe.Satellite.empty() ) {
        if(fe.Mission.empty())
        {
            const string parentTag("PreInventory."+pluginConfSpaceId);
            string satFactoryKey = getSatMissionAssociatorFactoryKey( fe.Satellite, parentTag);
            if( not satFactoryKey.empty() ) {
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "SatelliteMissionAssociator factory key \"" << satFactoryKey << "\"");

                SatelliteMissionAssociatorFactory factoryObject ;

                SatelliteMissionAssociator* satelliteMissionAssociator = SatelliteMissionAssociatorSingleton::instance()->newSatelliteMissionAssociator(StringUtils::uppercase(satFactoryKey));
                throwConditionedException(!satelliteMissionAssociator, exFilenamePreInventoryException("Can't Instantiate \"" + satFactoryKey + "\": got NULL pointer from SatelliteMissionAssociatorFactory") );	 // PRQA S 3081

                // MISSION 
                int missionNumber = satelliteMissionAssociator->getMissionId(fe.Satellite);
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Mission number=" << missionNumber);
                if( missionNumber != 0 ) {
                    metadata.setMetadataValue( "Mission", missionNumber);
                }

                satelliteCode = satelliteMissionAssociator->getSatelliteAcronym(fe.Satellite);
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite code=\"" << satelliteCode << "\"");

            }
        }
        else
        {
            ACS_LOG_INFO("Set mission number from FilenameElements field '" << fe.Mission << "'");
            metadata.setMetadataValue( "Mission", fe.Mission);
        }
	}

	short int satelliteId = getSatelliteId(satelliteCode, conn);
	if( 0 != satelliteId ) {
		metadata.setMetadataValue( "Satellite_Id", satelliteId);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite_Id=" << satelliteId);
	}


		
	// *FILE_VERSION: if already valorized it is already formatted,
	string fileVersion = fe.File_Version;
	if( fileVersion.empty() ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File version not valorized");
		int intVersion = 1;
		fileVersion = getFormattedVersion(intVersion);
	}
	metadata.setMetadataValue( "File_Version", fileVersion);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File_Version=\"" << fileVersion << "\"");

	// *SOURCE
	string source = getContext().metadataInfo.processorName;
	if( !source.empty() ) {
		metadata.setMetadataValue( "Source", source);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Source=" << source );
	}
	
	// *SOURCE_SW_VERSION 
	string sourceSwVersion = getContext().metadataInfo.processorVersion;
	if( !sourceSwVersion.empty() ) {
		metadata.setMetadataValue( "Source_Sw_Version", sourceSwVersion);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Source_Sw_Version=" << sourceSwVersion );
	}

	// *GENERATION_TIME
	{
		string tmpDate = "UTC=" + fe.CreationDate;
		metadata.setMetadataValue( "Generation_Time", tmpDate);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generation_Time=" << tmpDate);
	}

	// *VALIDITY_START
	{
		string tmpDate = "UTC=" + fe.StartDate;
		metadata.setMetadataValue( "Validity_Start", tmpDate);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity_Start=" << tmpDate);
	}

	// *VALIDITY_STOP
	{
		string tmpDate = "UTC=" + fe.StopDate;
		metadata.setMetadataValue( "Validity_Stop", tmpDate);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity_Stop=" << tmpDate);
	}
	
	// *VALIDITY_FLAG
	metadata.setMetadataValue( "Validity_Flag", "true");
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Validity_Flag= true");
	

	// *VALIDATION_DATE
	{
		DateTime dt;            //today datetime
		ostringstream strTime;
		strTime << "UTC=";
		dt.asISO8601( strTime, 6);
		string valTime = strTime.str();
		metadata.setMetadataValue( "Validation_Date", valTime); 
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB," Validation_Date=" << valTime);
	}

	// *HEADER_SIZE 
	off_t headerSize = 0;
	metadata.setMetadataValue( "Header_Size", headerSize); 
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Header_Size=" << headerSize );

	// *DATA_SIZE prelevo la size reale
	off_t fileSize = File::size( _currentFileName);
	metadata.setMetadataValue( "Data_Size", fileSize);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Data_Size=" << fileSize );
		
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	// *FILE_TYPE
	string fType;
	if( findConfigurationValue(fileType, rs, "FiletypeFromPlugin", fType) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File type must NOT be got from framework");
		if( fType.empty() ) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FiletypeFromPlugin tag is empty: taking from FilenameElements class");
			fType = fe.File_Type;
		}
    }
	else {
		fType = fileType;
	}
	metadata.setMetadataValue( "File_Type", fType);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " File_Type=" << fType);

	// *FILE_CLASS
	string fileClass;
	if( findConfigurationValue(fileType, rs, "FileClass", fileClass) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " File_Class got from configuration: " << fileClass);
	}
	else {
		fileClass = fe.ClassId;
	}
	ACS_COND_THROW( fileClass.empty(), exFilenamePreInventoryException("File class neither occurring in product name nor configured!") );
	metadata.setMetadataValue( "File_Class", fileClass);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " File_Class=" << fileClass );
	
	if( not fe.GeneratingCentre.empty() ) {
		metadata.setMetadataValue( "System", fe.GeneratingCentre);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " System=" <<  fe.GeneratingCentre );
	}
	

	if( !fe.geoCoords.empty() ) {
#warning "extend for more than four values."
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to fille geographic localization section.");
		vector<Metadata::GeoPointInfo>  vGeoPntInfo;

		Metadata::GeoPointInfo gPntInfoA;
		gPntInfoA.latitude = fe.geoCoords[0];
		gPntInfoA.longitude = fe.geoCoords[1];

		Metadata::GeoPointInfo gPntInfoB;
		gPntInfoB.latitude = fe.geoCoords[2];
		gPntInfoB.longitude = fe.geoCoords[3];


		vGeoPntInfo.push_back( gPntInfoA);
		vGeoPntInfo.push_back( gPntInfoB);

		metadata.setGeoPoints( vGeoPntInfo);
        metadata.setGeoType( dbGeoObject::Box);
		
		// TIMERANGE of Geographic_Localization
		{
			string tmpDate = "UTC=" + fe.StartDate;
			metadata.setFieldValue( "Start_Time", tmpDate);
		}

		{
			string tmpDate = "UTC=" + fe.StopDate;
			metadata.setMetadataValue( "Stop_Time", tmpDate);
		}

	}
	
	if( !fe.OrbitNumber.empty() ) {
		int orbitNumber;
		istringstream is(fe.OrbitNumber);
		is >> orbitNumber;
		metadata.setMetadataValue( "Start_Orbit_Number", orbitNumber);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start_Orbit_Number=" <<  orbitNumber );
	}
	
	setFurther( metadata, conn );
	
	return metadata;
}


void FilenamePreInventory::updateProductAndMetadata( const std::string& newVersion, Metadata& metadata)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	
	
	metadata.setMetadataValue( "File_Version", newVersion);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File_Version field updated with the value \"" << newVersion << "\"");
			
	// QUALITY_INFO
	string qualityInfo = "DUP";
	metadata.setMetadataValue( "Quality_Info", qualityInfo);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Quality_Info field set to \"" << qualityInfo << "\"");
			
}

string FilenamePreInventory::doPackaging( Metadata& metadata, const string & firstPath, const string & filetype, const string & secondPath, dbConnection & )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	
			
	throwConditionedException( !secondPath.empty(), exFilenamePreInventoryException("No package foreseen: data filename should no be valorized \"" + secondPath + "\".") );

	return firstPath;
}

//Default implementation. Unique Id is passed in parameter file only in Data storage case.
string FilenamePreInventory::getUniqueId()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	string uID;

	if( getContext().which == Context::DS)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Context is " << getContext().which );
		string pFileName = getContext().parameterFile;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ParametersFile \""  << pFileName << "\"");
        if ( ! pFileName.empty() )
        {

            rsResourceSet imXML;
                XMLIstream xml( pFileName);
                xml >> imXML;

            
            string tag = "Unique_Id";
            if( imXML.hasKey( tag))
            {
                imXML.getValue( tag, uID);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Unique_Id=\"" << uID << "\"");
            }
            else
            {
                ACS_THROW(exFilenamePreInventoryException("NOT found Unique_Id tag in the ParametersFile \"" + pFileName + "\""));
            }
        }
	}
	else if(getContext().which == Context::TL) {
		ACS_LOG_WARNING("getUniqueId NOT IMPLEMENTED for TL");
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
		msg << "getUniqueId context.which=\"" << getContext().which << "\" NOT Recognized!!";
		throwException( exFilenamePreInventoryException(msg.str()) );
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to return \"" << uID << "\"");

	return uID;
	 
}


// string FilenamePreInventory::getParametersFile()
// {
// 	//apro il joborder e prelevo il parameterfile	
// 	DefaultJobOrder jo;
// 
// 	XMLIstream jofileXml( getContext().argvParameters["jobOrder"].as<string>());
//         jo << jofileXml;
// 	
// 	string parFileName = jo.getParametersFileName();
// 	if( parFileName.empty()) {
// 		//just notify.			
// 		ACS_LOG_WARNING("ParametersFile not specified!");
// 	}
// 	else {
// 		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Parameters filename =\"" << parFileName << "\"");
// 	}
// 
// 	return parFileName;
// 	
// }

bool FilenamePreInventory::overrideVersionHandling(bool& doVersioning)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to check if versioning is allowed for file type \"" << _currentFileType << "\"");
	
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	if( !findConfigurationValue(_currentFileType, rs, "DoVersioning", doVersioning) ) {
		throwException( exFilenamePreInventoryException("No DoVersioning tag in configuration!") );
    }
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "DoVersioning " << boolalpha << doVersioning) ;
	
	return true;	
}

void FilenamePreInventory::setFtypeParentTag(const string & filetype, const rsResourceSet & rs )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	
	throwConditionedException(_configurationRootTag.empty(), exFilenamePreInventoryException("Root configuration tag not valorized."));	 // PRQA S 3081
	
	string localRootTag = _configurationRootTag + ".List_Of_Registrations.Registration";
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Root tag=\"" << localRootTag << "\"") ;
	size_t registeredEntries = rs.getArraySize(localRootTag);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Registration entries=" << registeredEntries) ;
			
	bool found = false;
	for( size_t i=0; i<registeredEntries ; i++) {
		
		string tag = localRootTag;
		{
			ostringstream os ;
			os << "[" << i << "]" ;
			tag += os.str();
		}
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "parentTag=\"" << tag << "\"") ;
		string regFileType;
		rs.getValue( tag + ".Object", regFileType);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << " fileType=\"" << regFileType << "\"");

		if( filetype == regFileType) {
			
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File type \"" << filetype << "\" found in configuration");
			_ftypeParentTag = tag ;
			found = true;
			break;
		}
	}
	
	throwConditionedException(!found, exFilenamePreInventoryException("Cannot find fileType=\""+filetype+"\" in configuration"));	 // PRQA S 3081
}
		

string FilenamePreInventory::getFtypeParentTag(const string & filetype, const rsResourceSet & rs)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	if( _ftypeParentTag.empty() ) {
		setFtypeParentTag(filetype, rs);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, filetype << " filetype parent tag just set: \"" << _ftypeParentTag << "\"") ;
	}
	return _ftypeParentTag;
	
}


bool FilenamePreInventory::doVersioning(const Metadata& metadata, std::string& newVersion, dbConnection &conn)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);

    bool isTheVersionChanged(false);

    //Execute Query
    dbInventoryObjectQuery* dbInvObjectQuery =dynamic_cast<dbInventoryObjectQuery*>( dbQueryFactoryChain::instance()->newObject("dbInventoryObjectQuery", conn.getKey()) ); // PRQA S 3081, 4412
	throwConditionedException( !dbInvObjectQuery, exFilenamePreInventoryException("Error in executing dbSensorQuery") );	 // PRQA S 3081
	dbGenQueryScopeGuard stationQueryGuard(dbInvObjectQuery);
	
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

//**** The only case of decimal version managed so far is xx.x ****		
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
					throwException( exFilenamePreInventoryException("Decimal version trasformation with size != 3 still not implemented") );
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
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "New version string \"" << newVersion << "\"");
			
			isTheVersionChanged=true;
		}
		else {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No t_inventory entry with filename \"" << metadata.getGlobalFileName() << "\" has version \"" << currentVersion
					<< "\": no new Version required.");
		}

    } else {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No t_inventory entry with filename \"" << metadata.getGlobalFileName() << "\" occurs in db: no new Version required." ); 
    }

    return isTheVersionChanged;
}


short int FilenamePreInventory::getSatelliteId(const string& satellite, dbConnection &)
{
	return 0;
}

string FilenamePreInventory::getPluginConfSpaceId() const
{
	return pluginConfSpaceId;
}


void FilenamePreInventory::getBeginOfMission(const rsResourceSet & rs , string & beginOfMission) 
{
	string tag = "PreInventory." + pluginConfSpaceId + '.' + beginOfMissionTag;
	rs.getValue( tag, beginOfMission);
}
	
void FilenamePreInventory::getEndOfMission(const rsResourceSet & rs , string & endOfMission )
{
	string tag = "PreInventory." +pluginConfSpaceId + '.' + endOfMissionTag;
	if(rs.hasKey(tag))
	{
		rs.getValue( tag, endOfMission);
	}
	else
	{
		ACS_LOG_WARNING("no endOfMission Tag found in confSpace, using default: " << endOfMission )
		endOfMission = "20991231T235959" ;
	}
}

void FilenamePreInventory::checkFieldLenWithinFilename(const size_t startPos, const size_t fieldLength, const size_t filenameLength, const string & field )
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Field name=\"" << field << "\" start position=" << startPos << " field length=" << fieldLength << " filename length=" << filenameLength );
	if( ( startPos + fieldLength ) > filenameLength ) {
		ostringstream exMsg ;
		exMsg << '\"' << field << "\" field length " << fieldLength << " from position " << startPos << " exceeds filename length: " << filenameLength ;
		ACS_THROW( exFilenamePreInventoryException( exMsg.str() ) ) ; 
	}
}


FilenamePreInventory::FilenameElements::FilenameElements(FilenamePreInventory::FilenameElements const &other):
								Satellite(other.Satellite) ,
								Mission(other.Mission) ,
								ClassId(other.ClassId) ,
								File_Type(other.File_Type) ,
								StartDate(other.StartDate),							
								StopDate(other.StopDate),
								File_Version(other.File_Version),
								GeneratingCentre(other.GeneratingCentre),
								CreationDate(other.CreationDate),
								OrbitNumber(other.OrbitNumber),
								geoCoords(other.geoCoords),
								additionalInfo(other.additionalInfo)
{
}								

FilenamePreInventory::FilenameElements & FilenamePreInventory::FilenameElements::operator = (FilenamePreInventory::FilenameElements const &other)
{
	if( this != &other ) {
		Satellite=other.Satellite;
		Mission=other.Mission;
		ClassId=other.ClassId;
		File_Type=other.File_Type;
		StartDate=other.StartDate;							
		StopDate=other.StopDate;
		File_Version=other.File_Version;
		GeneratingCentre=other.GeneratingCentre;
		CreationDate=other.CreationDate;
		OrbitNumber=other.OrbitNumber;
		geoCoords=other.geoCoords;
		additionalInfo=other.additionalInfo;
	}
		
	return (*this);
}								


_ACS_END_NAMESPACE
