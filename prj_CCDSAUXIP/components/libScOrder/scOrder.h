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
	
	Revision 1.8  2012/02/20 15:09:40  marpas
	refactoring
	
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

#ifndef _SCORDER_H
#define _SCORDER_H

#include <acs_c++config.hpp>
#include <ThreadSafe.h>

#include <vector>
#include <map>
#include <string>
#include <fstream>


_ACS_BEGIN_NAMESPACE(acs)

// This class maps the XML configuration file used as interface between the launcher and the scheduler.
// Upon creation, it loads the XML file internally, and allows to query it..

// Scheduler must try to load Satellite and Mission configuration parameters from TaskTable
// if not provided (set to blank) by the WorkOrder XML file.
// Note that the Satellite and the Mission fields are mandatory in scOrder.

class scOrder
{
  public:
    scOrder();
    explicit scOrder(const std::string& aPath);
    scOrder(const scOrder& o);
    virtual ~scOrder() throw () {} // PRQA S 2131 

    void setLogFileName(const std::string& );
    std::string getLogFileName() const;
    void setProcName (const std::string&);
    std::string getProcName() const;
    void setProcVersion(const std::string&);
    std::string getProcVersion() const;
    void setTaskTable(const std::string& );
    std::string getTaskTable() const;
    void setJobOrder(const std::string& );
    std::string getJobOrder() const;
    void setBaseDir(const std::string& );
    std::string getBaseDir() const;
    void doICreateDir(bool val);
    bool doICreateDir() const;
    
    // Cleanup on exit applies only to succesful processing chains
    // An additional flag applyCleanupPolicyOnError will ask to apply the
    // current cleanupOnExit policy also in case of failed processing
    void doICleanupOnExit(bool val);
    bool doICleanupOnExit() const;

    void applyCleanupPolicyOnError (bool val);
    bool applyCleanupPolicyOnError () const;

    void doIKillBackgroundTasks(bool val);
    bool doIKillBackgroundTasks() const;
    void setSchedulerLogName (const std::string& );
    std::string getSchedulerLogName () const;
    void setTrackerFileName (const std::string& );
    std::string getTrackerFileName () const;
    void setSatellite (const std::string&);
    std::string getSatellite() const;
    void setMission (const std::string&);
    std::string getMission() const;

    void getJobOrderParams(std::vector<std::string>&) const;
    void setJobOrderParams(const std::vector<std::string>&);
    void getCmdLineParams(const std::string& task, std::vector<std::string>&);
    void setCmdLineParams(const std::string& task, const std::vector<std::string>&);

    // Returns the regexp associated to a filetype (if any)
    std::string getRegExpAssociation (const std::string&) const;

    // Returns true if the filetype is present into the RegExpAssociation map
    bool hasRegExpAssociation (const std::string&) const;

    // Sets the association between filetype and regexp
    void setRegExpAssociation (const std::string&, const std::string&);

    // Sets the complete association map
    void setRegExpAssociationMap (const std::map<std::string, std::string>&);

    // Gets the complete association map
    std::map <std::string, std::string> getRegExpAssociationMap () const;

    // Set/get the standard exstreams notification 
    // allowing the processor messages to be reported in the main application log file
    void setLogProcMessages (bool);
    bool getLogProcMessages () const;

    // Set/get the standard exstreams notification 
    // allowing the processor messages to be reported in the main application log file
    void setPosixExitCode (bool);
    bool getPosixExitCode () const;

    // Set/get the resource manager (e.g. core) execution parameters
    // ECICMF-94
    const std::string& getResManagerClientId() const;
    void setResManagerClientId(const std::string& resManagerClientId);

    const std::string& getResManagerKey() const;
    void setResManagerKey(const std::string& resManagerKey);

    const std::string& getResManagerUrl() const;
    void setResManagerUrl(const std::string& resManagerUrl);



    const scOrder& operator >> (std::ostream& o) const; // PRQA S 2070, 2072
    scOrder& operator =(const scOrder& o);

    //! write the work order to a stream
    friend std::ostream & operator << (std::ostream &, const scOrder &) ; // PRQA S 2107

  private: // attributes
    Mutex _mutex ;
    std::string _name;
    std::string _procName;
    std::string _procVersion;
    std::string _tt;
    std::string _jo;
    std::string _path;
    std::string _schedulerLogName;
    std::string _trackerFileName;
    std::string _satellite;
    std::string _mission;
    std::vector<std::string> _joParams;
    std::map<std::string, std::vector<std::string> > _cmdParams;
    std::map<std::string, std::string> _regExpAssociationMap;
    bool   _create_dir;
    bool   _cleanup_dir;
    bool   _kill_bckgrnd;
    bool   _cleanup_error;
    bool _logProcMessages;
    bool _posixExitCode;

    // ECICMF-94 Resource (e.g. core) run attributes
    std::string _resManagerKey;
    std::string _resManagerUrl;
    std::string _resManagerClientId;
};

std::ostream & operator<<(std::ostream &, const scOrder &) ;

_ACS_END_NAMESPACE

#endif  //_SCORDER_H
