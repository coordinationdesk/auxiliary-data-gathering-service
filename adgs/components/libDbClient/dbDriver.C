// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
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
	Revision 5.3  2014/02/07 18:36:52  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.2  2013/06/19 18:12:08  marpas
	removing compilation warning over deprecated include files
	
	Revision 5.1  2013/06/11 17:02:59  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:58  marpas
	adopting libException 5.x standards
	
	Revision 4.14  2013/06/03 12:36:26  marpas
	coding best practices applied
	qa rules
	
	Revision 4.13  2013/05/30 11:21:58  marpas
	getting rid of useless code
	
	Revision 4.12  2013/05/30 11:18:41  marpas
	dbDriver is now a Singleton (ParamConstructibleSingletonMap)
	
	Revision 4.11  2013/05/29 18:14:16  marpas
	the so_loader is now an auto_ptr to allow its dtor to be called in the right order
	NB the driver MUST be destoryed BEFORE the plugin
	
	Revision 4.10  2013/05/29 16:48:56  marpas
	private data order rearrangement, to avoid dtor of dbDriver to be called after plugin unload
	
	Revision 4.9  2013/05/14 18:13:59  marpas
	dbQuery.h
	
	Revision 4.8  2013/02/26 17:12:24  marpas
	coding best practice appplied
	interface rationalization
	
	Revision 4.7  2013/01/24 19:27:58  marpas
	query stat strings fixed
	exception notification improved
	useless debug macros removed
	some stats added
	
	Revision 4.6  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.5  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.3  2012/03/01 15:23:01  marpas
	introducing new db.conf path policy
	
	Revision 4.2  2012/02/17 13:55:14  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:14  marpas
	new statistics adopted
	
	Revision 1.1  2010/09/11 17:43:00  marpas
	renamed from dbxDriverWrapper
	adopt libDbExpress 3.2 interface
	
	Revision 3.2  2010/09/08 17:47:26  marpas
	debug improved
	
	Revision 3.1  2010/02/02 18:27:36  enrcar
	EC:: debug improved
	
	Revision 3.0  2009/10/05 14:47:54  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.5  2009/07/14 12:21:47  enrcar
	EC:: c++rules
	
	Revision 2.4  2009/07/14 09:31:45  matteo.airoldi
	added new define to overrun unsupported cast on RHEL3 (pig action to many RHEL 3 define)
	
	Revision 2.3  2009/05/22 15:13:50  marpas
	some rules fixed - need more work

	Revision 2.2  2007/09/14 14:15:33  lucio.pulvirenti
	Extra debug printouts added.
	
	Revision 2.1  2006/06/28 16:57:53  marpas
	getPlugin() const method added
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2005/10/25 19:43:12  marpas
	fix a loop when there are more than one db server configured
	
	Revision 1.7  2005/06/09 08:41:54  enrcar
	*** empty log message ***
	
	Revision 1.6  2005/05/03 10:23:48  enrcar
	connectionString removed
	unused constructor removed
	
	Revision 1.5  2005/04/27 10:05:52  enrcar
	work in progress
	
	Revision 1.4  2005/04/21 17:11:15  marpas
	work in progress
	
	Revision 1.3  2005/04/19 15:06:29  enrcar
	*** empty log message ***
	
	Revision 1.2  2005/03/31 08:54:50  enrcar
	*** empty log message ***
	
	Revision 1.1.1.1  2005/03/07 14:00:35  marpas
	Importing dbExpressWrapper
	
	

*/


#include <dbDriver.h>
#include <SQLDialect.h>
#include <Plugin.h>
#include <dbAppResSet.h>
#include <Filterables.h>

#include <dlfcn.h>

#include <dbV.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

namespace {
    dbV version ;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbDriver)

dbDriver::dbDriver(const string& key) :
    pattern::ParamConstructibleSingletonMap<dbDriver, std::string>(),
    ThreadSafe(),
    _sqlDriver(),
	_getDriver(0),
	_key(key),
#if __cplusplus > 199711L
	_so_loader(nullptr)
#else
	_so_loader(0)
#endif
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "this: " << this) ;

	// just to initialize the instance (if not done yet! otherwise it is doesn't do anything)
	const string def_conf = "" ; 
	dbAppResSet* ars = dbAppResSetFactory::instance(def_conf, _key) ;

	const dbConnectionParameters &dbi=ars->connectionParameters() ;
    
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "this: " << this  
            << " calling init:  key: \"" << _key << "\"  Obtained values:  vendorLibrary: \"" << dbi->vendorLibrary() << "\"  driverPath: \"" 
            << dbi->driverLibrary() << "\"  function: \"" << dbi->getDriverFunc() << "\"") ;
    
	init(dbi->driverLibrary(),dbi->vendorLibrary(), dbi->getDriverFunc()) ;
}

dbDriver::~dbDriver() throw()
{
// PRQA S 4631 L1
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "this: " << this  
            << " _sqlDriver: " << _sqlDriver.get()
            << " _so_loader: " << _so_loader.get()
            << " released in their respective order") ;
// PRQA L:L1
}

void dbDriver::init(const std::string &dbx_so_path, const std::string &vendorLib, const std::string &function) 
{
	Lock lock(*this) ;	/* Mutex */
 
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "this: " << this  
		    <<  "  vendorLibrary: \"" << vendorLib 
            << "\" driverPath: \"" << dbx_so_path 
            << "\" function: \"" << function << "\"") ;

	if (!_so_loader.get()) {

#if __cplusplus > 199711L
		_so_loader=unique_ptr<Plugin>(new Plugin(dbx_so_path,false,"",RTLD_NOW|RTLD_GLOBAL)) ; // PRQA S 3050, 3081
#else
		_so_loader=auto_ptr<Plugin>(new Plugin(dbx_so_path,false,"",RTLD_NOW|RTLD_GLOBAL)) ; // PRQA S 3050, 3081
#endif
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "this: " << this  
			    << "  so_loader: " << _so_loader.get()
			    << "  loading so: " << dbx_so_path) ;

		void *SQLDllHandle = _so_loader.get()->open() ;	/* Load the shared object */

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "this: " << this  
                << " locating symbol: \"" << function << "\". " 
                << " open error (if any): \"" << _so_loader.get()->openError() << "\"")
				
		
		void *GetDriver = dlsym(SQLDllHandle,function.c_str()) ;	/* Obtain the entry point function */
		
		
		if (!GetDriver) {
			ostringstream os ;	/* Unable to obtain function */
			os << "Unable to find symbol \"" << function << "\"" ;
			ACS_THROW(dbDriver::LoadException(os.str(),errno)) ; 
		}

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "this: " << this  
                << " symbol found : " << GetDriver) ;
		

		_getDriver = reinterpret_cast<getSQLDriverPrototype>(GetDriver) ; // PRQA S 3081

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "this: " << this  
			    << " calling : _getDriver with vendorlib:" << vendorLib) ;
		
		// Try getting the driver
		long result = SQLDialect::SQLReturn(_key, dbSQLDriverCreator( _getDriver, vendorLib.c_str(),"",_sqlDriver)) ; // PRQA S 4412
		try {
			if (result != SQL_SUCCESS) {
				ostringstream os ;
				os << "Error calling symbol " << function << "(" << vendorLib << ") [" << result << "]" ;
				ACS_THROW(dbDriver::LoadException(os.str(),errno)) ; 
			}
		}
		catch(exception &x) {
			if (!_sqlDriver) {
				throw ;
            }
			ACS_LOG_WARNING(x) ;
		}
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "this: " << this  
			    << " _getDriver returned : " << _sqlDriver.get()) ;
		
		// Set options
		long ret = SQLDialect::SQLReturn(_key, _sqlDriver->setOption(eDrvRestrict,0)) ;
		if (ret != SQL_SUCCESS) {
			ostringstream os ;
			os << "SetOption(eDrvRestrict,0) returned " << ret ;
			ACS_THROW(dbDriver::LoadException(os.str())) ; // PRQA S 3081
		}
	}	/* if (!_so_loader) */
	
}
	
	
const dbSQLDriver &dbDriver::getDbDriver() const 
{
	Lock lock(*this) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "this: " << this) ;
	return _sqlDriver ;
}

dbSQLConnection dbDriver::getSqlConnection() const // PRQA S 4214
{
	ACS_COND_THROW(!_sqlDriver,LoadException("No sqldriver loaded yet")) ; // PRQA S 3081
	Lock lock(*this) ;
	dbSQLConnection sqlcon ;
		
	long ret = SQLDialect::SQLReturn(_key, dbSQLConnectionCreator(_sqlDriver,sqlcon)) ;	/* Obtain SQL Connection */
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "this: " << this 
            << " getSQLConnection returned: " << ret << " (" << sqlcon.get() << ")") ;
	if (ret != SQL_SUCCESS) {
		ostringstream os ;	/* Unable to get SQL Connection */
		os << "getSQLConnection returned " << ret ;
		ACS_THROW(LoadException(os.str())) ; // PRQA S 3081
	}
	return sqlcon ;
}


const Plugin *dbDriver::getPlugin() const // PRQA S 4214 
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "this: " << this 
		    << "  _so_loader: " << _so_loader.get()) ;
	return _so_loader.get() ;
} 


_ACS_END_NAMESPACE

