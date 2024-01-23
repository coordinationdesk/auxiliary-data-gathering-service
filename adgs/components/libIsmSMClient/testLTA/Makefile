#
#	Copyright 2020-2021, Exprivia SpA
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.acsys.it
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia SpA
#
#	$Author$
#
#	$Prod: A.C.S. LTA File Action Test Suite $
#
#

include ../../../common/commondefs.mk

#must be an absolute path for coverage 
CUSTOM_INCLUDEPATH = -I$(shell pwd)/..
#must be an absolute path for coverage 
CUSTOM_LIBRARYPATH = -L$(shell pwd)/..
#must be an absolute path to avoid linking the installed one
CUSTOM_LDOPTS = -Wl,-rpath,$(shell pwd)/..


EXES = TestISMStorageManager\
	tISMStorageManagerIfs

C++SOURCES= \
	$(EXES:=.C)

OTHERFLAGS+=-DACS_TEST

VERSIONRULE = 
ALLRULE = alltest
TESTCOMPILERULE= 
CLEANDEPENDTEST=
DEPENDTESTRULE=


SPECIFICLIBS= -lcppunit -lIsmSMClient

all: alltest

alltest: $(EXES)

$(EXES): %: %.o.undefsym %.o
	$(TESTBUILDRECIPE)
	
cleanspecific:
	@rm -f $(EXES)	
	@rm -f core *.o *.log *.bck *.pure

#empty rule, do not remove, you can add specific help strings
helpspecific:

#no nested test
CLEANTESTRULE =


include ../../../common/commonrules.mk

