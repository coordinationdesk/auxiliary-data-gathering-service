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
#	$Author$
#
#	$Id$
#
#   $Log$
#   Revision 1.18  2013/02/28 16:53:36  enrcar
#   EC:: versioning renamed
#
#   Revision 1.17  2013/02/26 15:12:25  enrcar
#   EC:: Application updated
#
#  

include ../../common/commondefs.mk

  USERFLAG = $(PROJECTFLAG)

  LIBRARYNAME = MultiRemoveInventoryObject

  VERSIONFILE = miV.C

   C++SOURCES = \
		miV.C \
		MultiRemoveInventoryObject.C \

EXES =  \
	MultiRemoveInventoryObject \
	

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

