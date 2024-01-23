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
	Revision 5.2  2018/09/21 14:10:51  marpas
	The getSMPluginPath() signature has changed and it is no longer inline.
	Now it returns the configured plugins file path and name with the followng behaviour:
	if path is absolute returns it.
	if path is relative, it will be relative to the acs::base::defaultSMPluginsPath()
	if it is empty the full pathname will be computed according:
	<acs::base::defaultSMPluginsPath()>/<acs::base::defaultSMPluginsFileName()>
	
	Obsolescent exception specifier has been removed according to new c++20 standard
	
	Revision 5.1  2017/03/22 11:26:49  marfav
	Removing boost shared_ptr from global variable due to crash in global dtors
	
	Revision 5.0  2013/06/06 18:52:57  marpas
	adopting libException 5.x standards
	
	Revision 4.10  2013/05/14 18:07:39  marpas
	useless classes support removed. Interface rationalization.
	ATTENTION: ctor signature has changed meaning.
	
	Revision 4.9  2013/02/26 17:12:24  marpas
	coding best practice appplied
	interface rationalization
	
	Revision 4.8  2012/12/11 17:30:11  marpas
	qa rules
	
	Revision 4.7  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.4  2012/03/01 18:19:15  marpas
	refactoring in progress
	
	Revision 4.3  2012/03/01 15:23:01  marpas
	introducing new db.conf path policy
	
	Revision 4.2  2012/02/17 13:55:13  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.1  2011/12/23 09:46:26  marpas
	thread safety improved
	
	Revision 4.0  2011/10/18 12:31:12  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/05 14:47:49  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.2  2009/06/22 12:36:35  marpas
	robustness improved
	got rid of useless ifdef and warning
	useless code removed
	test compilation fixed
	
	Revision 2.1  2009/05/22 15:13:50  marpas
	some rules fixed - need more work
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2006/01/17 11:13:16  marpas
	getSMPluginPath method added, it relies on <Dialect>.SMPluginConfFile to load the full path of the plugin conf file
	
	Revision 1.3  2005/10/25 12:34:15  marpas
	getSQLDialect method implemented (returns the dialect string)
	
	Revision 1.2  2005/04/18 19:42:27  marpas
	work in progress
	
	Revision 1.1.1.1  2005/03/07 14:00:35  marpas
	Importing dbExpressWrapper
	
	Revision 1.11  2004/02/04 10:17:16  paoscu
	Better doxygen comments.
	
	Revision 1.10  2003/08/01 16:12:47  marpas
	is possible to ignore the MASTER/SLAVE status and perform the connection
	on the first database available iff any
	
	Revision 1.9  2003/04/30 13:36:03  marpas
	using namespace std partially removed because informix headers
	
	Revision 1.8  2003/04/16 08:44:44  marpas
	GCC3.2
	
	Revision 1.7  2003/03/14 20:21:52  paoscu
	new policy to establish who is the arbiter.
	
	Revision 1.6  2003/03/14 19:16:30  paoscu
	Master/Slave policy implemented
	
	Revision 1.5  2003/02/26 17:28:42  paoscu
	dbAppResSet* instance now accepts a configuration std::string.
	
	Revision 1.4  2002/11/12 13:31:43  marpas
	#include <rsResourceSet.h> moved from dbAppResSet.h to dbAppResSet.C
	
	Revision 1.3  2002/11/08 15:39:20  marpas
	Singletons are now destroyed registering a private static function
	with atexit
	
	Revision 1.2  2002/10/04 16:41:16  marpas
	Serial increment for database is now left to the dbAppResSet class instance.
	
	Revision 1.1.1.1  2002/10/02 09:58:02  marpas
	Import libDbClient
	

*/


#ifndef _dbAppResSet_H_
#define _dbAppResSet_H_ 

#include <acs_c++config.hpp>
#include <dbConnectionParameters.h>
#include <exException.h>
#include <string>
#include <map>

//
//
// class dbAppResSet
//
//


_ACS_BEGIN_NAMESPACE(acs)

/**
 * Class dbAppResSet reads database configuration file
 *
 * It can be used through its singleton implementation
 */
class dbAppResSet   // PRQA S 2109
{
public:

	dbAppResSet(const std::string& key, const std::string& dbconf);		
	~dbAppResSet() ;

	const std::string &getSQLDialectName() const { return _sqlDialectName ; }
	std::string getSMPluginPath() const ;
	const std::string &getDbConfFile() const     { return _confDatabaseName ; }
	const std::string &getConfDatabaseName() const { return _dbConfFile ; }
	const std::string &getKey() const            { return _key ; }
    const dbConnectionParameters &connectionParameters() const { return _connectionParameters ; }

    /*- \brief search a candidate conf file  
     * newkey can be changed and is the one to search into the conf file (or "" for default)
     * if key is the empty string, do not touch the key
     */
    static std::string candidate_conf_file(const std::string &key, const std::string& conf, std::string &newkey) ;
    static std::string setDefaultDbConfName(const std::string &	) ;
    static std::string getDefaultDbConfName() ;

private:
	// declared but not implemented
	dbAppResSet(const dbAppResSet &) ; 	// Copy Constructor
	dbAppResSet &operator=(const dbAppResSet &) ; 
	dbAppResSet() ; 	
	
	void init() ;
	
private:
	std::string _confDatabaseName ;
	std::string	_sqlDialectName ; 
	std::string	_smPluginConfFile ; 
	std::string	_dbConfFile;
    std::string _key ; 
    std::string _conf ; 
    dbConnectionParameters _connectionParameters ;
    
    static std::string _defaultDbConf ; // default to base::defaultDbConfFileName

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbAppResSet) ;

} ; 

std::ostream &operator<<(std::ostream &, const dbAppResSet&) ;  
acs::exostream &operator<<(acs::exostream &, const dbAppResSet&) ;  // PRQA S 2072



class dbAppResSetFactory 
{
public:
	static dbAppResSet* instance(const std::string& conf, const std::string &key="") ;

	~dbAppResSetFactory() ; 
    
protected:
	dbAppResSetFactory() ; 
	
private:
	dbAppResSetFactory &operator=(const dbAppResSetFactory &) ; 	// declared but not implemented
	dbAppResSetFactory (const dbAppResSetFactory &) ; 				// declared but not implemented
	
	static ThreadSafe _mutex ;
	static std::map<std::string, dbAppResSet* > _instances ; 
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbAppResSetFactory) ;
} ; 

_ACS_END_NAMESPACE

#endif // _dbAppResSet_H_

