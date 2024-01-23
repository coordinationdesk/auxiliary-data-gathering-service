/*

	Copyright 1995-2003, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer Daemon $

	$Id$

	$Author$

	$Log$
	Revision 3.2  2017/10/23 15:46:33  marfav
	Resource management service integration in progress
	
	Revision 3.1  2017/02/02 16:12:55  marfav
	Removed inheritance from stl vector
	
	Revision 3.0  2016/10/11 08:27:16  marfav
	Major release due to cache management modifications
	
	Revision 2.2  2015/12/01 17:44:45  marpas
	coding best practice application in progress
	
	Revision 2.1  2015/11/19 17:23:03  marpas
	coding best practices application in progress
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.3  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.2  2004/05/17 08:21:18  marfav
	thread safety extended to size method
	
	Revision 1.1  2004/05/12 08:27:02  marfav
	Importing OrderSlot class for Slot Number Management
	

*/


#include <OrderSlot.h>
#include <ExecutingOrder.h>
#include <algorithm>
#include <set> 

#include <TlStatusMgrSingleton.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

OrderSlot::OrderSlot () : _slots(), _maxEmittedSlot(0)
{
}

unsigned int
OrderSlot::getSlotNumber()
{
	ThreadSafe::Lock (*this);
	// Assuming that the check on the resource availability has been performed by the caller

	// At least one more of the currently used slots
	// Create all the slot numbers between 1 and the size of running executors +1
	set<unsigned int> slots;
	for (unsigned int idx=1; idx<=_slots.size()+1; ++idx)
	{
		slots.insert (idx);
	}

	// Remove now all the running slot numbers from the set
    size_t sz = _slots.size() ;
	for (size_t i=0; i<sz; i++)
	{
		slots.erase( _slots[i]->getAssignedSlot() );
	}
	
	// Dereference the minor free slot number
	unsigned int firstFreeSlot = (* slots.begin());
	
	// Verify if it is the first time this slot has been emitted
	if (firstFreeSlot > _maxEmittedSlot)
	{
		// Init the status manager
		(ProcessorStatusMgrSingleton::instance()->getMgr())->enterStatus (PR_IDLE, firstFreeSlot);
		_maxEmittedSlot = firstFreeSlot;
	}
	
	return firstFreeSlot;
}

const vector<ExecutingOrder *>& 
OrderSlot::getVector() const
{
	ThreadSafe::Lock (*this);
    return _slots;
}

std::vector<ExecutingOrder *>& 
OrderSlot::getVector()
{
	ThreadSafe::Lock (*this);
    return _slots;
}



_ACS_END_NAMESPACE
