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
	Revision 1.6  2016/11/21 16:12:17  marfav
	S3MPM-847
	Added functionality to show plugins capabilities
	Subclass specific plugin loading moved in a dedicated virtual operation
	
	Revision 1.5  2014/03/19 19:20:54  marpas
	coding best practices applied
	warning fixed (qa & g++)
	
	Revision 1.4  2013/02/21 18:21:21  marpas
	interface redesign to allow dbConnectionPool and explicit dbConnections
	debug improved
	coding best practices applied
	qa rules
	test fixing
	
	Revision 1.3  2008/10/23 09:11:17  nicvac
	implementation
	
	Revision 1.2  2008/10/13 14:48:34  nicvac
	TlPreInventory implementation
	
	Revision 1.1  2008/10/13 14:42:25  nicvac
	PfPreInventory renamed in TlPreInventory
	
        
*/

#ifndef _TlPreInventory_H_
#define _TlPreInventory_H_

#include <acs_c++config.hpp>
#include <PreInventory.h>

#include <ConfigurablePluginLoader.h>

#include <exException.h>

#include <boost/program_options.hpp> // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)
      
/**
 * \brief
 * PreInventory running in Thin Layer context 
 **/

class TlPreInventory: public PreInventory  { // PRQA S 2109
public:

    /** TlPreInventory Exceptions */
    exDECLARE_EXCEPTION(exTlPreInventoryException, exException) ; // Base TlPreInventory Exception. // PRQA S 2131, 2502 2 
    exDECLARE_EXCEPTION(exTlPreInventoryCriticalException, exTlPreInventoryException) ; // Critical Exception. // PRQA S 2153


    /** Default Class constructor */
    TlPreInventory();
    /** Destructor */
    virtual ~TlPreInventory()  throw() ;

protected:
    /** Interface implementation */
    virtual void initFromInput(const boost::program_options::variables_map& argvParameters, char const * const * env, dbConnection &);
    /** Load JobOrder Plugins from the configured path */
	virtual void loadSpecificPlugins();

private:
    TlPreInventory(const TlPreInventory & ); // not implemented
    TlPreInventory &operator=(const TlPreInventory &); // not implemented

private: /** Specific Plugin loader */
    /** Job Order PlugIn Loader.  The loader must have the same lifetime of the application */
    ConfigurablePluginLoader _joPluginLoader;

private:

    /** Remove the filetypes not registered in DB. Useful to filter Not interesting interim filetypes.
    * Consider only FileTypes to be archieved */
    void filterFileTypes(dbConnection &);

private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(TlPreInventory)

};

_ACS_END_NAMESPACE

#endif //_TlPreInventory_H_

