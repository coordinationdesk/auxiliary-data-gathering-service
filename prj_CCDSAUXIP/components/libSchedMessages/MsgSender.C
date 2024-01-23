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
	Revision 1.8  2012/06/07 09:28:21  marfav
	Code improved removing compiler warnings
	
	Revision 1.7  2010/09/07 10:14:37  marfav
	VEN-1875
	Messages not compliant to GEN_PROC_ICD can be filtered and
	not forwarded to MCF
	
	Revision 1.6  2005/07/04 10:48:22  marfav
	excmcf notification is configurable via configuration singleton
	
	Revision 1.5  2005/05/11 08:37:34  marfav
	Messages are sent via excmcf instead of excerr
	
	Revision 1.4  2004/12/03 17:58:10  marfav
	Header fixed
	
	Revision 1.3  2004/04/05 13:30:08  marfav
	using exStream.h
	
	Revision 1.2  2003/07/16 15:07:48  marfav
	Public interface modified
	
	Revision 1.1.1.1  2003/07/16 09:44:32  marfav
	Import libSchedMessages
	
	

*/

#include <scmsgV.h>

#include <MsgSender.h>
#include <MsgSplitter.h>
#include <MsgFilterableWrp.h>

#include <exException.h>
#include <ConfigurationSingleton.h>
#include <rsResourceSet.h>


using namespace std;
using namespace acs;

static scmsgV version;

static const char* _mcfLogEnableKey = "TlScheduler.Enable_MCF_Notification";

// This field is defaulted to TRUE to keep back compatibility
// if set to false the messages that are not well formatted 
// will not be forwarded to MCF
static const char* _mcfForwardEverythingKey = "TlScheduler.MCF_Forward_Everything";

MsgSender::MsgSender (const string& m) : _messages(m), _notifyMcf(false), _mcfForwardAll (true)
{
  init();
}

MsgSender::~MsgSender()
{
}

MsgSender::MsgSender(const MsgSender& aMsgSender):
		_messages (aMsgSender._messages),
		_notifyMcf (aMsgSender._notifyMcf),
		_mcfForwardAll (aMsgSender._mcfForwardAll)
{
}

MsgSender&
MsgSender::operator = (const MsgSender& aMsgSender)
{
  if (this != &aMsgSender)
  {
	_messages  = aMsgSender._messages;
	_notifyMcf = aMsgSender._notifyMcf;
	_mcfForwardAll = aMsgSender._mcfForwardAll;
  }
  return *this;
}

void
MsgSender::init()
{
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();

	// Check the availability of the configuration
	// about the MCF notification feature 
	if (rs.hasKey (_mcfLogEnableKey))
	{
		rs.getValue(_mcfLogEnableKey, _notifyMcf);
	}
	else
	{
		// no key... disable the MCF stream
		_notifyMcf = false;
	}

	// VEN-1875
	// Check the availability of the configuration
	// about the need to forward everything or only
	// the well formatted messages to MCF
	if (rs.hasKey (_mcfForwardEverythingKey))
	{
		rs.getValue(_mcfForwardEverythingKey, _mcfForwardAll);
	}
	else
	{
		// no key... forward everything to MCF 
		_mcfForwardAll = true;
	}

}

void
MsgSender::insertMessage (const string& m, bool endline)
{
	append(m);
	if (endline) append("\n");
}

void
MsgSender::sendMessage (const string& m)
{
	// force a '\n' at the end of the message
	// insert the message in the queue and then send all the stored messages
	insertMessage (m, true);
	flush();
}

void
MsgSender::flush ()
{
	MsgFilterableWrp wrapper;
	MsgSplitter mS (_messages);
	if (_notifyMcf)
	{
		bool validMessage;

		for (vector<string>::const_iterator i = mS.getParsedMessages().begin(); i != mS.getParsedMessages().end(); i++)
		{
			ACS_SMARTPTR<exFMessage> theMsg (wrapper.getFilterable (*i, &validMessage));

			// VEN-1875
			// Notify the message only if well formatted or 
			// if MCF can acceppt every kind of messages
			if ( _mcfForwardAll || validMessage )
			{
				excmcf.notify (*theMsg);
			}
		}
	}
	_messages = "";
}

void
MsgSender::append (const string& s)
{
	_messages.append (s);
}

