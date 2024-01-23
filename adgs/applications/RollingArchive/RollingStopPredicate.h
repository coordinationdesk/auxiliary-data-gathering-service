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

#ifndef _RollingStopPredicate_H_
#define _RollingStopPredicate_H_

#include <ConfigurationSingleton.h>

#include <StopController.h>

#include <exException.h>



_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* The Rolling stop predicate to pass to services
**/

class RollingStopPredicate: public ConfigurationSingleton::StopPredicate  {
public:

	/** Default Class constructor */
	RollingStopPredicate();
	/** Destructor */
	virtual ~RollingStopPredicate() ACS_NOEXCEPT ;

    virtual bool evalCondition() const ACS_NOEXCEPT {
    	StopController& stopController = AppStopController();
    	return stopController.requestedCancel();
    }

	ACS_DECLARE_NO_COPY_CLASS(RollingStopPredicate);

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(RollingStopPredicate)

};

_ACS_END_NAMESPACE

#endif //_RollingStopPredicate_H_

