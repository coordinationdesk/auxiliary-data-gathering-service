#
#	Copyright 2019, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. libEOFPreInventory$
#	
#        
#

include ../../common/commondefs.mk

LIBRARYNAME = libEOFPreInventory

VERSIONFILE = EOFPreInventoryV.C

CSOURCES =	

CINCLUDE =	

C++SOURCES = EOFPreInventory.C \
    EOFADMPreInventory.C \
    EOFMPMPreInventory.C \
	EOFPreInventoryFactory.C \
	EOFPreInventoryV.C

C++INCLUDE = EOFPreInventory.h \
    EOFADMPreInventory.h \
    EOFMPMPreInventory.h \
	EOFPreInventoryFactory.h 

COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:


# DO NOT DELETE

include ../../common/commonrules.mk






