// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.5  2016/10/07 11:25:04  marpas
	new Venus query (VEN-2163)
	
	Revision 6.4  2016/10/06 17:01:58  marpas
	implementing VEN-2163 on HEAD - ready to be reported on venus branch - test to be fixed
	
	Revision 6.3  2016/07/27 14:46:38  damdec
	Fixed.
	
	Revision 6.2  2016/07/27 14:34:50  damdec
	dbBaseline and dbBaselineQuery handling removed
	
	Revision 6.1  2016/07/27 09:41:53  damdec
	OLD_PDS_SCHEMA flag removed.
	
	Revision 6.0  2016/07/07 13:44:03  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.2  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.1  2013/10/22 17:37:25  marpas
	DistPolOriginator & DistPolOriginatorQuery concepts removed
	
	Revision 4.0  2013/09/30 12:30:53  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.1  2013/09/26 16:05:28  lucio.pulvirenti
	Method added to return dbInventorySpecialQuery object.
	
	Revision 3.0  2013/09/19 14:57:21  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.28  2013/07/22 18:34:20  marpas
	aligning to new libDbData interface
	
	Revision 2.27  2013/07/02 12:03:19  marpas
	qa warnings and coding best practices
	
	Revision 2.26  2013/06/19 18:04:40  marpas
	adopting libException 5.x standards
	
	Revision 2.25  2013/03/29 19:51:28  marpas
	removing html related functions
	
	Revision 2.24  2012/11/28 13:13:50  marpas
	storage type is no longer needed with new schema - compile with PDS_OLD_SCHEMA to have it again
	
	Revision 2.23  2012/03/07 17:30:46  marpas
	new special query: dbInventoryOrderTriggeringFilesQuery added
	
	Revision 2.22  2012/02/14 12:52:45  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.21  2011/09/28 15:34:24  lucia.galli
	Fixed
	
	Revision 2.20  2011/09/28 13:29:39  lucia.galli
	New special query added
	
	Revision 2.19  2011/04/29 13:35:00  chicas
	work in progress for GeoValIntersect special query
	
	Revision 2.18  2010/10/27 15:30:49  chicas
	added dbInventoryNamedValCoverOrIntersectQuery
	
	Revision 2.17  2010/03/12 17:26:31  marfav
	Added Delayed Inventory Objects Special Queries
	
	Revision 2.16  2009/01/26 17:53:32  marpas
	special queries aliases introduced
	
	Revision 2.15  2008/12/16 15:59:33  marpas
	 work in progress
	
	Revision 2.14  2008/12/03 18:09:03  paoscu
	dbInventoryMultiBandOnSiteValCoverOrIntersectQuery Added
	
	Revision 2.13  2008/12/02 16:57:29  paoscu
	dbInventoryOnOrbitValCoverOrIntersectQuery added
	
	Revision 2.12  2008/12/01 20:07:05  paoscu
	Added dbInventoryOnSiteValCoverOrIntersectQuery
	
	Revision 2.11  2008/11/05 18:16:18  marpas
	dbInventoryQualified(Latest)Val(Cover/intersect)Query implemented
	
	Revision 2.10  2008/10/20 13:40:27  marpas
	dbInventorySite classes added
	
	Revision 2.9  2008/10/14 15:01:58  marpas
	dbOrdersAttachmentQuery class added
	
	Revision 2.8  2008/10/14 13:36:20  marpas
	ordersattachment class implemented and tested
	
	Revision 2.7  2008/03/21 18:09:16  paoscu
	dbInventoryValFullIntersectQuery added
	
	Revision 2.6  2007/04/18 22:12:55  marpas
	Management of dbDistributionpolicies relations with respect to data sets and originators
	
	Revision 2.5  2007/03/16 14:00:19  marpas
	work in progress
	
	Revision 2.4  2007/03/16 12:28:59  marpas
	work in progress
	
	Revision 2.3  2007/03/14 17:45:39  marpas
	work in progress
	
	Revision 2.2  2007/03/13 14:18:59  marpas
	work in progress
	
	Revision 2.1  2006/06/27 10:40:16  ivafam
	t_baseline added
	
	Revision 2.0  2006/02/28 09:09:34  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.33  2005/10/20 18:01:26  marpas
	embedded factory declaration removed. Now nit needs to be explicitely declared.
	useless (commented out) lines removed
	
	Revision 1.32  2005/07/21 10:23:56  marpas
	dbConfigurationSpace and its query moved to libDbConfiguration
	
	Revision 1.31  2005/06/20 10:36:11  marpas
	gaps management added
	
	Revision 1.30  2005/06/17 16:37:49  marpas
	Inventory gaps concepts introduced
	
	Revision 1.29  2005/06/15 07:44:49  integrator
	multiple storage manager implemented
	
	Revision 1.28  2005/03/09 18:03:23  marpas
	some messages fixed
	namespaces fixed
	
	Revision 1.27  2005/03/04 22:49:07  marpas
	dbWaitingOrder and dbWaitingOrderQuery classes  and their tests added , db_values script updated accordingly
	
	Revision 1.26  2005/01/03 14:14:26  ivafam
	The constructor of classes deriving from dbFileAction is protected.
	dbInventoryFileActionFactory becomes dbCryosatFileActionFactory
	dbCryosatFactory declares dbCryosatFileActionFactory
	FileAction basic classes moved in libDbData
	
	Revision 1.25  2004/12/30 22:47:59  paoscu
	headers fixed
	
	Revision 1.24  2004/09/24 12:21:14  ivafam
	Added dbOrdersInput class
	
	Revision 1.23  2004/09/08 07:43:53  ivafam
	Bug Fixed in ~dbCryosatFactory()
	
	Revision 1.22  2004/09/07 17:38:26  ivafam
	Bug Fixed
	
	Revision 1.21  2004/09/06 12:53:04  ivafam
	Class added
	
	Revision 1.20  2004/09/01 17:22:13  marpas
	*** empty log message ***
	
	Revision 1.19  2004/07/19 17:50:59  paoscu
	dbEMailAddress and dbEMailAddressQuery removed
	dbUser has set and get EMAIL ADDRESS
	
	Revision 1.18  2004/04/26 15:38:29  paoscu
	dbMediaCapFormat class added.
	
	Revision 1.17  2004/04/16 16:38:23  paoscu
	dbDistributionFormat added.
	
	Revision 1.16  2003/11/10 15:57:53  paoscu
	Regression bug fixed: inventory special queuries classes names were wrong.
	
	Revision 1.15  2003/10/21 10:41:39  paoscu
	staticClassName used instead string constants.
	
	Revision 1.14  2003/07/14 13:45:59  paoscu
	useless string ctor removed
	
	Revision 1.13  2003/07/08 18:32:22  paoscu
	DistributionView added.
	
	Revision 1.12  2003/07/08 17:40:00  paoscu
	dbAlgotithm and dbAlgotithmQuery added.
	
	Revision 1.11  2003/05/26 20:06:15  paoscu
	new Attributes factory instantiated
	
	Revision 1.10  2003/05/20 15:31:44  paoscu
	ConfigurationStyleSheets table added.
	
	Revision 1.9  2003/05/02 17:43:28  paoscu
	Class dbInventoryObjectWithComments added.
	
	Revision 1.8  2003/04/17 15:43:33  paoscu
	dbInventoryBestCenteredQuery added.
	
	Revision 1.7  2003/01/27 14:52:57  paoscu
	dbDistributedItem, dbDistributedItemQuery, dbProcessedOrder, dbProcessedOrderQuery eliminated
	
	Revision 1.6  2002/12/17 17:43:49  paoscu
	dbInventoryLatestValidityClosestQuery added.
	
	Revision 1.5  2002/12/16 14:06:05  danalt
	added dbOrderStateTransition table
	
	Revision 1.4  2002/12/13 18:41:06  paoscu
	Special Queries
	
	Revision 1.3  2002/11/27 11:26:04  paoscu
	More debug prints.
	
	Revision 1.2  2002/11/06 12:12:31  danalt
	aligned with libDbSpace
	
	Revision 1.1.1.1  2002/10/25 09:57:24  danalt
	Import libDbCryosat
		

*/

#include <dbCryosatFactory.h>

#include <dbConfigurationSchema.h>
#include <dbConfigurationSchemaQuery.h>

#include <dbConfigurationStyleSheet.h>
#include <dbConfigurationStyleSheetQuery.h>

#include <dbDistribution.h>
#include <dbDistributionQuery.h>

#include <dbDistributionItem.h>
#include <dbDistributionItemQuery.h>

#include <dbDistributionView.h>
#include <dbDistributionViewQuery.h>

#include <dbDistPolMDS.h>
#include <dbDistPolMDSQuery.h>

#include <dbFileType.h>
#include <dbFileTypeQuery.h>

#include <dbInventoryMDS.h>
#include <dbInventoryMDSQuery.h>

#include <dbInventoryAddParams.h>
#include <dbInventoryAddParamsQuery.h>

#include <dbInventorySite.h>
#include <dbInventorySiteQuery.h>

#include <dbInventoryComment.h>
#include <dbInventoryCommentQuery.h>

#include <dbInventoryGap.h>
#include <dbInventoryGapQuery.h>

#include <dbInventoryObjectTEC.h>
#include <dbInventoryObjectTECQuery.h>
#include <dbInventoryObject.h>
#include <dbInventoryObjectQuery.h>
#include <dbInventoryValCoverQuery.h>
#include <dbInventoryValIntersectQuery.h>
#include <dbInventoryValFullIntersectQuery.h>
#include <dbInventoryLatestValidityClosestQuery.h>
#include <dbInventoryBestCenteredQuery.h>
#include <dbInventoryNamedValCoverOrIntersectQuery.h>
#include <dbInventoryGeoValIntersectQuery.h>
#include <dbInventoryQualifiedValCoverOrIntersectQuery.h>
#include <dbInventoryOnSiteValCoverOrIntersectQuery.h>
#include <dbInventoryMultiBandOnSiteValCoverOrIntersectQuery.h>
#include <dbInventoryMultiBandValCoverOrIntersectQuery.h>
#include <dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery.h>
#include <dbInventoryOnOrbitValCoverOrIntersectQuery.h>
#include <dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery.h>
#include <dbInventoryObjectWithGapsComments.h>
#include <dbInventoryObjectWithGapsCommentsQuery.h>
#include <dbInventoryObjectWithGaps.h>
#include <dbInventoryObjectWithGapsQuery.h>
#include <dbInventoryOrderTriggeringFilesQuery.h>

#include <dbInventoryDelayedValCoverQuery.h>
#include <dbInventoryDelayedValIntersectQuery.h>

#include <dbMediaType.h>
#include <dbMediaTypeQuery.h>

#include <dbOnDemandDistribution.h>
#include <dbOnDemandDistributionQuery.h>

#include <dbOrder.h>
#include <dbOrderQuery.h>
#include <dbOrdersAttachment.h>
#include <dbOrdersAttachmentQuery.h>

#include <dbSMactions.h>
#include <dbSMactionsQuery.h>
#include <dbSMqueue.h>
#include <dbSMqueueQuery.h>


#include <dbWaitingOrder.h>
#include <dbWaitingOrderQuery.h>

#include <dbOrderGeneration.h>
#include <dbOrderGenerationQuery.h>

#include <dbAlgorithm.h>
#include <dbAlgorithmQuery.h>

#include <dbOrderStateTransition.h>
#include <dbOrderStateTransitionQuery.h>

#include <dbOrderType.h>
#include <dbOrderTypeQuery.h>

#include <dbProcessor.h>
#include <dbProcessorQuery.h>

#include <dbUser.h>
#include <dbUserQuery.h>

#include <dbUserType.h>
#include <dbUserTypeQuery.h>

#include <dbDistributionFormat.h>
#include <dbDistributionFormatQuery.h>

#include <dbMediaCapFormat.h>
#include <dbMediaCapFormatQuery.h>

#include <dbInvRollingPolicies.h>
#include <dbInvRollingPoliciesQuery.h>

#include <dbInvSpecialRollingPolicies.h>
#include <dbInvSpecialRollingPoliciesQuery.h>

#include <dbOrdersInput.h>
#include <dbOrdersInputQuery.h>

#include <dbPersistentFactoryChain.h>
#include <dbQueryFactoryChain.h>
#include <dbConnectionPool.h>

#include <Filterables.h>

#include <dbcsV.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

namespace {
    // versioning support
    dbcsV version ;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbCryosatFactory);



dbCryosatFactory::dbCryosatFactory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		

	registerDbPersistentFactoryMethod(dbConfigurationSchema::staticClassName() , &buildDbConfigurationSchema );
	registerDbQueryFactoryMethod(dbConfigurationSchemaQuery::staticClassName() , &buildDbConfigurationSchemaQuery );

	registerDbPersistentFactoryMethod(dbConfigurationStyleSheet::staticClassName() , &buildDbConfigurationStyleSheet );
	registerDbQueryFactoryMethod(dbConfigurationStyleSheetQuery::staticClassName() , &buildDbConfigurationStyleSheetQuery );

	registerDbPersistentFactoryMethod(dbDistribution::staticClassName() , &buildDbDistribution );
	registerDbQueryFactoryMethod(dbDistributionQuery::staticClassName() , &buildDbDistributionQuery );

	registerDbPersistentFactoryMethod(dbDistributionItem::staticClassName() , &buildDbDistributionItem );
	registerDbQueryFactoryMethod(dbDistributionItemQuery::staticClassName() , &buildDbDistributionItemQuery );

	registerDbPersistentFactoryMethod(dbDistributionView::staticClassName() , &buildDbDistributionView );
	registerDbQueryFactoryMethod(dbDistributionViewQuery::staticClassName() , &buildDbDistributionViewQuery );

	registerDbPersistentFactoryMethod(dbDistPolMDS::staticClassName() , &buildDbDistPolMDS );
	registerDbQueryFactoryMethod(dbDistPolMDSQuery::staticClassName() , &buildDbDistPolMDSQuery );

	registerDbPersistentFactoryMethod(dbFileType::staticClassName() , &buildDbFileType );
	registerDbQueryFactoryMethod(dbFileTypeQuery::staticClassName() , &buildDbFileTypeQuery );

	registerDbPersistentFactoryMethod(dbInventoryMDS::staticClassName() , &buildDbInventoryMDS );
	registerDbQueryFactoryMethod(dbInventoryMDSQuery::staticClassName() , &buildDbInventoryMDSQuery );

	registerDbPersistentFactoryMethod(dbInventoryAddParams::staticClassName() , &buildDbInventoryAddParams );
	registerDbQueryFactoryMethod(dbInventoryAddParamsQuery::staticClassName() , &buildDbInventoryAddParamsQuery );

	registerDbPersistentFactoryMethod(dbInventorySite::staticClassName() , &buildDbInventorySite );
	registerDbQueryFactoryMethod(dbInventorySiteQuery::staticClassName() , &buildDbInventorySiteQuery );

	registerDbPersistentFactoryMethod(dbInventoryComment::staticClassName() , &buildDbInventoryComment );
	registerDbQueryFactoryMethod(dbInventoryCommentQuery::staticClassName() , &buildDbInventoryCommentQuery );

	registerDbPersistentFactoryMethod(dbInventoryGap::staticClassName() , &buildDbInventoryGap );
	registerDbQueryFactoryMethod(dbInventoryGapQuery::staticClassName() , &buildDbInventoryGapQuery );

	registerDbPersistentFactoryMethod(dbInventoryObject::staticClassName() , &buildDbInventoryObject );
	registerDbPersistentObjType(dbInventoryObject::staticClassName() , "Inventory");
	registerDbQueryFactoryMethod(dbInventoryObjectQuery::staticClassName(), &buildDbInventoryObjectQuery );

	registerDbPersistentFactoryMethod(dbInventoryObjectTEC::staticClassName() , &buildDbInventoryObjectTEC );
	registerDbPersistentObjType(dbInventoryObjectTEC::staticClassName() , "Inventory");
	registerDbQueryFactoryMethod(dbInventoryObjectTECQuery::staticClassName(), &buildDbInventoryObjectTECQuery );

	registerDbPersistentFactoryMethod(dbInventoryObjectWithGapsComments::staticClassName(), &buildDbInventoryObjectWithGapsComments );
	registerDbPersistentObjType(dbInventoryObjectWithGapsComments::staticClassName(), "Inventory");
	registerDbQueryFactoryMethod(dbInventoryObjectWithGapsCommentsQuery::staticClassName(), &buildDbInventoryObjectWithGapsCommentsQuery );

	//special inventory queries
	registerDbQueryFactoryMethod("dbInventoryValCoverQuery" , &buildDbInventoryValCoverQuery );
	registerDbQueryFactoryMethod("dbInventoryLatestValCoverQuery", &buildDbInventoryLatestValCoverQuery );
	registerDbQueryFactoryMethod("dbInventoryValIntersectQuery", &buildDbInventoryValIntersectQuery );
	registerDbQueryFactoryMethod("dbInventoryValFullIntersectQuery", &buildDbInventoryValFullIntersectQuery );
	registerDbQueryFactoryMethod("dbInventoryLatestValIntersectQuery", &buildDbInventoryLatestValIntersectQuery );
	registerDbQueryFactoryMethod("dbInventoryLatestValidityClosestQuery", &buildDbInventoryLatestValidityClosestQuery );
	registerDbQueryFactoryMethod("dbInventoryBestCenteredQuery", &buildDbInventoryBestCenteredQuery );

	registerDbQueryFactoryMethod("dbInventoryNamedLatestValCoverQuery", &buildDbInventoryNamedLatestValCoverQuery );
	registerDbQueryFactoryMethod("dbInventoryNamedLatestValIntersectQuery", &buildDbInventoryNamedLatestValIntersectQuery );
	registerDbQueryFactoryMethod("dbInventoryNamedValCoverQuery", &buildDbInventoryNamedValCoverQuery );
	registerDbQueryFactoryMethod("dbInventoryNamedValIntersectQuery", &buildDbInventoryNamedValIntersectQuery );
	
	registerDbQueryFactoryMethod("dbInventoryGeoValIntersectQuery", &buildDbInventoryGeoValIntersectQuery );

	registerDbQueryFactoryMethod("dbInventoryQualifiedLatestValCoverQuery", &buildDbInventoryQualifiedLatestValCoverQuery );
	registerDbQueryFactoryMethod("dbInventoryQualifiedLatestValIntersectQuery", &buildDbInventoryQualifiedLatestValIntersectQuery );
	registerDbQueryFactoryMethod("dbInventoryQualifiedValCoverQuery", &buildDbInventoryQualifiedValCoverQuery );
	registerDbQueryFactoryMethod("dbInventoryQualifiedValIntersectQuery", &buildDbInventoryQualifiedValIntersectQuery );
	
	registerDbQueryFactoryMethod("dbInventoryOnSiteLatestValCoverQuery", &buildDbInventoryOnSiteLatestValCoverQuery );
	registerDbQueryFactoryMethod("dbInventoryLatestValCoverOnSiteQuery", &buildDbInventoryOnSiteLatestValCoverQuery );

	registerDbQueryFactoryMethod("dbInventoryOnSiteLatestValIntersectQuery", &buildDbInventoryOnSiteLatestValIntersectQuery );
	registerDbQueryFactoryMethod("dbInventoryLatestValIntersectOnSiteQuery", &buildDbInventoryOnSiteLatestValIntersectQuery );

	registerDbQueryFactoryMethod("dbInventoryOnSiteValCoverQuery", &buildDbInventoryOnSiteValCoverQuery );
	registerDbQueryFactoryMethod("dbInventoryValCoverOnSiteQuery", &buildDbInventoryOnSiteValCoverQuery );

	registerDbQueryFactoryMethod("dbInventoryOnSiteValIntersectQuery", &buildDbInventoryOnSiteValIntersectQuery );
	registerDbQueryFactoryMethod("dbInventoryValIntersectOnSiteQuery", &buildDbInventoryOnSiteValIntersectQuery );
	
	registerDbQueryFactoryMethod("dbInventoryMultiBandOnSiteLatestValCoverQuery", &buildDbInventoryMultiBandOnSiteLatestValCoverQuery );
	registerDbQueryFactoryMethod("dbInventoryMultiBandLatestValCoverOnSiteQuery", &buildDbInventoryMultiBandOnSiteLatestValCoverQuery );

	registerDbQueryFactoryMethod("dbInventoryMultiBandOnSiteLatestValIntersectQuery", &buildDbInventoryMultiBandOnSiteLatestValIntersectQuery );
	registerDbQueryFactoryMethod("dbInventoryMultiBandLatestValIntersectOnSiteQuery", &buildDbInventoryMultiBandOnSiteLatestValIntersectQuery );

	registerDbQueryFactoryMethod("dbInventoryMultiBandOnSiteValCoverQuery", &buildDbInventoryMultiBandOnSiteValCoverQuery );
	registerDbQueryFactoryMethod("dbInventoryMultiBandValCoverOnSiteQuery", &buildDbInventoryMultiBandOnSiteValCoverQuery );

	registerDbQueryFactoryMethod("dbInventoryMultiBandOnSiteValIntersectQuery", &buildDbInventoryMultiBandOnSiteValIntersectQuery );
	registerDbQueryFactoryMethod("dbInventoryMultiBandValIntersectOnSiteQuery", &buildDbInventoryMultiBandOnSiteValIntersectQuery );
	
	registerDbQueryFactoryMethod("dbInventoryMultiBandLatestValCoverQuery", &buildDbInventoryMultiBandLatestValCoverQuery );
	registerDbQueryFactoryMethod("dbInventoryMultiBandLatestValIntersectQuery", &buildDbInventoryMultiBandLatestValIntersectQuery );
	registerDbQueryFactoryMethod("dbInventoryMultiBandValCoverQuery", &buildDbInventoryMultiBandValCoverQuery );
	registerDbQueryFactoryMethod("dbInventoryMultiBandValIntersectQuery", &buildDbInventoryMultiBandValIntersectQuery );
	
	registerDbQueryFactoryMethod("dbInventoryTDIMultiBandOnSiteLatestValCoverQuery", &buildDbInventoryTDIMultiBandOnSiteLatestValCoverQuery );
	registerDbQueryFactoryMethod("dbInventoryMultiBandTDILatestValCoverOnSiteQuery", &buildDbInventoryTDIMultiBandOnSiteLatestValCoverQuery );

	registerDbQueryFactoryMethod("dbInventoryTDIMultiBandOnSiteLatestValIntersectQuery", &buildDbInventoryTDIMultiBandOnSiteLatestValIntersectQuery );
	registerDbQueryFactoryMethod("dbInventoryMultiBandTDILatestValIntersectOnSiteQuery", &buildDbInventoryTDIMultiBandOnSiteLatestValIntersectQuery );

	registerDbQueryFactoryMethod("dbInventoryTDIMultiBandOnSiteValCoverQuery", &buildDbInventoryTDIMultiBandOnSiteValCoverQuery );
	registerDbQueryFactoryMethod("dbInventoryMultiBandTDIValCoverOnSiteQuery", &buildDbInventoryTDIMultiBandOnSiteValCoverQuery );

	registerDbQueryFactoryMethod("dbInventoryTDIMultiBandOnSiteValIntersectQuery", &buildDbInventoryTDIMultiBandOnSiteValIntersectQuery );
	registerDbQueryFactoryMethod("dbInventoryMultiBandTDIValIntersectOnSiteQuery", &buildDbInventoryTDIMultiBandOnSiteValIntersectQuery );
	
	registerDbQueryFactoryMethod("dbInventoryOnOrbitQualifiedLatestValIntersectQuery", &buildDbInventoryOnOrbitQualifiedLatestValIntersectQuery );
	registerDbQueryFactoryMethod("dbInventoryOnOrbitQualifiedLatestValCoverQuery", &buildDbInventoryOnOrbitQualifiedLatestValCoverQuery );
	registerDbQueryFactoryMethod("dbInventoryOnOrbitLatestValCoverQuery", &buildDbInventoryOnOrbitLatestValCoverQuery );
	registerDbQueryFactoryMethod("dbInventoryLatestValCoverOnOrbitQuery", &buildDbInventoryOnOrbitLatestValCoverQuery );

	registerDbQueryFactoryMethod("dbInventoryOnOrbitLatestValIntersectQuery", &buildDbInventoryOnOrbitLatestValIntersectQuery );
	registerDbQueryFactoryMethod("dbInventoryLatestValIntersectOnOrbitQuery", &buildDbInventoryOnOrbitLatestValIntersectQuery );

	registerDbQueryFactoryMethod("dbInventoryOnOrbitValCoverQuery", &buildDbInventoryOnOrbitValCoverQuery );
	registerDbQueryFactoryMethod("dbInventoryValCoverOnOrbitQuery", &buildDbInventoryOnOrbitValCoverQuery );

	registerDbQueryFactoryMethod("dbInventoryOnOrbitValIntersectQuery", &buildDbInventoryOnOrbitValIntersectQuery );
	registerDbQueryFactoryMethod("dbInventoryValIntersectOnOrbitQuery", &buildDbInventoryOnOrbitValIntersectQuery );
	
	registerDbQueryFactoryMethod("dbInventoryDelayedValCoverQuery" , &buildDbInventoryDelayedValCoverQuery );
	registerDbQueryFactoryMethod("dbInventoryDelayedLatestValCoverQuery", &buildDbInventoryDelayedLatestValCoverQuery );
	registerDbQueryFactoryMethod("dbInventoryDelayedValIntersectQuery", &buildDbInventoryDelayedValIntersectQuery );
	registerDbQueryFactoryMethod("dbInventoryDelayedLatestValIntersectQuery", &buildDbInventoryDelayedLatestValIntersectQuery );
	
	registerDbQueryFactoryMethod("dbInventoryOrderTriggeringFilesQuery", &buildDbInventoryOrderTriggeringFilesQuery );
	registerDbQueryFactoryMethod("dbInventorySpecialQuery", &buildDbInventoryObjectSpecialQuery );

	registerDbPersistentFactoryMethod(dbMediaType::staticClassName(), &buildDbMediaType );
	registerDbQueryFactoryMethod(dbMediaTypeQuery::staticClassName(), &buildDbMediaTypeQuery );

	registerDbPersistentFactoryMethod(dbOnDemandDistribution::staticClassName(), &buildDbOnDemandDistribution );
	registerDbQueryFactoryMethod(dbOnDemandDistributionQuery::staticClassName(), &buildDbOnDemandDistributionQuery );

	registerDbPersistentFactoryMethod(dbOrder::staticClassName(), &buildDbOrder );
	registerDbQueryFactoryMethod(dbOrderQuery::staticClassName(), &buildDbOrderQuery );

	registerDbPersistentFactoryMethod(dbOrdersAttachment::staticClassName(), &buildDbOrdersAttachment );
	registerDbQueryFactoryMethod(dbOrdersAttachmentQuery::staticClassName(), &buildDbOrdersAttachmentQuery );

	registerDbPersistentFactoryMethod(dbSMactions::staticClassName(), &buildDbSMactions );
	registerDbQueryFactoryMethod(dbSMactionsQuery::staticClassName(), &buildDbSMactionsQuery );

	registerDbPersistentFactoryMethod(dbSMqueue::staticClassName(), &buildDbSMqueue );
	registerDbQueryFactoryMethod(dbSMqueueQuery::staticClassName(), &buildDbSMqueueQuery );

	registerDbPersistentFactoryMethod(dbWaitingOrder::staticClassName(), &buildDbWaitingOrder );
	registerDbQueryFactoryMethod(dbWaitingOrderQuery::staticClassName(), &buildDbWaitingOrderQuery );

	registerDbPersistentFactoryMethod(dbOrderGeneration::staticClassName(), &buildDbOrderGeneration );
	registerDbQueryFactoryMethod(dbOrderGenerationQuery::staticClassName(), &buildDbOrderGenerationQuery );

	registerDbPersistentFactoryMethod(dbAlgorithm::staticClassName(), &buildDbAlgorithm );
	registerDbQueryFactoryMethod(dbAlgorithmQuery::staticClassName(), &buildDbAlgorithmQuery );

	registerDbPersistentFactoryMethod(dbOrderStateTransition::staticClassName() , &buildDbOrderStateTransition );
	registerDbQueryFactoryMethod(dbOrderStateTransitionQuery::staticClassName(), &buildDbOrderStateTransitionQuery );

	registerDbPersistentFactoryMethod(dbOrderType::staticClassName(), &buildDbOrderType );
	registerDbQueryFactoryMethod(dbOrderTypeQuery::staticClassName(), &buildDbOrderTypeQuery );

	registerDbPersistentFactoryMethod(dbProcessor::staticClassName(), &buildDbProcessor );
	registerDbQueryFactoryMethod(dbProcessorQuery::staticClassName(), &buildDbProcessorQuery );

	registerDbPersistentFactoryMethod(dbUser::staticClassName(), &buildDbUser );
	registerDbQueryFactoryMethod(dbUserQuery::staticClassName(), &buildDbUserQuery );

	registerDbPersistentFactoryMethod(dbUserType::staticClassName(), &buildDbUserType );
	registerDbQueryFactoryMethod(dbUserTypeQuery::staticClassName(), &buildDbUserTypeQuery );

	registerDbPersistentFactoryMethod(dbDistributionFormat::staticClassName(), &buildDbDistributionFormat );
	registerDbQueryFactoryMethod(dbDistributionFormatQuery::staticClassName(), &buildDbDistributionFormatQuery );

	registerDbPersistentFactoryMethod(dbMediaCapFormat::staticClassName(), &buildDbMediaCapFormat );
	registerDbQueryFactoryMethod(dbMediaCapFormatQuery::staticClassName(), &buildDbMediaCapFormatQuery );
	
	registerDbPersistentFactoryMethod(dbInvRollingPolicies::staticClassName(), &buildDbInvRollingPolicies );
	registerDbQueryFactoryMethod(dbInvRollingPoliciesQuery::staticClassName(), &buildDbInvRollingPoliciesQuery );
	
	registerDbPersistentFactoryMethod(dbInvSpecialRollingPolicies::staticClassName(), &buildDbInvSpecialRollingPolicies );
	registerDbQueryFactoryMethod(dbInvSpecialRollingPoliciesQuery::staticClassName(), &buildDbInvSpecialRollingPoliciesQuery );
	
	registerDbPersistentFactoryMethod(dbOrdersInput::staticClassName(), &buildDbOrdersInput );
	registerDbQueryFactoryMethod(dbOrdersInputQuery::staticClassName(), &buildDbOrdersInputQuery );

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		

}

dbCryosatFactory::~dbCryosatFactory() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
    try {
	    unregisterDbPersistentFactoryMethod(dbConfigurationSchema::staticClassName(), &buildDbConfigurationSchema );
	    unregisterDbQueryFactoryMethod(dbConfigurationSchemaQuery::staticClassName(), &buildDbConfigurationSchemaQuery );

	    unregisterDbPersistentFactoryMethod(dbConfigurationStyleSheet::staticClassName(), &buildDbConfigurationStyleSheet );
	    unregisterDbQueryFactoryMethod(dbConfigurationStyleSheetQuery::staticClassName(), &buildDbConfigurationStyleSheetQuery );

	    unregisterDbPersistentFactoryMethod(dbDistribution::staticClassName(), &buildDbDistribution );
	    unregisterDbQueryFactoryMethod(dbDistributionQuery::staticClassName(), &buildDbDistributionQuery );

	    unregisterDbPersistentFactoryMethod(dbDistributionItem::staticClassName(), &buildDbDistributionItem );
	    unregisterDbQueryFactoryMethod(dbDistributionItemQuery::staticClassName(), &buildDbDistributionItemQuery );

	    unregisterDbPersistentFactoryMethod(dbDistributionView::staticClassName(), &buildDbDistributionView );
	    unregisterDbQueryFactoryMethod(dbDistributionViewQuery::staticClassName(), &buildDbDistributionViewQuery );

	    unregisterDbPersistentFactoryMethod(dbDistPolMDS::staticClassName() , &buildDbDistPolMDS );
	    unregisterDbQueryFactoryMethod(dbDistPolMDSQuery::staticClassName() , &buildDbDistPolMDSQuery );

	    unregisterDbPersistentFactoryMethod(dbFileType::staticClassName(), &buildDbFileType );
	    unregisterDbQueryFactoryMethod(dbFileTypeQuery::staticClassName(), &buildDbFileTypeQuery );

	    unregisterDbPersistentFactoryMethod(dbInventoryGap::staticClassName(), &buildDbInventoryGap );
	    unregisterDbQueryFactoryMethod(dbInventoryGapQuery::staticClassName(), &buildDbInventoryGapQuery );

	    unregisterDbPersistentFactoryMethod(dbInventoryMDS::staticClassName(), &buildDbInventoryMDS );
	    unregisterDbQueryFactoryMethod(dbInventoryMDSQuery::staticClassName(), &buildDbInventoryMDSQuery );

	    unregisterDbPersistentFactoryMethod(dbInventoryAddParams::staticClassName(), &buildDbInventoryAddParams );
	    unregisterDbQueryFactoryMethod(dbInventoryAddParamsQuery::staticClassName(), &buildDbInventoryAddParamsQuery );

	    unregisterDbPersistentFactoryMethod(dbInventorySite::staticClassName(), &buildDbInventorySite );
	    unregisterDbQueryFactoryMethod(dbInventorySiteQuery::staticClassName(), &buildDbInventorySiteQuery );

	    unregisterDbPersistentFactoryMethod(dbInventoryComment::staticClassName(), &buildDbInventoryComment );
	    unregisterDbQueryFactoryMethod(dbInventoryCommentQuery::staticClassName(), &buildDbInventoryCommentQuery );

	    unregisterDbPersistentFactoryMethod(dbInventoryObject::staticClassName(), &buildDbInventoryObject );
	    unregisterDbPersistentObjType(dbInventoryObject::staticClassName() , "Inventory");
	    unregisterDbQueryFactoryMethod(dbInventoryObjectQuery::staticClassName(), &buildDbInventoryObjectQuery );

	    unregisterDbPersistentFactoryMethod(dbInventoryObjectTEC::staticClassName(), &buildDbInventoryObjectTEC );
	    unregisterDbPersistentObjType(dbInventoryObjectTEC::staticClassName() , "Inventory");
	    unregisterDbQueryFactoryMethod(dbInventoryObjectTECQuery::staticClassName(), &buildDbInventoryObjectTECQuery );





	    unregisterDbPersistentFactoryMethod(dbInventoryObjectWithGapsComments::staticClassName(), &buildDbInventoryObjectWithGapsComments );
	    unregisterDbPersistentObjType(dbInventoryObjectWithGapsComments::staticClassName(), "Inventory");
	    unregisterDbQueryFactoryMethod(dbInventoryObjectWithGapsCommentsQuery::staticClassName(), &buildDbInventoryObjectWithGapsCommentsQuery);

	    //special inventory queries
	    unregisterDbQueryFactoryMethod("dbInventoryValCoverQuery", &buildDbInventoryValCoverQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryLatestValCoverQuery", &buildDbInventoryLatestValCoverQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryValIntersectQuery", &buildDbInventoryValIntersectQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryValFullIntersectQuery", &buildDbInventoryValFullIntersectQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryLatestValIntersectQuery", &buildDbInventoryLatestValIntersectQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryLatestValidityClosestQuery", &buildDbInventoryLatestValidityClosestQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryBestCenteredQuery", &buildDbInventoryBestCenteredQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryNamedLatestValCoverQuery", &buildDbInventoryNamedLatestValCoverQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryNamedLatestValIntersectQuery", &buildDbInventoryNamedLatestValIntersectQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryNamedValCoverQuery", &buildDbInventoryNamedValCoverQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryNamedValIntersectQuery", &buildDbInventoryNamedValIntersectQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryGeoValIntersectQuery", &buildDbInventoryGeoValIntersectQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryQualifiedLatestValCoverQuery", &buildDbInventoryQualifiedLatestValCoverQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryQualifiedLatestValIntersectQuery", &buildDbInventoryQualifiedLatestValIntersectQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryQualifiedValCoverQuery", &buildDbInventoryQualifiedValCoverQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryQualifiedValIntersectQuery", &buildDbInventoryQualifiedValIntersectQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryOnSiteLatestValCoverQuery", &buildDbInventoryOnSiteLatestValCoverQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryLatestValCoverOnSiteQuery", &buildDbInventoryOnSiteLatestValCoverQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryOnSiteLatestValIntersectQuery", &buildDbInventoryOnSiteLatestValIntersectQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryLatestValIntersectOnSiteQuery", &buildDbInventoryOnSiteLatestValIntersectQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryOnSiteValCoverQuery", &buildDbInventoryOnSiteValCoverQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryValCoverOnSiteQuery", &buildDbInventoryOnSiteValCoverQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryOnSiteValIntersectQuery", &buildDbInventoryOnSiteValIntersectQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryValIntersectOnSiteQuery", &buildDbInventoryOnSiteValIntersectQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryMultiBandOnSiteLatestValCoverQuery", &buildDbInventoryMultiBandOnSiteLatestValCoverQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryMultiBandLatestValCoverOnSiteQuery", &buildDbInventoryMultiBandOnSiteLatestValCoverQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryMultiBandOnSiteLatestValIntersectQuery", &buildDbInventoryMultiBandOnSiteLatestValIntersectQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryMultiBandLatestValIntersectOnSiteQuery", &buildDbInventoryMultiBandOnSiteLatestValIntersectQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryMultiBandOnSiteValCoverQuery", &buildDbInventoryMultiBandOnSiteValCoverQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryMultiBandValCoverOnSiteQuery", &buildDbInventoryMultiBandOnSiteValCoverQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryMultiBandOnSiteValIntersectQuery", &buildDbInventoryMultiBandOnSiteValIntersectQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryMultiBandValIntersectOnSiteQuery", &buildDbInventoryMultiBandOnSiteValIntersectQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryMultiBandLatestValCoverQuery", &buildDbInventoryMultiBandLatestValCoverQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryMultiBandLatestValIntersectQuery", &buildDbInventoryMultiBandLatestValIntersectQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryMultiBandValCoverQuery", &buildDbInventoryMultiBandValCoverQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryMultiBandValIntersectQuery", &buildDbInventoryMultiBandValIntersectQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryTDIMultiBandOnSiteLatestValCoverQuery", &buildDbInventoryTDIMultiBandOnSiteLatestValCoverQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryMultiBandTDILatestValCoverOnSiteQuery", &buildDbInventoryTDIMultiBandOnSiteLatestValCoverQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryTDIMultiBandOnSiteLatestValIntersectQuery", &buildDbInventoryTDIMultiBandOnSiteLatestValIntersectQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryMultiBandTDILatestValIntersectOnSiteQuery", &buildDbInventoryTDIMultiBandOnSiteLatestValIntersectQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryTDIMultiBandOnSiteValCoverQuery", &buildDbInventoryTDIMultiBandOnSiteValCoverQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryMultiBandTDIValCoverOnSiteQuery", &buildDbInventoryTDIMultiBandOnSiteValCoverQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryTDIMultiBandOnSiteValIntersectQuery", &buildDbInventoryTDIMultiBandOnSiteValIntersectQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryMultiBandTDIValIntersectOnSiteQuery", &buildDbInventoryTDIMultiBandOnSiteValIntersectQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryOnOrbitQualifiedLatestValIntersectQuery", &buildDbInventoryOnOrbitQualifiedLatestValIntersectQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryOnOrbitQualifiedLatestValCoverQuery", &buildDbInventoryOnOrbitQualifiedLatestValCoverQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryOnOrbitLatestValCoverQuery", &buildDbInventoryOnOrbitLatestValCoverQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryLatestValCoverOnOrbitQuery", &buildDbInventoryOnOrbitLatestValCoverQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryOnOrbitLatestValIntersectQuery", &buildDbInventoryOnOrbitLatestValIntersectQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryLatestValIntersectOnOrbitQuery", &buildDbInventoryOnOrbitLatestValIntersectQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryOnOrbitValCoverQuery", &buildDbInventoryOnOrbitValCoverQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryValCoverOnOrbitQuery", &buildDbInventoryOnOrbitValCoverQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryOnOrbitValIntersectQuery", &buildDbInventoryOnOrbitValIntersectQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryValIntersectOnOrbitQuery", &buildDbInventoryOnOrbitValIntersectQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryDelayedValCoverQuery" , &buildDbInventoryDelayedValCoverQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryDelayedLatestValCoverQuery", &buildDbInventoryDelayedLatestValCoverQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryDelayedValIntersectQuery", &buildDbInventoryDelayedValIntersectQuery );
	    unregisterDbQueryFactoryMethod("dbInventoryDelayedLatestValIntersectQuery", &buildDbInventoryDelayedLatestValIntersectQuery );

	    unregisterDbQueryFactoryMethod("dbInventoryOrderTriggeringFilesQuery", &buildDbInventoryOrderTriggeringFilesQuery );
		unregisterDbQueryFactoryMethod("dbInventorySpecialQuery", &buildDbInventoryObjectSpecialQuery );

	    unregisterDbPersistentFactoryMethod(dbMediaType::staticClassName(), &buildDbMediaType );
	    unregisterDbQueryFactoryMethod(dbMediaTypeQuery::staticClassName(), &buildDbMediaTypeQuery );

	    unregisterDbPersistentFactoryMethod(dbOnDemandDistribution::staticClassName(), &buildDbOnDemandDistribution );
	    unregisterDbQueryFactoryMethod(dbOnDemandDistributionQuery::staticClassName(), &buildDbOnDemandDistributionQuery );

	    unregisterDbPersistentFactoryMethod(dbOrder::staticClassName(), &buildDbOrder );
	    unregisterDbQueryFactoryMethod(dbOrderQuery::staticClassName(), &buildDbOrderQuery );

	    unregisterDbPersistentFactoryMethod(dbOrdersAttachment::staticClassName(), &buildDbOrdersAttachment );
	    unregisterDbQueryFactoryMethod(dbOrdersAttachmentQuery::staticClassName(), &buildDbOrdersAttachmentQuery );

	    unregisterDbPersistentFactoryMethod(dbSMactions::staticClassName(), &buildDbSMactions );
	    unregisterDbQueryFactoryMethod(dbSMactionsQuery::staticClassName(), &buildDbSMactionsQuery );

	    unregisterDbPersistentFactoryMethod(dbSMqueue::staticClassName(), &buildDbSMqueue );
	    unregisterDbQueryFactoryMethod(dbSMqueueQuery::staticClassName(), &buildDbSMqueueQuery );

	    unregisterDbPersistentFactoryMethod(dbWaitingOrder::staticClassName(), &buildDbWaitingOrder );
	    unregisterDbQueryFactoryMethod(dbWaitingOrderQuery::staticClassName(), &buildDbWaitingOrderQuery );

	    unregisterDbPersistentFactoryMethod(dbOrderGeneration::staticClassName(), &buildDbOrderGeneration );
	    unregisterDbQueryFactoryMethod(dbOrderGenerationQuery::staticClassName(), &buildDbOrderGenerationQuery );

	    unregisterDbPersistentFactoryMethod(dbAlgorithm::staticClassName(), &buildDbAlgorithm );
	    unregisterDbQueryFactoryMethod(dbAlgorithmQuery::staticClassName(), &buildDbAlgorithmQuery );

	    unregisterDbPersistentFactoryMethod(dbOrderStateTransition::staticClassName(), &buildDbOrderStateTransition );
	    unregisterDbQueryFactoryMethod(dbOrderStateTransitionQuery::staticClassName(), &buildDbOrderStateTransitionQuery );

	    unregisterDbPersistentFactoryMethod(dbOrderType::staticClassName(), &buildDbOrderType );
	    unregisterDbQueryFactoryMethod(dbOrderTypeQuery::staticClassName() , &buildDbOrderTypeQuery );

	    unregisterDbPersistentFactoryMethod(dbProcessor::staticClassName(), &buildDbProcessor );
	    unregisterDbQueryFactoryMethod(dbProcessorQuery::staticClassName(), &buildDbProcessorQuery );

	    unregisterDbPersistentFactoryMethod(dbUser::staticClassName(), &buildDbUser );
	    unregisterDbQueryFactoryMethod(dbUserQuery::staticClassName(), &buildDbUserQuery );

	    unregisterDbPersistentFactoryMethod(dbUserType::staticClassName(), &buildDbUserType );
	    unregisterDbQueryFactoryMethod(dbUserTypeQuery::staticClassName(), &buildDbUserTypeQuery );

	    unregisterDbPersistentFactoryMethod(dbDistributionFormat::staticClassName(), &buildDbDistributionFormat );
	    unregisterDbQueryFactoryMethod(dbDistributionFormatQuery::staticClassName(), &buildDbDistributionFormatQuery );

	    unregisterDbPersistentFactoryMethod(dbMediaCapFormat::staticClassName(), &buildDbMediaCapFormat );
	    unregisterDbQueryFactoryMethod(dbMediaCapFormatQuery::staticClassName(), &buildDbMediaCapFormatQuery );

	    unregisterDbPersistentFactoryMethod(dbInvRollingPolicies::staticClassName(), &buildDbInvRollingPolicies );
	    unregisterDbQueryFactoryMethod(dbInvRollingPoliciesQuery::staticClassName(), &buildDbInvRollingPoliciesQuery );

	    unregisterDbPersistentFactoryMethod(dbInvSpecialRollingPolicies::staticClassName(), &buildDbInvSpecialRollingPolicies );
	    unregisterDbQueryFactoryMethod(dbInvSpecialRollingPoliciesQuery::staticClassName(), &buildDbInvSpecialRollingPoliciesQuery );

	    unregisterDbPersistentFactoryMethod(dbOrdersInput::staticClassName(), &buildDbOrdersInput );
	    unregisterDbQueryFactoryMethod(dbOrdersInputQuery::staticClassName(), &buildDbOrdersInputQuery );

    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		


}

// *************** dbConfigurationSchema ***************************

dbPersistent* dbCryosatFactory::buildDbConfigurationSchema(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbConfigurationSchema;
}


dbGeneralQuery* dbCryosatFactory::buildDbConfigurationSchemaQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbConfigurationSchemaQuery(dbKey) ;
}


// *************** dbConfigurationStyleSheet ***************************

dbPersistent* dbCryosatFactory::buildDbConfigurationStyleSheet(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbConfigurationStyleSheet;
}


dbGeneralQuery* dbCryosatFactory::buildDbConfigurationStyleSheetQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbConfigurationStyleSheetQuery(dbKey) ;
}


// *************** dbDistribution ***************************

dbPersistent *dbCryosatFactory::buildDbDistribution(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbDistribution;
}

dbGeneralQuery *dbCryosatFactory::buildDbDistributionQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbDistributionQuery(dbKey) ;
}

// *************** dbDistributionItem ***************************

dbPersistent *dbCryosatFactory::buildDbDistributionItem(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbDistributionItem;
}

dbGeneralQuery *dbCryosatFactory::buildDbDistributionItemQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbDistributionItemQuery(dbKey) ;
}

// *************** dbDistributionView ***************************

dbPersistent *dbCryosatFactory::buildDbDistributionView(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbDistributionView;
}

dbGeneralQuery *dbCryosatFactory::buildDbDistributionViewQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbDistributionViewQuery(dbKey) ;
}

// *************** dbDistPolMDS ***************************

dbPersistent *dbCryosatFactory::buildDbDistPolMDS(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    		
	return new dbDistPolMDS(dbConnectionWrapper(db::ConnPools::instance()->getPool(dbKey)));
}

dbGeneralQuery *dbCryosatFactory::buildDbDistPolMDSQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbDistPolMDSQuery(dbKey) ;
}


// *************** dbFileType ***************************

dbPersistent *dbCryosatFactory::buildDbFileType(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbFileType;
}

dbGeneralQuery *dbCryosatFactory::buildDbFileTypeQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbFileTypeQuery(dbKey) ;
}

// *************** dbInventoryMDS ***************************

dbPersistent *dbCryosatFactory::buildDbInventoryMDS(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryMDS(dbConnectionWrapper(db::ConnPools::instance()->getPool(dbKey)));
}


dbGeneralQuery *dbCryosatFactory::buildDbInventoryMDSQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryMDSQuery(dbKey) ;
}

// *************** dbInventoryAddParams ***************************

dbPersistent *dbCryosatFactory::buildDbInventoryAddParams(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryAddParams(dbConnectionWrapper(db::ConnPools::instance()->getPool(dbKey)));
}


dbGeneralQuery *dbCryosatFactory::buildDbInventoryAddParamsQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryAddParamsQuery(dbKey) ;
}



// *************** dbInventorySite ***************************

dbPersistent *dbCryosatFactory::buildDbInventorySite(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventorySite(dbConnectionWrapper(db::ConnPools::instance()->getPool(dbKey)));
}


dbGeneralQuery *dbCryosatFactory::buildDbInventorySiteQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventorySiteQuery(dbKey) ;
}



// *************** dbInventoryComment ***************************

dbPersistent *dbCryosatFactory::buildDbInventoryComment(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryComment;
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryCommentQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryCommentQuery(dbKey) ;
}

// *************** dbInventoryGap ***************************

dbPersistent *dbCryosatFactory::buildDbInventoryGap(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryGap;
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryGapQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryGapQuery(dbKey) ;
}

// *************** dbInventoryObject ***************************

dbPersistent *dbCryosatFactory::buildDbInventoryObject(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryObject;
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryObjectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryObjectQuery(dbKey) ;
}

// *************** dbInventoryObject ***************************
dbPersistent *dbCryosatFactory::buildDbInventoryObjectTEC(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryObjectTEC;
}


dbGeneralQuery *dbCryosatFactory::buildDbInventoryObjectTECQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryObjectTECQuery(dbKey) ;
}

// *************** dbInventoryObjectWithComments ***************************

dbPersistent *dbCryosatFactory::buildDbInventoryObjectWithGapsComments(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryObjectWithGapsComments;
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryObjectWithGapsCommentsQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryObjectWithGapsCommentsQuery(dbKey) ;
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryDelayedValCoverQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryDelayedValCoverQuery(false, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryValCoverQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryValCoverQuery(false, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryDelayedLatestValCoverQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryDelayedValCoverQuery(true, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryLatestValCoverQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryValCoverQuery(true, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryDelayedValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryDelayedValIntersectQuery(false, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryValIntersectQuery(false, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryValFullIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryValFullIntersectQuery(false, dbKey);
}


dbGeneralQuery *dbCryosatFactory::buildDbInventoryOrderTriggeringFilesQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryOrderTriggeringFilesQuery(dbKey) ;
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryObjectSpecialQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryObjectSpecialQuery(dbKey) ;
}


dbGeneralQuery *dbCryosatFactory::buildDbInventoryDelayedLatestValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryDelayedValIntersectQuery(true, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryLatestValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryValIntersectQuery(true, dbKey);
}


dbGeneralQuery *dbCryosatFactory::buildDbInventoryLatestValidityClosestQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryLatestValidityClosestQuery(dbKey) ;
}


dbGeneralQuery *dbCryosatFactory::buildDbInventoryBestCenteredQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryBestCenteredQuery(dbKey) ;
}

// buildDbInventoryNamedXXXXXX
dbGeneralQuery *dbCryosatFactory::buildDbInventoryNamedLatestValCoverQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryNamedValCoverOrIntersectQuery(true,false, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryNamedLatestValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryNamedValCoverOrIntersectQuery(true,true, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryNamedValCoverQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryNamedValCoverOrIntersectQuery(false,false, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryNamedValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryNamedValCoverOrIntersectQuery(false,true, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryGeoValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryGeoValIntersectQuery(dbKey) ;
}



// buildDbInventoryQualifiedXXXXXX
dbGeneralQuery *dbCryosatFactory::buildDbInventoryQualifiedLatestValCoverQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryQualifiedValCoverOrIntersectQuery(true,false, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryQualifiedLatestValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryQualifiedValCoverOrIntersectQuery(true,true, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryQualifiedValCoverQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryQualifiedValCoverOrIntersectQuery(false,false, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryQualifiedValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryQualifiedValCoverOrIntersectQuery(false,true, dbKey);
}




// buildDbInventoryOnSiteXXXXXX
dbGeneralQuery *dbCryosatFactory::buildDbInventoryOnSiteLatestValCoverQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryOnSiteValCoverOrIntersectQuery(true,false, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryOnSiteLatestValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryOnSiteValCoverOrIntersectQuery(true,true, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryOnSiteValCoverQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryOnSiteValCoverOrIntersectQuery(false,false, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryOnSiteValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryOnSiteValCoverOrIntersectQuery(false,true, dbKey);
}

// buildDbInventoryMultiBandOnSiteXXXXXX
dbGeneralQuery *dbCryosatFactory::buildDbInventoryMultiBandOnSiteLatestValCoverQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryMultiBandOnSiteValCoverOrIntersectQuery(true,false, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryMultiBandOnSiteLatestValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryMultiBandOnSiteValCoverOrIntersectQuery(true,true, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryMultiBandOnSiteValCoverQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryMultiBandOnSiteValCoverOrIntersectQuery(false,false, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryMultiBandOnSiteValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryMultiBandOnSiteValCoverOrIntersectQuery(false,true, dbKey);
}

// buildDbInventoryMultiBandXXXXXX
dbGeneralQuery *dbCryosatFactory::buildDbInventoryMultiBandLatestValCoverQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryMultiBandValCoverOrIntersectQuery(true,false, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryMultiBandLatestValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryMultiBandValCoverOrIntersectQuery(true,true, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryMultiBandValCoverQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryMultiBandValCoverOrIntersectQuery(false,false, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryMultiBandValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryMultiBandValCoverOrIntersectQuery(false,true, dbKey);
}

// buildDbInventoryTDIMultiBandOnSiteXXXXXX
dbGeneralQuery *dbCryosatFactory::buildDbInventoryTDIMultiBandOnSiteLatestValCoverQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery(true,false, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryTDIMultiBandOnSiteLatestValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery(true,true, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryTDIMultiBandOnSiteValCoverQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery(false,false, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryTDIMultiBandOnSiteValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery(false,true, dbKey);
}


// buildDbInventoryOnOrbitXXXXXX
dbGeneralQuery *dbCryosatFactory::buildDbInventoryOnOrbitLatestValCoverQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryOnOrbitValCoverOrIntersectQuery(true,false, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryOnOrbitQualifiedLatestValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery(true,true, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryOnOrbitQualifiedLatestValCoverQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery(true,false, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryOnOrbitLatestValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryOnOrbitValCoverOrIntersectQuery(true,true, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryOnOrbitValCoverQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryOnOrbitValCoverOrIntersectQuery(false,false, dbKey);
}

dbGeneralQuery *dbCryosatFactory::buildDbInventoryOnOrbitValIntersectQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInventoryOnOrbitValCoverOrIntersectQuery(false,true, dbKey);
}

// *************** dbMediaType ***************************

dbPersistent *dbCryosatFactory::buildDbMediaType(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbMediaType;
}

dbGeneralQuery *dbCryosatFactory::buildDbMediaTypeQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbMediaTypeQuery(dbKey) ;
}

// *************** dbOnDemandDistribution ***************************

dbPersistent *dbCryosatFactory::buildDbOnDemandDistribution(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbOnDemandDistribution;
}

dbGeneralQuery *dbCryosatFactory::buildDbOnDemandDistributionQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbOnDemandDistributionQuery(dbKey) ;
}

// *************** dbOrder ***************************

dbPersistent *dbCryosatFactory::buildDbOrder(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbOrder;
}

dbGeneralQuery *dbCryosatFactory::buildDbOrderQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbOrderQuery(dbKey) ;
}

// *************** dbSMactions ***************************

dbPersistent *dbCryosatFactory::buildDbSMactions(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbSMactions(dbConnectionWrapper(db::ConnPools::instance()->getPool(dbKey))) ;
}

dbGeneralQuery *dbCryosatFactory::buildDbSMactionsQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbSMactionsQuery(dbKey) ;
}


// *************** dbSMqueue ***************************

dbPersistent *dbCryosatFactory::buildDbSMqueue(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbSMqueue(dbConnectionWrapper(db::ConnPools::instance()->getPool(dbKey))) ;
}

dbGeneralQuery *dbCryosatFactory::buildDbSMqueueQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbSMqueueQuery(dbKey) ;
}


// *************** dbOrdersAttachment ***************************

dbPersistent *dbCryosatFactory::buildDbOrdersAttachment(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbOrdersAttachment(dbConnectionWrapper(db::ConnPools::instance()->getPool(dbKey)));
}


dbGeneralQuery *dbCryosatFactory::buildDbOrdersAttachmentQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbOrdersAttachmentQuery(dbKey) ;
}

// *************** dbWaitingOrder ***************************

dbPersistent *dbCryosatFactory::buildDbWaitingOrder(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbWaitingOrder;
}

dbGeneralQuery *dbCryosatFactory::buildDbWaitingOrderQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbWaitingOrderQuery(dbKey) ;
}

// *************** dbOrderGeneration ***************************

dbPersistent *dbCryosatFactory::buildDbOrderGeneration(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbOrderGeneration;
}

dbGeneralQuery *dbCryosatFactory::buildDbOrderGenerationQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbOrderGenerationQuery(dbKey) ;
}

// *************** dbAlgorithm ***************************

dbPersistent *dbCryosatFactory::buildDbAlgorithm(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbAlgorithm;
}

dbGeneralQuery *dbCryosatFactory::buildDbAlgorithmQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbAlgorithmQuery(dbKey) ;
}

// *************** dbOrderStateTransition ***************************

dbPersistent *dbCryosatFactory::buildDbOrderStateTransition(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbOrderStateTransition;
}

dbGeneralQuery *dbCryosatFactory::buildDbOrderStateTransitionQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbOrderStateTransitionQuery(dbKey) ;
}

// *************** dbOrderType ***************************

dbPersistent *dbCryosatFactory::buildDbOrderType(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbOrderType;
}

dbGeneralQuery *dbCryosatFactory::buildDbOrderTypeQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbOrderTypeQuery(dbKey) ;
}

// *************** dbProcessor ***************************

dbPersistent* dbCryosatFactory::buildDbProcessor(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbProcessor;
}


dbGeneralQuery* dbCryosatFactory::buildDbProcessorQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbProcessorQuery(dbKey) ;
}


// *************** dbUser ***************************

dbPersistent *dbCryosatFactory::buildDbUser(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbUser;
}

dbGeneralQuery *dbCryosatFactory::buildDbUserQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbUserQuery(dbKey) ;
}


// *************** dbUserType ***************************

dbPersistent* dbCryosatFactory::buildDbUserType(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbUserType;
}


dbGeneralQuery* dbCryosatFactory::buildDbUserTypeQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbUserTypeQuery(dbKey) ;
}


// *************** dbDistributionFormat ***************************

dbPersistent* dbCryosatFactory::buildDbDistributionFormat(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbDistributionFormat;
}


dbGeneralQuery* dbCryosatFactory::buildDbDistributionFormatQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbDistributionFormatQuery(dbKey) ;
}

// *************** dbMediaCapFormat ***************************

dbPersistent* dbCryosatFactory::buildDbMediaCapFormat(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbMediaCapFormat;
}


dbGeneralQuery* dbCryosatFactory::buildDbMediaCapFormatQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbMediaCapFormatQuery(dbKey) ;
}

// *************** dbInvRollingPolicies ***************************

dbPersistent* dbCryosatFactory::buildDbInvRollingPolicies(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInvRollingPolicies;
}


dbGeneralQuery* dbCryosatFactory::buildDbInvRollingPoliciesQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInvRollingPoliciesQuery(dbKey) ;
}

// *************** dbInvSpecialRollingPolicies ***************************

dbPersistent* dbCryosatFactory::buildDbInvSpecialRollingPolicies(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInvSpecialRollingPolicies;
}


dbGeneralQuery* dbCryosatFactory::buildDbInvSpecialRollingPoliciesQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbInvSpecialRollingPoliciesQuery(dbKey) ;
}


// *************** dbOrdersInput ***************************

dbPersistent* dbCryosatFactory::buildDbOrdersInput (const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbOrdersInput ;
}


dbGeneralQuery* dbCryosatFactory::buildDbOrdersInputQuery(const std::string &dbKey)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new dbOrdersInputQuery(dbKey) ;
}

_ACS_END_NAMESPACE
