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

#include <TarAlgo.h>

#include <Timer.h>
#include <SpawnProc.h>					
#include <Dir.h>

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

TarAlgo::TarAlgo(const map<string, string> &archiveCustomOptions, const map<string, string> &extractCustomOptions, const PackagingAlgo::FilesChangeFixStrategy & filesChangeFixStrategy):
		PackagingAlgo(archiveCustomOptions, extractCustomOptions, filesChangeFixStrategy) 
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
		
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << " files change fix strategy\n" << filesChangeFixStrategy ) ;
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
	
	string filePathToWorkOn = itemPath ; 

	map< string, time_t > modificationTimeMap ;
	FilesChangeFixStrategy filesChangeFixStrategy = getFilesChangeFixStrategy() ;
	bool modificationTimeMapValid = false ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Files change fix strategy\n" << filesChangeFixStrategy ) ;
	vector<string> tarArgs ;
	if( extract ) {
		map<string, string> extractCustomOptions = getExtractCustomOptions() ;
		for (map<string,string>::const_iterator it=extractCustomOptions.begin(); it!=extractCustomOptions.end(); ++it) {
			tarArgs.push_back(it->first) ;
			if( not it->second.empty() ) {
				tarArgs.push_back(it->second) ;
			}
		}
	}
	else {
		
		map<string, string> archiveCustomOptions = getArchiveCustomOptions() ;
		if( archiveCustomOptions.size() != 0 ) {
			int blockingFactor ;
			checkBlockingFactor( itemSize, archiveCustomOptions, blockingFactor );
			for (map<string,string>::const_iterator it=archiveCustomOptions.begin(); it!=archiveCustomOptions.end(); ++it) {
				tarArgs.push_back(it->first) ;
				if( not it->second.empty() ) {
					tarArgs.push_back(it->second) ;
				}
			}
		}
		
		if( filesChangeFixStrategy.ignoreAttributesChange ) { // Navigate itemPath if it is dir and get modification time of all items
			fillModificationTimesMap( itemPath, modificationTimeMap, modificationTimeMapValid ) ;
			if( not modificationTimeMapValid ) {
				ACS_LOG_WARNING( "Items modification times map cannot be used!" ) ;
			}
			ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
				if( modificationTimeMapValid ) {
					map< string , time_t>::const_iterator it ;
					for (it=modificationTimeMap.begin(); it!=modificationTimeMap.end(); ++it) {
 						ACS_LOG_DEBUG(SimpleDebugSignatureThread << '\"' << it->first << "\" -> " << it->second );
					}
				}
			ACS_CLASS_END_DEBUG
					
		}
		
		
	}
	tarArgs.push_back(options) ;
	tarArgs.push_back(packagePath) ;
	if( extract ) {
		tarArgs.push_back("-C") ;
		filePathToWorkOn = packagePath ;
	}
	tarArgs.push_back(itemPath) ;
	
	executeTar( tarArgs, packagePath, filePathToWorkOn, computationTimeout, extract, filesChangeFixStrategy, modificationTimeMap, modificationTimeMapValid ) ;
	
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


void TarAlgo::fillModificationTimesMap( const string & itemPath, map< string, time_t > & outMap, bool & valid) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);		// PRQA S 4631
	
	if( File::isDir(itemPath) ) {
		DirTree dirTree(itemPath, "/", true);
		size_t dirTreeSize = dirTree.size() ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, dirTreeSize << " elements for \"" << itemPath << "\" directory" ) ;
		for (size_t i = 0 ; i < dirTreeSize; ++i) {
			try {
				time_t modificationTime = File::getModificationTime(  dirTree[i], & valid ) ;
				if( not valid ) {
					ACS_LOG_WARNING("Cannot compute modification time on \"" << dirTree[i] << "\" path" ) ;
					break ;
				}
				outMap.insert ( pair<string, time_t>(dirTree[i], modificationTime) ); 
			}
			catch ( exception &e ) {
				ACS_LOG_WARNING("Cannot compute modification time on \"" << dirTree[i] << "\" path: " << e.what() ) ;
				valid = false ;
				break ;
			}
			
		}
		
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << itemPath << "\" item is a file" ) ;
		try {
			time_t modificationTime = File::getModificationTime(  itemPath, & valid ) ;
			if( not valid ) {
				ACS_LOG_WARNING("Cannot compute modification time on \"" << itemPath << "\" path" ) ;
			} 
			outMap.insert ( pair<string, time_t>(itemPath, modificationTime) ); 
		}
		catch ( exception &e ) {
			ACS_LOG_WARNING("Cannot compute modification time on \"" << itemPath << "\" path: " << e.what() ) ;
			valid = false ;
		}
	}
	
}

void TarAlgo::executeTar( 	const vector<string> & tarArgs,
							const string & packagePath ,
							const string & filePathToWorkOn, 
							int computationTimeout, 
							bool extract,
							const FilesChangeFixStrategy & filesChangeFixStrategy, 
							const map< string, time_t > & timeMap, 
							bool timeMapValid )
{
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);		// PRQA S 4631
	
	int retryAttempts = 0 ;
	bool retry = true ;
	do {
		const string command("tar");
		SpawnProc proc(command) ;
		size_t tarArgsSize = tarArgs.size() ;
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Going to pass the following arguments to \"" << command << "\" command" ) ;
		for( size_t i = 0 ; i < tarArgsSize ; ++i ) {
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "arg[" << i << "]=" << tarArgs[i] ) ;
			proc.args()[i] = tarArgs[i] ;
			
		}
		
		string outmsg ; 
		int retCode = executeSpawn(proc, command, filePathToWorkOn, computationTimeout, outmsg) ;
		if(	0 == retCode ) { // tar successful
			retry = false ;
			if( 0 == retryAttempts ) {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, outmsg) ;
			}
			else {
				ACS_LOG_INFO(outmsg) ;
			}
		}
		else if( 1 == retCode ) { // some files were changed while being archived: if properly configured check the reason and/or just retry.
			ACS_COND_THROW( extract, PackagingAlgoException( outmsg ) ) ;

			bool validTar = false;
			if( filesChangeFixStrategy.ignoreAttributesChange && timeMapValid ) {
				ACS_LOG_WARNING( outmsg << " Going to check items modification time" ) ;
				validTar = filesAttributeChanged( timeMap ) ;
			}

			if( validTar ) {
				ACS_LOG_INFO("Files changed since attributes only modified: \"" << packagePath << "\" tar is valid" ) ;
				retry = false ;
			}
			else {
				// check number of attempts and eventually wait to reiterate the command
				if( retryAttempts >= filesChangeFixStrategy.retryParameters.maxNumber ) {
					if( 0 == filesChangeFixStrategy.retryParameters.maxNumber ) {
						ACS_THROW( PackagingAlgoException( outmsg ) ) ;
					}
					ostringstream exMsg ;
					exMsg << outmsg << " Maximum number of retry attemps " << filesChangeFixStrategy.retryParameters.maxNumber << " exceeded. Tar command definetely failed" ;
					ACS_THROW( PackagingAlgoException( exMsg.str() ) ) ;
				}
				else {
					ACS_LOG_ERROR( outmsg << " Retry attempts=" << retryAttempts << ", max number=" << filesChangeFixStrategy.retryParameters.maxNumber 
									<< "; going to wait " << filesChangeFixStrategy.retryParameters.waitingPeriod << " ms before next attempt" );
					++retryAttempts ;
					File::unlink(packagePath) ;
					Timer::delay(filesChangeFixStrategy.retryParameters.waitingPeriod);
					retry = true ; 
				}

			}

		}
		else {
			ACS_THROW( PackagingAlgoException( outmsg ) ) ;
		}
	}
	while( retry ) ;
}


bool TarAlgo::filesAttributeChanged( const map< string, time_t > & timeMap ) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);		// PRQA S 4631

	bool attributesChanged = true ;
	
	map< string , time_t>::const_iterator it ;
	for (it = timeMap.begin(); it != timeMap.end(); ++it) {
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, '\"' << it->first << "\" -> " << it->second );
		if( File::exists( it->first ) ) {
			try {
				bool valid = false ;
				time_t modificationTime = File::getModificationTime(  it->first, & valid ) ;
				if( not valid ) {
					ACS_LOG_WARNING("Cannot compute modification time on \"" << it->first << "\" path" ) ;
					attributesChanged = false ;
					break ;
				} 
				if( it->second != modificationTime ) {
					ACS_LOG_WARNING('\"' << it->first << "\" entry: before tar modification time was " << it->second << " after tar is " << modificationTime << ": content modified" ) ;
					attributesChanged = false ;
					break ;
				}
				else { // just for debug
					ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, '\"' << it->first << "\" entry content not modified") ;
				}
			}
			catch ( exception &e ) {
				ACS_LOG_WARNING("Cannot compute modification time on \"" << it->first << "\" path: " << e.what() ) ;
				attributesChanged = false ;
				break ;
			}
			
		}
		else {
			ACS_LOG_WARNING('\"' << it->first << "\" path exists no more (before tar execution it did exist)!" ) ;
			attributesChanged = false ;
			break ;
		}
 	}
	
	return attributesChanged ;	
}
_ACS_END_NAMESPACE
