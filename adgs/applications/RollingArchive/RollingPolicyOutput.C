/*
	Copyright 1995-2020, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	$Prod: A.C.S. RollingArchive$
        
*/ 

#include <RollingPolicyOutput.h>

#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(RollingPolicyOutput)



RollingPolicyOutput::RollingPolicyOutput():
_policyId(0),
_isSpecialPolicy(false),
_newPolicyStatus(),
_listRollingActions(),
_listRollingActionsSkipped()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

RollingPolicyOutput::~RollingPolicyOutput() ACS_NOEXCEPT {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void RollingPolicyOutput::applyBlackList() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	size_t sizeBefore = _listRollingActions.size();

	std::set<RollingAction> difference;

	std::set_difference( _listRollingActions.begin(), _listRollingActions.end(),
			_listRollingActionsSkipped.begin(), _listRollingActionsSkipped.end(),
			std::inserter(difference, difference.end()));

	_listRollingActions = difference;

	size_t sizeAfter = _listRollingActions.size();

	if ( sizeBefore != sizeAfter ) {
		ACS_LOG_WARNING( "Policy id ["<<_policyId<<"]: Filtered out ["<< sizeBefore - sizeAfter <<"] actions because they were black listed in previous loop." );
	}

}

//GETTERS AND SETTERS
dbPersistent::IdType RollingPolicyOutput::getPolicyId() const {
	return _policyId;
}

void RollingPolicyOutput::setPolicyId(dbPersistent::IdType policyId) {
	_policyId = policyId;
}

bool RollingPolicyOutput::isSpecialPolicy() const {
	return _isSpecialPolicy;
}

void RollingPolicyOutput::setIsSpecialPolicy(bool isSpecialPolicy) {
	_isSpecialPolicy = isSpecialPolicy;
}

std::set<RollingAction>& RollingPolicyOutput::getListRollingActions() {
	return _listRollingActions;
}

std::set<RollingAction>& RollingPolicyOutput::getListRollingActionsSkipped() {
	return _listRollingActionsSkipped;
}

const std::string& RollingPolicyOutput::getNewPolicyStatus() const {
	return _newPolicyStatus;
}

void RollingPolicyOutput::setNewPolicyStatus(const std::string& newPolicyStatus) {
	_newPolicyStatus = newPolicyStatus;
}


_ACS_END_NAMESPACE

