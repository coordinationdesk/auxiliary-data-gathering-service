// PRQA S 1050 EOF
/*
   Copyright 2019-2021, Exprivia SpA
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Exprivia, SpA

	$Prod: MQTT DbSubscriber Header File $

	$Log$
*/ 


#ifndef _MQTTDbMultiSubscriber_H_
#define _MQTTDbMultiSubscriber_H_

#include <MQTTDbTopicGenerator.h>
#include <MQTTDbSubscriberThread.h>
#include <MQTTWorker.h>
#include <Filterables.h>
#include <ThreadSafe.h>
#include <Timer.h>
#include <dbConnectionPool.h>
#include <exException.h>
#include <semaphore.h>	/* POSIX semaphore */

_ACS_BEGIN_NAMESPACE(acs) 


/**
	Between the arguments of the c'tor there is also a CALLBACK FUNCTION.
	Signature: 
	void myfunc(bool new_messages, size_t number_of_messages)
	
	- The callback function is optional, a NULL ptr can be provided instead of a function
	- The arguments are:
		- the boolean "new_messages" is true if at least one of the subscriber threads returned a true boolean
		- the size_t "number_of_messages" contains the number of messages (before any filtering)
		
	NOTE:
	- The boolean should never appear false, since the function is called only if the boolean is true. In any case, the caller
		should verify if the boolean is true, in case of future changes.
	- The number of messages could be zero or greater
	- The boolean might be TRUE even if the number of messages is 0 -> the meaning is that a timer-generated or forced condition 
		was triggered, without receiving actual messages.
	- The latter case (TRUE with no messages) should NEVER BE IGNORED OR FILTERED by the caller.
*/

class MQTTDbMultiSubscriber: public Thread
{

public:

	/** If the config.host is SET, the c'tor automatically connects with the MQTT configured server.
		Otherwise, if config.host == "", the c'tor is assumed VOID, so MQTT service is not activated:
		the whole notification system will rely only on automatic hits 
	 */
	MQTTDbMultiSubscriber(const bool HB_enabled, dbConnectionPool&, const std::string&, const int&, \
		void (*callback)(bool, size_t, pthread_t), \
		StopController const&, const std::string& threadName="MQTTDbMultiSubscriber") ;

	/** D'tor */
	virtual ~MQTTDbMultiSubscriber() ACS_NOEXCEPT ;

	/** Subscribe to a table/action -OR- topic. The unique index is returned */
	size_t subscribe (const bool messages_enabled, const std::string& table, const enum SQLDialect::SQLToken& action, \
		const MQTTDbSubscriber::Enum_TimeWait& time_wait=MQTTDbSubscriber::WAIT_STD) ;
	size_t subscribe (const bool messages_enabled, const std::string& table, const std::string& action, \
		const MQTTDbSubscriber::Enum_TimeWait& time_wait=MQTTDbSubscriber::WAIT_STD) ;
	size_t subscribe (const bool messages_enabled, const std::string& topic, const MQTTDbSubscriber::Enum_TimeWait& time_wait=MQTTDbSubscriber::WAIT_STD) ;

	/** Unsubscribe from all indexes and reset index counter */
	void unsubscribe () ; 
	/** Unsubscribe from one index */
	bool unsubscribe (const size_t& index) ; 

	/** Check if the messages are enabled for all indexes */
	bool messages_enabled () const ; 
	/** Enable messages for all indexes */
	void enable_messages (const bool b=true) ; 
	/** Enable messages for one index */
	void enable_messages (const bool b, const size_t& index) ; 

	/**
		Return TRUE accordingly to the condition of reporting and move the message stack to the caller, resetting the local stack/flag
	*/
	bool retrieveMessages(std::vector <MQTTMessageHandler>& messages_V) ; 

	/**
		Return TRUE accordingly to the condition of reporting. If reset is true, it reset the local stack/flag
	*/
	bool gotMessages(size_t& messages, const bool reset) ;

	/**
		VIRTUAL FUNCTION
		Customized message-filter. The default implementation is "return true" (no filter)
	*/
	virtual bool filterMessage(const MQTTMessageHandler& message);

	/**
		Size of the subscriber vector
	*/
	inline size_t size() { return _threads_V.size() ; }

	/**
		The operator[] is used to address a specific MQTTDbSubscriber
	*/
	MQTTDbSubscriber* operator[](size_t index) ;

	bool callback_enabled() const;
	void enable_callback(const bool b=true);
	void disable_callback() ;
	bool heartbeat_enabled() const;
	void enable_heartbeat(const bool b=true);
	void disable_heartbeat() ;
	
	void setPort(int port);
	void setHost(const std::string & host);

protected:
	virtual void init();
	/** Thread interface implementation */
	virtual void run();

private:
	/** Restart the thread in case of crash */
	void monitorThisThreadIsRunning() ;
	static void compute_wait_time (const uint16_t& timeout_ms, struct timespec& ts) ;

private:

	// Disable copy
	MQTTDbMultiSubscriber (const MQTTDbMultiSubscriber&);
	MQTTDbMultiSubscriber& operator= (const MQTTDbMultiSubscriber&);

private:

	dbConnectionPool&									_connectionPool ;			
	std::vector <MQTTMessageHandler>					_messages_V ;		// Local stack of messages
	bool												_message_received ; // Local flag, set as true accordingly to the report condition
	std::vector < std::pair<MQTTDbSubscriberThread*, bool> >	_threads_V ;
	std::string											_host ;
	int													_port ;
	sem_t												_sem_threads ;
	void (*_callback)(bool, size_t, pthread_t) ;
	volatile bool										_callback_enabled ; 
	bool												_heartbeat_enabled ; 
	StopController	const&								_stopController ;
	static ThreadSafe 									_resourceMutex ; 
	volatile pthread_t 									_signalReceiverThreadId;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(MQTTDbMultiSubscriber) ;
} ;

_ACS_END_NAMESPACE

#endif //_MQTTDbMultiSubscriber_H_
