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

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 1.4  2014/03/19 19:20:54  marpas
	coding best practices applied
	warning fixed (qa & g++)
	
	Revision 1.3  2013/02/21 18:21:21  marpas
	interface redesign to allow dbConnectionPool and explicit dbConnections
	debug improved
	coding best practices applied
	qa rules
	test fixing
	
	Revision 1.2  2012/12/14 16:59:45  chicas
	removed plugin; use DefaultJobOrder
	
	Revision 1.1  2012/10/11 15:02:14  chicas
	Added TaskPreInventory
	
        
*/

#ifndef _TaskPreInventory_H_
#define _TaskPreInventory_H_

#include <acs_c++config.hpp>
#include <PreInventory.h>

#include <exException.h>

#include <boost/program_options.hpp> // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)
      
/**
 * \brief
 * PreInventory running in Thin Layer context 
 **/

class TaskPreInventory: public PreInventory  { // PRQA S 2109
public:

    /** TaskPreInventory Exceptions */
    exDECLARE_EXCEPTION(exTaskPreInventoryException, exException) ; // Base TaskPreInventory Exception. // PRQA S 2131, 2502 2 
    exDECLARE_EXCEPTION(exTaskPreInventoryCriticalException, exTaskPreInventoryException) ; // Critical Exception. // PRQA S 2153


    /** Default Class constructor */
    TaskPreInventory();
    /** Destructor */
    virtual ~TaskPreInventory()  throw() ;

protected:
    /** Interface implementation */
    virtual void initFromInput(const boost::program_options::variables_map& argvParameters, char const * const * env, dbConnection &);

private:  
    TaskPreInventory(const TaskPreInventory & ); // not implemented
    TaskPreInventory &operator=(const TaskPreInventory &); // not implemented

    /** Remove the filetypes not registered in DB. Useful to filter Not interesting interim filetypes.
    * Consider only FileTypes to be archieved */
    void filterFileTypes(dbConnection &);

private:
    static const std::string _taskName ; 
    ACS_CLASS_DECLARE_DEBUG_LEVEL(TaskPreInventory)

};

_ACS_END_NAMESPACE

#endif //_TaskPreInventory_H_

