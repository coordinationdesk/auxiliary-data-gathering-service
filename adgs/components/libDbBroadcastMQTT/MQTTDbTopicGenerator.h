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

#ifndef _MQTTDbTopicGenerator_H_
#define _MQTTDbTopicGenerator_H_

#include <SQLDialect.h>

#include <Filterables.h>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs) 

class dbConnection ;

class MQTTDbTopicGenerator 
{
public:
	
	MQTTDbTopicGenerator(const dbConnection& conn) ;
	virtual ~MQTTDbTopicGenerator() ACS_NOEXCEPT ;


	/**
		Getters:
	*/
	inline std::string get_topic_organization() const			{ return _topic_organization ; }  
	inline std::string get_topic_subsystem() const				{ return _topic_subsystem ; }  
	inline std::string get_topic_subsystem_id() const			{ return _topic_subsystem_id ; }
	inline std::string get_topic_entity() const					{ return _topic_entity ; }  
	inline std::string get_topic_entity_id() const				{ return _topic_entity_id ; }

	/**
		Setters:
	*/
	inline void set_topic_organization(const std::string& c )	{ _topic_organization = c ; }  
	inline void set_topic_subsystem(const std::string& c )		{ _topic_subsystem = c ; }  
	inline void set_topic_subsystem_id(const std::string& c )	{ _topic_subsystem_id = c ; }  
	inline void set_topic_entity(const std::string& c )			{ _topic_entity = c ; }  
	inline void set_topic_entity_id(const std::string& c )		{ _topic_entity_id = c ; }  

	/**
		Generate a Topic [class member]
	*/
	std::string generate_topic (const std::string& table, const enum SQLDialect::SQLToken& action) const ;
	std::string generate_topic (const std::string& table, const std::string& action) const ;


	/**
		Generate a Topic [static method]
	*/
	static std::string generate_topic (const std::string& topic_organization, const std::string& topic_subsystem,		\
			const std::string& topic_subsystem_id, const std::string& topic_entity, const std::string& topic_entity_id,	\
			const std::string& table, const enum SQLDialect::SQLToken&) ;

	static std::string generate_topic (const std::string& topic_organization, const std::string& topic_subsystem,		\
			const std::string& topic_subsystem_id, const std::string& topic_entity, const std::string& topic_entity_id,	\
			const std::string& table, const std::string& action) ;

	static std::string generate_topic (const dbConnection&, const std::string& table, const enum SQLDialect::SQLToken&) ;

	/**
		Generate Subsystem Id [static method]
	*/
	static std::string generate_subsystem_id (const dbConnection &conn) ; 

	static std::string get_default_topic_subsystem()			{ return _default_topic_subsystem ; }  
	static std::string get_default_topic_entity()				{ return _default_topic_entity ; }  
	static std::string get_default_topic_entity_id ()			{ return _default_topic_entity_id ; }  



private:
	MQTTDbTopicGenerator (const MQTTDbTopicGenerator&);
	MQTTDbTopicGenerator& operator= (const MQTTDbTopicGenerator&);

private:
	const dbConnection&			_connection ;			
	std::string					_topic_organization ;  
	std::string					_topic_subsystem ;  
	std::string					_topic_subsystem_id ;  
	std::string					_topic_entity ;  
	std::string					_topic_entity_id ;  
	static const std::string	_default_topic_subsystem ;
	static const std::string	_default_topic_entity ;
	static const std::string	_default_topic_entity_id ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(MQTTDbTopicGenerator) ;
};

_ACS_END_NAMESPACE

#endif //_MQTTDbTopicGenerator_H_
