#
#	Copyright 1995-2016, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. PackageDistCurlTask$
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.1.1.1  2016/01/15 09:47:48  nicvac
#	Import PackageDistCurlTask
#	
#        
#

include ../../common/commondefs.mk

USERFLAG = $(PROJECTFLAG)

LIBRARYNAME = PackageDistCurlTask

VERSIONFILE = PackageDistCurlTaskV.C

CUSTOM_INCLUDEPATH =

CUSTOM_LIBRARYPATH =

CSOURCES =	

CINCLUDE =	

C++SOURCES = PackageDistCurlTask.C \
	PackageDistCurlTaskV.C

UIC_SOURCES= 

MOC_SOURCES = 

C++INCLUDE = 

SPECIFICLIBS = \
	-lboost_serialization \


EXES =  \
	PackageDistCurlTask


INST_EXECUTABLE = $(EXES)

TARGETFILES = $(EXES)

$(TARGETFILES):  $(OBJECTS)
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS) $(SPECIFICLIBS)


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


