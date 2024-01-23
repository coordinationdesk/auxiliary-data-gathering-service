#
#	Copyright 1995-2008, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. TLPreInventory$
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 2.1  2016/11/22 10:04:33  marfav
#	Removed dependency from libSendMessage
#	
#	Revision 2.0  2016/11/21 16:18:18  marfav
#	Major release
#	
#	Revision 1.9  2014/04/08 14:26:46  matteo.airoldi
#	Added TimeConverter includes
#	
#	Revision 1.8  2013/04/24 10:46:03  chicas
#	updated LIB
#	
#	Revision 1.7  2012/02/28 14:27:17  marpas
#	no more warnings from Makefile
#	
#	Revision 1.6  2011/07/18 16:09:20  marfav
#	Added time convertion wrapper variable
#	
#	Revision 1.5  2010/10/20 14:33:59  marpas
#	no include files must be installed
#	
#	Revision 1.4  2009/03/05 13:31:13  nicvac
#	Lib deps fixed.
#	
#	Revision 1.3  2008/12/19 17:05:12  nicvac
#	Inventory working dir moving.
#	
#	Revision 1.2  2008/11/10 16:01:52  marfav
#	Makefile fixed
#	
#	Revision 1.1.1.1  2008/10/17 14:21:04  nicvac
#	Import TLPreInventory
#	
#        
#

include ../../common/commondefs.mk

   LIBRARYNAME = TLPreInventory

   VERSIONFILE = TLPreInventoryAppV.C

   CUSTOM_INCLUDEPATH =

   CUSTOM_LIBPATH =

   CSOURCES =	

   CINCLUDE =	

   C++SOURCES = \
	TLPreInventoryApp.C \
	TLPreInventoryAppV.C

   UIC_SOURCES= 
	
   MOC_SOURCES = 


   C++INCLUDE = 

   EXTCOMPLIBS = \
	-lboost_program_options \
	-lxerces-c \
	-lpthread \
	-ldl \


EXES =  \
	TLPreInventory


INST_EXECUTABLE = $(EXES)

TARGETFILES = $(EXES)


SPECIFICLIBS= \
    $(PROJECT_TIMECONV_WRAP) \
    $(PROJECT_ORBPROPAG_WRAP) \
    $(PROJECT_ORBITPROPAGATOR)\
    $(PROJECT_TIMECONVERTER) \
    $(EXTCOMPLIBS)



$(TARGETFILES) : $(OBJECTS)
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

