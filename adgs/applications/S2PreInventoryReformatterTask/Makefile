#
#	Copyright 2019, Advanced Computer Systems , Inc.
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
#
#


include ../../common/commondefs.mk



  LIBRARYNAME = S2PreInventoryReformatterTask

  VERSIONFILE = S2PreInventoryReformatterTaskV.C


   C++SOURCES = \
   	S2PreInventoryReformatterTask.C \
	S2PreInventoryReformatterTaskV.C

#prevent remotion of objects, target and versioning
      VERSIONRULE =
      TARGETFILES = S2PreInventoryReformatterTask
      CLEANTESTRULE=


$(TARGETFILES):  $(OBJECTS)
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(SPECIFICLIBS) $(PRJLIBS)
	
cleanspecific:

helpspecific:

include ../../common/commonrules.mk

