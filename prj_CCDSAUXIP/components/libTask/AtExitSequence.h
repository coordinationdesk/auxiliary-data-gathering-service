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
	Revision 2.3  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.2  2008/04/22 11:07:45  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.1  2006/03/27 18:03:24  fracar
	added id/def management
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.1  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	

*/

#ifndef _ATEXITSEQUENCE_H_
#define _ATEXITSEQUENCE_H_

#include <acs_c++config.hpp>
#include <exException.h>
					   
#include <TaskTableElement.h>
#include <Sequence.h>
#include <TaskDataTypes.h>
#include <WeightNormalizer.h>

_ACS_BEGIN_NAMESPACE(acs)
	
/**
 *\brief This class is the representation of an AtExit sequence
 *       Upon completion of a processing the scheduler can execute
 *       an extra-sequence (the AtExitSequence) according to
 *       the exit status of the processor
 */

class AtExitSequence : public Sequence // PRQA S 2153
{

public:
	
	/**
	 * CTOR and copy CTOR
	 */ 
	AtExitSequence();
	AtExitSequence(const AtExitSequence&);

	/**
	 * DTOR
	 */ 
	virtual ~AtExitSequence() throw() ;

	/**
	 * Methods for set and get the exit status triggering the sequence
	 * Not all the possible status of the scheduler are mapped here
	 * Only OK, ERROR and ABORT status are allowed 
	 */ 
	bool isSetExitStatus() const;
	void setExitStatus (ExitStatus);
	ExitStatus getExitStatus () const;

	/**
	 * Methods for set and get the override status flag
	 * This flag is used to override the general processor exit status when 
	 * exiting from the at_exit sequence
	 */ 
	bool isSetOverrideStatus () const;
	void setOverrideStatus (bool);
	bool getOverrideStatus () const;

	/**
	 * Methods for set and get the sequence to be executed at exit
	 */
	bool isSetSequence() const;
	//void setSequence (const Sequence&);
	//Sequence getSequence() const;


	/**
	 *  Implementation for the virtual interface TaskTableElement
	 */

	/**
	 * Clone method implementation
	 */ 
	virtual TaskTableElement* clone() const; // PRQA S 2502
	virtual bool acceptEnterVisit (TaskTableVisitor&);
	virtual bool acceptVisit (TaskTableVisitor&);
	virtual bool acceptExitVisit (TaskTableVisitor&);
	
	/**
	 * Copy operator
	 */ 
	AtExitSequence& operator = (const AtExitSequence&);

	/** Apply normalization
	 *  It is possible to specify a normalizer or to use the default one
	 */ 
	virtual void normalize(acs::WeightNormalizer* = 0);	

protected:
	// Disable the aggregation feature
	// void push_back (const TaskTableElement&);
	// void push_back (TaskTableElement*);
	
private:
	// attributes
	ExitStatus _onExitStatus; 
	bool _overrideStatus;
	bool _onExitStatusSet; 
	bool _overrideStatusSet;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(AtExitSequence) ;
};

_ACS_END_NAMESPACE

#endif //_ATEXITSEQUENCE_H_

