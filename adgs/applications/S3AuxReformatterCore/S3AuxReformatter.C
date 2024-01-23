/*

	Copyright 1995-2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Sentinel3 Auxiliary Reformatter application $

	$Id$

	$Author$
	
	$Log$
	Revision 1.21  2015/10/22 14:41:06  matteo.airoldi
	Removed order_id
	
	Revision 1.20  2015/09/11 10:51:03  nicvac
	S3MPM-621: uncompress input option.
	
	Revision 1.19  2015/09/10 16:35:45  nicvac
	S3MPM-621: implementation
	
	Revision 1.18  2015/09/09 16:19:01  nicvac
	S3MPM-621: input uncompress case.
	
	Revision 1.17  2015/05/12 13:30:24  matteo.airoldi
	Added order_id to metadata file for ingestion
	
	Revision 1.16  2014/12/17 11:10:49  matteo.airoldi
	SPR ADC-287 fixed
	
	Revision 1.15  2014/06/11 14:43:10  matteo.airoldi
	GenerationCentre now configured for filetype
	
	Revision 1.14  2014/03/25 11:24:02  matteo.airoldi
	fixed bug. Main always returned 0 ... now it returns the right code
	
	Revision 1.13  2014/02/13 14:40:58  matteo.airoldi
	OME SPR-1490 fixed
	
	Revision 1.12  2013/10/04 14:18:39  matteo.airoldi
	The information about validation is needed in both
	manifest and metadata files.
	
	Revision 1.11  2013/10/04 09:49:09  matteo.airoldi
	quality flag info fixed
	
	Revision 1.10  2013/09/13 14:16:37  marfav
	Added Satellite Id and Mission Number to the extracted metadata
	
	Revision 1.9  2013/07/12 13:00:22  marfav
	Added file size in dataObject section
	QualityFlag remapped as per ADC-130
	
	Revision 1.8  2013/07/08 15:26:34  marfav
	Manifest size not included in xfdumanifest file size attribute
	
	Revision 1.7  2013/06/18 14:40:32  marfav
	Development completed - first issue
	
	Revision 1.6  2013/06/14 11:00:11  marfav
	Size in manifest fixed writing the manifest 2 times
	
	Revision 1.5  2013/06/14 10:33:28  marfav
	Aligned to PDGS V2 schema
	
	Revision 1.4  2013/06/12 14:59:31  marfav
	First issue of ReformatterCore completed
	
	Revision 1.3  2013/06/12 10:53:06  marfav
	Work in progress
	
	Revision 1.2  2013/06/11 17:02:22  marfav
	Work in progress. Started configuraion management
	
	Revision 1.1.1.1  2013/06/11 13:33:29  marfav
	Importing S3AuxReformatterCore

*/

#include <S3AuxReformatter.h>

#include <exMacros.h>
#include <Filterables.h>
#include <OcadaJobOrder.h>

#include <XMLIstream.h>
#include <rsPushNSpace.h>
#include <DateTime.h>
#include <Md5Sum.h>
#include <Metadata.h>

#include <SpawnProc.h>

#include <Dir.h>

#include <S3AuxRefCoreV.h>

#include <boost/algorithm/string.hpp>

using namespace acs;
using namespace std;

namespace {
	const string taskName_ ("S3_AUX_CORE");
	const string convArrayTag_ ("FileTypeConversionMap.List_of_Conversions.Conversion");
	const string inftTag_ ("InputFileType");
	const string outftTag_ ("OutputFileType");
	const string ftdescTag_ ("FileTypeDescription");
	const string timelinessTag_ ("Timeliness");
	const string gencentreTag_ ("GenerationCentre");
	const string uncompressInputTag_ ("UncompressInput");
	const string platformTag_ ("Platform");
	const string baselineTag_ ("Baseline");
	const string facilitynameTag_ ("FacilityName");
	const string organizationTag_ ("Organization");
	const string siteTag_ ("Site");
	const string countryTag_ ("Country");
	const string hwnameTag_ ("HwName");
	const string compnameTag_ ("SwName");
	const string compversionTag_ ("SwVersion");
}

namespace {
	const string filenameKey_ ("ObjectMetadata.FileName");
	const string fileversionKey_ ("ObjectMetadata.FileVersion");
	const string filetypeKey_ ("ObjectMetadata.FileType");
	const string fileclassKey_ ("ObjectMetadata.FileClass");
	const string valstartKey_ ("ObjectMetadata.ValidityStart");
	const string valstopKey_ ("ObjectMetadata.ValidityStop");
	const string satnameKey_ ("ObjectMetadata.SatName");
	const string satIdKey_ ("ObjectMetadata.SatId");
	const string missionKey_ ("ObjectMetadata.Mission");
	const string qualifiedIdTag_ ("ObjectMetadata.QualifiedId");
	const string qualityinfoTag_ ("ObjectMetadata.QualityInfoValue");
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(S3AuxReformatter)

static S3AuxRefCoreV Version;

Application::GenericBuilder<S3AuxReformatter> theApplicationBuilder("S3AuxReformatterCore", "ADC");


//Class constructor
S3AuxReformatter::S3AuxReformatter(const string& appName, const string& subSys) :	
#if   defined (HAS_MISSION_INCLUDES)
PROJECT_STANDALONE_APP(appName, subSys),
#else
StandaloneApp(appName, subSys),
#endif
	_ftypeDescMap(),
	_platform(),
	_baseline(),
	_facilityName(),
	_organization(),
	_site(),
	_country(),
	_hwName(),
	_compName(),
	_compVers(),
	_swName(),
	_swVersion(),
	_ordId(0)
{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"S3AuxReformatter::" << __LINE__ << " Id: " << Id) ;
} 

//Class destructor
S3AuxReformatter::~S3AuxReformatter() throw() 
{  
}

void S3AuxReformatter::usage(int argc,  char const * const *argv) const
{
	excerr << "usage: " << argv[0] << " <JobOrder file> \n";
}

int S3AuxReformatter::main(int argc, char const * const * argv, char const * const * env)		
{																								
	if (argc < 2) 
	{ 
		usage(argc, argv) ;
		return 1 ;
	}
	
	// This application does not use logfiles
	int returnCode = 0;
	try
	{
		string joFileName(argv[1]);
		returnCode = execute(joFileName) ;
	}
	catch(exception& e)
	{
		ACS_LOG_ERROR("Error detected during the processing. Exiting in ERROR.");
		ACS_LOG_NOTIFY_EX(e);

		returnCode = 255;

	} 

	ACS_LOG_INFO ("Application exiting with code [" << returnCode << "]");
	return returnCode;								
}

/////////////////////////////////////////////////////////////////////////////////////////
// Private methods 
/////////////////////////////////////////////////////////////////////////////////////////


void S3AuxReformatter::readPConf (const string& pconfName)
{
	ACS_LOG_INFO ("Parsing Private Config file");
	XMLIstream is (pconfName);
	rsResourceSet rs;
	is >> rs;

	size_t conversions = rs.getArraySize (convArrayTag_);
	ACS_LOG_INFO ("Found [" << conversions << "] conversions in pconf");

	// Temporary storage variables
	string inFtype, outFtype, description, timeliness, genCentre;
	bool uncompressInput(false);
	S3AuxManifest::FTypeDescriptor ftd;

	// Read all of them and build the map
	for (size_t i=0; i<conversions; ++i)
	{
		ostringstream os;
		os << convArrayTag_ << "[" << i << "]";

		// Push the i-th array element
		rsPushNSpace pusher (rs, os.str());

		// And extract its content
		rs.getValue (inftTag_, inFtype);
		rs.getValue (outftTag_, outFtype);
		rs.getValue (ftdescTag_, description);
		rs.getValue (timelinessTag_, timeliness);
		rs.getValue (gencentreTag_, genCentre);
		uncompressInput=false; rs.getValue( uncompressInputTag_, uncompressInput, false); //Not mandatory tag

		// Debug message to monitor what has been read
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Read association [" << inFtype << ", " << outFtype << ", " << description << ", " << timeliness << ", " << genCentre << ", "<< uncompressInput << "]") ;

		// Create the association
		ftd.fileType = outFtype;
		ftd.description = description;
		ftd.timeliness = timeliness;
		ftd.genCentre = genCentre;
		ftd.uncompressInput = uncompressInput;

		// And store it in the map
		_ftypeDescMap [inFtype] = ftd;
	}

	// Get static file attributes (maybe to be changed in dynamic ones)
	rs.getValue (platformTag_, _platform);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Platform set to [" << _platform << "]") ;

	rs.getValue (baselineTag_, _baseline);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Baseline set to [" << _baseline << "]") ;

	rs.getValue (facilitynameTag_, _facilityName);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Facility name set to [" << _facilityName << "]") ;

	rs.getValue (organizationTag_, _organization);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Organization name set to [" << _organization << "]") ;

	rs.getValue (siteTag_, _site);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Site set to [" << _site << "]") ;

	rs.getValue (countryTag_, _country);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Country set to [" << _country << "]") ;

	rs.getValue (hwnameTag_, _hwName);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Hardware platform name set to [" << _hwName << "]") ;

	rs.getValue (compnameTag_, _compName);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Component name set to [" << _compName << "]") ;

	rs.getValue (compversionTag_, _compVers);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Component version set to [" << _compVers << "]") ;
}



int S3AuxReformatter::execute(const string & jobOrderFileName)
{

	ACS_LOG_INFO ("Application start. Initializing.");
	ACS_LOG_INFO ("JobOrder file is [" << jobOrderFileName << "]");

	// Read the jobOrder
	OcadaJobOrder theJobOrder;

	try
	{
		XMLIstream is (jobOrderFileName);
		is >> theJobOrder ;
	}
	catch (exception& e)
	{
		ostringstream os;
		os <<  "Unable to load JobOrder from file  \"" << jobOrderFileName << "\"";
		throwException (S3AuxReformatterException (e, os.str() ));
	}

	// Prepare some metadata here
	_swName = theJobOrder.getProcName();
	_swVersion = theJobOrder.getProcVersion();
	_ordId = theJobOrder.getOrderIdFromFilename(jobOrderFileName);
	ACS_LOG_INFO ("Software info from Joborder is [" << _swName << ", " << _swVersion << ", " << _ordId << "]");


	// extract the relevant task from the jobOrder
	Task task (theJobOrder.getTask(taskName_));
	
	// Check that exactly one input has been provided
	vector<InputDescriptor> idl;
	task.getInputList (idl);

	if (1 != idl.size())
	{
		ostringstream os;
		os <<  "Too many inputs [" << idl.size() << "] in task [" << taskName_ << "] - JobOrder is \"" << jobOrderFileName << "\"";
		throwException (S3AuxReformatterException (os.str() ));
	}
	
	// Get the unique alternative and extract the input file name
	InputAlternative alt ( *(static_cast<InputAlternative*> ((idl.at(0))[0])) );

	vector<string> inputList;
    alt.getPathNames(inputList);

	// Just one input is expected now
	if (1 != inputList.size())
	{
		ostringstream os;
		os <<  "Too many inputs [" << idl.size() << "] in task [" << taskName_ << "] - JobOrder is \"" << jobOrderFileName << "\"";
		throwException (S3AuxReformatterException (os.str() ));
	}

	string inputName (inputList.at(0));

	// The input file name is now known
	ACS_LOG_INFO ("Input file name is [" << inputName << "]");
	
	// Start reading the configuration now
	string pconfpath (theJobOrder.getConfig());
	ACS_LOG_INFO ("Private Config file name is [" << pconfpath << "]");

	try
	{
		readPConf (pconfpath);
	}
	catch (exception& e)
	{
		ostringstream os;
		os <<  "Unable to read the private config file [" << pconfpath << "]";
		throwException (S3AuxReformatterException (e, os.str() ));
	}
	
	// Everything is now ready to create the new object
	try
	{
		// Create the package, the MTD file and the LIST file
		createSafePackage (inputName);
		// ready to go in inventory
	}
	catch (exception& e)
	{
		ostringstream os;
		os <<  "There was an error during the input processing. Operation failed.";
		throwException (S3AuxReformatterException (e, os.str() ));
	}

	// If here no problems occurred. So return 0.
	return 0;

}


// Naming convention of the generated product
// S3x_fffffffffff_20150118T180000_20150119T060000_20150118T055246_uuuuuuuuuuuuuuuuu_GGG_P_XX_NNN.SEN3
// x = A, B or _ (depends on the satellite/mission set to the input aux data)
// fffffffffff = filetype 11 chars
// Times are 3 x 15 char plus underscores (start, stop, generation)
// uuuuuuuuuuuuuuuuu = 17 underscores
// GGG = generation centre (___ if not relevant)
// 
// P_XX_NNN, where
// P = 1 uppercase letter indicating the platform. O for operational, F for reference, D for
//     development, R for reprocessing or 1 underscore “_” if not relevant.
// XX = 2 uppercase letters/digits indicating the applicability of the file in terms of timeliness.
//      NR for NRT, ST for STC, NT for NTC, SN for files used on both STC & NTC, NS for files
//      used on both NRT & STC, NN for files used on both NRT & NTC, AL for files used for
//      NRT & STC & NTC or 2 underscores “_” if not relevant.
// NNN = 3 letters/digits. Free text for indicating the baseline collection (001, 002 ….) or
//       data usage (e.g. test, GSV, etc) or 3 underscores “_” if not relevant.

string S3AuxReformatter::invTime2SafeNameTime (const string& in) const
{
	// Transform the format 2013-03-30T22:29:57.000000 to the new one 20130330T222957
	//                      01234567890123456789012345

	return ( in.substr (0, 4) + in.substr (5,2) + in.substr (8,5) + in.substr (14,2) + in.substr(17,2) );
}

static inline bool _doUnTgz( string const& inFile, string const& wherePath ) {

	SpawnProc tar("tar");

	int countArg(0);

	tar.args()[ countArg++ ] = string("xvfz");
	tar.args()[ countArg++ ] = inFile;
	tar.args()[ countArg++ ] = string("-C");
	tar.args()[ countArg++ ] = wherePath;

	tar.spawn();
	tar.waitProcCompletion();

	int exitCode;

	return ( tar.exitCode(exitCode) && 0==exitCode );
}

static inline bool _doUnZip( string const& inFile, string const& wherePath ) {

	//Create zip
	SpawnProc zip("unzip");

	int countArg(0);

	zip.args()[ countArg++ ] = inFile;
	zip.args()[ countArg++ ] = string("-d");
	zip.args()[ countArg++ ] = wherePath;

	zip.spawn();
	zip.waitProcCompletion();

	int exitCode;

	return ( zip.exitCode(exitCode) && 0==exitCode );
}


void S3AuxReformatter::fillSafePackage( string const& packagePath, string const& inputName, bool uncompressInput, vector<string>& packageContent ) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ACS_LOG_INFO("Creating the package directory") ;
	File::mkdir (packagePath);

	if ( uncompressInput ) {
		ACS_LOG_INFO("Uncompressing the input product ["<<inputName<<"] in the package ["<<packagePath<<"]") ;

		bool uncompressed=false;
		if ( boost::to_lower_copy( File::getExt(inputName) ) == "zip"  ) {
			uncompressed = _doUnZip( inputName, packagePath );
		} else {
			uncompressed = _doUnTgz( inputName, packagePath );
		}

		ACS_COND_THROW( !uncompressed,
				S3AuxReformatterException("Error uncompressing ["+inputName+"] in path ["+packagePath+"]"));

	} else {
		ACS_LOG_INFO("Hard-Linking the input product ["<<inputName<<"] in the package ["<<packagePath<<"]") ;
		// Now create a link of the input file inside the safe package

		File::hardlink (inputName, packagePath+"/"+ File::getFileName(inputName) );
	}

	Dir packDir( packagePath );
	for (size_t i(0); i<packDir.size(); ++i) {

		ACS_COND_THROW( ! File::isRegularFile( packagePath +"/"+ packDir[i] ),
				S3AuxReformatterException("Package ["+packagePath+"] expected to contain regular files only. ["+packDir[i]+"] is not a regular file." ) );

		packageContent.push_back( packDir[i] );
	}

}

void S3AuxReformatter::createSafePackage (const string& inputName)
{
	// Open the metadata file and put it into an InventoryObject istance
	string invobjname = inputName + ".metadata";
	ACS_LOG_INFO ("Loading metadata info [" << invobjname << "]");

	XMLIstream is (invobjname);
	rsResourceSet rs;
	is >> rs;

	// Start to extract information
	// we need:
	// - filetype
	// - start
	// - stop
	// - satellite (to apply to one sat or both)


	// FileType extraction
	string fileType;
	rs.getValue (filetypeKey_, fileType);

	// FileType token is extracted form the conversion map
	string filetypeToken (_ftypeDescMap[fileType].fileType);
	ACS_LOG_INFO ("FileType is [" << fileType << "] mapped to [" << filetypeToken << "]");

	string timelinessToken (_ftypeDescMap[fileType].timeliness);
	ACS_LOG_INFO ("Timeliness is [" << timelinessToken << "]");

	string genCentreToken (_ftypeDescMap[fileType].genCentre);
	ACS_LOG_INFO ("Genereation Centre is [" << genCentreToken << "]");


	// FileClass
	string fileClass;
	rs.getValue (fileclassKey_, fileClass);	
	ACS_LOG_INFO ("FileClass is [" << fileClass << "]");

	// ValidityStart extraction
	string valStart;
	rs.getValue (valstartKey_, valStart);
	string valStartNameToken ( invTime2SafeNameTime (valStart) );
	ACS_LOG_INFO ("Validity start [" << valStart << "] mapped to [" << valStartNameToken << "]");

	// ValidityStop extraction
	string valStop;
	rs.getValue (valstopKey_, valStop);
	string valStopNameToken ( invTime2SafeNameTime (valStop) );
	ACS_LOG_INFO ("Validity start [" << valStop << "] mapped to [" << valStopNameToken << "]");

	// Satellite extraction
	string satValue;
	int satIdValue(0);
	int missionValue(0);
	string satToken ("_");
	rs.getValue (satnameKey_, satValue);
	if (!satValue.empty())
	{
		// The satellite is set. Search the mission flags
		// The map is 1 -> S3A, 2 -> S3B, other -> S3_
		rs.getValue (satIdKey_, satIdValue);
		rs.getValue (missionKey_, missionValue);
		switch (missionValue)
		{
			case (1):
				satToken = "A";
				break;
			case (2):
				satToken = "B";
				break;
			case (3):
				satToken = "C";
				break;
			case (4):
				satToken = "D";
				break;
			default:
				satToken = "_"; // not useful but for sake of code clearness
		}
	}
	ACS_LOG_INFO ("Satellite applicability set to [" << ( ( "_" == satToken ) ? "both" : ("Sentinel-3" + satToken) ) << "]");
	
	// Generation time computation (now)
	DateTime now;
	string gentimeToken (now.YYYYMMDDThhmmss());
	ACS_LOG_INFO ("Generation time is [" << gentimeToken << "]");

	// Quality Info extraction and remapping
	// The rule is VALID_PRODUCT maps to VALID
	// any other value is mapped to FAILED and a warning is issued
	string qualityMetadata;
	string qualityInfo ("PASSED");
	rs.getValue (qualityinfoTag_, qualityMetadata);

	if ("VALID_PRODUCT" != qualityMetadata)
	{
		qualityInfo = "FAILED";
		ACS_LOG_WARNING ("Quality Check has not been passed succesfully for the current ADF");
	}

	ACS_LOG_INFO ("Quality info set to [" << qualityInfo << "]");

	int qualifiedId ;
	rs.getValue (qualifiedIdTag_,  qualifiedId);

	// Other statically configured stuff

	// Get static file attributes (maybe to be changed in dynamic ones)
	ACS_LOG_INFO("Platform set to [" << _platform << "]") ;
	ACS_LOG_INFO("Baseline set to [" << _baseline << "]") ;
   
	// Ready to create the SAFE package
	ostringstream os;
	os << "./S3" << satToken << "_" << filetypeToken << "_" << valStartNameToken << "_" << valStopNameToken << "_" << gentimeToken << "_" 
	   << "_________________" << "_" << genCentreToken << "_" << _platform << "_" << timelinessToken << "_" << _baseline << ".SEN3";

	string safeFileName (os.str());
	ACS_LOG_INFO("SAFE package is [" << File::getFileName(safeFileName) << "]") ;

	//Fill package's content
	vector<string> packageContent;
	fillSafePackage(  safeFileName, inputName, _ftypeDescMap[fileType].uncompressInput, packageContent);

	ACS_COND_THROW( packageContent.empty(),
			S3AuxReformatterException("Package ["+safeFileName+"] expected to contain one or more files." ) );


	// Now it is time to make the manifest
	S3AuxManifest manifest;

	// Some fields are hard-coded

	S3AuxManifest::FTypeDescriptor ftd;
	S3AuxManifest::FTypeDescMap ftypeDescMap;

	//Size of the element in the packages
	size_t packageContentSize(0);

	for ( vector<string>::const_iterator it=packageContent.begin(); it!=packageContent.end(); ++it ) {
		string const& packageElement = *it;

		string packageElementAbsPath = safeFileName+"/"+packageElement;

		// Computing the MD5 Signature
		ACS_LOG_INFO("Computing the MD5 signature") ;
		Md5Sum md5computer ( packageElementAbsPath );
		string md5signature (md5computer.computeSignature());
		ACS_LOG_INFO("MD5 signature of the package element ["<<packageElement<<"] is [" << md5signature << "]") ;

		size_t elementSize = File::size(packageElementAbsPath);
		packageContentSize += elementSize;

		// Fill the data object descriptor
		S3AuxManifest::DataObjectDescriptor d;
		d.repoID = "ADCRepoID";
		d.fileType = fileType;
		d.URL = "./" + File::getFileName (packageElement);
		d.MD5Signature = md5signature;
		d.qualityInfo = qualityInfo;
		d.productSize = elementSize;

		// Add it in the manifest
		manifest.addDataObject (d);
	}

	// Put also the association array in the manifest
	manifest.setDescriptorMap (_ftypeDescMap);

	// General info section 
	S3AuxManifest::GeneralInfo g;
	g.fileName = File::getFileName(safeFileName);
	g.baselineCollection = _baseline;
	// The file size in the manifest does not take into account the manifest itself
	g.productSize = packageContentSize;
	ostringstream nowUTC;
	now.asISO8601 (nowUTC, 6);
	g.creationTime = gentimeToken;
	g.validityStartTime = valStart + "Z";
	g.validityStopTime = valStop + "Z";
	manifest.setGeneralInfo (g);

	// Processing INFO section
	S3AuxManifest::ProcessingInfo i;
	i.processingStart = nowUTC.str();// + "Z"; // The same of the creation date
	i.facilityName = _facilityName;
	i.organization = _organization;
	i.site = _site;
	i.country = _country;
	i.swname = _compName;
	i.swversion = _compVers;
	i.hwname = _hwName;
	string inputFileVersion; rs.getValue (fileversionKey_, inputFileVersion);
	i.resources.push_back( make_pair( File::getFileName(inputName), inputFileVersion) );

	manifest.setProcessingInfo(i);

	// write it down now
	manifest.writeXMLManifest (safeFileName+"/xfdumanifest.xml");

	// re-compute the file size including the manifest (this is for Data Storage needs)
	size_t totalProductSize = File::getDirectoryOccupationInMB(File::getFileName(safeFileName)) * (1024*1024);

	// Now it is time to prepare the MTD and the LIST
	Metadata mtd;
	mtd.setMetadataValue("File_Name", File::getFileName(safeFileName));

	string fileVersion;
	rs.getValue (fileversionKey_, fileVersion);
	mtd.setMetadataValue("File_Version", fileVersion);
	mtd.setMetadataValue("Generation_Time", string("UTC=")+nowUTC.str());
	mtd.setMetadataValue("Validity_Start",string("UTC=")+valStart);
	mtd.setMetadataValue("Validity_Stop",string("UTC=")+valStop);
	mtd.setMetadataValue("Validity_Flag","TRUE");
	mtd.setMetadataValue("Validation_Date",string("UTC=")+nowUTC.str());
	mtd.setMetadataValue("File_Type",filetypeToken);
	mtd.setMetadataValue("File_Class",fileClass);
	mtd.setMetadataValue("Source",_swName);
	mtd.setMetadataValue("Source_Sw_Version",_swVersion);
	mtd.setMetadataValue("Header_Size", 0);
	mtd.setMetadataValue("Data_Size",  totalProductSize);
	mtd.setMetadataValue("Qualified",  qualifiedId);
	if (_ordId > 0)
		mtd.setMetadataValue("Order_Id",  _ordId) ;

	if (0 != satIdValue)
	{
		mtd.setMetadataValue("Satellite_Id", satIdValue);
	}

	if (0 != missionValue)
	{
		mtd.setMetadataValue("Mission", missionValue);
	}

	// Write the metadata structure
	mtd.writeMetadataFile(File::getFileName(safeFileName)+".MTD");

	// Write the LIST file
	string listfilename (File::getFileName(safeFileName)+".LIST");
	ofstream ofs (listfilename.c_str());
	ofs << File::getFileName(safeFileName) << endl;
	ofs.close();
	
	// That's all folks!
}

