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

#ifndef _MQTTMessageHandler_H_
#define _MQTTMessageHandler_H_

#include <JsonSerializable.h>
#include <DbMessageHandler.h>
#include <DateTime.h>
#include <exException.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs) 

class MQTTMessageHandler : public JsonSerializable
{
	/**
		This class handles the payload of the MQTT messages.
		The class can be used either with getters/setters, or, more likely, with a filled DbMessageHandler (see the specific c'tor)
		The point of the DbMessageHandler c'tor is to create a MQTT message directly with the DbMessageHandler, filled by the specific dialect-based handler.
	*/

public:
	MQTTMessageHandler() ;
	MQTTMessageHandler(const std::string& mqtt_message) ;			/* Import (unserialize) a MQTT message */
	MQTTMessageHandler(const DbMessageHandler* db_message_ptr) ;	/* Import (unserialize) a db message */
	virtual ~MQTTMessageHandler() ACS_NOEXCEPT;

	/**
		Getters for the message elements
	*/
	inline uint64_t get_timestamp() const						{ return timestamp_ ; }      	  
	inline uint64_t get_counter() const							{ return counter_ ; }        	  
	inline std::string get_user() const							{ return user_ ; }	        	  
	inline std::string get_process() const						{ return process_ ; }        	  
	inline std::string get_database() const						{ return database_ ; }       	  
	inline std::string get_subsystem() const					{ return subsystem_ ; }        
	inline std::string get_table() const						{ return table_ ; }	          
	inline std::string get_action() const						{ return action_ ; }         	  
	inline uint64_t get_pid() const								{ return pid_ ; }	          
	inline ssize_t get_affected_rows() const					{ return affected_rows_ ; }	          
	inline std::vector<uint64_t> get_id_vector() const			{ return id_vector_ ; } 
	
public:
	
	/**
		self test
	*/
	static bool self_test() ; 
	
	/** Method overloaded */
	virtual void unserialize(const std::string& blob) ;	

	/** Method overloaded */
	virtual std::string dump() const ;							

private:
	
	/**
		Generate the message payload starting from a filled DbMessageHandler
	*/	
	void unserialize_from_db_message(const DbMessageHandler* db_message_ptr) ; 
	
	/**
		Fill the message container
	*/	
	void load_container() ;

private:
	
	uint64_t timestamp_ ;
	uint64_t counter_ ;
	std::string user_ ;
	std::string process_ ;
	std::string database_ ;
	std::string subsystem_ ;
	std::string table_ ;
	std::string action_ ;
	uint64_t pid_ ;
	ssize_t affected_rows_ ;
	std::vector<uint64_t> id_vector_ ;
		
	ACS_CLASS_DECLARE_DEBUG_LEVEL(MQTTMessageHandler);

};

_ACS_END_NAMESPACE

#endif //_MQTTMessageHandler_H_
	
