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

	$Prod: A.C.S. Storage Manager Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/18 14:40:02  marpas
	adoption of libException 5.x standards
	
	Revision 2.4  2013/03/28 14:02:23  marpas
	coding best practices applied
	dbConnectionPool concept forced
	qa warnings
	
	Revision 2.3  2013/02/25 17:00:18  marpas
	coding best practices applied, useless compilation unti removed
	
	Revision 2.2  2012/02/13 17:08:58  marpas
	refactoring in proegress
	
	Revision 2.1  2008/11/11 18:13:30  ivafam
	ScopeGard Removed
	
	Revision 2.0  2006/02/28 09:11:13  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/10/14 09:01:02  marpas
	Strategy interface changed - there are cases where the strategy could update the inventoryobject itself
	
	Revision 1.1  2005/08/05 07:02:59  ivafam
	Added classes for packing strategy management
	
	
*/


#include <SMPackingStrategy.h>


_ACS_BEGIN_NAMESPACE(acs)

SMPackingStrategy::SMPackingStrategy(dbPersistent &obj, dbConnectionPool &p) : 
	_obj(obj), // PRQA S 2528 2
    pool_(p) 
{

}
	
const dbPersistent &SMPackingStrategy::getObject() const // PRQA S 4120
{
	return _obj;
}

dbPersistent &SMPackingStrategy::getObject()    // PRQA S 4120
{
	return _obj;    // PRQA S 4024
}

dbConnectionPool &SMPackingStrategy::getPool() noexcept { return pool_ ;} // PRQA S 4024, 4120

_ACS_END_NAMESPACE

