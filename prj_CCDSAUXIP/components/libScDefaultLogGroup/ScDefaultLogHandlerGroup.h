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

	$Prod: A.C.S. Scheduler Default Log Plugin library $

	$Id$

	$Author$

	$Log$
	Revision 1.7  2013/12/16 19:13:18  marpas
	no exceptions from dtors
	
	Revision 1.6  2013/10/18 11:19:52  marpas
	adopting libException 5.16 interfaces
	
	Revision 1.5  2009/06/26 10:48:38  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.4  2004/12/23 18:32:17  marfav
	Added PROCESSORLOG tag to MCF messages
	
	Revision 1.3  2004/12/03 17:54:43  marfav
	Header fixed
	
	Revision 1.2  2004/09/23 15:22:18  marfav
	Log handlers derived from ScLogHandlerBase
	
	Revision 1.1.1.1  2004/09/14 16:36:52  marfav
	Import libScDefaultLogGroup
	

*/


#ifndef _ScDefaultLogHandlerGroup_H_
#define _ScDefaultLogHandlerGroup_H_

#include <ScLogHandlerGroup.h>
#include <ScLogHandlerBase.h>
#include <MsgSender.h>
#include <exException.h>
#include <scOrder.h>
#include <exStream.h>


_ACS_BEGIN_NAMESPACE(acs)

class SchedulerLogFileHandler : public ScLogHandlerBase
{
public:
	SchedulerLogFileHandler(const scOrder&);
	~SchedulerLogFileHandler() throw() ;
	virtual bool computeTaskCompletionPercentage (exFilterable&, float&);
	virtual void userFilterableNotification (exFilterable&);

private:
	SchedulerLogFileHandler (const SchedulerLogFileHandler&);
	SchedulerLogFileHandler& operator= (const SchedulerLogFileHandler&);

private:
	exLogStream* _log;
	MsgSender _theMsgSender;
	std::string _orderId;
	std::string _logOrderSignature;
};


class ScDefaultLogHandlerGroup : public ScLogHandlerGroup
{

public:
	ScDefaultLogHandlerGroup(const scOrder&);
	virtual ~ScDefaultLogHandlerGroup() throw() ;

private:
	ScDefaultLogHandlerGroup (const ScDefaultLogHandlerGroup&);
	ScDefaultLogHandlerGroup& operator= (const ScDefaultLogHandlerGroup&);

private: //attributes
	SchedulerLogFileHandler _scLogHandler;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ScDefaultLogHandlerGroup)
};


_ACS_END_NAMESPACE


#endif //_ScDefaultLogHandlerGroup_H_


