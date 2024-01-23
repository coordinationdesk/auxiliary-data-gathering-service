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

#ifndef _HttpProgressData_H_
#define _HttpProgressData_H_

#include <StopController.h>
#include <curl/curl.h> // PRQA S 1013

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl)

class HttpProgressData {
public:
	/** Ctors/Dtors */
	explicit HttpProgressData(const StopController& sc);

	HttpProgressData(const HttpProgressData&) = delete ;
	HttpProgressData& operator=(const HttpProgressData&) = delete ;
	
	~HttpProgressData() = default ;

	static int progress_callback(void* ptrtodata, 
									double downloadTotal, 
									double downloadNow, 
									double uploadTotal,
									double uploadNow);

private:
	const StopController &stopController_;
    
	ACS_CLASS_DECLARE_DEBUG_LEVEL(HttpProgressData);
	
};

_ACS_END_NESTED_NAMESPACE

#endif /* _HttpProgressData_H_ */
