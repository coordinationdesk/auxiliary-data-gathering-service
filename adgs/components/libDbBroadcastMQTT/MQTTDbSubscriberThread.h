/*
	Copyright 2021-, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. MQTTDbSubscriberThread HEADER $
*/


#ifndef _MQTTDbSubscriberThread_H_
#define _MQTTDbSubscriberThread_H_

#include <MQTTDbTopicGenerator.h>
#include <MQTTDbSubscriber.h>
#include <MQTTWorker.h>

#include <acs_c++config.hpp>
#include <exException.h>
#include <Filterables.h>
#include <Thread.h>
#include <ThreadSafe.h>
#include <dbConnectionPool.h>

#include <stdint.h>
#include <string>
#include <semaphore.h>	/* POSIX semaphore */


_ACS_BEGIN_NAMESPACE(acs)


class MQTTDbSubscriberThread: public Thread  {
public:

	/** Default Class constructor */
	explicit MQTTDbSubscriberThread(
		const bool hb_enabled, 
		const bool messages_enabled, 
		const std::string& topic, 
		const MQTTDbSubscriber::Enum_TimeWait time_wait,
 		const MQTTWorker::Configuration config,
		sem_t* p_sem_threads,
		StopController const& stopController,
		const std::string& threadName="MQTTDbSubscriberThread") ;

	/** Destructor */
	virtual ~MQTTDbSubscriberThread() ACS_NOEXCEPT ;
	MQTTDbSubscriberThread(const MQTTDbSubscriberThread & ) = delete ;
	MQTTDbSubscriberThread &operator=(const MQTTDbSubscriberThread &) = delete ;
	MQTTDbSubscriberThread(MQTTDbSubscriberThread && ) = delete ;
	MQTTDbSubscriberThread &operator=(MQTTDbSubscriberThread &&) = delete ;

public:
	/** Return the pointer to the embedded MQTTDbSubscriber */
	inline MQTTDbSubscriber* subscriber()				{ return _p_subscriber ; }

	/**
		Return TRUE accordingly to the condition of reporting and move the message stack to the caller, resetting the local stack/flag
	*/
	bool retrieveMessages(std::vector <MQTTMessageHandler>& messages_V) ;

	
	inline bool messages_enabled() const			{ ThreadSafe::Lock lock(_messagesMutex) ; return _messages_enabled ; }
	inline void enable_messages(const bool b=true)	{ ThreadSafe::Lock lock(_messagesMutex) ; _messages_enabled = b ; }
	inline void disable_messages() 				   { enable_messages(false) ; }

	
private: 
	
	/** Connect to the MQTT server */
	void connect() ; 
	/** Disconnect from the MQTT server */
	void disconnect() ; 
	
protected:
	/** Thread interface implementation */
	virtual void run();


private:
	MQTTDbSubscriber*									_p_subscriber ;
	bool												_hb_enabled ;
	std::string											_topic	;		 
	std::vector <MQTTMessageHandler>					_messages_V	;		// Local stack of messages
	bool												_message_received ;	// Local flag, set as true accordingly to the report condition	 
	MQTTDbSubscriber::Enum_TimeWait						_time_wait ;
 	MQTTWorker::Configuration							_config ;
	sem_t*												_p_sem_threads ;
	bool												_messages_enabled ;
	StopController const&								 _stopController ;
	static Mutex 									     _messagesMutex ; 

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(MQTTDbSubscriberThread)

};

_ACS_END_NAMESPACE

#endif //_MQTTDbSubscriberThread_H_

