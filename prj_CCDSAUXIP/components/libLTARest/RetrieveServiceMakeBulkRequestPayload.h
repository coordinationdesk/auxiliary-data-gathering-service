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

#ifndef _RetrieveServiceMakeBulkRequestPayload_h_
#define _RetrieveServiceMakeBulkRequestPayload_h_

#include <acs_c++config.hpp>
#include <unistd.h>
#include <string>
#include <map>

#include <ThreadSafe.h>
#include <json.hpp>
#include <Filterables.h>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful request payload sent to LTARetrieveService */
class RetrieveServiceMakeBulkRequestPayload
{
public:
  exDECLARE_EXCEPTION(RetrieveServiceMakeBulkRequestPayloadException, exException);
  static const std::string REQUEST_PAYLOAD_KEY_FILTERQUERY;
  static const std::string REQUEST_PAYLOAD_KEY_DESTINATIONURL;
  static const std::string REQUEST_PAYLOAD_KEY_SELALGORITHM;
  static const std::string REQUEST_PAYLOAD_KEY_LTANAME;
  static const std::string REQUEST_PAYLOAD_KEY_EXTRACTMETADATA;

  enum Algorithms
  {
    NAMED,
    ALL,
    ROUNDROBIN,
    LEASTLOAD,
    INVALID
  };
  static const std::map<Algorithms, std::string> ALGORITHMSNAMES;
  static const std::map<std::string, Algorithms> ALGORITHMSVALUES;

public:
  /** Ctors/Dtors */
  RetrieveServiceMakeBulkRequestPayload();
  explicit RetrieveServiceMakeBulkRequestPayload(const std::string& jsstr);
  explicit RetrieveServiceMakeBulkRequestPayload(const nlohmann::json& json);
  RetrieveServiceMakeBulkRequestPayload(const RetrieveServiceMakeBulkRequestPayload& c);
  virtual ~RetrieveServiceMakeBulkRequestPayload() ACS_NOEXCEPT;

  /** Operators overload */
  RetrieveServiceMakeBulkRequestPayload& operator=(const RetrieveServiceMakeBulkRequestPayload& c);

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& jsstr);

  inline std::string getFilterQuery() const
  {
    Lock l(_mutex);
    return _filterQuery;
  }

  inline std::string getDestinationURL() const
  {
    Lock l(_mutex);
    return _destinationURL;
  }

  inline Algorithms getSelAlgorithm() const
  {
    Lock l(_mutex);
    return _selAlgorithm;
  }

  inline std::string getLTAName(bool* valid = nullptr) const
  {
    Lock l(_mutex);
    if (nullptr == valid)
    {
      ACS_COND_THROW(_selAlgorithm != NAMED, RetrieveServiceMakeBulkRequestPayloadException(
                                               "Impossible to get LTA Name for the chosen algorithm"));
      return _ltaName;
    }
    else
    {
      *valid = (_selAlgorithm == NAMED);
      if (*valid)
      {
        return _ltaName;
      }
      else
      {
        return "";
      }
    }
  }

  inline bool getExtractMetadata(bool* set = nullptr) const
  {
    Lock l(_mutex);
    if (nullptr == set)
    {
      ACS_COND_THROW(!_extractMetadataSet, RetrieveServiceMakeBulkRequestPayloadException(
                                             "Extract Metadata field not set"));
    }
    else
    {
      *set = _extractMetadataSet;
    }
    return _extractMetadata;
  }

  inline void setFilterQuery(const std::string& newVal)
  {
    Lock l(_mutex);
    _filterQuery = newVal;
  }

  inline void setDestinationURL(const std::string& newVal)
  {
    Lock l(_mutex);
    _destinationURL = newVal;
  }

  inline void setSelAlgorithm(Algorithms newVal)
  {
    Lock l(_mutex);
    _selAlgorithm = newVal;
  }

  inline void setLTAName(const std::string& newVal)
  {
    Lock l(_mutex);
    ACS_COND_THROW(_selAlgorithm != NAMED, RetrieveServiceMakeBulkRequestPayloadException(
                                             "Impossible to get LTA Name for the chosen algorithm"));
    _ltaName = newVal;
  }

  inline void setExtractMetadata(const bool newVal)
  {
    Lock l(_mutex);
    _extractMetadata = newVal;
    _extractMetadataSet = true;
  }

  inline void unsetExtractMetadata()
  {
    Lock l(_mutex);
    _extractMetadataSet = false;
  }

private:
  Mutex _mutex;

  std::string _filterQuery;
  std::string _destinationURL;
  Algorithms _selAlgorithm;
  std::string _ltaName;
  bool _extractMetadataSet;
  bool _extractMetadata;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(RetrieveServiceMakeBulkRequestPayload);
};

_ACS_END_NAMESPACE

#endif /* _RetrieveServiceMakeBulkRequestPayload_h_ */
