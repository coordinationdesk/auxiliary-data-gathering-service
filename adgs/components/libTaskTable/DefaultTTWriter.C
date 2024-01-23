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
	Revision 5.2  2016/09/16 11:59:12  lucio.pulvirenti
	PDSEV-41: depending on set of InputAlternative obj to filter by satellite/mission the corresponding task table tag are written.
	
	Revision 5.1  2013/11/19 17:35:12  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 5.0  2013/07/07 18:24:07  marpas
	itroducing libException 5.x standards
	coding best practices applied
	qa warnings removed
	compilation warnings partially removed
	
	Revision 2.10  2012/06/07 11:54:03  marfav
	Code improved removing compiler warnings
	
	Revision 2.9  2012/02/10 17:14:05  marpas
	refactoring in progress
	
	Revision 2.8  2011/09/16 14:02:20  marfav
	Adding Stream tag support to alternatives
	
	Revision 2.7  2009/11/26 14:21:49  marfav
	Alternative Filter needs also the XML filter root tag
	
	Revision 2.6  2009/11/25 11:11:34  marfav
	Support to alternative Filter_Def completed
	
	Revision 2.5  2009/11/20 17:26:40  marfav
	Added support for Alternative_Filter_Tag optional field
	
	Revision 2.4  2008/10/20 13:19:25  marfav
	added support to task cmdline parameters
	
	Revision 2.3  2008/04/22 11:17:09  marfav
	Design improved using the conditioned visitor pattern
	
	Revision 2.2  2006/03/23 14:53:51  fracar
	added support to id/ref in input definition
	
	Revision 2.1  2006/03/06 14:19:15  marfav
	Added default reader and writer
	

*/

#include <DefaultTTWriter.h>
#include <TaskTable.h>
#include <XMLOstream.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(DefaultTTWriter)

DefaultTTWriter::DefaultTTWriter(XMLOstream& o) :
    TaskTableVisitor(),
	_theOTM(),
	_defs(),
	_theOstream(o), // PRQA S 2528
    _tagsStack()
{
}

DefaultTTWriter::~DefaultTTWriter() throw() 
{
	// Empty DTOR
}

// Task Table section is composed by
// - Satellite (not mandatory)
// - Mission (not mandatory)
// - Processor Name
// - Processor Version
// - Test Flag
// - Min Disk Space
// - Max Time
// - Private Config (SECTION)
// - Config Spaces (SECTION)
// - Pools OR Sequences (SECTION)
// - AtExit Sequences (SECTION - not mandatory)

bool 
DefaultTTWriter::visit (TaskTable& t)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	// the xmlostream is open and will handle the tab spacing

	// Satellite and mission tags are not mandatory... do not write if not set

	if (t.isSetSatellite()) 
	{ XMLOstream::Tag tag(_theOstream, _defs._satelliteKey, t.getSatellite(), "", true); }

	if (t.isSetMission()) 
	{ XMLOstream::Tag tag(_theOstream, _defs._missionKey, t.getMission(), "", true); }
	
	{ XMLOstream::Tag tag(_theOstream, _defs._processorNameKey,    t.getProcName(),           "",              true); }
	{ XMLOstream::Tag tag(_theOstream, _defs._processorVersionKey, t.getProcVersion(),        "",              true); }
	{ XMLOstream::Tag tag(_theOstream, _defs._testKey,             t.isTest() ? "Yes" : "No", "",              true); } // PRQA S 3380
	{ XMLOstream::Tag tag(_theOstream, _defs._diskSpaceKey,        t.getMinDiskSpace(),       "units=\"MB\"",  true); }
	{ XMLOstream::Tag tag(_theOstream, _defs._maxTimeKey,          t.getMaxAllottedTime(),    "units=\"sec\"", true); }

	// Private config section
	{
		XMLOstream::Tag tag(_theOstream, _defs._pconfKey);  // PCONF

		// The list of private config files
		vector<ConfigDescriptor> confList;
		t.getConfigFileList (confList);
		
		ostringstream os;
        size_t cl_size = confList.size() ;
		os << "count=\"" << cl_size << "\""; 
		XMLOstream::Tag tag1(_theOstream, _defs._cfgListKey, "", os.str()); // LIST OF CONFIGS

		for (size_t i=0; i<cl_size; ++i)
		{
			XMLOstream::Tag t1(_theOstream, _defs._cfgKey); // CONFIG_FILE
			// Write the single private configuration entry
			confList[i].acceptVisitor (*this);
		}
			
		// explicitely close array tag before dumping the "Default" one
		tag1.close();

		{ XMLOstream::Tag t1(_theOstream, _defs._defaultKey, t.getDefaultConfigVersIdx(), "", true); }
	}

	// Configuration spaces section
	{
		// The list of private config files
		vector<ConfigSpaceDescriptor> confList;
		t.getConfigSpaceList (confList);

		ostringstream os;
        size_t cl_size = confList.size() ;
		os << "count=\"" << cl_size << "\""; 
		XMLOstream::Tag tag1(_theOstream, _defs._cfgSpaceArrayKey, "", os.str()); // LIST OF CONFIGS SPACES

		for (size_t i=0; i<cl_size; ++i)
		{
			// Write the single configuration space entry
			confList[i].acceptVisitor (*this);
		}
		
	}
	
	//Dynamic Parameter section
	{
		vector<DynProcParam> paramList ;
		t.getDynProcParamList(paramList) ;
		size_t n = paramList.size() ;
		if( n > 0 ) {
			ostringstream os;
			os << "count=\"" << n << "\""; 
			XMLOstream::Tag tag1(_theOstream, _defs._dynParamListKey, "", os.str(),false);//List_of_Dyn_ProcParams
			for (size_t i=0; i<n; ++i)
			{
				// Write the single parameter
				XMLOstream::Tag tag2(_theOstream, _defs._dynParamKey,"","",false) ;
					XMLOstream::Tag tag_name(_theOstream, _defs._dynParamNameKey,paramList[i].getParamName(),"",true) ;
					XMLOstream::Tag tag_type(_theOstream, _defs._dynParamTypeKey,paramList[i].getParamType(),"",true) ;
					XMLOstream::Tag tag_def(_theOstream, _defs._dynParamDefaultKey,paramList[i].getParamDefault(),"",true) ;
				tag2.close() ;
			}
			tag1.close() ;
		}
	}

	// Alternative filter tag
	{
		if (t.isSetAlternativeFilterTag ())
		{
			{ XMLOstream::Tag tag(_theOstream, _defs._alternativeFilterTagKey, t.getAlternativeFilterTag(), "", true); }
		}	
	}

	// Pools // Sequences 
	ostringstream os;
	os << "count=\"" << t.size() << "\""; 
	string arraykey = _defs._poolArrayKey; // LIST OF POOLS is default, even if empty
	if (t.size() > 0) 
	{
	    if (dynamic_cast<Sequence *>(t[0])) // PRQA S 3081
	    {
		    // It is a TaskTable made of sequences
		    arraykey = _defs._sequenceArrayKey; // LIST OF SEQUENCES
	    }
	    else
	    {
		    // It is a TaskTable made of pools
		    // Write this section even if the Pools count is zero (empy task table)
		    arraykey = _defs._poolArrayKey; // LIST OF POOLS
	    }
	}


	// write the LIST OF POOLS/SEQUENCES/TASKS section tag
	_tagsStack.push( new XMLOstream::Tag(_theOstream, arraykey, "", os.str()));


	// AtExitSequences section (not mandatory)
	// if present, will be dumped in exitVisit, after closing the List_of_ tag

	return true;
}



bool
DefaultTTWriter::exitVisit (TaskTable& t)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)


	// close  the List_of_ (pools/sequences/tasks) tag
	delete _tagsStack.top();
	_tagsStack.pop();


	// now, dump AtExitSequences, if any
	// AtExitSequences section (not mandatory)
	{
	    vector<AtExitSequence> s=t.getAtExitSequences();
	    if (not s.empty()) 
	    {
		    ostringstream os;
            size_t s_size = s.size() ;
		    os << "count=\"" << s_size << "\""; 
		    XMLOstream::Tag t1(_theOstream, _defs._atExitArrayKey, "", os.str()); // LIST OF ATEXIT SEQUENCES

		    for (size_t i=0; i<s_size; ++i)
		    {
		        s[i].acceptVisitor (*this);
		    }
		
	    }
	}
	return true;
}

bool 
DefaultTTWriter::visit (ConfigDescriptor& desc)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)


	{ XMLOstream::Tag tag(_theOstream, _defs._cfgVersionKey, desc.getVersion(), "", true); }
	{ XMLOstream::Tag tag(_theOstream, _defs._cfgPathKey,    desc.getPath(),    "", true); }
	
	return true;
}


bool 
DefaultTTWriter::visit (ConfigSpaceDescriptor& desc)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	XMLOstream::Tag t(_theOstream, _defs._cfgSpaceNameKey, desc.getSpaceName(), "", true);
	
	return true;
}



bool
DefaultTTWriter::enterVisit (AtExitSequence&)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

    _tagsStack.push( new XMLOstream::Tag(_theOstream, _defs._atExitKey)); // ATEXIT

    return true;
}



bool 
DefaultTTWriter::visit (AtExitSequence& s)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)


    // Write the single ATEXIT structure
    { XMLOstream::Tag t(_theOstream, _defs._atExitStatusKey, ExitStatus2String(s.getExitStatus()), "", true); }
    { XMLOstream::Tag t(_theOstream, _defs._atExitOverrideKey, s.getOverrideStatus() ? "true" : "false", "", true); } // PRQA S 3380

    return true;
}



bool
DefaultTTWriter::exitVisit (AtExitSequence&)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	// close  the AtExitSequence tag
	delete _tagsStack.top();
	_tagsStack.pop();

	return true;
}


bool 
DefaultTTWriter::enterVisit (Sequence& s)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	_tagsStack.push( new XMLOstream::Tag(_theOstream, _defs._sequenceKey)); // SEQUENCE

	return true;
}


bool 
DefaultTTWriter::visit (Sequence& s)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)


	// From the point of view of writing the TT, the Sequence is defined as a Pool (only the list_of tag is different)
	// Call the Pool writer 
	return this->visit(static_cast<Pool&> (s)); // PRQA S 3081
}


bool
DefaultTTWriter::exitVisit (Sequence& s)
{


	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	// From the point of view of writing the TT, the Sequence is defined as a Pool (only the list_of tag is different)
	return this->exitVisit(static_cast<Pool&>(s)); // PRQA S 3081
}


bool 
DefaultTTWriter::enterVisit (Pool&)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	_tagsStack.push( new XMLOstream::Tag(_theOstream, _defs._poolKey)); // POOL

	return true;
}

bool 
DefaultTTWriter::visit (Pool& p)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)


	// Weight (not mandatory)
	// Quitting Signal (not mandatory)
	// Killing Signal
	// Detached Flag
	// Pools OR Sequences OR Tasks (structures)
	
	if (p.isSetAssignedPercentage()) 
	{ 
		XMLOstream::Tag t(_theOstream, _defs._poolWeightKey, p.getAssignedPercentage(), "", true); 
	}
	
	{ XMLOstream::Tag t(_theOstream, _defs._poolDetachedKey, p.isDetached() ? "true" : "false", "", true); } // PRQA S 3380

	if (p.isSetQuitSignal()) 
	{ 
		XMLOstream::Tag t(_theOstream, _defs._poolQuitSignalKey, p.getQuitSignal(), "", true); 
	}

	{ XMLOstream::Tag t(_theOstream, _defs._poolKillSignalKey, p.getKillSignal(), "", true); }

	
	ostringstream os;
	os << "count=\"" << p.size() << "\""; 
	string arraykey = _defs._taskArrayKey; // LIST OF TASKS is default, even if empty
	if (p.size() > 0) 
	{
	    //if (dynamic_cast<Sequence *>(p.TaskTableVisitable::operator[] (0)))
	    if (dynamic_cast<Sequence *>(p[0])) // PRQA S 3081
	    {
		    // It is a Pool made of sequences
		    arraykey = _defs._sequenceArrayKey; // LIST OF SEQUENCES
	    }
	    //else if (dynamic_cast<Pool *>(p.TaskTableVisitable::operator[] (0)))
	    else if (dynamic_cast<Pool *>(p[0])) // PRQA S 3081
	    {
		    // It is a Pool made of pools
		    arraykey = _defs._poolArrayKey; // LIST OF POOLS
	    }
	    else
	    {
		    // It is a Pool made of Tasks
		    // redoundant setting: they are already in the variable declaration!
		    // left here for the sake of clarity: we won't die for this ;-)
		    arraykey = _defs._taskArrayKey; // LIST OF TASKS
	    }
	}

	_tagsStack.push( new XMLOstream::Tag(_theOstream, arraykey, "", os.str()));

	return true;
}



bool
DefaultTTWriter::exitVisit (Pool&)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	// close  the List_of (task/pool/sequence) tag
	delete _tagsStack.top();
	_tagsStack.pop();

	// close  the Pool / Sequence tag
	delete _tagsStack.top();
	_tagsStack.pop();

	return true;
}



bool
DefaultTTWriter::enterVisit (Task&)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	_tagsStack.push( new XMLOstream::Tag(_theOstream, _defs._taskKey)); // TASK

	return true;
}



bool 
DefaultTTWriter::visit (Task& t)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)



	// Task Structure
	// Detached (Not Mandatory)
	// Quitting_Signal (Not Mandatory)
	// Killing_Signal (Not Mandatory)
	// Weight (Not Mandatory)
	// Posix_ExitCode (Not Mandatory)
	// Name
	// Version
	// Critical
	// Criticality Level
	// File Name
    // Parameters (Structure)
	// Inputs (Structure)
	// Breakpoints (Structure)
	// Outputs (Structure)

	if (t.isSetDetached()) 
	{ 
		XMLOstream::Tag tag(_theOstream, _defs._taskDetachedKey, t.isDetached() ? "true" : "false", "", true);  // PRQA S 3380
	}

	if (t.isSetQuitSignal()) 
	{ 
		XMLOstream::Tag tag(_theOstream, _defs._taskQuitSignalKey, t.getQuitSignal(), "", true); 
	}

	if (t.isSetKillSignal()) 
	{ 
		XMLOstream::Tag tag(_theOstream, _defs._taskKillSignalKey, t.getKillSignal(), "", true); 
	}

	if (t.isSetAssignedPercentage()) 
	{ 
		XMLOstream::Tag tag(_theOstream, _defs._taskWeightKey, t.getAssignedPercentage(), "", true); 
	}

	if (t.isSetPosixExitCode())
	{
		XMLOstream::Tag tag(_theOstream, _defs._taskPosixExitCodeKey, t.isPosixExitCode() ? "true" : "false", "", true); 
	}

	{ XMLOstream::Tag tag(_theOstream, _defs._taskNameKey, t.getName(), "", true); }
	{ XMLOstream::Tag tag(_theOstream, _defs._taskVersionKey, t.getVersion(), "", true); }
	{ XMLOstream::Tag tag(_theOstream, _defs._taskCriticalKey, t.isCritical() ? "true" : "false", "", true); } // PRQA S 3380
	{ XMLOstream::Tag tag(_theOstream, _defs._taskCrLevelKey, t.getCriticalityLevel(), "", true); }
	{ XMLOstream::Tag tag(_theOstream, _defs._taskPathKey, t.getPath(), "", true); }


	return true;
}



bool
DefaultTTWriter::exitVisit (Task& l)
{


	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	// close  the Task tag
	delete _tagsStack.top();
	_tagsStack.pop();

	return true;
}



bool
DefaultTTWriter::enterVisit (InputDescriptor& desc)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)


	ostringstream os;
	if (desc.isSetId()) {
	    os << "id=\"" << desc.getId() << "\""; 
    }
	else if (desc.isSetRef()) {
	    os << "ref=\"" << desc.getRef() << "\""; 
    }
	else {
	    os << "";
    }

	_tagsStack.push( new XMLOstream::Tag(_theOstream, _defs._inputKey, "", os.str())); // INPUT


	return true;
}


bool
DefaultTTWriter::visit (InputDescriptor& desc) // PRQA S 4020
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)


	// first thing: sort alternatives by PreferenceOrder
	// (no requirement on this, but it looks better ;-)
	desc.sortAlternatives(InputDescriptor::SortByPreferenceOrder);

	// Alternatives structure (only if not Ref)
	if (!desc.isSetRef())
	{
		{ XMLOstream::Tag t(_theOstream, _defs._inputModeKey, _theOTM.getTTConfigType(desc.getOrderType()), "", true); }
		{ XMLOstream::Tag t(_theOstream, _defs._inputMandatoryKey, desc.isMandatory() ? "Yes" : "No", "", true); } // PRQA S 3380

		ostringstream os;
		os << "count=\"" << desc.size() << "\""; 
		_tagsStack.push( new XMLOstream::Tag(_theOstream, _defs._alternArrayKey, "", os.str())); // LIST OF ALTERNATIVES
	}
	else {
	    return false; // this will prevent navigating through alternatives
    }

	return true;
	
}



bool
DefaultTTWriter::exitVisit (InputDescriptor& desc)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)


	// close (if needed) the List_of_alternatives tag
	if (!desc.isSetRef())
	{
	    delete _tagsStack.top();
	    _tagsStack.pop();
	}

	// close  the Input tag
	delete _tagsStack.top();
	_tagsStack.pop();

	return true;
}

bool 
DefaultTTWriter::visit (InputAlternative& a)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)


	// Alternative Structure
	// - Order
	// - Origin
	// - Retrieval_Mode
	// - PARAMETERS (TODO?)
	// - T0
	// - T1
	// - Alternative Filter
	// - File_Type
	// - File_Class (not mandatory)
	// - File_Name_Type
	// - Stream
	// - FilterBySatellite (not mandatory)
	// - FilterByMission (not mandatory)
	// - AlternativeActivationTime (not mandatory)

	XMLOstream::Tag tag(_theOstream, _defs._alternKey); // ALTERNATIVE

	{ XMLOstream::Tag t(_theOstream, _defs._alternOrderKey, a.getPreferenceOrder(), "", true); }
	{ XMLOstream::Tag t(_theOstream, _defs._alternOriginKey, InputOrigin2String(a.getOrigin()), "", true); }
	{ XMLOstream::Tag t(_theOstream, _defs._alternModeKey, a.getRetrievalMode(), "", true); }
	{ XMLOstream::Tag t(_theOstream, _defs._alternT0Key, a.getT0(), "units=\"sec\"", true); }
	{ XMLOstream::Tag t(_theOstream, _defs._alternT1Key, a.getT1(), "units=\"sec\"", true); }

	if (a.isSetFilter())
	{
		istringstream is (a.getFilterContent());
		_theOstream.insertXML (is);
	}

	{ XMLOstream::Tag t(_theOstream, _defs._alternTypeKey, a.getFileType(), "", true); }

	if (a.isSetFileClass()) 
	{
		XMLOstream::Tag t(_theOstream, _defs._alternClassKey, a.getFileClass(), "", true);
	}

	{ XMLOstream::Tag t(_theOstream, _defs._alternFileNameTypeKey, FileNameType2String(a.getFileNameType()), "", true); }

	if (a.isSetStream())
	{
		XMLOstream::Tag t(_theOstream, _defs._alternStreamKey, ( a.getStream() ? "True" : "False" ), "", true); // PRQA S 3380
	}

	if (a.isSetFilterBySatellite())
	{
		XMLOstream::Tag t(_theOstream, _defs._alternFilterBySatelliteKey, ( a.getFilterBySatellite() ? "True" : "False" ), "", true); // PRQA S 3380
	}

	if (a.isSetFilterByMission())
	{
		XMLOstream::Tag t(_theOstream, _defs._alternFilterByMissionKey, ( a.getFilterByMission() ? "True" : "False" ), "", true); // PRQA S 3380
	}

	if (a.isSetAltActivationTime())
	{
		XMLOstream::Tag t(_theOstream, _defs._alternActivationTimeKey, a.getAltActivationTime(), "", true); // PRQA S 3380
	}

	return true;
}


bool 
DefaultTTWriter::visit (OutputDescriptor& d)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	XMLOstream::Tag tag(_theOstream, _defs._outputKey); // OUTPUT


	// Output Descriptor structure
	// - Destination
	// - Mandatory
	// - File_Type
	// - FileNameType
	
	{ XMLOstream::Tag t(_theOstream, _defs._outputDestinationKey, OutputDest2String(d.getDestination()), "", true); }
	{ XMLOstream::Tag t(_theOstream, _defs._outputMandatoryKey, d.isMandatory()?"Yes":"No", "", true); } // PRQA S 3380
	{ XMLOstream::Tag t(_theOstream, d.getFileTypeTag(), d.getFileType(), "", true); }
	{ XMLOstream::Tag t(_theOstream, _defs._outputFileNameTypeKey, FileNameType2String(d.getFileNameType()), "", true); }


	return true;
}

bool 
DefaultTTWriter::visit (BreakpointDescriptor& d)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	// The breakpoint descriptor is a single tag element
	XMLOstream::Tag t(_theOstream, _defs._brkptKey, d.getFileType(), "", true);


	return true;
}


bool 
DefaultTTWriter::visit (TaskParameterDescriptor& d)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	// The root tag has a mandatory attribute
    ostringstream os;
	os << _defs._paramMandatoryTag << "=\"" << boolalpha << d.isMandatory() << noboolalpha << "\""; 

    // open the root tag 
    XMLOstream::Tag rtag (_theOstream, _defs._paramKey, "", os.str());

    // write the name (not mandatory)
    bool isSet = false ;
    string name = d.getName(&isSet);
    if (isSet) 
    { 
        XMLOstream::Tag t(_theOstream, _defs._paramNameKey, name, "", true); 
    }

    // write the value (mandatory)
    { XMLOstream::Tag t(_theOstream, _defs._paramValueKey, d.getValue(), "", true); }


	return true;
}

bool
DefaultTTWriter::enterVisit (TaskParameterList& l)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)


    // this is a not mandatory element
    // if there are no parameters do not write the tag

    if (l.size() > 0 ) 
    {
	    ostringstream os;
	    os << "count=\"" << l.size() << "\""; 
        _tagsStack.push( new XMLOstream::Tag(_theOstream, _defs._paramArrayKey, "", os.str())); // LIST OF TASKPARAMETERS 
    }

	return true;
}

bool
DefaultTTWriter::exitVisit (TaskParameterList& l)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

    // this is a not mandatory element
    // if there are no parameters the tag has never been opened

    if (l.size() > 0 ) 
    {
        delete _tagsStack.top();    
        _tagsStack.pop();
    }

	return true;
}


bool
DefaultTTWriter::enterVisit (InputList& l)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	ostringstream os;
	os << "count=\"" << l.size() << "\""; 

	_tagsStack.push( new XMLOstream::Tag(_theOstream, _defs._inputArrayKey, "", os.str())); // LIST OF INPUTS

	return true;
}

bool
DefaultTTWriter::exitVisit (InputList& l)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	delete _tagsStack.top();
	_tagsStack.pop();

	return true;
}

bool
DefaultTTWriter::enterVisit (OutputList& l)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	ostringstream os;
	os << "count=\"" << l.size() << "\""; 

	_tagsStack.push( new XMLOstream::Tag(_theOstream, _defs._outputArrayKey, "", os.str())); // LIST OF OUTPUTS

	return true;
}

bool
DefaultTTWriter::exitVisit (OutputList& l)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)


	delete _tagsStack.top();
	_tagsStack.pop();

	return true;
}

bool
DefaultTTWriter::enterVisit (BreakpointList& l)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	ostringstream os;
	os << "count=\"" << l.size() << "\""; 

	_tagsStack.push( new XMLOstream::Tag(_theOstream, _defs._brkptArrayKey, "", os.str())); // LIST OF BREAKPOINTS

	return true;
}

bool
DefaultTTWriter::exitVisit (BreakpointList& l)
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)


	delete _tagsStack.top();
	_tagsStack.pop();

	return true;
}


_ACS_END_NAMESPACE
