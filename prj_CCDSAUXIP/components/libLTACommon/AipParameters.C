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

#include <AipParameters.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, lta)

ACS_CLASS_DEFINE_DEBUG_LEVEL(AipParameters);

const std::string AipParameters::DIRECTDL_AIP_KEY = "direct_dl_aip";
const std::string AipParameters::AWS_S3_PARAMETERS_KEY = "aws_s3_parameters";
const std::string AipParameters::AWS_S3_ENDPOINT_KEY = "aws_s3_endpoint";
const std::string AipParameters::AWS_S3_KEYAK_KEY = "aws_s3_key_ak";
const std::string AipParameters::AWS_S3_KEYSK_KEY = "aws_s3_key_sk";
const std::string AipParameters::AWS_S3_PROVIDER_KEY = "aws_s3_provider";
const std::string AipParameters::AWS_S3_REMOTE_BASEPATH_KEY = "aws_s3_remotebasepath";
const std::string AipParameters::AWS_S3_REGION_KEY = "aws_s3_region";
const std::string AipParameters::AWS_S3_UPLOAD_POOL_SIZE_KEY = "aws_s3_ul_pool_size";
const std::string AipParameters::AWS_S3_UPLOAD_BUFFER_SIZE_KEY = "aws_s3_ul_buffer_size";
const std::string AipParameters::AWS_S3_DOWNLOAD_POOL_SIZE_KEY = "aws_s3_dl_pool_size";
const std::string AipParameters::AWS_S3_DOWNLOAD_BUFFER_SIZE_KEY = "aws_s3_dl_buffer_size";

AipParameters::AipParameters(const std::string& paramsJson) {
	fromJsonString(paramsJson);
}

AipParameters::AipParameters(const std::string& s3endpoint, const std::string& keyak, const std::string& keysk, 
				const std::string& provider, const std::string& remoteBasePath) :
	awsS3Endpoint_(s3endpoint),
	awsS3KeyAK_(keyak),
	awsS3KeySK_(keysk),
	awsS3Provider_(provider),
	awsS3RemoteBasePath_(remoteBasePath)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void AipParameters::fromJsonString(const std::string& paramsJson) {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Parsing json string: " << paramsJson);
	nlohmann::json js;
	try {
		js = nlohmann::json::parse(paramsJson);
	} catch (const nlohmann::json::exception& e) {
		ACS_THROW(AipParametersParseException(e, "Error parsing JSON."));
	}

	ACS_COND_THROW((js.find(AWS_S3_PARAMETERS_KEY) == js.end()) || (js[AWS_S3_PARAMETERS_KEY].is_null()),
				AipParametersMissingKeyException() << "Key \"" << AWS_S3_PARAMETERS_KEY << 
					"\" not found into json request payload or the associated value is null. RequestId key is mandatory");

	// Read general parameters
	isDirectDlAipSet_ = getValue(js, DIRECTDL_AIP_KEY, directDlAip_, false);

	// Read aws parameters
	nlohmann::json paramsJs = js[AWS_S3_PARAMETERS_KEY];
	getValue(paramsJs, AWS_S3_ENDPOINT_KEY, awsS3Endpoint_);
	getValue(paramsJs, AWS_S3_KEYAK_KEY, awsS3KeyAK_);
	getValue(paramsJs, AWS_S3_KEYSK_KEY, awsS3KeySK_);
	getValue(paramsJs, AWS_S3_PROVIDER_KEY, awsS3Provider_);
	getValue(paramsJs, AWS_S3_REMOTE_BASEPATH_KEY, awsS3RemoteBasePath_);

	isAwsS3RegionSet_ = getValue(paramsJs, AWS_S3_REGION_KEY, awsS3Region_, false);
	isAwsS3UploadPoolSizeSet_ = getValue(paramsJs, AWS_S3_UPLOAD_POOL_SIZE_KEY, awsS3UploadPoolSize_, false);
	isAwsS3UploadBufferSizeSet_ = getValue(paramsJs, AWS_S3_UPLOAD_BUFFER_SIZE_KEY, awsS3UploadBufferSize_, false);
	isAwsS3DownloadPoolSizeSet_ = getValue(paramsJs, AWS_S3_DOWNLOAD_POOL_SIZE_KEY, awsS3DownloadPoolSize_, false);
	isAwsS3DownloadBufferSizeSet_ = getValue(paramsJs, AWS_S3_DOWNLOAD_BUFFER_SIZE_KEY, awsS3DownloadBufferSize_, false);
}

std::string AipParameters::toJsonString() {
	std::string jsStr;
	try {
		nlohmann::json js;
		nlohmann::json jsParams;

		jsParams[AWS_S3_ENDPOINT_KEY] = awsS3Endpoint_;
		jsParams[AWS_S3_KEYAK_KEY] = awsS3KeyAK_;
		jsParams[AWS_S3_KEYSK_KEY] = awsS3KeySK_;
		jsParams[AWS_S3_PROVIDER_KEY] = awsS3Provider_;
		jsParams[AWS_S3_REMOTE_BASEPATH_KEY] = awsS3RemoteBasePath_;

		if (isAwsS3RegionSet_) { jsParams[AWS_S3_REGION_KEY] = awsS3Region_; }
		if (isAwsS3UploadPoolSizeSet_) { jsParams[AWS_S3_UPLOAD_POOL_SIZE_KEY] = awsS3UploadPoolSize_; }
		if (isAwsS3UploadBufferSizeSet_) { jsParams[AWS_S3_UPLOAD_BUFFER_SIZE_KEY] = awsS3UploadBufferSize_; }
		if (isAwsS3DownloadPoolSizeSet_) { jsParams[AWS_S3_DOWNLOAD_POOL_SIZE_KEY] = awsS3DownloadPoolSize_; }
		if (isAwsS3DownloadBufferSizeSet_) { jsParams[AWS_S3_DOWNLOAD_BUFFER_SIZE_KEY] = awsS3DownloadBufferSize_; }
		
		if (isDirectDlAipSet_) { js[DIRECTDL_AIP_KEY] = directDlAip_; }
		
		js[AWS_S3_PARAMETERS_KEY] = jsParams;
		jsStr = js.dump();
	} catch (const nlohmann::json::exception& e) {
		ACS_THROW(AipParametersException(e, "Error creating JSON Object."));
	}
	 
	return jsStr;
}

std::string AipParameters::getAwsS3Endpoint() const {
	return awsS3Endpoint_;
}

std::string AipParameters::getAwsS3KeyAK() const {
	return awsS3KeyAK_;
}

std::string AipParameters::getAwsS3KeySK() const {
	return awsS3KeySK_;
}

std::string AipParameters::getAwsS3Provider() const {
	return awsS3Provider_;
}

std::string AipParameters::getAwsS3RemoteBasePath() const {
	return awsS3RemoteBasePath_;
}

std::string AipParameters::getAwsS3Region(bool& set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set = isAwsS3RegionSet_;
	return awsS3Region_;
}

unsigned short AipParameters::getAwsS3UploadPoolSize(bool& set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set = isAwsS3UploadPoolSizeSet_;
	return awsS3UploadPoolSize_;
}

size_t AipParameters::getAwsS3UploadBufferSize(bool& set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set = isAwsS3UploadBufferSizeSet_;
	return awsS3UploadBufferSize_;
}

unsigned short AipParameters::getAwsS3DownloadPoolSize(bool& set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set = isAwsS3DownloadPoolSizeSet_;
	return awsS3DownloadPoolSize_;
}

size_t AipParameters::getAwsS3DownloadBufferSize(bool& set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set = isAwsS3DownloadBufferSizeSet_;
	return awsS3DownloadBufferSize_;
}

bool AipParameters::getDirectDlAip(bool& set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	set = isDirectDlAipSet_;
	return directDlAip_;
}

void AipParameters::setAwsS3Region(const std::string& s3region) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	awsS3Region_ = s3region;
	isAwsS3RegionSet_ = true;
}

void AipParameters::setAwsS3UploadPoolSize(unsigned short uploadPoolSize) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	awsS3UploadPoolSize_ = uploadPoolSize;
	isAwsS3UploadPoolSizeSet_ = true;
}

void AipParameters::setAwsS3UploadBufferSize(size_t uploadBufferSize) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	awsS3UploadBufferSize_ = uploadBufferSize;
	isAwsS3UploadBufferSizeSet_ = true;
}

void AipParameters::setAwsS3DownloadPoolSize(unsigned short downloadPoolSize) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	awsS3DownloadPoolSize_ = downloadPoolSize;
	isAwsS3DownloadPoolSizeSet_ = true;
}

void AipParameters::setAwsS3DownloadBufferSize(size_t downloadBufferSize) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	awsS3DownloadBufferSize_ = downloadBufferSize;
	isAwsS3DownloadBufferSizeSet_ = true;
}

void AipParameters::setDirectDlAip(bool directDlAip) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	directDlAip_ = directDlAip;
	isDirectDlAipSet_ = true;
}

_ACS_END_NESTED_NAMESPACE
