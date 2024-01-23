// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2014/02/07 18:36:52  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.2  2013/06/12 17:07:03  marpas
	coding best practices and qa rules application in progress
	
	Revision 5.1  2013/06/11 17:02:59  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:55  marpas
	adopting libException 5.x standards
	
	Revision 4.10  2013/05/14 18:06:25  marpas
	interface rationalization.
	behaviour now try to get rid of memory during exit
	
	Revision 4.9  2013/04/27 14:44:54  marpas
	coding best practices: no exception from dtors
	
	Revision 4.8  2013/01/24 19:27:57  marpas
	query stat strings fixed
	exception notification improved
	useless debug macros removed
	some stats added
	
	Revision 4.7  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.6  2012/11/19 13:30:19  marpas
	HEAD REALIGNEMENT
	
	Revision 4.4  2012/03/01 15:23:01  marpas
	introducing new db.conf path policy
	
	Revision 4.3  2012/02/22 11:29:29  enrcar
	EC:: If the same database dialect is intanced more than once, the method addNewInstance of an already existent Plugin will be called. Therefore, any plugin is allowed to contain several instances of the same SQLDialect class (a map is used).
	
	Revision 4.2  2012/02/17 13:55:12  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:11  marpas
	new statistics adopted
	
	Revision 3.3  2010/09/11 18:11:55  marpas
	libException 3.3 interface adopted
	
	Revision 3.2  2010/09/08 17:50:10  marpas
	debug improved, other general code improvements
	
	Revision 3.1  2010/03/19 11:27:36  marpas
	class SQLDialectAppSpecific added to manage specific dialect issues for applications
	
	Revision 3.0  2009/10/05 14:47:47  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.4  2009/05/22 15:13:50  marpas
	some rules fixed - need more work
	
	Revision 2.3  2009/03/04 17:50:04  marpas
	CODECHECK parsing improved
	
	Revision 2.2  2009/03/02 14:03:29  marpas
	CODECHECK parsing, no regressions
	
	Revision 2.1  2006/03/09 16:36:01  marpas
	new dbColumn implementation - column type moved into dedicated classes
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.9  2006/01/11 22:48:36  marpas
	dbAppResSetFactory initialization improved and optimized
	
	Revision 1.8  2006/01/11 16:14:35  marpas
	dialect initialization forced at first use
	
	Revision 1.7  2005/09/27 14:54:44  enrcar
	Potential bug fixed in return value
	
	Revision 1.6  2005/04/29 16:13:09  enrcar
	*** empty log message ***
	
	Revision 1.5  2005/04/21 17:11:15  marpas
	work in progress
	
	Revision 1.4  2005/04/20 09:10:29  marpas
	work in progress
	
	Revision 1.3  2005/04/19 08:16:10  marpas
	work in progress
	
	Revision 1.2  2005/04/18 19:42:27  marpas
	work in progress
	
	Revision 1.1  2005/04/18 16:37:33  marpas
	just added
	
	

*/


#include <SQLDialectFactory.h>
#include <dbAppResSet.h>
#include <SQLDialect.h>
#include <exStackTrace.h>
#include <Filterables.h>
#include <Plugin.h>
#include <File.h>


_ACS_BEGIN_NAMESPACE(acs)



namespace {
    dbConnectionParametersBinder bindToConnectionParameters ;
}

void SQLDialectAppSpecific::init(SQLDialect &) {}

ThreadSafe SQLDialectFactory::_mutex ; 

using std::map ;
using std::string ;
using std::pair ;

map <string, SQLDialect*> SQLDialectFactory::_theDialect ;
map <string, SQLDialectFactory*> SQLDialectFactory::_theDialectFactory ;
map <pair<string, string>, SQLDialectFactory*> SQLDialectFactory::_dialectFactories ;
map <string, std::shared_ptr<Plugin> > SQLDialectFactory::_dialectPlugin ;
map <string, SQLDialectAppSpecific*> SQLDialectFactory::_dialectAppSpecific ;

string SQLDialectFactory::_curkey ; 


ACS_CLASS_DEFINE_DEBUG_LEVEL(SQLDialectFactory)


SQLDialectFactory::~SQLDialectFactory() noexcept
{
// PRQA S 4631 9
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

    try {
        unregisterFactory() ;
    }
    catch(std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
}

void SQLDialectFactory::initPlugin(const string &key, const string &so_path, const string &dialect) 
{
	ThreadSafe::Lock l(_mutex) ;

	_curkey = key ; 	/* This is passed to SPECIFIC dialect */

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "key: \"" << key << "\"  loading plugin (abs path): " << so_path) ;

	// initialize dialect shared object
	_dialectPlugin[key] = std::shared_ptr<Plugin>(new Plugin(so_path)) ; // PRQA S 3081

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "key: \"" << key << "\"  plugin loaded: " << File::getFileName(so_path)) ;

	_dialectPlugin[key]->open() ;	/* The empty constructor() is called. The current key is passed through _curkey */

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "key: \"" << key << "\"  plugin open: " << File::getFileName(so_path)) ;

	// the constructor of the plugin above (whose name is passed) should have initialized (calling registerFactory) _dialectFactories map

	map <pair<string,string>, SQLDialectFactory *>::iterator dialectsI = _dialectFactories.find(pair<string, string>(key, dialect)) ; 

	if (dialectsI != _dialectFactories.end()) {
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Assigning: " << dialectsI->second) ;

		_theDialectFactory[key] = dialectsI->second ; 
	}

	ACS_COND_THROW( ((dialectsI == _dialectFactories.end()) || !_theDialectFactory[key]), // if no factory found: error // PRQA S 3081
			WrongSQLDialect("Couldn't find SQL dialect \"" + dialect + "\" with key: \"" + key + "\"")) ;

	// Specifically init the dialect:
	if ( (_dialectAppSpecific.find(key) != _dialectAppSpecific.end()) && _dialectAppSpecific[key]) {
		_dialectAppSpecific[key]->init(_theDialectFactory[key]->dialect(key)) ;
	}
}

SQLDialect &SQLDialectFactory::init(const string &key, const string &so_path, const string &dialect)  // PRQA S 4020
{
// THIS IS THE ENTRY POINT -- THE INIT FUNCTION IS CALLED BY dbAppResSet::init

	ThreadSafe::Lock l(_mutex) ;
	bool found = false ; 
	
	map<string, SQLDialect*>::iterator theDialectI = _theDialect.find(key) ;

	if (theDialectI != _theDialect.end()) { // has a dialect initialized ?
		return *(theDialectI->second) ; // return it 
	}

	/* Look if any previous istance has the same dialect: */
	map <pair<string,string>, SQLDialectFactory *>::iterator dialectsI ; 
	found = false ; for (dialectsI = _dialectFactories.begin() ; dialectsI != _dialectFactories.end() ; dialectsI++)
	{
		if ( ( (dialectsI->first).second == dialect ) && ( _theDialectFactory.find((dialectsI->first).first) != _theDialectFactory.end() ) )
		{
			/* FOUND: A previous instance has the same dialect */
			const string prev_key = (dialectsI->first).first ; // The key of the previous instance with the same dialect */
	
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "key: \"" << key << "\"  has the same dialect: \"" << dialect << "\" of key: \"" << prev_key << "\"") ;
            				
			_theDialectFactory[key] = _theDialectFactory[prev_key] ;	/* set key in _theDialectFactory */
			
			(_theDialectFactory[key])->addNewInstance(key) ; 			/* set key in _dialect */

			// _dialectPlugin		won't be modified
			// _dialectFactories    won't be modified  
			// _dialectName			won't be modified   
			// _dialectAppSpecific	won't be modified
			// _appResInitialized	won't be modified
			// _dialectAppSpecific	won't be modified
			// _theDialect			will be modified below 
			
			found = true ; break	/* quit for */ ; 
		}
	}	/* for */


	if (!found)	{
        initPlugin(key, so_path, dialect) ;
	}

	// ask to the factory to build a dialect and store it.
	SQLDialect &ret = *(_theDialect[key] = &(_theDialectFactory[key]->dialect(key))) ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method finished") ;

	// return it
	return ret ;
}

SQLDialect &SQLDialectFactory::sqlDialect(const string &key) 
{		
	ThreadSafe::Lock l(_mutex) ;

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "key: \"" << key << "\"") ;
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "key: \"" << key << "\"  _theDialect: " // PRQA S 3380, 3385
        << (_theDialect.find(key) != _theDialect.end() ? _theDialect[key] : 0)) ;

	map<string, SQLDialect*>::iterator theDialectI = _theDialect.find(key) ;

	if (theDialectI != _theDialect.end()) { // has a dialect initialized ?
		return *(theDialectI->second) ; // return it 
	}

	// error: no dialect initialized to be returned
	ACS_THROW(WrongSQLDialect("SQL Dialect not yet initialized or was removed for key \"" + key +"\"")) ; // PRQA S 3081
}

const string &SQLDialectFactory::dialectName(const string& key) 
{
    return sqlDialect(key).dialectName() ;
}


dbConnectionParameters SQLDialectFactory::connectionParameters(const string &key, const string &parameters) // PRQA S 4020
{
	ThreadSafe::Lock l(_mutex) ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "key: \"" << key << "\"  parameters: \"" << parameters << "\"") ;

	map <string, SQLDialectFactory*>::iterator theDialectFactoryI = _theDialectFactory.find(key) ;
  
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "iterator pointing to \"" << key << "\"") ;
	ACS_COND_THROW( (theDialectFactoryI==_theDialectFactory.end()), // any factory ? // PRQA S 3081
		WrongSQLDialect("No SQL dialect initialized.")) ;
	// return the connection parameters associated with the factory
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Candidate dialect: 0x" << theDialectFactoryI->second) ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "key: \"" << key << "\"" // PRQA S 3380
            << " calling SQLDialectFactory->getConnectionParameters(" 
            << (parameters.empty() ? "parameters" : "" ) << ")") ;
	return (theDialectFactoryI->second)->getConnectionParameters(key, parameters) ;
}


SQLDialectFactory::SQLDialectFactory(const string &dialect_name) :
    _dialects(),
    _dialectName(),
    _key(_curkey)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_curkey: \"" << _curkey << "\"  dialect: \"" << dialect_name << "\"") ;
	
	_dialectName[ _key ] = dialect_name ;
	registerFactory( ) ;
}


SQLDialect &SQLDialectFactory::dialect(const string& key) /*const*/
{ 
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	ACS_COND_THROW( (_dialects.find(key) == _dialects.end()), // PRQA S 3081
			WrongSQLDialect("Couldn't find SQL dialect with key: \"" + key + "\"")) ;

	return *(_dialects[key]) ;
}

void SQLDialectFactory::registerNewInstance(const string& key, SQLDialect *d) 
{
    _dialects[key] = d ;
}

void SQLDialectFactory::registerFactory()
{
    Lock l(_mutex) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "key: \"" << _key << "\" registering dialect: \"" << _dialectName[_key] << "\" with add:" << this)  ;
	
	_dialectFactories[pair<string, string>(_key, _dialectName[_key])] = this ;	// EC:: probably useless
}

void SQLDialectFactory::unregisterKey(const string &key)
{
    Lock l(_mutex) ;
    map <string, SQLDialectFactory*>::iterator _theDialectFactoryI = _theDialectFactory.find(key) ;
    if (_theDialectFactoryI != _theDialectFactory.end()) {
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "unregistering _theDialectFactory: " << _theDialectFactoryI->second) ;
        _theDialectFactory.erase(_theDialectFactoryI) ;
    }
    map<string, SQLDialect* >::iterator _dialectsI = _dialects.find(key) ;
    if (_dialectsI != _dialects.end()) {
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "unregistering _dialects: " << _dialectsI->second) ;
        _dialects.erase(_dialectsI) ;
    }
   
    map<string, SQLDialect* >::iterator _theDialectI = _theDialect.find(key) ;
    if (_theDialectI != _theDialect.end()) {
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "unregistering _theDialect: " << _theDialectI->second) ;
        _theDialect.erase(_theDialectI) ;
    }
    
    
}


void SQLDialectFactory::unregisterFactory()
{
    Lock l(_mutex) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "key: \"" << _key << "\" unregistering dialect: \"" << _dialectName[_key] << "\"") ;
	map <pair<string,string>, SQLDialectFactory *>::iterator tbd = _dialectFactories.find(make_pair(_key, _dialectName[_key])) ;
    if ((tbd != _dialectFactories.end()) && (tbd->second == this)) {
 	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "erasing addr: " << this 
                << " for key: \"" << tbd->first.first << "\""
                << " dialect: " << tbd->first.second) ;
        unregisterKey(tbd->first.first) ;
        _dialectFactories.erase(tbd) ;
    }
}


void SQLDialectFactory::setDialectAppSpecific(const string &key, SQLDialectAppSpecific *das) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "key: \"" << key << "\"") ;

	_dialectAppSpecific[key] = das ;

	map<string, SQLDialect*>::iterator theDialectI = _theDialect.find(key) ;

	if ( (theDialectI != _theDialect.end()) && _dialectAppSpecific[key]) {
		_dialectAppSpecific[key]->init(* theDialectI->second) ;
	}
}



_ACS_END_NAMESPACE
