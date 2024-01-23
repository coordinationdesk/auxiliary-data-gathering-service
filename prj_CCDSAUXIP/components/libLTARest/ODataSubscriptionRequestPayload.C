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

#include <ODataSubscriptionRequestPayload.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(ODataSubscriptionRequestPayload);

const std::string ODataSubscriptionRequestPayload::REQUEST_PAYLOAD_KEY_ODATACONTEXT = "@odata.context";
const std::string ODataSubscriptionRequestPayload::REQUEST_PAYLOAD_KEY_FILTERPARAM = "FilterParam";
const std::string ODataSubscriptionRequestPayload::REQUEST_PAYLOAD_KEY_STAGEORDER = "StageOrder";
const std::string ODataSubscriptionRequestPayload::REQUEST_PAYLOAD_KEY_NOTIFICATIONENDPOINT = "NotificationEndpoint";
const std::string ODataSubscriptionRequestPayload::REQUEST_PAYLOAD_KEY_NOTIFICATIONEPUSERNAME =
  "NotificationEpUsername";
const std::string ODataSubscriptionRequestPayload::REQUEST_PAYLOAD_KEY_NOTIFICATIONEPPASSWORD =
  "NotificationEpPassword";
const std::string ODataSubscriptionRequestPayload::REQUEST_PAYLOAD_KEY_PRIORITY = "Priority";
const std::string ODataSubscriptionRequestPayload::REQUEST_PAYLOAD_KEY_EVENT = "SubscriptionEvent";

const std::string ODataSubscriptionRequestPayload::REQUEST_PAYLOAD_DEFAULT_ODATACONTEXT =
  "$metadata#Subscription/$entity";

std::string ODataSubscriptionRequestPayload::subscriptionEvent2String(const ODataSubscriptionRequestPayload::SubscriptionEvent& se) {
	std::string event;
	switch (se) {
	case ODataSubscriptionRequestPayload::created:
		event = "created";
		break;
	case ODataSubscriptionRequestPayload::deleted:
		event = "deleted";
		break;
	default:
		std::ostringstream ss;
		ss << "Invalid subscription event having id " << static_cast<unsigned int>(se);
		ACS_THROW(ODataSubscriptionRequestPayloadException(ss.str()));
		break;
	}

	return event;
}

ODataSubscriptionRequestPayload::SubscriptionEvent ODataSubscriptionRequestPayload::string2SubscriptionEvent(const std::string& se) {
	SubscriptionEvent val = ODataSubscriptionRequestPayload::unknown;
	if (StringUtils::equalsNoCase(se, "created")) {
		val = ODataSubscriptionRequestPayload::created;
	}

	else if (StringUtils::equalsNoCase(se, "deleted")) {
		val = ODataSubscriptionRequestPayload::deleted;
	}
	else {
		ACS_THROW(ODataSubscriptionRequestPayloadException("Unrecognized subscription event \"" + se + "\""));
	}

	return val;
}

ODataSubscriptionRequestPayload::ODataSubscriptionRequestPayload() :
  _mutex(),
  _oDataContext(REQUEST_PAYLOAD_DEFAULT_ODATACONTEXT),
  _filterParam(),
  _stageOrder(false),
  _eventSet(false),
  _event(unknown),
  _notificationEndpointSet(false),
  _notificationEndpoint(),
  _notificationEpUsernameSet(false),
  _notificationEpUsername(),
  _notificationEpPasswordSet(false),
  _notificationEpPassword(),
  _prioritySet(false),
  _priority(0)
{
}

ODataSubscriptionRequestPayload::ODataSubscriptionRequestPayload(const std::string& jsstr) :
  _mutex(),
  _oDataContext(REQUEST_PAYLOAD_DEFAULT_ODATACONTEXT),
  _filterParam(),
  _stageOrder(false),
  _eventSet(false),
  _event(unknown),
  _notificationEndpointSet(false),
  _notificationEndpoint(),
  _notificationEpUsernameSet(false),
  _notificationEpUsername(),
  _notificationEpPasswordSet(false),
  _notificationEpPassword(),
  _prioritySet(false),
  _priority(0)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

ODataSubscriptionRequestPayload::ODataSubscriptionRequestPayload(const nlohmann::json& js) :
  _mutex(),
  _oDataContext(REQUEST_PAYLOAD_DEFAULT_ODATACONTEXT),
  _filterParam(),
  _stageOrder(false),
  _eventSet(false),
  _event(unknown),
  _notificationEndpointSet(false),
  _notificationEndpoint(),
  _notificationEpUsernameSet(false),
  _notificationEpUsername(),
  _notificationEpPasswordSet(false),
  _notificationEpPassword(),
  _prioritySet(false),
  _priority(0)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

ODataSubscriptionRequestPayload::ODataSubscriptionRequestPayload(
  const ODataSubscriptionRequestPayload& data) :
  _mutex(),
  _oDataContext(data._oDataContext),
  _filterParam(data._filterParam),
  _stageOrder(data._stageOrder),
  _eventSet(data._eventSet),
  _event(data._event),
  _notificationEndpointSet(data._notificationEndpointSet),
  _notificationEndpoint(data._notificationEndpoint),
  _notificationEpUsernameSet(data._notificationEpUsernameSet),
  _notificationEpUsername(data._notificationEpUsername),
  _notificationEpPasswordSet(data._notificationEpPasswordSet),
  _notificationEpPassword(data._notificationEpPassword),
  _prioritySet(data._prioritySet),
  _priority(data._priority)
{
}

ODataSubscriptionRequestPayload::~ODataSubscriptionRequestPayload() ACS_NOEXCEPT
{
}

ODataSubscriptionRequestPayload& ODataSubscriptionRequestPayload::operator=(
  const ODataSubscriptionRequestPayload& data)
{
  if (this != &data)
  {
    bool toSet = true;
    while (toSet)
    {
      Lock l1(_mutex);
      TryLock l2(data._mutex);
      if (l2.locked())
      {
        _oDataContext = data._oDataContext;
        _filterParam = data._filterParam;
        _stageOrder = data._stageOrder;
	_eventSet = data._eventSet;
	_event = data._event;
        _notificationEndpointSet = data._notificationEndpointSet;
        _notificationEndpoint = data._notificationEndpoint;
        _notificationEpUsernameSet = data._notificationEpUsernameSet;
        _notificationEpUsername = data._notificationEpUsername;
        _notificationEpPasswordSet = data._notificationEpPasswordSet;
        _notificationEpPassword = data._notificationEpPassword;
        _prioritySet = data._prioritySet;
        _priority = data._priority;
        toSet = false;
      }
      else
      {
        usleep(100);
      }
    }
  }
  return *this;
}

void ODataSubscriptionRequestPayload::fromJSON(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataSubscriptionRequestPayloadException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void ODataSubscriptionRequestPayload::fromJSONObject(const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  try
  {
    if (json.find(REQUEST_PAYLOAD_KEY_ODATACONTEXT) == json.end() || json[REQUEST_PAYLOAD_KEY_ODATACONTEXT].is_null())
    {
      ACS_LOG_WARNING("Cannot find key \"" << REQUEST_PAYLOAD_KEY_ODATACONTEXT
                                           << "\" into json request payload or the associated value is null.");
      _oDataContext = REQUEST_PAYLOAD_DEFAULT_ODATACONTEXT;
    }
    else
    {
      _oDataContext = json[REQUEST_PAYLOAD_KEY_ODATACONTEXT];
    }
    if (_oDataContext != REQUEST_PAYLOAD_DEFAULT_ODATACONTEXT)
    {
      ACS_LOG_WARNING("Value \"" << _oDataContext << "\" is not what is expected (\""
                                 << REQUEST_PAYLOAD_DEFAULT_ODATACONTEXT << "\") for \""
                                 << REQUEST_PAYLOAD_KEY_ODATACONTEXT << "\" key into json request payload ")
    }

    if (json.find(REQUEST_PAYLOAD_KEY_FILTERPARAM) == json.end() || json[REQUEST_PAYLOAD_KEY_FILTERPARAM].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << REQUEST_PAYLOAD_KEY_FILTERPARAM
         << "\" into json request payload or the associated value is null. Key is mandatory";
      ACS_THROW(ODataSubscriptionRequestPayloadException(os.str()));
    }
    _filterParam = json[REQUEST_PAYLOAD_KEY_FILTERPARAM];
    if (_filterParam.empty())
    {
      std::ostringstream os;
      os << "Empty value for key \"" << REQUEST_PAYLOAD_KEY_FILTERPARAM
         << "\" into json request payload. Value is mandatory";
      ACS_THROW(ODataSubscriptionRequestPayloadException(os.str()));
    }

    if (json.find(REQUEST_PAYLOAD_KEY_STAGEORDER) == json.end() || json[REQUEST_PAYLOAD_KEY_STAGEORDER].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << REQUEST_PAYLOAD_KEY_STAGEORDER
         << "\" into json request payload or the associated value is null. Key is mandatory";
      ACS_THROW(ODataSubscriptionRequestPayloadException(os.str()));
    }
    _stageOrder = json[REQUEST_PAYLOAD_KEY_STAGEORDER];

    if (json.find(REQUEST_PAYLOAD_KEY_EVENT) == json.end() || json[REQUEST_PAYLOAD_KEY_EVENT].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << REQUEST_PAYLOAD_KEY_EVENT
         << "\" into json request payload or the associated value is null. Key is mandatory";
      ACS_THROW(ODataSubscriptionRequestPayloadException(os.str()));
    }
    _event = string2SubscriptionEvent(json[REQUEST_PAYLOAD_KEY_EVENT]);
    _eventSet = true;

    if (json.find(REQUEST_PAYLOAD_KEY_NOTIFICATIONENDPOINT) != json.end() &&
        !json[REQUEST_PAYLOAD_KEY_NOTIFICATIONENDPOINT].is_null())
    {
      _notificationEndpoint = json[REQUEST_PAYLOAD_KEY_NOTIFICATIONENDPOINT];
      _notificationEndpointSet = true;
    }
    else
    {
      _notificationEndpoint = "";
      _notificationEndpointSet = false;
    }

    if (json.find(REQUEST_PAYLOAD_KEY_NOTIFICATIONEPUSERNAME) != json.end() &&
        !json[REQUEST_PAYLOAD_KEY_NOTIFICATIONEPUSERNAME].is_null())
    {
      _notificationEpUsername = json[REQUEST_PAYLOAD_KEY_NOTIFICATIONEPUSERNAME];
      _notificationEpUsernameSet = true;
    }
    else
    {
      _notificationEpUsername = "";
      _notificationEpUsernameSet = false;
    }

    if (json.find(REQUEST_PAYLOAD_KEY_NOTIFICATIONEPPASSWORD) != json.end() &&
        !json[REQUEST_PAYLOAD_KEY_NOTIFICATIONEPPASSWORD].is_null())
    {
      _notificationEpPassword = json[REQUEST_PAYLOAD_KEY_NOTIFICATIONEPPASSWORD];
      _notificationEpPasswordSet = true;
    }
    else
    {
      _notificationEpPassword = "";
      _notificationEpPasswordSet = false;
    }

    if (json.find(REQUEST_PAYLOAD_KEY_PRIORITY) != json.end() && !json[REQUEST_PAYLOAD_KEY_PRIORITY].is_null())
    {
      _priority = json[REQUEST_PAYLOAD_KEY_PRIORITY];
      _prioritySet = true;
    }
    else
    {
      _priority = 0;
      _prioritySet = false;
    }
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataSubscriptionRequestPayloadException(e, "Error reading JSON Object."));
  }
}

std::string ODataSubscriptionRequestPayload::toJSON() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  std::string str;
  nlohmann::json js;
  try
  {
    js = toJSONObject();
    str = js.dump();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Request data transformed data into the following json string: \"\n"
                                          << str << "\"");
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataSubscriptionRequestPayloadException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return str;
}

nlohmann::json ODataSubscriptionRequestPayload::toJSONObject() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  nlohmann::json js;
  try
  {
    js[REQUEST_PAYLOAD_KEY_ODATACONTEXT] = _oDataContext;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _oDataContext << "\" for \""
                                                           << REQUEST_PAYLOAD_KEY_ODATACONTEXT << "\" key");

    js[REQUEST_PAYLOAD_KEY_FILTERPARAM] = _filterParam;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _filterParam << "\" for \""
                                                           << REQUEST_PAYLOAD_KEY_FILTERPARAM << "\" key");

    js[REQUEST_PAYLOAD_KEY_STAGEORDER] = _stageOrder;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _stageOrder << "\" for \""
                                                           << REQUEST_PAYLOAD_KEY_STAGEORDER << "\" key");
    js[REQUEST_PAYLOAD_KEY_EVENT] = subscriptionEvent2String(_event);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << subscriptionEvent2String(_event) << "\" for \""
                                                           << REQUEST_PAYLOAD_KEY_EVENT << "\" key");

    if (_notificationEndpointSet)
    {
      js[REQUEST_PAYLOAD_KEY_NOTIFICATIONENDPOINT] = _notificationEndpoint;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _notificationEndpoint << "\" for \""
                                                             << REQUEST_PAYLOAD_KEY_NOTIFICATIONENDPOINT << "\" key");
    }

    if (_notificationEpUsernameSet)
    {
      js[REQUEST_PAYLOAD_KEY_NOTIFICATIONEPUSERNAME] = _notificationEpUsername;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _notificationEpUsername << "\" for \""
                                                             << REQUEST_PAYLOAD_KEY_NOTIFICATIONEPUSERNAME << "\" key");
    }

    if (_notificationEpPasswordSet)
    {
      js[REQUEST_PAYLOAD_KEY_NOTIFICATIONEPPASSWORD] = _notificationEpPassword;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _notificationEpPassword << "\" for \""
                                                             << REQUEST_PAYLOAD_KEY_NOTIFICATIONEPPASSWORD << "\" key");
    }

    if (_prioritySet)
    {
      js[REQUEST_PAYLOAD_KEY_PRIORITY] = _priority;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _priority << "\" for \""
                                                             << REQUEST_PAYLOAD_KEY_PRIORITY << "\" key");
    }
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataSubscriptionRequestPayloadException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

_ACS_END_NAMESPACE
