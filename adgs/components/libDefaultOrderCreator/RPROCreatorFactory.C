// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. Default Order Creation Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/19 18:07:27  marpas
	adopting libException 5.x standards
	types fixed
	using OrderCreator::getNUllSatId and OrderCreator::getNullSatName()
	
	Revision 2.12  2013/06/19 14:54:15  marfav
	Restored null satellite id management (use zero to identify null value)
	
	Revision 2.11  2013/06/18 15:24:01  marfav
	Null satellite management fixed
	
	Revision 2.10  2013/04/09 08:39:55  marpas
	NameIdAssociator refactoring
	
	Revision 2.9  2013/03/07 19:47:57  marpas
	Using new NoMergeOrderCreator interface changed due to jira ADC-122
	
	Revision 2.8  2013/02/12 13:46:53  marpas
	dbConnectionPool concept introduced
	libDBOrderCreator new interfaces
	coding best practices applied
	debug improved
	qa rules
	FileGuard used when needed instead of File::unlink mess around
	
	Revision 2.7  2012/03/07 13:50:54  marpas
	preparing order parameters file
	
	Revision 2.6  2008/09/03 16:30:04  marpas
	CRYOSAT: RPRO External is like NRT - was lost during Aeolus coding
	
	Revision 2.5  2007/04/16 21:10:12  marpas
	work in progress
	
	Revision 2.4  2007/03/29 12:05:24  marpas
	work in progress
	
	Revision 2.3  2007/03/09 11:16:40  marpas
	AEOLUS (OPER|OFFL|RPRO) NOMERGE orders supported
	
	Revision 2.2  2006/06/23 10:01:00  ivafam
	StringUtils::compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 2.1  2006/06/20 16:46:37  ivafam
	Method compareNoCAse replaced with StringUtils::equalsNoCase
	
	Revision 2.0  2006/02/28 10:24:18  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/11/14 13:11:42  marpas
	ANX2ANX algo allowed for RPRO order in case of GOCE and CRYOSAT satellites
	
	Revision 1.1.1.1  2005/10/06 15:28:45  marfav
	Import libDefaultOrderCreator
	
	
*/




#include <RPROCreatorFactory.h>
#include <NoMergeOrderCreator.h>
#include <MergeOrderCreator.h>
#include <RPROExtOrderCreator.h>
#include <OfflineAnx2AnxOrderCreator.h>
#include <SatelliteName.h>
#include <StringUtils.h> 
#include <Filterables.h>



_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(RPROCreatorFactory)


RPROCreatorFactory::RPROCreatorFactory() :
    OrderCreatorFactory() 
{
}

RPROCreatorFactory::~RPROCreatorFactory() throw() {}

OrderCreator *RPROCreatorFactory::newCreator(const string &orderType, const string &algo, unsigned int satId, dbConnection &conn)  // PRQA S 4020
{
	SatelliteName* sat = SatelliteName::instance(conn.getKey());
        
	string satName = ( OrderCreator::getNullSatId() == satId ) ? OrderCreator::getNullSatName() : sat->name(satId) ; // PRQA S 3000, 3384, 3385

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,orderType << " " << algo << " " << satName)

	string rpro = "RPRO" ;
	string merge = "MERGE" ;
	string nomerge = "NOMERGE" ;
	string nomergesticky = "NOMERGESTICKY" ;
	string external = "EXTERNAL" ;
	string anx2anx = "ANX2ANX" ;

	if (!StringUtils::equalsNoCase(orderType,rpro)) {
		return 0 ;
    }

	if (StringUtils::equalsNoCase(satName,"CRYOSAT")) {
		if (StringUtils::equalsNoCase(algo, external)) {
			return new RPROExtOrderCreator(rpro, external, satId, conn) ;
        }	
		if (StringUtils::equalsNoCase(algo,merge)) {
			return new MergeOrderCreator(rpro, merge, satId, conn) ;
        }	
		if (StringUtils::equalsNoCase(algo,nomerge)) {
			return new NoMergeOrderCreator(rpro, nomerge, satId, conn, true) ;
        }	
        if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(rpro, nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
		if (StringUtils::equalsNoCase(algo,anx2anx)) {
			return new OfflineAnx2AnxOrderCreator(rpro, anx2anx, satId, conn) ;
        }	
		
		ACS_LOG_WARNING(algo << "not recognized for type " << rpro << " Creating (default) NoMergeOrderCreator") ;
		return new NoMergeOrderCreator(rpro, algo, satId, conn, true) ;
	}
	
	if (StringUtils::equalsNoCase(satName,"AEOLUS")) {
		if (StringUtils::equalsNoCase(algo, external)) {
			return new RPROExtOrderCreator(rpro, external, satId, conn) ;
        }	
		if (StringUtils::equalsNoCase(algo,nomerge)) {
			return new NoMergeOrderCreator(rpro, nomerge, satId, conn, true) ;
        }	
        if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(rpro, nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
	}
	
	if (StringUtils::equalsNoCase(satName,"GOCE")) {
		if (StringUtils::equalsNoCase(algo,merge)) {
			return new MergeOrderCreator(rpro, merge, satId, conn) ;
        }	
		if (StringUtils::equalsNoCase(algo,nomerge)) {
			return new NoMergeOrderCreator(rpro, nomerge, satId, conn, true) ;
        }	
        if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(rpro, nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
		if (StringUtils::equalsNoCase(algo,anx2anx)) {
			return new OfflineAnx2AnxOrderCreator(rpro, anx2anx, satId, conn) ;
        }	
		
		ACS_LOG_WARNING("Creating NoMergeOrderCreator factory for algo: " << algo) ;
		return new NoMergeOrderCreator(rpro, algo, satId, conn, true) ;
	}
	
	return 0 ;
}

void RPROCreatorFactory::capabilities(map <string, string> &cap) const
{
	cap["AEOLUS"] += " RPRO NOMERGE\n" ;
	cap["AEOLUS"] += " RPRO NOMERGESTICKY\n" ;
	cap["AEOLUS"] += " RPRO EXTERNAL\n" ;
	cap["CRYOSAT"] += " RPRO EXTERNAL\n" ;
	cap["CRYOSAT"] += " RPRO MERGE\n" ;
	cap["CRYOSAT"] += " RPRO NOMERGESTICKY\n" ;
	cap["CRYOSAT"] += " RPRO NOMERGE\n" ;
	cap["CRYOSAT"] += " RPRO ANX2ANX\n" ;
	cap["GOCE"] += " RPRO MERGE\n" ;
	cap["GOCE"] += " RPRO NOMERGE\n" ;
	cap["GOCE"] += " RPRO NOMERGESTICKY\n" ;
	cap["GOCE"] += " RPRO ANX2ANX\n" ;
	return ;
}

_ACS_END_NAMESPACE
