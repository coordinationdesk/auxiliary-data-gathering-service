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
	
	Revision 2.9  2012/06/07 11:54:03  marfav
	Code improved removing compiler warnings
	
	Revision 2.8  2012/02/10 17:14:05  marpas
	refactoring in progress
	
	Revision 2.7  2009/11/25 11:11:34  marfav
	Support to alternative Filter_Def completed
	
	Revision 2.6  2009/11/20 17:26:40  marfav
	Added support for Alternative_Filter_Tag optional field
	
	Revision 2.5  2008/10/20 13:19:25  marfav
	added support to task cmdline parameters
	
	Revision 2.4  2008/04/22 11:17:09  marfav
	Design improved using the conditioned visitor pattern
	
	Revision 2.3  2006/03/23 14:53:51  fracar
	added support to id/ref in input definition
	
	Revision 2.2  2006/03/06 14:27:02  marfav
	Exception 2.1 I/F adopted
	
	Revision 2.1  2006/03/06 14:19:15  marfav
	Added default reader and writer
	

*/

#ifndef _DwfaultTTReader_H_
#define _DwfaultTTReader_H_

#include <TaskTableVisitor.h>
#include <OrderTypeMgr.h>
#include <DefaultRWDefinitions.h>
#include <rsResourceSet.h>

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet;

class DefaultTTReader : public TaskTableVisitor // PRQA S 2109, 2153
{
public:

	// General purpose exception
	exDECLARE_EXCEPTION(DefaultTTReaderException, acs::exException) ; // PRQA S 2131, 2502

	/**
	 *\brief This is the default implementation for the TaskTable Reader 
	 */ 
	explicit DefaultTTReader(const std::string&);
	virtual ~DefaultTTReader() throw() ;
    
    // Un-hide not redefined operations
    using TaskTableVisitor::enterVisit;
    using TaskTableVisitor::visit;
    using TaskTableVisitor::exitVisit;

	// enterVisit needed to avoid reload List scanned during Task visit
	virtual bool enterVisit (InputList&);
	virtual bool enterVisit (OutputList&);
	virtual bool enterVisit (BreakpointList&);
	virtual bool enterVisit (TaskParameterList&);

	virtual bool visit (TaskTable&);
	virtual bool visit (Task&);
	virtual bool visit (ConfigSpaceDescriptor&);
	virtual bool visit (ConfigDescriptor&);
	virtual bool visit (Sequence&);
	virtual bool visit (Pool&);
	virtual bool visit (TaskParameterDescriptor&);
	virtual bool visit (InputDescriptor&);
	virtual bool visit (InputAlternative&);
	virtual bool visit (OutputDescriptor&);
	virtual bool visit (BreakpointDescriptor&);
	virtual bool visit (AtExitSequence&);

	// exitVisit needed to avoid navigation of children loaded during visit
	virtual bool exitVisit (TaskTable&);
	virtual bool exitVisit (Task&);
	virtual bool exitVisit (Sequence&);
	virtual bool exitVisit (Pool&);
	virtual bool exitVisit (InputDescriptor&);
	virtual bool exitVisit (AtExitSequence&);

private:
	DefaultTTReader (); // not implemented
	DefaultTTReader (const DefaultTTReader&); // not implemented
	DefaultTTReader& operator= (const DefaultTTReader&); // not implemented

	// Tries to discover the array size of a resource
	// Checking the "count.value" attribute and raising an exception
	// if the count value is not aligned with the array size
	int getArraySize (rsResourceSet&, const std::string& countKey, const std::string& arrayKey, const std::string& elementKey);
	
private:
	// Attributes
	std::string _ttPathname;
	OrderTypeMgr _theOTM;
	DefaultReadWriteTaskTableDefs _defs;
	rsResourceSet _rs;
	std::string _alternativeFilterTag;
	std::string _taskTableRootTag;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(DefaultTTReader)
};

_ACS_END_NAMESPACE
		 
#endif //_DwfaultTTReader_H_
	   
