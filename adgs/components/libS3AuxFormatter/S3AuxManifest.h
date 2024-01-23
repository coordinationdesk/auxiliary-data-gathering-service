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
	Revision 1.9  2015/09/10 16:35:55  nicvac
	S3MPM-621: implementation
	
	Revision 1.8  2015/09/09 16:19:11  nicvac
	S3MPM-621: input uncompress case.
	
	Revision 1.7  2014/06/11 14:50:51  matteo.airoldi
	Management of GenerationCentre at filetype level  implemented
	
	Revision 1.6  2013/07/12 12:58:18  marfav
	Added file size in dataObject manifest section
	Dead code ifdeffed (to be removed)
	
	Revision 1.5  2013/06/14 10:11:33  marfav
	Updated to PDSV2 reference schema
	
	Revision 1.4  2013/06/11 08:49:18  marfav
	First issue of the manifest completed
	
	Revision 1.3  2013/06/10 17:04:47  marfav
	Manifest creation in progress
	
	Revision 1.2  2013/06/10 16:08:16  marfav
	Manifest generation in progress
	
	Revision 1.1.1.1  2013/06/10 12:44:59  marfav
	importing libS3AuxFormatter
	

*/

#ifndef _S3AuxManifest_H_
#define _S3AuxManifest_H_

#include <exException.h>

#include <string>

namespace acs
{

// Forward declaration
class XMLOstream;

class S3AuxManifest
{
public:

	// Internal exception
	exDECLARE_EXCEPTION(S3AuxManifestException,exException) ;

#ifdef PART_TO_BE_REMOVED
public:
	struct AcqPeriod
	{
		std::string startTime;
		std::string stopTime;
		AcqPeriod() : startTime(), stopTime() {}
	};

	void setAcqPeriod (const AcqPeriod&);

private:
	void buildAcquisitionPeriodSection (XMLOstream&) const;
	void buildPlatformSection (XMLOstream&) const;

	AcqPeriod _acqPeriod;

#endif //PART_TO_BE_REMOVED

public:

	struct GeneralInfo
	{
		std::string fileName;
		std::string baselineCollection;
		size_t      productSize;
		std::string creationTime;
		std::string validityStartTime;
		std::string validityStopTime;

		GeneralInfo() : fileName(), baselineCollection(), productSize(), 
		                creationTime(), validityStartTime(), validityStopTime() {}
	};

	struct DataObjectDescriptor
	{
		std::string repoID;
		std::string fileType;
		std::string URL;
		std::string version;
		std::string MD5Signature;
		std::string qualityInfo;
		size_t      productSize;

		DataObjectDescriptor() : repoID(), fileType(), URL(), version(), MD5Signature(), qualityInfo(), productSize() {}
	};

	struct FTypeDescriptor
	{
		std::string fileType;
		std::string description;
		std::string timeliness; 
		std::string genCentre; 
		bool uncompressInput;

		FTypeDescriptor() : fileType(), description(), timeliness(), genCentre(), uncompressInput(false) {}
	};

	typedef std::map <std::string, FTypeDescriptor> FTypeDescMap;

	struct ProcessingInfo
	{
		std::string processingStart;
		std::string facilityName;
		std::string organization;
		std::string site;
		std::string country;
		std::string swname;
		std::string swversion;
		std::string hwname;
		std::vector< std::pair< std::string, std::string > > resources; //Pairs of filename, fileversion
		ProcessingInfo() : processingStart(), facilityName(), organization(), site(), country(), swname(), swversion(), hwname(), resources() {}
	};

public:
	S3AuxManifest();
	~S3AuxManifest();

	// dump the XML structure on the disk
	void writeXMLManifest (const std::string& path) const;

	// Used the defined structure to proper fill the manifest
	void setDescriptorMap (const FTypeDescMap&);
	void addDataObject (const DataObjectDescriptor&);
	void setProcessingInfo (const ProcessingInfo&);
	void setGeneralInfo (const GeneralInfo&);


private:
	void buildInformationPackageMap (XMLOstream&) const;
	void buildDataObjectSection (XMLOstream&) const;
	void buildGeneralInfoSection (XMLOstream&) const;
	void buildProcessingSection (XMLOstream&) const;
	void buildQualitySection (XMLOstream&) const;

private:
	std::vector<DataObjectDescriptor> _dataObjects;
	FTypeDescMap _ftypeDescMap;
	ProcessingInfo _procInfo;
	GeneralInfo _genInfo;
    ACS_CLASS_DECLARE_DEBUG_LEVEL (S3AuxManifest) ;

};

} // end of namespace acs

#endif //_S3AuxManifest_H_

