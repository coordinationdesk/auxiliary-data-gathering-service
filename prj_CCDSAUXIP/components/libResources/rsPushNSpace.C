// PRQA S 1050 EOF
/*

    Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

 	$Prod: A.C.S. Resources Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2013/07/16 17:00:44  marpas
	getArraySize returns 0 if the array is not present
	returns 1 if the array is not a real array but a single value
	getArray and setArray are now template methods managing the above two statements.
	
	Revision 5.0  2013/06/11 09:29:11  marpas
	adopting libException 5.x standards
	
	Revision 2.2  2013/02/22 13:56:41  marpas
	rsResourceSet does not longer inherit virtually from ThreadSafe, it has an internal mutex instead
	Interface rationalization
	coding best practices
	
	Revision 2.1  2012/11/20 22:39:55  marpas
	total quality: code, coverage etc.
	
	Revision 2.0  2006/02/28 08:34:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/01/12 13:33:58  marpas
	headers fixed
	
	Revision 1.4  2003/09/25 08:47:36  achval
	resourceSet can be deleted even if some rsPushNSpace is not yet out of scope
	
	Revision 1.3  2003/07/15 18:10:36  marpas
	global namespace concept implemented
	
	Revision 1.2  2003/04/30 09:20:38  marpas
	using namespace std removed from includes
	
	Revision 1.1.1.1  2002/10/03 20:54:59  marpas
	libResources import
	
	

*/

#include <rsPushNSpace.h>
#include <rsResourceSet.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using std::string ;

//////////////////////////////
//							//
// classe rsPushNSpace		//
//							//
//////////////////////////////
ACS_CLASS_DEFINE_DEBUG_LEVEL(rsPushNSpace) 

rsPushNSpace::rsPushNSpace(rsResourceSet &rs, const string &ns, bool global) :
	_rs(rs), // PRQA S 2528 
	_ns(ns),
	_global(global)
{
	push() ;
}

rsPushNSpace::~rsPushNSpace() noexcept 
{
    try {
	    pop() ; // PRQA S 4631
    }
    catch(const std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ; // PRQA S 4631
    }
}


void rsPushNSpace::pop() // PRQA S 4020
{

	if(!_valid) { return ; }
    
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "popping namespace: " << _ns.c_str()
				<< " before: " << _rs.currentNameSpace().c_str()) ;
	
	_rs.unregisterNameSpace(this) ;
	_rs.popNameSpace() ;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "popping namespace: " << _ns.c_str()
				<< " after: " << _rs.currentNameSpace().c_str()) ;
}

void rsPushNSpace::push()
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "pushing namespace: " << _ns.c_str()
				<< " before: " << _rs.currentNameSpace().c_str()) ;
	
	_rs.registerNameSpace(this) ;
	_rs.pushNameSpace(_ns, _global) ;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "pushing namespace: " << _ns.c_str()
				<< " after: " << _rs.currentNameSpace().c_str()) ;
	
}

void rsPushNSpace::invalidate() noexcept 
{
	_valid = false ;
}

const string &rsPushNSpace::pushedNS(bool *global) const 
{ 
	if (global) { *global = _global ; }
	return _ns ; 
}



_ACS_END_NAMESPACE

