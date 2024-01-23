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
*/

#ifndef _BaseSoap_H_
#define _BaseSoap_H_ 

#include <exMacros.h>

#include <stdsoap2.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor" // PRQA S 1040 3
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wold-style-cast"

#include <basesoapH.h>

#pragma GCC diagnostic pop


SOAP_NMAC struct Namespace namespaces[] =
{
};

_ACS_BEGIN_NAMESPACE(acs)

class BaseSoap
{

	public:
		static void doSoapSSLInit ();

	private:
		// This is just an helper class providing static operations
		BaseSoap();
		~BaseSoap();
		BaseSoap (const BaseSoap&);
		BaseSoap& operator= (const BaseSoap&);
};


_ACS_END_NAMESPACE


#endif // _BaseSoap_H_
