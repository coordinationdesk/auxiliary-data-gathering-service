#
#	Copyright 1995-2005, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. Thread Library $
#	$Id$
#
#   $Log$
#   Revision 5.0  2013/06/10 16:08:25  marpas
#   adopting libException 5.x standards
#
#   Revision 2.6  2012/11/06 18:01:57  marpas
#   Quality improved
#
#   Revision 2.5  2012/03/29 16:29:53  marpas
#   coverage integration
#
#   Revision 2.4  2010/07/29 09:48:10  matteo.airoldi
#   added dependency of libm
#
#   Revision 2.3  2009/02/10 15:12:46  lucio.pulvirenti
#   LP and PS: commonrules.mk moved at the end
#
#   Revision 2.2  2007/11/22 11:34:44  lucio.pulvirenti
#   Stop Control feature added
#
#   Revision 2.1  2006/08/23 14:52:13  paoscu
#   Shared libraries dependencies added
#
#   Revision 2.0  2006/02/28 08:27:50  marpas
#   Exception 2.1 I/F adopted
#
#   Revision 1.5  2005/11/03 09:19:42  marpas
#   slightly changed
#
#   Revision 1.4  2005/06/20 14:33:38  enrcar
#   INCLUDEPATH Removed
#
#   Revision 1.3  2004/04/13 13:21:09  paoscu
#   ACE dependencies removed
#
#   Revision 1.2  2003/01/31 17:29:33  marpas
#   LIB_INTEGRATION_PATH variable management
#
#   Revision 1.1.1.1  2002/10/21 18:07:37  paoscu
#   Import libThread
#
#
#


include ../../common/commondefs.mk

  LIBRARYNAME =	libThread

  VERSIONFILE = thV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = Thread.C InitCallbackManagerSingleton.C thV.C 

   C++INCLUDE = Thread.h InitCallbackManagerSingleton.h

DSOSPECIFICDEPS = -lm

COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)


#empty rules, do not remove, you can add specific help strings
cleanspecific:

helpspecific:



include ../../common/commonrules.mk
