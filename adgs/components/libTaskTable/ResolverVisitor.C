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


#include <ResolverVisitor.h>
#include <TaskTable.h>
#include <ResolverVisitor.h>

#include <rsResourceSet.h>
#include <rsPushNSpace.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ResolverVisitor)

ResolverVisitor::ResolverVisitor(const TTSymbolTable& st) :
    TaskTableVisitor(),
	_theSymbolTable(st) // PRQA S 2528
{
}

ResolverVisitor::~ResolverVisitor() throw() 
{
}



bool 
ResolverVisitor::visit (InputDescriptor& desc)
{
    if (desc.isSetRef())
    {
        // ...unresolved references are evil!

	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "checking for unresolved Ref " << desc.getRef()) ;

        //
        // search for this id
        //

        InputDescriptor d; // will contain (if any) retrieved input in the symbol table matching pointed by this one
        if (!_theSymbolTable.getInput(desc.getRef(), d))  // not found!
        {

	        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Unresolved Input Reference " << desc.getRef() << " found") ;

	        ostringstream os;
	        os << "Unresolved Input Reference " << desc.getRef() << " found";
	        exIllegalValueException ex(os.str());
	        ACS_THROW (ex);
        }
        else
        {
		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Input Reference " << desc.getRef() << " successfully resolved") ;

	        //
	        // ok... green light... let's admit the new input into the high society ;-)
	        //


	        {
		        // at this point, d contains the referenced input...
		        // after all, we successfully looked for it a couple of moments ago!


		        // let's copy input and align flags
		        string Ref = desc.getRef();
		        string ordertype = desc.getOrderType();
		        bool mandatory = desc.isMandatory();

		        desc = d;


		        // restore mandatory flag and order type in case they've been overwritten
		        // (can change in Ref input wrt. Id)
		        desc.isMandatory(mandatory);
		        desc.setOrderType(ordertype);

		        desc.resetId();   // unset ID...
		        desc.setRef(Ref); // ... so it will be treated as Ref
				          // even if some more fields have been filled-in
	        }
        }
    }

    return true;
}


_ACS_END_NAMESPACE
