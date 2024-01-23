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
#
#	$Prod: $
#
#	$Author$
#	$Id$
#
#	$Log$
#	Revision 2.4  2013/04/09 08:21:48  marpas
#	meIdAssociator refactoring
#	
#	Revision 2.3  2013/03/08 17:16:47  marpas
#	libDbBasic no longer used
#	
#	Revision 2.2  2013/02/07 17:51:33  marpas
#	DistributorExecutor class added
#	Factories, Creators and related classes interface changed to allow dbConnectionPoll easy handling and stop predicates
#	
#	Revision 2.1  2013/02/07 12:02:04  marpas
#	introducing CompletedOrderFinalizer class
#	qa rules enforced
#	
#	Revision 2.0  2013/01/21 17:52:32  marpas
#	introducing dbConnectionPool
#	coding best practices
#	error management improved
#	qa rules
#	
#	Revision 1.4  2012/03/01 15:44:23  chicas
#	Added DistributionAlgo NameId associator
#	
#	Revision 1.3  2008/10/07 10:32:58  crivig
#	porting to postgres8
#	
#	Revision 1.2  2008/05/20 10:52:00  marpas
#	minor changes
#	
#	Revision 1.1.1.1  2007/03/14 13:11:12  crivig
#	Imported source
#	
#
#
include ../../common/commondefs.mk

#CUSTOMFLAGS:=-DDBEXPRESS_COMPAT $(CUSTOMFLAGS)


  LIBRARYNAME =	libDistributorCreator

  VERSIONFILE =	dgenV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		DistributorCreator.C \
		DistributorCreatorFactoryChain.C \
		DistributorCreatorFactory.C \
		CompletedOrderFinalizer.C \
		DGDBSimulationSingleton.C \
        DistributorExecutor.C \
		dgenV.C 


   C++INCLUDE = \
		DistributionAlgo.h \
		DistributorCreator.h \
		DGDBSimulationSingleton.h \
		DistributorCreatorFactory.h \
		DistributorCreatorFactoryChain.h \
		CompletedOrderFinalizer.h \
        DistributorExecutor.h \


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 


DSOSPECIFICDEPS =


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:


include ../../common/commonrules.mk
