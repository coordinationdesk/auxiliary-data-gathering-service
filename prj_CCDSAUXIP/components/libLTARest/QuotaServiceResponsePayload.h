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

#ifndef _QuotaServiceResponsePayload_h_
#define _QuotaServiceResponsePayload_h_

#include <acs_c++config.hpp>
#include <unistd.h>
#include <string>

#include <ThreadSafe.h>
#include <json.hpp>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful response payload sent by LTAQuotaService */
class QuotaServiceResponsePayload
{
public:
  exDECLARE_EXCEPTION(QuotaServiceResponsePayloadException, exException);
  static const std::string RESPONSE_PAYLOAD_KEY_OPERATIONUUID;
  static const std::string RESPONSE_PAYLOAD_KEY_GRANTED;
  static const std::string RESPONSE_PAYLOAD_KEY_REASON;
  static const std::string RESPONSE_PAYLOAD_KEY_TOKENUUID;

public:
  /** Ctors/Dtors */
  QuotaServiceResponsePayload();
  explicit QuotaServiceResponsePayload(const std::string& jsstr);
  explicit QuotaServiceResponsePayload(const nlohmann::json& json);
  QuotaServiceResponsePayload(const QuotaServiceResponsePayload& c);
  virtual ~QuotaServiceResponsePayload() ACS_NOEXCEPT;

  /** Operators overload */
  QuotaServiceResponsePayload& operator=(const QuotaServiceResponsePayload& c);

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& jsstr);

  inline std::string getOperationUUID() const
  {
    Lock l(_mutex);
    return _operationUUID;
  }

  inline bool getGranted() const
  {
    Lock l(_mutex);
    return _granted;
  }

  inline std::string getReason() const
  {
    Lock l(_mutex);
    return _reason;
  }

  inline std::string getTokenUUID() const
  {
    Lock l(_mutex);
    if (_tokenUUIDSet)
    {
      return _tokenUUID;
    }
    else
    {
      ACS_THROW(QuotaServiceResponsePayloadException("TokenUUID not set."));
    }
  }

  inline void setOperationUUID(const std::string& newVal)
  {
    Lock l(_mutex);
    _operationUUID = newVal;
  }

  inline void setGranted(bool newVal)
  {
    Lock l(_mutex);
    _granted = newVal;
  }

  inline void setReason(const std::string& newVal)
  {
    Lock l(_mutex);
    _reason = newVal;
  }

  inline void setTokenUUID(const std::string& newVal)
  {
    Lock l(_mutex);
    _tokenUUID = newVal;
    _tokenUUIDSet = true;
  }

private:
  Mutex _mutex;

  std::string _operationUUID;
  bool _granted;
  std::string _reason;
  bool _tokenUUIDSet;
  std::string _tokenUUID;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(QuotaServiceResponsePayload);
};

_ACS_END_NAMESPACE

#endif /* _QuotaServiceResponsePayload_h_ */
