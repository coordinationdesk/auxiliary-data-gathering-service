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
 *  $Prod: A.C.S. Data Consumer Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.3  2017/01/31 16:11:17  lucio.pulvirenti
 *  S2PDGS-1656: work in progress.
 *
 *  Revision 5.2  2013/10/28 17:33:29  marpas
 *  work in progress
 *
 *  Revision 5.1  2013/07/16 14:52:48  marpas
 *  coding best practices applied
 *  robustness and performances improved
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
 *  Revision 1.1  2012/02/28 17:19:35  micmaz
 *  refactory Data circulator
 *
 *  Revision 1.6  2011/08/26 09:57:29  micmaz
 *  removed executable flag.
 *
 *  Revision 1.5  2011/03/15 17:26:32  micmaz
 *  implemented TERM
 *
 *  Revision 1.4  2011/03/04 15:19:14  micmaz
 *  work in progress...
 *
 *  Revision 1.3  2011/03/02 10:44:52  marpas
 *  work in progress
 *
 *  Revision 1.2  2011/03/01 17:30:45  micmaz
 *  removed boost::thread for  added acs::Thread
 *
 *  Revision 1.1  2011/01/21 18:05:48  micmaz
 *  producer/consumer rework.
 *
 *
 *
 *
 */
 
#ifndef SyncPriorityConsumer_H_
#define SyncPriorityConsumer_H_

#include<ConsumingRegister.h>
#include<SyncConsumer.h>
#include<SyncPriorityContainer.h>
#include<Thread.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, pc)
/**
 *This class is the specialization of the generic SyncConsumer used for  the DstaConsumerItem class.
 *
 */
template<typename Item>
class SyncPriorityConsumer: // PRQA S 2109, 2153
    public acs::pc::SyncConsumer<Item, SyncPriorityContainer<Item> >, 
    public Thread {
public:
    SyncPriorityConsumer(ConsumingRegister<Item, SyncPriorityContainer<Item> >& channel, 
                         StopController& stopper) :
        
        SyncConsumer<Item, SyncPriorityContainer<Item> >(channel, stopper),
        Thread() 
    {}

    virtual ~SyncPriorityConsumer() throw() {
        //empty.
    }

    virtual void run() {
        SyncConsumer<Item, SyncPriorityContainer<Item> >::operator()();
    }

protected:
    /**
     * Consumes a Item (transfer the file).
     */
    virtual void consume(Item& item) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Consume: " << item.id());
        if (item.lastErrorCode() == conf::NOT_STARTED) { // PRQA S 1021
            item.consume();
        }
    }

    /**
     * Starts consuming (simply print out a debug message).
     */
    virtual void startConsuming(Item& item) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start Consuming " << item.id());
    }

    /**
     * End of consuming (simply print out a debug message).
     */
    virtual void consumed(Item& item) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Successfully transferred file:  " << item.id() << std::setfill(' ') << std::setw(100));
    }

    /**
     * End of consuming (simply print out a debug message).
     */
    virtual void consumed(Item& item, std::exception const& ex) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Error during transferring file: " << item.id() << '\n' << ">>>>> " << ex.what());
    }

    /**
     * The Consumer has been started (simply print out a debug message).
     */
    virtual void started() {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Started Consumer");
    }

    /**
     * The Consumer has finished its job (simply print out a debug message).
     */
    virtual void done() {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Done Consumer");
    }
private:
    SyncPriorityConsumer() ; // not implemented
    SyncPriorityConsumer(const SyncPriorityConsumer &) ; // not implemented
    SyncPriorityConsumer& operator= (const SyncPriorityConsumer &) ; // not implemented
private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(SyncPriorityConsumer) ;
};


ACS_TEMPLATE_DEFINE_DEBUG_LEVEL (SyncPriorityConsumer, Item) ;

_ACS_END_NESTED_NAMESPACE


#endif /* SyncPriorityConsumer_H_ */
