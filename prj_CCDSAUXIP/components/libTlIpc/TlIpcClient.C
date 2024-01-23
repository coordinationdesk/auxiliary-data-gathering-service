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
	Revision 2.1  2011/03/01 16:31:42  marpas
	GCC 4.4.x support
	
	Revision 2.0  2006/02/28 10:10:03  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 18:04:18  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:33:13  marfav
	Import libTlIpc
	


*/

#include <TlIpcClient.h>

#include <rsResourceSet.h>
#include <XMLOstream.h>

#include <iostream>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


TlIpcClient::TlIpcClient() :
		TcpIpIpcClient<TlMessage>()
{}


TlIpcClient::~TlIpcClient() throw() {}

string
TlIpcClient::tlMessage2XMLString (const TlMessage& m)
{
	ostringstream os;
	{
		XMLOstream xos (os, "CmdRoot");
		{
			XMLOstream::Tag fieldCmdType (xos, "cmdType", m.cmdType());
		}
		{
			XMLOstream::Tag fieldCmdValue (xos, "cmdValue", m.cmdValue());
		}
		{
			XMLOstream::Tag fieldParamValue (xos, "paramValue", m.paramValue());
		}
		{
			ostringstream oids;
			oids << m.orderId();
			XMLOstream::Tag fieldOrderId (xos, "orderId", oids.str());
		}
	} // close CmdRoot Tag
	return os.str();
}

void
TlIpcClient::marshal (const TlMessage& aTlMessage)
{
	string toBeSent = tlMessage2XMLString (aTlMessage);
	toBeSent = packString (toBeSent);
	sendToServer (toBeSent);
}

string
TlIpcClient::packString (string& toPack)
{
	string packed(toPack);
	// replace tabs
	replace (toPack.begin(), toPack.end() , '\t', '\n');
    size_t pos = 0 ;
	while ((pos=toPack.find('\n')) != string::npos) { // PRQA S 4244
		toPack.erase (pos,1);
    }
	toPack.append("\n");
	return toPack;
}


_ACS_END_NAMESPACE
