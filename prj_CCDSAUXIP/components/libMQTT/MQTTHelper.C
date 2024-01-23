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

#include <MQTTHelper.h>
#include <Mappleton.h>


_ACS_BEGIN_NAMESPACE (acs)


// Organization field
std::string MQTTHelper::getOrganization()
{
	return Mappleton::instance()->getValue (XPR_KVS_MQTT_TOPIC_ORG, "", false);
}

void MQTTHelper::setOrganization(const std::string& s)
{
	Mappleton::instance()->setValue (XPR_KVS_MQTT_TOPIC_ORG, s);
}

// Subsystem field
std::string MQTTHelper::getSubsystem()
{
	return Mappleton::instance()->getValue (XPR_KVS_MQTT_TOPIC_SUB, "", false);
}

void MQTTHelper::setSubsystem(const std::string& s)
{
	Mappleton::instance()->setValue (XPR_KVS_MQTT_TOPIC_SUB, s);
}

// SubsystemId field
std::string MQTTHelper::getSubsystemId()
{
	return Mappleton::instance()->getValue (XPR_KVS_MQTT_TOPIC_SUBID, "", false);
}

void MQTTHelper::setSubsystemId(const std::string& s)
{
	Mappleton::instance()->setValue (XPR_KVS_MQTT_TOPIC_SUBID, s);
}

// Entity field
std::string MQTTHelper::getEntity()
{
	return Mappleton::instance()->getValue (XPR_KVS_MQTT_TOPIC_ENT, "", false);
}

void MQTTHelper::setEntity(const std::string& s)
{
	Mappleton::instance()->setValue (XPR_KVS_MQTT_TOPIC_ENT, s);
}

// Entity field
std::string MQTTHelper::getEntityId()
{
	return Mappleton::instance()->getValue (XPR_KVS_MQTT_TOPIC_ENTID, "", false);
}

void MQTTHelper::setEntityId(const std::string& s)
{
	Mappleton::instance()->setValue (XPR_KVS_MQTT_TOPIC_ENTID, s);
}



_ACS_END_NAMESPACE
