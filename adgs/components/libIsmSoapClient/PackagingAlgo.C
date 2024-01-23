// PRQA S 1050 EOF
/*
	Copyright 2020, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.
	
*/ 

#include <PackagingAlgo.h>
			
#include <SpawnProc.h>
#include <Timer.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(PackagingAlgo)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PackagingAlgo::PackagingAlgo(const map<string, string> &archiveCustomOptions, const map<string, string> &extractCustomOptions):
							_archiveCustomOptions(archiveCustomOptions),
							_extractCustomOptions(extractCustomOptions)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);			
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PackagingAlgo::~PackagingAlgo() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);		// PRQA S 4631	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void PackagingAlgo::executeSpawn(SpawnProc & proc, const string & command, const string & pathToWorkOn, int computationTimeout)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);		// PRQA S 4631	
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to execute \"" << command << "\" command on \"" << pathToWorkOn << "\"; timeout=" << computationTimeout << " s" );
	
	proc.spawn();

	Timer watchdog ;
	bool wdTimeout = false ;
	watchdog.start() ;

	unsigned long int timeout_ms = computationTimeout * 1000;
	while(proc.isRunning()) {
		Timer::delay(100); // PRQA S 4400
		if(watchdog.isOlder(timeout_ms)) { // PRQA S 3000, 4400
			proc.kill(); // it took more than timeout: kill it
			wdTimeout = true ;
		}
	}

	int exitCode = -1 ;
	bool goodExit=true;
	if( !proc.exitCode(exitCode) ) {
		goodExit=false;
    }

	if(exitCode != 0) {
		ostringstream os ;
		if (wdTimeout) {
			os << watchdog.elapsed() / 1000 // PRQA S 3084, 4400
			   << " [sec] elapsed since " << command << " was started on \"" << pathToWorkOn << "\" path. It is bigger than the timeout (" << computationTimeout
				<< " [sec]). The process was killed. " ;
			ACS_THROW(PackagingAlgoException(os.str())) ; // PRQA S 3081
		}

		os << command << " command on \"" << pathToWorkOn << "\" path failed; exitCode: " << exitCode;
		int exitSignal = 0 ;
		proc.exitSignal(exitSignal) ;
		if( 0 != exitSignal ) {
			os << " exitSignal: " << exitSignal ;
		}
		ACS_THROW(PackagingAlgoException(os.str())) ; // PRQA S 3081
	}
	else if( not goodExit ) {
		ostringstream exMsg;
		exMsg << command << " command on \"" << pathToWorkOn << "\" path not terminated";
		ACS_THROW(PackagingAlgoException( exMsg.str() ));
	}
	else { // just for debug
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, command << " command on \"" << pathToWorkOn << "\" path successfull");
	}

}


_ACS_END_NAMESPACE
