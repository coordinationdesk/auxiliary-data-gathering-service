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

#ifndef _EventNotifyFactory_H_
#define _EventNotifyFactory_H_

#include <acs_c++config.hpp>
#include <Filterables.h>
#include <Singleton.hpp>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton;

// FORWARD DECLARATIONS
class EventNotifier;
class EventNotifyFactory;

//**********************************************************************
// 
//                   EventNotifyFactorySingleton
// 
//**********************************************************************

class EventNotifyFactorySingleton : public Singleton< EventNotifyFactorySingleton> { // PRQA S 2109, 2153
public:
	virtual ~EventNotifyFactorySingleton() ACS_NOEXCEPT;

	EventNotifyFactorySingleton(const EventNotifyFactorySingleton&) = delete;
	EventNotifyFactorySingleton& operator=(const EventNotifyFactorySingleton&) = delete;
	EventNotifyFactorySingleton(EventNotifyFactorySingleton&&) = delete;
	EventNotifyFactorySingleton& operator=(EventNotifyFactorySingleton&&) = delete;

	
	/**
	 * Returns the event notifier built by the last registered builder
	 */
	EventNotifier* newItem();
	
	/**
	 * Register the input builder
	 */
	void registerBuilder(EventNotifyFactory* builder);

	/**
	 * Unregister the input builder
	 */
	void unregisterBuilder(EventNotifyFactory* builder);

	/**
	 * Shutdown any registered builder and prevent the creation of new events from now on 
	 */
     void shutdown();

    
protected:
	EventNotifyFactorySingleton();


private:
	// Guards builders stack
	Mutex _stackMutex;
	std::vector<EventNotifyFactory*> _buildersStack;
    bool _shutDownInProgress;
	
	// Allows the singleton pattern to access default constructor
	friend EventNotifyFactorySingleton* Singleton<EventNotifyFactorySingleton>::instance(); // PRQA S 2107

	ACS_CLASS_DECLARE_DEBUG_LEVEL(EventNotifyFactorySingleton);
};

//**********************************************************************
// 
//                         EventNotifyFactory
// 
//**********************************************************************

/** 
 * This is the base class for all EventNotifyFactory.
 * It is able to instanciate an EventNotifier that by default does not notify
 * anything. 
 */
class EventNotifyFactory {
public:
	EventNotifyFactory();
	virtual ~EventNotifyFactory() ACS_NOEXCEPT;

    /**
     * Used to shutdown any "active" part of the service (like MQTT Threads)
     * If the EventNotifyFactory is not properly shut down before exiting the process
     * the active threads may cause crashes when destroying global variables
     * Can be overriden in case of notifiers needing a shutdown
     */
    virtual void shutdown() {} // Default is a no-operation
    virtual bool safeToExit() { return true; } // always safe to exit 


protected:
	/**
	 * Must be overwritten by derived classes to create their own notifiers.
	 */
	virtual EventNotifier* createItem() = 0;


private:
	// Defined but not implemented
	EventNotifyFactory(const EventNotifyFactory&);
	EventNotifyFactory& operator=(const EventNotifyFactory&);
	
	// Friend function that allows the singleton to call the createInstance function
	friend EventNotifier* EventNotifyFactorySingleton::newItem(); // PRQA S 2107

	ACS_CLASS_DECLARE_DEBUG_LEVEL(EventNotifyFactory);
};


//**********************************************************************
// 
//                           EventNotifier
// 
//**********************************************************************

/**
 * This is a base event notifier. Derived classes must overwrite all the notifyStatistics
 */
class EventNotifier {
public:
	EventNotifier();
	virtual ~EventNotifier() ACS_NOEXCEPT;
	
	/**
	 * Must be overwritten by derived classes
	 */
	virtual void notifyStatistics(const std::string& statNodePath, int) = 0;
	virtual void notifyStatistics(const std::string& statNodePath, long) = 0;
	virtual void notifyStatistics(const std::string& statNodePath, long long) = 0;
	virtual void notifyStatistics(const std::string& statNodePath, size_t) = 0;
	virtual void notifyStatistics(const std::string& statNodePath, float) = 0;
	virtual void notifyStatistics(const std::string& statNodePath, double) = 0;
	virtual void notifyStatistics(const std::string& statNodePath, long double) = 0;
	virtual void notifyStatistics(const std::string& statNodePath, const std::string& item) = 0;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(EventNotifier);
};

_ACS_END_NAMESPACE

#endif /* _EventNotifyFactory_H_ */
