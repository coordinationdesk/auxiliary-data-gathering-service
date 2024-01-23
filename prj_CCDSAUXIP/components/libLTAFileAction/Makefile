#
#	Copyright 2020-2021, Exprivia SpA - DFDA-AS
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
#	$Prod: A.C.S. Makefile for libCryosatFileAction $
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

TARGETFILES=$(DSO_LIBRARY) 
  LIBRARYNAME =	libLTAFileAction

  VERSIONFILE = gfaV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		gfaV.C \
		dbLTAUploadInventoryFile.C \
		dbLTADownloadInventoryFile.C \
		dbLTAGiveUpInventoryFile.C \
		dbLTAEraseInventoryFile.C \
		dbLTAReleaseCacheInventoryFile.C \
		dbLTAIsOnCacheInventoryFile.C \
		dbLTAPutOnCacheInventoryFile.C \
		dbLTAFileActionFactory.C \
		LTAInventoryObjectOperatorIF.C \
		LTASMTmp.C \
		
		

   C++INCLUDE = \
		dbLTAUploadInventoryFile.h \
		dbLTADownloadInventoryFile.h \
		dbLTAGiveUpInventoryFile.h \
		dbLTAEraseInventoryFile.h \
		dbLTAReleaseCacheInventoryFile.h \
		dbLTAIsOnCacheInventoryFile.h \
		dbLTAPutOnCacheInventoryFile.h \
		LTAInventoryObjectOperatorIF.h \


COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	@rm -f *.bak;


#empty rule, do not remove, you can add specific remotion commands
helpspecific:



# DO NOT DELETE
include ../../common/commonrules.mk


