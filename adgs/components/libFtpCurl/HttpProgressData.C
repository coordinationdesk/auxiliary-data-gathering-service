// PRQA S 1050 EOF
/*

	Copyright 2020, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
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

HttpProgressData::~HttpProgressData() ACS_NOEXCEPT { }

int HttpProgressData::progress_callback(void* ptrtodata, 
								double downloadTotal, 
								double downloadNow, 
								double uploadTotal,
								double uploadNow)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	HttpProgressData* progressData = reinterpret_cast<HttpProgressData*>(ptrtodata); // PRQA S 3081
	
	ACS_COND_THROW(0 == progressData, exIllegalValueException("Cannot reinterpret cast the input data into an HttpProgressData object"));
	
    if (progressData->stopController_.haltImmediate()) {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "halt level is IMMEDIATE");
        return -1;	// causes exit
    }
    
    return 0;
}

_ACS_END_NESTED_NAMESPACE
