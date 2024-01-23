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
	Revision 1.3  2012/06/07 09:28:21  marfav
	Code improved removing compiler warnings
	
	Revision 1.2  2004/12/03 17:58:10  marfav
	Header fixed
	
	Revision 1.1.1.1  2003/07/16 09:44:32  marfav
	Import libSchedMessages
	
	

*/

#ifndef _MSGSPLITTER_H_
#define _MSGSPLITTER_H_

#include <string>
#include <vector>

#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)
class MsgSplitter //: public std::vector<std::string>
{
	public:
		MsgSplitter (const std::string& m = "");
		~MsgSplitter ();

		MsgSplitter (const MsgSplitter &) ;
		MsgSplitter &operator=(const MsgSplitter &) ;
        
        const std::vector <std::string> & getParsedMessages() const;


	private:
		void ParseMessages();
		std::string _messages;
        std::vector<std::string> _parsedMessages;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(MsgSplitter)
};

_ACS_END_NAMESPACE

#endif // _MSGSPLITTER_H_
