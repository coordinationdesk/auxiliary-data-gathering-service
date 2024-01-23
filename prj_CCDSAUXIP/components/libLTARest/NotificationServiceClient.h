// PRQA S 1050 EOF
/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: LTA Rest Interface $


*/

#ifndef _NotificationServiceClient_h_
#define _NotificationServiceClient_h_

#include <acs_c++pers.hpp>
#include <RemoteSite.h>
#include <NotificationServiceRequestPayload.h>
#include <NotificationServiceResponsePayload.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This class implements the client API to interact with the LTANotificationServiceDispatcher */
class NotificationServiceClient {
public:
	exDECLARE_EXCEPTION(NotificationServiceClientException, exException);

	explicit NotificationServiceClient(const curl::RemoteSite& site);
	virtual ~NotificationServiceClient() ACS_NOEXCEPT;
	
	void setMaxChunkSize(size_t max); 
	
	NotificationServiceResponsePayloadList sendProductAvailabilityNotificationRequest(const NotificationServiceRequestPayloadList& data) const;

private:
	void split(std::vector<NotificationServiceRequestPayloadList>& chunks, const NotificationServiceRequestPayloadList& data, size_t chunkSize) const;

private:
	/* Defined but not implemented */
	NotificationServiceClient();
	NotificationServiceClient(const NotificationServiceClient&);
	NotificationServiceClient& operator=(const NotificationServiceClient&);
private:
	curl::RemoteSite _site;
	size_t _maxChunkSize;
	
    ACS_CLASS_DECLARE_DEBUG_LEVEL(NotificationServiceClient);
};

_ACS_END_NAMESPACE

#endif /* _NotificationServiceClient_h_ */
