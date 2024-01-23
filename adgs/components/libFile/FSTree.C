// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

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

using namespace std;


// Unnamed namespace (hide this function to other modules)
namespace 
{
    bool DefaultNOPPredicate (const std::string& path, const acs::os::FSTree::Entry&)
    { return true ; }
};


_ACS_BEGIN_NESTED_NAMESPACE(acs, os)

FSTree::Entry::Entry () :
    stat_r(),
    dirent_r()
{}

FSTree::Entry::Entry (const FSTree::Entry & e) :
    stat_r (e.stat_r),
    dirent_r (e.dirent_r)
{}

const FSTree::Entry& FSTree::Entry::operator= (const FSTree::Entry& e)
{
    if (this != &e)
    {
        stat_r = e.stat_r;
        dirent_r = e.dirent_r;
    }
    return *this;
}


// FSTree implementation
FSTree::FSTree (const string& basePath):
        _path(basePath),
        _maxDepth (0),
        _hidden (true),
        _resolveLinks (true),
        _fatalFailures (true),
        _tree()
{ 
    // NOP Ctor
    // Load must be called after proper object confugration to avoid multiple load calls
}

// Copy CTOR
FSTree::FSTree (const FSTree& f) :
        _path (f._path),
        _maxDepth (f._maxDepth),
        _hidden (f._hidden),
        _resolveLinks (f._resolveLinks),
        _fatalFailures (f._fatalFailures),
        _tree(f._tree)
{}

// Assignment
const FSTree& FSTree::operator= (const FSTree& f)
{
    if (this != &f)
    {
        _path = f._path;
        _maxDepth =f._maxDepth;
        _hidden = f._hidden;
        _resolveLinks = f._resolveLinks;
        _fatalFailures = f._fatalFailures;
        _tree = f._tree;
    }
    return *this;
}

void FSTree::load()
{
    this->load (DefaultNOPPredicate);
}


_ACS_END_NESTED_NAMESPACE

