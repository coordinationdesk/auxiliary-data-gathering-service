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

	$Prod: libPreInventoryBase $

	$Id$

	$Author$

	$Log$
	Revision 1.7  2016/04/11 13:35:22  marpas
	adopting new db interfaces
	
	Revision 1.6  2016/03/09 12:10:07  marpas
	coding best practices applied
	
	Revision 1.5  2015/07/17 15:25:19  lucio.pulvirenti
	S3MPM-549: DS context added.
	
	Revision 1.4  2015/05/25 14:16:05  marpas
	coding best practice applied
	
	Revision 1.3  2015/04/28 13:49:31  marpas
	static string could be deleted twice.
	
	Revision 1.2  2015/03/05 10:41:45  marpas
	removed ACSTimeConv dependency
	added DefaultJobOrder dependency
	
	Revision 1.1  2015/03/04 10:48:18  marpas
	importing PDGSExtDataPreInventory class from libPDGSExtDataPreInventory
	
	Revision 1.8  2014/06/04 14:40:52  matteo.airoldi
	Code aligned to last version of libDateTime and libAcsTimeConv
	
	Revision 1.7  2014/03/26 15:33:59  lucio.pulvirenti
	Static const maxStopValidityTime added with corrispondent method.
	
	Revision 1.6  2014/03/25 16:13:40  lucio.pulvirenti
	convertGpsTime: format now given in input.
	
	Revision 1.5  2014/03/21 16:51:12  lucio.pulvirenti
	getProductFilenameInInventoryQueryParameters now get connection as parameter.
	
	Revision 1.4  2014/03/21 16:04:34  lucio.pulvirenti
	convertGpsTimeToUTC method moved to base class.
	
	Revision 1.3  2014/03/17 11:39:41  lucio.pulvirenti
	filleMetadataFilenamePED method introduced in IF to let re-definition of file names into base class.
	Generating Centre got by base class by configuration.
	
	Revision 1.2  2014/03/14 16:41:11  lucio.pulvirenti
	Work in progress.
	
	Revision 1.1.1.1  2014/03/14 12:22:32  lucio.pulvirenti
	Import libPDGSExtDataPreInventory
	
	
        
*/ 

#include <PDGSExtDataPreInventory.h>
#include <Filterables.h>

#include <ConfigurationSingleton.h>

#include <XMLIstream.h>
#include <DefaultJobOrder.h>

#include <dbQueryFactoryChain.h>
#include <dbGenQueryScopeGuard.h>
#include <dbInventoryObjectQuery.h>
#include <dbInventoryObject.h>

#include <StringUtils.h>
#include <boost/format.hpp> // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(PDGSExtDataPreInventory)


string PDGSExtDataPreInventory::getMaxStopValidityTime() 
{
    return "99991231T235959";
}

PDGSExtDataPreInventory::PDGSExtDataPreInventory() : 
    PreInventoryBase(),
	_currentFileName(),
	_currentFileType(),
	_configurationRootTag(),
	_ftypeParentTag(),
	_filenameId()
{
}

PDGSExtDataPreInventory::~PDGSExtDataPreInventory() throw() 
{
}


void PDGSExtDataPreInventory::computeMetadata( const string& pathFileName, const string& fileType, Metadata& metadata, dbConnection &conn)
{
    //NB: the pathFileName passed to this method is an absolute path
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	try {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MetadataInfo:\n" 
			<< " OrderId=" << getContext().metadataInfo.orderId << '\n'
			<< "  pathFileName= " << pathFileName << '\n'
			<< "  filetype from framework= " << fileType);

		_currentFileName = pathFileName;
		_currentFileType = fileType;
		
		// check on name
		rsResourceSet& rs = ConfigurationSingleton::instance()->get();
		if( findConfigurationValue(fileType, rs, "FilenameIdentifier", _filenameId) ) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FilenameIdentifier found in configuration \"" << _filenameId << "\"");
    	}

		FileInfo fileInfo;
		extractFileInfo(pathFileName, fileType, conn, fileInfo);
		
		metadata = prepareMetadata( fileType, fileInfo, conn);
	}
    catch( exception& ex)
    {
        ACS_THROW(exPDGSExtDataPreInventoryException( ex, "computeMetadata Exception"));
    }
}      


string PDGSExtDataPreInventory::fillMetadataFilenamePED( const string &filename,  const std::string &fileType,  const FileInfo &, Metadata &metadata , dbConnection &conn)
{
	return fillMetadataFilename( filename, fileType, metadata, conn);
}


Metadata PDGSExtDataPreInventory::prepareMetadata( const string& fileType, const FileInfo &fileInfo, dbConnection &conn)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	//filling Metadata
	Metadata metadata;

	// *UNIQUE_ID
	string uniqueId = getUniqueId();
    if ( !uniqueId.empty()) {
        metadata.setMetadataValue( "Unique_Id", uniqueId); 
    }
	
	string fileName = fillMetadataFilenamePED( _currentFileName, fileType, fileInfo, metadata, conn);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File will be inventoried with the name \"" << fileName << "\"");
	
	if( 0 != fileInfo.SatelliteId ) {
		metadata.setMetadataValue( "Satellite_Id", fileInfo.SatelliteId);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite_Id" << fileInfo.SatelliteId);
		
	}
	
	if( 0 != fileInfo.Mission ) {
		metadata.setMetadataValue( "Mission", fileInfo.Mission);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Mission=" << fileInfo.Mission);
	}
	
	// *FILE_VERSION: if already valorized it is already formatted,
	string fileVersion = fileInfo.File_Version;
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
		string tmpDate = "UTC=" + fileInfo.CreationDate;
		metadata.setMetadataValue( "Generation_Time", tmpDate);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generation_Time=" << tmpDate);
	}

	// *VALIDITY_START
	{
		string tmpDate = "UTC=" + fileInfo.StartDate;
		metadata.setMetadataValue( "Validity_Start", tmpDate);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity_Start=" << tmpDate);
	}

	// *VALIDITY_STOP
	{
		string tmpDate = "UTC=" + fileInfo.StopDate;
		metadata.setMetadataValue( "Validity_Stop", tmpDate);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity_Stop=" << tmpDate);
	}
	
	// *VALIDITY_FLAG
	metadata.setMetadataValue( "Validity_Flag", "true");
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Validity_Flag= true");
	

	// *VALIDATION_DATE
	{
		string valTime = "UTC=" + DateTime().asISO8601(6) ; // PRQA S 4400
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
    }
	else {
		fType = fileType;
	}
	metadata.setMetadataValue( "File_Type", fType);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " File_Type=" << fType);

	// *FILE_CLASS
	string fileClass = fileInfo.ClassId;
	if( fileClass.empty() ) {
		bool classConfigured = findConfigurationValue(fileType, rs, "FileClass", fileClass); // PRQA S 4412
		ACS_COND_THROW( !classConfigured, exPDGSExtDataPreInventoryException("No file class configured!") ); // PRQA S 3081
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " File_Class got from configuration" );
	}
	metadata.setMetadataValue( "File_Class", fileClass);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " File_Class=" << fileClass );
	
	string generatingCentre;
	if( findConfigurationValue(fileType, rs, "GeneratingCentre", generatingCentre) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generating Centre got from configuration");
    }
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generating Centre missing in configuration");
		generatingCentre = fileInfo.GeneratingCentre;
	}
	ACS_COND_THROW( generatingCentre.empty(), exPDGSExtDataPreInventoryException("Generating centre not valorized") ); // PRQA S 3081

	metadata.setMetadataValue( "System", generatingCentre);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " System=" <<  generatingCentre );
	

	if( !fileInfo.geoCoords.empty() ) {
#warning "extend for more than four values."
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to fille geographic localization section.");
		vector<Metadata::GeoPointInfo>  vGeoPntInfo;

		Metadata::GeoPointInfo gPntInfoA;
		gPntInfoA.latitude = fileInfo.geoCoords[0];
		gPntInfoA.longitude = fileInfo.geoCoords[1];

		Metadata::GeoPointInfo gPntInfoB;
		gPntInfoB.latitude = fileInfo.geoCoords[2]; // PRQA S 4400 2
		gPntInfoB.longitude = fileInfo.geoCoords[3];


		vGeoPntInfo.push_back( gPntInfoA);
		vGeoPntInfo.push_back( gPntInfoB);

		metadata.setGeoPoints( vGeoPntInfo);
        metadata.setGeoType( dbGeoObject::Box);
		
		// TIMERANGE of Geographic_Localization
		{
			string tmpDate = "UTC=" + fileInfo.StartDate;
			metadata.setFieldValue( "Start_Time", tmpDate);
		}

		{
			string tmpDate = "UTC=" + fileInfo.StopDate;
			metadata.setMetadataValue( "Stop_Time", tmpDate);
		}

	}
	
	if( !fileInfo.StartOrbitNumber.empty() ) {
		int orbitNumber = -1;
		istringstream is(fileInfo.StartOrbitNumber);
		is >> orbitNumber;
		metadata.setMetadataValue( "Start_Orbit_Number", orbitNumber);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start_Orbit_Number=" <<  orbitNumber );
	}

	if( !fileInfo.StopOrbitNumber.empty() ) {
		int orbitNumber = -1;
		istringstream is(fileInfo.StopOrbitNumber);
		is >> orbitNumber;
		metadata.setMetadataValue( "Stop_Orbit_Number", orbitNumber);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Stop_Orbit_Number=" <<  orbitNumber );
	}

	
	return metadata;
}


void PDGSExtDataPreInventory::updateProductAndMetadata( const std::string& newVersion, Metadata& metadata)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);	
	
	metadata.setMetadataValue( "File_Version", newVersion);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File_Version field updated with the value \"" << newVersion << "\"");
			
    // QUALITY_INFO
	string qualityInfo = "DUP";
	metadata.setMetadataValue( "Quality_Info", qualityInfo);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Quality_Info field set to \"" << qualityInfo << "\"");
}

string PDGSExtDataPreInventory::doPackaging( Metadata& metadata, const string & firstPath, const string & filetype, const string & secondPath, dbConnection & )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);	
			
	ACS_COND_THROW( !secondPath.empty(), exPDGSExtDataPreInventoryException("No package foreseen: data filename should no be valorized \"" + secondPath + "\".") ); // PRQA S 3081

	return firstPath;
}


string PDGSExtDataPreInventory::getUniqueId()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	string uID;

	if( (getContext().which == Context::TL) || (getContext().which == Context::DS)  )
	{
		string pFileName = getParametersFile();
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
                ACS_THROW( exPDGSExtDataPreInventoryException ( "NOT found Unique_Id tag in the ParametersFile \""  + pFileName + "\"" ) ); // PRQA S 3081
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
		msg << "getUniqueId context.which=\"" << getContext().which << "\" NOT Recognized!!";
		ACS_THROW( exPDGSExtDataPreInventoryException(msg.str()) ); // PRQA S 3081
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to return \"" << uID << "\"");

	return uID;
	 
}


string PDGSExtDataPreInventory::getParametersFile()
{
	//apro il joborder e prelevo il parameterfile	
	DefaultJobOrder jo;

	XMLIstream jofileXml( getContext().argvParameters["jobOrder"].as<string>());
    jofileXml >> jo ;
	
	string parFileName = jo.getParametersFileName();
	if( parFileName.empty()) {
		//just notify.			
		ACS_LOG_WARNING("ParametersFile not specified!");
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Parameters filename =\"" << parFileName << "\"");
	}

	return parFileName;
	
}

bool PDGSExtDataPreInventory::overrideVersionHandling(bool& doVersioning)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to check if versioning is allowed for file type \"" << _currentFileType << "\"");
	
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	if( !findConfigurationValue(_currentFileType, rs, "DoVersioning", doVersioning) ) {
		ACS_THROW( exPDGSExtDataPreInventoryException("No DoVersioning tag in configuration!") ); // PRQA S 3081
    }
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DoVersioning " << boolalpha << doVersioning) ;
	
	return true;	
}

void PDGSExtDataPreInventory::setFtypeParentTag(const string & filetype, const rsResourceSet & rs )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	ACS_COND_THROW(_configurationRootTag.empty(), exPDGSExtDataPreInventoryException("Root configuration tag not valorized."));	 // PRQA S 3081
	
	string localRootTag = _configurationRootTag + ".List_Of_Registrations.Registration";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Root tag=\"" << localRootTag << "\"") ;
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
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "parentTag=\"" << tag << "\"") ;
		string regFileType;
		rs.getValue( tag + ".Object", regFileType);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TAG=" << tag << " fileType=\"" << regFileType << "\"");

		if( filetype == regFileType) {
			
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File type \"" << filetype << "\" found in configuration");
			_ftypeParentTag = tag ;
			found = true;
			break;
		}
	}
	
	ACS_COND_THROW(!found, exPDGSExtDataPreInventoryException("Cannot find fileType=\""+filetype+"\" in configuration"));	 // PRQA S 3081
}
		

string PDGSExtDataPreInventory::getFtypeParentTag(const string & filetype, const rsResourceSet & rs)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	if( _ftypeParentTag.empty() ) {
		setFtypeParentTag(filetype, rs);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, filetype << " filetype parent tag just set: \"" << _ftypeParentTag << "\"") ;
	}
	return _ftypeParentTag;
	
}


bool PDGSExtDataPreInventory::doVersioning(const Metadata& metadata, std::string& newVersion, dbConnection &conn)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    bool isTheVersionChanged = false ;

    //Execute Query
    dbInventoryObjectQuery* dbInvObjectQuery =dynamic_cast<dbInventoryObjectQuery*>( dbQueryFactoryChain::instance()->newObject("dbInventoryObjectQuery", conn.getKey()) ); // PRQA S 3081, 4412
	ACS_COND_THROW( !dbInvObjectQuery, exPDGSExtDataPreInventoryException("Error in executing dbSensorQuery") );	 // PRQA S 3081
	dbGenQueryScopeGuard stationQueryGuard(dbInvObjectQuery);
	
    //Get specific dbQueryParametes
    dbQueryParameters parameters = getProductFilenameInInventoryQueryParameters(conn, metadata);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "About to query t_inventory record with the following condition: " << parameters.getSqlString());

    dbInvObjectQuery->addParameter( parameters );
    unsigned long int countRow = dbInvObjectQuery->doQuery(conn);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Version already present: " << boolalpha << (countRow != 0) ); 
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
				if( decimalVersion && (transformedVersion.size() != 3) ) { // PRQA S 4400
					ACS_THROW( exPDGSExtDataPreInventoryException("Decimal version trasformation with size != 3 still not implemented") ); // PRQA S 3081
				}
				int vers = ::atoi(transformedVersion.c_str()) ;
            	maxVers = max(vers, maxVers) ; 
			}

        }	
		
		if( found ) {
        	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "maxVers = " << maxVers) ; 

        	int newVers = maxVers+1;
#warning "Fix in next version"
			if( decimalVersion ) {
				boost::format fmt("%03d");
				fmt % newVers;
				string newVersString = fmt.str();
				newVersion = newVersString.substr(0, 2) + "." + newVersString[newVersString.size()-1]; // PRQA S 3084, 4400
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



PDGSExtDataPreInventory::FileInfo::FileInfo(PDGSExtDataPreInventory::FileInfo const &other):
								SatelliteId(other.SatelliteId) ,
								Mission(other.Mission) ,
								ClassId(other.ClassId) ,
								File_Type(other.File_Type) ,
								StartDate(other.StartDate),							
								StopDate(other.StopDate),
								File_Version(other.File_Version),
								GeneratingCentre(other.GeneratingCentre),
								CreationDate(other.CreationDate),
								StartOrbitNumber(other.StartOrbitNumber),
								StopOrbitNumber(other.StopOrbitNumber),
								geoCoords(other.geoCoords),
								additionalInfo(other.additionalInfo)
{
}								

PDGSExtDataPreInventory::FileInfo & PDGSExtDataPreInventory::FileInfo::operator = (PDGSExtDataPreInventory::FileInfo const &other)
{
	if( this != &other ) {
		SatelliteId=other.SatelliteId;
		Mission=other.Mission;
		ClassId=other.ClassId;
		File_Type=other.File_Type;
		StartDate=other.StartDate;							
		StopDate=other.StopDate;
		File_Version=other.File_Version;
		GeneratingCentre=other.GeneratingCentre;
		CreationDate=other.CreationDate;
		StartOrbitNumber=other.StartOrbitNumber;
		StopOrbitNumber=other.StopOrbitNumber;
		geoCoords=other.geoCoords;
		additionalInfo=other.additionalInfo;
	}
		
	return (*this);
}								


_ACS_END_NAMESPACE
