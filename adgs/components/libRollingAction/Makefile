#
#	Copyright 1995-2004, Advanced Computer Systems , Inc.
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

  LIBRARYNAME =	libRollingAction

  VERSIONFILE = rlaV.C

     CSOURCES =	

     CINCLUDE =	
     
     C++SOURCES = \
	rlaV.C \
	RollingAction.C \
	InvPolicyPerformerFactory.C \
	InvPolicyPerformer.C \
	ActionPerformer.C \
	KeepPerformer.C \
	RemovePerformer.C \
	InvalidPerformer.C \
	CleanStoragePerformer.C \
	InvalidateParentRemoveChildrenPerformer.C \
	RemoveParentRemoveChildrenPerformer.C \
	AutoParentRemoveChildrenPerformer.C \
	RollingReport.C \
	RollingReportGenerator.C \
	RollingReportGeneratorSingleton.C \
			
			
			
   C++INCLUDE = \
	RollingAction.h \
	InvPolicyPerformerFactory.h \
	InvPolicyPerformer.h \
	ActionPerformer.h \
	KeepPerformer.h \
	RemovePerformer.h \
	InvalidPerformer.h \
	CleanStoragePerformer.h \
	InvalidateParentRemoveChildrenPerformer.h \
	RemoveParentRemoveChildrenPerformer.h \
	AutoParentRemoveChildrenPerformer.h \
	RollingReport.h \
	RollingReportGenerator.h \
	RollingReportGeneratorSingleton.h \


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

