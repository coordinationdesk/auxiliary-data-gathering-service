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


#include <SchedulerExitStatus.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


string schedulerStatus2String (SchedulerStatus s) // PRQA S 4020
{
	switch (s)
	{
	case schedulerIDLE: { return "schedulerIDLE"; }
	case schedulerRUNNING: { return "schedulerRUNNING"; }
	case schedulerSUSPENDED: { return "schedulerSUSPENDED"; }
	case schedulerSTOPPING: { return "schedulerSTOPPING"; }
	case schedulerABORTING: { return "schedulerABORTING"; }
	case schedulerCOMPLETED: { return "schedulerCOMPLETED"; }
	default: { return "schedulerUNKNOWN"; }
	}
}


string schedulerExitStatus2String (SchedulerExitStatus s) // PRQA S 4020
{
	switch (s)
	{
	case ssKILLED: { return "ssKILLED"; }
	case ssOK: { return "ssOK"; }
	case ssINTERNAL_ERR: { return "ssINTERNAL_ERR"; }
	case ssNO_ORDER: { return "ssNO_ORDER"; }
	case ssNO_SPACE: { return "ssNO_SPACE"; }
	case ssNO_WDIR: { return "ssNO_WDIR"; }
	case ssTASK_FAILED: { return "ssTASK_FAILED"; }
	case ssTASK_CRASHED: { return "ssTASK_CRASHED"; }
	case ssABORTED: { return "ssABORTED"; }
	case ssSTOPPED: { return "ssSTOPPED"; }
	case ssNOTSTARTED: { return "ssNOTSTARTED"; }
	case ssUNDEFINED: { return "ssUNDEFINED"; }
	default: { return "ssUNKNOWN"; }
	}
}




_ACS_END_NAMESPACE
