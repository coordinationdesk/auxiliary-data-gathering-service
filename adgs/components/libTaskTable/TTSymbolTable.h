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
	
	Revision 2.2  2006/03/23 14:53:51  fracar
	added support to id/ref in input definition
	
	Revision 2.1  2006/03/06 14:19:15  marfav
	Added default reader and writer
	

*/

#ifndef _TTSymbolTable_H_
#define _TTSymbolTable_H_

#include <exException.h>
#include <TaskTableVisitor.h>
#include <InputDescriptor.h>

#include <map>

_ACS_BEGIN_NAMESPACE(acs)


class TTSymbolTable : public TaskTableVisitor // PRQA S 2109, 2153
{
public:
	// General purpose exception
	exDECLARE_EXCEPTION(TTSymbolTableException, acs::exException) ; // PRQA S 2131, 2502

	/**
	 *\brief This is the default implementation for the TaskTable Writer 
	 */ 
	
	TTSymbolTable();
	virtual ~TTSymbolTable() throw() ;
    
    // Un-hide not redefined operations
    using TaskTableVisitor::visit;

	virtual bool visit (InputDescriptor&);



	/**
	 *\brief Retrieves an Input descriptor from the symbol table
	 *       based on its Id
	 */ 
	bool getInput(const std::string& id, InputDescriptor& d) const;



private:
	TTSymbolTable (const TTSymbolTable&); // not implemented
	TTSymbolTable& operator= (const TTSymbolTable&); // not implemented

private:
	// Attributes
	std::map<std::string, InputDescriptor> _symbolTable;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(TTSymbolTable)

}; // class DefaultTTWRiter
	
_ACS_END_NAMESPACE


#endif //_TTSymbolTable_H_

