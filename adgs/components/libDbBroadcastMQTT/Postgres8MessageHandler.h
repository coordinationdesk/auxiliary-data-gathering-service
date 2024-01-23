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

   Postgres8MessageHandler header file
*/ 

#ifndef _Postgres8MessageHandler_H_
#define _Postgres8MessageHandler_H_

#include <JsonSerializable.h>
#include <DbMessageHandler.h>
#include <exException.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs) 

class Postgres8MessageHandler : public DbMessageHandler
{
public:
	Postgres8MessageHandler(const std::string& payload) ;
	Postgres8MessageHandler() ;
	virtual ~Postgres8MessageHandler() ACS_NOEXCEPT;

	/**
		Getters for the specific payload :
	*/		
	virtual std::string get_server_ip() const				{ return server_ip_ ; }
	virtual std::string get_client_ip() const				{ return client_ip_ ; }
	virtual std::string get_user() const					{ return user_ ; }     
	virtual std::string get_process() const 	 			{ return process_ ; }  
	virtual std::string get_database() const				{ return database_ ; } 
	virtual std::string get_subsystem() const				{ return subsystem_ ; }
	virtual std::string get_table() const					{ return table_ ; }    
	virtual std::string get_action() const					{ return action_ ; }   
	virtual uint64_t get_pid() const						{ return pid_ ; }	   
	virtual ssize_t get_affected_rows() const				{ return affected_rows_ ; }	   
	virtual std::vector <uint64_t> get_id_vector() const	{ return id_vector_ ; }
	
	virtual bool self_test() ;
	
	/** Method overloaded, due to a problem in the payload (ID is passed as string instead of a vector). More details inside the method */
	virtual void unserialize(const std::string& blob) ;	

	/** Method overloaded */
	virtual std::string dump() const ;							

private:

	/** Convert ID from space-separated string to vector of numbers */
	static std::vector <uint64_t> convert_id_vector(const std::string& s) ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(Postgres8MessageHandler);

	std::string				server_ip_ ;             
	std::string				client_ip_ ;             
	std::string				user_ ;              	
	std::string				process_ ;               
	std::string				database_ ;              
	std::string				subsystem_ ;             
	std::string				table_ ;              	
	std::string				action_ ;              	
	uint64_t				pid_ ;              		
	ssize_t					affected_rows_ ;              		
	std::vector <uint64_t>	id_vector_ ;  

};

_ACS_END_NAMESPACE

#endif //_Postgres8MessageHandler_H_

