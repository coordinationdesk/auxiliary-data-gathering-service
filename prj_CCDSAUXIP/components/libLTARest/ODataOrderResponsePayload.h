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

#ifndef _ODataOrderResponsePayload_h_
#define _ODataOrderResponsePayload_h_

#include <acs_c++config.hpp>
#include <unistd.h>
#include <string>
#include <map>

#include <ThreadSafe.h>
#include <json.hpp>
#include <Filterables.h>
#include <LTACommon.h>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful response payload sent by LTA Service in OData format */
class ODataOrderResponsePayload
{
public:
  exDECLARE_EXCEPTION(ODataOrderResponsePayloadException, exException);
  static const std::string RESPONSE_PAYLOAD_KEY_ODATACONTEXT;
  static const std::string RESPONSE_PAYLOAD_KEY_ID;
  static const std::string RESPONSE_PAYLOAD_KEY_STATUS;

  static const std::string RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT;

public:
  /** Ctors/Dtors */
  ODataOrderResponsePayload();
  explicit ODataOrderResponsePayload(const std::string& jsstr);
  explicit ODataOrderResponsePayload(const nlohmann::json& json);
  ODataOrderResponsePayload(const ODataOrderResponsePayload&);
  virtual ~ODataOrderResponsePayload() ACS_NOEXCEPT;

  /** Operators overload */
  ODataOrderResponsePayload& operator=(const ODataOrderResponsePayload&);

  inline std::string getID() const
  {
    Lock l(_mutex);
    return _id;
  }

  inline std::string getODataContext() const
  {
    Lock l(_mutex);
    return _oDataContext;
  }

  inline lta::OrderStatus getStatus() const
  {
    Lock l(_mutex);
    return _status;
  }

  inline void setODataContext(const std::string& newVal)
  {
    Lock l(_mutex);
    ACS_LOG_WARNING("Value of \"" << RESPONSE_PAYLOAD_KEY_ODATACONTEXT << "\" tag changed to \"" << newVal << "\"");
    _oDataContext = newVal;
  }

  inline void setID(const std::string& newVal)
  {
    Lock l(_mutex);
    _id = newVal;
  }

  inline void setStatus(const lta::OrderStatus newVal)
  {
    Lock l(_mutex);
    _status = newVal;
  }

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& json);

private:
  Mutex _mutex;
  std::string _oDataContext;
  std::string _id;
  lta::OrderStatus _status;
  //TODO: Aggiungere gli altri campi di "@odata.context": "$metadata#Orders"

  ACS_CLASS_DECLARE_DEBUG_LEVEL(ODataOrderResponsePayload);
};

_ACS_END_NAMESPACE

#endif /* _ODataOrderResponsePayload_h_ */
