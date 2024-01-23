// PRQA S 1050 EOF
/*

	Copyright 2020-2021, Exprivia SpA Defence & Aerospace
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Database Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 5.13  2018/09/27 15:20:00  marpas
	auto_ptr changed into unique_ptr whenever possible
	
	Revision 5.12  2017/11/27 15:32:57  marpas
	qa warning fixed
	
	Revision 5.11  2017/09/25 15:43:10  lucio.pulvirenti
	S2PDGS-1843: oprional boolean parameter to consider conf space as absolute path added to CTOR and coherently used in mergeIn,
	get and update methods.
	
	Revision 5.10  2017/05/03 10:21:23  lucio.pulvirenti
	S2PDGS-1754: Upgrade of version managed.
	
	Revision 5.9  2017/04/27 11:39:14  lucio.pulvirenti
	S2PDGS-1754: Base class get and update methods implemented to get resource space from db and reload it.
	
	Revision 5.8  2017/04/26 15:44:06  lucio.pulvirenti
	S2PDGS-1754: WORK IN PROGRESS.
	
	Revision 5.7  2016/05/11 09:38:39  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.6  2016/04/28 09:20:44  marpas
	useless members removed and interface simplified
	
	Revision 5.5  2016/04/11 13:27:13  marpas
	adopting new db interfaces
	
	Revision 5.4  2016/03/26 21:42:59  marpas
	using new db I/F
	
	Revision 5.3  2014/10/30 10:55:47  marpas
	during merge checks LockException instead of IOException from file locking functions
	
	Revision 5.2  2013/07/18 11:11:53  marpas
	different exception raised in case the space was not loaded or not found in the db.
	
	Revision 5.1  2013/06/11 14:27:21  marpas
	coding best practices applied
	
	Revision 5.0  2013/06/11 14:16:32  marpas
	adopting libException 5.x standards
	
	Revision 2.9  2013/03/14 15:27:23  marpas
	connection pool concept use enforced
	interface rationalization
	
	Revision 2.8  2013/03/12 17:44:57  marpas
	dbConnectionPool used in file actions
	
	Revision 2.7  2013/01/24 19:22:39  marpas
	connection pool management improved
	
	Revision 2.6  2013/01/24 10:39:48  marpas
	dbConnectionPool correctly used (if given)
	
	Revision 2.5  2013/01/23 17:23:09  marpas
	introducing dbConnectionPool for factories
	coding best practices
	interface rationalization
	qa rules
	
	Revision 2.4  2012/12/11 17:30:29  marpas
	qa rules
	
	Revision 2.3  2012/02/13 16:53:02  marpas
	refactoring
	
	Revision 2.2  2009/08/03 18:26:10  marpas
	faster and less connections to db.
	
	Revision 2.1  2009/03/30 17:48:54  marpas
	robust implementation
	rules ...
	
	Revision 2.0  2006/02/28 09:09:03  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.19  2005/06/20 13:01:26  enrcar
	std namespace handling added
	
	Revision 1.18  2005/06/15 07:42:40  integrator
	multiple storage manager implemented
	
	Revision 1.17  2005/05/31 14:23:25  integrator
	file actions interfaces changed
	
	Revision 1.16  2005/03/17 12:42:56  marpas
	headers fixed
	
	Revision 1.15  2005/01/26 18:54:34  ivafam
	Control added
	
	Revision 1.14  2005/01/26 17:36:41  ivafam
	dbDownloadConfigurationSpace I/F changed
	
	Revision 1.13  2004/06/04 08:17:31  marfav
	dbResourceSpaceException now inherits from SpaceLoadException
	
	Revision 1.12  2004/02/03 18:24:35  paoscu
	dbPersistent interface changed.
	
	Revision 1.11  2003/10/16 17:27:09  marpas
	connection exception is now caught
	
	Revision 1.10  2003/08/01 16:11:14  marpas
	single connection in downloading file
	speedup in case of master/slave
	
	Revision 1.9  2003/07/11 21:27:53  marpas
	exException::clone implemented
	
	Revision 1.8  2003/03/14 19:34:14  marpas
	factory used to create the dbPersistent
	
	Revision 1.7  2003/03/11 11:43:07  marpas
	error messages improved
	
	Revision 1.6  2003/03/05 10:29:00  marpas
	uses File::Lock::tryLock static method
	
	Revision 1.5  2003/03/03 18:14:00  marpas
	resource lock while downloading and reading
	
	Revision 1.4  2003/02/03 18:56:53  marpas
	Parse exceptions are no longer recovered internally
	
	Revision 1.3  2003/01/31 15:12:08  marpas
	LIB_INTEGRATION_PATH variable management
	
	Revision 1.2  2003/01/14 15:41:48  marpas
	versioning ...
	
	Revision 1.1.1.1  2003/01/14 15:34:55  marpas
	libDbConfiguration Import
		

*/


#include <dbResourceSpace.h>
#include <ResourceSpaceFactory.h>
#include <dbConnectionPool.h>
#include <dbConfigurationSpace.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>
#include <dbSmartQuery.h>
#include <XMLIstream.h>
#include <rsResourceSet.h>
#include <dbDownloadFile.h>
#include <dbFileActionFactoryChain.h>
#include <dbFileActionScopeGuard.h>
#include <Filterables.h>
#include <ftmpstream.h>


#include <Dir.h>
#include <File.h>
#include <fstream>
#include <dbcf.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

namespace {
    // versioning support
    dbcf lib_version_ ;
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbResourceSpace)

// constructor
dbResourceSpace::dbResourceSpace(const string &spc, const string &ver, dbConnectionPool &cp, bool absPath) :
	ResourceSpace(spc,ver, absPath),
    _connectionPool(cp)  // PRQA S 2528
{
}


dbResourceSpace::dbResourceSpace(const dbResourceSpace &o) : 
    ResourceSpace(o), 
    _connectionPool(o._connectionPool)  // PRQA S 2528
{
}

dbResourceSpace &dbResourceSpace:: operator=(const dbResourceSpace &o) 
{
	if (this != &o) {
		ResourceSpace::operator=(o) ;
        // connection pool not copied
	}
	return *this ;
}


dbResourceSpace::~dbResourceSpace() throw() {}

void dbResourceSpace::fillResources(const string &path, rsResourceSet &myrs)
{
    rsResourceSet::MergeMode oldmode = myrs.mergeMode() ;
	try {
		// the file is an XML one
		XMLIstream  file(path);			
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading xml: " << path) ;

		// allow last override mode on container
		myrs.mergeMode(rsResourceSet::ModeUnion) ;

		// load the container with the content of the file
		file >> myrs ;
		myrs.mergeMode(oldmode) ;
	}
	catch (exception &e) {
		myrs.mergeMode(oldmode) ;
		ACS_THROW(SpaceParseException(e,"Cannot load/parse path:\"" + path +"\"")) ; // PRQA S 3081
	}
}

void dbResourceSpace::lockAndFill(const std::string &filepath, rsResourceSet &myrs) 
{
    File::Lock lock(filepath) ;
    File::Lock::tryLock(lock, 10, 1000, true) ; // will try 10 times with 1 second timeout, then File::Lock::LockException // PRQA S 4400

    fillResources(filepath,myrs) ;

    // store the whole path in the _replacement to signal 
    // the loading of that file instead of the one on the DB
    replacement(filepath) ; 
}


bool dbResourceSpace::mergeIn(rsResourceSet &rs, bool std_path)
{
	replacement("") ;
	rsResourceSet myrs ;
	
	string filepath=path(std_path) ;
	if( absolutePath() ) {
		try {
			fillResources(filepath, myrs) ;
		}
		catch (SpaceParseException &) {
			// specific parse exception just rethrow it
			throw ;
		}
		catch(exception &x) {
	    	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
				ACS_LOG_NOTIFY_EX(x) ;
			ACS_CLASS_END_DEBUG
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading dbConfigurationSpace: " << space() << " versions: " << version()) ;
			// check if the file exists anyway
			if (File::exists(filepath)) {

                lockAndFill(filepath, myrs) ;

			}
			else { // the file with the requested version doesn't exists
				ACS_THROW(dbSpaceNotFoundException(x,"Cannot find space " + space() + (version().empty() ? string(" with no version") : string(" version ") + version()) + " on disk")) ; // PRQA S 3081, 3380, 3385
			}
		}	
	}
	else {
		dbConfigurationSpace *s= dynamic_cast<dbConfigurationSpace*>(dbPersistentFactoryChain::instance()->newObject("dbConfigurationSpace", _connectionPool.getKey())); // PRQA S 3081, 4412
		if (!s) {
			ACS_THROW(dbSpaceLoadException("Cannot allocate a dbConfigurationSpace")) ; // PRQA S 3081
		}
		dbPersistentScopeGuard theObjScopeGuard(s) ;
		s->setNamespace(space()) ; 
		s->setVersion(version()) ; 


		try {
        	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading dbConfigurationSpace: " << space() << " version: " << version()) ;
        	{
            	dbConnectionWrapper pw(_connectionPool) ;
            	dbConnection &theConnection = pw ; // PRQA S 3050 
		    	if (!s->load(theConnection)) { // not found
			    	ACS_THROW(dbSpaceNotFoundException("Space not found on db: " + space() + (version().empty() ? string(" with no version") : string(" version ") + version()))) ; // PRQA S 3081, 3380, 3385
		    	}
			}
			// set the current version
			version(s->getVersion()) ;
			
			// must re-compute the filename to be used, the version might be different now
			filepath=path(std_path) ;
			
			File::Lock lock(filepath) ;
			File::Lock::tryLock(lock) ; // will try 10 times with 1 second timeout, then File::Lock::IOException
			// in case no exclusive lock can be done ... see below ... shared attempt
			// start the download of the configuration
        	dbFileAction* dcs =  0 ;
        	dcs = dbFileActionFactoryChain::instance()->newDownloadAction(_connectionPool,*s,filepath);
        	if(!dcs)
			{
				ACS_THROW(exIllegalValueException("Cannot allocate a download action.")); // PRQA S 3081
			}
			dbFileActionScopeGuard guard(dcs);
			dcs->start();

			fillResources(filepath, myrs) ;

		}
		catch (SpaceParseException &) {
			// specific parse exception just rethrow it
			throw ;
		}
		catch(exception &x) {
	    	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
				ACS_LOG_NOTIFY_EX(x) ;
			ACS_CLASS_END_DEBUG
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading dbConfigurationSpace: " << space() << " versions: " << version()) ;
			// check if the file exists anyway
			// version requested ?
			if (version() == "") {
				// try to recover loading the space with the greatest version

				// here we count on no version passed as for the previous if 
				string radix = buildPathWithoutExtension(space(),"") + "_";

				string candidate ;
				double version = 0 ; // PRQA S 2502

				Dir dir(configurationPath(std_path)) ;
				for (unsigned long i =0; i < dir.size(); i++) {

					if (File::getExt(dir[i]) != dbResourceSpace::defaultExtension()) { // wrong extension
						continue ;
                	}

					string::size_type start = dir[i].find(radix) ;

					if (start == 0) { // the file could be a candidate
						double cver = atof(dir[i].c_str()+radix.length()) ; // PRQA S 3084
						if (cver > version) {
							version = cver ;
							candidate = dir[i] ;					
						}
					}
				}
				if (candidate.empty()) { // no candidate throw an exception		
					ACS_THROW(dbSpaceNotFoundException(x,"Cannot find space " + space() + " on disk")) ;
				}

				string filetl = configurationPath(std_path)+"/"+candidate ;

                lockAndFill(filetl, myrs) ;

			}
			else { // the path is assumed complete: with version
				if (File::exists(filepath)) {

                    lockAndFill(filepath, myrs) ;
				}
				else { // the file with the requested version doesn't exists
					ACS_THROW(dbSpaceNotFoundException(x,"Cannot find space " + space() + (version().empty() ? string(" with no version") : string(" version ") + version()) + " on disk")) ; // PRQA S 3081, 3380, 3385
				}
			}				
		}	
	}
    if (not myrs.empty()) {
	    rsResourceSet::MergeMode oldmm = rs.mergeMode() ;
	    rs.mergeMode(rsResourceSet::ModeUnion) ;
	    rs.merge(myrs) ;
	    rs.mergeMode(oldmm) ;
        return true ;
    }
    
    return false ;
}

void dbResourceSpace::get(string & resSpace, bool std_path)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	string filename;
    ACS_SMARTPTR<FileGuard> fg;

	if( absolutePath() ) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Going to get \"" << space() << '\"' );
		ACS_COND_THROW( not File::exists( space() ), dbSpaceNotFoundException('\"'+ space()+"\" space path does exist") ) ; // PRQA S 3081, 3380, 3385

		filename = space();
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Going to download \"" << space() << '\"' << ( version().empty() ? "" : " version="+version()) ); // PRQA S 3380, 3385

		dbConnectionWrapper pw(_connectionPool) ;
		dbConnection &conn = pw ; // PRQA S 3050 

		dbSmartPersistent sp("t_configurationspaces", conn);
		sp = getResourceRecord(conn, version());
		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			bool isNull = false ;
			string version; // version of record to update
			sp.get("version", version, isNull, conn);
			ACS_LOG_DEBUG(SimpleDebugSignature << "Retrieved \"" << space() << "\" t_configurationspaces version=" <<version );
    	ACS_CLASS_END_DEBUG ;

		filename = File::tempname("/tmp/"+ space(), ".clob"); //Temp filename // PRQA S 3050
		fg.reset(new  FileGuard(filename) );
		sp.downloadFile("space", filename, dbConnectionWrapper(_connectionPool)) ; // PRQA S 3050, 3081
	}
	
	{
		std::ifstream ifs( filename.c_str() );
		std::stringstream buffer;
		buffer << ifs.rdbuf();
		resSpace = buffer.str();
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "space:\n" << resSpace) ;
	
}


void dbResourceSpace::update(const string &resSpace, bool upgradeVersion, bool std_path ) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	if( absolutePath() ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to update \"" << space() << '\"' << ( version().empty() ? "" : " version="+version() ) ); // PRQA S 3380, 3385
        ACS_SMARTPTR<oftmpstream> of;
		string filename = getTemporaryResourceFilepath(  resSpace, of, std_path );
		File::rename(filename, space() );
		
	}
	else { 
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to upload \"" << space() << '\"' << ( version().empty() ? "" : " version="+version() ) ); // PRQA S 3380, 3385
		
		dbConnectionWrapper pw(_connectionPool) ;
		dbConnection &conn = pw ; // PRQA S 3050 

		const string &dbK = conn.getKey() ;
		dbSmartQuery sq("t_configurationspaces", dbK );

		getResourceRecords( sq, conn, version() );
		ACS_COND_THROW(sq.size() == 0, dbSpaceNotFoundException('\"'+space()+'\"' + ( version().empty() ? "" : " version=" + version()) + " not occurring in T_ConfigurationSpaces"));	 // PRQA S 3081, 3380, 3385

		bool isNull = false ;
		string recVersion; // version of record to update
		sq.at(0)->get("version", recVersion, isNull, conn);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << space() << "\" t_configurationspaces version=" << recVersion );

		bool update = true;
		if( upgradeVersion ) {
			recVersion = computeNewVersion( recVersion, conn );
			update = false;
		}

        ACS_SMARTPTR<oftmpstream> of;

		string filename = getTemporaryResourceFilepath(  resSpace, of, std_path );

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to upload \"" << space() << "\" t_configurationspaces with version=" << recVersion );
		// Load configuration space 
		loadResourceRecord(recVersion, filename, conn, update);
	}

}


string dbResourceSpace::defaultExtension() const
{
	return "xml" ;
}


void dbResourceSpace::getResourceRecords(dbSmartQuery & query, dbConnection &conn, const string & version) // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

	const string &dbK = conn.getKey() ;
	
	{
		dbQueryParameters parameters(dbK) ; 
		parameters.addParameter(dbParam(dbK, "T_ConfigurationSpaces","Namespace", dbParam::EQUAL_TO, db::toSQL(dbK, space()))) ;
		query.addParameter(parameters) ;
	}
	
	if( not version.empty() ) {
		dbQueryParameters parameters(dbK) ; 
		parameters.addParameter( dbParam(dbK, "T_ConfigurationSpaces","version", dbParam::EQUAL_TO, db::toSQL(dbK, version)) ) ;
		query.addParameter(parameters) ;
	}
	else {
		query.setOrderBy("version DESC") ;
	}
	query.doQuery(conn) ;

}

dbSmartPersistent dbResourceSpace::getResourceRecord(dbConnection &conn, const string & version)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

	const string &dbK = conn.getKey() ;
	dbSmartQuery sq("t_configurationspaces", dbK );
	
	getResourceRecords(sq, conn, version);

	ACS_COND_THROW(sq.size() == 0, dbSpaceNotFoundException('\"'+space()+'\"' + ( version.empty() ? "" : " version=" + version) + " not occurring in T_ConfigurationSpaces"));	 // PRQA S 3081, 3380, 3385
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "T_ConfigurationSpaces record with Namespace=\"" << space() << '\"' << ( version.empty() ? "" : " version=" + version) << " found") ; // PRQA S 3380, 3385
	
	ACS_COND_THROW( !(sq.at(0)->isSet ("space", conn)), dbSpaceLoadException("space of T_ConfigurationSpaces \""+space()+'\"' + ( version.empty() ? "" : " version=" + version) + " is NULL"));	 // PRQA S 3081, 3380, 3385
	
	return *sq.at(0);

}

void dbResourceSpace::loadResourceRecord(const string & version, const string & resourceFilename, dbConnection &conn, bool update) // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	ACS_COND_THROW( not File::exists(resourceFilename), dbSpaceLoadException('\"'+resourceFilename+"\" file does not exist") ); // PRQA S 3081
	
	// Update configuration space 
	dbConfigurationSpace *configurationSpace= dynamic_cast<dbConfigurationSpace*>(dbPersistentFactoryChain::instance()->newObject("dbConfigurationSpace", conn.getKey())); // PRQA S 3081, 4412
	dbPersistentScopeGuard spaceGuard(configurationSpace) ;
	configurationSpace->setNamespace(space()) ;
	configurationSpace->setVersion(version) ;
	configurationSpace->setSpaceUploadFileName(resourceFilename) ;
	if( update ) {
		configurationSpace->remove(conn) ;
	}
	configurationSpace->save(conn) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "t_configurationspaces \"" << space() << "\" with version=" << version << " successfully " << ( update ? "updated" : "saved" ) ); // PRQA S 3380
	
}


string dbResourceSpace::getTemporaryResourceFilepath(  
        const string &resSpace, 
        ACS_SMARTPTR<oftmpstream> & of,

        bool std_path ) // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

	string tempFilename = space();
	if( absolutePath() ) {
		tempFilename = File::removeFExt( File::getFileName(space()) );
	}
	const string prefix = "/tmp/"+ tempFilename + '_';
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Prefix = \"" << prefix << '\"') ;
	
	of.reset(new oftmpstream( prefix, defaultExtension() ) );
	
	string filename = of->path();
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\"" << filename << "\" temporary file generated to write updated configuration space") ;
	(*of.get())() << resSpace;
	
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignature << '\"' << filename << "\" temporary file will NOT be removed for debug") ;
		of->setAutoRemove(false);
	ACS_CLASS_END_DEBUG
	of->close();
	
	return filename;
}

string dbResourceSpace::computeNewVersion( const string & version, dbConnection & conn)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	string newVersion;
	incrementVersion(version, newVersion);
	
	// check version already occurs
	const string &dbK = conn.getKey() ; 
	dbConfigurationSpace *s= dynamic_cast<dbConfigurationSpace*>(dbPersistentFactoryChain::instance()->newObject("dbConfigurationSpace", dbK)); // PRQA S 3081, 4412
	s->setNamespace(space()) ;
	s->setVersion(newVersion) ;
	if( s->load(conn) ) { // if configuration space of upgraded version already occurs, the last version conf space is retrieved and version is upgraded again
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "t_configurationspaces \"" << space() << "\" with version=" << newVersion << " already occurs, going to compute a greater version");
		
		dbSmartPersistent sp("t_configurationspaces", conn);
		sp = getResourceRecord(conn);
		bool isNull = false ;
		string recordVersion; // version of record to update
		sp.get("version", recordVersion, isNull, conn);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "t_configurationspaces \"" << space() << "\" last version=" << recordVersion << " going to upgrade");
		incrementVersion(recordVersion, newVersion);
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "t_configurationspaces \"" << space() << "\" original version=" << version << ", upgraded version=" << newVersion);

	return newVersion;
} 
_ACS_END_NAMESPACE
