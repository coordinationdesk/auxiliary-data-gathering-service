#
#	Copyright 1995-2019, Exprivia
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.acsys.it
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Advanced Computer Systems, Inc.
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
		MQTTSubscriber.C \
		MQTTPublisher.C \
		MosquittoInitializer.C \
		MQTTHelper.C \
		MQTTHeartBeat.C \
		libmqttV.C

   C++INCLUDE = \
		MQTTWorker.h \
		MQTTSubscriber.h \
		MQTTPublisher.h \
		MQTTHelper.h \
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



