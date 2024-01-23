#
#	Copyright 1995-2001, Advanced Computer Systems , Inc.
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
#	$Author$ 
#
#	@(#) A.C.S. libAuxFileHandling Makefile - %M% %I% %E%
#
#	$Log$
#	Revision 2.0  2006/03/01 16:41:23  serfol
#	Exception 2.0 I/F adapted
#	
#	Revision 1.1.1.1  2006/01/31 13:44:10  seralb
#	Imported libAuxFileHandlingBase
#	
#	Revision 1.9  2005/06/20 14:13:43  enrcar
#	INCLUDEPATH REMOVED
#	
#	Revision 1.8  2005/02/21 14:42:50  giucas
#	Removed files previously moved to libAuxFileParsing
#	
#	Revision 1.7  2005/02/02 09:18:29  danalt
#	added OBTUTCTimeCorrParams class
#	
#	Revision 1.6  2004/12/07 14:33:40  giucas
#	Added OrbitScenarioFileParser and CryosatOSFileParser
#	
#	Revision 1.5  2004/12/07 11:33:42  giucas
#	Added OrbitScenario
#	
#	Revision 1.4  2004/12/07 10:10:32  giucas
#	Added new classes for OBT file parsing
#	
#	Revision 1.3  2004/12/07 08:25:54  giucas
#	Updated for new classes
#	
#	Revision 1.2  2004/12/06 13:43:05  giucas
#	Added AuxFileParser
#	
#	Revision 1.1.1.1  2004/12/06 11:21:40  giucas
#	Imported libAuxFileHandling
#	
#	
#

include ../../common/commondefs.mk

  LIBRARYNAME =	libAuxFileHandlingBase

  VERSIONFILE = auxFileHandlV.C

   C++SOURCES = AuxDownloader.C \
		AuxFileList.C \
		AuxDownloaderSingleton.C \
		auxFileHandlV.C
				
   C++INCLUDE = AuxDownloader.h \
  		AuxFileList.h \
 		AuxDownloaderSingleton.h 

    
COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	
#empty rule, do not remove, you can add specific help strings
helpspecific:

include ../../common/commonrules.mk
	
	
