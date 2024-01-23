#
#	Copyright 1995-2020, Advanced Computer Systems , Inc.
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
#   $Prod: SRV Inventory application $
#
#

include ../../common/commondefs.mk


  USERFLAG = $(PROJECTFLAG)

	LIBRARYNAME = SRVInventory

	VERSIONFILE = srvinvV.C


	C++SOURCES = \
		SRVInventory.C \
		srvinvV.C



#prevent remotion of objects, target and versioning
	VERSIONRULE =
	TARGETFILES = SRVInventory
	CLEANTESTRULE=


SPECIFICLIBS= 


INSTALLSPECIFICRULE = installspecific 

$(TARGETFILES) : $(OBJECTS)
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS) $(SPECIFICLIBS)

cleanspecific:

helpspecific:

include ../../common/commonrules.mk


installspecific:
	cp ./*.sh  $(EXE_INTEGRATION_PATH)


