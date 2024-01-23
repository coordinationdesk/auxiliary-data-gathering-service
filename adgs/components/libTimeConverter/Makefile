#
#	Copyright 1995-2001, Advanced Computer Systems , Inc.
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
#	@(#) A.C.S. libTimeConverter Makefile - %M% %I% %E%
#

include ../../common/commondefs.mk

  LIBRARYNAME =	libTimeConverter

  VERSIONFILE = tcV.C

   C++SOURCES = TimeConverterSingleton.C \
   		TimeConverter.C \
   		TimeFormat.C \
   		TransportTime.C \
		ProcessingTime.C \
		AsciiTime.C \
		OnBoardTime.C \
		TimeConverterLocalInitializer.C \
		tcV.C


   C++INCLUDE = TimeConverterSingleton.h \
   		TimeConverter.h \
		TransportTime.h \
		ProcessingTime.h \
		AsciiTime.h \
		TimeFormat.h \
		OnBoardTime.h \
		TimeConverterLocalInitializer.h


COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	
#empty rule, do not remove, you can add specific help strings
helpspecific:
	
	
include ../../common/commonrules.mk
