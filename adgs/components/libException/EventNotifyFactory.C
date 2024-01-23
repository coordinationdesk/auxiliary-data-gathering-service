// PRQA S 1050 EOF
/*
	Copyright 2018-2021, Exprivia SpA - DFDA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

    libException EventNotifyFactory class
*/

#include <EventNotifyFactory.h>
#include <algorithm>
#include <StopController.h>

_ACS_BEGIN_NAMESPACE(acs)


//**********************************************************************
// 
//                   EventNotifyFactorySingleton
// 
//**********************************************************************

ACS_CLASS_DEFINE_DEBUG_LEVEL(EventNotifyFactorySingleton);

EventNotifyFactorySingleton::EventNotifyFactorySingleton() :
	Singleton<EventNotifyFactorySingleton>(),
	_stackMutex(),
	_buildersStack(),
    _shutDownInProgress() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

EventNotifyFactorySingleton::~EventNotifyFactorySingleton() ACS_NOEXCEPT { }

/*
 * Returns the event notifier built by the last registered builder
 */
EventNotifier* EventNotifyFactorySingleton::newItem() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    if (_shutDownInProgress)
    {
        return 0;
    }

	// Lock the builder stack
	Lock slock(_stackMutex);
	
	EventNotifier* notifier = 0;
	
	// If builders exists
	if (!_buildersStack.empty()) {
		notifier = _buildersStack.back()->createItem();
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No builder found. Returning null pointer");
	}
	
	// Return the last builder inserted
	return notifier;
}

void EventNotifyFactorySingleton::shutdown()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    // Avoid any other interaction with ::newItem
    _shutDownInProgress = true;

	// Lock the builder stack
	Lock slock(_stackMutex);

    for (auto&& notifier : _buildersStack)
    {
        // Ask the notifier to shutdown.. this should not block forever
        notifier->shutdown();
    }

    // Time tracking structures
    struct timespec tp; 
    struct timespec newtp; 
    
    // Wait all the notifiers to complete the shutdown sequence
    for (auto&& notifier : _buildersStack)
    {
        // Get the current time
        if ( 0 != ::clock_gettime (CLOCK_MONOTONIC, &tp) )
        {
            //Unable to get current time... exit the loop and pray
            ACS_LOG_WARNING ("EventNotifyFactorySingleton::shutdown impossible to get current time, builders will not be properly stopped");
            break;
        }

        while ( ! notifier->safeToExit() )
        {
            // Get again the current time.. to compute elapsed milliseconds
            if ( 0 != ::clock_gettime (CLOCK_MONOTONIC, &newtp) )
            {
                //Unable to get current time... skip this notifier (and pray...)
                ACS_LOG_WARNING ("EventNotifyFactorySingleton::shutdown impossible to get current time, builders will not be properly stopped");
                break;
            }

            // 5 seconds of timeout
            long elapsedms = (newtp.tv_sec-tp.tv_sec)*1000 + (newtp.tv_nsec-tp.tv_nsec)/1000000;
            if (elapsedms >= 5000)
            {
                // timeout.
                // cannot ensure that the notifier will not crash when exiting
                // do not wait anymore, go ahead and pray :)
                ACS_LOG_WARNING ("EventNotifyFactorySingleton::shutdown impossible to shut down a notifier");
                break;
            }
            StopController::nanosleep (1000 * 10); // 10 milliseconds
        }
    }

    // No more builders allowed from now on
    _buildersStack.clear();
}

/*
 * Register the input builder
 */
void EventNotifyFactorySingleton::registerBuilder(EventNotifyFactory* builder) { // PRQA S 4020
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	if (0 == builder || _shutDownInProgress) { 
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Null builder pointer given as input or shutting down. No registration performed.");
		return;
	}

	// Lock the builder stack
	Lock slock(_stackMutex);
	_buildersStack.push_back(builder);
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Builder pointer " << builder << " successfully registered.");
}

/*
 * Unregister the input builder
 */
void EventNotifyFactorySingleton::unregisterBuilder(EventNotifyFactory* builder) { // PRQA S 4020
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	if (0 == builder || _shutDownInProgress) { 
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Null builder pointer given as input or shutting down. Simply returns");
		return;
	}
	
	// Lock the builder stack
	Lock slock(_stackMutex);
	std::vector<EventNotifyFactory*>::iterator it = std::find(_buildersStack.begin(), _buildersStack.end(), builder);
	if (it != _buildersStack.end()) {
		_buildersStack.erase(it);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Builder pointer " << builder << " successfully unregistered.");
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Builder pointer " << builder << " not found.");
	}
}

//**********************************************************************
// 
//                         EventNotifyFactory
// 
//**********************************************************************


ACS_CLASS_DEFINE_DEBUG_LEVEL(EventNotifyFactory)

EventNotifyFactory::EventNotifyFactory() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	// Register itself
	EventNotifyFactorySingleton::instance()->registerBuilder(this);
}

EventNotifyFactory::~EventNotifyFactory() ACS_NOEXCEPT {
	// PRQA S 4631 L1
	try {
		EventNotifyFactorySingleton::instance()->unregisterBuilder(this);
	} catch (std::exception& e) {
		ACS_LOG_NOTIFY_EX(e);
	}
	// PRQA L:L1
}

//**********************************************************************
// 
//                         EventNotifier
// 
//**********************************************************************

ACS_CLASS_DEFINE_DEBUG_LEVEL(EventNotifier)

EventNotifier::EventNotifier() { }
EventNotifier::~EventNotifier() ACS_NOEXCEPT { }

_ACS_END_NAMESPACE
