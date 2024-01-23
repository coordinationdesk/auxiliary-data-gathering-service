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
#	@(#) A.C.S. libOrbitPropagator Makefile - %M% %I% %E%
#

include ../../common/commondefs.mk

  LIBRARYNAME =	libOrbitPropagator

  VERSIONFILE = opV.C

   C++SOURCES = OrbitPropagatorSingleton.C \
   		OrbitPropagator.C \
   		AbsOrbitInfo.C \
		RelOrbitInfo.C \
		StateVector.C \
		PropagFormat.C \
		ExtraInfo.C \
		KeplerianElement.C \
		AnxInfo.C \
		PoleInfo.C \
		OrbitPropagatorLocalInitializer.C \
		PolarMotion.C \
		opV.C
				
   				

   C++INCLUDE = OrbitPropagatorSingleton.h \
   		OrbitPropagator.h \
		AbsOrbitInfo.h \
		RelOrbitInfo.h \
		PropagFormat.h \
		StateVector.h \
		ExtraInfo.h \
		KeplerianElement.h \
		AnxInfo.h \
		PoleInfo.h \
		OrbitPropagatorLocalInitializer.h \
		PolarMotion.h
   
   				

COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	
#empty rule, do not remove, you can add specific help strings
helpspecific:
	
	
include ../../common/commonrules.mk
