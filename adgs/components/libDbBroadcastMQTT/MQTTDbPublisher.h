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

   MQTT DbPublisher header file
*/ 

#ifndef _MQTTDbPublisher_H_
#define _MQTTDbPublisher_H_

#include <MQTTDbTopicGenerator.h>
#include <MQTTPublisher.h>
#include <dbConnectionPool.h>
#include <SQLDialectFactory.h>
#include <SQLString.h>
#include <SQLDialect.h>
#include <StopController.h>
#include <DbMessageHandler.h>


_ACS_BEGIN_NAMESPACE(acs) 

class MQTTDbPublisher : public MQTTPublisher
{
public:
	MQTTDbPublisher(const std::string& name, const struct MQTTPublisher::Configuration& config, dbConnection &conn, 
					StopController const& stopController = StopController()) ;
	virtual ~MQTTDbPublisher() ACS_NOEXCEPT;

	/**
		Start (once) the db listener
	*/
	int start_dblistener(const std::string& channel) ;
	
	/**
		Listen to db message and publish MQTT messages (loop)
	*/
	void run_publisher() ;


	/**
		Getters:
	*/
	inline std::string get_topic_organization() const			{ return _topic_generator.get_topic_organization() ; }  
	inline std::string get_topic_subsystem() const				{ return _topic_generator.get_topic_subsystem() ; }  
	inline std::string get_topic_subsystem_id() const			{ return _topic_generator.get_topic_subsystem_id() ; }
	inline std::string get_topic_entity() const					{ return _topic_generator.get_topic_entity() ; }  
	inline std::string get_topic_entity_id() const				{ return _topic_generator.get_topic_entity_id() ; }

	/**
		Setters:
	*/
	inline void set_topic_organization(const std::string& c )	{ _topic_generator.set_topic_organization(c) ; }  
	inline void set_topic_subsystem(const std::string& c )		{ _topic_generator.set_topic_subsystem(c) ; }  
	inline void set_topic_subsystem_id(const std::string& c )	{ _topic_generator.set_topic_subsystem_id(c) ; }  
	inline void set_topic_entity(const std::string& c )			{ _topic_generator.set_topic_entity(c) ; }  
	inline void set_topic_entity_id(const std::string& c )		{ _topic_generator.set_topic_entity_id(c) ; }  

private:

	/**
		Given a message from DB, feed the DbMessageHandler.
	*/
	void fill_db_message_handler(const std::string& db_message_in) ;

	/**
		Turn a db message (stored in the member DbMessageHandler) to a MQTT topic+message
	*/
	void format_mqtt_message(std::string& mqtt_topic_out, std::string& mqtt_message_out) ; 
	
	/**
		Call startPublishing on the framework. The latter is able to cope with automatic connection/disconnection.
		This method MUST BE EXECUTED ONCE.
	*/
	bool startFrameworkPublishing() ;

	// Disable copy
	MQTTDbPublisher (const MQTTDbPublisher&);
	MQTTDbPublisher& operator= (const MQTTDbPublisher&);


private:
	dbConnection&			_connection ;			/* Just a single connection is used */
	MQTTDbTopicGenerator	_topic_generator ;
	DbMessageHandler*		_db_message_handler ; 
	StopController const &	_stopController ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(MQTTDbPublisher) ;


};

_ACS_END_NAMESPACE

#endif //_MQTTDbPublisher_H_
