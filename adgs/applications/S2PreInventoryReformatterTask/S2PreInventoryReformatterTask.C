// PRQA S 1050 EOF
/*
	Copyright 2020, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.
	
*/ 

#include <S2PreInventoryReformatterTask.h>
#include <S2PreInventoryReformatterTaskV.h>

#include <Metadata.h>
#include <dbSmartPersistent.h>
#include <dbFileTypeRecognition.h>
#include <dbConnectionPool.h>
#include <dbConnection.h>
#include <StringUtils.h>
#include <rsPushNSpace.h>
#include <ConfigurationSingleton.h>
#include <SpawnProc.h>
#include <ftmpstream.h>
#include <RegExp.h>
#include <Dir.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(S2PreInventoryReformatterTask)

namespace {
    S2PreInventoryReformatterTaskV version;
    PROJECT_APP::GenericBuilder<S2PreInventoryReformatterTask> theApplicationBuilder("S2PreInventoryReformatterTask","MCC");
	
    const char* namespaces[] =
    {
	    "S2PreInventoryReformatterTask",
	    "Inventory"
    };

}


S2PreInventoryReformatterTask::S2PreInventoryReformatterTask(const std::string& subsys, const std::string& app):
    PROJECT_APP("S2PreInventoryReformatterTask","MCC"),
	_mtdExtension(),
	_listExtension(),
	_wdir("."),
	_filetypesToManage(),
	_tarExecutionTimeout(600),
	_hdrSystemFromFilename()
{
   	static char Id[] = "@(#) ACS: $Id$";
   	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: "<< Id) ;
}

S2PreInventoryReformatterTask::~S2PreInventoryReformatterTask() throw() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Destructor called.") ; // PRQA S 4631
}


void S2PreInventoryReformatterTask::loadConf() // PRQA S 4211
{
	//first: load general configuration spaces
	for (size_t i=0; i<sizeof(namespaces)/sizeof(namespaces[0]); ++i) // PRQA S 3084
	{
		ACS_LOG_INFO("requesting configuration space " << namespaces[i]) ;
		addSpace( ConfigurationSingleton::ConfSpace(namespaces[i])); // PRQA S 3081
	}

	// do the actual load
	ACS_LOG_INFO( "loading configuration spaces");
	loadConfiguration(false);
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "configuration spaces loaded") ;
	
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	
    rs.getValue( "Inventory.MetadataFileExtension", _mtdExtension);
	rs.getValue( "Inventory.ListFileExtension" , _listExtension);
	
	{
		rsPushNSpace rspn(rs, "S2PreInventoryReformatterTask");
		if( rs.hasKey("WorkingDir") ) {
		    rs.getValue ("WorkingDir", _wdir);
		}

		string rootTag = "FileTypes.FileType";
    	const size_t elements = rs.getArraySize( rootTag ); // PRQA S 3000, 3010, 4412
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=" << rootTag << ", elements " << elements);
		for( size_t i=0; i<elements ; i++) {
			string tag = rootTag;
			if( elements > 1 ) {
				ostringstream os ;
				os << "[" << i << "]" ;
				tag += os.str();
			}
			string filetype ;
			rs.getValue( tag, filetype);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TAG=\"" << tag << "\"=: \"" << filetype << '\"');
			string finalFiletype = filetype ;
			string attributeKey = tag + ".final.value" ;
			if( rs.hasKey(attributeKey) ) {
		    	rs.getValue (attributeKey, finalFiletype);
			}
			_filetypesToManage[filetype] = finalFiletype;
		}
		
		if( rs.hasKey("TarExecutionTimeout") ) {
		    rs.getValue ("TarExecutionTimeout", _tarExecutionTimeout);
			if( _tarExecutionTimeout < 0 ) {
				ostringstream exMsg;
				exMsg << "Bad configuration: TarExecutionTimeout=" << _tarExecutionTimeout << " s; it should be greater than zero";
				ACS_THROW(  exS2PreInventoryReformatterTaskException(exMsg.str()) );
			}
		}
		
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg;
		msg << "MetadataFileExtension=\""<<_mtdExtension << '\"' 
			<< "\n\tListFileExtension=\"" << _listExtension << '\"'
			<< "\n\tWorkingDir=\"" << _wdir << '\"' ;
		for (map<string,string>::const_iterator it=_filetypesToManage.begin(); it!=_filetypesToManage.end(); ++it) {
    		msg << "\n\tFiletype=\"" << it->first << "\" => \"" << it->second << '\"';
		}
		msg << "\n\tTarExecutionTimeout=" <<_tarExecutionTimeout << " s";
		ACS_LOG_DEBUG(SimpleDebugSignature << msg.str() );
	ACS_CLASS_END_DEBUG

}


int S2PreInventoryReformatterTask::usage ( const std::string & ) const 
{
	excerr << "Use: \n" ;
	excerr << applicationName() << "\n" ;

	return 1 ;
}



int S2PreInventoryReformatterTask::main(int argc, char const * const * argv, char const * const * env) // PRQA S 4020
{
	    
	// Merge all streams into the log
    vector<exostream *> streams ;
    streams.push_back(&exostream::getDefaultOutLogger()) ;
    streams.push_back(&excout) ;
	
	//******************** Setup Log ****************************************
	string logFullPath = "./" + applicationName() + ".log" ; // create the log path name
	LogRotationPolicy lrp(logFullPath, LogRotationPolicy::FirstApplying, defaultLogAge, 1048576, 20, true) ;
 	setLRPolicy(lrp) ;
	Application::rotateLog() ; // open the first log


	try 
	{

		loadConf();
		
		execute();

		return 0;                               
	} 
	catch( exception &e) 
	{
        ACS_LOG_NOTIFY_EX(e) ;
		ACS_LOG_ERROR(applicationName() + " : Exiting in ERROR...");
		return 0xff ; // PRQA S 4400
	}
}

void S2PreInventoryReformatterTask::execute()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

	const string metadataFilepath = getFilepathFromExt(_mtdExtension) ;

	Metadata metadata;
	metadata.readMetadataFile(metadataFilepath);
	
	string filetype = metadata.getMetadataValue("File_Type");
	std::map<string, string>::const_iterator it = _filetypesToManage.find(filetype) ;
	if( it == _filetypesToManage.end() ) {
		ACS_LOG_INFO("File_Type extracted from \"" << metadataFilepath << "\" metadata file is \"" << filetype << "\": \"" << metadata.getGlobalFileName() 
					<< "\" product not to be reformatted");
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << filetype << " filetype: \"" << metadata.getGlobalFileName() << "\" product to be reformatted");
		reformat(filetype, it->second, metadataFilepath, metadata) ;
	}
	
}


string S2PreInventoryReformatterTask::getFilepathFromExt(const string & fileExt) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	const string rExpString("^.*."+fileExt+"$");
	RegExp rExp(rExpString);
	Dir dir(_wdir) ;
	const size_t itemsNum = dir.size();
	string filename ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to test \"" << rExpString << "\" regular expression");
	for( size_t i = 0; i < itemsNum && filename.empty(); ++i ) {
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, '[' << i << "] \"" << dir[i] << '\"');
		if( rExp.match(dir[i]) ) {
			filename = dir[i] ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << fileExt << "\" file found: \"" << filename << '\"' );
		}
	
	}
	
	ACS_COND_THROW( filename.empty(),  exS2PreInventoryReformatterTaskException("Cannot find \""+ fileExt+"\" file according to \""+rExpString+"\" regular expression in \""+_wdir+"\" directory" ) );
	
	return StringUtils::pathJoin(_wdir, filename);


}




void S2PreInventoryReformatterTask::reformat( const string & filetype, const string & finalFiletype, const string & metadataFilepath, Metadata & metadata )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	string filename = getFilenameFromFileTypeConf( metadata.getGlobalFileName(), filetype, false ); 
	
	string filepath = StringUtils::pathJoin(_wdir, filename );
	ACS_COND_THROW( not File::exists(filepath), exS2PreInventoryReformatterTaskException('\"'+filepath+"\" does not exist!") );
	
	string hdrFilename = File::addExt( File::removeExt(filename), "HDR" )  ;
	string hdrFilePath = StringUtils::pathJoin(_wdir, hdrFilename) ;
	FileGuard hdrFg(hdrFilePath);
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignature << '\"' << hdrFilePath << "\" file will NOT be removed for debug") ;
		hdrFg.detach();
	ACS_CLASS_END_DEBUG
	string filenameNoExt = File::removeExt(filename);
	writeHdrFile(hdrFilePath, filenameNoExt, finalFiletype, metadata );
	
	string tgzFilename = File::addExt( filenameNoExt, "TGZ" ) ;
	string tgzFilePath = StringUtils::pathJoin(_wdir, tgzFilename) ;
	// create tgz file
	createPackage( hdrFilename, filename, tgzFilePath );

	// update metadata struct
	off_t fileSize = File::size(tgzFilePath);
	updateMetadata( tgzFilename, finalFiletype, fileSize, metadata );

	// write new metadata file.
	string newMetadataFilepath = File::addExt( tgzFilePath, _mtdExtension );
	metadata.writeMetadataFile(newMetadataFilepath);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << newMetadataFilepath << "\" file successfully written" );
	
	// update file LIST
	string listFilePath = getFilepathFromExt( _listExtension )  ;
	updateListFile( filename, tgzFilename, listFilePath );
}


void S2PreInventoryReformatterTask::writeHdrFile(const string & hdrFilePath, const string & filename, const string & fileType, const Metadata & metadata) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	ofstream ofs( hdrFilePath.c_str(), ofstream::out | ofstream::trunc);
	ACS_COND_THROW( not ofs.is_open(), exS2PreInventoryReformatterTaskException("Cannot open \""+ hdrFilePath+"\" file") );
	
    XMLOstream xst( ofs, "Earth_Explorer_Header");


    {
		XMLOstream::Tag tagFIXEDHEADER( xst, "Fixed_Header");
		XMLOstream::Tag tagFILENAME( xst, "File_Name", filename, "", true);

		XMLOstream::Tag tagDESCRIPTION( xst, "File_Description", ("File "+fileType), "", true);

		XMLOstream::Tag tagNOTES( xst, "Notes", "", "", true);

		string mission = getMission(metadata);
		if( mission.empty() ) {
			mission = "Sentinel-2";
		}
		XMLOstream::Tag tagMISSION( xst, "Mission", mission, "", true);

		XMLOstream::Tag tagFILECLASS( xst, "File_Class", metadata.getMetadataValue("File_Class"), "", true);

		XMLOstream::Tag tagFILETYPE( xst, "File_Type", fileType, "", true);
        {
			XMLOstream::Tag tagVALIDITYPERIOD( xst, "Validity_Period");
			XMLOstream::Tag tagVALIDITYSTART( xst, "Validity_Start", getTime(metadata, "Validity_Start"), "", true);
			XMLOstream::Tag tagVALIDITYSTOP( xst, "Validity_Stop", getTime(metadata, "Validity_Stop"), "", true);
		}

		XMLOstream::Tag tagFILEVERSION( xst, "File_Version", metadata.getMetadataValue("File_Version"), "", true);
        {
			XMLOstream::Tag tagSOURCE( xst, "Source");
			
			string system = metadata.getMetadataValue( "System", false);
			if( system.empty() ) {
			// extract from name: as detailed in [EOFFS-PDGS]_GMES-GSEG-EOPG-TN-2010-0099_[1.1] section 2.1.4.1.1 :
			// <<... Site Centre is a 4 characters field defined by either, uppercase letters, digits or underscore...>>
				string filename = metadata.getGlobalFileName() ;
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "System not valorized in metadata: going to extract from \"" << filename << '\"');
				system = filename.substr(20, 4);
				_hdrSystemFromFilename = system ;
			}
			XMLOstream::Tag tagSYSTEM( xst, "System", system, "", true);
			
			string creator = metadata.getMetadataValue( "Source", false);
			XMLOstream::Tag tagCREATOR( xst, "Creator", creator, "", true);  

			XMLOstream::Tag tagCREATORVERSION( xst, "Creator_Version", "1.0", "", true);

			XMLOstream::Tag tagCREATIONDATE( xst, "Creation_Date", getTime(metadata, "Generation_Time"), "", true);
			
		}
	} // Close Fixed_Header tag

    XMLOstream::Tag tagVARIABLEHEADER( xst, "Variable_Header", "", "", true);
	
	xst.close();
	ofs.close();

}


void S2PreInventoryReformatterTask::createPackage( const string & hdrFilename, const string & originalFilename, const string & tgzFilePath ) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	// first of all make an hard link of original file
	string dblFilename = File::addExt( File::removeExt(hdrFilename), "DBL" ) ;
	string dblFilepath = StringUtils::pathJoin(_wdir, dblFilename) ;
	File::hardlink( StringUtils::pathJoin(_wdir, originalFilename), dblFilepath );
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << dblFilepath << "\" generated from hard link");
	FileGuard fg(dblFilepath);	
	
	// making hard link of original file as DBL is preferred w.r.t. to use tar transfom option for the following reason:
	// option to be used on myFile would be --transform='flags=r;s|myFile|myFile.DBL|' (single quote in transform command need to execute in bash shell but must be removed from string:
	// "--transform=flags=r;s|"+originalFilename+"|"+File::addExt( File::removeExt(hdrFilename), "DBL" ) +"|")
	// neverthless packing myFile.HDR would transform into myFile.DBL.HDR. This means that 2 tar commands should be executed with the second one adding a file to the .TGZ generated
	/// by the first command.
	
	const string command("tar");
	SpawnProc proc(command) ;
	int i = 0;
 	proc.args()[i++] = "-C";
 	proc.args()[i++] = _wdir;
	proc.args()[i++] = "-czf";
	proc.args()[i++] = tgzFilePath;
	proc.args()[i++] = hdrFilename;
	proc.args()[i++] = dblFilename;
	
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		WaitPid::ProcArgs procArgs = proc.args();
		ostringstream msg;
		msg << "Command: " << command ;
		size_t commandArgsSize = proc.args().size();
		for(size_t i = 0; i < commandArgsSize; i++) {
			msg << ' ' << procArgs[i].arg();
		}
		ACS_LOG_DEBUG(SimpleDebugSignature << msg.str() );
	ACS_CLASS_END_DEBUG
	
	proc.spawn() ;
	
	Timer watchdog ;
	bool wdTimeout = false ;
	watchdog.start() ;
	unsigned long int timeout = _tarExecutionTimeout * 1000;
	while(proc.isRunning()) {
		Timer::delay(100); // PRQA S 4400
		if(watchdog.isOlder(timeout)) { // PRQA S 3000, 4400
			proc.kill(); // it took more than timeout: kill it
			wdTimeout = true ;
		}
	}

	int exitCode = -1 ;
	bool goodExit=true;
	if( !proc.exitCode(exitCode) ) {
		goodExit=false;
    }

	if(exitCode != 0) {
		ostringstream os ;
		if (wdTimeout) {
			os << watchdog.elapsed() / 1000 // PRQA S 3084, 4400
			   << " [sec] elapsed since " << command << " was started to build \"" << tgzFilePath << "\" file. It is bigger than the timeout (" << _tarExecutionTimeout
				<< " [sec]). The process was killed. " ;
			ACS_THROW(exS2PreInventoryReformatterTaskException(os.str())) ; // PRQA S 3081
		}
		os << command << " command to build \"" << tgzFilePath << "\" file failed; exitCode: " << exitCode;
		int exitSignal = 0 ;
		proc.exitSignal(exitSignal) ;
		if( 0 != exitSignal ) {
			os << " exitSignal: " << exitSignal ;
		}
		ACS_THROW(exS2PreInventoryReformatterTaskException(os.str())) ; // PRQA S 3081
	}
	else if( not goodExit ) {
		ostringstream exMsg;
		exMsg << command << " command to build \"" << tgzFilePath << "\" file path not terminated";
		ACS_THROW( exS2PreInventoryReformatterTaskException( exMsg.str()) );
	}
	else { // just for debug
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, command << " command to build \"" << tgzFilePath << "\" file successfull");
	}


}

void S2PreInventoryReformatterTask::updateMetadata( const string & filename, const string & filetype, off_t fileSize, Metadata & metadata ) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	string metadataFilename = getFilenameFromFileTypeConf( filename, filetype, true ); 
	metadata.setMetadataValue( "File_Name", metadataFilename);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File_Name=\"" << metadataFilename << '\"');

	metadata.setMetadataValue( "File_Type", filetype);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File_Type=\"" << filetype << '\"');

	metadata.setMetadataValue( "Data_Size", fileSize);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Data_Size=" << fileSize);
	
	if( not _hdrSystemFromFilename.empty() ) {
		metadata.setMetadataValue( "System", _hdrSystemFromFilename);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "System=\"" << _hdrSystemFromFilename << '\"');
	}
	
}

string S2PreInventoryReformatterTask::getFilenameFromFileTypeConf( const string & filename, const string & filetype, bool toMetadata ) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	string convertedFilename = filename ; 
	
	dbConnectionWrapper pw ( *(Application::instance()->getDefaultConnectionPool()));
	dbConnection& conn (pw);

	dbFileTypeRecognition ftr ;
	dbFileTypeRecognition::ftype_traits ftt = ftr.getTraits(filetype, conn) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Traits for ftype: "  << filetype << ": " << ftt.str() );
	if( ftt.mngsngext ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to convert filename " << ( toMetadata ? "to" : "from" ) << " metadata");
		if( toMetadata ) {
			convertedFilename = File::removeExt( filename ) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << filetype << "\" t_filetype record configured with mngsngext set: filename=\"" << convertedFilename << '\"'); 
		}
		else {
			convertedFilename = File::addExt( filename, ftt.sngext ) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << filetype << "\" t_filetype record configured with mngsngext set: filename=\"" << convertedFilename << '\"'); 
		}
	}
	else { // just for debug
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, '\"' << filetype << "\" t_filetype record configured with mngsngext not set: filename=\"" << convertedFilename << '\"');
	}
	
	return convertedFilename ;
}


string S2PreInventoryReformatterTask::getMission( const Metadata & metadata) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

	string mission;
	string satIdStr = metadata.getMetadataValue("Satellite_Id", false); // do not throw exc
	if( not satIdStr.empty() ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Metadata Satellite_Id=" << satIdStr );
		int satId = 0;
		istringstream iss(satIdStr);
		iss >> satId ;
		
		dbConnectionWrapper pw ( *(Application::instance()->getDefaultConnectionPool()));
		dbConnection& conn (pw);

		dbSmartPersistent sp("t_satellites", conn) ;
		if( sp.load(conn, satId) ) {
			bool isNull;
			sp.get( "satellitename", mission, isNull, conn );
		}
		else {
			ACS_LOG_WARNING("Metadata Satellite_Id valorized with \"" << satIdStr << "\" but cannot find t_satellites record with id=" << satId << ": mission will not be valorized");
		}

	}
	else { // just for debug
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Metadata Satellite_Id not valorized");
	}

	return mission;

}


string S2PreInventoryReformatterTask::getTime( const Metadata & metadata, const string & field) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	string validityString = metadata.getMetadataValue(field); 
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Metadata \"" << field << "\"=\"" << validityString << '\"' );

	const string utcString("UTC=");
	size_t pos = validityString.find(utcString) ;
	if( pos != string::npos ) {
		validityString = validityString.substr( (pos+utcString.length()) ) ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, utcString << " string found; stripped: \"" << validityString << '\"');
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, utcString << " string not found");
	}
	
	DateTime d(validityString);
	validityString = utcString + d.asISO8601(0);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to return \"" << validityString << '\"');
	
	return validityString;
}

void S2PreInventoryReformatterTask::updateListFile( const string & oldFilename, const string & newFilename, const string & listFilepath ) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	ACS_COND_THROW( not File::exists(listFilepath), exS2PreInventoryReformatterTaskException('\"'+listFilepath+"\" does not exist!") );
	
	oftmpstream tmpFile( StringUtils::pathJoin(_wdir, "S2PreInvRft") , _listExtension) ;
	string tmpFilepath = tmpFile.path();
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << tmpFilepath << "\" file just created");
	
  	ifstream listFile (listFilepath.c_str());
	ACS_COND_THROW( not listFile.is_open(), exS2PreInventoryReformatterTaskException("Cannot open \""+listFilepath+"\" file", errno) );
	string line;
    while ( getline (listFile, line) ) {
		if( StringUtils::equals(line,  oldFilename) ) {
			line = newFilename;
		}
     	tmpFile() << line << '\n';
    }
    listFile.close();
	
	File::rename( tmpFilepath, listFilepath);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << tmpFilepath << "\" file successfully renamed as \"" << listFilepath << '\"');


}
_ACS_END_NAMESPACE
