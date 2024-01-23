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
	Revision 5.0  2013/07/07 18:24:07  marpas
	itroducing libException 5.x standards
	coding best practices applied
	qa warnings removed
	compilation warnings partially removed
	
	Revision 2.3  2012/06/07 11:54:03  marfav
	Code improved removing compiler warnings
	
	Revision 2.2  2012/02/10 17:14:05  marpas
	refactoring in progress
	
	Revision 2.1  2008/04/22 11:17:09  marfav
	Design improved using the conditioned visitor pattern
	
	Revision 2.3  2006/03/23 14:53:51  fracar
	added support to id/ref in input definition
	
	Revision 2.2  2006/03/06 14:27:02  marfav
	Exception 2.1 I/F adopted
	
	Revision 2.1  2006/03/06 14:19:15  marfav
	Added default reader and writer
	

*/

#ifndef _ResolverVisitor_H_
#define _ResolverVisitor_H_

#include <TaskTableVisitor.h>
#include <InputDescriptor.h>
#include <TTSymbolTable.h>

#include <exException.h>



	
_ACS_BEGIN_NAMESPACE(acs)
										

class ResolverVisitor : public TaskTableVisitor // PRQA S 2109, 2153
{
public:

	// General purpose exception
	exDECLARE_EXCEPTION(ResolverVisitorException, acs::exException) ; // PRQA S 2131, 2502

	/**
	 *\brief This is the default implementation for the TaskTable TaskTable 
	 */ 
	explicit ResolverVisitor(const TTSymbolTable&);
	virtual ~ResolverVisitor() throw() ;
    
    // Un-hide not redefined operations
    using TaskTableVisitor::visit;

	virtual bool visit (InputDescriptor&);


private:
	ResolverVisitor (); // not implemented
	ResolverVisitor (const ResolverVisitor&); // not implemented
	ResolverVisitor& operator= (const ResolverVisitor&); // not implemented

private:
	// Attributes
	const TTSymbolTable& _theSymbolTable;


	ACS_CLASS_DECLARE_DEBUG_LEVEL(ResolverVisitor) ;
};


_ACS_END_NAMESPACE

		 
#endif //_ResolverVisitor_H_
	   
