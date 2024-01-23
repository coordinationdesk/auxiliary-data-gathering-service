// PRQA S 1050 EOF
/*
   Copyright 2019-, Exprivia SpA
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Advanced Computer Systems, Inc. 

   MQTTMessageHandler Message Handler
   
   $Log$
   
*/ 

#include <MQTTMessageHandler.h>
#include <exException.h>
#include <DbMessageHandlerFactory.h> /* Only needed for the self-test */

using namespace std;
_ACS_BEGIN_NAMESPACE(acs) 

ACS_CLASS_DEFINE_DEBUG_LEVEL(MQTTMessageHandler);


/***************************************************************************************
**
                             MQTTMessageHandler
**
***************************************************************************************/
MQTTMessageHandler::MQTTMessageHandler() :
	JsonSerializable(JsonSerializable::SF_TEXT),
	timestamp_(0),
	counter_(0),
	user_(),
	process_(),
	database_(),
	subsystem_(),
	table_(),
	action_(),   
	pid_(0),
	affected_rows_(0),
	id_vector_()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
}


/***************************************************************************************
**
                             MQTTMessageHandler
**
***************************************************************************************/
MQTTMessageHandler::MQTTMessageHandler(const string& mqtt_message) :
	JsonSerializable(JsonSerializable::SF_TEXT),
	timestamp_(0),
	counter_(0),
	user_(),
	process_(),
	database_(),
	subsystem_(),
	table_(),
	action_(),   
	pid_(0),
	affected_rows_(0),
	id_vector_()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	MQTTMessageHandler::unserialize(mqtt_message) ;
}


/***************************************************************************************
**
                             MQTTMessageHandler
**
***************************************************************************************/
MQTTMessageHandler::MQTTMessageHandler(const DbMessageHandler* db_message_ptr) :
	JsonSerializable(JsonSerializable::SF_TEXT),
	timestamp_(0),
	counter_(0),
	user_(),
	process_(),
	database_(),
	subsystem_(),
	table_(),
	action_(),   
	pid_(0),
	affected_rows_(0),
	id_vector_()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	unserialize_from_db_message(db_message_ptr) ;	/* Import a DB Message */
}


/***************************************************************************************
**
                             ~MQTTMessageHandler
**
***************************************************************************************/
MQTTMessageHandler::~MQTTMessageHandler() ACS_NOEXCEPT
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
}


/***************************************************************************************
**
                             unserialize
**
***************************************************************************************/
void MQTTMessageHandler::unserialize(const string& message) 
{
	JsonSerializable::unserialize(message) ; //  Call parent method (fill the JsonSerializable container)

	load_container() ; 
}


/***************************************************************************************
**
                             unserialize_from_db_message
**
***************************************************************************************/
void MQTTMessageHandler::unserialize_from_db_message(const DbMessageHandler* db_message_ptr) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	/**
		Generate the message payload starting from a filled DbMessageHandler
	*/	
	add_pair( "timestamp",		db_message_ptr->get_timestamp() ) ;  		
	add_pair( "counter",		db_message_ptr->get_counter() ) ;  			
	add_pair( "user",			db_message_ptr->get_user() ) ;				 
	add_pair( "process",		db_message_ptr->get_process() ) ;  			
	add_pair( "database",		db_message_ptr->get_database() ) ;			
	add_pair( "subsystem",		db_message_ptr->get_subsystem() ) ;  		
	add_pair( "table",			db_message_ptr->get_table() ) ;  			  
	add_pair( "action",			db_message_ptr->get_action() ) ;			   
	add_pair( "pid",			db_message_ptr->get_pid() ) ;  				
	add_pair( "affected_rows",	db_message_ptr->get_affected_rows() ) ;  				
	add_pair( "id",				db_message_ptr->get_id_vector() ) ;     

	load_container() ;        
}


/***************************************************************************************
**
                             load_container
**
***************************************************************************************/
void MQTTMessageHandler::load_container()
{
	/**
		Fill the local container
	*/	

	if (true != is_null("timestamp"))		{ get_value("timestamp", timestamp_) ; }			else { timestamp_ = 0 ; }
	if (true != is_null("counter"))			{ get_value("counter",   counter_) ; }				else { counter_ = 0 ; }
	if (true != is_null("user"))			{ get_value("user", 	 user_) ; }					else { user_ = "" ; }
	if (true != is_null("process"))			{ get_value("process",   process_) ; }				else { process_ = "" ; }
	if (true != is_null("database"))		{ get_value("database",  database_) ; }				else { database_ = "" ; } 
	if (true != is_null("subsystem"))		{ get_value("subsystem", subsystem_) ; }			else { subsystem_ = "" ; } 
	if (true != is_null("table"))			{ get_value("table", 	 table_) ; }				else { table_ = "" ; } 
	if (true != is_null("action"))			{ get_value("action", 	 action_) ; } 				else { action_ = "" ; }
	if (true != is_null("pid"))				{ get_value("pid",  	 pid_) ; }					else { pid_ = 0 ; }
	if (true != is_null("affected_rows"))	{ get_value("affected_rows", affected_rows_) ; }	else { affected_rows_ = 0 ; }
	if (true != is_null("id"))				{ get_value("id", 		 id_vector_) ; }			else { id_vector_.clear() ; }
}


/***************************************************************************************
**
                             dump
**
***************************************************************************************/
string MQTTMessageHandler::dump() const
{
	ostringstream v ; v << "[ " ; for (size_t i=0; i<id_vector_.size(); i++) { v << ((i==0)? "": " ,") << id_vector_.at(i) ; } ; v << " ]" ; 
	
	ostringstream o ; o
		<< "timestamp: " << timestamp_ << "\n" \
		<< "counter: " << counter_ << "\n" \
		<< "user: \"" << user_ << "\"\n" \
		<< "process: \"" << process_ << "\"\n" \
		<< "database: \"" <<  database_ << "\"\n" \
		<< "subsystem: \"" << subsystem_ << "\"\n" \
		<< "table: \"" << table_ << "\"\n" \
		<< "action: \"" <<  action_ << "\"\n" \
		<< "pid: " << pid_ << "\n" \
		<< "affected_rows: " << affected_rows_ << "\n" \
		<< "id: " << v.str() << endl ;

	return o.str() ; 
}						


/***************************************************************************************
**
                             self_test
**
***************************************************************************************/
bool MQTTMessageHandler::self_test()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	bool status (false) ; 
	excerr << "About to run Self test for MQTTMessageHandler base class" << endl ;
		
	vector <uint64_t> exp_v ; exp_v.push_back(33) ; exp_v.push_back(44) ; exp_v.push_back(55) ; 

	{
		// Test 1/2, using a JSON as MQTT input
		
		ostringstream mqtt_message ;
		mqtt_message \
			<< "{ " \
			<<		"\"timestamp\": " << DbMessageHandler::DateTime_to_timestamp(DateTime("UTC=20121110T090807.654321")) << ", " \
			<<		"\"counter\": " << 112244 << ", " \
			<<		"\"user\":      \"USER\", " \
			<<		"\"process\":   \"PROCESS\", " \
			<<		"\"database\":  \"DATABASE\", " \
			<<		"\"subsystem\": \"SUBSYSTEM\", " \
			<<		"\"table\":     \"TABLE\", " \
			<<		"\"action\":    \"ACTION\", " \
			<<		"\"pid\":    	1234, " \
			<<		"\"affected_rows\": 3, " \
			<<		"\"id\":    	[33, 44, 55]"
			<< " }" ;


		MQTTMessageHandler p(mqtt_message.str()) ; 	
		
		if (405853687654321LL != p.get_timestamp())	{ ostringstream o ; o << "Mismatching value when comparing timestamp: " << p.get_timestamp() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if ("2012-11-10T09:08:07.654321" != DbMessageHandler::timestamp_to_DateTime(p.get_timestamp()).asISO8601(6)) \
													{ ostringstream o ; o << "Mismatching value when comparing timestamp: " << p.get_timestamp() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if (112244 !=      p.get_counter())			{ ostringstream o ; o << "Mismatching value when comparing counter: " << p.get_counter() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if ("USER" !=      p.get_user())			{ ostringstream o ; o << "Mismatching value when comparing user: " << p.get_user() ; ACS_THROW (exIllegalValueException(o.str())) ; }		    		 
		if ("PROCESS" !=   p.get_process())			{ ostringstream o ; o << "Mismatching value when comparing process: " << p.get_process() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if ("DATABASE" !=  p.get_database())		{ ostringstream o ; o << "Mismatching value when comparing database: " << p.get_database() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if ("SUBSYSTEM" != p.get_subsystem())		{ ostringstream o ; o << "Mismatching value when comparing subsystem: " << p.get_subsystem() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if ("TABLE" !=     p.get_table())			{ ostringstream o ; o << "Mismatching value when comparing table: " << p.get_table() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if ("ACTION" !=    p.get_action())			{ ostringstream o ; o << "Mismatching value when comparing action: " << p.get_action() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if (1234 !=        p.get_pid())				{ ostringstream o ; o << "Mismatching value when comparing pid: " << p.get_pid() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if (3 !=           p.get_affected_rows())	{ ostringstream o ; o << "Mismatching value when comparing affected_rows: " << p.get_affected_rows() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if (exp_v !=       p.get_id_vector())		{ ostringstream o ; o << "Mismatching value when comparing id_vector: " ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
	}

	{
		// Test 2/2. Full test, using even the factory. A specific postgres test message is generated.
		ostringstream postgres_db_message ;
		postgres_db_message \
			<< "{ " \
			<< "\"driver_name\": " << "\"Postgres8\"" << ", " \
			<< "\"channel\": " << "\"CHANNEL_NAME\"" << ", " \
			<< "\"counter\": " << 112244 << ", " \
			<< "\"timestamp\": " << DbMessageHandler::DateTime_to_timestamp("UTC=20121110T090807.654321") << ", " \
			<<		"\"payload\": { " \
			<<				"\"serverip\":  \"SERVER_IP\", " \
			<<				"\"clientip\":  \"CLIENT_IP\", " \
			<<				"\"user\":      \"USER\", " \
			<<				"\"process\":   \"PROCESS\", " \
			<<				"\"database\":  \"DATABASE\", " \
			<<				"\"subsystem\": \"SUBSYSTEM\", " \
			<<				"\"table\":     \"TABLE\", " \
			<<				"\"action\":    \"ACTION\", " \
			<<				"\"pid\":    	1234, " \
			<<				"\"affectedrows\":  3, " \
			<<				"\"id\":    	\"33 44 55\"" /* Note the QUOTES and spaces, ("x y" instead of [x,y]) to correctly simulate the WRONG return from postgres */ \
			<< 		" }" \
			<< " }" ;

		/* A dbmessage handler is generated, using the message above (postgres-specific) */
		DbMessageHandler* db_message_handler_ptr = DbMessageHandlerFactory::instance(postgres_db_message.str()) ;
		
		ACS_COND_THROW( (NULL == db_message_handler_ptr), exIllegalValueException("NULL POINTER for _b_message_handler_ptr") ) ; 
		
		// The mqtt message handler is created, and the values contained are verified. 
		MQTTMessageHandler p(db_message_handler_ptr) ;
		
		if (405853687654321LL != p.get_timestamp())	{ ostringstream o ; o << "Mismatching value when comparing timestamp: " << p.get_timestamp() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if ("2012-11-10T09:08:07.654321" != DbMessageHandler::timestamp_to_DateTime(p.get_timestamp()).asISO8601(6)) \
													{ ostringstream o ; o << "Mismatching value when comparing timestamp: " << p.get_timestamp() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if (112244 !=      p.get_counter())			{ ostringstream o ; o << "Mismatching value when comparing counter: " << p.get_counter() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if ("USER" !=      p.get_user())			{ ostringstream o ; o << "Mismatching value when comparing user: " << p.get_user() ; ACS_THROW (exIllegalValueException(o.str())) ; }		    		 
		if ("PROCESS" !=   p.get_process())			{ ostringstream o ; o << "Mismatching value when comparing process: " << p.get_process() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if ("DATABASE" !=  p.get_database())		{ ostringstream o ; o << "Mismatching value when comparing database: " << p.get_database() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if ("SUBSYSTEM" != p.get_subsystem())		{ ostringstream o ; o << "Mismatching value when comparing subsystem: " << p.get_subsystem() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if ("TABLE" !=     p.get_table())			{ ostringstream o ; o << "Mismatching value when comparing table: " << p.get_table() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if ("ACTION" !=    p.get_action())			{ ostringstream o ; o << "Mismatching value when comparing action: " << p.get_action() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if (1234 !=        p.get_pid())				{ ostringstream o ; o << "Mismatching value when comparing pid: " << p.get_pid() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if (3 !=           p.get_affected_rows())	{ ostringstream o ; o << "Mismatching value when comparing affected_rows: " << p.get_affected_rows() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
		if (exp_v !=       p.get_id_vector())		{ ostringstream o ; o << "Mismatching value when comparing id_vector: " ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
	}

	// IF HERE: TEST IS SUCCESSFULLY
	status = true ; 

	excerr << "Self test for MQTTMessageHandler base class finished: " << (status? "SUCCESS":"FAILURE") << endl ;
		
	return status ; 	
}

_ACS_END_NAMESPACE
