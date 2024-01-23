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
	Revision 5.6  2017/10/20 08:31:51  davide.tiriticco
	Added REG-X algorithm
	
	Revision 5.5  2017/06/30 15:50:14  dancan
	PRS-22: Added Prisma Support
	
	Revision 5.4  2016/05/13 12:25:44  nicvac
	Support for SNAV added.
	
	Revision 5.3  2015/07/27 14:47:41  lucgal
	added ALSAT-1B
	
	Revision 5.2  2014/03/21 17:32:48  lucgal
	JERS added
	
	Revision 5.1  2013/09/05 10:17:09  marfav
	Adding support to Sentinel-3 satellite
	
	Revision 5.0  2013/06/19 18:07:27  marpas
	adopting libException 5.x standards
	types fixed
	using OrderCreator::getNUllSatId and OrderCreator::getNullSatName()
	
	Revision 2.31  2013/06/19 14:54:15  marfav
	Restored null satellite id management (use zero to identify null value)
	
	Revision 2.30  2013/04/09 08:39:55  marpas
	NameIdAssociator refactoring
	
	Revision 2.29  2013/03/07 19:48:47  marpas
	DUP management as in jira ADC-122
	NOMERGENODUP algo used only for offline (no sat) and offline (sentinels)
	
	Revision 2.28  2013/02/12 13:46:53  marpas
	dbConnectionPool concept introduced
	libDBOrderCreator new interfaces
	coding best practices applied
	debug improved
	qa rules
	FileGuard used when needed instead of File::unlink mess around
	
	Revision 2.27  2012/10/23 09:32:00  marpas
	algo fixed
	
	Revision 2.26  2012/09/18 09:53:27  marpas
	NoMergeUsrDistribOrderCreator class added to the repository
	
	Revision 2.25  2012/09/18 09:24:16  marpas
	NoMergeUsrDistribOrderCreator class added and NOMRG_USRDISTRIB algo implemented
	
	Revision 2.24  2012/05/09 15:46:02  giucas
	LANDSAT and SPOT added.
	
	Revision 2.23  2012/03/07 13:50:54  marpas
	preparing order parameters file
	
	Revision 2.22  2012/01/17 17:40:36  masdal
	sentinel3 added
	
	Revision 2.21  2011/09/14 12:28:00  marpas
	capabilities list fixed
	
	Revision 2.20  2011/09/14 09:10:20  masdal
	added SENTINEL-1A SENTINEL-1B
	
	Revision 2.19  2011/07/12 09:14:57  lucia.galli
	added Sentinel2 satellites
	
	Revision 2.18  2011/01/17 09:05:47  masdal
	SACD offline ordercreator added
	
	Revision 2.17  2010/06/24 10:20:13  marpas
	METO OFFL NOMERGE Added
	
	Revision 2.16  2010/06/23 14:21:53  marpas
	supporting nullsat OFFL NOMERGE in progress
	
	Revision 2.15  2010/05/05 09:25:17  masdal
	rename Oceansat2 to Oceansat
	
	Revision 2.14  2009/09/25 12:13:17  danalt
	added OCEANSAT2
	
	Revision 2.13  2009/01/21 11:56:37  masdal
	RADARSAT offline ordercreator added
	
	Revision 2.12  2008/12/16 10:15:03  masdal
	ALOS offline ordercreator added
	
	Revision 2.11  2008/12/01 10:35:32  danalt
	EOSAM capability only NOMERGE
	
	Revision 2.10  2008/11/27 14:02:17  marpas
	venus implementation ongoing
	other changes for convenience method(s)
	
	Revision 2.9  2008/11/25 11:18:11  marpas
	adding VENUS
	
	Revision 2.8  2008/11/25 09:30:24  masdal
	added EOSAM and EOSPM offline ordercreator
	
	Revision 2.7  2008/09/02 15:39:56  serfol
	adding the ERS, NRT and OFFL
	
	Revision 2.6  2007/03/12 15:15:54  marpas
	Aeolus OPER managed
	
	Revision 2.5  2007/03/09 11:16:40  marpas
	AEOLUS (OPER|OFFL|RPRO) NOMERGE orders supported
	
	Revision 2.4  2006/07/06 11:04:52  marpas
	LTA_ ordertype added to GOCE capabilities
	
	Revision 2.3  2006/06/23 10:01:00  ivafam
	StringUtils::compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 2.2  2006/06/20 16:46:37  ivafam
	Method compareNoCAse replaced with StringUtils::equalsNoCase
	
	Revision 2.1  2006/05/29 08:09:29  marpas
	take into account CONS & OFFL ordertype for GOCE satellite
	
	Revision 2.0  2006/02/28 10:24:18  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/12/01 16:39:08  marpas
	ENVISAT OFFL MERGE & NOMERGE orders added
	
	Revision 1.1.1.1  2005/10/06 15:28:45  marfav
	Import libDefaultOrderCreator
	
	
*/


#include <OfflineCreatorFactory.h>
#include <OfflineAnx2AnxOrderCreator.h>
#include <NoMergeRegXOrderCreator.h>
#include <NoMergeUsrDistribOrderCreator.h>
#include <NoMergeOrderCreator.h>
#include <MergeOrderCreator.h>
#include <GroupedCollectionOrderCreator.h>
#include <SatelliteName.h>
#include <StringUtils.h>
#include <Filterables.h>


#include <dorcV.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

namespace { // unnamed
    dorcV version;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(OfflineCreatorFactory) ;


OfflineCreatorFactory::OfflineCreatorFactory() :
    OrderCreatorFactory() 
{
}

OfflineCreatorFactory::~OfflineCreatorFactory() throw() {}

OrderCreator *OfflineCreatorFactory::newCreator(const string &orderType, const string &algo, unsigned int satId, dbConnection &conn) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"orderType: \"" << orderType << "\"  algo:\"" << algo) ;
	const string nrt_ = "NRT" ; // for S1 & S3 until there must be a difference
	const string offline = "OFFL" ;
	const string oper = "OPER" ;
	const string consol = "CONS" ;
	const string lta_ = "LTA_" ;
	const string level1 = "LEVEL1" ;
	const string merge = "MERGE" ;
	const string nomerge = "NOMERGE" ;
	const string nomergesticky = "NOMERGESTICKY" ;
	const string nomergenodup = "NOMERGENODUP" ;
	const string level2 = "LEVEL2" ;
	const string l2anx2anx = "L2ANX2ANX" ;
	const string l1anx2anx = "L1ANX2ANX" ;
	const string anx2anx = "ANX2ANX" ;
	const string groupcoll = "GROUPEDCOLLECTION" ;
	const string no_merge_usr_distrib = "NOMRG_USRDISTRIB" ;
	const string no_merge_regx = "REG-X-";
	if (!StringUtils::equalsNoCase(orderType,offline) && 
		!StringUtils::equalsNoCase(orderType,oper) && 
		!StringUtils::equalsNoCase(orderType,consol) && 
		!StringUtils::equalsNoCase(orderType,lta_) &&
		!StringUtils::equalsNoCase(orderType,nrt_)
       ) {
		return 0 ;
    }
	
	
	SatelliteName* sat = SatelliteName::instance(conn.getKey());
	
	const string satName = ( OrderCreator::getNullSatId() == satId ) ? OrderCreator::getNullSatName() : sat->name(satId) ; // PRQA S 3000, 3384, 3385

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"satName: \"" << satName << "\"") ;
	if (StringUtils::equalsNoCase(satName,OrderCreator::getNullSatName())) {
		if (StringUtils::equalsNoCase(orderType,offline)) {
			if (StringUtils::equalsNoCase(algo,nomerge)) {
				return new NoMergeOrderCreator(offline, nomerge, satId, conn, true) ;
            }
            if (StringUtils::equalsNoCase(algo,nomergenodup)) {
				return new NoMergeOrderCreator(offline, nomergenodup, satId, conn, false) ;
            }
            if (StringUtils::equalsNoCase(algo,nomergesticky)) {
				NoMergeOrderCreator *oc = new NoMergeOrderCreator(offline, nomergesticky, satId, conn, true) ;
                oc->setSticky(true) ;
                return oc ;
            }
            
 		}
	}


	if ((StringUtils::equalsNoCase(satName,"Sentinel-2") ||
		 StringUtils::equalsNoCase(satName,"Sentinel-2A") ||
		 StringUtils::equalsNoCase(satName,"Sentinel-2B"))
		&& StringUtils::equalsNoCase(orderType,offline)) {

		if (StringUtils::equalsNoCase(algo,nomerge)) {
			return new NoMergeOrderCreator(offline, nomerge, satId, conn, true) ;
        }
		if (StringUtils::equalsNoCase(algo,nomergenodup)) {
			return new NoMergeOrderCreator(offline, nomergenodup, satId, conn, false) ;
		}
	    if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(offline, nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
   }

	if (StringUtils::equalsNoCase(satName,"CRYOSAT") && StringUtils::equalsNoCase(orderType,offline)) {

		if (StringUtils::equalsNoCase(algo,level1)) {
			return new MergeOrderCreator(offline, level1, satId, conn) ;
        }
		if (StringUtils::equalsNoCase(algo,merge)) {
			return new MergeOrderCreator(offline, merge, satId, conn) ;
        }
		// at the moment no special algorithm is foreseen
		if (StringUtils::equalsNoCase(algo,level2)) {
			return new MergeOrderCreator(offline, level2, satId, conn) ;
        }
		if (StringUtils::equalsNoCase(algo,l2anx2anx)) {
			return new OfflineAnx2AnxOrderCreator(offline, l2anx2anx, satId, conn) ;
        }
		if (StringUtils::equalsNoCase(algo,anx2anx)) {
			return new OfflineAnx2AnxOrderCreator(offline, anx2anx, satId, conn) ;
        }
		if (StringUtils::equalsNoCase(algo,nomerge)) {
			return new NoMergeOrderCreator(offline, nomerge, satId, conn, true) ;
        }
        if (StringUtils::equalsNoCase(algo, no_merge_usr_distrib)) {
            return new NoMergeUsrDistribOrderCreator(offline, no_merge_usr_distrib, satId, conn) ;
        }
	    if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(offline, nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
	}
	
	if (StringUtils::equalsNoCase(satName,"PRISMA")) {
		
		if (!StringUtils::equalsNoCase(orderType,offline) && !StringUtils::equalsNoCase(orderType,oper)) {
			return 0 ;
        }
		if (StringUtils::equalsNoCase(algo,nomerge)) {
			return new NoMergeOrderCreator(StringUtils::uppercase(orderType), nomerge, satId, conn, true) ;
        }
	    if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(StringUtils::uppercase(orderType), nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
	}

    if (StringUtils::equalsNoCase(satName,"PLATINO1-1") || StringUtils::equalsNoCase(satName,"PLATINO2-1") ) {
		if (!StringUtils::equalsNoCase(orderType,offline) && !StringUtils::equalsNoCase(orderType,oper)) {
			return 0 ;
        }
		if (StringUtils::equalsNoCase(algo,nomerge)) {
			return new NoMergeOrderCreator(StringUtils::uppercase(orderType), nomerge, satId, conn, true) ;
        }
	    if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(StringUtils::uppercase(orderType), nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
    }

	if (StringUtils::equalsNoCase(satName,"AEOLUS")) {
		
		if (!StringUtils::equalsNoCase(orderType,offline) && !StringUtils::equalsNoCase(orderType,oper)) {
			return 0 ;
        }
			
		if (StringUtils::equalsNoCase(algo,nomerge)) {
			return new NoMergeOrderCreator(StringUtils::uppercase(orderType), nomerge, satId, conn, true) ;
        }
	    if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(StringUtils::uppercase(orderType), nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
	}
	
	if (StringUtils::equalsNoCase(satName,"GOCE") && !StringUtils::equalsNoCase(orderType,oper)) {
		if (StringUtils::equalsNoCase(algo,l1anx2anx)) {
			return new OfflineAnx2AnxOrderCreator(StringUtils::uppercase(orderType), l1anx2anx, satId, conn) ;
        }
		if (StringUtils::equalsNoCase(algo,anx2anx)) {
			return new OfflineAnx2AnxOrderCreator(StringUtils::uppercase(orderType), anx2anx, satId, conn) ;
        }
		if (StringUtils::equalsNoCase(algo,merge)) {
			return new MergeOrderCreator(StringUtils::uppercase(orderType), merge, satId, conn) ;
        }
		if (StringUtils::equalsNoCase(algo,nomerge)) {
			return new NoMergeOrderCreator(StringUtils::uppercase(orderType), nomerge, satId, conn, true) ;
        }
	    if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(StringUtils::uppercase(orderType), nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
		if (StringUtils::equalsNoCase(algo,groupcoll)) {
			return new GroupedCollectionOrderCreator(StringUtils::uppercase(orderType), groupcoll, satId, conn) ;
        }
	}

	if (StringUtils::equalsNoCase(satName,"ENVISAT") && StringUtils::equalsNoCase(orderType,offline)) {

		if (StringUtils::equalsNoCase(algo,merge)) {
			return new MergeOrderCreator(offline, merge, satId, conn) ;
        }
		if (StringUtils::equalsNoCase(algo,nomerge)) {
			return new NoMergeOrderCreator(offline, nomerge, satId, conn, true) ;
        }
	    if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(offline, nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
	}
	
	if (StringUtils::equalsNoCase(satName,"ERS") && StringUtils::equalsNoCase(orderType,offline)) {

		if (StringUtils::equalsNoCase(algo,merge)) {
			return new MergeOrderCreator(offline, merge, satId, conn) ;
        }
		if (StringUtils::equalsNoCase(algo,nomerge)) {
			return new NoMergeOrderCreator(offline, nomerge, satId, conn, true) ;
        }
	    if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(offline, nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
	}

	if (StringUtils::equalsNoCase(satName,"EOSAM") && StringUtils::equalsNoCase(orderType,offline)) {

		if (StringUtils::equalsNoCase(algo,merge)) {
			return new MergeOrderCreator(offline, merge, satId, conn) ;
        }
		if (StringUtils::equalsNoCase(algo,nomerge)) {
			return new NoMergeOrderCreator(offline, nomerge, satId, conn, true) ;
        }
	    if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(offline, nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
	}

	if (StringUtils::equalsNoCase(satName,"EOSPM") && StringUtils::equalsNoCase(orderType,offline)) {

		if (StringUtils::equalsNoCase(algo,merge)) {
			return new MergeOrderCreator(offline, merge, satId, conn) ;
        }
		if (StringUtils::equalsNoCase(algo,nomerge)) {
			return new NoMergeOrderCreator(offline, nomerge, satId, conn, true) ;
        }
	    if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(offline, nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
	}

    if (StringUtils::equalsNoCase(satName,"ALOS") && StringUtils::equalsNoCase(orderType,offline)) {

        if (StringUtils::equalsNoCase(algo,merge)) {
			return new MergeOrderCreator(offline, merge, satId, conn) ;
        }
        if (StringUtils::equalsNoCase(algo,nomerge)) {
			return new NoMergeOrderCreator(offline, nomerge, satId, conn, true) ;
        }
	    if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(offline, nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
    }

    if (StringUtils::equalsNoCase(satName,"RADARSAT") && StringUtils::equalsNoCase(orderType,offline)) {

        if (StringUtils::equalsNoCase(algo,merge)) {
            return new MergeOrderCreator(offline, merge, satId, conn) ;
        }
        if (StringUtils::equalsNoCase(algo,nomerge)) {
            return new NoMergeOrderCreator(offline, nomerge, satId, conn, true) ;
        }
	    if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(offline, nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
    }

    if (StringUtils::equalsNoCase(satName,"OCEANSAT") && StringUtils::equalsNoCase(orderType,offline)) {

        if (StringUtils::equalsNoCase(algo,merge)) {
            return new MergeOrderCreator(offline, merge, satId, conn) ;
        }
        if (StringUtils::equalsNoCase(algo,nomerge)) {
            return new NoMergeOrderCreator(offline, nomerge, satId, conn, true) ;
        }
	    if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(offline, nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
    }

    if (StringUtils::equalsNoCase(satName,"SACD") && StringUtils::equalsNoCase(orderType,offline)) {

        if (StringUtils::equalsNoCase(algo,merge)) {
            return new MergeOrderCreator(offline, merge, satId, conn) ;
        }
        if (StringUtils::equalsNoCase(algo,nomerge)) {
            return new NoMergeOrderCreator(offline, nomerge, satId, conn, true) ;
        }
	    if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(offline, nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
    }

    if (StringUtils::equalsNoCase(satName,"METOP") && StringUtils::equalsNoCase(orderType,offline)) {

		if (StringUtils::equalsNoCase(algo,nomerge)) {
            return new NoMergeOrderCreator(offline, nomerge, satId, conn, true) ;
        }
	    if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(offline, nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
    }

    if (StringUtils::equalsNoCase(satName,"SENTINEL-1") || 
	StringUtils::equalsNoCase(satName,"SENTINEL-1A") || 
        StringUtils::equalsNoCase(satName,"SENTINEL-1B") || 
        StringUtils::equalsNoCase(satName,"SENTINEL-3A") ||
        StringUtils::equalsNoCase(satName,"SENTINEL-3B") ||
        StringUtils::equalsNoCase(satName,"SENTINEL-3C") ||
        StringUtils::equalsNoCase(satName,"SENTINEL-3D") ||
        StringUtils::equalsNoCase(satName,"SENTINEL-3")  ||
        StringUtils::equalsNoCase(satName,"SENTINEL-6A") ||
        StringUtils::equalsNoCase(satName,"SENTINEL-6B") ||
        StringUtils::equalsNoCase(satName,"SENTINEL-6")
        ) {

        if (StringUtils::equalsNoCase(orderType,offline)) {
            if (StringUtils::equalsNoCase(algo,merge)) {
                return new MergeOrderCreator(offline, merge, satId, conn) ;
            }
            if (StringUtils::equalsNoCase(algo,nomerge)) {
                return new NoMergeOrderCreator(offline, nomerge, satId, conn, true) ;
            }
            if (StringUtils::equalsNoCase(algo,nomergenodup)) {
                return new NoMergeOrderCreator(offline, nomergenodup, satId, conn, false) ;
            }
            if (algo.size() > no_merge_regx.size() && StringUtils::equalsNoCase(StringUtils::substring(algo, 0, no_merge_regx.length()), no_merge_regx)) {
				return new NoMergeRegXOrderCreator(offline, StringUtils::substring(algo, 0, no_merge_regx.length()), StringUtils::substringAfter(algo, no_merge_regx), satId, conn);
            }
	        if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			    NoMergeOrderCreator *oc = new NoMergeOrderCreator(offline, nomergesticky, satId, conn, true) ;
                oc->setSticky(true) ;
                return oc ;
            }
        }
        if (StringUtils::equalsNoCase(orderType,nrt_)) {
            if (StringUtils::equalsNoCase(algo,merge)) {
                return new MergeOrderCreator(nrt_, merge, satId, conn) ;
            }
            if (StringUtils::equalsNoCase(algo,nomerge)) {
                return new NoMergeOrderCreator(nrt_, nomerge, satId, conn, true) ;
            }
            if (StringUtils::equalsNoCase(algo,nomergenodup)) {
                return new NoMergeOrderCreator(nrt_, nomergenodup, satId, conn, false) ;
            }
            if (algo.size() > no_merge_regx.size() && StringUtils::equalsNoCase(StringUtils::substring(algo, 0, no_merge_regx.length()), no_merge_regx)) {
				return new NoMergeRegXOrderCreator(nrt_, StringUtils::substring(algo, 0, no_merge_regx.length()), StringUtils::substringAfter(algo, no_merge_regx), satId, conn);
            }
	        if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			    NoMergeOrderCreator *oc = new NoMergeOrderCreator(nrt_, nomergesticky, satId, conn, true) ;
                oc->setSticky(true) ;
                return oc ;
            }
        }
    }
    
    if (StringUtils::equalsNoCase(orderType,offline)) {
        if (StringUtils::equalsNoCase(satName,"ALSAT-1B") ) {

            if (StringUtils::equalsNoCase(algo,merge)) {
                return new MergeOrderCreator(offline, merge, satId, conn) ;
            }
            if (StringUtils::equalsNoCase(algo,nomerge)) {
                return new NoMergeOrderCreator(offline, nomerge, satId, conn, true) ;
            }
            if (StringUtils::equalsNoCase(algo,nomergenodup)) {
                return new NoMergeOrderCreator(offline, nomergenodup, satId, conn, false) ;
            }
	        if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			    NoMergeOrderCreator *oc = new NoMergeOrderCreator(offline, nomergesticky, satId, conn, true) ;
                oc->setSticky(true) ;
                return oc ;
            }
        }
    }
    
    
	if (StringUtils::equalsNoCase(satName,"LANDSAT")) {
		
		if (StringUtils::equalsNoCase(algo,nomerge)) {
			return new NoMergeOrderCreator(StringUtils::uppercase(orderType), algo, satId, conn, true) ;
        }
	    if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(StringUtils::uppercase(orderType), nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
	}
	if (StringUtils::equalsNoCase(satName,"SPOT")) {
		
		if (StringUtils::equalsNoCase(algo,nomerge)) {
			return new NoMergeOrderCreator(StringUtils::uppercase(orderType), algo, satId, conn, true) ;
        }
	    if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(StringUtils::uppercase(orderType), nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
	}
	
	if (StringUtils::equalsNoCase(satName,"JERS")) {
		
		if (StringUtils::equalsNoCase(algo,nomerge)) {
			return new NoMergeOrderCreator(StringUtils::uppercase(orderType), algo, satId, conn, true) ;
        }
	    if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			NoMergeOrderCreator *oc = new NoMergeOrderCreator(StringUtils::uppercase(orderType), nomergesticky, satId, conn, true) ;
            oc->setSticky(true) ;
            return oc ;
        }
	}

    if (StringUtils::equalsNoCase(orderType, offline)) {
        if (StringUtils::equalsNoCase(satName,"SNAV") ) {

            if (StringUtils::equalsNoCase(algo,merge)) {
                return new MergeOrderCreator(offline, merge, satId, conn) ;
            }
            if (StringUtils::equalsNoCase(algo,nomerge)) {
                return new NoMergeOrderCreator(offline, nomerge, satId, conn, true) ;
            }
            if (StringUtils::equalsNoCase(algo,nomergenodup)) {
                return new NoMergeOrderCreator(offline, nomergenodup, satId, conn, false) ;
            }
	        if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			    NoMergeOrderCreator *oc = new NoMergeOrderCreator(offline, nomergesticky, satId, conn, true) ;
                oc->setSticky(true) ;
                return oc ;
            }
        }
    }

	if (StringUtils::equalsNoCase(orderType,offline)) 
	{
		if (StringUtils::equalsNoCase(satName,"MUSTER") || StringUtils::equalsNoCase(satName,"MUSTER2") ) 
		{
			if (StringUtils::equalsNoCase(algo,nomerge)) 
			{
				return new NoMergeOrderCreator(offline, nomerge, satId, conn, true) ;
			}
			if (StringUtils::equalsNoCase(algo,nomergenodup)) 
			{
				return new NoMergeOrderCreator(offline, nomergenodup, satId, conn, false) ;
			}
			if (algo.size() > no_merge_regx.size() && StringUtils::equalsNoCase(StringUtils::substring(algo, 0, no_merge_regx.length()), no_merge_regx)) 
			{
				return new NoMergeRegXOrderCreator(offline, StringUtils::substring(algo, 0, no_merge_regx.length()), StringUtils::substringAfter(algo, no_merge_regx), satId, conn);
			}
	        if (StringUtils::equalsNoCase(algo,nomergesticky)) {
			    NoMergeOrderCreator *oc = new NoMergeOrderCreator(offline, nomergesticky, satId, conn, true) ;
                oc->setSticky(true) ;
                return oc ;
            }
		}
	}

	return 0 ;
	
}


void OfflineCreatorFactory::capabilities(map <string, string> &cap) const
{
	cap["<null sat>"]   += " OFFL NOMERGE\n" ;
	cap["<null sat>"]   += " OFFL NOMERGENODUP\n" ;
	cap["<null sat>"]   += " OFFL NOMERGESTICKY\n" ;

	cap["AEOLUS"]       += " OFFL NOMERGE\n" ;
	cap["AEOLUS"]       += " OPER NOMERGE\n" ;
	cap["AEOLUS"]       += " OFFL NOMERGESTICKY\n" ;
	cap["AEOLUS"]       += " OPER NOMERGESTICKY\n" ;

	cap["PRISMA"]       += " OFFL NOMERGE\n" ;
	cap["PRISMA"]       += " OPER NOMERGE\n" ;
	cap["PRISMA"]       += " OFFL NOMERGESTICKY\n" ;
	cap["PRISMA"]       += " OPER NOMERGESTICKY\n" ;

    cap["PLATINO1-1"]    += " OFFL NOMERGE\n" ;
    cap["PLATINO1-1"]    += " OPER NOMERGE\n" ;
    cap["PLATINO1-1"]    += " OPER NOMERGESTICKY\n" ;
    cap["PLATINO1-1"]    += " OFFL NOMERGESTICKY\n" ;
    cap["PLATINO2-1"]    += " OFFL NOMERGE\n" ;
    cap["PLATINO2-1"]    += " OPER NOMERGE\n" ;
    cap["PLATINO2-1"]    += " OFFL NOMERGESTICKY\n" ;
    cap["PLATINO2-1"]    += " OPER NOMERGESTICKY\n" ;

	cap["CRYOSAT"]      += " OFFL LEVEL1 (obsolescent use MERGE instead)\n" ;
	cap["CRYOSAT"]      += " OFFL MERGE\n" ;
	cap["CRYOSAT"]      += " OFFL NOMERGE\n" ;
	cap["CRYOSAT"]      += " OFFL NOMERGESTICKY\n" ;
	cap["CRYOSAT"]      += " OFFL LEVEL2 (obsolescent use MERGE instead)\n" ;
	cap["CRYOSAT"]      += " OFFL L2ANX2ANX (obsolescent use ANX2ANX instead)\n" ;
	cap["CRYOSAT"]      += " OFFL ANX2ANX\n" ;
	cap["CRYOSAT"]      += " OFFL NOMRG_USRDISTRIB\n" ;

	cap["GOCE"]         += " OFFL L1ANX2ANX (obsolescent use ANX2ANX instead)\n" ;
	cap["GOCE"]         += " OFFL MERGE\n" ;
	cap["GOCE"]         += " OFFL ANX2ANX\n" ;
	cap["GOCE"]         += " OFFL NOMERGE\n" ;
	cap["GOCE"]         += " OFFL NOMERGESTICKY\n" ;
	cap["GOCE"]         += " OFFL GROUPEDCOLLECTION\n" ;
	cap["GOCE"]         += " CONS L1ANX2ANX (obsolescent use ANX2ANX instead)\n" ;
	cap["GOCE"]         += " CONS MERGE\n" ;
	cap["GOCE"]         += " CONS NOMERGE\n" ;
	cap["GOCE"]         += " CONS NOMERGESTICKY\n" ;
	cap["GOCE"]         += " CONS ANX2ANX\n" ;
	cap["GOCE"]         += " CONS GROUPEDCOLLECTION\n" ;
	cap["GOCE"]         += " LTA_ MERGE\n" ;
	cap["GOCE"]         += " LTA_ NOMERGE\n" ;
	cap["GOCE"]         += " LTA_ NOMERGESTICKY\n" ;
	cap["GOCE"]         += " LTA_ ANX2ANX\n" ;
	cap["GOCE"]         += " LTA_ GROUPEDCOLLECTION\n" ;

	cap["ENVISAT"]      += " OFFL MERGE\n" ;
	cap["ENVISAT"]      += " OFFL NOMERGE\n" ;
	cap["ENVISAT"]      += " OFFL NOMERGESTICKY\n" ;

	cap["ERS"]          += " OFFL MERGE\n" ;
	cap["ERS"]          += " OFFL NOMERGE\n" ;
	cap["ERS"]          += " OFFL NOMERGESTICKY\n" ;

	cap["EOSAM"]        += " OFFL NOMERGE\n" ;
	cap["EOSAM"]        += " OFFL NOMERGESTICKY\n" ;

	cap["EOSPM"]        += " OFFL NOMERGE\n" ;
	cap["EOSPM"]        += " OFFL NOMERGESTICKY\n" ;

	cap["ALOS"]         += " OFFL NOMERGE\n" ;
	cap["ALOS"]         += " OFFL NOMERGESTICKY\n" ;

	cap["RADARSAT"]     += " OFFL NOMERGE\n" ;
	cap["RADARSAT"]     += " OFFL NOMERGESTICKY\n" ;

	cap["OCEANSAT"]     += " OFFL NOMERGE\n" ;
	cap["OCEANSAT"]     += " OFFL NOMERGESTICKY\n" ;

    cap["SACD"]         += " OFFL NOMERGE\n" ;
    cap["SACD"]         += " OFFL NOMERGESTICKY\n" ;

	cap["METOP"]        += " OFFL NOMERGE\n" ;
	cap["METOP"]        += " OFFL NOMERGESTICKY\n" ;

	cap["SENTINEL-1A"]  += " OFFL NOMERGE\n" ;
	cap["SENTINEL-1B"]  += " OFFL NOMERGE\n" ;
	cap["SENTINEL-2A"]  += " OFFL NOMERGE\n" ;
	cap["SENTINEL-2B"]  += " OFFL NOMERGE\n" ;
    cap["SENTINEL-3A"]  += " OFFL NOMERGE\n" ;
    cap["SENTINEL-3B"]  += " OFFL NOMERGE\n" ;
    cap["SENTINEL-3C"]  += " OFFL NOMERGE\n" ;
    cap["SENTINEL-3D"]  += " OFFL NOMERGE\n" ;
    cap["SENTINEL-3"]   += " OFFL NOMERGE\n" ;
	cap["SENTINEL-1A"]  += " OFFL NOMERGENODUP\n" ;
	cap["SENTINEL-1B"]  += " OFFL NOMERGENODUP\n" ;
	cap["SENTINEL-2A"]  += " OFFL NOMERGENODUP\n" ;
	cap["SENTINEL-2B"]  += " OFFL NOMERGENODUP\n" ;
    cap["SENTINEL-3A"]  += " OFFL NOMERGENODUP\n" ;
    cap["SENTINEL-3B"]  += " OFFL NOMERGENODUP\n" ;
    cap["SENTINEL-3C"]  += " OFFL NOMERGENODUP\n" ;
    cap["SENTINEL-3D"]  += " OFFL NOMERGENODUP\n" ;
    cap["SENTINEL-3"]   += " OFFL NOMERGENODUP\n" ;
	cap["SENTINEL-1A"]  += " OFFL NOMERGESTICKY\n" ;
	cap["SENTINEL-1B"]  += " OFFL NOMERGESTICKY\n" ;
	cap["SENTINEL-2A"]  += " OFFL NOMERGESTICKY\n" ;
	cap["SENTINEL-2B"]  += " OFFL NOMERGESTICKY\n" ;
    cap["SENTINEL-3A"]  += " OFFL NOMERGESTICKY\n" ;
    cap["SENTINEL-3B"]  += " OFFL NOMERGESTICKY\n" ;
    cap["SENTINEL-3C"]  += " OFFL NOMERGESTICKY\n" ;
    cap["SENTINEL-3D"]  += " OFFL NOMERGESTICKY\n" ;
    cap["SENTINEL-3"]   += " OFFL NOMERGESTICKY\n" ;
	cap["SENTINEL-1A"]  += "  NRT NOMERGE\n" ;
	cap["SENTINEL-1B"]  += "  NRT NOMERGE\n" ;
	cap["SENTINEL-2A"]  += "  NRT NOMERGE\n" ;
	cap["SENTINEL-2B"]  += "  NRT NOMERGE\n" ;
    cap["SENTINEL-3A"]  += "  NRT NOMERGE\n" ;
    cap["SENTINEL-3B"]  += "  NRT NOMERGE\n" ;
    cap["SENTINEL-3C"]  += "  NRT NOMERGE\n" ;
    cap["SENTINEL-3D"]  += "  NRT NOMERGE\n" ;
    cap["SENTINEL-3"]   += "  NRT NOMERGE\n" ;
	cap["SENTINEL-1A"]  += "  NRT NOMERGESTICKY\n" ;
	cap["SENTINEL-1B"]  += "  NRT NOMERGESTICKY\n" ;
	cap["SENTINEL-2A"]  += "  NRT NOMERGESTICKY\n" ;
	cap["SENTINEL-2B"]  += "  NRT NOMERGESTICKY\n" ;
    cap["SENTINEL-3A"]  += "  NRT NOMERGESTICKY\n" ;
    cap["SENTINEL-3B"]  += "  NRT NOMERGESTICKY\n" ;
    cap["SENTINEL-3C"]  += "  NRT NOMERGESTICKY\n" ;
    cap["SENTINEL-3D"]  += "  NRT NOMERGESTICKY\n" ;
    cap["SENTINEL-3"]   += "  NRT NOMERGESTICKY\n" ;

	cap["ALSAT-1B"]  += " OFFL NOMERGE\n" ;
	cap["ALSAT-1B"]  += " OFFL NOMERGESTICKY\n" ;
	cap["ALSAT-1B"]  += " OFFL NOMERGENODUP\n" ;

    cap["LANDSAT"]      += " OFFL NOMERGE\n" ;
    cap["SPOT"]         += " OFFL NOMERGE\n" ;
    cap["JERS"]         += " OFFL NOMERGE\n" ;
    cap["LANDSAT"]      += " OFFL NOMERGESTICKY\n" ;
    cap["SPOT"]         += " OFFL NOMERGESTICKY\n" ;
    cap["JERS"]         += " OFFL NOMERGESTICKY\n" ;
    
    cap["SENTINEL-3A"]  += " OFFL REG-X-\n" ;
    cap["SENTINEL-3B"]  += " OFFL REG-X-\n" ;
    cap["SENTINEL-3C"]  += " OFFL REG-X-\n" ;
    cap["SENTINEL-3D"]  += " OFFL REG-X-\n" ;
    cap["SENTINEL-3"]   += " OFFL REG-X-\n" ;

	cap["MUSTER"]   += " OFFL NOMERGE\n" ;
	cap["MUSTER"]   += " OFFL NOMERGESTICKY\n" ;
	cap["MUSTER"]   += " OFFL NOMERGENODUP\n" ;
	cap["MUSTER"]   += " OFFL REG-X-\n" ;
	cap["MUSTER2"]  += " OFFL NOMERGE\n" ;
	cap["MUSTER2"]  += " OFFL NOMERGESTICKY\n" ;
	cap["MUSTER2"]  += " OFFL NOMERGENODUP\n" ;
	cap["MUSTER2"]  += " OFFL REG-X-\n" ;

	return ;

}

_ACS_END_NAMESPACE
