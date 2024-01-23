#
#	Copyright 1995-2017, Advanced Computer Systems , Inc.
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
#	$Author$
#
#	$Prod: A.C.S. DBXpress for Postgres $
#
#	$Id$
#
#   $Log$
#   Revision 1.11  2012/02/02 14:43:02  marpas
#   removing compiler warnings and enforcing robustness
#
#   Revision 1.10  2010/11/24 20:52:05  marpas
#   compilation flags similar to the other drivers
#
#   Revision 1.9  2010/10/22 15:53:36  enrcar
#   EC:: Optional CUSTOMFLAGS added
#
#   Revision 1.8  2009/03/02 17:41:12  marpas
#   fixed
#
#   Revision 1.7  2008/11/20 11:01:23  marpas
#   depends on libpq
#
#   Revision 1.6  2008/07/21 17:12:08  paoscu
#   version files added
#
#   Revision 1.5  2008/07/21 17:03:51  paoscu
#   work in progress
#
#   Revision 1.4  2008/07/21 16:51:12  paoscu
#   work in progress
#
#   Revision 1.3  2008/07/21 16:46:15  paoscu
#   aligning with dev env
#
#   Revision 1.2  2008/05/16 10:01:49  enrcar
#   subcomponent removed
#
#   Revision 1.1.1.1  2007/12/14 18:42:20  marpas
#   Import Postgres dbExpress driver
#
#
#

include ../../common/commondefs.mk

 CUSTOMFLAGS = -D_REENTRANT -s -Wall \
	  			   -Weffc++ \
				   -Wnon-virtual-dtor \
				   -Wno-non-template-friend \
				   -Wold-style-cast \
				   -Woverloaded-virtual \
				   -Wno-pmf-conversions \
				   -Wsign-promo
 DSO_CUSTOM_FLAGS = -shared -s -O3


# Enable DEBUG: (decomment following line)
#CUSTOMFLAGS := $(CUSTOMFLAGS) -DACDBEXPPGL_DEBUG


  LIBRARYNAME = libacdbexppgl

  VERSIONFILE = acdbexppglV.C

     CSOURCES =	

     CINCLUDE =	
	 
   C++SOURCES = acdbexppglV.C 

   CPPSOURCES = pgldriver.cpp \
			    pglconnection.cpp \
			    pglcommand.cpp \
				pglvariable.cpp \
			    pglconversion.cpp \
			    pglcursor.cpp \
			    pglerror.cpp \

   C++INCLUDE = 

      DSOSPECIFICDEPS =  -lpq

 
#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

include ../../common/commonrules.mk

