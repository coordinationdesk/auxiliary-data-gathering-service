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

#include <ODataOrderRequestPayload.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(ODataOrderRequestPayload);

const std::string ODataOrderRequestPayload::REQUEST_PAYLOAD_KEY_NOTIFICATIONENDPOINT = "NotificationEndpoint";
const std::string ODataOrderRequestPayload::REQUEST_PAYLOAD_KEY_NOTIFICATIONEPUSERNAME = "NotificationEpUsername";
const std::string ODataOrderRequestPayload::REQUEST_PAYLOAD_KEY_NOTIFICATIONEPPASSWORD = "NotificationEpPassword";
const std::string ODataOrderRequestPayload::REQUEST_PAYLOAD_KEY_PRIORITY = "Priority";

ODataOrderRequestPayload::ODataOrderRequestPayload() :
  _mutex(),
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

ODataOrderRequestPayload::ODataOrderRequestPayload(const std::string& jsstr) :
  _mutex(),
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

ODataOrderRequestPayload::ODataOrderRequestPayload(const nlohmann::json& js) :
  _mutex(),
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

ODataOrderRequestPayload::ODataOrderRequestPayload(
  const ODataOrderRequestPayload& data) :
  _mutex(),
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

ODataOrderRequestPayload::~ODataOrderRequestPayload() ACS_NOEXCEPT
{
}

ODataOrderRequestPayload& ODataOrderRequestPayload::operator=(
  const ODataOrderRequestPayload& data)
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

void ODataOrderRequestPayload::fromJSON(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataOrderRequestPayloadException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void ODataOrderRequestPayload::fromJSONObject(const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  try
  {
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
    ACS_THROW(ODataOrderRequestPayloadException(e, "Error reading JSON Object."));
  }
}

std::string ODataOrderRequestPayload::toJSON() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Prevents generating an empty JSON object
  if (!_notificationEndpointSet && !_notificationEpUsernameSet && !_notificationEpPasswordSet && !_prioritySet)
  {
    return "";
  }
  
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
    ACS_THROW(ODataOrderRequestPayloadException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return str;
}

nlohmann::json ODataOrderRequestPayload::toJSONObject() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  nlohmann::json js;
  try
  {
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
    ACS_THROW(ODataOrderRequestPayloadException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

_ACS_END_NAMESPACE
