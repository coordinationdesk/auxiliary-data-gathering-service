#
#	Copyright 1995-2021, Exprivia SpA
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.exprivia.com
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia SpA
#
#	$Prod: A.C.S. Makefile for libGenFileAction $
#
#	$Id$
#
#	$Author$
#
#	$Log: %
#	
#		
#

include ../../common/commondefs.mk

#dbexpress compatibility mode (for Informix w/ SQLString)
#CUSTOMFLAGS:=-DDBEXPRESS_COMPAT $(CUSTOMFLAGS)

# COSMO Specific libraries
ifeq ($(PROJECT), Cosmo)
	CUSTOMFLAGS:=-DCOSMO $(CUSTOMFLAGS)
endif

TARGETFILES=$(DSO_LIBRARY) 
  LIBRARYNAME =	libGenFileAction

  VERSIONFILE = gfaV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		gfaV.C \
		dbUploadInventoryFile.C \
		dbEraseInventoryFile.C \
		dbDownloadInventoryFile.C \
		dbGiveUpInventoryFile.C \
		dbGenFileActionFactory.C \
		dbInventoryObjectOperatorIF.C \
		GenSMPackingStrategyFactory.C \
		PlainStrategy.C \
		TgzStrategy.C \
		GzStrategy.C \
		GFASMTmp.C \
		dbInventoryObjectCacheManager.C \
		GenStorageManagerInfo.C \
		GenStorageManagerInfoFactory.C \
		UploadPredicate.C \
		ByMission.C \
		UploadPredicateFactory.C \
		
		

   C++INCLUDE = \
		dbUploadInventoryFile.h \
		dbEraseInventoryFile.h \
		dbDownloadInventoryFile.h \
		dbGiveUpInventoryFile.h \
		dbGenFileActionFactory.h \
		dbInventoryObjectOperatorIF.h \
		GenSMPackingStrategyFactory.h \
		PlainStrategy.h \
		TgzStrategy.h \
		GzStrategy.h \
		dbInventoryObjectCacheManager.h \
		GenStorageManagerInfo.h \
		GenStorageManagerInfoFactory.h \
		UploadPredicate.h \
		ByMission.h \
		UploadPredicateFactory.h \

DSOSPECIFICDEPS =
		
		


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared -L$(LIB_INTEGRATION_PATH)/lib


COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

include ../../common/commonrules.mk

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	@rm -f *.bak;


#empty rule, do not remove, you can add specific remotion commands
helpspecific:



# DO NOT DELETE


