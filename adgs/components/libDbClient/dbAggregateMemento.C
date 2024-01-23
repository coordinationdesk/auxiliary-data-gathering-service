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
	
	Revision 1.1  2012/08/02 14:16:59  chicas
	 libDbClient REFACTORY; just added
	
	
*/


#include <dbAggregateMemento.h>
#include <sstream>
#include <algorithm>
#include <iomanip>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

dbAggregateMemento::dbAggregateMemento() :
    _managers()  
    //_keyMap()

{

}

dbAggregateMemento::~dbAggregateMemento() throw ()
{
	for(size_t i=0; i<_managers.size(); i++) {
		try { // no exception out of this method
			delete _managers[i] ;
		}
		catch(exception &x) {
			ACS_LOG_NOTIFY_EX(x) ;
		}
	}
}

void dbAggregateMemento::add(dbMemento &w)
{
	Lock lock(*this) ;

    // a manager can manage the given state ?
    vector<dbMemento*>::iterator it =   find(_managers.begin(),_managers.end(),&w);
  
    if (it != _managers.end())
        return;// ok done
		
	//dbMemento* m=new dbMemento(w) ; // else we need a new state manager to manage the given one
	//_managers.push_back(m) ; // add to the collection
	_managers.push_back(&w) ; // add to the collection
}


void dbAggregateMemento::undo()
{
	Lock lock(*this) ;
    for_each(_managers.begin(),_managers.end(),mem_fun(&dbMemento::rollback) );

	//for(size_t i=0; i<_managers.size(); i++)
	//	_managers[i]->rollback() ;
	
}



_ACS_END_NAMESPACE
