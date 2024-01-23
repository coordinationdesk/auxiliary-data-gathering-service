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

	$Prod:$

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
	
	Revision 1.5  2012/12/21 13:30:33  marpas
	if compiled withoud PDS_OLD_SCHEMA use the rule policy id to fill the distribution items.
	coding best practices
	qa rules
	
	Revision 1.4  2009/06/25 09:19:08  crivig
	Algo string converted to upper case-> no conversion for db queries
	
	Revision 1.3  2009/06/23 11:41:49  marpas
	lgos set to lowercase
	
	Revision 1.2  2009/06/22 16:12:50  marpas
	algo strings in uppercase
	
	Revision 1.1  2009/06/05 08:52:42  crivig
	added new distribution algo
			
*/




#include <PartialCvrgDistributorFactory.h>
#include <CvrgDistributorCreator.h>
#include <StringUtils.h> 
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(PartialCvrgDistributorFactory);
PartialCvrgDistributorFactory::PartialCvrgDistributorFactory() :
    DistributorCreatorFactory() 
{
}

PartialCvrgDistributorFactory::~PartialCvrgDistributorFactory() throw()
{
}

DistributorCreator *PartialCvrgDistributorFactory::newCreator(const string &algo)  // PRQA S 4020 
{

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "algo:\"" << algo);
	string PartialCvrg = "PARTIALCVRG" ;

	if (!StringUtils::equalsNoCase(algo,PartialCvrg)) {
		return 0 ;
    }
	return new CvrgDistributorCreator(PartialCvrg, false );
}

void PartialCvrgDistributorFactory::capabilities(vector <string> &cap) const
{
	string algo = " PARTIALCVRG\n" ;
	cap.push_back(algo);
	return ;
}


_ACS_END_NAMESPACE
