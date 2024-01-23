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
	
	Revision 2.6  2012/06/07 11:54:03  marfav
	Code improved removing compiler warnings
	
	Revision 2.5  2012/02/10 17:14:05  marpas
	refactoring in progress
	
	Revision 2.4  2008/10/20 13:19:25  marfav
	added support to task cmdline parameters
	
	Revision 2.3  2008/04/22 11:17:09  marfav
	Design improved using the conditioned visitor pattern
	
	Revision 2.2  2006/03/23 14:53:51  fracar
	added support to id/ref in input definition
	
	Revision 2.1  2006/03/06 14:19:15  marfav
	Added default reader and writer
	

*/

#ifndef _DefaultTTWriter_H_
#define _DefaultTTWriter_H_

#include <exException.h>
#include <TaskTableVisitor.h>
#include <OrderTypeMgr.h>
#include <DefaultRWDefinitions.h>
#include <XMLOstream.h>

#include <stack>

_ACS_BEGIN_NAMESPACE(acs)


class DefaultTTWriter : public TaskTableVisitor // PRQA S 2109, 2153
{
public:
	// General purpose exception
	exDECLARE_EXCEPTION(DefaultTTWriterException, acs::exException) ; // PRQA S 2131, 2502

	/**
	 *\brief This is the default implementation for the TaskTable Writer 
	 */ 
	
	explicit DefaultTTWriter (XMLOstream&);
	virtual ~DefaultTTWriter() throw() ;
    
    // Un-hide not redefined operations
    using TaskTableVisitor::enterVisit;
    using TaskTableVisitor::visit;
    using TaskTableVisitor::exitVisit;
    
	virtual bool enterVisit (AtExitSequence&);
	virtual bool enterVisit (BreakpointList&);
	virtual bool enterVisit (InputDescriptor&);
	virtual bool enterVisit (InputList&);
	virtual bool enterVisit (OutputList&);
	virtual bool enterVisit (Pool&);
	virtual bool enterVisit (Sequence&);
	virtual bool enterVisit (Task&);
    virtual bool enterVisit (TaskParameterList&);

	virtual bool visit (AtExitSequence&);
	virtual bool visit (BreakpointDescriptor&);
	virtual bool visit (ConfigDescriptor&);
	virtual bool visit (ConfigSpaceDescriptor&);
	virtual bool visit (InputAlternative&);
	virtual bool visit (InputDescriptor&);
	virtual bool visit (OutputDescriptor&);
	virtual bool visit (Pool&);
	virtual bool visit (Sequence&);
	virtual bool visit (Task&);
	virtual bool visit (TaskTable&);
    virtual bool visit (TaskParameterDescriptor&);

	virtual bool exitVisit (AtExitSequence&);
	virtual bool exitVisit (BreakpointList&);
	virtual bool exitVisit (InputDescriptor&);
	virtual bool exitVisit (InputList&);
	virtual bool exitVisit (OutputList&);
	virtual bool exitVisit (Pool&);
	virtual bool exitVisit (Sequence&);
	virtual bool exitVisit (Task&);
	virtual bool exitVisit (TaskTable&);
    virtual bool exitVisit (TaskParameterList&);



private:
    DefaultTTWriter() ; // not implmented
	DefaultTTWriter (const DefaultTTWriter&); // not implemented
	DefaultTTWriter& operator= (const DefaultTTWriter&); // not implemented

private:
	// Attributes
	OrderTypeMgr _theOTM;
	DefaultReadWriteTaskTableDefs _defs;
	XMLOstream& _theOstream;
	std::stack<XMLOstream::Tag*> _tagsStack; // will hold tags opened in enterVisit and
						 // closed in exitVisit
						 // (to handle List_of_... tags)


	ACS_CLASS_DECLARE_DEBUG_LEVEL(DefaultTTWriter) ;
};


_ACS_END_NAMESPACE

#endif //_DefaultTTWriter_H_

