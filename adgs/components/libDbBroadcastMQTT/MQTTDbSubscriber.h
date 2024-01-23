// PRQA S 1050 EOF
/*
   Copyright 2019-2021, Exprivia SpA
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Advanced Computer Systems, Inc. 

	$Prod: MQTT DbSubscriber Header File $

	$Log$
*/ 


#ifndef _MQTTDbSubscriber_H_
#define _MQTTDbSubscriber_H_

#include <MQTTMessageHandler.h>
#include <MQTTSubscriber.h>
#include <MQTTHeartBeat.h>
#include <Filterables.h>
#include <ThreadSafe.h>
#include <Timer.h>
#include <exException.h>
#include <semaphore.h>	/* POSIX semaphore */

_ACS_BEGIN_NAMESPACE(acs) 

class MQTTDbSubscriber : public MQTTSubscriber
{

public:

	/* Time slots used during the waiting of new messages in the queue */
	enum Enum_TimeWait { 	WAIT_NONE=0x00,			/* NO WAIT -- SHOULD BE USED FOR ONE-TIME CALLS, OR IN CONDITIONED LOOPS -- DO NOT USE IN CLOSED-LOOPS (HIGH CPU LOAD) */
							WAIT_FAST=0x02,			/* Fast performance. Dynamic delay of at maximum 20 ms. It CAN be used in closed-loops (acceptable CPU load) */ 
							WAIT_STD=0x04,			/* Standard performance. Dynamic delay of at maximum 100 ms. It CAN be used in closed-loops (very low CPU load) */ 
							WAIT_CT_FAST=0x12, 		/* Fast constant performance. Constant delay of exactly 20 ms */ 
							WAIT_CT_STD=0x14, 		/* Standard constant performance. Constant delay of exactly 100 ms */ 
							WAIT_CT_SLOW=0x20,		/* Extemelely slow performance. Constant delay of exactly 250 ms */  
							WAIT_UNKNOWN=0xff } ;

	
	/* Time slots used to (optionally) generated false/forced hits */
	enum Enum_TimeForcedHit { 	FORCED_DISABLE=0x00,/* DISABLE FORCED HITS */
								FORCED_NOWAIT=0x01,	/* NO WAIT -- HIT ALWAYS RETURNED. SHOULD BE USED FOR ONE-TIME CALLS WHEN NOTIFICATION SERVICE IS NOT AVAILABLE */
								FORCED_FASTEST=0x10,/* VERY SHORT WAIT [100 msec] -- Could be used on closed loops when notification service is NOT available */
								FORCED_FAST=0x20,	/* Short wait [5 sec] -- For high-reliability systems */
								FORCED_STD=0x30,	/* Standard wait [30 sec] -- Could be used as fallback in standard-performance systems */
								FORCED_SLOW=0x40,	/* Long wait [5 minutes]. For low-performance system */
								FORCED_SLOWEST=0x7f,/* Very long wait [30 minutes]. For low-performance system */
								FORCED_20MSEC=0x100,/* Custom time:    20 msec wait */
								FORCED_1SEC=0x110,	/* Custom time:  1000 msec wait */
								FORCED_10SEC=0x120,	/* Custom time: 10000 msec wait */
								FORCED_UNKNOWN=0xff
							} ;
	
public:

	/** If the config.host is SET, the c'tor automatically connects with the MQTT configured server.
		Otherwise, if config.host == "", the c'tor is assumed VOID, so MQTT service is not activated:
		the whole notification system will rely only on automatic hits 
	 */
	MQTTDbSubscriber(const bool enable_hb, const std::string& topic, const struct MQTTWorker::Configuration&, StopController const&, const Enum_TimeWait& time_wait=WAIT_STD) ;

	/** D'tor */
	virtual ~MQTTDbSubscriber() ACS_NOEXCEPT ;

	/** Start the heartbeats receiver */
	void receive_heartbeats(const struct MQTTWorker::Configuration& config) ; 
	void receive_heartbeats(const struct MQTTWorker::Configuration& config, \
		const std::string& subsystem_id, const std::string& entity, const std::string& entity_id) ; 

	/**
		Move any message, if any, to the stack of the caller. Regardless any condition of reporting.
		The use of this method is _NOT_ recommended, always prefer methods which return accordingly to the condition of reporting.
	*/
	void retrieveAnyMessage(std::vector <MQTTMessageHandler>& messages_V) ; 


	/**
		Return TRUE accordingly to the condition of reporting.
		Also, if (and only if) reporting true, move the message stack to the caller.
	*/
	bool retrieveMessages(std::vector <MQTTMessageHandler>& messages_V) ; 


	/**
		Return TRUE accordingly to the condition of reporting. If reporting is true, the number of involved messages is filled.
		If reset is true purges the message-stack, o/w messages might be obtained via retrieveMessages.
	*/
	bool gotMessages(size_t& messages, const bool reset) ;

	/**
		As gotMessages, but always purges the message stack (the user is _not_ interested to the messages)
	*/
	inline bool gotMessagesAndReset(size_t& messages)	{ const bool reset = true ;  return gotMessages (messages, reset) ; }


	/***************************************************************************************
	**  Flag setters/getters
	***************************************************************************************/

	/**
		Don't stack new messages (CAUTION, YOU HAVE TO RELY UPON "FORCED HITS")
	*/
	inline void report_halt()										{ _flag_report_is_halted = true ; }
	inline bool get_report_halted() const							{ return _flag_report_is_halted ; }
		
	/**
		Resume stacking new messages
	*/
	inline void report_resume()										{ _flag_report_is_halted = false ; }
	
	/**
		Report new messages (if any) only after an elapsed time AFTER the last positive report. -1 -> always report [ no argument to disable ] 
	*/
	inline void report_only_after_ms(const int64_t& ms=-1)			{ _flag_report_only_after_ms = ms ; }
	inline int64_t get_report_only_after_ms() const					{ return _flag_report_only_after_ms ; }

	/**
		Report new messages only if a minimum number of messages has been reached. -1 -> always report [ no argument to disable ] 
	*/
	inline void report_only_after_messages(const int64_t& mesgs=-1) { _flag_report_only_after_mesgs = mesgs ; }
	inline int64_t get_report_only_after_messages() const			{ return _flag_report_only_after_mesgs ; }

	/**
		When reporting new messages, report up to "N" entries, the oldest (so: from 1 to "N"). 
		This might be used along with report_only_after_messages to report an EXACT number of messages (or don't report at all)
	*/
	inline void report_up_to_mesgs(const int64_t& mesgs=-1)			{ _flag_report_up_to_mesgs = mesgs ; }
	inline int64_t get_report_up_to_mesgs() const					{ return _flag_report_up_to_mesgs ; }
		
	/**
		Force a positive report (even if no new messages are present) after an elapsed time AFTER the last positive report. 
	*/
	inline void force_auto_hits(const Enum_TimeForcedHit &timeout = FORCED_STD)												/* Interface is: Enum_TimeForcedHit */	\
																	{ _flag_force_auto_hits = timeout ; }						
	inline void force_auto_hits_ms(const int64_t& time_ms)			{ force_auto_hits(toEnum_TimeForcedHit(time_ms)) ; }	/* Interface is: time expressed in millisec -- the nearest Enum will be used */
	inline void disable_auto_hits() 								{ _flag_force_auto_hits = FORCED_DISABLE ; }
	inline Enum_TimeForcedHit get_force_auto_hits() const			{ return _flag_force_auto_hits ; }
	
	/**
		Automatic configuration of auto-hits
	*/
	inline void self_toggle_auto_hits(const bool& sc = true)		{ _self_toggle_auto_hits = sc ; }
	bool force_auto_hits(const Enum_TimeForcedHit &timeout_fallback, const Enum_TimeForcedHit &timeout_serverless) ;
	inline bool force_auto_hits_ms(const int64_t& timeout_fallback_ms, const int64_t& timeout_serverless_ms) \
		{ return force_auto_hits( toEnum_TimeForcedHit(timeout_fallback_ms), toEnum_TimeForcedHit(timeout_serverless_ms) ) ; }

	/**
		Force one single positive report (even if no new messages are present) at next retrieval. JUST ONE HIT (the flag will self turn-off). 
	*/
	inline void force_next_hit(const bool& b=true)					{ _flag_force_next_hit = b; } 
	inline bool get_force_next_hit() const							{ return _flag_force_next_hit ; }
	
	
	/**
		Macros, to apply per-scenatio recommended settings (THESE PRE-DEFINED SETTINGS ARE JUST A COMBINATION OF THE FUNCTIONS ABOVE). 
	*/
	void macro_fast_loop() ;		/** The scenario is a fast while-loop where high reliability is required */
	void macro_standard_loop() ;	/** The scenario is a typical while-loop, monitoring one table for new items to appear */
	void macro_slow_loop() ;		/** The scenario is a low-performance loop (or a deamon) */
	
			
	/**
		Set/get a new time_wait
	*/
	inline void set_time_wait(const Enum_TimeWait& e)				{ _time_wait = e ; }
	inline Enum_TimeWait get_time_wait() const						{ return _time_wait ; }

	static Enum_TimeWait toEnum_TimeWait(const std::string& s) ;
	static Enum_TimeForcedHit toEnum_TimeForcedHit(const std::string& s) ;
	static Enum_TimeForcedHit toEnum_TimeForcedHit(const int64_t& s) ;
		
	static struct MQTTWorker::Configuration	buildConfiguration() ;
	static struct MQTTWorker::Configuration	buildConfiguration(const MQTTWorker::Configuration&) ;	/* Generate new random clientID*/
	static struct MQTTWorker::Configuration	buildConfiguration(const std::string& host, const int& port = 1884) ;

	static std::string toStr(const Enum_TimeWait& e) ;
	static std::string toStr(const Enum_TimeForcedHit& e) ;
	
		
private:
	// Callbacks
	virtual void onMessage (const struct mosquitto_message *msg) ;
	virtual void onConnect (int rc) ;
	virtual void onDisconnect (int rc) ;

	/**
		Call activateSubscription on the framework. The latter is able to cope with automatic connection/disconnection.
		This method MUST BE EXECUTED ONCE.
	*/
	bool activateFrameworkSubscription() ;

	bool got_heartbeat() ; 

	void compute_wait_time (const uint16_t& timeout_ms) ; 
	bool check_report_event() ;
	void toggle_auto_hits(const bool is_connected, const bool hb_is_valid) ;
	
	inline bool decode_time_wait(uint16_t &time_wait) { bool b ; return decode_time_wait(_time_wait, b, time_wait) ; }

	static uint64_t toMillisec(const Enum_TimeForcedHit& e) ;
	static bool decode_time_wait(const Enum_TimeWait& e, bool &is_real_time, uint16_t &time_wait) ;
	static uint64_t abs_diff(const uint64_t& a, const uint64_t& b) { return ( (a>=b)? (a-b) : (b-a) ) ; }

	static void move(std::list <MQTTMessageHandler> &src, std::vector <MQTTMessageHandler> &dest) ;
	static void move(std::list <MQTTMessageHandler> &src, std::vector <MQTTMessageHandler> &dest, const ssize_t& n) ;

	// Disable copy
	MQTTDbSubscriber (const MQTTDbSubscriber&);
	MQTTDbSubscriber& operator= (const MQTTDbSubscriber&);


private:
	MQTTHeartBeat::Subscriber* 			_hb_subscriber_p ;
	bool								_hb_enabled ;
	std::list <MQTTMessageHandler>		_messages_L ; 
	bool								_flag_report_is_halted ; 
	int64_t								_flag_report_only_after_mesgs ;  							   
	int64_t								_flag_report_up_to_mesgs ;  							   
	int64_t								_flag_report_only_after_ms ; 							   
	Enum_TimeForcedHit					_flag_force_auto_hits ;
	bool								_flag_force_next_hit ;							   
	Timer								_timer ; 
	sem_t								_sem ; 
	struct timespec						_ts ; 
	Enum_TimeWait						_time_wait ; 
	Enum_TimeForcedHit					_timeout_fallback_auto_hits ;
	Enum_TimeForcedHit					_timeout_serverless_auto_hits ;
	bool								_self_toggle_auto_hits ;
	StopController	const				&_stopController ;

private:
	static ThreadSafe 					_resourceMutex ;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(MQTTDbSubscriber) ;
} ;

_ACS_END_NAMESPACE

#endif //_MQTTDbSubscriber_H_
