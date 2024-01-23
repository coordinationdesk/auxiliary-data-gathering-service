#
#	Copyright 1995-2002, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. Application Library $
#

include ../../common/commondefs.mk

  LIBRARYNAME =	libApp

  VERSIONFILE = laV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = AppResSet.C \
   				Application.C \
  				ApplicationDebug.C \
				ApplicationStatic.C \
  				ApplicationLock.C \
				ApplicationUuid.C \
		        StandaloneResourceSpace.C \
		        StandaloneResourceSpaceFactory.C \
				MCDefaultFilter.C \
				Main.C \
				laV.C

   C++INCLUDE = AppResSet.h \
   				ApplicationLock.h \
				ApplicationUuid.h \
   				Application.h \
   				ApplicationDebug.h \
   				StandaloneResourceSpace.h \
		        StandaloneResourceSpaceFactory.h \
				MCDefaultFilter.h
    

ifeq ($(ALLOWQTCOMPONENTS), 1)
	ifneq ($(PROJECT), cosmopf)
    		C++SOURCES := ApplicationQApp.C $(C++SOURCES)
    		C++INCLUDE := ApplicationQApp.h $(C++INCLUDE)
  			MOC_SOURCES = ApplicationQApp.h
	endif

    DSOSPECIFICDEPS = -lqt-mt -lpthread

    CXXAUXOPTS_QT = -Wall \
				 -Wnon-virtual-dtor \
				 -Wno-non-template-friend \
				 -Woverloaded-virtual \
				 -Wno-pmf-conversions \
				 -Wsign-promo 
				 
ApplicationQApp.o: CXXAUXOPTS=$(CXXAUXOPTS_QT)

moc_ApplicationQApp.o: CXXAUXOPTS=$(CXXAUXOPTS_QT)

else
    DSOSPECIFICDEPS = -lpthread
endif
   

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared -L$(QTDIR)/lib 




include ../../common/commonrules.mk


# DO NOT DELETE


