/*
	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libProc$

	$Id$

	$Author$

	$Log$
	Revision 2.1  2018/03/07 10:41:37  nicvac
	Spawn Proc with logs. Executor of pool of Spawn Proc.
	
        
*/

#ifndef _SpawnProcExecutor_H_
#define _SpawnProcExecutor_H_

#include <exException.h>

#include <SpawnProc.h>

_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* Utility to execute one or more (parallel) SpawnProc.
**/

class SpawnProcExecutor  { 
public:
	
	/** Run a set of commands (parallel) */
	static void execute( const std::vector< SpawnProc* > & procs, const std::vector<int>& allowedExitCodes = std::vector<int>(1,0) );

	/** Run a command with logs */
	static void execute( SpawnProc& proc, const std::vector<int>& allowedExitCodes = std::vector<int>(1,0) );

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SpawnProcExecutor)

};

_ACS_END_NAMESPACE

#endif //_SpawnProcExecutor_H_

