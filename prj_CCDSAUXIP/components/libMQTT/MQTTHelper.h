// PRQA S 1050 EOF
/*
   Copyright 2019-2022 Exprivia - DFDA-AS
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Exprivia SpA. 

   libMQTT class
*/ 


#ifndef _MQTTHelper_H_
#define _MQTTHelper_H_

#include <string>
#include <acs_c++config.hpp>

#define XPR_KVS_MQTT_TOPIC_ORG "MQTT_ORGANIZATION"
#define XPR_KVS_MQTT_TOPIC_SUB "MQTT_SUBSYSTEM"
#define XPR_KVS_MQTT_TOPIC_SUBID "MQTT_SUBSYSTEM_ID"
#define XPR_KVS_MQTT_TOPIC_ENT "MQTT_ENTITY"
#define XPR_KVS_MQTT_TOPIC_ENTID "MQTT_ENTITY_ID"


_ACS_BEGIN_NAMESPACE(acs) 

class MQTTHelper
{
public:

	// This class is a wrapper for static operations allowing to access
	// the Mappleton key valye store by function name instead of using
	// a shared set of keys

	// Organization field
	static std::string getOrganization();
	static void setOrganization(const std::string& s);

	// Subsystem field
	static std::string getSubsystem();
	static void setSubsystem(const std::string& s);

	// SubsystemId field
	static std::string getSubsystemId();
	static void setSubsystemId(const std::string& s);

	// Entity field
	static std::string getEntity();
	static void setEntity(const std::string& s);

	// Entity field
	static std::string getEntityId();
	static void setEntityId(const std::string& s);

};

_ACS_END_NAMESPACE

#endif // _MQTTHelper_H_
