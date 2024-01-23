// PRQA S 1050 EOF
/*

    Copyright 2020, Exprivia SPA - DFDA-AS
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.it


    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Exprivia SpA

    SRV Notification Dispatcher main

*/ 
#ifndef _SRVRestInterface_H_
#define _SRVRestInterface_H_ 

#include <acs_c++config.hpp>

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <dbPersistent.h>
//#include <json.hpp>
#include <RestServerBase.h>

#include <NotificationServiceRequestPayload.h>


_ACS_BEGIN_NAMESPACE(acs)


class SRVRestInterface  : public RestServerBase::RestService {
public:
	exDECLARE_EXCEPTION(SRVRestInterfaceException, exException); // PRQA S 2131, 2502

	SRVRestInterface(const std::string & service);
	virtual ~SRVRestInterface() noexcept { }

// 	void init(size_t thr = 2);
// 
// 	void start();
// 
// 	void shutdown();
// 	void setMaxRequestSize(const uint & size) {_maxRequestSize = size; _maxRequestSizeSet=true;};

private:
//	void setupRoutes();

	virtual void doPost(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

	void setPendingTable(const NotificationServiceRequestPayload & payload);

// 	std::shared_ptr<Pistache::Http::Endpoint> _httpEndpoint;
// 	Pistache::Rest::Router _router;
	
// 	std::string _serviceName;
// 	uint	_maxRequestSize;
// 	bool _maxRequestSizeSet;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(SRVRestInterface) ;
};

_ACS_END_NAMESPACE

#endif /* _SRVRestInterface_H_ */
