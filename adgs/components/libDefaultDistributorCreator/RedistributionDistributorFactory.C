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
	
	Revision 2.0  2013/01/21 18:02:23  marpas
	introducing dbConnectionPool use
	coding best practices
	filterable management improved
	qa rules
	
	Revision 1.5  2012/12/21 13:30:34  marpas
	if compiled withoud PDS_OLD_SCHEMA use the rule policy id to fill the distribution items.
	coding best practices
	qa rules
	
	Revision 1.4  2009/06/25 09:19:09  crivig
	Algo string converted to upper case-> no conversion for db queries
	
	Revision 1.3  2009/06/23 11:41:50  marpas
	lgos set to lowercase
	
	Revision 1.2  2009/06/22 16:17:02  marpas
	algorithms names in uppercase
	
	Revision 1.1  2007/04/02 14:46:39  crivig
	*** empty log message ***
	
*/


#include <RedistributionDistributorFactory.h>
#include <StringUtils.h>
#include <RedistributionDistributorCreator.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(RedistributionDistributorFactory) ;

RedistributionDistributorFactory::RedistributionDistributorFactory() :
    DistributorCreatorFactory()
{
}

RedistributionDistributorFactory::~RedistributionDistributorFactory() throw() 
{
}

DistributorCreator *RedistributionDistributorFactory::newCreator(const string &algo)  // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "algo:\"" << algo << '\"') ;
	string Redistribution = "REDISTRIBUTION" ;

	if (!StringUtils::equalsNoCase(algo,Redistribution)) {
		return 0 ;
    }
	return new RedistributionDistributorCreator(Redistribution) ;
}

void RedistributionDistributorFactory::capabilities(vector <string> &cap) const
{
	string algo = ' ' + string("REDISTRIBUTION") + '\n' ; // PRQA S 3081
	cap.push_back(algo);
	return ;
}

_ACS_END_NAMESPACE
