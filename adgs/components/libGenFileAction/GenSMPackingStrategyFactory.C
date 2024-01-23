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

	$Prod: A.C.S. Generic File Action Library $

	$Id$

	$Author$

	$Log$
	Revision 6.1  2013/10/10 17:42:46  marpas
	dbUploadInventoryFile can be configured to upload blobs possibly conditioned by size
	cond space gives an example of configuration
	PDS_OLD_SCHEMA no longer supported.
	
	Revision 6.0  2013/09/19 15:40:44  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.0  2013/07/23 17:07:21  marpas
	adopting libException 5.x interface
	coding best practices applied
	compilation warnings fixed
	qa warning fixed
	
	Revision 1.6  2013/03/28 14:30:52  marpas
	coding best practices applied
	ConnectionPool and connections usage enforced
	dbQueries statistics updated
	qa rules
	
	Revision 1.5  2013/01/22 18:14:55  marpas
	qa rules work in progress
	
	Revision 1.4  2012/02/13 16:49:03  marpas
	refactoring
	
	Revision 1.3  2009/02/19 11:40:35  marpas
	bGenFileAction conf space is not mandatory ... for the moment becuse contains only the minimum cost that should be defaulted to ... ?
	
	Revision 1.2  2009/01/29 10:57:43  marpas
	configuration issues
	
	Revision 1.1.1.1  2008/10/09 17:15:51  ivafam
	First Import
	
	
	
*/


#include <GenSMPackingStrategyFactory.h>
#include <PlainStrategy.h>
#include <TgzStrategy.h>
#include <GzStrategy.h>
#include <dbInventoryObject.h>
#include <StringUtils.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

GenSMPackingStrategyFactory GenSMPackingStrategyFactory::_theFactory ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(GenSMPackingStrategyFactory) ;

GenSMPackingStrategyFactory::GenSMPackingStrategyFactory() : SMPackingStrategyFactory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}

GenSMPackingStrategyFactory::~GenSMPackingStrategyFactory() throw() 
{
    // PRQA S 4631 L1
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
    // PRQA L:L1
}

SMPackingStrategy * GenSMPackingStrategyFactory::getStrategy(const std::string &strategy, dbPersistent & invobj, dbConnectionPool &pool) // PRQA S 4020
{
	const dbInventoryObject * o = dynamic_cast<const dbInventoryObject*>(&invobj) ; // PRQA S 3081
	if(o != 0 )
	{
		if(StringUtils::equalsNoCase(strategy,"Plain"))
		{
			return new PlainStrategy(invobj, pool) ;
		}
		else if(StringUtils::equalsNoCase(strategy,"Tgz"))
		{
			return new TgzStrategy(invobj, pool) ;
		}
		else if(StringUtils::equalsNoCase(strategy,"GZ"))
		{
			return new GzStrategy(invobj, pool) ;
		}
        else {}
	}
	
	return 0 ;
}


string GenSMPackingStrategyFactory::capabilities() const
{
	string ret ;
	ret += string("Nothing on InventoryObject") + "\n" ; // PRQA S 3081
	ret += string("Tgz <---> UncompressUniqueFile on InventoryObject") + "\n" ; // PRQA S 3081
	return ret ;
}



_ACS_END_NAMESPACE
