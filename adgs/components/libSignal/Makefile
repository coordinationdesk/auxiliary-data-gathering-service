#
#	Copyright 1995-2012, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. DataBase Client Library $
#
#	$Id$
#
#   $Log $
#
#

include ../../common/commondefs.mk

  LIBRARYNAME =	libSignal

  VERSIONFILE = SignalV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
        RecvTimeoutPipeTrick.C \
        SignalDispatcher.C\
        SigalrmEmulator.C\
        SigMaskManagerSingleton.C\
        SigMaskGuard.C\
        SignalV.C

   C++INCLUDE = \
        RecvTimeoutPipeTrick.h \
        SignalDispatcher.h\
        SigalrmEmulator.h\
        SigMaskManagerSingleton.h\
        SigMaskGuard.h



COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)


# DO NOT DELETE
#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

DSOSPECIFICDEPS = -lException  -lThread  -lTimer 


include ../../common/commonrules.mk

