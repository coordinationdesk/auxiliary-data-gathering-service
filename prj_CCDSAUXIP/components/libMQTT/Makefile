#
#	Copyright 2019-2022, Exprivia - DFDA-AS
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.exprivia.it
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia SpA.
#
#	$Author$
#
#	$Prod: A.C.S. MQTT Library $
#
#
include ../../common/commondefs.mk

  LIBRARYNAME =	libMQTT

  VERSIONFILE = libmqttV.C

  C++SOURCES = \
		MQTTCallbackBridge.C \
		MQTTWorker.C \
		MosquittoMessageQueue.C \
		MQTTSubscriber.C \
		MQTTMultiSubscriber.C \
		MQTTPublisher.C \
		MosquittoInitializer.C \
		MQTTHelper.C \
		MQTTHeartBeat.C \
		libmqttV.C

   C++INCLUDE = \
		MQTTWorker.h \
		MosquittoMessageQueue.h \
		MQTTSubscriber.h \
		MQTTMultiSubscriber.h \
		MQTTPublisher.h \
		MQTTHelper.h \
		MQTTSubIdHelper.h \
		MQTTHeartBeat.h


COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

ifeq (,$(findstring DISABLE_MQTT,$(OTHERFLAGS)))
   DSOSPECIFICDEPS = -lmosquitto
else
   DSOSPECIFICDEPS =
endif

include ../../common/commonrules.mk



