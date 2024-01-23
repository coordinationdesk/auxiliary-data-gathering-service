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

	$Prod: MQTT DbSubscriber $

	$Log$

*/ 


#include <ACS_SSL.h>
#include <MQTTDbSubscriber.h>
#include <MQTTHeartBeat.h>
#include <Filterables.h>

#include <semaphore.h>	/* POSIX semaphore */
#include <errno.h>

using namespace std;

_ACS_BEGIN_NAMESPACE(acs) 

ACS_CLASS_DEFINE_DEBUG_LEVEL(MQTTDbSubscriber) ;

ThreadSafe MQTTDbSubscriber::_resourceMutex ;


/***************************************************************************************
**
                               toStr [STATIC]  
**
***************************************************************************************/
string MQTTDbSubscriber::toStr(const Enum_TimeWait& e)
{
	string s ; switch (e)
	{
		case WAIT_NONE:			{ s = "WAIT_NONE";		break ; }
		case WAIT_FAST:			{ s = "WAIT_FAST";		break ; }
		case WAIT_STD:			{ s = "WAIT_STD";		break ; }
		case WAIT_CT_FAST:		{ s = "WAIT_CT_FAST";	break ; }
		case WAIT_CT_STD:		{ s = "WAIT_CT_STD";	break ; }
		case WAIT_CT_SLOW:		{ s = "WAIT_CT_SLOW";	break ; }
		default:				{ s = "WAIT_UNKNOWN";	break ; }			
	}
	return s ;
}

string MQTTDbSubscriber::toStr(const Enum_TimeForcedHit& e)
{
	string s ; switch (e)
	{
		case FORCED_DISABLE:	{ s = "FORCED_DISABLE";	break ; }
		case FORCED_NOWAIT: 	{ s = "FORCED_NOWAIT";	break ; }
		case FORCED_FASTEST:	{ s = "FORCED_FASTEST";	break ; }
		case FORCED_FAST:		{ s = "FORCED_FAST";	break ; }
		case FORCED_STD:		{ s = "FORCED_STD";		break ; }
		case FORCED_SLOW:		{ s = "FORCED_SLOW";	break ; }
		case FORCED_SLOWEST:	{ s = "FORCED_SLOWEST";	break ; }
		case FORCED_20MSEC:		{ s = "FORCED_20MSEC";	break ; }
		case FORCED_1SEC:		{ s = "FORCED_1SEC";	break ; }
		case FORCED_10SEC:		{ s = "FORCED_10SEC";	break ; }
		default:				{ s = "FORCED_UNKNOWN";	break ; }			
	}
	return s ;
}


/***************************************************************************************
**
                               toEnum_TimeWait [STATIC]  
**
***************************************************************************************/
MQTTDbSubscriber::Enum_TimeWait MQTTDbSubscriber::toEnum_TimeWait(const string& s) 
{
	Enum_TimeWait e ; 
	if (s == "WAIT_NONE")			{ e = WAIT_NONE ;		}
	else if (s == "WAIT_FAST")		{ e = WAIT_FAST ;		}
	else if (s == "WAIT_STD")		{ e = WAIT_STD ;		}
	else if (s == "WAIT_CT_FAST") 	{ e = WAIT_CT_FAST ;	}
	else if (s == "WAIT_CT_STD") 	{ e = WAIT_CT_STD ;		}
	else if (s == "WAIT_CT_SLOW")	{ e = WAIT_CT_SLOW ;	}
	else							{ e = WAIT_UNKNOWN ;	}			
	return e ;
}


/***************************************************************************************
**
                             toEnum_TimeForcedHit [STATIC]  
**
***************************************************************************************/
MQTTDbSubscriber::Enum_TimeForcedHit MQTTDbSubscriber::toEnum_TimeForcedHit(const string& s) 
{
	Enum_TimeForcedHit e ; 
	if (s == "FORCED_DISABLE")		{ e = FORCED_DISABLE ;	}
	else if (s == "FORCED_NOWAIT") 	{ e = FORCED_NOWAIT ;	}
	else if (s == "FORCED_FASTEST")	{ e = FORCED_FASTEST ;	}
	else if (s == "FORCED_FAST")	{ e = FORCED_FAST ;		}
	else if (s == "FORCED_STD")		{ e = FORCED_STD ;		}
	else if (s == "FORCED_SLOW")	{ e = FORCED_SLOW ;		}
	else if (s == "FORCED_SLOWEST")	{ e = FORCED_SLOWEST ;	}
	else if (s == "FORCED_20MSEC")	{ e = FORCED_20MSEC ;	}
	else if (s == "FORCED_1SEC")	{ e = FORCED_1SEC ;		}
	else if (s == "FORCED_10SEC")	{ e = FORCED_10SEC ;	}
	else							{ e = FORCED_UNKNOWN ;	}			
	return e ;
}


/***************************************************************************************
**
                               toEnum_TimeForcedHit [STATIC]  
**
***************************************************************************************/
MQTTDbSubscriber::Enum_TimeForcedHit  MQTTDbSubscriber::toEnum_TimeForcedHit(const int64_t& v)
{
	/** Find the nearest enum to the provided value ... */
	
	Enum_TimeForcedHit chosen_enum ;
	
	if (v < 0)	
	{ chosen_enum = FORCED_DISABLE ; }
	else if (v == 0)	
	{ chosen_enum = FORCED_NOWAIT ; }
	else {

		Enum_TimeForcedHit	e ;
		uint64_t			diff, chosen_diff ; 

		// As first attempt, the last value is assumed ...
		e = FORCED_SLOWEST ;	diff = abs_diff(toMillisec(e), v) ; if (1 == 1)				{ chosen_enum = e ; chosen_diff = diff ; }

		// All the values are challenged.
		e = FORCED_FASTEST ;	diff = abs_diff(toMillisec(e), v) ; if (diff < chosen_diff)	{ chosen_enum = e ; chosen_diff = diff ; } 
		e = FORCED_FAST ;		diff = abs_diff(toMillisec(e), v) ; if (diff < chosen_diff)	{ chosen_enum = e ; chosen_diff = diff ; } 
		e = FORCED_STD ;		diff = abs_diff(toMillisec(e), v) ; if (diff < chosen_diff)	{ chosen_enum = e ; chosen_diff = diff ; } 
		e = FORCED_SLOW ;		diff = abs_diff(toMillisec(e), v) ; if (diff < chosen_diff)	{ chosen_enum = e ; chosen_diff = diff ; } 
		e = FORCED_SLOWEST ;	diff = abs_diff(toMillisec(e), v) ; if (diff < chosen_diff)	{ chosen_enum = e ; chosen_diff = diff ; } 
		e = FORCED_20MSEC ;		diff = abs_diff(toMillisec(e), v) ; if (diff < chosen_diff)	{ chosen_enum = e ; chosen_diff = diff ; } 
		e = FORCED_1SEC ;		diff = abs_diff(toMillisec(e), v) ; if (diff < chosen_diff)	{ chosen_enum = e ; chosen_diff = diff ; } 
		e = FORCED_10SEC ;		diff = abs_diff(toMillisec(e), v) ; if (diff < chosen_diff)	{ chosen_enum = e ; chosen_diff = diff ; } 
	}
			
	return chosen_enum ;
}


/***************************************************************************************
**
                               decode [STATIC]  
**
***************************************************************************************/
bool MQTTDbSubscriber::decode_time_wait(const Enum_TimeWait& e, bool &is_const_time, uint16_t &time_wait_ms)
{
	switch (e)
	{
		case WAIT_NONE:			{ is_const_time =  true ;	time_wait_ms =   0 ; break ; }	/* Note that since "no waiting" means "wait exactly 0", it's constant time */
		case WAIT_FAST:			{ is_const_time = false ;	time_wait_ms =  20 ; break ; }
		case WAIT_STD:			{ is_const_time = false ;	time_wait_ms = 100 ; break ; }
		case WAIT_CT_FAST:		{ is_const_time =  true ;	time_wait_ms =  20 ; break ; } 
		case WAIT_CT_STD:		{ is_const_time =  true ;	time_wait_ms = 100 ; break ; } 
		case WAIT_CT_SLOW:		{ is_const_time =  true ;	time_wait_ms = 250 ; break ; }
		default:				{ ACS_THROW(exIllegalValueException("Internal error, undefined Enum_TimeWait: " + toStr(e))) ; break ; }
	}

	return is_const_time ;
}


/***************************************************************************************
**
                               toMillisec [STATIC]  
**
***************************************************************************************/
uint64_t MQTTDbSubscriber::toMillisec(const Enum_TimeForcedHit& e)
{
	uint64_t v ; switch (e)
	{
		case FORCED_DISABLE:	{ v =     0 ;						break ; } /* Undefined value -- The wait should NOT be enforced by the caller */
		case FORCED_NOWAIT: 	{ v =     0 ;						break ; } /* No wait */
		case FORCED_FASTEST:	{ v =   100/*[msec]*/ *1 ;			break ; }
		case FORCED_FAST:		{ v =     5/*[sec]*/  *1000 ;		break ; }
		case FORCED_STD:		{ v =    30/*[sec]*/  *1000 ;		break ; }
		case FORCED_SLOW:		{ v =     5/*[min]*/  *60*1000 ;	break ; }
		case FORCED_SLOWEST:	{ v =    30/*[min]*/  *60*1000 ;	break ; }
		case FORCED_20MSEC:		{ v =    20/*[msec]*/ *1 ; 			break ; }
		case FORCED_1SEC:	 	{ v =  1000/*[msec]*/ *1 ; 			break ; }
		case FORCED_10SEC:	 	{ v = 10000/*[msec]*/ *1 ; 			break ; }
		default:				{ ACS_THROW(exIllegalValueException("Internal error, undefined Enum_TimeForcedHit: " + toStr(e))) ; break ; }			
	}
	return v ;
}


/***************************************************************************************
**
                               move [STATIC]  
**
***************************************************************************************/
void MQTTDbSubscriber::move(list <MQTTMessageHandler> &src, vector <MQTTMessageHandler> &dest) 
{
	dest.clear() ; 

	if (src.size() > 0) 
	{ 
		copy(src.begin(), src.end(), std::back_inserter(dest));
		src.clear() ; 
	}
}

void MQTTDbSubscriber::move(list <MQTTMessageHandler> &src, vector <MQTTMessageHandler> &dest, const ssize_t& n) 
{
	dest.clear() ; 

	if ( (n != 0) && (src.size() > 0) )
	{
		if ( (n<0) || (n >= static_cast<ssize_t>(src.size())) ) { move(src,dest) ; }
		else
		{
			list <MQTTMessageHandler>::iterator it_start (src.begin()) ; 			
			list <MQTTMessageHandler>::iterator it_stop (src.begin()) ; advance(it_stop, n) ;
			copy(it_start, it_stop, std::back_inserter(dest)); 
			src.erase(it_start, it_stop) ; 
		}
	}
}


/***************************************************************************************
**
                               compute_wait_time   
**
***************************************************************************************/
void MQTTDbSubscriber::compute_wait_time (const uint16_t& timeout_ms)  
{
	/*
		Obtain a time IN THE FUTURE, by adding an interval to the current time
	*/

	clock_gettime(CLOCK_REALTIME, &_ts) ;								// Get the current time

	_ts.tv_nsec += (timeout_ms * 1000000L /* from [ms] to [ns] */) ;	// Compute target time

	// Make up the nanoseconds, in order not to exceed 1s
	if      (_ts.tv_nsec >= 1999999999)	{ _ts.tv_sec += (_ts.tv_nsec / 1000000000) ; _ts.tv_nsec = (_ts.tv_nsec % 1000000000) ; /* General case */ }
	else if (_ts.tv_nsec >   999999999) { _ts.tv_nsec -= 1000000000 ; _ts.tv_sec ++ ; /* Within one second, avoid division, which is slow */ }

	return ;
}


/***************************************************************************************
**
                               buildConfiguration [STATIC] 
**
***************************************************************************************/
struct MQTTWorker::Configuration MQTTDbSubscriber::buildConfiguration(const string& host, const int& port) 
{
	/**
		Build a brand-new configuration, using only host and port.
		A random client-id is created.
	*/

	uint64_t client_id ; 
	ACS_SSL::ACS_RAND_bytes(reinterpret_cast<unsigned char*>(&client_id), sizeof(client_id)) ; 
	ostringstream o_client_id ; o_client_id << client_id ; 
	
	MQTTWorker::Configuration theConf;
	theConf.host = host ; 
	theConf.port = port;
	theConf.clientId = o_client_id.str() ; 

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		excerr << SimpleDebugSignatureThread << "Returning a brand-new configuration with this parameters: " << theConf.host << ":" << theConf.port 
			<< "  client-id: " << theConf.clientId << endl ;
	ACS_CLASS_END_DEBUG

	return theConf;
}


/***************************************************************************************
**
                               buildConfiguration [STATIC] 
**
***************************************************************************************/
struct MQTTWorker::Configuration MQTTDbSubscriber::buildConfiguration(const MQTTWorker::Configuration& confIn) 
{
	/**
		Build a brand-new configuration, using only host and port read from an input Configuration.
		A random client-id is created.
	*/

	uint64_t client_id ; 
	ACS_SSL::ACS_RAND_bytes(reinterpret_cast<unsigned char*>(&client_id), sizeof(client_id)) ; 
	ostringstream o_client_id ; o_client_id << client_id ; 
	
	MQTTWorker::Configuration theConf;
	theConf.host = confIn.host ; 
	theConf.port = confIn.port;
	theConf.clientId = o_client_id.str() ; 

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		excerr << SimpleDebugSignatureThread << "Returning a brand-new configuration with this parameters: " << theConf.host << ":" << theConf.port 
			<< "  client-id: " << theConf.clientId << endl ;
	ACS_CLASS_END_DEBUG

	return theConf;
}


struct MQTTWorker::Configuration MQTTDbSubscriber::buildConfiguration() 
{
	/**
		Build a void configuration, with host = "" and port = 0 ; 
	*/

	MQTTWorker::Configuration theConf;
	theConf.host = "" ; 
	theConf.port = 0;
	theConf.clientId = "" ; 

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		excerr << SimpleDebugSignatureThread << "Returning a void configuration" << endl ;
	ACS_CLASS_END_DEBUG

	return theConf;
}


/***************************************************************************************
**
                             MQTTDbSubscriber
**
***************************************************************************************/
MQTTDbSubscriber::MQTTDbSubscriber(const bool enable_hb, const std::string& topic, const struct MQTTWorker::Configuration& config, StopController const& stopController, const Enum_TimeWait& time_wait) :
	MQTTSubscriber (config.clientId, topic, config, stopController),
	_hb_subscriber_p(NULL),
	_hb_enabled(false),
	_messages_L(),
	_flag_report_is_halted(false),
	_flag_report_only_after_mesgs(-1),
	_flag_report_up_to_mesgs(-1),
	_flag_report_only_after_ms(-1),
	_flag_force_auto_hits(FORCED_DISABLE),
	_flag_force_next_hit(false),
	_timer(),
	_sem(),
	_ts(),
	_time_wait(time_wait),
	_timeout_fallback_auto_hits(FORCED_STD),
	_timeout_serverless_auto_hits(FORCED_10SEC),
	_self_toggle_auto_hits(false),
	_stopController(stopController)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB)
	
	bool is_connected ;

	if ( true == config.host.empty() )
	{
		// MQTT service is _NOT_ connected.
		is_connected = isConnected() ; 
		
		ostringstream o ; o << "Void MQTT DbSubscriber started. Topic: '" << topic << "'  TimeWait: " << toStr(_time_wait) << "  Connected to MQTT ? " << (is_connected? "TRUE" : "FALSE") << "  [Expected: FALSE]" ; 
		ACS_LOG_INFO(o.str()) ;//FIXME PROBABLY TOO VERBOSE 

		ACS_COND_THROW( (true == is_connected), exIllegalValueException("INTERNAL ERROR -- THE VOID MQTT SUBSCRIPTION SHOULD _NEVER_ BE CONNECTED") ) ; 
	}
	else
	{
		// Start the framework method of subscribing.
		// Method is automatically able to detected the status of the MQTT server.
		// NOTE: IT MIGHT THROW EXCEPTION !
		is_connected = activateFrameworkSubscription() ;

		ostringstream o ; o << "MQTT DbSubscriber started. Topic: '" << topic << "'  TimeWait: " << toStr(_time_wait) ;
		ACS_LOG_INFO(o.str()) ; //FIXME PROBABLY TOO VERBOSE 

#ifndef	MQTTDBSUBSCRIBER_PREVENT_HEARTBEAT
		if (true == enable_hb) { receive_heartbeats(config) ; }
#endif
	} 

	const int flag_inter_process (0) ; 		// False, since semaphore is _NOT_ inter-process
	const unsigned int initial_value (0) ;	// Empty when initialized
	ACS_COND_THROW ( (0 != sem_init (&_sem, flag_inter_process, initial_value)), exIllegalValueException("Fatal error, unable to initialize semaphore") ) ; 

	_timer.start() ; 	
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished. is_connected: " << (is_connected?"T":"F") << endl ;
	ACS_CLASS_END_DEBUG
}


/***************************************************************************************
**
                             ~MQTTDbSubscriber
**
***************************************************************************************/
MQTTDbSubscriber::~MQTTDbSubscriber() ACS_NOEXCEPT
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB)
	
	if ( (true == _hb_enabled) && (NULL != _hb_subscriber_p) )
	{
		delete _hb_subscriber_p ; // Which also stop the thread
		_hb_subscriber_p = NULL ; 
		_hb_enabled = false ;
	}

	sem_destroy (&_sem);
}

/***************************************************************************************
**
                             receive_heartbeats
**
***************************************************************************************/
void MQTTDbSubscriber::receive_heartbeats(const struct MQTTWorker::Configuration& config) 
{
	// Use default values
	const string hb_subsystem_id("SDF_01") ; 
	const string hb_entity("DbBroadcast_MQTT_Bridge") ; 
	const string hb_entity_id (hb_entity + "_01") ; 

	receive_heartbeats(config, hb_subsystem_id, hb_entity, hb_entity_id) ; 
}


/***************************************************************************************
**
                             receive_heartbeats
**
***************************************************************************************/
void MQTTDbSubscriber::receive_heartbeats(const struct MQTTWorker::Configuration& config, \
	const string& hb_subsystem_id, const string& hb_entity, const string& hb_entity_id) 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called." << endl ;
	ACS_CLASS_END_DEBUG

	ACS_COND_THROW ( ((true == _hb_enabled) || (NULL != _hb_subscriber_p)), exIllegalValueException("Heartbeat service already running") ) ;
	
  	_hb_subscriber_p = new MQTTHeartBeat::Subscriber(buildConfiguration(config), hb_subsystem_id, hb_entity, hb_entity_id, _stopController) ; 

	ACS_COND_THROW( (NULL == _hb_subscriber_p), exIllegalValueException("Invalid (NULL) pointer for MQTTHeartBeat::Subscriber") ) ; 

	_hb_enabled = true ; 
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
}


/***************************************************************************************
**
                             activateFrameworkSubscription
**
***************************************************************************************/
bool MQTTDbSubscriber::activateFrameworkSubscription()
{
	/**
		Call activateSubscription on the framework. The latter is able to cope with automatic connection/disconnection.
		This method MUST BE EXECUTED ONCE.
	*/

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called" << endl ;
	ACS_CLASS_END_DEBUG

	activateSubscription(MQTTWorker::QOS_0);
	
	const bool is_connected( isConnected() ) ;	// Even if _not_ connected the F/W will be able to detect the server 
												// is up and reconnect it.

	{
		ostringstream o ; o << SimpleDebugSignatureThread << "MQTT DbSubscriber started.  MQTT Server: " \
			<< getConfiguration().host << ":" << getConfiguration().port << "  Connected to MQTT ? " \
			<< (is_connected? "TRUE" : "FALSE") ; 
		ACS_LOG_INFO(o.str()) ; //FIXME PROBABLY TOO VERBOSE 
	} 

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished. is_connected: " << (is_connected?"T":"F") << endl ;
	ACS_CLASS_END_DEBUG

	return is_connected ; 
}


/***************************************************************************************
**
                             got_heartbeat
**
***************************************************************************************/
bool MQTTDbSubscriber::got_heartbeat()
{
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called."<< endl ;
	ACS_CLASS_END_DEBUG

	bool hb_valid (true) ; 
	
	if ( (true == _hb_enabled) && (NULL != _hb_subscriber_p) )
	{
		hb_valid = ( not _hb_subscriber_p->hb_is_expired() ) ; 
	}

	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished. Returning: " << (hb_valid?"T":"F") << endl ;
	ACS_CLASS_END_DEBUG
	
	return hb_valid ; 
}


/***************************************************************************************
**
                                  onConnect
**
***************************************************************************************/
void MQTTDbSubscriber::onConnect (int rc)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	const bool is_connected (true) ; // Since this is connect method

	ostringstream o ; o 
		<< "\n--------------------------------------------------------------------------------------" << endl \
		<< "CONNECTED TO MQTT Server: " << getConfiguration().host << ":" << getConfiguration().port << endl \
		<< "--------------------------------------------------------------------------------------\n" << endl ;
	ACS_LOG_INFO(o.str()) ;

	if (true == _self_toggle_auto_hits) { toggle_auto_hits(is_connected, got_heartbeat()) ; }
}


/***************************************************************************************
**
                                  onDisconnect
**
***************************************************************************************/
void MQTTDbSubscriber::onDisconnect (int rc)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	const bool is_connected (false) ; // Since this is disconnect method

	ostringstream o ; o 
		<< "\n--------------------------------------------------------------------------------------" << endl \
		<< "LOST CONNECTION TO MQTT Server: " << getConfiguration().host << ":" << getConfiguration().port << endl \
		<< "Until a new MQTT connection, no broadcast will be received !" << endl 
		<< "--------------------------------------------------------------------------------------\n" << endl ;
	ACS_LOG_WARNING(o.str()) ;

	if (true == _self_toggle_auto_hits) { toggle_auto_hits(is_connected, got_heartbeat()) ; }
}


/***************************************************************************************
**
                                  onMessage
**
***************************************************************************************/
void MQTTDbSubscriber::onMessage (const struct mosquitto_message *msg)
{
#ifndef DISABLE_MQTT
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		_timer.stop() ; excerr << SimpleDebugSignatureThread << "Time elapsed: " << _timer.elapsed() << "  stack size: " << _messages_L.size() 
			<< "  Received mosquitto_message of length: " << msg->payloadlen << endl ;
		if (msg->payloadlen > 0)
		{
			string mqtt_message( static_cast<char *>(msg->payload), msg->payloadlen ) ;
			excerr << SimpleDebugSignatureThread << "  Reporting is halted: " << (_flag_report_is_halted?"T":"F") << "  Message: \"" << mqtt_message << "\"" << endl ;
		}
	ACS_CLASS_END_DEBUG
					
	if ( (false == _flag_report_is_halted) && (msg->payloadlen > 0) )
	{
		string mqtt_message(static_cast<char *>(msg->payload), msg->payloadlen) ;
		MQTTMessageHandler mh (mqtt_message) ;  		
		
		{
			// DON'T MOVE FROM HERE:
			ThreadSafe::Lock lock(_resourceMutex) ;
			
			if (_messages_L.size() > (128*1024))
			{
				// If there are more than 128K UNREAD messages, probably the caller has some problem ... ...
				// To avoid overflooding, the whole stack is PURGED, so it will only contain the new message !
				_messages_L.clear() ; // Emergency clean !

				ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
					excerr << SimpleDebugSignatureThread << "Stack purged, since it was overwhelming !" << endl ;
				ACS_CLASS_END_DEBUG
			}

			/* Add message to the stack */			
			_messages_L.push_back( mh ) ; 	
		
			ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
				excerr << SimpleDebugSignatureThread << "Message just added: " << mh.dump() << endl ;
			ACS_CLASS_END_DEBUG
			
			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
				excerr << SimpleDebugSignatureThread << "Messages stacked (including this one): " << _messages_L.size() << endl ;
			ACS_CLASS_END_DEBUG
			
		}	// END: _resourceMutex
				
		// WHEN HERE, a nes massge has been added to the stack.
		// Accordingly to the conditions (e.g., report only after a certain time or number of messages) the semaphore is freed.

		if  ( true == check_report_event() ) 
		{
			// Free the semaphore, unless it's ALREADY available.
			
			int sem_value ; 
			ACS_COND_THROW ( (0 != sem_getvalue(&_sem, &sem_value)), exIllegalValueException("Fatal error, unable to read semaphore") ) ; 
		
			/* If counter unset (0 means unset, <0 means unset with user waiting) INCREASE the counter */
			if (sem_value <= 0) { ACS_COND_THROW ( (0 != sem_post(&_sem)), exIllegalValueException("Fatal error, unable to increase semaphore") ) ; }
		
		}	// END: if  ( true == check_report_event() )
				
	}	// END: if ( (false == _flag_report_is_halted) && (msg->payloadlen > 0) )

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
#endif
}


/***************************************************************************************
**
                               force_auto_hits  
**
***************************************************************************************/
bool MQTTDbSubscriber::force_auto_hits(const Enum_TimeForcedHit &timeout_fallback, const Enum_TimeForcedHit &timeout_serverless)
{
	/** Store the timeout for fallback/serverless and implement the correct value, accordingly to the connection status */
	
	_timeout_fallback_auto_hits		= timeout_fallback ;
	_timeout_serverless_auto_hits	= timeout_serverless ;

	const bool is_connected ( isConnected() ) ; toggle_auto_hits(is_connected, got_heartbeat()) ; 

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		excerr << SimpleDebugSignatureThread << "Method finished.  is_connected? " << (is_connected?"T":"F") << endl ;
	ACS_CLASS_END_DEBUG
	
	return is_connected ;
}


/***************************************************************************************
**
                               toggle_auto_hits  
**
***************************************************************************************/
void MQTTDbSubscriber::toggle_auto_hits(const bool is_connected, const bool hb_is_valid)
{
	/** Store the timeout for fallback/serverless and implement the correct value, accordingly to the connection status */
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method called.  is_connected? " << (is_connected?"T":"F") \
			<<"  HeartBeat valid: " << (hb_is_valid?"T":"F") << endl ;
	ACS_CLASS_END_DEBUG

	if ( (true == is_connected) && (true == hb_is_valid) )
	{
		// Real MQTT server connected:
		force_auto_hits(_timeout_fallback_auto_hits) ;		// Automatic hits (as fallback) ; 		
	}
	else
	{
		// No MQTT service running
		force_auto_hits(_timeout_serverless_auto_hits) ;	// Automatic hits (mandatory, since no broadcast is received)			
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << SimpleDebugSignatureThread << "Method finished" << endl ;
	ACS_CLASS_END_DEBUG
}


/***************************************************************************************
**
                               macro_fast_loop   
**
***************************************************************************************/
void MQTTDbSubscriber::macro_fast_loop()
{
	/** The scenario is a fast while-loop where high reliability is required */

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB)

	force_next_hit() ;									// Force the first pass
	force_auto_hits(FORCED_FAST, FORCED_FASTEST) ;		// Automatic hits (two timeout, accordingly to the connection status) ;
	self_toggle_auto_hits(true) ;						// Turn-on the self toggle of automatic timeout, whether the server is connected or no.
}


/***************************************************************************************
**
                               macro_standard_loop   
**
***************************************************************************************/
void MQTTDbSubscriber::macro_standard_loop()
{
	/** The scenario is a typical while-loop, monitoring one table for new items to appear */

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB)

	force_next_hit() ;									// Force the first pass
	force_auto_hits(FORCED_STD, FORCED_10SEC) ;		// Automatic hits (two timeout, accordingly to the connection status) ;
	self_toggle_auto_hits(true) ;						// Turn-on the self toggle of automatic timeout, whether the server is connected or no.
}


/***************************************************************************************
**
                               macro_slow_deamon  
**
***************************************************************************************/
void MQTTDbSubscriber::macro_slow_loop()
{
	/** The scenario is a low-performance loop (or a deamon) */
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB)

	force_next_hit() ;									// Force the first pass
	force_auto_hits(FORCED_SLOW, FORCED_STD) ;			// Automatic hits (two timeout, accordingly to the connection status) ;
	self_toggle_auto_hits(true) ;						// Turn-on the self toggle of automatic timeout, whether the server is connected or no.
}


/***************************************************************************************
**
                               check_report_event   
**
***************************************************************************************/
bool MQTTDbSubscriber::check_report_event()
{
	/**
		Assuming that the stack contains UNREAD messages, report them by applying conditions:
		- No condition (always report if at least one message is stacked)
		- Report only after a timeout relative to latest reporting
		- Report only if the stack contains a certain number of messages
	*/

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		excerr << SimpleDebugSignatureThread << "Method started."<< endl ;
	ACS_CLASS_END_DEBUG

	bool report_event (false) ;
	size_t stack_size(0) ;
	
	{
		ThreadSafe::Lock lock(_resourceMutex) ;
		stack_size = (_messages_L.size()) ; 
	}	// END: _resourceMutex
	
	if (stack_size > 0)
	{
		// This block is executed ONLY IF THERE ARE MESSAGES ON THE STACK -- o/w it's useless to execute it.
	
		/* No limitations over number of messages nor elapsed time (standard behavior) */
		if ( (_flag_report_only_after_ms < 0) && (_flag_report_only_after_mesgs < 0) ) { report_event = true ; }
		else
		{
			/* Report true if the expected number of messages to report has been reached (regardless the timer) */
			if (_flag_report_only_after_mesgs > -1) { report_event |= (static_cast<int64_t>(stack_size) >= _flag_report_only_after_mesgs) ; }

			/* Report true if the minimum time has been elapsed, and there are new messages */
			if ((_flag_report_only_after_ms > -1) && (true == _timer.isOlder(_flag_report_only_after_ms))) { report_event = true ; }
		}
		
	}	// END: if (stack_size > 0)

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		excerr << SimpleDebugSignatureThread << "Method finished. Returning: "<< (report_event?"T":"F") << endl ;
	ACS_CLASS_END_DEBUG

	return report_event ;
}


/***************************************************************************************
**
                               retrieveMessages   
**
***************************************************************************************/
bool MQTTDbSubscriber::retrieveMessages(std::vector <MQTTMessageHandler>& messages_V)
{
	/**
		Return TRUE accordingly to the condition of reporting.
		Also, if (and only if) reporting true, move the message stack to the caller.
	*/
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		_timer.stop() ; excerr << SimpleDebugSignatureThread << "Method started. Time elapsed: " << _timer.elapsed() << "  stack size: " << _messages_L.size() << endl ;
	ACS_CLASS_END_DEBUG

	bool b ;
		
	messages_V.clear() ;	// Clean the destination structure

	// First, a check on any timeout is expired (or the message stack is not empty)
	const bool reset (false) ; 
	size_t messages_IGNORE ; 
	if ( true == (b = gotMessages(messages_IGNORE, reset)) )
	{
		// DON'T MOVE FROM HERE:
		ThreadSafe::Lock lock(_resourceMutex) ;

		_timer.start() ;		// Timer is re-started

		// Only if reporting true, the messages are moved to the caller.
		if (_messages_L.size() > 0) { move(_messages_L, messages_V, _flag_report_up_to_mesgs) ; } /* If (flag == -1), all the messages will be moved  */


	}	// END: _resourceMutex
	
	if (true == b)
	{
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			_timer.stop() ; excerr << SimpleDebugSignatureThread << "Method finished. Returning: " << (b?"T":"F") << "  Size of vector: " << messages_V.size() << "  Time elapsed: " << _timer.elapsed() 
					<< "  stack size: " << _messages_L.size() << endl ;
		ACS_CLASS_END_DEBUG
	} else {
		ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
			_timer.stop() ; excerr << SimpleDebugSignatureThread << "Method finished. Returning: " << (b?"T":"F") << "  Size of vector: " << messages_V.size() << "  Time elapsed: " << _timer.elapsed() 
					<< "  stack size: " << _messages_L.size() << endl ;
		ACS_CLASS_END_DEBUG
	}

	return b ; 
}


/***************************************************************************************
**
                               retrieveAnyMessage   
**
***************************************************************************************/
void MQTTDbSubscriber::retrieveAnyMessage(std::vector <MQTTMessageHandler>& messages_V)
{
	/**
		Move any message, if any, to the stack of the caller. Regardless any condition of reporting.
	*/

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		_timer.stop() ; excerr << SimpleDebugSignatureThread << "Method started. Time elapsed: " << _timer.elapsed() << "  stack size: " << _messages_L.size() << endl ;
	ACS_CLASS_END_DEBUG
		
	messages_V.clear() ;	// Clean the destination structure

	{
		// DON'T MOVE FROM HERE:
		ThreadSafe::Lock lock(_resourceMutex) ;

		// If any message is present in the stack, move to the output (and also PURGE the internal stack)
		if (_messages_L.size() > 0) {
			 move(_messages_L, messages_V) ; 
			 _timer.start() ;		// Timer is re-started
		}
		
	}	// END: _resourceMutex

	if (messages_V.size() > 0)
	{
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			_timer.stop() ; excerr << SimpleDebugSignatureThread << "Method finished.  Size of vector: " << messages_V.size() << "  Time elapsed: " << _timer.elapsed() << "  stack size: " << _messages_L.size() << endl ;
		ACS_CLASS_END_DEBUG
	} else {
		ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
			_timer.stop() ; excerr << SimpleDebugSignatureThread << "Method finished.  Size of vector: " << messages_V.size() << "  Time elapsed: " << _timer.elapsed() << "  stack size: " << _messages_L.size() << endl ;
		ACS_CLASS_END_DEBUG
	}
}


/***************************************************************************************
**
                               gotMessages  
**
***************************************************************************************/
bool MQTTDbSubscriber::gotMessages(size_t& messages, const bool to_reset)
{
	/**
		Return TRUE accordingly to the condition of reporting.
		Note that TRUE might be returned even if no message is present ("forced hit") or FALSE might be returned even if there are messages on the stack (timeout not expired, e.g.)
	*/

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		_timer.stop() ; excerr << SimpleDebugSignatureThread << "Method started.  To Reset? " << (to_reset?"T":"F") << "  Connected: " << (isConnected()?"T":"F")
			<< "  Time elapsed: " << _timer.elapsed() << "  stack size: " << _messages_L.size() << endl ;
	ACS_CLASS_END_DEBUG
		
	/*
		Verify if the semaphore (which signals if one or more event has arrived) can be acquired during a given interval.
		If it cannot be acquired within the interval, it means that no event is available
		If it can be acquired, means that at least one event is available (note that when the semaphore is acquired, only a new event can release it).
		
		Note that, even if the semaphore is not available, meaning that no new event is stacked or has arrived during the interval, a check on the stack is performed.
		The point is that the stack MIGHT contain events, arrived before, which was not supposed to trigger when they arrived, but now are STALE.
		This might happen in case of flag: report_only_after_ms
		
		[*] The semaphore is released by method onMessage, whenever a new event is obtained by MQTT.
	*/
	
	int sem_acquired ; 
	messages = 0 ;

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		uint16_t timeout_ms ; if (true == decode_time_wait(timeout_ms))
		{ excerr << SimpleDebugSignatureThread << "Starting waiting cycle. Fixed timeout ms: " << timeout_ms << endl ; }
		else 
		{ excerr << SimpleDebugSignatureThread << "Starting waiting cycle. Dynamic timeout ms: " << timeout_ms << endl ; }
	ACS_CLASS_END_DEBUG
 
 	/* Used to force the frequent hits in case of disconnection / missing HB */
 	toggle_auto_hits(isConnected(), got_heartbeat()) ; 

	uint16_t timeout_ms ; 
	if (true == decode_time_wait(timeout_ms))	{ if (timeout_ms >= 1) { usleep(1000 * timeout_ms) ; } ; sem_acquired = sem_trywait(&_sem) ; }	/* Constant-time wait */
	else 										{ compute_wait_time(timeout_ms) ; sem_acquired = sem_timedwait(&_sem, &_ts) ; }					/* Dynamic-time wait */	
	
	bool success = (0 == sem_acquired) ; 

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		excerr << SimpleDebugSignatureThread << "Waiting cycle finished. Timeout ms: " << timeout_ms << "  success: " << (success?"T":"F") << endl ;
	ACS_CLASS_END_DEBUG
 
	// Even if the semaphore has not been acquired (no new events arrived prior or during the observation period, check if the stack is stale:
	if ( (false == success) && (_flag_report_only_after_ms > -1)) { success |= check_report_event() ; }
	// Check if a simulated hit should be issued:
	if ( (false == success) && (true == _flag_force_next_hit) ) { success = true ; } 			/* Forced */
	if ( (false == success) && (FORCED_NOWAIT == _flag_force_auto_hits) ) { success = true ; }	/* Forced */
	if ( (false == success) && (FORCED_DISABLE != _flag_force_auto_hits) && (true == _timer.isOlder(toMillisec(_flag_force_auto_hits))) ) { success = true ; }
	
	if (true == success) 
	{
		// IF HERE: At least one message to report [ or a simulated hit ] is available
		messages = _messages_L.size() ;
	
		if (true == to_reset)
		{
			// DON'T MOVE FROM HERE:
			ThreadSafe::Lock lock(_resourceMutex) ;
			_timer.start() ;		// Timer is re-started
			_messages_L.clear() ;	// Message stack is purged
		}	// END: _resourceMutex

		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			_timer.stop() ; excerr << SimpleDebugSignatureThread << "Method finished. Returning: " << (success?"T":"F") << "  Time elapsed: " << _timer.elapsed() << "  stack size: " << _messages_L.size() << endl ;
		ACS_CLASS_END_DEBUG
	}
	else
	{
		ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
			_timer.stop() ; excerr << SimpleDebugSignatureThread << "Method finished. Returning: " << (success?"T":"F") << "  Time elapsed: " << _timer.elapsed() << "  stack size: " << _messages_L.size() << endl ;
		ACS_CLASS_END_DEBUG
	}
	
	_flag_force_next_hit = false ; // Disable this, in case it was set BEFORE this call.
	
	return success ; 
}



_ACS_END_NAMESPACE
