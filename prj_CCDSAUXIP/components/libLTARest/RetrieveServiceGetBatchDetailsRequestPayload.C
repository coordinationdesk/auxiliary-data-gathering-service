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

#include <RetrieveServiceGetBatchDetailsRequestPayload.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(RetrieveServiceGetBatchDetailsRequestPayload);

const std::string RetrieveServiceGetBatchDetailsRequestPayload::RESPONSE_PAYLOAD_KEY_BATCHID = "batch_id";

RetrieveServiceGetBatchDetailsRequestPayload::RetrieveServiceGetBatchDetailsRequestPayload() :
  _mutex(),
  _batchID(0)
{
}

RetrieveServiceGetBatchDetailsRequestPayload::RetrieveServiceGetBatchDetailsRequestPayload(const std::string& jsstr) :
  _mutex(),
  _batchID(0)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

RetrieveServiceGetBatchDetailsRequestPayload::RetrieveServiceGetBatchDetailsRequestPayload(const nlohmann::json& js) :
  _mutex(),
  _batchID(0)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

RetrieveServiceGetBatchDetailsRequestPayload::RetrieveServiceGetBatchDetailsRequestPayload(
  const RetrieveServiceGetBatchDetailsRequestPayload& data) :
  _mutex(),
  _batchID(data._batchID)
{
}

RetrieveServiceGetBatchDetailsRequestPayload::~RetrieveServiceGetBatchDetailsRequestPayload() ACS_NOEXCEPT
{
}

RetrieveServiceGetBatchDetailsRequestPayload& RetrieveServiceGetBatchDetailsRequestPayload::operator=(
  const RetrieveServiceGetBatchDetailsRequestPayload& data)
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
        _batchID = data._batchID;
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

void RetrieveServiceGetBatchDetailsRequestPayload::fromJSON(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(RetrieveServiceGetBatchDetailsRequestPayloadException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void RetrieveServiceGetBatchDetailsRequestPayload::fromJSONObject(const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  try
  {
    if (json.find(RESPONSE_PAYLOAD_KEY_BATCHID) == json.end() || json[RESPONSE_PAYLOAD_KEY_BATCHID].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_BATCHID
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(RetrieveServiceGetBatchDetailsRequestPayloadException(os.str()));
    }
    _batchID = json[RESPONSE_PAYLOAD_KEY_BATCHID];
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(RetrieveServiceGetBatchDetailsRequestPayloadException(e, "Error reading JSON Object."));
  }
}

std::string RetrieveServiceGetBatchDetailsRequestPayload::toJSON() const
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
    ACS_THROW(RetrieveServiceGetBatchDetailsRequestPayloadException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return str;
}

nlohmann::json RetrieveServiceGetBatchDetailsRequestPayload::toJSONObject() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  nlohmann::json js;
  try
  {
    js[RESPONSE_PAYLOAD_KEY_BATCHID] = _batchID;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _batchID << "\" for \""
                                                           << RESPONSE_PAYLOAD_KEY_BATCHID << "\" key");
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(RetrieveServiceGetBatchDetailsRequestPayloadException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

_ACS_END_NAMESPACE
