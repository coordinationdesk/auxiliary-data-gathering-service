/*

	Copyright 1995-2002, Advanced Computer Systems , Inc.
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
	Revision 1.4  2012/06/07 09:28:21  marfav
	Code improved removing compiler warnings
	
	Revision 1.3  2004/12/03 17:58:10  marfav
	Header fixed
	
	Revision 1.2  2004/04/05 13:30:08  marfav
	using exStream.h
	
	Revision 1.1.1.1  2003/07/16 09:44:32  marfav
	Import libSchedMessages
	
	

*/

#include <MsgSplitter.h>

#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(MsgSplitter)

MsgSplitter::MsgSplitter (const string& m): _messages (m), _parsedMessages()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MsgSplitter::Ctor... calling ParseMessage - the string is [" << _messages << "]") ;

	ParseMessages();
}

MsgSplitter::~MsgSplitter()
{
}

MsgSplitter::MsgSplitter(const MsgSplitter& a) :
    _messages (a._messages),
    _parsedMessages (a._parsedMessages)
{
}

MsgSplitter&
MsgSplitter::operator = (const MsgSplitter& a)
{
  if (this != &a)
  {
	_messages = a._messages;
    _parsedMessages = a._parsedMessages;
  }
  return *this;
}


void
MsgSplitter::ParseMessages()
{
	size_t firstIndex, lastIndex;
	_parsedMessages.clear();

	firstIndex = lastIndex = 0;

	while ((lastIndex = _messages.find('\n',firstIndex)) != string::npos  )
	{
		string temp = _messages.substr (firstIndex, lastIndex-firstIndex);

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "ParseMessage inserting message [" << temp << "]") ;

		// do not push back empty messages
		if (temp.size() > 1) _parsedMessages.push_back (temp);
		firstIndex = lastIndex+1;
	}
}

const vector<string> & 
MsgSplitter::getParsedMessages() const
{
    return _parsedMessages;
}
_ACS_END_NAMESPACE




