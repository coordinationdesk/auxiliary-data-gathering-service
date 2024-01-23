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
#	$Prod: Sentinel3 Auxiliary Reformatter application $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	
#	

include ../../common/commondefs.mk

  USERFLAG    = $(PROJECTFLAG)

  LIBRARYNAME = S3AuxReformMetadataExtractor
	
  VERSIONFILE = S3AuxRefMEV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = S3AuxReformMetadataExtractor.C \
		S3AuxRefMEV.C
	
   UIC_SOURCES= 
	
   MOC_SOURCES = 

   C++INCLUDE = 


		
LIB =	$(PROJECTAPP) \
	-lOcadaJobOrder \
	-lApp \
	-lException \
	-lm 

#prevent remotion of objects, target and versioning
      VERSIONRULE =
      TARGETFILES = S3AuxReformMetadataExtractor
      CLEANTESTRULE =
	

$(TARGETFILES):  $(OBJECTS) 
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@ $(OBJECTS) $(LIB)


CLEANTESTRULE =
TESTCOMPILERULE=
DEPENDTESTRULE=
CLEANDEPENDTEST=

#include linuxcommonrules.mk

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

include ../../common/commonrules.mk

# DO NOT DELETE
