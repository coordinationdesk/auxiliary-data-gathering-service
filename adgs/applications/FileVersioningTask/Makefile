#
#	Copyright 1995-2014, Advanced Computer Systems , Inc.
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
#	$Prod: $
#
#	$Id$
#
#	$ $
#
#	$Log$
#	
#	
#        
#

include ../../common/commondefs.mk

   USERFLAG = $(PROJECTFLAG)

   LIBRARYNAME = FileVersioningTask

   VERSIONFILE = FileVersioningTaskV.C

   CUSTOM_INCLUDEPATH = 

   CUSTOM_LIBPATH =

   CSOURCES =	

   CINCLUDE =	

   C++SOURCES = \
	FileVersioningTask.C \
	FileVersioningTaskV.C


   C++INCLUDE = 

LIB = \
	-lApp \
	-lConfiguration \
	-lDbClient \
	-lDbCryosat \
	-lDbData \
	-lException \
	-lFileInventory \
	-lResources \
	$(PROJECTAPP) \

EXES =  \
	FileVersioningTask


INST_EXECUTABLE = $(EXES)

TARGETFILES = $(EXES)


$(TARGETFILES):  $(OBJECTS) 
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(LIB); 

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

