/*
	Copyright 2019, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	
        
*/ 

#include <S1ProdManifestDataExtractor.h>


#include <rsPushNSpace.h>
#include <ConfigurationSingleton.h>
#include <UtilXml.h>
#include <StringUtils.h>
#include <DateTime.h>
#include <File.h>

#include <Filterables.h>



using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(S1ProdManifestDataExtractor)

namespace {
	const map< string, string > satellitesIdMap = S1ProdManifestDataExtractor::createSatellitesIdMap();
	const string defaultFileClass = "TEST";
	const string defaultQualityFlag = "NOT_AVAILABLE";
}


S1ProdManifestDataExtractor::S1ProdManifestDataExtractor():
	ManifestDataExtractor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}

S1ProdManifestDataExtractor::~S1ProdManifestDataExtractor()  throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}

#define XmlStr( _x_, _p_, _y_) \
		_x_->getValue( _p_, _y_ ); \
		ACS_COND_THROW( _y_.empty(), exS1ProdManifestDataExtractorException(string("Empty value in Manifest for [")+_p_+"]") );


void S1ProdManifestDataExtractor::doWork(string const & manifestFilepath, SafePreInventory::ManifestData & manifestData) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

	ACS_COND_THROW( !File::exists(manifestFilepath), exS1ProdManifestDataExtractorException("\"" + manifestFilepath + "\" file does not exist!") );

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to extract metadata from \"" << manifestFilepath << "\" manifest file" );

	XmlDocSptr x( new XmlDoc );
	x->setParameters(XmlDoc::parDefault | XmlDoc::parSetRootNs).initWithFile( manifestFilepath );

	// in RAW data no namespace occurs
	XmlStr( x, "//xmlData/*[local-name() = 'acquisitionPeriod']/*[local-name() = 'startTime']/text()", manifestData.startTime);
	XmlStr( x, "//xmlData/*[local-name() = 'acquisitionPeriod']/*[local-name() = 'stopTime']/text()", manifestData.stopTime);

	try {
		XmlStr( x, "//xmlData/*[local-name() = 'processing']/@stop", manifestData.creationTime);
	}
	catch( exception &e )
	{
		ACS_LOG_NOTIFY_EX(e);
// use metadata Quality_Info to write
		pair<string, string> p( "Quality_Info", "Cannot extract generation time: using current time");
		manifestData.additionalData.push_back(p);
	}
	
	// the following 2 fields do not occur in RAW data
	try {
		XmlStr( x, "//xmlData/*[local-name() = 'processing']/*[local-name() = 'facility']/*[local-name() = 'software']/@name", manifestData.softwareName);
	}
	catch( XmlDoc::exXmlDocNotFoundException &e )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, e.what() );
	}
	try {
		XmlStr( x, "//xmlData/*[local-name() = 'processing']/*[local-name() = 'facility']/*[local-name() = 'software']/@version", manifestData.softwareVersion);
	}
	catch( XmlDoc::exXmlDocNotFoundException &e )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, e.what() );
	}
	
	XmlStr( x, "//xmlData/*[local-name() = 'platform']/*[local-name() = 'instrument']/*[local-name() = 'familyName']/@abbreviation", manifestData.instrumentName );
			
	vector<string> footPrintPosListV;
	const string footPrintXpath = "//xmlData/*[local-name() = 'frameSet']/*[local-name() = 'frame']/*[local-name() = 'footPrint']/*[local-name() = 'coordinates']/text()";
	XmlStr( x, footPrintXpath, footPrintPosListV );
	if( footPrintPosListV.size() > 1 ) {
	// in test product could occur mone than one frame but it is an anomaly.
		ACS_LOG_WARNING("Found more than one value matching xpath \"" << footPrintXpath << "\". Taking the first one");
	}		
	manifestData.footPrintPosList = footPrintPosListV[0];

	// extract mission e.g. S1A, S1B
	{
		string platformFamilyName;
		XmlStr( x, "//xmlData/*[local-name() = 'platform']/*[local-name() = 'familyName']/text()", platformFamilyName);
		string platformNumber;
		XmlStr( x, "//xmlData/*[local-name() = 'platform']/*[local-name() = 'number']/text()", platformNumber);

		string satelliteName = platformFamilyName + platformNumber;
		map<string, string>::const_iterator it = satellitesIdMap.find(StringUtils::uppercase(satelliteName));
		ACS_COND_THROW(it == satellitesIdMap.end(), exS1ProdManifestDataExtractorException("Can't find [" + StringUtils::uppercase(satelliteName) + "] key in satellites map"));
		manifestData.mission = it->second;
	}

	XmlStr( x, "//xmlData/*[local-name() = 'orbitReference']/*[local-name() = 'orbitNumber'][@type='start']/text()", manifestData.startOrbitNumber );
	XmlStr( x, "//xmlData/*[local-name() = 'orbitReference']/*[local-name() = 'orbitNumber'][@type='stop']/text()", manifestData.stopOrbitNumber );
	
	
	const string rootConfSpaceId = "PreInventory." + getConfSpaceId();
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Root configuration space id=\"" << rootConfSpaceId << '\"' );
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	
	manifestData.fileClass = defaultFileClass;
	string tag = rootConfSpaceId + ".FileClass";
	if( rs.hasKey(tag) ) {
		rs.getValue( tag, manifestData.fileClass);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << ": \"" << manifestData.fileClass << '\"');
	}

	manifestData.qualityFlag = defaultQualityFlag;
	tag = rootConfSpaceId + ".QualityFlag";
	if( rs.hasKey(tag) ) {
		rs.getValue( tag, manifestData.qualityFlag);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << ": \"" << manifestData.qualityFlag << '\"');
	}

}


_ACS_END_NAMESPACE

