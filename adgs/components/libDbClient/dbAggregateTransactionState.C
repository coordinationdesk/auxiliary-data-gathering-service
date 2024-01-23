// PRQA S 1050 EOF
/*

	Copyright 1995-2013 Advanced Computer Systems , Inc.
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
	Revision 5.0  2013/06/06 18:52:56  marpas
	adopting libException 5.x standards
	
	Revision 4.5  2013/04/17 17:34:44  marpas
	dbStateManager & dbStateWrapper interface changed
	
	Revision 4.4  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.3  2012/11/21 09:58:23  marpas
	readding files for realignement
	
	Revision 4.1  2012/02/17 13:55:13  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:12  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/05 14:47:49  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.1  2008/01/10 11:41:41  paoscu
	It is now possible by default to save more times the same persistent wrapper during a transaction.
	Only the first one is actully stored while others are silently ignored.
	The method dbConnection::newTransactionPhase has been removed.
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/03/07 14:00:35  marpas
	Importing dbExpressWrapper
	
	Revision 1.7  2004/05/13 17:28:01  marpas
	ignoreMultipleObjectStates to ignore multiple instances of the same object memento. It is used during multiphase transactions. 
	In this case no exception is issued and the oldest state is maintained
	
	Revision 1.6  2004/05/11 09:21:23  marpas
	multiple management of the same dbPersistent checked
	
	Revision 1.5  2004/04/14 09:40:17  marpas
	ThreadSafe used
	
	Revision 1.4  2004/04/13 08:30:54  marpas
	documentation updated
	
	Revision 1.3  2004/03/30 18:47:46  marpas
	dbStateWrapper and dbStateManager are connected in order to avoid a
	manager would try to call undo on a destroyed wrapper.
	
	Revision 1.2  2004/02/03 17:00:59  paoscu
	Work on transaction state.
	Better doxygen comments.
	
	Revision 1.1  2004/01/27 19:28:44  paoscu
	Transaction state management added.
	
*/


#include <dbAggregateTransactionState.h>
#include <dbStateManager.h>
#include <sstream>
#include <iomanip>



_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

dbAggregateTransactionState::dbAggregateTransactionState() :
    ThreadSafe(),
    _managers(),  
    _keyMap()

{

}

dbAggregateTransactionState::~dbAggregateTransactionState() throw ()
{
// PRQA S 4631 L1
	for(size_t i=0; i<_managers.size(); i++) { // PRQA S 4238, 4244
		try { // no exception out of this method
			delete _managers[i] ;
		}
		catch(exception &x) {
			ACS_LOG_NOTIFY_EX(x) ;
		}
	}	
// PRQA L:L1
}


void dbAggregateTransactionState::add(dbStateWrapper &w, dbConnection &c) // PRQA S 4020
{
	Lock lock(*this) ;

	for(size_t i=0; i<_managers.size(); i++)  { // PRQA S 4238, 4244
		if (_managers[i]->manages(w)) { // a manager can manage the given state ?
			return ; // ok done
		}
    }
		
	dbStateManager* m=new dbStateManager(w, c) ; // else we need a new state manager to manage the given one
	_managers.push_back(m) ; // add to the collection
}


void dbAggregateTransactionState::undo()
{
	Lock lock(*this) ;
	for(size_t i=0; i<_managers.size(); i++) { // PRQA S 4238, 4244
		_managers[i]->undo() ;
    }
	
}


_ACS_END_NAMESPACE
