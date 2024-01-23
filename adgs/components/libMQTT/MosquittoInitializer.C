// PRQA S 1050 EOF
/*
   Copyright 1995-2019, Exprivia
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Advanced Computer Systems, Inc. 

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
    acs::libmqttV version ;
}

_ACS_BEGIN_NAMESPACE(acs) 

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(MosquittoInitializer)

MosquittoInitializer::MosquittoInitializer() ACS_NOEXCEPT : 
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


MosquittoInitializer::~MosquittoInitializer() ACS_NOEXCEPT
{
	ACS_LOG_PRIVINFO ("cleaning up libmosquitto");
#ifndef DISABLE_MQTT
// MP no initialization if DISABLED

	// No need to check exit code
	mosquitto_lib_cleanup();
#endif
}

_ACS_END_NAMESPACE
