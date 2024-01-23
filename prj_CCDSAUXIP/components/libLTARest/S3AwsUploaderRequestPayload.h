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

#ifndef _S3AwsUploaderRequestPayload_h_
#define _S3AwsUploaderRequestPayload_h_

#include <acs_c++config.hpp>
#include <DateTime.h>
#include <ThreadSafe.h>
#include <string>
#include <json.hpp>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful request sent to the LTANotificationServiceDispatcher */
class S3AwsUploaderRequestPayload {
public:
	
	exDECLARE_EXCEPTION(S3AwsUploaderRequestPayloadException, exException);

public:
	/** Ctors/Dtors */
	S3AwsUploaderRequestPayload();
	explicit S3AwsUploaderRequestPayload(const std::string& jsstr);
	explicit S3AwsUploaderRequestPayload(const nlohmann::json& js);
	S3AwsUploaderRequestPayload(const S3AwsUploaderRequestPayload&);
	virtual ~S3AwsUploaderRequestPayload() ACS_NOEXCEPT;
	
	/** Operators overload */
	S3AwsUploaderRequestPayload& operator=(const S3AwsUploaderRequestPayload&);
	
	/** Setters */
	inline void setJobId(const uint32_t& id) { 
		Lock l(_mutex);
		_jobId = id;
		_jobIdSet = true;
	}
	
	inline void setNextRetry(const time_t& v) { 
		Lock l(_mutex);
		_nextRetry = DateTime(v) ;
		_nextRetrySet = true;
	}

	inline void setNextRetry(const std::string& v) { 
		Lock l(_mutex);
		_nextRetry = DateTime(v) ;
		_nextRetrySet = true;
	}

	inline void setNextRetry(const DateTime& v) { 
		Lock l(_mutex);
		_nextRetry = v;
		_nextRetrySet = true;
	}
		
	inline void setRetries(const uint16_t& v) {
		Lock l(_mutex);
		_retries = v;
		_retriesSet = true;
	}

	inline void setPriority(const uint16_t& v) {
		Lock l(_mutex);
		_priority = v;
		_prioritySet = true;
	}

	inline void setOccupation(const uint64_t& v) {
		Lock l(_mutex);
		_occupation = v;
		_occupationSet = true;
	}
	
	inline void setUploadStatus(const std::string& v) {
		Lock l(_mutex);
		_uploadStatus = v;
		_uploadStatusSet = true;
	}
	
	inline void setLocalPath(const std::string& v) {
		Lock l(_mutex);
		_localPath = v;
		_localPathSet = true;
	}
	
	inline void setRemotePath(const std::string& v) {
		Lock l(_mutex);
		_remotePath = v;
		_remotePathSet = true;
	}

	inline void setHostname(const std::string& v) {
		Lock l(_mutex);
		_hostname = v;
		_hostnameSet = true;
	}

	inline void setLastErrorMsg(const std::string& v) {
		Lock l(_mutex);
		_lastErrorMsg = v;
		_lastErrorMsgSet = true;
	}

	inline void setAWS_S3_Provider(const std::string& v) {
		Lock l(_mutex);
		_aws_S3_Provider = v;
		_aws_S3_ProviderSet = true;	/* This is used as unique "set" for all the other S3 parameters */
	}

	inline void setAWS_S3_Key_AK(const std::string& v) {
		Lock l(_mutex); _aws_S3_Key_AK = v;
	}

	inline void setAWS_S3_Key_SK(const std::string& v) {
		Lock l(_mutex); _aws_S3_Key_SK = v;
	}

	inline void setAWS_S3_Upload_Pool_Size(const uint16_t& v) {
		Lock l(_mutex); _aws_S3_upload_pool_size = v;
	}

	inline void setAWS_S3_Upload_Buffer_Size(const uint64_t& v) {
		Lock l(_mutex); _aws_S3_upload_buffer_size = v;
	}

	inline void setAWS_S3_RemoteBasepath(const std::string& v) {
		Lock l(_mutex); _aws_S3_RemoteBasepath = v;
	}

	inline void setAWS_S3_Region(const std::string& v) {
		Lock l(_mutex); _aws_S3_Region = v;
	}

	inline void setAWS_S3_Endpoint(const std::string& v) {
		Lock l(_mutex); _aws_S3_Endpoint = v;
	}


	/** Getters */

	inline const uint32_t& getJobId(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _jobIdSet; }
		return _jobId;
	}
	
	inline const DateTime& getNextRetry(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _nextRetrySet; }
		return _nextRetry;
	}

	inline const uint16_t& getRetries(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _retriesSet; }
		return _retries;
	}

	inline const uint16_t& getPriority(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _prioritySet; }
		return _priority;
	}

	inline const uint64_t& getOccupation(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _occupationSet; }
		return _occupation;
	}
	
	inline std::string getUploadStatus(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _uploadStatusSet; }
		return _uploadStatus;
	}
		
	inline const std::string& getLocalPath(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _localPathSet; }
		return _localPath;
	}
	
	inline const std::string& getRemotePath(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _remotePathSet; }
		return _remotePath;
	}

	inline const std::string& getHostname(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _hostnameSet; }
		return _hostname;
	}

	inline const std::string& getLastErrorMsg(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _lastErrorMsgSet ; }
		return _lastErrorMsg;
	}
	
	inline const std::string& getAWS_S3_Provider(bool* isset = 0) const {
		/* _aws_S3_ProviderSet is used as unique "set" for all the other S3 parameters */
		Lock l(_mutex); if (isset != 0) { *isset = _aws_S3_ProviderSet ; }
		return _aws_S3_Provider;
	}
	
	inline const std::string& getAWS_S3_Key_AK(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _aws_S3_ProviderSet ; /* Pls. note */ }
		return _aws_S3_Key_AK ;
	}

	inline const std::string& getAWS_S3_Key_SK(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _aws_S3_ProviderSet ; /* Pls. note */ }
		return _aws_S3_Key_SK ;
	}

	inline const uint16_t& getAWS_S3_Upload_Pool_Size(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _aws_S3_ProviderSet ; /* Pls. note */ }
		return _aws_S3_upload_pool_size ;
	}

	inline const uint64_t& getAWS_S3_Upload_Buffer_Size(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _aws_S3_ProviderSet ; /* Pls. note */ }
		return _aws_S3_upload_buffer_size ;
	}

	inline const std::string& getAWS_S3_RemoteBasepath(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _aws_S3_ProviderSet ; /* Pls. note */ }
		return _aws_S3_RemoteBasepath ;
	}

	inline const std::string& getAWS_S3_Region(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _aws_S3_ProviderSet ; /* Pls. note */ }
		return _aws_S3_Region ;
	}

	inline const std::string& getAWS_S3_Endpoint(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _aws_S3_ProviderSet ; /* Pls. note */ }
		return _aws_S3_Endpoint ;
	}
	
	
	/** Returns the serialized json representation of this object */
	std::string toJSON() const;
	nlohmann::json toJSONObject() const;
	
	/** Initialize this object from the json string in input */
	void fromJSONObject(const nlohmann::json& json);
	void fromJSON(const std::string& json);
	
private:
	static const std::string REQUEST_PAYLOAD_KEY_JOB_ID ;
	static const std::string REQUEST_PAYLOAD_KEY_RETRIES ;
	static const std::string REQUEST_PAYLOAD_KEY_PRIORITY ;
	static const std::string REQUEST_PAYLOAD_KEY_OCCUPATION ;
	static const std::string REQUEST_PAYLOAD_KEY_NEXT_RETRY ;
	static const std::string REQUEST_PAYLOAD_KEY_LOCAL_PATH ;
	static const std::string REQUEST_PAYLOAD_KEY_REMOTE_PATH ;
	static const std::string REQUEST_PAYLOAD_KEY_HOSTNAME ;
	static const std::string REQUEST_PAYLOAD_KEY_UPLOAD_STATUS ;
	static const std::string REQUEST_PAYLOAD_KEY_LAST_ERROR_MESSAGE ;
	static const std::string REQUEST_PAYLOAD_KEY_AWS_S3_PROVIDER ;
	static const std::string REQUEST_PAYLOAD_KEY_AWS_S3_CRED_AK ;
	static const std::string REQUEST_PAYLOAD_KEY_AWS_S3_CRED_SK ;
	static const std::string REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_POOL_SZ ;
	static const std::string REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_BFFR_SZ ;
	static const std::string REQUEST_PAYLOAD_KEY_AWS_S3_REMOTE_BASEPATH ;
	static const std::string REQUEST_PAYLOAD_KEY_AWS_S3_REGION ;
	static const std::string REQUEST_PAYLOAD_KEY_AWS_S3_ENDPOINT ;
 
private:
	Mutex _mutex;
	
	bool _jobIdSet;
	bool _nextRetrySet;
	bool _retriesSet;
	bool _prioritySet;
	bool _uploadStatusSet;
	bool _localPathSet;
	bool _occupationSet;
	bool _remotePathSet;
	bool _hostnameSet;
	bool _lastErrorMsgSet ;
	bool _aws_S3_ProviderSet ;	/* This is used as unique "set" for all the other S3 parameters */
	
	uint32_t _jobId; 				
	DateTime _nextRetry;				
	uint16_t _retries;	
	uint16_t _priority;	
	std::string _uploadStatus;
	std::string _localPath;			
	uint64_t _occupation ;			
	std::string _remotePath;		
	std::string _hostname;		
	std::string _lastErrorMsg ;
	std::string _aws_S3_Provider ;
	std::string _aws_S3_Key_AK ;
	std::string _aws_S3_Key_SK ;
	uint16_t	_aws_S3_upload_pool_size ;
	uint64_t	_aws_S3_upload_buffer_size ;
	std::string _aws_S3_RemoteBasepath ;
	std::string _aws_S3_Region ;
	std::string _aws_S3_Endpoint ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(S3AwsUploaderRequestPayload);
};


bool operator==(const S3AwsUploaderRequestPayload &a, const S3AwsUploaderRequestPayload &b) ;

inline bool operator!=(const S3AwsUploaderRequestPayload &a, const S3AwsUploaderRequestPayload &b)  
{ return (!(a == b)) ; }


_ACS_END_NAMESPACE

#endif /* _S3AwsUploaderRequestPayload_h_ */
