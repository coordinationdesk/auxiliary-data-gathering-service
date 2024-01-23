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
#	$Author$
#
#	$Prod: A.C.S. Rolling Algo Library$
#
#	$Id$
#
#   $Log $
#
#

include ../../common/commondefs.mk

#dbexpress compatibility mode (for Informix w/ SQLString)
#CUSTOMFLAGS:=-DDBEXPRESS_COMPAT $(CUSTOMFLAGS)


  LIBRARYNAME =	libRollingAlgo

  VERSIONFILE = rlV.C

     CSOURCES =	

     CINCLUDE =	

 C++SOURCES = \
  		rlV.C \
		InvPolicy.C \
		InvPolicySelectorFactory.C \
		InvPolicySelector.C \
		InvPolicyCommonSelector.C \
		TimeElapsedSelector.C \
		OnDistributionSelector.C \
		UsedSelector.C \
		ParametersBasedSelector.C \
		S2ParametersBasedSelector.C \
		S1ParametersBasedSelector.C \
		AsynchSmRestoredSizeSelector.C \
		StorageSizeSelector.C \
		RollingAlgoUtils.C \
			
  			
   C++INCLUDE = \
		InvPolicy.h \
		InvPolicySelectorFactory.h \
		InvPolicySelector.h \
		InvPolicyCommonSelector.h \
		TimeElapsedSelector.h \
		OnDistributionSelector.h \
		UsedSelector.h \
		ParametersBasedSelector.h \
		S2ParametersBasedSelector.h \
		S1ParametersBasedSelector.h \
		AsynchSmRestoredSizeSelector.h \
		StorageSizeSelector.h \
		RollingAlgoUtils.h \

CUSTOMFLAGS=-DACS_DBCLIENT_PREVENT_DEFAULTKEY

COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

# do not declare any dependency from other component library
DSOSPECIFICDEPS= 

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

# DO NOT DELETE
include ../../common/commonrules.mk

