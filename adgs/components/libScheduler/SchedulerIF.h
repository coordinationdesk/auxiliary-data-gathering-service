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
	Revision 2.4  2015/04/30 13:04:21  marfav
	Deriving privately from Thread and Proc
	Alignment to libTaskExecution 2.30 signatures
	Operation clashing with base class signatures renamed
	
	Revision 2.3  2013/12/16 19:31:41  marpas
	coding best practices applied
	
	Revision 2.2  2013/07/26 14:04:00  marfav
	waitExitOrTimeout operation exposed to the users
	
	Revision 2.1  2013/07/07 18:33:02  marpas
	application of coding best practices in progress
	some qa warnings and compilation ones removed
	adoption of libException 5.x standards in progress
	
	Revision 2.0  2006/02/28 10:17:14  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2004/12/03 17:58:52  marfav
	Header fixed
	
	Revision 1.4  2004/11/19 14:58:21  fracar
	fixed problems with disk space check and open pipes dangling
	
	Revision 1.3  2004/04/09 10:11:03  marfav
	Disk quota reservation support added
	
	Revision 1.2  2004/04/07 13:11:31  marfav
	Added support for IPC
	Does not use ACE anymore
	using exStream.h
	
	Revision 1.1  2004/03/11 18:45:44  marfav
	Importing Scheduler Engine in Library
	

*/

#ifndef _SCHEDULER_IF_
#define _SCHEDULER_IF_

#include <acs_c++config.hpp>
#include <scOrder.h>
#include <SchedulerExitStatus.h>

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

class SchedulerIF // PRQA S 2109
{
public:
	/*! class SchedulerIFException declaration */
	exDECLARE_EXCEPTION(SchedulerIFException,exException) ; // PRQA S 2131, 2502

public:
	SchedulerIF() {}
	virtual ~SchedulerIF () {}  // PRQA S 2131

	virtual void stop() = 0;
	virtual void abort()= 0; // PRQA S 2502
	virtual void suspend()= 0;
	virtual void resume()= 0;
	virtual void execute (const scOrder& workOrder) = 0;
	virtual void sendCmd (const std::string &cmdValue, const std::string &paramValue) = 0;
	virtual void abortBySigTerm() = 0;
	virtual void doKill () = 0;
	virtual bool isSchedulerRunning() = 0;
	virtual bool getExitStatus (SchedulerExitStatus&) = 0;
	virtual bool timedWaitExit (unsigned long) = 0;

#ifdef SCHEDULER_CHECKS_DISK_SPACE
	virtual size_t getReservedDiskSpaceQuota() const = 0;
	virtual void setReservedDiskSpaceQuota(size_t MB) = 0;
	static const size_t NoReservedQuota = 0;
#endif // SCHEDULER_CHECKS_DISK_SPACE
protected:
	SchedulerIF(const SchedulerIF&) {}
	SchedulerIF& operator= (const SchedulerIF&) { return *this ; } // PRQA S 4072
};

_ACS_END_NAMESPACE

#endif //_SCHEDULER_IF_
