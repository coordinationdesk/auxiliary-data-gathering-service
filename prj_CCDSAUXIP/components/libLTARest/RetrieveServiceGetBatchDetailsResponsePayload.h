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

#ifndef _RetrieveServiceGetBatchDetailsResponsePayload_h_
#define _RetrieveServiceGetBatchDetailsResponsePayload_h_

#include <acs_c++config.hpp>
#include <string>
#include <json.hpp>

#include <ThreadSafe.h>
#include <dbPersistent.h>

#include <RetrieveServiceResponseErrorCodeEnum.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful response payload sent by LTARetrieveService */
class RetrieveServiceGetBatchDetailsResponsePayload
{
public:
  /** This object implements the files_details list. */
  class FilesDetailsList
  {
  public:
    /** This object implements a files_detail entry */
    class FilesDetailsElement
    {
    public:
      exDECLARE_EXCEPTION(FilesDetailsElementException, exException);
      static const std::string RESPONSE_PAYLOAD_KEY_FILEID;
      static const std::string RESPONSE_PAYLOAD_KEY_STATUS;
      static const std::string RESPONSE_PAYLOAD_KEY_LTANAME;

    public:
      /** Ctors/Dtors */
      FilesDetailsElement();
      explicit FilesDetailsElement(const std::string& jsstr);
      explicit FilesDetailsElement(const nlohmann::json& json);
      FilesDetailsElement(const FilesDetailsElement& c);
      virtual ~FilesDetailsElement() ACS_NOEXCEPT;

      /** Operators overload */
      FilesDetailsElement& operator=(const FilesDetailsElement& c);

      /** Returns the serialized json representation of this object */
      std::string toJSON() const;
      nlohmann::json toJSONObject() const;

      /** Initialize this object from the json string in input */
      void fromJSONObject(const nlohmann::json& json);
      void fromJSON(const std::string& jsstr);

      inline dbPersistent::IdType getFileID() const
      {
        Lock l(_mutex);
        return _fileID;
      }

      inline std::string getStatus() const
      {
        Lock l(_mutex);
        return _status;
      }

      inline std::string getLTAName() const
      {
        Lock l(_mutex);
        return _ltaName;
      }

      inline void setFileID(dbPersistent::IdType newVal)
      {
        Lock l(_mutex);
        _fileID = newVal;
      }

      inline void setStatus(const std::string& newVal)
      {
        Lock l(_mutex);
        _status = newVal;
      }

      inline void setLTAName(const std::string& newVal)
      {
        Lock l(_mutex);
        _ltaName = newVal;
      }

    private:
      Mutex _mutex;

      dbPersistent::IdType _fileID;
      std::string _status;
      std::string _ltaName;

      ACS_CLASS_DECLARE_DEBUG_LEVEL(FilesDetailsElement);
    };

  public:
    exDECLARE_EXCEPTION(FilesDetailsListException, exException);

  public:
    /** Ctors / Dtors */
    FilesDetailsList();
    explicit FilesDetailsList(const std::string& jsstr);
    explicit FilesDetailsList(const nlohmann::json& js);
    FilesDetailsList(const FilesDetailsList&);
    virtual ~FilesDetailsList() ACS_NOEXCEPT;

    /** Access operators overload */
    const FilesDetailsElement& operator[](size_t idx) const;
    FilesDetailsElement& operator[](size_t idx);

    /** Returns the filesdetails vector. */
    inline const std::vector<FilesDetailsElement>& getFilesDetails() const
    {
      ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
      Lock l(_mutex);
      return _filesDetails;
    }

    /** Set a new filesdetails vector. */
    inline void setFilesDetails(const std::vector<FilesDetailsElement>& newVal)
    {
      ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
      Lock l(_mutex);
      _filesDetails = newVal;
    }

    /** Inserts the payload in the list */
    void add(const FilesDetailsElement& payload);
    void add(const FilesDetailsList& payload);
    void add(const std::vector<FilesDetailsElement>& payload);

    /** In-place sort function. Sorts the payloads with respect to their file_id */
    void sort();
    /** Out-of-place sort function. Sorts the payloads with respect to their file_id */
    void sort(std::vector<FilesDetailsElement>&) const;

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
    std::vector<FilesDetailsElement> _filesDetails;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(FilesDetailsList);
  };

public:
  exDECLARE_EXCEPTION(RetrieveServiceGetBatchDetailsResponsePayloadException, exException);
  static const std::string RESPONSE_PAYLOAD_KEY_STATUS;
  static const std::string RESPONSE_PAYLOAD_KEY_FILESDETAILS;
  static const std::string RESPONSE_PAYLOAD_KEY_ERRORMSG;
  static const std::string RESPONSE_PAYLOAD_KEY_ERRORCODE;

public:
  /** Ctors/Dtors */
  RetrieveServiceGetBatchDetailsResponsePayload();
  explicit RetrieveServiceGetBatchDetailsResponsePayload(const std::string& jsstr);
  explicit RetrieveServiceGetBatchDetailsResponsePayload(const nlohmann::json& json);
  RetrieveServiceGetBatchDetailsResponsePayload(const RetrieveServiceGetBatchDetailsResponsePayload&);
  virtual ~RetrieveServiceGetBatchDetailsResponsePayload() ACS_NOEXCEPT;

  /** Operators overload */
  RetrieveServiceGetBatchDetailsResponsePayload& operator=(const RetrieveServiceGetBatchDetailsResponsePayload&);

  inline std::string getStatus() const
  {
    Lock l(_mutex);
    return _status;
  }

  inline FilesDetailsList getFilesDetails() const
  {
    Lock l(_mutex);
    return _filesDetails;
  }

  const FilesDetailsList& filesDetails = _filesDetails;

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
      ACS_COND_THROW(!_errorCodeSet, RetrieveServiceGetBatchDetailsResponsePayloadException(
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

  inline void setFilesDetails(const FilesDetailsList& newVal)
  {
    Lock l(_mutex);
    _filesDetails = newVal;
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
  FilesDetailsList _filesDetails;
  std::string _errormsg;
  RetrieveServiceResponseErrorCode _errorCode;
  bool _errorCodeSet;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(RetrieveServiceGetBatchDetailsResponsePayload);
};

_ACS_END_NAMESPACE

#endif /* _RetrieveServiceGetBatchDetailsResponsePayload_h_ */
