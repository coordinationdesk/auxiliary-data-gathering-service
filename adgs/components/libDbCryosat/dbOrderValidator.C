// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.2  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:26  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:31:02  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:30  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.3  2012/02/14 12:52:50  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.2  2008/11/13 19:49:56  marpas
	extra tokens at end of include directive removed
	
	Revision 2.1  2008/10/29 11:57:49  marpas
	dbOrderValidator added
	

*/


#include <dbOrderValidator.h>
#include <ConfigurationSingleton.h>
#include <rsResourceSet.h>
#include <dbOrder.h>
#include <Filterables.h>
#include <sstream>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbOrderValidator) 

dbOrderValidator::dbOrderValidator(const dbOrder &order) :
	_theOrder(order) // PRQA S 2528
{

}

dbOrderValidator::~dbOrderValidator() throw()
{
}

void dbOrderValidator::validate()  // PRQA S 4020, 4211
{
	unsigned int maxOrderLengthDay = 0 ;
	string key = "dbOrder.defaultMaxValidityLength" ;
	try {
		ConfigurationSingleton::instance()->get().getValue("dbOrder.defaultMaxValidityLength", maxOrderLengthDay) ;
	}
	catch(rsResourceSet::NotFoundException &) {
		ACS_LOG_WARNING("dbOrderValidator::validate: WARNING cannot find key in configuration: \"" << key << "\"") ; 
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Max lenght of an order (" << key << ") " <<  maxOrderLengthDay << " [day] ") ; 
	
	if (0 == maxOrderLengthDay) {
		return ; 
    }
		
	DateTime maxDate = _theOrder.getStartTime() ;
	maxDate += DateTime::JD50(maxOrderLengthDay) ; // PRQA S 3011, 3050, 3081
	if (maxDate >=  _theOrder.getStopTime() ) {
		return ;
    }
	
	ostringstream os ;
	os << "Max order length exceeded (stop-start time). Max length is " << maxOrderLengthDay << " [day]" ;
	ACS_THROW(dbException(os.str())) ; // PRQA S 3081

}
_ACS_END_NAMESPACE
