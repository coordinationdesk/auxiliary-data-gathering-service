#
#	Copyright 1995-2007, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. dbExpress for Postgres Client Library test $
#
#	$Id$
#
#   $Log$
#   Revision 1.4  2012/02/06 10:03:03  marpas
#   removing compiler warning
#
#   Revision 1.3  2008/11/20 11:01:23  marpas
#   depends on libpq
#
#   Revision 1.2  2008/04/08 15:08:13  marpas
#   shared libraries search path added
#
#   Revision 1.1.1.1  2007/12/14 18:42:20  marpas
#   Import Postgres dbExpress driver
#
#
#

include ../../../common/commondefs.mk




#must be an absolute path for coverage 
CUSTOM_INCLUDEPATH = -I$(shell pwd)/..
#must be an absolute path for coverage 
CUSTOM_LIBRARYPATH = -L$(shell pwd)/..
#must be an absolute path to avoid linking the installed one
CUSTOM_LDOPTS = -Wl,-rpath,$(shell pwd)/..

OPTFLAG=-g
LIBS = \
		-lPlugin \
		-lException \
		-lm \
		-ldl \
		-lpthread 
		
EXES =  tPGLDriver \
		tDriver \
		tConnection \
		tCommand \
		tCursor 

C++SOURCES:= testCommon.C $(EXES:=.C)
		
#prevent remotion of objects, target and versioning
      CLEANOBJECTSRULE =
      CLEANTARGETSRULE =
      VERSIONRULE =
      ALLRULE = alltest

		

all: alltest
alltest: $(EXES)

#tPGLDriver: testCommon.o tPGLDriver.o 
$(EXES): %: %.o testCommon.o %.o.undefsym testCommon.o.undefsym
	$(TESTBUILDRECIPE)

#tDriver: tDriver.o testCommon.o 
#	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) testCommon.o  $@.o  -o $@ $(LIBS)

#tConnection: tConnection.o testCommon.o 
#	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) testCommon.o  $@.o  -o $@ $(LIBS)

#tCommand: tCommand.o testCommon.o 
#	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) testCommon.o  $@.o  -o $@ $(LIBS)

#tCursor: tCursor.o testCommon.o 
#	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) testCommon.o  $@.o  -o $@ $(LIBS)

#don't remove
cleanspecific:
	@rm -f core.* core 
	@rm -f $(EXES)
	@rm -f *.o *.log *.bck *.pure
	

#don't remove
helpspecific:

include ../../../common/commonrules.mk





