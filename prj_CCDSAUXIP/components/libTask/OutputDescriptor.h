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

#ifndef _OUTPUTDESCRIPTOR_H_
#define _OUTPUTDESCRIPTOR_H_


#include <acs_c++config.hpp>
#include <exException.h>
							   
#include <TaskDataTypes.h>
#include <TaskTableVisitable.h>



_ACS_BEGIN_NAMESPACE(acs)


class OutputList : public TaskTableVisitable
{
public:
	OutputList();
	OutputList(const OutputList&);
	virtual ~OutputList() throw() {}  // PRQA S 2131

	/**
	 * Operator =
	 * Used by copy ctor
	 */
	OutputList& operator = (const OutputList&);


	/**
	 *  Implementation for the virtual interface TaskTableVisitable
	 */
	virtual TaskTableVisitable* clone() const; // PRQA S 2502
	virtual bool acceptEnterVisit (TaskTableVisitor&);
	virtual bool acceptVisit (TaskTableVisitor&);
	virtual bool acceptExitVisit (TaskTableVisitor&);

	virtual bool isContainer() const {return true;} // PRQA S 2131

private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(OutputList) ;
};

	
/**
 * \brief This class is the representation of an output descriptor
 *        Output has neither alternatives nor intervals... One output, one filename
 */
class OutputDescriptor : public TaskTableVisitable
{
public:
	/**
	 * CTOR and copy CTOR 
	 */ 
	OutputDescriptor();
	OutputDescriptor(const OutputDescriptor& d);
	virtual ~OutputDescriptor() throw() {} // PRQA S 2131
	
	/**
	 * Methods for setting and getting the destination of the represented file
	 * Destination can be PROC for local files or DB for files that need upload in database
	 * or DBPROC for files that both need upload in database and are locally input for following tasks
	 */ 
	bool isSetDestination() const;
	void setDestination(OutputDest aDestination);
	OutputDest getDestination() const;
	
	/**
	 *  Methods for setting and getting the Output FileType
	 *  the FileType is a plain string mapped to database or local filetypes
	 */
	bool isSetFileType() const;
	void setFileType(const std::string& aType);
	void setFileTypeTag(const std::string& aTypeTag);
	std::string getFileType() const;
	std::string getFileTypeTag() const;

	/**
	 *  Methods for setting and getting the Path of the Output File
	 */
	bool isSetPathName() const;
	void setPathName(const std::string& aPath);
	std::string getPathName() const;
    
	/**
	 *  Methods for setting and getting the Filename Type
	 *  the parameter is defined using the enumerative type FileNameType
	 */
	bool isSetFileNameType() const;
	FileNameType getFileNameType() const;
	void setFileNameType (FileNameType aType);

	/**
	 *  Methods for setting and getting the Mandatory flag for the output
	 */
	bool isSetMandatory() const;
	bool isMandatory() const;
	void isMandatory(bool val);
	
	/**
	 * Operator =
	 * Used by copy ctor
	 */
	OutputDescriptor& operator = (const OutputDescriptor& d);

	/**
	 *  Implementation for the virtual interface TaskTableVisitable
	 */
	virtual TaskTableVisitable* clone() const; // PRQA S 2502
	virtual bool acceptEnterVisit (TaskTableVisitor&);
	virtual bool acceptVisit (TaskTableVisitor&);
	virtual bool acceptExitVisit (TaskTableVisitor&);

private:
	// attributes
	OutputDest _Destination;
	bool _IsMandatory;
	std::string _FileType;
	std::string _FileTypeTag;
	std::string _PathName;
	FileNameType _NameType;
	bool _DestinationSet;
	bool _IsMandatorySet;
	bool _FileTypeSet;
	bool _PathNameSet;
	bool _NameTypeSet;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(OutputDescriptor) ;
};

_ACS_END_NAMESPACE


#endif //_OUTPUTDESCRIPTOR_H_

