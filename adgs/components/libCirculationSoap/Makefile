#
#	Copyright 1995-2021, Exprivia SpA.
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.exprivia.com
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia SpA, Inc.
#
#	$Author$
#
#	$Prod: libCirculationSoap $
#	$Id$
#
#   $Log$
#   Revision 1.5  2013/07/11 18:31:10  marpas
#   work in progress
#
#   Revision 1.4  2013/04/16 18:18:13  nicvac
#   Exported the headers for soap clients.
#
#   Revision 1.3  2012/02/09 16:29:21  marpas
#   refactoring,
#   SENT-12 implemented
#   removing compiler warnings
#
#   Revision 1.2  2011/06/16 14:29:58  crivig
#   testing..
#
#   Revision 1.1.1.1  2011/05/31 13:51:16  crivig
#   imported new library
#


include ../../common/commondefs.mk

  PROJECTFLAG = -D$(shell echo $(PROJECT) | tr a-z A-Z)
  	
  USERFLAG += $(PROJECTFLAG)

  LIBRARYNAME =	libCirculationSoap

  VERSIONFILE = lCSoapV.C

  CUSTOM_INCLUDEPATH+=-Igenerated


   C++SOURCES = \
				CirculationSoap.C \
				lCSoapV.C

   C++INCLUDE = \
				CirculationSoap.h \
				generated/CirculationH.h \
				generated/CirculationStub.h \
				generated/CirculationPDS_USCOREOrderingBindingService.h \
				generated/CirculationPDS_USCOREOrderingBindingProxy.h \
   
   CPPSOURCES = \
			   generated/CirculationPDS_USCOREOrderingBindingService.cpp \
			   generated/CirculationPDS_USCOREOrderingBindingProxy.cpp \
			   generated/CirculationC.cpp \
			   

############ gsoap generation ###############################

CirculationSoapGenerated.h:
	wsdl2h -z2 -qCirculation -t typemap.dat -I/usr/share/gsoap/import/ -o CirculationSoapGenerated.h  pdsCirculation.xsd  pdsCirculation.wsdl   ;
	



generatedFiles := \
		generated/Circulation.nsmap \
		generated/CirculationC.cpp \
		generated/CirculationH.h \
		generated/CirculationStub.h \
		generated/CirculationPDS_USCOREOrderingBindingService.h \
		generated/CirculationPDS_USCOREOrderingBindingProxy.h \
		generated/CirculationPDS_USCOREOrderingBindingProxy.cpp \
		generated/CirculationPDS_USCOREOrderingBindingService.cpp
		



.PHONY : generatedFiles


generated/timestamp :  CirculationSoapGenerated.h 
	@if [ ! -d generated ] ; then echo "creating directory generated" ; mkdir generated ; fi
	@if cd generated ; then \
		if soapcpp2 -I/usr/share/gsoap/import/  -i -n ../CirculationSoapGenerated.h ; then \
			touch timestamp ; \
		else \
			echo "Failure in soapcpp2 execution" ; \
			rm -f timestamp ; \
			cd .. ; \
			exit 1 ; \
		fi ; \
		cd .. ; \
	else \
		echo "cannot enter into directory generated" ; \
		exit 1 ; \
	fi ;


$(generatedFiles) : generated/timestamp 

CirculationSoap.o: $(generatedFiles)


############ gsoap generation ###############################

depend: generated/timestamp



DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 


DSODEPENDLIBS = 

DSOSPECIFICDEPS =

COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

generated/CirculationC.o: CXXAUXOPTS = 
generated/CirculationPDS_USCOREOrderingBindingProxy.o: CXXAUXOPTS =
generated/CirculationPDS_USCOREOrderingBindingService.o: CXXAUXOPTS =
cleanspecific:
	rm -fr generated
	rm -fr CirculationSoapGenerated.h

#empty rule, do not remove, you can add specific help strings
helpspecific:


include ../../common/commonrules.mk


# DO NOT DELETE

