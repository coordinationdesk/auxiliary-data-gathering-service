/*
	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libSafePreInventory$

	$Id$

	$Author$

	$Log$
	Revision 1.3  2017/02/28 16:13:04  lucio.pulvirenti
	S3EVOL-9: ManifestData additionalData vector filled on the base of additional info: XPath extracted.
	
	Revision 1.2  2015/09/14 12:41:55  damdec
	S3MPM-615: Mission set on metadata file generated.
	
	Revision 1.1  2015/06/19 12:50:13  nicvac
	S3MPM-533: Support for S3 products. Added also Sensor id and Geolocation.
	
        
*/ 

#include <S3ProdManifestDataExtractor.h>

#include <S3AuxManifestDataExtractor.h>

#include <ManifestParametersListBuilder.h>

#include <rsPushNSpace.h>
#include <ConfigurationSingleton.h>
#include <UtilXml.h>
#include <StringUtils.h>
#include <DateTime.h>
#include <File.h>

#include <Filterables.h>



using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(S3ProdManifestDataExtractor)



S3ProdManifestDataExtractor::S3ProdManifestDataExtractor():
	ManifestDataExtractor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	setConfSpaceId("ConfigSafePreInventory");
}

S3ProdManifestDataExtractor::~S3ProdManifestDataExtractor()  throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}

#define XmlStr( _x_, _p_, _y_) \
		_x_->getValue( _p_, _y_ ); \
		ACS_COND_THROW( _y_.empty(), exS3ProdManifestDataExtractorException(string("Empty value in Manifest for [")+_p_+"]") );

#define XmlDate( _x_, _p_, _y_) \
		_x_->getValue( _p_, _y_ ); \
		ACS_COND_THROW( _y_.empty(), exS3ProdManifestDataExtractorException(string("Empty value in Manifest for [")+_p_+"]") ); \
		DateTime dt( _y_ ); \
		_y_ = dt.asISO8601(6);


void S3ProdManifestDataExtractor::doWork(string const & manifestFilepath, SafePreInventory::ManifestData & manifestData) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

	ACS_COND_THROW( !File::exists(manifestFilepath), exS3ProdManifestDataExtractorException("\"" + manifestFilepath + "\" file does not exist!") );

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to extract metadata from \"" << manifestFilepath << "\" manifest file" );

	//Init maps
	const map< string, string > classMap = S3AuxManifestDataExtractor::createClassMap();
	const map< string, string > qualityCheckMap = S3AuxManifestDataExtractor::createQualityCheckMap();
	const map< string, string > satellitesIdMap = S3AuxManifestDataExtractor::createSatellitesIdMap();

	string platformFamilyName;
	string platformNumber;
	string hardwareName;
	string onlineQualityCheck;
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	string rootConfSpaceId = "PreInventory." + getConfSpaceId();
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Root configuration space id=\"" << rootConfSpaceId << '\"' );
	if( rs.hasKey( rootConfSpaceId + ".ManifestVersionManagement") ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Management of manifest version" );

		ManifestParametersListBuilder paramListBuilder;

		paramListBuilder.readManifestVersions(manifestFilepath);
		string filetype = this->getFiletype();
		// it should not occur.
		ACS_COND_THROW( filetype.empty(), exS3ProdManifestDataExtractorException("Filetype not valorized!"));
		
		rsPushNSpace ns(rs, rootConfSpaceId) ;
		paramListBuilder.buildParametersList(rs, filetype);
		
		ManifestParametersListBuilder::ParListIterator itDataEnd = paramListBuilder.getEndDataIterator();
		for( ManifestParametersListBuilder::ParListIterator it = paramListBuilder.getBeginDataIterator(); it != itDataEnd; ++it) {
			string parameterName;
			string value = paramListBuilder.getParameter(it, parameterName);
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Parameter name=\"" << parameterName << "\"; value=\"" << value << '\"' );
			if( StringUtils::equals(parameterName, "productName") ) {
				manifestData.filename = value;
			}
			else if( StringUtils::equals(parameterName, "productType") ) {
				manifestData.filetype = value;
			}
			else if( StringUtils::equals(parameterName, "platformFamilyName") ) {
				platformFamilyName = value;
			}
			else if( StringUtils::equals(parameterName, "platformNumber") ) {
				platformNumber = value;
			}
			else if( StringUtils::equals(parameterName, "productSize") ) {
				manifestData.productSize = value;
			}
			else if( StringUtils::equals(parameterName, "startTime") ) {
				manifestData.startTime = value;
			}
			else if( StringUtils::equals(parameterName, "stopTime") ) {
				manifestData.stopTime = value;
			}
			else if( StringUtils::equals(parameterName, "creationTime") ) {
				manifestData.creationTime = value;
			}
			else if( StringUtils::equals(parameterName, "facilityName") ) {
				manifestData.facilityName = value;
			}
			else if( StringUtils::equals(parameterName, "softwareName") ) {
				manifestData.softwareName = value;
			}
			else if( StringUtils::equals(parameterName, "softwareVersion") ) {
				manifestData.softwareVersion = value;
			}
			else if( StringUtils::equals(parameterName, "hardwareName") ) {
				hardwareName = value;
			}
			else if( StringUtils::equals(parameterName, "onlineQualityCheck") ) {
				onlineQualityCheck = value;
			}
			else if( StringUtils::equals(parameterName, "instrumentFamilyName") ) {
				manifestData.instrumentName = value;
			}
			else if( StringUtils::equals(parameterName, "footPrintPosList") ) {
				manifestData.footPrintPosList = value;
			}
			else {
				if( not value.empty() ) {
					manifestData.additionalData.push_back( make_pair(parameterName, value) );
				}
			}
			
		}
		
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Manifest version not managed" );

		XmlDocSptr x( new XmlDoc );
		x->setParameters(XmlDoc::parDefault | XmlDoc::parSetRootNs).initWithFile( manifestFilepath );

		XmlStr( x, "//sentinel3:generalProductInformation/sentinel3:productName/text()", manifestData.filename );
		XmlStr( x, "//sentinel3:generalProductInformation/sentinel3:productType/text()", manifestData.filetype );
		
		XmlStr( x, "//sentinel-safe:platform/sentinel-safe:familyName/text()", platformFamilyName);
		// S3MPM-615: number added to familyName
		XmlStr( x, "//sentinel-safe:platform/sentinel-safe:number/text()", platformNumber);
		XmlStr( x, "//sentinel3:generalProductInformation/sentinel3:productSize/text()", manifestData.productSize);
		
		// Here can be use XmlDate to get formatted string. But date will be formatted later during Metadata creation
		// So here keep value as is from manifest.
		XmlStr( x, "//sentinel-safe:acquisitionPeriod/sentinel-safe:startTime/text()", manifestData.startTime);
		XmlStr( x, "//sentinel-safe:acquisitionPeriod/sentinel-safe:stopTime/text()", manifestData.stopTime);
		XmlStr( x, "//sentinel3:generalProductInformation/sentinel3:creationTime/text()", manifestData.creationTime );
				
		XmlStr( x, "//xmlData/sentinel-safe:processing/sentinel-safe:facility/@name", manifestData.facilityName);

		XmlStr( x, "//xmlData/sentinel-safe:processing/sentinel-safe:facility/sentinel-safe:software/@name", manifestData.softwareName);
		XmlStr( x, "//xmlData/sentinel-safe:processing/sentinel-safe:facility/sentinel-safe:software/@version", manifestData.softwareVersion);
				
		XmlStr( x, "//xmlData/sentinel-safe:processing/sentinel-safe:facility/sentinel-safe:hardware/@name", hardwareName);
		XmlStr( x, "//xmlData/sentinel-safe:qualityInformation/sentinel-safe:extension/sentinel3:productQuality/sentinel3:onlineQualityCheck/text()", onlineQualityCheck);
				
		XmlStr( x, "//xmlData/sentinel-safe:platform/sentinel-safe:instrument/sentinel-safe:familyName/@abbreviation", manifestData.instrumentName );

		XmlStr( x, "//xmlData/sentinel-safe:frameSet/sentinel-safe:footPrint/gml:posList/text()", manifestData.footPrintPosList );
				
		vector< pair<string, string> > additionalFieldsPath = this->getAdditionalDataFields();
		const size_t fieldsSize = additionalFieldsPath.size();
		for( size_t i = 0; i < fieldsSize; ++i ) {
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Additional data to extract #" << i << ": XPath=\"" << additionalFieldsPath[i].first << "\"; name=\"" << additionalFieldsPath[i].second << '\"');
			vector<string> values;
			XmlStr( x, additionalFieldsPath[i].first, values );	
			if( values.size() > 1 ) {
				ACS_LOG_WARNING("Extracted " << values.size() << " values from \"" << additionalFieldsPath[i].first << "\" XML Path of \"" << manifestFilepath << "\" manifest file: taking the first one only!");
			}
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "(The first) extracted value: \"" << values[0] << '\"');
			manifestData.additionalData.push_back( make_pair(additionalFieldsPath[i].second, values[0]) );
		}

	}


	{
		platformFamilyName += platformNumber;
		map<string, string>::const_iterator it = satellitesIdMap.find(StringUtils::uppercase(platformFamilyName));
		ACS_COND_THROW(it == satellitesIdMap.end(),
				exS3ProdManifestDataExtractorException("Can't find [" + StringUtils::uppercase(platformFamilyName) + "] key in satellites map"));
		manifestData.mission = it->second;
	}

	{
		map<string, string>::const_iterator it = classMap.find(StringUtils::uppercase(hardwareName));
		ACS_COND_THROW(it == classMap.end(),
				exS3ProdManifestDataExtractorException("Can't find [" + StringUtils::uppercase(hardwareName) + "] key in class map"));
		manifestData.fileClass = it->second;
	}

	{
		map<string, string>::const_iterator it = qualityCheckMap.find(StringUtils::uppercase(onlineQualityCheck));
		ACS_COND_THROW(it == qualityCheckMap.end(),
				exS3ProdManifestDataExtractorException("Can't find [" + StringUtils::uppercase(onlineQualityCheck) + "] key in quality check map"));
		manifestData.qualityFlag = it->second;
	}

}


_ACS_END_NAMESPACE

