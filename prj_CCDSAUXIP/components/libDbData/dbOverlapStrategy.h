// PRQA S 1050 EOF
/*

	Copyright 1995-2017, Advanced Computer Systems , Inc.
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
	Revision 5.0  2013/06/13 14:53:05  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.2  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.1  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2005/01/03 21:26:12  marpas
	headers fixed
	
	Revision 1.7  2004/02/13 10:15:13  paoscu
	exCLASS_DECLARE_DEBUG_LEVEL is the last attribute.
	
	Revision 1.6  2003/07/11 13:31:01  marpas
	exException::clone implemented
	
	Revision 1.5  2003/04/30 14:06:05  paoscu
	using namespace std removed.
	
	Revision 1.4  2003/04/17 14:04:30  marpas
	GCC3.2
	
	Revision 1.3  2003/04/17 13:58:42  marpas
	GCC3.2
	
	Revision 1.2  2003/02/13 13:53:36  marpas
	computeOverlap method overloaded to take into account a clamping time range.
	
	Revision 1.1  2003/01/13 11:05:00  marpas
	dbOverlapStrategy class added
	
		

*/


#ifndef _dbOverlapStrategy_H_
#define _dbOverlapStrategy_H_ 

//
//
//
// class dbOverlapStrategy
//		
//
//

#include <acs_c++config.hpp>
#include <dbGeoTimeRange.h>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)

class dbGeneralQuery ;

class dbOverlapStrategy // PRQA S 2108, 2109
{
public:
	/**
	 * This exception is thrown by computeOverlap() if the passed query is not of the correct type.
	 */
	/*! class WrongQueryTypeException declaration */
	exDECLARE_EXCEPTION(WrongQueryTypeException,exException) ; // PRQA S 2131, 2502

public:

	explicit dbOverlapStrategy(const dbGeneralQuery &);
	virtual ~dbOverlapStrategy();

	virtual std::vector< std::pair <int, dbGeoTimeRange> >& computeOverlap(std::vector< std::pair <int, dbGeoTimeRange> >&) = 0 ; 
	virtual std::vector< std::pair <int, dbGeoTimeRange> >& computeOverlap(std::vector< std::pair <int, dbGeoTimeRange> >&, const dbGeoTimeRange &clamp) = 0 ; 
private:
	//declared but not implemented 
	dbOverlapStrategy() ; 
	dbOverlapStrategy(const dbOverlapStrategy &) ; 
	dbOverlapStrategy &operator=(const dbOverlapStrategy &) ; 

protected:		
	const dbGeneralQuery &_query ; // PRQA S 2101

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbOverlapStrategy)  // PRQA S 2101
} ; 


_ACS_END_NAMESPACE

#endif
