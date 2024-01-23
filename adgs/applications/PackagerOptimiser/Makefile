#
#	Copyright 1995-2021, Exprivia SpA - DADF
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.exprivia.it
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia SpA
#
#	Authors: 
#
#	$Prod$
#
#	$Name$
#
#	$Id$
#
#
 

include ../../common/commondefs.mk

  USERFLAG = $(PROJECTFLAG)

  LIBRARYNAME = PackagerOptimiser

  VERSIONFILE = PackagerOptimiserAppV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = PackagerOptimiserApp.C \
   		PackagerOptBroadcastListener.C \
                PackagerOptimiserAppV.C
   
   C++INCLUDE = 
   				
#prevent remotion of objects, target and versioning
      TARGETFILES = PackagerOptimiser
      VERSIONRULE =
      CLEANTESTRULE=

$(TARGETFILES) : $(OBJECTS)
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	
#empty rule, do not remove, you can add specific help strings
helpspecific:

include ../../common/commonrules.mk
