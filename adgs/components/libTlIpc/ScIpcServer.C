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

	$Prod: A.C.S. ThinLayer IPC Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/07/07 18:30:26  marpas
	adopting libException 5.x standards
	qa warnigs removed
	coding best practices applied
	compilation warnings removed
	
	Revision 2.0  2006/02/28 10:10:03  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 18:04:18  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:33:14  marfav
	Import libTlIpc
	


*/


#include <ScIpcServer.h>

#include <XMLIstringstream.h>
#include <rsResourceSet.h>
#include <exStream.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(ScIpcServer) ;


ScIpcServer::ScIpcServer() :
		TcpIpIpcServer<TlMessage> (false, true) // listen to standard input stream only
{
}

ScIpcServer::~ScIpcServer() throw() {}

void
ScIpcServer::unmarshal (const string& s)
{
	try
	{
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "unmarshalling string " << s ) ;

		XMLIstringstream xis(s);
		rsResourceSet readRs;
		xis >> readRs;

		TlMessage theMessage;
		string aString;
		readRs.getValue ("cmdType", aString);
		theMessage.cmdType (aString);
		readRs.getValue ("cmdValue", aString);
		theMessage.cmdValue (aString);
		readRs.getValue ("paramValue", aString);
		theMessage.paramValue (aString);
		unsigned int aUInt = 0 ;
		readRs.getValue ("orderId", aUInt);
		theMessage.orderId (aUInt);

		putMessage (theMessage);
	}
	catch (exException& ex)
	{
		ACS_THROW (TcpUnmarshalException (ex, "Unable to unmarshal message " + s)); // PRQA S 3081
	}
}

_ACS_END_NAMESPACE
