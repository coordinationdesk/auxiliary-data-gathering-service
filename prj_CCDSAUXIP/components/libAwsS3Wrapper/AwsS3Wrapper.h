/*

	Copyright 2019-2022, Exprivia S.p.A. - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia S.p.A. ;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia S.p.A.

	$Prod: Wrapper for the open source AWS S3 Library - HEADER FILE $


*/


#ifndef _AMAZONS3WRAPPER_H_
#define _AMAZONS3WRAPPER_H_


#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentials.h>
#include <aws/transfer/TransferManager.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/Bucket.h>
#include <aws/s3/model/CreateBucketConfiguration.h>
#include <aws/s3/model/CreateBucketRequest.h>
#include <aws/s3/model/DeleteBucketRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/s3/model/GetBucketLocationRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/CopyObjectRequest.h>
#include <aws/s3/model/HeadBucketRequest.h>
#include <aws/s3/model/HeadObjectRequest.h>
#include <aws/s3/model/ListObjectsRequest.h>

#include <string>

#include <File.h>
#include <AwsS3WrapperConfigurationJSON.h>
#include <StopController.h>
#include <DateTime.h>
#include <StringUtils.h>
#include <exException.h>
#include <sys/stat.h>           
#include <sys/types.h>              
#include <cstring>              

_ACS_BEGIN_NAMESPACE(acs)


/*! \class AwsS3Wrapper
	
	Utility class to support the AwsS3Wrapper.
*/
class AwsS3Wrapper
{
public:
		
	enum Enum_S3Provider	{	S3P_AWS=0x00, S3P_WASABI=0x10, S3P_ORANGE=0x11, S3P_HUAWEI=0x12, S3P_CYNNYSPACE=0x13, \
								S3P_OVH=0x14, S3P_OVH_PERF=0x15, S3P_MINIO=0xa0, S3P_UNKNOWN=0xff } ;

	enum Enum_S3MetadataTag	{	S3TAG_UUID=0x00, S3TAG_TO_BE_RENAMED=0x01, S3TAG_ID1_APPL=0x10, S3TAG_ID2_APPL=0x11, S3TAG_ID3_APPL=0x12, \
								S3TAG_FORMAT1_APPL=0x20, S3TAG_FORMAT2_APPL=0x21, S3TAG_FORMAT3_APPL=0x22, S3TAG_VERSION_APPL=0x30, \
								S3TAG_CUSTOM_PAIR=0xff } ;

	class DirContent {

		// See file: README_S3_FILESYSTEM.TXT

		public:
			DirContent() : _list(), _name(ROOT_NAME), _is_file(false) {} 
			DirContent(const std::string& name, const bool is_file) : _list(), _name(name), _is_file(is_file) {} 
			virtual ~DirContent() {}
			bool operator== (const std::string& elem) const				{ return (this->_name == elem) ; }

		public:
			inline DirContent& addDirectory(const std::string& d)  		{ return addElement (d, false) ; }
			inline DirContent& addFile(const std::string& f)  			{ return addElement (f, true) ; }

			inline void clear() 										{ _list.clear() ; _name = ROOT_NAME ; _is_file = false ; }
			inline bool empty() 										{ return ( _list.empty() && (_name == ROOT_NAME) && (_is_file == false) ) ; }
			const DirContent& at(const std::string& rel_path) const	;

			std::vector < std::pair <std::string, bool> > getEntries(const std::string& root_path="") const ;
			std::vector <std::string> dumpEntries(const std::string& prefix, const std::string& directory_suffix="/") const ;

			inline const std::list< DirContent >& getList() const		{ return _list ;}
			inline std::list< DirContent >& getList() 					{ return _list ;}
			inline std::string getName() const							{ return _name ;  }
			inline bool getIsFile()	const								{ return _is_file ; }
			static std::vector <std::string> forward_vector (const std::vector <std::string>& v) ;

		private:
			static const DirContent& at(const AwsS3Wrapper::DirContent &dc, const std::vector <std::string> &path_V) ; 

			static std::string rawDump(const DirContent& dc, const std::string& t="") ;
			static void getEntries(const DirContent& dc, const std::string& dir_tree, std::vector < std::pair <std::string, bool> > &) ;
			DirContent& addElement(const std::string &path, const bool is_file) ;
			static DirContent& addElement(DirContent &dc, const std::vector <std::string> &path_V, const bool is_file) ;

		private:
			std::list< DirContent > _list ;
			std::string _name ;
			bool _is_file ;

		/* Static Constants:  */
		static const std::string ROOT_NAME ; // Marker for the first root (to be ignored)

	} ;	// END: class DirContent

	class AwsS3WrapperConfiguration {
		public:
			AwsS3WrapperConfiguration() : _aws_s3_provider(AwsS3Wrapper::S3P_UNKNOWN), _aws_key_ak(), _aws_key_sk(), \
					_upload_pool_size(AwsS3Wrapper::s3_defaultUpload_pool_size), \
					_upload_buffer_size(AwsS3Wrapper::s3_defaultUpload_buffer_size), \
					_download_pool_size(AwsS3Wrapper::s3_defaultDownload_pool_size), \
					_download_buffer_size(AwsS3Wrapper::s3_defaultDownload_buffer_size), \
					_aws_region(), _aws_endpoint(), _aws_remote_basepath(), _options(), _endpoint_is_secure(false) {}
			virtual ~AwsS3WrapperConfiguration() throw() {}
			AwsS3Wrapper::Enum_S3Provider get_aws_s3_provider() const 		{ return _aws_s3_provider ; }
			std::string	get_aws_key_ak() const								{ return _aws_key_ak ; }
			std::string get_aws_key_sk() const								{ return _aws_key_sk ; }
			uint16_t get_upload_pool_size() const  		  					{ return _upload_pool_size ; }
			uint64_t get_upload_buffer_size() const		  					{ return _upload_buffer_size ; }
			uint16_t get_download_pool_size() const  		  				{ return _download_pool_size ; }
			uint64_t get_download_buffer_size() const		  				{ return _download_buffer_size ; }
			std::string get_aws_region() const								{ return _aws_region ; }
			std::string get_aws_endpoint() const							{ return _aws_endpoint ; }  
			std::string get_aws_remote_basepath() const						{ return _aws_remote_basepath ; }
			std::string get_options() const									{ return _options ; }
			bool get_endpoint_is_secure() const								{ return _endpoint_is_secure ; }

			void set_aws_s3_provider(const AwsS3Wrapper::Enum_S3Provider& v){ _aws_s3_provider = v ; }
			void set_aws_key_ak(const std::string& v)						{ _aws_key_ak = v ; }
			void set_aws_key_sk(const std::string& v)						{ _aws_key_sk = v ; }
			void set_upload_pool_size(const uint16_t& v)					{ _upload_pool_size = v ; }
			void set_upload_buffer_size(const uint64_t& v)					{ _upload_buffer_size = v ; }
			void set_download_pool_size(const uint16_t& v)					{ _download_pool_size = v ; }
			void set_download_buffer_size(const uint64_t& v)				{ _download_buffer_size = v ; }
			void set_aws_region(const std::string& v)						{ _aws_region = v ; }
			void set_aws_endpoint(const std::string& v)						{ _aws_endpoint = v ; }  
			void set_aws_remote_basepath(const std::string& v)				{ _aws_remote_basepath = v ; }
			void set_options(const std::string& v) 							{ _options = v; }
			void set_endpoint_is_secure(const bool v) 						{ _endpoint_is_secure = v ; }

		public:
			void import_from_json(const std::string& json_string) ;
			void import_from_json(const AwsS3WrapperConfigurationJSON& json_handler) ;
			void export_to_json(AwsS3WrapperConfigurationJSON& json_handler) const ;
			void json_self_test() const ;

		private:
			AwsS3Wrapper::Enum_S3Provider	_aws_s3_provider ;
			std::string						_aws_key_ak ;
			std::string						_aws_key_sk ;
			uint16_t						_upload_pool_size ; 
			uint64_t						_upload_buffer_size ; 
			uint16_t						_download_pool_size ; 
			uint64_t						_download_buffer_size ; 
			std::string						_aws_region ;
			std::string						_aws_endpoint ;	
			std::string						_aws_remote_basepath ;
			std::string						_options ;	// Not used
			bool							_endpoint_is_secure ;

	} ;	/* END: AwsS3WrapperConfiguration */

	
public:
	exDECLARE_EXCEPTION(AwsS3WrapperException, exException) ; 
	exDECLARE_EXCEPTION(AwsS3WrapperNoSuchBucketException, AwsS3WrapperException) ; 		// Invalid [remote] bucket
	exDECLARE_EXCEPTION(AwsS3WrapperNoSuchObjectException, AwsS3WrapperException) ; 		// Invalid [remote] object (file or dir)
	exDECLARE_EXCEPTION(AwsS3WrapperNoSuchLocalFileException, AwsS3WrapperException) ; 	// Invalid local file on filesystem [local, not S3]
	exDECLARE_EXCEPTION(AwsS3WrapperConnectionException, AwsS3WrapperException) ; 		// Connection failed


public:
	AwsS3Wrapper(const Enum_S3Provider& s3provider, const Aws::Auth::AWSCredentials&, StopController const &) ;
	AwsS3Wrapper(const Enum_S3Provider& s3provider, const Aws::Auth::AWSCredentials&, const std::string& region, \
					StopController const &) ;
	AwsS3Wrapper(const Enum_S3Provider& s3provider, const Aws::Auth::AWSCredentials&, const std::string& region, \
					const std::string& endpoint, StopController const&) ;
	AwsS3Wrapper(const AwsS3WrapperConfiguration &aws_configuration, StopController const&) ;
	virtual ~AwsS3Wrapper() throw() ;

public:

	// This is the API:

	// Buckets operations
	bool isBucket(const std::string& bucket_name, const bool emit_exception=false) const ;
	std::vector <std::string> listBuckets() const ;
	std::string getBucketRegion(const std::string& bucket_name, const bool override_unset) const ;
	void createBucket(const std::string& region, const std::string& bucket_name) ;
	inline void createBucket(const std::string& bucket_name)				{ createBucket(region_, bucket_name) ; }
	void removeBucket(const std::string& region, const std::string& bucket_name) ;
	inline void removeBucket(const std::string& bucket_name)				{ removeBucket (region_, bucket_name) ; }

	// Objects operations
	bool isObject(const std::string& bucket, const std::string& s3key, const bool emit_exception=false) const ;
	bool isFileObject(const std::string& bucket, const std::string& s3key, const bool emit_exception=false) const ;
	bool isDirectoryObject(const std::string& bucket, const std::string& s3key, const bool emit_exception=false) const ;
	void removeSingleObject(const std::string& bucket, const std::string& s3key) ;
	bool multiRemoveObjects(const std::string& bucket, const std::string& remote_rel_path) ;
	std::string createDirectory(const std::string& bucket, const std::string& remote_rel_path, const bool is_recursive=true) ;
	std::string uploadSmallObject(const std::string& local_object_source, const std::string& bucket, const std::string& s3key) ;
	inline std::string uploadObject(const std::string& local_object_source, const std::string& bucket, const std::string& s3key) 
	{ return uploadObject( local_object_source, bucket, s3key, std::multimap <Enum_S3MetadataTag, std::string>()) ; }
	std::string uploadObject(const std::string& local_object_source, const std::string& bucket, const std::string& s3key, \
									const std::multimap <Enum_S3MetadataTag, std::string>& tags_M) ;
	off64_t downloadObject(const std::string& bucket, const std::string& s3key, const std::string& local_object_dest) ;
	std::string copyObject(const std::string& bucket_in, const std::string& s3key_in, \
							const std::string& bucket_out, const std::string& s3key_out) const ;
	Aws::S3::Model::HeadObjectResult getHeadObjectResult(const std::string& bucket_name_in, const std::string& object_in) const ;	
	std::map <std::string, std::string> getObjectMetadata(const std::string& bucket, const std::string& object) const ;
	std::multimap <Enum_S3MetadataTag, std::string> getObjectMetadataTags(const std::string& bucket, const std::string& object) const ;
	inline off64_t getObjectSize(const std::string& bucket, const std::string& object) const \
		{ return getHeadObjectResult(bucket, object).GetContentLength() ; }
	inline DateTime getObjectModificationTime(const std::string& bucket, const std::string& object) const \
		{ return to_DateTime (getHeadObjectResult(bucket, object).GetLastModified()) ; }
		

	void listContent(const std::string& bucket, const std::string& rel_path, DirContent&) const ;
	inline void listContent(const std::string& bucket, DirContent& d) const { return listContent(bucket, "", d) ; }

	// Setters
	inline void	set_download_pool_size(/* As default*/)		{ set_download_pool_size(0) ; }
	inline void	set_download_pool_size(const uint16_t& v)		
	{ download_pool_size_ = ( (v >= 1)? v : s3_defaultDownload_pool_size ) ; } 
	inline void set_download_buffer_size(/* As default*/)	{ set_download_buffer_size(0) ;  }
	inline void set_download_buffer_size(const uint64_t& v)	
	{ download_buffer_size_ = ( (v > 0)? v : s3_defaultDownload_buffer_size ) ; }
	
	inline void	set_upload_pool_size(/* As default*/)		{ set_upload_pool_size(0) ; }
	inline void	set_upload_pool_size(const uint16_t& v)		
	{ upload_pool_size_ = ( (v >= 1)? v : s3_defaultUpload_pool_size ) ; } 
	inline void set_upload_buffer_size(/* As default*/)		{ set_upload_buffer_size(0) ;  }
	inline void set_upload_buffer_size(const uint64_t& v)	
	{ upload_buffer_size_ = ( (v > 0)? v : s3_defaultUpload_buffer_size ) ; }

	// Getters
	inline Enum_S3Provider get_s3provider() const			{ return s3provider_ ; }
	inline std::string get_region() const					{ return region_ ; }
	inline std::string get_endpoint() const					{ return endpoint_ ;}
	inline bool get_endpoint_is_secure() const					{ return endpoint_is_secure_ ;}
	inline uint16_t	get_download_pool_size() const			{ return download_pool_size_ ; }
	inline uint64_t get_download_buffer_size() const		{ return download_buffer_size_ ; }
	inline uint16_t	get_upload_pool_size() const			{ return upload_pool_size_ ; }
	inline uint64_t get_upload_buffer_size() const			{ return upload_buffer_size_ ; }

public:

	// Utility / Service methods:

	bool diagnosticSelfTest(const std::string& bucket_name_in, const bool emit_exception = true) ; // Run a diagnostic self-test. Return true if success

	static void relative_to_absolute_path(const std::string& bucket_in, const std::string& relative_path_in, std::string& absolute_path_out, bool force_prefix_s3 = false) ;
	static void absolute_to_relative_path(const std::string& absolute_path_in, std::string& bucket_out, std::string& relative_path_out) ;
	static void disassemble_full_path(const std::string& full_path_in, std::string& endpoint_out, std::string& bucket_out, std::string& relative_path_out) ;

	static std::string dump(const std::vector <std::string>&, const std::string& t="") ; 
	static std::string dump(const std::multimap <Enum_S3MetadataTag, std::string>, const std::string& t="") ; 
	
	static std::string toStr(const Aws::Transfer::TransferStatus t) ;
	static Aws::S3::Model::BucketLocationConstraint regionFromString(const Enum_S3Provider&, const std::string&, bool emit_exception=true) ;
	static std::string stringFromRegion(const Aws::S3::Model::BucketLocationConstraint&, bool emit_exception=true) ;
	
	static std::string toStr(const Enum_S3Provider&) ;
	static Enum_S3Provider to_S3Provider(const std::string &s) ;

	static std::string toStr(const Enum_S3MetadataTag&) ;
	static Enum_S3MetadataTag to_S3MetadataTag(const std::string &s) ;

	inline static std::string    to_string(const Aws::String& s_in)	{ std::string v ; return (s_in.empty()? v : std::string(s_in.c_str(), s_in.length()) ); }	
	static DateTime    	to_DateTime(const Aws::Utils::DateTime& aws_epoch) ;
	inline static Aws::String to_AwsString(const std::string& s_in)	{ Aws::String v ; return (s_in.empty()? v : Aws::String(s_in.c_str(), s_in.length()) ); }

	inline std::vector<std::string> get_list_of_regions() const		{ return get_list_of_regions(s3provider_) ; }
	static std::vector<std::string> get_list_of_regions(const Enum_S3Provider&) ;	

	// Download / Upload callback 
	static void downloadProgressCallback(const Aws::Transfer::TransferManager* tm, const std::shared_ptr<const Aws::Transfer::TransferHandle>& th) ;
	static void uploadProgressCallback(const Aws::Transfer::TransferManager* tm, const std::shared_ptr<const Aws::Transfer::TransferHandle>& th) ;
	
private:
	AwsS3Wrapper(const AwsS3Wrapper &) ;
	AwsS3Wrapper &operator=(const AwsS3Wrapper &) ;

	void apply_specific_configuration(const Enum_S3Provider& s3provider, const std::string& endpoint,
		Aws::Client::ClientConfiguration& client_configuration, bool& useVirtualAddressing) ;
	bool internalIsObject(const std::string& bucket_name_in, const std::string& object_in, const bool emit_exception) const ;
	inline std::string get_default_region() const					{ return get_default_region(s3provider_) ; }
	inline std::string get_unset_region() const						{ return get_unset_region(s3provider_) ; }	
	inline std::string get_default_endpoint() const					{ return get_default_endpoint(s3provider_, region_) ; }	

	static std::string get_default_region(const Enum_S3Provider&) ;
	static std::string get_unset_region(const Enum_S3Provider&) ;	
	static std::string get_default_endpoint(const Enum_S3Provider&, const std::string& region) ;	
	static std::string strip_s3protocol(const std::string& path) ;	
	static inline void strip_s3protocol(std::string& path) 			{ const std::string tmp(path) ; \
																		path = strip_s3protocol(tmp) ; }
	static std::string forge_range(const off64_t& position, const off64_t &length) ;

	
	Aws::Auth::AWSCredentials 					credentials_ ;	
	Enum_S3Provider								s3provider_ ;
	std::string									region_ ;
	std::string									endpoint_ ;
	bool										endpoint_is_secure_ ;
	uint16_t									download_pool_size_ ;
	uint64_t									download_buffer_size_ ;
	uint16_t									upload_pool_size_ ;
	uint64_t									upload_buffer_size_ ;
	std::shared_ptr<Aws::S3::S3Client>			s3_client_p_ ;
	StopController const &						stopController_ ;


public:
	
	/* Public Static Constants:  */
	static const uint32_t s3_defaultConnectTimeoutMs, s3_defaultRequestTimeoutMs ;
	static const uint16_t s3_defaultDownload_pool_size ;
	static const uint64_t s3_defaultDownload_buffer_size ;
	static const uint16_t s3_defaultUpload_pool_size ;
	static const uint64_t s3_defaultUpload_buffer_size ;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(AwsS3Wrapper) ;
} ;


std::ostream &operator<<(std::ostream &, const AwsS3Wrapper::AwsS3WrapperConfiguration&) ;  
acs::exostream &operator<<(acs::exostream &, const AwsS3Wrapper::AwsS3WrapperConfiguration&) ;  


_ACS_END_NAMESPACE

#endif	/* _AMAZONS3WRAPPER_H_ */
