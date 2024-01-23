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
	Revision 1.6  2012/06/07 09:28:21  marfav
	Code improved removing compiler warnings
	
	Revision 1.5  2010/09/07 10:14:37  marfav
	VEN-1875
	Messages not compliant to GEN_PROC_ICD can be filtered and
	not forwarded to MCF
	
	Revision 1.4  2005/07/04 10:48:22  marfav
	excmcf notification is configurable via configuration singleton
	
	Revision 1.3  2004/12/03 17:58:10  marfav
	Header fixed
	
	Revision 1.2  2003/07/16 15:07:48  marfav
	Public interface modified
	
	Revision 1.1.1.1  2003/07/16 09:44:32  marfav
	Import libSchedMessages
	
	

*/

#ifndef _MSGSENDER_H_
#define _MSGSENDER_H_

#include <string>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

class MsgSender
{
	public:
		MsgSender (const std::string& messages = "");
		~MsgSender();

		MsgSender (const MsgSender &) ;
		MsgSender &operator=(const MsgSender &) ;

		void insertMessage (const std::string&, bool endline = true);
		void sendMessage (const std::string& m = "");

	private:
		void init();
		void flush();
		void append (const std::string&);
        
    private: // attributes
		std::string _messages;
		bool _notifyMcf;
		bool _mcfForwardAll;
};

_ACS_END_NAMESPACE

#endif // _MSGSENDER_H_
