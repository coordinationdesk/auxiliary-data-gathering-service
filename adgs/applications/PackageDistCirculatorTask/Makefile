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
#	$Prod: A.C.S. libPackageDistCirculatorTask$
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.8  2013/06/18 16:40:44  nicvac
#	S2PDGS-405: Support for multiple item circulations in one datapackage.
#	
#	Revision 1.7  2013/04/16 18:18:42  nicvac
#	Soap Circulation if implemented.
#	
#	Revision 1.6  2013/03/15 17:27:32  nicvac
#	ADC-83: Synch and Asynch distribution.
#	
#	Revision 1.5  2013/01/22 13:33:06  nicvac
#	lib list updated.
#	
#	Revision 1.4  2012/12/12 16:31:41  nicvac
#	No JobOrder, new Distribution.
#	
#	Revision 1.3  2012/12/05 17:18:29  nicvac
#	libs list
#	
#	Revision 1.2  2012/11/27 14:45:48  nicvac
#	Implementation.
#	
#	Revision 1.1.1.1  2012/11/27 11:44:37  nicvac
#	Import PackageDistCirculatorTask
#	
#        
#

include ../../common/commondefs.mk

USERFLAG = $(PROJECTFLAG)

LIBRARYNAME = PackageDistCirculatorTask

VERSIONFILE = PackageDistCirculatorTaskV.C

CUSTOM_INCLUDEPATH =

CUSTOM_LIBPATH =

CSOURCES =	

CINCLUDE =	

C++SOURCES = PackageDistCirculatorTask.C \
	PackageDistCirculatorTaskV.C

UIC_SOURCES= 

MOC_SOURCES = 

C++INCLUDE = 

EXTCOMPLIBS = \
	-lboost_serialization \
	
EXES =  \
	PackageDistCirculatorTask


INST_EXECUTABLE = $(EXES)

TARGETFILES = $(EXES)


$(TARGETFILES):  $(OBJECTS) 
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(EXTCOMPLIBS); \
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

