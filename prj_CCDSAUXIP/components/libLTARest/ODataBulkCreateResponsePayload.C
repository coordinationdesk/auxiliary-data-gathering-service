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

#include <ODataBulkCreateResponsePayload.h>

_ACS_BEGIN_NAMESPACE(acs)

/***********
 * ELEMENT *
 **********/

ACS_CLASS_DEFINE_DEBUG_LEVEL(ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement);

const std::string ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement::RESPONSE_PAYLOAD_KEY_ID =
  "Id";
const std::string ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement::
  RESPONSE_PAYLOAD_KEY_STATUS = "Status";

const std::map<ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement::Status, std::string>
  ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement::STATUSTOSTRING = {
    { CREATED, "created" },
    { INPROGRESS, "in_progress" },
    { COMPLETED, "completed" },
    { FAILED, "failed" },
    { CANCELLED, "cancelled" },
  };
const std::map<std::string, ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement::Status>
  ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement::STRINGTOSTATUS = {
    { "created", CREATED },
    { "in_progress", INPROGRESS },
    { "completed", COMPLETED },
    { "failed", FAILED },
    { "cancelled", CANCELLED },
  };

ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement::BulkCreateValueElement() :
  _mutex(),
  _id(),
  _status(INVALIDSTATUS)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement::BulkCreateValueElement(
  const std::string& jsstr) :
  _mutex(),
  _id(),
  _status(INVALIDSTATUS)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement::BulkCreateValueElement(
  const nlohmann::json& js) :
  _mutex(),
  _id(),
  _status(INVALIDSTATUS)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement::BulkCreateValueElement(
  const ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement& data) :
  _mutex(),
  _id(data._id),
  _status(data._status)
{
}

ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement::~BulkCreateValueElement() ACS_NOEXCEPT
{
}

ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement&
ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement::operator=(
  const ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement& data)
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
        _id = data._id;
        _status = data._status;
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

void ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement::fromJSON(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BulkCreateValueElementException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement::fromJSONObject(
  const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);

  try
  {
    if (json.find(RESPONSE_PAYLOAD_KEY_ID) == json.end() || json[RESPONSE_PAYLOAD_KEY_ID].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_ID
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(BulkCreateValueElementException(os.str()));
    }
    _id = json[RESPONSE_PAYLOAD_KEY_ID];
    if (_id.empty())
    {
      std::ostringstream os;
      os << "Empty value for key \"" << RESPONSE_PAYLOAD_KEY_ID << "\" into json response payload. Value is mandatory";
      ACS_THROW(BulkCreateValueElementException(os.str()));
    }

    if (json.find(RESPONSE_PAYLOAD_KEY_STATUS) == json.end() || json[RESPONSE_PAYLOAD_KEY_STATUS].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(BulkCreateValueElementException(os.str()));
    }
    std::string statusStr = json[RESPONSE_PAYLOAD_KEY_STATUS];
    if (statusStr.empty())
    {
      std::ostringstream os;
      os << "Empty value for key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload. Value is mandatory";
      ACS_THROW(BulkCreateValueElementException(os.str()));
    }
    std::map<std::string, Status>::const_iterator it = STRINGTOSTATUS.find(statusStr);
    if (it == STRINGTOSTATUS.end())
    {
      _status = INVALIDSTATUS;
      std::ostringstream os;
      os << "Invalid value for key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload. Value is mandatory";
      ACS_THROW(BulkCreateValueElementException(os.str()));
    }
    _status = it->second;
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BulkCreateValueElementException(e, "Error reading JSON Object."));
  }
}

std::string ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement::toJSON() const
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
    ACS_THROW(BulkCreateValueElementException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP

  return str;
}

nlohmann::json ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement::toJSONObject() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  nlohmann::json js;
  try
  {
    // Add members to json object
    if (_id.empty())
    {
      std::ostringstream os;
      os << "Empty value for \"" << RESPONSE_PAYLOAD_KEY_ID << "\" key";
      ACS_THROW(BulkCreateValueElementException(os.str()));
    }
    else
    {
      js[RESPONSE_PAYLOAD_KEY_ID] = _id;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _id << "\" for \""
                                                             << RESPONSE_PAYLOAD_KEY_ID << "\" key");
    }

    if (INVALIDSTATUS == _status)
    {
      std::ostringstream os;
      os << "Invalid value for key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload. Value is mandatory";
      ACS_THROW(BulkCreateValueElementException(os.str()));
    }
    std::string statusStr = STATUSTOSTRING.at(_status);
    js[RESPONSE_PAYLOAD_KEY_STATUS] = statusStr;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << statusStr << "\" for \"" << RESPONSE_PAYLOAD_KEY_STATUS
                                                           << "\" key");
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BulkCreateValueElementException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

/***********
 *  LIST   *
 **********/

namespace
{
bool compareFunction(const ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement& a,
                     const ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement& b)
{
  return (a.getID() < b.getID());
}
}  // namespace

ACS_CLASS_DEFINE_DEBUG_LEVEL(ODataBulkCreateResponsePayload::BulkCreateValueList);

ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueList() :
  _mutex(),
  _value()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueList(const std::string& jsstr) :
  _mutex(),
  _value()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueList(const nlohmann::json& js) :
  _mutex(),
  _value()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueList(const BulkCreateValueList& list) :
  _mutex(),
  _value(list._value)
{
}

ODataBulkCreateResponsePayload::BulkCreateValueList::~BulkCreateValueList() ACS_NOEXCEPT {}

const ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement&
ODataBulkCreateResponsePayload::BulkCreateValueList::operator[](size_t idx) const
{
  Lock l(_mutex);
  if (idx >= _value.size())
  {
    std::ostringstream ss;
    ss << "Invalid index " << idx << ". Size is " << _value.size();
    ACS_THROW(exBoundsException(ss.str()));
  }

  return _value[idx];
}

ODataBulkCreateResponsePayload::BulkCreateValueList::BulkCreateValueElement&
ODataBulkCreateResponsePayload::BulkCreateValueList::operator[](size_t idx)
{
  const BulkCreateValueList& me = (*this);
  const BulkCreateValueElement& e = me[idx];      // calls const version
  return const_cast<BulkCreateValueElement&>(e);  // cast away constness
}

void ODataBulkCreateResponsePayload::BulkCreateValueList::add(const BulkCreateValueElement& payload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  _value.push_back(payload);
}

void ODataBulkCreateResponsePayload::BulkCreateValueList::add(const BulkCreateValueList& payload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);
  add(payload.getList());
}

void ODataBulkCreateResponsePayload::BulkCreateValueList::add(
  const std::vector<BulkCreateValueElement>& payload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);
  _value.insert(_value.end(), payload.begin(), payload.end());
}

void ODataBulkCreateResponsePayload::BulkCreateValueList::sort()
{
  Lock l(_mutex);
  std::sort(_value.begin(), _value.end(), compareFunction);
}

void ODataBulkCreateResponsePayload::BulkCreateValueList::sort(std::vector<BulkCreateValueElement>& out)
  const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  out = _value;
  std::sort(out.begin(), out.end(), compareFunction);
}

void ODataBulkCreateResponsePayload::BulkCreateValueList::clear()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  _value.clear();
}

size_t ODataBulkCreateResponsePayload::BulkCreateValueList::size() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  return _value.size();
}

void ODataBulkCreateResponsePayload::BulkCreateValueList::fromJSON(const std::string& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(json);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BulkCreateValueListException(e, "Error reading JSON."));
  }

  fromJSONObject(js);
}

void ODataBulkCreateResponsePayload::BulkCreateValueList::fromJSONObject(const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  try
  {
    _value.clear();

    if (json.is_null() || !json.is_array())
    {
      ACS_THROW(BulkCreateValueListException("Json response payload is not an array or the associated value is null"));
    }

    size_t listSize = json.size();
    for (size_t idx = 0; idx < listSize; ++idx)
    {
      BulkCreateValueElement element(json[idx]);
      add(element);
    }
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the is_null and is_array checks, other check are in
  // BulkCreateValueElement fromJSONObject functions
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BulkCreateValueListException(e, "Error reading JSON Object."));
  }
  // LCOV_EXCL_STOP
}

std::string ODataBulkCreateResponsePayload::BulkCreateValueList::toJSON() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  std::string jsdumped;
  nlohmann::json js;
  try
  {
    js = toJSONObject();
    jsdumped = js.dump();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generated the following json: " << jsdumped)
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BulkCreateValueListException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return jsdumped;
}

nlohmann::json ODataBulkCreateResponsePayload::BulkCreateValueList::toJSONObject() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  nlohmann::json js;
  try
  {
    js = nlohmann::json::array();  // create the empty array

    size_t rqSize = _value.size();
    for (size_t idx = 0; idx < rqSize; ++idx)
    {
      js.push_back(_value[idx].toJSONObject());
    }
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BulkCreateValueListException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

/***********
 * PAYLOAD *
 **********/

ACS_CLASS_DEFINE_DEBUG_LEVEL(ODataBulkCreateResponsePayload);

const std::string ODataBulkCreateResponsePayload::RESPONSE_PAYLOAD_KEY_ODATACONTEXT = "@odata.context";
const std::string ODataBulkCreateResponsePayload::RESPONSE_PAYLOAD_KEY_VALUE = "value";
const std::string ODataBulkCreateResponsePayload::RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT =
  "$metadata#OData.CSC.Bulk";

ODataBulkCreateResponsePayload::ODataBulkCreateResponsePayload() :
  _mutex(),
  _oDataContext(RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT),
  _value()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

ODataBulkCreateResponsePayload::ODataBulkCreateResponsePayload(const std::string& jsstr) :
  _mutex(),
  _oDataContext(RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT),
  _value()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

ODataBulkCreateResponsePayload::ODataBulkCreateResponsePayload(const nlohmann::json& js) :
  _mutex(),
  _oDataContext(RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT),
  _value()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

ODataBulkCreateResponsePayload::ODataBulkCreateResponsePayload(
  const ODataBulkCreateResponsePayload& data) :
  _mutex(),
  _oDataContext(data._oDataContext),
  _value(data._value)
{
}

ODataBulkCreateResponsePayload::~ODataBulkCreateResponsePayload() ACS_NOEXCEPT
{
}

ODataBulkCreateResponsePayload& ODataBulkCreateResponsePayload::operator=(
  const ODataBulkCreateResponsePayload& data)
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
        _value = data._value;
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

void ODataBulkCreateResponsePayload::fromJSON(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataBulkCreateResponsePayloadException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void ODataBulkCreateResponsePayload::fromJSONObject(const nlohmann::json& json)
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
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value \"" << _oDataContext << "\" is not what is expected (\""
                                 << RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT << "\") for \""
                                 << RESPONSE_PAYLOAD_KEY_ODATACONTEXT << "\" key into json response payload ")
    }

    if (json.find(RESPONSE_PAYLOAD_KEY_VALUE) == json.end() ||
        json[RESPONSE_PAYLOAD_KEY_VALUE].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_VALUE
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(ODataBulkCreateResponsePayloadException(os.str()));
    }
    _value.fromJSONObject(json[RESPONSE_PAYLOAD_KEY_VALUE]);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataBulkCreateResponsePayloadException(e, "Error reading JSON Object."));
  }
}

std::string ODataBulkCreateResponsePayload::toJSON() const
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
    ACS_THROW(ODataBulkCreateResponsePayloadException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP

  return str;
}

nlohmann::json ODataBulkCreateResponsePayload::toJSONObject() const
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

    js[RESPONSE_PAYLOAD_KEY_VALUE] = _value.toJSONObject();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting Value JSON Object for \"" << RESPONSE_PAYLOAD_KEY_VALUE << "\" key");
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataBulkCreateResponsePayloadException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

_ACS_END_NAMESPACE
