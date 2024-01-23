// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Application Library $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2015/10/22 16:09:17  marpas
	perfo, readability and mantenability of code improved
	
	Revision 1.1  2015/06/12 08:01:32  tergem
	Monitoring anc Control Filter class added
	

*/


#include <MCDefaultFilter.h>
#include <Filterables.h>
#include <exFMCFilterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
using namespace mc;

ACS_CLASS_DEFINE_DEBUG_LEVEL(MCDefaultFilter)


MCDefaultFilter::MCDefaultFilter(exostream *s) : exFilter(s)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);	
}

MCDefaultFilter::~MCDefaultFilter() ACS_NOEXCEPT
{
}

bool MCDefaultFilter::filterInsertion(exFilterable &fo) // PRQA S 4020
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);	
/*
	exFEvent* fevent = dynamic_cast<exFEvent*>   (&fo); // PRQA S 3081
	exFMCCritical* fMCcritical =  dynamic_cast<exFMCCritical*>   (&fo); // PRQA S 3081
	exFMCError* fMCerror =  dynamic_cast<exFMCError*>	(&fo); // PRQA S 3081
	exFMCWarning* fMCwarning =  dynamic_cast<exFMCWarning*>   (&fo); // PRQA S 3081
	exFMCInfo* fMCinfo =  dynamic_cast<exFMCInfo*>   (&fo); // PRQA S 3081
	
	bool retval = true;

	//only event and MC filterables are catched
	if ( ( 0 == fevent)  && ( 0 == fMCcritical) && ( 0 == fMCerror ) &&  ( 0 == fMCwarning) && ( 0 == fMCinfo) ) 	{
		retval = false;
	}

	return retval;
*/ 
// new implementation - trying to improve perfo:
// first, using the likelihood of filterables, then avoiding useless operations
// then, is the following code harder to read or mantain than previous one ?
    if (dynamic_cast<exFMCInfo*>(&fo) != 0) { // is an exFMCInfo // PRQA S 3081 L1
        return true ;
    }
    if (dynamic_cast<exFMCWarning*>(&fo) != 0) { // is an exFMCWarning
        return true ;
    }
    if (dynamic_cast<exFMCError*>(&fo) != 0) { // is an exFMCError
        return true ;
    }
    if (dynamic_cast<exFMCCritical*>(&fo) != 0) { // is an exFMCCritical
        return true ;
    }
    if (dynamic_cast<exFEvent*>(&fo) != 0) { // is an exFEvent
        return true ;
    }
    // PRQA L: L1
    return false ;
}

_ACS_END_NAMESPACE
