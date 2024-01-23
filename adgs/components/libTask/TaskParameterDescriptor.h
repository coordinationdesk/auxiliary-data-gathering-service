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
	Revision 1.2  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 1.1  2008/10/21 09:03:30  marfav
	Adding TaskParameterDescriptor classes
	

	

*/

#ifndef _TASKPARAMETERDESCRIPTOR_H_
#define _TASKPARAMETERDESCRIPTOR_H_

#include <acs_c++config.hpp>
#include <exException.h>
							   
#include <TaskDataTypes.h>
#include <TaskTableVisitable.h>


_ACS_BEGIN_NAMESPACE(acs)

class TaskParameterList : public TaskTableVisitable
{
public:
	TaskParameterList();
	TaskParameterList(const TaskParameterList&);
	virtual ~TaskParameterList() throw() {}  // PRQA S 2131

	/**
	 * Operator =
	 * Used by copy ctor
	 */
	TaskParameterList& operator = (const TaskParameterList&);


	/**
	 *  Implementation for the virtual interface TaskTableVisitable
	 */
	virtual TaskTableVisitable* clone() const; // PRQA S 2502
	virtual bool acceptEnterVisit (TaskTableVisitor&);
	virtual bool acceptVisit (TaskTableVisitor&);
	virtual bool acceptExitVisit (TaskTableVisitor&);

	virtual bool isContainer() const {return true;} // PRQA S 2131

private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(TaskParameterList) ;
};

	
/**
 * \brief This class is the representation of an task parameter descriptor
 */
class TaskParameterDescriptor : public TaskTableVisitable // PRQA S 2109
{
public:
	/**
	 * CTOR and copy CTOR 
	 */ 
	TaskParameterDescriptor();
	TaskParameterDescriptor(const TaskParameterDescriptor& d);
	virtual ~TaskParameterDescriptor() throw() {} // PRQA S 2131
	
	/**
	 * Operator =
	 * Used by copy ctor
	 */
	TaskParameterDescriptor& operator = (const TaskParameterDescriptor& d);

	/**
	 *  Implementation for the virtual interface TaskTableVisitable
	 */
	virtual TaskTableVisitable* clone() const; // PRQA S 2502
	virtual bool acceptEnterVisit (TaskTableVisitor&);
	virtual bool acceptVisit (TaskTableVisitor&);
	virtual bool acceptExitVisit (TaskTableVisitor&);

public:
	/**
	*  Methods for get and set attributes
	*/

	/**
	*  Name
	*/
	std::string getName (bool* isSet = 0) const;
	void setName (const std::string&);

	/**
	*  Value
	*/
	std::string getValue (bool* isSet = 0) const;
	void setValue (const std::string&);

    /**
     * Mandatory Flag
     */
    bool isMandatory (bool* isSet = 0) const;
    void isMandatory (bool); // PRQA S 2020



private:
	// attributes
	std::string _name;
	std::string _value;
    bool _mandatory;
    bool _nameSet;
	bool _valueSet;
    bool _mandatorySet;


    ACS_CLASS_DECLARE_DEBUG_LEVEL(TaskParameterDescriptor) ;
};


_ACS_END_NAMESPACE


#endif //_TASKPARAMETERDESCRIPTOR_H_

