// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA_AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

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
	
	Revision 2.7  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.6  2013/01/25 11:39:33  marpas
	using pattern::Singleton from libException
	
	Revision 2.5  2013/01/22 17:37:08  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.4  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.2  2012/02/21 10:50:29  marpas
	refactoring
	
	Revision 2.1  2009/05/14 11:49:16  marpas
	some rules fixing
	comments makeup
	
	Revision 2.0  2006/02/28 08:47:38  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2005/01/03 21:26:12  marpas
	headers fixed
	
	Revision 1.5  2004/09/15 14:03:36  marpas
	thread safety, debug messages improved
	
	Revision 1.4  2003/07/11 13:31:01  marpas
	exException::clone implemented
	
	Revision 1.3  2003/04/30 14:06:05  paoscu
	using namespace std removed.
	
	Revision 1.2  2003/01/27 17:53:26  paoscu
	DEstructors called bt atexit
	
	Revision 1.1.1.1  2002/11/29 17:19:30  marpas
	Import libDbData
		

*/

#ifndef _dbQueryFactoryChain_H_
#define _dbQueryFactoryChain_H_ 

#include <acs_c++config.hpp>
#include <Singleton.hpp>
#include <dbGeneralQuery.h>
#include <exException.h>
#include <map>
#include <deque>


_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;


//class dbQueryFactoryChain;

class dbQueryFactoryChain : public Singleton<dbQueryFactoryChain > // PRQA S 2109, 2153
{
	friend dbQueryFactoryChain* Singleton< dbQueryFactoryChain >::instance(); // PRQA S 2107
public:
	/*! class KeyNotFound declaration */
	exDECLARE_EXCEPTION(KeyNotFound,exException) ; // PRQA S 2131, 2502

	dbQueryFactoryChain(const dbQueryFactoryChain &) = delete ;
	dbQueryFactoryChain &operator=(const dbQueryFactoryChain &) = delete ;
	dbQueryFactoryChain(dbQueryFactoryChain &&) = delete ;
	dbQueryFactoryChain &operator=(dbQueryFactoryChain &&) = delete ;

	virtual ~dbQueryFactoryChain() noexcept ;	

	void registerFactory(std::string const &, dbGeneralQuery* (*)(const std::string &) );
	void unregisterFactory(std::string const &, dbGeneralQuery* (*)(const std::string &) );
	dbGeneralQuery *newObject(std::string const &, const std::string &dbKey);

protected:
	dbQueryFactoryChain();

private:
private:
	std::map<std::string, dbGeneralQuery* (*)(const std::string &)> _map;
	std::map<std::string, std::deque<dbGeneralQuery* (*)(const std::string &)> > _oldMap;
    Mutex   mutex_ ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbQueryFactoryChain) ;


};


void registerDbQueryFactoryMethod(std::string const & , dbGeneralQuery* (*)(const std::string &) );
void unregisterDbQueryFactoryMethod(std::string const & , dbGeneralQuery* (*)(const std::string &) );


_ACS_END_NAMESPACE


#endif // _dbQueryFactoryChain_H_
