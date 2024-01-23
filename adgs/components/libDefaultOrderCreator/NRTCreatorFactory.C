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
	Revision 2.10  2013/06/19 14:54:14  marfav
	Restored null satellite id management (use zero to identify null value)
	
	Revision 2.9  2013/06/18 15:24:01  marfav
	Null satellite management fixed
	
	Revision 2.8  2013/04/09 08:39:55  marpas
	NameIdAssociator refactoring
	
	Revision 2.7  2013/02/12 13:46:53  marpas
	dbConnectionPool concept introduced
	libDBOrderCreator new interfaces
	coding best practices applied
	debug improved
	qa rules
	FileGuard used when needed instead of File::unlink mess around
	
	Revision 2.6  2012/03/07 13:50:54  marpas
	preparing order parameters file
	
	Revision 2.5  2008/11/27 14:02:17  marpas
	venus implementation ongoing
	other changes for convenience method(s)
	
	Revision 2.4  2008/11/25 11:18:11  marpas
	adding VENUS
	
	Revision 2.3  2008/09/02 15:39:55  serfol
	adding the ERS, NRT and OFFL
	
	Revision 2.2  2006/06/23 10:01:00  ivafam
	StringUtils::compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 2.1  2006/06/20 16:46:37  ivafam
	Method compareNoCAse replaced with StringUtils::equalsNoCase
	
	Revision 2.0  2006/02/28 10:24:17  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/11/28 20:18:02  marpas
	NRTExternalOrderCreator class added - supports Envisat for EXTERNAL algorithm
	
	Revision 1.1.1.1  2005/10/06 15:28:44  marfav
	Import libDefaultOrderCreator
	
	
*/


#include <acs_c++config.hpp>
#include <NRTCreatorFactory.h>
#include <NRTLevel1OrderCreator.h>
#include <NRTExternalOrderCreator.h>
#include <SatelliteName.h>
#include <StringUtils.h>


_ACS_BEGIN_NAMESPACE(acs)
	
using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(NRTCreatorFactory)


NRTCreatorFactory::NRTCreatorFactory() :
    OrderCreatorFactory() 
{
}

NRTCreatorFactory::~NRTCreatorFactory() throw() {}

OrderCreator *NRTCreatorFactory::newCreator(const string &orderType, const string &algo, unsigned int satId, dbConnection &conn)  // PRQA S 4020
{
	SatelliteName* sat = SatelliteName::instance(conn.getKey());
        string satName = ( OrderCreator::getNullSatId() == satId ) ? OrderCreator::getNullSatName() : sat->name(satId) ; // PRQA S 3000, 3384, 3385

	string nrt = "NRT" ;
	string level1 = "LEVEL1" ;
	string external = "EXTERNAL" ;

	if (!StringUtils::equalsNoCase(orderType,nrt)) {
		return 0 ;
    }

	if (StringUtils::equalsNoCase(satName,"CRYOSAT")) {
		if (StringUtils::equalsNoCase(algo,level1)) {
			return new NRTLevel1OrderCreator(nrt, level1, satId, conn) ;
        }
		if (StringUtils::equalsNoCase(algo,external)) {
			return new NRTLevel1OrderCreator(nrt, external, satId, conn) ;
        }
	}
	
	if (StringUtils::equalsNoCase(satName,"ENVISAT")) {
		if (StringUtils::equalsNoCase(algo,external)) {
			return new NRTExternalOrderCreator(nrt, external, satId, conn) ;
        }
	}
	
	if (StringUtils::equalsNoCase(satName,"ERS")) {
		if (StringUtils::equalsNoCase(algo,external)) {
			return new NRTExternalOrderCreator(nrt, external, satId, conn) ;
        }
	}
	
	return 0 ;
}

void NRTCreatorFactory::capabilities(map <string, string> &cap) const
{
	cap["CRYOSAT"] += "  NRT  LEVEL1\n" ;
	cap["CRYOSAT"] += "  NRT  EXTERNAL\n" ;
	
	cap["ENVISAT"] += "  NRT  EXTERNAL\n" ;

	cap["ERS"]     += "  NRT  EXTERNAL\n" ;

	return ;
}

_ACS_END_NAMESPACE

