/*

  Copyright 2022, Exprivia - DFDA-AS
  Via Della Bufalotta, 378 - 00139 Roma - Italy
  http://www.exprivia.it

  All Rights Reserved.

  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
  the contents of this file may not be disclosed to third parties, copied or
  duplicated in any form, in whole or in part, without the prior written
  permission of Exprivia SpA

  $Prod: LTA Rest Interface $

*/

#ifndef _DirectDLResolverResolveUUIDResponsePayload_h_
#define _DirectDLResolverResolveUUIDResponsePayload_h_

#include <acs_c++config.hpp>

#include <json.hpp>
#include <Filterables.h>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful response payload sent by LTAQuotaService */
class DirectDLResolverResolveUUIDResponsePayload
{
public:
  exDECLARE_EXCEPTION(DirectDLResolverResolveUUIDResponsePayloadException, exException);

  /** Ctors/Dtors */
  DirectDLResolverResolveUUIDResponsePayload() = default;
  explicit DirectDLResolverResolveUUIDResponsePayload(const std::string& jsstr);
  explicit DirectDLResolverResolveUUIDResponsePayload(const nlohmann::json& json);
  DirectDLResolverResolveUUIDResponsePayload(const DirectDLResolverResolveUUIDResponsePayload& c) = default;
  virtual ~DirectDLResolverResolveUUIDResponsePayload() noexcept = default;

  /** Operators overload */
  DirectDLResolverResolveUUIDResponsePayload& operator=(const DirectDLResolverResolveUUIDResponsePayload& c) = default;

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& jsstr);

  inline const std::string& getDownloadURL() const
  {
    return _downloadURL;
  }
  inline void setDownloadURL(const std::string& val)
  {
    _downloadURL = val;
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
  std::string _downloadURL{ "" };
  std::string _message{ "" };
  bool _messageSet{ false };

  ACS_CLASS_DECLARE_DEBUG_LEVEL(DirectDLResolverResolveUUIDResponsePayload);
};

_ACS_END_NAMESPACE

#endif /* _DirectDLResolverResolveUUIDResponsePayload_h_ */
