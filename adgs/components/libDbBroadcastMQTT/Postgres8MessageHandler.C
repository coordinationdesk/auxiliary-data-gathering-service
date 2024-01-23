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

   Postgres8MessageHandler
*/ 

#include <Postgres8MessageHandler.h>
#include <StringUtils.h>
#include <exException.h>

using namespace std;

namespace {
	class to_i {
		// Helper class to use in a foreach-loop (each element converted from string to uint64_t)           
		public:
			to_i(std::vector <uint64_t>& v) : v_(v) {} 
 			void operator() (const string& s) { v_.push_back(atoi( s.c_str()) ) ; }
		private:
			to_i() ; // not implemented
			std::vector <uint64_t>& v_ ;	
	} ;
} // END: namespace

_ACS_BEGIN_NAMESPACE(acs) 

ACS_CLASS_DEFINE_DEBUG_LEVEL(Postgres8MessageHandler);

/***************************************************************************************
**
                             Postgres8MessageHandler
**
***************************************************************************************/
Postgres8MessageHandler::Postgres8MessageHandler(const std::string& message) :
	DbMessageHandler(message),
	server_ip_(),
	client_ip_(),
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

	Postgres8MessageHandler::unserialize(message) ;	// Explicitly call this class method -- since method has been overload
}


/***************************************************************************************
**
                             Postgres8MessageHandler
**
***************************************************************************************/
Postgres8MessageHandler::Postgres8MessageHandler() :
	DbMessageHandler(),
	server_ip_(),
	client_ip_(),
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
                             ~Postgres8MessageHandler
**
***************************************************************************************/
Postgres8MessageHandler::~Postgres8MessageHandler() ACS_NOEXCEPT
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
}


/***************************************************************************************
**
                             self_test
**
***************************************************************************************/
bool Postgres8MessageHandler::self_test() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	bool status (false) ; 
	excerr << "About to run Self test for DbMessageHandler specific payload class: " << get_driver_name() << endl ;

	// Create a test object, in order not to overwrite the content of *this
	Postgres8MessageHandler p ; 
	
	// Self test the base class (only the payload is not tested)
	p.DbMessageHandler::self_test() ; 
	
	// Generate a db-specific payload
	ostringstream db_specific_message ;
	db_specific_message \
		<< "{ " \
		<<		"\"driver_name\": null, " \
		<<		"\"channel\": null, " \
		<<		"\"counter\": null, " \
		<<		"\"timestamp\": null, " \
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
		<<				"\"affectedrows\": 3, " \
		<<				"\"id\":    	\"33 44 55\"" /* Note the QUOTES and spaces, ("x y" instead of [x,y]) to correctly simulate the WRONG return from postgres */ \
		<< 		" }" \
		<< " }" ;
 
 	// Inject the payload to the test object
	p.unserialize(db_specific_message.str()) ;
	
	vector <uint64_t> exp_v ; exp_v.push_back(33) ; exp_v.push_back(44) ; exp_v.push_back(55) ; 
	
	if ("SERVER_IP" != p.get_server_ip())		{ ostringstream o ; o << "Mismatching value when comparing server_ip: " << p.get_server_ip() ; ACS_THROW (exIllegalValueException(o.str())) ; }
	if ("CLIENT_IP" != p.get_client_ip())		{ ostringstream o ; o << "Mismatching value when comparing client_ip: " << p.get_client_ip() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
	if ("USER" !=      p.get_user())			{ ostringstream o ; o << "Mismatching value when comparing user: " << p.get_user() ; ACS_THROW (exIllegalValueException(o.str())) ; }		    		 
	if ("PROCESS" !=   p.get_process())			{ ostringstream o ; o << "Mismatching value when comparing process: " << p.get_process() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
	if ("DATABASE" !=  p.get_database())		{ ostringstream o ; o << "Mismatching value when comparing database: " << p.get_database() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
	if ("SUBSYSTEM" != p.get_subsystem())		{ ostringstream o ; o << "Mismatching value when comparing subsystem: " << p.get_subsystem() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
	if ("TABLE" !=     p.get_table())			{ ostringstream o ; o << "Mismatching value when comparing table: " << p.get_table() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
	if ("ACTION" !=    p.get_action())			{ ostringstream o ; o << "Mismatching value when comparing action: " << p.get_action() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
	if (1234 !=        p.get_pid())				{ ostringstream o ; o << "Mismatching value when comparing pid: " << p.get_pid() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
	if (3 !=           p.get_affected_rows())	{ ostringstream o ; o << "Mismatching value when comparing affected_rows: " << p.get_affected_rows() ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
	if (exp_v !=       p.get_id_vector())		{ ostringstream o ; o << "Mismatching value when comparing id_vector: " ; ACS_THROW (exIllegalValueException(o.str())) ; }	    		 
	status = true ; 

	excerr << "Self test for DbMessageHandler specific payload class: " << get_driver_name() << " finished: " << (status? "SUCCESS":"FAILURE") << endl ;

	return status ; 	   
}


/***************************************************************************************
**
                             unserialize
**
***************************************************************************************/
void Postgres8MessageHandler::unserialize(const string& message) 
{
	/**
		The method unserialize has been overloaded in order to handle the wrong type of the ID inside the JSON returned by postgres.
		Instead of type ARRAY (e.g. "ID": [1, 2, 3...]) a string of space-separated entries is returned, in the form: "ID": "1 2 3..."
		Of course, the JSON handling library is NOT able to handle the entry as a vector (being actually a string, not a vector).
		So a converter is called (convert_id_vector). 
		After the converted, the data member is correctly saved as a vector of integers, once.
	*/

	const bool convert_id(true) ; // So far it is always true

	DbMessageHandler::unserialize(message) ;  //  Call parent method

	if ( (false == get_payload().has_key("serverip")) || (true == get_payload().is_null("serverip")) )		{ server_ip_ = "" ; }	else { get_payload().get_value("serverip", server_ip_) ; }
	if ( (false == get_payload().has_key("clientip")) || (true == get_payload().is_null("clientip")) )		{ client_ip_ = "" ; }	else { get_payload().get_value("clientip", client_ip_) ; }
	if ( (false == get_payload().has_key("user"))  	  || (true == get_payload().is_null("user")) )	  		{ user_ = "" ; }  	  	else { get_payload().get_value("user", user_) ; }
	if ( (false == get_payload().has_key("process"))  || (true == get_payload().is_null("process")) )		{ process_ = "" ; }	  	else { get_payload().get_value("process", process_) ; }
	if ( (false == get_payload().has_key("database")) || (true == get_payload().is_null("database")) )		{ database_ = "" ; }    else { get_payload().get_value("database", database_) ; }
	if ( (false == get_payload().has_key("subsystem"))|| (true == get_payload().is_null("subsystem")) )		{ subsystem_ = "" ; }	else { get_payload().get_value("subsystem", subsystem_) ; }
	if ( (false == get_payload().has_key("table")) 	  || (true == get_payload().is_null("table")) )			{ table_ = "" ; }		else { get_payload().get_value("table", table_) ; }
	if ( (false == get_payload().has_key("action"))	  || (true == get_payload().is_null("action")) )		{ action_ = "" ; }		else { get_payload().get_value("action", action_) ; }
	if ( (false == get_payload().has_key("pid"))   	  || (true == get_payload().is_null("pid")) )			{ pid_ = 0 ; }			else { get_payload().get_value("pid", pid_) ; }
	if ( (false == get_payload().has_key("affectedrows")) || (true == get_payload().is_null("affectedrows")) ){ affected_rows_ = 0 ; }else { get_payload().get_value("affectedrows", affected_rows_) ; }

	id_vector_.clear() ; if ( (true == get_payload().has_key("id")) && (false == get_payload().is_null("id")) )
	{
		if (false == convert_id) 
		{ get_payload().get_value("id", id_vector_) ; }
		else
		{
			string tmp_s ; get_payload().get_value("id", tmp_s) ; 
			id_vector_ = convert_id_vector(tmp_s) ;
		}
	}
}


/***************************************************************************************
**
                             dump
**
***************************************************************************************/
string Postgres8MessageHandler::dump() const 
{
	
	ostringstream v ; v << "[ " ; for (size_t i=0; i<id_vector_.size(); i++) { v << ((i==0)? "": " ,") << id_vector_.at(i) ; } ; v << " ]" ; 
	
	ostringstream o ; o
		<< DbMessageHandler::dump() \
		<< "serverip: \"" << server_ip_ << "\"\n" \
		<< "clientip: \"" << client_ip_ << "\"\n" \
		<< "user: \"" << user_ << "\"\n" \
		<< "process: \"" << process_ << "\"\n" \
		<< "database: \"" << database_ << "\"\n" \
		<< "subsystem: \"" << subsystem_ << "\"\n" \
		<< "table: \"" <<  table_ << "\"\n" \
		<< "action: \"" << action_ << "\"\n" \
		<< "pid: " << pid_ << "\n" \
		<< "affectedrows: " << affected_rows_ << "\n" \
		<< "id: " << v.str() << endl ;

	return o.str() ; 
}


/***************************************************************************************
**
                             convert_id_vector [STATIC]
**
***************************************************************************************/
vector <uint64_t> Postgres8MessageHandler::convert_id_vector(const string& s) 
{
	/**
		 Convert from space-separated string to vector of numbers.
		 Example:
		 	IN: "6 9 22 ..."
			OUT: vector<uint64_t> { 6, 9, 22, ... }
	*/

	// Tokenize the space-separated values to a vector of strings ("6","9","22"...)
	vector <string> tmp_sv ; StringUtils::Tokenize(s, tmp_sv, " ") ;
	
	// Convert each entry in the string 
	vector <uint64_t> v ; for_each(tmp_sv.begin(), tmp_sv.end(), to_i(v)) ;
	return v ; 
}


_ACS_END_NAMESPACE
