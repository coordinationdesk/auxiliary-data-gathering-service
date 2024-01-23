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

	$Prod: A.C.S. Database Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2017/09/25 15:45:13  lucio.pulvirenti
	S2PDGS-1843: Passed optional boolean parameter to be passed to objects CTOR
	
	Revision 5.2  2016/04/28 09:20:44  marpas
	useless members removed and interface simplified
	
	Revision 5.1  2013/07/22 18:01:03  marpas
	aligning to new libDbData interface
	
	Revision 5.0  2013/06/11 14:16:32  marpas
	adopting libException 5.x standards
	
	Revision 2.8  2013/03/14 15:27:23  marpas
	connection pool concept use enforced
	interface rationalization
	
	Revision 2.7  2013/03/12 17:44:57  marpas
	dbConnectionPool used in file actions
	
	Revision 2.6  2013/02/06 16:09:58  marpas
	coding best practices applied
	some qa rule fixed
	
	Revision 2.5  2013/01/24 19:22:39  marpas
	connection pool management improved
	
	Revision 2.4  2013/01/23 17:23:09  marpas
	introducing dbConnectionPool for factories
	coding best practices
	interface rationalization
	qa rules
	
	Revision 2.3  2012/12/11 17:30:29  marpas
	qa rules
	
	Revision 2.2  2012/02/13 16:53:02  marpas
	refactoring
	
	Revision 2.1  2009/08/03 18:26:10  marpas
	faster and less connections to db.
	
	Revision 2.0  2006/02/28 09:09:04  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2005/07/21 10:50:16  marpas
	file actions for dbConfiguration implemented
	
	Revision 1.5  2005/07/21 10:24:43  marpas
	dbConfigurationSpace and its query imported from libDbCryosat
	
	Revision 1.4  2005/03/17 12:42:56  marpas
	headers fixed
	
	Revision 1.3  2003/04/30 17:15:36  marpas
	using namespace acs removed
	
	Revision 1.2  2003/04/30 14:01:54  marpas
	using namespace std no longer fount into acs includes
	
	Revision 1.1.1.1  2003/01/14 15:34:55  marpas
	libDbConfiguration Import
		

*/


#ifndef _dbResourceSpaceFactory_H_
#define _dbResourceSpaceFactory_H_ 


#include <acs_c++config.hpp>
#include <ResourceSpaceFactory.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbPersistent;
class dbGeneralQuery;
class dbConnectionPool ;
//
//
//
// class dbResourceSpaceFactory
//
//
//
class dbResourceSpaceFactory : public ResourceSpaceFactory // PRQA S 2109, 2153
{
public:

	explicit dbResourceSpaceFactory(dbConnectionPool &);
	virtual ~dbResourceSpaceFactory() throw() ;

	virtual ResourceSpace *createItem(const std::string &space, const std::string &version, bool absPath = false) ;
		
	static dbPersistent* buildDbConfigurationSpace(const std::string &dbKey);
	static dbGeneralQuery* buildDbConfigurationSpaceQuery(const std::string &dbKey);

    virtual bool hasOptionalConf() const ; 
 
private:
	// declared but not implemented to prevent their use
	dbResourceSpaceFactory() ;
	dbResourceSpaceFactory(const dbResourceSpaceFactory &) ;
	dbResourceSpaceFactory &operator=(const dbResourceSpaceFactory &) ;
		
private:
    dbConnectionPool &_connectionPool ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbResourceSpaceFactory) ;
	
} ; 

_ACS_END_NAMESPACE


#endif // _dbResourceSpaceFactory_H_
