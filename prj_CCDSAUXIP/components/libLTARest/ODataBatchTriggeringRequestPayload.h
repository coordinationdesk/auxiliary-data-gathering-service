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

#ifndef _ODataBatchTriggeringRequestPayload_h_
#define _ODataBatchTriggeringRequestPayload_h_

#include <acs_c++config.hpp>
#include <unistd.h>
#include <string>

#include <ThreadSafe.h>
#include <json.hpp>
#include <Filterables.h>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful request payload sent to LTA Service in OData format */
class ODataBatchTriggeringRequestPayload
{
public:
  exDECLARE_EXCEPTION(ODataBatchTriggeringRequestPayloadException, exException);
  static const std::string REQUEST_PAYLOAD_KEY_PRIORITY;

public:
  /** Ctors/Dtors */
  ODataBatchTriggeringRequestPayload();
  explicit ODataBatchTriggeringRequestPayload(const std::string& jsstr);
  explicit ODataBatchTriggeringRequestPayload(const nlohmann::json& json);
  ODataBatchTriggeringRequestPayload(const ODataBatchTriggeringRequestPayload& c);
  virtual ~ODataBatchTriggeringRequestPayload() ACS_NOEXCEPT;

  /** Operators overload */
  ODataBatchTriggeringRequestPayload& operator=(const ODataBatchTriggeringRequestPayload& c);

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& jsstr);

  inline int64_t getPriority(bool* set = nullptr) const
  {
    Lock l(_mutex);
    if (nullptr != set)
    {
      *set = _prioritySet;
      return _priority;
    }
    else
    {
      if (_prioritySet)
      {
        return _priority;
      }
      else
      {
        ACS_THROW(ODataBatchTriggeringRequestPayloadException("Priority not set"));
      }
    }
  }

  inline void setPriority(const int64_t newVal)
  {
    Lock l(_mutex);
    _priority = newVal;
    _prioritySet = true;
  }

private:
  Mutex _mutex;

  bool _prioritySet;
  int64_t _priority;
  //TODO: Aggiungere gli altri campi (ad chiedere ICD completo)
  //NB: esiste un check per impedire di generare un oggetto json vuoto in toJSON. Da modificare se si aggiungono campi

  ACS_CLASS_DECLARE_DEBUG_LEVEL(ODataBatchTriggeringRequestPayload);
};

_ACS_END_NAMESPACE

#endif /* _ODataBatchTriggeringRequestPayload_h_ */
