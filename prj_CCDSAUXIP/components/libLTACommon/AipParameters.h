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

	$Prod: LTA common functions $


*/

#ifndef _AipParameters_H_
#define _AipParameters_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <Filterables.h>
#include <json.hpp>


_ACS_BEGIN_NESTED_NAMESPACE(acs, lta)

/*- This class is used to wrap the parameters defined in t_srv_aip
 */
class AipParameters {
public:
	exDECLARE_EXCEPTION(AipParametersException, exException);
	exDECLARE_EXCEPTION(AipParametersParseException, AipParametersException);
	exDECLARE_EXCEPTION(AipParametersMissingKeyException, AipParametersException);

	AipParameters()                                = default;
	virtual ~AipParameters() noexcept              = default;
	AipParameters(const AipParameters&)            = default;
	AipParameters& operator=(const AipParameters&) = default;

	explicit AipParameters(const std::string& paramsJson);
	AipParameters(const std::string& s3endpoint, const std::string& keyak, const std::string& keysk, 
					const std::string& provider, const std::string& remoteBasePath);

	void fromJsonString(const std::string& paramsJson);
	std::string toJsonString();

	// Getters
	std::string getAwsS3Endpoint() const;
	std::string getAwsS3KeyAK() const;
	std::string getAwsS3KeySK() const;
	std::string getAwsS3Provider() const;
	std::string getAwsS3RemoteBasePath() const;
	std::string getAwsS3Region(bool& set) const;
	unsigned short getAwsS3UploadPoolSize(bool& set) const;
	size_t getAwsS3UploadBufferSize(bool& set) const;
	unsigned short getAwsS3DownloadPoolSize(bool& set) const;
	size_t getAwsS3DownloadBufferSize(bool& set) const;
	bool getDirectDlAip(bool& set) const;
	
	// Setters
	void setAwsS3Region(const std::string& s3region);
	void setAwsS3UploadPoolSize(unsigned short uploadPoolSize);
	void setAwsS3UploadBufferSize(size_t uploadBufferSize);
	void setAwsS3DownloadPoolSize(unsigned short downloadPoolSize);
	void setAwsS3DownloadBufferSize(size_t downloadBufferSize);
	void setDirectDlAip(bool directDlAip);
private:
	template <typename T>
	bool getValue(const nlohmann::json& json, const std::string& key, T& res, bool isMandatory = true) {
		bool found = false;
		if ((json.find(key) == json.end()) || (json[key].is_null())) {
			ACS_COND_THROW(isMandatory, AipParametersMissingKeyException() << "Key \"" << key << 
						"\" not found into json request payload or the associated value is null. RequestId key is mandatory");
		}
		else {
			res = json[key];
			found = true;
		}
		return found;
	}

private:
	static const std::string AWS_S3_PARAMETERS_KEY;
	static const std::string AWS_S3_ENDPOINT_KEY;
	static const std::string AWS_S3_KEYAK_KEY;
	static const std::string AWS_S3_KEYSK_KEY;
	static const std::string AWS_S3_PROVIDER_KEY;
	static const std::string AWS_S3_REMOTE_BASEPATH_KEY;
	static const std::string AWS_S3_REGION_KEY;
	static const std::string AWS_S3_UPLOAD_POOL_SIZE_KEY;
	static const std::string AWS_S3_UPLOAD_BUFFER_SIZE_KEY;
	static const std::string AWS_S3_DOWNLOAD_POOL_SIZE_KEY;
	static const std::string AWS_S3_DOWNLOAD_BUFFER_SIZE_KEY;
	static const std::string DIRECTDL_AIP_KEY;

private:
	// Mandatory parameters
	std::string awsS3Endpoint_ = "";
	std::string awsS3KeyAK_ = "";
	std::string awsS3KeySK_ = "";
	std::string awsS3Provider_ = "";
	std::string awsS3RemoteBasePath_ = "";

	// Not mandatory parameters
	std::string awsS3Region_ = "";
	unsigned short awsS3UploadPoolSize_ = 0;
	size_t awsS3UploadBufferSize_ = 0;
	unsigned short awsS3DownloadPoolSize_ = 0;
	size_t awsS3DownloadBufferSize_ = 0;
	bool directDlAip_ = false;
	bool isAwsS3RegionSet_ = false;
	bool isAwsS3UploadPoolSizeSet_ = false;
	bool isAwsS3UploadBufferSizeSet_ = false;
	bool isAwsS3DownloadPoolSizeSet_ = false;
	bool isAwsS3DownloadBufferSizeSet_ = false;
	bool isDirectDlAipSet_ = false;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(AipParameters);
};

_ACS_END_NESTED_NAMESPACE

#endif /* _AipParameters_H_ */