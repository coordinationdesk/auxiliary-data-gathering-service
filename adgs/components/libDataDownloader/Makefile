#
#	Copyright 1995-2013, Advanced Computer Systems , Inc.
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
#	$Prod: DataDownloader MAKEFILE $
#	
#       $Id$
#
#       $Author$
#
#       $Log$
#       Revision 1.2  2013/10/01 10:25:13  chicas
#       bug fixed (DDV.C in source files)
#
#       Revision 1.1.1.1  2013/05/16 09:35:23  enrcar
#       Imported source
#
#

include ../../common/commondefs.mk

CUSTOM_LIBPATH=-L/raid0/venus/boost/lib
CUSTOM_INCLUDEPATH=-I/raid0/venus/boost/include
CUSTOM_LDOPTS=-Wl,-rpath,/raid0/venus/boost/lib

  LIBRARYNAME =	libDataDownloader

  VERSIONFILE = DDV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
			   DataDownloader.C \
			   ParallelDataDownloader.C \
			   DDV.C 


   C++INCLUDE = \
			   DataDownloader.h \
			   ParallelDataDownloader.h \


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 




include ../../common/commonrules.mk
DSOSPECIFICDEPS =




#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:


# DO NOT DELETE

