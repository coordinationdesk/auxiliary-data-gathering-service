#
#	Copyright 2019, Exprivia - DFDA-AS
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia SpA.
#
#	$Author: $
#
#	$Prod: LTA common functions test $
#


include ../../../common/commondefs.mk

#must be an absolute path for coverage 
CUSTOM_INCLUDEPATH = -I$(shell pwd)/..
#must be an absolute path for coverage 
CUSTOM_LIBRARYPATH = -L$(shell pwd)/..
#must be an absolute path to avoid linking the installed one
CUSTOM_LDOPTS = -Wl,-rpath,$(shell pwd)/..

OTHERFLAGS+=-DTEST_LIB_LTA_COMMON

SPECIFICLIBS = -lcppunit

#the list of executables that are generated from a single C++ source file with the same name
#this will be linked by a default target implemented in the common rules
UNITTESTEXES =  \
	testLTACommon

#the list of executables that are generated from one or more C++ source files, and having at least one
#source with the same name. There will be no default target to link it and a specific target must be 
#defined in this makefile
CUSTOMEXES =  \
	testAipParameters

#a list of additional source files that must be compiled in order to allow an explicit link from 
#a custom rule
AUX_C++SOURCES =

include ../../../common/unittestrules.mk
include ../../../common/commonrules.mk

#Specific build rules must be placed after the external rules include

#additional dependencies from AUX_C++SOURCES shall be placed here
#also ../LIBUNDERTEST must be reported as a direct dependency to avoid linkage of 
#the installed library (from ../../shared)
testAipParameters: %: %.o %.o.undefsym ../$(LIBUNDERTEST)
	$(UNITTESTBUILDRECIPE)

#don't remove
#bisogna automatizzre la cancellazione delle cose prodotte dal runner dei test unitari
#creando un nuovo target cleanunittestreports da chiamare in aggiunta a cleanspecific in tutti i target
#di pulizia che hanno a che fare con i test (cleanallbuttest e simili)
cleanspecific:
	@rm -f core *.o *.o.undefsym *.log *.bck *_Results.xml



