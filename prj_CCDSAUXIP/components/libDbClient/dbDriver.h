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
	Revision 5.1  2013/06/11 17:02:59  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:58  marpas
	adopting libException 5.x standards
	
	Revision 4.9  2013/06/03 12:36:26  marpas
	coding best practices applied
	qa rules
	
	Revision 4.8  2013/05/30 11:21:58  marpas
	getting rid of useless code
	
	Revision 4.7  2013/05/30 11:18:41  marpas
	dbDriver is now a Singleton (ParamConstructibleSingletonMap)
	
	Revision 4.6  2013/05/29 18:14:16  marpas
	the so_loader is now an auto_ptr to allow its dtor to be called in the right order
	NB the driver MUST be destoryed BEFORE the plugin
	
	Revision 4.5  2013/05/29 16:48:56  marpas
	private data order rearrangement, to avoid dtor of dbDriver to be called after plugin unload
	
	Revision 4.4  2013/05/14 18:13:59  marpas
	dbQuery.h
	
	Revision 4.3  2013/01/25 11:47:57  marpas
	using pattern::Singleton from libException
	
	Revision 4.2  2012/12/11 17:30:11  marpas
	qa rules
	
	Revision 4.1  2012/02/17 13:55:14  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:14  marpas
	new statistics adopted
	
	Revision 1.1  2010/09/11 17:43:00  marpas
	renamed from dbxDriverWrapper
	adopt libDbExpress 3.2 interface
	
	Revision 3.0  2009/10/05 14:47:54  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.1  2006/06/28 16:57:53  marpas
	getPlugin() const method added
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2005/06/09 08:42:30  enrcar
	*** empty log message ***
	
	Revision 1.5  2005/05/03 10:23:48  enrcar
	connectionString removed
	unused constructor removed
	
	Revision 1.4  2005/04/21 17:11:15  marpas
	work in progress
	
	Revision 1.3  2005/04/19 15:06:29  enrcar
	*** empty log message ***
	
	Revision 1.2  2005/03/31 09:18:07  enrcar
	*** empty log message ***
	
	Revision 1.1.1.1  2005/03/07 14:00:35  marpas
	Importing dbExpressWrapper
	

*/

#ifndef _dbDriver_H_
#define _dbDriver_H_

#include <acs_c++config.hpp>
#include <dbException.h>
#include <DbExpress.h>
#include <Singleton.hpp>
#include <Plugin.h>
#include <map>
#include <memory>

_ACS_BEGIN_NAMESPACE(acs)

class dbDriver: // PRQA S 2109, 2153
    public pattern::ParamConstructibleSingletonMap<dbDriver, std::string>,
    public ThreadSafe
{
    friend dbDriver* pattern::ParamConstructibleSingletonMap<dbDriver, std::string>::instance(const std::string &) ; // PRQA S 2107 
public:

	/*! class LoadException declaration */
	exDECLARE_EXCEPTION(LoadException,dbException) ; // PRQA S 2131, 2153, 2502

	dbDriver(const dbDriver &) = delete ;
	dbDriver& operator=(const dbDriver &) = delete ;
	~dbDriver() override = default  ;

	virtual const dbSQLDriver & getDbDriver() const ;
	
	dbSQLConnection getSqlConnection() const ;

    const Plugin *getPlugin() const ; 

private:
	explicit dbDriver(const std::string &key) ;	

	
	void init(const std::string &dbx_so_path, const std::string &vendorLib, const std::string &function) ;
	
private: // NOSONAR - for clarity
	dbSQLDriver _sqlDriver = {} ;
	getSQLDriverPrototype _getDriver ;
	std::string _key ;
    // this must be the last, else when the plugin is destroyed
    // hence unloaded, the _sqlDriver dtor will try to access to plugin
    // internal data ... 
	std::unique_ptr<Plugin> _so_loader = { nullptr } ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbDriver) ;
} ;


_ACS_END_NAMESPACE


#endif // _dbDriver_H_
