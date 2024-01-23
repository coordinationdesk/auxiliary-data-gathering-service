// PRQA S 1050 EOF
/*
   Copyright 2020-, Exprivia SpA
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Advanced Computer Systems, Inc. 

   MQTT Db Topic Generator header file
*/ 

#include <SQLDialect.h>
#include <dbConnection.h>
#include <MQTTDbTopicGenerator.h>
#include <MQTTHelper.h>

using namespace std;

_ACS_BEGIN_NAMESPACE(acs) 

ACS_CLASS_DEFINE_DEBUG_LEVEL(MQTTDbTopicGenerator) ;
	
const string MQTTDbTopicGenerator::_default_topic_subsystem("DB") ;
const string MQTTDbTopicGenerator::_default_topic_entity("DbBroadcast_MQTT_Bridge") ;
const string MQTTDbTopicGenerator::_default_topic_entity_id("01") ;


/***************************************************************************************
**
                             MQTTDbTopicGenerator
**
***************************************************************************************/
MQTTDbTopicGenerator::MQTTDbTopicGenerator(	const dbConnection& conn ):
	_connection(conn),
	_topic_organization(MQTTHelper::getOrganization()),
	_topic_subsystem(get_default_topic_subsystem()),
	_topic_subsystem_id(generate_subsystem_id(conn)),
	_topic_entity(get_default_topic_entity()),
	_topic_entity_id(get_default_topic_entity_id())
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
}


/***************************************************************************************
**
                             ~MQTTDbTopicGenerator
**
***************************************************************************************/
MQTTDbTopicGenerator::~MQTTDbTopicGenerator() ACS_NOEXCEPT
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB)
}


/***************************************************************************************
**
                             generate_topic [class member]
**
***************************************************************************************/
string MQTTDbTopicGenerator::generate_topic (const string& table, const enum SQLDialect::SQLToken& action) const
{
	return generate_topic(	get_topic_organization(),
							get_topic_subsystem(),
							get_topic_subsystem_id(),
							get_topic_entity(),
							get_topic_entity_id(),
							table,
							action ) ;
}


/***************************************************************************************
**
                             generate_topic [class member]
**
***************************************************************************************/
string MQTTDbTopicGenerator::generate_topic (const string& table, const string& action) const
{
	return generate_topic(	get_topic_organization(),
							get_topic_subsystem(),
							get_topic_subsystem_id(),
							get_topic_entity(),
							get_topic_entity_id(),
							table,
							action ) ;
}

/***************************************************************************************
**
                             generate_topic  [static method]
**
***************************************************************************************/
string MQTTDbTopicGenerator::generate_topic (	const dbConnection &conn, const string& table, \
												const enum SQLDialect::SQLToken& action)
{
	return generate_topic(	MQTTHelper::getOrganization(), \
							get_default_topic_subsystem(), \
							generate_subsystem_id(conn), \
							get_default_topic_entity(), \
							get_default_topic_entity_id(), \
							table, \
							action ) ;
}


/***************************************************************************************
**
                             generate_topic  [static method]
**
***************************************************************************************/
string MQTTDbTopicGenerator::generate_topic (	const string& topic_organization,	\
												const string& topic_subsystem,		\
												const string& topic_subsystem_id,	\
												const string& topic_entity,			\
												const string& topic_entity_id,		\
												const string& table,				\
												const enum SQLDialect::SQLToken& e) 
{
	string action ; switch (e)
	{
		case SQLDialect::INSERT: { action = "INSERT" ; break ; }
		case SQLDialect::UPDATE: { action = "UPDATE" ; break ; }
		case SQLDialect::DELETE: { action = "DELETE" ; break ; }
		default: { ACS_THROW ( exIllegalValueException("Invalid action") ) ;  }			
	} // END: case 
	
	return generate_topic(	topic_organization, \
							topic_subsystem, \
							topic_subsystem_id, \
							topic_entity, \
							topic_entity_id, \
							table, \
							action ) ;
}


/***************************************************************************************
**
                             generate_topic  [static method]
**
***************************************************************************************/
string MQTTDbTopicGenerator::generate_topic (	const string& topic_organization,	\
												const string& topic_subsystem,		\
												const string& topic_subsystem_id,	\
												const string& topic_entity,			\
												const string& topic_entity_id,		\
												const string& table,				\
												const string& action) 
{
	ostringstream topic ; 
	topic 
			<< "free/" 			/* FIXME -- THIS LINE TO BE REMOVED */			\
			<< topic_organization \
			<< "/" << topic_subsystem \
			<< "/" << topic_subsystem_id \
			<< "/" << topic_entity \
			<< "/" << topic_entity_id \
			<< "/" << table \
			<< "/" << action \
	;
		
	const string topic_out ( topic.str() ) ; 

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished. topic_organization: \"" << topic_organization << "\"" \
			<< "  topic_subsystem: \"" << topic_subsystem << "\"" \
			<< "  topic_subsystem_id: \"" << topic_subsystem_id << "\"" \
			<< "  topic_entity: \"" << topic_entity << "\"" \
			<< "  topic_entity_id: \"" << topic_entity_id << "\"" \
			<< "  topic_organization: \"" << topic_organization << "\"" \
			<< "  table: \"" << table << "\"" \
			<< "  action: \"" << action << "\"" \
			<< "  Topic: \"" << topic_out << "\"" << endl ;
	ACS_CLASS_END_DEBUG

	return topic_out ;
}


/***************************************************************************************
**
                             generate_subsystem_id [static member]
**
***************************************************************************************/
string MQTTDbTopicGenerator::generate_subsystem_id (const dbConnection &connection) 
{
	ostringstream o ; o
		<< connection.connectionParameters()->server() \
		<< "_" \
		<< connection.connectionParameters()->db() ;
	
	const string subsystem_id_out (o.str()) ;
	
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
		excerr << SimpleDebugSignatureThread << "  Method finished.  Subsystem_ID: \"" << subsystem_id_out << "\"" << endl ;
	ACS_CLASS_END_DEBUG
	
	return subsystem_id_out ;
} 


_ACS_END_NAMESPACE
