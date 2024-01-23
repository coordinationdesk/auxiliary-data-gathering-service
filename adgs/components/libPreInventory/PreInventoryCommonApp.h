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
	Revision 1.4  2014/03/19 19:20:54  marpas
	coding best practices applied
	warning fixed (qa & g++)
	
	Revision 1.3  2013/02/21 18:21:20  marpas
	interface redesign to allow dbConnectionPool and explicit dbConnections
	debug improved
	coding best practices applied
	qa rules
	test fixing
	
	Revision 1.2  2009/01/28 17:06:26  nicvac
	Ignoring of extra Command line parameter.
	
	Revision 1.1  2008/10/13 14:40:44  nicvac
	Common app functions added.
	
        
*/

#ifndef _PreInventoryCommonApp_H_
#define _PreInventoryCommonApp_H_

#include <acs_c++config.hpp>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)
  
/**
 * \brief
 * Preinventory Common Application functions 
 **/

class PreInventoryCommonApp  { // PRQA S 2109
public:

    /** PreInventoryCommonApp Exceptions */
    exDECLARE_EXCEPTION(exPreInventoryCommonAppException, exException) ; // Base PreInventoryCommonApp Exception. // PRQA S 2131, 2502 2 
    exDECLARE_EXCEPTION(exPreInventoryCommonAppCriticalException, exPreInventoryCommonAppException) ; // Critical Exception. // PRQA S 2153

    /** Class constructor */
    explicit PreInventoryCommonApp(const std::string& applicationName);
    /** Destructor */
    virtual ~PreInventoryCommonApp() throw() ;

    /**Constants*/
    static class Constants {
    public:
        Constants():IgnoredPositionals("IgnoredPositionals")  {}
        /** Parameter name for command line parameter to ignore */
        const std::string IgnoredPositionals;   // PRQA S 2100
    } c;   // PRQA S 2100

protected:
    /** Read configuration */
    void getConfiguration();

private: 
    PreInventoryCommonApp(); // not implemented
    PreInventoryCommonApp(const PreInventoryCommonApp & ); // not implemented
    PreInventoryCommonApp &operator=(const PreInventoryCommonApp &); // not implemented

private:
    /** Application name */
    std::string _applicationName;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(PreInventoryCommonApp)

};

_ACS_END_NAMESPACE

#endif //_PreInventoryCommonApp_H_

