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

#include <ODataBatchOrderResponsePayload.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace lta;

/***********
 * ELEMENT *
 **********/

ACS_CLASS_DEFINE_DEBUG_LEVEL(ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueElement);

const std::string ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueElement::
  RESPONSE_PAYLOAD_KEY_ID = "Id";
const std::string ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueElement::
  RESPONSE_PAYLOAD_KEY_STATUS = "Status";

ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueElement::
  BatchOrderValueElement() :
  _mutex(),
  _id(),
  _status(ORDERSTATUS_INVALIDSTATUS)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueElement::
  BatchOrderValueElement(const std::string& jsstr) :
  _mutex(),
  _id(),
  _status(ORDERSTATUS_INVALIDSTATUS)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueElement::
  BatchOrderValueElement(const nlohmann::json& js) :
  _mutex(),
  _id(),
  _status(ORDERSTATUS_INVALIDSTATUS)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueElement::
  BatchOrderValueElement(
    const ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueElement& data) :
  _mutex(),
  _id(data._id),
  _status(data._status)
{
}

ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueElement::
  ~BatchOrderValueElement() ACS_NOEXCEPT
{
}

ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueElement&
ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueElement::operator=(
  const ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueElement& data)
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

void ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueElement::fromJSON(
  const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BatchOrderValueElementException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueElement::fromJSONObject(
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
      ACS_THROW(BatchOrderValueElementException(os.str()));
    }
    _id = json[RESPONSE_PAYLOAD_KEY_ID];
    if (_id.empty())
    {
      std::ostringstream os;
      os << "Empty value for key \"" << RESPONSE_PAYLOAD_KEY_ID << "\" into json response payload. Value is mandatory";
      ACS_THROW(BatchOrderValueElementException(os.str()));
    }

    if (json.find(RESPONSE_PAYLOAD_KEY_STATUS) == json.end() || json[RESPONSE_PAYLOAD_KEY_STATUS].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(BatchOrderValueElementException(os.str()));
    }
    std::string statusStr = json[RESPONSE_PAYLOAD_KEY_STATUS];
    if (statusStr.empty())
    {
      std::ostringstream os;
      os << "Empty value for key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload. Value is mandatory";
      ACS_THROW(BatchOrderValueElementException(os.str()));
    }
    std::map<std::string, OrderStatus>::const_iterator it = StringToOrderStatus.find(statusStr);
    if (it == StringToOrderStatus.end())
    {
      _status = ORDERSTATUS_INVALIDSTATUS;
      std::ostringstream os;
      os << "Invalid value for key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload. Value is mandatory";
      ACS_THROW(BatchOrderValueElementException(os.str()));
    }
    _status = it->second;
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BatchOrderValueElementException(e, "Error reading JSON Object."));
  }
}

std::string ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueElement::toJSON() const
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
    ACS_THROW(BatchOrderValueElementException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP

  return str;
}

nlohmann::json ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueElement::
  toJSONObject() const
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
      ACS_THROW(BatchOrderValueElementException(os.str()));
    }
    else
    {
      js[RESPONSE_PAYLOAD_KEY_ID] = _id;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _id << "\" for \""
                                                             << RESPONSE_PAYLOAD_KEY_ID << "\" key");
    }

    if (ORDERSTATUS_INVALIDSTATUS == _status)
    {
      std::ostringstream os;
      os << "Invalid value for key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload. Value is mandatory";
      ACS_THROW(BatchOrderValueElementException(os.str()));
    }
    std::string statusStr = OrderStatusToString.at(_status);
    js[RESPONSE_PAYLOAD_KEY_STATUS] = statusStr;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << statusStr << "\" for \"" << RESPONSE_PAYLOAD_KEY_STATUS
                                                           << "\" key");
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BatchOrderValueElementException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

/***********
 *  LIST   *
 **********/

namespace
{
bool compareFunction(const ODataBatchOrderResponsePayload::BatchOrderValueList::
                       BatchOrderValueElement& a,
                     const ODataBatchOrderResponsePayload::BatchOrderValueList::
                       BatchOrderValueElement& b)
{
  return (a.getID() < b.getID());
}
}  // namespace

ACS_CLASS_DEFINE_DEBUG_LEVEL(ODataBatchOrderResponsePayload::BatchOrderValueList);

ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueList() :
  _mutex(),
  _value()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueList(const std::string& jsstr) :
  _mutex(),
  _value()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueList(const nlohmann::json& js) :
  _mutex(),
  _value()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueList(
  const BatchOrderValueList& list) :
  _mutex(),
  _value(list._value)
{
}

ODataBatchOrderResponsePayload::BatchOrderValueList::~BatchOrderValueList() ACS_NOEXCEPT {}

const ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueElement&
ODataBatchOrderResponsePayload::BatchOrderValueList::operator[](size_t idx) const
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

ODataBatchOrderResponsePayload::BatchOrderValueList::BatchOrderValueElement&
ODataBatchOrderResponsePayload::BatchOrderValueList::operator[](size_t idx)
{
  const BatchOrderValueList& me = (*this);
  const BatchOrderValueElement& e = me[idx];      // calls const version
  return const_cast<BatchOrderValueElement&>(e);  // cast away constness
}

void ODataBatchOrderResponsePayload::BatchOrderValueList::add(const BatchOrderValueElement& payload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  _value.push_back(payload);
}

void ODataBatchOrderResponsePayload::BatchOrderValueList::add(const BatchOrderValueList& payload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);
  add(payload.getList());
}

void ODataBatchOrderResponsePayload::BatchOrderValueList::add(
  const std::vector<BatchOrderValueElement>& payload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);
  _value.insert(_value.end(), payload.begin(), payload.end());
}

void ODataBatchOrderResponsePayload::BatchOrderValueList::sort()
{
  Lock l(_mutex);
  std::sort(_value.begin(), _value.end(), compareFunction);
}

void ODataBatchOrderResponsePayload::BatchOrderValueList::sort(std::vector<BatchOrderValueElement>& out)
  const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  out = _value;
  std::sort(out.begin(), out.end(), compareFunction);
}

void ODataBatchOrderResponsePayload::BatchOrderValueList::clear()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  _value.clear();
}

size_t ODataBatchOrderResponsePayload::BatchOrderValueList::size() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  return _value.size();
}

void ODataBatchOrderResponsePayload::BatchOrderValueList::fromJSON(const std::string& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(json);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BatchOrderValueListException(e, "Error reading JSON."));
  }

  fromJSONObject(js);
}

void ODataBatchOrderResponsePayload::BatchOrderValueList::fromJSONObject(const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  try
  {
    _value.clear();

    if (json.is_null() || !json.is_array())
    {
      ACS_THROW(BatchOrderValueListException(
        "Json response payload is not an array or the associated value is null"));
    }

    size_t listSize = json.size();
    for (size_t idx = 0; idx < listSize; ++idx)
    {
      BatchOrderValueElement element(json[idx]);
      add(element);
    }
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the is_null and is_array checks, other check are in
  // BatchOrderValueElement fromJSONObject functions
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BatchOrderValueListException(e, "Error reading JSON Object."));
  }
  // LCOV_EXCL_STOP
}

std::string ODataBatchOrderResponsePayload::BatchOrderValueList::toJSON() const
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
    ACS_THROW(BatchOrderValueListException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return jsdumped;
}

nlohmann::json ODataBatchOrderResponsePayload::BatchOrderValueList::toJSONObject() const
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
    ACS_THROW(BatchOrderValueListException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

/***********
 * PAYLOAD *
 **********/

ACS_CLASS_DEFINE_DEBUG_LEVEL(ODataBatchOrderResponsePayload);

const std::string ODataBatchOrderResponsePayload::RESPONSE_PAYLOAD_KEY_ODATACONTEXT = "@odata.context";
const std::string ODataBatchOrderResponsePayload::RESPONSE_PAYLOAD_KEY_VALUE = "value";

const std::string ODataBatchOrderResponsePayload::RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT =
  "$metadata#BatchOrder/$entity";

ODataBatchOrderResponsePayload::ODataBatchOrderResponsePayload() :
  _mutex(),
  _oDataContext(RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT),
  _value()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

ODataBatchOrderResponsePayload::ODataBatchOrderResponsePayload(const std::string& jsstr) :
  _mutex(),
  _oDataContext(RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT),
  _value()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

ODataBatchOrderResponsePayload::ODataBatchOrderResponsePayload(const nlohmann::json& js) :
  _mutex(),
  _oDataContext(RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT),
  _value()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

ODataBatchOrderResponsePayload::ODataBatchOrderResponsePayload(
  const ODataBatchOrderResponsePayload& data) :
  _mutex(),
  _oDataContext(data._oDataContext),
  _value(data._value)
{
}

ODataBatchOrderResponsePayload::~ODataBatchOrderResponsePayload() ACS_NOEXCEPT
{
}

ODataBatchOrderResponsePayload& ODataBatchOrderResponsePayload::operator=(
  const ODataBatchOrderResponsePayload& data)
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

void ODataBatchOrderResponsePayload::fromJSON(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataBatchOrderResponsePayloadException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void ODataBatchOrderResponsePayload::fromJSONObject(const nlohmann::json& json)
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
      ACS_THROW(ODataBatchOrderResponsePayloadException(os.str()));
    }
    _value.fromJSONObject(json[RESPONSE_PAYLOAD_KEY_VALUE]);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataBatchOrderResponsePayloadException(e, "Error reading JSON Object."));
  }
}

std::string ODataBatchOrderResponsePayload::toJSON() const
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
    ACS_THROW(ODataBatchOrderResponsePayloadException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP

  return str;
}

nlohmann::json ODataBatchOrderResponsePayload::toJSONObject() const
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
    ACS_THROW(ODataBatchOrderResponsePayloadException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

_ACS_END_NAMESPACE
