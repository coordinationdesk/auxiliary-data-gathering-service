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

#include <EstimationTimeServiceRequestPayload.h>
#include <FtpCurl.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace curl;

ACS_CLASS_DEFINE_DEBUG_LEVEL(EstimationTimeServiceRequestPayload);
ACS_CLASS_DEFINE_DEBUG_LEVEL(EstimationTimeServiceRequestPayloadList);

const std::string EstimationTimeServiceRequestPayload::REQUEST_PAYLOAD_KEY_INVENTORY_IDS = "inventory_ids";
const std::string EstimationTimeServiceRequestPayload::REQUEST_PAYLOAD_KEY_JOBRESPONSIBLE = "jobresponsible";
const std::string EstimationTimeServiceRequestPayload::REQUEST_PAYLOAD_KEY_PRIORITY = "priority";

EstimationTimeServiceRequestPayload::EstimationTimeServiceRequestPayload() :
	_mutex(),
	_jobResponsible(),
	_inventoryIds(),
	_priority(0)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

EstimationTimeServiceRequestPayload::EstimationTimeServiceRequestPayload(const std::string& jsstr) :
	_mutex(),
	_jobResponsible(),
	_inventoryIds(),
	_priority(0)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSON(jsstr);
}

EstimationTimeServiceRequestPayload::EstimationTimeServiceRequestPayload(const nlohmann::json& js) :
	_mutex(),
	_jobResponsible(),
	_inventoryIds(),
	_priority()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSONObject(js);
}

EstimationTimeServiceRequestPayload::EstimationTimeServiceRequestPayload(const EstimationTimeServiceRequestPayload& data) :
	_mutex(),
	_jobResponsible(data._jobResponsible),
	_inventoryIds(data._inventoryIds),
	_priority(data._priority)
{ }

EstimationTimeServiceRequestPayload::~EstimationTimeServiceRequestPayload() ACS_NOEXCEPT { }

EstimationTimeServiceRequestPayload& EstimationTimeServiceRequestPayload::operator=(const EstimationTimeServiceRequestPayload& data) {
	if (this != &data) {
		bool toSet = true;
		while (toSet) {
			Lock l1(_mutex);
			TryLock l2(data._mutex);
			if (l2.locked()) {
				_jobResponsible = data._jobResponsible;
				_inventoryIds = data._inventoryIds;
				_priority = data._priority;
				toSet = false;
			}
			else {
				usleep(100);
			}
		}
	}
	return *this;
}

nlohmann::json EstimationTimeServiceRequestPayload::toJSONObject() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	Lock l(_mutex);
	
	nlohmann::json js;
	try {
		// Add members to json object
		js[REQUEST_PAYLOAD_KEY_JOBRESPONSIBLE] = _jobResponsible;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _jobResponsible << "\" for key \"" << REQUEST_PAYLOAD_KEY_JOBRESPONSIBLE << "\"");
		
		js[REQUEST_PAYLOAD_KEY_INVENTORY_IDS] = _inventoryIds;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting list of inventory ids for key \"" << REQUEST_PAYLOAD_KEY_INVENTORY_IDS << "\"");
		
		js[REQUEST_PAYLOAD_KEY_PRIORITY] = _priority;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value " << _priority << " for key \"" << REQUEST_PAYLOAD_KEY_PRIORITY << "\"");

	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceRequestPayloadException(e, "Error creating JSON Object."));
	}
	return js;
}

std::string EstimationTimeServiceRequestPayload::toJSON() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	std::string jsstr;
	nlohmann::json js;
	try {
		js = toJSONObject();
		jsstr = js.dump();
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceRequestPayloadException(e, "Error creating JSON."));
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Transformed request data into the following json string:\n" << jsstr);
	
	return jsstr;
}

void EstimationTimeServiceRequestPayload::fromJSON(const std::string& jsstr) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	nlohmann::json js;
	try {
		js = nlohmann::json::parse(jsstr);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceRequestPayloadException(e, "Error reading JSON."));
	}
	fromJSONObject(js);
}

void EstimationTimeServiceRequestPayload::fromJSONObject(const nlohmann::json& js) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	
	try {
		
		ACS_COND_THROW((js.find(REQUEST_PAYLOAD_KEY_JOBRESPONSIBLE) == js.end()), 
			EstimationTimeServiceRequestPayloadException("Key \"" + REQUEST_PAYLOAD_KEY_JOBRESPONSIBLE + "\" not found into json request payload."));
			
		_jobResponsible = "";
		if (!js[REQUEST_PAYLOAD_KEY_JOBRESPONSIBLE].is_null()) {
			_jobResponsible = js[REQUEST_PAYLOAD_KEY_JOBRESPONSIBLE];
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << _jobResponsible << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_JOBRESPONSIBLE << "\".");
		
		_inventoryIds.clear();
		ACS_COND_THROW((js.find(REQUEST_PAYLOAD_KEY_INVENTORY_IDS) == js.end()),
						EstimationTimeServiceRequestPayloadException("Key \"" + REQUEST_PAYLOAD_KEY_INVENTORY_IDS + "\" not found into json request payload."));

		js[REQUEST_PAYLOAD_KEY_INVENTORY_IDS].get_to(_inventoryIds);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded " << _inventoryIds.size() << " inventory ids associated to key \"" << REQUEST_PAYLOAD_KEY_INVENTORY_IDS << "\".");

		ACS_COND_THROW((js.find(REQUEST_PAYLOAD_KEY_PRIORITY) == js.end() || js[REQUEST_PAYLOAD_KEY_PRIORITY].is_null()),
												EstimationTimeServiceRequestPayloadException("Key \"" + REQUEST_PAYLOAD_KEY_PRIORITY + "\" not found into json request payload."));
		_priority = js[REQUEST_PAYLOAD_KEY_PRIORITY];
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << _priority << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_PRIORITY << "\".");
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceRequestPayloadException(e, "Error reading JSON Object."));
	}
}

EstimationTimeServiceRequestPayloadList::EstimationTimeServiceRequestPayloadList() : 
	_mutex(),
	_requests()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

EstimationTimeServiceRequestPayloadList::EstimationTimeServiceRequestPayloadList(const std::string& jsstr) : 
	_mutex(),
	_requests()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSON(jsstr);
}

EstimationTimeServiceRequestPayloadList::EstimationTimeServiceRequestPayloadList(const nlohmann::json& js) : 
	_mutex(),
	_requests()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSONObject(js);
}

EstimationTimeServiceRequestPayloadList::EstimationTimeServiceRequestPayloadList(const EstimationTimeServiceRequestPayloadList& list) :
	_mutex(),
	_requests(list._requests)
{ }

EstimationTimeServiceRequestPayload& EstimationTimeServiceRequestPayloadList::operator[](size_t idx) {
	Lock l(_mutex);
	if (idx >= _requests.size()) {
		std::ostringstream ss;
		ss << "Invalid index " << idx << ". Size is " << _requests.size();
		ACS_THROW(exBoundsException(ss.str()));
	}
	
	return _requests[idx];
}

const EstimationTimeServiceRequestPayload& EstimationTimeServiceRequestPayloadList::operator[](size_t idx) const {
	Lock l(_mutex);
	if (idx >= _requests.size()) {
		std::ostringstream ss;
		ss << "Invalid index " << idx << ". Size is " << _requests.size();
		ACS_THROW(exBoundsException(ss.str()));
	}
	
	return _requests[idx];
}

EstimationTimeServiceRequestPayloadList::~EstimationTimeServiceRequestPayloadList() ACS_NOEXCEPT { }

void EstimationTimeServiceRequestPayloadList::add(const EstimationTimeServiceRequestPayload& payload) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	_requests.push_back(payload);
}

void EstimationTimeServiceRequestPayloadList::add(const EstimationTimeServiceRequestPayloadList& payload) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	Lock l(_mutex);
	const std::vector<EstimationTimeServiceRequestPayload>& p = payload.getRequests();
	_requests.insert(_requests.end(), p.begin(), p.end());
}

const std::vector<EstimationTimeServiceRequestPayload>& EstimationTimeServiceRequestPayloadList::getRequests() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	return _requests;
}

void EstimationTimeServiceRequestPayloadList::clear() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	_requests.clear();
}

size_t EstimationTimeServiceRequestPayloadList::size() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	return _requests.size();
}

nlohmann::json EstimationTimeServiceRequestPayloadList::toJSONObject() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	nlohmann::json js;
	try {
		
		js["estimation_time_request"] = nlohmann::json::array(); // create the empty array

		size_t rqSize = _requests.size();
		for (size_t idx = 0; idx < rqSize; ++idx) {
			const EstimationTimeServiceRequestPayload& payload = _requests[idx];
			nlohmann::json rqjs = payload.toJSONObject();
			js["estimation_time_request"].push_back(rqjs);
		}
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceRequestPayloadListException(e, "Error creating JSON Object."));
	}
	 
	return js;
}

std::string EstimationTimeServiceRequestPayloadList::toJSON() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string jsdumped;
	nlohmann::json js;
	try {
		js = toJSONObject();
		jsdumped = js.dump();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generated the following json: " << jsdumped)
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceRequestPayloadListException(e, "Error creating JSON."));
	}
	return jsdumped;
}

void EstimationTimeServiceRequestPayloadList::fromJSON(const std::string& json) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	nlohmann::json js;
	try {
		js = nlohmann::json::parse(json);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceRequestPayloadListException(e, "Error reading JSON."));
	}
	
	fromJSONObject(js);
}

void EstimationTimeServiceRequestPayloadList::fromJSONObject(const nlohmann::json& js) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	try {
		if (js.find("estimation_time_request") == js.end()) {
			ACS_THROW(EstimationTimeServiceRequestPayloadListException("Cannot find key \"estimation_time_request\" into json response payload."));
		}
		size_t listSize = js["estimation_time_request"].size();
		for (size_t idx = 0; idx < listSize; ++idx) {
			nlohmann::json jsobj = js["estimation_time_request"][idx];
			EstimationTimeServiceRequestPayload rq(jsobj);
			add(rq);
		}
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceRequestPayloadListException(e, "Error reading JSON Object."));
	}
}

_ACS_END_NAMESPACE
