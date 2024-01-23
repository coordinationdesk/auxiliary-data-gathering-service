#
#	Copyright 1995-2009, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. Process library $
#
#	$Id$
#
#	$Log$
#	Revision 2.6  2013/07/24 10:30:22  marfav
#	Added ThreadedProcWrapper class
#	
#	Revision 2.5  2012/11/20 09:09:13  marpas
#	coverage
#	
#	Revision 2.4  2009/04/03 16:37:03  marpas
#	exMailStream.h was not installed
#	
#	Revision 2.3  2009/04/02 13:10:49  marpas
#	exMailStream class introduced
#	some quality issues fixed
#	
#	Revision 2.2  2006/08/23 14:51:18  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.1  2006/05/18 15:11:46  marpas
#	ftailstream class family added (implemented only for input - iftailstream)
#	
#	Revision 2.0  2006/02/28 08:34:06  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.5  2005/09/08 14:18:09  paoscu
#	INCLUDEPATH REMOVED
#	
#	Revision 1.4  2005/08/03 10:25:46  nicvac
#	Obsolete reference to ACE removed.
#	
#	Revision 1.3  2003/02/25 18:51:57  marpas
#	ftmpstream classes added
#	
#	Revision 1.2  2003/01/31 16:48:28  marpas
#	LIB_INTEGRATION_PATH variable management
#	
#	Revision 1.1.1.1  2002/10/08 13:59:08  marpas
#	libProc import
#	
#
#

  LIBRARYNAME =	libProc

  VERSIONFILE = prV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
				ftmpstream.C \
				fucstream.C \
				ftarstream.C \
				ftailstream.C \
				WaitPid.C \
				WaitPidGuard.C \
	    			SpawnProc.C \
				SpawnProcWithLogs.C \
				SpawnProcExecutor.C \
				exMailStream.C \
				ThreadedProcWrapper.C \
				prV.C

   C++INCLUDE = \
				ftmpstream.h \
				fucstream.h \
				ftarstream.h \
				ftailstream.h \
    				SpawnProc.h \
				SpawnProcWithLogs.h \
				SpawnProcExecutor.h \
				WaitPid.h \
				exMailStream.h \
				ThreadedProcWrapper.h \
				WaitPidGuard.h


include ../../common/commondefs.mk

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 

COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

include ../../common/commonrules.mk
DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:
