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
	Revision 2.5  2012/06/05 15:39:18  marfav
	code improved to remove compiler warnings
	
	Revision 2.4  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.3  2008/04/22 11:07:45  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.2  2006/03/27 18:03:24  fracar
	added id/def management
	
	Revision 2.1  2006/03/09 14:25:57  giucas
	Task Operator [] fixed
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.1  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	

*/

#ifndef _POOL_H_
#define _POOL_H_

#include <acs_c++config.hpp>
#include <TaskTableElement.h>
#include <Task.h>

_ACS_BEGIN_NAMESPACE(acs)
	
/**
 *\brief This class maps the description of a pool as derived from the GEN_PROC icd
 *       A pool can recursively contain pools, sequences or tasks
 *       It is a Task Table Element and will accept ElementVisitors 
 *       for specific action purposes (reading and writing)
 */
class Pool : public TaskTableElement // PRQA S 2109
{
public:

	/**
	 * CTOR and Copy CTOR 
	 */ 
	Pool();
	Pool(const Pool& p);

	virtual ~Pool() throw() ;

	/**
	 * Copy Operator
	 */ 
	Pool& operator = (const Pool& p);
	
	/**
	 * Clone method implementation
	 */ 
	virtual TaskTableElement* clone() const; // PRQA S 2502

	/**
	 * These methods gets the input/output/breakpoint list referred to the local object
	 */ 
     
    using TaskTableElement::getLocalInputList;
	virtual void getLocalInputList (std::vector<InputDescriptor>& aList, 
									const std::string& theOrderType, 
									bool erase = false) const;
	
	virtual void getExtLocalInputList (std::vector<InputDescriptor>& aList, 
									   const std::string& theOrderType, 
									   bool erase = false) const;

	virtual void getLocalOutputList (std::vector<OutputDescriptor>& aList,
									 bool erase=false) const;
	
	virtual void getLocalBreakpointList (std::vector<BreakpointDescriptor>& aList,
										 bool erase=false) const;

	/** Apply normalization
	 *  It is possible to specify a normalizer or to use the default one
	 */ 
	virtual void normalize(acs::WeightNormalizer* = 0);
	
	/**
	 *  Implementation for the virtual interface TaskTableElement
	 */
	virtual void getLocalTaskList (std::vector<Task> & aList, bool erase = false) const;

	virtual bool acceptVisit (TaskTableVisitor&);
	virtual bool acceptEnterVisit (TaskTableVisitor&);
	virtual bool acceptExitVisit (TaskTableVisitor&);


	
private: // attributes

private: // aggregates

	ACS_CLASS_DECLARE_DEBUG_LEVEL(Pool) ;
};

// This typedef is needed due to retro-compatibility code issues
// TaskPool is not a real class 
typedef Pool TaskPool;

_ACS_END_NAMESPACE

#endif //_POOL_H_

