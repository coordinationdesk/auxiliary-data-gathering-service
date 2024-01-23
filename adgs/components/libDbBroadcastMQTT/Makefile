#
#	Copyright 1995-2021, Exprivia
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

# This force to compile all test after the Library has been compiled
  #POSTCOMPILERULE += test

  LIBRARYNAME =	libDbBroadcastMQTT

  VERSIONFILE = libdbbmqttV.C

  C++SOURCES = \
		MQTTDbTopicGenerator.C \
		DbMessageHandler.C \
		Postgres8MessageHandler.C \
		MQTTMessageHandler.C \
		MQTTDbPublisher.C \
		MQTTDbSubscriber.C \
		MQTTDbSubscriberThread.C \
		MQTTDbMultiSubscriber.C \
		MQTTDbBroadcastListenerBase.C \
		libdbbmqttV.C

   C++INCLUDE = \
		MQTTDbTopicGenerator.h \
		DbMessageHandler.h \
		Postgres8MessageHandler.h \
		DbMessageHandlerFactory.h \
		MQTTMessageHandler.h \
		MQTTDbPublisher.h \
		MQTTDbSubscriber.h \
		MQTTDbSubscriberThread.h \
		MQTTDbMultiSubscriber.h \
		MQTTDbBroadcastListenerBase.h \


COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

#DSOSPECIFICDEPS = -lmosquitto

include ../../common/commonrules.mk



