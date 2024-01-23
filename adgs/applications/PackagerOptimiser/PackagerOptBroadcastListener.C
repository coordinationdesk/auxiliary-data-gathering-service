// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

*/
 
#include <PackagerOptBroadcastListener.h>
#include <rsResourceSet.h>
#include <ConfigurationSingleton.h>
#include <Filterables.h>
#include <StringUtils.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
ACS_CLASS_DEFINE_DEBUG_LEVEL(PackagerOptBroadcastListener);


PackagerOptBroadcastListener::PackagerOptBroadcastListener(dbConnectionPool& cp):
    MQTTDbBroadcastListenerBase(cp)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)  ;

	// if true a SIGUSR2 is emitted when a "wished" message comes from the DB
	sendSignal(false);
}

PackagerOptBroadcastListener::~PackagerOptBroadcastListener() ACS_NOEXCEPT 
{
}

void PackagerOptBroadcastListener::init()
{
	std::vector<MQTTDbEventType> dbEvents;
	// T_DistributionItems events
	MQTTDbEventType et1("t_distributionitems", "INSERT");
	et1.addEvent("UPDATE");
	dbEvents.push_back(et1);

	setDbEventInterests(dbEvents);
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Subscription for Db broadcast events performed.");
}

_ACS_END_NAMESPACE
