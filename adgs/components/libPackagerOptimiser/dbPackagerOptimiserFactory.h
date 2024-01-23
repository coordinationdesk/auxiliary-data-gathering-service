// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.


	$Prod: A.C.S. Packager Optimiser Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2014/03/10 15:00:33  francesco.avanzi
	better code practices applied
	
	Revision 2.2  2013/07/22 18:27:03  marpas
	aligning to libDbData new interface
	
	Revision 2.1  2009/03/24 14:32:09  enrcar
	EC:: c++rules
	
	Revision 2.0  2006/02/28 10:05:36  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2004/09/14 16:28:37  marpas
	object extended to limit query number
	


*/

#ifndef _dbPackagerOptimiserFactory_H_
#define _dbPackagerOptimiserFactory_H_ 

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)


class dbPersistent;
class dbGeneralQuery;

class dbPackagerOptimiserFactory
{
public:
	dbPackagerOptimiserFactory();
	~dbPackagerOptimiserFactory() throw() ;

	static dbPersistent* buildDbDistributionItemExtended(const std::string &dbKey);
	static dbGeneralQuery* buildDbDistributionItemExtendedQuery(const std::string &dbKey);


	dbPackagerOptimiserFactory(const dbPackagerOptimiserFactory &);
	dbPackagerOptimiserFactory &operator=(const dbPackagerOptimiserFactory &);

private:


	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbPackagerOptimiserFactory);

};


_ACS_END_NAMESPACE

#endif // _dbPackagerOptimiserFactory_H_

