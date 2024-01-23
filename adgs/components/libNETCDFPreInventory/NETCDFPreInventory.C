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

	$Prod: A.C.S. libNETCDFPreInventory$

	$Id$

	$Author$

	$Log$
	Revision 1.14  2015/07/20 14:12:44  lucio.pulvirenti
	S3MPM-549: getUniqueId: DS context now managed.
	
	Revision 1.13  2014/09/10 09:31:27  lucio.pulvirenti
	S1PDGS-31530: New format of Sea ICE products in NetCDF format managed. New exMacros used.
	
	Revision 1.12  2014/09/09 17:08:48  lucio.pulvirenti
	S1PDGS-31530: work in progress.
	
	Revision 1.11  2013/04/29 16:29:27  marpas
	base class interface changed
	
	Revision 1.10  2013/04/05 09:38:47  marpas
	NameIdAssociator partial refactoring
	
	Revision 1.9  2013/02/22 14:37:07  lucio.pulvirenti
	dbConnection parameter added to prepareMetadata,getSatelliteIdFromSatelliteName,computeMetadata methods.
	
	Revision 1.8  2012/12/14 15:12:10  chicas
	*** empty log message ***
	
	Revision 1.7  2012/12/07 17:06:50  chicas
	 FilenameUtils substitued by File class ;
	doPackaging interface changed : now returning the path on disk of file to inventory ;
	satellite, misision and uniqueid metadata are optionals;
	GeoBox bug fixed (two points instead of four)
	
	Revision 1.6  2012/10/29 13:30:23  giufor
	removed obsolete comments
	
	Revision 1.5  2012/10/29 13:24:08  giufor
	implemented getTypeNetCDF method
	
	Revision 1.4  2012/10/25 14:57:49  giufor
	updated prepareMetadata
	
	Revision 1.3  2012/10/25 13:53:11  giufor
	implemented readNETCDFFile method
	
	Revision 1.2  2012/10/23 16:38:35  giufor
	implemented getDeltaValidities, getFileNameRegexps, getValidityTime methods
	
	Revision 1.1.1.1  2012/10/22 10:25:47  giufor
	Import libNETCDFPreInventory
	
	
*/ 

#include <NETCDFPreInventory.h>
#include <NETCDFPreInventoryV.h>
#include <Filterables.h>

#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>

#include <XMLIstream.h>
#include <StringUtils.h>
#include <dbPersistent.h>

#include <NameIdAssociator.h>

#include <ncFile.h>

using namespace std;
using namespace acs;
using namespace netCDF;


ACS_CLASS_DEFINE_DEBUG_LEVEL(NETCDFPreInventory)

static NETCDFPreInventoryV version;

NETCDFPreInventory::NETCDFPreInventory() : 
					_currentFileName(),
					_netCDFcontentId()
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: "<< Id );
}

NETCDFPreInventory::~NETCDFPreInventory() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

string NETCDFPreInventory::getClassId()
{
        //return the class name
	return "NETCDFPreInventory";
}
        

void NETCDFPreInventory::computeMetadata( const string& pathFileName, const string& fileType, Metadata& metadata, dbConnection & conn)
{
        //NB: the pathFileName passed to this method is an absolute path
 	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	try
        {
			ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "MetadataInfo:\n"
									<< "  OrderId=" << getContext().metadataInfo.orderId << '\n'
									<< "  pathFileName= " << pathFileName << '\n'
									<< "  filetype= " << fileType << '\n');

			_currentFileName = pathFileName;

			NETCDFStruct netcdfs;
			readNETCDFFile( pathFileName, netcdfs);

			metadata = prepareMetadata( fileType, netcdfs, conn);
        }
        catch( exception& ex)
        {
            ACS_THROW( exNETCDFPreInventoryException ( ex, "computeMetadata Exception") );
        }
}      




Metadata NETCDFPreInventory::prepareMetadata( const string& fileType, const NETCDFStruct& netcdfs, dbConnection & conn)
{
 	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);


	//filling Metadata
	Metadata metadata;

	// *UNIQUE_ID
	string uniqueId = getUniqueId();
    if (!uniqueId.empty()) {
        metadata.setMetadataValue( "Unique_Id", uniqueId); 
	}
	
	
	// *FILE_NAME  NB:e' il nome del file NETCDF, cioe' il file da inventoriare;
	string fileName = fillMetadataFilename( _currentFileName, fileType, metadata, conn);
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB," File_Name=" << fileName);

	
	// *FILE_VERSION
	string fileVersion = "1";
	metadata.setMetadataValue( "File_Version", getFormattedVersion("1"));
	
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB," File_Version=" << fileVersion);
	
	// *SOURCE (lo prendo dal context)
	string source = getContext().metadataInfo.processorName;
	metadata.setMetadataValue( "Source", source);

	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB," Source=" << source);

	
	// *SOURCE_SW_VERSION (lo prendo dal context)
	string sourceSwVersion = getContext().metadataInfo.processorVersion;
	metadata.setMetadataValue( "Source_Sw_Version", sourceSwVersion);

	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB," Source_Sw_Version=" << sourceSwVersion);

		
	// *GENERATION_TIME 
	string generationTime = netcdfs.CreationDate;
	metadata.setMetadataValue( "Generation_Time", generationTime);
	
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB," Generation_Time=" << generationTime);
	
	
	string validityStart;
	string validityStop;
	getValidityTime( validityStart, validityStop);
	
	// *VALIDITY_START
	metadata.setMetadataValue( "Validity_Start", validityStart);
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "Validity_Start=" << validityStart);

	  

	// *VALIDITY_STOP
	metadata.setMetadataValue( "Validity_Stop", validityStop);
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, " Validity_Stop=" << validityStop);

	
	// *GEOGRAPHIC_LOCALIZATION
	vector<Metadata::GeoPointInfo>  vGeoPntInfo;
	
	//suppongo senso antiorario a partire da A->B->C->D (A in alto a destra)
	Metadata::GeoPointInfo gPntInfoA;
	gPntInfoA.latitude = netcdfs.NorthernLat;
	gPntInfoA.longitude = netcdfs.WesternLon;
	
	//Metadata::GeoPointInfo gPntInfoB;
	//gPntInfoB.latitude = netcdfs.SouthernLat;
	//gPntInfoB.longitude = netcdfs.WesternLon;
	
	Metadata::GeoPointInfo gPntInfoC;
	gPntInfoC.latitude = netcdfs.SouthernLat;
	gPntInfoC.longitude = netcdfs.EasternLon;
	
	//Metadata::GeoPointInfo gPntInfoD;
	//gPntInfoD.latitude = netcdfs.NorthernLat;
	//gPntInfoD.longitude = netcdfs.EasternLon;
	
	vGeoPntInfo.push_back( gPntInfoA);
	//vGeoPntInfo.push_back( gPntInfoB);
	vGeoPntInfo.push_back( gPntInfoC);
	//vGeoPntInfo.push_back( gPntInfoD);
	
	metadata.setGeoPoints( vGeoPntInfo);
        metadata.setGeoType( dbGeoObject::Box);

       	// TIMERANGE of Geographic_Localization
        metadata.setFieldValue( "Start_Time", validityStart);
        metadata.setFieldValue( "Stop_Time", validityStop);
	
	//lanciare il warning
	ACS_LOG_WARNING(" GeographicLocalization: Consider a box type with the sides parallel to the meridians and parallels!");
	
	
	
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
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "Validity_Flag= true");
	

	// *VALIDATION_DATE
	DateTime dt;            //today datetime
	ostringstream strTime;
	strTime << "UTC=";
	dt.asISO8601( strTime, 6);
	string creationTime = strTime.str();
	metadata.setMetadataValue( "Validation_Date", creationTime); 

	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "Validation_Date=" << creationTime);


	// *HEADER_SIZE 
	long long unsigned int headerSize = 0;
	metadata.setMetadataValue( "Header_Size", headerSize); 
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "Header_Size=" << headerSize);
	

	// *DATA_SIZE prelevo la size reale
	long long unsigned int fileSize = File::size( _currentFileName);
	metadata.setMetadataValue( "Data_Size", fileSize);
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "Data_Size=" << fileSize);


	// *FILE_TYPE
	metadata.setMetadataValue( "File_Type", fileType);
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "File_Type=" << fileType);
	
	
	// *FILE_CLASS
	string fileClass = "OPER";
	metadata.setMetadataValue( "File_Class", fileClass);
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "File_Class=" << fileClass);

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
	
	// *SATELLITE_ID (lo prendo dal context)
	string satelliteName = getContext().metadataInfo.satelliteName;	
    if( !satelliteName.empty()) {
        metadata.setMetadataValue( "Satellite_Id", getSatelliteIdFromSatelliteName( satelliteName, conn));
	}
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "SatelliteName=" << satelliteName);

/*
	// ASCENDING FLAG
	bool ascendingFlag = false;
	metadata.setMetadataValue( "Ascending_Flag", ascendingFlag);

	exCLASS_BGN_DEBUG(10)
		ostringstream msg;
		msg << " Ascending_Flag=" << ascendingFlag;
		exFDebug::notify(msg.str());
	exCLASS_END_DEBUG
*/


	// *MISSION  dal context
	string missionId = getContext().metadataInfo.mission;    
    if( !missionId.empty()) {
        metadata.setMetadataValue( "Mission", missionId);
	}
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "Mission_Id=" << missionId);
	
	
	return metadata;
}

void NETCDFPreInventory::updateProductAndMetadata( const std::string& newVersion, Metadata& metadata)
{
 	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}


string NETCDFPreInventory::doPackaging( Metadata& metadata, const string & firstPath, const string & filetype, const string & secondPath, dbConnection & )
{
 	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ACS_COND_THROW( not secondPath.empty(), exNETCDFPreInventoryException("No package foreseen: data filename should no be valorized \"" + secondPath + "\".") );

	return firstPath;
}

//UniqueId is NO a mandatory metadata.
//
string NETCDFPreInventory::getUniqueId()
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
                    ACS_LOG_WARNING("Empty Unique_Id tag in the ParametersFile \""  << pFileName << "\"");
                }

                ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "Unique_Id=" << uID);
            }
            else
            {
                ACS_LOG_WARNING("NOT found Unique_Id tag in the ParametersFile \""  << pFileName << "\"");
            }
        }
	}
	else if( getContext().which == Context::DIS)
	{
		ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "getUniqueId NOT IMPLEMENTED for DIS");
	}
	else if( getContext().which == Context::PROD)
	{
		ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "getUniqueId NOT IMPLEMENTED for PROD");
	}
	else
	{
		ostringstream exMsg;
		exMsg << "getUniqueId context.wich=\"" << getContext().which << "\" NOT Recognized!!";
		ACS_THROW( exNETCDFPreInventoryException(exMsg.str()) );
	}

	return uID;
	 
}


// string NETCDFPreInventory::getParametersFile()
// {
// 	//apro il joborder e prelevo il parameterfile	
// 	DefaultJobOrder jo;
// 
// 	XMLIstream jofileXml( getContext().argvParameters["jobOrder"].as<string>());
//         jo << jofileXml;
// 	
// 	string parFileName = jo.getParametersFileName();
//     //it could be emnpty!
//     //
// 	/*if( parFileName.empty())
// 	{
// 		exNETCDFPreInventoryException e( "Parameters_File.File_Name is EMPTY!");
// 		throwException(e);
// 	}*/
// 	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "parameters filename =\"" << parFileName << '\"');
// 
// 	return parFileName;
// 	
// }

string NETCDFPreInventory::getFormattedVersion( const string& version)
{
        ostringstream os;
        os << std::setw( 4) << std::setfill( '0') << std::setiosflags( ios_base::right) << version;
        return os.str();
}

int NETCDFPreInventory::getSatelliteIdFromSatelliteName( const string& satName, dbConnection & conn)
{
 	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	NameIdAssociator<dbPersistent::IdType> satellites( "t_satellites", "satelliteid", "satellitename", conn.getKey());
    satellites.load();

    return satellites.id( satName);
}





void NETCDFPreInventory::getDeltaValidities( int& iceDelta, int& waveDelta)
{
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	try {
        //read the configuration file from database
        rsResourceSet& rs = ConfigurationSingleton::instance()->get();

        string tag = "PreInventory.ConfigNETCDFPreInventory.ICEDeltaValidity";
		if( rs.hasKey( tag)) {
			rs.getValue( tag, iceDelta);
		}
		else
		{
			iceDelta = 1; //1 day
			ACS_LOG_WARNING("NOT found ICEDeltaValidity tag in configuration: default value 1 day");
		}
		
		tag = "PreInventory.ConfigNETCDFPreInventory.WAVEDeltaValidity";
		if( rs.hasKey( tag)) {
			rs.getValue( tag, waveDelta);
		}
		else
		{
			waveDelta = 1; //1 day
			ACS_LOG_WARNING("NOT found WAVEDeltaValidity tag in configuration: default value 1 day");
		}
        
	}
    catch( exception& ex)
    {
		ACS_THROW( exNETCDFPreInventoryException( ex, "NETCDFPreInventory::getDeltaValidities Exception"));
    }
}


void NETCDFPreInventory::getFileNameRegexps( string & regExpICE, string & regExpNewICE, string & regExpWAVE)
{
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	try {
         //read the configuration file from database
         rsResourceSet& rs = ConfigurationSingleton::instance()->get();
		 rsPushNSpace confNs(rs, "PreInventory.ConfigNETCDFPreInventory" ); 

        string tag = "ICEFileNameRegExp";
		if( rs.hasKey( tag)) {
			rs.getValue( tag, regExpICE);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, tag << '=' << regExpICE);
		}
		else {
			ACS_LOG_WARNING("NOT found " << tag << " tag in configuration");
		}
		
		tag = "NewICEFileNameRegExp";
		if( rs.hasKey( tag)) {
			rs.getValue( tag, regExpNewICE);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, tag << '=' << regExpNewICE);
		}
		else {
			ACS_LOG_WARNING("NOT found " << tag << " tag in configuration");
		}

		
		tag = "WAVEFileNameRegExp";
		if( rs.hasKey( tag)) {
			rs.getValue( tag, regExpWAVE);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, tag << '=' << regExpWAVE);
		}
		else {
			ACS_LOG_WARNING("NOT found " << tag << " tag in configuration");
		}
		
	}
	catch( exception& ex)
	{
		ACS_THROW( exNETCDFPreInventoryException( ex, "NETCDFPreInventory::getFileNameRegexps Exception"));
	}
}



void NETCDFPreInventory::getValidityTime( string& validityStart, string& validityStop)
{
//from filename ice_conc_XX_YYYYMMDDHOMI.nc the validity start is YYYYMMDDHOMI
// *VALIDITY_STOP (configurabile)
	//OSS: il validity stop non esiste pero' osservo 4.1 CFI40
	//Generation Frequency: 2 files per day (one for the northen and the other for the southern hemisphere
	//allora posso considerare VALIDITY_STOP=VALIDITY_START + 24h

//from filename IFR_WW3-GLOBAL-30MIN_YYYYMMDDTHHZ.nc the validity start is YYYYMMDDTHHH
//              ifremer-ww3-glob-30min-20040724t03z-uss.nc  (nome reale che differisce dal naming)
// *VALIDITY_STOP (configurabile)
	//OSS: il validity stop non esiste pero' osservo 5.1 CFI40
	//Generation Frequency: The operational file is updated every 12h at 00:00 and 12:00 for a validity of 6 days with a temporal resolution of 3h
	//allora posso considerare VALIDITY_STOP=VALIDITY_START + 24h*6 = VALIDITY_START + 144

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	int iceDelta = 0;
	int waveDelta = 0;
	
	getDeltaValidities( iceDelta, waveDelta);
	
	const string fileName = File::getFileName( _currentFileName);
	
   	RegExp reg;
	reg.setReg( _netCDFcontentId.second);
    RegExp::KeyMatches keyMatches;
	ACS_COND_THROW( not reg.match( fileName, keyMatches), exNETCDFPreInventoryException('\"' + fileName + "\" filename does not match " + _netCDFcontentId.second + " regular expression"));
	
	size_t keyMatchesSize = keyMatches.size();
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "KeyMatches content (" << keyMatchesSize << " elements) of \"" << fileName << "\" file matching " + _netCDFcontentId.second + " regular expression:" );
		for(size_t i = 0; i < keyMatchesSize; i++ ) {
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << "[" << i << "]: " << keyMatches[i].getMatched() );
		}
	ACS_CLASS_END_DEBUG

	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	rsPushNSpace confNs(rs, "PreInventory.ConfigNETCDFPreInventory" ); 

	if( ICE == _netCDFcontentId.first || NEWICE == _netCDFcontentId.first )
	{
		size_t regExpInx;
		string tag;
		if( ICE == _netCDFcontentId.first ) {
			// if regular expression is ice_conc_([n|s]h)_([[:digit:]]{12}).nc -> validity start string index is 2 (default)
			// if regular expression were ice_(conc|type|edge)_([n|s]h)_([[:digit:]]{12}).nc -> validity start string index should be set to 3.
			regExpInx = _ICERegExpValStartInxDefault;		
        	tag = "ICERegExpValStartInx";
		}
		else { // NEWICE
			regExpInx = _NewICERegExpValStartInxDefault;		
        	tag = "NewICERegExpValStartInx";
		}
		
		if( rs.hasKey( tag)) {
			rs.getValue( tag, regExpInx);
		}
		else {
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, tag << " tag not found in configuration: default is " << regExpInx);
		}

		if( regExpInx >= keyMatchesSize ) {
			ostringstream exMsg;
			exMsg << tag << " tag value " << regExpInx << " not consistent with key matches size " << keyMatchesSize << ": it should be minor";
			ACS_THROW( exNETCDFPreInventoryException(exMsg.str()));
		}
		
		string vs = keyMatches[regExpInx].getMatched();
		size_t expectedSize = 12;
		if( vs.size() != expectedSize ) {
			ostringstream exMsg;
			exMsg << '\"' + vs + "\" should be validity start date of " << expectedSize << " characters length (real length is " << vs.size() << " characters) extracted from " << fileName << " file name";
			ACS_THROW( exNETCDFPreInventoryException(exMsg.str()));
		}
		
		
		//estraggo la validityStart from ICE filename es 201107031200 <=> YYYYMMDDHOMI
		//                                               012345678901
		validityStart = "UTC=";
		validityStart += vs.substr( 0, 4) + "-";  //Year
		validityStart += vs.substr( 4, 2) + "-";  //Mounth
		validityStart += vs.substr( 6, 2) + "T";  //Day
		validityStart += vs.substr( 8, 2) + ":";  //hour
		validityStart += vs.substr( 10, 2) + ":"; //min
		validityStart += "00";                    //sec
		
		double delta = iceDelta/24.0; //il delta e' in giorni quindi ad esempio 3 ore sono 3/24giorni
		DateTime dStop( validityStart);
        	dStop += DateTime::JD50( delta);
        	ostringstream os;
		os << "UTC=";
		dStop.asISO8601( os, 0);
		validityStop = os.str();
		
	}
	else if( WAVE == _netCDFcontentId.first ) {
		
		size_t regExpInx = _WAVERegExpValStartInxDefault;
		string tag = "WAVERegExpValStartInx";
		if( rs.hasKey( tag)) {
			rs.getValue( tag, regExpInx);
		}
		else {
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, tag << " tag not found in configuration: default is " << regExpInx);
		}
		
		if( regExpInx >= keyMatchesSize ) {
			ostringstream exMsg;
			exMsg << tag << " tag value " << regExpInx << " not consistent with key matches size " << keyMatchesSize << ": it should be minor";
			ACS_THROW( exNETCDFPreInventoryException(exMsg.str()));
		}

		//estraggo la validityStart from WAVE filename es 20040724t03 <=> YYYYMMDDTHH
		//                                                01234567890
		string vs = keyMatches[regExpInx].getMatched();
		size_t expectedSize = 11;
		if( vs.size() != expectedSize ) {
			ostringstream exMsg;
			exMsg << '\"' + vs + "\" should be validity start date of " << expectedSize << " characters length (real length is " << vs.size() << " characters) extracted from " << fileName << " file name";
			ACS_THROW( exNETCDFPreInventoryException(exMsg.str()));
		}
		
		validityStart = "UTC=";
		validityStart += vs.substr( 0, 4) + "-";  //Year
		validityStart += vs.substr( 4, 2) + "-";  //Mounth
		validityStart += vs.substr( 6, 2) + "T";  //Day
		validityStart += vs.substr( 9, 2) + ":";  //hour
		validityStart += string("00") + ":";              //min
		validityStart += "00";                    //sec

		double delta = waveDelta/24.0; //il delta e' in giorni quindi ad esempio 3 ore sono 3/24giorni
		DateTime dStop( validityStart);
        	dStop += DateTime::JD50( delta);
        	ostringstream os;
		os << "UTC=";
		dStop.asISO8601( os, 0);
		validityStop = os.str();

	}
	else {
		ACS_THROW(exNETCDFPreInventoryException( "Impossible to get ValidityStart from FileName since content type unrecognized: " + getNetCDFTypeStr(_netCDFcontentId.first) ) );
	}

}



void NETCDFPreInventory::readNETCDFFile( const string& pathFileName, NETCDFStruct& netcdfs)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Path=\""  << pathFileName << '\"');

	getTypeNetCDF( pathFileName );
	NetCDFType netCDFType = _netCDFcontentId.first;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Content type of \""  << pathFileName << "\" file is " << getNetCDFTypeStr(netCDFType));
	
		
	//Reading data from .nc file
	NcFile dataFile( pathFileName.c_str(), NcFile::read);
	ACS_LOG_INFO('\"' << pathFileName << "\" file successfully read");

	if( netCDFType == ICE || netCDFType == WAVE ) {
		NcGroupAtt att = dataFile.getAtt("creation_date");
		att.getValues( netcdfs.CreationDate);	
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "creation_date attribute extracted: " << netcdfs.CreationDate );
		if( netCDFType == ICE)
		{
			netcdfs.CreationDate = "UTC=" + netcdfs.CreationDate + "T00:00:00";
		}
		else
		{		
			netcdfs.CreationDate = "UTC=" + netcdfs.CreationDate.substr( 0, netcdfs.CreationDate.length()-1);					
		}

	}
	else if( netCDFType == NEWICE ) {
		NcGroupAtt att = dataFile.getAtt("history");
		string historyAttr;
		att.getValues( historyAttr);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "history attribute found with value: \"" << historyAttr << '\"' );
		
		vector<string> tokens;
		StringUtils::Tokenize(historyAttr, tokens, " ");
        size_t t_size = tokens.size();
		size_t s = 0;
		for(size_t i = 0; i < t_size; ++i ) {
			if( tokens[i] == "creation" ) {
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "creation label found in attribute value");
				s = i;
				break;
			}
		}
		ACS_COND_THROW( 0 == s, exNETCDFPreInventoryException("Cannot find creation value in history attribute value") );
		
		netcdfs.CreationDate = tokens[s-1];
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "creation date extracted from attribute value: " << netcdfs.CreationDate);
		netcdfs.CreationDate = "UTC=" + netcdfs.CreationDate + "T00:00:00";
	}
	else {
		ostringstream exMsg;
		exMsg << pathFileName << " file content type not managed: " << getNetCDFTypeStr(netCDFType);
		ACS_THROW( exNETCDFPreInventoryException(exMsg.str()) );
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "creationdate=" << netcdfs.CreationDate );

	
	{
		NcGroupAtt att = dataFile.getAtt("area");
		att.getValues( netcdfs.Area);
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "-area=" << netcdfs.Area);
	}
	
	{
		NcGroupAtt att = dataFile.getAtt("southernmost_latitude");
		if( ICE == netCDFType || NEWICE == netCDFType)
		{
			float sl;
			att.getValues( &sl);
			
			ostringstream footprint;
        		footprint << fixed << setprecision(5) << sl;
			netcdfs.SouthernLat = footprint.str();
		}
		else if( WAVE == netCDFType ) 
		{
			char sl[ att.getAttLength()];
			att.getValues( &sl);
			netcdfs.SouthernLat = string( sl, att.getAttLength());
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "-sudlat=" << netcdfs.SouthernLat);
	}
	
	{
		NcGroupAtt att = dataFile.getAtt("northernmost_latitude");
		if( ICE == netCDFType || NEWICE == netCDFType)
		{
			float nl;
			att.getValues( &nl);
			
			ostringstream footprint;
        		footprint << fixed << setprecision(5) << nl;
			netcdfs.NorthernLat = footprint.str();
		}
		else if( WAVE == netCDFType ) 
		{
			char nl[ att.getAttLength()];
			att.getValues( &nl);
			netcdfs.NorthernLat = string( nl, att.getAttLength());
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "-nordlat=" << netcdfs.NorthernLat);
	}
	
	{
		NcGroupAtt att = dataFile.getAtt("westernmost_longitude");
		if( ICE == netCDFType || NEWICE == netCDFType)
		{
			float wl;
			att.getValues( &wl);
			
			ostringstream footprint;
        		footprint << fixed << setprecision(5) << wl;
			netcdfs.WesternLon = footprint.str();
		}
		else if( WAVE == netCDFType ) 
		{
			char wl[ att.getAttLength()];
			att.getValues( &wl);
			netcdfs.WesternLon = string( wl, att.getAttLength());
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "-ovestlon=" << netcdfs.WesternLon);
	}
	
	{
		NcGroupAtt att = dataFile.getAtt("easternmost_longitude");
		if( ICE == netCDFType || NEWICE == netCDFType)
		{
			float el;
			att.getValues( &el);
			
			ostringstream footprint;
        		footprint << fixed << setprecision(5) << el;
			netcdfs.EasternLon = footprint.str();
		}
		else if( WAVE == netCDFType ) 
		{
			char el[ att.getAttLength()];
			att.getValues( &el);
			netcdfs.EasternLon = string( el, att.getAttLength());	
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "-estlon=" << netcdfs.EasternLon);
	}
	
	{
		NcGroupAtt att = dataFile.getAtt("start_date");
		att.getValues( netcdfs.StartDate);
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "-startdate=" << netcdfs.StartDate);
	}
	
	if( ICE == netCDFType)
	{
		NcGroupAtt att = dataFile.getAtt("start_time");
		att.getValues( netcdfs.StartTime);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "-starttime=" << netcdfs.StartTime);
	}
	
	{	
		NcGroupAtt att = dataFile.getAtt("stop_date");
		att.getValues( netcdfs.StopDate);
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "-stopdate=" << netcdfs.StopDate);
 	}
	
	if( ICE == netCDFType) {
		NcGroupAtt att = dataFile.getAtt("stop_time");
		att.getValues( netcdfs.StopTime);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "-stoptime=" << netcdfs.StopTime);
	}
	
	{
		NcGroupAtt att = dataFile.getAtt("institution");
		att.getValues( netcdfs.Institution);
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "-institution=" << netcdfs.Institution);
	}
	
	if( ICE == netCDFType || WAVE == netCDFType) {
		NcGroupAtt att = dataFile.getAtt("operational_status");
		att.getValues( netcdfs.OperationalStatus);
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "-operationalstatus=" << netcdfs.OperationalStatus);
	}
      
}



//get the content: ICE, NEWICE, WAVE
void NETCDFPreInventory::getTypeNetCDF( const string & pathFileName)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	string regExpICEFileName ; 
	string regExpNewICEFileName ;
	string regExpWAVEFileName ;
	getFileNameRegexps( regExpICEFileName, regExpNewICEFileName, regExpWAVEFileName);
	
	ACS_COND_THROW(regExpICEFileName.empty() && regExpWAVEFileName.empty() && regExpNewICEFileName.empty(), exNETCDFPreInventoryException("Impossible to recognize file content: all regexp in configuration are NULL!"));
	
	const string fileName = File::getFileName( pathFileName);
	NetCDFType netCDFType = NONE;
	string filenameRegExp;
   	
	RegExp reg;
	
	//if ICE
	if( not regExpICEFileName.empty() ) {
		reg.setReg( regExpICEFileName);
		if( reg.match( fileName ) ) {
			netCDFType = ICE; 
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, fileName << " file content is ICE");
			filenameRegExp = regExpICEFileName;
		}
	}

	// if ICE new content
	if( netCDFType == NONE && not regExpNewICEFileName.empty() ) {
		reg.setReg( regExpNewICEFileName);
		if( reg.match( fileName ) ) {
			netCDFType = NEWICE; 
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, fileName << " file content is NEW ICE");
			filenameRegExp = regExpNewICEFileName;
		}
		
	}
	
	//if WAVE
	if( netCDFType == NONE && not regExpWAVEFileName.empty() ) {
		reg.setReg( regExpWAVEFileName);
		if( reg.match( fileName ) ) {
			netCDFType = WAVE; 
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, fileName << " file content is WAVE");
			filenameRegExp = regExpWAVEFileName;
		}
		
	}
	
	ACS_COND_THROW( netCDFType == NONE, exNETCDFPreInventoryException("Impossible to recognize " + pathFileName + " file content from file name"));

	_netCDFcontentId = make_pair(netCDFType, filenameRegExp);

}


string NETCDFPreInventory::getNetCDFTypeStr(NetCDFType type)
{
	
	string typeString;
	switch(type) {
    	case NONE:
        	{ 
				typeString = "NONE";
				break;
			}
    	case ICE:
        	{ 
				typeString = "ICE";
				break;
			}
    	case NEWICE:
        	{ 
				typeString = "NEWICE";
				break;
			}
    	case WAVE:
        	{ 
				typeString = "WAVE";
				break;
			}
    	default:
        	{ 
            	ostringstream os ;
            	os << "<UNKNOWN: " << int(type) << ">" ; // PRQA S 3081
            	typeString = os.str() ;
        	}
	}
	
	return typeString;
	
}
