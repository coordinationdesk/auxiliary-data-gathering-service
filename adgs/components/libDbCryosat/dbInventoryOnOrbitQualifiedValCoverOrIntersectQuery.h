// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.1  2016/10/06 17:01:58  marpas
	implementing VEN-2163 on HEAD - ready to be reported on venus branch - test to be fixed
	
	
*/

#ifndef _dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery_H_
#define _dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery_H_

#include <dbInventoryOnOrbitValCoverOrIntersectQuery.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery : // PRQA S 2153
	public dbInventoryOnOrbitValCoverOrIntersectQuery
{
	friend class dbCryosatFactory ; // PRQA S 2107
public:

	dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery(dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery const &) ;
	dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery &operator=(dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery const &) ;
	virtual ~dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery() throw() ;
	
	virtual unsigned long doQuery(dbConnection &, bool incremental=false, bool enabled = true) ;

protected:
	dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery(bool latest, bool intersect, const std::string &dbKey) ;
private:
	dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery() ; // declared but not implmented
};


_ACS_END_NAMESPACE

#endif // _dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery_H_
