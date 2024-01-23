#
#	Copyright 2020-2021, Exprivia SpA - DFDA-AS
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.exprivia.com
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia SpA
#
#	$Prod: A.C.S. Makefile for libStorageManager $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 6.0  2013/12/11 11:53:49  marfav
#	Issuing major release 6.0
#	
#	Revision 5.1  2013/09/16 17:03:53  marpas
#	dependencies now include libDbClient 'cause insertInvSM uses a query
#	
#	Revision 5.0  2013/06/18 14:40:01  marpas
#	adoption of libException 5.x standards
#	
#	Revision 2.6  2013/02/28 13:23:14  marpas
#	no libMessage dependency
#	
#	Revision 2.5  2013/02/25 17:00:17  marpas
#	coding best practices applied, useless compilation unti removed
#	
#	Revision 2.4  2008/11/11 11:14:10  ivafam
#	work in progess
#	
#	Revision 2.3  2008/09/18 06:52:31  ivafam
#	Added StorageName
#	
#	Revision 2.2  2008/06/20 10:19:52  marpas
#	version file moved
#	
#	Revision 2.1  2006/08/23 14:51:35  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.0  2006/02/28 09:11:13  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.9  2005/08/05 07:02:58  ivafam
#	Added classes for packing strategy management
#	
#	Revision 1.8  2005/06/20 14:29:17  enrcar
#	INCLUDEPATH Removed
#	
#	Revision 1.7  2005/06/15 07:59:34  integrator
#	Work in progress
#	
#	Revision 1.6  2005/06/07 09:56:56  integrator
#	split in progress
#	
#	Revision 1.5  2005/06/07 09:04:48  integrator
#	splitting in progress
#	
#	Revision 1.4  2005/05/31 14:25:51  integrator
#	work in progress...
#	
#	Revision 1.3  2005/05/30 18:14:03  ivafam
#	Added EraseInventoryFile
#	
#	Revision 1.2  2005/05/23 17:25:08  ivafam
#	*** empty log message ***
#	
#	Revision 1.1.1.1  2005/05/18 07:57:07  marpas
#	Importing libStorageManager
#	
#		
#

include ../../common/commondefs.mk

  LIBRARYNAME =	libStorageManager

  VERSIONFILE = stormgrV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		stormgrV.C \
		SMInterface.C \
		AsynchSMInterface.C \
		StorageName.C \
		SMInterfaceFactoryMethod.C \
		SMPluginLoader.C \
		SMPackingStrategyFactoryChain.C \
		SMPackingStrategyFactory.C \
		SMPackingStrategy.C \
        MigrationReport.C
		

   C++INCLUDE = \
		SMInterface.h \
		StorageName.h \
		AsynchSMInterface.h \
		SMInterfaceFactoryMethod.h \
		SMInterfaceScopeGuard.h \
		AsynchSMInterfaceScopeGuard.h \
		SMPluginLoader.h \
		SMPackingStrategyFactoryChain.h \
		SMPackingStrategyFactory.h \
		SMPackingStrategy.h \
		SMPackingStrategyScopeGuard.h \
        MigrationReport.h 
		
#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	@rm -f *.bak;


#empty rule, do not remove, you can add specific remotion commands
helpspecific:



# DO NOT DELETE


include ../../common/commonrules.mk

