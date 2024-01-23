#
#	Copyright 1995-2007, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. Linux version of dbExpress driver for Postgres $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.5  2008/07/21 16:46:23  paoscu
#	aligning with dev env
#	
#	Revision 1.4  2008/05/16 10:08:56  enrcar
#	warning threshold ehnanced
#	
#	Revision 1.3  2008/02/25 17:20:56  marpas
#	work in progres
#	
#	Revision 1.2  2007/12/17 18:38:31  marpas
#	work in progress
#	
#	Revision 1.1.1.1  2007/12/14 18:42:20  marpas
#	Import Postgres dbExpress driver
#	
#	

ifndef VERSIONDIR
VERSIONDIR	= ../..
endif

      

ifdef ACDBEXPPGL_DEBUG
DEBUGFLAG =  -DACDBEXPPGL_DEBUG
else
DEBUGFLAG =  
endif
      
# Decomment next line to always force debug:
#DEBUGFLAG =  -DACDBEXPPGL_DEBUG


SOURCE		= $(VERSIONDIR)/libacdbexppgl

CC			= g++ -Wno-long-long -pedantic
#CFLAGS		= -DHAVE_CONFIG_H -pipe -O3 -Wall -Wno-deprecated -D_REENTRANT -s -c
CFLAGS		= -DHAVE_CONFIG_H -pipe -g -Wno-deprecated -D_REENTRANT -fPIC -c $(DEBUGFLAG)
INCLUDEPATH	= 	\
				-I$(SOURCE)/base \
				-I$(SOURCE)/command \
				-I$(SOURCE)/connection \
			  	-I$(SOURCE)/metadata \
				-I$(SOURCE)/utilities \
				-I$(SOURCE)/variable
                          
OBJFILES	= \
			  $(SOURCE)/connection/pgldriver.o \
			  $(SOURCE)/variable/pglvariable.o \
			  $(SOURCE)/command/pglcursor.o \
			  $(SOURCE)/command/pglcommand.o \
			  $(SOURCE)/base/pglerror.o \
			  $(SOURCE)/connection/pglconnection.o \
			  $(SOURCE)/base/pglconversion.o \
                          

LD			= g++

LFLAGS		= -shared -g
DYNAMICLIBS	= -lecpg_compat -lpq
STATICLIBS	= -lecpg_compat -lpq
		  
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDEPATH) $<
	
build: compile linkdynamic linkstatic clean

compile: 

linkdynamic: $(OBJFILES)
	$(LD) $(LFLAGS) -o $(VERSIONDIR)/../shared/libacdbexppgl.so  *.o $(DYNAMICLIBS)

linkstatic: $(OBJFILES)
	$(LD) $(LFLAGS) -o $(VERSIONDIR)/../lib/libacdbexppgl.so *.o $(STATICLIBS)

clean:
	#rm -f *.o
