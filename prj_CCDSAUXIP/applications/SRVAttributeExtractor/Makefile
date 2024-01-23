#
#    Copyright 2020, Exprivia - DFDA-AS
#    Via Della Bufalotta, 378 - 00139 Roma - Italy
#    http://www.exprivia.it
#
#    All Rights Reserved.
#
#   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
#    the contents of this file may not be disclosed to third parties, copied or
#    duplicated in any form, in whole or in part, without the prior written
#    permission of Exprivia SpA
#
#    $Prod: SRV Attribute Extractor $

include $(PRJROOTTREE)/common/commondefs.mk

LIBRARYNAME = SRVAttributeExtractor

VERSIONFILE = SRVExtractorV.C


C++SOURCES = 	SRVAttributeExtractor.C \
   		SRVExtractorV.C

COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

VERSIONRULE =
TARGETFILES = SRVAttributeExtractor
CLEANTESTRULE=

$(TARGETFILES) : $(OBJECTS)
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS) $(SPECIFICLIBS)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

include $(PRJROOTTREE)/common/commonrules.mk

