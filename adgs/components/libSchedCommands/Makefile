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
#	$Prod: A.C.S. Scheduler Abstract Commands Library Makefile $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.3  2006/08/23 14:50:37  paoscu
#	Shared libraries dependencies added
#	
#	Revision 1.2  2005/06/20 15:06:58  enrcar
#	INCLUDEPATH REMOVED
#	
#	Revision 1.1.1.1  2004/04/07 13:31:01  marfav
#	Import libSchedCommands
#	
#	
#

include ../../common/commondefs.mk

  LIBRARYNAME = libSchedCommands

  VERSIONFILE = scacV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		AbstractQueue.C \
		ScCommandQueue.C \
		ScAbstractCommand.C\
		ScCommandFactoryChain.C \
		scacV.C \


   C++INCLUDE = \
		AbstractQueue.h \
		ScCommandQueue.h \
		ScAbstractCommand.h\
		ScCommandFactoryChain.h \


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 

DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands

cleanspecific:

helpspecific:


include ../../common/commonrules.mk

