#
#	Copyright 1995-2020, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. libIsmSoap$
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.10  2012/02/22 09:42:10  marpas
#	no more warning in compilation
#	
#	Revision 1.9  2011/04/07 09:43:47  lucio.pulvirenti
#	Soap common stuff moved in libBaseSoap module
#	
#	Revision 1.8  2008/07/04 16:31:11  paoscu
#	Enhanced
#	
#	Revision 1.7  2008/06/16 17:20:15  paoscu
#	Makefile modified to work with parallel execution
#	
#	Revision 1.6  2008/05/07 17:43:29  paoscu
#	Interfaces changed
#	
#	Revision 1.5  2008/05/07 13:46:35  paoscu
#	Work in progress
#	
#	Revision 1.4  2008/05/06 19:19:24  paoscu
#	Now the libary include only the common code.
#	
#	Revision 1.3  2008/05/05 17:29:44  paoscu
#	Fixed includes
#	
#	Revision 1.2  2008/05/05 17:10:41  paoscu
#	Fixed install
#	
#	Revision 1.1.1.1  2008/05/05 16:54:47  paoscu
#	Imported into CVS
#	
#        
#

include ../../common/commondefs.mk

   LIBRARYNAME = libIsmSoap

   VERSIONFILE = IsmSoapV.C
   
   USERFLAG = $(PROJECTFLAG) $(OTHERFLAGS)

.PHONY : COMMON_OBJECTS
   CUSTOM_INCLUDEPATH +=  -IinterfaceFiles

   C++SOURCES = \
		IsmSoapV.C  \
		IsmServiceMessages.C \
		IsmService.C

   CPPSOURCES = \
		interfaceFiles/acsC.cpp \
		interfaceFiles/acsIsmProxy.cpp \
		interfaceFiles/acsIsmService.cpp \


   C++INCLUDE =  \
     soapIsm.h \
     IsmServiceMessages.h \
     interfaceFiles/acsIsmProxy.h \
     interfaceFiles/acsH.h \
     interfaceFiles/acsIsmService.h \
     interfaceFiles/acsStub.h \
     interfaceFiles/acs.nsmap

   COV_PATHS := $(shell pwd)/test

   COV_BASE_DIR := $(shell pwd)



ifneq (,$(findstring EXPRIVIA_ISM_USE_AWS_S3,$(USERFLAG)))
soapIsm.h: soapIsmAdd.h
soapIsmAdd.h: soapIsmExtCacheAdd.h
	cp soapIsmExtCacheAdd.h soapIsmAdd.h ; 
else
soapIsm.h: soapIsmAdd.h 
soapIsmAdd.h: soapIsmStdAdd.h
	cp soapIsmStdAdd.h soapIsmAdd.h; 
endif

interfaceFilesEntries := \
                 interfaceFiles/acs.nsmap \
				 interfaceFiles/acsH.h \
				 interfaceFiles/acsC.cpp  \
				interfaceFiles/acsIsmProxy.cpp \
				interfaceFiles/acsIsmService.cpp \
				interfaceFiles/acsIsmProxy.h \
				interfaceFiles/acsIsmService.h \
				interfaceFiles/acsStub.h



.PHONY : interfaceFilesEntries

interfaceFiles/timestamp :  soapIsm.h 
	@if [ ! -d interfaceFiles ] ; then echo "creating directory interfaceFiles" ; mkdir interfaceFiles ; fi
	@if cd interfaceFiles ; then \
		if soapcpp2 -I/usr/share/gsoap/import/ -i -n -pIsm ../soapIsm.h ; then \
			touch timestamp ; \
		else \
			echo "Failure in soapcpp2 execution" ; \
			rm -f timestamp ; \
			cd .. ; \
			exit 1 ; \
		fi ; \
		cd .. ; \
	else \
		echo "cannot enter int directory interfaceFiles" ; \
		exit 1 ; \
	fi ;


$(interfaceFilesEntries) : interfaceFiles/timestamp


IsmServiceMessages.o : interfaceFiles/acsH.h
interfaceFiles/acsC.o: CXXAUXOPTS = 
interfaceFiles/acsIsmService.o: CXXAUXOPTS =
interfaceFiles/acsIsmProxy.o: CXXAUXOPTS =

depend: interfaceFiles/timestamp

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	rm -fr interfaceFiles soapIsmAdd.h;


#empty rule, do not remove, you can add specific help strings
helpspecific:


# DO NOT DELETE

include ../../common/commonrules.mk

