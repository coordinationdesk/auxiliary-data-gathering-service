/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Sentinel-3 ADF reformatter library $

	$Id$

	$Author$

	$Log$
	Revision 1.16  2015/09/10 16:35:55  nicvac
	S3MPM-621: implementation
	
	Revision 1.15  2015/03/31 08:36:48  matteo.airoldi
	Manifest aligned with Metadata Specification 3.06
	
	Revision 1.14  2014/06/25 10:29:41  matteo.airoldi
	Update manifest file to match "S3IPF PDS 008 - i3r0 4 - Metadata Specification"
	
	Revision 1.13  2014/06/11 14:50:51  matteo.airoldi
	Management of GenerationCentre at filetype level  implemented
	
	Revision 1.12  2014/03/27 11:17:11  matteo.airoldi
	Added name attribute to processing section
	
	Revision 1.11  2014/02/25 15:19:49  matteo.airoldi
	Added _ as mission = NULL
	
	Revision 1.10  2014/02/13 13:11:37  matteo.airoldi
	Manifest fixed as per IPF integration problems
	
	Revision 1.9  2013/10/04 09:51:03  matteo.airoldi
	removed ifdef for AuxV2 management
	
	Revision 1.8  2013/07/12 12:58:18  marfav
	Added file size in dataObject manifest section
	Dead code ifdeffed (to be removed)
	
	Revision 1.7  2013/07/08 15:27:45  marfav
	Typo fixed in quality info preamble
	Number field added and ifdeffed
	
	Revision 1.6  2013/06/14 10:11:33  marfav
	Updated to PDSV2 reference schema
	
	Revision 1.5  2013/06/11 14:56:09  marfav
	Manifest format updated to follow examples S3IPF_L0_01.02 and applicable excel
	
	Revision 1.4  2013/06/11 08:49:18  marfav
	First issue of the manifest completed
	
	Revision 1.3  2013/06/10 17:04:47  marfav
	Manifest creation in progress
	
	Revision 1.2  2013/06/10 16:08:16  marfav
	Manifest generation in progress
	
	Revision 1.1.1.1  2013/06/10 12:44:59  marfav
	importing libS3AuxFormatter
	

*/

#include <S3AuxManifest.h>

#include <XMLOstream.h>
#include <File.h>

#include <Filterables.h>

#include <s3afV.h>


namespace {
	static const char* pointerName = "ADFData";
};

using namespace acs;
using namespace std;

static s3afV version;

ACS_CLASS_DEFINE_DEBUG_LEVEL (acs::S3AuxManifest) ;

S3AuxManifest::S3AuxManifest() :
	_dataObjects(),
	_ftypeDescMap(),
	//_acqPeriod(),
	_procInfo(),
	_genInfo()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "CTOR called") ;
}

S3AuxManifest::~S3AuxManifest() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DTOR called") ;
}


void S3AuxManifest::setDescriptorMap (const FTypeDescMap& m)
{
	_ftypeDescMap = m;
}

void S3AuxManifest::addDataObject (const DataObjectDescriptor& v)
{
	_dataObjects.push_back( v );
}

#ifdef PART_TO_BE_REMOVED

void S3AuxManifest::setAcqPeriod (const AcqPeriod& a)
{
	_acqPeriod = a;
}

void S3AuxManifest::buildAcquisitionPeriodSection (XMLOstream& xos) const
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Writing Acquisition Period section") ;

	// Preamble
	METADATA_OBJECT_PREAMBLE ("acquisitionPeriod", "Acquisition Period");

	// acquisition period tag
	XMLOstream::Tag tag3 (xos, "acquisitionPeriod", "", "xmlns=\"http://www.esa.int/safe/1.3\"");

	// finally start and stop closing inline
	{ XMLOstream::Tag tag4 (xos, "startTime", _acqPeriod.startTime, "", true); }
	{ XMLOstream::Tag tag5 (xos, "stopTime", _acqPeriod.stopTime, "", true); }


	// Done!
}


void S3AuxManifest::buildPlatformSection (XMLOstream& xos) const
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Writing Platform section") ;

	// Preamble
	METADATA_OBJECT_PREAMBLE ("platform", "Platform Description");

	// platform tag
	XMLOstream::Tag tag3 (xos, "platform", "", "xmlns=\"http://www.esa.int/safe/1.3\"");

	// Instrument
	XMLOstream::Tag tag4 (xos, "instrument");

	// Leafs in the end
	{ XMLOstream::Tag tag5 (xos, "familyName", "Auxiliary Data File", "abbreviation=\"AUX\"", true); }
	{ XMLOstream::Tag tag6 (xos, "mode", "10", "", true); }
	
	// Done!
}

#define METADATA_OBJECT_PREAMBLE(ID_, TextInfo_)    \
	ostringstream attros_; \
	attros_ << "ID=\"" << ID_ << "\""; \
	XMLOstream::Tag rootTag (xos, "metadataObject", "", attros_.str()); \
	attros_.str(""); \
	attros_ << "mimeType=\"text/xml\" "; \
	attros_ << "vocabularyName=\"Sentinel-SAFE\" "; \
	attros_ << "textInfo=\"" << TextInfo_ << "\""; \
	XMLOstream::Tag tag1 (xos, "metadataWrap", "", attros_.str()); \
	XMLOstream::Tag tag2 (xos, "xmlData"); 



#endif //PART_TO_BE_REMOVED


void S3AuxManifest::setProcessingInfo (const ProcessingInfo& i)
{
	_procInfo = i;
}

void S3AuxManifest::setGeneralInfo (const GeneralInfo& g)
{
	_genInfo = g;
}

void S3AuxManifest::writeXMLManifest (const string& path) const
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Writing XML manifest in path [" << path << "]") ;
	try
	{
		ofstream ofs (path.c_str());
		XMLOstream xos (ofs, "xfdu:XFDU", "xmlns:xfdu=\"urn:ccsds:schema:xfdu:1\" xmlns:sentinel-safe=\"http://www.esa.int/safe/sentinel/1.1\" xmlns:sentinel-3=\"http://www.esa.int/safe/sentinel/sentinel-3\" xmlns:sentinel3aux=\"http://www.esa.int/safe/sentinel/sentinel-3/adf/1.0\" version=\"esa/safe/sentinel/sentinel-3/adf/1.0\"");

		// First step is the Information package map
		buildInformationPackageMap (xos);

		// Everything in the middle here!
		{
			// Open the metadata section tag
			XMLOstream::Tag tag1 (xos, "metadataSection");

			// Build the general info section
			buildGeneralInfoSection (xos);
			// Build the processing section
			buildProcessingSection (xos);
			// Build the quality section
			buildQualitySection (xos);
		}

		// Last section is the data object setcion
		buildDataObjectSection (xos);

	}
	catch (exception& ex)
	{
		throwException (S3AuxManifestException (ex, "Unable to write XML manifest."));
	}
}


// Subsection of the Manifest
// written using internal class attributes
void S3AuxManifest::buildInformationPackageMap (XMLOstream& xos) const
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Writing Information Package Map section") ;

	// Gather info on the filetype and detect misconfiguration
	FTypeDescriptor ftd;

	ACS_COND_THROW( _dataObjects.empty(),
			S3AuxManifestException("Data Objects empty. Expected "));


	string filetype(""); //Filetype is homogeneous
	try	{
		filetype = _dataObjects.empty() ? "" : _dataObjects.at(0).fileType ;
		ftd = _ftypeDescMap.at( filetype );
	
	} catch (exception& ex) {
		ostringstream os;
		os << "Unable to get info on file type [" << filetype << "]. Check configuration.";
		throwException (S3AuxManifestException (ex, os.str()));
	}	

	// The root tag
	XMLOstream::Tag rootTag (xos, "informationPackageMap");

	ostringstream attros;
	// collect all the attributes of the root section tag
	attros << "unitType=\"Information Package\" ";
	// This is the converted filetype
	// gained through the LUT _ftypeDescMap converting input ftypes in output ftypes
//	attros << "textInfo=\"" << ftd.fileType << "\" "; 
	attros << "textInfo=\"SENTINEL-3 ADF Package\" "; 
	attros << "pdiID=\"processing\" ";
	attros << "dmdID=\"generalProductInformation measurementQualityInformation processing\" ";
	attros << "ID=\"packageUnit\"";

	// the first contentUnit
	XMLOstream::Tag tag1 (xos, "xfdu:contentUnit", "", attros.str());

	// there are nested content unit
	// reuse the ostringstream
	
	// Set the pointer names
	for ( size_t i(0); i<_dataObjects.size(); ++i ) {

		ostringstream currPointerName; currPointerName<<pointerName;
		if ( _dataObjects.size() > 1 ) {
			currPointerName<< setfill('0') << setw(3) << i+1;
		}

		attros.str("");
	//	attros << "unitType=\"" << ftd.description << "\" ";
		attros << "unitType=\"Annotation Data Unit\" ";
		attros << "repID=\""<<currPointerName.str()<<"\" ";
		attros << "dmdID=\""<<currPointerName.str()<<"\" ";
		attros << "ID=\"ADFUnit\"";
		// Open the second TAG
		XMLOstream::Tag tag2 (xos, "xfdu:contentUnit", "", attros.str());

		XMLOstream::Tag tagx (xos, "dataObjectPointer", "", string("dataObjectID=\"") + currPointerName.str() + "\"", true);
	}

}


void S3AuxManifest::buildDataObjectSection (XMLOstream& xos) const
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Writing Data Object section") ;

	for ( size_t i(0); i<_dataObjects.size(); ++i ) {

		ostringstream currPointerName; currPointerName<<pointerName;
		if ( _dataObjects.size() > 1 ) {
			currPointerName<< setfill('0') << setw(3) << i+1;
		}

		// The root tag
		XMLOstream::Tag rootTag (xos, "dataObjectSection");

		ostringstream attros;

		// collect attributes
		attros << "ID=\"" << currPointerName.str() << "\" ";
		attros << "repID=\"" << currPointerName.str() << "\"";
		XMLOstream::Tag tag1 (xos, "dataObject", "", attros.str());

		attros.str("");
		attros << "mimeType=\"application/octetstream\" ";
		attros << "size=\"" << _dataObjects.at(i).productSize << "\"";
		XMLOstream::Tag tag2 (xos, "byteStream", "", attros.str());

		// reuse the ostringstream
		attros.str("");
		attros << "locatorType=\"URL\" ";
		attros << "textInfo=\"" << _dataObjects.at(i).fileType << "\" ";
		attros << "href=\"" << _dataObjects.at(i).URL << "\"";
		{ XMLOstream::Tag tag3 (xos, "fileLocation", "", attros.str(), true); }
		{ XMLOstream::Tag tag4 (xos, "checksum", _dataObjects.at(i).MD5Signature, "checksumName=\"MD5\"", true); }

	}

	// Done!
}

// Macro definition for MetadataObject wrapping structure
#define METADATA_OBJECT_PREAMBLE2(ID_, Class_, Cat_, TextInfo_)    \
	ostringstream attros_; \
	attros_ << "ID=\"" << ID_ << "\" "; \
	attros_ << "classification=\"" << Class_ << "\" "; \
	attros_ << "category=\"" << Cat_ << "\""; \
	XMLOstream::Tag rootTag (xos, "metadataObject", "", attros_.str()); \
	attros_.str(""); \
	attros_ << "mimeType=\"text/xml\" "; \
	attros_ << "vocabularyName=\"Sentinel-SAFE\" "; \
	attros_ << "textInfo=\"" << TextInfo_ << "\""; \
	XMLOstream::Tag tag1 (xos, "metadataWrap", "", attros_.str()); \
	XMLOstream::Tag tag2 (xos, "xmlData"); 

void S3AuxManifest::buildGeneralInfoSection (XMLOstream& xos) const
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Writing Product General Information section") ;

	// Preamble
	METADATA_OBJECT_PREAMBLE2 ("generalProductInformation", "DESCRIPTION", "DMD", "General Product Information");

	//Filetype is homogeneous
	string currFiletype = _dataObjects.empty() ? "" : _dataObjects.at(0).fileType ;

	XMLOstream::Tag genTag (xos, "sentinel3aux:generalProductInformation");
	{ XMLOstream::Tag tag1 (xos, "sentinel3aux:fileName", _genInfo.fileName, "", true); }
	{ XMLOstream::Tag tag1 (xos, "sentinel3aux:fileType", _ftypeDescMap.at( currFiletype ).fileType, "", true); }
	{ XMLOstream::Tag tag1 (xos, "sentinel3aux:timeliness", _ftypeDescMap.at( currFiletype ).timeliness, "", true); }
	{ XMLOstream::Tag tag1 (xos, "sentinel3aux:baselineCollection", _genInfo.baselineCollection, "", true); }
	{ XMLOstream::Tag tag1 (xos, "sentinel3aux:familyName", "Sentinel-3", "", true); } // This is hardcoded by specifications
	// Extract the third char of the file name and use it as "number"
	string missionNumber = _genInfo.fileName.substr (2,1) ;

#ifdef MISSION_NULL
	if (missionNumber == "_")
		{ XMLOstream::Tag tag1 (xos, "sentinel3aux:number", "", "", true); }
	else
		{ XMLOstream::Tag tag1 (xos, "sentinel3aux:number", missionNumber, "", true); }
#else
	{ XMLOstream::Tag tag1 (xos, "sentinel3aux:number", missionNumber, "", true); }
#endif

	{ XMLOstream::Tag tag1 (xos, "sentinel3aux:productSize", _genInfo.productSize, "", true); }
	{ XMLOstream::Tag tag1 (xos, "sentinel3aux:creationTime", _genInfo.creationTime, "", true); }
	{ XMLOstream::Tag tag1 (xos, "sentinel3aux:validityStartTime", _genInfo.validityStartTime, "", true); }
	{ XMLOstream::Tag tag1 (xos, "sentinel3aux:validityStopTime", _genInfo.validityStopTime, "", true); }

}

void S3AuxManifest::buildProcessingSection (XMLOstream& xos) const
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Writing Processing section") ;

	// Preamble
	METADATA_OBJECT_PREAMBLE2 ("processing", "PROVENANCE", "PDI", "Processing");

	ostringstream attros;

	// create a tag with attributes
	//attros << "start=\"" << _procInfo.processingStart << "\" ";
	//attros << "xmlns=\"http://www.esa.int/safe/1.3\"";
	attros.str("");
	attros << "name=\"ADC_AUX_REFORMATTER\" ";
	XMLOstream::Tag tag3 (xos, "sentinel-safe:processing", "", attros.str());

	{
		// create another tag "facility" with nested tags
		attros.str("");
		attros << "name=\"" << _procInfo.facilityName << "\" ";
		attros << "organisation=\"" << _procInfo.organization << "\" ";
		attros << "site=\"" << _procInfo.site << "\" ";
		attros << "country=\"" << _procInfo.country << "\"";
		XMLOstream::Tag tag4 (xos, "sentinel-safe:facility", "", attros.str());

		// create hardware tag
		attros.str("");
		attros << "name=\"" << _procInfo.hwname << "\"";
		{ XMLOstream::Tag tag5 (xos, "sentinel-safe:hardware", "", attros.str(), true); }

		// create software tag
		attros.str("");
		attros << "name=\"" << _procInfo.swname << "\" ";
		attros << "version=\"" << _procInfo.swversion << "\"";
		{ XMLOstream::Tag tag6 (xos, "sentinel-safe:software", "", attros.str(), true); }
	}
	
	// Facility is closed here
	// create resource tag
	for ( vector< pair <string, string> >::const_iterator it=_procInfo.resources.begin(); it!=_procInfo.resources.end(); ++it ) {
		pair<string, string> const& procRes = *it;
		attros.str("");
		attros << "name=\"" << File::getFileName (procRes.first ) << "\" ";
		attros << "role=\"Raw Data\" ";
		attros << "version=\""<< procRes.second <<"\"";

		{ XMLOstream::Tag tag7 (xos, "sentinel-safe:resource", "", attros.str(), true); }
	}

}

void S3AuxManifest::buildQualitySection (XMLOstream& xos) const
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Writing Quality Information section") ;

	// Preamble
	METADATA_OBJECT_PREAMBLE2 ("measurementQualityInformation", "DESCRIPTION", "DMD", "Quality Information");
	XMLOstream::Tag qtag (xos, "sentinel-safe:qualityInformation");
	XMLOstream::Tag qtag2 (xos, "sentinel-safe:extension");
	XMLOstream::Tag qtag3 (xos, "sentinel3aux:adfQuality");
	// platform tag
	string qualityInfo = _dataObjects.empty() ? "" : _dataObjects.at(0).qualityInfo ;
	{ XMLOstream::Tag tag3 (xos, "sentinel3aux:adfQualityCheck", qualityInfo, "", true); }
}



