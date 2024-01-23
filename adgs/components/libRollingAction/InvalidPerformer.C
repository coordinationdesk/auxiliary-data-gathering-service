// PRQA S 1050 EOF
/*
	Copyright 1995-2022, Exprivia SpA - DFDA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: Rolling Archive $

*/


#include <InvalidPerformer.h>

#include <RollingAction.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(InvalidPerformer)

InvalidPerformer::InvalidPerformer() : ActionPerformer()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

InvalidPerformer::~InvalidPerformer() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void InvalidPerformer::dowork(const RollingAction & actionToProcess, bool & errorOccurred, RollingReport & rollingReport, ConfigurationSingleton::StopPredicate & stop, dbConnection & conn) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	try {

		RollingReportAutoSession rollingReportAutoSession( rollingReport );

		string uniqueId;
		RollingReport::InfoRecord currentItemInfo;
		// CPS-91: the last parameter passed to invalidateItem allows the temporary order item removal according to the configuration
		if ( this->invalidateItem( conn, actionToProcess.getObject(), actionToProcess.getStorage(), uniqueId, currentItemInfo, true ) ) {
			rollingReport.record( uniqueId, RollingReport::invalidated, currentItemInfo );
		}

		ACS_LOG_INFO("Invalidated Inventory Id ["<<actionToProcess.getObject()<<"] "<<uniqueId<<" on storage id ["<<actionToProcess.getStorage()<<"]" );

	} catch(exception & ex) {

		errorOccurred = true;
		ACS_LOG_WARNING("Problem occurred invalidating Inventory Id ["<< actionToProcess.getObject()<<"]. Will retry on next loop. Problem occurred: "<<ex.what());
	}
	
}

_ACS_END_NAMESPACE

