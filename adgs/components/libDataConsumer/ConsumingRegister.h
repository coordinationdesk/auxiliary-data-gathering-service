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
 *  Revision 5.10  2017/10/19 16:21:27  marpas
 *  qa warnings fixed
 *  coding best practice implemented
 *
 *  Revision 5.9  2017/03/03 09:40:21  marfav
 *  S3MPM-874
 *  Aligned to libException Rev_5_93 Condition Interface
 *
 *  Revision 5.8  2017/02/06 09:33:18  lucio.pulvirenti
 *  S2PDGS-1656: acquire: release method invoked on item if exception is caught, then exception is re-thrown.
 *
 *  Revision 5.7  2017/01/31 16:11:17  lucio.pulvirenti
 *  S2PDGS-1656: work in progress.
 *
 *  Revision 5.6  2015/01/26 13:54:21  lucio.pulvirenti
 *  S2PDGS-1056: acquire: if immediate stop requested, exStopChannelException thrown instead of exChannelException.
 *
 *  Revision 5.5  2014/06/25 17:29:46  marpas
 *  Logger.C and Logger.h removed
 *
 *  Revision 5.4  2014/06/18 13:45:53  lucio.pulvirenti
 *  S2PDGS-726: exStopChannelException thrown.
 *
 *  Revision 5.3  2013/10/28 17:33:29  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/07/18 12:29:08  marpas
 *  coding best practices applied
 *  type mismatching fix in progress
 *
 *  Revision 5.1  2013/07/16 14:52:47  marpas
 *  coding best practices applied
 *  robustness and performances improved
 *
 *  Revision 5.0  2013/06/06 20:55:20  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.2  2013/04/05 09:47:16  marpas
 *  libDCUtility interface change
 *  flow control changed
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.1  2012/06/18 12:35:54  micmaz
 *  moved ConsumingRegister to DataConsumer library
 *
 *  Revision 1.2  2012/03/30 12:50:37  micmaz
 *  work in progress
 *
 *  Revision 1.1  2012/02/28 17:16:16  micmaz
 *  refactory Data circulator
 *
 *  Revision 1.12  2012/02/07 16:54:12  micmaz
 *  removed warnings.
 *
 *  Revision 1.11  2011/06/08 15:29:20  micmaz
 *  build break: added limits as included file.
 *
 *  Revision 1.10  2011/03/22 18:01:56  micmaz
 *  resolved the unterminated packages bug
 *
 *  Revision 1.9  2011/03/01 17:30:45  micmaz
 *  removed boost::thread for  added acs::Thread
 *
 *  Revision 1.8  2011/02/01 18:13:40  micmaz
 *  integrated the acs::ThreadSafe class.
 *
 *  Revision 1.7  2010/12/03 14:21:18  micmaz
 *  The Registers now use the Stopper token in order to synch stopping / terminating events.
 *
 *  Revision 1.6  2010/11/30 16:45:27  micmaz
 *  renamed pc_thread in pcUtility.
 *
 *  Revision 1.5  2010/11/22 13:00:32  micmaz
 *  clear the queue when the stop event is catched.
 *
 *  Revision 1.4  2010/11/11 16:14:19  micmaz
 *  corrected the cancel policy, added some comments.
 *
 *  Revision 1.3  2010/11/05 16:39:46  micmaz
 *  The syncContainer must only be a synchronized channel. other responsibilites should be in charge to the Register.
 *
 *  Revision 1.2  2010/11/04 10:14:02  micmaz
 *  the channel is now prioritized.
 *
 *  Revision 1.1  2010/11/03 16:58:24  micmaz
 *  library completely reworked in order to simply its use.
 *
 *
 *
 */

#ifndef _ConsumingRegister_H_
#define _ConsumingRegister_H_

#include <acs_c++config.hpp>
#include <StopController.h>
#include <Filterables.h>
#include <exException.h>
#include <limits>


_ACS_BEGIN_NESTED_NAMESPACE(acs, pc)

    exDECLARE_EXCEPTION(exChannelException,exException); // PRQA S 2131, 2502
    exDECLARE_EXCEPTION(exStopChannelException,exException); // PRQA S 2131, 2502

/**
 * @brief the consuming register is responsible for the consumer actions.
 *
 *This class is a convenient register for a generic Consumer.
 * It handles two synchronized containers: <UL>
 * <LI> items to consume;
 * <LI> items have been consuming;
 *
 * Using the two channels we can know, at every time, which items have been consuming and which
 * items are still waiting to be consumed.
 * The register can be:<UL>
 * <LI>stopped: every item still not consumed will be ignored and no more items will be consumed;
 * <LI>interrupted: every item still not consumed will be consumed but no new items will be accepted.
 *
 * This class is copiable and copy-constructible.
 *
 *
 */
template<typename Tp, typename SyncContainer>
class ConsumingRegister { // PRQA S 2109

public:

    /**
     *@brief  Constructor of the channel: it sets the queue limit to <code> limit </code>.
     */
    explicit ConsumingRegister(StopController& stopper, int limit = -1) :
        _maxSize((limit <= 0) ? std::numeric_limits<short>::max() : limit), // PRQA S 3000, 3380, 3385
        _consumeCondition(),
        _toConsume(),
        _consuming(),
        _stopper(stopper) // PRQA S 2528
    {
        //empty.
    }
public:
    /**
     * Wake up all pending threads and terminate the register activity.
     */
    void wakeUpAllAndTerminate() {
        wakeUpAll();
    }
    /**
     * Wake up all pending threads and stop immediately the register activity.
     */
    void wakeUpAllAndStop() {
        wakeUpAll();
        Lock lk(_consumeCondition);
        _toConsume.clear();
    }

    /**
     * @brief this method is called from the consumers thread (makes a pop()).
     *
     * It ensures the synchronization on the queue.
     *
     */
    Tp acquire() {
        if (_stopper.haltImmediate()) {
            ACS_THROW(exStopChannelException("Immediate stop requested: consumer to end")); // PRQA S 3081
        }
        // The move from _toConsume in _consuming is made by findNextToAcquire that keeps the
        // lock on the condition all the time to avoid race condition when moving the elements
		Tp item = findNextToAcquire(); // in case of errors exception is thrown from here
		try {
			itemAcquired(_consuming, item);
			return item;
		}
		catch(std::exception &e)
		{
			try {
				release(item);
			}
			catch(std::exception &ee)
			{
				ACS_LOG_WARNING("Cannot even release item... misalignement could occur" << ee.what());
			}
			throw;
		}

    }

    /**
     * Release the item (remove it from the consuming queue).
     */
    void release(Tp const& item) {
        Lock lk(_consumeCondition);
        for (unsigned int counter = 0; counter < _consuming.size(); ++counter) {
            if (_consuming[counter] == item) {
                _consuming.erase(counter);
                break;
            }
        }
        itemReleased(_consuming, item);
    }

    /**
     * @brief this method checks if an item is also in the management list or not
     *
     */
	bool isManagingItem (Tp const& item)
	{
		Lock lk(_consumeCondition);
		// _toConsume and _consuming are based on std::vector and hasItem searches into the vectors
		// the operator == of the items is based on the id() function, that is a signature of the item
		// including the record id plus other information
		bool retval = (_toConsume.hasItem(item) || _consuming.hasItem(item));
		return  retval;
	}

    /**
     * @brief this method is called from the producers thread (makes a push()).
     *
     * It ensures the synchronization on the queue.
     *
     */
	bool push(Tp const &item, bool force = false) {
		Lock lk(_consumeCondition);
		if (!force && _stopper.requestedCancel()) {
			ACS_THROW(exStopChannelException("Consumer stopped or terminated: cannot accept new items to consume.")); // PRQA S 3081 
		}
		// This should be useless now, records are filtered during the query
		// nevertheless it has been decided to leave it to have at least a sign in the log 
		// in case of orders took in charge more than once
		if ( isManagingItem (item) )
		{
			ACS_LOG_PRIVINFO ("Skipping Item " << item.id() << " because is already in the active circulation jobs queue");
		}
		else
		{
			_toConsume.push(item);
			wakeUpOnce();
		}
		return true;
	}
    /**
     * @brief this method is called in order to empty the consuming items when the whole process has been finished.
     *
     *
     */
    Tp pop(bool force = false) {
        Lock lk(_consumeCondition);
        if (!force && _stopper.requestedCancel()) {
            ACS_THROW(exStopChannelException("Consumer stopped or terminated: cannot accept new items to consume.")); // PRQA S 3081
        }
        return _toConsume.pop(force);
    }

    /**
     * @brief Gets the channel's size (only items to be consumed).
     */
    virtual size_t size() const { // PRQA S 2131
        Lock lk(_consumeCondition);
        return _toConsume.size();
    }

     /**
     * @brief Gets the channel's size (only items to be consumed).
     */
    virtual bool empty() const {
        Lock lk(_consumeCondition);
        return _toConsume.empty();
    }
   /**
     * Empty dtor.
     */
    virtual ~ConsumingRegister() throw() {}
	
	
protected:

    /**
     * Takes the item without removing it from the consume queue.
     * This method is intended only for internal use.
     */
    Tp& operator[](size_t counter) const { // PRQA S 2011, 2141
        Lock lk(_consumeCondition);
        return _toConsume[counter];
    }

    /**
     * Wake up only once thread.
     */
    void wakeUpOnce() { // PRQA S 4211
        _consumeCondition.notify_one();
    }

    /**
     * Wake up all threads.
     */
    void wakeUpAll() { // PRQA S 4211
        _consumeCondition.notify_all();
    }

    /**
     * Takes the next available product (in this case the first element in the queue,i.e. pop_front()).
     */
    virtual Tp findNextToAcquire() {
        Lock lk(_consumeCondition);
        while (_toConsume.empty() && !_stopper.requestedCancel()) {
            _consumeCondition.wait();
        }
        Tp item = _toConsume.pop();
        _consuming.push(item);
        return item;
    }
    /**
     * Call back invoked when an item has been acquired.
     */
    virtual void itemAcquired(SyncContainer& _consuming, Tp item) {} // PRQA S 2131
    /**
     * Call back invoked when an item has been released.
     */
    virtual void itemReleased(SyncContainer& _consuming, Tp item) {} // PRQA S 2131
private:
    ConsumingRegister(); // not implemented
    ConsumingRegister(ConsumingRegister const& ); // not implemented
    ConsumingRegister& operator=(ConsumingRegister const& ); // not implemented

protected:
    unsigned int _maxSize; // PRQA S 2101 7
    Condition _consumeCondition;
    SyncContainer _toConsume;
    SyncContainer _consuming;
    StopController & _stopper;
};

_ACS_END_NESTED_NAMESPACE

#endif /* _ConsumingRegister_H_ */
