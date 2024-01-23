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

#ifndef _RetrieveServiceGetBulkDetailsResponsePayload_h_
#define _RetrieveServiceGetBulkDetailsResponsePayload_h_

#include <acs_c++config.hpp>
#include <string>
#include <json.hpp>

#include <ThreadSafe.h>
#include <dbPersistent.h>

#include <RetrieveServiceResponseErrorCodeEnum.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful response payload sent by LTARetrieveService */
class RetrieveServiceGetBulkDetailsResponsePayload
{
public:
  /** This object implements the batches_details list. */
  class BatchesDetailsList
  {
  public:
    /** This object implements a batches_detail entry */
    class BatchesDetailsElement
    {
    public:
      exDECLARE_EXCEPTION(BatchesDetailsElementException, exException);
      static const std::string RESPONSE_PAYLOAD_KEY_BATCHID;
      static const std::string RESPONSE_PAYLOAD_KEY_STATUS;

    public:
      /** Ctors/Dtors */
      BatchesDetailsElement();
      explicit BatchesDetailsElement(const std::string& jsstr);
      explicit BatchesDetailsElement(const nlohmann::json& json);
      BatchesDetailsElement(const BatchesDetailsElement& c);
      virtual ~BatchesDetailsElement() ACS_NOEXCEPT;

      /** Operators overload */
      BatchesDetailsElement& operator=(const BatchesDetailsElement& c);

      /** Returns the serialized json representation of this object */
      std::string toJSON() const;
      nlohmann::json toJSONObject() const;

      /** Initialize this object from the json string in input */
      void fromJSONObject(const nlohmann::json& json);
      void fromJSON(const std::string& jsstr);

      inline dbPersistent::IdType getBatchID() const
      {
        Lock l(_mutex);
        return _batchID;
      }

      inline std::string getStatus() const
      {
        Lock l(_mutex);
        return _status;
      }

      inline void setBatchID(dbPersistent::IdType newVal)
      {
        Lock l(_mutex);
        _batchID = newVal;
      }

      inline void setStatus(const std::string& newVal)
      {
        Lock l(_mutex);
        _status = newVal;
      }

    private:
      Mutex _mutex;

      dbPersistent::IdType _batchID;
      std::string _status;

      ACS_CLASS_DECLARE_DEBUG_LEVEL(BatchesDetailsElement);
    };

  public:
    exDECLARE_EXCEPTION(BatchesDetailsListException, exException);

  public:
    /** Ctors / Dtors */
    BatchesDetailsList();
    explicit BatchesDetailsList(const std::string& jsstr);
    explicit BatchesDetailsList(const nlohmann::json& js);
    BatchesDetailsList(const BatchesDetailsList&);
    virtual ~BatchesDetailsList() ACS_NOEXCEPT;

    /** Access operators overload */
    const BatchesDetailsElement& operator[](size_t idx) const;
    BatchesDetailsElement& operator[](size_t idx);

    /** Returns the batchesdetails vector. */
    inline const std::vector<BatchesDetailsElement>& getBatchesDetails() const
    {
      ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
      Lock l(_mutex);
      return _batchesDetails;
    }

    /** Set a new batchesdetails vector. */
    inline void setBatchesDetails(const std::vector<BatchesDetailsElement>& newVal)
    {
      ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
      Lock l(_mutex);
      _batchesDetails = newVal;
    }

    /** Inserts the payload in the list */
    void add(const BatchesDetailsElement& payload);
    void add(const BatchesDetailsList& payload);
    void add(const std::vector<BatchesDetailsElement>& payload);

    /** In-place sort function. Sorts the payloads with respect to their batch_id */
    void sort();
    /** Out-of-place sort function. Sorts the payloads with respect to their batch_id */
    void sort(std::vector<BatchesDetailsElement>&) const;

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
    std::vector<BatchesDetailsElement> _batchesDetails;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(BatchesDetailsList);
  };

public:
  exDECLARE_EXCEPTION(RetrieveServiceGetBulkDetailsResponsePayloadException, exException);
  static const std::string RESPONSE_PAYLOAD_KEY_STATUS;
  static const std::string RESPONSE_PAYLOAD_KEY_BATCHESDETAILS;
  static const std::string RESPONSE_PAYLOAD_KEY_ERRORMSG;
  static const std::string RESPONSE_PAYLOAD_KEY_ERRORCODE;

public:
  /** Ctors/Dtors */
  RetrieveServiceGetBulkDetailsResponsePayload();
  explicit RetrieveServiceGetBulkDetailsResponsePayload(const std::string& jsstr);
  explicit RetrieveServiceGetBulkDetailsResponsePayload(const nlohmann::json& json);
  RetrieveServiceGetBulkDetailsResponsePayload(const RetrieveServiceGetBulkDetailsResponsePayload&);
  virtual ~RetrieveServiceGetBulkDetailsResponsePayload() ACS_NOEXCEPT;

  /** Operators overload */
  RetrieveServiceGetBulkDetailsResponsePayload& operator=(const RetrieveServiceGetBulkDetailsResponsePayload&);

  inline std::string getStatus() const
  {
    Lock l(_mutex);
    return _status;
  }

  inline BatchesDetailsList getBatchesDetails() const
  {
    Lock l(_mutex);
    return _batchesDetails;
  }

  const BatchesDetailsList& batchesDetails = _batchesDetails;

  inline std::string getErrormsg() const
  {
    Lock l(_mutex);
    return _errormsg;
  }

  inline RetrieveServiceResponseErrorCode getErrorCode(bool* set = nullptr) const
  {
    Lock l(_mutex);
    if (nullptr == set)
    {
      ACS_COND_THROW(!_errorCodeSet, RetrieveServiceGetBulkDetailsResponsePayloadException(
                                       RESPONSE_PAYLOAD_KEY_ERRORCODE + " not set"));
    }
    else
    {
      *set = _errorCodeSet;
    }

    return _errorCode;
  }

  inline void setStatus(const std::string& newVal)
  {
    Lock l(_mutex);
    _status = newVal;
  }

  inline void setBatchesDetails(const BatchesDetailsList& newVal)
  {
    Lock l(_mutex);
    _batchesDetails = newVal;
  }

  inline void setErrormsg(const std::string& newVal)
  {
    Lock l(_mutex);
    _errormsg = newVal;
  }

  inline void setErrorCode(const RetrieveServiceResponseErrorCode& newVal)
  {
    Lock l(_mutex);
    _errorCode = newVal;
    _errorCodeSet = true;
  }

  inline void unsetErrorCode()
  {
    Lock l(_mutex);
    _errorCodeSet = false;
  }

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& json);

private:
  Mutex _mutex;
  std::string _status;
  BatchesDetailsList _batchesDetails;
  std::string _errormsg;
  RetrieveServiceResponseErrorCode _errorCode;
  bool _errorCodeSet;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(RetrieveServiceGetBulkDetailsResponsePayload);
};

_ACS_END_NAMESPACE

#endif /* _RetrieveServiceGetBulkDetailsResponsePayload_h_ */
