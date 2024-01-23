// PRQA S 1050 EOF
/*

	Copyright 2020-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: Ftp Curl Library $

*/

#include <HttpProgressData.h>
#include <Filterables.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl)

ACS_CLASS_DEFINE_DEBUG_LEVEL(HttpProgressData);

HttpProgressData::HttpProgressData(const StopController& sc) :
	stopController_(sc)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

int HttpProgressData::progress_callback(void* ptrtodata, double, double, double, double ) // NOSONAR - callback 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	ACS_COND_THROW(!ptrtodata, exIllegalValueException("Cannot reinterpret cast the input data into an HttpProgressData object"));
	
	auto progressData = reinterpret_cast<const HttpProgressData*>(ptrtodata); // NOSONAR - callback 
	
    if (progressData->stopController_.haltImmediate()) {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "halt level is IMMEDIATE");
        return -1;	// causes exit
    }
    
    return 0;
}

_ACS_END_NESTED_NAMESPACE
