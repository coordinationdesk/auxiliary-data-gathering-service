// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
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
	
	Revision 2.5  2015/12/01 17:44:45  marpas
	coding best practice application in progress
	
	Revision 2.4  2015/12/01 15:49:53  marfav
	Fixing metric types
	
	Revision 2.3  2015/11/27 17:38:06  marpas
	coding best practice applicationin progress
	
	Revision 2.2  2015/11/19 17:23:03  marpas
	coding best practices application in progress
	
	Revision 2.1  2012/02/09 17:26:24  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.3  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.2  2004/05/17 08:21:18  marfav
	thread safety extended to size method
	
	Revision 1.1  2004/05/12 08:27:02  marfav
	Importing OrderSlot class for Slot Number Management
	

*/



#ifndef _ORDERSLOT_H_
#define _ORDERSLOT_H_

#include <ThreadSafe.h>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)

class ExecutingOrder ;

class OrderSlot : public acs::ThreadSafe // PRQA S 2109, 2153
{
public:
	OrderSlot();
	virtual ~OrderSlot() throw() { } // PRQA S 2131

	unsigned int getSlotNumber();

    // This class was designed to derive from std::vector
    // To help the old code to work without using the vector as a superclass
    // a direct access to the internal vector is provided

    const std::vector<ExecutingOrder *> & getVector() const;
    std::vector<ExecutingOrder *> & getVector();

private:
    OrderSlot(const OrderSlot &) ; // not implemented
    OrderSlot& operator= (const OrderSlot &) ; // not implemented

private:
    std::vector <ExecutingOrder *> _slots;
    unsigned _maxEmittedSlot;

};

_ACS_END_NAMESPACE

#endif //_ORDERSLOT_H_

