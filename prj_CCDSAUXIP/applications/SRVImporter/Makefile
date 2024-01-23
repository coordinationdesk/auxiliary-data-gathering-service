#
#	Copyright 1995-2020, Exprivia SPA - DADF
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.acsys.it
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia SPA - DADF.
#
#	$Prod: A.C.S. libSRVImporterApp$
#        
#


include ../../common/commondefs.mk

  LIBRARYNAME = SRVImporter

  VERSIONFILE = SRVImporterAppV.C


  C++SOURCES = \
    SRVImporterApp.C \
    SRVImporterAppV.C

  SPECIFICLIBS = \
	-lxerces-c \
	-lpthread \
	-ldl \


  TARGETFILES = SRVImporter
CLEANTESTRULE =
    
    
$(TARGETFILES):  $(OBJECTS)
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS) $(SPECIFICLIBS)


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	
#empty rule, do not remove, you can add specific help strings
helpspecific:

include ../../common/commonrules.mk

