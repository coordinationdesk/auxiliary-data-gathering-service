#
#	Copyright 1995-2012, Advanced Computer Systems , Inc.
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

include ../../common/commondefs.mk

  LIBRARYNAME = Importer

  VERSIONFILE = ImporterVersion.C


  C++SOURCES = \
    ImporterApp.C \
    ImporterVersion.C


  SPECIFICLIBS = \
	-lxerces-c \
	-lpthread \
	-ldl \


  TARGETFILES = Importer
    
    
$(TARGETFILES):  $(OBJECTS)
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS) $(SPECIFICLIBS)

cleanspecific:

helpspecific:


include ../../common/commonrules.mk

