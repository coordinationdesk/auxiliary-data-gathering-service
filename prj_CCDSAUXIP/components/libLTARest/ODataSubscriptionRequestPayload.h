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

#ifndef _ODataSubscriptionRequestPayload_h_
#define _ODataSubscriptionRequestPayload_h_

#include <acs_c++config.hpp>
#include <unistd.h>
#include <string>

#include <ThreadSafe.h>
#include <json.hpp>
#include <Filterables.h>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful request payload sent to LTA Service in OData format */
class ODataSubscriptionRequestPayload
{
public:
  exDECLARE_EXCEPTION(ODataSubscriptionRequestPayloadException, exException);

  enum SubscriptionEvent 
  {
	unknown,
	created,
	deleted
  };

  static std::string subscriptionEvent2String(const SubscriptionEvent& se);
  static SubscriptionEvent string2SubscriptionEvent(const std::string& se);

  static const std::string REQUEST_PAYLOAD_KEY_ODATACONTEXT;
  static const std::string REQUEST_PAYLOAD_KEY_FILTERPARAM;
  static const std::string REQUEST_PAYLOAD_KEY_STAGEORDER;
  static const std::string REQUEST_PAYLOAD_KEY_NOTIFICATIONENDPOINT;
  static const std::string REQUEST_PAYLOAD_KEY_NOTIFICATIONEPUSERNAME;
  static const std::string REQUEST_PAYLOAD_KEY_NOTIFICATIONEPPASSWORD;
  static const std::string REQUEST_PAYLOAD_KEY_PRIORITY;
  static const std::string REQUEST_PAYLOAD_KEY_EVENT;

  static const std::string REQUEST_PAYLOAD_DEFAULT_ODATACONTEXT;

public:
  /** Ctors/Dtors */
  ODataSubscriptionRequestPayload();
  explicit ODataSubscriptionRequestPayload(const std::string& jsstr);
  explicit ODataSubscriptionRequestPayload(const nlohmann::json& json);
  ODataSubscriptionRequestPayload(const ODataSubscriptionRequestPayload& c);
  virtual ~ODataSubscriptionRequestPayload() ACS_NOEXCEPT;

  /** Operators overload */
  ODataSubscriptionRequestPayload& operator=(const ODataSubscriptionRequestPayload& c);

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& jsstr);

  inline std::string getODataContext() const
  {
    Lock l(_mutex);
    return _oDataContext;
  }

  inline std::string getFilterParam() const
  {
    Lock l(_mutex);
    return _filterParam;
  }

  inline bool getStageOrder() const
  {
    Lock l(_mutex);
    return _stageOrder;
  }

  inline std::string getNotificationEndpoint(bool* set = nullptr) const
  {
    Lock l(_mutex);
    if (nullptr != set)
    {
      *set = _notificationEndpointSet;
      return _notificationEndpoint;
    }
    else
    {
      if (_notificationEndpointSet)
      {
        return _notificationEndpoint;
      }
      else
      {
        ACS_THROW(ODataSubscriptionRequestPayloadException("NotificationEndpoint not set"));
      }
    }
  }

  inline std::string getNotificationEpUsername(bool* set = nullptr) const
  {
    Lock l(_mutex);
    if (nullptr != set)
    {
      *set = _notificationEpUsernameSet;
      return _notificationEpUsername;
    }
    else
    {
      if (_notificationEpUsernameSet)
      {
        return _notificationEpUsername;
      }
      else
      {
        ACS_THROW(ODataSubscriptionRequestPayloadException("NotificationEpUsername not set"));
      }
    }
  }

  inline std::string getNotificationEpPassword(bool* set = nullptr) const
  {
    Lock l(_mutex);
    if (nullptr != set)
    {
      *set = _notificationEpPasswordSet;
      return _notificationEpPassword;
    }
    else
    {
      if (_notificationEpPasswordSet)
      {
        return _notificationEpPassword;
      }
      else
      {
        ACS_THROW(ODataSubscriptionRequestPayloadException("NotificationEpPassword not set"));
      }
    }
  }
  inline SubscriptionEvent getSubscriptionEvent(bool* set = nullptr) const
  {
    Lock l(_mutex);
    if (nullptr != set)
    {
      *set = _eventSet;
      return _event;
    }
    else
    {
      if (_eventSet)
      {
        return _event;
      }
      else
      {
        ACS_THROW(ODataSubscriptionRequestPayloadException("SubscriptionEvent not set"));
      }
    }
  }

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
        ACS_THROW(ODataSubscriptionRequestPayloadException("Priority not set"));
      }
    }
  }

  inline void setODataContext(const std::string& newVal)
  {
    Lock l(_mutex);
    ACS_LOG_WARNING("Value of \"" << REQUEST_PAYLOAD_KEY_ODATACONTEXT << "\" tag changed to \"" << newVal << "\"");
    _oDataContext = newVal;
  }

  inline void setFilterParam(const std::string& newVal)
  {
    Lock l(_mutex);
    _filterParam = newVal;
  }

  inline void setStageOrder(const bool& newVal)
  {
    Lock l(_mutex);
    _stageOrder = newVal;
  }

  inline void setNotificationEndpoint(const std::string& newVal)
  {
    Lock l(_mutex);
    _notificationEndpoint = newVal;
    _notificationEndpointSet = true;
  }

  inline void setNotificationEpUsername(const std::string& newVal)
  {
    Lock l(_mutex);
    _notificationEpUsername = newVal;
    _notificationEpUsernameSet = true;
  }

  inline void setNotificationEpPassword(const std::string& newVal)
  {
    Lock l(_mutex);
    _notificationEpPassword = newVal;
    _notificationEpPasswordSet = true;
  }

  inline void setPriority(const int64_t newVal)
  {
    Lock l(_mutex);
    _priority = newVal;
    _prioritySet = true;
  }
  inline void setSubscriptionEvent(const SubscriptionEvent & event)
  {
    Lock l(_mutex);
    _event = event;
    _eventSet = true;
  }

private:
  Mutex _mutex;

  std::string _oDataContext;
  std::string _filterParam;
  bool _stageOrder;
  bool _eventSet;
  SubscriptionEvent _event;
  bool _notificationEndpointSet;
  std::string _notificationEndpoint;
  bool _notificationEpUsernameSet;
  std::string _notificationEpUsername;
  bool _notificationEpPasswordSet;
  std::string _notificationEpPassword;
  bool _prioritySet;
  int64_t _priority;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(ODataSubscriptionRequestPayload);
};

_ACS_END_NAMESPACE

#endif /* _ODataSubscriptionRequestPayload_h_ */
