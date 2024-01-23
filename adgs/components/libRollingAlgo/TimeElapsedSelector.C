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

 $Prod: A.C.S. Rolling Algo Library $

 */

#include <TimeElapsedSelector.h>

#include <RollingAlgoUtils.h>

#include <Application.h>

#include <InvPolicy.h>

#include <SQLDialectFactory.h>

#include <ConfigurationSingleton.h>

#include <DateTime.h>
#include <File.h>

#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(TimeElapsedSelector)

TimeElapsedSelector::TimeElapsedSelector(const InvPolicy & policy) :
S2ParametersBasedSelector(policy),
_latency()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	policy.get_latencyhours( _latency );
}

TimeElapsedSelector::~TimeElapsedSelector() ACS_NOEXCEPT {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}

void TimeElapsedSelector::selectInit(dbConnection &conn) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ostringstream polId;
	polId << _polId;

	try {
		ostringstream hoursThreshold; hoursThreshold << _latency;
		_sqlSelections.push_back( this->getTimeThresholdQuery("inventoried", hoursThreshold.str(), conn) );

	} catch (exception& ex) {
		ACS_THROW( InvPolicySelector::exInvPolicySelectorException(ex,
				"Error in processing policy Id [" + polId.str() + "]"));
	}

}


_ACS_END_NAMESPACE
