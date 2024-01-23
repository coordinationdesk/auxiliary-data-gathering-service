// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer TaskTable Library $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2014/06/10 15:49:47  marfav
	Added support to DynProcParams and SensingTimeFlag
	
	Revision 5.1  2013/11/19 17:35:12  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 5.0  2013/07/07 18:24:07  marpas
	itroducing libException 5.x standards
	coding best practices applied
	qa warnings removed
	compilation warnings partially removed
	
	Revision 2.12  2012/06/07 11:54:03  marfav
	Code improved removing compiler warnings
	
	Revision 2.11  2012/02/10 17:14:05  marpas
	refactoring in progress
	
	Revision 2.10  2011/05/09 16:10:00  nicvac
	Clone method returns TaskTable pointer.
	
	Revision 2.9  2010/08/03 14:28:06  nicvac
	Task Table plugins management
	
	Revision 2.8  2009/11/25 11:11:34  marfav
	Support to alternative Filter_Def completed
	
	Revision 2.7  2009/11/20 17:26:40  marfav
	Added support for Alternative_Filter_Tag optional field
	
	Revision 2.6  2008/04/22 11:17:09  marfav
	Design improved using the conditioned visitor pattern
	
	Revision 2.4  2006/03/23 14:53:51  fracar
	added support to id/ref in input definition
	
	Revision 2.3  2006/03/09 14:27:44  giucas
	Operators [] fixed
	
	Revision 2.2  2006/03/07 15:13:09  marfav
	Task Table normalization can be disabled when reading from file
	
	Revision 2.1  2006/03/06 13:37:45  marfav
	AtExit Sequences structure support added
	
	Revision 2.0  2006/02/28 10:07:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.26  2006/01/19 18:21:24  marfav
	Addedd support for not mandatory FileClass attribute of Input Alternative structure
	
	Revision 1.25  2005/08/29 09:44:48  fracar
	sorting alternatives by preference order before writing task tables on file
	
	Revision 1.24  2005/06/28 08:22:22  marfav
	Modified Task interface
	The order types are mapped to plain text strings
	
	Revision 1.23  2005/06/08 17:33:41  fracar
	writing to file in support of Task table merger application for IPF
	
	Revision 1.22  2005/06/08 14:42:51  marpas
	exception catching fixed
	
	Revision 1.21  2005/01/13 15:16:30  alefor
	removed the tweak for IPF-Standalone
	
	Revision 1.20  2005/01/13 13:36:07  alefor
	Alternatives can have List_of_File_Names as in JobOrder (IPF-Standalone)
	
	Revision 1.19  2004/12/03 18:03:32  marfav
	Header fixed
	
	Revision 1.18  2004/09/28 15:06:01  marfav
	Added percentage support
	
	Revision 1.17  2004/09/16 12:57:53  marpas
	XMLResourceHandler include removed
	
	Revision 1.16  2004/09/01 09:19:07  marfav
	Added support for tracking exit code of spawned processes
	
	Revision 1.15  2004/04/29 16:58:46  marfav
	does not notify exception in ctor
	
	Revision 1.14  2004/04/15 12:32:02  marfav
	Added messages for ProcessorTracker support
	
	Revision 1.13  2004/04/07 13:08:31  marfav
	Does not use ACE anymore
	Using exStream.h
	
	Revision 1.12  2004/03/22 18:10:07  marfav
	warning due to a compare between signed and unsigned int fixed
	
	Revision 1.11  2004/03/22 17:42:35  marfav
	getConfigFile implementation improved
	
	Revision 1.10  2004/03/11 18:32:57  marfav
	stop method sends the killing signal specified in the Task Table
	kill method sends a signal to the Task or Pool
	
	Revision 1.9  2003/07/10 14:06:12  marfav
	clone method added to exceptions
	
	Revision 1.8  2003/07/04 15:28:08  marfav
	Added support for breakpoints fileNameType field
	
	Revision 1.7  2003/06/05 14:18:26  marfav
	Added Criticality Level to TaskTables
	
	Revision 1.6  2003/05/21 09:04:23  marfav
	Added FileNameType support
	
	Revision 1.5  2003/04/30 14:50:15  marfav
	Align to ICD v2.0
	
	Revision 1.4  2003/04/18 12:49:24  marfav
	Using namespaces removed from .h
	
	Revision 1.3  2003/02/18 09:13:34  fracar
	strengthened exit strategy
	
	Revision 1.2  2003/02/05 13:41:43  fracar
	handling T0 and T1 fields inside the task table
	
	Revision 1.1.1.1  2003/01/31 11:04:58  marfav
	Import libTaskTable
	
	

*/

#include <tlttV.h>

#include <TaskTable.h>
#include <DefaultTTWriter.h>
#include <DefaultTTReader.h>


#include <XMLIstream.h>
#include <XMLOstream.h>
#include <exException.h>
#include <rsPushNSpace.h>
#include <exStream.h>
#include <OrderTypeMgr.h>

#include <sstream>
#include <algorithm>
#include <functional>

#include <iostream>





_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

namespace {

    tlttV version;
}


// class TaskTable

ACS_CLASS_DEFINE_DEBUG_LEVEL(TaskTable)


TaskTable::TaskTable () : 
	TaskTableElement(),	
	_path(),
	_pathSet(false),
	_satellite("CRYOSAT"),
	_mission("CRYOSAT"),
	_processorName(),
	_processorVersion(),
	_isTest(false),
	_minDiskSpace(0),
	_maxAllottedTime(0),
    _myResources(),
	_defaultCfgIdx(0),
	_satelliteSet(false),
	_missionSet(false),
	_processorNameSet(false),
	_processorVersionSet(false),
	_isTestSet(false),
	_minDiskSpaceSet(false),
	_maxAllottedTimeSet(false),
	_myResourcesSet(false),
	_defaultCfgIdxSet(false),
	_alternativeFilterTag ("Filter_Def"),
	_alternativeFilterTagSet (false),
	_isReadOnlyProcessor(false),
    _isSetSensingTimeFlag(false),
    _sensingTimeFlag(false),
    _theConfigs(),
    _theConfigSpaces(),
    _theExitSequences(),
    _theDynProcParams()
{
}

TaskTable::TaskTable (const string& PathName, bool doNormalization) : 
	TaskTableElement(),	
	_path(),
	_pathSet(false),
	_satellite("CRYOSAT"),
	_mission("CRYOSAT"),
	_processorName(),
	_processorVersion(),
	_isTest(false),
	_minDiskSpace(0),
	_maxAllottedTime(0),
    _myResources(),
	_defaultCfgIdx(0),
	_satelliteSet(false),
	_missionSet(false),
	_processorNameSet(false),
	_processorVersionSet(false),
	_isTestSet(false),
	_minDiskSpaceSet(false),
	_maxAllottedTimeSet(false),
	_myResourcesSet(false),
	_defaultCfgIdxSet(false),
	_alternativeFilterTag ("Filter_Def"),
	_alternativeFilterTagSet (false),
	_isReadOnlyProcessor(false),
    _isSetSensingTimeFlag(false),
    _sensingTimeFlag(false),
    _theConfigs(),
    _theConfigSpaces(),
    _theExitSequences(),
    _theDynProcParams()    
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "loading from file " << PathName) ;
    TaskTable::readFromFile (PathName, doNormalization);
}



TaskTable::TaskTable(const TaskTable& t) : 
    TaskTableElement(t),  
	_path(t._path),
	_pathSet(t._pathSet),
	_satellite(t._satellite),
	_mission(t._mission),
	_processorName(t._processorName),
	_processorVersion(t._processorVersion),
	_isTest(t._isTest),
	_minDiskSpace(t._minDiskSpace),
    _maxAllottedTime(t._maxAllottedTime),
	_myResources(t._myResources),
	_defaultCfgIdx(t._defaultCfgIdx),
	_satelliteSet(t._satelliteSet),
	_missionSet(t._missionSet),
	_processorNameSet(t._processorNameSet),
	_processorVersionSet(t._processorVersionSet),
	_isTestSet(t._isTestSet),
	_minDiskSpaceSet(t._minDiskSpaceSet),
    _maxAllottedTimeSet(t._maxAllottedTimeSet),
	_myResourcesSet(t._myResourcesSet),
	_defaultCfgIdxSet(t._defaultCfgIdxSet),
	_alternativeFilterTag(t._alternativeFilterTag),
	_alternativeFilterTagSet(t._alternativeFilterTagSet),
	_isReadOnlyProcessor(t._isReadOnlyProcessor),
    _isSetSensingTimeFlag (t._isSetSensingTimeFlag),
    _sensingTimeFlag (t._sensingTimeFlag),
    _theConfigs(t._theConfigs),
    _theConfigSpaces(t._theConfigSpaces),
    _theExitSequences(t._theExitSequences),
    _theDynProcParams(t._theDynProcParams)

{
}


TaskTable::~TaskTable () throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) // PRQA S 4631
}

TaskTable*
TaskTable::clone() const
{
	TaskTable* newTaskTable = new TaskTable (*this);
	return newTaskTable;
}


TaskTable&                                	
TaskTable::operator = (const TaskTable& t)
{
	if (this != &t)
	{
		_satellite =               t._satellite;
		_mission =                 t._mission;
		_processorName =           t._processorName;
		_processorVersion =        t._processorVersion;
		_isTest =                  t._isTest;
		_minDiskSpace =            t._minDiskSpace;
		_path =                    t._path;
		_myResources =             t._myResources;
		_defaultCfgIdx =           t._defaultCfgIdx;
		_satelliteSet =            t._satelliteSet;
		_missionSet =              t._missionSet;
		_processorNameSet =        t._processorNameSet;
		_processorVersionSet =     t._processorVersionSet;
		_isTestSet =               t._isTestSet;
		_minDiskSpaceSet =         t._minDiskSpaceSet;
        _maxAllottedTime =         t._maxAllottedTime;
        _maxAllottedTimeSet =      t._maxAllottedTimeSet;
		_pathSet =                 t._pathSet;
		_myResourcesSet =          t._myResourcesSet;
		_defaultCfgIdxSet=         t._defaultCfgIdxSet;
		_theConfigs =              t._theConfigs;
		_theConfigSpaces =         t._theConfigSpaces;
		_theExitSequences =        t._theExitSequences;
		_alternativeFilterTag =    t._alternativeFilterTag;
		_alternativeFilterTagSet = t._alternativeFilterTagSet;
		_isReadOnlyProcessor =     t._isReadOnlyProcessor;
        _theDynProcParams =        t._theDynProcParams;
        _sensingTimeFlag =         t._sensingTimeFlag;
        _isSetSensingTimeFlag =    t._isSetSensingTimeFlag;
		TaskTableElement::operator= (t);
	}
	return *this;
}



void
TaskTable::readFromFile (const std::string& filename, bool doNormalization)
{
	// This can raise an exception
	// If exception in caught do not set the _path attribute


	// Start from an empty TaskTable

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "loading the file") ;

	TaskTable t;
//	rsResourceSet rs;
//	XMLIstream  xmlIstream(filename);
//	xmlIstream >> rs;

	
	// Default reader here

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "going with the reader") ;
	DefaultTTReader theReader(filename);
	t.acceptVisitor (theReader);

	
	// Now copy the new object into this one
	*this = t;

	// Normalize the weights if required
	if (doNormalization)
	{

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "going with the normalization") ;

		normalize();
	}
	
	// and finally set the flag
	setPath (filename);
}


void
TaskTable::writeOnFile  (const string& filename, const string& rootTag)
{
	ofstream ofs (filename.c_str());
	XMLOstream xos (ofs, rootTag, "xmlns:a=\"http://www.acsys.it/schemas/IPF\" xmlns:xsi=\"http://www.w3.org/2000/10/XMLSchema-instance\" xsi:schemaLocation=\"http://www.acsys.it/schemas/IPF TaskTable.xsd\"");
	// Default writer here    
	DefaultTTWriter theWriter(xos);
	this->acceptVisitor (theWriter);
}


void 
TaskTable::getLocalInputList (vector<InputDescriptor>& aList, const string& theOrderType, bool erase) const
{
	// No local input list... delete if required and return
	if (erase) 
	{
		aList.clear();
	}
}

void 
TaskTable::getExtLocalInputList (vector<InputDescriptor>& aList, const string& theOrderType, bool erase) const
{
	// No local input list... delete if required and return
	if (erase) 
	{
		aList.clear();
	}
}

void 
TaskTable::getLocalOutputList (vector<OutputDescriptor>& aList, bool erase) const
{
	// No local output list... delete if required and return
	if (erase) 
	{
		aList.clear();
	}
}

void 
TaskTable::getLocalBreakpointList (vector<BreakpointDescriptor>& aList, bool erase) const
{
	// No local breakpoint list... delete if required and return
	if (erase) 
	{
		aList.clear();
	}
} 

void 
TaskTable::getLocalTaskList (vector<Task> & aList, bool erase) const
{
	if (erase) 
	{
		aList.clear();
	}
	
	// No tasks here
}

/**
 * Min Disk Space Management
 */ 

bool
TaskTable::isSetMinDiskSpace() const // PRQA S 4120
{
	return _minDiskSpaceSet;
}

int
TaskTable::getMinDiskSpace() const // PRQA S 4120
{
	return _minDiskSpace;
}

void
TaskTable::setMinDiskSpace(int d)
{
	_minDiskSpaceSet = true;
	_minDiskSpace = d;
}


/**
 * Max Allotted Time Management
 */ 

bool
TaskTable::isSetMaxAllottedTime() const // PRQA S 4120
{
	return _maxAllottedTimeSet;
}

int
TaskTable::getMaxAllottedTime() const // PRQA S 4120
{
	return _maxAllottedTime;
}

void
TaskTable::setMaxAllottedTime(int t)
{
	_maxAllottedTimeSet = true;
	_maxAllottedTime = t;;
}


/**
 * Processor Name Management
 */ 

string
TaskTable::getProcName() const // PRQA S 4120
{
	return _processorName;
}

bool
TaskTable::isSetProcName() const // PRQA S 4120
{
	return _processorNameSet;
}

void
TaskTable::setProcName(const string& s)
{
	_processorNameSet = true;
	_processorName = s;
}

/**
 * Processor Version Management
 */ 

string
TaskTable::getProcVersion() const // PRQA S 4120
{
	return _processorVersion;
}

bool
TaskTable::isSetProcVersion() const // PRQA S 4120
{
	return _processorVersionSet;
}

void
TaskTable::setProcVersion(const string& s) 
{
	_processorVersionSet = true;
	_processorVersion = s;
}

/**
 * Satellite Management
 */ 

bool
TaskTable::isSetSatellite() const // PRQA S 4120
{
	return _satelliteSet;
}
string
TaskTable::getSatellite() const // PRQA S 4120
{
	return _satellite;
}
void
TaskTable::setSatellite(const string& s)
{
	_satelliteSet = true;
	_satellite = s;
}

/**
 * Mission Management
 */ 
string
TaskTable::getMission() const // PRQA S 4120
{
	return _mission;
}

bool
TaskTable::isSetMission() const // PRQA S 4120
{
	return _missionSet;
}

void
TaskTable::setMission(const string& s) 
{
	_missionSet = true;
	_mission = s;
}

/**
 * Test Management
 */ 

bool
TaskTable::isTest() const // PRQA S 4120
{
	return _isTest;
}
bool
TaskTable::isSetTest() const // PRQA S 4120
{
	return _isTestSet;
}
void
TaskTable::isTest(bool b) 
{
	_isTestSet = true;
	_isTest = b;
}

bool 
TaskTable::isSetAlternativeFilterTag () const // PRQA S 4120
{
	return _alternativeFilterTagSet;
}

void 
TaskTable::setAlternativeFilterTag (const string& s)
{
	_alternativeFilterTagSet = true;
	_alternativeFilterTag = s;
}

string 
TaskTable::getAlternativeFilterTag (bool* isSet) const
{
	if (isSet)
	{
		*isSet = _alternativeFilterTagSet;
	}
	return _alternativeFilterTag;
}

bool
TaskTable::isSetSensingTimeFlag () const
{
    return _isSetSensingTimeFlag;
}

void
TaskTable::setSensingTimeFlag (bool b)
{
    _sensingTimeFlag = b;
    _isSetSensingTimeFlag = true;
}

bool
TaskTable::getSensingTimeFlag (bool* isSet) const
{
    if (isSet)
    {
        *isSet = _isSetSensingTimeFlag;
    }
    return _sensingTimeFlag;
}


// STL Helper class
// This class searches a ConfigDescriptor for a particular version
struct CfgVers_eq  // PRQA S 2173, 2175, 2185
{
	string s; // PRQA S 2100

	explicit CfgVers_eq(const string& ss) : s(ss) {}
	bool operator() (const ConfigDescriptor& c) const { return c.getVersion() == s; }
};


string
TaskTable::getConfigFile(const string& Version) const // If empty, the default config is returned // PRQA S 4020
{
	if (Version == "") // Default configuration or null configuration requested
	{
		if (_theConfigs.size() == 0)
		{
			// no private configuration files in the task table
			return "";
		}
		else if ( (unsigned(_defaultCfgIdx) < _theConfigs.size()) && (_defaultCfgIdx >= 0) ) // PRQA S 3081
		{
			// The defaultCfgIdx is valid
			 return _theConfigs[_defaultCfgIdx].getPath(); // PRQA S 3000
		}
		else
		{
			ostringstream os;
			os << "Default Cfg Index [" << _defaultCfgIdx << "] is out of bounds in task table " << _path;
			TaskTableNotFoundException e(os.str());
			ACS_THROW (e) ;
		}
	}
	else // a specific version of the private configuration file is requested
	{
		vector<ConfigDescriptor>::const_iterator p =
			find_if(_theConfigs.begin(), _theConfigs.end(), CfgVers_eq(Version)); // PRQA S 3081
		if (p == _theConfigs.end())
		{
			TaskTableNotFoundException e("Cfg version " + Version + " not found in task table " + _path);
			ACS_THROW (e) ;
		}
		return p->getPath();
	}
}

string
TaskTable::getDefaultConfigVers() const // PRQA S 4020
{
	if (_theConfigs.empty()) {
		return "";
    }
	else {
        return _theConfigs[_defaultCfgIdx].getVersion(); // PRQA S 3000
    }
}

void TaskTable::setDefaultConfigVersIdx (int& idx)
{
	_defaultCfgIdxSet = true;
	_defaultCfgIdx = false;
}

bool TaskTable::isSetDefaultConfigVersIdx() const // PRQA S 4120
{
	return _defaultCfgIdxSet;
}

int TaskTable::getDefaultConfigVersIdx () const // PRQA S 4120
{
	return _defaultCfgIdx;
}

//TaskTableVisitable*
//TaskTable::operator [] (int idx) const
//{
//	return _aggregates.at(idx);
//}   	


void
TaskTable::getConfigSpaceList (vector <ConfigSpaceDescriptor>& aList)  const
{
	aList.clear();
	copy(_theConfigSpaces.begin(), _theConfigSpaces.end(), back_inserter(aList));
}     	

void
TaskTable::getConfigFileList (vector <ConfigDescriptor>& aList)  const
{
	aList.clear();
	copy(_theConfigs.begin(), _theConfigs.end(), back_inserter(aList));
}           	

void
TaskTable::getDynProcParamList (vector <DynProcParam>& aList)  const
{
	aList.clear();
	copy(_theDynProcParams.begin(), _theDynProcParams.end(), back_inserter(aList));
} 

void 
TaskTable::addAtExitSequence (const AtExitSequence& s)
{
	_theExitSequences.push_back(s);
}

void 
TaskTable::push_back (const ConfigDescriptor& d)
{
	_theConfigs.push_back(d);
}

void 
TaskTable::push_back (const ConfigSpaceDescriptor& d)
{
	_theConfigSpaces.push_back(d);
}

void 
TaskTable::push_back (const DynProcParam& d)
{
	_theDynProcParams.push_back(d);
}

const vector<AtExitSequence>& 
TaskTable::getAtExitSequences() const // PRQA S 4120
{
	return _theExitSequences;
}

TaskTable::operator std::vector<ConfigDescriptor>& ()
{
	return _theConfigs; 
}

TaskTable::operator std::vector<ConfigSpaceDescriptor>& ()
{
	return _theConfigSpaces;
}

TaskTable::operator std::vector<AtExitSequence>& ()
{
	return _theExitSequences;
}

TaskTable::operator std::vector<DynProcParam>& ()
{
	return _theDynProcParams;
}

void 
TaskTable::normalize(WeightNormalizer* theNormalizer)
{
	ACS_SMARTPTR<WeightNormalizer> defNormalizer ;

	if (!theNormalizer) 
	{
		// Instantiate the default normalizer
		// the ACS_SMARTPTR will delete it when exiting the scope
		defNormalizer.reset(new WeightNormalizer());
		theNormalizer = defNormalizer.get();
	}

	// Normalize the pools and the sequences
	// empty structure are handled inside the normalization loop
	theNormalizer->normalize (*this);
}


bool
TaskTable::acceptVisit (TaskTableVisitor& theVisitor)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	return theVisitor.visit (*this);
}


bool
TaskTable::acceptExitVisit(TaskTableVisitor& v)
{
	return v.exitVisit(*this);
}


void 
TaskTable::setPath (const string& path)
{
	_path = path;
	_pathSet = true;
}

string 
TaskTable::getPath (bool* isSet) const
{
	if (isSet) {
		*isSet = _pathSet;
    }
	return _path;
}

void 
TaskTable::resetPath ()
{
	_path = "";
	_pathSet = false;
}


_ACS_END_NAMESPACE
