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

	$Prod: A.C.S. ThinLayer PreConditionChecker Library $

	$Id$

	$Author$

	$Log$
	Revision 1.10  2017/01/16 13:25:32  marfav
	Enforcing cpp coding rules
	
	Revision 1.9  2016/04/11 13:30:51  marpas
	adopting new db interfaces
	
	Revision 1.8  2016/03/26 21:40:46  marpas
	using new db I/F
	
	Revision 1.7  2015/11/20 11:57:18  marpas
	coding best practices application in progress
	
	Revision 1.6  2015/06/16 13:09:25  damdec
	Compilation warnings solved.
	
	Revision 1.5  2013/02/21 13:43:56  marfav
	Compiler warnings removed
	
	Revision 1.4  2012/02/10 17:24:44  marpas
	refactoring in progress
	
	Revision 1.3  2010/06/24 08:20:24  marfav
	Checking duplicates in the alternative only if the alternative is in the symbol table
	
	Revision 1.2  2010/03/04 15:14:12  marfav
	Addedd support for alternative filtering based on
	- equal files detection
	- generation time sorting
	
	Revision 1.1.1.1  2008/04/22 13:08:31  marfav
	Importing libTlPreconditionChecker
	
	

*/

#include <Alternative_PCC.h>

#include <dbConnectionPool.h>
#include <dbInventoryObject.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(Alternative_PCC)

Alternative_PCC::Alternative_PCC(const PCC_ParamsPtr& theParameters) : 
	PCC_Base(theParameters),
	_discardMap(),
	_lut(),
	_infoMap()

{
	// Default CTOR
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
}

Alternative_PCC::~Alternative_PCC() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) // PRQA S 4631
}



bool 
Alternative_PCC::exitVisit (InputAlternative& a)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	// before exiting remove the duplicated version of the same files
	// but only if there is something to check

	SymTable::const_iterator it = getParameters()->tempSymbolTable.find(a);

	if ( it != getParameters()->tempSymbolTable.end() )
	{
		if (not getAlternativeValFromTempSymTable(a, true).fileList.empty())
		{
			_discardMap.clear();
			_infoMap.clear();
			_lut.clear();

			searchDiscardableInputs(a);
			createLUT(a);
			updateSymbolTable(a);
		}
	}

	return PCC_Base::exitVisit (a);
}


// STL Helper class finding indexes in the timeline
class timeLineIndexFinder // PRQA S 2109
{
public:
	explicit timeLineIndexFinder (size_t toFind) : _toFind (toFind)
	{}
	
	bool operator () (const PCC_Base::TimelineDetails& d) const throw()
	{
		return (int(_toFind) == d.fileNameIdx); // PRQA S 3081
	}

private:
    timeLineIndexFinder() ; // not implemented 
private:
	size_t _toFind;
};

void 
Alternative_PCC::searchDiscardableInputs (const InputAlternative& a)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	vector<InputFileName> fileList = getAlternativeValFromTempSymTable(a, true).fileList;
	vector<TimelineDetails> fileTimeline = getAlternativeValFromTempSymTable(a, true).fileTimeline;
	
	// Old school double for.. no assumptions made about ordering of the files in the symbol table
	size_t flsize_1= (fileList.size() - 1) ;
    for (size_t idx = 0; idx < flsize_1 ; ++idx)
	{
		// If the file has already been discarder skip the check and go ahead
		if (_discardMap.find(idx) == _discardMap.end())
		{
			size_t flsize = fileList.size() ;
			for (size_t idy = idx + 1; idy < flsize ; ++idy)
			{
				// If the file has already been discarder skip the check and go ahead
				if (_discardMap.find(idy) == _discardMap.end())
				{
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "comparing [" << idx << "] with [" << idy << "]") ;					
					bool equals = equalInputs (fileList[idx], fileList[idy]);
					
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "inputs are " << (equals?"equals":"different")) ; // PRQA S 3380					
					
					if (equals)
					{
						// files are equals... have to remove the one with lower version
						int lowerIndex = idy;
						if (inputVersionLessThan (fileList[idx], fileList[idy]))
						{
							// X version is less than Y version
							lowerIndex = idx;
						}
                        /*
						else
						{
							// Y version is less than X version
							lowerIndex = idy;
						}*/
						
						ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Trying to remove [" << lowerIndex << "]") ;					
						
						// Can remove the lower index if and only if it is never used in the computed timeline
						if ( find_if (fileTimeline.begin(), fileTimeline.end(), timeLineIndexFinder (lowerIndex)) == fileTimeline.end() ) // PRQA S 3081
						{
							// The lowerIndex is never addressed in the timeline
							ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Allowed to remove [" << lowerIndex << "]") ;					
							_discardMap[lowerIndex] = true;
						}
						else
						{
							// The lowerIndex is addressed in the timeline
							ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "NOT Allowed to remove [" << lowerIndex << "]. Used in timeline!") ;					
						}
					}
				}

			}
		}
	}
}

void 
Alternative_PCC::createLUT(const InputAlternative& a)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	int outCount = -1;
	
	// -1 index always mapped to -1
	_lut[-1] = -1; // 	_lut[inCount] = outCount;

	
    size_t fl_size = getAlternativeValFromTempSymTable(a, true).fileList.size() ;
	for (size_t inCount = 0; inCount < fl_size ; ++inCount)
	{
		if ( _discardMap.find(inCount) == _discardMap.end())
		{
			// This index is not discardable
			++outCount;
			_lut[inCount] = outCount;
		}
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "createLUT LUT DUMP") ;					
    	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		for (map<int,int>::iterator it=_lut.begin(); it != _lut.end(); ++it)
		{
			excerr << "LUT [ " << it->first << " -> " << it->second << " ]";
		}
	ACS_CLASS_END_DEBUG

}

void 
Alternative_PCC::updateSymbolTable(const InputAlternative& a)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	// Get current params from temp symtable
	SymTableParams params = getAlternativeValFromTempSymTable(a, true);
	
	// Create the temporary filename list
	vector <InputFileName> newList;
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "creating new input list") ;					


	// This should be a work for copy_if algo... but it seems to not exists at all in STL
	for (size_t idx=0; idx < params.fileList.size(); ++idx)
	{
		if (_discardMap.find(idx) == _discardMap.end())
		{
			// To not be discarder
			newList.push_back (params.fileList[idx]);
		}
	}

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "updating input list") ;					
	
	// Replace the list
	params.fileList = newList;
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "applying LUT") ;					
	
	// Time to apply the LUT 
	for (vector<TimelineDetails>::iterator it=params.fileTimeline.begin(); it != params.fileTimeline.end(); ++it)
	{
		it->fileNameIdx = _lut[it->fileNameIdx];
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "replacing alternative in temp symtable") ;					

	addAlternativeToTempSymTable (a, params, true);

}


Alternative_PCC::FileInfos
Alternative_PCC::getFileInfos (const PCC_Base::InputFileName& input)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "searching for [" << input.fileName << ", " << input.fileVersion << "]") ;					
	
	FileInfos retValue;
	string mapKey = input.fileName + "_" + input.fileVersion;
	
	if (_infoMap.find(mapKey) == _infoMap.end())
	{
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "NOT FOUND. Querying the database.") ;					

		// Never loaded
		// Create object and guard for the input
		dbInventoryObject *obja= dynamic_cast<dbInventoryObject*>(dbPersistentFactoryChain::instance()->newObject("dbInventoryObject", getParameters()->dbKey));
		dbPersistentScopeGuard theGuarda (obja);

		// Set object primary key
		obja->setFileName (input.fileName);
		obja->setFileVersion (input.fileVersion);

		obja->load(dbConnectionWrapper(db::ConnPools::instance()->getPool(getParameters()->dbKey)));
		
		retValue.validityStart = obja->getValidityStart();
		retValue.validityStop = obja->getValidityStop();
		retValue.generationTime = obja->getGenerationTime();
		
		_infoMap[mapKey] = retValue;
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "FOUND. Using saved data.") ;					

		// Info found
		retValue = _infoMap[mapKey];
	}
	
	return retValue;
}

bool 
Alternative_PCC::equalInputs (const PCC_Base::InputFileName& a, const PCC_Base::InputFileName& b)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	// Retrieving file infos
	FileInfos infoA = getFileInfos (a);
	FileInfos infoB = getFileInfos (b);
	

	// Input in alternative are equals if have same start and same stop time
	bool retval =
		(infoA.validityStart == infoB.validityStart ) &&
		(infoA.validityStop == infoB.validityStop );
		
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "returning [" << boolalpha << retval << "]") ;					
	

	return retval;
}

bool 
Alternative_PCC::inputVersionLessThan (const PCC_Base::InputFileName& a, const PCC_Base::InputFileName& b)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	// Retrieving file infos
	FileInfos infoA = getFileInfos (a);
	FileInfos infoB = getFileInfos (b);
	
	return infoA.generationTime < infoB.generationTime;
}

_ACS_END_NAMESPACE
