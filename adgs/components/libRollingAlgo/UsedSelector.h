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

	$Prod: Rolling Algo Library $

 */

#ifndef _UsedSelector_H_
#define _UsedSelector_H_ 

#include <InvPolicyCommonSelector.h>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief
 * Select inventory items if they have been used N times for processing (by Thin Layer).
 */

class UsedSelector: public InvPolicyCommonSelector { // PRQA S 2109
public:

	// Costructor
	explicit UsedSelector(const InvPolicy &);

	// Destructor
	virtual ~UsedSelector() ACS_NOEXCEPT;

	ACS_DECLARE_NO_COPY_CLASS(UsedSelector);

public:
	/** IF implementation. cf. base class */
	virtual void selectInit( dbConnection & conn );

private:

	// Time ( in hour) that has to pass before select the item from modification date
	int64_t _latency ;

	// Minumum numer of time object has to be used
	unsigned int _usedTimes;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(UsedSelector)
};

_ACS_END_NAMESPACE


#endif /* _UsedSelector_H_ */
