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
	
	Revision 4.3  2012/12/11 17:30:11  marpas
	qa rules
	
	Revision 4.2  2012/11/21 09:58:23  marpas
	readding files for realignement
	
	Revision 4.0  2011/10/18 12:31:12  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/05 14:47:49  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.2  2009/03/02 14:03:29  marpas
	CODECHECK parsing, no regressions
	
	Revision 2.1  2008/01/10 11:41:41  paoscu
	It is now possible by default to save more times the same persistent wrapper during a transaction.
	Only the first one is actully stored while others are silently ignored.
	The method dbConnection::newTransactionPhase has been removed.
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/03/07 14:00:35  marpas
	Importing dbExpressWrapper
	
	Revision 1.6  2004/05/13 17:28:01  marpas
	ignoreMultipleObjectStates to ignore multiple instances of the same object memento. It is used during multiphase transactions. 
	In this case no exception is issued and the oldest state is maintained
	
	Revision 1.5  2004/05/11 09:21:23  marpas
	multiple management of the same dbPersistent checked
	
	Revision 1.4  2004/04/14 09:40:17  marpas
	ThreadSafe used
	
	Revision 1.3  2004/04/13 08:30:54  marpas
	documentation updated
	
	Revision 1.2  2004/02/03 17:00:59  paoscu
	Work on transaction state.
	Better doxygen comments.
	
	Revision 1.1  2004/01/27 19:28:44  paoscu
	Transaction state management added.
	
*/


#ifndef _dbAggregateTransactionState_H_
#define _dbAggregateTransactionState_H_

#include <dbException.h>
#include <rsResourceSet.h>

#include <vector>


_ACS_BEGIN_NAMESPACE(acs)

	
class dbStateManager ;
class dbStateWrapper ;
class dbConnection ;

/**
 * This Class is a repository of resources for a transaction
 */
class dbAggregateTransactionState : virtual public ThreadSafe // PRQA S 1703, 2109, 2153
{
public:
	dbAggregateTransactionState() ;
	virtual ~dbAggregateTransactionState() throw() ;
	void add(dbStateWrapper &, dbConnection &) ;
	void undo() ;

	//Method removed. This is now the default behaviour
	//void ignoreMultipleObjectStates() ;

	template<class _Tp>
	void setResource(const std::string &, const _Tp &value) ;

	template<class _Tp>
	void getResource(const std::string &, _Tp &value) const;


private:
	//Not implemented
	dbAggregateTransactionState(const dbAggregateTransactionState &) ;
	dbAggregateTransactionState& operator=(const dbAggregateTransactionState &) ;

private:
	std::vector<dbStateManager*> _managers ;
	rsResourceSet _keyMap ;
};


template<class _Tp>
inline void dbAggregateTransactionState::setResource(const std::string &s, const _Tp &value)
{
	_keyMap.setValue( s, value) ;
}


template<class _Tp>
inline void dbAggregateTransactionState::getResource(const std::string &s, _Tp &value) const
{
	_keyMap.getValue( s, value) ;
}


_ACS_END_NAMESPACE

#endif // _dbAggregateTransactionState_H_
