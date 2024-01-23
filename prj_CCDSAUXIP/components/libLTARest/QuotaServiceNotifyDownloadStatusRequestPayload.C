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

#include <QuotaServiceNotifyDownloadStatusRequestPayload.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(QuotaServiceNotifyDownloadStatusRequestPayload);

const std::string QuotaServiceNotifyDownloadStatusRequestPayload::REQUEST_PAYLOAD_KEY_TOKENUUID = "TokenUUID";
const std::string QuotaServiceNotifyDownloadStatusRequestPayload::REQUEST_PAYLOAD_KEY_FILENAME = "FileName";
const std::string QuotaServiceNotifyDownloadStatusRequestPayload::REQUEST_PAYLOAD_KEY_DATASIZE = "DataSize";
const std::string QuotaServiceNotifyDownloadStatusRequestPayload::REQUEST_PAYLOAD_KEY_COMPLETED = "Completed";

QuotaServiceNotifyDownloadStatusRequestPayload::QuotaServiceNotifyDownloadStatusRequestPayload() :
  _mutex(),
  _tokenUUID(),
  _fileName(),
  _dataSize(0),
  _dataSizeSet(false),
  _completed(false)
{
}

QuotaServiceNotifyDownloadStatusRequestPayload::QuotaServiceNotifyDownloadStatusRequestPayload(
  const std::string& jsstr) :
  _mutex(),
  _tokenUUID(),
  _fileName(),
  _dataSize(0),
  _dataSizeSet(false),
  _completed(false)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

QuotaServiceNotifyDownloadStatusRequestPayload::QuotaServiceNotifyDownloadStatusRequestPayload(
  const nlohmann::json& js) :
  _mutex(),
  _tokenUUID(),
  _fileName(),
  _dataSize(0),
  _dataSizeSet(false),
  _completed(false)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

QuotaServiceNotifyDownloadStatusRequestPayload::QuotaServiceNotifyDownloadStatusRequestPayload(
  const QuotaServiceNotifyDownloadStatusRequestPayload& data) :
  _mutex(),
  _tokenUUID(data._tokenUUID),
  _fileName(data._fileName),
  _dataSize(data._dataSize),
  _dataSizeSet(data._dataSizeSet),
  _completed(data._completed)
{
}

QuotaServiceNotifyDownloadStatusRequestPayload::~QuotaServiceNotifyDownloadStatusRequestPayload() ACS_NOEXCEPT {}

QuotaServiceNotifyDownloadStatusRequestPayload& QuotaServiceNotifyDownloadStatusRequestPayload::operator=(
  const QuotaServiceNotifyDownloadStatusRequestPayload& data)
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
        _tokenUUID = data._tokenUUID;
        _fileName = data._fileName;
        _dataSize = data._dataSize;
        _dataSizeSet = data._dataSizeSet;
        _completed = data._completed;
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

void QuotaServiceNotifyDownloadStatusRequestPayload::fromJSON(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(QuotaServiceNotifyDownloadStatusRequestPayloadException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void QuotaServiceNotifyDownloadStatusRequestPayload::fromJSONObject(const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  try
  {
    if (json.find(REQUEST_PAYLOAD_KEY_TOKENUUID) == json.end() || json[REQUEST_PAYLOAD_KEY_TOKENUUID].is_null())
    {
      ACS_THROW(QuotaServiceNotifyDownloadStatusRequestPayloadException(
        "Cannot find key \"" + REQUEST_PAYLOAD_KEY_TOKENUUID +
        "\" into json request payload or the associated value is null. Key is mandatory"));
    }
    _tokenUUID = json[REQUEST_PAYLOAD_KEY_TOKENUUID];
    if (_tokenUUID.empty())
    {
      ACS_THROW(QuotaServiceNotifyDownloadStatusRequestPayloadException(
        "Empty value for key \"" + REQUEST_PAYLOAD_KEY_TOKENUUID +
        "\" into json request payload. Value is mandatory"));
    }

    if (json.find(REQUEST_PAYLOAD_KEY_FILENAME) == json.end() || json[REQUEST_PAYLOAD_KEY_FILENAME].is_null())
    {
      ACS_THROW(QuotaServiceNotifyDownloadStatusRequestPayloadException(
        "Cannot find key \"" + REQUEST_PAYLOAD_KEY_FILENAME +
        "\" into json request payload or the associated value is null. Key is mandatory"));
    }
    _fileName = json[REQUEST_PAYLOAD_KEY_FILENAME];
    if (_fileName.empty())
    {
      ACS_THROW(QuotaServiceNotifyDownloadStatusRequestPayloadException(
        "Empty value for key \"" + REQUEST_PAYLOAD_KEY_FILENAME +
        "\" into json request payload. Value is mandatory"));
    }

    if (json.find(REQUEST_PAYLOAD_KEY_DATASIZE) == json.end())
    {
      _dataSize = 0;
      _dataSizeSet = false;
    }
    else
    {
      ACS_COND_THROW(json[REQUEST_PAYLOAD_KEY_DATASIZE].is_null(),
                     QuotaServiceNotifyDownloadStatusRequestPayloadException(
                       "Value associated to key \"" + REQUEST_PAYLOAD_KEY_DATASIZE +
                       "\" into json request payload is null"));

      _dataSize = json[REQUEST_PAYLOAD_KEY_DATASIZE];
      _dataSizeSet = true;
    }

    if (json.find(REQUEST_PAYLOAD_KEY_COMPLETED) == json.end() || json[REQUEST_PAYLOAD_KEY_COMPLETED].is_null())
    {
      ACS_THROW(QuotaServiceNotifyDownloadStatusRequestPayloadException(
        "Cannot find key \"" + REQUEST_PAYLOAD_KEY_COMPLETED +
        "\" into json request payload or the associated value is null. Key is mandatory"));
    }
    _completed = json[REQUEST_PAYLOAD_KEY_COMPLETED];
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(QuotaServiceNotifyDownloadStatusRequestPayloadException(e, "Error reading JSON Object."));
  }
}

std::string QuotaServiceNotifyDownloadStatusRequestPayload::toJSON() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  std::string str;
  nlohmann::json js;
  try
  {
    js = toJSONObject();
    str = js.dump();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Transformed request data into the following json string:\n"
                                          << str);
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(QuotaServiceNotifyDownloadStatusRequestPayloadException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return str;
}

nlohmann::json QuotaServiceNotifyDownloadStatusRequestPayload::toJSONObject() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  nlohmann::json js;
  try
  {
    js[REQUEST_PAYLOAD_KEY_TOKENUUID] = _tokenUUID;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _tokenUUID << "\" for key \""
                                                           << REQUEST_PAYLOAD_KEY_TOKENUUID << "\"");

    js[REQUEST_PAYLOAD_KEY_FILENAME] = _fileName;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _fileName << "\" for key \""
                                                           << REQUEST_PAYLOAD_KEY_FILENAME << "\"");

    if (_dataSizeSet)
    {
      js[REQUEST_PAYLOAD_KEY_DATASIZE] = _dataSize;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _dataSize << "\" for key \""
                                                             << REQUEST_PAYLOAD_KEY_DATASIZE << "\"");
    }

    js[REQUEST_PAYLOAD_KEY_COMPLETED] = _completed;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _completed << "\" for key \""
                                                           << REQUEST_PAYLOAD_KEY_COMPLETED << "\"");
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(QuotaServiceNotifyDownloadStatusRequestPayloadException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

_ACS_END_NAMESPACE
