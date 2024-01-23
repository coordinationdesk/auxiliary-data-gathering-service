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
	Revision 1.4  2011/03/01 16:20:45  marpas
	GCC 4.4.x support
	
	Revision 1.3  2010/09/07 10:14:37  marfav
	VEN-1875
	Messages not compliant to GEN_PROC_ICD can be filtered and
	not forwarded to MCF
	
	Revision 1.2  2004/12/03 17:58:10  marfav
	Header fixed
	
	Revision 1.1.1.1  2003/07/16 09:44:33  marfav
	Import libSchedMessages
	
	

*/

#ifndef _MSGFILTERABLEWRP_H_
#define  _MSGFILTERABLEWRP_H_

#include <Filterables.h>

#include <string>
#include <memory>


_ACS_BEGIN_NAMESPACE(acs)

class MsgFilterableWrp
{
	public:
		MsgFilterableWrp();
		~MsgFilterableWrp();
		// VEN-1875
		// validity boolean flag addedd to detect well formatted messages
		ACS_SMARTPTR<acs::exFMessage> getFilterable (const std::string &, bool* validFlag);
};

_ACS_END_NAMESPACE

#endif //  _MSGFILTERABLEWRP_H_
