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

	$Prod: A.C.S. ThinLayer TaskTableElements Library $

	$Id$

	$Author$

	$Log$
	Revision 2.5  2013/07/07 18:22:08  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.4  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.3  2008/05/06 09:54:29  marfav
	Added complete getLocalInputList search method
	
	Revision 2.2  2008/04/22 11:07:46  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.1  2006/03/27 18:03:24  fracar
	added id/def management
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.1  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	

*/

#ifndef _TASKTABLEELEMENT_H_
#define _TASKTABLEELEMENT_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <TaskTableVisitable.h>



_ACS_BEGIN_NAMESPACE(acs)
	
/**
 *\brief A Class for defining the interface and the common implementation of task table elements
 */

class Task;
class InputDescriptor;
class OutputDescriptor;
class BreakpointDescriptor;
class Task;
class WeightNormalizer;

	
class TaskTableElement : public TaskTableVisitable // PRQA S 2109
{

public:
	static const std::string _alwaysOrderType; // PRQA S 2100


public:
	
	/**
	 * This is the base exception for TaskTableElement object
	 */ 
	exDECLARE_EXCEPTION(TaskTableElementException, exException) ; // PRQA S 2131, 2502
	
	/**
	 * This exception is raised when trying to select a task that does not exists
	 */ 
	exDECLARE_EXCEPTION(TaskNotFoundException, TaskTableElementException) ; // PRQA S 2131, 2153, 2502


	/**
	 * Ctor and Dtor 
	 */ 
	TaskTableElement(); 
	virtual ~TaskTableElement() throw() ;

	
	/**
	 * Gets the complete list of the InputDescriptors
	 * It is possible to choose if erase or not the list container
	 * The Ptr version allows to get a smart vector including the pointers to cloned inputs
	 */ 
	void getInputList(std::vector<InputDescriptor>& aList, bool erase=false) const;
	void getInputPtrList(TaskTableSmartVector<InputDescriptor>& aList, bool erase=false) const;

	/**
	 * These methods gets the input list filtered by orderType and taskName
	 * It is possible to search in local tasks or in nested tasks too
	 * The extended input list include all the input having ALWAYS as order type
	 */ 
	virtual void getInputList(std::vector<InputDescriptor>& aList,
							  const std::string& anOrderType,
							  const std::string& theTask = "", // If theTask is empty, the input list over all the tasks is returned
							  bool erase = false,
							  bool nested = true) const;

	virtual void getExtInputList(std::vector<InputDescriptor>& aList,
								 const std::string& anOrderType,
								 const std::string& theTask = "", // If theTask is empty, the input list over all the tasks is returned
								 bool erase = false,
								 bool nested = true) const;

	/**
	 * Variant with smartvector and cloned ptrs
	**/
	virtual void getInputPtrList(TaskTableSmartVector<InputDescriptor>& aList,
							  const std::string& anOrderType,
							  const std::string& theTask = "", // If theTask is empty, the input list over all the tasks is returned
							  bool erase = false,
							  bool nested = true) const;

	virtual void getExtInputPtrList(TaskTableSmartVector<InputDescriptor>& aList,
								 const std::string& anOrderType,
								 const std::string& theTask = "", // If theTask is empty, the input list over all the tasks is returned
								 bool erase = false,
								 bool nested = true) const;


	/**
	 * These methods gets the input list referred to the local object
	 * Pure virtual methods shall be implemented in every subclass
	 */ 
	virtual void getLocalInputList(std::vector<InputDescriptor>& aList, bool erase=false) const;

	virtual void getLocalInputList (std::vector<InputDescriptor>& aList, 
					const std::string& anOrderType, 
					bool erase = false) const;
	
	virtual void getExtLocalInputList (std::vector<InputDescriptor>& aList, 
					   const std::string& anOrderType, 
					   bool erase = false) const;

	
	/**
	 * This method gets the output list filtered by taskName
	 * It is possible to search in local tasks or in nested tasks too
	 */ 
	virtual void getOutputList (std::vector<OutputDescriptor>& aList,
				    const std::string& theTask = "", // If theTask is empty, the input list over all the tasks is returned
				    bool erase=false,
				    bool nested=true) const;
	
	virtual void getLocalOutputList (std::vector<OutputDescriptor>& aList,
					 bool erase=false) const;


	/**
	 * This method gets the output list filtered by taskName
	 * It is possible to search in local tasks or in nested tasks too
	 */ 
	virtual void getBreakpointList (std::vector<BreakpointDescriptor>& aList,
					const std::string& theTask = "", // If theTask is empty, the breakpoint list over all the tasks is returned
					bool erase=false,
					bool nested=true) const;

	virtual void getLocalBreakpointList (std::vector<BreakpointDescriptor>& aList,
					     bool erase=false) const;

	/**
	 * This method returns the list of tasks building the element
	 * It is possible to search recursively in order to discover every nested task
	 * Tasks having the same TaskName shall not be allowed !!
	 */ 
	virtual void getTaskList (std::vector<Task> & aList, bool erase = false, bool nested = true) const;
	virtual void getLocalTaskList (std::vector<Task> & aList, bool erase = false) const;

	
	/**
	 * This method allows to access to a task (if present in the structure)
	 * using the task name as a search key, overloading the operator []
	 */ 
	virtual Task operator[] (const std::string& TaskName) const; // PRQA S 2141

	/**
	 * This dectlaration allows to access the inherited operator []
	 * defined in the TaskTableVisitable class and unacessible since this class defined  
         * an overloaded operator []
	 */
	using TaskTableVisitable::operator[];
        // TaskTableVisitable* operator[] (int idx) const {return TaskTableVisitable::operator[] (idx);}

	/**
	 * This method allows to check the existance of a task 
	 * using the task name as a search key
	 * It is possible to search including the nested tasks 
	 * belonging to inner structure elements
	 * When the task exists, a copy of it can be returned using the task parameter
	 */ 
	virtual bool exists(const std::string& TaskName, bool nested = true, Task* foundTask = 0) const;

	/**
	 *  Weight management and normalization
	 *  It is possible to specify a normalizer or to use the default one
	 *  Methods for setting and getting the assigned percentage are needed too
	 */ 
	bool isSetAssignedPercentage () const;
	float getAssignedPercentage () const;
	void setAssignedPercentage(float);

	virtual void normalize(WeightNormalizer* = 0) {} // PRQA S 2131


	/**
	 * Methods for setting and getting the Detached Flag
	 */ 
	bool isSetDetached () const;
	bool isDetached () const;
	void isDetached (bool);

	/**
	 * Methods for setting and getting the Killing Signal
	 * This signal will be used to force the immediate stop of the element
	 * default value is SIGKILL (9)
	 */ 
	bool isSetKillSignal () const;
	int getKillSignal () const;
	void setKillSignal (int=9);

	/**
	 * Methods for setting and getting the quit signal for clean stop of the element
	 * Default value is "no action" that is signal 0 
	 */ 
	bool isSetQuitSignal () const;
	int getQuitSignal() const;
	void setQuitSignal (int s = 0);

	/**
	 * Methods for retrieving the property
	 * is interruptable or not 
	 */ 
	bool isInterruptable () const { return _interruptable;}
	
protected:	
	/**
	 * Copy CTOR and assignment operator
	 **/
	TaskTableElement (const TaskTableElement&);
	TaskTableElement& operator= (const TaskTableElement&);

protected: // attributes  
	float _assignedPercentage; // PRQA S 2101 9
	bool _assignedPercentageSet;
	bool _isDetached;
	int _killingSignal;
	int _quitSignal;
	bool _isDetachedSet;
	bool _killingSignalSet;
	bool _quitSignalSet;
	bool _interruptable;

private: 
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TaskTableElement) ;

};

_ACS_END_NAMESPACE


#endif //_TASKTABLEELEMENT_H_

