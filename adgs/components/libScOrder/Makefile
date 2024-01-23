#
#	Copyright 1995-2014, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. TL Scheduler Order Library $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.7  2014/05/16 09:34:25  marpas
#	fixing helpspecific target
#	
#	Revision 1.6  2006/08/23 14:50:43  paoscu
#	Shared libraries dependencies added
#	
#	Revision 1.5  2005/09/07 09:32:18  marfav
#	INCLUDEPATH definition removed
#	
#	Revision 1.4  2005/08/31 07:47:19  clanas
#	Removed xercex include
#	
#	Revision 1.3  2005/08/30 16:02:33  serfol
#	INCLUDEPATH cannot be defined here: fixed
#	
#	Revision 1.2  2005/08/03 10:25:47  nicvac
#	Obsolete reference to ACE removed.
#	
#	Revision 1.1.1.1  2004/09/15 08:12:07  marfav
#	Import libScOrder
#	
#


include ../../common/commondefs.mk

  LIBRARYNAME =	libScOrder

  VERSIONFILE = scoV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		scOrder.C \
		scoV.C \


   C++INCLUDE = \
		scOrder.h \



COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 


DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:

include ../../common/commonrules.mk
