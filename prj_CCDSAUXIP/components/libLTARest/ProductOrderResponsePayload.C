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

	LTA Rest Interface


*/

#include <ProductOrderResponsePayload.h>
#include <FtpCurl.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace curl;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ProductOrderResponsePayload)
ACS_CLASS_DEFINE_DEBUG_LEVEL(ProductOrderResponsePayloadList)

std::string ProductOrderResponsePayload::jobStatus2String(const ProductOrderResponsePayload::JobStatus& js) {
	std::string status;
	switch (js) {
	case ProductOrderResponsePayload::unknown:
		status = "unknown";
		break;
	case ProductOrderResponsePayload::queued:
		status = "queued";
		break;
	case ProductOrderResponsePayload::in_progress:
		status = "in_progress";
		break;
	case ProductOrderResponsePayload::completed:
		status = "completed";
		break;
	case ProductOrderResponsePayload::failed:
		status = "failed";
		break;
	case ProductOrderResponsePayload::cancelled:
		status = "cancelled";
		break;
	default:
		std::ostringstream ss;
		ss << "Invalid job status having id " << static_cast<unsigned int>(js);
		ACS_THROW(ProductOrderResponsePayloadException(ss.str()));
		break;
	}
	
	return status;
}

ProductOrderResponsePayload::JobStatus ProductOrderResponsePayload::string2JobStatus(const std::string& js) {
	JobStatus val = ProductOrderResponsePayload::unknown;
	if (StringUtils::equalsNoCase(js, "unknown")) {
		val = ProductOrderResponsePayload::unknown;
	}
	else if (StringUtils::equalsNoCase(js, "queued")) {
		val = ProductOrderResponsePayload::queued;
	}
	else if (StringUtils::equalsNoCase(js, "in_progress")) {
		val = ProductOrderResponsePayload::in_progress;
	}
	else if (StringUtils::equalsNoCase(js, "completed")) {
		val = ProductOrderResponsePayload::completed;
	}
	else if (StringUtils::equalsNoCase(js, "failed")) {
		val = ProductOrderResponsePayload::failed;
	}
	else if (StringUtils::equalsNoCase(js, "cancelled")) {
		val = ProductOrderResponsePayload::cancelled;
	}
	else {
		ACS_THROW(ProductOrderResponsePayloadException("Unrecognized status \"" + js + "\""));
	}
	
	return val;
}

ProductOrderResponsePayload::ProductOrderResponsePayload() :
	_mutex(),
	_orderIdSet(false),
	_statusSet(false),
	_statusMessageSet(false),
	_orderSizeSet(false),
	_submissionDateSet(false),
	_estimatedDateSet(false),
	_completedDateSet(false),
	_evictionDateSet(false),
	_prioritySet(false),
	_endpointSet(false),
	_epuserSet(false),
	_eppasswdSet(false),
	_orderId(),
	_status(),
	_statusMessage(),
	_orderSize(),
	_submissionDate(),
	_estimatedDate(),
	_completedDate(),
	_evictionDate(),
	_priority(),
	_endpoint(),
	_epuser(),
	_eppasswd()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

ProductOrderResponsePayload::ProductOrderResponsePayload(const ProductOrderResponsePayload& data) :
	_mutex(),
	_orderIdSet(data._orderIdSet),
	_statusSet(data._statusSet),
	_statusMessageSet(data._statusMessageSet),
	_orderSizeSet(data._orderSizeSet),
	_submissionDateSet(data._submissionDateSet),
	_estimatedDateSet(data._estimatedDateSet),
	_completedDateSet(data._completedDateSet),
	_evictionDateSet(data._evictionDateSet),
	_prioritySet(data._prioritySet),
	_endpointSet(data._endpointSet),
	_epuserSet(data._epuserSet),
	_eppasswdSet(data._eppasswdSet),
	_orderId(data._orderId),
	_status(data._status),
	_statusMessage(data._statusMessage),
	_orderSize(data._orderSize),
	_submissionDate(data._submissionDate),
	_estimatedDate(data._estimatedDate),
	_completedDate(data._completedDate),
	_evictionDate(data._evictionDate),
	_priority(data._priority),
	_endpoint(data._endpoint),
	_epuser(data._epuser),
	_eppasswd(data._eppasswd)
{ }
ProductOrderResponsePayload::ProductOrderResponsePayload(const nlohmann::json& js) :
	_mutex(),
	_orderIdSet(false),
	_statusSet(false),
	_statusMessageSet(false),
	_orderSizeSet(false),
	_submissionDateSet(false),
	_estimatedDateSet(false),
	_completedDateSet(false),
	_evictionDateSet(false),
	_prioritySet(false),
	_endpointSet(false),
	_epuserSet(false),
	_eppasswdSet(false),
	_orderId(),
	_status(),
	_statusMessage(),
	_orderSize(),
	_submissionDate(),
	_estimatedDate(),
	_completedDate(),
	_evictionDate(),
	_priority(),
	_endpoint(),
	_epuser(),
	_eppasswd()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSONObject(js);
}
ProductOrderResponsePayload::ProductOrderResponsePayload(const std::string& jsstr):
	_mutex(),
	_orderIdSet(false),
	_statusSet(false),
	_statusMessageSet(false),
	_orderSizeSet(false),
	_submissionDateSet(false),
	_estimatedDateSet(false),
	_completedDateSet(false),
	_evictionDateSet(false),
	_prioritySet(false),
	_endpointSet(false),
	_epuserSet(false),
	_eppasswdSet(false),
	_orderId(),
	_status(),
	_statusMessage(),
	_orderSize(),
	_submissionDate(),
	_estimatedDate(),
	_completedDate(),
	_evictionDate(),
	_priority(),
	_endpoint(),
	_epuser(),
	_eppasswd()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSON(jsstr);
}
ProductOrderResponsePayload::~ProductOrderResponsePayload() ACS_NOEXCEPT { }

ProductOrderResponsePayload& ProductOrderResponsePayload::operator=(const ProductOrderResponsePayload& data) {
	if (this != &data) {
		Lock l1(_mutex);
		Lock l2(data._mutex);
		_orderIdSet = data._orderIdSet;
		_statusSet = data._statusSet;
		_statusMessageSet = data._statusMessageSet;
		_orderSizeSet = data._orderSizeSet;
		_submissionDateSet = data._submissionDateSet;
		_estimatedDateSet = data._estimatedDateSet;
		_completedDateSet = data._completedDateSet;
		_evictionDateSet = data._evictionDateSet;
		_prioritySet = data._prioritySet;
		_endpointSet = data._endpointSet;
		_epuserSet = data._epuserSet;
		_eppasswdSet = data._eppasswdSet;
		_orderId = data._orderId;
		_status = data._status;
		_statusMessage = data._statusMessage;
		_orderSize = data._orderSize;
		_submissionDate = data._submissionDate;
		_estimatedDate = data._estimatedDate;
		_completedDate = data._completedDate;
		_evictionDate = data._evictionDate;
		_priority = data._priority;
		_endpoint = data._endpoint;
		_epuser = data._epuser;
		_eppasswd = data._eppasswd;
	}
	return *this;
}

nlohmann::json ProductOrderResponsePayload::toJSONObject() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	nlohmann::json j;
	try {

		j["@odata.context"] = "$metadata#Order/$entity";
	
		bool isSet = false;
		std::string 		ordId = getOrderId(&isSet);
		if (isSet!=false)
		{
			j["Id"] = ordId;
		}
		else
		{
			ACS_THROW(exCriticalException("Mandatory Id missing"));
		}
		std::string 		status = jobStatus2String(getStatus(&isSet));
		if (isSet!=false)
		{
			j["Status"] = status;
		}
		else
		{
			ACS_THROW(exCriticalException("Mandatory Status missing"));
		}
		std::string 		msg = getStatusMessage(&isSet);
		if (isSet!=false)
		{
			j["StatusMessage"] = msg;
		}
		else
		{
			ACS_THROW(exCriticalException("Mandatory StatusMessage missing"));
		}
		int 		ordS = getOrderSize(&isSet);
		if (isSet!=false)
		{
			j["OrderSize"] = ordS;
		}
		std::string	 	subDate = getSubmissionDate(&isSet);
		if (isSet!=false)
		{
			j["SubmissionDate"] = subDate;
		}
		else
		{
			ACS_THROW(exCriticalException("Mandatory SubmissionDate missing"));
		}
		std::string estDate = getEstimatedDate(&isSet);
	
		if (isSet!=false)
		{
			j["EstimatedDate"] = estDate;
		}
		else
		{
			ACS_THROW(exCriticalException("Mandatory EstimatedDate missing"));
		}
		std::string compDate = getCompletedDate(&isSet);
	
		if (isSet!=false)
		{
			j["CompletedDate"] = compDate;
		}
		std::string evDate = getEvictionDate(&isSet);

		if (isSet!=false)
		{
			j["EvictionDate"] = evDate;
		}
		int priority = getPriority(&isSet);
		if (isSet!=false)
		{
			j["Priority"] = priority;
		}
		else
		{
			ACS_THROW(exCriticalException("Mandatory Priority missing"));
		}
		std::string notificationEp = getNotificationEndpoint(&isSet);
		if (isSet!=false)
		{
			j["NotificationEndpoint"] = notificationEp;
		}
		std::string epUser = getNotificationEpUsername(&isSet);
		if (isSet!=false)
		{
			j["NotificationEpUsername"] = epUser;
		}
		std::string epPw = getNotificationEpPassword(&isSet);
		if (isSet!=false)
		{
			j["NotificationEpPassword"] = epPw;
		}
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(ProductOrderResponsePayloadException(e, "Error creating JSON Object."));
	}
	return j;
}

std::string ProductOrderResponsePayload::toJSON() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	std::string jsstr;
	nlohmann::json js;
	try {
		js = toJSONObject();
		jsstr = js.dump();
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(ProductOrderResponsePayloadException(e, "Error creating JSON."));
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Transformed request data into the following json string:\n" << jsstr);
	
	return jsstr;
}


void ProductOrderResponsePayload::fromJSON(const std::string& jsstr) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	nlohmann::json js;
	try {
		js = nlohmann::json::parse(jsstr);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(ProductOrderResponsePayloadException(e, "Error reading JSON."));
	}
	fromJSONObject(js);
}

void ProductOrderResponsePayload::fromJSONObject(const nlohmann::json& j) {

	std::string   	orderId;
	std::string   	statusMessage;
	int		orderSize(0);
	std::string   	submissionDate;
	std::string   	estimatedDate;
	std::string   	completedDate;
	std::string   	evictionDate;
	int	 	priority(0);
	std::string   	endpoint;
	std::string   	epuser;
	std::string	eppasswd;
     
	if (j.find("Id") != j.end() && !j["Id"].is_null()) 
	{
     		j.at("Id").get_to<std::string>(orderId);
		if (!orderId.empty())
		{
			setOrderId(orderId);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << orderId << "\" associated to key Id.");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"Id\". It will not be loaded.");
		}
	}
	else
	{
		ACS_LOG_WARNING("Mandatory Id element not found!");
	}
	if (j.find("Status") != j.end() && !j["Status"].is_null()) {
		std::string val = j["Status"];
		if (!val.empty()) {
			setStatus(string2JobStatus(val));
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key status.");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"status\". It will not be loaded.");
		}
	} else {
			ACS_LOG_WARNING("Key \"status\" not found or the associated value was null. It will not be loaded.");
	}
	
	if (j.find("StatusMessage") != j.end() && !j["StatusMessage"].is_null()) 
	{
     		j.at("StatusMessage").get_to<std::string>(statusMessage);
		if (!statusMessage.empty())
		{
			setStatusMessage(statusMessage);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << statusMessage << "\" associated to key StatusMessage.");
		}
		else 
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"StatusMessage\". It will not be loaded.");
		}
	}
	else
	{
		ACS_LOG_WARNING("Mandatory StatusMessage element not found!");
	}
	if (j.find("OrderSize") != j.end() && !j["OrderSize"].is_null())
	{
     		j.at("OrderSize").get_to<int>(orderSize);
		setOrderSize(orderSize);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << orderSize << "\" associated to key OrderSize.");
	}
	else
	{
		bool isSet = false;
		JobStatus s = getStatus(&isSet);
		if (isSet && ProductOrderResponsePayload::completed == s)
		{
			ACS_LOG_WARNING("Mandatory OrderSize element not found!");
		}
	}
	if (j.find("SubmissionDate") != j.end() && !j["SubmissionDate"].is_null())
	{
		j.at("SubmissionDate").get_to<std::string>(submissionDate);
		if (!submissionDate.empty())
		{
			setSubmissionDate(submissionDate);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << submissionDate << "\" associated to key SubmissionDate.");
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"SubmissionDate\". It will not be loaded.");
		}
		
	}
	else
	{
		ACS_LOG_WARNING("Mandatory SubmissionDate element not found!");
	}
	if (j.find("EstimatedDate") != j.end() && !j["EstimatedDate"].is_null())
	{
     		j.at("EstimatedDate").get_to<std::string>(estimatedDate);
		if (!estimatedDate.empty())
		{
			setEstimatedDate(estimatedDate);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << estimatedDate << "\" associated to key EstimatedDate.");
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"EstimatedDate\". It will not be loaded.");
		}
	}
	else
	{
		ACS_LOG_WARNING("Mandatory EstimatedDate element not found!");
	}
	if (j.find("CompletedDate") != j.end() && !j["CompletedDate"].is_null())
	{
     		j.at("CompletedDate").get_to<std::string>(completedDate);
		if (!completedDate.empty())
		{
			setCompletedDate(completedDate);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << completedDate << "\" associated to key CompletedDate.");
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"CompletedDate\". It will not be loaded.");
		}
		
	}
	if (j.find("EvictionDate") != j.end() && !j["EvictionDate"].is_null())
	{
     		j.at("EvictionDate").get_to<std::string>(evictionDate);
		if (!evictionDate.empty())
		{
			setEvictionDate(evictionDate);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << evictionDate << "\" associated to key EvictionDate.");
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"EvictionDate\". It will not be loaded.");
		}
	}
	if (j.find("Priority") != j.end() && !j["Priority"].is_null())
	{
     		j.at("Priority").get_to<int>(priority);
		setPriority(priority);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << priority << "\" associated to key Priority.");
	}
	else
	{
		ACS_LOG_WARNING("Mandatory Priority element not found!");
	}     	
	if (j.find("NotificationEndpoint") != j.end() && !j["NotificationEndpoint"].is_null())
	{
     		j.at("NotificationEndpoint").get_to<std::string>(endpoint);
		if (!endpoint.empty())
		{
			setNotificationEndpoint(endpoint);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << endpoint << "\" associated to key NotificationEndpoint.");
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"NotificationEndpoint\". It will not be loaded.");
		}
	}
	if (j.find("NotificationEpUsername") != j.end() && !j["NotificationEpUsername"].is_null())
	{
     		j.at("NotificationEpUsername").get_to<std::string>(epuser);
		if (!epuser.empty())
		{
			setNotificationEpUsername(epuser);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << epuser << "\" associated to key NotificationEpUsername.");
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"NotificationEpUsername\". It will not be loaded.");
		}
		
	}
	if (j.find("NotificationEpPassword") != j.end() && !j["NotificationEpPassword"].is_null())
	{
     		j.at("NotificationEpPassword").get_to<std::string>(eppasswd);
		
		if (!eppasswd.empty())
		{
			setNotificationEpPassword(eppasswd);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << eppasswd << "\" associated to key NotificationEpPassword.");
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"NotificationEpPassword\". It will not be loaded.");
		}
	}
}

ProductOrderResponsePayloadList::ProductOrderResponsePayloadList() : 
	_mutex(),
	_response()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

ProductOrderResponsePayloadList::ProductOrderResponsePayloadList(const std::string& jsstr) : 
	_mutex(),
	_response()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSON(jsstr);
}

ProductOrderResponsePayloadList::ProductOrderResponsePayloadList(const nlohmann::json& js) : 
	_mutex(),
	_response()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSONObject(js);
}

ProductOrderResponsePayloadList::ProductOrderResponsePayloadList(const ProductOrderResponsePayloadList& list) :
	_mutex(),
	_response(list._response)
{ }

ProductOrderResponsePayload& ProductOrderResponsePayloadList::operator[](size_t idx) {
	Lock l(_mutex);
	if (idx >= _response.size()) {
		std::ostringstream ss;
		ss << "Invalid index " << idx << ". Size is " << _response.size();
		ACS_THROW(exBoundsException(ss.str()));
	}
	
	return _response[idx];
}

const ProductOrderResponsePayload& ProductOrderResponsePayloadList::operator[](size_t idx) const {
	Lock l(_mutex);
	if (idx >= _response.size()) {
		std::ostringstream ss;
		ss << "Invalid index " << idx << ". Size is " << _response.size();
		ACS_THROW(exBoundsException(ss.str()));
	}
	
	return _response[idx];
}

ProductOrderResponsePayloadList::~ProductOrderResponsePayloadList() ACS_NOEXCEPT { }

void ProductOrderResponsePayloadList::add(const ProductOrderResponsePayload& payload) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	_response.push_back(payload);
}

void ProductOrderResponsePayloadList::add(const ProductOrderResponsePayloadList& payload) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	Lock l(_mutex);
	const std::vector<ProductOrderResponsePayload>& p = payload.getResponses();
	_response.insert(_response.end(), p.begin(), p.end());
}

const std::vector<ProductOrderResponsePayload>& ProductOrderResponsePayloadList::getResponses() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	return _response;
}

void ProductOrderResponsePayloadList::clear() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	_response.clear();
}

size_t ProductOrderResponsePayloadList::size() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	return _response.size();
}

nlohmann::json ProductOrderResponsePayloadList::toJSONObject() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	nlohmann::json js;
	try {
		
		js["value"] = nlohmann::json::array(); // create the empty array
		std::vector<ProductOrderResponsePayload> rq = _response;

		size_t rqSize = rq.size();
		for (size_t idx = 0; idx < rqSize; ++idx) {
			const ProductOrderResponsePayload& payload = rq[idx];
			nlohmann::json rqjs = payload.toJSONObject();
			js["value"].push_back(rqjs);
		}
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(ProductOrderResponsePayloadListException(e, "Error creating JSON Object."));
	}
	 
	return js;
}

std::string ProductOrderResponsePayloadList::toJSON() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string jsdumped;
	nlohmann::json js;
	try {
		js = toJSONObject();
		jsdumped = js.dump();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generated the following json: " << jsdumped)
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(ProductOrderResponsePayloadListException(e, "Error creating JSON."));
	}
	return jsdumped;
}

void ProductOrderResponsePayloadList::fromJSON(const std::string& json)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	nlohmann::json js;
	try {
		js = nlohmann::json::parse(json);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(ProductOrderResponsePayloadListException(e, "Error reading JSON."));
	}
	
	fromJSONObject(js);
}


void ProductOrderResponsePayloadList::fromJSONObject(const nlohmann::json& js) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	try {
		if (js.find("value") == js.end()) {
			ACS_THROW(ProductOrderResponsePayloadListException("Cannot find key \"value\" into json response payload. value key is mandatory"));
		}
		size_t listSize = js["value"].size();
		for (size_t idx = 0; idx < listSize; ++idx) {
			nlohmann::json jsobj = js["value"][idx];
			ProductOrderResponsePayload rq(jsobj);
			add(rq);
		}
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(ProductOrderResponsePayloadListException(e, "Error reading JSON Object."));
	}
}

_ACS_END_NAMESPACE
