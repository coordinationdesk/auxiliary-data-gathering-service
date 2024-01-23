// PRQA S 1050 EOF
/*
	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libPreInventory$

	$Id$

	$Author$

	$Log$
	Revision 1.5  2014/03/19 19:20:54  marpas
	coding best practices applied
	warning fixed (qa & g++)
	
	Revision 1.4  2013/02/21 18:21:20  marpas
	interface redesign to allow dbConnectionPool and explicit dbConnections
	debug improved
	coding best practices applied
	qa rules
	test fixing
	
	Revision 1.3  2008/10/23 09:07:55  nicvac
	program_options used for argv.
	
	Revision 1.2  2008/10/08 13:50:30  nicvac
	Implementation.
	
	Revision 1.1.1.1  2008/10/01 15:41:36  nicvac
	Import libPreInventory
	
        
*/

#ifndef _DisPreInventory_H_
#define _DisPreInventory_H_

#include <acs_c++config.hpp>
#include <PreInventory.h>

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)
     
/**
 * \brief
 * PreInventory running in DIS context
 **/

class DisPreInventory: public PreInventory  { // PRQA S 2109
public:

    /** DisPreInventory Exceptions */
    exDECLARE_EXCEPTION(exDisPreInventoryException, exException) ; // Base DisPreInventory Exception. // PRQA S 2131, 2502 2 
    exDECLARE_EXCEPTION(exDisPreInventoryCriticalException, exDisPreInventoryException) ; // Critical Exception. // PRQA S 2153


    /** Default Class constructor */
    DisPreInventory();
    /** Destructor */
    virtual ~DisPreInventory() throw() ;
#ifdef ACS_TEST
public:
#else
protected:
#endif
    /** Interface implementation */
    virtual void initFromInput(const boost::program_options::variables_map& argvParameters, char const * const * env, dbConnection &);


private:
    DisPreInventory(const DisPreInventory & ); // not implemented 
    DisPreInventory &operator=(const DisPreInventory &); // not implemented 


private:

    ACS_CLASS_DECLARE_DEBUG_LEVEL(DisPreInventory)
};

_ACS_END_NAMESPACE

#endif //_DisPreInventory_H_

