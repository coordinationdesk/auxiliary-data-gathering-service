// PRQA S 1050 EOF
/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: LTA JobOrder Library $

*/


#include <LTAJobOrderFactory.h>
#include <JobOrderFactoryChain.h>
#include <LTAJobOrder.h>

using namespace std;
using namespace acs;

ACS_CLASS_DEFINE_DEBUG_LEVEL(LTAJobOrderFactory);

static const char* _ltaJobOrderConfigKey = "TlDaemon.PluginsRegistrationSettings.LTAJobOrderFactory";

LTAJobOrderFactory::LTAJobOrderFactory() :
	StringKeyFactory<JobOrder>(_ltaJobOrderConfigKey, &LTAJobOrderFactory::buildLTAJobOrder)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

JobOrder* LTAJobOrderFactory::buildLTAJobOrder() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return new LTAJobOrder;
}

LTAJobOrderFactory EmbeddedLTAJobOrderFactory;
