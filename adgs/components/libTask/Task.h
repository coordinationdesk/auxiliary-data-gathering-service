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

	$Prod: A.C.S. ThinLayer Task Library $

	$Id$

	$Author$

	$Log$
	Revision 2.8  2017/01/13 09:55:14  marfav
	Fixed clear operation signature now it is virtual
	Fixed use of virtual operations in DTOR
	
	Revision 2.7  2015/11/19 14:10:24  marpas
	warning added
	
	Revision 2.6  2014/06/10 15:47:46  marfav
	Addedd support to number of CPUs
	
	Revision 2.5  2013/07/07 18:22:08  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.4  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.3  2008/10/20 13:18:56  marfav
	Added support to task cmdline parameters
	
	Revision 2.2  2008/04/22 11:07:46  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.1  2006/03/27 18:03:24  fracar
	added id/def management
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.16  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	
	Revision 1.15  2006/01/19 18:17:35  marfav
	Fileclass attribute added to InputAlternative
	
	Revision 1.14  2005/06/28 08:21:09  marfav
	Using strings only order type definitions
	
	Revision 1.13  2004/12/03 18:01:40  marfav
	Header fixed
	
	Revision 1.12  2004/09/28 15:05:42  marfav
	Added percentage support
	
	Revision 1.11  2004/08/03 13:52:17  marfav
	Using TimeRange instead of dbGeoTimeRange
	
	Revision 1.10  2003/09/22 11:06:38  fracar
	improved readability
	
	Revision 1.9  2003/07/10 14:05:53  marfav
	clone method added to exceptions
	
	Revision 1.8  2003/07/04 15:24:13  marfav
	Enhanced management of input and output descriptors
	
	Revision 1.7  2003/06/05 14:17:16  marfav
	Added Criticality Level support
	
	Revision 1.6  2003/05/21 08:57:37  marfav
	FileNameType added to OutputDescriptor and IInput Alternative
	
	Revision 1.5  2003/04/30 14:44:18  marfav
	Align to ICD v2.0
	
	Revision 1.4  2003/04/18 12:47:37  marfav
	Using namespaces removed from .h
	
	Revision 1.3  2003/02/10 18:52:29  fracar
	prepared for multi-interval management
	
	Revision 1.2  2003/02/05 13:40:20  fracar
	handling T0 and T1 fields inside the task table
	
	Revision 1.1.1.1  2003/01/30 18:25:35  marfav
	Import libTask
	
	

*/


#ifndef _TASK_H
#define _TASK_H

#include <acs_c++config.hpp>
#include <exException.h>

#include <TaskTableElement.h>
#include <TaskDataTypes.h>
#include <InputDescriptor.h>
#include <OutputDescriptor.h>
#include <BreakpointDescriptor.h>
#include <TaskParameterDescriptor.h>
#include <ConfigDescriptor.h>
#include <ConfigSpaceDescriptor.h>
#include <WeightNormalizer.h>



_ACS_BEGIN_NAMESPACE(acs)


/**
 *\brief This class maps the description of a task as derived from the GEN_PROC icd
 *       It is a Task Table Element and will accept ElementVisitors 
 *       for specific action purposes (reading and writing)
 */
	
class Task : public TaskTableElement // PRQA S 2109
{

public:
	
	/**
	 * CTOR and copy CTOR
	 */ 
	Task();
	Task(const Task& aTask);

	/**
	 * Virtual DTOR
	 */ 
	virtual ~Task() throw() ;

	/**
	 * Copy operator
	 */ 
	Task& operator = (const Task& aTask);

	/**
	 * Clone method implementation
	 */ 
	virtual TaskTableElement* clone() const; // PRQA S 2502
	
	
	/**
	 * Methods for adding elements. The task is composed by aggregates of
	 * - InputDescriptors
	 * - OutputDescriptors
	 * - BreakpoinDescriptors
	 * - TaskParameterDescriptors
	 * stored internally as class attributes (vectors)
	 */ 
	void addInput(const InputDescriptor& aDescriptor);
	void addInputs(const std::vector<InputDescriptor>& aList);
	void addInputs(const std::vector<InputDescriptor>::const_iterator &from, const std::vector<InputDescriptor>::const_iterator &to);
	void addOutput(const OutputDescriptor& aDescriptor);
	void addBreakpoint(const BreakpointDescriptor& aDescriptor);
	void addParameter(const TaskParameterDescriptor& aDescriptor);

	
	
	/**
	 * Methods for deleting elements.
	 */ 
    // need to redefine the TaskTableVisitable::clear method, in order to preserve containers
    // containers will be freed by parent desctructor
	virtual void clear(); 

	void clearInputs();
	void clearOutputs();
	void clearBreakpoints();
	void clearParameters();


	/**
	 * Methods for setting and getting the TaskName
	 */ 
	bool isSetName () const;
	const std::string &getName() const;
	void setName(const std::string& NewName);
	
	/**
	 * Methods for setting and getting the Task Version
	 */ 
	bool isSetVersion () const;
	const std::string &getVersion() const;
	void setVersion(const std::string& NewVersion);
	
	/**
	 * Methods for setting and getting the Task Binary Path
	 * This binary file will be executed when spawning the task
	 */ 
	bool isSetPath() const;
	const std::string &getPath() const;
	void setPath(const std::string& NewPath);
	
	/**
	 * Methods for setting and getting the Critical flag
	 * A failure on a critical task will mark all the processing as failed
	 */ 
	bool isSetCritical() const;
	bool isCritical() const;
	void isCritical(bool);

	/**
	 * Methods for setting and getting the Number of CPU to dedicate to the task
	 * It should be used in some way by the execution infrastructure (TBD)
	 */ 
	bool isSetNumberOfCPUs() const;
	unsigned int getNumberOfCPUs () const;
	void setNumberOfCPUs (unsigned int);


	/**
	 * Methods for setting and getting the Criticallity level for the Task
	 * When a critical task fails all the tasks having criticality level less than or equal
	 * are immediately stopped when running in a pool
	 * Tasks having higher criticality level will continue the execution
	 */ 
	bool isSetCriticalityLevel() const;
	int getCriticalityLevel () const;
	void setCriticalityLevel (int level = 0);

	// APF-411
	// Tasks now can declare if are posix or Gen-ICD compliant
	// for what concerns the exit code interpretation

	/**
	 * Methods for setting and getting the compliancy with exit code posix standards
	 */ 
	bool isSetPosixExitCode() const;
	bool isPosixExitCode () const;
	void isPosixExitCode (bool isPosix);


	/**
	 *  Implementation for the virtual interface TaskTableElement
	 */
	virtual void getLocalTaskList (std::vector<Task> & aList, bool erase = false) const;

	virtual void normalize(acs::WeightNormalizer* = 0);

	/**
	 *  Implementation for the virtual interface TaskTableElement
	 */
	virtual bool acceptVisit (TaskTableVisitor&);
	virtual bool acceptEnterVisit (TaskTableVisitor&);
	virtual bool acceptExitVisit (TaskTableVisitor&);



private: // attributes
	std::string _name;
	std::string _version;
	std::string _path;
	bool _isCritical;
	int _criticalityLevel;
	bool _nameSet;
	bool _versionSet;
	bool _pathSet;
	bool _isCriticalSet;
	bool _criticalityLevelSet;
    unsigned int _numberOfCPUs;
    bool _isNumberOfCPUsSet;
	bool _isPosixExitCodeSet;
	bool _isPosixExitCode;


private: // associations
	InputList* _theInputDescriptors;
	OutputList* _theOutputDescriptors;
	BreakpointList* _theBreakpointDescriptors;
	TaskParameterList* _theParameterDescriptors;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(Task) ;
};

_ACS_END_NAMESPACE

#endif // _TASK_H
