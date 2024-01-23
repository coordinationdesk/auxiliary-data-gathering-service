// PRQA S 1050 EOF
/*

	Copyright 2011-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

 	$Prod: A.C.S. Safe Product Base Library $
 
 	$Id$
 
 	$Author$
 
 	$Log$
 	Revision 1.1  2011/09/12 08:32:15  aleber
 	first implementation
 	

*/

#include <ProductContainerBase.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ProductContainerBase)


ProductContainerBase::ProductContainerBase( 
		const string &productClass,
		const string &productClassDescription
):
    _productClass(productClass),
    _productClassDescription(productClassDescription)

{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: " << Id) 

}

ProductContainerBase::~ProductContainerBase() throw()
{
}

// copy constructor
ProductContainerBase::ProductContainerBase(const ProductContainerBase & e) :
    _productClass(e._productClass),
    _productClassDescription(e._productClassDescription)
{
}

// operator =
ProductContainerBase&  ProductContainerBase::operator=(const ProductContainerBase & e)
{
	if (this != &e) {
	    _productClass = e._productClass;
	    _productClassDescription = e._productClassDescription;
	}
	return *this ;
}


string ProductContainerBase::toStr() const                                                                                   
{                                                                                                           

	std::ostringstream msg; 
	msg << "\tProductContainerBase content: " << "\n"
	    << "\t\t productClass    =" <<  _productClass  << "\n"
	    << "\t\t productClassDescription     =" <<  _productClassDescription; 
	return msg.str() ;
	
}                                                                                                           



_ACS_END_NAMESPACE


