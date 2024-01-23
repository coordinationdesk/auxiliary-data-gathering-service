// PRQA S 1050 EOF
/*
	Copyright 2020-2022, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA, Inc.
	
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

PackagingAlgo::RetryParameters::RetryParameters() :
    maxNumber(0),
    waitingPeriod(0)
{
}


PackagingAlgo::RetryParameters::RetryParameters(PackagingAlgo::RetryParameters const &o):
	maxNumber(o.maxNumber),
	waitingPeriod(o.waitingPeriod)
{
}
								
PackagingAlgo::RetryParameters & PackagingAlgo::RetryParameters::operator = (PackagingAlgo::RetryParameters const &o)
{
	if( this != &o ) {
		maxNumber=o.maxNumber;
		waitingPeriod=o.waitingPeriod;
	}
	return (*this);
}								


ostream &operator<<(ostream &s, const PackagingAlgo::RetryParameters & v)
{
	s << "****************** PackageAlgoRetry ****************\n";
	s << "\tmaxNumber =" 		<< v.maxNumber 		<< '\n';
	s << "\twaitingPeriod =" 	<< v.waitingPeriod 	<< '\n';
	s << "****************************************************\n";

	return s ; // PRQA S 4028
}

exostream &operator<<(exostream &s, const PackagingAlgo::RetryParameters & v) // PRQA S 4222
{
	s << "****************** PackageAlgoRetry ****************\n";
	s << "\tmaxNumber =" 		<< v.maxNumber 		<< '\n';
	s << "\twaitingPeriod =" 	<< v.waitingPeriod 	<< '\n';
	s << "****************************************************\n";

	return s ; // PRQA S 4028

}

template<>
void rsResourceSet::getValue(const std::string &res, PackagingAlgo::RetryParameters &v) const
{
    Lock lock(_mutex) ;
	
	getValue(res+".maxNumber", v.maxNumber) ;
	getValue(res+".waitingPeriod", v.waitingPeriod) ;

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PackagingAlgo::FilesChangeFixStrategy::FilesChangeFixStrategy() :
    retryParameters(),
    ignoreAttributesChange(false)
{
}


PackagingAlgo::FilesChangeFixStrategy::FilesChangeFixStrategy(PackagingAlgo::FilesChangeFixStrategy const &o):
	retryParameters(o.retryParameters),
	ignoreAttributesChange(o.ignoreAttributesChange)
{
}
								
PackagingAlgo::FilesChangeFixStrategy & PackagingAlgo::FilesChangeFixStrategy::operator = (PackagingAlgo::FilesChangeFixStrategy const &o)
{
	if( this != &o ) {
		retryParameters=o.retryParameters;
		ignoreAttributesChange=o.ignoreAttributesChange;
	}
	return (*this);
}								


ostream &operator<<(ostream &s, const PackagingAlgo::FilesChangeFixStrategy & v)
{
	s << "****************** FilesChangeFixStrategy ****************\n";
	s << v.retryParameters 		<< '\n';
	s << "\tignoreAttributesChange =" << boolalpha << v.ignoreAttributesChange << '\n';
	s << "**********************************************************\n";

	return s ; // PRQA S 4028
}

exostream &operator<<(exostream &s, const PackagingAlgo::FilesChangeFixStrategy & v) // PRQA S 4222
{
	s << "****************** FilesChangeFixStrategy ****************\n";
	s << v.retryParameters 		<< '\n';
	s << "\tignoreAttributesChange =" << boolalpha << v.ignoreAttributesChange << '\n';
	s << "**********************************************************\n";

	return s ; // PRQA S 4028

}

template<>
void rsResourceSet::getValue(const std::string &res, PackagingAlgo::FilesChangeFixStrategy &v) const
{
    Lock lock(_mutex) ;
	
	if( hasKey(res+".retryParameters") ) {
		try {
			PackagingAlgo::RetryParameters tmp ;
			getValue(res+".retryParameters", tmp) ;
			v.retryParameters = tmp ;
		}
		catch (rsResourceSet::NotFoundException & )
		{
			ACS_LOG_WARNING( res << ".retryParameters misconfiguration! Default value will be used" )
		}
	}
	
	getValue(res+".ignoreAttributesChange", v.ignoreAttributesChange, false) ;

} 


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PackagingAlgo::PackagingAlgo(const map<string, string> &archiveCustomOptions, const map<string, string> &extractCustomOptions, const FilesChangeFixStrategy & filesChangeFixStrategy):
							_archiveCustomOptions(archiveCustomOptions),
							_extractCustomOptions(extractCustomOptions),
							_filesChangeFixStrategy(filesChangeFixStrategy)
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


int PackagingAlgo::executeSpawn(SpawnProc & proc, const string & command, const string & pathToWorkOn, int computationTimeout, string & outmsg)
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
		if (wdTimeout) {
			ostringstream exMsg ;
			exMsg << watchdog.elapsed() / 1000 // PRQA S 3084, 4400
			   << " [sec] elapsed since " << command << " was started on \"" << pathToWorkOn << "\" path. It is bigger than the timeout (" << computationTimeout
				<< " [sec]). The process was killed. " ;
			ACS_THROW(PackagingAlgoException(exMsg.str())) ; // PRQA S 3081
		}

		ostringstream wmsg ;
		wmsg << command << " command on \"" << pathToWorkOn << "\" path failed; exitCode: " << exitCode;
		int exitSignal = 0 ;
		proc.exitSignal(exitSignal) ;
		if( 0 != exitSignal ) {
			wmsg << " exitSignal: " << exitSignal ;
		}
		wmsg << '.' ;
		outmsg = wmsg.str() ;
	}
	else if( not goodExit ) { 
		ostringstream exMsg;
		exMsg << command << " command on \"" << pathToWorkOn << "\" path not terminated";
		ACS_THROW(PackagingAlgoException( exMsg.str() ));
	}
	else { // just for debug
		outmsg = command + " command on \"" + pathToWorkOn + "\" path successfull";
	}
	
	return exitCode ;

}


_ACS_END_NAMESPACE
