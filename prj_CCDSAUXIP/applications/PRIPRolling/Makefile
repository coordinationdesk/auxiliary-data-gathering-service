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
#   $Prod: LTAQBackEnd $
#
#

include ../../common/commondefs.mk


  USERFLAG = $(PROJECTFLAG)

  LIBRARYNAME = PRIPRolling

  VERSIONFILE = priprollV.C


  C++SOURCES = \
	PRIPRolling.C \
	priprollV.C



#prevent remotion of objects, target and versioning
      VERSIONRULE =
      TARGETFILES = PRIPRolling
      CLEANTESTRULE=

SPECIFICLIBS= 


$(TARGETFILES) : $(OBJECTS)
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS) $(SPECIFICLIBS)

cleanspecific:

helpspecific:

include ../../common/commonrules.mk
