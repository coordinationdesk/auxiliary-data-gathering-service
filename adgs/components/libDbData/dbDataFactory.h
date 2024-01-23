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
	Revision 5.0  2013/06/13 14:53:04  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.5  2013/02/28 14:06:45  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.4  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.3  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.1  2009/05/14 11:49:16  marpas
	some rules fixing
	comments makeup
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/01/03 21:26:12  marpas
	headers fixed
	
	Revision 1.1  2003/03/05 10:03:35  marpas
	data factory added
	
		

*/

#ifndef _dbDataFactory_H_
#define _dbDataFactory_H_ 

#include <acs_c++config.hpp>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbPersistent;
class dbGeneralQuery; // PRQA S 1703

class dbDataFactory
{
public:
	dbDataFactory();
	~dbDataFactory();

	static dbPersistent* buildDbApplicationStatus(const std::string &);
	static dbGeneralQuery* buildDbApplicationStatusQuery(const std::string &);

private:
	// declared but not implemented to prevent their use 
	dbDataFactory(const dbDataFactory &);
	dbDataFactory & operator=(const dbDataFactory &);

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbDataFactory) ;

};

_ACS_END_NAMESPACE

#endif // _dbDataFactory_H_

