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
	Revision 2.3  2015/06/16 10:26:52  damdec
	Compilation warnings solved.
	
	Revision 2.2  2012/06/06 16:53:03  marfav
	Code improved removing compiler warnings
	
	Revision 2.1  2012/02/13 16:54:18  marpas
	removing deprecated macros
	
	Revision 2.0  2006/02/28 10:06:01  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/07/29 13:56:10  marfav
	Removed dependency from database classes. Using tlOrder objects instead of dbOrder ones.
	
	Revision 1.2  2005/04/29 16:56:59  marfav
	Load clears the queue first
	
	Revision 1.1  2005/03/09 09:26:08  marfav
	TlOrderQueue added
	

*/

#include <TlOrderQueue.h>
#include <tlOrder.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(TlOrderQueue)

TlOrderQueue::TlOrderQueue () :
    _orderQueue()
{
}

bool
TlOrderQueue::isEmpty() const
{
	ThreadSafe::Lock lock (*this);
	return ( _orderQueue.empty() );
}


tlOrder
TlOrderQueue::getNext()
{
	ThreadSafe::Lock lock (*this);
	if (isEmpty())
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "The queue is empty... throwing exception") ;

		ACS_THROW (TlOrderQueueException ("Order queue is empty")); // PRQA S 3081
	}
	tlOrder order = _orderQueue.front();
	_orderQueue.pop_front();

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "returning orderId " << order.getOrderId() << ". Queue size is " << _orderQueue.size()) ;

	return order;
}


// ****************************
// Helper STL class
// ****************************
namespace {
	class OrderIdEqual // PRQA S 2109
	{
	public:
		explicit OrderIdEqual(unsigned int orderId) : _orderId (orderId) {}
        ~OrderIdEqual() throw() {}
		bool operator() (const tlOrder& o) const throw()
        {
	        return _orderId == o.getOrderId() ;
        }
        OrderIdEqual(const OrderIdEqual &r) : _orderId(r._orderId) {}
    private:
        OrderIdEqual() ; // not implemented
        OrderIdEqual& operator=(const OrderIdEqual &) ; // not implemented 
	private:
		unsigned int _orderId;
	};

	class OrderInserter // PRQA S 2109
	{
	public:
		explicit OrderInserter (TlOrderQueue& q) : _oq (q) {} // PRQA S 2528
        ~OrderInserter() throw() {}
		void operator() (const tlOrder& o) { _oq.push_back (o); }
        OrderInserter(const OrderInserter &r) : _oq(r._oq) {} // PRQA S 2528
    private:
        OrderInserter() ; // not implemented
        OrderInserter& operator=(const OrderInserter &) ; // not implemented 

	private:
		TlOrderQueue& _oq;
	};

}


bool
TlOrderQueue::purge (unsigned int order)
{
	ThreadSafe::Lock lock (*this);

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Purging the order " << order) ;

	list<tlOrder>::iterator it;
    
	it = find_if (_orderQueue.begin(), _orderQueue.end(), OrderIdEqual(order)); // PRQA S 3081
	bool retval = (it != _orderQueue.end());
	if ( retval )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "order found! Erasing it!") ;
		_orderQueue.erase (it);
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "order not found! Returning FALSE") ;
	}
	return retval;
}

bool
TlOrderQueue::purge (const vector <unsigned int>& tlOrderList)
{
	ThreadSafe::Lock lock (*this);
	bool retval = true;
	for (vector<unsigned int>::const_iterator it = tlOrderList.begin(); it != tlOrderList.end(); it++)
	{
		retval = retval && this->purge (*it);
	}
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "returning " << boolalpha << retval) ;

	return retval;
}

void
TlOrderQueue::load (const std::vector<tlOrder>& v)
{
	ThreadSafe::Lock lock (*this);
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Loading " << v.size() << " orders in the queue") ;

	_orderQueue.clear();

	for_each (v.begin(), v.end(), OrderInserter (*this)); // PRQA S 3081
}

void
TlOrderQueue::push_back (const tlOrder& o)
{
	ThreadSafe::Lock lock (*this);
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "pushing back an order in the queue" ) ;
    
	_orderQueue.push_back (o);
}

size_t 
TlOrderQueue::size() const
{
	ThreadSafe::Lock lock (*this);
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "size of the queue is " << _orderQueue.size()) ;
    
	return _orderQueue.size();

}

void
TlOrderQueue::clear()
{
	ThreadSafe::Lock lock (*this);
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "clearing the queue") ;
    
	_orderQueue.clear();
}

_ACS_END_NAMESPACE
