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
	
	Revision 2.2  2008/04/22 11:07:46  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.1  2006/03/27 18:03:24  fracar
	added id/def management
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.1  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	

*/

#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_

#include <acs_c++config.hpp>
#include <Pool.h>

_ACS_BEGIN_NAMESPACE(acs)

/** 
 *\brief This class represents the Sequence element of the Task Table
 *       as per Gen Proc ICD
 *       The Sequence structure is the same of the Pool
 */
	 
class Sequence : public Pool // PRQA S 2153
{
public:

	/**
	 * CTORS and DTOR
	 */ 
	Sequence(); 
	Sequence(const Sequence&);
	virtual ~Sequence() throw() ;

	
	/**
	 * Clone method implementation
	 */ 
	virtual TaskTableElement* clone() const; // PRQA S 2502

	/**
	 * Assignment operator
	 */ 
	Sequence& operator= (const Sequence&);
	
	virtual bool acceptVisit (TaskTableVisitor&);
	virtual bool acceptEnterVisit (TaskTableVisitor&);
	virtual bool acceptExitVisit (TaskTableVisitor&);


private:
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(Sequence) ;

};

_ACS_END_NAMESPACE


#endif //_SEQUENCE_H_
