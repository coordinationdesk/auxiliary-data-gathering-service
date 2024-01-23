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
	Revision 1.8  2016/03/11 09:03:01  tergem
	parameter file adde to TaskIn args
	
	Revision 1.7  2014/03/19 19:20:54  marpas
	coding best practices applied
	warning fixed (qa & g++)
	
	Revision 1.6  2013/02/21 18:21:20  marpas
	interface redesign to allow dbConnectionPool and explicit dbConnections
	debug improved
	coding best practices applied
	qa rules
	test fixing
	
	Revision 1.5  2012/12/17 12:43:17  marpas
	compilation warning removed
	
	Revision 1.4  2012/12/17 12:36:00  marpas
	compilation warning removed
	test compilation partially fixed
	
	Revision 1.3  2008/11/28 10:49:29  nicvac
	attribute renamed.
	
	Revision 1.2  2008/10/23 09:11:38  nicvac
	Implementation
	
	Revision 1.1.1.1  2008/10/01 15:41:36  nicvac
	Import libPreInventory
	
        
*/

#ifndef _SingleProdPreInventory_H_
#define _SingleProdPreInventory_H_

#include <acs_c++config.hpp>
#include <PreInventory.h>

#include <boost/program_options.hpp> // PRQA S 1013

#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)
   
/**
 * \brief
 * PreInventory running on single file (e.g. ftp DGF context)
 **/

class SingleProdPreInventory: public PreInventory { // PRQA S 2109
public:

    /** SingleProdPreInventory Exceptions */
    exDECLARE_EXCEPTION(exSingleProdPreInventoryException, exException) ; // Base SingleProdPreInventory Exception. // PRQA S 2131, 2502 2 
    exDECLARE_EXCEPTION(exSingleProdPreInventoryCriticalException, exSingleProdPreInventoryException) ; // Critical Exception. // PRQA S 2153


    /** Default Class constructor */
    SingleProdPreInventory();
    /** Destructor */
    virtual ~SingleProdPreInventory()  throw() ;

    /** Constants */
    static class Constants {
    public:
        ~Constants()  throw() {}
        Constants() : frameworkParams(), Id()
        { 
            //Setting parameters required by the Framework. They must be in parameters configuration
            //This forced cast because I want mandatoryParams as constants (without using get and set methods)
            (const_cast< std::vector<std::string>* >(&frameworkParams))->push_back("inputFile");    // PRQA S 3081 4
            (const_cast< std::vector<std::string>* >(&frameworkParams))->push_back("satellitename");
            (const_cast< std::vector<std::string>* >(&frameworkParams))->push_back("missionId");
            (const_cast< std::vector<std::string>* >(&frameworkParams))->push_back("fileType");
            (const_cast< std::vector<std::string>* >(&frameworkParams))->push_back("parameterFile");
        }

        /** Parameters needed by the Framework */
        const std::vector<std::string> frameworkParams;                 // PRQA S 2100
        /** Parameters Id */
        enum { inputFile=0, satellitename, missionId, fileType, parameterFile } Id;    // PRQA S 2100
    private:
        Constants(const Constants &) ; // not implemented
        Constants& operator= (const Constants &) ; // not implemented
    } c; // PRQA S 2100

protected:
    /** Interface implementation */
    virtual void initFromInput(const boost::program_options::variables_map& argvParameters, char const * const * env, dbConnection &);


private:  
    SingleProdPreInventory(const SingleProdPreInventory & ); // not implemented
    SingleProdPreInventory &operator=(const SingleProdPreInventory &); // not implemented

private:

  ACS_CLASS_DECLARE_DEBUG_LEVEL(SingleProdPreInventory) ;

};

_ACS_END_NAMESPACE

#endif //_SingleProdPreInventory_H_

