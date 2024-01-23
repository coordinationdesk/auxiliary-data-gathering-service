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

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2013/02/07 17:52:27  marpas
	Factories and Creators interfaces changed to allow easier connection pool handling and stop predicates
	
	Revision 2.0  2013/01/21 18:02:22  marpas
	introducing dbConnectionPool use
	coding best practices
	filterable management improved
	qa rules
	
	Revision 1.2  2012/12/21 13:30:33  marpas
	if compiled withoud PDS_OLD_SCHEMA use the rule policy id to fill the distribution items.
	coding best practices
	qa rules
	
	Revision 1.1  2012/02/29 15:21:12  chicas
	Added; to be tested
	
	
*/

#include <QCCReportDistributorFactory.h>
#include <StringUtils.h>
#include <QCCReportDistributorCreator.h>
#include <Filterables.h>

	_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(QCCReportDistributorFactory) ;

QCCReportDistributorFactory::QCCReportDistributorFactory() :
    DistributorCreatorFactory()
{
}

QCCReportDistributorFactory::~QCCReportDistributorFactory() throw() 
{
}

DistributorCreator *QCCReportDistributorFactory::newCreator(const string &algo) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"algo:\"" << algo );
	string QCCDistr = "QCC_DISTR_CONDITIONED" ;

	if (!StringUtils::equalsNoCase(algo,QCCDistr)) {
		return 0 ;
    }
	return new QCCReportDistributorCreator(QCCDistr) ;
}
void QCCReportDistributorFactory::capabilities(vector <string> &cap) const
{
	string algo = " QCC_DISTR_CONDITIONED\n" ;
	cap.push_back(algo);
	return ;
}

_ACS_END_NAMESPACE
