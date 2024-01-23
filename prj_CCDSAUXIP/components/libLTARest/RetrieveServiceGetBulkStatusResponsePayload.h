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

#ifndef _RetrieveServiceGetBulkStatusResponsePayload_h_
#define _RetrieveServiceGetBulkStatusResponsePayload_h_

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
class RetrieveServiceGetBulkStatusResponsePayload
{
public:
  exDECLARE_EXCEPTION(RetrieveServiceGetBulkStatusResponsePayloadException, exException);
  static const std::string RESPONSE_PAYLOAD_KEY_STATUS;
  static const std::string RESPONSE_PAYLOAD_KEY_ERRORMSG;
  static const std::string RESPONSE_PAYLOAD_KEY_ERRORCODE;

public:
  /** Ctors/Dtors */
  RetrieveServiceGetBulkStatusResponsePayload();
  explicit RetrieveServiceGetBulkStatusResponsePayload(const std::string& jsstr);
  explicit RetrieveServiceGetBulkStatusResponsePayload(const nlohmann::json& json);
  RetrieveServiceGetBulkStatusResponsePayload(const RetrieveServiceGetBulkStatusResponsePayload& c);
  virtual ~RetrieveServiceGetBulkStatusResponsePayload() ACS_NOEXCEPT;

  /** Operators overload */
  RetrieveServiceGetBulkStatusResponsePayload& operator=(const RetrieveServiceGetBulkStatusResponsePayload& c);

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& jsstr);

  inline std::string getStatus() const
  {
    Lock l(_mutex);
    return _status;
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
      ACS_COND_THROW(!_errorCodeSet, RetrieveServiceGetBulkStatusResponsePayloadException(
                                       RESPONSE_PAYLOAD_KEY_ERRORCODE + " not set"));
    }
    else
    {
      *set = _errorCodeSet;
    }

    return _errorCode;
  }

  inline void setStatus(const std::string& newVal)
  {
    Lock l(_mutex);
    _status = newVal;
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

  inline void unsetErrorCode()
  {
    Lock l(_mutex);
    _errorCodeSet = false;
  }

private:
  Mutex _mutex;

  std::string _status;
  std::string _errormsg;
  RetrieveServiceResponseErrorCode _errorCode;
  bool _errorCodeSet;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(RetrieveServiceGetBulkStatusResponsePayload);
};

_ACS_END_NAMESPACE

#endif /* _RetrieveServiceGetBulkStatusResponsePayload_h_ */
