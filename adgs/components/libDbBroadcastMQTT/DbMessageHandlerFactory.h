// PRQA S 1050 EOF
/*
   Copyright 2019-, Exprivia SpA
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Advanced Computer Systems, Inc. 

   DbMessageHandlerFactory header file
*/ 

#ifndef _DbMessageHandlerFactory_H_
#define _DbMessageHandlerFactory_H_

#include <DbMessageHandler.h>
#include <Postgres8MessageHandler.h>
#include <exException.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs) 

class DbMessageHandlerFactory 
{
/**
	Factory to create an instance of a specific payload message handler.
	So far, only Posgres8 is supported.
*/

public:
	
	inline static DbMessageHandler* instance(const std::string& message) 
	{ 
		const std::string driver_name (DbMessageHandler::get_driver_name(message)) ; 

		if ("Postgres8" == driver_name)
		{
			return new Postgres8MessageHandler(message) ; 

		} else if ("Informix9" == driver_name)
		{
			ACS_THROW( exIllegalValueException("Driver not yet supported: \"" + driver_name + "\"") ) ; 
		}
		else 
		{
			ACS_THROW( exIllegalValueException("Driver unknown: \"" + driver_name + "\"") ) ; 
		}

		return NULL ; 
	}

};

_ACS_END_NAMESPACE

#endif //_DbMessageHandlerFactory_H_
