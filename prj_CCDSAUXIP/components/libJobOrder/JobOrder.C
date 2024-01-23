// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. ThinLayer JobOrder Library $

	$Id$

	$Author$

	$Log$
	Revision 2.16  2014/08/01 10:06:17  marfav
	Added support to list of pconf files as per S2SL2P-194
	
	Revision 2.15  2013/07/07 18:25:16  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.14  2012/02/17 13:40:55  marpas
	refactoring
	
	Revision 2.13  2011/05/13 13:08:39  nicvac
	getTaskTable impl.
	
	Revision 2.12  2011/05/13 12:55:59  marfav
	getTaskTable now uses auto_ptr
	
	Revision 2.11  2011/05/10 08:03:19  nicvac
	getTaskTable added.
	
	Revision 2.10  2011/05/09 16:09:05  nicvac
	parseParameters is the method to override in subclasses (not setTaskTable anymore).
	
	Revision 2.9  2010/08/03 14:30:01  nicvac
	Task Table plugins management
	
	Revision 2.8  2009/04/23 14:24:38  nicvac
	Attrib init order fixed.
	
	Revision 2.7  2009/04/23 14:21:06  nicvac
	Attributes initialization in Constructor.
	
	Revision 2.6  2009/02/02 16:43:10  nicvac
	implemented static getOrderIdFromFilename.
	
	Revision 2.5  2008/11/05 14:38:05  marfav
	Addedd support for ExternalOrderId
	
	Revision 2.4  2008/09/15 16:28:52  marfav
	Added attachments support
	
	Revision 2.3  2006/09/22 17:54:25  fracar
	fixes JIRA issues: APF-4 and APF-5
	(symbol overload while loading plug-ins with same global classes redefined)
	
	Revision 2.2  2006/04/21 12:59:16  marfav
	DEFAULT log level support added
	
	Revision 2.1  2006/04/21 12:32:30  marfav
	Added support for indipendent Stdout and Stderr log levels
	the new log levels are defaulted to unknown
	
	Revision 2.0  2006/02/28 10:11:29  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.13  2005/06/28 08:19:28  marfav
	Task have only string definitions for order type
	
	Revision 1.12  2004/12/03 17:47:54  marfav
	Header fixed
	
	Revision 1.11  2004/09/16 13:00:35  marpas
	XMLResourceHandler include removed
	
	Revision 1.10  2004/08/25 13:40:45  marfav
	Using ParametersFileName instead of BlobName in methods
	
	Revision 1.9  2004/08/24 14:13:18  marfav
	Added support for satellite, mission, stations and blob
	
	Revision 1.8  2003/08/26 16:30:32  fracar
	added getConfigSpace method
	
	Revision 1.7  2003/07/10 14:04:56  marfav
	clone method added to exceptions
	
	Revision 1.6  2003/07/04 15:30:51  marfav
	Added the "read" method
	
	Revision 1.5  2003/04/30 14:55:33  marfav
	Align to ICD v2.0
	
	Revision 1.4  2003/04/18 16:11:01  fracar
	align to new compiler version
	
	Revision 1.3  2003/04/18 12:52:21  marfav
	Using namespaces removed from .h
	
	Revision 1.2  2003/02/10 18:51:51  fracar
	got rid of ^M
	
	Revision 1.1.1.1  2003/01/31 15:00:48  marfav
	Import libJobOrder
	
	


*/

#include <tljoV.h>
#include <JobOrder.h>

#include <TaskTable.h>

#include <RegExp.h>

#include <fstream>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace {
    tljoV version;
}

string LogLevel2String(joLogLevel l) // PRQA S 4020
{
  switch (l)
  {
    case LogLevelERROR:		{ return "ERROR"; }
    case LogLevelWARNING:	{ return "WARNING"; }
    case LogLevelPROGRESS:	{ return "PROGRESS"; }
    case LogLevelINFO:		{ return "INFO"; }
    case LogLevelDEBUG:		{ return "DEBUG"; }
    case LogLevelDEFAULT:	{ return "DEFAULT"; }
    default:			{ return "Unknown"; }
  }
}

joLogLevel string2LogLevel(const string& s, joLogLevel def) // PRQA S 4020
{
       if (s == "ERROR")    { return LogLevelERROR; }
  else if (s == "WARNING")  { return LogLevelWARNING; }
  else if (s == "PROGRESS") { return LogLevelPROGRESS; }
  else if (s == "INFO")     { return LogLevelINFO; }
  else if (s == "DEBUG")    { return LogLevelDEBUG; }
  else if (s == "DEFAULT")  { return LogLevelDEFAULT; }
  else	                    { return def; }
}



// JobOrderAttachmentDescriptor CLASS - BEGIN

JobOrderAttachmentDescriptor::JobOrderAttachmentDescriptor() :
	_type(),
	_path()
{
	// Empty CTOR
}

JobOrderAttachmentDescriptor::JobOrderAttachmentDescriptor(const string& type, const string& path) :
	_type (type),
	_path (path)
{
	// Initialized CTOR
}

JobOrderAttachmentDescriptor::JobOrderAttachmentDescriptor (const JobOrderAttachmentDescriptor& d) :
	_type(d._type),
	_path(d._path)
{
	// empty
}

JobOrderAttachmentDescriptor& JobOrderAttachmentDescriptor::operator= (const JobOrderAttachmentDescriptor& d)
{
	if (&d != this)
	{
		_type = d._type;
		_path = d._path;
	}
	return *this;
}

string JobOrderAttachmentDescriptor::getType() const // PRQA S 4120
{
	return _type;
}

string JobOrderAttachmentDescriptor::getPath() const // PRQA S 4120
{
	return _path;
}

void JobOrderAttachmentDescriptor::setType (const string& t) // PRQA S 4121
{
	_type = t;
}

void JobOrderAttachmentDescriptor::setPath (const std::string& p) // PRQA S 4121
{
	_path = p;
}

// JobOrderAttachmentDescriptor CLASS - END


JobOrder::JobOrder()
  : // just set sensible defaults
    _configSpaces(),
    _attachments(),
    _logLevel(LogLevelINFO), // default as per PROC_ICD
    _outLogLevel (LogLevelDEFAULT),
    _errLogLevel (LogLevelDEFAULT),
    _brkEnabled(false),      // default as per PROC_ICD
    _isTest(false),
    _isTroubleshooting(false),
    _configList(),
	_tasks(),
    _start(),
    _stop(),
	_workingDir("."),    // defaults to current dir
	_orderType(),
	_procName(),
    _procVersion(),
    _procStation(),
    _stationName(),
    _satellite(),
    _mission(),
    _parametersFileName(),
    _orderId(0),
    _externalOrderId(),
	_taskTable(0)
  {}

JobOrder::JobOrder(const JobOrder& j) :
    _configSpaces(),
    _attachments(),
    _logLevel(j._logLevel),
    _outLogLevel(j._outLogLevel),
    _errLogLevel(j._errLogLevel),
    _brkEnabled(j._brkEnabled),
    _isTest(j._isTest),
    _isTroubleshooting(j._isTroubleshooting),
    _configList(j._configList),
	_tasks(),
    _start(j._start),
    _stop(j._stop),
    _workingDir(j._workingDir),
    _orderType(j._orderType),
    _procName(j._procName),
    _procVersion(j._procVersion),
    _procStation(j._procStation),
    _stationName(j._stationName),
    _satellite(j._satellite),
    _mission(j._mission),

    _parametersFileName(j._parametersFileName),
    _orderId(j._orderId),
    _externalOrderId(j._externalOrderId),
    _taskTable(j._taskTable ? j._taskTable->clone() : 0) // PRQA S 3380
{
    copy (j._configSpaces.begin(), j._configSpaces.end(), back_inserter (_configSpaces));
    copy (j._attachments.begin(), j._attachments.end(), back_inserter (_attachments));
    copy (j._tasks.begin(), j._tasks.end(), back_inserter (_tasks));
    // copy(j.begin(), j.end(), back_inserter(*this));
}

JobOrder::~JobOrder() throw() { delete _taskTable; }

/* MP: wrong signature: removed !
JobOrder&
JobOrder::operator >> (ofstream& o) // PRQA S 4222
{
  write(o);
  return *this;
}

JobOrder&
JobOrder::operator << (ifstream& i) // PRQA S 4222
{
  read (i);
  return *this;
}

JobOrder&
JobOrder::operator << (XMLIstream& i) // PRQA S 4222
{
  read (i);
  return *this;
}
*/

void
JobOrder::reset()
{
    _configSpaces.clear();
    _attachments.clear();

    _logLevel=LogLevelINFO;
    _outLogLevel=LogLevelDEFAULT;
    _errLogLevel=LogLevelDEFAULT;
    _brkEnabled=false;
    _isTest=false;
    _isTroubleshooting=false;
    _start = DateTime();
    _stop = DateTime();
    _configList.clear();
	_tasks.clear();
    _workingDir=".";
	_orderType="";
    _procName="";
    _procVersion="";
    _procStation="";
    _stationName="";
    _satellite="";
    _mission="";
    _parametersFileName="";
    _orderId=0;
    _externalOrderId="";

	if ( _taskTable ) {
		delete _taskTable;
		_taskTable=0;
    }
}

JobOrder&
JobOrder::operator = (const JobOrder& j)
{
  if (this != &j)
  {
    _brkEnabled = j._brkEnabled;
    _isTest = j._isTest;
    _isTroubleshooting = j._isTroubleshooting;
    _start = j._start;
    _stop = j._stop;
    _logLevel = j._logLevel;
    _outLogLevel = j._outLogLevel;
    _errLogLevel = j._errLogLevel;
    _orderType = j._orderType;
//     _configList.clear();
//     copy (j._configList.begin(), j._configList.end(), back_inserter (_configList));
	_configList = j._configList;
	_tasks = j._tasks;

//     _configSpaces.clear();
//     copy (j._configSpaces.begin(), j._configSpaces.end(), back_inserter (_configSpaces));
	_configSpaces = j._configSpaces;

//     _attachments.clear();
//     copy (j._attachments.begin(), j._attachments.end(), back_inserter (_attachments));
	_attachments = j._attachments;

    _procStation = j._procStation;
    _stationName = j._stationName;
    _procName = j._procName;
    _procVersion = j._procVersion;
    _workingDir = j._workingDir;
    _satellite = j._satellite;
    _mission = j._mission;
    _parametersFileName = j._parametersFileName;
    _orderId = j._orderId;
    _externalOrderId = j._externalOrderId;

	if ( _taskTable )
	{
		delete _taskTable;
		_taskTable = 0;
	}

	if (j._taskTable)
	{
		_taskTable = j._taskTable->clone();
	}

  }

    return *this;
}


void
JobOrder::setOrderId (unsigned int id) // PRQA S 4121
{
	_orderId = id;
}

unsigned int
JobOrder::getOrderId () const // PRQA S 4120
{
	return _orderId;
}

unsigned int JobOrder::getOrderIdFromFilename(const std::string& jobOrderFilename) 
{

    //Default is 0 (0 returned also if filename does not match)
    unsigned int orderIdToReturn(0);

    const string regExp("JobOrder\\.([[:digit:]]+)\\.xml");

    RegExp re; 
    re.setReg( regExp );

    const unsigned short orderId(1); //Element to get in RegExp
    RegExp::KeyMatches keyMatches;

    bool res = re.match(jobOrderFilename, keyMatches);
    if ( res ) 
    {
        istringstream buffer( keyMatches[orderId].getMatched() );
        buffer >> orderIdToReturn;
    } 

    return orderIdToReturn;
}


void 
JobOrder::setTaskTable(TaskTable* taskTable) 
{
	if ( _taskTable ) {
		delete _taskTable;
    }

	_taskTable = taskTable->clone();

	parseParameters();
}

ACS_SMARTPTR<TaskTable> 
JobOrder::getTaskTable() const // PRQA S 4020
{
	if (_taskTable)
	{
		return ACS_SMARTPTR<TaskTable> (_taskTable->clone()); // PRQA S 3081
	}
	else
	{
		return ACS_SMARTPTR<TaskTable>() ; // PRQA S 3081
	}

}


void 
JobOrder::parseParameters() 
{
	return;
}


void
JobOrder::setLogLevel(joLogLevel l) // PRQA S 4121
{
  _logLevel = l;
}

joLogLevel
JobOrder::getLogLevel() const // PRQA S 4120
{
  return _logLevel;
}

void
JobOrder::setOutLogLevel(joLogLevel l) // PRQA S 4121
{
  _outLogLevel = l;
}

joLogLevel
JobOrder::getOutLogLevel() const // PRQA S 4120
{
  return _outLogLevel;
}

void
JobOrder::setErrLogLevel(joLogLevel l) // PRQA S 4121
{
  _errLogLevel = l;
}

joLogLevel
JobOrder::getErrLogLevel() const // PRQA S 4120
{
  return _errLogLevel;
}

void
JobOrder::setOrderType(const string& s) // PRQA S 4121
{
  _orderType = s;
}

const string &
JobOrder::getOrderType() const // PRQA S 4120
{
  return _orderType;
}


void
JobOrder::brkEnabled(bool b) // PRQA S 4121
{
  _brkEnabled = b;
}

bool
JobOrder::brkEnabled() const // PRQA S 4120
{
  return _brkEnabled;
}

void
JobOrder::isTest(bool b) // PRQA S 4121
{
  _isTest = b;
}

bool
JobOrder::isTest() const // PRQA S 4120
{
  return _isTest;
}

void
JobOrder::isTroubleshooting(bool b) // PRQA S 4121
{
  _isTroubleshooting = b;
}

bool
JobOrder::isTroubleshooting() const // PRQA S 4120
{
  return _isTroubleshooting;
}


void
JobOrder::addConfigSpace (const ConfigSpaceDescriptor& c)
{
	_configSpaces.push_back(c);
}

void
JobOrder::getConfigSpaces (vector<ConfigSpaceDescriptor>& aList) const
{
  aList.clear();
  copy(_configSpaces.begin(), _configSpaces.end(), back_inserter(aList));
}

void 
JobOrder::addAttachment (const JobOrderAttachmentDescriptor& a)
{
	_attachments.push_back(a);
}

void
JobOrder::getAttachments (std::vector<JobOrderAttachmentDescriptor>& aList) const
{
  aList.clear();
  copy(_attachments.begin(), _attachments.end(), back_inserter(aList));
}

JobOrderAttachmentDescriptor 
JobOrder::getAttachmentDescriptor (const std::string& type) const
{
	vector<JobOrderAttachmentDescriptor>::const_iterator i =
		find_if ( _attachments.begin(), _attachments.end(), AttachmentTypeEquals (type)); // PRQA S 3081

	if ( i == _attachments.end() )
	{
		ACS_THROW ( JobOrderException ( "Attachment type not found [" + type + "]" )); // PRQA S 3081
	}

	return *i;
}


void
JobOrder::setConfig(const string& c) // PRQA S 4121
{
  // This will clear the list and set the firsst element
  _configList.clear();
  _configList.push_back (c);
}

std::string 
JobOrder::getConfig() const // PRQA S 4120
{
  string retval ("");
  if (0 != _configList.size())
  {
    retval = _configList.at(0);
  }
  return retval;  
}

void JobOrder::setConfigList (const std::vector<std::string>& v)
{
    _configList.clear();
    copy (v.begin(), v.end(), back_inserter(_configList));
}

void JobOrder::addConfigFile (const std::string& s)
{
    _configList.push_back (s);
}

void JobOrder::getConfigList( std::vector<std::string>& l) const
{
    l.clear();
    copy (_configList.begin(), _configList.end(), back_inserter(l));
}



void
JobOrder::setTimeInterval(const DateTime& start, const DateTime& stop)
{
  _start = start;
  _stop = stop;
}

void
JobOrder::getTimeInterval(DateTime& start, DateTime& stop) const
{
  start = _start;
  stop = _stop;
}

void
JobOrder::setWorkingDir(const string& c) // PRQA S 4121
{
  _workingDir = c;
}

const std::string &
JobOrder::getWorkingDir() const // PRQA S 4120
{
  return _workingDir;
}

void
JobOrder::setProcName(const string& c) // PRQA S 4121
{
  _procName = c;
}

const std::string &
JobOrder::getProcName() const // PRQA S 4120
{
  return _procName;
}

void
JobOrder::setProcVersion(const string& c) // PRQA S 4121
{
  _procVersion = c;
}

const string &
JobOrder::getProcVersion() const // PRQA S 4120
{
  return _procVersion;
}

void
JobOrder::setProcStation(const string& c) // PRQA S 4121
{
  _procStation = c;
}

const std::string &
JobOrder::getProcStation() const // PRQA S 4120
{
  return _procStation;
}


void
JobOrder::setStationName (const string& n) // PRQA S 4121
{
	_stationName = n;
}

const std::string &
JobOrder::getStationName () const // PRQA S 4120
{
	return _stationName;
}


void
JobOrder::setSatellite(const string& s) // PRQA S 4121
{
  _satellite = s;
}

const std::string &
JobOrder::getSatellite() const // PRQA S 4120
{
  return _satellite;
}

void
JobOrder::setMission(const string& m) // PRQA S 4121
{
  _mission = m;
}

const std::string &
JobOrder::getMission() const // PRQA S 4120
{
  return _mission;
}

void
JobOrder::setParametersFileName(const string& b) // PRQA S 4121
{
  _parametersFileName = b;
}

const std::string &
JobOrder::getParametersFileName() const // PRQA S 4120
{
  return _parametersFileName;
}

void 
JobOrder::setExternalOrderId (const string& id) // PRQA S 4121
{
  _externalOrderId = id;
}

const std::string & 
JobOrder::getExternalOrderId () const // PRQA S 4120
{
  return _externalOrderId;
}


ConfigSpaceDescriptor
JobOrder::getConfigSpace (const string& s) const
{
	vector<ConfigSpaceDescriptor>::const_iterator i =
		find_if ( _configSpaces.begin(), _configSpaces.end(), SpaceNameEquals (s)); // PRQA S 3081

	if ( i == _configSpaces.end() )
	{
		ACS_THROW ( JobOrderException ( "Configuration Space not found [" + s + "]")); // PRQA S 3081
	}

	return *i;
}

std::vector<Task> JobOrder::getTasks() const
{
	return _tasks;
}

Task
JobOrder::getTask (const string& s) const // PRQA S 4120
{
	// returns a copy of the required task
	return (*this)[s];
}

const Task&
JobOrder::operator [] (const std::string& s) const
{

	vector<Task>::const_iterator i =
		find_if ( _tasks.begin(), _tasks.end(), TaskNameEquals (s)); // PRQA S 3081

	if ( i == _tasks.end() )
	{
		ACS_THROW ( JobOrderException ( "Task not found [" + s + "]" )); // PRQA S 3081
	}

	return *i;
}
const Task& JobOrder::operator [] (const int& s) const
{

	return _tasks[s];
}

const Task& JobOrder::at (const std::string& s) const
{
	return (*this)[s];
}
const Task& JobOrder::at (const int & s) const
{
	return (*this)[s];
}

size_t JobOrder::size() const
{
	return _tasks.size();
}

void JobOrder::clear()
{
	_tasks.clear();
}
void JobOrder::push_back(Task & t)
{
	_tasks.push_back(t);
}
JobOrder::operator const std::vector<Task>& () const
{
	return _tasks;
}
JobOrder::operator std::vector<Task>& () 
{
	return _tasks;
}
std::vector<Task>::const_iterator JobOrder::begin() const
{
	return _tasks.begin();
}
std::vector<Task>::const_iterator JobOrder::end() const
{
	return _tasks.end();
}
std::vector<Task>::iterator JobOrder::begin() 
{
	return _tasks.begin();
}
std::vector<Task>::iterator JobOrder::end() 
{
	return _tasks.end();
}

/*
 *   helper classes STL algos
 */

JobOrder::SpaceNameEquals::SpaceNameEquals (const string& s) :_s(s)
{}

bool 
JobOrder::SpaceNameEquals::operator() (const ConfigSpaceDescriptor& c) const
{
	return (c.getSpaceName() == _s);
}

JobOrder::TaskNameEquals::TaskNameEquals (const string& s): _s(s)
{}

bool 
JobOrder::TaskNameEquals::operator () (const Task& t) const
{
	return (t.getName() == _s);
}


JobOrder::AttachmentTypeEquals::AttachmentTypeEquals (const std::string& s) : _s(s)
{}

bool 
JobOrder::AttachmentTypeEquals::operator () (const JobOrderAttachmentDescriptor& d) const
{
	return (d.getType() == _s);
}

/*
 *   helper classes for JobOrder xml file writing
 *   (to be used inside the write() method implementation
 */


unsigned int JobOrder::xml_indent::_indentCount = 0;

JobOrder::xml_section_tag::xml_section_tag(ofstream& o, const string & s, const string & a)
      : xml_indent(), _o(o), _s(s) // PRQA S 2528
{
	// indentation
	for (unsigned int i=0; i<_indentCount; i++) {
		o << "\t";
    }

	if (a != "") {
		o << "<" << s << " " << a << ">\n";
    }
	else {
		o << "<" << s << ">\n";
    }

	_indentCount++;

}

JobOrder::xml_section_tag::~xml_section_tag() throw() 
{
    // PRQA S 4631 L1
    try {
	    if (_indentCount>0) {
		    _indentCount--;
        }

	    // indentation
	    for (unsigned int i=0; i<_indentCount; i++) {
		    _o << "\t";
        }

	    _o << "</" << _s << ">\n";
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    // PRQA L:L1
}

JobOrder::xml_element_tag::xml_element_tag(ofstream& o, const string &s, const string & v) :
    xml_indent()

{
	// indentation
	for (unsigned int i=0; i<_indentCount; i++) {
		o << "\t";
    }
	o << "<" << s << ">" << v << "</" << s << ">\n";
}
JobOrder::xml_element_tag::xml_element_tag(ofstream& o, const string &s, unsigned int u) :
    xml_indent()
{
	// indentation
	for (unsigned int i=0; i<_indentCount; i++) {
		o << "\t";
    }
	o << "<" << s << ">" << u << "</" << s << ">\n";
}

_ACS_END_NAMESPACE
