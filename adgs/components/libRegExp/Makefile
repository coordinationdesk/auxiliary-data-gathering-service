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
#	$Prod: A.C.S. Regular Expression Library $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 2.4  2012/03/29 16:05:42  marpas
#	coverage integration
#	
#	Revision 2.3  2012/03/26 14:10:11  chicas
#	coverage integration : added COV_PATHS_IGNORE ; moved some COV defs in commondefs.mk
#	
#	Revision 2.2  2012/03/21 13:39:25  marpas
#	coverage integration in progress
#	
#	Revision 2.1  2006/08/23 14:50:12  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.0  2006/02/28 08:25:52  marpas
#	Exception 2.1 I/ adopted
#	
#	Revision 1.3  2005/06/20 16:34:15  enrcar
#	INCLUDEPATH REMOVED
#	
#	Revision 1.2  2003/01/31 17:18:55  marpas
#	LIB_INTEGRATION_PATH variable management
#	
#	Revision 1.1.1.1  2002/11/21 15:00:23  paoscu
#	Import libRegExp
#	
#
#	

include ../../common/commondefs.mk

  LIBRARYNAME =	libRegExp

  VERSIONFILE = lregx.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = RegExp.C \
				lregx.C

   C++INCLUDE = RegExp.h \
   

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 
DSOSPECIFICDEPS =

COV_PATHS = $(shell pwd)/test

COV_BASE_DIR := ./

cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

include ../../common/commonrules.mk

# DO NOT DELETE

