#
#	Copyright 1995-2013, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. Makefile for Garbage Cleaner $
#
#	$Id$
#
#	$Author$
#
#	$Log :$
#

include ../../common/commondefs.mk


# CRYOSAT Specific libraries
# CRYOSAT Specific libraries
ifeq ($(PROJECT), Cryosat)

PROJECTFLAG     =
PROJECTEXTCOMP  = $(MCFSUBLIBS)
PROJECTDBLIB    = -lDbCryosat 
PROJECTAPP      = -lCryosatApp \
                  $(MCFLIB)
ALLLIBS = $(LIB_INTEGRATION_PATH)/lib/*.a
endif

# GOCE Specific libraries
ifeq ($(PROJECT), Goce)

PROJECTFLAG     =
PROJECTEXTCOMP  = $(MCFSUBLIBS)
PROJECTDBLIB    = -lDbCryosat 
PROJECTAPP      = -lCryosatApp \
                  $(MCFLIB)
ALLLIBS = $(LIB_INTEGRATION_PATH)/lib/*.a

endif

# ADM Specific libraries
ifeq ($(PROJECT), ADM)

PROJECTFLAG     =
PROJECTEXTCOMP  = $(MCFSUBLIBS)
PROJECTDBLIB    = -lDbCryosat 
PROJECTAPP      = -lCryosatApp \
                  $(MCFLIB)
ALLLIBS = $(LIB_INTEGRATION_PATH)/lib/*.a
endif

# GOCELTA Specific libraries
ifeq ($(PROJECT), GoceLTA)

PROJECTFLAG     =
PROJECTEXTCOMP  = $(MCFSUBLIBS)
PROJECTDBLIB    = -lDbCryosat 
PROJECTAPP      = -lCryosatApp \
                  $(MCFLIB)

ALLLIBS = $(LIB_INTEGRATION_PATH)/lib/*.a
endif

# GOCECMF Specific libraries
ifeq ($(PROJECT), Gocecmf)

PROJECTFLAG     =
PROJECTEXTCOMP  = $(MCFSUBLIBS)
PROJECTDBLIB    = -lDbCryosat 
PROJECTAPP      = -lCryosatApp \
                  $(MCFLIB)

ALLLIBS = $(LIB_INTEGRATION_PATH)/lib/*.a
endif


# COSMO Specific libraries
ifeq ($(PROJECT), Cosmo)

PROJECTFLAG     = -DCOSMO
PROJECTEXTCOMP  = -lCMAPI
PROJECTDBLIB    = -lDbCosmoExtended\
				  -lDbCosmo
PROJECTAPP      = \
                -lCosmoApp \
                -lCosmoLog \
                -lOSMHandler \
                -lCMWrapper \

ALLLIBS = $(LIB_INTEGRATION_PATH)/lib/*.a
endif


# CompactStation Specific libraries
ifeq ($(PROJECT), CompactStation)
PROJECTFLAG     = -DENVISAT
PROJECTEXTCOMP  = $(MCFSUBLIBS)
PROJECTDBLIB    = -lDbCryosat
PROJECTAPP      = -lCryosatApp \
                  $(MCFLIB)
ALLLIBS = $(LIB_INTEGRATION_PATH)/lib/*.a
endif

# CRYOSAT GTS Specific libraries
ifeq ($(PROJECT), CryoGTS)

PROJECTFLAG     =
PROJECTEXTCOMP  = $(MCFSUBLIBS)
PROJECTDBLIB    = -lDbCryosat 
PROJECTAPP      = -lCryosatApp \
                  $(MCFLIB)
ALLLIBS = $(LIB_INTEGRATION_PATH)/lib/*.a
endif

ifeq ($(PROJECT), Harm)

PROJECTFLAG     =
PROJECTEXTCOMP  = $(MCFSUBLIBS)
PROJECTDBLIB    = -lDbHarm
PROJECTAPP      = -lHarmApp \
                  $(MCFLIB)

ALLLIBS = $(LIB_INTEGRATION_PATH)/shared/*.so
endif

###################################
#  MdpsPorting Specific libraries #
###################################
ifeq ($(PROJECT), MdpsPorting)

PROJECTFLAG     = -DSTANDALONE
PROJECTEXTCOMP  = 
PROJECTDBLIB    =  
PROJECTAPP      = -lStandaloneApp 

ALLLIBS = $(LIB_INTEGRATION_PATH)/shared/*.so

endif



USERFLAG = $(PROJECTFLAG)

  LIBRARYNAME = GarbageCleaner

  VERSIONFILE =  GCappV.C


   UIC_SOURCES= \


   C++SOURCES = \
				GarbageCleanerApp.C \
				GarbageCleaner.C \
				GCappV.C \
		
#prevent remotion of objects, target and versioning
      VERSIONRULE =
      TARGETFILES = GarbageCleaner
    CLEANTESTRULE =

$(TARGETFILES):  $(OBJECTS) $(ALLLIBS)
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS)


cleanspecific:

helpspecific:

include ../../common/commonrules.mk

# DO NOT DELETE
