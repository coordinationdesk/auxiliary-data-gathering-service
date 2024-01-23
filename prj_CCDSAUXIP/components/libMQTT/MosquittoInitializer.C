// PRQA S 1050 EOF
/*
   Copyright 2019-2022, Exprivia - DFDA-AS
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Exprivia SpA. 

   libMQTT External library initialization singleton
*/ 

#include <MosquittoInitializer.h>
#include <Filterables.h>
#include <libmqttV.h>

#ifndef DISABLE_MQTT
#include <mosquitto.h>
#else 
#warning MQTT has been disabled
#endif


namespace {
    acs::libmqttV version ; // NOSONAR
}

_ACS_BEGIN_NAMESPACE(acs) 

ACS_CLASS_DEFINE_DEBUG_LEVEL(MosquittoInitializer)

MosquittoInitializer::MosquittoInitializer() noexcept : 
	Singleton<MosquittoInitializer>()
{
	ACS_LOG_PRIVINFO ("initializing libmosquitto");
#ifndef DISABLE_MQTT
// MP no initialization if DISABLED
	// No need to check exit code
	mosquitto_lib_init();

	ACS_LOG_PRIVINFO ("libmosquitto initialized");
#else
	ACS_LOG_PRIVINFO ("libmosquitto is DISABLED");
#endif
}


MosquittoInitializer::~MosquittoInitializer() noexcept
{
	ACS_LOG_PRIVINFO ("cleaning up libmosquitto");
#ifndef DISABLE_MQTT
// MP no initialization if DISABLED

	// No need to check exit code
	mosquitto_lib_cleanup();
#endif
}

_ACS_END_NAMESPACE
