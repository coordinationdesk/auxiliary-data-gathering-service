/*

	Copyright 2019-2022, Exprivia S.p.A. - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia S.p.A. ;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia S.p.A.

	$Prod: AwsS3Wrapper JSON Handler for configuration $

*/

#include <AwsS3WrapperConfigurationJSON.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(AwsS3WrapperConfigurationJSON);

const std::string AwsS3WrapperConfigurationJSON::REQUEST_PAYLOAD_KEY_AWS_S3_PROVIDER = "AWS_S3_Provider" ;
const std::string AwsS3WrapperConfigurationJSON::REQUEST_PAYLOAD_KEY_AWS_S3_CRED_AK = "AWS_S3_Key_AK" ;
const std::string AwsS3WrapperConfigurationJSON::REQUEST_PAYLOAD_KEY_AWS_S3_CRED_SK = "AWS_S3_Key_SK" ;
const std::string AwsS3WrapperConfigurationJSON::REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_POOL_SZ = "AWS_S3_Upload_Pool_Size" ;
const std::string AwsS3WrapperConfigurationJSON::REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_BFFR_SZ = "AWS_S3_Upload_Buffer_Size" ;
const std::string AwsS3WrapperConfigurationJSON::REQUEST_PAYLOAD_KEY_AWS_S3_DOWNLOAD_POOL_SZ = "AWS_S3_Download_Pool_Size" ;
const std::string AwsS3WrapperConfigurationJSON::REQUEST_PAYLOAD_KEY_AWS_S3_DOWNLOAD_BFFR_SZ = "AWS_S3_Download_Buffer_Size" ;
const std::string AwsS3WrapperConfigurationJSON::REQUEST_PAYLOAD_KEY_AWS_S3_REMOTE_BASEPATH = "AWS_S3_RemoteBasepath" ;
const std::string AwsS3WrapperConfigurationJSON::REQUEST_PAYLOAD_KEY_AWS_S3_REGION = "AWS_S3_Region" ;
const std::string AwsS3WrapperConfigurationJSON::REQUEST_PAYLOAD_KEY_AWS_S3_ENDPOINT = "AWS_S3_Endpoint" ;
const std::string AwsS3WrapperConfigurationJSON::REQUEST_PAYLOAD_KEY_AWS_S3_EP_IS_SECURE = "AWS_S3_Endpoint_Is_Secure" ;


AwsS3WrapperConfigurationJSON::AwsS3WrapperConfigurationJSON() :
	_mutex(),
	_aws_S3_ProviderSet(false),	/* This is used as unique "set" for all the other S3 parameters */
	_aws_S3_Provider(),
	_aws_S3_Key_AK(),
	_aws_S3_Key_SK(),
	_aws_S3_upload_pool_size(0),
	_aws_S3_upload_buffer_size(0),
	_aws_S3_download_pool_size(0),
	_aws_S3_download_buffer_size(0),
	_aws_S3_RemoteBasepath(),
	_aws_S3_Region(),
	_aws_S3_Endpoint(),
	_aws_S3_Endpoint_Is_Secure(false)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}


AwsS3WrapperConfigurationJSON::AwsS3WrapperConfigurationJSON(const std::string& jsstr) :
	_mutex(),
	_aws_S3_ProviderSet(false),	/* This is used as unique "set" for all the other S3 parameters */
	_aws_S3_Provider(),
	_aws_S3_Key_AK(),
	_aws_S3_Key_SK(),
	_aws_S3_upload_pool_size(0),
	_aws_S3_upload_buffer_size(0),
	_aws_S3_download_pool_size(0),
	_aws_S3_download_buffer_size(0),
	_aws_S3_RemoteBasepath(),
	_aws_S3_Region(),
	_aws_S3_Endpoint(),
	_aws_S3_Endpoint_Is_Secure(false)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSON(jsstr);
}


AwsS3WrapperConfigurationJSON::AwsS3WrapperConfigurationJSON(const JsonSerializable& js) :
	_mutex(),
	_aws_S3_ProviderSet(false),	/* This is used as unique "set" for all the other S3 parameters */
	_aws_S3_Provider(),
	_aws_S3_Key_AK(),
	_aws_S3_Key_SK(),
	_aws_S3_upload_pool_size(0),
	_aws_S3_upload_buffer_size(0),
	_aws_S3_download_pool_size(0),
	_aws_S3_download_buffer_size(0),
	_aws_S3_RemoteBasepath(),
	_aws_S3_Region(),
	_aws_S3_Endpoint(),
	_aws_S3_Endpoint_Is_Secure(false)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSONObject(js);
}

AwsS3WrapperConfigurationJSON::AwsS3WrapperConfigurationJSON(const AwsS3WrapperConfigurationJSON& data) :
	_mutex(),
	_aws_S3_ProviderSet(data._aws_S3_ProviderSet),
	_aws_S3_Provider(data._aws_S3_Provider),
	_aws_S3_Key_AK(data._aws_S3_Key_AK),
	_aws_S3_Key_SK(data._aws_S3_Key_SK),
	_aws_S3_upload_pool_size(data._aws_S3_upload_pool_size),
	_aws_S3_upload_buffer_size(data._aws_S3_upload_buffer_size),
	_aws_S3_download_pool_size(data._aws_S3_download_pool_size),
	_aws_S3_download_buffer_size(data._aws_S3_download_buffer_size),
	_aws_S3_RemoteBasepath(data._aws_S3_RemoteBasepath),
	_aws_S3_Region(data._aws_S3_Region),
	_aws_S3_Endpoint(data._aws_S3_Endpoint),
	_aws_S3_Endpoint_Is_Secure(data._aws_S3_Endpoint_Is_Secure)
{ 	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

AwsS3WrapperConfigurationJSON::~AwsS3WrapperConfigurationJSON() ACS_NOEXCEPT { }

AwsS3WrapperConfigurationJSON& AwsS3WrapperConfigurationJSON::operator=(const AwsS3WrapperConfigurationJSON& data) {

	if (this != &data) {

		bool        tmp_aws_S3_ProviderSet ;
		std::string tmp_aws_S3_Provider ;
		std::string tmp_aws_S3_Key_AK ;
		std::string tmp_aws_S3_Key_SK ;
		uint16_t	tmp_aws_S3_upload_pool_size ;
		uint64_t	tmp_aws_S3_upload_buffer_size ;
		uint16_t	tmp_aws_S3_download_pool_size ;
		uint64_t	tmp_aws_S3_download_buffer_size ;
		std::string tmp_aws_S3_RemoteBasepath ;
		std::string tmp_aws_S3_Region ;
		std::string tmp_aws_S3_Endpoint ;
		bool		tmp_aws_S3_Endpoint_Is_Secure ;

		{
			Lock lk(data._mutex);		
			
			tmp_aws_S3_ProviderSet          = data._aws_S3_ProviderSet ;								
			tmp_aws_S3_Provider             = data._aws_S3_Provider ;								
			tmp_aws_S3_Key_AK               = data._aws_S3_Key_AK ; 								
			tmp_aws_S3_Key_SK               = data._aws_S3_Key_SK ; 								
			tmp_aws_S3_upload_pool_size     = data._aws_S3_upload_pool_size ;				  
			tmp_aws_S3_upload_buffer_size   = data._aws_S3_upload_buffer_size ; 			  
			tmp_aws_S3_download_pool_size   = data._aws_S3_download_pool_size ; 			  
			tmp_aws_S3_download_buffer_size = data._aws_S3_download_buffer_size ;			  
			tmp_aws_S3_RemoteBasepath       = data._aws_S3_RemoteBasepath ;			  
			tmp_aws_S3_Region               = data._aws_S3_Region ;						
			tmp_aws_S3_Endpoint            = data._aws_S3_Endpoint ;  				   
			tmp_aws_S3_Endpoint_Is_Secure            = data._aws_S3_Endpoint_Is_Secure ;  				   
		}
		
		{
			Lock lk(_mutex);					   
		
			_aws_S3_ProviderSet			 = tmp_aws_S3_ProviderSet ;									
			_aws_S3_Provider			 = tmp_aws_S3_Provider ;									
			_aws_S3_Key_AK  			 = tmp_aws_S3_Key_AK ;  									
			_aws_S3_Key_SK  			 = tmp_aws_S3_Key_SK ;  									
			_aws_S3_upload_pool_size	 = tmp_aws_S3_upload_pool_size ;					  
			_aws_S3_upload_buffer_size   = tmp_aws_S3_upload_buffer_size ;  				  
			_aws_S3_download_pool_size   = tmp_aws_S3_download_pool_size ;  				  
			_aws_S3_download_buffer_size = tmp_aws_S3_download_buffer_size ;				  
			_aws_S3_RemoteBasepath  	 = tmp_aws_S3_RemoteBasepath ;  					  
			_aws_S3_Region  			 = tmp_aws_S3_Region ;  									
			_aws_S3_Endpoint			 = tmp_aws_S3_Endpoint ;									
			_aws_S3_Endpoint_Is_Secure			 = tmp_aws_S3_Endpoint_Is_Secure ;									
		}

	}
	
	return *this;
}


std::string AwsS3WrapperConfigurationJSON::dump(const std::string& t)
{
	std::string strval;
	uint64_t val_uint64 ;
	uint16_t val_uint16 ;
	bool val_bool ; 
	std::string val_time_string ;
	bool set = false;

	std::ostringstream o ;
	
	strval = getAWS_S3_Provider(&set);
	o << t << REQUEST_PAYLOAD_KEY_AWS_S3_PROVIDER << ": " << (set? ("\"" + strval + "\"") : "null") << "\n" ;
	
	strval = getAWS_S3_Key_AK(&set);
	o << t << REQUEST_PAYLOAD_KEY_AWS_S3_CRED_AK << ": " << (set? ("\"" + strval + "\"") : "null") << "\n" ;

	strval = getAWS_S3_Key_SK(&set);
	o << t << REQUEST_PAYLOAD_KEY_AWS_S3_CRED_SK << ": " << (set? ("\"" + strval + "\"") : "null") << "\n" ;

	val_uint16 = getAWS_S3_Upload_Pool_Size(&set); 
	o << t << REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_POOL_SZ << ": " << (set? std::to_string(static_cast<uint64_t>(val_uint16)) : "null") << "\n" ;

	val_uint64 = getAWS_S3_Upload_Buffer_Size(&set);
	o << t << REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_BFFR_SZ << ": " << (set? std::to_string(val_uint64) : "null") << "\n" ;

	val_uint16 = getAWS_S3_Download_Pool_Size(&set);
	o << t << REQUEST_PAYLOAD_KEY_AWS_S3_DOWNLOAD_POOL_SZ << ": " << (set? std::to_string(static_cast<uint64_t>(val_uint16)) : "null") << "\n" ;

	val_uint64 = getAWS_S3_Download_Buffer_Size(&set);
	o << t << REQUEST_PAYLOAD_KEY_AWS_S3_DOWNLOAD_BFFR_SZ << ": " << (set? std::to_string(val_uint64) : "null") << "\n" ;

	strval = getAWS_S3_RemoteBasepath(&set);
	o << t << REQUEST_PAYLOAD_KEY_AWS_S3_REMOTE_BASEPATH << ": " << (set? ("\"" + strval + "\"") : "null") << "\n" ;

	strval = getAWS_S3_Region(&set);
	o << t << REQUEST_PAYLOAD_KEY_AWS_S3_REGION << ": " << (set? ("\"" + strval + "\"") : "null") << "\n" ;

	strval = getAWS_S3_Endpoint(&set);
	o << t << REQUEST_PAYLOAD_KEY_AWS_S3_ENDPOINT << ": " << (set? ("\"" + strval + "\"") : "null") << "\n" ;

	val_bool = getAWS_S3_Endpoint_Is_Secure(&set);
	o << t << REQUEST_PAYLOAD_KEY_AWS_S3_EP_IS_SECURE << ": " << (set? (val_bool?"T":"F") : "null") ;

	return o.str() ;
}


JsonSerializable AwsS3WrapperConfigurationJSON::toJSONObject() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	JsonSerializable js ;
	try {
		// Add members to json object
		std::string strval;
		uint64_t val_uint64 ;
		uint16_t val_uint16 ;
		bool val_bool ;
		std::string val_time_string ;
		bool set = false;
		
		strval = getAWS_S3_Provider(&set);
		if (set) {
			js.add_pair(REQUEST_PAYLOAD_KEY_AWS_S3_PROVIDER,strval) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_PROVIDER << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_PROVIDER << "\" not set. It will not be written");
		}
		
		strval = getAWS_S3_Key_AK(&set);
		if (set) {
			js.add_pair(REQUEST_PAYLOAD_KEY_AWS_S3_CRED_AK,strval) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_CRED_AK << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_CRED_AK << "\" not set. It will not be written");
		}

		strval = getAWS_S3_Key_SK(&set);
		if (set) {
			js.add_pair(REQUEST_PAYLOAD_KEY_AWS_S3_CRED_SK,strval) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_CRED_SK << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_CRED_SK << "\" not set. It will not be written");
		}
		
		val_uint16 = getAWS_S3_Upload_Pool_Size(&set);
		if (set) {
			js.add_pair(REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_POOL_SZ,val_uint16) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << val_uint16 << "\" for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_POOL_SZ << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_POOL_SZ << "\" not set. It will not be written");
		}

		val_uint64 = getAWS_S3_Upload_Buffer_Size(&set);
		if (set) {
			js.add_pair(REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_BFFR_SZ,val_uint64) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << val_uint64 << "\" for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_BFFR_SZ << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_BFFR_SZ << "\" not set. It will not be written");
		}

		val_uint16 = getAWS_S3_Download_Pool_Size(&set);
		if (set) {
			js.add_pair(REQUEST_PAYLOAD_KEY_AWS_S3_DOWNLOAD_POOL_SZ,val_uint16) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << val_uint16 << "\" for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_DOWNLOAD_POOL_SZ << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_DOWNLOAD_POOL_SZ << "\" not set. It will not be written");
		}

		val_uint64 = getAWS_S3_Download_Buffer_Size(&set);
		if (set) {
			js.add_pair(REQUEST_PAYLOAD_KEY_AWS_S3_DOWNLOAD_BFFR_SZ,val_uint64) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << val_uint64 << "\" for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_DOWNLOAD_BFFR_SZ << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_DOWNLOAD_BFFR_SZ << "\" not set. It will not be written");
				}

		strval = getAWS_S3_RemoteBasepath(&set);
		if (set) {
			js.add_pair(REQUEST_PAYLOAD_KEY_AWS_S3_REMOTE_BASEPATH,strval) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_REMOTE_BASEPATH << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_REMOTE_BASEPATH << "\" not set. It will not be written");
		}
		
		strval = getAWS_S3_Region(&set);
		if (set) {
			js.add_pair(REQUEST_PAYLOAD_KEY_AWS_S3_REGION,strval) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_REGION << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_REGION << "\" not set. It will not be written");
		}
	
		strval = getAWS_S3_Endpoint(&set);
		if (set) {
			js.add_pair(REQUEST_PAYLOAD_KEY_AWS_S3_ENDPOINT,strval) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_ENDPOINT << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_ENDPOINT << "\" not set. It will not be written");
		}
		
		val_bool = getAWS_S3_Endpoint_Is_Secure(&set);
		if (set) {
			js.add_pair(REQUEST_PAYLOAD_KEY_AWS_S3_EP_IS_SECURE,val_bool) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << (val_bool?"T":"F") << "\" for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_EP_IS_SECURE << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_AWS_S3_EP_IS_SECURE << "\" not set. It will not be written");
		}
		
	} catch (std::exception& e) {
		ACS_THROW(AwsS3WrapperConfigurationJSONException(e, "Error creating JSON Object."));
	}
	
	return js;
}

std::string AwsS3WrapperConfigurationJSON::toJSON() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	std::string jsstr;
	JsonSerializable js ;
	try {
		js = toJSONObject();
		jsstr = js.dump();
	} catch (std::exception& e) {
		ACS_THROW(AwsS3WrapperConfigurationJSONException(e, "Error creating JSON."));
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Transformed request data into the following json string:\n" << jsstr);
	
	return jsstr;
}

void AwsS3WrapperConfigurationJSON::fromJSON(const std::string& jsstr) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	JsonSerializable js;
	try {
		js.unserialize(jsstr) ;
	} catch (std::exception& e) {
		ACS_THROW(AwsS3WrapperConfigurationJSONException(e, "Error reading JSON."));
	}
		
	fromJSONObject(js);
}

void AwsS3WrapperConfigurationJSON::fromJSONObject(const JsonSerializable& js) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	try {
		std::string k = REQUEST_PAYLOAD_KEY_AWS_S3_PROVIDER ; if (js.has_key(k) && (not js.is_null(k)))
		{ 
			std::string val ; js.get_value(k, val) ; 
			if (!val.empty()) {
				setAWS_S3_Provider(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << k << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << k << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << k << "\" not found or the associated value was null. It will not be loaded.");
		}

		k = REQUEST_PAYLOAD_KEY_AWS_S3_CRED_AK ; if (js.has_key(k) && (not js.is_null(k)))
		{ 
			std::string val ; js.get_value(k, val) ; 
			if (!val.empty()) {
				setAWS_S3_Key_AK(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << k << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << k << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << k << "\" not found or the associated value was null. It will not be loaded.");
		}

		k = REQUEST_PAYLOAD_KEY_AWS_S3_CRED_SK ; if (js.has_key(k) && (not js.is_null(k)))
		{ 
			std::string val ; js.get_value(k, val) ; 
			if (!val.empty()) {
				setAWS_S3_Key_SK(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << k << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << k << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << k << "\" not found or the associated value was null. It will not be loaded.");
		}

		k = REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_POOL_SZ ; if (js.has_key(k) && (not js.is_null(k)))
		{ 
			uint16_t val ; js.get_value(k, val) ; 
			setAWS_S3_Upload_Pool_Size(val);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << k << "\".");

		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << k << "\" not found or the associated value was null. It will not be loaded.");
		}

		k = REQUEST_PAYLOAD_KEY_AWS_S3_UPLOAD_BFFR_SZ	 ; if (js.has_key(k) && (not js.is_null(k)))
		{ 
			uint64_t val ; js.get_value(k, val) ; 
			setAWS_S3_Upload_Buffer_Size(val);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << k << "\".");

		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << k << "\" not found or the associated value was null. It will not be loaded.");
		}

		k = REQUEST_PAYLOAD_KEY_AWS_S3_DOWNLOAD_POOL_SZ ; if (js.has_key(k) && (not js.is_null(k)))
		{ 
			uint16_t val ; js.get_value(k, val) ; 
			setAWS_S3_Download_Pool_Size(val);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << k << "\".");

		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << k << "\" not found or the associated value was null. It will not be loaded.");
		}

		k = REQUEST_PAYLOAD_KEY_AWS_S3_DOWNLOAD_BFFR_SZ ; if (js.has_key(k) && (not js.is_null(k)))
		{ 
			uint64_t val ; js.get_value(k, val) ; 
			setAWS_S3_Download_Buffer_Size(val);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << k << "\".");

		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << k << "\" not found or the associated value was null. It will not be loaded.");
		}

		k = REQUEST_PAYLOAD_KEY_AWS_S3_REMOTE_BASEPATH ; if (js.has_key(k) && (not js.is_null(k)))
		{ 
			std::string val ; js.get_value(k, val) ; 
			if (!val.empty()) {
				setAWS_S3_RemoteBasepath(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << k << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << k << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << k << "\" not found or the associated value was null. It will not be loaded.");
		}

		k = REQUEST_PAYLOAD_KEY_AWS_S3_REGION ; if (js.has_key(k) && (not js.is_null(k)))
		{ 
			std::string val ; js.get_value(k, val) ; 
			if (!val.empty()) {
				setAWS_S3_Region(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << k << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << k << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << k << "\" not found or the associated value was null. It will not be loaded.");
		}

		k = REQUEST_PAYLOAD_KEY_AWS_S3_ENDPOINT ; if (js.has_key(k) && (not js.is_null(k)))
		{ 
			std::string val ; js.get_value(k, val) ; 
			if (!val.empty()) {
				setAWS_S3_Endpoint(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << k << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << k << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << k << "\" not found or the associated value was null. It will not be loaded.");
		}
				
		k = REQUEST_PAYLOAD_KEY_AWS_S3_EP_IS_SECURE ; if (js.has_key(k) && (not js.is_null(k)))
		{ 
			bool val ; js.get_value(k, val) ; 
			setAWS_S3_Endpoint_Is_Secure(val);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << (val?"T":"F") << "\" associated to key \"" << k << "\".");

		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << k << "\" not found or the associated value was null. It will not be loaded.");
		}
		
		
	} catch (std::exception& e) {
		ACS_THROW(AwsS3WrapperConfigurationJSONException(e, "Error reading JSON Object."));
	}
}

bool AwsS3WrapperConfigurationJSON::is_equal(const AwsS3WrapperConfigurationJSON &a, const AwsS3WrapperConfigurationJSON &b) 
{
	bool is_equal (true) ; 
	bool a_set, b_set, c ; 

	c = (a.getAWS_S3_Provider(&a_set) == b.getAWS_S3_Provider(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	c = (a.getAWS_S3_Key_AK(&a_set) == b.getAWS_S3_Key_AK(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }
	
	c = (a.getAWS_S3_Key_SK(&a_set) == b.getAWS_S3_Key_SK(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	c = (a.getAWS_S3_Upload_Pool_Size(&a_set) == b.getAWS_S3_Upload_Pool_Size(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	c = (a.getAWS_S3_Upload_Buffer_Size(&a_set) == b.getAWS_S3_Upload_Buffer_Size(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	c = (a.getAWS_S3_Download_Pool_Size(&a_set) == b.getAWS_S3_Download_Pool_Size(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	c = (a.getAWS_S3_Download_Buffer_Size(&a_set) == b.getAWS_S3_Download_Buffer_Size(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	c = (a.getAWS_S3_RemoteBasepath(&a_set) == b.getAWS_S3_RemoteBasepath(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }
	
	c = (a.getAWS_S3_Region(&a_set) == b.getAWS_S3_Region(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }
	
	c = (a.getAWS_S3_Endpoint(&a_set) == b.getAWS_S3_Endpoint(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	c = (a.getAWS_S3_Endpoint_Is_Secure(&a_set) == b.getAWS_S3_Endpoint_Is_Secure(&b_set)) ;
	is_equal = (is_equal && (a_set == b_set)) ; if (true == a_set) { is_equal = (is_equal && c) ; }

	return is_equal ; 
}

_ACS_END_NAMESPACE
