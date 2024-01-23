// PRQA S 1050 EOF
/*
   Copyright 2021-2022, Exprivia S.p.A. - DFDA-AS
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia S.p.A. ;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Exprivia S.p.A. 

   $Prod: MQTT HeartBeat Module Header $

   $Id: [CPS-101] $

   $Log$
*/ 


#ifndef _MQTTHeartBeat_H_
#define _MQTTHeartBeat_H_

#include <MQTTWorker.h>
#include <MQTTSubscriber.h>
#include <MQTTPublisher.h>
#include <JsonSerializable.h>
#include <Filterables.h>
#include <ThreadSafe.h>
#include <Timer.h>
#include <Thread.h>
#include <exException.h>

#ifndef DISABLE_MQTT
#include <mosquitto.h>
#endif


_ACS_BEGIN_NAMESPACE(acs) 

class MQTTHeartBeat
{
	/*--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
			class JsonHBData
	--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --*/
public:
	class JsonHBData : public JsonSerializable
	{	
	public:
		JsonHBData() = default ;
		explicit JsonHBData(const std::string &s) { JsonSerializable::unserialize(s) ; }
		JsonHBData (const JsonHBData&) = default ;
		JsonHBData& operator= (const JsonHBData&) = default ;
		virtual ~JsonHBData() = default ;

		bool operator==(const JsonHBData &other) const ;
		inline bool operator!=(const JsonHBData &other) const { return (not (*this == other)) ; }


	public:
		time_t get_first_time() const { 
            time_t t(0) ; const std::string k(_TAG_first_time) ; 
		    if (has_key(k) && !is_null(k)) { get_value (_TAG_first_time, t) ; } ; 
            return t ; }
            
		time_t get_current_time() const { 
            time_t t(0) ; const std::string k(_TAG_current_time) ; 
		    if (has_key(k) && !is_null(k)) { get_value (_TAG_current_time, t) ; } ; 
            return t ; }
        
		off64_t get_step_time() const { 
            off64_t d(0) ; const std::string k(_TAG_step_time) ; 
            if (has_key(k) && !is_null(k)) { get_value (_TAG_step_time, d) ; } ; 
            return d ; }
            
		off64_t get_counter() const { 
            off64_t d(0) ; const std::string k(_TAG_counter) ; 
		    if (has_key(k) && !is_null(k)) { get_value (_TAG_counter, d) ; } ; 
            return d ; }
            
		off64_t get_pid() const { 
            off64_t d(0) ; const std::string k(_TAG_pid) ; 
		    if (has_key(k) && !is_null(k)) { get_value (_TAG_pid, d) ; } ; 
            return d ; }
            
		std::string get_unique_id() const { 
            std::string s ; const std::string k(_TAG_unique_id) ; 
		    if (has_key(k) && !is_null(k)) { get_value (_TAG_unique_id, s) ; } ; 
            return s ; }

		void set_first_time(const time_t& t)	  { add_pair (_TAG_first_time, t) ; }
		void set_current_time(const time_t& t)    { add_pair (_TAG_current_time, t) ; }
		void set_step_time(const off64_t& d)	  { add_pair (_TAG_step_time, d) ; }
		void set_counter(const off64_t& d)  	  { add_pair (_TAG_counter, d) ; }
		void set_pid(const off64_t& d)  		  { add_pair (_TAG_pid, d) ; }
		void set_unique_id(const std::string& s)  { add_pair (_TAG_unique_id, s) ; }

		/** Update the heartbeat */
		void self_update() ;
		/** Initialize the heartbeat */
		void initialize (const std::string& unique_id, const off64_t& step_time) ; 


	private:
		static const std::string _TAG_first_time ;
		static const std::string _TAG_current_time ;
		static const std::string _TAG_step_time ;
		static const std::string _TAG_counter ;
		static const std::string _TAG_pid ;
		static const std::string _TAG_unique_id ;
	} ;


	/*--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
			class TopicGenerator
	--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --*/
public:
	class TopicGenerator // NOSONAR
	{	
	public:
		TopicGenerator(const std::string& subsystem_id, const std::string& entity, const std::string& entity_id) ;
		TopicGenerator() ;
		TopicGenerator (const TopicGenerator&) = default ;
		TopicGenerator& operator= (const TopicGenerator&) = default ;
		virtual ~TopicGenerator() noexcept ;  

		std::string generate_topic() const ;
		static std::string generate_topic (\
			const std::string& topic_subsystem_id, const std::string& topic_entity, const std::string& topic_entity_id) ;
		static std::string generate_topic (const std::string& topic_organization, const std::string& topic_subsystem,	\
			const std::string& topic_subsystem_id, const std::string& topic_entity, const std::string& topic_entity_id) ;

		/**
			Getters:
		*/
		inline std::string get_topic_organization() const			{ return _topic_organization ; }  
		inline std::string get_topic_subsystem() const				{ return _topic_subsystem ; }  
		inline std::string get_topic_subsystem_id() const			{ return _topic_subsystem_id ; }
		inline std::string get_topic_entity() const					{ return _topic_entity ; }  
		inline std::string get_topic_entity_id() const				{ return _topic_entity_id ; }

		/**
			Setters:
		*/
		inline void set_topic_organization(const std::string& c )	{ _topic_organization = c ; }  
		inline void set_topic_subsystem(const std::string& c )		{ _topic_subsystem = c ; }  
		inline void set_topic_subsystem_id(const std::string& c )	{ _topic_subsystem_id = c ; }  
		inline void set_topic_entity(const std::string& c )			{ _topic_entity = c ; }  
		inline void set_topic_entity_id(const std::string& c )		{ _topic_entity_id = c ; }  


	private:
		static std::string get_default_topic_subsystem()			{ return _default_topic_subsystem ; }  

	private:
		std::string					_topic_organization ;  
		std::string					_topic_subsystem ;  
		std::string					_topic_subsystem_id ;  
		std::string					_topic_entity ;  
		std::string					_topic_entity_id ;  
		static const std::string	_default_topic_subsystem ;

		ACS_CLASS_DECLARE_DEBUG_LEVEL(TopicGenerator) ;

	} ; // END: class TopicGenerator


	/*--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
			class Publisher
	--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --*/
public:
	class Publisher : public MQTTPublisher, public Thread // NOSONAR
	{
	public:
		Publisher(const MQTTWorker::Configuration& config, const std::string& subsystem_id, 
			const std::string& entity, const std::string& entity_id, StopController const& stopController, \
			const size_t& hb_timeout_s=30) ;
		Publisher( const MQTTWorker::Configuration& config, StopController const& stopController, \
			const size_t& hb_timeout_s=30) ;
		Publisher (const Publisher&) = delete ;
		Publisher& operator= (const Publisher&) = delete ;
		virtual ~Publisher() noexcept; 

		inline JsonHBData get_lastHeartBeat() const			{ return _lastHeartBeat ; }
		inline void set_lastHeartBeat(const JsonHBData& hb) { _lastHeartBeat = hb ; }

		TopicGenerator& topic_generator() noexcept { return _topic_generator ; }	/* As reference */

		/** Connect to MQTT and run the HB publishing thread (this method will return immediately)
			THIS METHOD **MUST** BE RUN BY THE UPPER-LEVEL COMPONENT.
			The thread will be started only if the step_time is greater than 0 sec.
			(The opposite of this method is the usual ::shutDownMQTTLayer(), which is automatically called by the ::~dtor)
		 */
		void generate_heartbeats() ;
		

	protected:
		/** Thread interface implementation (do not call directly. See: generate_heartbeats) */
		virtual void run();

	private:
		TopicGenerator			_topic_generator ;
		JsonHBData				_lastHeartBeat ;
		StopController const &	_stopController ;

		ACS_CLASS_DECLARE_DEBUG_LEVEL(Publisher) ; // Hint, to activate debug: MQTTHeartBeat__Publisher_DEBUG=10
	};


	/*--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
			class Subscriber
	--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --*/
public:
	class Subscriber : public MQTTSubscriber // NOSONAR
	{
	public:

		/** If the config.host is SET, the c'tor automatically connects with the MQTT configured server.
			Otherwise, if config.host == "", the c'tor is assumed VOID, so MQTT service is not activated:
			the whole notification system will rely only on automatic hits 
		 */
		Subscriber(const MQTTWorker::Configuration& config, \
			const std::string& subsystem_id, const std::string& entity, const std::string& entity_id, \
			StopController const& stopController, const size_t& hb_timeout_s=30) ;

		Subscriber (const Subscriber&) = delete ;
		Subscriber& operator= (const Subscriber&) = delete ;
		virtual ~Subscriber() noexcept ; 

		inline JsonHBData get_lastHeartBeat() const			{ return _lastHeartBeat ; }
		inline void set_lastHeartBeat(const JsonHBData& hb) { _lastHeartBeat = hb ; }
		inline double get_max_ratio_pc() const				{ return _max_ratio_pc ; }
		inline void set_max_ratio_pc(const double& d)		{ _max_ratio_pc = d ; }
		
		/** Connect to MQTT and run the HB receiver thread (this method will return immediately)
			THIS METHOD **MUST** BE RUN BY THE UPPER-LEVEL COMPONENT.
			(The opposite of this method is the usual ::deactivateSubscription(), which is automatically called by the ::~dtor)
		 */
		void receive_heartbeats() ;

		bool hb_is_expired() const ;
		static bool hb_is_expired(const JsonHBData& lastHeartBeat, const double& max_ratio_pc) ;

	private:
		// Callbacks
#ifndef DISABLE_MQTT
		virtual void onMessage (const mosquitto_message *msg) ;
#endif

	private:
		JsonHBData              _lastHeartBeat ;
		double                  _max_ratio_pc ;	// Threshold, as ratio% of the heathbeat period 
		StopController	const   &_stopController ;
		static Mutex            _resourceMutex ;

		ACS_CLASS_DECLARE_DEBUG_LEVEL(Subscriber) ; // Hint, to activate debug: MQTTHeartBeat__Subscriber_DEBUG=10
	} ;	// END: class Subscriber


} ; // END: class MQTTHeartBeat

_ACS_END_NAMESPACE

#endif //_MQTTHeartBeat_H_
