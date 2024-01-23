// PRQA S 1050 EOF
/*
   Copyright 2019-2022 Exprivia S.p.A. - DFDA-AS
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia S.p.A. ;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Exprivia S.p.A. 

   libMQTT Subscriber base class - HEADER FILE

   $Id: [CPS-101] $

*/ 

#ifndef _MosquittoMessageQueue_H_
#define _MosquittoMessageQueue_H_

#include <MQTTWorker.h>
#include <Thread.h> 
#include <deque>
#ifndef DISABLE_MQTT
#include <mosquitto.h>
#endif

// forward declaration of libmosquitto stuff
struct mosquitto;
struct mosquitto_message;

_ACS_BEGIN_NAMESPACE(acs) 

class StopController ;


/*-------------------------------------------------------------------------------
--                                                                             --
		Class: MosquittoMessage
--                                                                             --
-------------------------------------------------------------------------------*/
		
// Helper class, just used to incapsulate the lowlevel C object "struct mosquitto_message",
// which contains allocated pointers.
// This higher level class allows easier object copy and automatically deallocate the memory after object destruction.
// This can be used to store the received ephemeral mosquitto_message (which is deallocated from the caller immediately after the
// return of method onMessage) for further use (e.g. stored in a queue).
class MosquittoMessage // NOSONAR
{
public:
	MosquittoMessage() ;
	explicit MosquittoMessage(const mosquitto_message* msgp_in) ;
	virtual ~MosquittoMessage() noexcept ;
	MosquittoMessage(const MosquittoMessage&) ;
	MosquittoMessage& operator= (const MosquittoMessage&) ;
#ifndef DISABLE_MQTT
	inline const mosquitto_message* ptr() const { return &_msg ; }
private:
	mosquitto_message _msg ;
#endif
} ;



/*-------------------------------------------------------------------------------
--                                                                             --
		Class: MosquittoMessageQueue
--                                                                             --
-------------------------------------------------------------------------------*/

class MosquittoMessageQueue : public Thread // NOSONAR
{
public:
	MosquittoMessageQueue(const std::string& name, MQTTWorker& subs, StopController const& stopController) ;
	MosquittoMessageQueue(const MosquittoMessageQueue&) = default ;
	MosquittoMessageQueue& operator= (const MosquittoMessageQueue&) = default ;
	virtual ~MosquittoMessageQueue() noexcept ; 

public:
	virtual void run();

	/** Setters/getters for the queue limits */
	size_t get_queue_capacity() const			{ return _queue_capacity ; }
	size_t get_queue_trim_pcg() const			{ return _queue_trim_pcg ; }
	void set_queue_capacity(const size_t& s)	{ _queue_capacity = s ; }
	void set_queue_trim_pcg(const size_t& s)	{ _queue_trim_pcg = s ; }

public:

	/**
		Add a message to the FIFO internal queue, and send a signal to the waiting thread. 
	*/
	size_t push_to_queue_and_signal(const mosquitto_message *msg_in) ;

	/**
		Totally empty the queue. 
	*/
	inline void clear_queue() { trim_queue_to_pcg(0) ; }
	
	/**
		Get queue size. 
	*/
	size_t queue_size() const ; 

	/**
		Check if the thread is running. If requested, throw an exception when not running in order to raise the issue to the user. 
	*/
	bool self_check(const bool emit_exception) const ;

private:

	/**
		Extract the oldest message from the FIFO queue, if any. If empty, wait until the "push" signal is received or the timeout is reached.
	*/
	bool wait_and_pop_from_queue(MosquittoMessage& msg_out, const size_t& max_wait_msec) ;

	
	/**
		Extract the first (i.e.) oldest message the FIFO internal queue, if any.
	*/
	bool pop_from_queue(MosquittoMessage& msg_out) ;

	/**
		Add a message to the FIFO internal queue. Bulk-remove messages from the queue if overflooding.
	*/
	size_t push_to_queue(const mosquitto_message *msg_in) ;
	
	/**
		Shorten the queue up to a given percentage (0-100). 0 means -> queue must be filled to 0% (i.e. must be completely empty, 0 elements)
		100 means -> queue filled up to 100% (can be completely full, do not clean it)
		35 means -> queue filled up to 35% (if queue limit is 1000 elements, it will be shortened to have no more than 350 elements).
		Return the final size. 
	*/
	size_t trim_queue_to_pcg(const size_t& pcg) ;

private:
	MQTTWorker& _sub ;
	std::deque <MosquittoMessage> _mqtt_messages_queue ;	/* FIFO internal queue of messages */
	size_t _queue_capacity ;
	size_t _queue_trim_pcg ;
	Condition _condition ;
	StopController const &_stopController ;
	static Mutex _mqtt_queue_mutex ; 

	ACS_CLASS_DECLARE_DEBUG_LEVEL(MosquittoMessageQueue) ;
} ;


_ACS_END_NAMESPACE

#endif //_MosquittoMessageQueue_H_
