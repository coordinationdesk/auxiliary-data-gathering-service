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
#	$Prod: $
#	$Id$
#
#
#

include ../../common/commondefs.mk

  USERFLAG = $(PROJECTFLAG)

  LIBRARYNAME = DownloadInventoryFile

  VERSIONFILE = dfV.C



   C++SOURCES = \
   		dfV.C \
		DownloadFileClass.C \
		DownloadInventoryFile.C

EXES =  \
	DownloadInventoryFile
	

#prevent remotion of objects, target and versioning
      VERSIONRULE =
      ALLRULE = alltest
      TARGETFILES=$(EXES)
      CLEANTESTRULE=


$(TARGETFILES) : $(OBJECTS)
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS)


cleanspecific:

helpspecific:

include ../../common/commonrules.mk

