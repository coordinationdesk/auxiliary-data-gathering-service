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

	$Prod: A.C.S. Generic FileAction Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2013/09/19 15:40:45  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.0  2013/07/23 17:07:22  marpas
	adopting libException 5.x interface
	coding best practices applied
	compilation warnings fixed
	qa warning fixed
	
	Revision 1.4  2013/03/28 14:30:52  marpas
	coding best practices applied
	ConnectionPool and connections usage enforced
	dbQueries statistics updated
	qa rules
	
	Revision 1.3  2013/01/22 18:14:55  marpas
	qa rules work in progress
	
	Revision 1.2  2012/02/13 16:49:03  marpas
	refactoring
	
	Revision 1.1.1.1  2008/10/09 17:15:51  ivafam
	First Import
	
	
*/


#ifndef _PlainStrategy_H_
#define _PlainStrategy_H_ 

#include <SMPackingStrategy.h>
#include <exException.h>
#include <string>

namespace acs {

class XMLIstream ;
class PlainStrategyScopeGuard ;
class dbPersistent ;

/**
 * Class to deal with Plain Storage Strategy.
 */
class PlainStrategy : public SMPackingStrategy
{
public:
	
	explicit PlainStrategy(dbPersistent &obj, dbConnectionPool &) ;
	virtual ~PlainStrategy() throw() ;

	
	virtual void pack(const std::string &input, XMLIstream &, std::string &output) ;
	
	virtual void unpack(const std::string &input, XMLIstream &, std::string &output)  ;
				
private:
	// declared but not implemented to prevent their use
	PlainStrategy() ;
	PlainStrategy(const PlainStrategy &) ;
	PlainStrategy &operator=(const PlainStrategy &) ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PlainStrategy) ;
} ; 

} // namespace


#endif // _PlainStrategy_H_
