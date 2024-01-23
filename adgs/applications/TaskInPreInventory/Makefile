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
#	$Prod: $
#
#	$Id$
#
#	$ $
#
#	$Log$
#	Revision 2.2  2017/03/28 08:10:56  clanas
#	changed to have PRJ and SPECIFIC libs
#	
#	Revision 2.1  2017/03/23 11:41:25  clanas
#	modified to use new mechanism dependency
#	
#	Revision 2.0  2016/11/22 09:58:18  marfav
#	Major release
#	
#	Revision 1.4  2016/11/22 09:57:58  marfav
#	S3MPM-847
#	Adding functions to dump plugin capabilities
#	Removed dependency from libSendMessage
#	
#	Revision 1.3  2014/04/08 14:25:20  matteo.airoldi
#	Added TimeCOnverter includes
#	
#	Revision 1.2  2013/04/24 10:45:16  chicas
#	updated LIB
#	
#	Revision 1.1.1.1  2012/11/30 12:44:54  chicas
#	Imported sources
#	
#	Revision 1.1.1.1  2012/10/17 10:07:58  chicas
#	Imported sources
#	
#	
#        
#

include ../../common/commondefs.mk

   LIBRARYNAME = TaskInPreInventory

   VERSIONFILE = TaskInPreInventoryAppV.C

   CUSTOM_INCLUDEPATH =

   CUSTOM_LIBPATH =

   CSOURCES =	

   CINCLUDE =	

   C++SOURCES = \
	TaskInPreInventoryApp.C \
	TaskInPreInventoryAppV.C

   UIC_SOURCES= 
	
   MOC_SOURCES = 


   C++INCLUDE = 


EXES =  \
	TaskInPreInventory


INST_EXECUTABLE = $(EXES)

TARGETFILES = $(EXES)


SPECIFICLIBS= \
    $(PROJECT_TIMECONV_WRAP) \
    $(PROJECT_ORBPROPAG_WRAP) \
    $(PROJECT_ORBITPROPAGATOR)\
    $(PROJECT_TIMECONVERTER) \
	-lboost_program_options



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

