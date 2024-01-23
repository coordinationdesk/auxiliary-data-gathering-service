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

#ifndef _RollingPolicyOutput_H_
#define _RollingPolicyOutput_H_

#include <exException.h>

#include <dbPersistent.h>
#include <RollingAction.h>

#include <memory>

#include <set>

_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* Output produced by RollingPolicyProcessor
* Cosumed by the Rolling Actions executor
**/

class RollingPolicyOutput  { 
public:

	/** Default Class constructor */
	RollingPolicyOutput();
	/** Destructor */
	virtual ~RollingPolicyOutput() ACS_NOEXCEPT ;

	/** Shared Pointer */
	typedef std::shared_ptr< RollingPolicyOutput > Sptr;

	/** Remove actions related to failed items */
	void applyBlackList();

	/** Getters and Setters. Cf. attributes' descrition */
	dbPersistent::IdType           getPolicyId() const;
	void                           setPolicyId(dbPersistent::IdType policyId);
	bool                           isSpecialPolicy() const;
	void                           setIsSpecialPolicy(bool isSpecialPolicy);
	std::set<RollingAction>&       getListRollingActions();
	std::set<RollingAction>&       getListRollingActionsSkipped();
	const std::string&             getNewPolicyStatus() const;
	void                           setNewPolicyStatus(const std::string& newPolicyStatus);

	ACS_DECLARE_NO_COPY_CLASS(RollingPolicyOutput);

private:
	/** The policy id */
	dbPersistent::IdType _policyId;

	/** Is a special rolling policy (manual rolling) */
	bool _isSpecialPolicy;

	/** The new status to be annotated in the policy after the actions will be processed */
	std::string _newPolicyStatus;

	/** The Actions to apply on the set of inventory items. Filled by Policy executor. */
	std::set<RollingAction> _listRollingActions;

	/** The Failed Actions. Filled by Action executor and used by Selector to filter out items
	 * Implements the blacklist requested in S3MARINE-93 */
	std::set<RollingAction> _listRollingActionsSkipped;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(RollingPolicyOutput)

};

_ACS_END_NAMESPACE

#endif //_RollingPolicyOutput_H_

