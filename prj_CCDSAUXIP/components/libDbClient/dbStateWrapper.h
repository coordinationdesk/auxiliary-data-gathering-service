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
	Revision 5.1  2013/06/11 17:03:00  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:53:00  marpas
	adopting libException 5.x standards
	
	Revision 4.4  2013/04/17 17:34:44  marpas
	dbStateManager & dbStateWrapper interface changed
	
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
	
	Revision 1.3  2004/03/30 18:47:46  marpas
	dbStateWrapper and dbStateManager are connected in order to avoid a
	manager would try to call undo on a destroyed wrapper.
	
	Revision 1.2  2004/02/03 17:00:59  paoscu
	Work on transaction state.
	Better doxygen comments.
	
	Revision 1.1  2004/01/27 19:28:44  paoscu
	Transaction state management added.
	
*/



#ifndef _dbStateWrapper_H_
#define _dbStateWrapper_H_

#include <acs_c++config.hpp>
#include <dbException.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbStateManager ;
class dbConnection ;

/**
 * This is the interface for classes that can save their state in database
 * They must provide methods to get and set the state (dbMemento) 
 */
class dbStateWrapper // PRQA S 2109
{
public:
		
	//////////////////////////////
	// Class dbMemento          //
	//////////////////////////////

	/**
	 * This is the base class to save the inner state of
	 * objects that can be stored in database
	 * \see dbStateWrapper
	 */
	class dbMemento
	{
	public:
		dbMemento() ;
		dbMemento(dbMemento const &) = delete ;
		dbMemento& operator=(dbMemento const &) = delete ;
		virtual ~dbMemento() ;
	};
		
	dbStateWrapper() ;
	virtual ~dbStateWrapper() ;
	void registerManager(dbStateManager *) ;
	void unregisterManager(dbStateManager *) ;

	virtual dbMemento* getState(dbConnection &) const = 0 ;
	virtual void setState(dbMemento const*) = 0 ;
protected:
	dbStateWrapper(const dbStateWrapper &) ;
	dbStateWrapper &operator=(const dbStateWrapper &) ;
private:
	void signalDeletion() ;
private:
	dbStateManager *_mgr ;
};


_ACS_END_NAMESPACE

#endif // _dbStateWrapper_H_
