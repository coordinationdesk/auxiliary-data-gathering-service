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
#	Authors: 
#
#	$Prod: A.C.S. Resources Library $
#
#	$Id$
#
#   $Log$
#   Revision 6.1  2013/11/21 10:22:27  marpas
#   dependencies fixed
#
#   Revision 6.0  2013/07/16 17:00:44  marpas
#   getArraySize returns 0 if the array is not present
#   returns 1 if the array is not a real array but a single value
#   getArray and setArray are now template methods managing the above two statements.
#
#   Revision 5.0  2013/06/11 09:29:10  marpas
#   adopting libException 5.x standards
#
#   Revision 2.5  2013/02/22 13:56:41  marpas
#   rsResourceSet does not longer inherit virtually from ThreadSafe, it has an internal mutex instead
#   Interface rationalization
#   coding best practices
#
#   Revision 2.4  2012/11/20 22:39:55  marpas
#   total quality: code, coverage etc.
#
#   Revision 2.3  2007/02/05 13:16:04  giucas
#   Added RmLegacy
#
#   Revision 2.2  2006/10/11 09:49:16  marpas
#   RmLegacy header provided
#
#   Revision 2.1  2006/08/23 14:51:06  paoscu
#   Shared libraries dependencies added
#
#   Revision 2.0  2006/02/28 08:34:37  marpas
#   Exception 2.1 I/F adopted
#
#   Revision 1.3  2005/06/20 14:35:35  enrcar
#   INCLUDEPATH REMOVED
#
#   Revision 1.2  2003/01/31 17:23:44  marpas
#   LIB_INTEGRATION_PATH variable management
#
#   Revision 1.1.1.1  2002/10/03 20:54:59  marpas
#   libResources import
#
#
#
#

include ../../common/commondefs.mk

  LIBRARYNAME =	libResources

  VERSIONFILE = rsV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = rsResourceSet.C \
    		    rsPushNSpace.C \
                RmLegacy.C \
                rsV.C

   C++INCLUDE = rsResourceSet.h \
    			rsPushNSpace.h \
				RmLegacy.h


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 


COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)



include ../../common/commonrules.mk
DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:
