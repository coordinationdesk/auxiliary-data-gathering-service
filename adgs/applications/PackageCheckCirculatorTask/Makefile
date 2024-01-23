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
#	$Prod: A.C.S. PackageCheckCirculatorTask$
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.4  2016/09/08 09:40:12  danalt
#	removed PROJECTPROP
#	
#	Revision 1.3  2013/06/18 16:44:27  nicvac
#	S2PDGS-405: Support for multiple item circulations in one datapackage.
#	
#	Revision 1.2  2013/04/16 18:19:05  nicvac
#	Soap Circulation If used.
#	
#	Revision 1.1.1.1  2013/03/15 19:34:05  nicvac
#	Import PackageCheckCirculatorTask
#	
#

include ../../common/commondefs.mk

USERFLAG = $(PROJECTFLAG)

LIBRARYNAME = PackageCheckCirculatorTask

VERSIONFILE = PackageCheckCirculatorTaskV.C

CUSTOM_INCLUDEPATH =

CUSTOM_LIBPATH =

CSOURCES =	

CINCLUDE =	

C++SOURCES = PackageCheckCirculatorTask.C \
	PackageCheckCirculatorTaskV.C

UIC_SOURCES= 

MOC_SOURCES = 

C++INCLUDE = 

EXES =  \
	PackageCheckCirculatorTask


INST_EXECUTABLE = $(EXES)

TARGETFILES = $(EXES)


$(TARGETFILES):  $(OBJECTS) 
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS) $(DEPLIBS) $(EXTCOMPLIBS); \
cp ./Scripts/*-* $(EXE_INTEGRATION_PATH)


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

