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

#ifndef _QuotaServiceUpdateOrderRegistryRequestPayload_h_
#define _QuotaServiceUpdateOrderRegistryRequestPayload_h_

#include <acs_c++config.hpp>
#include <unistd.h>
#include <string>

#include <ThreadSafe.h>
#include <json.hpp>
#include <Filterables.h>

#include <QuotaServicePayloadsExceptions.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful request payload sent to LTAQuotaService */
class QuotaServiceUpdateOrderRegistryRequestPayload
{
public:
  exDECLARE_EXCEPTION(QuotaServiceUpdateOrderRegistryRequestPayloadException,
                      QuotaServicePayloadsExceptions::RequestPayloadException);
  static const std::string REQUEST_PAYLOAD_KEY_REGISTRYUUID;
  static const std::string REQUEST_PAYLOAD_KEY_ORDERUUID;

public:
  /** Ctors/Dtors */
  QuotaServiceUpdateOrderRegistryRequestPayload();
  explicit QuotaServiceUpdateOrderRegistryRequestPayload(const std::string& jsstr);
  explicit QuotaServiceUpdateOrderRegistryRequestPayload(const nlohmann::json& json);
  QuotaServiceUpdateOrderRegistryRequestPayload(const QuotaServiceUpdateOrderRegistryRequestPayload& c);
  virtual ~QuotaServiceUpdateOrderRegistryRequestPayload() ACS_NOEXCEPT;

  /** Operators overload */
  QuotaServiceUpdateOrderRegistryRequestPayload& operator=(const QuotaServiceUpdateOrderRegistryRequestPayload& c);

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& jsstr);

  inline std::string getRegistryUUID() const
  {
    Lock l(_mutex);
    return _registryUUID;
  }

  inline std::string getOrderUUID() const
  {
    Lock l(_mutex);
    return _orderUUID;
  }

  inline void setRegistryUUID(const std::string& newVal)
  {
    Lock l(_mutex);
    _registryUUID = newVal;
  }

  inline void setOrderUUID(const std::string& newVal)
  {
    Lock l(_mutex);
    _orderUUID = newVal;
  }

private:
  Mutex _mutex;

  std::string _registryUUID;
  std::string _orderUUID;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(QuotaServiceUpdateOrderRegistryRequestPayload);
};

_ACS_END_NAMESPACE

#endif /* _QuotaServiceUpdateOrderRegistryRequestPayload_h_ */
