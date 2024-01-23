/*

	Copyright 2019-2022, Exprivia S.p.A. - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia S.p.A. ;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia S.p.A.

	$Prod: AwsS3Wrapper JSON Handler for configuration -- HEADER FILE $

*/


#ifndef _AwsS3WrapperConfigurationJSON_h_
#define _AwsS3WrapperConfigurationJSON_h_

#include <exException.h>
#include <Filterables.h>
#include <acs_c++config.hpp>
#include <string>
#include <JsonSerializable.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful request sent to the LTANotificationServiceDispatcher */
class AwsS3WrapperConfigurationJSON {
public:
	
	exDECLARE_EXCEPTION(AwsS3WrapperConfigurationJSONException, exException);

public:
	/** Ctors/Dtors */
	AwsS3WrapperConfigurationJSON();
	explicit AwsS3WrapperConfigurationJSON(const std::string&);
	explicit AwsS3WrapperConfigurationJSON(const JsonSerializable&);
	AwsS3WrapperConfigurationJSON(const AwsS3WrapperConfigurationJSON&);
	virtual ~AwsS3WrapperConfigurationJSON() ACS_NOEXCEPT;
	
	/** Operators overload */
	AwsS3WrapperConfigurationJSON& operator=(const AwsS3WrapperConfigurationJSON&) ;

	virtual std::string dump(const std::string& t="") ;
	
	/** Setters */
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

	inline void setAWS_S3_Download_Pool_Size(const uint16_t& v) {
		Lock l(_mutex); _aws_S3_download_pool_size = v;
	}

	inline void setAWS_S3_Download_Buffer_Size(const uint64_t& v) {
		Lock l(_mutex); _aws_S3_download_buffer_size = v;
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

	inline void setAWS_S3_Endpoint_Is_Secure(const bool& v) {
		Lock l(_mutex); _aws_S3_Endpoint_Is_Secure = v;
	}


	/** Getters */
	
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

	inline const uint16_t& getAWS_S3_Download_Pool_Size(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _aws_S3_ProviderSet ; /* Pls. note */ }
		return _aws_S3_download_pool_size ;
	}

	inline const uint64_t& getAWS_S3_Download_Buffer_Size(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _aws_S3_ProviderSet ; /* Pls. note */ }
		return _aws_S3_download_buffer_size ;
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

	inline const bool& getAWS_S3_Endpoint_Is_Secure(bool* isset = 0) const {
		Lock l(_mutex); if (isset != 0) { *isset = _aws_S3_ProviderSet ; /* Pls. note */ }
		return _aws_S3_Endpoint_Is_Secure ;
	}
	
	
	/** Returns the serialized json representation of this object */
	virtual std::string toJSON() const;
	virtual JsonSerializable toJSONObject() const;
	
	/** Initialize this object from the json string in input */
	virtual void fromJSONObject(const JsonSerializable&);
	virtual void fromJSON(const std::string&);
	
	/** Used to build operator== */
	static bool is_equal(const AwsS3WrapperConfigurationJSON &a, const AwsS3WrapperConfigurationJSON &b) ;

private:
	static const std::string REQUEST_PAYLOAD_KEY_AWS_S3_PROVIDER ;
	static const std::string REQUEST_PAYLOAD_KEY_AWS_S3_CRED_AK ;
	static const std::string REQUEST_PAYLOAD_KEY_AWS_S3_CRED_SK ;
	static const std::string REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_POOL_SZ ;
	static const std::string REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_BFFR_SZ ;
	static const std::string REQUEST_PAYLOAD_KEY_AWS_S3_DOWNLOAD_POOL_SZ ;
	static const std::string REQUEST_PAYLOAD_KEY_AWS_S3_DOWNLOAD_BFFR_SZ ;
	static const std::string REQUEST_PAYLOAD_KEY_AWS_S3_REMOTE_BASEPATH ;
	static const std::string REQUEST_PAYLOAD_KEY_AWS_S3_REGION ;
	static const std::string REQUEST_PAYLOAD_KEY_AWS_S3_ENDPOINT ;
	static const std::string REQUEST_PAYLOAD_KEY_AWS_S3_EP_IS_SECURE ;
 
protected:
	Mutex _mutex;

	bool _aws_S3_ProviderSet ;	/* This is used as unique "set" for all the other S3 parameters */

private:	
	std::string _aws_S3_Provider ;
	std::string _aws_S3_Key_AK ;
	std::string _aws_S3_Key_SK ;
	uint16_t	_aws_S3_upload_pool_size ;
	uint64_t	_aws_S3_upload_buffer_size ;
	uint16_t	_aws_S3_download_pool_size ;
	uint64_t	_aws_S3_download_buffer_size ;
	std::string _aws_S3_RemoteBasepath ;
	std::string _aws_S3_Region ;
	std::string _aws_S3_Endpoint ;
	bool		_aws_S3_Endpoint_Is_Secure ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(AwsS3WrapperConfigurationJSON);
};


inline bool operator==(const AwsS3WrapperConfigurationJSON &a, const AwsS3WrapperConfigurationJSON &b) 
{ return AwsS3WrapperConfigurationJSON::is_equal(a,b) ; }

inline bool operator!=(const AwsS3WrapperConfigurationJSON &a, const AwsS3WrapperConfigurationJSON &b)  
{ return (!(a == b)) ; }


_ACS_END_NAMESPACE

#endif /* _AwsS3WrapperConfigurationJSON_h_ */
