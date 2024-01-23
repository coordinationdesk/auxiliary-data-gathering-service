#
#	Copyright 1995-2023, Exprivia SpA - DADF
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.exprivia.it
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia SpA
#
#	$Prod: A.C.S. libS1L2AuxReformatter$
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.2  2012/04/05 09:07:36  damdec
#	ecmwfGribDateExtractor class added.
#	
#	Revision 1.1.1.1  2012/03/27 11:50:11  damdec
#	Import libS1L2AuxReformatter
#	
#        
#

include ../../common/commondefs.mk

LIBRARYNAME = libS1L2AuxReformatter

VERSIONFILE = S1L2AuxReformatterV.C

CUSTOM_INCLUDEPATH =

CUSTOM_LIBRARYPATH =

CSOURCES =	

CINCLUDE =	

C++SOURCES = S1L2AuxReformatter.C \
	S1L2AuxReformatterSingleton.C \
	S1L2AuxReformatterFactory.C \
	S1AuxGeneralInformation.C \
	S1AuxWndReformatter.C \
	S1AuxWavReformatter.C \
	S1AuxIceReformatter.C \
	S1AuxTroReformatter.C \
	S1AuxTecReformatter.C \
	ecmwfGribDateExtractor.C \
	S1L2AuxReformatterV.C

C++INCLUDE = S1L2AuxReformatter.h \
	S1AuxGeneralInformation.h \
	S1L2AuxReformatterSingleton.h \
	S1L2AuxReformatterFactory.h \
	S1AuxWndReformatter.h \
	S1AuxWavReformatter.h \
	S1AuxIceReformatter.h \
	S1AuxTroReformatter.h \
	S1AuxTecReformatter.h \
	ecmwfGribDateExtractor.h

COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)
DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:


# DO NOT DELETE

include ../../common/commonrules.mk

DSOSPECIFICDEPS =
