// PRQA S 1050 EOF
/*
	Copyright 2020-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	
*/ 

#include <TarAlgo.h>

#include <SpawnProc.h>					
#include <File.h>
#include <Filterables.h>

#include <math.h>
 
_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace {
	const double defaultThresholdFactor = 10.0;
	const int defaultBlockingFactor = 20;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(TarAlgo)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TarAlgo::TarAlgo(const map<string, string> &archiveCustomOptions, const map<string, string> &extractCustomOptions):
		PackagingAlgo(archiveCustomOptions, extractCustomOptions) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	
	
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		map<string, string> archiveCustomOptions = getArchiveCustomOptions() ;
		if( archiveCustomOptions.size() != 0 ) {
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << " archive custom options map" );
			for (map<string,string>::const_iterator it=archiveCustomOptions.begin(); it!=archiveCustomOptions.end(); ++it) {
				ACS_LOG_DEBUG(SimpleDebugSignatureThread << it->first << " => " << it->second  );
			}
		}
		
		map<string, string> extractCustomOptions = getExtractCustomOptions() ;
		if( extractCustomOptions.size() != 0 ) {
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << " extract custom options map" );
			for (map<string,string>::const_iterator it=extractCustomOptions.begin(); it!=extractCustomOptions.end(); ++it) {
				ACS_LOG_DEBUG(SimpleDebugSignatureThread << it->first << " => " << it->second  );
			}
		}
	ACS_CLASS_END_DEBUG
	
		
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TarAlgo::~TarAlgo() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);		// PRQA S 4631	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void TarAlgo::pack( const string & itemPath, const string & packagePath, int computationTimeout, const long long int itemSize )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	doWork("-cf", packagePath, itemPath, false, computationTimeout, itemSize) ;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TarAlgo::unpack(  const string & packagePath, const string & itemPath, int computationTimeout  )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	doWork("-pmxf", packagePath, itemPath, true, computationTimeout) ;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

long long int TarAlgo::estimatePackageSize( const long long int itemSize )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	map<string, string> customOptions = getArchiveCustomOptions() ;
	int blockingFactor ;
	checkBlockingFactor( itemSize, customOptions, blockingFactor ) ;
	int blockSize = blockingFactor * 512;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "block size=" << blockSize << " bytes");
	
	long long int packageSize = ceil( ( double(itemSize) + 1.) / blockSize ) * blockSize ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "original size=" << itemSize << " bytes; estimated package size=" << packageSize << " bytes");
	return packageSize;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TarAlgo::doWork(const string & options, const string & packagePath, const string & itemPath,  bool extract, int computationTimeout, long long int itemSize )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);		// PRQA S 4631
	
	const string command("tar");
	SpawnProc proc("tar") ;

	string filePathToWorkOn = itemPath ; 

	size_t i = 0;
	
	if( extract ) {
		map<string, string> extractCustomOptions = getExtractCustomOptions() ;
		for (map<string,string>::const_iterator it=extractCustomOptions.begin(); it!=extractCustomOptions.end(); ++it) {
			proc.args()[i++] = it->first ;
			if( not it->second.empty() ) {
				proc.args()[i++] = it->second ;
			}
		}
	}
	else {
		
		map<string, string> archiveCustomOptions = getArchiveCustomOptions() ;
		if( archiveCustomOptions.size() != 0 ) {
			int blockingFactor ;
			checkBlockingFactor( itemSize, archiveCustomOptions, blockingFactor );
			for (map<string,string>::const_iterator it=archiveCustomOptions.begin(); it!=archiveCustomOptions.end(); ++it) {
				proc.args()[i++] = it->first ;
				if( not it->second.empty() ) {
					proc.args()[i++] = it->second ;
				}
			}
		}
		
	}
	proc.args()[i++] = options ;
	proc.args()[i++] = packagePath ;
	if( extract ) {
		proc.args()[i++] = "-C"; 
		filePathToWorkOn = packagePath ;
	}
	proc.args()[i++] = itemPath ;
	
	executeSpawn(proc, command, filePathToWorkOn, computationTimeout) ;
	
}


void TarAlgo::checkBlockingFactor(const long long int itemSize, map<string, string> & customOptions, int & blockingFactor )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);		// PRQA S 4631

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << " input custom options map" );
		for (map<string,string>::const_iterator it=customOptions.begin(); it!=customOptions.end(); ++it) {
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << it->first << " => " << it->second  );
		}
	ACS_CLASS_END_DEBUG

	double thresholdFactor = defaultThresholdFactor ;	
	map<string, string>::const_iterator it = customOptions.find("thresholdFactor") ;
	if( it != customOptions.end() ) {
		// check it is a number
		istringstream is(it->second);
		is >> thresholdFactor;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Threshold factor found in options: " << thresholdFactor);
		customOptions.erase (it);
	} 
	else { //just for debug
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Threshold factor not found in options: using default (" << thresholdFactor << ')');
	}

	blockingFactor = defaultBlockingFactor ;
	it = customOptions.find("-b") ;
	if( it != customOptions.end()  ) {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Blocking factor custom option found" );
		
		int customBlockingFactor ;
		{
			// check it is a number
			istringstream is(it->second);
			is >> customBlockingFactor;
		}
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Custom blocking factor=" << customBlockingFactor);

		double sizeThreshold = thresholdFactor * customBlockingFactor * 512;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Item size=" << itemSize << ", size threshold=" << sizeThreshold);
		if( double(itemSize) >= sizeThreshold ) {
			blockingFactor = customBlockingFactor ;
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Item size major than threshold: using custom blocking factor (" << blockingFactor << ')');
		}
		else { 
			customOptions.erase (it);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Item size minor than threshold: using default blocking factor");
		}
			
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Blocking factor option not found; returning default: " << blockingFactor);
	}

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << " output custom options map" );
		for (map<string,string>::const_iterator it=customOptions.begin(); it!=customOptions.end(); ++it) {
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << it->first << " => " << it->second  );
		}
	ACS_CLASS_END_DEBUG
			
}

_ACS_END_NAMESPACE
