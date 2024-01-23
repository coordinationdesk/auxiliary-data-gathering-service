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

	$Prod: libSafePreInventory $

*/ 

#include <LTAS1SafePreInventory.h>

#include <ConfigurationSingleton.h>
#include <File.h>
#include <StringUtils.h>
#include <SpawnProc.h>
#include <Dir.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(LTAS1SafePreInventory)

namespace {
	const string defaultTmpRootDir = ".";

}

LTAS1SafePreInventory::LTAS1SafePreInventory() : 
    SafePreInventory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	setPluginConfSpaceId("ConfigLTAS1SafePreInventory");
	setFactoryConfSpaceId("LTAS1SafePreInventoryFactory");
}

LTAS1SafePreInventory::~LTAS1SafePreInventory() throw() 
{
// PRQA S 4631 L1
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
// PRQA L:L1
}

string LTAS1SafePreInventory::getClassId()
{
        //return the class name
	return "LTAS1SafePreInventory";
}
        

void LTAS1SafePreInventory::computeMetadata( const string& pathFileName, const string& fileType, Metadata& metadata, dbConnection &conn)
{
    //NB: the pathFileName passed to this method is an absolute path
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	
	ACS_SMARTPTR<FileGuard> fg;
	try {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MetadataInfo:" << endl
			<< " OrderId=" << getContext().metadataInfo.orderId << endl
			<< "  pathFileName= " << pathFileName << endl
			<< "  filetype from framework= " << fileType << endl );
		

		setCurrentFileName(pathFileName);
		setCurrentFileType(fileType);
		string productDir = pathFileName;
		if( StringUtils::equalsNoCase(File::getExt(File::getFileName(pathFileName)), "ZIP") ) { 
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "zip extension found: going to unzip \"" << pathFileName << '\"');
			extractFilesFromZipArchive( pathFileName, fg, productDir );
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to extract metadata from \"" << productDir << "\" product");
		}

		ManifestData manifestData;
		
		readManifest(productDir, fileType, manifestData);
		
		metadata = prepareMetadata( fileType, manifestData, conn);
		
		
	}
    catch( exception& ex)
    {
		fg->detach();
        ACS_THROW( exLTAS1SafePreInventoryException( ex, "computeMetadata Exception"));
    }
}      




void LTAS1SafePreInventory::extractFilesFromZipArchive( const string & zipArchive, ACS_SMARTPTR<FileGuard> & fg, string & productDir )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
				
	string tmpRootDir = defaultTmpRootDir;
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	string pluginConfSpaceId = getPluginConfSpaceId();
	try {
		rs.getValue( "PreInventory."+pluginConfSpaceId+".TmpRootDir", tmpRootDir);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TmpRootDir from configuration: " << tmpRootDir );
		if( not File::exists(tmpRootDir) || not File::isDir(tmpRootDir) ) {
			ACS_THROW( exIllegalValueException('\"'+tmpRootDir+"\" temporary directory root either does not exist or it is not a directory") );
		}
	}
	catch(rsResourceSet::NotFoundException &)
	{}

	string dirPrefix = StringUtils::pathJoin(tmpRootDir, "LTAS1SafePreInv_XXXXXX" );
	char tmpl[4096] ;
	strcpy(tmpl, dirPrefix.c_str()) ;
	char *res = mkdtemp(tmpl);
	ACS_COND_THROW(not res, exSystemException("Cannot create temporary directory in \""+tmpRootDir+"\" root", errno) );
	string outTempDir = res;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << outTempDir << "\" temporary directory just created to unzip product.");

	try {

		SpawnProc proc("unzip") ;
		
		size_t i=0;
		proc.args()[i++] = zipArchive;
		proc.args()[i++] = "-d";
		proc.args()[i++] = outTempDir;
		proc.spawn() ;
		while (proc.isRunning()) {
			proc.waitExitOrTimeout(1000); // PRQA S 4400 // 1 sec
		}

		int exitCode = 0 ;
		ACS_COND_THROW( not proc.exitCode(exitCode),  exSystemException('\"'+zipArchive+"\" unzip not terminated") );
		if( exitCode != 0 ) {
			ostringstream exMsg;
			exMsg << '\"' << zipArchive << "\" zip failed";

			int exitSignal = 0 ;
			if( proc.exitSignal(exitSignal) ) {
				exMsg << " terminated with signal: " << exitSignal ;
			}
			exMsg << " exit code " << exitCode;
			exSystemException(exMsg.str());
    	}

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << zipArchive << "\" unzip successfully executed in \"" << outTempDir << "\" directory");

	}
	catch( exception &e )
	{
		ACS_THROW (exSystemException (e, "Check \""+outTempDir+"\" temporary directory content" ) ) ;
	}

	fg.reset(new  FileGuard(outTempDir) );
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		fg->detach();
	ACS_CLASS_END_DEBUG
			
			
	Dir dir(outTempDir);
	if( dir.size() != 1 ) {
		ostringstream exMsg;
		exMsg << dir.size() << " files contained in \"" << outTempDir << "\" directory after unzip: 1 file expected";
		ACS_THROW( exLTAS1SafePreInventoryException( exMsg.str() ) );
	}
	productDir = StringUtils::pathJoin(outTempDir, dir[0]);

}

void LTAS1SafePreInventory::setFurther(Metadata & metadata , dbConnection &)
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method Called");
	
	string md5Signature = File::computeMd5Sum(getCurrentFileName());
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "md5 signature just computed on file \"" << getCurrentFileName() << "\": \"" << md5Signature << '\"');

	metadata.setMetadataValue( "Md5", md5Signature);
	
	// Unique_Id must be empty
	metadata.setMetadataValue( "Unique_Id", ""); 
	
}

void LTAS1SafePreInventory::manageFootPrintPosList(const ManifestData& manifestData, Metadata & metadata) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Parsing footprint [" << manifestData.footPrintPosList << "]");
	
	vector<string> geoPointsCoords;
	StringUtils::Tokenize(manifestData.footPrintPosList , geoPointsCoords, " ");
	
	size_t geoPointsCoordsSize = geoPointsCoords.size();
	vector<Metadata::GeoPointInfo>  vGeoPntInfo;
	for( size_t i = 0; i < geoPointsCoordsSize; ++i ) {
		vector<string> geoCoords;
		StringUtils::Tokenize(geoPointsCoords[i], geoCoords, ",");
		if( geoCoords.size() != 2 ) {
			ostringstream exMsg;
			exMsg << '\"' << geoPointsCoords[i] << "\" does not contain a geographic coordinates pair.";
			ACS_THROW( exLTAS1SafePreInventoryException( exMsg.str() ) );		
		}
		
		Metadata::GeoPointInfo gPntInfo;
		gPntInfo.latitude = geoCoords[0];
		gPntInfo.longitude = geoCoords[1];

		vGeoPntInfo.push_back( gPntInfo );


	}
	
	if( not StringUtils::equals( geoPointsCoords[0], geoPointsCoords[geoPointsCoordsSize-1] ) ) {
		vGeoPntInfo.push_back(vGeoPntInfo[0]);
	}
	metadata.setGeoPoints( vGeoPntInfo );
	metadata.setGeoType( dbGeoObject::Polygon );

	// time range of Geographic_Localization
	metadata.setFieldValue( "Start_Time", metadata.getMetadataValue("Validity_Start") );
	metadata.setFieldValue( "Stop_Time", metadata.getMetadataValue("Validity_Stop"));
	
}											
	
_ACS_END_NAMESPACE
