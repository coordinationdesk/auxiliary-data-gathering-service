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

	$Prod: A.C.S. ThinLayer Scheduler Library $

	$Id$

	$Author$

	$Log$
	Revision 2.2  2015/07/16 08:54:34  marfav
	APF-285 adding support to warning exit status for the tasks
	
	Revision 2.1  2013/12/16 19:31:41  marpas
	coding best practices applied
	
	Revision 2.0  2006/02/28 10:17:14  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2004/12/03 17:58:52  marfav
	Header fixed
	
	Revision 1.2  2004/04/07 13:11:31  marfav
	Added support for IPC
	Does not use ACE anymore
	using exStream.h
	
	Revision 1.1  2004/03/11 18:45:44  marfav
	Importing Scheduler Engine in Library
	

*/


#ifndef _SchedulerExitStatus_H_
#define _SchedulerExitStatus_H_

#include <acs_c++config.hpp>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)


enum SchedulerExitStatus
{
  ssKILLED = -1,
  ssOK = 0,
  // In case some tasks returned an exit code in the range 1..127 
  // that means execution is OK but with product potentially incomplete
  ssINTERNAL_ERR,
  ssNO_ORDER,
  ssNO_SPACE,
  ssNO_WDIR,
  ssTASK_FAILED,
  ssTASK_CRASHED,
  ssABORTED,
  ssSTOPPED,

  // The next exit status are used internally by the ThreadedScheduler and SpawnedScheduler classes
  // ssNOTSTARTED means that the Scheduler has never started the execution
  ssNOTSTARTED,
  // ssUNDEFINED means that the Scheduler has been started and it is not still running but has not
  // produced any exit code.
  // It is used in the ThreadedScheduler class only and it is returned when there is an exception
  // that caused unexpected exit from the run.
  ssUNDEFINED
};

enum SchedulerStatus
{
   schedulerIDLE,
   schedulerRUNNING,
   schedulerSUSPENDED,
   schedulerSTOPPING,
   schedulerABORTING,
   schedulerCOMPLETED
};

std::string schedulerStatus2String (SchedulerStatus s);
std::string schedulerExitStatus2String (SchedulerExitStatus s);

_ACS_END_NAMESPACE

#endif //_SchedulerExitStatus_H_
