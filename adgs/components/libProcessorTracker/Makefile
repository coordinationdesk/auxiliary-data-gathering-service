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
#	$Prod: A.C.S. Processor Tracker Library Makefile $
#	
#	$Id$
#
#	$Log$
#	Revision 2.1  2006/08/23 14:51:23  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.0  2006/02/28 10:18:10  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.2  2005/06/20 16:37:50  enrcar
#	INCLUDEPATH REMOVED
#	
#	Revision 1.1.1.1  2004/04/15 12:19:58  marfav
#	Import libProcessorTracker
#	
#	
#

include ../../common/commondefs.mk

# UNIT_TEST -> enables test class friendship

  USERFLAG = -DUNIT_TEST

  LIBRARYNAME =	libProcessorTracker

  VERSIONFILE =	ptrkV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		ProcessorTracker.C \
		CryosatProcessorTracker.C \
		ptrkV.C \


   C++INCLUDE = \
		ProcessorTracker.h \
		CryosatProcessorTracker.h \


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 


DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:


include ../../common/commonrules.mk
