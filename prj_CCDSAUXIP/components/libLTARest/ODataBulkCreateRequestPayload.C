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

#include <ODataBulkCreateRequestPayload.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(ODataBulkCreateRequestPayload);

const std::string ODataBulkCreateRequestPayload::REQUEST_PAYLOAD_KEY_ODATACONTEXT = "@odata.context";
const std::string ODataBulkCreateRequestPayload::REQUEST_PAYLOAD_KEY_FILTER = "FilterParam";
const std::string ODataBulkCreateRequestPayload::REQUEST_PAYLOAD_KEY_BATCHSIZEPRODUCTS = "BatchsizeProducts";
const std::string ODataBulkCreateRequestPayload::REQUEST_PAYLOAD_KEY_BATCHSIZEVOLUME = "BatchsizeVolume";
const std::string ODataBulkCreateRequestPayload::REQUEST_PAYLOAD_KEY_NOTIFICATIONENDPOINT = "NotificationEndpoint";
const std::string ODataBulkCreateRequestPayload::REQUEST_PAYLOAD_KEY_NOTIFICATIONEPUSERNAME = "NotificationEpUsername";
const std::string ODataBulkCreateRequestPayload::REQUEST_PAYLOAD_KEY_NOTIFICATIONEPPASSWORD = "NotificationEpPassword";
const std::string ODataBulkCreateRequestPayload::REQUEST_PAYLOAD_KEY_PRIORITY = "Priority";

const std::string ODataBulkCreateRequestPayload::REQUEST_PAYLOAD_DEFAULT_ODATACONTEXT = "$metadata#Bulk/$entity";

ODataBulkCreateRequestPayload::ODataBulkCreateRequestPayload() :
  _mutex(),
  _oDataContext(REQUEST_PAYLOAD_DEFAULT_ODATACONTEXT),
  _filter(),
  _batchsizeProductsSet(false),
  _batchsizeProducts(0),
  _batchsizeVolumeSet(false),
  _batchsizeVolume(0),
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

ODataBulkCreateRequestPayload::ODataBulkCreateRequestPayload(const std::string& jsstr) :
  _mutex(),
  _oDataContext(REQUEST_PAYLOAD_DEFAULT_ODATACONTEXT),
  _filter(),
  _batchsizeProductsSet(false),
  _batchsizeProducts(0),
  _batchsizeVolumeSet(false),
  _batchsizeVolume(0),
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

ODataBulkCreateRequestPayload::ODataBulkCreateRequestPayload(const nlohmann::json& js) :
  _mutex(),
  _oDataContext(REQUEST_PAYLOAD_DEFAULT_ODATACONTEXT),
  _filter(),
  _batchsizeProductsSet(false),
  _batchsizeProducts(0),
  _batchsizeVolumeSet(false),
  _batchsizeVolume(0),
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

ODataBulkCreateRequestPayload::ODataBulkCreateRequestPayload(
  const ODataBulkCreateRequestPayload& data) :
  _mutex(),
  _oDataContext(data._oDataContext),
  _filter(data._filter),
  _batchsizeProductsSet(data._batchsizeProductsSet),
  _batchsizeProducts(data._batchsizeProducts),
  _batchsizeVolumeSet(data._batchsizeVolumeSet),
  _batchsizeVolume(data._batchsizeVolume),
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

ODataBulkCreateRequestPayload::~ODataBulkCreateRequestPayload() ACS_NOEXCEPT
{
}

ODataBulkCreateRequestPayload& ODataBulkCreateRequestPayload::operator=(
  const ODataBulkCreateRequestPayload& data)
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
        _filter = data._filter;
        _batchsizeProductsSet = data._batchsizeProductsSet;
        _batchsizeProducts = data._batchsizeProducts;
        _batchsizeVolumeSet = data._batchsizeVolumeSet;
        _batchsizeVolume = data._batchsizeVolume;
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

void ODataBulkCreateRequestPayload::fromJSON(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataBulkCreateRequestPayloadException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void ODataBulkCreateRequestPayload::fromJSONObject(const nlohmann::json& json)
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

    if (json.find(REQUEST_PAYLOAD_KEY_FILTER) == json.end() ||
        json[REQUEST_PAYLOAD_KEY_FILTER].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << REQUEST_PAYLOAD_KEY_FILTER
         << "\" into json request payload or the associated value is null. Key is mandatory";
      ACS_THROW(ODataBulkCreateRequestPayloadException(os.str()));
    }
    _filter = json[REQUEST_PAYLOAD_KEY_FILTER];
    if (_filter.empty())
    {
      std::ostringstream os;
      os << "Empty value for key \"" << REQUEST_PAYLOAD_KEY_FILTER
         << "\" into json request payload. Value is mandatory";
      ACS_THROW(ODataBulkCreateRequestPayloadException(os.str()));
    }

    if (json.find(REQUEST_PAYLOAD_KEY_BATCHSIZEPRODUCTS) != json.end() &&
        !json[REQUEST_PAYLOAD_KEY_BATCHSIZEPRODUCTS].is_null())
    {
      _batchsizeProducts = json[REQUEST_PAYLOAD_KEY_BATCHSIZEPRODUCTS];
      _batchsizeProductsSet = true;
    }
    else
    {
      _batchsizeProducts = 0;
      _batchsizeProductsSet = false;
    }

    if (json.find(REQUEST_PAYLOAD_KEY_BATCHSIZEVOLUME) != json.end() &&
        !json[REQUEST_PAYLOAD_KEY_BATCHSIZEVOLUME].is_null())
    {
      _batchsizeVolume = json[REQUEST_PAYLOAD_KEY_BATCHSIZEVOLUME];
      _batchsizeVolumeSet = true;
    }
    else
    {
      _batchsizeVolume = 0;
      _batchsizeVolumeSet = false;
    }

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
    ACS_THROW(ODataBulkCreateRequestPayloadException(e, "Error reading JSON Object."));
  }
}

std::string ODataBulkCreateRequestPayload::toJSON() const
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
    ACS_THROW(ODataBulkCreateRequestPayloadException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return str;
}

nlohmann::json ODataBulkCreateRequestPayload::toJSONObject() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  nlohmann::json js;
  try
  {
    // Add members to json object
    js[REQUEST_PAYLOAD_KEY_ODATACONTEXT] = _oDataContext;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _oDataContext << "\" for \""
                                                           << REQUEST_PAYLOAD_KEY_ODATACONTEXT << "\" key");

    js[REQUEST_PAYLOAD_KEY_FILTER] = _filter;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _filter << "\" for \""
                                                           << REQUEST_PAYLOAD_KEY_FILTER << "\" key");

    if (_batchsizeProductsSet)
    {
      js[REQUEST_PAYLOAD_KEY_BATCHSIZEPRODUCTS] = _batchsizeProducts;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _batchsizeProducts << "\" for \""
                                                             << REQUEST_PAYLOAD_KEY_BATCHSIZEPRODUCTS << "\" key");
    }

    if (_batchsizeVolumeSet)
    {
      js[REQUEST_PAYLOAD_KEY_BATCHSIZEVOLUME] = _batchsizeVolume;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _batchsizeVolume << "\" for \""
                                                             << REQUEST_PAYLOAD_KEY_BATCHSIZEVOLUME << "\" key");
    }

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
    ACS_THROW(ODataBulkCreateRequestPayloadException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

_ACS_END_NAMESPACE
