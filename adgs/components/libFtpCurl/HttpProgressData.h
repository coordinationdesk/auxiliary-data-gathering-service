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

#ifndef _HttpProgressData_H_
#define _HttpProgressData_H_

#include <StopController.h>
#include <curl/curl.h> // PRQA S 1013

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl)

class HttpProgressData {
public:
	/** Ctors/Dtors */
	explicit HttpProgressData(const StopController& sc);
	virtual ~HttpProgressData() ACS_NOEXCEPT;

	static int progress_callback(void* ptrtodata, 
									double downloadTotal, 
									double downloadNow, 
									double uploadTotal,
									double uploadNow);

private:
	// Defined but not implemented
	HttpProgressData();
	HttpProgressData(const HttpProgressData&);
	HttpProgressData& operator=(const HttpProgressData&);
	
private:
	const StopController &stopController_;
    
	ACS_CLASS_DECLARE_DEBUG_LEVEL(HttpProgressData);
	
};

_ACS_END_NESTED_NAMESPACE

#endif /* _HttpProgressData_H_ */
