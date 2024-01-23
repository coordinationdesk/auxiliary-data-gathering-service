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
#	$Prod: A.C.S. PackageFmtEOFpackTask$
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.4  2016/02/25 11:49:00  nicvac
#	Wrapper scripts
#	
#	Revision 1.3  2016/02/24 17:03:30  nicvac
#	OPMAN-151: Implementation completed.
#	
#	Revision 1.2  2013/01/22 13:33:29  nicvac
#	Implemented
#	
#	Revision 1.1.1.1  2013/01/21 16:33:30  nicvac
#	Import PackageFmtEOFpackTask
#	
#	Revision 1.1.1.1  2013/01/21 15:35:53  nicvac
#	Import PackageFmtEOFpackTask
#	
#        
#

include ../../common/commondefs.mk

USERFLAG = $(PROJECTFLAG)

LIBRARYNAME = PackageFmtEOFpackTask

VERSIONFILE = PackageFmtEOFpackTaskV.C

CUSTOM_INCLUDEPATH =

CUSTOM_LIBPATH =

CSOURCES =	

CINCLUDE =	

C++SOURCES = PackageFmtEOFpackTask.C \
	PackageFmtEOFpackTaskV.C

UIC_SOURCES= 

MOC_SOURCES = 

C++INCLUDE = 

EXTCOMPLIBS = \
	-lboost_serialization \
	-lboost_program_options \

EXES =  \
	PackageFmtEOFpackTask


INST_EXECUTABLE = $(EXES)

TARGETFILES = $(EXES)


$(TARGETFILES):  $(OBJECTS) 
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS) $(EXTCOMPLIBS); \
cp ./Scripts/PackageFmtEOFpackTask* $(EXE_INTEGRATION_PATH)


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:


CLEANTESTRULE =
TESTCOMPILERULE=
DEPENDTESTRULE=
CLEANDEPENDTEST=

# DO NOT DELETE

include ../../common/commonrules.mk


