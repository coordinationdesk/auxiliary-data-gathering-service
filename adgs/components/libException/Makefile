#
#	Copyright 1995-2018, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. Exception Library $
#
#
include ../../common/commondefs.mk

  LIBRARYNAME =	libException

  VERSIONFILE = exV.C

     CSOURCES =	

     CINCLUDE =	
   CPPSOURCES = acs_c++config.cpp

   C++SOURCES = \
		ThreadSafe.C \
		Statistics.C \
		RTTr.C \
		Filterables.C \
		exRTDebuggable.C \
		exRTDebug.C \
		exException.C \
		exStream.C \
		exFilterable.C \
		exFilter.C \
		exAdaptiveFilter.C \
		exFMCFilterables.C \
		exMsgFmtFactory.C \
		exStackTrace.C \
		OnExitDetector.C \
		ProcessStatus.C \
		MCFilterables.C \
		EventNotifyFactory.C \
		exV.C \
		StopController.C \
		Mappleton.C

   C++INCLUDE = \
		ThreadSafe.h \
		Statistics.h \
		RTTr.hpp \
		Filterables.h \
		exException.h \
		exStream.h \
		exFilterable.h \
		exFilter.h \
		exAdaptiveFilter.h \
		exFMessage.h \
		exFWarning.h \
		exFError.h \
		exFOpAlert.h \
		exFDebug.h \
		exFEvent.h \
		exFMCFilterables.h \
		MCFilterables.h \
		exFOpEventAlert.h \
		exMacros.h \
		exRTDebuggable.h \
		exRTDebug.h \
		exMsgFmtFactory.h \
		exStackTrace.h \
		FEConvenienceValues.h \
		Guard.hpp \
		ErrorLegacy.h \
		Functional.h \
		OnExitDetector.h \
		ProcessStatus.h \
		Singleton.hpp \
		EventNotifyFactory.h \
		acs_c++config.hpp \
		acs_c++pers.hpp \
		StopController.h \
		Mappleton.h \
		Observer.h


COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 

DSOSPECIFICDEPS = -lm -lssl -lpthread -lrt  



include ../../common/commonrules.mk



