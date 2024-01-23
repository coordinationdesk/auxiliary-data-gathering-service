// PRQA S 1050 EOF
/*

	Copyright 1995-2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:  $

	$Id$

	$Author$

	$Log$
	Revision 2.2  2015/05/15 16:31:05  nicvac
	APF-264
	
	Revision 2.1  2014/02/05 15:39:14  francesco.avanzi
	ACS_CLASS_DEFINE_DEBUG_LEVEL reintroduced after been removed by mistake
	
	Revision 2.0  2013/01/21 18:02:22  marpas
	introducing dbConnectionPool use
	coding best practices
	filterable management improved
	qa rules
	
	Revision 1.2  2012/12/21 13:30:33  marpas
	if compiled withoud PDS_OLD_SCHEMA use the rule policy id to fill the distribution items.
	coding best practices
	qa rules
	
	Revision 1.1  2012/02/29 15:20:41  chicas
	Added
	
	
	
*/

#include <DefaultDistributorFactories.h>

using namespace acs ;
ACS_CLASS_DEFINE_DEBUG_LEVEL(DefaultDistributorFactories);
PreselectionDistributorFactory DefaultDistributorFactories::thePreselectionDistributorFactory ;
AlwaysDistributorFactory DefaultDistributorFactories::theAlwaysDistributorFactory ;
QualifiedDistributorFactory DefaultDistributorFactories::theQualifiedDistributorFactory ;
QualifyReportDistributorFactory DefaultDistributorFactories::theQualifyReportDistributorFactory ;
QCCReportDistributorFactory DefaultDistributorFactories::theQCCReportDistributorFactory ;
RedistributionDistributorFactory DefaultDistributorFactories::theRedistributionDistributorFactory ;

TotalCvrgDistributorFactory DefaultDistributorFactories::theTotalCvrgDistributorFactory ;
PartialCvrgDistributorFactory DefaultDistributorFactories::thePartialCvrgDistributorFactory ;

QualifiedValiditystopDistributorFactory DefaultDistributorFactories::theQualifiedValiditystopDistributorFactory;

QualifiedFileclassOriginatorDistributorFactory DefaultDistributorFactories::theQualifiedFileclassOriginatorDistributorFactory;
