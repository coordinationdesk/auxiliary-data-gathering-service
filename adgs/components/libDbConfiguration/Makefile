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
#	$Prod: A.C.S. Makefile for libDbConfiguration $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 5.1  2014/05/16 09:30:50  marpas
#	fixing helpspecific target
#	
#	Revision 5.0  2013/06/11 14:16:31  marpas
#	adopting libException 5.x standards
#	
#	Revision 2.7  2013/03/29 19:51:19  marpas
#	removing html related functions
#	
#	Revision 2.6  2013/03/08 17:00:42  marpas
#	libDbBasic no longer used
#	
#	Revision 2.5  2013/02/28 13:20:21  marpas
#	no libMessage dependency
#	
#	Revision 2.4  2008/12/16 14:51:37  marpas
#	dbSystemMode implemented
#	
#	Revision 2.3  2008/10/08 12:48:57  crivig
#	porting to postgres8
#	
#	Revision 2.2  2006/08/23 14:52:27  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.1  2006/04/20 12:30:17  clanas
#	added new source files
#	
#	Revision 2.0  2006/02/28 09:09:03  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.7  2005/10/24 16:47:11  marpas
#	dbexpress compatibilty
#	
#	Revision 1.6  2005/09/08 14:17:21  paoscu
#	INCLUDEPATH REMOVED
#	
#	Revision 1.5  2005/07/21 10:50:16  marpas
#	file actions for dbConfiguration implemented
#	
#	Revision 1.4  2005/07/21 10:24:43  marpas
#	dbConfigurationSpace and its query imported from libDbCryosat
#	
#	Revision 1.3  2003/03/03 14:18:56  marpas
#	fixed
#	
#	Revision 1.2  2003/01/31 15:12:08  marpas
#	LIB_INTEGRATION_PATH variable management
#	
#	Revision 1.1.1.1  2003/01/14 15:34:55  marpas
#	libDbConfiguration Import
#		
#


include ../../common/commondefs.mk

  LIBRARYNAME =	libDbConfiguration

  VERSIONFILE = dbcf.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		dbResourceSpace.C \
		dbResourceSpaceFactory.C \
		dbSystemModeFactory.C \
		dbSystemMode.C \
		dbConfigurationSpace.C \
		dbConfigurationSpaceQuery.C \
		dbConfigurationFileActionFactory.C \
		dbDownloadConfigurationSpace.C \
		dbcf.C

   C++INCLUDE = \
		dbConfigurationSpace.h \
		dbConfigurationSpaceQuery.h \
		dbResourceSpaceFactory.h \
		dbResourceSpace.h \
		dbSystemModeFactory.h \
		dbSystemMode.h




DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 




DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:

include ../../common/commonrules.mk


# DO NOT DELETE
