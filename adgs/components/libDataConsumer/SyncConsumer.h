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
 *  $Prod: A.C.S. Producer SyncConsumer Library $
 *
 *  $Name$
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.5  2017/01/31 16:11:17  lucio.pulvirenti
 *  S2PDGS-1656: work in progress.
 *
 *  Revision 5.4  2015/01/26 13:58:10  lucio.pulvirenti
 *  S2PDGS-1056: debug declaration added. operator(): if exStopChannelException caught, just debug a message and break.
 *
 *  Revision 5.3  2015/01/14 11:18:47  marfav
 *  S2PDGS-1039: robustness improved against failure in getting item signatures and channel management
 *
 *  Revision 5.2  2013/07/16 14:52:48  marpas
 *  coding best practices applied
 *  robustness and performances improved
 *
 *  Revision 5.1  2013/07/15 16:48:07  marpas
 *  qa warnings fixed
 *  robusteness improved
 *
 *  Revision 5.0  2013/06/06 20:55:21  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.3  2013/04/05 09:47:16  marpas
 *  libDCUtility interface change
 *  flow control changed
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.2  2012/03/30 12:50:27  micmaz
 *  work in progress
 *
 *  Revision 1.1  2012/02/28 17:19:36  micmaz
 *  refactory Data circulator
 *
 *  Revision 1.6  2011/03/02 10:44:52  marpas
 *  work in progress
 *
 *  Revision 1.5  2010/12/03 14:21:18  micmaz
 *  The Registers now use the Stopper token in order to synch stopping / terminating events.
 *
 *  Revision 1.4  2010/11/22 13:00:05  micmaz
 *  added the enqueue method in the synch container.
 *
 *  Revision 1.3  2010/11/11 16:14:19  micmaz
 *  corrected the cancel policy, added some comments.
 *
 *  Revision 1.2  2010/11/05 16:39:46  micmaz
 *  The syncContainer must only be a synchronized channel. other responsibilites should be in charge to the Register.
 *
 *  Revision 1.1  2010/11/03 16:58:23  micmaz
 *  library completely reworked in order to simply its use.
 *
 *  Revision 1.4  2010/10/22 15:40:27  micmaz
 *  corrected some bugs.
 *
 *  Revision 1.3  2010/10/18 16:07:43  micmaz
 *  added the stopper class.
 *
 *  Revision 1.2  2010/10/13 15:48:44  micmaz
 *  improved the priority SyncConsumer.
 *
 *  Revision 1.1.1.1  2010/10/01 09:27:15  micmaz
 *  Added producer/SyncConsumer generic algorithm
 *
 *
 */

#ifndef SyncConsumer_H_
#define SyncConsumer_H_

#include <StopController.h>
#include <ConsumingRegister.h>
#include <Thread.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, pc) ;


/**
 * @brief The SyncConsumer actor: it is responsible to consume the whole channel.
 *
 * Every SyncConsumer runs in a separated thread in order to consume the underling channel.
 * <P>
 * The SyncConsumer class implements the operator operator()() in order to be "callable" from a boost's thread.
 * <P>
 * See the boost documentation for more information:
 * <P>
 * \code
 * struct callable
 {
 void operator()();
 };
 boost::thread copies_are_safe()
 {
 callable x;
 return boost::thread(x);
 } // x is destroyed, but the newly-created thread has a copy, so this is OK

 \endcode
 * The SyncConsumer loop can be interrupted  setting manually the StopController to stop.
 *
 */
template<typename Tp, typename SyncContainer>
class SyncConsumer { // PRQA S 2109
public:

    SyncConsumer(ConsumingRegister<Tp, SyncContainer>& channel, StopController& stopper) :
        _channel(channel),  // PRQA S 2528 2
        _stopper(stopper) 
    {}

    /**
     * @brief The virtual destructor (the class can be overwritten).
     *
     */
    virtual ~SyncConsumer() throw() {}

    /**
     * @brief the runnable method of this class.
     *
     * This method taks a product from the channel (and consume it) until
     * externally someone makes stop (or terminate) using the stopper instance;
     *
     */
    void operator()() {
        started();
        while (!cancel()) {
			Tp item;
			bool itemConsumed = false;
            try {
				ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Waiting to acquire item" );
                item = _channel.acquire();
				ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Item acquired") ;
                startConsuming(item);
                try {
                    consume(item);
                    consumed(item);
                } catch (std::exception const& ex) {
                    consumed(item, ex);
                } 
				itemConsumed = true;
                _channel.release(item);
			} catch (exStopChannelException const& ex) {
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Stop requested when acquiring new items to consume before management: " << ex.what())
				break;
           	} catch (std::exception const& ex) {
				ACS_LOG_NOTIFY_EX (ex);
				if( itemConsumed ) {
					ACS_LOG_WARNING ("CANNOT RELEASE ITEM " << item.id() << ", consuming queue cannot be emptied"); 
				}
				else {
					ACS_LOG_WARNING ("Got an exception before consuming item" );
				}
            } 
            Thread::yield(); // need breath...
        }
        done();
    }

    /**
     * Stop the consumer and wake up all other consumers.
     */
    void stop() {
        _stopper.setHaltLevel(StopController::IMMEDIATE);
        _channel.wakeUpAll();
    }

    /**
     * Terminate the consumer and wake up all other consumers.
     */
    void terminate() { // PRQA S 2502
        _stopper.setHaltLevel(StopController::LAZY);
        _channel.wakeUpAll();
    }

protected:
    /**
     * @brief Consumes the current product.
     *
     * ...And must be overriden.
     *
     */
    virtual void consume(Tp& x) {
        // default implementation does nothing
    }

    virtual void startConsuming(Tp& x) {
        // default implementation does nothing
    }
    virtual void consumed(Tp& x) {
        // default implementation does nothing
    }
    virtual void consumed(Tp& x, std::exception const& ex) {
        // default implementation does nothing
    }

    /**
     * @brief The cancel policy: it must be overwritten.
     */
    virtual bool cancel() {
        return _stopper.haltImmediate() || (_stopper.haltLazy() && !_channel.size());
    }
    /**
     * @brief callback for the start event.
     *
     */
    virtual void started() {
        // default implementation does nothing
    }
    /**
     * @brief callback for the done event.
     *
     */
    virtual void done() {
        // default implementation does nothing
    }
private:
    SyncConsumer() ; // not implemented
    SyncConsumer(const SyncConsumer&) ; // not implemented
    SyncConsumer& operator= (const SyncConsumer&) ; // not implemented
protected:

    ConsumingRegister<Tp, SyncContainer>& _channel; // PRQA S 2101 2 
    StopController& _stopper;

private	:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SyncConsumer) ;


};

ACS_TEMPLATE_DEFINE_DEBUG_LEVEL2 (SyncConsumer, Tp, SyncContainer) ; // PRQA S 2101


_ACS_END_NESTED_NAMESPACE

#endif /* SyncConsumer_H_ */
