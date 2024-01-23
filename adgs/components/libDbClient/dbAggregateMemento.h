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
	
	Revision 1.2  2012/12/11 17:30:11  marpas
	qa rules
	
	Revision 1.1  2012/08/02 14:16:59  chicas
	 libDbClient REFACTORY; just added
	
*/


#ifndef _dbAggregateMemento_H_
#define _dbAggregateMemento_H_

#include <dbException.h>
#include <rsResourceSet.h>
#include <acs_c++config.hpp>


#include <vector>

_ACS_BEGIN_NAMESPACE(acs)


class dbMemento 
{
	public:
		
		/**
		 * The state should be stored in the 
		 * constructor
		 */
		dbMemento() { }
		virtual ~dbMemento() throw()  { }
		
		/**
		 */
		virtual void rollback() =0;
		

	private:
		// declared Not implemented
		dbMemento(dbMemento &) ;
		dbMemento& operator=(dbMemento const &) ;
		
		//void wrapperDeletion(dbStateWrapper *) ;
		
	private:

};

/**
 * This Class is a repository of resources for a transaction
 */
class dbAggregateMemento : virtual public ThreadSafe
{
	public:
		dbAggregateMemento() ;
		~dbAggregateMemento() throw() ;
		void add(dbMemento &) ;
		void undo() ;
		

	private:
		//Not implemented
		dbAggregateMemento(dbAggregateMemento &) ;
		dbAggregateMemento& operator=(dbAggregateMemento const &) ;
	
	private:
		std::vector<dbMemento*> _managers ;
//		rsResourceSet _keyMap ;
};

_ACS_END_NAMESPACE


#endif // _dbAggregateMemento_H_
