// PRQA S 1050 EOF
/*

	Copyright 2002-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.1  2013/12/09 14:01:12  giucas
	Library completely revisited.
	

*/

#include <ProductTypeDescriptor.h>
#include <ProdTypeDescrV.h>

_ACS_BEGIN_NAMESPACE(acs)

namespace {
	ProdTypeDescrV vers;
}

/*
 * Class constructors
 */
ProductTypeDescriptor::ProductTypeDescriptor()
{
}

ProductTypeDescriptor::ProductTypeDescriptor(const ProductTypeDescriptor &)
{
}

ProductTypeDescriptor& ProductTypeDescriptor::operator=(const ProductTypeDescriptor &r)
{
    if (this != &r) { }
    return *this ;
}


/*
 * Class destructor
 */
ProductTypeDescriptor::~ProductTypeDescriptor() throw()
{
}

_ACS_END_NAMESPACE



