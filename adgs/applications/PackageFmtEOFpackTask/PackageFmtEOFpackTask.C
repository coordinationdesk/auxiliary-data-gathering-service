/*
	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. PackageFmtEOFpackTask$

	$Id$

	$Author$

	$Log$
	Revision 1.23  2017/06/12 17:22:27  nicvac
	APF-415: Handle tar in PackageExporter operational context (hard link counter while compression).
	
	Revision 1.22  2016/10/10 16:35:48  francesco.avanzi
	OPMAN-202: work in progress
	
	Revision 1.21  2016/10/10 16:22:01  francesco.avanzi
	OPMAN-202: now querying database to get filetype in getHdrDblExtension()
	
	Revision 1.20  2016/10/05 15:55:25  nicvac
	OPMAN-197: To support for QCC, headerseparation strategy has been modified.
	
	Revision 1.19  2016/10/05 12:36:49  nicvac
	OPMAN-197: To support for QCC, headerseparation strategy has been modified.
	
	Revision 1.18  2016/10/05 09:02:35  nicvac
	2PDGS-1544: Log handling revised.
	
	Revision 1.17  2016/09/21 10:25:20  nicvac
	OPMAN-164: cryo2evo. Extension stripped from package name if container used. Capitalized default extensions.
	
	Revision 1.16  2016/09/15 14:27:11  nicvac
	S2PDGS-1544: Log handling revision.
	
	Revision 1.15  2016/02/29 11:05:22  marfav
	Fixed link management in gzip creation
	
	Revision 1.14  2016/02/26 11:12:13  nicvac
	OPMAN-151: fixed final extension. Fixed gzip on symlink.
	
	Revision 1.13  2016/02/25 16:56:16  nicvac
	Fixed db case.
	
	Revision 1.12  2016/02/25 14:44:31  nicvac
	OPMAN-151: Fixed Header case
	
	Revision 1.11  2016/02/25 11:31:26  nicvac
	Test and fix loop
	
	Revision 1.10  2016/02/24 17:12:11  nicvac
	log
	
	Revision 1.9  2016/02/24 17:07:36  nicvac
	OPMAN-151: implementation and test
	
	Revision 1.8  2016/02/24 17:03:30  nicvac
	OPMAN-151: Implementation completed.
	
	Revision 1.7  2016/02/23 17:20:52  nicvac
	OPMAN-151: format implementation
	
	Revision 1.6  2013/10/04 15:19:51  nicvac
	acs sdk warnings processed.

	Revision 1.5  2013/02/22 17:38:42  nicvac
	S2PDGS-186: mail notification implemented.

	Revision 1.4  2013/02/07 16:06:44  nicvac
	load and dump for serializable objects.

	Revision 1.3  2013/01/22 16:52:37  nicvac
	Fixed after unit test.

	Revision 1.2  2013/01/22 13:33:29  nicvac
	Implemented

	Revision 1.1.1.1  2013/01/21 16:33:30  nicvac
	Import PackageFmtEOFpackTask


 */

#include <PackageFmtEOFpackTask.h>
#include "PackageFmtEOFpackTaskV.h"

#include <dbFileTypeRecognition.h>
#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbSet.h>


#include <Filterables.h>

#include <RegExp.h>
#include <Dir.h>
#include <File.h>

#include <algorithm>
#include <sstream>
#include <iterator>

#include <boost/program_options.hpp>
namespace po = boost::program_options;


using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(PackageFmtEOFpackTask)

static PackageFmtEOFpackTaskV version;

PROJECT_APP::GenericBuilder<PackageFmtEOFpackTask> theApplicationBuilder("PackageFmtEOFpackTask", "DataCirculation");


PackageFmtEOFpackTask::CommandLineParameters PackageFmtEOFpackTask::cl;

void PackageFmtEOFpackTask::customSigTermHandler() {

	ACS_LOG_ERROR("SIGTERM received - exiting...");

	_stop = true;
}

void PackageFmtEOFpackTask::customSigIntHandler() {

	ACS_LOG_ERROR("SIGINT received - exiting...");

	_stop = true;
}



PackageFmtEOFpackTask::PackageFmtEOFpackTask(const std::string& app, const std::string& subsys):
			PROJECT_APP(app, subsys),
			_expProcInfo(),
			_datapackage(),
			_argvParameters(),
			_stop(false)
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Id: "<< Id);
}

PackageFmtEOFpackTask::~PackageFmtEOFpackTask() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void PackageFmtEOFpackTask::doCommand( SpawnProc& proc, string const& tag, vector<int> allowedExitCodes ) {

	if ( allowedExitCodes.empty() ) {
		allowedExitCodes.push_back(0);
	}

	string log = File::tempname( _expProcInfo.getWorkingDir() +"/Log_"+tag+"_");
	string errLog = log + "_err";
	string outLog = log + "_out";
	FILE* logErr = fopen ( errLog.c_str(),"w");
	FILE* logOut = fopen ( outLog.c_str(),"w");

	proc.setErrDescriptor( fileno( logErr ) );
	proc.setOutDescriptor( fileno( logOut ) );

	ACS_LOG_INFO("Log files: ["<<outLog<<", "<<errLog<<"]");

	proc.spawn();
	proc.waitProcCompletion();

	fclose (logErr);
	fclose (logOut);

	int exitCode(0);
	bool validExitCode = proc.exitCode(exitCode);

	vector<int>::iterator it = find( allowedExitCodes.begin(), allowedExitCodes.end(), exitCode );
	bool exitCodeIsAllowed = ( it != allowedExitCodes.end() );

	ACS_COND_THROW( ! (validExitCode && exitCodeIsAllowed),
			exPackageFmtEOFpackTaskCriticalException("Error executing command ["+tag+"]. See log files ["+outLog+", "+errLog+"]") );

}

void PackageFmtEOFpackTask::doTar( std::string const& inAbsPathDir, std::vector<std::string> const& inRelPaths,
		std::string const& outAbsFilename ) {

	//Create the tar
	//tar --warning=no-file-changed --dereference --directory=/tmp/test/prova/ -c -v -f /tmp/test/prova_Fmt/file.tar  file.hdr file.dbl

	//Create tgz
	SpawnProc tar("tar");

	int countArg(0);
	tar.args()[ countArg++ ] = string("--warning=no-file-changed");
	tar.args()[ countArg++ ] = string("--dereference");
	tar.args()[ countArg++ ] = string("--directory=")+inAbsPathDir;
	tar.args()[ countArg++ ] = string("-cvf");
	tar.args()[ countArg++ ] = outAbsFilename;

	for ( vector<string>::const_iterator it( inRelPaths.begin() ); it != inRelPaths.end(); ++it ) {
		if ( *it != "" ) {
			tar.args()[countArg++] = *it;
		}
	}

	ACS_LOG_INFO("Executing tar...");
	vector<int> allowedExitCodes;
	allowedExitCodes.push_back(0);
	allowedExitCodes.push_back(1);

	/* NOTE:
	 * In the Package Exporter operational context, each file contained in the datapackage is in the end
	 * an hard link to the file in cache.
	 * The same file in cache can be hard linked (by Package Exporter) to another datapackage.
	 * If the hardlink to another datapackage is performed during the tar creation, tar command exits with
	 * code = 1: `Some files differ'.
	 * The content of the file is not changed. It is only changed the hard link counter, but tar consider it as a change.
	 * So in this context we allow the exit code = 1.
	 * Also the related tar's warning message is suppressed (--warning=no-file-changed)
	 * */

	this->doCommand( tar, "tar", allowedExitCodes );

}

void PackageFmtEOFpackTask::doGzip(bool preserveInput, std::string const& inAbsPathDir, std::vector<std::string> const& inRelPaths,
		std::string const& outAbsFilename ) {

	ACS_COND_THROW( inRelPaths.size() > 1,
			exPackageFmtEOFpackTaskCriticalException("Found more than one file in ["+inAbsPathDir+"]. Cannot run gzip on multiple files."));

	string in = inAbsPathDir+"/"+ inRelPaths.at(0);

	//gzip works on regular files only. Also it removes the original file!
	if ( preserveInput ) {
		string newIn = File::getDirPath( outAbsFilename ) +"/"+ File::getFileName( in );
		ACS_LOG_INFO ("Preserving " << in << " - copying it in " << newIn);
		File::copy( in, newIn );
		in = newIn;
	}

	ACS_COND_THROW( ! File::isRegularFile( in ),
			exPackageFmtEOFpackTaskCriticalException("Item ["+in+"] is not a regular file."));

	string out = in + ".gz";

	SpawnProc cmd("gzip");

	int countArg(0);
	cmd.args()[ countArg++ ] = in;

	ACS_LOG_INFO("Executing gzip...");
	this->doCommand( cmd, "gzip" );

	File::rename( out, outAbsFilename);
}

class GoToPathAtExit {
public:
	explicit GoToPathAtExit(string const& absPath): _absPath(absPath) { }
	~GoToPathAtExit() { File::chdir( _absPath ); }
private:
	string _absPath;
};

void PackageFmtEOFpackTask::doZip( std::string const& inAbsPathDir, std::vector<std::string> const& inRelPaths,
		std::string const& outAbsFilename ) {

	GoToPathAtExit scopeAction( File::getCurrentPath() );

	File::chdir( inAbsPathDir );

	//Create zip
	SpawnProc zip("zip");

	int countArg(0);
	zip.args()[ countArg++ ] = string("-") + _argvParameters[cl.zipLevel].as<string>() ;
	zip.args()[ countArg++ ] = "-r";
	zip.args()[ countArg++ ] = outAbsFilename;

	for ( vector<string>::const_iterator it( inRelPaths.begin() ); it != inRelPaths.end(); ++it ) {
		if ( *it != "" ) {
			zip.args()[countArg++] = *it ;
		}
	}

	ACS_LOG_INFO("Executing zip...");
	this->doCommand( zip, "zip" );
}

void PackageFmtEOFpackTask::getCompressionOptions( bool& doTar, CompressionType& compressionType, string& archiveExtension ) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	bool formatFromTusers = ( _argvParameters[cl.formatFrom].as< string >() == "fromUsers" );

	if ( formatFromTusers ) {

		dbConnectionWrapper pw ( *getDefaultConnectionPool() );
		dbConnection &conn = pw;

		const string &dbKey = conn.getKey() ;

		SQLString sqlString( dbKey, SQLString::SELECT );
		sqlString.setLimit(1);

		sqlString.addSelectList("usr.userid");
		sqlString.addSelectList("usr.packagerequired");
		sqlString.addSelectList("usr.compresspackage");
		sqlString.addSelectList("usr.compressionmode");
		sqlString.addTableList("t_users", "usr" );
		sqlString.addTableList("t_distributionitems", "disit" );
		sqlString.addTableList("t_distributionrules", "disr");
		sqlString.addTableList("t_distributionqueue", "disq");
		dbQueryParameters pp(dbKey);
		pp.addParameter( dbParam(dbKey, "disq" ,"packageid"          , dbParam::EQUAL_TO, db::toSQL(dbKey, _datapackage.getPackageId()) ));
		pp.addParameter( dbParam(dbKey, "disq" ,"packageid"          , dbParam::EQUAL_TO, "disit.packageid"                        	));
		pp.addParameter( dbParam(dbKey, "disit","distributionrule_id", dbParam::EQUAL_TO, "disr.id_distributionrule"               	));
		pp.addParameter( dbParam(dbKey, "disr", "user_id", 		 	   dbParam::EQUAL_TO, "usr.id_user"               				));


		sqlString.addANDWhereParameter( pp );

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query: "<<sqlString.getSQL());


		dbQuery query ( conn, sqlString, "GetCompressionOptions" );
		dbSet set;
        query.execForSet ( set );

    	ACS_COND_THROW(
    		set.size()!=1,
    		exPackageFmtEOFpackTaskCriticalException( "Retrieving format compression parameters from t_users, expected exactly one row from query: "+sqlString.getSQL()) );

        dbRow row(dbKey);
        set.getRow(1, row);

    	bool isNull(false); //If Null on DB, the attribute is left with its original value
    	int i(0);

    	string userid;
    	bool packagerequired(true);
    	bool compresspackage(true);
    	string compressionmode("gzip");

    	row.column(i++, userid, &isNull);
    	row.column(i++, packagerequired, &isNull);
    	row.column(i++, compresspackage, &isNull);
    	row.column(i++, compressionmode, &isNull);


    	ACS_LOG_INFO("Info from DB"
    			<<"; packageId: "<<_datapackage.getPackageId()
    			<<"; userid: "<<userid
    			<<"; packagerequired: "<<packagerequired
    			<<"; compresspackage: "<<compresspackage
    			<<"; compressionmode: "<<compressionmode);

    	boost::to_lower( compressionmode );

    	//Set output parameters from DB
    	doTar = packagerequired;

    	if ( compresspackage ) {
    		if (compressionmode == "zip")	{
    			compressionType = CompressionTypeZIP;
    			archiveExtension = ( doTar ? "TAR.ZIP" : "ZIP" );

    		} else if (compressionmode == "gzip")	{
    			compressionType = CompressionTypeGZIP;
    			archiveExtension = ( doTar ? "TGZ" : "GZ" );

    		} else {
    			ostringstream s; s<<compressionmode;
    			ACS_THROW(exPackageFmtEOFpackTaskCriticalException("t_users.compressionmode set to ["+s.str()+"]. Not allowed value."));
    		}

    	} else {
    		compressionType = CompressionTypeNONE;
    		archiveExtension = ( doTar ? "TAR" : "" );
    	}

	} else {

		//Set output parameters from command line
		doTar = _argvParameters[cl.cmdDoTar].as< bool >();

		string ct = _argvParameters[cl.cmdCompression].as< string >();
		if 		( ct == "none" )	compressionType = CompressionTypeNONE;
		else if ( ct == "zip")		compressionType = CompressionTypeZIP;
		else if ( ct == "gzip")		compressionType = CompressionTypeGZIP;
		else 						ACS_THROW(exPackageFmtEOFpackTaskCriticalException("Parameter ["+cl.cmdDoTar+"] set to ["+ct+"]. Not allowed value."));

		archiveExtension = _argvParameters[ cl.cmdExtension ].as<string>();
	}

	ACS_LOG_INFO("Packaging options"
			<<"; doTar: "<<doTar
			<<"; compressionType: "<<static_cast<int>(compressionType)
			<<"; archiveExtension: "<<archiveExtension);

}

void PackageFmtEOFpackTask::getHdrDblExtension( bool& headerSeparation, string& hdrExt, string& dblExt, string& filetypeRegExp ) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    dbConnectionWrapper pw ( *getDefaultConnectionPool() );
    dbConnection &conn = pw;
    const string &dbKey = conn.getKey() ;
	SQLString select( dbKey, SQLString::SELECT );
    ACS_LOG_INFO("PID: " <<  _datapackage.getPackageId()) ;
	select.setLimit(1); 
	select.addTableList("t_distributionitems", "di" );
	select.addSelectList("filetype");
    {
        dbQueryParameters pp( dbKey);
        pp.addParameter(dbParam( dbKey, "di","distributionrule_id" , dbParam::EQUAL_TO , "t_distributionrules.id_distributionrule " )) ;
        select.addJoinList(SQLDialect::INNER,"t_distributionrules",pp) ;
    }
    {
        dbQueryParameters pp( dbKey);
        pp.addParameter(dbParam( dbKey, "t_distributionrules","filetype_id" , dbParam::EQUAL_TO , "t_filetypes.id_filetype " )) ;
        select.addJoinList(SQLDialect::INNER,"t_filetypes",pp) ;
    }
    {
        dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, "di" ,"packageid" , dbParam::EQUAL_TO , db::toSQL(dbKey, _datapackage.getPackageId() ) )) ;
		select.addANDWhereParameter(pp) ;
	}
    
    dbQuery query ( conn, select, "GetDistItemFiletype" );
    dbSet set ;
    query.execForSet( set );
    ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081

    dbRow row(dbKey) ;
    set.getNextRow( row );
    string fileType ;
    bool isNull = false ;
	row.column(0, fileType, &isNull) ;
    ACS_COND_THROW(isNull, exIllegalValueException("Filetype cannot be Null string")) ;
    
	ACS_LOG_INFO("Filetype triggering this export: ["<<fileType<<"]");

	dbFileTypeRecognition filetyperec;

	dbFileTypeRecognition::ftype_traits splitInfo;
	{
		dbConnectionWrapper pw ( *getDefaultConnectionPool() );
		dbConnection &conn = pw;
		splitInfo = filetyperec.getTraits(fileType, conn) ;
		filetypeRegExp = filetyperec.getRegExp(fileType, conn);
	}

	headerSeparation = splitInfo.header_separator;
	hdrExt = splitInfo.hdrext;
	dblExt = splitInfo.datext;
}

string PackageFmtEOFpackTask::getOutputProductName( vector<string>& packageContentRelPaths,
													bool doTar, CompressionType compressionType,
													string const& filetypeRegExp) {

	string outProdname("");
	std::sort(packageContentRelPaths.begin(), packageContentRelPaths.end() );

	//If multiple files, take the package name from the file matching the filetype that triggered this order.
	// If not found, take it from the first file
	// Extension is stripped in any case

	if ( packageContentRelPaths.size() > 1 ) {

		RegExp reFiletype( filetypeRegExp );

		for ( vector<string>::const_iterator it=packageContentRelPaths.begin(); it!=packageContentRelPaths.end() && outProdname.empty(); ++it ) {
			if ( reFiletype.match( *it ) ) {
				outProdname = *it;
				ACS_LOG_INFO("Item ["<<*it<<"] matches filetype regexp ["<<filetypeRegExp<<"]. Used to define output package name.");
			}
		}

		//No regext matching => Take the first file
		if ( outProdname.empty() ) {
			outProdname = packageContentRelPaths.at(0);
			ACS_LOG_INFO("Items do not match filetype regexp ["<<filetypeRegExp<<"]. Using ["<<outProdname<<"] to define output package name.");
		}

		//Strip extension
		outProdname = File::removeExt( outProdname );

	} else {
		outProdname = packageContentRelPaths.at(0); //The unique file

		//If used a container (tar, zip) ==> strip the extension from the package name (the files'extension are preserved within the container).
		// This means that extension will be preserved only for gzip without tar (gzip runs on a single file and does not store the original filename).
		if ( !doTar && (compressionType == CompressionTypeGZIP) ) {
			; //preserve extension
		} else {
			outProdname = File::removeExt( outProdname );
		}
	}

	return outProdname;

}

int PackageFmtEOFpackTask::execute() {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	//Load IF files
	const string expProcInfoFilename = _argvParameters[ cl.expProcInfo ].as<string>();
	{

		//Load ExpProcInfo
		ACS_LOG_INFO("Loading "<<expProcInfoFilename);
		_expProcInfo.load( expProcInfoFilename );

		//Load Datapackage
		ACS_LOG_INFO("Loading "<<_expProcInfo.getDatapackageFilename());
		_datapackage.load( _expProcInfo.getDatapackageFilename() );

	}

	//Get content of the package
	vector<string> packageContentRelPaths;
	string filetypeRegExp;
	{

		//If the received datapackage dir to transfer is empty, the datapackage is not transferred
		// and the task exit with Error. An empty datapackage can be caused by previous package processing.
		Dir dataPackageDir( _expProcInfo.getDatapackageAbsPath() );
		if ( dataPackageDir.size() == 0 ) {

			ACS_LOG_ERROR("Datapackage ["<<_expProcInfo.getDatapackageAbsPath()<<"] is empty. Exiting with error.");
			//Return Error
			return 0xFF;
		}

		//Get header separation info from filetype
		bool headerSeparation(false);
		string dblExt; string hdrExt;
		this->getHdrDblExtension(headerSeparation, hdrExt, dblExt, filetypeRegExp );

		ACS_LOG_INFO( "Header separation: ["<< std::boolalpha << headerSeparation <<"]" );
		if (headerSeparation) ACS_LOG_INFO( "Header, Data extension: ["<<hdrExt<<","<<dblExt<<"]" );

		boost::to_upper(dblExt);
		boost::to_upper(hdrExt);

		//Locate DBL and HDR files
		RegExp reDbl(string(".+\\.") + dblExt +"$");
		RegExp reHdr(string(".+\\.") + hdrExt +"$");

		string dblFile(""); string hdrFile("");

		//Collect Files to be packaged

		bool headerOnly = _argvParameters[ cl.headerOnly ].as<bool>();

		for ( size_t i=0; i<dataPackageDir.size(); ++i ) {

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "found "<<dataPackageDir[i]);

			bool thisIsDbl=false;

			if ( reDbl.match( boost::to_upper_copy(dataPackageDir[i]) ) ) {
				dblFile=dataPackageDir[i];
				thisIsDbl=true;
			}
			if ( reHdr.match( boost::to_upper_copy(dataPackageDir[i]) ) ) {
				hdrFile=dataPackageDir[i];
			}

			if ( thisIsDbl && headerSeparation && headerOnly ) {
				ACS_LOG_INFO("Packaging the Header file. Data file ["<<dblFile<<"] skipped.");
			} else {
				packageContentRelPaths.push_back( dataPackageDir[i] );
			}
		}

		//Header separation. Expected Hdr and Dbl in datapackage
		ACS_COND_THROW( headerSeparation && (dblFile.empty() || hdrFile.empty() ),
				exPackageFmtEOFpackTaskCriticalException(string("")+"Cannot find Datablock ["+reDbl.getReg()+"] and Header ["+reHdr.getReg()+"] files in datapackage "+_expProcInfo.getDatapackageAbsPath()) );

	}

	//Compression option
	bool doTar(false);
	CompressionType compressionType;
	string archiveExtension;
	this->getCompressionOptions( doTar, compressionType, archiveExtension );

	{
		ACS_LOG_INFO( "Package to format ["<< _expProcInfo.getDatapackageAbsPath() <<"]" );
		ostringstream oss;
		std::copy(packageContentRelPaths.begin(), packageContentRelPaths.end(), std::ostream_iterator<string>(oss, "; "));
		ACS_LOG_INFO( "Content: ["<<oss.str()<<"]");
	}


	//Package files
	{

		//Create a new datapackage dir for the filtered items
		{
			string currentFmtDir( File::getDirPath(_expProcInfo.getDatapackageAbsPath()) + "/Fmt_Filtered/" );
			File::mkdir(currentFmtDir);

			for ( vector<string>::iterator it = packageContentRelPaths.begin(); it!=packageContentRelPaths.end(); ++it ) {
				File::rename( _expProcInfo.getDatapackageAbsPath()+"/"+ *it, currentFmtDir+"/"+ *it  );
			}
			_expProcInfo.setDatapackageAbsPath( currentFmtDir );
		}

		//Ouput product name
		const string outProdname = this->getOutputProductName( packageContentRelPaths, doTar, compressionType, filetypeRegExp );

		//Tar step
		bool gzipPreserveInput=true;
		if ( doTar ) {

			//Create a new datapackage dir for the tar step
			string currentFmtDir( File::getDirPath(_expProcInfo.getDatapackageAbsPath()) + "/Fmt_Tar/" );
			File::mkdir(currentFmtDir);

			string tarAbsFilename = currentFmtDir+"/"+ outProdname + ".tar";
			tarAbsFilename = File::getAbsolutePath(tarAbsFilename);

			this->doTar( _expProcInfo.getDatapackageAbsPath(), packageContentRelPaths, tarAbsFilename );

			//Updates for the next step
			packageContentRelPaths.clear();
			packageContentRelPaths.push_back( File::getFileName( tarAbsFilename ) );
			_expProcInfo.setDatapackageAbsPath( currentFmtDir );

			gzipPreserveInput=false;
		}

		if ( compressionType == CompressionTypeGZIP ) {
			//Create a new datapackage dir for gzip step
			string currentFmtDir( File::getDirPath(_expProcInfo.getDatapackageAbsPath()) + "/Fmt_Gzip/" );
			File::mkdir(currentFmtDir);

			string gzipAbsFilename = currentFmtDir+"/"+ outProdname + ".tempGz";
			gzipAbsFilename = File::getAbsolutePath(gzipAbsFilename);

			this->doGzip( gzipPreserveInput, _expProcInfo.getDatapackageAbsPath(), packageContentRelPaths, gzipAbsFilename );

			//Updates for the next step
			packageContentRelPaths.clear();
			packageContentRelPaths.push_back( File::getFileName( gzipAbsFilename ) );
			_expProcInfo.setDatapackageAbsPath( currentFmtDir );

		} else if ( compressionType == CompressionTypeZIP ) {

			//Create a new datapackage dir for the zip step
			string currentFmtDir( File::getDirPath(_expProcInfo.getDatapackageAbsPath()) + "/Fmt_Zip/" );
			File::mkdir(currentFmtDir);

			string zipAbsFilename = currentFmtDir+"/"+ outProdname + ".tempZip";
			zipAbsFilename = File::getAbsolutePath(zipAbsFilename);

			this->doZip( _expProcInfo.getDatapackageAbsPath(), packageContentRelPaths, zipAbsFilename );

			//Updates for the next step
			packageContentRelPaths.clear();
			packageContentRelPaths.push_back( File::getFileName( zipAbsFilename ) );
			_expProcInfo.setDatapackageAbsPath( currentFmtDir );

		}

		if ( doTar || compressionType != CompressionTypeNONE ) {
			//If archived, only one file expected
			string currentName = _expProcInfo.getDatapackageAbsPath()+"/"+packageContentRelPaths.at(0);
			string finalName = File::fileWExt( currentName, archiveExtension);

			File::rename( currentName, finalName );

			ACS_LOG_INFO( "Formatted package ["<<finalName<<"]" );

		} else {
			ACS_LOG_INFO( "Output Package in ["<< _expProcInfo.getDatapackageAbsPath()<<"]. Archive type: none." );
		}
	}

	//Set the formatted filename. Used in notification.
	_expProcInfo.setNotificationListOfFiles( packageContentRelPaths );

	//Dump the updated I/O interface with PackageProcessing
	ACS_LOG_INFO("Updating "<<expProcInfoFilename);
	_expProcInfo.dump( expProcInfoFilename );

	//Return Ok
	return 0x00;
}

int PackageFmtEOFpackTask::main(int argc, char const * const * argv, char const * const * env) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	try {

		//Command Line printing - Useful for log inspection
		{
			ostringstream msg; msg<<"Executing: ";
			for ( int i=0; i<argc; i++  ) msg<<argv[i]<<" ";
			msg<<endl;

			ACS_LOG_INFO(msg.str());
		}

		//Command Line option definition
		//General Definition
		string opt;
		bool flag;
		po::options_description optGeneralDef(string("\nUsage: ")+argv[0]+" <PackageExporter IF file>\nwith the following parameter");
		optGeneralDef.add_options()
	        		//(cl.headerOnly.c_str(),   	po::bool_switch(&flag)   ->default_value(false), 		"flag.\nDefault=false\nIgnored if not applicable.\nApplicable when filetype has headerseparation.\nNot Mandatory.")
					(cl.headerOnly.c_str(),   	po::value< bool >(&flag) ->default_value(false), 		"arg: 0|1.\nIgnored if not applicable.\nApplicable when filetype has headerseparation.\nIf set, related data file is filtered out from package.\nNot Mandatory.")
	        		(cl.formatFrom.c_str(),   	po::value< string >(&opt)->default_value("fromCommand"),"arg: fromUsers|fromCommand.\nfromUser: take compression options from t_users.\nfromCommand: take compression options from command line.\nNot Mandatory.")
	        		(cl.cmdDoTar.c_str(),       po::value< bool >(&flag) ->default_value(true), 		"arg: 0|1.\nDo tar.\nNot Mandatory.")
	        		(cl.cmdCompression.c_str(), po::value< string >(&opt)->default_value("gzip"), 		"arg: none|zip|gzip.\nCompression format.\nNot Mandatory.")
	        		(cl.cmdExtension.c_str(),   po::value< string >(&opt)->default_value("TGZ"), 		"arg: <extension>.\nExtension of the final package.\nNot Mandatory.")
	        		(cl.zipLevel.c_str(), 		po::value< string >(&opt)->default_value("6"), 			"arg: [0..9].\nZip compression level.\nNot Mandatory.");


		//Exp proc info is positional
		po::options_description optPositDef("Positional parameters");
		optPositDef.add_options()
	        		(cl.expProcInfo.c_str(), po::value< string >(), "arg: <PackageExporter IF file>");

		//Add special Positional parameter in tail. Useful to ignore other parameter passed in command line
		optPositDef.add_options()( cl.IgnoredPositionals.c_str(), boost::program_options::value< vector<string> >() );

		//Merge all the option definitions
		po::options_description optAllDef("All options");
		optAllDef.add(optGeneralDef).add(optPositDef);

		//ExpProcInfo is positional
		po::positional_options_description p;
		p.add(cl.expProcInfo.c_str(), 1);

		//Add The special Ignoring Positional parameter in tail. -1 means one or more positional. After -1 cannot call other posPars.add func.
		p.add( cl.IgnoredPositionals.c_str(), -1 );

		boost::program_options::command_line_parser parser(argc, const_cast<char**>(argv));
		parser.options(optAllDef).positional(p).allow_unregistered();
		boost::program_options::parsed_options parsed = parser.run();

		try {
			po::store(parsed, _argvParameters);
		} catch (boost::program_options::unknown_option& ex) {
			ACS_LOG_INFO("Some command line parameters ignored.");
		}

		po::notify(_argvParameters);

		//Print Named Unrecognized options
		vector<string> unrecognizedOptions =
				boost::program_options::collect_unrecognized(parsed.options, boost::program_options::exclude_positional);
		for ( vector<string>::iterator it = unrecognizedOptions.begin(); it != unrecognizedOptions.end(); it++ ) {
			ostringstream msg; msg<<"Ignored Option: "<<*it<<endl; ACS_LOG_INFO(msg.str()) ;
		}
		//Print Positionals Unrecognized options
		if ( _argvParameters.count( cl.IgnoredPositionals ) ) {
			ACS_LOG_INFO("Some command line positional parameters ignored");
			vector<string> ignoredParams = _argvParameters[cl.IgnoredPositionals].as< vector<string> >();
			for (vector<string>::iterator it = ignoredParams.begin(); it != ignoredParams.end(); ++it) {
				ACS_LOG_INFO("Ignored Positional Option: "<<*it);
			}
		}

		//MANDATORY PARAMETERS CHECK
		if ( !_argvParameters.count( cl.expProcInfo ) ) {
			ostringstream msg; msg<<optGeneralDef;
			ACS_LOG_INFO( msg.str() );
			return 1;
		}

		//No Log rotation. A new log file is created in the working dir.
		// Note: privinfo messages will be collected in the log file. Project App could filter them out.
		const string expProcInfoFilename = _argvParameters[ cl.expProcInfo ].as<string>();
		string logFileName = File::tempname( File::getDirPath( expProcInfoFilename ) +"/"+ applicationName() +"_" );
		logFileName += ".log";

		LogRotationPolicy lrp(logFileName, LogRotationPolicy::FirstApplying, 0, 0, 1, false) ;
		Application::setLRPolicy (lrp);

		Application::instance()->setRotateLogParametersFromConf(false);
		Application::rotateLog();

		ACS_LOG_INFO( "Detailed log messages written in ["<<logFileName<<"]" );
		//

		int status = this->execute();

		ACS_COND_THROW( status!=0, exPackageFmtEOFpackTaskCriticalException(string("Failed executing task on [")+_argvParameters[cl.expProcInfo].as<string>()+"]"));

		ACS_LOG_INFO("Exiting with SUCCESS.");

		return 0;

	} catch(exception& ex) {

		ACS_LOG_ERROR("Exiting with ERROR. Error message: "<<ex.what());

		return 0xFF;
	} catch(...) {
		ACS_LOG_ERROR("Exiting with ERROR. Unknown Exception cought.");
		return 0xFF;
	}
}

_ACS_END_NAMESPACE
