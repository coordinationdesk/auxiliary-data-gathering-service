#
#	Copyright 1995-2021, Exprivia SpA.
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.exprivia.com
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of AExprivia SpA;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia SpA.
#
#	$Prod: A.C.S. libCirculationCallback$
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.3  2016/08/31 08:38:23  damdec
#	Compilation problems fixed
#	
#	Revision 1.2  2016/08/30 08:57:41  damdec
#	Files automatically generated removed.
#	
#	Revision 1.1.1.1  2016/08/29 08:41:32  marfav
#	Importing libCirculationCallback
#	
#        
#

include ../../common/commondefs.mk

LIBRARYNAME = libCirculationCallback

VERSIONFILE = CirculationCallbackV.C

CUSTOM_INCLUDEPATH += -I$(shell pwd)/generated

COV_PATHS := /raid0/pdgs/components/libCirculationCallback/test

COV_BASE_DIR := /raid0/pdgs/components/libCirculationCallback/

CUSTOM_LIBRARYPATH =

CSOURCES =

CINCLUDE =

C++SOURCES = CirculationCallbackSoap.C \
	DC_CallbackSingleton.C \
	CirculationCallbackV.C

C++INCLUDE = CirculationCallbackSoap.h \
		DC_CallbackSingleton.h \
				generated/CirculationCallback.nsmap \
				generated/CirculationCallbackH.h \
				generated/CirculationCallbackStub.h \
				generated/CirculationCallbackPDS_USCOREDCBindingService.h \
				generated/CirculationCallbackPDS_USCOREDCBindingProxy.h \

   CPPSOURCES = \
		generated/CirculationCallbackC.cpp \
		generated/CirculationCallbackPDS_USCOREDCBindingService.cpp \
		generated/CirculationCallbackPDS_USCOREDCBindingProxy.cpp

############ gsoap generation ###############################

CirculationCallbackSoapGenerated.h:
	wsdl2h -z2 -qCirculationCallback -t typemap.dat -I/usr/share/gsoap/import/ -o CirculationCallbackSoapGenerated.h  circulationCallback.xsd  circulationCallback.wsdl   ;

generatedFiles := \
		generated/CirculationCallback.nsmap \
		generated/CirculationCallbackC.cpp \
		generated/CirculationCallbackH.h \
		generated/CirculationCallbackStub.h \
		generated/CirculationCallbackPDS_USCOREDCBindingService.cpp \
		generated/CirculationCallbackPDS_USCOREDCBindingService.h \
		generated/CirculationCallbackPDS_USCOREDCBindingProxy.cpp \
		generated/CirculationCallbackPDS_USCOREDCBindingProxy.h \



.PHONY : generatedFiles


generated/timestamp :  CirculationCallbackSoapGenerated.h 
	@if [ ! -d generated ] ; then echo "creating directory generated" ; mkdir generated ; fi
	@if cd generated ; then \
		if soapcpp2 -I/usr/share/gsoap/import/ -i -n ../CirculationCallbackSoapGenerated.h ; then \
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

CirculationCallbackSoap.o: $(generatedFiles)
generated/CirculationCallbackPDS_USCOREDCBindingService.o: CXXAUXOPTS= 
generated/CirculationCallbackC.o: CXXAUXOPTS= 
generated/CirculationCallbackPDS_USCOREDCBindingProxy.o: CXXAUXOPTS= 

############ gsoap generation ###############################

#depend must be sure that generated files 
depend: generated/timestamp


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	rm -f CirculationCallbackSoapGenerated.h; rm -fr generated

#empty rule, do not remove, you can add specific help strings
helpspecific:


# DO NOT DELETE

include ../../common/commonrules.mk

#DSODEPENDLIBS = -lBaseSoap

