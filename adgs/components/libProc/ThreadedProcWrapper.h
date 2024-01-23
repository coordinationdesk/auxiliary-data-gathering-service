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

	$Prod: A.C.S. Process library $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2013/07/24 10:30:22  marfav
	Added ThreadedProcWrapper class
	

*/

#ifndef _THREADEDPROCWRAPPER_H_
#define _THREADEDPROCWRAPPER_H_

#include <exMacros.h>
#include <Thread.h>

_ACS_BEGIN_NAMESPACE(acs)

class SpawnProc;

class ThreadedProcWrapper : public Thread
{

public:
	// CTOR and DOTR
	explicit ThreadedProcWrapper(SpawnProc*);
	virtual ~ThreadedProcWrapper() throw();

	// Copy CTOR and Assignment
	ThreadedProcWrapper (const ThreadedProcWrapper&);
	ThreadedProcWrapper& operator= (const ThreadedProcWrapper&);

	// Thread implementation
	virtual void run();

private:
    ThreadedProcWrapper() ; // not implemented
	// Attributes
	SpawnProc* _spawnProc;

	// Debug level declaration
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ThreadedProcWrapper);

};


_ACS_END_NAMESPACE

#endif //_THREADEDPROCWRAPPER_H_

