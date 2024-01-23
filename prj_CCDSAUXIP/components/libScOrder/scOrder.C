// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. TL Scheduler Order Library $

	$Id$

	$Author$

	$Log$
	Revision 1.13  2017/10/19 09:55:44  marfav
	ECICMF-76
	Added support to resource manager key
	
	Revision 1.12  2017/09/27 08:35:08  marfav
	ECICMF-94
	Added parameters for passing REST URL and ClientId information in the XML file
	
	Revision 1.11  2017/03/17 15:31:16  marfav
	APF-411
	Added the Posix exit code management flag
	
	Revision 1.10  2014/01/31 15:36:48  marfav
	S2PDGS-603 Processor messages can be filtered by configuration
	
	Revision 1.9  2013/07/07 17:37:19  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 1.8  2012/06/06 15:07:44  marfav
	CTOR initialization list added
	
	Revision 1.7  2010/08/03 14:31:50  nicvac
	Added procname and procversion to support Task Table plugins management
	
	Revision 1.6  2007/07/17 08:26:21  marfav
	Added ThreadSafe guards
	
	Revision 1.5  2007/07/16 17:09:12  marfav
	Added support for filetype vs regexp map
	
	Revision 1.4  2005/07/26 13:23:08  fracar
	fixed problem with getCmdLineParams call
	
	Revision 1.3  2005/07/25 11:26:20  fracar
	added command line parameters management
	
	Revision 1.2  2004/12/03 17:56:12  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/09/15 08:12:08  marfav
	Import libScOrder
	
	

*/


#include <scOrder.h>

#include <XMLIstream.h>
#include <XMLOstream.h>
#include <exException.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>

#include <sstream>
#include <algorithm>
#include <functional>

#include <exStream.h>

#include <scoV.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

namespace {
    scoV version ;

const string _logfileKey            = "Log_File";
const string _procNameKey			  = "Processor_Name";
const string _procVersionKey		  = "Processor_Version";
const string _tasktableKey          = "Task_Table";
const string _joborderKey           = "Job_Order";
const string _joborderParamArrayKey = "List_of_Job_Order_Params";
const string _joborderParamCountKey = "List_of_Job_Order_Params.count.value";
const string _joborderParamKey      = "Job_Order_Param";
const string _cmdLinesArrayKey      = "List_of_Command_Lines";
const string _cmdLinesCountKey      = "List_of_Command_Lines.count.value";
const string _cmdLineKey            = "Command_Line";
const string _taskNameKey           = "Task_Name";
const string _cmdLineParamArrayKey  = "List_of_Cmd_Line_Params";
const string _cmdLineParamCountKey  = "List_of_Cmd_Line_Params.count.value";
const string _cmdLineParamKey       = "Cmd_Line_Param";
const string _workingdirKey         = "Working_Dir";
const string _createwdKey           = "Create_Working_Dir";
const string _doCleanupKey          = "Cleanup_on_Exit";
const string _cleanErrorKey         = "Apply_Cleanup_on_Error";
const string _killbackgroundKey     = "Kill_Background";
const string _schedulerLogFileKey   = "Scheduler_Log_File";
const string _trackerFileNameKey    = "Tracker_File";
const string _satelliteKey          = "Satellite";
const string _missionKey            = "Mission";

const string _regexpMapArrayKey     = "List_of_RegExp_Associations";
const string _regexpMapCountKey     = "List_of_RegExp_Associations.count.value";
const string _regexpMapKey          = "RegExp_Association";
const string _mapFileTypeKey        = "File_Type";
const string _mapRegExpKey          = "RegExp";

const string _logProcMessagesKey    = "Log_Processor_Messages";
const string _posixExitCodeKey      = "Posix_Tasks_ExitCode";

const string _resManagerURLKey      = "ResourceManagerServiceURL";
const string _resManagerClientIdKey = "ResourceManagerClientId";
const string _resManagerKeyKey      = "ResourceManagerKey";
}


scOrder::scOrder() :
	_mutex(),
	_name(),
	_procName(),
	_procVersion(),
	_tt(),
	_jo(),
	_path(),
	_schedulerLogName(),
	_trackerFileName(),
	_satellite(),
	_mission(),
	_joParams(),
	_cmdParams(),
	_regExpAssociationMap(),
	_create_dir(),
	_cleanup_dir(),
	_kill_bckgrnd(),
	_cleanup_error(false),
	_logProcMessages(true),
	_posixExitCode(true),
	_resManagerKey(),
	_resManagerUrl(),
	_resManagerClientId()
{
}


scOrder::scOrder(const string& aPath) :
	_mutex(),
	_name(),
	_procName(),
	_procVersion(),
	_tt(),
	_jo(),
	_path(),
	_schedulerLogName(),
	_trackerFileName(),
	_satellite(),
	_mission(),
	_joParams(),
	_cmdParams(),
	_regExpAssociationMap(),
	_create_dir(),
	_cleanup_dir(),
	_kill_bckgrnd(),
	_cleanup_error(),
	_logProcMessages(true),
	_posixExitCode(true),
	_resManagerKey(),
	_resManagerUrl(),
	_resManagerClientId()
{

    Lock lock(_mutex) ;

    rsResourceSet rs;
    XMLIstream  xmlIstream(aPath);

    xmlIstream >> rs;


    string s;
    rs.getValue(_logfileKey, s);
    setLogFileName(s);

	if ( rs.hasKey(_procNameKey) ) {
	    string s1 ;
		rs.getValue(_procNameKey, s1);
	    setProcName(s1);
    }

	if ( rs.hasKey(_procVersionKey ) ) {
	    string s1 ;
		rs.getValue(_procVersionKey, s1);
	    setProcVersion(s1);
    }

    rs.getValue(_tasktableKey, s);
    setTaskTable(s);

    rs.getValue(_joborderKey, s);
    setJobOrder(s);


    if (rs.hasKey (_joborderParamArrayKey))
    {
	    size_t prmCount = 0;

	try
	{
	    rs.getValue(_joborderParamCountKey, prmCount);

	    // next block {} necessary to handle rs namespaces
	    {
		    rsPushNSpace nspace(rs, _joborderParamArrayKey);

		    if (rs.hasKey(_joborderParamKey))
		    {
		        vector<string> prms;
		        rs.getArray(_joborderParamKey, prms);
		        setJobOrderParams(prms);
		    }
	    }
	}
	catch(rsResourceSet::NotFoundException &toCatch)
	{
	    ACS_LOG_NOTIFY_EX(toCatch);
	    // ignore and do not load parameters
	}

    }


    if (rs.hasKey(_cmdLinesArrayKey))
    {
	    size_t cmdLinesCount = 0;

	    try
	    {
	        rs.getValue(_cmdLinesCountKey, cmdLinesCount);

	        {
		        rsPushNSpace nspace(rs, _cmdLinesArrayKey);

		        for(size_t i=0; i<cmdLinesCount; i++)
		        {
		            ostringstream os;
		            os << _cmdLineKey << "["<<i<<"]";
		            string prefix=os.str();
		            rsPushNSpace nspace1(rs, prefix);

		            string task;
		            rs.getValue(_taskNameKey, task);

		            rsPushNSpace nspace2(rs, _cmdLineParamArrayKey);
		            vector<string> prms;
		            rs.getArray(_cmdLineParamKey, prms);

		            setCmdLineParams(task, prms);
		        }
	        }
	    }
	    catch(rsResourceSet::NotFoundException &toCatch)
	    {
	        ACS_LOG_NOTIFY_EX(toCatch);
	        // ignore and do not load parameters
	    }
    }

    setSchedulerLogName (s);

    rs.getValue(_workingdirKey, s);
    setBaseDir(s);

    bool b = false ;
    rs.getValue(_createwdKey, b);
    doICreateDir(b);

    rs.getValue(_doCleanupKey, b);
    doICleanupOnExit(b);

    rs.getValue(_killbackgroundKey, b);
    doIKillBackgroundTasks(b);

    rs.getValue(_satelliteKey, s);
    setSatellite (s);

    rs.getValue(_missionKey, s);
    setMission (s);


    if (rs.hasKey (_schedulerLogFileKey))
    {
        rs.getValue (_schedulerLogFileKey, s);
        setSchedulerLogName (s);
    }
    else
    {
        // _schedulerLogFileKey is an optional parameter
        // default value is the null string.
        // Applications using scOrder have to provide default log name generation
        s = "";
        setSchedulerLogName (s);
    }

    if (rs.hasKey (_trackerFileNameKey))
    {
      rs.getValue (_trackerFileNameKey, s);
      setTrackerFileName (s);
    }
    else
    {
      // _trackerFileNameKey is an optional parameter
      // default value is the null string.
      // Applications using scOrder have to provide default tracker file name generation
      s = "";
      setTrackerFileName (s);
    }

	// Not Mandatory Section 
	// FileType vs Regular Expression association map
	if (rs.hasKey(_regexpMapArrayKey))
	{
		size_t associationsCount = 0;
	
		try
		{
			rs.getValue(_regexpMapCountKey, associationsCount);
	
			{
				rsPushNSpace nspace(rs, _regexpMapArrayKey);
	
				for(size_t i=0; i<associationsCount; i++)
				{
					ostringstream os;
					os << _regexpMapKey << "["<<i<<"]";
					string prefix=os.str();
					rsPushNSpace nspace3(rs, prefix);
	
					string filetype;
					rs.getValue (_mapFileTypeKey, filetype);
					string regexp;
					rs.getValue (_mapRegExpKey, regexp);
	
					setRegExpAssociation (filetype, regexp);
				}
			}
		}
		catch(rsResourceSet::NotFoundException &toCatch)
		{
			ACS_LOG_NOTIFY_EX(toCatch);
			// ignore and do not load parameters
		}
	}
    
    if (rs.hasKey (_logProcMessagesKey))
    {
        // Not mandatory and defaulted to true
        rs.getValue (_logProcMessagesKey, _logProcMessages);
    }

    if (rs.hasKey (_posixExitCodeKey))
    {
        // Not mandatory and defaulted to true
        rs.getValue (_posixExitCodeKey, _posixExitCode);
    }

	// Core Based execution parameters are optional and defaulted to empty strings
    if (rs.hasKey (_resManagerURLKey))
    {
        // Not mandatory and defaulted to true
        rs.getValue (_resManagerURLKey, _resManagerUrl );
    }
    if (rs.hasKey (_resManagerClientIdKey))
    {
        // Not mandatory and defaulted to true
        rs.getValue (_resManagerClientIdKey, _resManagerClientId );
    }

    // Not mandatory, defaulted to false
    _cleanup_error = false;
    rs.getValue (_cleanErrorKey, _cleanup_error, false);
    
    // Not mandatory field
    _resManagerKey="";
    rs.getValue (_resManagerKeyKey, _resManagerKey, false);
}

scOrder::scOrder(const scOrder& o) :
	_mutex(),
	_name(),
	_procName(),
	_procVersion(),
	_tt(),
	_jo(),
	_path(),
	_schedulerLogName(),
	_trackerFileName(),
	_satellite(),
	_mission(),
	_joParams(),
	_cmdParams(),
	_regExpAssociationMap(),
	_create_dir(),
	_cleanup_dir(),
	_kill_bckgrnd(),
	_cleanup_error(),
	_logProcMessages(true),
	_posixExitCode(true),
	_resManagerKey(),
	_resManagerUrl(),
	_resManagerClientId()
{
    *this = o;
}

scOrder&
scOrder::operator = (const scOrder& o)
{
	if (this != &o)
	{
		Lock l1 (_mutex );
		Lock l2 (o._mutex);

		_cleanup_dir          = o._cleanup_dir;
		_cleanup_error        = o._cleanup_error;
		_create_dir           = o._create_dir;
		_kill_bckgrnd         = o._kill_bckgrnd;
		_jo                   = o._jo;
		_name                 = o._name;
		_path                 = o._path;
		_procName			  = o._procName;
		_procVersion		  = o._procVersion;
		_tt                   = o._tt;
		_schedulerLogName     = o._schedulerLogName;
		_trackerFileName      = o._trackerFileName;
		_satellite            = o._satellite;
		_mission              = o._mission;
		_joParams             = o._joParams;
		_cmdParams            = o._cmdParams;
		_regExpAssociationMap = o._regExpAssociationMap;
		_logProcMessages      = o._logProcMessages;
		_posixExitCode        = o._posixExitCode;
		_resManagerKey        = o._resManagerKey;
		_resManagerUrl        = o._resManagerUrl;
		_resManagerClientId   = o._resManagerClientId;
	}

	return *this;
}                                                                                	

void scOrder::setLogProcMessages (bool b)
{
	Lock lock(_mutex) ;
    _logProcMessages=b;
}

bool scOrder::getLogProcMessages () const
{
	Lock lock(_mutex) ;
    return _logProcMessages;
}

void scOrder::setPosixExitCode (bool b)
{
	Lock lock(_mutex) ;
    _posixExitCode=b;
}

bool scOrder::getPosixExitCode () const
{
	Lock lock(_mutex) ;
    return _posixExitCode;
}

// ECICMF-94
// Core Based execution REST Service parameters get/set
const std::string& scOrder::getResManagerClientId() const {
	Lock lock(_mutex) ;
	return _resManagerClientId;
}

void scOrder::setResManagerClientId(const std::string& resManagerClientId)
{
	Lock lock(_mutex) ;
	_resManagerClientId = resManagerClientId;
}

const std::string& scOrder::getResManagerKey() const
{
	Lock lock(_mutex) ;
	return _resManagerKey;
}

void scOrder::setResManagerKey(const std::string& resManagerKey)
{
	Lock lock(_mutex) ;
	_resManagerKey = resManagerKey;
}

const std::string& scOrder::getResManagerUrl() const
{
	Lock lock(_mutex) ;
	return _resManagerUrl;
}

void scOrder::setResManagerUrl(const std::string& resManagerUrl)
{
	Lock lock(_mutex) ;
	_resManagerUrl = resManagerUrl;
}
// ECICMF-94 End

void
scOrder::setSatellite (const std::string& s)
{
	Lock lock(_mutex) ;
	_satellite = s;
}

string
scOrder::getSatellite() const
{
	Lock lock(_mutex) ;
	return _satellite;
}

void
scOrder::setMission (const std::string& m)
{
	Lock lock(_mutex) ;
	_mission = m;
}

string
scOrder::getMission() const
{
	Lock lock(_mutex) ;
	return _mission;
}


void
scOrder::setTrackerFileName (const string& newTrk)
{
	Lock lock(_mutex) ;
	_trackerFileName = newTrk;
}

string
scOrder::getTrackerFileName () const
{
	Lock lock(_mutex) ;
	return _trackerFileName;
}


void
scOrder::setSchedulerLogName (const string& newLog)
{
	Lock lock(_mutex) ;
	_schedulerLogName = newLog;
}

string
scOrder::getSchedulerLogName () const
{
	Lock lock(_mutex) ;
	return _schedulerLogName;
}


void
scOrder::setLogFileName(const string& NewName)
{
	Lock lock(_mutex) ;
	_name = NewName;
}

string
scOrder::getLogFileName() const
{
	Lock lock(_mutex) ;
	return _name;
}

void
scOrder::setJobOrder(const string& NewJobOrder)
{
	Lock lock(_mutex) ;
	_jo = NewJobOrder;
}

string
scOrder::getJobOrder() const
{
	Lock lock(_mutex) ;
	return _jo;
}

void
scOrder::setProcName (const std::string& procName) 
{
	Lock lock(_mutex) ;
	_procName = procName;
}

string
scOrder::getProcName() const
{
	Lock lock(_mutex) ;
	return _procName;
}

void
scOrder::setProcVersion(const string& procVersion)
{
	Lock lock(_mutex) ;
	_procVersion = procVersion;
}

string
scOrder::getProcVersion() const
{
	Lock lock(_mutex) ;
	return _procVersion;
}

void
scOrder::setTaskTable(const string& NewTaskTable)
{
	Lock lock(_mutex) ;
	_tt = NewTaskTable;
}

string
scOrder::getTaskTable() const
{
	Lock lock(_mutex) ;
	return _tt;
}

void
scOrder::setBaseDir(const string& NewBaseDir)
{
	Lock lock(_mutex) ;
	_path = NewBaseDir;
}

string
scOrder::getBaseDir() const
{
	Lock lock(_mutex) ;
	return _path;
}

void
scOrder::doICreateDir(bool val)
{
	Lock lock(_mutex) ;
	_create_dir = val;
}

bool
scOrder::doICreateDir() const
{
	Lock lock(_mutex) ;
	return _create_dir;
}

void
scOrder::doICleanupOnExit(bool val)
{
	Lock lock(_mutex) ;
	_cleanup_dir = val;
}

bool
scOrder::doICleanupOnExit() const
{
	Lock lock(_mutex) ;
	return _cleanup_dir;
}

void 
scOrder::applyCleanupPolicyOnError (bool val)
{
    Lock lock(_mutex) ;
    _cleanup_error = val;
}

bool 
scOrder::applyCleanupPolicyOnError () const
{
    Lock lock(_mutex) ;
    return _cleanup_error;
}



void
scOrder::doIKillBackgroundTasks(bool val)
{
	Lock lock(_mutex) ;
	_kill_bckgrnd = val;
}

bool
scOrder::doIKillBackgroundTasks() const
{
	Lock lock(_mutex) ;
	return _kill_bckgrnd;
}

void
scOrder::getJobOrderParams(vector<string>& p) const
{
	Lock lock(_mutex) ;
	p = _joParams;
}

void
scOrder::setJobOrderParams(const vector<string>& p)
{
	Lock lock(_mutex) ;
	_joParams = p;
}

void
scOrder::getCmdLineParams(const std::string& task, std::vector<std::string>& p)
{
	Lock lock(_mutex) ;
	p = _cmdParams[task];
}

void
scOrder::setCmdLineParams(const std::string& task, const std::vector<std::string>& p)
{
	Lock lock(_mutex) ;
	_cmdParams[task] = p;
}

string 
scOrder::getRegExpAssociation (const string& theFileType) const
{
	Lock lock(_mutex) ;
	string retVal = "";

	map<string, string>::const_iterator it;

	if (hasRegExpAssociation(theFileType)) 
	{
		it = _regExpAssociationMap.find(theFileType);
		retVal = it->second;
	}

	return retVal;
}

bool 
scOrder::hasRegExpAssociation (const string& theFileType) const
{
	Lock lock(_mutex) ;
	return (_regExpAssociationMap.find(theFileType) != _regExpAssociationMap.end());
}

void 
scOrder::setRegExpAssociation (const string& theFileType, const string& theRegExp)
{
	Lock lock(_mutex) ;
	_regExpAssociationMap[theFileType] = theRegExp;
}

void 
scOrder::setRegExpAssociationMap (const map<string, string>& theMap)
{
	Lock lock(_mutex) ;
	_regExpAssociationMap = theMap;
}

map <string, string> 
scOrder::getRegExpAssociationMap () const
{
	Lock lock(_mutex) ;
	return _regExpAssociationMap;
}



class CmdLineParamsDumper // PRQA S 2109
{
public:
	explicit CmdLineParamsDumper(XMLOstream& os): _o(os) {} // PRQA S 2528

	void operator () (const pair<string, vector<string> >& cmd)
	{
		const vector<string>& v = cmd.second;

		if (not v.empty())
		{
			XMLOstream::Tag t(_o, _cmdLineKey);

			{ XMLOstream::Tag t1(_o, _taskNameKey, cmd.first, "", true); }

			{
				ostringstream ss;
				ss << "count=\"" << v.size() << "\"";
				XMLOstream::Tag t2(_o, _cmdLineParamArrayKey, "", ss.str());
                size_t v_size = v.size() ;
				for (size_t j=0; j < v_size; ++j)
				{
					XMLOstream::Tag t3(_o, _cmdLineParamKey, v[j], "", true);
				}
			}
		}
		// else nothing... condition is forbidden by schema
	}
private:
    CmdLineParamsDumper( ); // not implemented
private:
	XMLOstream& _o;

};


class RegExpAssociationDumper // PRQA S 2109
{
public:
	explicit RegExpAssociationDumper(XMLOstream& os): _o(os) {} // PRQA S 2528

	void operator () (const pair<string, string>& association)
	{
		XMLOstream::Tag t(_o, _regexpMapKey);
		
		{ XMLOstream::Tag tfiletype (_o, _mapFileTypeKey, association.first , "", true); }
		{ XMLOstream::Tag tregexp   (_o, _mapRegExpKey,   association.second, "", true); }
	}
private:
    RegExpAssociationDumper( ); // not implemented
private:
	XMLOstream& _o;

};                                              	
	

const scOrder&
scOrder::operator >> (ostream& o) const // PRQA S 4222
{
	Lock lock(_mutex) ;
	XMLOstream os(o, "Scheduler_Order");

	{ XMLOstream::Tag t(os, _logfileKey, getLogFileName(), "", true); }
	{ XMLOstream::Tag t(os, _procNameKey, getProcName(), "", true); }
	{ XMLOstream::Tag t(os, _procVersionKey, getProcVersion(), "", true); }
	{ XMLOstream::Tag t(os, _tasktableKey, getTaskTable(), "", true); }
	{ XMLOstream::Tag t(os, _joborderKey, getJobOrder(), "", true); }

	{
		vector<string> v;
		getJobOrderParams(v);
		if (not v.empty())
		{
			ostringstream ss;
			ss << "count=\"" << v.size() << "\"";
			XMLOstream::Tag t(os, _joborderParamArrayKey, "", ss.str());
            size_t v_size = v.size() ;
			for (size_t j=0; j < v_size; ++j)
			{
				XMLOstream::Tag t1(os, _joborderParamKey, v[j], "", true);
			}
		}
	}

	if (not _cmdParams.empty())
	{
		ostringstream ss;
		ss << "count=\"" << _cmdParams.size() << "\"";
		XMLOstream::Tag t(os, _cmdLinesArrayKey, "", ss.str());
		
		for_each(_cmdParams.begin(), _cmdParams.end(), CmdLineParamsDumper(os)); // PRQA S 3081
	}
	

	{ XMLOstream::Tag t(os, _workingdirKey, getBaseDir(), "", true); }
	{ XMLOstream::Tag t(os, _schedulerLogFileKey, getSchedulerLogName(), "", true); }
	{ XMLOstream::Tag t(os, _trackerFileNameKey, getTrackerFileName(), "", true); }
	{ XMLOstream::Tag t(os, _createwdKey, (doICreateDir() ? "true" : "false"), "", true); } // PRQA S 3380
	{ XMLOstream::Tag t(os, _doCleanupKey, (doICleanupOnExit() ? "true" : "false"), "", true); } // PRQA S 3380
	{ XMLOstream::Tag t(os, _cleanErrorKey, (applyCleanupPolicyOnError() ? "true" : "false"), "", true); } // PRQA S 3380
	{ XMLOstream::Tag t(os, _killbackgroundKey, (doIKillBackgroundTasks() ? "true" : "false"), "", true); } // PRQA S 3380
	{ XMLOstream::Tag t(os, _satelliteKey, getSatellite(), "", true); }
	{ XMLOstream::Tag t(os, _missionKey, getMission(), "", true); }

	if (not _regExpAssociationMap.empty()) 
	{
		ostringstream ss;
		ss << "count=\"" << _regExpAssociationMap.size() << "\"";
		XMLOstream::Tag t(os, _regexpMapArrayKey, "", ss.str());
		for_each(_regExpAssociationMap.begin(), _regExpAssociationMap.end(), RegExpAssociationDumper(os)); // PRQA S 3081
	}

	{ XMLOstream::Tag t(os, _logProcMessagesKey, (getLogProcMessages() ? "true" : "false") , "", true); }

	{ XMLOstream::Tag t(os, _posixExitCodeKey, (getPosixExitCode() ? "true" : "false") , "", true); }

	// ECICMF-94 Core based execution tags
	{ XMLOstream::Tag t(os, _resManagerURLKey, getResManagerUrl(), "", true); }
	{ XMLOstream::Tag t(os, _resManagerClientIdKey, getResManagerClientId(), "", true); }
	{ XMLOstream::Tag t(os, _resManagerKeyKey, getResManagerKey(), "", true); }


	return *this;
}

ostream & operator << (ostream &str, const acs::scOrder &o)
{
	o >> str;
	return str ; // PRQA S 4028
}



_ACS_END_NAMESPACE

