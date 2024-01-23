// PRQA S 1050 EOF
/*

	Copyright 1995-2009, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/06 18:53:00  marpas
	adopting libException 5.x standards
	
	Revision 4.3  2012/12/11 17:30:12  marpas
	qa rules
	
	Revision 4.2  2012/11/21 09:58:24  marpas
	readding files for realignement
	
	Revision 4.0  2011/10/18 12:31:15  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/05 14:47:53  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/03/07 14:00:35  marpas
	Importing dbExpressWrapper
	
	Revision 1.2  2004/03/30 18:47:46  marpas
	dbStateWrapper and dbStateManager are connected in order to avoid a
	manager would try to call undo on a destroyed wrapper.
	
	Revision 1.1  2004/01/27 19:28:44  paoscu
	Transaction state management added.
	
*/

#include<dbStateWrapper.h>
#include<dbStateManager.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace acs;

dbStateWrapper::dbMemento::dbMemento(){}

dbStateWrapper::dbMemento::~dbMemento() {}


dbStateWrapper::dbStateWrapper() : _mgr(0) 
{
}

dbStateWrapper::dbStateWrapper(const dbStateWrapper &) : _mgr(0) 
{
	// nothing to copy 
}

dbStateWrapper &dbStateWrapper::operator=(const dbStateWrapper &w) 
{
	if (this != &w) {
		signalDeletion() ; // no longer owned 
		_mgr = 0 ;
	}
	return *this ;
}

dbStateWrapper::dbStateWrapper(dbStateWrapper && other) noexcept :
	_mgr(0)
{
	// nothing to move
}

dbStateWrapper& dbStateWrapper::operator=(dbStateWrapper && other) noexcept {
	signalDeletion() ; // no longer owned 
	other.signalDeletion() ; 
	_mgr = 0 ;
	other._mgr = 0;
	return *this;
}

dbStateWrapper::~dbStateWrapper() 
{
    // PRQA S 4631 L1
	try {
		signalDeletion() ;
	}
	catch(std::exception &x) {
		ACS_LOG_NOTIFY_EX(x) ;
	}
    // PRQA L:L1
}

void dbStateWrapper::registerManager(dbStateManager *m) // PRQA S 4121
{
	_mgr = m ;
}

void dbStateWrapper::unregisterManager(dbStateManager *m) 
{
	if (_mgr == m) { _mgr = 0 ; }
}

void dbStateWrapper::signalDeletion()
{
	if (_mgr) {
		_mgr->wrapperDeletion(this) ;
    }
}



_ACS_END_NAMESPACE
