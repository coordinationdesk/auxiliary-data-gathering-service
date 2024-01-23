#
#	Copyright 1995-2011 Advanced Computer Systems , Inc.
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

include ../../common/commondefs.mk

   LIBRARYNAME = libBaseSoap

   VERSIONFILE = lBaseSoapV.C

   CUSTOM_INCLUDEPATH += -I/usr/share/gsoap/plugin

   CUSTOM_LIBPATH = -L..

   CSOURCES =

   CINCLUDE =

   USERFLAG += -DWITH_OPENSSL -DWITH_DOM

   C++SOURCES = \
			lBaseSoapV.C \
			BaseSoap.C

   C++INCLUDE =  \
			BaseSoap.h \
			basesoapH.h \
			basesoapStub.h


ifeq ($(shell if [ -f /usr/share/gsoap/plugin/mecevp.c ] ; then echo OK; fi),OK)
	GSOAPSOURCES = \
		wsseapi.cpp \
		smdevp.cpp \
		mecevp.cpp
else
	GSOAPSOURCES = \
		wsseapi.cpp \
		smdevp.cpp 
endif


ifeq ($(shell if [ -f /usr/share/gsoap/custom_acs/stdsoap2.cpp ] ; then echo OK; fi),OK)

   CPPSOURCES = \
		stdsoap2.cpp \
		dom.cpp \
		$(GSOAPSOURCES) \
		basesoapC.cpp

else

   CPPSOURCES = \
		$(GSOAPSOURCES) \
		basesoapC.cpp

   DSOSPECIFICDEPS=-lgsoapssl++ -lssl

endif

basesoapH.h: 
	soapcpp2 -i -pbasesoap -L -I/usr/share/gsoap/import env.h;
	ln -s basesoapH.h soapH.h

stdsoap2.cpp :
	ln -s  /usr/share/gsoap/custom_acs/stdsoap2.cpp ./stdsoap2.cpp;

dom.cpp :
	ln -s  /usr/share/gsoap/custom_acs/dom.cpp ./dom.cpp;
	ln -s  /usr/share/gsoap/custom_acs/dom.cpp ./dom.h;

wsseapi.cpp :
	ln -s  /usr/share/gsoap/plugin/wsseapi.c ./wsseapi.cpp;
	ln -s  /usr/share/gsoap/plugin/wsseapi.h ./wsseapi.h;

smdevp.cpp :
	ln -s  /usr/share/gsoap/plugin/smdevp.c ./smdevp.cpp;
	ln -s  /usr/share/gsoap/plugin/smdevp.h ./smdevp.h;

mecevp.cpp :
	ln -s  /usr/share/gsoap/plugin/mecevp.c ./mecevp.cpp;
	ln -s  /usr/share/gsoap/plugin/mecevp.h ./mecevp.h;

stdsoap2.o : stdsoap2.cpp
	g++ -DWITH_NONAMESPACES -DWITH_OPENSSL -DWITH_DOM  $(CXXFLAGS)   -c   stdsoap2.cpp -o stdsoap2.o

smdevp.o : smdevp.cpp
	g++ -DWITH_NONAMESPACES -DWITH_OPENSSL -DWITH_DOM  $(CXXFLAGS)   -c   smdevp.cpp -o smdevp.o

mecevp.o : mecevp.cpp
	g++ -DWITH_NONAMESPACES -DWITH_OPENSSL -DWITH_DOM  $(CXXFLAGS)   -c   mecevp.cpp -o mecevp.o

wsseapi.o : wsseapi.cpp basesoapH.h
	g++ -DWITH_NONAMESPACES -DWITH_OPENSSL -DWITH_DOM  $(CXXFLAGS)   -c   wsseapi.cpp -o wsseapi.o

dom.o : dom.cpp
	g++ -DWITH_NONAMESPACES -DWITH_OPENSSL -DWITH_DOM  $(CXXFLAGS)   -c   dom.cpp -o dom.o

basesoapC.o : basesoapH.h
	g++ -DWITH_NONAMESPACES -DWITH_OPENSSL -DWITH_DOM  $(CXXFLAGS)   -c   basesoapC.cpp -o basesoapC.o

BaseSoap.o: basesoapH.h 

stdsoap2.o: CXXAUXOPTS=
smdevp.o: CXXAUXOPTS=
dom.o: CXXAUXOPTS=
wsseapi.o: CXXAUXOPTS=
basesoapC.o: CXXAUXOPTS=
mecevp.o: CXXAUXOPTS=

depend: basesoapH.h

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	@rm -f stdsoap2.*;
	@rm -f smdevp.*;
	@rm -f mecevp.*;
	@rm -f soap*.* ;
	@rm -f wsseapi.* ;
	@rm -f dom.* ;
	@rm -f wsse.* ;
	@rm -f basesoap*.* ;


#empty rule, do not remove, you can add specific help strings
helpspecific:


# DO NOT DELETE

include ../../common/commonrules.mk

