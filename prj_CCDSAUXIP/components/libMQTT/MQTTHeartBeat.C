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

   $Prod: MQTT HeartBeat Module $

   $Id: [CPS-101] $

   $Log$

*/ 


#include <MQTTHeartBeat.h>
#include <Filterables.h>
#include <MQTTHelper.h>
#include <MQTTWorker.h>

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#ifndef DISABLE_MQTT
#include <mosquitto.h>
#else 
#warning MQTT has been disabled
#endif

_ACS_BEGIN_NAMESPACE(acs) 

ACS_CLASS_DEFINE_DEBUG_LEVEL(MQTTHeartBeat::TopicGenerator) ;
ACS_CLASS_DEFINE_DEBUG_LEVEL(MQTTHeartBeat::Publisher) ;
ACS_CLASS_DEFINE_DEBUG_LEVEL(MQTTHeartBeat::Subscriber) ;

using std::string ;

const string MQTTHeartBeat::JsonHBData::_TAG_first_time = "first_time_epoch" ;
const string MQTTHeartBeat::JsonHBData::_TAG_current_time = "current_time_epoch" ;
const string MQTTHeartBeat::JsonHBData::_TAG_step_time = "step_time_s" ;
const string MQTTHeartBeat::JsonHBData::_TAG_counter = "counter" ;
const string MQTTHeartBeat::JsonHBData::_TAG_pid = "pid" ;
const string MQTTHeartBeat::JsonHBData::_TAG_unique_id = "unique_id" ;
const string MQTTHeartBeat::TopicGenerator::_default_topic_subsystem("HB") ;
ThreadSafe MQTTHeartBeat::Subscriber::_resourceMutex ;


/*--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --   

		class JsonHBData

--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --*/

/***************************************************************************************
**
				JsonHBData::initialize
**
***************************************************************************************/
void MQTTHeartBeat::JsonHBData::initialize (const std::string& unique_id, const off64_t& step_time)  
{
	/*
		The heartbeat is initialized, by the user when he/she is ready to start.
		* step_time, pid and unique_id are provided by the caller
		* last time and counter are zero
		* first time and current time are updated with the current timestamp
	*/
	const time_t cur_time(time(nullptr)) ; 
	set_first_time(cur_time) ;
	set_current_time(cur_time) ;
	set_step_time(step_time) ; 
	set_counter(0) ;
	set_pid(::getpid()) ;
	set_unique_id(unique_id) ;
}


/***************************************************************************************
**
				JsonHBData::self_update
**
***************************************************************************************/
void MQTTHeartBeat::JsonHBData::self_update()
{
	/*
		The heartbeat is updated:
		* step_time, pid and unique_id are not touched
		* counter is increased by 1 and current_time is updated.
	*/
	set_current_time(time(nullptr)) ;
	set_counter(get_counter()+1) ;
}


/***************************************************************************************
**
				JsonHBData::operator==
**
***************************************************************************************/
bool MQTTHeartBeat::JsonHBData::operator==(const JsonHBData &other) const 
{
	return (
			(this->get_first_time()      == other.get_first_time()) 
			&& (this->get_current_time() == other.get_current_time()) 
			&& (this->get_step_time()    == other.get_step_time()) 
			&& (this->get_counter()      == other.get_counter()) 
			&& (this->get_pid()          == other.get_pid()) 
			&& (this->get_unique_id()    == other.get_unique_id()) 
	) ;			
}


/*--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --   

		class TopicGenerator

--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --*/


/***************************************************************************************
**
                             TopicGenerator
**
***************************************************************************************/
MQTTHeartBeat::TopicGenerator::TopicGenerator(const std::string& subsystem_id, const std::string& entity, const std::string& entity_id ):
	_topic_organization(MQTTHelper::getOrganization()),
	_topic_subsystem(get_default_topic_subsystem()),
	_topic_subsystem_id(subsystem_id),
	_topic_entity(entity),
	_topic_entity_id(entity_id)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "C'tor called.") ;
	ACS_CLASS_END_DEBUG
}


/***************************************************************************************
**
                             TopicGenerator
**
***************************************************************************************/
MQTTHeartBeat::TopicGenerator::TopicGenerator():
	_topic_organization(MQTTHelper::getOrganization()),
	_topic_subsystem(get_default_topic_subsystem()),
	_topic_subsystem_id(MQTTHelper::getSubsystemId()),
	_topic_entity(MQTTHelper::getEntity()),
	_topic_entity_id(MQTTHelper::getEntityId())
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "C'tor called.") ;
	ACS_CLASS_END_DEBUG
}


/***************************************************************************************
**
                             ~TopicGenerator
**
***************************************************************************************/
MQTTHeartBeat::TopicGenerator::~TopicGenerator() noexcept
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB)
}


/***************************************************************************************
**
				TopicGenerator::generate_topic
**
***************************************************************************************/
string MQTTHeartBeat::TopicGenerator::generate_topic() const
{
	return generate_topic(	get_topic_organization(),
							get_topic_subsystem(),
							get_topic_subsystem_id(),
							get_topic_entity(),
							get_topic_entity_id() ) ;
}


/***************************************************************************************
**
				[STATIC]  TopicGenerator::generate_topic 
**
***************************************************************************************/
string MQTTHeartBeat::TopicGenerator::generate_topic (const string& subsystem_id, const string& entity, const string& entity_id )
{
	return generate_topic ( 
		MQTTHelper::getOrganization(), 
		get_default_topic_subsystem(), 
		subsystem_id, 
		entity, 
		entity_id 
	) ;
} 


/***************************************************************************************
**
				[STATIC] TopicGenerator::generate_topic
**
***************************************************************************************/
string MQTTHeartBeat::TopicGenerator::generate_topic (	const string& topic_organization,	
														const string& topic_subsystem,		
														const string& topic_subsystem_id,	
														const string& topic_entity,			
														const string& topic_entity_id)
{
	std::ostringstream topic ; 
	topic 
			<< topic_organization 
			<< "/" << topic_subsystem 
			<< "/" << topic_subsystem_id 
			<< "/" << topic_entity 
			<< "/" << topic_entity_id ;
	;
		
	const string topic_out ( topic.str() ) ; 

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
		ACS_LOG_DEBUG(SimpleDebugSignatureThread 
            << "Method finished. topic_organization: \"" << topic_organization << "\"" 
			<< "  topic_subsystem: \"" << topic_subsystem << "\"" 
			<< "  topic_subsystem_id: \"" << topic_subsystem_id << "\"" 
			<< "  topic_entity: \"" << topic_entity << "\"" 
			<< "  topic_entity_id: \"" << topic_entity_id << "\"" 
			<< "  Topic: \"" << topic_out << "\"" ) ;
	ACS_CLASS_END_DEBUG

	return topic_out ;
}


/*--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --   

		class Publisher

--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --*/

/***************************************************************************************
**
                             Publisher
**
***************************************************************************************/
MQTTHeartBeat::Publisher::Publisher(const MQTTWorker::Configuration& config, 
									const string& subsystem_id, 
									const string& entity, 
									const string& entity_id, 
									StopController const& stopController, 
									const size_t& hb_timeout_s) :
	MQTTPublisher (entity_id, config, stopController), 
	Thread(("HB_"+entity_id)),
	_topic_generator(subsystem_id, entity, entity_id), 
	_lastHeartBeat(), 
	_stopController(stopController)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	// Create the first valid value for _lastHeartBeat, using "now" as current date
	_lastHeartBeat.initialize(entity_id, hb_timeout_s) ;	

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method finished.") ;
	ACS_CLASS_END_DEBUG
}


/***************************************************************************************
**
                             Publisher
**
***************************************************************************************/
MQTTHeartBeat::Publisher::Publisher(const MQTTWorker::Configuration& config, 
									StopController const& stopController, 
									const size_t& hb_timeout_s) :
	MQTTPublisher (MQTTHelper::getEntityId(), config, stopController), 
	Thread("HB_"+MQTTHelper::getEntityId()),
	_topic_generator(), 
	_lastHeartBeat(), 
	_stopController(stopController)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	// Create the first valid value for _lastHeartBeat, using "now" as current date
	_lastHeartBeat.initialize(MQTTHelper::getEntityId(), hb_timeout_s) ;	

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method finished.") ;
	ACS_CLASS_END_DEBUG
}


/***************************************************************************************
**
                             ~Publisher
**
***************************************************************************************/
MQTTHeartBeat::Publisher::~Publisher() noexcept
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	// Do nothing, the base dtor ::~Publisher() is in charge.
}


/***************************************************************************************
**
                      Publisher::generate_heartbeats
**
***************************************************************************************/
void MQTTHeartBeat::Publisher::generate_heartbeats() 
{
	/** Connect to MQTT and run the HB publishing thread (this method will return immediately)
		THIS METHOD **MUST** BE RUN BY THE UPPER-LEVEL COMPONENT.
		The thread will be started only if the step_time is greater than 0 sec 
		(The opposite of this method is the usual ::shutDownMQTTLayer(), which is automatically called by the ::~dtor)
	 */

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called.") ;
	ACS_CLASS_END_DEBUG

	const time_t step_time(_lastHeartBeat.get_step_time()) ; 

	if (step_time > 0)
	{
		ACS_LOG_INFO("Starting MQTT HeartBeat Publisher.  MQTT Server: " << getConfiguration().host << ":" << getConfiguration().port \
			<< "  SubsystemId: '" << topic_generator().get_topic_subsystem_id() \
			<< "'  Entity: '" << topic_generator().get_topic_entity() << "'  EntityId: '" << topic_generator().get_topic_entity_id() \
			<< "'  Topic: '" << topic_generator().generate_topic() << "'  HB Period_s: " << step_time) ; 
	
		startPublishing() ; 
	
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Connected to MQTT ? " << (isConnected()? "TRUE" : "FALSE") \
				<< "  Starting thread: " + threadName() + " . Step_time: " << step_time << " [s]") ;
		ACS_CLASS_END_DEBUG

		// Start this thread
		start() ; 

		// Check if the thread could start
		for (size_t i=0; 
			( (i<10) && (false==running()) 
				&& (false == _stopController.requestedCancel()) && (false == requestedCancel()) ); 
			i++) 
		{ sleep(1) ; /* Wait up to 10s */ }

		if ( (false == running()) && (false == _stopController.requestedCancel()) && (false == requestedCancel()) )
		{
			// Not running, and not stopping either
			ACS_THROW ( exIllegalValueException("Unable to self-start thread (" + threadName() + ")") ) ; 
		}
	}
	else
	{
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Step_time: " << step_time << " [s]:  Not starting thread (" + threadName() + ")") ;
		ACS_CLASS_END_DEBUG
	}
}


/***************************************************************************************
**
                      Publisher::run
**
***************************************************************************************/
void MQTTHeartBeat::Publisher::run() 
{
	/** Do not call this method directly. See: generate_heartbeats */

	const time_t step_time(_lastHeartBeat.get_step_time()) ; 

	if (step_time > 0) 
	{
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method started. step_time: " << step_time << " [s]") ;
		ACS_CLASS_END_DEBUG

		uint64_t i(0) ;	// Mainly, used to emit the first message immediately
		Timer tim ; tim.start() ; 
		while ( (++i) && (false == _stopController.requestedCancel()) && (false == requestedCancel()) )
		{
			if ( (1 == i) || (true == tim.isOlder(step_time *1000/*[sec to msec]*/)) )
			{
				_lastHeartBeat.self_update() ;
			
				const string mqtt_topic(_topic_generator.generate_topic()) ; 
				const string mqtt_message(_lastHeartBeat.serialize()) ; 

				if ( (false == mqtt_topic.empty()) && (false == mqtt_message.empty()) ) // NOSONAR
				{
					// WHEN HERE: The pair { mqtt_topic, mqtt_message } has been generated

					ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
						ACS_LOG_DEBUG(SimpleDebugSignatureThread << "On topic: \"" << mqtt_topic << "\"  Publishing the message: \"" 
							<< mqtt_message << "\"" );
					ACS_CLASS_END_DEBUG

					// Publish the topic/message
					setTopic(mqtt_topic);
					try {
						publishText(mqtt_message, MQTTWorker::QOS_0);
					} 
					catch (const MQTTWorker::ConnectionError& e)
					{ 
						ACS_LOG_WARNING ( "MQTT CONNECTION ERROR. Unable to publish the message: \"" << mqtt_message << "\"" << '\n' << e.what() ) ;  
					}
					catch (/*NOSONAR*/const std::exception& e)
					{ 
						ACS_LOG_WARNING ( "MQTT GENERIC ERROR. Unable to publish the message: \"" << mqtt_message << "\"" << '\n' << e.what() ) ;  
					}
				}	// END: if ( (false == mqtt_topic.empty()) && (false == mqtt_message.empty()) )
				
				tim.start() ; // Reset the timer
			}
			else { sleep (1) ; }	// Wait one second

		}	; // END: while
		
	}	// END: if (step_time > 0) 

	ACS_LOG_INFO ("MQTTHeartBeat::Publisher::run_publisher: quitting the main loop. step_time was: " << step_time) ; 
}


/*--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --   

		class Subscriber

--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --*/

/***************************************************************************************
**
                             Subscriber
**
***************************************************************************************/
MQTTHeartBeat::Subscriber::Subscriber(	const MQTTWorker::Configuration& config, 
										const string& subsystem_id, 
										const string& entity, 
										const string& entity_id, 
										StopController const& stopController, 
										const size_t& hb_timeout_s) :
	MQTTSubscriber (("HB_RECEIVER_" + config.clientId), TopicGenerator::generate_topic(subsystem_id, entity, entity_id), config, stopController), 
	_lastHeartBeat(), 
	_max_ratio_pc(200.0 /*[As %, i.e. write 150.0 for 1.5 times and so on]*/), 
	_stopController(stopController)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	// Due to the extremely low rate of messages, there is no need of an external message queue, an emitter thread and so on:
	setMessagesAreQueuedExternally(false) ;

	// Create the first valid value for the _lastHeartBeat, using "now" as current date
	// This simulates that the first HB has been received at the start of the subscriber,
	// preventing an "early alarm" (i.e. an alarm before waiting for a real HB).
	// If an ACTUAL HB is not received within the NEXT timeout, the fake _lastHeartBeat will expire and an alarm will be issued.
	_lastHeartBeat.initialize(entity_id, hb_timeout_s) ;	// Create the first fake HB

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		ACS_LOG_DEBUG("Method finished.") ;
	ACS_CLASS_END_DEBUG
}


/***************************************************************************************
**
                             ~Subscriber
**
***************************************************************************************/
MQTTHeartBeat::Subscriber::~Subscriber() noexcept
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	// Do nothing, the base dtor ::~Subscriber() is in charge.
}


#ifndef DISABLE_MQTT
/***************************************************************************************
**
                                  Subscriber::onMessage
**
***************************************************************************************/
void MQTTHeartBeat::Subscriber::onMessage (const mosquitto_message *msg)
{
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called.") ;
	ACS_CLASS_END_DEBUG
					
	if (msg->payloadlen > 0)
	{
		string mqtt_message(static_cast<char *>(msg->payload), msg->payloadlen) ;

		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << "  Received Message [as string]: \"" << mqtt_message << "\"");
		ACS_CLASS_END_DEBUG
	
		JsonHBData lastHeartBeat(mqtt_message) ;

		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << "  JSON dump: " << lastHeartBeat.dump()) ;
		ACS_CLASS_END_DEBUG
		
		{
			Mutex::Lock lock(_resourceMutex) ;
			_lastHeartBeat = lastHeartBeat ;
		}
	}	// END: if (msg->payloadlen > 0)
	

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method finished.") ;
	ACS_CLASS_END_DEBUG
}
#endif
	
	
/***************************************************************************************
**
				Subscriber::receive_heartbeats
**
***************************************************************************************/
void MQTTHeartBeat::Subscriber::receive_heartbeats() 
{
	/** Connect to MQTT and run the HB receiver thread (this method will return immediately)
		THIS METHOD **MUST** BE RUN BY THE UPPER-LEVEL COMPONENT.
		(The opposite of this method is the usual ::deactivateSubscription(), which is automatically called by the ::~dtor)
	 */

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called.") ;
	ACS_CLASS_END_DEBUG

	activateSubscription(MQTTWorker::QOS_0) ;
		
	ACS_LOG_INFO("MQTT HeartBeat Subscriber started.  MQTT Server: " << getConfiguration().host << ":" << getConfiguration().port 
		<< "  Topic: " << getTopic() << "  HB Period_s: " 
		<< _lastHeartBeat.get_step_time() << "  Connected to MQTT ? " << (isConnected()? "TRUE" : "FALSE")) ; 
}


/***************************************************************************************
**
				Subscriber::hb_is_expired
**
***************************************************************************************/
bool MQTTHeartBeat::Subscriber::hb_is_expired() const
{
	Mutex::Lock lock(_resourceMutex) ;

	return hb_is_expired(_lastHeartBeat, get_max_ratio_pc()) ; 
}


/***************************************************************************************
**
				[STATIC] Subscriber::hb_is_expired
**
***************************************************************************************/
bool MQTTHeartBeat::Subscriber::hb_is_expired(const JsonHBData& lastHeartBeat, const double& max_ratio_pc) 
{
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called") ;
	ACS_CLASS_END_DEBUG

	bool is_expired = false ; 
	
	const time_t current_date (time(nullptr)) ;
	const time_t last_hb(lastHeartBeat.get_current_time()) ; 
	const time_t step_time(lastHeartBeat.get_step_time()) ; 

	if ( (last_hb > 0) && (step_time > 0) )
	{
		if (current_date > last_hb)
		{
			const time_t delta (current_date - last_hb) ;	// Time elapsed after last received HB
			if (delta > step_time)
			{ 
				const double ratio_pc ( (delta*100.0)/(step_time*1.0) ) ; // NOSONAR, num. type capacity not exceeded
				
				// The ratio% is compared to threshold, _max_ratio_pc, defined has how much the step_time can be 
				// EXCEEDED before raising an alarm.
				// Example of computation of ratio_pc:
				// step_time is 60 s [ one HB every minute ]
				//   (1) delta is 40 s		->  not expired yet !
				//   (2) delta is 61 s		->  ratio_pc = (61/60) = 101.6%
				//   (3) delta is 90 s		->	ratio_pc = 150%
				//   (4) delta is 125 s		->	ratio_pc = 208%
				//   (5) delta is 1000 s	->	ratio_pc = 1600%
				//
				// If the _max_ratio_pc threshold (as %), is 120: (2) is not expired, (3) (4) (5) are expired
				// If the _max_ratio_pc threshold (as %), is 200: (1) (2) are not expired, (4) (5) are expired
				// And so on.
				// The threshold is usually 200%, so the alarm is issued after DOUBLING the step_time.
				//
				
				is_expired = (ratio_pc > max_ratio_pc) ; 
			}
		}
	}
	else
	{
		// During normal operations, both timestamp of last HB and the HB period time are always GREATER THAN 0:
		// The former contains the timestamp of the last received HB (epoch1970), or the start-time of ther application
		// The latter must be always greater than 0 (usually, 30 [s]).
	
		// Therefore, if here, either:
		// - Service not started (yet)
		// - Service not enabled
		
		// In both cases, the HB is considered not expired
		is_expired = false ; 
	}

	if (is_expired)
	{
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method finished. Returning: " << std::boolalpha << is_expired);
		ACS_CLASS_END_DEBUG
	} else {
		ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method finished. Returning: " << std::boolalpha << is_expired);
		ACS_CLASS_END_DEBUG
	}

	return is_expired ; 
}

_ACS_END_NAMESPACE
