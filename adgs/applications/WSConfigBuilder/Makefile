#
#	Copyright 1995-2017, Advanced Computer Systems , Inc.
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
#	$Prod: $
#
#	$Id$
#
#	$Author$
#
#
#

include ../../common/commondefs.mk

USERFLAG = $(PROJECTFLAG)

  VERSIONFILE = wscbV.C

     CSOURCES =	

     CINCLUDE =	

  LIBRARYNAME = WSConfigBuilder

   C++SOURCES = \
		WSConfigBuilder.C\
		wscbV.C\


		
   UIC_SOURCES= 
	
   MOC_SOURCES = 

   C++INCLUDE = \


#prevent remotion of objects, target and versioning
      VERSIONRULE =
      TARGETFILES = \
		  WSConfigBuilder \
		  

$(TARGETFILES) : $(OBJECTS)
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS) -lboost_program_options

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	@rm -f CM.log

helpspecific:

CLEANTESTRULE =
TESTCOMPILERULE=
DEPENDTESTRULE=
CLEANDEPENDTEST=


include ../../common/commonrules.mk


# DO NOT DELETE

