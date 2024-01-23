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

  LIBRARYNAME =	libMQTTStatistics

  VERSIONFILE = mqttstatV.C

  C++SOURCES = \
		InfluxDbStatisticPoint.C \
		StatEventNotifyFactory.C \
		StatisticsNotifier.C \
		MQTTStatPublisherThread.C \
		StatisticsBacklog.C \
		mqttstatV.C

   C++INCLUDE = \
		InfluxDbStatisticPoint.h \
		StatEventNotifyFactory.h \
		StatisticsNotifier.h \
		MQTTStatPublisherThread.h \
		StatisticsBacklog.h

COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

DSOSPECIFICDEPS =

include ../../common/commonrules.mk



