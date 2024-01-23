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
#	$Prod: A.C.S.PackageDistFilesystemTask$
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.1.1.1  2013/07/25 12:28:01  nicvac
#	Import PackageDistFilesystemTask
#	
#        
#

include ../../common/commondefs.mk

USERFLAG = $(PROJECTFLAG)

LIBRARYNAME = PackageDistFilesystemTask

VERSIONFILE = PackageDistFilesystemTaskV.C

CUSTOM_INCLUDEPATH = 

CUSTOM_LIBPATH =

CSOURCES =	

CINCLUDE =	

C++SOURCES = \
	PackageDistFilesystemTask.C \
	PackageDistFilesystemTaskV.C

UIC_SOURCES= 

MOC_SOURCES = 

C++INCLUDE = 

EXTCOMPLIBS = \
	-lboost_serialization



EXES =  \
	PackageDistFilesystemTask


INST_EXECUTABLE = $(EXES)

TARGETFILES = $(EXES)


$(TARGETFILES):  $(OBJECTS) 
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS) $(EXTCOMPLIBS)


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


