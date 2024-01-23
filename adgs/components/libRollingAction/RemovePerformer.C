// PRQA S 1050 EOF
/*
	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Rolling Archive $
	
*/


#include <RemovePerformer.h>

#include <RollingAction.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(RemovePerformer)

RemovePerformer::RemovePerformer() : ActionPerformer()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

RemovePerformer::~RemovePerformer() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void RemovePerformer::dowork(const RollingAction & actionToProcess, bool & errorOccurred, RollingReport & rollingReport, ConfigurationSingleton::StopPredicate & stop, dbConnection & conn) const {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	try{

		RollingReportAutoSession rollingReportAutoSession( rollingReport );

		string uniqueid;
		RollingReport::InfoRecord infoRecord;
		//Remove the object from DB and from all the storage
		if ( this->removeItem( conn, actionToProcess.getObject(), uniqueid, infoRecord )  ) {
			rollingReport.record( uniqueid, RollingReport::removed, infoRecord );
		}

	} catch(exception &e) {

		errorOccurred = true;

		ACS_LOG_NOTIFY_EX(e) ;
		ACS_LOG_INFO("InventoryObject: \""<<actionToProcess.getObject()<<"\" not removed.");
	}
}

_ACS_END_NAMESPACE
