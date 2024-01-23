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

#ifndef _LTAUploadTargetAIPResponsePayload_h_
#define _LTAUploadTargetAIPResponsePayload_h_

#include <acs_c++config.hpp>

#include <json.hpp>
#include <Filterables.h>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful response payload sent by LTAQuotaService */
class LTAUploadTargetAIPResponsePayload
{
public:
  exDECLARE_EXCEPTION(LTAUploadTargetAIPResponsePayloadException, exException);

  /** Ctors/Dtors */
  LTAUploadTargetAIPResponsePayload() = default;
  explicit LTAUploadTargetAIPResponsePayload(const std::string& jsstr);
  explicit LTAUploadTargetAIPResponsePayload(const nlohmann::json& json);
  LTAUploadTargetAIPResponsePayload(const LTAUploadTargetAIPResponsePayload& c) = default;
  virtual ~LTAUploadTargetAIPResponsePayload() noexcept = default;

  /** Operators overload */
  LTAUploadTargetAIPResponsePayload& operator=(const LTAUploadTargetAIPResponsePayload& c) = default;

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& jsstr);

  inline dbPersistent::IdType getAipID() const
  {
    return _aipID;
  }
  inline void setAipID(const dbPersistent::IdType val)
  {
    _aipID = val;
  }

  inline const std::string& getMessage(bool* set = nullptr) const
  {
    if (set != nullptr)
    {
      *set = _messageSet;
    }
    return _message;
  }
  inline void setMessage(const std::string& val)
  {
    _messageSet = true;
    _message = val;
  }
  inline void unsetMessage()
  {
    _messageSet = false;
    _message = "";
  }

private:
  dbPersistent::IdType _aipID{ 0 };
  std::string _message{ "" };
  bool _messageSet{ false };

  ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAUploadTargetAIPResponsePayload);
};

_ACS_END_NAMESPACE

#endif /* _LTAUploadTargetAIPResponsePayload_h_ */
