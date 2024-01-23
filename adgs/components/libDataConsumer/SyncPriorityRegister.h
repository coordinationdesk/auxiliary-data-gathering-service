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
 *  Revision 5.15  2017/03/22 08:09:08  clanas
 *  added this to virtual method call
 *
 *  Revision 5.14  2017/03/03 09:40:21  marfav
 *  S3MPM-874
 *  Aligned to libException Rev_5_93 Condition Interface
 *
 *  Revision 5.13  2017/02/06 09:28:32  lucio.pulvirenti
 *  S2PDGS-1656: _notAbleToConsumeItems attribute added to manage items set in error by _toConsume attribute after invoking popMin method.
 *  sizeOfNotAbleToConsume, popOfNotAbleToConsume, enqueueOfNotAbleToConsume methods added to manage _notAbleToConsumeItems attribute.
 *  itemReleased: new logic is to set item lastErrorCode to NOT_STARTED when starting and to SUCCESS_TRANSFER when transferring is successfull.
 *
 *  Revision 5.12  2017/01/31 16:11:17  lucio.pulvirenti
 *  S2PDGS-1656: work in progress.
 *
 *  Revision 5.11  2017/01/11 10:38:39  clanas
 *  added missed this to template method call
 *
 *  Revision 5.10  2015/01/26 13:56:15  lucio.pulvirenti
 *  S2PDGS-1056: findNextToAcquire: if stop requested, exStopChannelException thrown.
 *
 *  Revision 5.9  2015/01/14 11:18:47  marfav
 *  S2PDGS-1039: robustness improved against failure in getting item signatures and channel management
 *
 *  Revision 5.8  2014/06/24 10:58:34  marpas
 *  useless macros eviction in progress
 *
 *  Revision 5.7  2014/06/05 18:47:34  marpas
 *  getting rid of asserts
 *
 *  Revision 5.6  2014/02/14 13:54:46  marpas
 *  attempt to refactor band shaping mechanism
 *
 *  Revision 5.5  2013/10/28 17:33:29  marpas
 *  work in progress
 *
 *  Revision 5.4  2013/07/18 12:29:08  marpas
 *  coding best practices applied
 *  type mismatching fix in progress
 *
 *  Revision 5.3  2013/07/16 14:52:48  marpas
 *  coding best practices applied
 *  robustness and performances improved
 *
 *  Revision 5.2  2013/07/15 16:48:07  marpas
 *  qa warnings fixed
 *  robusteness improved
 *
 *  Revision 5.1  2013/07/05 11:11:30  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 5.0  2013/06/06 20:55:21  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.5  2013/04/05 09:47:16  marpas
 *  libDCUtility interface change
 *  flow control changed
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.4  2012/09/19 15:57:52  micmaz
 *  ACCEPTED - issue S2GPP-611: Development of Distribution Formatter framework for Export
 *  http://jira.acsys.it/browse/S2GPP-611
 *
 *  Revision 1.3  2012/03/30 12:50:27  micmaz
 *  work in progress
 *
 *  Revision 1.2  2012/03/27 10:53:29  micmaz
 *  work in progress
 *
 *  Revision 1.1  2012/02/28 17:19:35  micmaz
 *  refactory Data circulator
 *
 *  Revision 1.23  2012/02/08 16:13:17  micmaz
 *  slow down calculation wrong.
 *
 *  Revision 1.22  2012/02/07 16:54:12  micmaz
 *  removed warnings.
 *
 *  Revision 1.21  2012/02/02 17:19:58  micmaz
 *  IN PROGRESS - issue S1PDGS-2769: The Circulation Curl does not take into account the slow parameter
 *  http://jira.acsys.it/browse/S1PDGS-2769
 *
 *  Revision 1.20  2011/09/13 13:00:55  micmaz
 *  IN PROGRESS - issue S1PDGS-1781: Problems with loading of the Configuration Parameters
 *  http://jira.acsys.it/browse/S1PDGS-1781
 *
 *  Revision 1.19  2011/09/06 16:35:42  micmaz
 *  changed constructor method
 *
 *  Revision 1.18  2011/09/06 16:29:08  micmaz
 *  changed constructor method
 *
 *  Revision 1.17  2011/08/26 09:57:29  micmaz
 *  removed executable flag.
 *
 *  Revision 1.16  2011/07/12 11:29:41  micmaz
 *  RESOLVED - issue S1PDGS-1656: CriculationAgent cpu and memory occupation problem
 *  http://jira.acsys.it/browse/S1PDGS-1656
 *
 *  Revision 1.15  2011/04/26 17:31:56  micmaz
 *  if all the items have the same priority, then we must not slow down.
 *
 *  Revision 1.14  2011/04/22 16:47:41  micmaz
 *  work in progress...
 *
 *  Revision 1.13  2011/04/15 18:01:02  micmaz
 *  work in progress...
 *
 *  Revision 1.12  2011/04/15 15:33:21  micmaz
 *  work in progress...
 *
 *  Revision 1.11  2011/03/22 18:01:56  micmaz
 *  resolved the unterminated packages bug
 *
 *  Revision 1.10  2011/03/08 15:03:10  micmaz
 *  added logs.
 *
 *  Revision 1.9  2011/03/08 12:52:04  micmaz
 *  work in progress...
 *
 *  Revision 1.8  2011/03/03 14:28:17  micmaz
 *  work in progres.
 *
 *  Revision 1.7  2011/03/02 18:37:44  micmaz
 *  work in progress...
 *
 *  Revision 1.6  2011/03/01 17:30:45  micmaz
 *  removed boost::thread for  added acs::Thread
 *
 *  Revision 1.5  2011/02/28 11:22:20  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.4  2011/02/24 15:53:25  micmaz
 *  added logs.
 *
 *  Revision 1.3  2011/02/24 15:28:32  micmaz
 *  added logs.
 *
 *  Revision 1.2  2011/02/01 18:13:40  micmaz
 *  integrated the acs::ThreadSafe class.
 *
 *  Revision 1.1  2011/01/21 18:05:48  micmaz
 *  producer/consumer rework.
 *
 *
 *
 *
 */

#ifndef SyncPriorityRegister_H_
#define SyncPriorityRegister_H_

#include<ConsumingRegister.h>
#include<SyncPriorityContainer.h>
#include<DataConsumerConfigurer.h>

#include <Thread.h>
#include <Timer.h>
#include <exException.h>
#include <boost/shared_ptr.hpp> // PRQA S 1013

#include <algorithm>
#include <cmath>

_ACS_BEGIN_NESTED_NAMESPACE(acs, pc)
/**
 * @brief the SyncPriorityRegister specializes the generic consumer register.
 * This class is responsible to register all the Items that must be consumed and the list
 * of all items consumed with/without success.
 */
template<typename Item>
class SyncPriorityRegister: // PRQA S 2109, 2153
        public ConsumingRegister<Item,
		SyncPriorityContainer<Item> > 
{

public:

    const std::string &name() const throw() { return _name ; }

	/**
	 * The default ctor.
	 * The limit parameter is used to limit the channel capacity (up to now still not used).
	 */
	explicit SyncPriorityRegister(StopController& stopper,
			                     std::string const& name, 
                                 int limit = -1) :
			ConsumingRegister<Item, SyncPriorityContainer<Item> >(stopper, limit), 
            _finishedItems(), 
            _erroredItems(), 
			_notAbleToConsumeItems(),
            _lastTimeRetried(0),
            _name(name)
    {}
	/**
	 * The copy ctor.
	 */
	SyncPriorityRegister(SyncPriorityRegister const& r) :
			ConsumingRegister<Item, SyncPriorityContainer<Item> >(r), 
            _finishedItems(r._finishedItems), 
            _erroredItems(r._erroredItems), 
			_notAbleToConsumeItems(r._notAbleToConsumeItems),
            _lastTimeRetried(r._lastTimeRetried),
           _name(r._name)
     {}
    
	/**
	 * The copy assigner.
	 */
	SyncPriorityRegister& operator=(SyncPriorityRegister const& r) {
		if (this != &r) {
			ConsumingRegister<Item, SyncPriorityContainer<Item> >::operator=(r);
			_name = r._name;
			_finishedItems = r._finishedItems;
			_erroredItems = r._erroredItems;
			_notAbleToConsumeItems = r._notAbleToConsumeItems;
			_lastTimeRetried = r._lastTimeRetried;
		}
		return *this;
	}

	/**
	 * Empty dtor.
	 */
	virtual ~SyncPriorityRegister() throw() {
		//empty
	}

	/**
	 * Returns the number of finished items.
	 */
	size_t sizeOfFinished() const {
		Lock lock(this->_consumeCondition); // PRQA S 3050
		return _finishedItems.size();
	}

	/**
	 * Returns the number of items still in consuming state.
	 */
	size_t sizeOfConsuming() const {
		Lock lock(this->_consumeCondition); // PRQA S 3050
		return this->_consuming.size();
	}

	void getConsumingItems(std::vector<Item* >& items) {
		Lock lock(this->_consumeCondition); // PRQA S 3050
		for (size_t counter = 0; counter < this->_consuming.size(); ++counter) {
			Item* item = &this->_consuming[counter];
			items.push_back(item);
		}
	}

	/**
	 * Retry (and pop from the internal container) a finished item.
	 */
	Item popOfFinished() {
		Lock lock(this->_consumeCondition); // PRQA S 3050
		ACS_COND_THROW(_finishedItems.empty(), exIllegalValueException("Channel empty")) ; // PRQA S 3081
		return _finishedItems.pop();
	}

	/**
	 * This registry size: it calculated as the size of the items to consume plus the items errored.
	 */
	virtual size_t size() const { // PRQA S 2131
		Lock lock(this->_consumeCondition); // PRQA S 3050
		//check if the consumer is finished but we still have some errored items:
		//in this case (it is possible because the errored items are produced asynchronously
		//we have to clear() errored before calculate size.
		emptyErrored();
		size_t size = ConsumingRegister<Item, SyncPriorityContainer<Item> >::size();
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, '[' << name() << ']' << "(Size, Errored, Status) = (" << size << ", " << _erroredItems.size() << ", " << this->_stopper <<")");
		return (size + _erroredItems.size()); // PRQA S 3000, 3084
	}

	/**
	 * Starts the consumer.
	 */
	void operator()() {
		while (!this->_stopper.requestedCancel()) {
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Running channel....");
			retryErrored();
			this->_stopper.timeoutOrCancel(100); // 100 ms
		}
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Exiting from main running channel: retry errored items....");
		emptyErrored();
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Exit from main running channel.");
	}

	/**
	 * Retry the errored items: the errored items are stored in an internal container.
	 * The internal container is consumed by this thread.
	 * The thread will dead when the register will stopped or terminated.
	 */
	void retryErrored() {
		time_t now = time(0);
		if ((now - _lastTimeRetried > time_t(getElapsedTimeToRetry())) // PRQA S 3081
				&& !this->_stopper.requestedCancel()) {
			Lock lock(this->_consumeCondition); // PRQA S 3050
			size_t erroredSize = _erroredItems.size();
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, '[' << name() << ']' << "Time to retry errored ");
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, '[' << name() << ']' << "errored size " << erroredSize);
			while (erroredSize-- && !this->_stopper.requestedCancel()) {
				Item item = _erroredItems.pop();
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '[' << name() << ']' << "must retry next " << (now - item.lastAttemptTime()) << " ms.");
				if (now - item.lastAttemptTime() > time_t(getElapsedTimeToRetry())) { // PRQA S 3081
					item.setLastErrorCode(conf::NOT_STARTED); // PRQA S 1021
					this->push(item, true);
				} else {
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '[' << name() << ']' << "skip errored " << item.str());
					_erroredItems.enqueue(item);
				}
			}
			emptyErrored();
			_lastTimeRetried = now;
		}
	}

	/**
	 * Empty the errored list and add the remaining data items to the failed list.
	 */
	void emptyErrored() const {
		if (this->_stopper.requestedCancel()) {
			while (0 != _erroredItems.size()) {
				Item item = _erroredItems.pop();
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '[' << name() << ']' << "Consumer interrupted/stopped, the following item will not be retried:" << item.str());
				_finishedItems.push(item);
			}
		}
	}

	/**
	 * Returns the number of not consumed items.
	 */
	size_t sizeOfNotAbleToConsume() const {
		Lock lock(this->_consumeCondition); // PRQA S 3050
		return _notAbleToConsumeItems.size();
	}

	/**
	 * Get (and pop from the internal container) a not consumed item.
	 */
	Item popOfNotAbleToConsume() {
		Lock lock(this->_consumeCondition); // PRQA S 3050
		ACS_COND_THROW(_notAbleToConsumeItems.empty(), exIllegalValueException("No not able to consume item in channel")) ; // PRQA S 3081
		return _notAbleToConsumeItems.pop();
	}
	
	void enqueueOfNotAbleToConsume(Item const &item) {
		Lock lock(this->_consumeCondition); // PRQA S 3050
        // push front
		_notAbleToConsumeItems.enqueue(item);
    }

protected:
	/**
	 * Finds the next item to consume and push it in the _consuming structure
	 * The Item is orderable, thus this method will return the item with the best priority.
	 */
	virtual Item findNextToAcquire() {
		Lock lock(this->_consumeCondition); // PRQA S 3050
		while (!this->_toConsume.size() && !this->_stopper.requestedCancel()) {
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, '[' << name() << ']' << "empty.wait(lock);");
			this->_consumeCondition.wait();
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, '[' << name() << ']' << "End empty.wait(lock);");
		}
		if (this->_toConsume.size()) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '[' << name() << ']' << "Pop Min");
			try {
				Item theItem = this->_toConsume.popMin();
				this->_consuming.push(theItem);
				return theItem;
			}
			catch( std::exception &e ) {
			
				try {
				
					// retrieves all finished items from the consumer channel.
    				size_t sizeOfItemsInError = this->_toConsume.itemsInErrorSize();
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '[' << name() << ']' << "Exception caught, got " << sizeOfItemsInError << " items in error");
    				for (size_t counter = 0; counter < sizeOfItemsInError; ++counter) {
        				try {
            				// removes from the channel the finished items.
            				Item item = this->_toConsume.popItemInError();
							this->release(item);
        				} catch (std::exception const& ex) {
            				//ignore any error.
            				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ex);
        				}
    				}

				}
				catch( std::exception &ee ) {
					ACS_LOG_WARNING("Cannot even get items in error to release them, misalignment could occur..." << ee.what() );
				}
				throw;
			}
		}
		// if here this->_stopper.requestedCancel() must be true
		ACS_THROW(exStopChannelException("Cannot find next item to acquire: stop consumer.")) ;
	}

	virtual void logItemAcquired(SyncPriorityContainer<Item>& consuming,Item &item) {} // PRQA S 2131

	/**
	 * Call back of the acquire() method.
	 */
	virtual void itemAcquired(SyncPriorityContainer<Item>& consuming, Item item) // PRQA S 2131
    {
		ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
		logItemAcquired(consuming, item);
    	dumpItems(consuming) ;
	}

	virtual void log(int errorCode, Item& item) { } // PRQA S 2502

	virtual void logDetails(Item& item) { } // PRQA S 2131

	virtual void logItemReleasedWithError(SyncPriorityContainer<Item>& _consuming, Item item) 
    {
		log(item.lastErrorCode(), item);
	}
	virtual void logItemReleasedWithSuccess(SyncPriorityContainer<Item>& _consuming, Item &item) 
    {
		log(item.lastErrorCode(), item);
	}

	virtual bool canRetry(SyncPriorityContainer<Item>& _consuming, Item item) {
		return false;
	}

	/**
	 * Call back of the release() method.
	 */
	virtual void itemReleased(SyncPriorityContainer<Item>& consuming, Item item) 
    {
		try
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '[' << name() << ']' << "Item Released " << item.id() << " (" << item.lastErrorCode() << ") ");
		}
		catch (std::exception& ex)
		{
			ACS_LOG_NOTIFY_EX (ex);
			// do not care about this exception it is just for debug purposes
		}

		if (item.lastErrorCode() == conf::SUCCESS_TRANSFER) { // PRQA S 1021
			logItemReleasedWithSuccess(consuming, item);
			_finishedItems.push(item);
		}
		else if( item.lastErrorCode() == conf::NOT_STARTED) { 
			_notAbleToConsumeItems.push(item);
		}
		else { // PRQA S 1021
			logItemReleasedWithError(consuming, item);
			if (canRetry(consuming, item)) {
				_erroredItems.push(item);
			}
		} 
		dumpItems(consuming) ;
	}

	virtual size_t getElapsedTimeToRetry() const {
		return DefaultRetryTime_ms;
	}

private:
    SyncPriorityRegister() ; // not implemented
    void dumpItems(SyncPriorityContainer<Item>& consuming)
	{
		try
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB);
            	size_t consuming_size = consuming.size() ;
		    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '[' << name() << ']' << "******* List of all transferring files: " << consuming_size);
		    	for (size_t counter = 0; counter < consuming_size; ++counter) {
			    	Item item = consuming[counter];
			    	logDetails(item);
		    	}
		    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '[' << name() << ']' << "******* End of List of all transferring files ");

            	size_t errors_size = _erroredItems.size() ;
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '[' << name() << ']' << "******* List of all errored files: " << errors_size);
		    	for (size_t counter = 0; counter < errors_size; ++counter) {
			    	Item item = _erroredItems[counter];
			    	logDetails(item);
		    	}
		    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '[' << name() << ']' << "******* End of List of all errored files ");

				size_t notConsumed_size = _notAbleToConsumeItems.size() ;
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '[' << name() << ']' << "******* List of all not able to consume files: " << notConsumed_size);
		    	for (size_t counter = 0; counter < notConsumed_size; ++counter) {
			    	Item item = _notAbleToConsumeItems[counter];
			    	logDetails(item);
		    	}
		    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '[' << name() << ']' << "******* End of List of all not able to consume files ");

			ACS_CLASS_END_DEBUG;
		}
		catch (std::exception& ex)
		{
			// do not care about this is just for debug purposes
			ACS_LOG_NOTIFY_EX (ex);
		}
	}
    
public:
	static const int priorityThreshold = 10;
	static const size_t DefaultRetryTime_ms = 10;

protected:
	mutable SyncPriorityContainer<Item> _finishedItems;
	mutable SyncPriorityContainer<Item> _erroredItems;
	mutable SyncPriorityContainer<Item> _notAbleToConsumeItems;
	time_t _lastTimeRetried;
private:
	std::string _name;  // PRQA S 2101 4
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SyncPriorityRegister) ;

};

ACS_TEMPLATE_DEFINE_DEBUG_LEVEL(SyncPriorityRegister,Item) ;

_ACS_END_NESTED_NAMESPACE

#endif /* SyncPriorityRegister_H_ */
