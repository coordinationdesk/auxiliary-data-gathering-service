// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2013, Advanced Computer Systems , Inc.
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
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.5  2014/06/24 10:58:34  marpas
 *  useless macros eviction in progress
 *
 *  Revision 5.4  2014/02/14 13:54:46  marpas
 *  attempt to refactor band shaping mechanism
 *
 *  Revision 5.3  2013/10/28 17:33:29  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/07/16 14:52:48  marpas
 *  coding best practices applied
 *  robustness and performances improved
 *
 *  Revision 5.1  2013/07/15 16:48:06  marpas
 *  qa warnings fixed
 *  robusteness improved
 *
 *  Revision 5.0  2013/06/06 20:55:21  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.20  2013/04/05 09:47:16  marpas
 *  libDCUtility interface change
 *  flow control changed
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.19  2012/03/30 12:50:27  micmaz
 *  work in progress
 *
 *  Revision 1.18  2011/09/23 09:14:42  micmaz
 *  removed unused headers.
 *
 *  Revision 1.17  2011/09/13 16:31:05  micmaz
 *  IN PROGRESS - issue S1PDGS-1781: Problems with loading of the Configuration Parameters
 *  http://jira.acsys.it/browse/S1PDGS-1781
 *
 *  Revision 1.16  2011/09/06 16:35:57  micmaz
 *  changed constructor method
 *
 *  Revision 1.15  2011/09/06 16:29:21  micmaz
 *  changed constructor method
 *
 *  Revision 1.14  2011/04/27 09:50:21  micmaz
 *  The GenericRegister must retry using its derived Register class' getMaxRetries() and not the constant numeric value read from the configuration file.
 *
 *  Revision 1.13  2011/04/26 14:42:15  micmaz
 *  Updated logs and added removing of sources (if required)
 *
 *  Revision 1.12  2011/04/15 15:33:20  micmaz
 *  work in progress...
 *
 *  Revision 1.11  2011/04/11 12:24:53  micmaz
 *  work in progress
 *
 *  Revision 1.10  2011/03/22 18:02:11  micmaz
 *  resolved the unfinished packages bug.
 *
 *  Revision 1.9  2011/03/10 16:34:09  micmaz
 *  added new paramters for connection and db  time out.
 *  Improved log in case of failure
 *
 *  Revision 1.8  2011/03/01 17:31:11  micmaz
 *  removed boost::thread for  added acs::Thread
 *
 *  Revision 1.7  2011/02/24 16:11:58  micmaz
 *  added logs.
 *
 *  Revision 1.3  2011/01/21 18:06:25  micmaz
 *  data consumer rework.
 *
 *  Revision 1.2  2010/12/03 14:23:12  micmaz
 *  The Registers now use the Stopper token in order to synch stopping / terminating events.
 *
 *  Revision 1.1  2010/11/30 16:41:48  micmaz
 *  added mail warning.
 *
 *
 *
 */

#ifndef GENERICREGISTER_H_
#define GENERICREGISTER_H_

#include<SyncPriorityRegister.h>
#include <DataConsumerConfigurer.h>
#include<Thread.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, dc)


/**
 * @brief the GenericRegister specializes the generic consumer register.
 * This class is responsible to register all the Items that must be consumed and the list
 * of all items consumed with/without success.
 */
template<typename Item>
class GenericRegister: // PRQA S 2109, 2153
    public acs::pc::SyncPriorityRegister<Item>, 
    public acs::Thread {
public:
    /**
     * The default ctor.
     * The limit parameter is used to limit the channel capacity (up to now still not used).
     */
    explicit GenericRegister(StopController& stopper, std::string const& name, int limit = -1) :
        acs::pc::SyncPriorityRegister<Item> (stopper, name, limit),
        Thread()
    {}
    
    /**
     * Runner for acs::Thread threads.
     * Now the Register is compatible with both boost::thread and acs::Thread.
     */
    virtual void run() {
        acs::pc::SyncPriorityRegister<Item> ::operator()();
    }

    virtual ~GenericRegister() throw () {}

protected:

    virtual void logItemAcquired(acs::pc::SyncPriorityContainer<Item>& consuming, Item &item) // PRQA S 2131
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,item.groupId() <<  
                "***     Consuming: " << item.id() << " ( p:" << item.getPriority() << ") ") ;
    }

    virtual void logItemReleasedWithError(acs::pc::SyncPriorityContainer<Item>& _consuming, Item item) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '[' << this->name() << ']' << "Error Encountered. (product,attempts,errno) = (" << item.groupId() << ", " << item.attempts() << ","
                        << item.lastErrorCode() << ")");
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '[' << this->name() << ']' << item.str());
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '[' << this->name() << ']' << "Error Encountered. (attempts,errno) = " << item.attempts() << "," << item.lastErrorCode());
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,item.groupId() <<"Error Encountered. (attempts,errno) = " << item.attempts() << "," << item.lastErrorCode());
        if (item.lastErrorCode() == conf::ERROR_HOST_UNKNOWN) { // PRQA S 1021
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,item.groupId() << "Host Unknown. Will Retry if the max number of retry not reached.");
        }
        if (item.lastErrorCode() == conf::ERROR_REMOTE_FILE_SYSTEM_ERROR) { // PRQA S 1021
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,item.groupId() << "Remote File System Error. Will Retry if the max number of retry not reached.");
        }
        if (item.lastErrorCode() == conf::ERROR_TRANSFER_INTERRUPTED) { // PRQA S 1021
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,item.groupId() << "Transfer interrupted. Will Retry if the max number of retry not reached.");
        }
        if (item.lastErrorCode() == conf::ERROR_UNKNOWN) { // PRQA S 1021
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,item.groupId() << "Unknown Error. Will Retry if the max number of retry not reached.");
        }
        if (item.lastErrorCode() == conf::ERROR_LOGIN) { // PRQA S 1021
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,item.groupId() << "Login denied. Cannot retry, change userid/password");
            this->_finishedItems.push(item);
        } else {
            if (item.attempts() < getMaxAttempts()) {
                if (!this->_stopper.requestedCancel()) {
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,item.groupId() << "Retry to transfer the last errored item.");
                    this->_erroredItems.push(item);
                } else {
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,item.groupId() << "Cannot retry this errored item because the system was stopped.");
                    this->_finishedItems.push(item);
                }
            } else {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,item.groupId() << "Max Attempts reached, transfer file finished with error.");
                if (item.lastErrorCode()) {
                    this->_finishedItems.push(item);
                }
            }
        }
    }

    virtual size_t getElapsedTimeToRetry() const =0;

    virtual size_t getMaxAttempts() const =0;
private:
    GenericRegister() ; // not implemented
    GenericRegister(const GenericRegister &) ; // not implemented
    GenericRegister& operator= (const GenericRegister &) ; // not implemented
protected:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(GenericRegister); // PRQA S 2101

};

ACS_TEMPLATE_DEFINE_DEBUG_LEVEL (GenericRegister, Item) ; // PRQA S 2101

_ACS_END_NESTED_NAMESPACE

#endif /* GENERICREGISTER_H_ */
