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

#include <S3AwsUploaderRequestPayload.h>
#include <FtpCurl.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace curl;

ACS_CLASS_DEFINE_DEBUG_LEVEL(S3AwsUploaderRequestPayload);

const std::string S3AwsUploaderRequestPayload::REQUEST_PAYLOAD_KEY_JOB_ID = "JobId";
const std::string S3AwsUploaderRequestPayload::REQUEST_PAYLOAD_KEY_NEXT_RETRY = "NextRetry";
const std::string S3AwsUploaderRequestPayload::REQUEST_PAYLOAD_KEY_RETRIES= "Retries";
const std::string S3AwsUploaderRequestPayload::REQUEST_PAYLOAD_KEY_PRIORITY = "Priority";
const std::string S3AwsUploaderRequestPayload::REQUEST_PAYLOAD_KEY_OCCUPATION = "Occupation";
const std::string S3AwsUploaderRequestPayload::REQUEST_PAYLOAD_KEY_UPLOAD_STATUS = "UploadStatus";
const std::string S3AwsUploaderRequestPayload::REQUEST_PAYLOAD_KEY_LOCAL_PATH = "LocalPath";
const std::string S3AwsUploaderRequestPayload::REQUEST_PAYLOAD_KEY_REMOTE_PATH = "RemotePath";
const std::string S3AwsUploaderRequestPayload::REQUEST_PAYLOAD_KEY_HOSTNAME = "Hostname";
const std::string S3AwsUploaderRequestPayload::REQUEST_PAYLOAD_KEY_LAST_ERROR_MESSAGE = "LastErrorMsg" ;
const std::string S3AwsUploaderRequestPayload::REQUEST_PAYLOAD_KEY_AWS_S3_PROVIDER = "AWS_S3_Provider" ;
const std::string S3AwsUploaderRequestPayload::REQUEST_PAYLOAD_KEY_AWS_S3_CRED_AK = "AWS_S3_Key_AK" ;
const std::string S3AwsUploaderRequestPayload::REQUEST_PAYLOAD_KEY_AWS_S3_CRED_SK = "AWS_S3_Key_SK" ;
const std::string S3AwsUploaderRequestPayload::REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_POOL_SZ = "AWS_S3_Upload_Pool_Size" ;
const std::string S3AwsUploaderRequestPayload::REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_BFFR_SZ = "AWS_S3_Upload_Buffer_Size" ;
const std::string S3AwsUploaderRequestPayload::REQUEST_PAYLOAD_KEY_AWS_S3_REMOTE_BASEPATH = "AWS_S3_RemoteBasepath" ;
const std::string S3AwsUploaderRequestPayload::REQUEST_PAYLOAD_KEY_AWS_S3_REGION = "AWS_S3_Region" ;
const std::string S3AwsUploaderRequestPayload::REQUEST_PAYLOAD_KEY_AWS_S3_ENDPOINT = "AWS_S3_Endpoint" ;


S3AwsUploaderRequestPayload::S3AwsUploaderRequestPayload() :
	_mutex(),
	_jobIdSet(false),
	_nextRetrySet(false),
	_retriesSet(false),
	_prioritySet(false),
	_uploadStatusSet(false),
	_localPathSet(false),
	_occupationSet(false),
	_remotePathSet(false),
	_hostnameSet(false),
	_lastErrorMsgSet(false),
	_aws_S3_ProviderSet(false),	/* This is used as unique "set" for all the other S3 parameters */
	_jobId(0),
	_nextRetry(DateTime(time_t(0))),
	_retries(0),
	_priority(0),
	_uploadStatus(),
	_localPath(),
	_occupation(0),
	_remotePath(),
	_hostname(),
	_lastErrorMsg(),
	_aws_S3_Provider(),
	_aws_S3_Key_AK(),
	_aws_S3_Key_SK(),
	_aws_S3_upload_pool_size(0),
	_aws_S3_upload_buffer_size(0),
	_aws_S3_RemoteBasepath(),
	_aws_S3_Region(),
	_aws_S3_Endpoint()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}


S3AwsUploaderRequestPayload::S3AwsUploaderRequestPayload(const std::string& jsstr) :
	_mutex(),
	_jobIdSet(false),
	_nextRetrySet(false),
	_retriesSet(false),
	_prioritySet(false),
	_uploadStatusSet(false),
	_localPathSet(false),
	_occupationSet(false),
	_remotePathSet(false),
	_hostnameSet(false),
	_lastErrorMsgSet(false),
	_aws_S3_ProviderSet(false),	/* This is used as unique "set" for all the other S3 parameters */
	_jobId(0),
	_nextRetry(DateTime(time_t(0))),
	_retries(0),
	_priority(0),
	_uploadStatus(),
	_localPath(),
	_occupation(0),
	_remotePath(),
	_hostname(),
	_lastErrorMsg(),
	_aws_S3_Provider(),
	_aws_S3_Key_AK(),
	_aws_S3_Key_SK(),
	_aws_S3_upload_pool_size(0),
	_aws_S3_upload_buffer_size(0),
	_aws_S3_RemoteBasepath(),
	_aws_S3_Region(),
	_aws_S3_Endpoint()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSON(jsstr);
}

S3AwsUploaderRequestPayload::S3AwsUploaderRequestPayload(const nlohmann::json& js) :
	_mutex(),
	_jobIdSet(false),
	_nextRetrySet(false),
	_retriesSet(false),
	_prioritySet(false),
	_uploadStatusSet(false),
	_localPathSet(false),
	_occupationSet(false),
	_remotePathSet(false),
	_hostnameSet(false),
	_lastErrorMsgSet(false),
	_aws_S3_ProviderSet(false),	/* This is used as unique "set" for all the other S3 parameters */
	_jobId(0),
	_nextRetry(DateTime(time_t(0))),
	_retries(0),
	_priority(0),
	_uploadStatus(),
	_localPath(),
	_occupation(0),
	_remotePath(),
	_hostname(),
	_lastErrorMsg(),
	_aws_S3_Provider(),
	_aws_S3_Key_AK(),
	_aws_S3_Key_SK(),
	_aws_S3_upload_pool_size(0),
	_aws_S3_upload_buffer_size(0),
	_aws_S3_RemoteBasepath(),
	_aws_S3_Region(),
	_aws_S3_Endpoint()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSONObject(js);
}

S3AwsUploaderRequestPayload::S3AwsUploaderRequestPayload(const S3AwsUploaderRequestPayload& data) :
	_mutex(),
	_jobIdSet(data._jobIdSet),
	_nextRetrySet(data._nextRetrySet),
	_retriesSet(data._retriesSet),
	_prioritySet(data._prioritySet),
	_uploadStatusSet(data._uploadStatusSet),
	_localPathSet(data._localPathSet),
	_occupationSet(data._occupationSet),
	_remotePathSet(data._remotePathSet),
	_hostnameSet(data._hostnameSet),
	_lastErrorMsgSet(data._lastErrorMsgSet),
	_aws_S3_ProviderSet(data._aws_S3_ProviderSet),
	_jobId(data._jobId),
	_nextRetry(data._nextRetry),
	_retries(data._retries),
	_priority(data._priority),
	_uploadStatus(data._uploadStatus),
	_localPath(data._localPath),
	_occupation(data._occupation),
	_remotePath(data._remotePath),
	_hostname(data._hostname),
	_lastErrorMsg(data._lastErrorMsg),
	_aws_S3_Provider(data._aws_S3_Provider),
	_aws_S3_Key_AK(data._aws_S3_Key_AK),
	_aws_S3_Key_SK(data._aws_S3_Key_SK),
	_aws_S3_upload_pool_size(data._aws_S3_upload_pool_size),
	_aws_S3_upload_buffer_size(data._aws_S3_upload_buffer_size),
	_aws_S3_RemoteBasepath(data._aws_S3_RemoteBasepath),
	_aws_S3_Region(data._aws_S3_Region),
	_aws_S3_Endpoint(data._aws_S3_Endpoint)
{ }

S3AwsUploaderRequestPayload::~S3AwsUploaderRequestPayload() ACS_NOEXCEPT { }

S3AwsUploaderRequestPayload& S3AwsUploaderRequestPayload::operator=(const S3AwsUploaderRequestPayload& data) {
	if (this != &data) {
		bool toSet = true;
		while (toSet) {
			Lock l1(_mutex);
			TryLock l2(data._mutex);
			if (l2.locked()) {
				_jobIdSet = data._jobIdSet;
				_nextRetrySet = data._nextRetrySet;
				_retriesSet = data._retriesSet;
				_prioritySet = data._prioritySet;
				_uploadStatusSet = data._uploadStatusSet;
				_localPathSet = data._localPathSet;
				_occupationSet = data._occupationSet;
				_remotePathSet = data._remotePathSet;
				_hostnameSet = data._hostnameSet;
				_lastErrorMsgSet = data._lastErrorMsgSet;
				_aws_S3_ProviderSet = data._aws_S3_ProviderSet;
				_jobId = data._jobId;
				_nextRetry = data._nextRetry;
				_retries = data._retries;
				_priority = data._priority;
				_uploadStatus = data._uploadStatus;
				_localPath = data._localPath;
				_occupation = data._occupation;
				_remotePath = data._remotePath;
				_hostname = data._hostname;
				_lastErrorMsg = data._lastErrorMsg;
				_aws_S3_Provider = data._aws_S3_Provider;
				_aws_S3_Key_AK = data._aws_S3_Key_AK;
				_aws_S3_Key_SK = data._aws_S3_Key_SK;
				_aws_S3_upload_pool_size = data._aws_S3_upload_pool_size;
				_aws_S3_upload_buffer_size = data._aws_S3_upload_buffer_size;
				_aws_S3_RemoteBasepath = data._aws_S3_RemoteBasepath;
				_aws_S3_Region = data._aws_S3_Region;
				_aws_S3_Endpoint = data._aws_S3_Endpoint;
				toSet = false;
			}
			else {
				usleep(10000);
			}
		}
	}
	return *this;
}

nlohmann::json S3AwsUploaderRequestPayload::toJSONObject() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	nlohmann::json js;
	try {
		// Add members to json object
		std::string strval;
		uint64_t val_uint64 ;
		uint32_t val_uint32 ;
		uint16_t val_uint16 ;
		std::string val_time_string ;
		bool set = false;
		
		val_uint32 = getJobId(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_JOB_ID] = val_uint32;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << val_uint32 << "\" for key \"" << REQUEST_PAYLOAD_KEY_JOB_ID << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_JOB_ID << "\" not set. It will not be written");
		}
		
		val_time_string = getNextRetry(&set).asISO8601(0) ;
		if (set) {
			js[REQUEST_PAYLOAD_KEY_NEXT_RETRY] = val_time_string;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << val_time_string << "\" for key \"" << REQUEST_PAYLOAD_KEY_NEXT_RETRY << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_NEXT_RETRY << "\" not set. It will not be written");
		}
			
		val_uint16 = getRetries(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_RETRIES] = val_uint16;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << val_uint16 << "\" for key \"" << REQUEST_PAYLOAD_KEY_RETRIES << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_RETRIES << "\" not set. It will not be written");
		}
	
		val_uint16 = getPriority(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_PRIORITY] = val_uint16;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << val_uint16 << "\" for key \"" << REQUEST_PAYLOAD_KEY_PRIORITY << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_PRIORITY << "\" not set. It will not be written");
		}
	
		val_uint64 = getOccupation(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_OCCUPATION] = val_uint64;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << val_uint64 << "\" for key \"" << REQUEST_PAYLOAD_KEY_OCCUPATION << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_OCCUPATION << "\" not set. It will not be written");
		}
		
		strval = getUploadStatus(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_UPLOAD_STATUS] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_UPLOAD_STATUS << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_UPLOAD_STATUS << "\" not set. It will not be written");
		}
		
		strval = getLocalPath(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_LOCAL_PATH] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_LOCAL_PATH << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_LOCAL_PATH << "\" not set. It will not be written");
		}
		
		strval = getRemotePath(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_REMOTE_PATH] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_REMOTE_PATH << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_REMOTE_PATH << "\" not set. It will not be written");
		}
	
		strval = getHostname(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_HOSTNAME] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_HOSTNAME << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_HOSTNAME << "\" not set. It will not be written");
		}
		
		strval = getLastErrorMsg(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_LAST_ERROR_MESSAGE] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_LAST_ERROR_MESSAGE << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_LAST_ERROR_MESSAGE << "\" not set. It will not be written");
		}
		
		strval = getAWS_S3_Provider(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_AWS_S3_PROVIDER] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_PROVIDER << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_PROVIDER << "\" not set. It will not be written");
		}
		
		strval = getAWS_S3_Key_AK(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_AWS_S3_CRED_AK] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_CRED_AK << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_CRED_AK << "\" not set. It will not be written");
		}

		strval = getAWS_S3_Key_SK(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_AWS_S3_CRED_SK] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_CRED_SK << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_CRED_SK << "\" not set. It will not be written");
		}
		
		val_uint16 = getAWS_S3_Upload_Pool_Size(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_POOL_SZ] = val_uint16;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << val_uint16 << "\" for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_POOL_SZ << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_POOL_SZ << "\" not set. It will not be written");
		}

		val_uint64 = getAWS_S3_Upload_Buffer_Size(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_BFFR_SZ] = val_uint64;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << val_uint64 << "\" for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_BFFR_SZ << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_BFFR_SZ << "\" not set. It will not be written");
		}
		
		strval = getAWS_S3_RemoteBasepath(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_AWS_S3_REMOTE_BASEPATH] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_REMOTE_BASEPATH << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_REMOTE_BASEPATH << "\" not set. It will not be written");
		}
		
		strval = getAWS_S3_Region(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_AWS_S3_REGION] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_REGION << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_REGION << "\" not set. It will not be written");
		}
	
		strval = getAWS_S3_Endpoint(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_AWS_S3_ENDPOINT] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_ENDPOINT << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_ENDPOINT << "\" not set. It will not be written");
		}
		
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(S3AwsUploaderRequestPayloadException(e, "Error creating JSON Object."));
	}
	return js;
}

std::string S3AwsUploaderRequestPayload::toJSON() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	std::string jsstr;
	nlohmann::json js;
	try {
		js = toJSONObject();
		jsstr = js.dump();
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(S3AwsUploaderRequestPayloadException(e, "Error creating JSON."));
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Transformed request data into the following json string:\n" << jsstr);
	
	return jsstr;
}

void S3AwsUploaderRequestPayload::fromJSON(const std::string& jsstr) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	nlohmann::json js;
	try {
		js = nlohmann::json::parse(jsstr);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(S3AwsUploaderRequestPayloadException(e, "Error reading JSON."));
	}
	fromJSONObject(js);
}

void S3AwsUploaderRequestPayload::fromJSONObject(const nlohmann::json& js) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	try {
		
		if (js.find(REQUEST_PAYLOAD_KEY_JOB_ID) != js.end() && !js[REQUEST_PAYLOAD_KEY_JOB_ID].is_null()) {
			const uint32_t val ( js[REQUEST_PAYLOAD_KEY_JOB_ID] ) ;
			setJobId(val);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_JOB_ID << "\".");
			
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_JOB_ID << "\" not found or the associated value was null. It will not be loaded.");
		}
		
		if (js.find(REQUEST_PAYLOAD_KEY_NEXT_RETRY) != js.end() && !js[REQUEST_PAYLOAD_KEY_NEXT_RETRY].is_null()) {
			std::string val_string = js[REQUEST_PAYLOAD_KEY_NEXT_RETRY] ;
			const DateTime val(val_string) ; 
			setNextRetry(val);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val.asISO8601(0) << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_NEXT_RETRY << "\".");
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_NEXT_RETRY << "\" not found or the associated value was null. It will not be loaded.");
		}

		if (js.find(REQUEST_PAYLOAD_KEY_RETRIES) != js.end() && !js[REQUEST_PAYLOAD_KEY_RETRIES].is_null()) {
			const uint16_t val ( js[REQUEST_PAYLOAD_KEY_RETRIES] ) ;
			setRetries(val);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_RETRIES << "\".");
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_RETRIES << "\" not found or the associated value was null. It will not be loaded.");
		}

		if (js.find(REQUEST_PAYLOAD_KEY_PRIORITY) != js.end() && !js[REQUEST_PAYLOAD_KEY_PRIORITY].is_null()) {
			const uint16_t val ( js[REQUEST_PAYLOAD_KEY_PRIORITY] ) ;
			setPriority(val);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_PRIORITY << "\".");
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_PRIORITY << "\" not found or the associated value was null. It will not be loaded.");
		}
		
		if (js.find(REQUEST_PAYLOAD_KEY_OCCUPATION) != js.end() && !js[REQUEST_PAYLOAD_KEY_OCCUPATION].is_null()) {
			const uint64_t val ( js[REQUEST_PAYLOAD_KEY_OCCUPATION] ) ;
			setOccupation(val);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_OCCUPATION << "\".");
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_OCCUPATION << "\" not found or the associated value was null. It will not be loaded.");
		}
		
		if (js.find(REQUEST_PAYLOAD_KEY_UPLOAD_STATUS) != js.end() && !js[REQUEST_PAYLOAD_KEY_UPLOAD_STATUS].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_UPLOAD_STATUS] ;
			if (!val.empty()) {
				setUploadStatus(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_UPLOAD_STATUS << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_UPLOAD_STATUS << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_UPLOAD_STATUS << "\" not found or the associated value was null. It will not be loaded.");
		}
			
		if (js.find(REQUEST_PAYLOAD_KEY_LOCAL_PATH) != js.end() && !js[REQUEST_PAYLOAD_KEY_LOCAL_PATH].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_LOCAL_PATH] ;
			if (!val.empty()) {
				setLocalPath(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_LOCAL_PATH << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_LOCAL_PATH << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_LOCAL_PATH << "\" not found or the associated value was null. It will not be loaded.");
		}
		
		if (js.find(REQUEST_PAYLOAD_KEY_REMOTE_PATH) != js.end() && !js[REQUEST_PAYLOAD_KEY_REMOTE_PATH].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_REMOTE_PATH] ;
			if (!val.empty()) {
				setRemotePath(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_REMOTE_PATH << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_REMOTE_PATH << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_REMOTE_PATH << "\" not found or the associated value was null. It will not be loaded.");
		}
		
		if (js.find(REQUEST_PAYLOAD_KEY_HOSTNAME) != js.end() && !js[REQUEST_PAYLOAD_KEY_HOSTNAME].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_HOSTNAME] ;
			if (!val.empty()) {
				setHostname(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_HOSTNAME << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_HOSTNAME << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_HOSTNAME << "\" not found or the associated value was null. It will not be loaded.");
		}

		
		if (js.find(REQUEST_PAYLOAD_KEY_LAST_ERROR_MESSAGE) != js.end() && !js[REQUEST_PAYLOAD_KEY_LAST_ERROR_MESSAGE].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_LAST_ERROR_MESSAGE] ;
			if (!val.empty()) {
				setLastErrorMsg(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_LAST_ERROR_MESSAGE << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_LAST_ERROR_MESSAGE << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_LAST_ERROR_MESSAGE << "\" not found or the associated value was null. It will not be loaded.");
		}

		if (js.find(REQUEST_PAYLOAD_KEY_AWS_S3_PROVIDER) != js.end() && !js[REQUEST_PAYLOAD_KEY_AWS_S3_PROVIDER].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_AWS_S3_PROVIDER] ;
			if (!val.empty()) {
				setAWS_S3_Provider(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_PROVIDER << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_PROVIDER << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_PROVIDER << "\" not found or the associated value was null. It will not be loaded.");
		}

		if (js.find(REQUEST_PAYLOAD_KEY_AWS_S3_CRED_AK) != js.end() && !js[REQUEST_PAYLOAD_KEY_AWS_S3_CRED_AK].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_AWS_S3_CRED_AK] ;
			if (!val.empty()) {
				setAWS_S3_Key_AK(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_CRED_AK << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_CRED_AK << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_CRED_AK << "\" not found or the associated value was null. It will not be loaded.");
		}

		if (js.find(REQUEST_PAYLOAD_KEY_AWS_S3_CRED_SK) != js.end() && !js[REQUEST_PAYLOAD_KEY_AWS_S3_CRED_SK].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_AWS_S3_CRED_SK] ;
			if (!val.empty()) {
				setAWS_S3_Key_SK(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_CRED_SK << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_CRED_SK << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_CRED_SK << "\" not found or the associated value was null. It will not be loaded.");
		}

		if (js.find(REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_POOL_SZ) != js.end() && !js[REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_POOL_SZ].is_null()) {
			const uint16_t val ( js[REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_POOL_SZ] ) ;
			setAWS_S3_Upload_Pool_Size(val);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_POOL_SZ << "\".");

		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_POOL_SZ << "\" not found or the associated value was null. It will not be loaded.");
		}

		if (js.find(REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_BFFR_SZ) != js.end() && !js[REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_BFFR_SZ].is_null()) {
			const uint64_t val ( js[REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_BFFR_SZ] ) ;
			setAWS_S3_Upload_Buffer_Size(val);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_BFFR_SZ << "\".");

		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_BFFR_SZ << "\" not found or the associated value was null. It will not be loaded.");
		}

		if (js.find(REQUEST_PAYLOAD_KEY_AWS_S3_REMOTE_BASEPATH) != js.end() && !js[REQUEST_PAYLOAD_KEY_AWS_S3_REMOTE_BASEPATH].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_AWS_S3_REMOTE_BASEPATH] ;
			if (!val.empty()) {
				setAWS_S3_RemoteBasepath(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_REMOTE_BASEPATH << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_REMOTE_BASEPATH << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_REMOTE_BASEPATH << "\" not found or the associated value was null. It will not be loaded.");
		}

		if (js.find(REQUEST_PAYLOAD_KEY_AWS_S3_REGION) != js.end() && !js[REQUEST_PAYLOAD_KEY_AWS_S3_REGION].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_AWS_S3_REGION] ;
			if (!val.empty()) {
				setAWS_S3_Region(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_REGION << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_REGION << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_REGION << "\" not found or the associated value was null. It will not be loaded.");
		}

		if (js.find(REQUEST_PAYLOAD_KEY_AWS_S3_ENDPOINT) != js.end() && !js[REQUEST_PAYLOAD_KEY_AWS_S3_ENDPOINT].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_AWS_S3_ENDPOINT] ;
			if (!val.empty()) {
				setAWS_S3_Endpoint(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_ENDPOINT << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_ENDPOINT << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_ENDPOINT << "\" not found or the associated value was null. It will not be loaded.");
		}
		
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(S3AwsUploaderRequestPayloadException(e, "Error reading JSON Object."));
	}
}

bool operator==(const S3AwsUploaderRequestPayload &a, const S3AwsUploaderRequestPayload &b) 
{
	bool is_equal (true) ; 
	bool a_set, b_set, c ; 
	
	c = (a.getJobId(&a_set) == b.getJobId(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	c = (a.getNextRetry(&a_set) == b.getNextRetry(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	c = (a.getRetries(&a_set) == b.getRetries(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	c = (a.getPriority(&a_set) == b.getPriority(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	c = (a.getUploadStatus(&a_set) == b.getUploadStatus(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	c = (a.getLocalPath(&a_set) == b.getLocalPath(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	c = (a.getOccupation(&a_set) == b.getOccupation(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	c = (a.getRemotePath(&a_set) == b.getRemotePath(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	c = (a.getHostname(&a_set) == b.getHostname(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	c = (a.getLastErrorMsg(&a_set) == b.getLastErrorMsg(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	c = (a.getAWS_S3_Provider(&a_set) == b.getAWS_S3_Provider(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	c = (a.getAWS_S3_Key_AK(&a_set) == b.getAWS_S3_Key_AK(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }
	
	c = (a.getAWS_S3_Key_SK(&a_set) == b.getAWS_S3_Key_SK(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	c = (a.getAWS_S3_RemoteBasepath(&a_set) == b.getAWS_S3_RemoteBasepath(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }
	
	c = (a.getAWS_S3_Region(&a_set) == b.getAWS_S3_Region(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }
	
	c = (a.getAWS_S3_Endpoint(&a_set) == b.getAWS_S3_Endpoint(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	return is_equal ; 
}

_ACS_END_NAMESPACE
