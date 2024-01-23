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

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
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
	
	Revision 2.1  2009/05/22 15:13:50  marpas
	some rules fixed - need more work
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/10/17 09:51:10  enrcar
	Aligned to dbClient
	
	Revision 1.5  2004/05/11 09:19:20  marpas
	method to avoid multiple instance in the same transaction state added
	
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




#ifndef _dbStateManager_H_
#define _dbStateManager_H_

#include <acs_c++config.hpp>
#include<dbStateWrapper.h>



_ACS_BEGIN_NAMESPACE(acs)

/**
 * This class remember the state of dbStateWrapper
 */
class dbStateManager // PRQA S 2109
{
public:

	/**
	 * The state of the dbStateWrapper is stored in the 
	 * constructor
	 */
	explicit dbStateManager(dbStateWrapper &, dbConnection &) ;
	dbStateManager() = delete ;
	dbStateManager(const dbStateManager &) = delete ;
	dbStateManager& operator=(const dbStateManager &) = delete ;
	dbStateManager(dbStateManager &&) = delete ;
	dbStateManager& operator=(dbStateManager &&) = delete ;
	~dbStateManager() noexcept ;

	/**
	 * Restore the state of the dbStateWrapper
	 * calling dbStateWrapper::setState
	 */
	void undo() ;

	bool manages(const dbStateWrapper &) const ;

private:
	// declared Not implemented

	friend class dbStateWrapper;    // PRQA S 2107

	void wrapperDeletion(dbStateWrapper *) ;

private:
	dbStateWrapper * _stateWrapper ;
	dbStateWrapper::dbMemento *_memento;
};


_ACS_END_NAMESPACE

#endif // _dbStateManager_H_
