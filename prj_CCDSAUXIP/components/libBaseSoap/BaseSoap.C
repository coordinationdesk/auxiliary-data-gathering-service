// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA, Inc.

	$Prod: A.C.S. Base Soap Library $

	$Id$

	$Author$

	$Log$
	Revision 1.7  2015/10/22 15:35:26  marpas
	init callback now requires a private message after call
	
	Revision 1.6  2015/10/22 09:07:33  marpas
	coding best practices applied
	initialization message is now a PRIVINFO instead of a plain INFO
	
	Revision 1.5  2015/06/10 10:59:09  damdec
	Temporary SoapSSL init removed.
	
	Revision 1.4  2015/06/03 13:20:55  damdec
	SOAP SSL initialization performed by means of InitCallbackManagerSingleton.
	
*/

#include <lBaseSoapV.h>
#include <BaseSoap.h>

#include <CryptoEngineInitializer.h>
#include <Filterables.h>

#include <InitCallbackManagerSingleton.h>

#include <stdsoap2.h>

using namespace acs;

namespace {
    lBaseSoapV version ; // versioning 

class initializer : public Singleton<initializer> // PRQA S 2153
{
public:
    friend initializer* Singleton<initializer>::instance(); // PRQA S 2107
	virtual ~initializer() throw() {} // PRQA S 2131

private:
	initializer() : Singleton<initializer>()
	{
		ACS_LOG_PRIVINFO ("Initializing SSL engine for SOAP library");
		soap_ssl_init();
		utils::CryptoEngineInitializer::instance() ;
	}
	initializer(const initializer &) ; // not implemented 
	initializer& operator=(const initializer &) ; // not implemented 

};

class callBackAdder
{
public:
	callBackAdder()
	{
		InitCallbackManagerSingleton::addInitCallback (&BaseSoap::doSoapSSLInit, "SOAP SSL Initializer", 5, true); // PRQA S 4400
	}

};

const callBackAdder myCallbackAdder;

};

using namespace acs;

void BaseSoap::doSoapSSLInit ()
{
	initializer::instance();
}



