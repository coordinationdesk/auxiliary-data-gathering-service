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

#ifndef _LTAAvailableDownloadAIPsResponsePayload_h_
#define _LTAAvailableDownloadAIPsResponsePayload_h_

#include <acs_c++config.hpp>

#include <json.hpp>
#include <Filterables.h>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful response payload sent by LTAQuotaService */
class LTAAvailableDownloadAIPsResponsePayload
{
public:
  exDECLARE_EXCEPTION(LTAAvailableDownloadAIPsResponsePayloadException, exException);

  /** Ctors/Dtors */
  LTAAvailableDownloadAIPsResponsePayload() = default;
  explicit LTAAvailableDownloadAIPsResponsePayload(const std::string& jsstr);
  explicit LTAAvailableDownloadAIPsResponsePayload(const nlohmann::json& json);
  LTAAvailableDownloadAIPsResponsePayload(const LTAAvailableDownloadAIPsResponsePayload& c) = default;
  virtual ~LTAAvailableDownloadAIPsResponsePayload() noexcept = default;

  /** Operators overload */
  LTAAvailableDownloadAIPsResponsePayload& operator=(const LTAAvailableDownloadAIPsResponsePayload& c) = default;

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& jsstr);

  inline const std::vector<dbPersistent::IdType>& getAipIDs() const
  {
    return _aipIDs;
  }
  inline dbPersistent::IdType getAipID(const size_t n) const
  {
    return _aipIDs.at(n);
  }
  inline size_t getAipIDsSize() const
  {
    return _aipIDs.size();
  }
  inline bool getAipIDsEmpty() const
  {
    return _aipIDs.empty();
  }
  inline void setAipIDs(const std::vector<dbPersistent::IdType>& val)
  {
    _aipIDs = val;
  }
  inline void addAipID(const dbPersistent::IdType val)
  {
    _aipIDs.push_back(val);
  }
  inline void clearAipIDs()
  {
    _aipIDs.clear();
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
  std::vector<dbPersistent::IdType> _aipIDs{};
  std::string _message{ "" };
  bool _messageSet{ false };

  ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAAvailableDownloadAIPsResponsePayload);
};

_ACS_END_NAMESPACE

#endif /* _LTAAvailableDownloadAIPsResponsePayload_h_ */
