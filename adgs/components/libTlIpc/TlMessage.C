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
	Revision 2.1  2012/06/05 13:28:51  marfav
	compiler warnings removed
	
	Revision 2.0  2006/02/28 10:10:03  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 18:04:18  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:33:13  marfav
	Import libTlIpc

*/

#include <TlMessage.h>
#include <tlipcV.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace {
    tlipcV version;
}

// static attributes initialization
const string TlMessage::suspendType = "SUSPEND";
const string TlMessage::resumeType = "RESUME";
const string TlMessage::stopType = "STOP";
const string TlMessage::abortType = "ABORT";
const string TlMessage::killType = "KILL";
const string TlMessage::userType = "USER";





TlMessage::TlMessage (const string &cmdType, const string &cmdValue, const string &paramValue, unsigned int orderid):
	_cmdType(cmdType),
	_cmdValue(cmdValue),
	_paramValue(paramValue),
	_orderId(orderid)
{
}

TlMessage::TlMessage (TlMessage const & r) :
	_cmdType(r._cmdType),
	_cmdValue(r._cmdValue),
	_paramValue(r._paramValue),
	_orderId(r._orderId)
{
}


TlMessage&
TlMessage::operator=(TlMessage const &t)
{
	if (this != &t)
	{
		_cmdType = t._cmdType;
		_cmdValue =t._cmdValue;
		_paramValue = t._paramValue;
		_orderId = t._orderId;
	}
	return *this;
}

void
TlMessage::cmdType (const string &s) // PRQA S 4121
{
	_cmdType = s;
}


void
TlMessage::cmdValue (const string &s) // PRQA S 4121
{
	_cmdValue = s;
}


void
TlMessage::paramValue (const string &s) // PRQA S 4121
{
	_paramValue = s;
}

void
TlMessage::orderId (unsigned int i) // PRQA S 4121
{
	_orderId = i;
}

_ACS_END_NAMESPACE
