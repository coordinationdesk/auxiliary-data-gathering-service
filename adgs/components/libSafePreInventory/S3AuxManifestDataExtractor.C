// PRQA S 1050 EOF
/*

	Copyright 2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:  $

	$Id$

	$Author$

	$Log$
	Revision 1.7  2015/03/12 14:40:31  paocas
	QualityFlag extraction modified
	
	Revision 1.6  2013/11/13 17:21:25  lucio.pulvirenti
	Value of productSize tag does not include manifest size: overall size (product + manifest) saved.
	
	Revision 1.5  2013/10/04 13:26:19  matteo.airoldi
	bug fixed
	
	Revision 1.4  2013/10/04 10:17:56  matteo.airoldi
	Aux Specification V2 implemented
	
	Revision 1.3  2013/07/02 10:27:46  lucio.pulvirenti
	Methods to extract information from Processing section changed. Method to extract information from
	quality information section added.
	
	Revision 1.2  2013/07/01 16:42:30  lucio.pulvirenti
	Work in progress.
	
	Revision 1.1.1.1  2013/06/28 16:29:01  lucio.pulvirenti
	Import libSafePreInventory
	
	

*/

#include <S3AuxManifestDataExtractor.h>

#include <XMLIstream.h>
#include <StringUtils.h>
#include <File.h>
#include <rsResourceSet.h>
#include <Filterables.h>

using namespace acs;
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(S3AuxManifestDataExtractor)

const map< string, string > S3AuxManifestDataExtractor::_classMap = S3AuxManifestDataExtractor::createClassMap();
const map< string, string > S3AuxManifestDataExtractor::_qualityCheckMap = S3AuxManifestDataExtractor::createQualityCheckMap();
const map< string, string > S3AuxManifestDataExtractor::_satellitesIdMap = S3AuxManifestDataExtractor::createSatellitesIdMap();


//Class constructor
S3AuxManifestDataExtractor::S3AuxManifestDataExtractor() :
								ManifestDataExtractor(),
								_manifestFullpath()
{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Id: " << Id);

} 


//Class destructor
S3AuxManifestDataExtractor::~S3AuxManifestDataExtractor() throw()
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");
}


void S3AuxManifestDataExtractor::doWork(string const & manifestFilepath, SafePreInventory::ManifestData & manifestData)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");
	
	throwConditionedException(!File::exists(manifestFilepath), exS3AuxManifestDataExtractorException("\"" + manifestFilepath + "\" file does not exist!") );	 // PRQA S 3081
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to extract metadata from \"" << manifestFilepath << "\" manifest file" );

	// extract data
    XMLIstream xml( manifestFilepath);
	
	// write in resource set	
	rsResourceSet rsSet;
	xml >> rsSet;
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "\"" << manifestFilepath << "\" file written in resource set:\n" << rsSet);

   	int metadataObjects = 1;
	try {
		rsSet.getValue("metadataSection.metadataObject[]",metadataObjects) ;
	}
	catch(rsResourceSet::NotFoundException &)
	{}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Metadata objects=" << metadataObjects );

	string root;
	if( 1 == metadataObjects ) {
		root = "metadataSection.metadataObject";
	}
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "root=\"" << root << "\"" );
	
	_manifestFullpath = manifestFilepath;
	extractGeneralProductInformationData(rsSet, root, metadataObjects, manifestData);
	extractProcessingData(rsSet, root, metadataObjects, manifestData);
	extractQualityInformationData(rsSet, root, metadataObjects, manifestData);
}


void S3AuxManifestDataExtractor::extractGeneralProductInformationData(rsResourceSet const &rsSet, string const &root, int metadataObjects, SafePreInventory::ManifestData &manifestData)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");
	
	string refMetadataObjectId("generalProductInformation");	
	bool found = false;
	for( int i=0; i<metadataObjects && !found; i++) {

		string localRoot = root;
		if( localRoot.empty() ) {
			ostringstream os ;
			os << "metadataSection.metadataObject[" << i << "]" ;
			localRoot = os.str();
		}
		
	   	string metadataObjectId;
	   
	   	{
			string tag = localRoot + ".ID.value";
			rsSet.getValue( tag, metadataObjectId);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << " section=" << metadataObjectId);
		}
	   
	  	if( metadataObjectId != refMetadataObjectId) {
		   continue;
	   	}

		string objectTag = localRoot + ".metadataWrap.xmlData.generalProductInformation";
	
		{
			string tag = objectTag + ".fileName";
		   	rsSet.getValue( tag, manifestData.filename);
		   	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << " value=" << manifestData.filename);
		}
	   
		{
			string tag = objectTag + ".fileType";
		   	rsSet.getValue( tag, manifestData.filetype);
		   	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << " value=" << manifestData.filetype);
		}
	   
		{
			string tag = objectTag + ".familyName";
			string tmp;
		   	rsSet.getValue( tag, tmp);
		   	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << " value=" << tmp);
			string tag1 = objectTag + ".number";
			string tmp1;
		   	rsSet.getValue( tag1, tmp1);
		   	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag1 << " value=" << tmp1);

			if (!tmp1.empty())
				tmp = tmp+tmp1 ;

			map<string, string>::const_iterator it = _satellitesIdMap.find(StringUtils::uppercase(tmp));
			throwConditionedException(it == _satellitesIdMap.end(), exS3AuxManifestDataExtractorException("Can't find \"" + StringUtils::uppercase(tmp) + "\" key in satellites map"));	 // PRQA S 3081
			manifestData.mission = it->second;
		}
	   
		{
			string tag = objectTag + ".productSize";
		   	rsSet.getValue( tag, manifestData.productSize);
		   	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << " value=" << manifestData.productSize);
			
// in this case the productSize value does not include manifest own size.
			size_t totalProductSize = 0;
			istringstream iss(manifestData.productSize);
			iss >> totalProductSize;
			totalProductSize += File::size(_manifestFullpath);
			
			ostringstream os;
			os << totalProductSize;
			manifestData.productSize = os.str();
			
		   	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Total size included manifest file=" << manifestData.productSize);
		}
	   
		{
			string tag = objectTag + ".validityStartTime";
		   	rsSet.getValue( tag, manifestData.startTime);
		   	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << " value=" << manifestData.startTime);
		}
		
		{
			string tag = objectTag + ".validityStopTime";
		   	rsSet.getValue( tag, manifestData.stopTime);
		   	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << " value=" << manifestData.stopTime);
		}
		
		{
			string tag = objectTag + ".creationTime";
		   	rsSet.getValue( tag, manifestData.creationTime);
		   	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << " value=" << manifestData.creationTime);
		}

		found = true;
	   
	}
	
	throwConditionedException(!found, exS3AuxManifestDataExtractorException( refMetadataObjectId + " object not found in manifest file!") );
	
}


void S3AuxManifestDataExtractor::extractProcessingData(rsResourceSet const &rsSet, string const &root, int metadataObjects, SafePreInventory::ManifestData &manifestData)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");
	
	string refMetadataObjectId("processing");	
	bool found = false;
	for( int i=0; i<metadataObjects && !found; i++) {

		string localRoot = root;
		if( localRoot.empty() ) {
			ostringstream os ;
			os << "metadataSection.metadataObject[" << i << "]" ;
			localRoot = os.str();
		}
		
	   	string metadataObjectId;
	   
	   	{
			string tag = localRoot + ".ID.value";
			rsSet.getValue( tag, metadataObjectId);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << " section=" << metadataObjectId);
		}
	   
	  	if( metadataObjectId != refMetadataObjectId) {
		   continue;
	   	}

		string objectTag = localRoot + ".metadataWrap.xmlData.processing";
	
		{
			string tag = objectTag + ".facility.name.value";
		   	rsSet.getValue( tag, manifestData.facilityName);
		   	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << " value=" << manifestData.facilityName);
		}
	   
		{
			string tag = objectTag + ".facility.software.name.value";
		   	rsSet.getValue( tag, manifestData.softwareName);
		   	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << " value=" << manifestData.softwareName);
		}
	   
		{
			string tag = objectTag + ".facility.software.version.value";
		   	rsSet.getValue( tag, manifestData.softwareVersion);
		   	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << " value=" << manifestData.softwareVersion);
		}

		{
			string tag = objectTag + ".facility.hardware.name.value";
			string tmp;
		   	rsSet.getValue( tag, tmp);
		   	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << " value=" << tmp);
			
			map<string, string>::const_iterator it = _classMap.find(StringUtils::uppercase(tmp));
			throwConditionedException(it == _classMap.end(), exS3AuxManifestDataExtractorException("Can't find \"" + StringUtils::uppercase(tmp) + "\" key in class map"));	 // PRQA S 3081
			manifestData.fileClass = it->second;
		}
		
		found = true;
	   
	}
	
	throwConditionedException(!found, exS3AuxManifestDataExtractorException( refMetadataObjectId + " object not found in manifest file!") );
	
}

void S3AuxManifestDataExtractor::extractQualityInformationData(rsResourceSet const &rsSet, string const &root, int metadataObjects, SafePreInventory::ManifestData &manifestData)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");
	
	string refMetadataObjectId1("qualityInformation");	
	string refMetadataObjectId2("measurementQualityInformation");

	bool found = false;
	for( int i=0; i<metadataObjects && !found; i++) {

		string localRoot = root;
		if( localRoot.empty() ) {
			ostringstream os ;
			os << "metadataSection.metadataObject[" << i << "]" ;
			localRoot = os.str();
		}
		
	   	string metadataObjectId;
	   
	   	{
			string tag = localRoot + ".ID.value";
			rsSet.getValue( tag, metadataObjectId);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << " section=" << metadataObjectId);
		}
	   
	  	if( metadataObjectId != refMetadataObjectId1 && metadataObjectId != refMetadataObjectId2) {
		   continue;
	   	}

		string objectTag = localRoot + ".metadataWrap.xmlData.qualityInformation";
	
		{
			string tag = objectTag + ".adfQualityCheck";
			string tag1 = objectTag + ".extension.adfQuality.adfQualityCheck";
			string tmp;
            		if (rsSet.hasKey(tag))
			{
		   		rsSet.getValue( tag, tmp);
		   		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag << " value=" << tmp);
			}
			else if (rsSet.hasKey(tag1))
			{
		   		rsSet.getValue( tag1, tmp);
		   		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << tag1 << " value=" << tmp);
			}
			else
			{
				ostringstream msg;
				msg << "Cannot find adfQualityCheck tag in Manifest File";
				exS3AuxManifestDataExtractorException e( msg.str());
				throwException(e);
			}

			map<string, string>::const_iterator it = _qualityCheckMap.find(StringUtils::uppercase(tmp));
			throwConditionedException(it == _qualityCheckMap.end(), exS3AuxManifestDataExtractorException("Can't find \"" + StringUtils::uppercase(tmp) + "\" key in quality check map"));	 // PRQA S 3081
			manifestData.qualityFlag = it->second;
		}
		found = true;
	}
	
	throwConditionedException(!found, exS3AuxManifestDataExtractorException( "Neither " + refMetadataObjectId1 + " nor " + refMetadataObjectId2 + " objects are found in manifest file!") );
	
}
