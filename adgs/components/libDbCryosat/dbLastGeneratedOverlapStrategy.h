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
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.3  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.1  2016/04/14 10:50:21  marpas
	coding best practices applicationin progress
	somw qa warnings removed
	
	Revision 5.0  2015/02/17 11:25:26  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:31:00  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:28  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.1  2012/02/14 12:52:49  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.7  2005/02/24 11:57:02  marpas
	compute overlap algorithm improved:
	1) it now deals with multiple zero-length files
	2) clamping timeline can enlarge it
	
	Revision 1.6  2005/02/23 17:53:47  fracar
	BUG 736 fixed
	
	Revision 1.5  2004/12/30 22:48:00  paoscu
	headers fixed
	
	Revision 1.4  2003/05/20 14:20:24  paoscu
	std namespace.
	
	Revision 1.3  2003/02/13 16:16:25  paoscu
	Overloaded computeOverlap implemented
	
	Revision 1.2  2003/01/14 14:05:04  paoscu
	Bugs fixed, debug prints added.
	
	Revision 1.1  2003/01/10 20:17:13  paoscu
	dbLastGeneratedOverlapStrategy class added
	
		

*/


#ifndef _dbLastGeneratedOverlapStrategy_H_
#define _dbLastGeneratedOverlapStrategy_H_ 

//
//
//
// class dbLastGeneratedOverlapStrategy
//		
//
//

#include <dbOverlapStrategy.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbGeoTimeRange ;

class dbLastGeneratedOverlapStrategy : public dbOverlapStrategy // PRQA S 2109
{
public:

	explicit dbLastGeneratedOverlapStrategy(const dbGeneralQuery &);
	virtual ~dbLastGeneratedOverlapStrategy() throw() ;

	virtual std::vector< std::pair <int, dbGeoTimeRange> >& computeOverlap(std::vector< std::pair <int, dbGeoTimeRange> >&) ; 
	virtual std::vector< std::pair <int, dbGeoTimeRange> >& computeOverlap(std::vector< std::pair <int, dbGeoTimeRange> >&, const dbGeoTimeRange &clamp ) ; 

private:
	dbLastGeneratedOverlapStrategy() ;  // not implemented
	dbLastGeneratedOverlapStrategy &operator=(const dbLastGeneratedOverlapStrategy &) ; // not implemented
	dbLastGeneratedOverlapStrategy(const dbLastGeneratedOverlapStrategy &) ;  // not implemented

	bool intersectInterval(const dbGeoTimeRange &a, const dbGeoTimeRange &b) ;
	bool coverInterval(const dbGeoTimeRange &cover, const dbGeoTimeRange &tobecovered) ;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbLastGeneratedOverlapStrategy)

} ; 


_ACS_END_NAMESPACE

#endif
