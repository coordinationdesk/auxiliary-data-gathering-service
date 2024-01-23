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

#include <ODataSubscriptionResponsePayload.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace lta;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ODataSubscriptionResponsePayload);

const std::string ODataSubscriptionResponsePayload::RESPONSE_PAYLOAD_KEY_ODATACONTEXT = "@odata.context";
const std::string ODataSubscriptionResponsePayload::RESPONSE_PAYLOAD_KEY_ID = "Id";
const std::string ODataSubscriptionResponsePayload::RESPONSE_PAYLOAD_KEY_STATUS = "Status";
const std::string ODataSubscriptionResponsePayload::RESPONSE_PAYLOAD_KEY_FILTERPARAM = "FilterParam";
const std::string ODataSubscriptionResponsePayload::RESPONSE_PAYLOAD_KEY_STAGEORDER = "StageOrder";
const std::string ODataSubscriptionResponsePayload::RESPONSE_PAYLOAD_KEY_NOTIFICATIONENDPOINT = "NotificationEndpoint";
const std::string ODataSubscriptionResponsePayload::RESPONSE_PAYLOAD_KEY_NOTIFICATIONEPUSERNAME =
  "NotificationEpUsername";
const std::string ODataSubscriptionResponsePayload::RESPONSE_PAYLOAD_KEY_NOTIFICATIONEPPASSWORD =
  "NotificationEpPassword";
const std::string ODataSubscriptionResponsePayload::RESPONSE_PAYLOAD_KEY_PRIORITY = "Priority";

const std::string ODataSubscriptionResponsePayload::RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT =
  "$metadata#OData.CSC.Subscription";

const std::map<ODataSubscriptionResponsePayload::SubscriptionStatus, std::string>
  ODataSubscriptionResponsePayload::SUBSCRIPTIONSTATUS_TO_STRING = {
    { RUNNING, "running" },
    { PAUSED, "paused" },
    { CANCELLED, "cancelled" }
  };
const std::map<std::string, ODataSubscriptionResponsePayload::SubscriptionStatus>
  ODataSubscriptionResponsePayload::STRING_TO_SUBSCRIPTIONSTATUS = {
    { "running", RUNNING },
    { "paused", PAUSED },
    { "cancelled", CANCELLED }
  };

ODataSubscriptionResponsePayload::ODataSubscriptionResponsePayload() :
  _mutex(),
  _oDataContext(RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT),
  _id(),
  _status(INVALID_STATUS),
  _filterParam(),
  _stageOrder(false),
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
}

ODataSubscriptionResponsePayload::ODataSubscriptionResponsePayload(const std::string& jsstr) :
  _mutex(),
  _oDataContext(RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT),
  _id(),
  _status(INVALID_STATUS),
  _filterParam(),
  _stageOrder(false),
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

ODataSubscriptionResponsePayload::ODataSubscriptionResponsePayload(const nlohmann::json& js) :
  _mutex(),
  _oDataContext(RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT),
  _id(),
  _status(INVALID_STATUS),
  _filterParam(),
  _stageOrder(false),
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

ODataSubscriptionResponsePayload::ODataSubscriptionResponsePayload(
  const ODataSubscriptionResponsePayload& data) :
  _mutex(),
  _oDataContext(data._oDataContext),
  _id(data._id),
  _status(data._status),
  _filterParam(data._filterParam),
  _stageOrder(data._stageOrder),
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

ODataSubscriptionResponsePayload::~ODataSubscriptionResponsePayload() ACS_NOEXCEPT
{
}

ODataSubscriptionResponsePayload& ODataSubscriptionResponsePayload::operator=(
  const ODataSubscriptionResponsePayload& data)
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
        _id = data._id;
        _status = data._status;
        _filterParam = data._filterParam;
        _stageOrder = data._stageOrder;
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

void ODataSubscriptionResponsePayload::fromJSON(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataSubscriptionResponsePayloadException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void ODataSubscriptionResponsePayload::fromJSONObject(const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);

  try
  {
    if (json.find(RESPONSE_PAYLOAD_KEY_ODATACONTEXT) == json.end() || json[RESPONSE_PAYLOAD_KEY_ODATACONTEXT].is_null())
    {
      ACS_LOG_WARNING("Cannot find key \"" << RESPONSE_PAYLOAD_KEY_ODATACONTEXT
                                           << "\" into json response payload or the associated value is null.");
      _oDataContext = RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT;
    }
    else
    {
      _oDataContext = json[RESPONSE_PAYLOAD_KEY_ODATACONTEXT];
    }
    if (_oDataContext != RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT)
    {
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Value \"" << _oDataContext << "\" is not what is expected (\""
                                 << RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT << "\") for \""
                                 << RESPONSE_PAYLOAD_KEY_ODATACONTEXT << "\" key into json response payload ")
    }

    if (json.find(RESPONSE_PAYLOAD_KEY_ID) == json.end() || json[RESPONSE_PAYLOAD_KEY_ID].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_ID
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(ODataSubscriptionResponsePayloadException(os.str()));
    }
    _id = json[RESPONSE_PAYLOAD_KEY_ID];
    if (_id.empty())
    {
      std::ostringstream os;
      os << "Empty value for key \"" << RESPONSE_PAYLOAD_KEY_ID << "\" into json response payload. Value is mandatory";
      ACS_THROW(ODataSubscriptionResponsePayloadException(os.str()));
    }

    if (json.find(RESPONSE_PAYLOAD_KEY_STATUS) == json.end() || json[RESPONSE_PAYLOAD_KEY_STATUS].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(ODataSubscriptionResponsePayloadException(os.str()));
    }
    std::string statusStr = json[RESPONSE_PAYLOAD_KEY_STATUS];
    if (statusStr.empty())
    {
      std::ostringstream os;
      os << "Empty value for key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload. Value is mandatory";
      ACS_THROW(ODataSubscriptionResponsePayloadException(os.str()));
    }
    std::map<std::string, SubscriptionStatus>::const_iterator it = STRING_TO_SUBSCRIPTIONSTATUS.find(statusStr);
    if (it == STRING_TO_SUBSCRIPTIONSTATUS.end())
    {
      _status = INVALID_STATUS;
      std::ostringstream os;
      os << "Invalid value for key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload. Value is mandatory";
      ACS_THROW(ODataSubscriptionResponsePayloadException(os.str()));
    }
    _status = it->second;

    if (json.find(RESPONSE_PAYLOAD_KEY_FILTERPARAM) == json.end() || json[RESPONSE_PAYLOAD_KEY_FILTERPARAM].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_FILTERPARAM
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(ODataSubscriptionResponsePayloadException(os.str()));
    }
    _filterParam = json[RESPONSE_PAYLOAD_KEY_FILTERPARAM];
    if (_filterParam.empty())
    {
      std::ostringstream os;
      os << "Empty value for key \"" << RESPONSE_PAYLOAD_KEY_FILTERPARAM
         << "\" into json response payload. Value is mandatory";
      ACS_THROW(ODataSubscriptionResponsePayloadException(os.str()));
    }

    if (json.find(RESPONSE_PAYLOAD_KEY_STAGEORDER) == json.end() || json[RESPONSE_PAYLOAD_KEY_STAGEORDER].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_STAGEORDER
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(ODataSubscriptionResponsePayloadException(os.str()));
    }
    _stageOrder = json[RESPONSE_PAYLOAD_KEY_STAGEORDER];

    if (json.find(RESPONSE_PAYLOAD_KEY_NOTIFICATIONENDPOINT) != json.end() &&
        !json[RESPONSE_PAYLOAD_KEY_NOTIFICATIONENDPOINT].is_null())
    {
      _notificationEndpoint = json[RESPONSE_PAYLOAD_KEY_NOTIFICATIONENDPOINT];
      _notificationEndpointSet = true;
    }
    else
    {
      _notificationEndpoint = "";
      _notificationEndpointSet = false;
    }

    if (json.find(RESPONSE_PAYLOAD_KEY_NOTIFICATIONEPUSERNAME) != json.end() &&
        !json[RESPONSE_PAYLOAD_KEY_NOTIFICATIONEPUSERNAME].is_null())
    {
      _notificationEpUsername = json[RESPONSE_PAYLOAD_KEY_NOTIFICATIONEPUSERNAME];
      _notificationEpUsernameSet = true;
    }
    else
    {
      _notificationEpUsername = "";
      _notificationEpUsernameSet = false;
    }

    if (json.find(RESPONSE_PAYLOAD_KEY_NOTIFICATIONEPPASSWORD) != json.end() &&
        !json[RESPONSE_PAYLOAD_KEY_NOTIFICATIONEPPASSWORD].is_null())
    {
      _notificationEpPassword = json[RESPONSE_PAYLOAD_KEY_NOTIFICATIONEPPASSWORD];
      _notificationEpPasswordSet = true;
    }
    else
    {
      _notificationEpPassword = "";
      _notificationEpPasswordSet = false;
    }

    if (json.find(RESPONSE_PAYLOAD_KEY_PRIORITY) != json.end() && !json[RESPONSE_PAYLOAD_KEY_PRIORITY].is_null())
    {
      _priority = json[RESPONSE_PAYLOAD_KEY_PRIORITY];
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
    ACS_THROW(ODataSubscriptionResponsePayloadException(e, "Error reading JSON Object."));
  }
}

std::string ODataSubscriptionResponsePayload::toJSON() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  std::string str;
  nlohmann::json js;
  try
  {
    js = toJSONObject();
    str = js.dump();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Response data transformed data into the following json string: \"\n"
                                          << str << "\"");
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataSubscriptionResponsePayloadException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP

  return str;
}

nlohmann::json ODataSubscriptionResponsePayload::toJSONObject() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  nlohmann::json js;
  try
  {
    // Add members to json object
    js[RESPONSE_PAYLOAD_KEY_ODATACONTEXT] = _oDataContext;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _oDataContext << "\" for \""
                                                           << RESPONSE_PAYLOAD_KEY_ODATACONTEXT << "\" key");

    js[RESPONSE_PAYLOAD_KEY_ID] = _id;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _id << "\" for \"" << RESPONSE_PAYLOAD_KEY_ID
                                                           << "\" key");

    if (INVALID_STATUS == _status)
    {
      std::ostringstream os;
      os << "Invalid value for key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload. Value is mandatory";
      ACS_THROW(ODataSubscriptionResponsePayloadException(os.str()));
    }
    std::string statusStr = SUBSCRIPTIONSTATUS_TO_STRING.at(_status);
    js[RESPONSE_PAYLOAD_KEY_STATUS] = statusStr;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << statusStr << "\" for \"" << RESPONSE_PAYLOAD_KEY_STATUS
                                                           << "\" key");

    js[RESPONSE_PAYLOAD_KEY_FILTERPARAM] = _filterParam;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _filterParam << "\" for \""
                                                           << RESPONSE_PAYLOAD_KEY_FILTERPARAM << "\" key");

    js[RESPONSE_PAYLOAD_KEY_STAGEORDER] = _stageOrder;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _stageOrder << "\" for \""
                                                           << RESPONSE_PAYLOAD_KEY_STAGEORDER << "\" key");

    if (_notificationEndpointSet)
    {
      js[RESPONSE_PAYLOAD_KEY_NOTIFICATIONENDPOINT] = _notificationEndpoint;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _notificationEndpoint << "\" for \""
                                                             << RESPONSE_PAYLOAD_KEY_NOTIFICATIONENDPOINT << "\" key");
    }

    if (_notificationEpUsernameSet)
    {
      js[RESPONSE_PAYLOAD_KEY_NOTIFICATIONEPUSERNAME] = _notificationEpUsername;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \""
                                            << _notificationEpUsername << "\" for \""
                                            << RESPONSE_PAYLOAD_KEY_NOTIFICATIONEPUSERNAME << "\" key");
    }

    if (_notificationEpPasswordSet)
    {
      js[RESPONSE_PAYLOAD_KEY_NOTIFICATIONEPPASSWORD] = _notificationEpPassword;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \""
                                            << _notificationEpPassword << "\" for \""
                                            << RESPONSE_PAYLOAD_KEY_NOTIFICATIONEPPASSWORD << "\" key");
    }

    if (_prioritySet)
    {
      js[RESPONSE_PAYLOAD_KEY_PRIORITY] = _priority;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _priority << "\" for \""
                                                             << RESPONSE_PAYLOAD_KEY_PRIORITY << "\" key");
    }
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataSubscriptionResponsePayloadException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

_ACS_END_NAMESPACE
