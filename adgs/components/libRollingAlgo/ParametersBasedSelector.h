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

	$Prod: A.C.S. libRollingAlgo$

 */

#ifndef _ParametersBasedSelector_H_
#define _ParametersBasedSelector_H_

#include <InvPolicyCommonSelector.h>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief
 * Implement the InvPolicySelector class selecting inventory items
 * according to general sql statements defined in Policy's parameters clob.
 **/

class ParametersBasedSelector: public InvPolicyCommonSelector  { // PRQA S 2109
public:

	/** Class constructor */
	explicit ParametersBasedSelector(const InvPolicy &);
	/** Destructor */
	virtual ~ParametersBasedSelector() ACS_NOEXCEPT ;

	ACS_DECLARE_NO_COPY_CLASS(ParametersBasedSelector);

public:

	/** IF implementation. cf. base class */
	virtual void selectInit( dbConnection & conn );

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ParametersBasedSelector)

};

_ACS_END_NAMESPACE

#endif //_ParametersBasedSelector_H_

