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

   MQTT DbPublisher
*/ 

#include <MQTTDbTopicGenerator.h>
#include <DbMessageHandlerFactory.h>
#include <MQTTDbPublisher.h>
#include <Filterables.h>
#include <Application.h>
#include <StopController.h>
#include <MQTTMessageHandler.h>

#include <Timer.h>

using namespace std;

_ACS_BEGIN_NAMESPACE(acs) 

ACS_CLASS_DEFINE_DEBUG_LEVEL(MQTTDbPublisher) ;
	

/***************************************************************************************
**
                             MQTTDbPublisher
**
***************************************************************************************/
MQTTDbPublisher::MQTTDbPublisher(	const string& name, const struct MQTTPublisher::Configuration& config, dbConnection& conn,
									StopController const& stopController) :
	MQTTPublisher (name, config, stopController),
	_connection(conn),
	_topic_generator(conn),
	_db_message_handler(NULL),
	_stopController(stopController)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	// Start the framework method of publishing.
	// Method is automatically able to detected the status of the MQTT server.
	// NOTE: IT MIGHT THROW EXCEPTION !
	const bool is_connected ( startFrameworkPublishing() ) ;
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished. is_connected: " << (is_connected?"T":"F") << endl ;
	ACS_CLASS_END_DEBUG
}


/***************************************************************************************
**
                             ~MQTTDbPublisher
**
***************************************************************************************/
MQTTDbPublisher::~MQTTDbPublisher() ACS_NOEXCEPT
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB)

	if (NULL != _db_message_handler) 
	{
		delete _db_message_handler ; 
		_db_message_handler = NULL ; 
	}
}


/***************************************************************************************
**
                             startFrameworkPublishing
**
***************************************************************************************/
bool MQTTDbPublisher::startFrameworkPublishing()
{
	/**
		Call startPublishing on the framework. The latter is able to cope with automatic connection/disconnection.
		This method MUST BE EXECUTED ONCE.
	*/
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called" << endl ;
	ACS_CLASS_END_DEBUG

	startPublishing();

	const bool is_connected( isConnected() ) ; 

	{
		ostringstream o ; o << SimpleDebugSignatureThread << "MQTT DbPublisher started. MQTT Server: " \
			<< getConfiguration().host << ":" << getConfiguration().port << "  Connected to MQTT ? " \
			<< (is_connected? "TRUE" : "FALSE") ; 
		ACS_LOG_INFO(o.str()) ;
	} 

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished. is_connected: " << (is_connected?"T":"F") << endl ;
	ACS_CLASS_END_DEBUG

	return is_connected ; 
}


/***************************************************************************************
**
                             fill_db_message_handler
**
***************************************************************************************/
void MQTTDbPublisher::fill_db_message_handler(const string& db_message_in) 
{
	/**
		Given a message from DB, feed the DbMessageHandler (factory).
		AT THE FIRST CALL, THE FACTORY WILL BE INVOKED WITH THE PROPER ENGINE,
		EXTRACTED BY METADATA IN THE MESSAGE ITSELF.
	*/

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB)

	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) 
		/* This is the only message printed in MID verbosity */
		excerr << SimpleDebugSignatureThread << "Message received by db (as string): " << db_message_in << endl ;
	ACS_CLASS_END_DEBUG

	// Check if the handler has been already created (when the FIRST message has been processed)
	if (NULL == _db_message_handler)
	{
		// Allocate a specific handler, using the "driver_name" contained in the encapsulated JSON message
		_db_message_handler = DbMessageHandlerFactory::instance(db_message_in) ;
		ACS_COND_THROW( (NULL == _db_message_handler), exIllegalValueException("NULL POINTER for _db_message_handler") ) ; 
		
		ostringstream o ; o << "Create an instance of DbMessageHandler for driver: \"" << _db_message_handler->get_driver_name() << "\"" ; 
		ACS_LOG_INFO(o.str()) ; 
	}
	else
	{
		// The specific handler has been already allocated, just feed the message
		_db_message_handler->unserialize(db_message_in) ; 
	}
	
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished. DbMessageHandler filled successfully: " << _db_message_handler->dump() << endl ;
	ACS_CLASS_END_DEBUG
}
	

/***************************************************************************************
**
                             format_mqtt_message
**
***************************************************************************************/
void MQTTDbPublisher::format_mqtt_message(string& topic_out, string& message_out) 
{
	/**
		Turn a db message (already stored in the member DbMessageHandler) to a MQTT topic+message
	*/
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB)

	// Load the MQTT message handler, using the already-filled DbMessageHandler
	MQTTMessageHandler mqtt_message_handler ( _db_message_handler ) ; 

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
		excerr << SimpleDebugSignatureThread << "MQTT Converted Message: " << mqtt_message_handler.dump() << endl ;
	ACS_CLASS_END_DEBUG

	topic_out = _topic_generator.generate_topic( _db_message_handler->get_table(), _db_message_handler->get_action() ) ;
	message_out = mqtt_message_handler.serialize() ; 
	
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		/* This is the only message printed in LOW verbosity */
		excerr << SimpleDebugSignatureThread << "MQTT message created. Topic: \"" << topic_out << "\"  JSON: " << message_out << endl ;
	ACS_CLASS_END_DEBUG

		
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
}


/***************************************************************************************
**
                             start_dblistener
**
***************************************************************************************/
int MQTTDbPublisher::start_dblistener(const string& channel)
{
	/**
		Start (once) the db listener. 
		NOTE THAT THE CONNECTION ON WHICH THE LISTENER IS STARTED (which just emits the SQL command: LISTEN <CHANNEL>)
		MUST BE THE SAME USED TO GET MESSAGES FROM THE LISTENER
	*/
	
    const string& dbKey ( _connection.getKey() ) ;

	// Start listening to the current channel:
	vector <string> channels_V ; StringUtils::Tokenize(channel, channels_V, " ") ;
	for (size_t i=0; i<channels_V.size() ; i++)
	{
		ostringstream o ; o << "Listening to channel: \"" << channels_V.at(i) << "\"" ;
		ACS_LOG_INFO(o.str()) ; 
		SQLDialectFactory::sqlDialect(dbKey).startListener( &_connection, channels_V.at(i) ) ;
	}	

	return channels_V.size() ;
}


/***************************************************************************************
**
                             run_publisher
**
***************************************************************************************/
void MQTTDbPublisher::run_publisher() 
{
	/**
		Listen to db message and publish MQTT messages (loop)
		NOTE THAT THE CONNECTION ON WHICH THE LISTENER HAS BEEN STARTED (which just emits the SQL command: LISTEN <CHANNEL>)
		MUST BE THE SAME USED TO GET MESSAGES FROM THE LISTENER
	*/

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB)
	
    const string& dbKey ( _connection.getKey() ) ;

	const uint64_t half_sec (500000) ;
	std::vector <string> db_messages_V ;


	while ( false == _stopController.requestedCancel() ) 
	{
		db_messages_V.clear() ; 
		// Wait for at maximum 1/2 second. Or less if one message is returned.
		if ( true == SQLDialectFactory::sqlDialect(dbKey).listenToMessages( &_connection, db_messages_V, half_sec ) )
		{
			// The db listener returned at least one message
			ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
				excerr << SimpleDebugSignatureThread << "Received: " << db_messages_V.size() << " messages from DB" << endl ;
			ACS_CLASS_END_DEBUG
		
			for (size_t i=0; i<db_messages_V.size(); i++)
			{
				ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
					excerr << SimpleDebugSignatureThread << "Processing message [1-rel] : " << (i+1) << "/" << db_messages_V.size() << endl ;
				ACS_CLASS_END_DEBUG
	
				// Extract (generate) the topic and the message, given the received db_message
				string mqtt_topic, mqtt_message ; 
				
				// 1/2: Feed the member DbMessageHandler with the db_message. The first time, initialize the factory, too.
				fill_db_message_handler(db_messages_V.at(i)); 
				
				ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) 
					if (0 == i) {
						ostringstream o ; o << "\n" ; if (db_messages_V.size() > 1) 
						{ o << "Received " << db_messages_V.size() << " messages from db. First message: " ;}
						else { o << "Received one message from db: " ; }
						o << _db_message_handler->short_dump() ; ACS_LOG_INFO(o.str()) ;
					}
				ACS_CLASS_END_DEBUG

				// 2/2: Turn a db message (stored in the member DbMessageHandler) to a MQTT topic+message
				format_mqtt_message(mqtt_topic, mqtt_message) ; 
				
				if ( (false == mqtt_topic.empty()) && (false == mqtt_message.empty()) )
				{
					// WHEN HERE: The pair { mqtt_topic, mqtt_message } has been generated

					// Publish the topic/message
					setTopic(mqtt_topic);
					try {
						publishText(mqtt_message, MQTTWorker::QOS_0);
					} 
					catch (MQTTWorker::ConnectionError& e)
					{ 
						ACS_LOG_WARNING ( "MQTT CONNECTION ERROR. Unable to publish the message: \"" << mqtt_message << "\"" << endl << e.what() ) ;  
					}
					catch (exception& e)
					{ 
						ACS_LOG_WARNING ( "MQTT GENERIC ERROR. Unable to publish the message: \"" << mqtt_message << "\"" << endl << e.what() ) ;  
					}
				}
			}
		}	// END: if ( true == SQLDialectFactory::sqlDialect(dbKey).listenToMessages ...)
		
	}	// END: while ( false == stopController.requestedCancel() ) 
	
	ACS_LOG_INFO ("MQTTDbPublisher::run_publisher: quitting the main loop") ; 
}

_ACS_END_NAMESPACE
