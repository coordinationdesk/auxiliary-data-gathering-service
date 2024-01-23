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

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2013/07/22 17:46:10  marpas
	enforcing use of dbkey string - deprecting older methods
	
	Revision 5.0  2013/06/13 14:53:05  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.8  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.7  2013/02/26 18:20:04  marpas
	qa warning
	
	Revision 2.6  2013/02/26 18:04:37  marpas
	coding best practices applied
	interface rationalization
	
	Revision 2.5  2013/01/25 11:39:33  marpas
	using pattern::Singleton from libException
	
	Revision 2.4  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.3  2012/02/21 10:50:29  marpas
	refactoring
	
	Revision 2.2  2009/05/14 11:49:16  marpas
	some rules fixing
	comments makeup
	
	Revision 2.1  2006/05/11 20:41:33  marpas
	constant strings changed from std::string to const char * to avoid std::string repository problem with shared objects
	
	Revision 2.0  2006/02/28 08:47:38  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.11  2005/11/03 09:54:19  marpas
	dbOperation01Factory class added and used
	
	Revision 1.10  2005/09/22 12:56:59  marpas
	missing includes added
	
	Revision 1.9  2005/06/17 14:14:25  marpas
	std namespace use fixed
	
	Revision 1.8  2005/01/03 21:26:12  marpas
	headers fixed
	
	Revision 1.7  2004/09/15 14:03:36  marpas
	thread safety, debug messages improved
	
	Revision 1.6  2004/09/01 13:14:55  marpas
	*** empty log message ***
	
	Revision 1.5  2003/07/11 13:31:01  marpas
	exException::clone implemented
	
	Revision 1.4  2003/04/30 14:06:05  paoscu
	using namespace std removed.
	
	Revision 1.3  2003/03/14 18:04:31  paoscu
	Moved dbDataFactory instance
	
	Revision 1.2  2003/01/27 17:53:26  paoscu
	DEstructors called bt atexit
	
	Revision 1.1.1.1  2002/11/29 17:19:30  marpas
	Import libDbData
		

*/

#ifndef _dbPersistentFactoryChain_H_
#define _dbPersistentFactoryChain_H_ 

#include <acs_c++config.hpp>
#include <Singleton.hpp>
#include<dbPersistent.h>
#include<dbDataFactory.h>
#include<exException.h>
#include<map>
#include<deque>
#include<string>
#include<vector>


_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;


class dbOperation01Factory ;

//class dbPersistentFactoryChain;

class dbPersistentFactoryChain : public Singleton<dbPersistentFactoryChain > // PRQA S 2109, 2153
{
friend dbPersistentFactoryChain* Singleton< dbPersistentFactoryChain >::instance(); // PRQA S 2107
public:
	/*! class KeyNotFound declaration */
	exDECLARE_EXCEPTION(KeyNotFound,exException) ; // PRQA S 2131, 2502


	void registerFactory(std::string const &, dbPersistent* (*)(const std::string &) );
	void registerObjType(std::string const &, const std::string & );
	void unregisterFactory(std::string const &, dbPersistent* (*)(const std::string &) );
	void unregisterObjType(std::string const &, const std::string & );
	dbPersistent *newObject(std::string const &, const std::string &dbKey);

	// returns the object type Unknown if it is unknown
	std::string getObjType(std::string const &) ;
	void getAllObjType(std::vector<std::string> &) ;
	std::string unknownObjType() const ;

	virtual ~dbPersistentFactoryChain() throw() ;

protected:
	dbPersistentFactoryChain();

private:
	// declared but not implemented to prevent their use
	dbPersistentFactoryChain(const dbPersistentFactoryChain &);
	dbPersistentFactoryChain &operator=(const dbPersistentFactoryChain &);

private:
	std::map<std::string, dbPersistent* (*)(const std::string &)> _map;
	std::map<std::string, std::string> _objTypes ;
	std::map<std::string, std::deque<dbPersistent* (*)(const std::string &)> > _oldMap;
	std::map<std::string, std::deque<std::string> > _oldObjTypes;
    Mutex _mutex ;
	static const char * _unknown ;

	static dbOperation01Factory *_theDbOperationFactory ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbPersistentFactoryChain) ;
};


void registerDbPersistentFactoryMethod(std::string const & , dbPersistent* (*)(const std::string &) );
void unregisterDbPersistentFactoryMethod(std::string const & , dbPersistent* (*)(const std::string &) );
void registerDbPersistentObjType(std::string const & , const std::string & );
void unregisterDbPersistentObjType(std::string const & , const std::string & );


_ACS_END_NAMESPACE

#endif // _dbPersistentFactoryChain_H_
