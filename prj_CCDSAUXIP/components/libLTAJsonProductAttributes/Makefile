#
#	Copyright 1995-2019, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. libLTAJsonProductAttributes$
#
#	$Id$
#
#	$Author$

include ../../common/commondefs.mk

  LIBRARYNAME =	libLTAJsonProductAttributes

  VERSIONFILE = LTAJsonProductAttributesV.C

   C++SOURCES = \
		  LTAUtilities.C \
                  LTAJsonProductAttributes.C \
		  LTAProductExtractor.C  \
                  LTAJsonProductAttributesV.C \

   CUSTOMFLAGS = -DENABLE_MY_CUSTOMFLAG

# add here only the header files to be exported to other libraries
   C++INCLUDE = \
                  LTAJsonProductAttributes.h \
		  LTAProductExtractor.h  \
		  LTAUtilities.h \


COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

# do not declare any dependency from other component library
DSOSPECIFICDEPS=-lnetcdf_c++4


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

# DO NOT DELETE
include ../../common/commonrules.mk
