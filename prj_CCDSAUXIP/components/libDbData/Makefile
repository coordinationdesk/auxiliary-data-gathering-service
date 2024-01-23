#
#	Copyright 1995-2022, Exprivia SpA - DFDA-AS
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
#	$Prod: A.C.S. Makefile for libDbData $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 5.4  2017/11/07 16:18:07  lucio.pulvirenti
#	S2PDGS-1843: classes added.
#	
#	Revision 5.3  2016/04/21 11:47:12  marpas
#	parent id provider concept evicted - as for related methods / members
#	marking dbPersistent concept evicted - as for related methods / members
#	
#	Revision 5.2  2016/04/11 13:21:14  marpas
#	DescriptionParser has been removed
#	
#	Revision 5.1  2014/05/16 09:29:23  marpas
#	fixing helpspecific target
#	
#	Revision 5.0  2013/06/13 14:53:03  marpas
#	adopting libException 5.x standards
#	removing deprecated use of dbConnectionMgr
#	general quality improvements
#	best practices applied
#	
#	Revision 2.17  2013/03/29 19:50:57  marpas
#	removing html related functions
#	
#	Revision 2.16  2013/03/08 16:58:39  marpas
#	libDbBasic no longer used
#	
#	Revision 2.15  2013/02/28 13:15:34  marpas
#	libMessage removed from dependencies
#	
#	Revision 2.14  2013/02/28 12:44:57  marpas
#	empty compilation units removed
#	
#	Revision 2.13  2013/02/28 12:42:13  marpas
#	dbPersistentScopeGuard.C no longer in the library
#	
#	Revision 2.12  2013/01/25 11:39:33  marpas
#	using pattern::Singleton from libException
#	
#	Revision 2.11  2013/01/22 17:37:06  marpas
#	coding best practices
#	interfaces rationalization
#	singletons use SingletonTemplate
#	qa rules
#	efficiency slightly improved
#	
#	Revision 2.10  2012/11/16 13:40:45  marpas
#	HEAD REALIGNEMENT
#	
#	Revision 2.6  2008/11/11 16:33:58  marpas
#	dbQueryWrapperMT removed
#	
#	Revision 2.5  2008/04/28 21:49:26  paoscu
#	Added dbSmartAggregateQuery
#	
#	Revision 2.4  2008/04/23 16:13:23  paoscu
#	dbSmartAggregatePersistent added.
#	
#	Revision 2.3  2006/10/26 17:51:49  paoscu
#	Class dbDoubleSmartPersistent added.
#	
#	Revision 2.2  2006/10/16 10:46:32  marpas
#	Drawable concept introduced in libGraphicAttributes, from which this library was already dependent
#	dbPersistent is now a Drawable
#	dbStObj Is a Drawable too because inheritance from dbPersistent and does not
#	longer implements attributes related methods
#	Factories dealing with Attributes are now moved to the more appropriate libGraphicAttributes
#	
#	Revision 2.1  2006/08/23 14:50:41  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.0  2006/02/28 08:47:37  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.21  2006/02/15 16:17:25  paoscu
#	dbField removed.
#	
#	Revision 1.20  2005/11/18 18:35:43  paoscu
#	dbSmartQuery added.
#	
#	Revision 1.19  2005/11/03 09:54:19  marpas
#	dbOperation01Factory class added and used
#	
#	Revision 1.18  2005/10/28 17:36:02  marpas
#	dbOperation01 class added
#	
#	Revision 1.17  2005/09/08 14:17:22  paoscu
#	INCLUDEPATH REMOVED
#	
#	Revision 1.16  2005/08/05 16:50:38  paoscu
#	dbSmartPersistent added
#	
#	Revision 1.15  2005/07/27 13:53:35  paoscu
#	Class dbField added.
#	
#	Revision 1.14  2005/07/22 09:44:22  ivafam
#	Added
#	
#	Revision 1.13  2005/01/04 10:25:26  ivafam
#	Added dbFileActionScopeGuard
#	
#	Revision 1.12  2005/01/03 14:22:01  ivafam
#	FileAction basic classes moved in libDbData
#	
#	Revision 1.11  2004/04/13 14:10:59  marpas
#	ACE dependencies removed
#	
#	Revision 1.10  2004/04/07 17:00:28  marpas
#	dbQueryWrapperMT & dbGeneralQueryIF classes added
#	
#	Revision 1.9  2004/03/23 12:34:17  paoscu
#	dbParentIdProvider.h added.
#	
#	Revision 1.8  2003/03/05 10:03:35  marpas
#	data factory added
#	
#	Revision 1.7  2003/03/04 10:50:19  marpas
#	new classes added
#	
#	Revision 1.6  2003/03/03 14:23:30  marpas
#	fixed
#	
#	Revision 1.5  2003/02/13 13:53:55  marpas
#	*** empty log message ***
#	
#	Revision 1.4  2003/02/07 17:46:38  marpas
#	dbPersistentScopeGuard class added
#	
#	Revision 1.3  2003/01/31 15:12:47  paoscu
#	LIB_INTEGRATION_PATH variable management
#	
#	Revision 1.2  2003/01/13 11:05:00  marpas
#	dbOverlapStrategy class added
#	
#	Revision 1.1.1.1  2002/11/29 17:19:30  marpas
#	Import libDbData
#		
#

include ../../common/commondefs.mk

  LIBRARYNAME =	libDbData

  VERSIONFILE = dbdV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		dbPersistent.C \
		dbPersistentFactoryChain.C \
		dbQueryFactoryChain.C \
		dbGeneralQuery.C \
		dbSmartPersistent.C \
		dbSmartQuery.C \
		dbSmartAggregateQuery.C \
		dbDoubleSmartPersistent.C \
		dbSmartAggregatePersistent.C \
		dbOverlapStrategy.C \
		dbStObj.C \
		dbApplicationStatus.C\
		dbApplicationStatusQuery.C\
		dbDataFactory.C \
		dbEraseFile.C \
		dbUploadFile.C \
		dbIsOnCacheFile.C \
		dbPutOnCacheFile.C \
		dbReleaseCacheFile.C \
		dbGiveUpFile.C \
		dbDownloadFile.C \
		dbFileAction.C \
		dbFileActionFactoryChain.C \
		dbFileActionFactory.C \
		dbPersistentOperatorIF.C \
		dbOperation01.C \
		dbOperation01Factory.C \
		StorageManagerInfo.C \
		StorageManagerInfoFactory.C \
		StorageManagerInfoFactoryChain.C \
		dbdV.C \
		rsTable.C

   C++INCLUDE = \
		dbPersistent.h \
		dbPersistentScopeGuard.h \
		dbPersistentFactoryChain.h \
		dbQueryFactoryChain.h \
		dbGeneralQuery.h \
		dbGeneralQueryIF.h \
		dbGenQueryScopeGuard.h \
		dbSmartPersistent.h \
		dbDoubleSmartPersistent.h \
		dbSmartAggregatePersistent.h \
		dbSmartQuery.h \
		dbSmartAggregateQuery.h \
		dbOverlapStrategy.h \
		dbStObj.h \
		dbApplicationStatus.h \
		dbApplicationStatusQuery.h \
		dbDataFactory.h \
		dbIsOnCacheFile.h \
		dbPutOnCacheFile.h \
		dbReleaseCacheFile.h \
		dbGiveUpFile.h \
		dbDownloadFile.h \
		dbUploadFile.h \
		dbEraseFile.h \
		dbFileAction.h \
		dbFileActionFactoryChain.h \
		dbFileActionFactory.h \
		dbFileActionScopeGuard.h \
		dbPersistentOperatorIF.h \
		StorageManagerInfo.h \
		StorageManagerInfoFactory.h \
		StorageManagerInfoFactoryChain.h \
		rsTable.h \





DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 




include ../../common/commonrules.mk
DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:




# DO NOT DELETE
