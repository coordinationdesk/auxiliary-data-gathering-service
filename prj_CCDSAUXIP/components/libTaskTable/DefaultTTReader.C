// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer TaskTableElements Library $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2016/09/16 11:55:37  lucio.pulvirenti
	PDSEV-41: depending on filter by satellite / mission key occurrency, corresponding InputAlterniative obj attributes are set.
	
	Revision 5.2  2016/05/10 14:43:03  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.1  2013/11/19 17:35:12  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 5.0  2013/07/07 18:24:07  marpas
	itroducing libException 5.x standards
	coding best practices applied
	qa warnings removed
	compilation warnings partially removed
	
	Revision 2.13  2012/12/03 17:01:07  marpas
	new StringUtils interface
	
	Revision 2.12  2012/06/07 11:54:03  marfav
	Code improved removing compiler warnings
	
	Revision 2.11  2012/02/10 17:14:05  marpas
	refactoring in progress
	
	Revision 2.10  2011/09/16 14:02:20  marfav
	Adding Stream tag support to alternatives
	
	Revision 2.9  2010/01/11 10:17:40  marfav
	XMLIstream::extractRoot call added
	
	Revision 2.8  2009/11/26 14:21:48  marfav
	Alternative Filter needs also the XML filter root tag
	
	Revision 2.7  2009/11/25 11:11:34  marfav
	Support to alternative Filter_Def completed
	
	Revision 2.6  2009/11/20 17:26:40  marfav
	Added support for Alternative_Filter_Tag optional field
	
	Revision 2.5  2008/10/20 13:19:25  marfav
	added support to task cmdline parameters
	
	Revision 2.4  2008/04/22 11:17:09  marfav
	Design improved using the conditioned visitor pattern
	
	Revision 2.3  2006/03/23 14:53:51  fracar
	added support to id/ref in input definition
	
	Revision 2.2  2006/03/06 14:27:02  marfav
	Exception 2.1 I/F adopted
	
	Revision 2.1  2006/03/06 14:19:15  marfav
	Added default reader and writer
	

*/


#include <DefaultTTReader.h>
#include <TaskTable.h>
#include <TTSymbolTable.h>
#include <ResolverVisitor.h>
#include <Sequence.h>
#include <Pool.h>

#include <XMLSubTreeXtractor.h>
#include <XMLIstream.h>

#include <StringUtils.h>

#include <rsResourceSet.h>
#include <rsPushNSpace.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(DefaultTTReader)

DefaultTTReader::DefaultTTReader(const string& path) :
    TaskTableVisitor(),
	_ttPathname(path),
	_theOTM(),
	_defs(),
    _rs(),
    _alternativeFilterTag(),
    _taskTableRootTag()
{
    
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	// Read the file in a resourceset and prepare it for the navigation
	// This can throw but it is not a problem, no dynamic objects in this ctor
	XMLIstream  xmlIstream(path);

	// The reader needs the root tag of the task table
	// Set the XMLIstream to read it and store it in the resource
	xmlIstream.extractRoot (true);

	// Extract...
	xmlIstream >> _rs;
}

DefaultTTReader::~DefaultTTReader() throw() 
{
}



bool 
DefaultTTReader::visit (TaskTable& tt)
{
    
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)


    // Read a default-formatted task table
    // as defined in Generic Processor ICD

    // Root level attributes:
    // - Satellite (not mandatory)
    // - Mission (not mandatory)
    // - Processor_Name
    // - Version
    // - Test
    // - Min_Disk_Space
    // - Max_Time

    // SATELLITE
    
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading SATELLITE") ;

    if (_rs.hasKey(_defs._satelliteKey)) 
    {
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SATELLITE is SET") ;
		
        string value;
        _rs.getValue(_defs._satelliteKey, value);
        tt.setSatellite(value);

	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SATELLITE = [" << value << "]") ;
	
    }
    else
    {
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SATELLITE is NOT SET") ;
    }


    // MISSION

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading MISSION") ;

    if (_rs.hasKey(_defs._missionKey)) 
    {
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MISSION is SET") ;

	    string value;
	    _rs.getValue(_defs._missionKey, value);
	    tt.setMission(value);

	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MISSION = [" << value << "]") ;

    }
    else
    {
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MISSION is NOT SET") ;
    }


    // PROCESSOR_NAME
    // No hasKey check. This field is mandatory and the exception has to be raised

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading PROCESSOR_NAME") ;

    {
	    string value;
	    _rs.getValue(_defs._processorNameKey, value);
	    tt.setProcName(value);

	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PROCESSOR_NAME = [" << value << "]") ;
    }

    // PROCESSOR_VERSION
    // No hasKey check. This field is mandatory and the exception has to be raised

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading PROCESSOR_VERSION") ;

    {
	    string value;
	    _rs.getValue(_defs._processorVersionKey, value);
	    tt.setProcVersion(value);

	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PROCESSOR_VERSION = [" << value << "]") ;
    }

    // TEST
    // No hasKey check. This field is mandatory and the exception has to be raised

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading TEST") ;

    {
	bool value = false ;
	_rs.getValue(_defs._testKey, value);
	tt.isTest(value);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TEST = [" << boolalpha << value << "]") ;
    }
    
    // MIN_DISK_SPACE
    // No hasKey check. This field is mandatory and the exception has to be raised

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading MIN_DISK_SPACE") ;

    {
	    int value = 0;
	    _rs.getValue(_defs._diskSpaceKey, value);
	    tt.setMinDiskSpace(value);

	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MIN_DISK_SPACE = [" << value << "]") ;
    }

    // MAX_TIME
    // No hasKey check. This field is mandatory and the exception has to be raised

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading MAX_TIME") ;

    {
	    int value = 0 ;
	    _rs.getValue(_defs._maxTimeKey, value);
	    tt.setMaxAllottedTime(value);

	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MAX_TIME = [" << value << "]") ;
    }

    // ***** CONFIGURATIONS_SECTION *****
    // 
    // DEFAULT_CONFIGURATION
    // No hasKey check. This field is mandatory and the exception has to be raised

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading DEFAULT_CONFIGURATION") ;

    {
	    int value = 0 ;
	    _rs.getValue(_defs._defCfgKey, value);
	    tt.setDefaultConfigVersIdx(value);

	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DEFAULT_CONFIGURATION = [" << value << "]") ;
    }

    // ARRAY OF CONFIGURATIONS (if any)
    // Double check Count attribute and Array size

    // _cfgCountKey    = "Private_Config.List_of_Cfg_Files.count.value
    // _cfgArrayKey    = "Private_Config.List_of_Cfg_Files";
    // _cfgKey         = "Cfg_File";
    {
	int cfgCounter = getArraySize (_rs, _defs._cfgCountKey, _defs._cfgArrayKey, _defs._cfgKey);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "CONFIGURATIONS COUNT =" << cfgCounter) ;


	for (int i=0; i<cfgCounter; ++i) 
	{
	    ostringstream pushKey;
	    pushKey << _defs._cfgArrayKey << "." << _defs._cfgKey << "[" << i << "]";

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PUSHING SPACE \"" << pushKey.str() << "\"") ;

	    // Read and store each Config file 
#warning MP use getArray template method instead
	    rsPushNSpace pusher(_rs, pushKey.str());
	    ConfigDescriptor cfgDescr("","");
	    cfgDescr.acceptVisitor (*this);
	    tt.push_back (cfgDescr); 

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ADDED CONFIGURATION [" << cfgDescr.getPath() << 
				      ", " << cfgDescr.getVersion() << "]") ;
	}
    }

    // **** CONFIGURATION SPACES SECTION ****
    // Array of Config Spaces (if any)
    {
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading CONFIGURATION SPACES") ;

	    int confSpaceCount = getArraySize (_rs, _defs._cfgSpaceCountKey, _defs._cfgSpaceArrayKey, _defs._cfgSpaceNameKey);

	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "CFG SPACE COUNT = " << confSpaceCount) ;

	    for (int i=0; i<confSpaceCount; ++i) 
	    {
	        // Read and store each configuration space HERE
	        ostringstream theKey;
	        theKey << _defs._cfgSpaceArrayKey << "." << _defs._cfgSpaceNameKey << "[" << i << "]";

		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "GETTING SPACE \"" << theKey.str() << "\"") ;

	        // Read and store each ConfigSpace 
	        string confSpace;
	        _rs.getValue(theKey.str(), confSpace);
	        ConfigSpaceDescriptor cfgSpaceDescr(confSpace);
	        tt.push_back (cfgSpaceDescr); 
		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ADDED CONFIGURATION SPACE [" << confSpace << "]") ;
	    }
    }

	//DYNAMIC PROCESSING PARAMETERS SECTION
    if (_rs.hasKey(_defs._dynParamCountKey)) 
    {
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DYNAMIC PROCESSING PARAMETERS AVAILABLE") ;

	    unsigned int count ;
		_rs.getValue(_defs._dynParamCountKey, count) ;
		for(unsigned int i=0; i<count; i++) {
			ostringstream theKey;
			theKey << _defs._dynParamArrayKey << "[" << i << "]." ;
			string name, type, deflt ;
			_rs.getValue(theKey.str()+_defs._dynParamNameKey, name);
			_rs.getValue(theKey.str()+_defs._dynParamTypeKey, type);
			_rs.getValue(theKey.str()+_defs._dynParamDefaultKey, deflt);
			tt.push_back(DynProcParam(name,type,deflt)) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ADDED PARAMETER = [" << name << "," << type << "," << deflt << "]") ;
		}
    }
    else
    {
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DYNAMIC PROCESSING PARAMETERS NOT AVAILABLE") ;
    }

    // **** ALTERNATIVE FILTER TAG ****
    // This field is not mandatory. A default value will be used
    // NOTE that the default value is declared in the TaskTable class

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading ALTERNATIVE_FILTER_TAG") ;

    {
	if (_rs.hasKey (_defs._alternativeFilterTagKey))
	{
		string value;
		_rs.getValue (_defs._alternativeFilterTagKey, value);
		tt.setAlternativeFilterTag (value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ALTERNATIVE_FILTER_TAG = [" << value << "]") ;
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ALTERNATIVE_FILTER_TAG -> NOT SET!!") ;
	}

	// Store the tag internally
	_alternativeFilterTag = tt.getAlternativeFilterTag();
    }

    // Extract the Root tag of XML struture (needed by XMLExtractor)
    _rs.getValue ("XmlExtractionRootTagName_", _taskTableRootTag);


    // **** POOLS AND SEQUENCES SECTION ****
    // Detect the type of task table and start reading it
    {
	    // Search for the "List_of_Sequence" keyword.
	    // If no match assume the Task Table composed of Pools
	    // If neither Pools nor Sequences are found, raise an exception ad exit
	    bool isMadeOfSequences = _rs.hasKey(_defs._sequenceArrayKey);

	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TASK TABLE made of [" << (isMadeOfSequences ? "SEQUENCES" : "POOLS") << "]") ; // PRQA S 3380

	    // Prepare the constants needed to browse the 1st level structure
	    string countKey = (isMadeOfSequences ? _defs._sequenceCountKey : _defs._poolCountKey); // PRQA S 3380, 3384 3
	    string arrayKey = (isMadeOfSequences ? _defs._sequenceArrayKey : _defs._poolArrayKey);
	    string nameKey  = (isMadeOfSequences ? _defs._sequenceKey      : _defs._poolKey);

	    int elementsCount = getArraySize (_rs, countKey, arrayKey, nameKey);
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ELEMENTS COUNT = " << elementsCount) ;

	    for (int i=0; i<elementsCount; ++i)
	    {
	        ostringstream pushKey;
#warning MP use getArray template method instead
	        pushKey << arrayKey << "." << nameKey << "[" << i << "]";

		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PUSHING SPACE \"" << pushKey.str() << "\"") ;

	        // Read and store each structure 
	        rsPushNSpace pusher(_rs, pushKey.str());
	        if (isMadeOfSequences) 
	        {
		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "READING SEQUENCE #" << i+1 << " out of " << elementsCount) ;

		    Sequence seq;
		    seq.acceptVisitor (*this);
		    tt.addElement(seq);
	        }
	        else
	        {
			    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "READING POOL #" << i+1 << " out of " << elementsCount) ;

		        Pool pool;
		        pool.acceptVisitor (*this);
		        tt.addElement(pool);
	        }
	    }
    }
    {
	    // ***** ATEXIT SEQUENCES SECTION *****
	    // Atexit sections are not mandatory in the Task Table schema
	    // Check for the existance before trying to load 

	    if ( _rs.hasKey(_defs._atExitArrayKey) )
	    {
	        string countKey = _defs._atExitCountKey;
	        string arrayKey = _defs._atExitArrayKey;
	        string nameKey  = _defs._atExitKey;

	        int elementsCount = getArraySize (_rs, countKey, arrayKey, nameKey);

		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AT EXIT SEQUENCES COUNT = " << elementsCount) ;

	        for (int i=0; i<elementsCount; ++i)
	        {
		        ostringstream pushKey;
		        pushKey << arrayKey << "." << nameKey << "[" << i << "]";

		        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PUSHING SPACE \"" << pushKey.str() << "\"") ;

		        // Read and store each atexit sequence
#warning MP use getArray template method instead
		        rsPushNSpace pusher(_rs, pushKey.str());

		        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "READING ATEXIT SEQUENCE #" << i+1 << " out of " << elementsCount) ;

		        AtExitSequence seq;
		        seq.acceptVisitor (*this);
		        tt.addAtExitSequence(seq);
	            }
	    }
	    else
	    {
	        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No AtExit sequences detected") ;
	    }
    }

    return false; // since we've added children in the process, we don't want to navigate them!
}



bool 
DefaultTTReader::exitVisit (TaskTable& tt)
{
    
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)


    // ok... let's polish the task table adjusting id/ref for inputs


    
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "building symbol table") ;

    // first step: let's create a symbol table
    // (en-passant, check for duplicated ID
    TTSymbolTable st;
    tt.acceptVisitor(st);

    

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "resolving input references") ;

    // second step: let's resolve references...
    ResolverVisitor v(st);
    tt.acceptVisitor(v);


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "exiting") ;

    return false; // don't want to re-scan children!
}



int 
DefaultTTReader::getArraySize (rsResourceSet& rs, const string& countKey, const string& arrayKey, const string& elementKey)
{
	int countParam = 0;
	int arraySize = 0;
	ostringstream theArrayKey;
	ostringstream theElementKey;
	theElementKey << arrayKey << "." << elementKey;
	theArrayKey << arrayKey << "." << elementKey << "[]";

	rs.getValue (countKey, countParam);
	try
	{
		rs.getValue (theArrayKey.str(), arraySize);
	}
	catch(rsResourceSet::NotFoundException &e)
	{
		if (rs.hasKey(theElementKey.str())) 
		{
			arraySize = 1;
		}
		else
		{
			arraySize = 0;
		}
	};

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading array size of [" << arrayKey << "]") ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, countKey << " = " << countParam) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, theArrayKey.str() << " size = " << arraySize) ;

	if (countParam != arraySize) 
	{
		ostringstream os;
		os << "Mismatch comparing count value [" << countKey << "] (" << countParam << ") and effective array size (" << arraySize << ")";
		DefaultTTReaderException ex(os.str());
		ACS_THROW(ex);
	}

	return arraySize;
}


bool 
DefaultTTReader::visit (ConfigSpaceDescriptor&)
{

	// No need to read the configuration space objects here
	// The Config Spaces can be addressed directly when pushing the ResourceSet
	// The structure will be read by ReadTaskTable method

	return true;
}

bool 
DefaultTTReader::visit (ConfigDescriptor& cd)
{

	// VERSION
	// No hasKey check. This field is mandatory and the exception has to be raised

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading VERSION") ;

	{
		string value;
		_rs.getValue(_defs._cfgVersionKey, value);
		cd.setVersion(value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "VERSION = [" << value << "]") ;
	}

	// CONFIG_FILE
	// No hasKey check. This field is mandatory and the exception has to be raised

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading PATH") ;

	{
		string value;
		_rs.getValue(_defs._cfgPathKey, value);
		cd.setPath(value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PATH = [" << value << "]") ;
	}


    return true;
}

bool 
DefaultTTReader::visit (Sequence& seq)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)


	// Sequence Structure
	// - Weight (not mandatory)
	// - Quitting_Signal (not mandatory)
	// - Killing_Signal
	// - Detached
	// - List_of Pools/Sequences/Tasks (1 choice)

	
	// WEIGHT
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading WEIGHT") ;

	if (_rs.hasKey(_defs._sequenceWeightKey)) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "WEIGHT is SET") ;
			
		float value = 0 ;
		_rs.getValue(_defs._sequenceWeightKey, value);
		seq.setAssignedPercentage(value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "WEIGHT = [" << value << "]") ;
		
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "WEIGHT is NOT SET") ;
	}


	// QUITTING_SIGNAL
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading QUITTING_SIGNAL") ;

	if (_rs.hasKey(_defs._sequenceQuitSignalKey)) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "QUITTING_SIGNAL is SET") ;
			
		int value = 0;
		_rs.getValue(_defs._sequenceQuitSignalKey, value);
		seq.setQuitSignal(value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "QUITTING_SIGNAL = [" << value << "]") ;
		
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "QUITTING_SIGNAL is NOT SET") ;
	}


	// KILLING_SIGNAL
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading KILLING_SIGNAL") ;

	{
		int value = 0 ;
		_rs.getValue(_defs._sequenceKillSignalKey, value);
		seq.setKillSignal(value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "KILLING_SIGNAL = [" << value << "]") ;
	}
		
	// DETACHED
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading DETACHED") ;

	{
		bool value = false ;
		_rs.getValue(_defs._sequenceDetachedKey, value);
		seq.isDetached(value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DETACHED = [" << boolalpha << value << "]") ;
	}

	// The pool can be composed of Pools, Sequences or Tasks... 
	bool madeOfPools = false;
	bool madeOfSequences = false;
	string countKey = "";
	string arrayKey = "";
	string nameKey  = "";

	if (_rs.hasKey (_defs._poolArrayKey)) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The Pool is a sequence of POOLS") ;

		// It is a Pool of Pools
		madeOfPools = true;
		countKey = _defs._poolCountKey;
		arrayKey = _defs._poolArrayKey;
		nameKey = _defs._poolKey;
	}
	else if (_rs.hasKey(_defs._sequenceArrayKey)) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The Pool is a sequence of SEQUENCES") ;
	
		// It is a Pool of Sequences
		madeOfSequences = true;
		countKey = _defs._sequenceCountKey;
		arrayKey = _defs._sequenceArrayKey;
		nameKey = _defs._sequenceKey;
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The Pool is a sequence of TASKS") ;

		// It is a Pool of Tasks
		countKey = _defs._taskCountKey;
		arrayKey = _defs._taskArrayKey;
		nameKey = _defs._taskKey;
	}

	// Read the elements structure
	
	int elementsCount = getArraySize (_rs, countKey, arrayKey, nameKey);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "STRUCTURE COUNT = " << elementsCount) ;

	for (int i=0; i<elementsCount; ++i)
	{
		ostringstream pushKey;
		pushKey << arrayKey << "." << nameKey << "[" << i << "]";

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PUSHING SPACE \"" << pushKey.str() << "\"") ;

		// Read and store each structure
#warning MP use getArray template method instead
		rsPushNSpace pusher(_rs, pushKey.str());

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "READING STRUCTURE #" << i+1 << " out of " << elementsCount) ;

		if (madeOfPools) 
		{
			Pool p;
			p.acceptVisitor (*this);
			seq.addElement(p);
		}
		else if (madeOfSequences) 
		{
			Sequence s;
			s.acceptVisitor (*this);
			seq.addElement(s);
		}
		else
		{
			Task t;
			t.acceptVisitor (*this);
			seq.addElement(t);
		}
	}

    return false; // since we've added children in the process, we don't want to navigate them!
}

bool 
DefaultTTReader::exitVisit (Sequence&)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

    return false; // since we've added children in the process, we don't want to navigate them!
}

bool 
DefaultTTReader::visit (Pool& pool)
{
    
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)



	// Pool Structure
	// - Weight (not mandatory)
	// - Quitting_Signal (not mandatory)
	// - Killing_Signal
	// - Detached
	// - List_of Pools/Sequences/Tasks (1 choice)

	
	// WEIGHT
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading WEIGHT") ;

	if (_rs.hasKey(_defs._poolWeightKey)) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "WEIGHT is SET") ;
			
		float value = 0 ;
		_rs.getValue(_defs._poolWeightKey, value);
		pool.setAssignedPercentage(value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "WEIGHT = [" << value << "]") ;
		
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "WEIGHT is NOT SET") ;
	}


	// QUITTING_SIGNAL
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Reading QUITTING_SIGNAL") ;

	if (_rs.hasKey(_defs._poolQuitSignalKey)) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "QUITTING_SIGNAL is SET") ;
			
		int value = 0 ;
		_rs.getValue(_defs._poolQuitSignalKey, value);
		pool.setQuitSignal(value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "QUITTING_SIGNAL = [" << value << "]") ;
		
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "QUITTING_SIGNAL is NOT SET") ;
	}


	// KILLING_SIGNAL
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading KILLING_SIGNAL") ;

	{
		int value = 0 ;
		_rs.getValue(_defs._poolKillSignalKey, value);
		pool.setKillSignal(value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "KILLING_SIGNAL = [" << value << "]") ;
	}
		
	// DETACHED
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading DETACHED") ;

	{
		bool value = false ;
		_rs.getValue(_defs._poolDetachedKey, value);
		pool.isDetached(value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DETACHED = [" << boolalpha << value << "]") ;
	}

	// The pool can be composed of Pools, Sequences or Tasks... 
	bool madeOfPools = false;
	bool madeOfSequences = false;
	string countKey = "";
	string arrayKey = "";
	string nameKey  = "";

	if (_rs.hasKey (_defs._poolArrayKey)) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The Pool is a sequence of POOLS") ;

		// It is a Pool of Pools
		madeOfPools = true;
		countKey = _defs._poolCountKey;
		arrayKey = _defs._poolArrayKey;
		nameKey = _defs._poolKey;
	}
	else if (_rs.hasKey(_defs._sequenceArrayKey)) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The Pool is a sequence of SEQUENCES") ;
	
		// It is a Pool of Sequences
		madeOfSequences = true;
		countKey = _defs._sequenceCountKey;
		arrayKey = _defs._sequenceArrayKey;
		nameKey = _defs._sequenceKey;
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The Pool is a sequence of TASKS") ;

		// It is a Pool of Tasks
		countKey = _defs._taskCountKey;
		arrayKey = _defs._taskArrayKey;
		nameKey = _defs._taskKey;
	}

	// Read the elements structure
	
	int elementsCount = getArraySize (_rs, countKey, arrayKey, nameKey);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "STRUCTURE COUNT = " << elementsCount) ;

	for (int i=0; i<elementsCount; ++i)
	{
		ostringstream pushKey;
		pushKey << arrayKey << "." << nameKey << "[" << i << "]";

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PUSHING SPACE \"" << pushKey.str() << "\"") ;

		// Read and store each structure 
#warning MP use getArray template method instead
		rsPushNSpace pusher(_rs, pushKey.str());

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "READING STRUCTURE #" << i+1 << " out of " << elementsCount) ;

		if (madeOfPools) 
		{
			Pool p;
			p.acceptVisitor (*this);
			pool.addElement(p);
		}
		else if (madeOfSequences) 
		{
			Sequence s;
			s.acceptVisitor (*this);
			pool.addElement(s);
		}
		else
		{
			Task t;
			t.acceptVisitor (*this);
			pool.addElement(t);
		}
	}

    return false; // since we've added children in the process, we don't want to navigate them!
}

bool 
DefaultTTReader::exitVisit (Pool&)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

    return false; // since we've added children in the process, we don't want to navigate them!
}

bool 
DefaultTTReader::enterVisit (InputList&)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

    // inputs have been loaded inside the Task visit;
    // since we've added children in the process, we don't want to navigate them!
    // (actually... we don't even talk of InputList ;-)
    return false;
}

bool 
DefaultTTReader::enterVisit (OutputList&)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

    // outputs have been loaded inside the Task visit;
    // since we've added children in the process, we don't want to navigate them!
    // (actually... we don't even talk of OutputList ;-)
    return false;
}

bool 
DefaultTTReader::enterVisit (BreakpointList&)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

    // breakpoints have been loaded inside the Task visit;
    // since we've added children in the process, we don't want to navigate them!
    // (actually... we don't even talk of BreakpointList ;-)
    return false;
}

bool 
DefaultTTReader::enterVisit (TaskParameterList&)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

    // TaskParameters have been loaded inside the Task visit;
    // since we've added children in the process, we don't want to navigate them!
    // (actually... we don't even talk of TaskParameterList ;-)
    return false;
}

bool 
DefaultTTReader::visit (InputDescriptor& inDes)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	//JIRA CI-98: CHECK REF ATTRIBUTE BEFORE. IF PRESENT THE INPUT SECTION SHALL BE EMPTY (GEN ICD 1.8) - 
	if ( _rs.hasKey(_defs._inputRefKey) )
	{
		string value;
		_rs.getValue(_defs._inputRefKey, value);
		inDes.setRef (value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "REF = " << value) ;
		return false ;
	}

	/**
	 * Input Descriptor Structure
	 * - Mode
	 * - Mandatory
	 * - List of Alternatives
	 */ 

	// MODE
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading MODE") ;

	{
		string value;
		_rs.getValue(_defs._inputModeKey, value);
		string convertedValue = _theOTM.getOrderTypeFromTT (value);
		inDes.setOrderType (convertedValue);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MODE = [" << value << "] - DB = [" << convertedValue << "]" ) ;
	}
	

	// MANDATORY
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading MANDATORY") ;

	{
		bool value = false ;
		_rs.getValue(_defs._inputMandatoryKey, value);
		inDes.isMandatory (value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MANDATORY = [" << boolalpha << value << "]") ;
	}
	

	// ID
	{
		// ***** ID/REF ATTRIBUTES SECTION *****
		// ID attribute is not mandatory in the Task Table schema
		// Check for the existence before trying to load 

		if ( _rs.hasKey(_defs._inputIdKey) )
		{
		    string value;
		    _rs.getValue(_defs._inputIdKey, value);
		    inDes.setId (value);

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ID = " << value) ;
		}
	}

		
	// if not a Ref, (Id or anonymous) keep on loading remaining input fields (alternatives, essentially)
	// LIST_OF_INPUT_ALTERNATIVES
	if (!inDes.isSetRef())
	{
		string countKey = _defs._alternCountKey;
		string arrayKey = _defs._alternArrayKey;
		string nameKey = _defs._alternKey;

		int elementsCount = getArraySize (_rs, countKey, arrayKey, nameKey);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ALTERNATIVE COUNT = " << elementsCount) ;

		for (int i=0; i<elementsCount; ++i)
		{
			ostringstream pushKey;
			pushKey << arrayKey << "." << nameKey << "[" << i << "]";

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PUSHING SPACE \"" << pushKey.str() << "\"") ;

			// Read and store each Input Descriptor
#warning MP use getArray template method instead
			rsPushNSpace pusher(_rs, pushKey.str());

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "READING INPUT ALTERNATIVE #" << i+1 << " out of " << elementsCount) ;

			InputAlternative inAlt;
			inAlt.acceptVisitor (*this);
			inDes.push_back (inAlt);
		}
	}


	// perform some checks (only the ones that can be done here at this point)
	{
	    // first one: nominal condition (only for debug)
	    if (!inDes.isSetId() && !inDes.isSetRef())
	    {
		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No id/ref attribute detected") ;
	    }
	    // next one: both set (error)
	    else if (inDes.isSetId() && inDes.isSetRef())
	    {
		    ostringstream os;
		    os << "Id (" << inDes.getId() << ") and Ref (" << inDes.getRef() << ") attributes both set for input";
		    exIllegalValueException ex(os.str());
		    ACS_THROW (ex);
	    }
        else { /* nothing */ }

	    // next: duplicate IDs are evil...!
	    // (they'll be checked inside the SymbolTable builder)

	    // ... unresolved references are evil, too!
	    // (they'll be checked inside the Resolver)
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "upon exit INPUT id = " << 
					          inDes.getId() << ", INPUT ref = " << inDes.getRef()) ;

    return false; // since we've added children in the process, we don't want to navigate them!
}

bool 
DefaultTTReader::exitVisit (InputDescriptor&)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

    return false; // since we've added children in the process, we don't want to navigate them!
}

bool 
DefaultTTReader::visit (InputAlternative& inAlt)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	/**
	 * InputAlternative Structure
	 * - Order
	 * - Origin
	 * - Retrieval_Mode
	 * - Parameters (TO BE DEFINED)
	 * - T0
	 * - T1
	 * - File_Type
	 * - File_Class (not mandatory)
	 * - File_Name_Type
	 * - Stream (not mandatory)
	 * - FilterBySatellite (not mandatory)
	 * - FilterByMission (not mandatory)
	 * - AlternativeActivationTime (not mandatory)
	 */ 

	// ORDER
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading ORDER") ;

	{
		int value = 0 ;
		_rs.getValue(_defs._alternOrderKey, value);
		inAlt.setPreferenceOrder (value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ORDER = [" << value << "]") ;
	}

	// ORIGIN
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading ORIGIN") ;

	{
		string value;
		_rs.getValue(_defs._alternOriginKey, value);
		InputOrigin origin = String2InputOrigin(value);
		if (origin == InputOriginUNKNOWN) 
		{
			// Unknown origin is not allowed.. 
			DefaultTTReaderException ex("Input origin UNKNOWN in namespace " + _rs.currentNameSpace());
			ACS_THROW (ex);
		}
		
		inAlt.setOrigin (origin);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ORIGIN = [" << value << "]") ;
	}


	// RETRIEVAL_MODE
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading RETRIEVAL_MODE") ;

	{
		string value;
		_rs.getValue(_defs._alternModeKey, value);
		inAlt.setRetrievalMode (value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "RETRIEVAL_MODE = [" << value << "]") ;
	}


	// PARAMETERS
#warning Input Alternative Parameters still not defined

	// T0
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading T0") ;

	{
		int value = 0 ;
		_rs.getValue(_defs._alternT0Key, value);
		inAlt.setT0 (value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "T0 = [" << value << "]") ;
	}

	// T1
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading T1") ;

	{
		int value = 0 ;
		_rs.getValue(_defs._alternT1Key, value);
		inAlt.setT1 (value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " T1 = [" << value << "]") ;
	}

	// FILE_TYPE
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading FILE_TYPE") ;

	{
		string value;
		_rs.getValue(_defs._alternTypeKey, value);
		inAlt.setFileType (value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FILE_TYPE = [" << value << "]") ;
	}
		


	// FILE_CLASS
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading FILE_CLASS") ;

	if (_rs.hasKey(_defs._alternClassKey)) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FILE_CLASS is SET") ;
			
		string value;
		_rs.getValue(_defs._alternClassKey, value);
		inAlt.setFileClass(value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FILE_CLASS = [" << value << "]") ;
		
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FILE_CLASS is NOT SET") ;
	}


	
	// FILE_NAME_TYPE
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading FILE_NAME_TYPE") ;

	{
		string value;
		_rs.getValue(_defs._alternFileNameTypeKey, value);
		FileNameType theType = String2FileNameType (value);
		if (theType == FileNameTypeUNKNOWN) 
		{
			// Unknown file name type is not allowed.. 
			DefaultTTReaderException ex("Input file name type UNKNOWN in namespace " +  _rs.currentNameSpace());
			ACS_THROW (ex);
		}
		inAlt.setFileNameType (theType);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FILE_NAME_TYPE = [" << value << "]") ;
	}
	
	// FILTER TAG
	// not mandatory
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading ALTERNATIVE_FILTER_TAG") ;
		
	{
		if (_rs.hasKey (_alternativeFilterTag))
		{
			ostringstream os1;
			os1 << "/" << _taskTableRootTag << "." << _rs.currentNameSpace() << "." << _alternativeFilterTag;
			map<char, char > tr ;
            tr['.'] = '/' ;
            tr['['] = '<' ;
            tr[']'] = '>' ;
            string extractionKey = StringUtils::transform(os1.str(), tr) ;
			
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ALTERNATIVE_FILTER_TAG key [" << extractionKey << "]") ;
			ostringstream os;

			try
			{
				// Task table name... need to read it !
				// Exception here are critical and will be managed outside
				XMLIstream  xmlIstream(_ttPathname);
				// XML extractor instance registration
				XMLSubTreeXtractor stx(os, extractionKey);
				xmlIstream.addConsumer(&stx);
				// start extraction
				xmlIstream.saxExtract();
			}
			catch (exception& ex)
			{
				DefaultTTReaderException ex1(ex, "Unable to read alternative FILTER in namespace " + _rs.currentNameSpace());
				ACS_THROW (ex1);
			}

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FILTER is [\n" << os.str() << "\n" << "] with RootKey [" << _alternativeFilterTag << "]") ;

			inAlt.setFilter (os.str(), _alternativeFilterTag);
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ALTERNATIVE_FILTER_TAG -> NOT SET!") ;
		}
	}

	// STREAM (not mandatory)
	if (_rs.hasKey(_defs._alternStreamKey)) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "STREAM is SET") ;
			
		bool value = false ;
		_rs.getValue(_defs._alternStreamKey, value);
		inAlt.setStream(value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "STREAM = [" << boolalpha << value << "]") ;
		
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "STREAM is NOT SET") ;
	}
	
	// Filter by Satellite
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading FILTER_BY_SATELLITE") ;

	if (_rs.hasKey(_defs._alternFilterBySatelliteKey)) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FILTER_BY_SATELLITE is SET") ;
			
		bool value = false;
		_rs.getValue(_defs._alternFilterBySatelliteKey, value);
		inAlt.setFilterBySatellite(value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FILTER_BY_SATELLITE = [" << value << "]") ;
		
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FILTER_BY_SATELLITE is NOT SET") ;
	}
	
	// Filter by Mission
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading FILTER_BY_MISSION") ;

	if (_rs.hasKey(_defs._alternFilterByMissionKey)) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FILTER_BY_MISSION is SET") ;
			
		bool value = false;
		_rs.getValue(_defs._alternFilterByMissionKey, value);
		inAlt.setFilterByMission(value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FILTER_BY_MISSION = [" << value << "]") ;
		
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FILTER_BY_MISSION is NOT SET") ;
	}
	
	// AlternativeActivationTime (not mandatory)
	if (_rs.hasKey(_defs._alternActivationTimeKey)) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AlternativeActivationTime is SET") ;
			
		double value = 0.0 ;
		_rs.getValue(_defs._alternActivationTimeKey, value);
		inAlt.setAltActivationTime(value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AlternativeActivationTime = [" << value << "]") ;
		
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AlternativeActivationTime is NOT SET") ;
	}
	
    return true;
}

bool 
DefaultTTReader::visit (OutputDescriptor& outDes)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	/**
	 * Output Descriptor Structure
	 * - Destination
	 * - Mandatory
	 * - File_Type || Type (Gen ICD 1.8)
	 * - File_Name_Type
	 */ 
	
	// DESTINATION
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading DESTINATION") ;

	{
		string value;
		_rs.getValue(_defs._outputDestinationKey, value);
		OutputDest dest=String2OutputDest (value);
		if (dest == OutputDestUNKNOWN) 
		{
			// Unknown destination is not allowed.. 
			DefaultTTReaderException ex("Output destination UNKNOWN in namespace " + _rs.currentNameSpace());
			ACS_THROW (ex);
		}
		outDes.setDestination (dest);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DESTINATION = [" << value << "]") ;
	}

	// MANDATORY
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading MANDATORY") ;

	{
		bool value = false ;
		_rs.getValue(_defs._outputMandatoryKey, value);
		outDes.isMandatory (value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MANDATORY = [" << boolalpha << value << "]") ;
	}
		
	// FILETYPE
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading FILETYPE") ;

	{
		string value;
		if(_rs.hasKey(_defs._outputFileTypeKey) ){ 
			_rs.getValue(_defs._outputFileTypeKey, value);
		}
		else {
			_rs.getValue(_defs._outputFileTypeKey2, value);
			outDes.setFileTypeTag(_defs._outputFileTypeKey2) ;
		}
		outDes.setFileType (value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FILETYPE = [" << value << "]") ;
	}

	// FILENAMETYPE
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading FILENAMETYPE") ;

	{
		string value;
		_rs.getValue(_defs._outputFileNameTypeKey, value);
		FileNameType theType = String2FileNameType(value);
		if (theType == FileNameTypeUNKNOWN) 
		{
			// Unknown file name type is not allowed.. 
			DefaultTTReaderException ex("Output File Name Type UNKNOWN in namespace "+ _rs.currentNameSpace());
			ACS_THROW (ex);
		}
		outDes.setFileNameType (theType);
		

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FILENAMETYPE = [" << value << "]") ;
	}
		

    return true;
}

bool 
DefaultTTReader::visit (BreakpointDescriptor&)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	// No need to call this
	// The brk descriptors are read inside the visit TaskTable method

    return true;
}

bool 
DefaultTTReader::visit (AtExitSequence& aes)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)


	// AtExitSequence is composed by
	// - On_Exit_Status
	// - Override_Status
	// - Sequence
	
	// Exit_Status
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading EXIT_STATUS") ;

	{
		string value;
		_rs.getValue(_defs._atExitStatusKey, value);
		ExitStatus theStatus = String2ExitStatus (value);
		if (theStatus == ExitStatusUNKNOWN) 
		{
			// Unknown exit status is not allowed.. 
			DefaultTTReaderException ex("At Exit status condition UNKNOWN in namespace "+ _rs.currentNameSpace());
			ACS_THROW (ex);
		}
		aes.setExitStatus (theStatus);
		

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "EXIT_STATUS = [" << value << "]") ;
	}

	// Override_Status
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading OVERRIDE_STATUS") ;

	{
		bool value = false ;
		_rs.getValue(_defs._atExitOverrideKey, value);
		aes.setOverrideStatus (value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "OVERRIDE_STATUS = [" << boolalpha << value << "]") ;
	}

	// Sequence
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading SEQUENCE") ;

	{
		rsPushNSpace pusher(_rs, _defs._sequenceKey);
		Sequence seq;
		seq.acceptVisitor (*this);
		aes.addElement (seq);
	}

    return false; // since we've added children in the process, we don't want to navigate them!
}

bool 
DefaultTTReader::exitVisit (AtExitSequence&)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

    return false; // since we've added children in the process, we don't want to navigate them!
}



bool 
DefaultTTReader::visit (Task& task)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	/**
	 * TASK Structure
	 * - Detached (not mandatory)
	 * - Quitting_Signal (not mandatory)
	 * - Killing Signal (not mandatory)
	 * - Weight (not mandatory)
	 * - Name
	 * - Version
	 * - Critical
	 * - Criticality_Level
	 * - File_Name
     * - Parameters
	 * - Inputs
	 * - Breakpoints
	 * - Outputs
	 */ 

	// DETACHED
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading DETACHED") ;

	if (_rs.hasKey(_defs._taskDetachedKey)) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DETACHED is SET") ;
			
		bool value = false ;
		_rs.getValue(_defs._taskDetachedKey, value);
		task.isDetached(value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DETACHED = [" << boolalpha << value << "]") ;
		
	}
	else
	{
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DETACHED is NOT SET") ;
	}

	
	// QUIT_SIGNAL
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading QUIT_SIGNAL") ;

	if (_rs.hasKey(_defs._taskQuitSignalKey)) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "QUIT_SIGNAL is SET") ;
			
		int value = 0 ;
		_rs.getValue(_defs._taskQuitSignalKey, value);
		task.setQuitSignal(value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "QUIT_SIGNAL = [" << value << "]") ;
		
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "QUIT_SIGNAL is NOT SET") ;
	}

	
	// KILL_SIGNAL
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading KILL_SIGNAL") ;

	if (_rs.hasKey(_defs._taskKillSignalKey)) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "KILL_SIGNAL is SET") ;
			
		int value = 0 ;
		_rs.getValue(_defs._taskKillSignalKey, value);
		task.setKillSignal(value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "KILL_SIGNAL = [" << value << "]") ;
		
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "KILL_SIGNAL is NOT SET") ;
	}


	// WEIGHT
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading WEIGHT") ;

	if (_rs.hasKey(_defs._taskWeightKey)) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "WEIGHT is SET") ;
		
			
		int value = 0 ;
		_rs.getValue(_defs._taskWeightKey, value);
		task.setAssignedPercentage(value); // PRQA S 3011

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "WEIGHT = [" << value << "]") ;
		
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "WEIGHT is NOT SET") ;
	}

	
	// NAME
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading NAME") ;

	{
		string value;
		_rs.getValue(_defs._taskNameKey, value);
		task.setName (value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "NAME = [" << value << "]") ;
	}

	
	// VERSION
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading VERSION") ;

	{
		string value;
		_rs.getValue(_defs._taskVersionKey, value);
		task.setVersion (value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "VERSION = [" << value << "]") ;
	}


	// CRITICAL
	
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading CRITICAL") ;

	{
		bool value = false ;
		_rs.getValue(_defs._taskCriticalKey, value);
		task.isCritical (value);
        
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "CRITICAL = [" << boolalpha << value << "]") ;
	}

		
	// CRITICALITY_LEVEL
	
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading CRITICALITY_LEVEL") ;

	{
		int value = 0 ;
		_rs.getValue(_defs._taskCrLevelKey, value);
		task.setCriticalityLevel (value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "CRITICALITY_LEVEL = [" << value << "]") ;
	}

	// PATH
	
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading PATH") ;

	{
		string value;
		_rs.getValue(_defs._taskPathKey, value);
		task.setPath (value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PATH = [" << value << "]") ;
	}

	// POSIX EXIT CODE
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading POSIX EXIT CODE mode") ;

	if (_rs.hasKey(_defs._taskPosixExitCodeKey)) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "POSIX EXIT CODE mode is SET") ;
		
			
		bool value = true ;
		_rs.getValue(_defs._taskPosixExitCodeKey, value);
		task.isPosixExitCode(value); // PRQA S 3011

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "POSIX EXIT CODE mode = [" << boolalpha << value << "]") ;
		
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "POSIX EXIT CODE mode is NOT SET") ;
	}

    // LIST_OF_PARAMETERS
    {
        string countKey = _defs._paramCountKey;
        string arrayKey = _defs._paramArrayKey;
        string nameKey = _defs._paramKey;

        // All the structure is optional
        // check for existence before trying to load it!
        if (_rs.hasKey (countKey)) 
        {
            int elementsCount = getArraySize (_rs, countKey, arrayKey, nameKey);

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PARAMETERS COUNT = " << elementsCount) ;

            for (int i=0; i<elementsCount; ++i)
            {
                ostringstream pushKey;
                pushKey << arrayKey << "." << nameKey << "[" << i << "]";

                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PUSHING SPACE \"" << pushKey.str() << "\"") ;

                // Read and store each Input Descriptor
#warning MP use getArray template method instead
                rsPushNSpace pusher(_rs, pushKey.str());

                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "READING PARAMETER DESCRIPTOR #" << i+1 << " out of " << elementsCount) ;

                TaskParameterDescriptor pDesc;
                pDesc.acceptVisitor (*this);


                task.addParameter(pDesc);
            }
        }
        else
        {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "NO PARAMETERS FOUND") ;
        }
    }

		
	// LIST_OF_INPUTS
	{
		string countKey = _defs._inputCountKey;
		string arrayKey = _defs._inputArrayKey;
		string nameKey = _defs._inputKey;

		int elementsCount = getArraySize (_rs, countKey, arrayKey, nameKey);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "INPUTS COUNT = " << elementsCount) ;
				
		for (int i=0; i<elementsCount; ++i)
		{
			ostringstream pushKey;
			pushKey << arrayKey << "." << nameKey << "[" << i << "]";

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PUSHING SPACE \"" << pushKey.str() << "\"") ;

			// Read and store each Input Descriptor
#warning MP use getArray template method instead
			rsPushNSpace pusher(_rs, pushKey.str());

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "READING INPUT DESCRIPTOR #" << i+1 << " out of " << elementsCount) ;

			InputDescriptor inDesc;
			inDesc.acceptVisitor (*this);


			task.addInput(inDesc);
		}
	}

	// LIST_OF_BRKPOINTS
	{
		string countKey = _defs._brkptCountKey;
		string arrayKey = _defs._brkptArrayKey;
		string nameKey = _defs._brkptKey;

		int elementsCount = getArraySize (_rs, countKey, arrayKey, nameKey);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "BRKPOINTS COUNT = " << elementsCount) ;

		for (int i=0; i<elementsCount; ++i)
		{
			ostringstream theKey;
			theKey << arrayKey << "." << nameKey << "[" << i << "]";

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "READING BRKPOINT DESCRIPTOR #" << i+1 << " out of " << elementsCount) ;

			string value;
			_rs.getValue(theKey.str(), value);

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "BRKPOINT Filetype [" << value << "]") ;
	
			BreakpointDescriptor brkDesc;
			brkDesc.setFileType(value);
			task.addBreakpoint(brkDesc);
		}
	}
	 
	// LIST_OF_OUTPUTS
	{
		string countKey = _defs._outputCountKey;
		string arrayKey = _defs._outputArrayKey;
		string nameKey = _defs._outputKey;

		int elementsCount = getArraySize (_rs, countKey, arrayKey, nameKey);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "OUTPUTS COUNT = " << elementsCount) ;
				
		for (int i=0; i<elementsCount; ++i)
		{
			ostringstream pushKey;
			pushKey << arrayKey << "." << nameKey << "[" << i << "]";

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PUSHING SPACE \"" << pushKey.str() << "\"") ;

			// Read and store each Output Descriptor
#warning MP use getArray template method instead
			rsPushNSpace pusher(_rs, pushKey.str());

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "READING OUTPUT DESCRIPTOR #" << i+1 << " out of " << elementsCount) ;

			OutputDescriptor outDesc;
			outDesc.acceptVisitor (*this);
			task.addOutput(outDesc);
		}
	}


    return false; // since we've added children in the process, we don't want to navigate them!
}

bool 
DefaultTTReader::exitVisit (Task&)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

    return false; // since we've added children in the process, we don't want to navigate them!
}


bool 
DefaultTTReader::visit (TaskParameterDescriptor& pDes)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	/**
	 * Task Parameter Descriptor Structure
	 * - Mandatory flag (root level attribute)
	 * - Name (not mandatory)
	 * - Value (mandatory)
	 */ 
	
	// MANDATORY FLAG
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading MANDATORY FLAG") ;

	{
		bool value = false ;
        ostringstream key;
        key << _defs._paramMandatoryTag << ".value";
		_rs.getValue(key.str(), value);

		pDes.isMandatory (value);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MANDATORY FLAG = [" << boolalpha << value << "]") ;
	}

	// NAME
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading NAME") ;

    if (_rs.hasKey (_defs._paramNameKey)) 
    {
        string name;
        _rs.getValue (_defs._paramNameKey, name);
        pDes.setName (name);

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "NAME = [" << name << "]") ; 
    }
    else
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "NO NAME SET") ; 
    }

    // VALUE
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading VALUE") ;

    {
        string value;
        _rs.getValue (_defs._paramValueKey, value);
        pDes.setValue (value);

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "VALUE = [" << value << "]") ; 
    }


    return true;
}


_ACS_END_NAMESPACE
