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

	$Prod: $

	$Name$

	$Id$

	$Author$

	$Log$
	Revision 1.5  2013/02/05 18:17:24  marpas
	listener re-added on t_temporarydistribution & t_completedorders
	logical flow with delay rewritten in main loop
	
	Revision 1.3  2012/02/09 15:12:44  marpas
	refactoring
	
	Revision 1.2  2009/06/26 10:21:21  marpas
	no message from some class dtors
	
	Revision 1.1.1.1  2007/03/15 15:01:03  crivig
	Imported source
	
*/
 
#include <DGListener.h>
#include <StringUtils.h>


_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(DGListener)
using namespace std ;

DGListener::DGListener(dbConnectionPool& cp) : MQTTDbBroadcastListenerBase(cp)
{
}

DGListener::~DGListener() ACS_NOEXCEPT
{
}
void DGListener::init()
{
	std::vector<MQTTDbEventType> dbEvents;
	// t_temporarydistribution events
	dbEvents.push_back(MQTTDbEventType("t_temporarydistribution", "INSERT"));
	// t_completed_orders events
	dbEvents.push_back(MQTTDbEventType("t_completed_orders", "INSERT"));

	setDbEventInterests(dbEvents);
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Subscription for Db broadcast events performed.");
}

_ACS_END_NAMESPACE
