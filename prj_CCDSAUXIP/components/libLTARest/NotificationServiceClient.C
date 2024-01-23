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

#include <NotificationServiceClient.h>
#include <NotificationServiceResponsePayload.h>
#include <RemoteSiteFactory.h>
#include <FtpCurl.h>
#include <LTARestV.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace curl;

namespace {
	LTARestV vers;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(NotificationServiceClient);

NotificationServiceClient::NotificationServiceClient(const curl::RemoteSite& site) :
	_site(site),
	_maxChunkSize(50)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

NotificationServiceClient::~NotificationServiceClient() ACS_NOEXCEPT {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void NotificationServiceClient::split(std::vector<NotificationServiceRequestPayloadList>& chunks, const NotificationServiceRequestPayloadList& data, size_t chunkSize) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	chunks.clear();
	if (chunkSize == 0) {
		std::ostringstream ss;
		ss << "Invalid chunk size " << chunkSize << " given as input"; 
		ACS_THROW(NotificationServiceClientException(ss.str()));
	}
	
	NotificationServiceRequestPayloadList tmplist;
	std::vector<NotificationServiceRequestPayload> requests;
	data.sort(requests);
	
	size_t rqSize = requests.size();
	for (size_t idx = 0; idx < rqSize; ++idx) {
		tmplist.add(requests[idx]);
		if (idx + 1 >= chunkSize || idx == rqSize - 1) {
			// By construction, chunks are sorted internally and externally
			chunks.push_back(tmplist);
			tmplist.clear();
		}
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The input payload list has been splitted into " << chunks.size() << " chunks of size at most " << chunkSize);
}

NotificationServiceResponsePayloadList NotificationServiceClient::sendProductAvailabilityNotificationRequest(const NotificationServiceRequestPayloadList& dataList) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	// Split the data list into chunks of size _maxPayloadListSize
	std::vector<NotificationServiceRequestPayloadList> chunks;
	try {
		split(chunks, dataList, _maxChunkSize);
	} catch (NotificationServiceClientException& e) {
		ACS_LOG_NOTIFY_EX(e);
		ACS_LOG_WARNING("No chunking will be applied for the request.");
		chunks.push_back(dataList);
	}
	
	NotificationServiceResponsePayloadList responses;
	size_t chunksNum = chunks.size();
	for (size_t idx = 0; idx < chunksNum; ++idx) {
		std::string payload = chunks[idx].toJSON();
		std::string uri = RemoteSiteFactory::toURL(_site); // with encrypted password
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Sending product availability notification request to " + uri + " and payload " + payload);
		
		curl::BandPriority noPrio;
		FtpCurl curl(noPrio);
		
		std::string responseJson = curl.httpPost(uri, payload, "application/json"); // can throw exception
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Notification service response is: " << responseJson);
		
		// Merging responses into a single list
		NotificationServiceResponsePayloadList res(responseJson);
		responses.add(res);
	}
	return responses;

}

_ACS_END_NAMESPACE
