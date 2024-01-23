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
#	Authors: 
#
#	$Prod: A.C.S. Sentinel-3 ADF reformatter library $
#	$Id$
#
#   $Log$
#   Revision 1.1.1.1  2013/06/10 12:44:59  marfav
#   importing libS3AuxFormatter
#
#

include ../../common/commondefs.mk

  PROJECTFLAG = -D$(shell echo $(PROJECT) | tr a-z A-Z)
  	
  USERFLAG += $(PROJECTFLAG)

  LIBRARYNAME =	libS3AuxFormatter

  VERSIONFILE = s3afV.C

   C++SOURCES = \
				S3AuxManifest.C \
				s3afV.C

   C++INCLUDE = \
			S3AuxManifest.h \

COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

DSODEPENDLIBS = -lXMLResources -lException

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

# DO NOT DELETE
include ../../common/commonrules.mk



