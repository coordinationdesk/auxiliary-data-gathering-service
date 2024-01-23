// PRQA S 1050 EOF
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
	Revision 2.2  2018/03/07 12:11:04  nicvac
	QA check
	
	Revision 2.1  2018/03/07 10:41:37  nicvac
	Spawn Proc with logs. Executor of pool of Spawn Proc.
	
        
*/ 

#include <SpawnProcExecutor.h>

#include <Filterables.h>

#include <algorithm>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(SpawnProcExecutor)


void SpawnProcExecutor::execute( const vector< SpawnProc* > & procs, const vector<int>& allowedExitCodes ) {

	for ( size_t i(0); i<procs.size(); ++i ) {
		SpawnProc* sp = procs.at(i);
		if ( sp ) {
			sp->spawn();
		}
	}

	for ( size_t i(0); i<procs.size(); ++i ) {
		SpawnProc* sp = procs.at(i);
		if ( sp ) {
			sp->waitProcCompletion();
		}
	}


	for ( size_t i(0); i<procs.size(); ++i ) {

		SpawnProc* sp = procs.at(i);
		if ( sp ) {
			const string& tagName = sp->getTagName();

			int exitCode(0);
			bool validExitCode = sp->exitCode(exitCode);

			ACS_LOG_PRIVINFO( "Process ["<<tagName<<"] executed. validExitCode: ["<<validExitCode<<"]; exitCode: ["<<exitCode<<"]");

			vector<int>::const_iterator it = find( allowedExitCodes.begin(), allowedExitCodes.end(), exitCode );
			bool exitCodeIsAllowed = ( it != allowedExitCodes.end() );

			ACS_COND_THROW( ! (validExitCode && exitCodeIsAllowed), // PRQA S 3081
					exCriticalException(" Failed. Error executing Process ["+tagName+"].") );
		}

	}

}

void SpawnProcExecutor::execute( SpawnProc& proc, const vector<int>& allowedExitCodes ) {

	vector< SpawnProc* > procs;
	procs.push_back( &proc );

	SpawnProcExecutor::execute( procs, allowedExitCodes );

}



_ACS_END_NAMESPACE

