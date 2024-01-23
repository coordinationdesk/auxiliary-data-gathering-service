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

#include <RetrieveServiceGetBatchDetailsResponsePayload.h>

_ACS_BEGIN_NAMESPACE(acs)

/***********
 * ELEMENT *
 **********/

ACS_CLASS_DEFINE_DEBUG_LEVEL(RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsElement);

const std::string RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsElement::
  RESPONSE_PAYLOAD_KEY_FILEID = "file_id";
const std::string RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsElement::
  RESPONSE_PAYLOAD_KEY_STATUS = "status";
const std::string RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsElement::
  RESPONSE_PAYLOAD_KEY_LTANAME = "lta_name";

RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsElement::FilesDetailsElement() :
  _mutex(),
  _fileID(0),
  _status(),
  _ltaName()
{
}

RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsElement::FilesDetailsElement(
  const std::string& jsstr) :
  _mutex(),
  _fileID(0),
  _status(),
  _ltaName()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsElement::FilesDetailsElement(
  const nlohmann::json& js) :
  _mutex(),
  _fileID(0),
  _status(),
  _ltaName()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsElement::FilesDetailsElement(
  const FilesDetailsElement& data) :
  _mutex(),
  _fileID(data._fileID),
  _status(data._status),
  _ltaName(data._ltaName)
{
}

RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsElement::~FilesDetailsElement()
  ACS_NOEXCEPT
{
}

RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsElement&
RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsElement::operator=(
  const FilesDetailsElement& data)
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
        _fileID = data._fileID;
        _status = data._status;
        _ltaName = data._ltaName;
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

void RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsElement::fromJSON(
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
    ACS_THROW(FilesDetailsElementException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsElement::fromJSONObject(
  const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  try
  {
    if (json.find(RESPONSE_PAYLOAD_KEY_FILEID) == json.end() || json[RESPONSE_PAYLOAD_KEY_FILEID].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_FILEID
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(FilesDetailsElementException(os.str()));
    }
    _fileID = json[RESPONSE_PAYLOAD_KEY_FILEID];

    if (json.find(RESPONSE_PAYLOAD_KEY_STATUS) != json.end() && !json[RESPONSE_PAYLOAD_KEY_STATUS].is_null())
    {
      _status = json[RESPONSE_PAYLOAD_KEY_STATUS];
    }
    else
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(FilesDetailsElementException(os.str()));
    }

    if (json.find(RESPONSE_PAYLOAD_KEY_LTANAME) != json.end() && !json[RESPONSE_PAYLOAD_KEY_LTANAME].is_null())
    {
      _ltaName = json[RESPONSE_PAYLOAD_KEY_LTANAME];
    }
    else
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_LTANAME
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(FilesDetailsElementException(os.str()));
    }
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(FilesDetailsElementException(e, "Error reading JSON Object."));
  }
}

std::string RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsElement::toJSON() const
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
    ACS_THROW(FilesDetailsElementException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return str;
}

nlohmann::json RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsElement::toJSONObject()
  const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  nlohmann::json js;
  try
  {
    js[RESPONSE_PAYLOAD_KEY_FILEID] = _fileID;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _fileID << "\" for \""
                                                           << RESPONSE_PAYLOAD_KEY_FILEID << "\" key");

    js[RESPONSE_PAYLOAD_KEY_STATUS] = _status;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _status << "\" for \""
                                                           << RESPONSE_PAYLOAD_KEY_STATUS << "\" key");

    js[RESPONSE_PAYLOAD_KEY_LTANAME] = _ltaName;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _ltaName << "\" for \""
                                                           << RESPONSE_PAYLOAD_KEY_LTANAME << "\" key");
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(FilesDetailsElementException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

/***********
 *  LIST   *
 **********/

namespace
{
bool compareFunction(const RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsElement& a,
                     const RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsElement& b)
{
  return (a.getFileID() < b.getFileID());
}
}  // namespace

ACS_CLASS_DEFINE_DEBUG_LEVEL(RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList);

RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsList() :
  _mutex(),
  _filesDetails()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsList(const std::string& jsstr) :
  _mutex(),
  _filesDetails()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsList(const nlohmann::json& js) :
  _mutex(),
  _filesDetails()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsList(const FilesDetailsList& list) :
  _mutex(),
  _filesDetails(list._filesDetails)
{
}

RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::~FilesDetailsList() ACS_NOEXCEPT {}

const RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsElement&
RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::operator[](size_t idx) const
{
  Lock l(_mutex);
  if (idx >= _filesDetails.size())
  {
    std::ostringstream ss;
    ss << "Invalid index " << idx << ". Size is " << _filesDetails.size();
    ACS_THROW(exBoundsException(ss.str()));
  }

  return _filesDetails[idx];
}

RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::FilesDetailsElement&
RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::operator[](size_t idx)
{
  const FilesDetailsList& me = (*this);
  const FilesDetailsElement& e = me[idx];      // calls const version
  return const_cast<FilesDetailsElement&>(e);  // cast away constness
}

void RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::add(const FilesDetailsElement& payload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  _filesDetails.push_back(payload);
}

void RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::add(const FilesDetailsList& payload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);
  add(payload.getFilesDetails());
}

void RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::add(
  const std::vector<FilesDetailsElement>& payload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);
  _filesDetails.insert(_filesDetails.end(), payload.begin(), payload.end());
}

void RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::sort()
{
  Lock l(_mutex);
  std::sort(_filesDetails.begin(), _filesDetails.end(), compareFunction);
}

void RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::sort(std::vector<FilesDetailsElement>& out)
  const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  out = _filesDetails;
  std::sort(out.begin(), out.end(), compareFunction);
}

void RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::clear()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  _filesDetails.clear();
}

size_t RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::size() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  return _filesDetails.size();
}

void RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::fromJSON(const std::string& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(json);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(FilesDetailsListException(e, "Error reading JSON."));
  }

  fromJSONObject(js);
}

void RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::fromJSONObject(const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  try
  {
    _filesDetails.clear();

    if (json.is_null() || !json.is_array())
    {
      ACS_THROW(FilesDetailsListException("Json response payload is not an array or the associated value is null"));
    }

    size_t listSize = json.size();
    for (size_t idx = 0; idx < listSize; ++idx)
    {
      FilesDetailsElement element(json[idx]);
      add(element);
    }
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the is_null and is_array checks, other check are in
  // FilesDetailsElement fromJSONObject functions
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(FilesDetailsListException(e, "Error reading JSON Object."));
  }
  // LCOV_EXCL_STOP
}

std::string RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::toJSON() const
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
    ACS_THROW(FilesDetailsListException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return jsdumped;
}

nlohmann::json RetrieveServiceGetBatchDetailsResponsePayload::FilesDetailsList::toJSONObject() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  nlohmann::json js;
  try
  {
    js = nlohmann::json::array();  // create the empty array

    size_t rqSize = _filesDetails.size();
    for (size_t idx = 0; idx < rqSize; ++idx)
    {
      js.push_back(_filesDetails[idx].toJSONObject());
    }
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(FilesDetailsListException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

/***********
 * PAYLOAD *
 **********/

ACS_CLASS_DEFINE_DEBUG_LEVEL(RetrieveServiceGetBatchDetailsResponsePayload);

const std::string RetrieveServiceGetBatchDetailsResponsePayload::RESPONSE_PAYLOAD_KEY_STATUS = "status";
const std::string RetrieveServiceGetBatchDetailsResponsePayload::RESPONSE_PAYLOAD_KEY_FILESDETAILS = "files_details";
const std::string RetrieveServiceGetBatchDetailsResponsePayload::RESPONSE_PAYLOAD_KEY_ERRORMSG = "errormsg";
const std::string RetrieveServiceGetBatchDetailsResponsePayload::RESPONSE_PAYLOAD_KEY_ERRORCODE = "error_code";

RetrieveServiceGetBatchDetailsResponsePayload::RetrieveServiceGetBatchDetailsResponsePayload() :
  _mutex(),
  _status(),
  _filesDetails(),
  _errormsg(),
  _errorCode(RetrieveServiceResponseErrorCode::RETRIEVESERVICE_INTERNAL_ERROR),
  _errorCodeSet(false)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

RetrieveServiceGetBatchDetailsResponsePayload::RetrieveServiceGetBatchDetailsResponsePayload(const std::string& jsstr) :
  _mutex(),
  _status(),
  _filesDetails(),
  _errormsg(),
  _errorCode(RetrieveServiceResponseErrorCode::RETRIEVESERVICE_INTERNAL_ERROR),
  _errorCodeSet(false)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

RetrieveServiceGetBatchDetailsResponsePayload::RetrieveServiceGetBatchDetailsResponsePayload(const nlohmann::json& js) :
  _mutex(),
  _status(),
  _filesDetails(),
  _errormsg(),
  _errorCode(RetrieveServiceResponseErrorCode::RETRIEVESERVICE_INTERNAL_ERROR),
  _errorCodeSet(false)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

RetrieveServiceGetBatchDetailsResponsePayload::RetrieveServiceGetBatchDetailsResponsePayload(
  const RetrieveServiceGetBatchDetailsResponsePayload& data) :
  _mutex(),
  _status(data._status),
  _filesDetails(data._filesDetails),
  _errormsg(data._errormsg),
  _errorCode(data._errorCode),
  _errorCodeSet(data._errorCodeSet)
{
}

RetrieveServiceGetBatchDetailsResponsePayload::~RetrieveServiceGetBatchDetailsResponsePayload() ACS_NOEXCEPT
{
}

RetrieveServiceGetBatchDetailsResponsePayload& RetrieveServiceGetBatchDetailsResponsePayload::operator=(
  const RetrieveServiceGetBatchDetailsResponsePayload& data)
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
        _status = data._status;
        _filesDetails = data._filesDetails;
        _errormsg = data._errormsg;
        _errorCode = data._errorCode;
        _errorCodeSet = data._errorCodeSet;
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

void RetrieveServiceGetBatchDetailsResponsePayload::fromJSON(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(RetrieveServiceGetBatchDetailsResponsePayloadException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void RetrieveServiceGetBatchDetailsResponsePayload::fromJSONObject(const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);

  try
  {
    if (json.find(RESPONSE_PAYLOAD_KEY_STATUS) == json.end() || json[RESPONSE_PAYLOAD_KEY_STATUS].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(RetrieveServiceGetBatchDetailsResponsePayloadException(os.str()));
    }
    _status = json[RESPONSE_PAYLOAD_KEY_STATUS];

    if (json.find(RESPONSE_PAYLOAD_KEY_FILESDETAILS) == json.end() ||
        json[RESPONSE_PAYLOAD_KEY_FILESDETAILS].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_FILESDETAILS
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(RetrieveServiceGetBatchDetailsResponsePayloadException(os.str()));
    }
    _filesDetails.fromJSONObject(json[RESPONSE_PAYLOAD_KEY_FILESDETAILS]);

    if (json.find(RESPONSE_PAYLOAD_KEY_ERRORMSG) != json.end() && !json[RESPONSE_PAYLOAD_KEY_ERRORMSG].is_null())
    {
      _errormsg = json[RESPONSE_PAYLOAD_KEY_ERRORMSG];
    }
    else
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_ERRORMSG
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(RetrieveServiceGetBatchDetailsResponsePayloadException(os.str()));
    }

    if (json.find(RESPONSE_PAYLOAD_KEY_ERRORCODE) != json.end())
    {
      if (!json[RESPONSE_PAYLOAD_KEY_ERRORCODE].is_null())
      {
        _errorCode = json[RESPONSE_PAYLOAD_KEY_ERRORCODE];
        _errorCodeSet = true;
      }
      else
      {
        _errorCodeSet = false;
        std::ostringstream os;
        os << "Null value for key \"" << RESPONSE_PAYLOAD_KEY_ERRORCODE << "\"";
        ACS_THROW(RetrieveServiceGetBatchDetailsResponsePayloadException(os.str()));
      }
    }
    else
    {
      _errorCodeSet = false;
    }
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(RetrieveServiceGetBatchDetailsResponsePayloadException(e, "Error reading JSON Object."));
  }
}

std::string RetrieveServiceGetBatchDetailsResponsePayload::toJSON() const
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
    ACS_THROW(RetrieveServiceGetBatchDetailsResponsePayloadException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP

  return str;
}

nlohmann::json RetrieveServiceGetBatchDetailsResponsePayload::toJSONObject() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  nlohmann::json js;
  try
  {
    // Add members to json object
    js[RESPONSE_PAYLOAD_KEY_STATUS] = _status;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _status << "\" for \""
                                                           << RESPONSE_PAYLOAD_KEY_STATUS << "\" key");

    js[RESPONSE_PAYLOAD_KEY_FILESDETAILS] = _filesDetails.toJSONObject();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting Files Details JSON Object for \""
                                          << RESPONSE_PAYLOAD_KEY_FILESDETAILS << "\" key");

    js[RESPONSE_PAYLOAD_KEY_ERRORMSG] = _errormsg;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _errormsg << "\" for \"" << RESPONSE_PAYLOAD_KEY_ERRORMSG
                                                           << "\" key");

    if (_errorCodeSet)
    {
      js[RESPONSE_PAYLOAD_KEY_ERRORCODE] = _errorCode;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << static_cast<int>(_errorCode) << "\" for \""
                                                             << RESPONSE_PAYLOAD_KEY_ERRORCODE << "\" key");
    }
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(RetrieveServiceGetBatchDetailsResponsePayloadException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

_ACS_END_NAMESPACE
