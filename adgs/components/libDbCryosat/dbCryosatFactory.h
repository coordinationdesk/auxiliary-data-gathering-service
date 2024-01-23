// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
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
	Revision 6.4  2016/10/07 11:25:04  marpas
	new Venus query (VEN-2163)
	
	Revision 6.3  2016/10/06 17:01:58  marpas
	implementing VEN-2163 on HEAD - ready to be reported on venus branch - test to be fixed
	
	Revision 6.2  2016/07/27 14:34:50  damdec
	dbBaseline and dbBaselineQuery handling removed
	
	Revision 6.1  2016/07/27 09:41:53  damdec
	OLD_PDS_SCHEMA flag removed.
	
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.1  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
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
	
	Revision 2.25  2013/07/22 18:34:20  marpas
	aligning to new libDbData interface
	
	Revision 2.24  2013/06/19 18:04:40  marpas
	adopting libException 5.x standards
	
	Revision 2.23  2012/11/28 13:13:50  marpas
	storage type is no longer needed with new schema - compile with PDS_OLD_SCHEMA to have it again
	
	Revision 2.22  2012/03/07 17:30:46  marpas
	new special query: dbInventoryOrderTriggeringFilesQuery added
	
	Revision 2.21  2012/02/14 12:52:45  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.20  2011/09/28 13:29:39  lucia.galli
	New special query added
	
	Revision 2.19  2011/04/29 13:35:00  chicas
	work in progress for GeoValIntersect special query
	
	Revision 2.18  2010/10/27 15:30:49  chicas
	added dbInventoryNamedValCoverOrIntersectQuery
	
	Revision 2.17  2010/03/12 17:26:31  marfav
	Added Delayed Inventory Objects Special Queries
	
	Revision 2.16  2009/05/13 15:12:13  marpas
	rules fixing and comments makeup
	
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
	
	Revision 2.2  2007/03/13 14:19:00  marpas
	work in progress
	
	Revision 2.1  2006/06/27 10:40:16  ivafam
	t_baseline added
	
	Revision 2.0  2006/02/28 09:09:34  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.20  2005/07/21 10:23:56  marpas
	dbConfigurationSpace and its query moved to libDbConfiguration
	
	Revision 1.19  2005/06/20 10:36:11  marpas
	gaps management added
	
	Revision 1.18  2005/06/17 16:37:49  marpas
	Inventory gaps concepts introduced
	
	Revision 1.17  2005/03/04 22:49:07  marpas
	dbWaitingOrder and dbWaitingOrderQuery classes  and their tests added , db_values script updated accordingly
	
	Revision 1.16  2004/12/30 22:47:59  paoscu
	headers fixed
	
	Revision 1.15  2004/09/24 12:21:14  ivafam
	Added dbOrdersInput class
	
	Revision 1.14  2004/09/06 12:53:04  ivafam
	Class added
	
	Revision 1.13  2004/07/19 17:50:59  paoscu
	dbEMailAddress and dbEMailAddressQuery removed
	dbUser has set and get EMAIL ADDRESS
	
	Revision 1.12  2004/04/26 15:38:29  paoscu
	dbMediaCapFormat class added.
	
	Revision 1.11  2004/04/16 16:38:23  paoscu
	dbDistributionFormat added.
	
	Revision 1.10  2003/07/08 18:32:22  paoscu
	DistributionView added.
	
	Revision 1.9  2003/07/08 17:40:00  paoscu
	dbAlgotithm and dbAlgotithmQuery added.
	
	Revision 1.8  2003/05/20 15:31:44  paoscu
	ConfigurationStyleSheets table added.
	
	Revision 1.7  2003/05/02 17:43:28  paoscu
	Class dbInventoryObjectWithComments added.
	
	Revision 1.6  2003/04/17 15:43:33  paoscu
	dbInventoryBestCenteredQuery added.
	
	Revision 1.5  2003/01/27 14:52:57  paoscu
	dbDistributedItem, dbDistributedItemQuery, dbProcessedOrder, dbProcessedOrderQuery eliminated
	
	Revision 1.4  2002/12/16 14:05:59  danalt
	added dbOrderStateTransition table
	
	Revision 1.3  2002/12/13 18:41:06  paoscu
	Special Queries
	
	Revision 1.2  2002/11/06 12:12:36  danalt
	aligned with libDbSpace
	
	Revision 1.1.1.1  2002/10/25 09:57:24  danalt
	Import libDbCryosat
		

*/

#ifndef _dbCryosatFactory_H_
#define _dbCryosatFactory_H_ 

#include <acs_c++config.hpp>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)
	
class dbPersistent;
class dbGeneralQuery;

class dbCryosatFactory
{
public:
	dbCryosatFactory();
	~dbCryosatFactory() throw() ;

	static dbPersistent* buildDbConfigurationSchema(const std::string &dbKey);
	static dbGeneralQuery* buildDbConfigurationSchemaQuery(const std::string &dbKey);

	static dbPersistent* buildDbConfigurationStyleSheet(const std::string &dbKey);
	static dbGeneralQuery* buildDbConfigurationStyleSheetQuery(const std::string &dbKey);

	static dbPersistent* buildDbDistribution(const std::string &dbKey);
	static dbGeneralQuery* buildDbDistributionQuery(const std::string &dbKey);

	static dbPersistent* buildDbDistributionItem(const std::string &dbKey);
	static dbGeneralQuery* buildDbDistributionItemQuery(const std::string &dbKey);

	static dbPersistent* buildDbDistributionView(const std::string &dbKey);
	static dbGeneralQuery* buildDbDistributionViewQuery(const std::string &dbKey);

	static dbPersistent* buildDbDistPolMDS(const std::string &dbKey);
	static dbGeneralQuery* buildDbDistPolMDSQuery(const std::string &dbKey);

	static dbPersistent* buildDbFileType(const std::string &dbKey);
	static dbGeneralQuery* buildDbFileTypeQuery(const std::string &dbKey);

	static dbPersistent* buildDbInventoryMDS(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryMDSQuery(const std::string &dbKey);

	static dbPersistent* buildDbInventoryAddParams(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryAddParamsQuery(const std::string &dbKey);

	static dbPersistent* buildDbInventorySite(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventorySiteQuery(const std::string &dbKey);

	static dbPersistent* buildDbInventoryComment(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryCommentQuery(const std::string &dbKey);

	static dbPersistent* buildDbInventoryGap(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryGapQuery(const std::string &dbKey);

	static dbPersistent* buildDbInventoryObjectTEC(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryObjectTECQuery(const std::string &dbKey);
	static dbPersistent* buildDbInventoryObject(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryObjectQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryValCoverQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryLatestValCoverQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryValFullIntersectQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryValIntersectQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryLatestValIntersectQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryLatestValidityClosestQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryBestCenteredQuery(const std::string &dbKey);

	static dbGeneralQuery* buildDbInventoryDelayedValCoverQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryDelayedLatestValCoverQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryDelayedValIntersectQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryDelayedLatestValIntersectQuery(const std::string &dbKey);

	static dbGeneralQuery* buildDbInventoryQualifiedLatestValCoverQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryQualifiedLatestValIntersectQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryQualifiedValCoverQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryQualifiedValIntersectQuery(const std::string &dbKey);

	static dbGeneralQuery* buildDbInventoryNamedLatestValCoverQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryNamedLatestValIntersectQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryNamedValCoverQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryNamedValIntersectQuery(const std::string &dbKey);

	static dbGeneralQuery* buildDbInventoryGeoValIntersectQuery(const std::string &dbKey);


	static dbGeneralQuery* buildDbInventoryOnSiteLatestValCoverQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryOnSiteLatestValIntersectQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryOnSiteValCoverQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryOnSiteValIntersectQuery(const std::string &dbKey);

	static dbGeneralQuery* buildDbInventoryMultiBandOnSiteLatestValCoverQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryMultiBandOnSiteLatestValIntersectQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryMultiBandOnSiteValCoverQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryMultiBandOnSiteValIntersectQuery(const std::string &dbKey);

	static dbGeneralQuery* buildDbInventoryMultiBandLatestValCoverQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryMultiBandLatestValIntersectQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryMultiBandValCoverQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryMultiBandValIntersectQuery(const std::string &dbKey);


	static dbGeneralQuery* buildDbInventoryTDIMultiBandOnSiteLatestValCoverQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryTDIMultiBandOnSiteLatestValIntersectQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryTDIMultiBandOnSiteValCoverQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryTDIMultiBandOnSiteValIntersectQuery(const std::string &dbKey);

	static dbGeneralQuery* buildDbInventoryOnOrbitQualifiedLatestValIntersectQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryOnOrbitQualifiedLatestValCoverQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryOnOrbitLatestValCoverQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryOnOrbitLatestValIntersectQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryOnOrbitValCoverQuery(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryOnOrbitValIntersectQuery(const std::string &dbKey);

    static dbGeneralQuery* buildDbInventoryOrderTriggeringFilesQuery(const std::string &dbKey) ;
    static dbGeneralQuery* buildDbInventoryObjectSpecialQuery(const std::string &dbKey) ;


	static dbPersistent* buildDbInventoryObjectWithGapsComments(const std::string &dbKey);
	static dbGeneralQuery* buildDbInventoryObjectWithGapsCommentsQuery(const std::string &dbKey);

	static dbPersistent* buildDbIORepository(const std::string &dbKey);
	static dbGeneralQuery* buildDbIORepositoryQuery(const std::string &dbKey);

	static dbPersistent* buildDbMediaType(const std::string &dbKey);
	static dbGeneralQuery* buildDbMediaTypeQuery(const std::string &dbKey);

	static dbPersistent* buildDbOnDemandDistribution(const std::string &dbKey);
	static dbGeneralQuery* buildDbOnDemandDistributionQuery(const std::string &dbKey);

	static dbPersistent* buildDbOrder(const std::string &dbKey);
	static dbGeneralQuery* buildDbOrderQuery(const std::string &dbKey);

	static dbPersistent* buildDbOrdersAttachment(const std::string &dbKey);
	static dbGeneralQuery* buildDbOrdersAttachmentQuery(const std::string &dbKey);

	static dbPersistent* buildDbWaitingOrder(const std::string &dbKey);
	static dbGeneralQuery* buildDbWaitingOrderQuery(const std::string &dbKey);

	static dbPersistent* buildDbOrderGeneration(const std::string &dbKey);
	static dbGeneralQuery* buildDbOrderGenerationQuery(const std::string &dbKey);

	static dbPersistent* buildDbAlgorithm(const std::string &dbKey);
	static dbGeneralQuery* buildDbAlgorithmQuery(const std::string &dbKey);

	static dbPersistent* buildDbOrderStateTransition(const std::string &dbKey);
	static dbGeneralQuery* buildDbOrderStateTransitionQuery(const std::string &dbKey);

	static dbPersistent* buildDbOrderType(const std::string &dbKey);
	static dbGeneralQuery* buildDbOrderTypeQuery(const std::string &dbKey);

	static dbPersistent* buildDbProcessedOrder(const std::string &dbKey);
	static dbGeneralQuery* buildDbProcessedOrderQuery(const std::string &dbKey);

	static dbPersistent* buildDbProcessor(const std::string &dbKey);
	static dbGeneralQuery* buildDbProcessorQuery(const std::string &dbKey);

	static dbPersistent* buildDbUser(const std::string &dbKey);
	static dbGeneralQuery* buildDbUserQuery(const std::string &dbKey);

	static dbPersistent* buildDbUserType(const std::string &dbKey);
	static dbGeneralQuery* buildDbUserTypeQuery(const std::string &dbKey);

	static dbPersistent* buildDbDistributionFormat(const std::string &dbKey);
	static dbGeneralQuery* buildDbDistributionFormatQuery(const std::string &dbKey);

	static dbPersistent* buildDbMediaCapFormat(const std::string &dbKey);
	static dbGeneralQuery* buildDbMediaCapFormatQuery(const std::string &dbKey);

	static dbPersistent* buildDbInvRollingPolicies(const std::string &dbKey);
	static dbGeneralQuery* buildDbInvRollingPoliciesQuery(const std::string &dbKey);

	static dbPersistent* buildDbInvSpecialRollingPolicies(const std::string &dbKey);
	static dbGeneralQuery* buildDbInvSpecialRollingPoliciesQuery(const std::string &dbKey);

	static dbPersistent* buildDbOrdersInput(const std::string &dbKey);
	static dbGeneralQuery* buildDbOrdersInputQuery(const std::string &dbKey);

	static dbPersistent* buildDbSMqueue(const std::string &dbKey);
	static dbGeneralQuery* buildDbSMqueueQuery(const std::string &dbKey);

	static dbPersistent* buildDbSMactions(const std::string &dbKey);
	static dbGeneralQuery* buildDbSMactionsQuery(const std::string &dbKey);

private:
	// declared but not implemented to prevent their use
	dbCryosatFactory(const dbCryosatFactory &);
	dbCryosatFactory& operator=(const dbCryosatFactory &);

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbCryosatFactory) ;

};


_ACS_END_NAMESPACE

#endif // _dbCryosatFactory_H_

