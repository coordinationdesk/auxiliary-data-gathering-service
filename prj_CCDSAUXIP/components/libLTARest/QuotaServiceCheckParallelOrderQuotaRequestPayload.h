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

#ifndef _QuotaServiceCheckParallelOrderQuotaRequestPayload_h_
#define _QuotaServiceCheckParallelOrderQuotaRequestPayload_h_

#include <acs_c++config.hpp>
#include <unistd.h>
#include <string>

#include <ThreadSafe.h>
#include <json.hpp>
#include <Filterables.h>

#include <QuotaServicePayloadsExceptions.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful request payload sent to LTAQuotaService */
class QuotaServiceCheckParallelOrderQuotaRequestPayload
{
public:
  exDECLARE_EXCEPTION(QuotaServiceCheckParallelOrderQuotaRequestPayloadException,
                      QuotaServicePayloadsExceptions::RequestPayloadException);
  static const std::string REQUEST_PAYLOAD_KEY_ORDERUUID;

public:
  /** Ctors/Dtors */
  QuotaServiceCheckParallelOrderQuotaRequestPayload();
  explicit QuotaServiceCheckParallelOrderQuotaRequestPayload(const std::string& jsstr);
  explicit QuotaServiceCheckParallelOrderQuotaRequestPayload(const nlohmann::json& json);
  QuotaServiceCheckParallelOrderQuotaRequestPayload(const QuotaServiceCheckParallelOrderQuotaRequestPayload& c);
  virtual ~QuotaServiceCheckParallelOrderQuotaRequestPayload() ACS_NOEXCEPT;

  /** Operators overload */
  QuotaServiceCheckParallelOrderQuotaRequestPayload& operator=(const QuotaServiceCheckParallelOrderQuotaRequestPayload& c);

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& jsstr);

  inline std::string getOrderUUID() const
  {
    Lock l(_mutex);
    return _orderUUID;
  }

  inline void setOrderUUID(const std::string& newVal)
  {
    Lock l(_mutex);
    _orderUUID = newVal;
  }

private:
  Mutex _mutex;

  std::string _orderUUID;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(QuotaServiceCheckParallelOrderQuotaRequestPayload);
};

_ACS_END_NAMESPACE

#endif /* _QuotaServiceCheckParallelOrderQuotaRequestPayload_h_ */
