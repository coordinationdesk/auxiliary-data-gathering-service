// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer Daemon $

	$Id$

	$Author$

	$Log$
	Revision 2.4  2015/12/01 17:44:45  marpas
	coding best practice application in progress
	
	Revision 2.3  2015/11/27 17:38:06  marpas
	coding best practice applicationin progress
	
	Revision 2.2  2015/04/30 15:49:17  marfav
	Compiler warning removed
	
	Revision 2.1  2012/02/09 17:26:24  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.1  2003/10/03 11:39:22  marfav
	Import SignalGuard
	


*/


#ifndef SIGNALGUARD_H
#define SIGNALGUARD_H

#include <exException.h>
#include <csignal>

_ACS_BEGIN_NAMESPACE(acs)

// This class is not thread-safe
// It is designed to mask and buffer the SIGINT and SIGTERM signals
// in order to raise them back on demand

class SignalGuard // PRQA S 2109
{
public:
	// Class ctor. Set the signal mask.
	SignalGuard ();
	// Class dtor. Raise the pending signals and unset the signal mask.
	~SignalGuard() throw() ;

	// Set the signal mask for SIGINT and SIGTERM
	void mask();

	// Unset the signal mask and raise the pending signals
	void unmask();

	// Unset the mask, raise the pending signals and set the mask again
	void flush(); // PRQA S 2502

private:
	void raiseSignals (sigset_t pendingSig);
	SignalGuard (const SignalGuard &); // not implemented
	SignalGuard& operator= (const SignalGuard &); // not implemented

private:
	sigset_t _oldSet;
	sigset_t _newSet;
	bool _masked;
	ACS_CLASS_DECLARE_DEBUG_LEVEL (SignalGuard)
};
_ACS_END_NAMESPACE

#endif // SIGNALGUARD_H

