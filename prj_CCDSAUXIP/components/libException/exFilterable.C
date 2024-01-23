// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2013/10/11 09:39:48  marpas
	streams graph traversal implemented to avoid multiple notification of the same filterable due to complex graphs
	
	Revision 5.1  2013/06/06 08:33:49  marpas
	namespaces enforced
	
	Revision 5.0  2013/06/04 15:35:02  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.2  2012/06/22 13:41:47  marpas
	pr qa instrumentation
	
	Revision 4.1  2012/06/12 17:37:01  marpas
	qacpp instrumentation
	
	Revision 4.0  2011/10/18 11:22:00  marpas
	new statistics
	
	Revision 3.0  2009/10/08 10:48:09  marpas
	Statistics class added
	rules application improved
	
	Revision 2.3  2009/06/26 10:41:55  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.2  2009/03/02 17:02:39  marpas
	CODECHECK parsing - no regressions
	
	Revision 2.1  2006/02/28 08:24:36  marpas
	new release, exException inherits from std::exception
	
	Revision 1.2  2003/07/10 14:43:39  marpas
	header make-up
	
	Revision 1.1  2002/10/03 15:05:38  marpas
	exFilter and exFilterable added
	
*/

#include <exFilterable.h>
#include <exException.h>
#include <algorithm>
#include <iostream>

_ACS_BEGIN_NAMESPACE(acs)



//
//
// exFilterable
//
//


exFilterable::exFilterable() :
    traversalRegister_()
{
}

bool exFilterable::traversed( exostream const * const s) noexcept // PRQA S 4211, 4670
{
    return (traversalRegister_ && traversalRegister_->traversed(s)) ; // PRQA S 3226, 3230
}


bool StreamGraphTraversal::traversed(exostream const * const s) noexcept // PRQA S 4020
{
    try {
        if (std::find(traversedStreams_.begin(), traversedStreams_.end(), s) == traversedStreams_.end()) { // PRQA S 3232
            traversedStreams_.push_back(s) ;
            return false ;
        }
        return true ;
    }
    catch(const std::exception &x) {
        std::cerr << explain(x) ; // NOSONAR - cannot use streams
        return false ;
    }
}

_ACS_END_NAMESPACE
