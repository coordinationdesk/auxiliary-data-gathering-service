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
	Revision 2.13  2014/08/01 10:06:17  marfav
	Added support to list of pconf files as per S2SL2P-194
	
	Revision 2.12  2013/07/07 18:25:16  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.11  2012/02/17 13:40:55  marpas
	refactoring
	
	Revision 2.10  2011/05/13 12:55:59  marfav
	getTaskTable now uses auto_ptr
	
	Revision 2.9  2011/05/10 08:03:19  nicvac
	getTaskTable added.
	
	Revision 2.8  2011/05/09 16:09:05  nicvac
	parseParameters is the method to override in subclasses (not setTaskTable anymore).
	
	Revision 2.7  2010/08/03 14:30:01  nicvac
	Task Table plugins management
	
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
	
	Revision 1.11  2004/08/25 13:40:45  marfav
	Using ParametersFileName instead of BlobName in methods
	
	Revision 1.10  2004/08/24 14:13:18  marfav
	Added support for satellite, mission, stations and blob
	
	Revision 1.9  2003/08/26 16:30:32  fracar
	added getConfigSpace method
	
	Revision 1.8  2003/07/10 14:04:56  marfav
	clone method added to exceptions
	
	Revision 1.7  2003/07/04 15:30:51  marfav
	Added the "read" method
	
	Revision 1.6  2003/05/07 15:31:38  marfav
	Align to new compiler version
	
	Revision 1.5  2003/04/30 16:33:43  marfav
	Dead code removed
	
	Revision 1.4  2003/04/30 14:55:33  marfav
	Align to ICD v2.0
	
	Revision 1.3  2003/04/18 12:52:21  marfav
	Using namespaces removed from .h
	
	Revision 1.2  2003/02/05 13:43:20  fracar
	got rid of ^M characters
	
	Revision 1.1.1.1  2003/01/31 15:00:48  marfav
	Import libJobOrder
	
	

*/


#ifndef _JOB_ORDER_H
#define _JOB_ORDER_H

#include <acs_c++config.hpp>
#include <Task.h>

#include <DateTime.h>
#include <XMLIstream.h>

#include <fstream>
#include <vector>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)

// Class JobOrder

class TaskTable;

enum joLogLevel
{
  LogLevelUNKNOWN=-1,
  LogLevelDEFAULT,
  LogLevelERROR,
  LogLevelWARNING,
  LogLevelPROGRESS,
  LogLevelINFO,
  LogLevelDEBUG
};

std::string LogLevel2String(joLogLevel l);
joLogLevel string2LogLevel(const std::string& s, joLogLevel def=LogLevelUNKNOWN);


class JobOrderAttachmentDescriptor
{
public:
	JobOrderAttachmentDescriptor();
	JobOrderAttachmentDescriptor(const std::string&, const std::string&);
	JobOrderAttachmentDescriptor (const JobOrderAttachmentDescriptor&);
	JobOrderAttachmentDescriptor& operator= (const JobOrderAttachmentDescriptor&);
	~JobOrderAttachmentDescriptor () throw() {} 

	std::string getType() const;
	std::string getPath() const;

	void setType (const std::string&);
	void setPath (const std::string&);

private:
	std::string _type;
	std::string _path;
};

class JobOrder // PRQA S 2153
{
  public:

	/*! class JobOrderException declaration */
	exDECLARE_EXCEPTION(JobOrderException,exException) ; // PRQA S 2131, 2502


    JobOrder();
    JobOrder(const JobOrder& j);
    virtual ~JobOrder() throw() ;

/* MP: wrong signature: removed !
    JobOrder& operator >> (std::ofstream& o); // PRQA S 2070, 2072 3
    JobOrder& operator << (std::ifstream& i);
    JobOrder& operator << (XMLIstream& i);
*/

    void reset();

    void setTaskTable(TaskTable*);
	ACS_SMARTPTR<TaskTable> getTaskTable() const;
	virtual void parseParameters();


    void setLogLevel(joLogLevel l);
    joLogLevel getLogLevel() const;

    void setOutLogLevel(joLogLevel l);
    joLogLevel getOutLogLevel() const;
    void setErrLogLevel(joLogLevel l);
    joLogLevel getErrLogLevel() const;

    void brkEnabled(bool b);
    bool brkEnabled() const;

    void addConfigSpace (const ConfigSpaceDescriptor& );
    void getConfigSpaces (std::vector<ConfigSpaceDescriptor>&) const;
    ConfigSpaceDescriptor getConfigSpace (const std::string&) const;

    void addAttachment (const JobOrderAttachmentDescriptor& );
    void getAttachments (std::vector<JobOrderAttachmentDescriptor>&) const;
    JobOrderAttachmentDescriptor getAttachmentDescriptor (const std::string&) const;

    void setConfig(const std::string& c);
    std::string getConfig() const;
    
    //S2SL2P-194 support to multpiple config files
    void setConfigList (const std::vector<std::string>&);
    void addConfigFile (const std::string&);
    void getConfigList( std::vector<std::string>& ) const;
    
    void setTimeInterval(const DateTime& start, const DateTime& stop);
    void getTimeInterval(DateTime& start, DateTime& stop) const;
    void isTest(bool b);
    bool isTest() const;
    void isTroubleshooting(bool b);
    bool isTroubleshooting() const;
    void setWorkingDir(const std::string& c);
    const std::string &getWorkingDir() const;
    void setOrderType(const std::string& s);
    const std::string &getOrderType() const;
    void setProcName(const std::string& c);
    const std::string &getProcName() const;
    void setProcVersion(const std::string& c);
    const std::string &getProcVersion() const;
    void setProcStation (const std::string& c);
    const std::string &getProcStation () const;
    void setStationName (const std::string&);
    const std::string &getStationName () const;
    void setParametersFileName (const std::string&);
    const std::string &getParametersFileName () const;

    void setOrderId (unsigned int);
    unsigned int getOrderId() const;

    /** Get the Order Id from Standard JobOrder's filename
     * Standard convention: Joborder.<orderid>.xml */
    static unsigned int getOrderIdFromFilename(const std::string& jobOrderFilename);


    void setSatellite (const std::string& s);
    const std::string &getSatellite () const;
    void setMission (const std::string& m);
    const std::string &getMission () const;

    void setExternalOrderId (const std::string&);
    const std::string &getExternalOrderId () const;

    std::vector<Task> getTasks () const;
    Task getTask (const std::string& s) const;
    const Task& operator [] (const std::string& s) const; // PRQA S 2141
    const Task& operator [] (const int& s) const; // PRQA S 2141
    const Task& at (const std::string& s) const; // PRQA S 2141
    const Task& at (const int & s) const; // PRQA S 2141

	size_t size() const;
	void clear();
	void push_back(Task & t);
	std::vector<Task>::const_iterator begin() const;
	std::vector<Task>::const_iterator end() const;
	
	std::vector<Task>::iterator begin(); 
	std::vector<Task>::iterator end(); 
	// operator implicit conversion
	operator const std::vector<Task>& () const;
	operator std::vector<Task>& () ;

  protected:
    JobOrder& operator = (const JobOrder& j);

    // friendship needeed to call read an write protected members
    friend std::ofstream& operator<< (std::ofstream&,  const JobOrder&) ;
    friend std::ifstream& operator>> (std::ifstream&, JobOrder& ) ; 
    friend XMLIstream& operator>> (XMLIstream&, JobOrder&) ;

    virtual void write(std::ofstream& o) const = 0;// writes in the joborder all inputs != "", regardless alternatives // PRQA S 2502 3
    virtual void read (XMLIstream& i) = 0;  // reads a joborder from XML stream
    virtual void read (std::ifstream& i) = 0;  // reads a joborder from plain ACII stream

    std::vector <ConfigSpaceDescriptor> _configSpaces; // PRQA S 2101 2
    std::vector <JobOrderAttachmentDescriptor> _attachments;

    /*
     *  helper classes used in STL algos
     */
    class SpaceNameEquals // PRQA S 2109
    {
    public:
        explicit SpaceNameEquals (const std::string&s);
        bool operator() (const ConfigSpaceDescriptor&) const;
    private:
        SpaceNameEquals() ; // not implemented
    private:
        std::string _s;
    };

    class TaskNameEquals // PRQA S 2109
    {
    public:
        explicit TaskNameEquals (const std::string& s);
        bool operator () (const Task& t) const;
    private:
        TaskNameEquals() ; // not implemented
    private:
        std::string _s;
    };

    class AttachmentTypeEquals // PRQA S 2109
    {
    public:
        explicit AttachmentTypeEquals (const std::string& s);
        bool operator () (const JobOrderAttachmentDescriptor&) const;
    private:
        AttachmentTypeEquals() ; // not implemented
    private:
        std::string _s;
    };

    /*
     *   helper classes for JobOrder xml file writing
     *   (to be used inside the write() method implementation
     */

    class xml_indent
    {
    public:
        xml_indent() {} 
        xml_indent(const xml_indent &) {} 
        xml_indent& operator= (const xml_indent &) { return *this ; } // PRQA S 4072
        virtual ~xml_indent() throw() { } // PRQA S 2131
 	protected:
		static unsigned int _indentCount; // PRQA S 2101
    };

    class xml_section_tag : public xml_indent // PRQA S 2109, 2153
    {
    public:
	  xml_section_tag(std::ofstream& o, const std::string &s, const std::string &a="");
	  virtual ~xml_section_tag() throw() ;
    private:
        xml_section_tag() ; // not implemented
        xml_section_tag(const xml_section_tag &) ; // not implemented
        xml_section_tag& operator= (const xml_section_tag &) ; // not implemented
    private:
	    std::ofstream& _o;
	    std::string _s;
    };

    class xml_element_tag : public xml_indent // PRQA S 2109, 2153
    {
    public:
	    xml_element_tag(std::ofstream& o, const std::string &s, const std::string &v);
	    xml_element_tag(std::ofstream& o, const std::string &s, unsigned int u);
	    virtual ~xml_element_tag() throw() {} // PRQA S 2131
    private:
        xml_element_tag() ; // not implemented
        xml_element_tag(const xml_element_tag &) ; // not implemented
        xml_element_tag& operator= (const xml_element_tag &) ; // not implemented
    };

  private:

    joLogLevel _logLevel;
    joLogLevel _outLogLevel;
    joLogLevel _errLogLevel;

    bool _brkEnabled;
    bool _isTest;
    bool _isTroubleshooting;

    // std::string _config;
    
    std::vector<std::string> _configList;
	std::vector<Task>		_tasks;
    
    DateTime    _start ;
    DateTime    _stop;
    std::string _workingDir;
    std::string _orderType;
    std::string _procName;
    std::string _procVersion;
    std::string _procStation;
    std::string _stationName;
    std::string _satellite;
    std::string _mission;
    std::string _parametersFileName;
    unsigned int _orderId;
    std::string _externalOrderId;

	TaskTable* _taskTable;

};

inline std::ofstream& operator<< (std::ofstream& o,  const JobOrder& jo) 
{ jo.write(o) ; return o ;}

inline std::ifstream& operator>> (std::ifstream& o, JobOrder& jo) 
{ jo.read(o) ; return o ; }

inline XMLIstream& operator>> (XMLIstream& o, JobOrder& jo) 
{ jo.read(o) ; return o ; }


_ACS_END_NAMESPACE

#endif // _JOB_ORDER_H
