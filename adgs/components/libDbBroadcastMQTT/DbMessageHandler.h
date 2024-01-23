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

   DbMessageHandler header file
*/ 

#ifndef _DbMessageHandler_H_
#define _DbMessageHandler_H_

#include <JsonSerializable.h>
#include <DateTime.h>
#include <exException.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs) 

class DbMessageHandler : public JsonSerializable
{

public:
	DbMessageHandler() ;
	DbMessageHandler(const std::string& message) ;
	virtual ~DbMessageHandler() ACS_NOEXCEPT;

	/**
		Getters for the generic database message (it contains a COMMON part and a SPECIFIC payload) 
	*/
	uint64_t get_counter() const		 { return counter_ ; }			/* COMMON: message counter */   
	uint64_t get_timestamp() const  	 { return timestamp_ ; }		/* COMMON: timestamp (as usec from 01-01-2000) */  	 
	std::string get_driver_name() const  { return driver_name_ ; }		/* COMMON: driver name */
	std::string get_channel() const 	 { return channel_ ; }			/* COMMON: channel name */
	JsonSerializable get_payload() const { return payload_ ; }			/* COMMON: Container for the SPECIFIC payload */

	/**
		Getters for the specific payload (each one to be implemented by the db-specific class) 
	*/
	virtual std::string get_server_ip() const			= 0 ; 
	virtual std::string get_client_ip() const			= 0 ; 
	virtual std::string get_user() const				= 0 ; 
	virtual std::string get_process() const 			= 0 ; 
	virtual std::string get_database() const			= 0 ; 
	virtual std::string get_subsystem() const	 		= 0 ; 
	virtual std::string get_table() const				= 0 ; 
	virtual std::string get_action() const  			= 0 ; 
	virtual uint64_t get_pid() const 		 			= 0 ; 
	virtual ssize_t get_affected_rows() const 			= 0 ; 
	virtual std::vector<uint64_t> get_id_vector() const	= 0 ; 

public:

	std::string short_dump() const ; // Not overloaded

	/** Method overloaded */
	virtual void unserialize(const std::string& blob) ;	

	/** Method overloaded */
	virtual std::string dump() const ;							

	/**
		Static method, used by the factory to extract the name of the driver (which is the "main key" for the factory)
	*/
	inline static std::string get_driver_name(const std::string& msg)	
	{ std::string v ; JsonSerializable h ; h.unserialize(msg) ; return h.get_value("driver_name", v) ; }
	
	/**
		self test
	*/
	virtual bool self_test() ; 
	
public:
	static uint64_t DateTime_to_timestamp(const DateTime& time_in) ;
	static DateTime timestamp_to_DateTime(const uint64_t& time_mjd2000_usec) ;

private:
	
	uint64_t counter_ ;
	uint64_t timestamp_ ;
	std::string driver_name_ ;
	std::string channel_ ;
	JsonSerializable payload_ ;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(DbMessageHandler);

};

_ACS_END_NAMESPACE

#endif //_DbMessageHandler_H_
