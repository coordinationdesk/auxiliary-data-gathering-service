/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:  A.C.S. Scheduler filterable messages wrapper Library $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2010/09/07 10:14:37  marfav
	VEN-1875
	Messages not compliant to GEN_PROC_ICD can be filtered and
	not forwarded to MCF
	
	Revision 1.2  2004/12/03 17:58:10  marfav
	Header fixed
	
	Revision 1.1.1.1  2003/07/16 09:44:33  marfav
	Import libSchedMessages
	
	

*/

#include <MsgFilterableWrp.h>

using namespace std;

_ACS_BEGIN_NAMESPACE(acs)
MsgFilterableWrp::MsgFilterableWrp()
{}

MsgFilterableWrp::~MsgFilterableWrp()
{}

ACS_SMARTPTR <exFMessage>
MsgFilterableWrp::getFilterable (const string& msg, bool* isValid)
{
	// message format is
	// 2003-07-04T16:36:11.072867 cryosatdev-3 L0_CHOPPER 01.00 [0000012659] : [I] Reading packet 10 out of 4236
	// have to find the second '['

	if (isValid)
	{
		// VEN-1875
		// first assumption is that the message is not valid
		// if the validity is found the flag is updated later
		*isValid = false;
	}

	char msg_type;
	size_t pos = msg.find ('[',0);
	if (pos == string::npos)
	{
		// not a valid message.. sending it as an info
		msg_type = 'I';
	}
	else if ( (pos = msg.find ('[',pos+1)) == string::npos)
	{
		// not a valid message.. sending it as an info
		msg_type = 'I';
	}
	else
	{
		// have found both the '[' expected
		msg_type = msg[pos+1]; // it will be 'I', 'D', 'P', 'E' or 'W'

		if (isValid)
		{
			// VEN-1875
			// this is a valid message, update the flag accordingly
			*isValid = true;
		}
	}

	switch (msg_type)
	{
		case 'W':
		{
			ACS_SMARTPTR<exFMessage> WMsg (new exFWarning() );
			*WMsg << msg;
			return WMsg;
		}
		case 'E':
		{
			ACS_SMARTPTR<exFMessage> EMsg (new exFError() );
			*EMsg << msg;
			return EMsg;
		}
		default:
		{
			ACS_SMARTPTR<exFMessage> MMsg (new exFMessage() );
			*MMsg << msg;
			return MMsg;
		}
	}
}
_ACS_END_NAMESPACE
