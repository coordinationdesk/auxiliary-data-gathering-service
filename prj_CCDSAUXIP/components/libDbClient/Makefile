#
#	Copyright 1995-2022, Exprivia SpA - DFDA-AS
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.exprivia.it
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia SpA
#
#	$Author$
#
#	$Prod: A.C.S. DataBase Client Library $
#
#	$Id$
#
#   $Log$
#   Revision 5.13  2018/02/08 13:36:34  marpas
#   new InvRelationTypes NameIdAssociator implemented
#
#   Revision 5.12  2017/11/23 19:11:50  marpas
#   InSet template class adedd - working in progress
#
#   Revision 5.11  2017/11/21 18:11:21  marpas
#   coverage variables added
#
#   Revision 5.10  2016/10/07 11:33:35  marpas
#   mporting into libDbClient associators from dbCryosat ( FileClasses, OrdersAttachmentType, RollingPoliciesActions, RollingPoliciesAlgo )
#
#   Revision 5.9  2016/07/25 15:50:16  lucio.pulvirenti
#   S1PDGS-31765: LocalTransferMode class added.
#
#   Revision 5.8  2016/03/26 21:48:28  marpas
#   removing dbConnectionMgr adn dbConnectionGuard classes
#   good (connected) connections can now only be obtained from a dbConnectionPool
#   passing of connection reference is enforced (preferred to the obsolete method to pass a default nullable connection pointer)
#
#   Revision 5.7  2015/02/17 14:28:16  marfav
#   Added new template for NameIdAssociator singeltons
#
#   Revision 5.6  2013/12/19 12:24:34  marpas
#   Transaction class added
#
#   Revision 5.5  2013/10/23 15:01:00  marfav
#   Added AnomalyTypes
#
#   Revision 5.4  2013/09/25 14:52:48  lucio.pulvirenti
#   SatelliteAcronym class added.
#
#   Revision 5.3  2013/07/23 14:02:35  lucio.pulvirenti
#   TmpNameStrategyTable Class added.
#
#   Revision 5.2  2013/07/22 16:21:48  marpas
#   new classes added
#
#   Revision 5.1  2013/07/12 08:58:48  marpas
#   Status class introduced
#
#   Revision 5.0  2013/06/06 18:52:55  marpas
#   adopting libException 5.x standards
#
#   Revision 4.14  2013/05/14 18:03:56  marpas
#   some classes removed: they are not longer needed
#
#
#

include ../../common/commondefs.mk

  LIBRARYNAME =	libDbClient

  VERSIONFILE = dbV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
   		dbColumnType.C \
   		dbStateWrapper.C \
		dbStateManager.C \
		dbAggregateTransactionState.C \
		dbAppResSet.C \
		dbConnection.C \
		dbConnectionParameters.C \
		dbConnectionPool.C \
		dbV.C \
		dbMultiLineQuery.C \
		dbParam.C \
		dbQueryParameters.C \
		dbConv.C \
		TableSelect.C \
		dbDriver.C \
		dbRow.C \
		dbColumn.C \
		dbSet.C \
		dbCursor.C\
		dbQuery.C \
		SQLDialectFactory.C \
		SQLDialect.C \
		SQLString.C \
		dbSchemaMetaDataProvider.C \
        Status.C \
        TransferProtocol.C \
        Transaction.C \
        TmpNameStrategyTable.C \
	    LocalTransferMode.C \
        InvRelationTypes.C \
        FlowTags.C \
        NameIdAssociator.C 

   C++INCLUDE = \
        AnomalyTypes.h \
   		dbStat.h \
   		dbException.h \
   		dbColumnType.h \
		dbDriver.h \
		dbStateWrapper.h \
		dbStateManager.h \
		dbAggregateTransactionState.h \
		dbAppResSet.h \
   		dbConnection.h\
   		dbConnectionParameters.h\
		dbConnectionPool.h \
		dbRow.h \
		dbColumn.h \
		dbSet.h \
		dbCursor.h\
   		dbQuery.h \
		dbMultiLineQuery.h \
		dbParam.h\
		NameIdAssociator.h \
		NameIdAssociators.h \
		dbQueryParameters.h \
		dbConv.h \
		BandName.h \
		FileClasses.h \
		OriginatorName.h \
        OrdersAttachmentType.h \
		QualifiedFlag.h \
		RollingPoliciesAlgo.h \
		RollingPoliciesActions.h \
		SatelliteName.h \
		SensorName.h \
		StationName.h \
		StationAcronym.h \
		SiteName.h \
		SupertypeName.h \
		DataModeName.h \
		TableSelect.h \
		SQLDialectFactory.h \
		SQLDialect.h \
		SQLString.h \
		dbSchemaMetaDataProvider.h \
        SatelliteAcronym.h \
        Status.h \
        TransferProtocol.h \
        Transaction.h \
        TmpNameStrategyTable.h \
	    LocalTransferMode.h \
	    InvRelationTypes.h \
        FlowTags.h \
	    InSet.h \

COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 

DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:


include ../../common/commonrules.mk



