#
#	Copyright 1995-2018, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. Makefile for libDbCryosat $
#
#	$Id$
#
#	$Author$
#d
#	$Log$
#	Revision 7.0  2017/05/31 17:32:57  nicvac
#	OPMAN-260: Need PDS DB schema PDS_12-04.
#	
#	Revision 6.7  2017/05/29 17:53:37  nicvac
#	OPMAN-260: PackageExporter concurrent Agents new algo (t_distributionruleslock to state change method)
#	
#	Revision 6.6  2016/10/07 12:56:54  marpas
#	base special queries are used by downloaders
#	
#	Revision 6.5  2016/10/07 11:31:44  marpas
#	moving all NameIdAssociator in libDbClient along all the others
#	
#	Revision 6.4  2016/10/06 17:01:58  marpas
#	implementing VEN-2163 on HEAD - ready to be reported on venus branch - test to be fixed
#	
#	Revision 6.3  2016/07/27 14:34:06  damdec
#	dbBaseline and dbBaselineQuery removed.
#	
#	Revision 6.2  2016/07/27 09:40:49  damdec
#	dbStorageLocation and dbStorageLocationQuery removed.
#	
#	Revision 6.1  2016/07/26 10:30:43  damdec
#	dbLockReceptionPolicy removed.
#	
#	Revision 6.0  2016/07/07 13:44:03  damdec
#	CSGACQ-82: special queries modified to be compatible with Oracle
#	
#	Revision 5.6  2016/07/04 14:07:30  damdec
#	-lProc removed.
#	
#	Revision 5.5  2016/04/18 15:22:27  marpas
#	some standard NameIdAssociators have lost their .C
#	
#	Revision 5.4  2016/04/15 10:13:02  marpas
#	rewritten as standard NameIdAssociator with dbKey
#	
#	Revision 5.3  2016/03/29 16:27:33  marpas
#	changed FileTypeDetails to adhere to new db i/f and calling protocols.
#	TL uses it during precondition check
#	
#	Revision 5.2  2016/03/26 21:42:50  marpas
#	using new db I/F
#	
#	Revision 5.1  2015/08/07 09:46:45  tergem
#	ALSATFixedHeaderFactory class added
#	
#	Revision 5.0  2015/02/17 11:25:24  marfav
#	Field unique_id added to table t_smactions
#	
#	Revision 4.1  2013/10/22 17:37:25  marpas
#	DistPolOriginator & DistPolOriginatorQuery concepts removed
#	
#	Revision 4.0  2013/09/30 12:30:51  marpas
#	the following classes have been removed:
#	dbDistributionPolicy
#	dbDistributionPolicyQuery
#	dbIORepository
#	dbIORepositoryQuery
#	dbReceptionPolicy
#	dbReceptionPolicyQuery
#	
#	Revision 3.0  2013/09/19 14:57:19  marfav
#	dbSMactions now include inventory id pointers
#	
#	Revision 2.43  2013/06/11 13:33:57  marpas
#	adding S3PDGSFixedHeaderFactory
#	
#	Revision 2.42  2013/03/29 19:51:28  marpas
#	removing html related functions
#	
#	Revision 2.41  2013/03/21 09:54:30  chicas
#	Added S2PDGSFixedHeaderFactory.h
#	
#	Revision 2.40  2013/03/20 16:37:05  chicas
#	added S2PDGSFixedHeaderFactory
#	
#	Revision 2.39  2013/03/08 17:01:39  marpas
#	libDbBasic no longer used
#	
#	Revision 2.38  2012/12/17 11:58:50  marpas
#	PROJECTFLAG added by commondefs
#	
#	Revision 2.37  2012/11/19 22:51:15  marpas
#	compile with PDS_OLD_SCHEMA to use old pds schema.
#	dbReceptionRule and related Query class introduced
#	
#	Revision 2.36  2012/11/16 14:10:44  lucio.pulvirenti
#	ADCFixedHeaderFactory.h added to list of include files to install
#	
#	Revision 2.35  2012/11/16 13:01:12  marpas
#	realignement of MAIN STREAM
#	Introducing ADCS1FixedHeaderFactory class
#	
#	Revision 2.34  2012/03/09 10:53:23  marpas
#	S1PDGSFixedHeaderFactory created and tested
#	all test compilation fixed
#	
#	Revision 2.33  2012/03/07 17:30:46  marpas
#	new special query: dbInventoryOrderTriggeringFilesQuery added
#	
#	Revision 2.32  2011/09/28 13:29:39  lucia.galli
#	New special query added
#	
#	Revision 2.31  2011/04/29 13:35:00  chicas
#	work in progress for GeoValIntersect special query
#	
#	Revision 2.30  2010/10/27 15:30:49  chicas
#	added dbInventoryNamedValCoverOrIntersectQuery
#	
#	Revision 2.29  2010/03/12 17:26:31  marfav
#	Added Delayed Inventory Objects Special Queries
#	
#	Revision 2.28  2009/05/22 13:57:27  marpas
#	QualifiedFlag duplicates the one in libDbClient
#	
#	Revision 2.27  2009/04/28 15:21:39  marpas
#	OCADA ifdef: QUALIFIED_PRODUCT or'ed with VALID_PRODUCT
#	
#	Revision 2.26  2009/04/03 17:08:38  marpas
#	dbFileType::checkPeriodicity implemented
#	
#	Revision 2.25  2009/03/17 17:11:25  marfav
#	Makefile fixed
#	
#	Revision 2.24  2009/03/06 16:48:04  marfav
#	Added FileTypeDetails class
#	
#	Revision 2.23  2008/12/16 15:59:33  marpas
#	 work in progress
#	
#	Revision 2.22  2008/12/03 18:09:03  paoscu
#	dbInventoryMultiBandOnSiteValCoverOrIntersectQuery Added
#	
#	Revision 2.21  2008/12/02 16:57:29  paoscu
#	dbInventoryOnOrbitValCoverOrIntersectQuery added
#	
#	Revision 2.20  2008/12/01 20:07:05  paoscu
#	Added dbInventoryOnSiteValCoverOrIntersectQuery
#	
#	Revision 2.19  2008/11/05 18:16:18  marpas
#	dbInventoryQualified(Latest)Val(Cover/intersect)Query implemented
#	
#	Revision 2.18  2008/11/04 17:03:02  paoscu
#	Added class to dbInventoryValCoverOrIntersectQuery to be inherited by other special queries
#	
#	Revision 2.17  2008/10/29 11:57:49  marpas
#	dbOrderValidator added
#	
#	Revision 2.16  2008/10/20 13:40:26  marpas
#	dbInventorySite classes added
#	
#	Revision 2.15  2008/10/14 15:01:58  marpas
#	dbOrdersAttachmentQuery class added
#	
#	Revision 2.14  2008/10/13 13:58:10  marpas
#	dbOrdersAttachment class added (work in progress) still missing BLOB management
#	
#	Revision 2.13  2008/03/21 18:09:16  paoscu
#	dbInventoryValFullIntersectQuery added
#	
#	Revision 2.12  2007/06/28 14:59:56  serfol
#	adding the EnvisatFixedHeaderFactory first time
#	
#	Revision 2.11  2007/04/18 22:12:55  marpas
#	Management of dbDistributionpolicies relations with respect to data sets and originators
#	
#	Revision 2.10  2007/03/28 15:02:44  marpas
#	AeolusFixedHeaderFactory created and added
#	
#	Revision 2.9  2007/03/19 13:55:47  marpas
#	work in progress
#	
#	Revision 2.8  2007/03/16 14:00:19  marpas
#	work in progress
#	
#	Revision 2.7  2007/03/16 12:28:59  marpas
#	work in progress
#	
#	Revision 2.6  2007/03/14 17:45:39  marpas
#	work in progress
#	
#	Revision 2.5  2007/03/12 18:08:10  marpas
#	work in progress
#	
#	Revision 2.4  2006/12/02 11:24:19  clanas
#	inserted in dynemic linc lib feature
#	
#	Revision 2.3  2006/08/23 14:51:58  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.2  2006/06/27 10:40:16  ivafam
#	t_baseline added
#	
#	Revision 2.1  2006/04/04 15:03:41  paoscu
#	GoceFixedHeaderFactory added.
#	EarthExplorerFixedHeaderWriter replaces CryosatFixedHeaderWriter
#	
#	Revision 2.0  2006/02/28 09:09:34  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.52  2005/10/24 16:44:01  marpas
#	dbexpress compatibility
#	
#	Revision 1.51  2005/07/26 10:22:06  ivafam
#	dbInventoryObjectOperatorIF moved in libCryosatFileAction
#	
#	Revision 1.50  2005/07/22 09:39:46  ivafam
#	Added
#	
#	Revision 1.49  2005/07/21 10:23:55  marpas
#	dbConfigurationSpace and its query moved to libDbConfiguration
#	
#	Revision 1.48  2005/06/20 16:17:37  enrcar
#	INCLUDEPATH Removed
#	
#	Revision 1.47  2005/06/20 10:36:11  marpas
#	gaps management added
#	
#	Revision 1.46  2005/06/17 16:37:49  marpas
#	Inventory gaps concepts introduced
#	
#	Revision 1.45  2005/06/15 07:44:49  integrator
#	multiple storage manager implemented
#	
#	Revision 1.44  2005/05/31 14:22:38  integrator
#	File actions interface changed
#	
#	Revision 1.43  2005/03/04 22:49:07  marpas
#	dbWaitingOrder and dbWaitingOrderQuery classes  and their tests added , db_values script updated accordingly
#	
#	Revision 1.42  2005/01/03 14:14:26  ivafam
#	The constructor of classes deriving from dbFileAction is protected.
#	dbInventoryFileActionFactory becomes dbCryosatFileActionFactory
#	dbCryosatFactory declares dbCryosatFileActionFactory
#	FileAction basic classes moved in libDbData
#	
#	Revision 1.41  2004/11/09 12:57:35  giucas
#	EC: :Makefile: dbLockDistributionPolicy.h was missing. Added.
#	
#	Revision 1.40  2004/11/08 13:52:50  paoscu
#	new class added
#	
#	Revision 1.39  2004/10/15 08:36:09  ivafam
#	Added dbFileActionFactoryChain dbFileActionFactory dbInventoryFileActionFactory
#	and bug fixed in dbInvRollingPolicies::fieldName
#	
#	Revision 1.38  2004/10/07 09:40:56  ivafam
#	dbFileAction added
#	
#	Revision 1.37  2004/09/24 12:21:14  ivafam
#	Added dbOrdersInput class
#	
#	Revision 1.36  2004/09/22 13:20:07  ivafam
#	Bug Fixed in do query: Now the SQLCLAUSE "ORDER BY" is added if _orderByString
#	is not empty.
#	
#	Revision 1.35  2004/09/06 12:53:04  ivafam
#	Class added
#	
#	Revision 1.34  2004/09/03 15:29:49  ivafam
#	Added new classes
#	
#	Revision 1.33  2004/09/03 13:33:03  paoscu
#	dbOrder::hasParametersFile removed
#	dbOrder::getParameters changed into dbOrder::hasParameters
#	bug fixed in updating internal dbOrder::_parameters member
#	dbOrder::update can save parameters file too
#	
#	Revision 1.32  2004/08/26 16:37:18  envisat
#	header list fixed
#	
#	Revision 1.31  2004/07/19 17:50:59  paoscu
#	dbEMailAddress and dbEMailAddressQuery removed
#	dbUser has set and get EMAIL ADDRESS
#	
#	Revision 1.30  2004/07/13 15:15:44  paoscu
#	using namespace std no longer in includes
#	exDbPersistent is an abstract class
#	
#	Revision 1.29  2004/05/18 10:36:53  marpas
#	INFORMIX multithread libs adopted
#	
#	Revision 1.28  2004/05/03 14:53:59  paoscu
#	New haeader writers.
#	
#	Revision 1.27  2004/04/19 15:56:40  paoscu
#	dbMediaCapFormat added.
#	
#	Revision 1.26  2004/04/16 16:33:19  paoscu
#	dbDistributionFormat added.
#	
#	Revision 1.25  2004/04/16 15:18:20  paoscu
#	ACEPATH resurrected due to the HSM
#	
#	Revision 1.24  2004/04/16 13:10:47  paoscu
#	Ace removed
#	
#	Revision 1.23  2003/10/07 16:16:28  paoscu
#	dbXMLTransformer class added.
#	
#	Revision 1.22  2003/09/03 10:50:31  paoscu
#	dbFileTypeCheck added.
#	
#	Revision 1.21  2003/07/08 18:32:22  paoscu
#	DistributionView added.
#	
#	Revision 1.20  2003/07/07 16:15:26  paoscu
#	dbAlogorithm and dbAlogorithmQuery added.
#	
#	Revision 1.19  2003/05/26 20:06:25  paoscu
#	minor changes
#	
#	Revision 1.18  2003/05/20 13:18:44  paoscu
#	Table T_ConfigurationStyleSheets added in database.
#	
#	Revision 1.17  2003/05/02 17:43:28  paoscu
#	Class dbInventoryObjectWithComments added.
#	
#	Revision 1.16  2003/04/17 15:44:10  paoscu
#	dbInventoryBestCenteredQuery added.
#	
#	Revision 1.15  2003/03/25 14:37:50  paoscu
#	Added class dbFixedHeaderWriter
#	
#	Revision 1.14  2003/02/18 11:30:46  paoscu
#	...
#	
#	Revision 1.13  2003/02/03 14:12:16  paoscu
#	LIB_INTEGRATION_PATH
#	
#	Revision 1.12  2003/01/29 15:49:38  danalt
#	added libGraphicAttributes
#	
#	Revision 1.11  2003/01/27 14:51:06  paoscu
#	bDistributedItem, dbDistributedItemQuery, dbProcessedOrder and dbProcessedOrderQuery  eliminated.
#	
#	Revision 1.10  2003/01/10 20:17:13  paoscu
#	dbLastGeneratedOverlapStrategy class added
#	
#	Revision 1.9  2002/12/17 17:43:49  paoscu
#	dbInventoryLatestValidityClosestQuery added.
#	
#	Revision 1.8  2002/12/16 14:06:14  danalt
#	added dbOrderStateTransition table
#	
#	Revision 1.7  2002/12/13 18:41:06  paoscu
#	Special Queries
#	
#	Revision 1.6  2002/11/27 12:15:10  paoscu
#	tdbUploadInventoryFile
#	
#	Revision 1.5  2002/11/15 12:08:41  paoscu
#	dbUploadFile added.
#	
#	Revision 1.4  2002/11/12 14:37:01  paoscu
#	ACE and HSM lib and include added.
#	
#	Revision 1.3  2002/11/06 12:12:44  danalt
#	aligned with libDbSpace
#	
#	Revision 1.2  2002/10/29 11:17:20  paoscu
#	Added dbDownloadInventoryFile class
#	
#	Revision 1.1.1.1  2002/10/25 09:57:24  danalt
#	Import libDbCryosat
#		
#

include ../../common/commondefs.mk

#dbexpress compatibility mode (for Informix w/ SQLString)
CUSTOMFLAGS:=-DDBEXPRESS_COMPAT $(CUSTOMFLAGS)


#Uncomment this to get also shared object library
#TARGETFILES = $(AR_LIBRARY) $(DSO_LIBRARY)


  LIBRARYNAME =	libDbCryosat

  VERSIONFILE = dbcsV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		dbcsV.C \
		InvRelationsRules.C \
		dbFileType.C \
		dbFileTypeQuery.C \
		dbMediaType.C \
		dbMediaTypeQuery.C \
		dbOnDemandDistribution.C \
		dbOnDemandDistributionQuery.C \
		dbOrder.C \
		dbOrderValidator.C \
		dbOrderQuery.C \
		dbOrdersAttachment.C \
		dbOrdersAttachmentQuery.C \
		dbOrderGeneration.C \
		dbOrderGenerationQuery.C \
		dbAlgorithm.C \
		dbAlgorithmQuery.C \
		dbOrderStateTransition.C \
		dbOrderStateTransitionQuery.C \
		dbOrderType.C \
		dbOrderTypeQuery.C \
		dbReceptionRule.C \
		dbReceptionRuleQuery.C \
		dbDistribution.C \
		dbDistributionQuery.C \
		dbDistributionItem.C \
		dbDistributionItemQuery.C \
		dbDistPolMDS.C \
		dbDistPolMDSQuery.C \
		dbInventoryGap.C \
		dbInventoryGapQuery.C \
		dbInventoryComment.C \
		dbInventoryMDS.C \
		dbInventoryMDSQuery.C \
		dbInventoryAddParams.C \
		dbInventoryAddParamsQuery.C \
		dbInventorySite.C \
		dbInventorySiteQuery.C \
		dbInventoryCommentQuery.C \
		dbInventoryObject.C \
		dbInventoryObjectQuery.C \
		dbInventoryObjectTEC.C \
		dbInventoryObjectTECQuery.C \
		dbInventoryObjectWithGaps.C \
		dbInventoryObjectWithGapsQuery.C \
		dbInventoryObjectWithGapsComments.C \
		dbInventoryObjectWithGapsCommentsQuery.C \
		dbConfigurationSchema.C \
		dbConfigurationSchemaQuery.C \
		dbConfigurationStyleSheet.C \
		dbConfigurationStyleSheetQuery.C \
		dbUserType.C \
		dbUserTypeQuery.C \
		dbUser.C \
		dbUserQuery.C \
		dbProcessor.C \
		dbProcessorQuery.C \
		dbDistributionView.C \
		dbDistributionViewQuery.C \
		dbCryosatFactory.C \
		dbFileTypeRecognition.C \
		dbFileTypeCheck.C \
		dbInventoryObjectSpecialQuery.C \
		dbInventoryQualifiedValCoverOrIntersectQuery.C \
		dbInventoryNamedValCoverOrIntersectQuery.C \
		dbInventoryGeoValIntersectQuery.C \
		dbInventoryOnSiteValCoverOrIntersectQuery.C \
		dbInventoryMultiBandOnSiteValCoverOrIntersectQuery.C \
		dbInventoryMultiBandValCoverOrIntersectQuery.C \
		dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery.C \
		dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery.C \
		dbInventoryOnOrbitValCoverOrIntersectQuery.C \
		dbInventoryValCoverQuery.C \
		dbInventoryOrderTriggeringFilesQuery.C \
		dbInventoryValCoverOrIntersectQuery.C \
		dbInventoryValIntersectQuery.C \
		dbInventoryDelayedValCoverQuery.C \
		dbInventoryDelayedValCoverOrIntersectQuery.C \
		dbInventoryDelayedValIntersectQuery.C \
		dbInventoryValFullIntersectQuery.C \
		dbInventoryLatestValidityClosestQuery.C \
		dbInventoryBestCenteredQuery.C \
		dbLastGeneratedOverlapStrategy.C \
		dbFixedHeaderWriter.C \
		EarthExplorerFixedHeaderWriter.C \
		dbFixedHeaderFactoryChain.C \
		CryosatFixedHeaderFactory.C \
		VenusFixedHeaderFactory.C \
		OcadaFixedHeaderFactory.C \
		AeolusFixedHeaderFactory.C \
		GoceFixedHeaderFactory.C \
		EnvisatFixedHeaderFactory.C \
		S1PDGSFixedHeaderFactory.C \
		S2PDGSFixedHeaderFactory.C \
		S3PDGSFixedHeaderFactory.C \
		ALSATFixedHeaderFactory.C \
		ADCFixedHeaderFactory.C \
		NoneFixedHeaderFactory.C \
		dbXMLTransformer.C \
		dbDistributionFormat.C \
		dbDistributionFormatQuery.C \
		dbMediaCapFormat.C \
		dbMediaCapFormatQuery.C \
		dbInvRollingPolicies.C \
		dbInvRollingPoliciesQuery.C \
		dbInvSpecialRollingPolicies.C \
		dbInvSpecialRollingPoliciesQuery.C \
		dbOrdersInput.C \
		dbOrdersInputQuery.C \
		dbWaitingOrder.C \
		dbWaitingOrderQuery.C \
		dbSMactions.C \
		dbSMactionsQuery.C \
		dbSMqueue.C \
		dbSMqueueQuery.C \
		FileTypeDetails.C 
		
		

   C++INCLUDE = \
		InvRelationsRules.h \
		dbDistribution.h \
		dbDistributionQuery.h \
		dbDistributionItem.h \
		dbDistributionItemQuery.h \
		dbDistPolMDS.h \
		dbDistPolMDSQuery.h \
		dbFileType.h \
		dbFileTypeQuery.h \
		dbInventoryGap.h \
		dbInventoryGapQuery.h \
		dbInventoryMDS.h \
		dbInventoryMDSQuery.h \
		dbInventoryAddParams.h \
		dbInventoryAddParamsQuery.h \
		dbInventorySite.h \
		dbInventorySiteQuery.h \
		dbInventoryComment.h \
		dbInventoryCommentQuery.h \
		dbInventoryObject.h \
		dbInventoryObjectQuery.h \
		dbInventoryObjectTEC.h \
		dbInventoryObjectTECQuery.h \
		dbInventoryObjectWithGaps.h \
		dbInventoryObjectWithGapsQuery.h \
		dbInventoryObjectWithGapsComments.h \
		dbInventoryObjectWithGapsCommentsQuery.h \
		dbMediaType.h \
		dbMediaTypeQuery.h \
		dbOnDemandDistribution.h \
		dbOnDemandDistributionQuery.h \
		dbOrder.h \
		dbOrderValidator.h \
		dbOrderQuery.h \
		dbOrdersAttachment.h \
		dbOrdersAttachmentQuery.h \
		dbOrderGeneration.h \
		dbOrderGenerationQuery.h \
		dbAlgorithm.h \
		dbAlgorithmQuery.h \
		dbOrderStateTransition.h \
		dbOrderStateTransitionQuery.h \
		dbOrderType.h \
		dbOrderTypeQuery.h \
		dbReceptionRule.h \
		dbReceptionRuleQuery.h \
		dbConfigurationSchema.h \
		dbConfigurationSchemaQuery.h \
		dbConfigurationStyleSheet.h \
		dbConfigurationStyleSheetQuery.h \
		dbUserType.h \
		dbUserTypeQuery.h \
		dbUser.h \
		dbUserQuery.h \
		dbProcessor.h \
		dbProcessorQuery.h \
		dbDistributionView.h \
		dbDistributionViewQuery.h \
		dbCryosatFactory.h \
		dbFileTypeRecognition.h \
		dbFileTypeCheck.h \
		dbInventoryObjectSpecialQuery.h \
		dbInventoryValCoverQuery.h \
		dbInventoryValIntersectQuery.h \
		dbInventoryValCoverOrIntersectQuery.h \
		dbLastGeneratedOverlapStrategy.h \
		dbFixedHeaderWriter.h \
		EarthExplorerFixedHeaderWriter.h \
		dbFixedHeaderFactoryChain.h \
		CryosatFixedHeaderFactory.h \
		OcadaFixedHeaderFactory.h \
		VenusFixedHeaderFactory.h \
		AeolusFixedHeaderFactory.h \
		GoceFixedHeaderFactory.h \
		EnvisatFixedHeaderFactory.h \
		NoneFixedHeaderFactory.h \
		S1PDGSFixedHeaderFactory.h \
		S2PDGSFixedHeaderFactory.h \
		S3PDGSFixedHeaderFactory.h \
		ALSATFixedHeaderFactory.h \
		ADCFixedHeaderFactory.h \
		dbXMLTransformer.h \
		dbDistributionFormat.h \
		dbDistributionFormatQuery.h \
		dbMediaCapFormat.h \
		dbMediaCapFormatQuery.h \
		dbInvRollingPolicies.h \
		dbInvRollingPoliciesQuery.h \
		dbInvSpecialRollingPolicies.h \
		dbInvSpecialRollingPoliciesQuery.h \
		dbOrdersInput.h \
		dbOrdersInputQuery.h \
		dbWaitingOrder.h \
		dbWaitingOrderQuery.h \
		dbSMactions.h \
		dbSMactionsQuery.h \
		dbSMqueue.h \
		dbSMqueueQuery.h \
		FileTypeDetails.h  
		

DSOSPECIFICDEPS =

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 


include ../../common/commonrules.mk

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	@rm -f *.bak;


#empty rule, do not remove, you can add specific remotion commands
helpspecific:



# DO NOT DELETE


