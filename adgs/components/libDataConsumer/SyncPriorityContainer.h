// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2017, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
 *
 *  $Prod: A.C.S.libDataConsumer Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.4  2017/01/31 16:11:17  lucio.pulvirenti
 *  S2PDGS-1656: work in progress.
 *
 *  Revision 5.3  2014/06/05 18:47:34  marpas
 *  getting rid of asserts
 *
 *  Revision 5.2  2013/07/22 13:18:15  marpas
 *  types and best practices fixed
 *
 *  Revision 5.1  2013/07/15 16:48:07  marpas
 *  qa warnings fixed
 *  robusteness improved
 *
 *  Revision 5.0  2013/06/06 20:55:21  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.1  2012/02/28 17:19:35  micmaz
 *  refactory Data circulator
 *
 *  Revision 1.5  2012/02/07 16:54:12  micmaz
 *  removed warnings.
 *
 *  Revision 1.4  2011/08/26 09:57:29  micmaz
 *  removed executable flag.
 *
 *  Revision 1.3  2011/04/15 15:33:21  micmaz
 *  work in progress...
 *
 *  Revision 1.2  2011/03/15 17:26:32  micmaz
 *  implemented TERM
 *
 *  Revision 1.1  2011/01/21 18:05:48  micmaz
 *  producer/consumer rework.
 *
 *
 *
 *
 */

#ifndef SyncPriorityContainer_H_
#define SyncPriorityContainer_H_

#include <SyncContainer.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, pc)
/**
 * The SyncPriorityContainer class is the specialization of the generic SyncContainer for the Item class.
 * This is a syncrhonized container (std::vector) of Item.
 */
template<typename Item>
class SyncPriorityContainer: public SyncContainer<Item> { // PRQA S 2153
 
public:

    explicit SyncPriorityContainer(int limit = -1) :
        SyncContainer<Item>(limit),
		_itemsInError() 
    {}

    /**
     *@brief  Copy constructor of the queue.
     */
    explicit SyncPriorityContainer(std::vector<Item> const& queue, int limit = -1) :
        SyncContainer<Item>(queue, limit),
		_itemsInError() 
    {}

    /**
     *@brief  Copy constructor of the queue.
     */
    SyncPriorityContainer(SyncPriorityContainer const& r) : SyncContainer<Item>(r), _itemsInError(r._itemsInError) {}

    SyncPriorityContainer& operator=(SyncPriorityContainer const& r) {
        if (this != &r) {
            SyncContainer<Item>::operator =(r);
			_itemsInError = r._itemsInError;
        }
        return *this;
    }
    virtual ~SyncPriorityContainer() throw() {} // PRQA S 2131
    
    Item popMin() {
        acs::Lock lk(*this);
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Size " << SyncContainer<Item>::size());
        ACS_COND_THROW(this->empty(), exIllegalValueException("Empty sync queue.")) ;
        int minPriority = std::numeric_limits<int>::max();
        unsigned int minCounter = 0;
        for (unsigned int counter = 0; counter < this->size(); ++counter) {
            Item item = SyncContainer<Item>::_container[counter];
			try {
            	if (minPriority > item.getPriority()) {
                	minCounter = counter;
            	}
			}
			catch( std::exception &e )
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, e.what() << ". Error computing priority on element #" << counter << " going to put into error items vector (current size is " << _itemsInError.size() << ')' );
				_itemsInError.push_back(this->erase(counter));
				throw;
			}
        }
        return this->erase(minCounter);
    }
		
	size_t itemsInErrorSize() const
	{
		acs::Lock lk(*this);
		return _itemsInError.size();
	}

	Item popItemInError() {
		acs::Lock lk(*this);
		ACS_COND_THROW(_itemsInError.empty(), exIllegalValueException("No item in error")) ; // PRQA S 3081
		Item item = _itemsInError.back();
		_itemsInError.pop_back();
		return item;
	}


protected:
	std::vector<Item> _itemsInError; // PRQA S 2101 2
private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(SyncPriorityContainer) ;


};

ACS_TEMPLATE_DEFINE_DEBUG_LEVEL (SyncPriorityContainer, Item) ;

_ACS_END_NESTED_NAMESPACE

#endif /* SyncPriorityContainer_H_ */
