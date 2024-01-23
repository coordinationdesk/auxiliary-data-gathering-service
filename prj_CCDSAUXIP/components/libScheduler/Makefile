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
#	$Prod: A.C.S. Scheduler Interface Library $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 2.2  2008/04/22 11:02:05  marfav
#	Aligned to new visitable task table elements
#	
#	Revision 2.1  2006/08/23 14:51:31  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.0  2006/02/28 10:17:14  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.12  2005/06/20 15:12:06  enrcar
#	INCLUDEPATH REMOVED
#	
#	Revision 1.11  2004/09/15 09:18:43  marfav
#	scOrder class moved to libScOrder
#	
#	Revision 1.10  2004/03/11 18:54:37  marfav
#	*** empty log message ***
#	
#	Revision 1.9  2004/03/11 18:43:10  marfav
#	Importing Scheduler Engine in library
#	
#	Revision 1.8  2003/09/04 10:39:13  giucas
#	Updated commondefs and commonrules paths from absolute to relative
#	
#	Revision 1.7  2003/08/19 09:16:08  marpas
#	version file no longer installed
#	
#	Revision 1.6  2003/07/16 15:17:49  marfav
#	Version file header added
#	
#	Revision 1.5  2003/02/05 13:35:49  fracar
#	fixed specific cleanup
#	
#	Revision 1.4  2003/02/04 17:25:43  fracar
#	got rid of ^M
#	
#	Revision 1.3  2003/01/31 12:42:36  marfav
#	handling different integration and test environments
#	
#

include ../../common/commondefs.mk

  LIBRARYNAME = libScheduler

  VERSIONFILE = scV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		SchedulerIF.C \
		SchedulerEngine.C \
		ThreadedScheduler.C \
		SpawnedScheduler.C \
		SchedulerExitStatus.C \
		scV.C\


   C++INCLUDE = \
		SchedulerEngine.h \
		SchedulerIF.h \
		ThreadedScheduler.h \
		SpawnedScheduler.h \
		SchedulerExitStatus.h \

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 


DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands

cleanspecific:

helpspecific:

include ../../common/commonrules.mk

