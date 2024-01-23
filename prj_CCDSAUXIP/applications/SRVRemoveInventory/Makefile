#
#	Copyright 2020, Exprivia SpA
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.exprivia.com
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Advanced Computer Systems, Inc.
#
#   SRVRemoveInventory
#
#   Author: Marco Pastori

include ../../common/commondefs.mk

  USERFLAG = $(PROJECTFLAG)

  LIBRARYNAME = SRVRemoveInventory

  VERSIONFILE = sriV.C

   C++SOURCES = \
   				sriV.C \
				SRVRemoveInventory.C \
				
#prevent remotion of objects, target and versioning
	VERSIONRULE =
	TARGETFILES = SRVRemoveInventory
	CLEANTESTRULE=


SPECIFICLIBS= 



$(TARGETFILES) : $(OBJECTS)
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS) $(SPECIFICLIBS)

cleanspecific:

helpspecific:

include ../../common/commonrules.mk
