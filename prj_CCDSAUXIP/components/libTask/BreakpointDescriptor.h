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

#ifndef _BREAKPOINTDESCRIPTOR_H_
#define _BREAKPOINTDESCRIPTOR_H_


#include <acs_c++config.hpp>
#include <TaskTableVisitable.h>
#include <TaskDataTypes.h>

_ACS_BEGIN_NAMESPACE(acs)

class BreakpointList : public TaskTableVisitable
{
public:
	BreakpointList();
	BreakpointList(const BreakpointList&);
	virtual ~BreakpointList() throw() {} // PRQA S 2131
	/**
	 * Operator =
	 * Used by copy ctor
	 */
	BreakpointList& operator = (const BreakpointList&);


	/**
	 *  Implementation for the virtual interface TaskTableVisitable
	 */
	virtual TaskTableVisitable* clone() const; // PRQA S 2502
	virtual bool acceptEnterVisit (TaskTableVisitor&);
	virtual bool acceptVisit (TaskTableVisitor&);
	virtual bool acceptExitVisit (TaskTableVisitor&);

	virtual bool isContainer() const {return true;} // PRQA S 2131

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(BreakpointList) ;
};

/**
 *\brief This class is the representation of a processor breakpoint
 *       The processors can be configure to write in the wd some debug files
 *       named breakpoint files
 */
	
class BreakpointDescriptor : public TaskTableVisitable
{
public:
	
	/**
	 * CTOR and copy CTOR
	 */ 
	BreakpointDescriptor();
	BreakpointDescriptor(const BreakpointDescriptor& d);
	virtual ~BreakpointDescriptor() throw() {} // PRQA S 2131

	/**
	 * Copy operator
	 */ 
	BreakpointDescriptor& operator = (const BreakpointDescriptor& d);

	/**
	 *  Methods for setting and getting the filetype of the breakpoint file
	 */
	bool isSetFileType() const;
	std::string getFileType() const;
	void setFileType(const std::string& aType);

	/**
	 *  Methods for setting and getting the pathname of the breakpoint file
	 */
	bool isSetPathName() const;
	std::string getPathName() const;
	void setPathName(const std::string& aPath);
    
	/**
	 *  Methods for setting and getting the file name type of the breakpoint file
	 */
	bool isSetFileNameType() const;
	FileNameType getFileNameType() const;
	void setFileNameType (FileNameType aType);

	/**
	 *  Implementation for the virtual interface TaskTableElement
	 */
	virtual bool acceptEnterVisit (TaskTableVisitor&);
	virtual bool acceptVisit (TaskTableVisitor&);
	virtual bool acceptExitVisit (TaskTableVisitor&);
	virtual TaskTableVisitable* clone() const; // PRQA S 2502
	
private:
	// Attributes
	std::string _FileType;
	bool _FileTypeSet;
	std::string _PathName;
	bool _PathNameSet;
	FileNameType _NameType;
	bool _NameTypeSet;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(BreakpointDescriptor) ;
};
	

_ACS_END_NAMESPACE

#endif //_BREAKPOINTDESCRIPTOR_H_

