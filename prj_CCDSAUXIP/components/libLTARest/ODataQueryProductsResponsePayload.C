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

#include <ODataQueryProductsResponsePayload.h>

_ACS_BEGIN_NAMESPACE(acs)

/***********
 * ELEMENT *
 **********/

ACS_CLASS_DEFINE_DEBUG_LEVEL(ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement);

const std::string ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement::
  RESPONSE_PAYLOAD_KEY_ID = "Id";
const std::string ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement::
  RESPONSE_PAYLOAD_KEY_NAME = "Name";
const std::string ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement::
  RESPONSE_PAYLOAD_KEY_ONLINE = "Online";

ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement::QueryProductsValueElement() :
  _mutex(),
  _id(),
  _name(),
  _online(false)
{
}

ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement::QueryProductsValueElement(
  const std::string& jsstr) :
  _mutex(),
  _id(),
  _name(),
  _online(false)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement::QueryProductsValueElement(
  const nlohmann::json& js) :
  _mutex(),
  _id(),
  _name(),
  _online(false)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement::QueryProductsValueElement(
  const ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement& data) :
  _mutex(),
  _id(data._id),
  _name(data._name),
  _online(data._online)
{
}

ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement::~QueryProductsValueElement()
  ACS_NOEXCEPT
{
}

ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement&
ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement::operator=(
  const ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement& data)
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
        _name = data._name;
        _online = data._online;
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

void ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement::fromJSON(
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
    ACS_THROW(QueryProductsValueElementException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement::fromJSONObject(
  const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  try
  {
    if (json.find(RESPONSE_PAYLOAD_KEY_ID) != json.end() && !json[RESPONSE_PAYLOAD_KEY_ID].is_null())
    {
      _id = json[RESPONSE_PAYLOAD_KEY_ID];
      if (_id.empty())
      {
        std::ostringstream os;
        os << "Empty value for key \"" << RESPONSE_PAYLOAD_KEY_ID
           << "\" into json response payload. Value is mandatory";
        ACS_THROW(QueryProductsValueElementException(os.str()));
      }
    }
    else
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_ID
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(QueryProductsValueElementException(os.str()));
    }

    if (json.find(RESPONSE_PAYLOAD_KEY_NAME) != json.end() && !json[RESPONSE_PAYLOAD_KEY_NAME].is_null())
    {
      _name = json[RESPONSE_PAYLOAD_KEY_NAME];
      if (_name.empty())
      {
        std::ostringstream os;
        os << "Empty value for key \"" << RESPONSE_PAYLOAD_KEY_NAME
           << "\" into json response payload. Value is mandatory";
        ACS_THROW(QueryProductsValueElementException(os.str()));
      }
    }
    else
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_NAME
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(QueryProductsValueElementException(os.str()));
    }

    if (json.find(RESPONSE_PAYLOAD_KEY_ONLINE) != json.end() && !json[RESPONSE_PAYLOAD_KEY_ONLINE].is_null())
    {
      _online = json[RESPONSE_PAYLOAD_KEY_ONLINE];
    }
    else
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_ONLINE
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(QueryProductsValueElementException(os.str()));
    }
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(QueryProductsValueElementException(e, "Error reading JSON Object."));
  }
}

std::string ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement::toJSON() const
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
    ACS_THROW(QueryProductsValueElementException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return str;
}

nlohmann::json ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement::toJSONObject()
  const
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
      ACS_THROW(QueryProductsValueElementException(os.str()));
    }
    else
    {
      js[RESPONSE_PAYLOAD_KEY_ID] = _id;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _id << "\" for \""
                                                             << RESPONSE_PAYLOAD_KEY_ID << "\" key");
    }

    if (_name.empty())
    {
      std::ostringstream os;
      os << "Empty value for \"" << RESPONSE_PAYLOAD_KEY_NAME << "\" key";
      ACS_THROW(QueryProductsValueElementException(os.str()));
    }
    else
    {
      js[RESPONSE_PAYLOAD_KEY_NAME] = _name;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _name << "\" for \""
                                                             << RESPONSE_PAYLOAD_KEY_NAME << "\" key");
    }

    js[RESPONSE_PAYLOAD_KEY_ONLINE] = _online;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _online << "\" for \"" << RESPONSE_PAYLOAD_KEY_ONLINE
                                                           << "\" key");
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(QueryProductsValueElementException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

/***********
 *  LIST   *
 **********/

namespace
{
bool compareFunction(const ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement& a,
                     const ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement& b)
{
  return (a.getID() < b.getID());
}
}  // namespace

ACS_CLASS_DEFINE_DEBUG_LEVEL(ODataQueryProductsResponsePayload::QueryProductsValueList);

ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueList() :
  _mutex(),
  _value()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueList(const std::string& jsstr) :
  _mutex(),
  _value()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueList(const nlohmann::json& js) :
  _mutex(),
  _value()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueList(const QueryProductsValueList& list) :
  _mutex(),
  _value(list._value)
{
}

ODataQueryProductsResponsePayload::QueryProductsValueList::~QueryProductsValueList() ACS_NOEXCEPT {}

const ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement&
ODataQueryProductsResponsePayload::QueryProductsValueList::operator[](size_t idx) const
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

ODataQueryProductsResponsePayload::QueryProductsValueList::QueryProductsValueElement&
ODataQueryProductsResponsePayload::QueryProductsValueList::operator[](size_t idx)
{
  const QueryProductsValueList& me = (*this);
  const QueryProductsValueElement& e = me[idx];      // calls const version
  return const_cast<QueryProductsValueElement&>(e);  // cast away constness
}

void ODataQueryProductsResponsePayload::QueryProductsValueList::add(const QueryProductsValueElement& payload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  _value.push_back(payload);
}

void ODataQueryProductsResponsePayload::QueryProductsValueList::add(const QueryProductsValueList& payload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);
  add(payload.getList());
}

void ODataQueryProductsResponsePayload::QueryProductsValueList::add(
  const std::vector<QueryProductsValueElement>& payload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);
  _value.insert(_value.end(), payload.begin(), payload.end());
}

void ODataQueryProductsResponsePayload::QueryProductsValueList::sort()
{
  Lock l(_mutex);
  std::sort(_value.begin(), _value.end(), compareFunction);
}

void ODataQueryProductsResponsePayload::QueryProductsValueList::sort(std::vector<QueryProductsValueElement>& out)
  const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  out = _value;
  std::sort(out.begin(), out.end(), compareFunction);
}

void ODataQueryProductsResponsePayload::QueryProductsValueList::clear()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  _value.clear();
}

size_t ODataQueryProductsResponsePayload::QueryProductsValueList::size() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  return _value.size();
}

void ODataQueryProductsResponsePayload::QueryProductsValueList::fromJSON(const std::string& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(json);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(QueryProductsValueListException(e, "Error reading JSON."));
  }

  fromJSONObject(js);
}

void ODataQueryProductsResponsePayload::QueryProductsValueList::fromJSONObject(const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  try
  {
    _value.clear();

    if (json.is_null() || !json.is_array())
    {
      ACS_THROW(QueryProductsValueListException(
        "Json response payload is not an array or the associated value is null"));
    }

    size_t listSize = json.size();
    for (size_t idx = 0; idx < listSize; ++idx)
    {
      QueryProductsValueElement element(json[idx]);
      add(element);
    }
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the is_null and is_array checks, other check are in
  // QueryProductsValueElement fromJSONObject functions
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(QueryProductsValueListException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
}

std::string ODataQueryProductsResponsePayload::QueryProductsValueList::toJSON() const
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
    ACS_THROW(QueryProductsValueListException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return jsdumped;
}

nlohmann::json ODataQueryProductsResponsePayload::QueryProductsValueList::toJSONObject() const
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
    ACS_THROW(QueryProductsValueListException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

/***********
 * PAYLOAD *
 **********/

ACS_CLASS_DEFINE_DEBUG_LEVEL(ODataQueryProductsResponsePayload);

const std::string ODataQueryProductsResponsePayload::RESPONSE_PAYLOAD_KEY_ODATACONTEXT = "@odata.context";
const std::string ODataQueryProductsResponsePayload::RESPONSE_PAYLOAD_KEY_VALUE = "value";
const std::string ODataQueryProductsResponsePayload::RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT =
  "$metadata#Products/$entity";

ODataQueryProductsResponsePayload::ODataQueryProductsResponsePayload() :
  _mutex(),
  _oDataContext(RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT),
  _value()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

ODataQueryProductsResponsePayload::ODataQueryProductsResponsePayload(const std::string& jsstr) :
  _mutex(),
  _oDataContext(RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT),
  _value()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

ODataQueryProductsResponsePayload::ODataQueryProductsResponsePayload(const nlohmann::json& js) :
  _mutex(),
  _oDataContext(RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT),
  _value()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

ODataQueryProductsResponsePayload::ODataQueryProductsResponsePayload(
  const ODataQueryProductsResponsePayload& data) :
  _mutex(),
  _oDataContext(data._oDataContext),
  _value(data._value)
{
}

ODataQueryProductsResponsePayload::~ODataQueryProductsResponsePayload() ACS_NOEXCEPT
{
}

ODataQueryProductsResponsePayload& ODataQueryProductsResponsePayload::operator=(
  const ODataQueryProductsResponsePayload& data)
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

void ODataQueryProductsResponsePayload::fromJSON(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataQueryProductsResponsePayloadException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void ODataQueryProductsResponsePayload::fromJSONObject(const nlohmann::json& json)
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
      ACS_THROW(ODataQueryProductsResponsePayloadException(os.str()));
    }
    _value.fromJSONObject(json[RESPONSE_PAYLOAD_KEY_VALUE]);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataQueryProductsResponsePayloadException(e, "Error reading JSON Object."));
  }
}

std::string ODataQueryProductsResponsePayload::toJSON() const
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
    ACS_THROW(ODataQueryProductsResponsePayloadException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP

  return str;
}

nlohmann::json ODataQueryProductsResponsePayload::toJSONObject() const
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
    ACS_THROW(ODataQueryProductsResponsePayloadException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

_ACS_END_NAMESPACE
