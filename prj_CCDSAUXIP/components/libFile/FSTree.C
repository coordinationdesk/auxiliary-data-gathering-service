/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. File Tools Library $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2014/02/10 17:49:27  marfav
	Forward conditioned iterator and const iterator added
	
	Revision 5.1  2014/02/07 15:32:59  marfav
	FSTree class added
	

*/

#include <FSTree.h>

// Unnamed namespace (hide this function to other modules)
namespace 
{
    bool DefaultNOPPredicate (const std::string&, const acs::os::FSTree::Entry&)
    { return true ; }
};


_ACS_BEGIN_NESTED_NAMESPACE(acs, os)

using std::string ;

// FSTree implementation
FSTree::FSTree (const string& basePath):
        _path(basePath)
{ 
    // NOP Ctor
    // Load must be called after proper object confugration to avoid multiple load calls
}

void FSTree::load()
{
    this->load (DefaultNOPPredicate);
}


_ACS_END_NESTED_NAMESPACE

