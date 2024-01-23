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

#include <RetrieveServiceGetBulkStatusRequestPayload.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(RetrieveServiceGetBulkStatusRequestPayload);

const std::string RetrieveServiceGetBulkStatusRequestPayload::RESPONSE_PAYLOAD_KEY_BULKID = "bulk_id";

RetrieveServiceGetBulkStatusRequestPayload::RetrieveServiceGetBulkStatusRequestPayload() :
  _mutex(),
  _bulkID(0)
{
}

RetrieveServiceGetBulkStatusRequestPayload::RetrieveServiceGetBulkStatusRequestPayload(const std::string& jsstr) :
  _mutex(),
  _bulkID(0)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

RetrieveServiceGetBulkStatusRequestPayload::RetrieveServiceGetBulkStatusRequestPayload(const nlohmann::json& js) :
  _mutex(),
  _bulkID(0)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

RetrieveServiceGetBulkStatusRequestPayload::RetrieveServiceGetBulkStatusRequestPayload(
  const RetrieveServiceGetBulkStatusRequestPayload& data) :
  _mutex(),
  _bulkID(data._bulkID)
{
}

RetrieveServiceGetBulkStatusRequestPayload::~RetrieveServiceGetBulkStatusRequestPayload() ACS_NOEXCEPT
{
}

RetrieveServiceGetBulkStatusRequestPayload& RetrieveServiceGetBulkStatusRequestPayload::operator=(
  const RetrieveServiceGetBulkStatusRequestPayload& data)
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
        _bulkID = data._bulkID;
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

void RetrieveServiceGetBulkStatusRequestPayload::fromJSON(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(RetrieveServiceGetBulkStatusRequestPayloadException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void RetrieveServiceGetBulkStatusRequestPayload::fromJSONObject(const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  try
  {
    if (json.find(RESPONSE_PAYLOAD_KEY_BULKID) == json.end() || json[RESPONSE_PAYLOAD_KEY_BULKID].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_BULKID
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(RetrieveServiceGetBulkStatusRequestPayloadException(os.str()));
    }
    _bulkID = json[RESPONSE_PAYLOAD_KEY_BULKID];
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(RetrieveServiceGetBulkStatusRequestPayloadException(e, "Error reading JSON Object."));
  }
}

std::string RetrieveServiceGetBulkStatusRequestPayload::toJSON() const
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
    ACS_THROW(RetrieveServiceGetBulkStatusRequestPayloadException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return str;
}

nlohmann::json RetrieveServiceGetBulkStatusRequestPayload::toJSONObject() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  nlohmann::json js;
  try
  {
    js[RESPONSE_PAYLOAD_KEY_BULKID] = _bulkID;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _bulkID << "\" for \"" << RESPONSE_PAYLOAD_KEY_BULKID
                                                           << "\" key");
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(RetrieveServiceGetBulkStatusRequestPayloadException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

_ACS_END_NAMESPACE
