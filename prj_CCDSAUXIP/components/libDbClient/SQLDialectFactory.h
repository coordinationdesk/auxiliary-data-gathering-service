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
	Revision 5.2  2013/06/13 18:10:53  marpas
	libException 5.x standards adoption finished
	interface rationalization
	removing useless code
	coding best practices added
	some performances improvements.
	test fixed
	
	Revision 5.1  2013/06/11 17:02:59  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:55  marpas
	adopting libException 5.x standards
	
	Revision 4.9  2013/05/14 18:06:25  marpas
	interface rationalization.
	behaviour now try to get rid of memory during exit
	
	Revision 4.8  2013/04/27 14:44:54  marpas
	coding best practices: no exception from dtors
	
	Revision 4.7  2013/04/12 12:12:18  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 4.6  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.5  2012/12/11 17:30:10  marpas
	qa rules
	
	Revision 4.4  2012/11/19 13:30:19  marpas
	HEAD REALIGNEMENT
	
	Revision 4.2  2012/02/22 11:29:29  enrcar
	EC:: If the same database dialect is intanced more than once, the method addNewInstance of an already existent Plugin will be called. Therefore, any plugin is allowed to contain several instances of the same SQLDialect class (a map is used).
	
	Revision 4.1  2012/02/17 13:55:12  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:11  marpas
	new statistics adopted
	
	Revision 3.1  2010/03/19 11:27:36  marpas
	class SQLDialectAppSpecific added to manage specific dialect issues for applications
	
	Revision 3.0  2009/10/05 14:47:48  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.5  2009/05/22 15:13:50  marpas
	some rules fixed - need more work
	
	Revision 2.4  2009/03/04 17:50:04  marpas
	CODECHECK parsing improved
	
	Revision 2.3  2009/03/02 14:03:29  marpas
	CODECHECK parsing, no regressions
	
	Revision 2.2  2008/05/22 14:37:54  ivafam
	EC:: Modified after importing into Rational Rose
	
	Revision 2.1  2006/03/09 16:36:01  marpas
	new dbColumn implementation - column type moved into dedicated classes
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2006/01/11 16:14:35  marpas
	dialect initialization forced at first use
	
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

#ifndef _SQLDialectFactory_
#define _SQLDialectFactory_

#include <acs_c++config.hpp>
#include <dbConnectionParameters.h>
#include <dbException.h>
#include <string>
#include <map>

_ACS_BEGIN_NAMESPACE(acs)

class SQLDialect ;
class Plugin ;
/**
 * \brief The factory to build all tyhe supported dialects
 */

class SQLDialectAppSpecific {
public:
	SQLDialectAppSpecific() = default ;
	SQLDialectAppSpecific(const SQLDialectAppSpecific &) = default ;
	SQLDialectAppSpecific & operator=(const SQLDialectAppSpecific &) = default ;
	SQLDialectAppSpecific(SQLDialectAppSpecific &&) = default ;
	SQLDialectAppSpecific & operator=(SQLDialectAppSpecific &&) = default ;
	virtual ~SQLDialectAppSpecific() = default ;
	virtual void init(SQLDialect &) ;
} ;


class SQLDialectFactory // PRQA S 2109
{
public:
	/*! class WrongSQLDialect declaration */
	
    exDECLARE_EXCEPTION(WrongSQLDialect,dbException) ;  // PRQA S 2131, 2153, 2502
    exDECLARE_EXCEPTION(KeyNotFoundError,dbException) ;  // PRQA S 2131, 2153, 2502

public:
    
    SQLDialectFactory() = delete ;
	
    SQLDialectFactory(const SQLDialectFactory &) = default ;
	SQLDialectFactory &operator=(const SQLDialectFactory &) = default ;
	virtual ~SQLDialectFactory() noexcept ; 

    
 	static SQLDialect &init(const std::string& key, const std::string &so_path,const std::string &dialect) ;

	static SQLDialect &sqlDialect(const std::string& key) ;
 
    static const std::string &dialectName(const std::string& key) ;
	
	static dbConnectionParameters connectionParameters(const std::string &key, const std::string &parameters) ;

	static void setDialectAppSpecific(const std::string &key, SQLDialectAppSpecific *) ;

protected:
    
	explicit SQLDialectFactory(const std::string &dialect_name) ;


    
	SQLDialect &dialect(const std::string &key) /* const*/ ;

	virtual dbConnectionParameters getConnectionParameters(const std::string &key, const std::string &parameters) = 0;
	
	virtual void addNewInstance(const std::string &key) =0;

	void registerNewInstance(const std::string& key, SQLDialect *) ;
	
private:
	void registerFactory() ;
	void unregisterFactory() ;
	void unregisterKey(const std::string &) ;
    static void initPlugin(const std::string &key, const std::string &so_path, const std::string &dialect) ;
    
	// declared but not implemented

public:	
	static std::string _curkey ;    // PRQA S 2100

private:
    
	std::map<std::string, SQLDialect* > _dialects ;
	
	std::map<std::string, std::string> _dialectName ;
    std::string _key ;
    
	static std::map <std::string, std::shared_ptr<Plugin> > _dialectPlugin ;
    
	static std::map <std::string, SQLDialectFactory*> _theDialectFactory ;
    
	static std::map <std::string, SQLDialect*> _theDialect ;
    
	static std::map <std::pair<std::string,std::string>, SQLDialectFactory *> _dialectFactories ;
    
	static std::map<std::string, SQLDialectAppSpecific*> _dialectAppSpecific ; 
	
	static ThreadSafe _mutex ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(SQLDialectFactory) ;
} ;

_ACS_END_NAMESPACE

#endif
