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

	$Prod: A.C.S. Default Order Creation Library $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2013/02/07 17:52:26  marpas
	Factories and Creators interfaces changed to allow easier connection pool handling and stop predicates
	
	Revision 2.0  2013/01/21 18:02:22  marpas
	introducing dbConnectionPool use
	coding best practices
	filterable management improved
	qa rules
	
	Revision 1.7  2012/12/21 13:30:33  marpas
	if compiled withoud PDS_OLD_SCHEMA use the rule policy id to fill the distribution items.
	coding best practices
	qa rules
	
	Revision 1.6  2009/06/25 09:19:08  crivig
	Algo string converted to upper case-> no conversion for db queries
	
	Revision 1.5  2009/06/23 11:41:49  marpas
	lgos set to lowercase
	
	Revision 1.4  2009/06/22 16:17:02  marpas
	algorithms names in uppercase
	
	Revision 1.3  2007/03/16 16:27:15  crivig
	coding + testing
	
	Revision 1.2  2007/03/15 15:08:03  crivig
	coding and compiling
	
	Revision 1.1.1.1  2007/03/14 17:15:00  crivig
	Imported new source
	
	Revision 2.2  2006/06/23 10:01:00  ivafam
	StringUtils::compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 2.1  2006/06/20 16:46:37  ivafam
	Method compareNoCAse replaced with StringUtils::equalsNoCase
	
	Revision 2.0  2006/02/28 10:24:18  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/11/14 13:11:42  marpas
	ANX2ANX algo allowed for Always order in case of GOCE and CRYOSAT satellites
	
	Revision 1.1.1.1  2005/10/06 15:28:45  marfav
	Import libDefaultDistributorCreator
	
	
*/




#include <AlwaysDistributorFactory.h>
#include <AlwaysDistributorCreator.h>
#include <StringUtils.h> 
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
ACS_CLASS_DEFINE_DEBUG_LEVEL(AlwaysDistributorFactory);

AlwaysDistributorFactory::AlwaysDistributorFactory() :
    DistributorCreatorFactory() 
{
}

AlwaysDistributorFactory::~AlwaysDistributorFactory() throw()
{
}

DistributorCreator *AlwaysDistributorFactory::newCreator(const string &algo) // PRQA S 4020
{

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"algo:\"" << algo );
	string Always = "ALWAYS" ;

	if (!StringUtils::equalsNoCase(algo,Always)) {
		return 0 ;
    }
	return new AlwaysDistributorCreator(Always) ;
}

void AlwaysDistributorFactory::capabilities(vector <string> &cap) const
{
	string algo = " ALWAYS\n" ;
	cap.push_back(algo);
	return ;
}


_ACS_END_NAMESPACE
