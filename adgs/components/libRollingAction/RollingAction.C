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

	$Prod: A.C.S. Rolling RollingAction Library $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2013/09/26 19:25:16  nicvac
	S2PDGS-303: Rolling actions can be ordered on inventory fields criteria.
	
	Revision 2.0  2013/09/23 12:19:34  nicvac
	S2PDGS-465: t_inventorysm.inv_id references t_inventory.id_inv.
	
	Revision 1.8  2013/09/19 17:33:52  nicvac
	S2PDGS-303: implementation.
	
	Revision 1.7  2013/09/13 15:34:23  nicvac
	S2PDGS-303 implementing.
	
	Revision 1.6  2012/06/06 16:25:51  marfav
	Code improved removing compiler warnings
	
	Revision 1.5  2009/03/04 18:10:54  marpas
	CODECHECK parsing strategy improved: no regressions
	
	Revision 1.4  2008/10/09 17:18:18  ivafam
	Work in progress
	
	Revision 1.3  2008/10/01 13:54:00  ivafam
	Comment improved
	
	Revision 1.2  2008/09/18 07:05:05  ivafam
	Work in progress
	
	Revision 1.1.1.1  2008/05/27 17:09:02  ivafam
	Import libRollingAction
	
	
*/


#include <RollingAction.h>
#include <dbInvRollingPolicies.h>
#include <rlaV.h>


using namespace std ;


_ACS_BEGIN_NAMESPACE(acs)

namespace {
    rlaV version ;
}

RollingAction::RollingAction( const dbPersistent::IdType& obj,  const unsigned int & actionid, const unsigned int & storage) :
	_obj(obj),
	_RollingActionid(actionid),
	_storageid(storage)
{
}

RollingAction::~RollingAction()
{
}

RollingAction::RollingAction(const RollingAction & e) :
	_obj(e._obj),
	_RollingActionid(e._RollingActionid),
	_storageid(e._storageid)
{
	// *this = e ;
}

RollingAction&  RollingAction::operator=(const RollingAction & e)
{
	if (this != &e) {
		_obj		= e._obj;
		_RollingActionid	= e._RollingActionid;
		_storageid	= e._storageid;
	}
	return *this ;
}




/////////////////////////////////////////////////////////////////////////////////////////
// operators 
/////////////////////////////////////////////////////////////////////////////////////////

bool operator<(const RollingAction &a, const RollingAction &b) throw()  // PRQA S 4020
{
	if (a.getObject()  < b.getObject()) { return true ; }
	if (a.getObject() == b.getObject()) {
		return a.getStorage() < b.getStorage();
	}
	return false;
}

bool operator>(const RollingAction &a, const RollingAction &b) throw() // PRQA S 4020
{
	if (a.getObject()  > b.getObject()) { return true ; }
	if (a.getObject() == b.getObject()) {
		return a.getStorage() > b.getStorage();
	}
	return false;
}


bool operator==(const RollingAction &a, const RollingAction &b) throw() 
{
    return (a.getObject() == b.getObject()) && 
           (a.getStorage() == b.getStorage()) && 
           (a.getRollingAction() == b.getRollingAction()) ;
}

bool operator!=(const RollingAction &a, const RollingAction &b) throw() 
{ 
	return not operator==(a,b) ; 
}
_ACS_END_NAMESPACE
