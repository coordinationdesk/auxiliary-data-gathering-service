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
	Revision 2.4  2012/03/13 17:30:34  marpas
	refactoring
	
	Revision 2.3  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.2  2008/10/20 13:18:56  marfav
	Added support to task cmdline parameters
	
	Revision 2.1  2008/04/22 11:07:46  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.1  2006/03/27 18:03:24  fracar
	added id/def management
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.1  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	

*/

#ifndef _TaskTableVisitor_H_
#define _TaskTableVisitor_H_


#include <acs_c++config.hpp>
#include <TaskTableVisitable.h>



_ACS_BEGIN_NAMESPACE(acs)


	
/**
 *\brief A Class for defining the interface of the element visitors
 *       The Element Visitors shall be able to deal with every TaskTableElement
 *       One method is defined for each existent element class
 *       The customization is the action implemented in every method
 */


class AtExitSequence;
class BreakpointList;
class BreakpointDescriptor;
class ConfigDescriptor;
class ConfigSpaceDescriptor;
class InputList;
class InputDescriptor;
class InputAlternative;
class OutputList;
class OutputDescriptor;
class TaskParameterList;
class TaskParameterDescriptor;
class Pool;
class Sequence;
class Task;
class TaskTable;

class TaskTableVisitor
{

public:

	/**
	 * Ctor and Dtor are empty. 
	 */ 
	TaskTableVisitor();
	virtual ~TaskTableVisitor() throw() ;
	TaskTableVisitor(const TaskTableVisitor & ) {}
	TaskTableVisitor & operator =(const TaskTableVisitor & ) { return *this ; } // PRQA S 4072



	/*
	 * Interface supporting the modified contitional visitor pattern implementation
	 */

	// see TaskTableVisitable::acceptVisitor (TaskTableVisitor& v)
	// for description of the functioning



	/**
	 *\brief TaskTableVisitable interface definition
	 *       Every TaskTableVisitable that needs to be operated upon will call this interface
	 *       passing itself as argument
	 */
	virtual bool enterVisit (TaskTableVisitable&);
	virtual bool visit (TaskTableVisitable&);
	virtual bool exitVisit (TaskTableVisitable& v);

// PRQA S 1020, 1023, 1030 4
#define DECLARE_VISITED(visitable) \
	virtual bool enterVisit (visitable& v) {return enterVisit(reinterpret_cast<TaskTableVisitable&>(v));} \
	virtual bool visit (visitable& v) {return visit(reinterpret_cast<TaskTableVisitable&>(v));} \
	virtual bool exitVisit (visitable& v) {return exitVisit(reinterpret_cast<TaskTableVisitable&>(v));}

 // PRQA S 2131, 3031, 3081 L1 
	/**
	 *\brief TaskTable interface definition
	 *       Every TaskTable that needs to be operated upon will call this interface
	 *       passing itself as argument
	 */
	DECLARE_VISITED(TaskTable) 

	/**
	 *\brief Task interface definition
	 *       Every Task that needs to be operated upon will call this interface
	 *       passing itself as argument
	 */
	DECLARE_VISITED(Task)

	/**
	 *\brief ConfigSpaceDescriptor interface definition
	 *       Every ConfigSpaceDescriptor that needs to be operated upon will call this interface
	 *       passing itself as argument
	 */
	DECLARE_VISITED(ConfigSpaceDescriptor)

	/**
	 *\brief ConfigSpaceDescriptor interface definition
	 *       Every ConfigSpaceDescriptor that needs to be operated upon will call this interface
	 *       passing itself as argument
	 */
	DECLARE_VISITED(ConfigDescriptor)

	/**
	 *\brief Sequence interface definition
	 *       Every Sequence that needs to be operated upon will call this interface
	 *       passing itself as argument
	 */
	DECLARE_VISITED(Sequence)

	/**
	 *\brief Pool interface definition
	 *       Every Pool that needs to be operated upon will call this interface
	 *       passing itself as argument
	 */
	DECLARE_VISITED(Pool)

	/**
	 *\brief InputDescriptor interface definition
	 *       Every InputDescriptor that needs to be operated upon will call this interface
	 *       passing itself as argument
	 */
	DECLARE_VISITED(InputDescriptor)

	/**
	 *\brief InputAlternative interface definition
	 *       Every InputAlternative that needs to be operated upon will call this interface
	 *       passing itself as argument
	 */
	DECLARE_VISITED(InputAlternative)

	/**
	 *\brief OutputDescriptor interface definition
	 *       Every OutputDescriptor that needs to be operated upon will call this interface
	 *       passing itself as argument
	 */
	DECLARE_VISITED(OutputDescriptor)

	/**
	 *\brief BreakpointDescriptor interface definition
	 *       Every BreakpointDescriptor that needs to be operated upon will call this interface
	 *       passing itself as argument
	 */
	DECLARE_VISITED(BreakpointDescriptor)

	/**
	 *\brief InputList interface definition
	 *       Every InputList that needs to be operated upon will call this interface
	 *       passing itself as argument
	 */
	DECLARE_VISITED(InputList)

	/**
	 *\brief OutputList interface definition
	 *       Every OutputList that needs to be operated upon will call this interface
	 *       passing itself as argument
	 */
	DECLARE_VISITED(OutputList)

	/**
	 *\brief BreakpointList interface definition
	 *       Every BreakpointList that needs to be operated upon will call this interface
	 *       passing itself as argument
	 */
	DECLARE_VISITED(BreakpointList)

	/**
	 *\brief TaskParameterList interface definition
	 *       Every TaskParameterDescriptor that needs to be operated upon will call this interface
	 *       passing itself as argument
	 */
	DECLARE_VISITED(TaskParameterList)

	/**
	 *\brief TaskParameterDescriptor interface definition
	 *       Every TaskParameterDescriptor that needs to be operated upon will call this interface
	 *       passing itself as argument
	 */
	DECLARE_VISITED(TaskParameterDescriptor)

	/**
	 *\brief AtExitSequence interface definition
	 *       Every AtExitSequence that needs to be operated upon will call this interface
	 *       passing itself as argument
	 */
	DECLARE_VISITED(AtExitSequence)
// PRQA L:L1
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TaskTableVisitor) ;
};


_ACS_END_NAMESPACE


#endif //_TaskTableVisitor_H_
