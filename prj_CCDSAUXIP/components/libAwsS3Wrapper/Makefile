#
#	Copyright 2015-2022, Exprivia S.p.A. - DFDA-AS
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.acsys.it
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia S.p.A. ;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia S.p.A.
#
#
#	$Prod: A.C.S. S3Wrapper library $
#
#	$Author$
#
#	$Id$
#
#	$Log$
#

include ../../common/commondefs.mk

  PROJECTFLAG = -D$(shell echo $(PROJECT) | tr a-z A-Z)
  	
  USERFLAG += $(PROJECTFLAG) $(OTHERFLAGS)

  CUSTOM_INCLUDEPATH := $(CUSTOM_INCLUDEPATH) 
  CUSTOM_LIBRARYPATH := $(CUSTOM_LIBRARYPATH) 

  # This force to compile all test after the Library has been compiled
  POSTCOMPILERULE += test

  LIBRARYNAME =	libAwsS3Wrapper

  VERSIONFILE = s3V.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
			AwsS3WrapperConfiguration.C \
			AwsS3WrapperConfigurationJSON.C \
			AwsS3Wrapper.C \
			s3V.C

   C++INCLUDE = \
			AwsS3WrapperConfiguration.h \
			AwsS3WrapperConfigurationJSON.h \
			AwsS3Wrapper.h 
			
COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

AWS_S3_LIBS = -laws-cpp-sdk-core -laws-cpp-sdk-s3 -laws-cpp-sdk-transfer

DSOSPECIFICDEPS = $(AWS_S3_LIBS)


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

# DO NOT DELETE
include ../../common/commonrules.mk



