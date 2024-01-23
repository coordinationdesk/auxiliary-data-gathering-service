#
#  Copyright 2021, Exprivia SPA - DFDA-AS
#  Via Della Bufalotta, 378 - 00139 Roma - Italy
#  http://www.exprivia.it
#
#  All Rights Reserved.
#
#  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
#  the contents of this file may not be disclosed to third parties, copied or
#  duplicated in any form, in whole or in part, without the prior written
#  permission of Exprivia SPA
#
#  $Prod: CDPNotificationAgent $
#

include ../../common/commondefs.mk

USERFLAG = $(PROJECTFLAG)

LIBRARYNAME = MultiDownloadInventoryObject

VERSIONFILE = mdiV.C


C++SOURCES = MultiDownloadInventoryObject.C \
             mdiV.C

#prevent remotion of objects, target and versioning
VERSIONRULE =
TARGETFILES = MultiDownloadInventoryObject
# INST_EXECUTABLE = $(TARGETFILES)
CLEANTESTRULE=
SPECIFICLIBS= 


$(TARGETFILES) : $(OBJECTS)
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS) $(SPECIFICLIBS)


COV_PATHS := $(shell pwd)/test
COV_BASE_DIR := $(shell pwd)


include ../../common/commonrules.mk

cleanspecific:

helpspecific:


