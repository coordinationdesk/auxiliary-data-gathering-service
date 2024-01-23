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

#ifndef _RetrieveServiceGetProductResponsePayload_h_
#define _RetrieveServiceGetProductResponsePayload_h_

#include <acs_c++config.hpp>
#include <unistd.h>
#include <string>

#include <ThreadSafe.h>
#include <json.hpp>
#include <Filterables.h>
#include <dbPersistent.h>

#include <RetrieveServiceResponseErrorCodeEnum.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful response payload sent by LTARetrieveService */
class RetrieveServiceGetProductResponsePayload
{
public:
  exDECLARE_EXCEPTION(RetrieveServiceGetProductResponsePayloadException, exException);
  static const std::string RESPONSE_PAYLOAD_KEY_FILEQUEUEID;
  static const std::string RESPONSE_PAYLOAD_KEY_ERRORMSG;
  static const std::string RESPONSE_PAYLOAD_KEY_ERRORCODE;
  static const std::string RESPONSE_PAYLOAD_KEY_HTTPCODE;

public:
  /** Ctors/Dtors */
  RetrieveServiceGetProductResponsePayload();
  explicit RetrieveServiceGetProductResponsePayload(const std::string& jsstr);
  explicit RetrieveServiceGetProductResponsePayload(const nlohmann::json& json);
  RetrieveServiceGetProductResponsePayload(const RetrieveServiceGetProductResponsePayload& c);
  virtual ~RetrieveServiceGetProductResponsePayload() ACS_NOEXCEPT;

  /** Operators overload */
  RetrieveServiceGetProductResponsePayload& operator=(const RetrieveServiceGetProductResponsePayload& c);

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& jsstr);

  inline dbPersistent::IdType getFilequeueID() const
  {
    Lock l(_mutex);
    return _filequeueID;
  }

  inline std::string getErrormsg() const
  {
    Lock l(_mutex);
    return _errormsg;
  }

  inline RetrieveServiceResponseErrorCode getErrorCode(bool* set = nullptr) const
  {
    Lock l(_mutex);
    if (nullptr == set)
    {
      ACS_COND_THROW(!_errorCodeSet, RetrieveServiceGetProductResponsePayloadException(
                                       RESPONSE_PAYLOAD_KEY_ERRORCODE + " not set"));
    }
    else
    {
      *set = _errorCodeSet;
    }

    return _errorCode;
  }

  inline long getHttpCode(bool* set = nullptr) const
  {
    Lock l(_mutex);
    if (nullptr == set)
    {
      ACS_COND_THROW(!_httpCodeSet, RetrieveServiceGetProductResponsePayloadException(
                                      RESPONSE_PAYLOAD_KEY_HTTPCODE + " not set"));
    }
    else
    {
      *set = _httpCodeSet;
    }

    return _httpCode;
  }

  inline void setFilequeueID(dbPersistent::IdType newVal)
  {
    Lock l(_mutex);
    _filequeueID = newVal;
  }

  inline void setErrormsg(const std::string& newVal)
  {
    Lock l(_mutex);
    _errormsg = newVal;
  }

  inline void setErrorCode(const RetrieveServiceResponseErrorCode& newVal)
  {
    Lock l(_mutex);
    _errorCode = newVal;
    _errorCodeSet = true;
  }

  inline void setHttpCode(const long& newVal)
  {
    Lock l(_mutex);
    _httpCode = newVal;
    _httpCodeSet = true;
  }

  inline void unsetErrorCode()
  {
    Lock l(_mutex);
    _errorCodeSet = false;
  }

  inline void unsetHttpCode()
  {
    Lock l(_mutex);
    _httpCodeSet = false;
  }

private:
  Mutex _mutex;

  dbPersistent::IdType _filequeueID;
  std::string _errormsg;
  RetrieveServiceResponseErrorCode _errorCode;
  bool _errorCodeSet;
  long _httpCode;
  bool _httpCodeSet;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(RetrieveServiceGetProductResponsePayload);
};

_ACS_END_NAMESPACE

#endif /* _RetrieveServiceGetProductResponsePayload_h_ */
