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

#include <RetrieveServiceMakeBulkRequestPayload.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(RetrieveServiceMakeBulkRequestPayload);

const std::string RetrieveServiceMakeBulkRequestPayload::REQUEST_PAYLOAD_KEY_FILTERQUERY = "filter_query";
const std::string RetrieveServiceMakeBulkRequestPayload::REQUEST_PAYLOAD_KEY_DESTINATIONURL = "destination_url";
const std::string RetrieveServiceMakeBulkRequestPayload::REQUEST_PAYLOAD_KEY_SELALGORITHM = "sel_algorithm";
const std::string RetrieveServiceMakeBulkRequestPayload::REQUEST_PAYLOAD_KEY_LTANAME = "lta_name";
const std::string RetrieveServiceMakeBulkRequestPayload::REQUEST_PAYLOAD_KEY_EXTRACTMETADATA = "extract_metadata";

const std::map<RetrieveServiceMakeBulkRequestPayload::Algorithms, std::string>
  RetrieveServiceMakeBulkRequestPayload::ALGORITHMSNAMES = {
    { NAMED, "NAMED" },
    { ALL, "ALL" },
    { ROUNDROBIN, "ROUNDROBIN" },
    { LEASTLOAD, "LEASTLOAD" }
  };

const std::map<std::string, RetrieveServiceMakeBulkRequestPayload::Algorithms>
  RetrieveServiceMakeBulkRequestPayload::ALGORITHMSVALUES = {
    { "NAMED", NAMED },
    { "ALL", ALL },
    { "ROUNDROBIN", ROUNDROBIN },
    { "LEASTLOAD", LEASTLOAD }
  };

RetrieveServiceMakeBulkRequestPayload::RetrieveServiceMakeBulkRequestPayload() :
  _mutex(),
  _filterQuery(),
  _destinationURL(),
  _selAlgorithm(INVALID),
  _ltaName(),
  _extractMetadataSet(false),
  _extractMetadata(false)
{
}

RetrieveServiceMakeBulkRequestPayload::RetrieveServiceMakeBulkRequestPayload(const std::string& jsstr) :
  _mutex(),
  _filterQuery(),
  _destinationURL(),
  _selAlgorithm(INVALID),
  _ltaName(),
  _extractMetadataSet(false),
  _extractMetadata(false)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

RetrieveServiceMakeBulkRequestPayload::RetrieveServiceMakeBulkRequestPayload(const nlohmann::json& js) :
  _mutex(),
  _filterQuery(),
  _destinationURL(),
  _selAlgorithm(INVALID),
  _ltaName(),
  _extractMetadataSet(false),
  _extractMetadata(false)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

RetrieveServiceMakeBulkRequestPayload::RetrieveServiceMakeBulkRequestPayload(
  const RetrieveServiceMakeBulkRequestPayload& data) :
  _mutex(),
  _filterQuery(data._filterQuery),
  _destinationURL(data._destinationURL),
  _selAlgorithm(data._selAlgorithm),
  _ltaName(data._ltaName),
  _extractMetadataSet(data._extractMetadataSet),
  _extractMetadata(data._extractMetadata)
{
}

RetrieveServiceMakeBulkRequestPayload::~RetrieveServiceMakeBulkRequestPayload() ACS_NOEXCEPT
{
}

RetrieveServiceMakeBulkRequestPayload& RetrieveServiceMakeBulkRequestPayload::operator=(
  const RetrieveServiceMakeBulkRequestPayload& data)
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
        _filterQuery = data._filterQuery;
        _destinationURL = data._destinationURL;
        _selAlgorithm = data._selAlgorithm;
        _ltaName = data._ltaName;
        _extractMetadataSet = data._extractMetadataSet;
        _extractMetadata = data._extractMetadata;
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

void RetrieveServiceMakeBulkRequestPayload::fromJSON(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(RetrieveServiceMakeBulkRequestPayloadException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void RetrieveServiceMakeBulkRequestPayload::fromJSONObject(const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  try
  {
    if (json.find(REQUEST_PAYLOAD_KEY_FILTERQUERY) == json.end() || json[REQUEST_PAYLOAD_KEY_FILTERQUERY].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << REQUEST_PAYLOAD_KEY_FILTERQUERY
         << "\" into json request payload or the associated value is null. Key is mandatory";
      ACS_THROW(RetrieveServiceMakeBulkRequestPayloadException(os.str()));
    }
    _filterQuery = json[REQUEST_PAYLOAD_KEY_FILTERQUERY];
    if (_filterQuery.empty())
    {
      std::ostringstream os;
      os << "Empty value for key \"" << REQUEST_PAYLOAD_KEY_FILTERQUERY
         << "\" into json request payload. Value is mandatory";
      ACS_THROW(RetrieveServiceMakeBulkRequestPayloadException(os.str()));
    }

    if (json.find(REQUEST_PAYLOAD_KEY_DESTINATIONURL) == json.end() ||
        json[REQUEST_PAYLOAD_KEY_DESTINATIONURL].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << REQUEST_PAYLOAD_KEY_DESTINATIONURL
         << "\" into json request payload or the associated value is null. Key is mandatory";
      ACS_THROW(RetrieveServiceMakeBulkRequestPayloadException(os.str()));
    }
    _destinationURL = json[REQUEST_PAYLOAD_KEY_DESTINATIONURL];
    if (_destinationURL.empty())
    {
      std::ostringstream os;
      os << "Empty value for key \"" << REQUEST_PAYLOAD_KEY_DESTINATIONURL
         << "\" into json request payload. Value is mandatory";
      ACS_THROW(RetrieveServiceMakeBulkRequestPayloadException(os.str()));
    }

    if (json.find(REQUEST_PAYLOAD_KEY_SELALGORITHM) == json.end() || json[REQUEST_PAYLOAD_KEY_SELALGORITHM].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << REQUEST_PAYLOAD_KEY_SELALGORITHM
         << "\" into json request payload or the associated value is null. Key is mandatory";
      ACS_THROW(RetrieveServiceMakeBulkRequestPayloadException(os.str()));
    }
    std::map<std::string, Algorithms>::const_iterator it =
      ALGORITHMSVALUES.find(json[REQUEST_PAYLOAD_KEY_SELALGORITHM]);
    if (it != ALGORITHMSVALUES.end())
    {
      _selAlgorithm = it->second;
    }
    else
    {
      _selAlgorithm = INVALID;
      std::ostringstream os;
      os << "Invalid value for \"" << REQUEST_PAYLOAD_KEY_SELALGORITHM << "\" key into json request payload";
      ACS_THROW(RetrieveServiceMakeBulkRequestPayloadException(os.str()));
    }

    if (_selAlgorithm == NAMED)
    {
      if (json.find(REQUEST_PAYLOAD_KEY_LTANAME) == json.end() || json[REQUEST_PAYLOAD_KEY_LTANAME].is_null())
      {
        std::ostringstream os;
        os << "Cannot find key \"" << REQUEST_PAYLOAD_KEY_LTANAME
           << "\" into json request payload or the associated value is null. Key is mandatory with the selected "
              "algorithm";
        ACS_THROW(RetrieveServiceMakeBulkRequestPayloadException(os.str()));
      }
      _ltaName = json[REQUEST_PAYLOAD_KEY_LTANAME];
      if (_ltaName.empty())
      {
        std::ostringstream os;
        os << "Empty value for key \"" << REQUEST_PAYLOAD_KEY_LTANAME
           << "\" into json request payload. Value is mandatory with the selected algorithm";
        ACS_THROW(RetrieveServiceMakeBulkRequestPayloadException(os.str()));
      }
    }

    if (json.find(REQUEST_PAYLOAD_KEY_EXTRACTMETADATA) == json.end())
    {
      _extractMetadataSet = false;
    }
    else if (json[REQUEST_PAYLOAD_KEY_EXTRACTMETADATA].is_null())
    {
      std::ostringstream os;
      os << "Value associated to key \"" << REQUEST_PAYLOAD_KEY_EXTRACTMETADATA
         << "\" into json request payload is null";
      ACS_THROW(RetrieveServiceMakeBulkRequestPayloadException(os.str()));
    }
    else
    {
      _extractMetadata = json[REQUEST_PAYLOAD_KEY_EXTRACTMETADATA];
      _extractMetadataSet = true;
    }
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(RetrieveServiceMakeBulkRequestPayloadException(e, "Error reading JSON Object."));
  }
}

std::string RetrieveServiceMakeBulkRequestPayload::toJSON() const
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
    ACS_THROW(RetrieveServiceMakeBulkRequestPayloadException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return str;
}

nlohmann::json RetrieveServiceMakeBulkRequestPayload::toJSONObject() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  nlohmann::json js;
  try
  {
    if (_filterQuery.empty())
    {
      std::ostringstream os;
      os << "Empty value for \"" << REQUEST_PAYLOAD_KEY_FILTERQUERY << "\" key";
      ACS_THROW(RetrieveServiceMakeBulkRequestPayloadException(os.str()));
    }
    else
    {
      js[REQUEST_PAYLOAD_KEY_FILTERQUERY] = _filterQuery;
      ACS_CLASS_WRITE_DEBUG(
        ACS_LOW_VERB, "Setting value \"" << _filterQuery << "\" for \"" << REQUEST_PAYLOAD_KEY_FILTERQUERY << "\" key");
    }

    if (_destinationURL.empty())
    {
      std::ostringstream os;
      os << "Empty value for \"" << REQUEST_PAYLOAD_KEY_DESTINATIONURL << "\" key";
      ACS_THROW(RetrieveServiceMakeBulkRequestPayloadException(os.str()));
    }
    else
    {
      js[REQUEST_PAYLOAD_KEY_DESTINATIONURL] = _destinationURL;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _destinationURL << "\" for \""
                                                             << REQUEST_PAYLOAD_KEY_DESTINATIONURL << "\" key");
    }

    if (_selAlgorithm != INVALID)
    {
      std::string algorithmsName = ALGORITHMSNAMES.at(_selAlgorithm);
      js[REQUEST_PAYLOAD_KEY_SELALGORITHM] = algorithmsName;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << algorithmsName << "\" for \""
                                                             << REQUEST_PAYLOAD_KEY_SELALGORITHM << "\" key");
    }
    else
    {
      std::ostringstream os;
      os << "Invalid value for \"" << REQUEST_PAYLOAD_KEY_SELALGORITHM << "\" key";
      ACS_THROW(RetrieveServiceMakeBulkRequestPayloadException(os.str()));
    }

    if (_selAlgorithm == NAMED)
    {
      if (_ltaName.empty())
      {
        std::ostringstream os;
        os << "Empty value for \"" << REQUEST_PAYLOAD_KEY_LTANAME << "\" key";
        ACS_THROW(RetrieveServiceMakeBulkRequestPayloadException(os.str()));
      }
      else
      {
        js[REQUEST_PAYLOAD_KEY_LTANAME] = _ltaName;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _ltaName << "\" for \""
                                                               << REQUEST_PAYLOAD_KEY_LTANAME << "\" key");
      }
    }

    if (_extractMetadataSet)
    {
      js[REQUEST_PAYLOAD_KEY_EXTRACTMETADATA] = _extractMetadata;
    }
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(RetrieveServiceMakeBulkRequestPayloadException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

_ACS_END_NAMESPACE
