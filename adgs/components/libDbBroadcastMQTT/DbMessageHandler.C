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

   DbMessageHandler
*/ 

#include <DbMessageHandler.h>
#include <exException.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs) 

ACS_CLASS_DEFINE_DEBUG_LEVEL(DbMessageHandler);


/***************************************************************************************
**
                             DbMessageHandler
**
***************************************************************************************/
DbMessageHandler::DbMessageHandler() :
	JsonSerializable(JsonSerializable::SF_TEXT),
	counter_(0),
	timestamp_(0),
	driver_name_(),
	channel_(),
	payload_()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
}


/***************************************************************************************
**
                             DbMessageHandler
**
***************************************************************************************/
DbMessageHandler::DbMessageHandler(const std::string& message) :
	JsonSerializable(JsonSerializable::SF_TEXT),
	counter_(0),
	timestamp_(0),
	driver_name_(),
	channel_(),
	payload_()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	DbMessageHandler::unserialize(message) ; 	// Explicitly call this class method -- since method has been overload
}


/***************************************************************************************
**
                             ~DbMessageHandler
**
***************************************************************************************/
DbMessageHandler::~DbMessageHandler() ACS_NOEXCEPT
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
}


/***************************************************************************************
**
                             unserialize
**
***************************************************************************************/
void DbMessageHandler::unserialize(const string& message) 
{
	JsonSerializable::unserialize(message) ; //  Call parent method

	if (true == is_null("counter"))		{ counter_ = 0 ; }		else { get_value("counter", counter_) ; }
	if (true == is_null("timestamp"))	{ timestamp_ = 0 ; }	else { get_value("timestamp", timestamp_) ; }
	if (true == is_null("driver_name"))	{ driver_name_ = "" ; }	else { get_value("driver_name", driver_name_) ; }
	if (true == is_null("channel"))		{ channel_ = "" ; }		else { get_value("channel", channel_) ; }
	payload_.reset() ; if (false == is_null("payload"))			{ get_value("payload", payload_) ; }
}


/***************************************************************************************
**
                             dump
**
***************************************************************************************/
string DbMessageHandler::dump() const
{
	ostringstream o ; o 
		<< "driver_name: \"" << get_driver_name() << "\"\n" \
		<< "channel: \"" << get_channel() << "\"\n" \
		<< "counter: " << get_counter() << "\n" \
		<< "timestamp: " << get_timestamp() << " [" << timestamp_to_DateTime(get_timestamp()).asISO8601(6) << "]\n" ;
		
	return o.str() ; 
}

/***************************************************************************************
**
                             short_dump
**
***************************************************************************************/
string DbMessageHandler::short_dump() const
{
	ostringstream o ; o 
		<< "driver_name: \"" << get_driver_name() << "\"" \
		<< " channel: \"" << get_channel() << "\"" \
		<< " counter: " << get_counter() \
		<< " timestamp: " << get_timestamp() << " [" << timestamp_to_DateTime(get_timestamp()).asISO8601(6) << "]" ;
		
	return o.str() ; 
}


/***************************************************************************************
**
                             self_test
**
***************************************************************************************/
bool DbMessageHandler::self_test()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	bool status (false) ; 
	excerr << "About to run Self test for DbMessageHandler base class" << endl ;
	
	ostringstream db_message ;
	db_message \
		<< "{ " \
		<< "\"driver_name\": " << "\"DRIVER_NAME\"" << ", " \
		<< "\"channel\": " << "\"CHANNEL_NAME\"" << ", " \
		<< "\"counter\": " << 112244 << ", " \
		<< "\"timestamp\": " << DateTime_to_timestamp("UTC=20121110T090807.654321") << ", " \
		<< "\"payload\": " << "null" /* The payload will be filled by the specific db-payload handler, in its own self_test method */ \
		<< " }" ;

	DbMessageHandler::unserialize ( db_message.str() ) ;
		
	if ("DRIVER_NAME" != get_driver_name()) { ostringstream o ; o << "Mismatching value when comparing driver_name: " << get_driver_name() ; ACS_THROW (exIllegalValueException(o.str())) ; }
	if ("CHANNEL_NAME" != get_channel()) { ostringstream o ; o << "Mismatching value when comparing channel: " << get_channel() ; ACS_THROW (exIllegalValueException(o.str())) ; }
	if (112244 != get_counter()) { ostringstream o ; o << "Mismatching value when comparing counter: " << get_counter() ; ACS_THROW (exIllegalValueException(o.str())) ; }
	if (405853687654321LL != get_timestamp()) { ostringstream o ; o << "Mismatching value when comparing timestamp: " << get_timestamp() ; ACS_THROW (exIllegalValueException(o.str())) ; }
	if ("2012-11-10T09:08:07.654321" != timestamp_to_DateTime(get_timestamp()).asISO8601(6)) { ostringstream o ; o << "Mismatching value when comparing timestamp: " << get_timestamp() ; ACS_THROW (exIllegalValueException(o.str())) ; }

	if (true != is_null("payload")) { ostringstream o ; o << "Unable to verify that payload is null" ; ACS_THROW (exIllegalValueException(o.str())) ; }
	status = true ; 

	excerr << "Self test for DbMessageHandler base class finished: " << (status? "SUCCESS":"FAILURE") << endl ;
		
	return status ; 	
}
					 
	
/***************************************************************************************
**
                           DateTime_to_timestamp [STATIC]
**
***************************************************************************************/
uint64_t DbMessageHandler::DateTime_to_timestamp(const DateTime& time_in)
{
	DateTime::MJD2000 time_mjd2000(time_in) ;										/* MJD2000 date: Number of days from 01-01-2000 */
	const uint64_t time_mjd2000_usec ( time_mjd2000.jd() * 86400.0 * 1000000.0 ) ;	/* Number of usec from 01-01-2000. */

	return time_mjd2000_usec ;
}


/***************************************************************************************
**
                           timestamp_to_DateTime [STATIC]
**
***************************************************************************************/
DateTime DbMessageHandler::timestamp_to_DateTime(const uint64_t& time_mjd2000_usec)		/* Number of usec from 01-01-2000. */
{
	const long double time_mjd2000 ( (time_mjd2000_usec / 1000000.0) / 86400.0 ) ;	/* Number of days from 01-01-2000 [ E.G. MJD2000 date ] */

	DateTime::MJD2000 d(time_mjd2000) ; 

	return d ;
}


_ACS_END_NAMESPACE
