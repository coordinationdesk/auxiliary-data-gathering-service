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

	$Prod: A.C.S. ThinLayer Order Library $

	$Id$

	$Author$

	$Log$
	Revision 2.4  2015/06/16 10:26:52  damdec
	Compilation warnings solved.
	
	Revision 2.3  2014/11/06 12:07:09  nicvac
	Fixed destructor
	
	Revision 2.2  2013/01/25 12:02:59  marpas
	using pattern::Singleton from libException
	
	Revision 2.1  2012/06/06 16:53:03  marfav
	Code improved removing compiler warnings
	
	Revision 2.0  2006/02/28 10:06:01  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/07/29 13:56:10  marfav
	Removed dependency from database classes. Using tlOrder objects instead of dbOrder ones.
	
	Revision 1.1  2005/03/09 09:26:08  marfav
	TlOrderQueue added
	

*/

#ifndef _TlOrderQueue_H_
#define _TlOrderQueue_H_

#include <exStream.h>
#include <exException.h>
#include <tlOrder.h>
#include <ThreadSafe.h>
#include <Singleton.hpp>
#include <tlOrder.h>

#include <list>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)


//class TlOrderQueue : private std::list<tlOrder>, public acs::ThreadSafe
class TlOrderQueue : public acs::ThreadSafe // PRQA S 2109, 2153
{

public:
	/*! class TlOrderQueueException declaration */
	exDECLARE_EXCEPTION(TlOrderQueueException,acs::exException) ; // PRQA S 2131, 2502

public:

	// Returns false if there is at least one element in the queue
	bool isEmpty() const;
    
    // Returns the size of the queue
    size_t size() const;

	// Returns the next tlOrder in the front of the queue and deletes it
	// throws an exception if there are no orders in the queue
	tlOrder getNext();

	// Deletes entries from the queue... returns false if one or more elements does not exists
	bool purge (unsigned int order);
	bool purge (const std::vector <unsigned int>&);

	// Inserts elements in the back of the queue
	void load (const std::vector <tlOrder>&);
    
    // Clears the queue
	void clear ();

    // Push back an element in the queue
    void push_back (const tlOrder&);

public:
	virtual ~TlOrderQueue() throw() {} // PRQA S 2131


private:

	friend TlOrderQueue* acs::pattern::Singleton<TlOrderQueue>::instance(); // PRQA S 2107 
    
	// CTOR is private and is accessible only by the singleton class
	TlOrderQueue();

	// Declared but not implemented due to singleton approach
	TlOrderQueue (const TlOrderQueue&);
	TlOrderQueue& operator= (const TlOrderQueue&);

private:

    std::list <tlOrder> _orderQueue;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(TlOrderQueue)

};

_ACS_END_NAMESPACE


#endif //_TlOrderQueue_H_

