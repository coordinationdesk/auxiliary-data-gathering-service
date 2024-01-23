// PRQA S 1050 EOF
/*
	Copyright 2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libPackageTask$

	$Id$

	$Author$

	$Log$

*/

#include <XMLInitializer.h>

#include <Filterables.h>
#include <XMLHandler.h>

#include <xercesc/util/PlatformUtils.hpp> // PRQA S 1013

using namespace acs;
using namespace std;


// Singleton class used to initialize and terminate Xerces infrastructure

XMLInitializer::XMLInitializer() : pattern::Singleton<XMLInitializer>()
{ 
    XERCES_CPP_NAMESPACE::XMLPlatformUtils::Initialize(); 
}

XMLInitializer::~XMLInitializer() throw() // PRQA S 2131
    { 
        //  PRQA S 4631 L1
        try {
            XERCES_CPP_NAMESPACE::XMLPlatformUtils::Terminate(); 
        }
        catch (const XERCES_CPP_NAMESPACE::XMLException& x)
        {
            const XMLCh * err = x.getMessage() ;
            std::string errmsg ;
            ACS_LOG_ERROR("XML Error: \"" << XMLHandler::hpTrimmedString(err, errmsg) << '\n') ;
        }    
        catch(std::exception &x) {
            ACS_LOG_NOTIFY_EX(x) ;
        }
        // PRQA L:L1
    } 
    

