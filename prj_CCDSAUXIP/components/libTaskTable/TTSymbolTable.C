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
	Revision 5.1  2013/11/19 17:35:12  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
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

#include <TTSymbolTable.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(TTSymbolTable)

TTSymbolTable::TTSymbolTable() :
    TaskTableVisitor(),
    _symbolTable()
{
}

TTSymbolTable::~TTSymbolTable() throw() 
{
	// Empty DTOR
}

bool
TTSymbolTable::visit (InputDescriptor& inDes)
{

     /*
      *  ONLY LOAD NAMED INPUTS, TO BUILD THE SYMBOL TABLE
      */

    if (inDes.isSetId())
    {
	// duplicate IDs are evil...!
	    {
		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "checking for duplicated ID " << inDes.getId()) ;
	        //
	        // search for this id
	        //

	        InputDescriptor desc;
	        if (this->getInput(inDes.getId(), desc))  // found!
	        {
			    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "duplicated ID " << inDes.getId() << " found") ;

		        ostringstream os;
		        os << "Duplicated Id " << inDes.getId() << " found";
		        exIllegalValueException ex(os.str());
		        ACS_THROW (ex);
	        }
	        else
	        {
			    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ID " << inDes.getId() << " not found as duplicated") ;
	        }
	    }

	    //
	    // ok... green light... let's admit the new input into the high society ;-)
	    //
	    {
	        _symbolTable[inDes.getId()] = inDes;

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Input ID " << inDes.getId() << " inserted in symbol table") ;
	    }
    }


    return true;
}


bool
TTSymbolTable::getInput(const string& id, InputDescriptor& d) const // PRQA S 4020
{
    map<std::string, InputDescriptor>::const_iterator desc = _symbolTable.find(id);

    if (desc == _symbolTable.end()) {
        return false;
    }
    else {
        d = desc->second;
    }

    return true;
}


_ACS_END_NAMESPACE
