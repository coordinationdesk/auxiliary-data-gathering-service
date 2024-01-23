// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 5.7  2017/11/21 18:42:31  marpas
	qa warning fixed
	
	Revision 5.6  2017/03/22 11:26:49  marfav
	Removing boost shared_ptr from global variable due to crash in global dtors
	
	Revision 5.5  2016/04/22 10:40:59  marpas
	changes to ease doxygen extraction in progress
	
	Revision 5.4  2014/02/07 18:36:52  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.3  2013/12/19 12:24:34  marpas
	Transaction class added
	
	Revision 5.2  2013/07/09 08:24:15  marpas
	compilation warning removed
	
	Revision 5.1  2013/06/19 18:12:07  marpas
	removing compilation warning over deprecated include files
	
	Revision 5.0  2013/06/06 18:52:56  marpas
	adopting libException 5.x standards
	
	Revision 4.13  2013/05/14 18:07:39  marpas
	useless classes support removed. Interface rationalization.
	ATTENTION: ctor signature has changed meaning.
	
	Revision 4.12  2013/02/26 17:12:24  marpas
	coding best practice appplied
	interface rationalization
	
	Revision 4.11  2013/01/24 19:27:57  marpas
	query stat strings fixed
	exception notification improved
	useless debug macros removed
	some stats added
	
	Revision 4.10  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.9  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.6  2012/03/05 10:56:14  marpas
	passing empty string to dbAppResSet::setDefaultDbConfName will set base::defaultDbConfFileName() as default
	
	Revision 4.5  2012/03/01 18:19:15  marpas
	refactoring in progress
	
	Revision 4.4  2012/03/01 15:23:01  marpas
	introducing new db.conf path policy
	
	Revision 4.3  2012/02/17 13:55:13  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.1  2011/12/23 09:46:26  marpas
	thread safety improved
	
	Revision 4.0  2011/10/18 12:31:12  marpas
	new statistics adopted
	
	Revision 3.2  2011/07/18 17:35:59  marpas
	debug improved
	test compilation fixed
	
	Revision 3.1  2010/09/11 18:10:23  marpas
	libDbExpress 3.2 interface adopted, libException 3.3 too
	
	Revision 3.0  2009/10/05 14:47:49  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.6  2009/06/22 12:36:35  marpas
	robustness improved
	got rid of useless ifdef and warning
	useless code removed
	test compilation fixed
	
	Revision 2.5  2008/10/14 14:38:12  marpas
	error message improved
	
	Revision 2.4  2007/09/14 14:14:44  lucio.pulvirenti
	Extra debug printouts added.
	
	Revision 2.3  2006/06/26 12:07:55  ivafam
	StringUtils::compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 2.2  2006/06/20 16:35:12  ivafam
	Method compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 2.1  2006/03/09 16:36:01  marpas
	new dbColumn implementation - column type moved into dedicated classes
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2006/01/17 11:13:16  marpas
	getSMPluginPath method added, it relies on <Dialect>.SMPluginConfFile to load the full path of the plugin conf file
	
	Revision 1.7  2006/01/11 16:14:35  marpas
	dialect initialization forced at first use
	
	Revision 1.6  2005/10/25 12:34:15  marpas
	getSQLDialect method implemented (returns the dialect string)
	
	Revision 1.5  2005/04/29 16:13:34  enrcar
	*** empty log message ***
	
	Revision 1.4  2005/04/21 17:11:15  marpas
	work in progress
	
	Revision 1.3  2005/04/19 08:16:10  marpas
	work in progress
	
	Revision 1.2  2005/04/18 19:42:27  marpas
	work in progress
	
	Revision 1.1.1.1  2005/03/07 14:00:35  marpas
	Importing dbExpressWrapper
	
	Revision 1.17  2004/05/31 13:17:46  marpas
	dbLockGuard revived
	
	Revision 1.15  2004/03/08 22:58:14  marpas
	singleton relese at exit conditioned by the compilation flag RELEASE_SINGLETON_AT_EXIT
	
	Revision 1.14  2003/04/30 13:36:03  marpas
	using namespace std partially removed because informix headers
	
	Revision 1.13  2003/04/10 16:58:01  marpas
	GL_DATETIME setting changed
	
	Revision 1.12  2003/03/14 20:21:52  paoscu
	new policy to establish who is the arbiter.
	
	Revision 1.11  2003/03/14 19:16:30  paoscu
	Master/Slave policy implemented
	
	Revision 1.10  2003/03/11 16:18:38  paoscu
	Arbiter almost complete
	
	Revision 1.9  2003/02/26 17:28:42  paoscu
	dbAppResSet* instance now accepts a configuration string.
	
	Revision 1.8  2003/02/11 17:26:26  paoscu
	GL_DATETIME changed in "%Y-%m-%d %H:%M:%S.%F3" to get dbGeoObjects to work correctly.
	
	Revision 1.7  2003/01/31 15:06:21  paoscu
	GL_DATETIME env var fixed
	
	Revision 1.6  2003/01/24 11:33:00  paoscu
	dbRow diagnostic improved.
	
	Revision 1.5  2002/11/12 13:31:43  marpas
	#include <rsResourceSet.h> moved from dbAppResSet.h to dbAppResSet.C
	
	Revision 1.4  2002/11/08 15:39:20  marpas
	Singletons are now destroyed registering a private static function
	with atexit
	
	Revision 1.3  2002/10/18 11:20:34  paoscu
	Fixed a bug in nextSerial(unsigned long v) const
	
	Revision 1.2  2002/10/04 16:41:16  marpas
	Serial increment for database is now left to the dbAppResSet class instance.
	
	Revision 1.1.1.1  2002/10/02 09:58:02  marpas
	Import libDbClient
	

*/


#include <dbAppResSet.h>
#include <dbSchemaMetaDataProvider.h>
#include <SQLDialectFactory.h>
#include <SQLDialect.h>
#include <rsResourceSet.h>
#include <StringUtils.h>
#include <rsPushNSpace.h>
#include <Filterables.h>
#include <exStackTrace.h>
#include <Filterables.h>
#include <File.h>
#include <sstream>
#include <cstdlib>

_ACS_BEGIN_NAMESPACE(acs)

using std::string ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbAppResSet)
string dbAppResSet::_defaultDbConf = base::defaultDbConfFileName() ;

dbAppResSet::dbAppResSet(const std::string &key, const std::string &dbconf) :
    _key(key),
    _conf(dbconf)
{ 
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Key: \"" << key << "\"  dbconf: \"" << dbconf << "\"") ;
	
	init() ; 
}



std::string dbAppResSet::candidate_conf_file(const std::string &key, const std::string &conf, std::string &newkey) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "key: \"" <<  key << "\" conf: \"" << conf << "\" newkey: \"" << newkey << "\"" ) ;
    // if key is "" try getDefault
    if (key.empty()) { // note: this block infinite recursive call
        string fullp = string(acs::base::defaultDbConfPath())+"/"+conf + ".conf" ; // PRQA S 3081
	    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Trying: \"" <<  fullp << "\"") ;
        ACS_COND_THROW( ! File::isReadable(fullp), // PRQA S 3081
                                    exIOException("Conf file: \"" + fullp + "\" is not accessible") ) ;
        return fullp ; // found
    }
    
    string fullp = string(acs::base::defaultConfPath()) + "/" + key + "/database/" + conf + ".conf" ; // PRQA S 3081
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Trying: \"" <<  fullp << "\"") ;

    if ( ! File::exists(fullp)) {
	    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "\"" <<  fullp << "\" doe not exists") ;
        newkey = key ;
        return candidate_conf_file("",conf, newkey) ; // recursive call 
    }
    
    newkey = "" ; //default
    return fullp ;
    
}


void dbAppResSet::init() // PRQA S 5500
{
// NB cyclomatic complexity is just slightly high, but is due to the number of DEBUG control
// any of them introduces at least a conditional expression
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "key: \"" << _key << "\"  conf: \"" << _conf << "\"") ;

    string confkey ;
    string conf_file = candidate_conf_file(_key, _conf, confkey) ;

	_dbConfFile = conf_file ;

	ACS_CLASS_WRITE_DEBUG_TH(ACS_INS_VERB, "stack: \n" << exStackTrace::getStack(true) ) ;

	// default: acs::base::defaultDbConfPath()/db.conf
	// else <$DBCONFROOT>/database/<conf>.conf
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "loading: " << conf_file ) ;
	std::ifstream file ; 
	file.open(conf_file.c_str()) ; // open
	ACS_COND_THROW( ! file.good(),exFileOpenException("Cannot open \"" + conf_file +"\"" ,errno)) ;

	// try to load a db conf file
	rsResourceSet rset ;
	file >> rset ; //load it 
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "looking for section: \"" << confkey << "\"") ;
    
    if (confkey.empty()) {
        confkey = "DatabaseName" ;
    }
    
    try {
	    // wich database name shall we use ?
	    rset.getValue(confkey,_confDatabaseName) ;
    }
    catch(const std::exception &x) { // NOSONAR - any exception 
        ACS_THROW(exIllegalValueException(x,"Cannot find section: \"" + confkey + 
                                                 "\" in file: \"" + conf_file + "\"" )) ;
    }
    
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "DatabaseName: " << _confDatabaseName) ;

	// extract interesting fields
	{ // NOSONAR - rsPushNSpace scope
		rsPushNSpace databaseName(rset,_confDatabaseName) ;

		// the Dialect
		rset.getValue("Dialect",_sqlDialectName) ;

		// its plugin path
		string sqlDialectPath ;
		rset.getValue("DialectSoPath",sqlDialectPath) ;

        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "running SQLDialectFactory::init : "  
              << sqlDialectPath << " , "<< _sqlDialectName) ;

		// initalize dialect
		SQLDialectFactory::init(_key, sqlDialectPath, _sqlDialectName) ; 
		try {
        	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "trying to get SMPluginConfFile value") ;
			// the SMPluginConfFile (for storage managers)
			rset.getValue("SMPluginConfFile",_smPluginConfFile) ;
		}
		catch (const std::exception &) { // NOSONAR - any exception 
        	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "no SMPluginConfFile found, empty string will be the default") ;
			; // simply ignore it
		}
        size_t nhosts = 0 ;
        string reskey = "host" ; 
        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Looking for " << reskey << " array size" );
        try {
	        nhosts = rset.getArraySize(reskey) ;
        }
        catch (const std::exception &x) { // NOSONAR - any exception 
            ACS_THROW(exIllegalValueException(x,"Error in getting hosts numbers")) ;
        }
        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Found " << nhosts << " host(s) configured") ;
	    // no hosts ? error !
	    ACS_COND_THROW(0 == nhosts, exIllegalValueException("No hosts configured")) ; // PRQA S 3081
        
	    try {
		    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "loading connection parameters ") ;

			std::ostringstream os ;
			os << reskey << "[0]" ;
			rsPushNSpace pfx(rset,os.str()) ;
            _connectionParameters = SQLDialectFactory::connectionParameters(_key, "") ;
            if (_connectionParameters.get()) {
            	rsPushNSpace pns(rset,"dbConnectionArbiterCParam") ;
		        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "importing connection parameters") ;
                _connectionParameters->importObj(rset) ;
                if (_connectionParameters->assumeNoPostGIS()) {
                    dbSchemaMetaDataProvider::instance(_key)->assumeNoPostGIS() ;
                }
            }
	    }
	    catch (const std::exception &x) { // NOSONAR - any exception 
		    ACS_THROW(exIllegalValueException(x,"Error in getting connection parameters")) ;
	    }	
    }
	
    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "about to SQLDialectFactory::sqlDialect(" << _key << ").setEnv(rset) ") ;
	// and any other dialect environment to be initialized
	SQLDialectFactory::sqlDialect(_key).setEnv(rset) ; 
}

string dbAppResSet::getDefaultDbConfName() { return _defaultDbConf ; }
string dbAppResSet::setDefaultDbConfName(const std::string &n) { 
    string o = _defaultDbConf ; 
    _defaultDbConf = n.empty() ? string(base::defaultDbConfFileName()) : n ;  // PRQA S 3081, 3380, 3385
    return o ; 
}

string dbAppResSet::getSMPluginPath() const 
{
    if ( 0 == _smPluginConfFile.length()) {
        string(base::defaultSMPluginsPath()) + '/' + base::defaultSMPluginsFileName() ;
    }
    
    if ( _smPluginConfFile[0] == '/') {
        return _smPluginConfFile ; 
    }
    
    return string(base::defaultSMPluginsPath()) + '/' + _smPluginConfFile ;
}



std::ostream &operator<<(std::ostream &o, const dbAppResSet &s) 
{
    o << "getKey(): \"" << s.getKey() << "\"\n" ;
    o << "getSQLDialectName(): \"" << s.getSQLDialectName() << "\"\n" ;
    o << "getSMPluginPath(): \"" << s.getSMPluginPath() << "\"\n" ;
    o << "getDbConfFile(): \"" << s.getDbConfFile() << "\"\n" ;
    o << "getConfDatabaseName(): \"" << s.getConfDatabaseName() << "\"\n" ;

    return o ;  // PRQA S 4028
}  

acs::exostream &operator<<(acs::exostream &o, const dbAppResSet & s) // PRQA S 4222
{
    o << "getKey(): \"" << s.getKey() << "\"\n" ;
    o << "getSQLDialectName(): \"" << s.getSQLDialectName() << "\"\n" ;
    o << "getSMPluginPath(): \"" << s.getSMPluginPath() << "\"\n" ;
    o << "getDbConfFile(): \"" << s.getDbConfFile() << "\"\n" ;
    o << "getConfDatabaseName(): \"" << s.getConfDatabaseName() << "\"\n" ;

    return o ;  // PRQA S 4028
}  



//
//
//	Singleton Implementation
//
//

ThreadSafe dbAppResSetFactory::_mutex ; 

std::map<string, dbAppResSet*, std::less<> > dbAppResSetFactory::_instances ; 

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbAppResSetFactory)

dbAppResSet* dbAppResSetFactory::instance(const std::string& conf, const std::string& key) // PRQA S 4020
{
    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Key: \"" << key << "\"  conf: \"" << conf << "\"") ;
	
	ThreadSafe::Lock l(_mutex) ;
		
	const auto i =  _instances.find(key) ; 

	if ( i != _instances.end() ) { return (i->second) ; }
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Creating a new instance of dbAppResSet with key: \"" << key << "\"") ;

	dbAppResSet* tmp = nullptr ; 
	
	if(!conf.empty()) {
		tmp = new dbAppResSet(key, conf) ; // new conf NOSONAR - those pointers are in a singleton: never freed
	} else {
		tmp = new dbAppResSet(key, dbAppResSet::getDefaultDbConfName()) ; // default conf NOSONAR - those pointers are in a singleton: never freed
	}	
		
	_instances[key] = tmp ;

	return tmp;
}


_ACS_END_NAMESPACE
