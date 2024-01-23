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
	Revision 2.2  2013/10/22 16:33:35  marpas
	t_distpoloriginator table moved in t_distruleoriginator
	coding best practices applied
	
	Revision 2.1  2013/02/07 17:52:27  marpas
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
	
	Revision 1.5  2009/06/23 11:41:50  marpas
	lgos set to lowercase
	
	Revision 1.4  2009/06/22 16:17:02  marpas
	algorithms names in uppercase
	
	Revision 1.3  2007/03/16 16:27:15  crivig
	coding + testing
	
	Revision 1.2  2007/03/15 15:08:03  crivig
	coding and compiling
	
	Revision 1.1.1.1  2007/03/14 17:15:00  crivig
	Imported new source
	
*/


#include <PreselectionDistributorFactory.h>
#include <PreselectionDistributorCreator.h>
#include <StringUtils.h>
#include <Filterables.h>

#include <ddrcV.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
	ACS_CLASS_DEFINE_DEBUG_LEVEL(PreselectionDistributorFactory);
namespace {
    ddrcV version;
}

PreselectionDistributorFactory::PreselectionDistributorFactory() : DistributorCreatorFactory()
{
}

PreselectionDistributorFactory::~PreselectionDistributorFactory() throw() 
{
}

DistributorCreator *PreselectionDistributorFactory::newCreator(const string &algo)  // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "algo:\"" << algo );
	string Preselection = "PRESELECTION" ;

	if (!StringUtils::equalsNoCase(algo,Preselection)) {
		return 0 ;
    }
	return new PreselectionDistributorCreator(Preselection) ;
	
}
void PreselectionDistributorFactory::capabilities(vector <string> &cap) const
{
	string algo = " " + string("PRESELECTION") + "\n" ; // PRQA S 3081
	cap.push_back(algo);
	return ;
}

_ACS_END_NAMESPACE
