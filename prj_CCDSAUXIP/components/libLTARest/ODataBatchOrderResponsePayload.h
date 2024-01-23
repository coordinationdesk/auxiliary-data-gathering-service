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

#ifndef _ODataBatchOrderResponsePayload_h_
#define _ODataBatchOrderResponsePayload_h_

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
class ODataBatchOrderResponsePayload
{
public:
  /** This object implements the batches_details list. */
  class BatchOrderValueList
  {
  public:
    /** This object implements a batches_detail entry */
    class BatchOrderValueElement
    {
    public:
      exDECLARE_EXCEPTION(BatchOrderValueElementException, exException);
      static const std::string RESPONSE_PAYLOAD_KEY_ID;
      static const std::string RESPONSE_PAYLOAD_KEY_STATUS;

    public:
      /** Ctors/Dtors */
      BatchOrderValueElement();
      explicit BatchOrderValueElement(const std::string& jsstr);
      explicit BatchOrderValueElement(const nlohmann::json& json);
      BatchOrderValueElement(const BatchOrderValueElement&);
      virtual ~BatchOrderValueElement() ACS_NOEXCEPT;

      /** Operators overload */
      BatchOrderValueElement& operator=(const BatchOrderValueElement&);

      inline std::string getID() const
      {
        Lock l(_mutex);
        return _id;
      }

      inline lta::OrderStatus getStatus() const
      {
        Lock l(_mutex);
        return _status;
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
      std::string _id;
      lta::OrderStatus _status;
      //TODO: Aggiungere gli altri campi di "@odata.context": "$metadata#BatchOrder/$entity"

      ACS_CLASS_DECLARE_DEBUG_LEVEL(BatchOrderValueElement);
    };

  public:
    exDECLARE_EXCEPTION(BatchOrderValueListException, exException);

  public:
    /** Ctors / Dtors */
    BatchOrderValueList();
    explicit BatchOrderValueList(const std::string& jsstr);
    explicit BatchOrderValueList(const nlohmann::json& js);
    BatchOrderValueList(const BatchOrderValueList&);
    virtual ~BatchOrderValueList() ACS_NOEXCEPT;

    /** Access operators overload */
    const BatchOrderValueElement& operator[](size_t idx) const;
    BatchOrderValueElement& operator[](size_t idx);

    /** Returns the value vector. */
    inline const std::vector<BatchOrderValueElement>& getList() const
    {
      ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
      Lock l(_mutex);
      return _value;
    }

    /** Set a new value vector. */
    inline void setList(const std::vector<BatchOrderValueElement>& newVal)
    {
      ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
      Lock l(_mutex);
      _value = newVal;
    }

    /** Inserts the payload in the list */
    void add(const BatchOrderValueElement& payload);
    void add(const BatchOrderValueList& payload);
    void add(const std::vector<BatchOrderValueElement>& payload);

    /** In-place sort function. Sorts the payloads with respect to their batch_id */
    void sort();
    /** Out-of-place sort function. Sorts the payloads with respect to their batch_id */
    void sort(std::vector<BatchOrderValueElement>&) const;

    /** Clears the payload list */
    void clear();

    /** Returns the size of the list */
    size_t size() const;

    /** Returns the serialized json representation of this object */
    std::string toJSON() const;
    /** Returns the nlohmann object associated to this list */
    nlohmann::json toJSONObject() const;

    /** Initialize this object from the nlohmann json object in input */
    void fromJSONObject(const nlohmann::json& json);
    /** Initialize this object from the json string in input */
    void fromJSON(const std::string& json);

  private:
    Mutex _mutex;
    std::vector<BatchOrderValueElement> _value;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(BatchOrderValueList);
  };

public:
  exDECLARE_EXCEPTION(ODataBatchOrderResponsePayloadException, exException);
  static const std::string RESPONSE_PAYLOAD_KEY_ODATACONTEXT;
  static const std::string RESPONSE_PAYLOAD_KEY_VALUE;
  static const std::string RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT;

public:
  /** Ctors/Dtors */
  ODataBatchOrderResponsePayload();
  explicit ODataBatchOrderResponsePayload(const std::string& jsstr);
  explicit ODataBatchOrderResponsePayload(const nlohmann::json& json);
  ODataBatchOrderResponsePayload(const ODataBatchOrderResponsePayload&);
  virtual ~ODataBatchOrderResponsePayload() ACS_NOEXCEPT;

  /** Operators overload */
  ODataBatchOrderResponsePayload& operator=(const ODataBatchOrderResponsePayload&);

  inline std::string getODataContext() const
  {
    Lock l(_mutex);
    return _oDataContext;
  }

  inline BatchOrderValueList getValue() const
  {
    Lock l(_mutex);
    return _value;
  }

  const BatchOrderValueList& value = _value;

  inline void setODataContext(const std::string& newVal)
  {
    Lock l(_mutex);
    ACS_LOG_WARNING("Value of \"" << RESPONSE_PAYLOAD_KEY_ODATACONTEXT << "\" tag changed to \"" << newVal << "\"");
    _oDataContext = newVal;
  }

  inline void setValue(const BatchOrderValueList& newVal)
  {
    Lock l(_mutex);
    _value = newVal;
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
  BatchOrderValueList _value;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(ODataBatchOrderResponsePayload);
};

_ACS_END_NAMESPACE

#endif /* _ODataBatchOrderResponsePayload_h_ */
