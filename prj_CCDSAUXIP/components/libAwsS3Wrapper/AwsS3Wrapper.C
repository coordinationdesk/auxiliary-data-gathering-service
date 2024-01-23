/*

	Copyright 2019-2022, Exprivia S.p.A. - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia S.p.A. ;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia S.p.A.

	$Prod: Wrapper for the open source AWS S3 Library  $


*/


#include <AwsS3WrapperConfiguration.h>
#include <AwsS3Wrapper.h>
#include <exException.h>
#include <Filterables.h>
#include <File.h>
#include <SpawnProc.h>
#include <StringUtils.h>
#include <cstring>
#include <fcntl.h> 
#include <sys/types.h> 
#include <sys/statfs.h> 
#include <errno.h> 

#include <s3V.h>

namespace {
    acs::s3V version ; // versioning 
}

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;



////////////////////////////////////////////////////////////////////////////
//
//   class AwsS3Wrapper
//
////////////////////////////////////////////////////////////////////////////

ACS_CLASS_DEFINE_DEBUG_LEVEL(AwsS3Wrapper) 

const string AwsS3Wrapper::DirContent::ROOT_NAME("__AwsS3Wrapper__UNUSED_ROOT_NAME__") ; // Marker for the first root (to be ignored)
const uint32_t AwsS3Wrapper::s3_defaultConnectTimeoutMs 	(120 /* unit is [s] */  *1000) ;
const uint32_t AwsS3Wrapper::s3_defaultRequestTimeoutMs 	(120 /* unit is [s] */  *1000) ;
const uint16_t AwsS3Wrapper::s3_defaultDownload_pool_size 	(1) ; 
const uint64_t AwsS3Wrapper::s3_defaultDownload_buffer_size	(4*1024*1024) ; 
const uint16_t AwsS3Wrapper::s3_defaultUpload_pool_size 	(1) ; 
const uint64_t AwsS3Wrapper::s3_defaultUpload_buffer_size	(4*1024*1024) ; 


////////////////////////////////////////////////////////////////////////////
//
//   class AwsS3Wrapper::AwsS3WrapperConfiguration::import_from_json
//
////////////////////////////////////////////////////////////////////////////
void AwsS3Wrapper::AwsS3WrapperConfiguration::import_from_json(const string& js) 
{
	AwsS3WrapperConfigurationJSON handler ;
	handler.fromJSON(js);
	import_from_json(handler) ; 
}


////////////////////////////////////////////////////////////////////////////
//
//   class AwsS3Wrapper::AwsS3WrapperConfiguration::import_from_json
//
////////////////////////////////////////////////////////////////////////////
void AwsS3Wrapper::AwsS3WrapperConfiguration::import_from_json(const AwsS3WrapperConfigurationJSON& handler) 
{
	bool aws_S3_ProviderSet	; handler.getAWS_S3_Provider(&aws_S3_ProviderSet) ;
	
	if (true == aws_S3_ProviderSet)
	{
		const string aws_s3_provider(handler.getAWS_S3_Provider()) ;
		set_aws_s3_provider( to_S3Provider(aws_s3_provider) ) ;
		set_aws_key_ak( handler.getAWS_S3_Key_AK() ) ;							
		set_aws_key_sk( handler.getAWS_S3_Key_SK() ) ;							
		set_upload_pool_size( handler.getAWS_S3_Upload_Pool_Size() ) ;					
		set_upload_buffer_size( handler.getAWS_S3_Upload_Buffer_Size() ) ;				
		set_download_pool_size( handler.getAWS_S3_Download_Pool_Size() ) ;				
		set_download_buffer_size( handler.getAWS_S3_Download_Buffer_Size() ) ;				
		set_aws_region( handler.getAWS_S3_Region() ) ;
		set_aws_endpoint( handler.getAWS_S3_Endpoint() ) ;			
		set_endpoint_is_secure( handler.getAWS_S3_Endpoint_Is_Secure() ) ;			
		set_aws_remote_basepath( handler.getAWS_S3_RemoteBasepath() ) ;	
	}
}


////////////////////////////////////////////////////////////////////////////
//
//   class AwsS3Wrapper::AwsS3WrapperConfiguration::export_to_json
//
////////////////////////////////////////////////////////////////////////////
void AwsS3Wrapper::AwsS3WrapperConfiguration::export_to_json(AwsS3WrapperConfigurationJSON& handler) const
{
	const string aws_s3_provider(toStr(get_aws_s3_provider())) ;
	
	handler.setAWS_S3_Provider( aws_s3_provider) ;
	handler.setAWS_S3_Key_AK( get_aws_key_ak() );
	handler.setAWS_S3_Key_SK( get_aws_key_sk() );
	handler.setAWS_S3_Upload_Pool_Size( get_upload_pool_size() );
	handler.setAWS_S3_Upload_Buffer_Size( get_upload_buffer_size() );
	handler.setAWS_S3_Download_Pool_Size( get_download_pool_size() );
	handler.setAWS_S3_Download_Buffer_Size( get_download_buffer_size() );
	handler.setAWS_S3_RemoteBasepath( get_aws_remote_basepath() );
	handler.setAWS_S3_Region( get_aws_region() );
	handler.setAWS_S3_Endpoint( get_aws_endpoint() );
	handler.setAWS_S3_Endpoint_Is_Secure( get_endpoint_is_secure() );
}


////////////////////////////////////////////////////////////////////////////
//
//   class AwsS3Wrapper::AwsS3WrapperConfiguration::json_self_test
//
////////////////////////////////////////////////////////////////////////////
void AwsS3Wrapper::AwsS3WrapperConfiguration::json_self_test() const 
{
	const string aws_s3_provider("ORANGE") ;
	const string aws_key_ak("AKAKAK") ;
	const string aws_key_sk("SKSKSK") ;
	const string aws_region("Region") ;
	const string aws_endpoint("http://s3.example.com") ;
	const bool endpoint_is_secure(true) ;
	const string aws_remote_basepath("s3://buket/dir1/dir2") ;
	const uint16_t upload_pool_size(16) ;
	const uint64_t upload_buffer_size(123456) ;
	const uint16_t download_pool_size(8) ;
	const uint64_t download_buffer_size(654321) ;

	{
		AwsS3WrapperConfigurationJSON handler ;
		
		AwsS3WrapperConfiguration s3_conf ;
		s3_conf.set_aws_s3_provider(to_S3Provider(aws_s3_provider));
		s3_conf.set_aws_key_ak(aws_key_ak) ;
		s3_conf.set_aws_key_sk(aws_key_sk) ;
		s3_conf.set_upload_pool_size(upload_pool_size) ;
		s3_conf.set_upload_buffer_size(upload_buffer_size) ;
		s3_conf.set_download_pool_size(download_pool_size) ;
		s3_conf.set_download_buffer_size(download_buffer_size) ;
		s3_conf.set_aws_region(aws_region) ;
		s3_conf.set_aws_endpoint(aws_endpoint) ;
		s3_conf.set_endpoint_is_secure(endpoint_is_secure) ;
		s3_conf.set_aws_remote_basepath(aws_remote_basepath) ;

		s3_conf.export_to_json(handler) ; 
		ACS_COND_THROW( (aws_s3_provider !=  	 handler.getAWS_S3_Provider()), 			exIllegalValueException("Mismatch: aws_s3_provider")) ; 																				 
		ACS_COND_THROW( (aws_key_ak !=			 handler.getAWS_S3_Key_AK()),   			exIllegalValueException("Mismatch: aws_key_ak")) ;  																					 
		ACS_COND_THROW( (aws_key_sk !=			 handler.getAWS_S3_Key_SK()),   			exIllegalValueException("Mismatch: aws_key_sk")) ;  																					 
		ACS_COND_THROW( (aws_region !=			 handler.getAWS_S3_Region()), 				exIllegalValueException("Mismatch: aws_region")) ;  																
		ACS_COND_THROW( (aws_endpoint != 		 handler.getAWS_S3_Endpoint()), 			exIllegalValueException("Mismatch: aws_endpoint")) ;																	
		ACS_COND_THROW( (endpoint_is_secure != 	 handler.getAWS_S3_Endpoint_Is_Secure()), 	exIllegalValueException("Mismatch: endpoint_is_secure")) ;																	
		ACS_COND_THROW( (aws_remote_basepath !=  handler.getAWS_S3_RemoteBasepath()),   	exIllegalValueException("Mismatch: aws_remote_basepath")) ; 																		
		ACS_COND_THROW( (upload_pool_size != 	 handler.getAWS_S3_Upload_Pool_Size()), 	exIllegalValueException("Mismatch: upload_pool_size")) ;																
		ACS_COND_THROW( (upload_buffer_size !=	 handler.getAWS_S3_Upload_Buffer_Size()),   exIllegalValueException("Mismatch: upload_buffer_size")) ;  																 
		ACS_COND_THROW( (download_pool_size !=	 handler.getAWS_S3_Download_Pool_Size()),   exIllegalValueException("Mismatch: download_pool_size")) ;  																			 
		ACS_COND_THROW( (download_buffer_size != handler.getAWS_S3_Download_Buffer_Size()), exIllegalValueException("Mismatch: download_buffer_size")) ;																			 
	}
	
		   
	{
		AwsS3WrapperConfiguration s3_conf ;
		
		AwsS3WrapperConfigurationJSON handler ;
		ostringstream ss;
		ss << "{\n";
		ss << "    \"AWS_S3_Provider\":\"ORANGE\",\n";
		ss << "    \"AWS_S3_Key_AK\":\"AKAKAK\",\n";
		ss << "    \"AWS_S3_Key_SK\":\"SKSKSK\",\n";
		ss << "    \"AWS_S3_Upload_Pool_Size\":16,\n";
		ss << "    \"AWS_S3_Upload_Buffer_Size\":123456,\n";
		ss << "    \"AWS_S3_Download_Pool_Size\":8,\n";
		ss << "    \"AWS_S3_Download_Buffer_Size\":654321,\n";
		ss << "    \"AWS_S3_RemoteBasepath\":\"s3://buket/dir1/dir2\",\n";
		ss << "    \"AWS_S3_Region\":\"Region\",\n";
		ss << "    \"AWS_S3_Endpoint\":\"http://s3.example.com\",\n";
		ss << "    \"AWS_S3_Endpoint_Is_Secure\":true\n";
		ss << "}" ;
		handler.fromJSON(ss.str());


		s3_conf.import_from_json(handler) ;
		ACS_COND_THROW( (aws_s3_provider !=  	 toStr(s3_conf.get_aws_s3_provider())),	exIllegalValueException("Mismatch: aws_s3_provider")) ;    
		ACS_COND_THROW( (aws_key_ak !=			 s3_conf.get_aws_key_ak()), 		    exIllegalValueException("Mismatch: aws_key_ak")) ;  	   
		ACS_COND_THROW( (aws_key_sk !=			 s3_conf.get_aws_key_sk()), 		    exIllegalValueException("Mismatch: aws_key_sk")) ;  	   
		ACS_COND_THROW( (aws_region !=			 s3_conf.get_aws_region()),  		    exIllegalValueException("Mismatch: aws_region")) ;  	   
		ACS_COND_THROW( (aws_endpoint != 		 s3_conf.get_aws_endpoint()),		    exIllegalValueException("Mismatch: aws_endpoint")) ;	   
		ACS_COND_THROW( (endpoint_is_secure != 		 s3_conf.get_endpoint_is_secure()),		    exIllegalValueException("Mismatch: endpoint_is_secure")) ;	   
		ACS_COND_THROW( (aws_remote_basepath !=  s3_conf.get_aws_remote_basepath()),    exIllegalValueException("Mismatch: aws_remote_basepath")) ;   
		ACS_COND_THROW( (upload_pool_size != 	 s3_conf.get_upload_pool_size()),	    exIllegalValueException("Mismatch: upload_pool_size")) ;   
		ACS_COND_THROW( (upload_buffer_size !=	 s3_conf.get_upload_buffer_size()),     exIllegalValueException("Mismatch: upload_buffer_size")) ; 
		ACS_COND_THROW( (download_pool_size !=	 s3_conf.get_download_pool_size()),     exIllegalValueException("Mismatch: download_pool_size")) ; 
		ACS_COND_THROW( (download_buffer_size != s3_conf.get_download_buffer_size()),   exIllegalValueException("Mismatch: download_buffer_size")) ;   
	}		   
			   
			   
	{
		AwsS3WrapperConfiguration s3_conf ;

		AwsS3WrapperConfigurationJSON handler ;
		handler.setAWS_S3_Provider(aws_s3_provider);
		handler.setAWS_S3_Key_AK(aws_key_ak);
		handler.setAWS_S3_Key_SK(aws_key_sk);
		handler.setAWS_S3_Upload_Pool_Size(upload_pool_size);
		handler.setAWS_S3_Upload_Buffer_Size(upload_buffer_size);
		handler.setAWS_S3_Download_Pool_Size(download_pool_size);
		handler.setAWS_S3_Download_Buffer_Size(download_buffer_size);
		handler.setAWS_S3_RemoteBasepath(aws_remote_basepath);
		handler.setAWS_S3_Region(aws_region);
		handler.setAWS_S3_Endpoint(aws_endpoint);
		handler.setAWS_S3_Endpoint_Is_Secure(endpoint_is_secure);
		
		s3_conf.import_from_json(handler) ;
		ACS_COND_THROW( (aws_s3_provider !=  	 toStr(s3_conf.get_aws_s3_provider())),	exIllegalValueException("Mismatch: aws_s3_provider")) ;    
		ACS_COND_THROW( (aws_key_ak !=			 s3_conf.get_aws_key_ak()), 		    exIllegalValueException("Mismatch: aws_key_ak")) ;  	   
		ACS_COND_THROW( (aws_key_sk !=			 s3_conf.get_aws_key_sk()), 		    exIllegalValueException("Mismatch: aws_key_sk")) ;  	   
		ACS_COND_THROW( (aws_region !=			 s3_conf.get_aws_region()), 		    exIllegalValueException("Mismatch: aws_region")) ;  	  
		ACS_COND_THROW( (aws_endpoint != 		 s3_conf.get_aws_endpoint()),		    exIllegalValueException("Mismatch: aws_endpoint")) ;	  
		ACS_COND_THROW( (endpoint_is_secure != 		 s3_conf.get_endpoint_is_secure()),		    exIllegalValueException("Mismatch: endpoint_is_secure")) ;	  
		ACS_COND_THROW( (aws_remote_basepath !=  s3_conf.get_aws_remote_basepath()),    exIllegalValueException("Mismatch: aws_remote_basepath")) ; 
		ACS_COND_THROW( (upload_pool_size != 	 s3_conf.get_upload_pool_size()),	    exIllegalValueException("Mismatch: upload_pool_size")) ;	 
		ACS_COND_THROW( (upload_buffer_size !=	 s3_conf.get_upload_buffer_size()),     exIllegalValueException("Mismatch: upload_buffer_size")) ;
		ACS_COND_THROW( (download_pool_size !=	 s3_conf.get_download_pool_size()),     exIllegalValueException("Mismatch: download_pool_size")) ;
		ACS_COND_THROW( (download_buffer_size != s3_conf.get_download_buffer_size()),   exIllegalValueException("Mismatch: download_buffer_size")) ;	
	}		   

	return ; 
}	


/**********************************************************************************
  *	                                                                              *
  *	  DirContent::rawDump
  *	                                                                              *
  **********************************************************************************/
string AwsS3Wrapper::DirContent::rawDump(const AwsS3Wrapper::DirContent& dc, const string& t)
{
	string s ;
	for (list< DirContent >::const_iterator it = dc._list.begin() ; it != dc._list.end() ; it ++ )
	{
		if (true == it->getIsFile()) { s += t + "FILE: " + it->getName() + "\n" ; }
		else { s += (t + "DIR: " + it->getName() + "\n") + rawDump (*it, (t+"\t")) ; }
	}
	
	return s ;
}


/**********************************************************************************
  *	                                                                              *
  *	  DirContent::addElement
  *	                                                                              *
  **********************************************************************************/
AwsS3Wrapper::DirContent& AwsS3Wrapper::DirContent::addElement(const string &path, const bool is_file)
{
	ACS_CLASS_WRITE_DEBUG_TH( ACS_HIG_VERB, "Method called. Full path: \"" + path + "\"  dir/file: " << (is_file?"file":"dir") ) ;

	const bool removeLeadingSlash(true) ; 
	const bool removeLeadingDot(true) ; 
	const vector <string> path_V ( StringUtils::pathDisassemble (path, removeLeadingSlash, removeLeadingDot) ) ;
	return addElement(*this, path_V, is_file) ;
}


/**********************************************************************************
  *	                                                                              *
  *	  DirContent::addElement
  *	                                                                              *
  **********************************************************************************/
AwsS3Wrapper::DirContent& AwsS3Wrapper::DirContent::addElement(AwsS3Wrapper::DirContent &dc, const vector <string> &path_V, const bool is_file)
{
	ACS_CLASS_WRITE_DEBUG_TH( ACS_HIG_VERB, "Method called. Full path size: " << path_V.size() << "  dir/file: " << (is_file?"file":"dir") ) ;

	if (path_V.size() >= 1)
	{
		// Cross all the intermediate levels, up to the last
		for (size_t i=0; i<path_V.size()-1; i++)
		{
			const string element_dir (path_V.at(i)); 	// Being an INTERMEDIATE level (not the last, by definition) is't assumed to be a directory !

			list< DirContent >::iterator it ( std::find( dc.getList().begin(), dc.getList().end(), element_dir ) ) ;

			if ( dc.getList().end() == it ) { 
				DirContent d(element_dir, false) ; dc.getList().push_back(d) ; return addElement( dc.getList().back(), forward_vector(path_V), is_file) ;
			}
			else { 
				ACS_COND_THROW ( (true == it->getIsFile()), exIllegalValueException("Invalid intermediate level: not a directory!") ) ;
				return addElement( *it, forward_vector(path_V), is_file) ;		
			}
		}

		const string element (path_V.at( path_V.size()-1 )); 	// Last level to add
		DirContent d(element, is_file) ; dc.getList().push_back(d) ;
	}
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Method finished" ) ;

	return dc.getList().back();
}


/**********************************************************************************
  *	                                                                              *
  *	  DirContent::at
  *	                                                                              *
  **********************************************************************************/
const AwsS3Wrapper::DirContent& AwsS3Wrapper::DirContent::at(const string& rel_path) const
{
	const bool removeLeadingSlash(true) ;
	const bool removeLeadingDot(false) ;
		
	// Disassemble the path as a vector -- this makes easier to extract the first part
	const vector <string> rel_path_V ( StringUtils:: pathDisassemble (StringUtils::pathNormalize(rel_path, removeLeadingSlash, removeLeadingDot)) ) ;
	
	return at( *this, rel_path_V ) ;
}


/**********************************************************************************
  *	                                                                              *
  *	  DirContent::at
  *	                                                                              *
  **********************************************************************************/
const AwsS3Wrapper::DirContent& AwsS3Wrapper::DirContent::at(const AwsS3Wrapper::DirContent &dc, const vector <string> &path_V) 	
{
	if (path_V.size() >= 1)
	{
		const string element(path_V.at(0)); 
		
		list< DirContent >::const_iterator it ( std::find( dc.getList().begin(), dc.getList().end(), element ) ) ;
		
		if ( dc.getList().end() == it ) { ACS_THROW ( exIllegalValueException( "Level: \"" + element + "\" not found") ) ; }

		if (path_V.size() >= 2) { return at( *it, forward_vector(path_V) ) ; }
		else { return *it ; }
	}
	
	return dc ; // fallback
}


/**********************************************************************************
  *	                                                                              *
  *	  DirContent::getEntries
  *	                                                                              *
  **********************************************************************************/
void AwsS3Wrapper::DirContent::getEntries(const AwsS3Wrapper::DirContent& dc, const string& dir_tree, vector < pair <string, bool> > &out) 
{
	/*
		Return a vector with the entries of the tree, starting from "dc".
		This is a private method, see also: getEntries(const string& root_path)
	*/

	 out.push_back( pair <string, bool>( (dir_tree + ((ROOT_NAME != dc.getName())? ("/" + dc.getName()) : "")), dc.getIsFile()) ) ; 

	if ( false == dc.getList().empty() ) 
	{
		for (std::list< DirContent >::const_iterator it = dc.getList().begin(); it != dc.getList().end(); it ++)
		{ getEntries (*it, (dir_tree + ((ROOT_NAME != dc.getName())? ("/" + dc.getName()) : "")), out) ; } 
	}
	else 
	{ 
		/* It has reached a leaf ... -- do nothing */ 
	}

	return  ;
}

/**********************************************************************************
  *	                                                                              *
  *	  DirContent::getEntries
  *	                                                                              *
  **********************************************************************************/
vector < pair <string, bool> > AwsS3Wrapper::DirContent::getEntries(const string& root_path) const 
{ 
	/*
		Return a vector with the entries of the tree, starting from the root.
		Each element in a vector is a pair: string entry_name, bool is_file
		The visit is in forward-order, i.e. starting from the less deep ("dir1", "dir1/dir2", useful if you want to create or copy a tree) up to the deepest
		You might want to _reverse_ the vector (just using a reverse iterator) to remove a tree

		If "root_path" is empty, the tree will be returned as relative, e.g.
		"FILE1", true
		"DIR1/DIR2", false
		"DIR1/DIR3/FILE3", true

		If "root_path" is not empty, each will be simply preprended with the root_path.
		For example, if root_path has been set with "/MYBUCKET" this would be the result:
		"/MYBUCKET/FILE1", true
		"/MYBUCKET/DIR1/DIR2", false
		"/MYBUCKET/DIR1/DIR3/FILE3", true
	
	*/

	const bool is_relative ( root_path.empty() ) ;
	
	vector < pair <string, bool> > v ; getEntries (*this, root_path, v) ; 
	
	if (true == is_relative)
	{
		// Turn the entries as relative:
		const bool removeLeadingSlash(true) ;  const bool removeLeadingDot(false) ; 

		for (size_t i=0; i<v.size(); i++)
		{ v[i].first = StringUtils::pathNormalize(v.at(i).first, removeLeadingSlash, removeLeadingDot) ; }
	}
	
	return v ; 
}


/**********************************************************************************
  *	                                                                              *
  *	  DirContent::dumpEntries
  *	                                                                              *
  **********************************************************************************/
vector < string > AwsS3Wrapper::DirContent::dumpEntries(const string& prefix, const string& directory_suffix) const 
{ 
	/*
		Return a vector with the entries of the tree, starting from the root.
		The directory suffix is used to "signal" when the latest level is a directory, for example with a trailing "/".
		In this example we assume that directory_suffix="/".

		If "root_path" is empty, the tree will be returned as relative, e.g.
		"FILE1"
		"DIR1/DIR2/"  [note the suffix, to signal it's a directory]
		"DIR1/DIR3/FILE3"

		If "root_path" is not empty, each will be simply preprended with the root_path.
		For example, if root_path has been set with "/MYBUCKET" this would be the result:
		"/MYBUCKETFILE1"
		"/MYBUCKETDIR1/DIR2/"  [note the suffix, to signal it's a directory]
		"/MYBUCKETDIR1/DIR3/FILE3"
	
	*/

	const bool is_relative ( prefix.empty() ) ;
	const bool removeLeadingSlash(true) ; const bool removeLeadingDot(false) ; 

	vector < pair <string, bool> > v ; getEntries (*this, prefix, v) ; 
	vector < string > v_out ;	
	

	for (size_t i=0; i<v.size(); i++)
	{ 
		string entry (v.at(i).first) ;
		const bool is_dir (! v.at(i).second) ; // The NOT is needed, since the vector marks the opposite (is_file)
		if (true == is_relative) { entry = StringUtils::pathNormalize(entry, removeLeadingSlash, removeLeadingDot) ; }
		if ( (true == is_dir) && (false == directory_suffix.empty()) ) { entry += directory_suffix ; }
		
		v_out.push_back(entry) ;
	}

	return v_out ; 
}



/**********************************************************************************
  *	                                                                              *
  *	  DirContent::forward_vector
  *	                                                                              *
  **********************************************************************************/
vector <string> AwsS3Wrapper::DirContent::forward_vector (const vector <string>& v_in) 
{
	vector <string> v_out ;
	if (v_in.size() >= 2) { for (size_t	i=1; i<v_in.size(); i++) { v_out.push_back(v_in.at(i)) ; } }
	return v_out ;
}


/**********************************************************************************
  *	                                                                              *
  *	  C'tor 
  *	                                                                              *
  **********************************************************************************/
AwsS3Wrapper::AwsS3Wrapper(	const Enum_S3Provider& s3provider,
							const Aws::Auth::AWSCredentials& credentials,
							StopController const & stopController		
):	credentials_(credentials), \
	s3provider_(s3provider), \
	region_(get_default_region(s3provider)), \
	endpoint_(get_default_endpoint(s3provider, get_default_region(s3provider))), \
	endpoint_is_secure_(false)/*Not handled*/, \
	download_pool_size_(s3_defaultDownload_pool_size), download_buffer_size_(s3_defaultDownload_buffer_size), \
	upload_pool_size_(s3_defaultUpload_pool_size), upload_buffer_size_(s3_defaultUpload_buffer_size), \
	s3_client_p_(), \
	stopController_(stopController)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "C'tor called. S3 Provider: " + toStr(s3provider_) + "  default region: \"" + region_ \
		+ "\"  default endpoint: \"" + endpoint_ + "\"" ) ;

	// Next call ensure that the S3Configuration is called exacly once-per-process
	AwsS3WrapperConfigurationSingleton::instance() ;

	ACS_COND_THROW ( (S3P_UNKNOWN == s3provider), exIllegalValueException("S3 Provider Invalid or not set: " + toStr(s3provider_)) ) ; 
	ACS_COND_THROW ( endpoint_.empty(), exIllegalValueException("Endpoint not set: please set it as e.g. http://example.com:9000") ); 

	Aws::Client::ClientConfiguration client_configuration ;
	// Set timeouts:
	client_configuration.connectTimeoutMs = s3_defaultConnectTimeoutMs ; client_configuration.requestTimeoutMs = s3_defaultRequestTimeoutMs ;

	if ( (not region_.empty()) && (region_ != get_unset_region()) ) { 
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Setting default region: \"" << region_ << "\"" ) ;
		client_configuration.region = to_AwsString(region_) ; 
	}
	
	if (not endpoint_.empty()) { 
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Setting default endpoint: \"" << endpoint_ << "\"" ) ;
		client_configuration.endpointOverride = to_AwsString(endpoint_) ; 
	} ;

	// Do not move from here:
	bool useVirtualAddressing (true) ;
	apply_specific_configuration(s3provider, to_string(client_configuration.endpointOverride), client_configuration, useVirtualAddressing) ;

	try {
		const Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy signPayloads = Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy::Never ;
		s3_client_p_ = Aws::MakeShared<Aws::S3::S3Client>("S3Client", credentials, client_configuration, \
																				signPayloads, useVirtualAddressing) ;
	}
	catch( exception & e )
	{
		ACS_THROW( AwsS3WrapperConnectionException( e, "Cannot instance S3Client ptr") );
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "C'tor finished." ) ;
}


/**********************************************************************************
  *	                                                                              *
  *	  C'tor 
  *	                                                                              *
  **********************************************************************************/
AwsS3Wrapper::AwsS3Wrapper(	const Enum_S3Provider& s3provider,
							const Aws::Auth::AWSCredentials& credentials,
							const string& region,
							StopController const & stopController		

):	credentials_(credentials), \
	s3provider_(s3provider), \
	region_(region), \
	endpoint_(get_default_endpoint(s3provider, region)), \
	endpoint_is_secure_(false)/*Not handled*/, \
	download_pool_size_(s3_defaultDownload_pool_size), download_buffer_size_(s3_defaultDownload_buffer_size), \
	upload_pool_size_(s3_defaultUpload_pool_size), upload_buffer_size_(s3_defaultUpload_buffer_size), \
	s3_client_p_(), \
	stopController_(stopController)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "C'tor called. S3 Provider: " + toStr(s3provider_) + "  region: \"" + region \
		+ "\"  default endpoint: \"" + endpoint_ + "\"" ) ;

	// Next call ensure that the S3Configuration is called exacly once-per-process
	AwsS3WrapperConfigurationSingleton::instance() ;

	ACS_COND_THROW ( (S3P_UNKNOWN == s3provider), exIllegalValueException("S3 Provider Invalid or not set: " + toStr(s3provider_)) ) ; 
	ACS_COND_THROW ( endpoint_.empty(), exIllegalValueException("Endpoint not set: please set it as e.g. http://example.com:9000") ); 

	Aws::Client::ClientConfiguration client_configuration ;
	// Set timeouts:
	client_configuration.connectTimeoutMs = s3_defaultConnectTimeoutMs ; client_configuration.requestTimeoutMs = s3_defaultRequestTimeoutMs ;

	if ( (not region_.empty()) && (region_ != get_unset_region()) ) { 
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Setting region: \"" << region_ << "\"" ) ;
		client_configuration.region = to_AwsString(region_) ; 
	}
	
	if (not endpoint_.empty()) { 
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Setting default endpoint: \"" << endpoint_ << "\"" ) ;
		client_configuration.endpointOverride = to_AwsString(endpoint_) ; 
	} ;

	// Do not move from here:
	bool useVirtualAddressing (true) ;
	apply_specific_configuration(s3provider, to_string(client_configuration.endpointOverride), client_configuration, useVirtualAddressing) ;

	try {
		const Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy signPayloads = Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy::Never ;
		s3_client_p_ = Aws::MakeShared<Aws::S3::S3Client>("S3Client", credentials, client_configuration,	\
																				signPayloads, useVirtualAddressing) ;
	}
	catch( exception & e )
	{
		ACS_THROW( AwsS3WrapperConnectionException( e, "Cannot instance S3Client ptr") );
	}


	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "C'tor finished." ) ;
}


/**********************************************************************************
  *	                                                                              *
  *	  C'tor 
  *	                                                                              *
  **********************************************************************************/
AwsS3Wrapper::AwsS3Wrapper(	const Enum_S3Provider& s3provider,
							const Aws::Auth::AWSCredentials& credentials,
							const string& region,
							const string& endpoint,
							StopController const & stopController		
):	credentials_(credentials), \
	s3provider_(s3provider), \
	region_( region.empty() ? get_default_region(s3provider) : region ), \
	endpoint_(endpoint), \
	endpoint_is_secure_(false)/*Not handled*/, \
	download_pool_size_(s3_defaultDownload_pool_size), download_buffer_size_(s3_defaultDownload_buffer_size), \
	upload_pool_size_(s3_defaultUpload_pool_size), upload_buffer_size_(s3_defaultUpload_buffer_size), \
	s3_client_p_(), \
	stopController_(stopController)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "C'tor called. S3 Provider: " + toStr(s3provider_) + "  region: \"" + region_ \
		+ "\"  endpoint: \"" + endpoint_ + "\"" ) ;

	// Next call ensure that the S3Configuration is called exacly once-per-process
	AwsS3WrapperConfigurationSingleton::instance() ;

	ACS_COND_THROW ( (S3P_UNKNOWN == s3provider), exIllegalValueException("S3 Provider Invalid or not set: " + toStr(s3provider_)) ) ; 
	ACS_COND_THROW ( endpoint_.empty(), exIllegalValueException("Endpoint not set: please set it as e.g. http://example.com:9000") ) ; 

	Aws::Client::ClientConfiguration client_configuration ;
	// Set timeouts:
	client_configuration.connectTimeoutMs = s3_defaultConnectTimeoutMs ; client_configuration.requestTimeoutMs = s3_defaultRequestTimeoutMs ;

	if ( region_ != get_unset_region() ) { 
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Setting region: \"" << region_ << "\"" ) ;
		client_configuration.region = to_AwsString(region_) ; 
	}
	else {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Using default region: \"" << region_ << "\"" ) ;
	}
	
	if (not endpoint_.empty()) { 
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Setting endpoint: \"" << endpoint_ << "\"" ) ;
		client_configuration.endpointOverride = to_AwsString(endpoint_) ; 
	} ;

	// Do not move from here:
	bool useVirtualAddressing (true) ;
	apply_specific_configuration(s3provider, to_string(client_configuration.endpointOverride), client_configuration, useVirtualAddressing) ;

	try {
		const Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy signPayloads = Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy::Never ;
		s3_client_p_ = Aws::MakeShared<Aws::S3::S3Client>("S3Client", credentials, client_configuration, \
																				signPayloads, useVirtualAddressing) ;
	}
	catch( exception & e )
	{
		ACS_THROW( AwsS3WrapperConnectionException( e, "Cannot instance S3Client ptr") );
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "C'tor finished." ) ;
}


/**********************************************************************************
  *	                                                                              *
  *	  C'tor 
  *	                                                                              *
  **********************************************************************************/
AwsS3Wrapper::AwsS3Wrapper(	const AwsS3WrapperConfiguration &s3_conf, \
							StopController const & stopController		
):	credentials_(Aws::Auth::AWSCredentials(AwsS3Wrapper::to_AwsString(s3_conf.get_aws_key_ak()), \
			AwsS3Wrapper::to_AwsString(s3_conf.get_aws_key_sk()))), \
	s3provider_(s3_conf.get_aws_s3_provider()), \
	region_( s3_conf.get_aws_region().empty() ? get_default_region(s3provider_) : s3_conf.get_aws_region() ), \
	endpoint_( s3_conf.get_aws_endpoint().empty() ? get_default_endpoint(s3provider_, region_) : s3_conf.get_aws_endpoint()), \
	endpoint_is_secure_( s3_conf.get_endpoint_is_secure()), \
	download_pool_size_(s3_conf.get_download_pool_size()), download_buffer_size_(s3_conf.get_download_buffer_size()), \
	upload_pool_size_(s3_conf.get_upload_pool_size()), upload_buffer_size_(s3_conf.get_upload_buffer_size()), \
	s3_client_p_(), \
	stopController_(stopController)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "C'tor called. S3 Provider: " + toStr(s3provider_) + "  region: \"" + region_ \
		+ "\"  endpoint: \"" + endpoint_ + "\"" ) ;

	// Next call ensure that the S3Configuration is called exacly once-per-process
	AwsS3WrapperConfigurationSingleton::instance() ;

	ACS_COND_THROW ( (S3P_UNKNOWN == s3provider_), exIllegalValueException("S3 Provider Invalid or not set: " + toStr(s3provider_)) ) ; 
	ACS_COND_THROW ( endpoint_.empty(), exIllegalValueException("Endpoint not set: please set it as e.g. http://example.com:9000") ) ; 

	Aws::Client::ClientConfiguration client_configuration ;
	// Set timeouts:
	client_configuration.connectTimeoutMs = s3_defaultConnectTimeoutMs ; client_configuration.requestTimeoutMs = s3_defaultRequestTimeoutMs ;

	if ( (not region_.empty()) && (region_ != get_unset_region()) ) { 
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Setting region: \"" << region_ << "\"" ) ;
		client_configuration.region = to_AwsString(region_) ; 
	}
	else {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Using default region: \"" << region_ << "\"" ) ;
	}
	
	if (not endpoint_.empty()) { 
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Setting endpoint: \"" << endpoint_ << "\"" ) ;
		client_configuration.endpointOverride = to_AwsString(endpoint_) ; 
	} ;


	// Do not move from here:
	bool useVirtualAddressing (true) ;
	apply_specific_configuration(s3provider_, to_string(client_configuration.endpointOverride), client_configuration, useVirtualAddressing) ;

	try {
		const Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy signPayloads = Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy::Never ;
		s3_client_p_ = Aws::MakeShared<Aws::S3::S3Client>("S3Client", credentials_, client_configuration, \
																				signPayloads, useVirtualAddressing) ;
	}
	catch( exception & e )
	{
		ACS_THROW( AwsS3WrapperConnectionException( e, "Cannot instance S3Client ptr") );
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "C'tor finished." ) ;
}


/**********************************************************************************
  *	                                                                              *
  *	  D'tor 
  *	                                                                              *
  **********************************************************************************/
AwsS3Wrapper::~AwsS3Wrapper() throw()
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "D'tor called." ) ;
}


/**********************************************************************************
  *	                                                                              *
  *	  apply_specific_configuration
  *	                                                                              *
  **********************************************************************************/
void AwsS3Wrapper::apply_specific_configuration(const Enum_S3Provider& s3provider, const string& endpoint,
	Aws::Client::ClientConfiguration& client_configuration, bool& useVirtualAddressing) 
{
	/**
		Return a valid useVirtualAddressing and it might load some parameter in the client_configuration
		[ So far, this method is mainly used to handle MINIO ]
	*/

// NOTE: the endpoint is used to detect http/https. This might be useful for all the providers.
// It is unused for now.
			
	useVirtualAddressing = true ; // Default for AWS and AWS-like. So far, just MINIO does not enable this.
	
	switch(s3provider) 
	{
		case S3P_AWS:
		case S3P_WASABI:
		case S3P_ORANGE:
		case S3P_HUAWEI:
		case S3P_OVH:
		case S3P_OVH_PERF:
		{ 
			// It might be useful: -> scheme / verifySSL function of endpoint being "http"/"https"
			// For now it's always assumed: HTTPS (typical of commercial-grade S3-like providers)
			useVirtualAddressing = true ; 
			break ; 
		}
		case S3P_CYNNYSPACE:
		{
			// Reverse engineered:
			useVirtualAddressing = false ;	
			// It might be useful: -> scheme / verifySSL function of endpoint being "http"/"https"
			client_configuration.scheme=Aws::Http::Scheme::HTTP ; // Forced
			client_configuration.verifySSL=false ; // Forced
			break ;
		}
		case S3P_MINIO:
		{
			useVirtualAddressing = false ;	// MINIO _DOES NOT_ ENABLE THE VIRTUAL ADDRESSING.
			// It might be useful: -> scheme / verifySSL function of endpoint being "http"/"https"
			client_configuration.scheme=Aws::Http::Scheme::HTTP ; // Forced
			client_configuration.verifySSL=false ; // Forced
			break ;
		}
		default:
		{
			ACS_THROW ( exIllegalValueException("S3 Provider Invalid or not set: " + toStr(s3provider_)) ) ; 
		}
	} // END: switch
}	


/**********************************************************************************
  *	                                                                              *
  *	  dump
  *	                                                                              *
  **********************************************************************************/
string AwsS3Wrapper::dump(const vector<string>& v, const string& t) 
{
	ostringstream o ; 
	for (size_t i=0; i<v.size(); i++) { o << t << "\"" << v.at(i) << "\"" << endl ;	}
	return o.str() ; 
}


/**********************************************************************************
  *	                                                                              *
  *	  dump
  *	                                                                              *
  **********************************************************************************/
string AwsS3Wrapper::dump(const multimap <Enum_S3MetadataTag, string> tags_M, const string& t) 
{
	ostringstream o ; 

	for (multimap <Enum_S3MetadataTag, string>::const_iterator it=tags_M.begin(); it!=tags_M.end(); it++)
	{
		const string key (toStr(it->first)) ;
		const string value (it->second) ;
		o << t << "Key: " << key << " Value: \"" << value << "\"" ;
		if (S3TAG_CUSTOM_PAIR == it->first)
		{
			const string split_key (value.substr(0, value.find('='))) ;
			const string split_value (value.substr(1+value.find('='))) ;
			o << "[ custom tags: \"" << split_key << "\" -> \"" << split_value << "\"]" ;
		}
		o << endl ;
	}

	return o.str() ; 
}


/**********************************************************************************
  *	                                                                              *
  *	  relative_to_absolute_path
  *	                                                                              *
  **********************************************************************************/
void AwsS3Wrapper::relative_to_absolute_path(const string& bucket_in, const string& relative_path_in, string& absolute_path_out, bool force_prefix_s3)
{
	absolute_path_out = StringUtils::pathNormalize ( StringUtils::pathJoin( ("/" + strip_s3protocol(bucket_in)), \
		strip_s3protocol(relative_path_in) ) )  ;
	if( force_prefix_s3 ) {
		absolute_path_out = "s3://"+StringUtils::trimBegin( absolute_path_out, '/' );
	}
}

/**********************************************************************************
  *	                                                                              *
  *	  absolute_to_relative_path
  *	                                                                              *
  **********************************************************************************/
void AwsS3Wrapper::absolute_to_relative_path(const string& absolute_path_in, string& bucket_out, string& relative_path_out)
{
	const bool removeLeadingSlash(true) ;
	const bool removeLeadingDot(false) ;
	
	bucket_out = "" ; relative_path_out = "" ; // default output values
	
	// Disassemble the path as a vector -- this makes easier to extract the first part
	vector <string> abs_path_V ( StringUtils:: pathDisassemble (StringUtils::pathNormalize( \
									strip_s3protocol(absolute_path_in), removeLeadingSlash, removeLeadingDot)) ) ;
	
	if (abs_path_V.size() >= 1)
	{
		// Extract the first part of the path:
		bucket_out = abs_path_V.at(0) ;
		
		if (abs_path_V.size() >= 2)
		{
			// Drop the first entry of the path (being the bucket) and assemble the rest, as relative path
			abs_path_V = AwsS3Wrapper::DirContent::forward_vector(abs_path_V) ;	// Drop the first component
			relative_path_out =  StringUtils::pathNormalize( StringUtils::pathAssemble(abs_path_V), removeLeadingSlash, removeLeadingDot);
		}
	}
}


/**********************************************************************************
  *	                                                                              *
  *	  disassemble_full_path
  *	                                                                              *
  **********************************************************************************/
void AwsS3Wrapper::disassemble_full_path(const string& path, string& endpoint_out, string& bucket_out, string& relative_path_out) 

{
	endpoint_out = bucket_out = relative_path_out = "" ;
	const bool removeLeadingSlash(true) ;
	const bool removeLeadingDot(false) ;

	// This comparison should be the fastest possible, single characters and matched befor longer substrings
	if ( (path.length() >= 3) && (':' == path.at(2)) && ("s3" == StringUtils::lowercase(path.substr(0,2))) )
	{ 
		// Got S3 absolute path
		
		absolute_to_relative_path(path, bucket_out, relative_path_out) ;
		
	} else if (path.find(':') != string::npos)
	{
		// Got protocol -- assumed endpoint / bucket / path
	
		const string protocol ( path.substr(0, path.find(':')) ) ;
		vector <string> abs_path_V ( StringUtils:: pathDisassemble (StringUtils::pathNormalize( \
									path.substr(1+path.find(':')), removeLeadingSlash, removeLeadingDot)) ) ;

		if (abs_path_V.size() >= 1) { endpoint_out = protocol + "://" + abs_path_V.at(0) ; }
		if (abs_path_V.size() >= 2) { bucket_out = abs_path_V.at(1) ; }
		if (abs_path_V.size() >= 3) 
		{ 
			for (size_t i=2; i<abs_path_V.size(); i++)
			{  
				relative_path_out = StringUtils::pathJoin(relative_path_out, abs_path_V.at(i)) ;
			}
		}

	} else {
	
		// Protocol not found -- assumed a standard absolute path without endpoint
		absolute_to_relative_path(path, bucket_out, relative_path_out) ;
	}
}


/**********************************************************************************
  *	                                                                              *
  *	  forge_range
  *	                                                                              *
  **********************************************************************************/
string AwsS3Wrapper::forge_range(const off64_t& position, const off64_t &length)
{
	ostringstream o ;
	if (length > 0) { o << "bytes=" << position << "-" << (position+length-1) ; }
	else { o << "bytes=" << position << "-" << position ; /* 1 byte is nevertheless transferred, a check should be inserted elsewhere */ }
	return o.str() ;
}


/**********************************************************************************
  *	                                                                              *
  *	  get_default_region
  *	                                                                              *
  **********************************************************************************/
string AwsS3Wrapper::get_default_region(const Enum_S3Provider& s3_provider) 
{
	/** These regions are assumed as default when no explicit region name is provided */

	string r ; switch (s3_provider)
	{
		case S3P_AWS:		{ r = "us-east-1" ;		break ; }
		case S3P_WASABI:	{ r = "us-east-1" ;		break ; }
		case S3P_ORANGE:	{ r = "eu-west-0" ;		break ; }	
		case S3P_HUAWEI:	{ r = "cn-north-1" ;	break ; } 
		case S3P_CYNNYSPACE:{ r = "" ;				break ; } 
		case S3P_OVH:		{ r = "" ;				break ; } 
		case S3P_OVH_PERF:	{ r = "" ;				break ; } 
		case S3P_MINIO:		{ r = "" ;				break ; } 
		default:			{ ACS_THROW ( exIllegalValueException("Usupported provider: \"" + toStr(s3_provider) + "\"") ) ; break ; }

	}	// END: switch (s3_provider)

	return r ;
}


/**********************************************************************************
  *	                                                                              *
  *	  get_unset_region
  *	                                                                              *
  **********************************************************************************/
string AwsS3Wrapper::get_unset_region(const Enum_S3Provider& s3_provider) 
{
	/** It turned out that, in Amazon AWS, setting a region which is the default raises an error. 
		Therefore the default region SHOULD NOT BE SET EVEN UNDER EXPLICIT REQUEST OF THE USER
		Other providers, on the contrary, might want to receive an explicit set of that region, even if is the default one.
		The concect of "unset_region" introduced here is the folloing:
		If a region matches the unsetregion, that SHOULD NOT BE SET by setting methods. 
		This is mandatory in Amazon AWS but maybe not true for some provider. For this provider set the string "VOID":
		it will never match. So any region, including the default one, will be set explicitly
	*/

	string r ; switch (s3_provider)
	{
		case S3P_AWS:		{ r = "us-east-1" ;		break ; }	
		case S3P_WASABI:	{ r = "us-east-1" ;		break ; }
		case S3P_ORANGE:	{ r = "eu-west-0" ;		break ; }	
		case S3P_HUAWEI:	{ r = "cn-north-1" ;	break ; }	
		case S3P_CYNNYSPACE:{ r = "" ;				break ; }	
		case S3P_OVH:		{ r = "" ;				break ; }	
		case S3P_OVH_PERF:	{ r = "" ;				break ; }	
		case S3P_MINIO:		{ r = "" ;				break ; }	
		default:			{ ACS_THROW ( exIllegalValueException("Usupported provider: \"" + toStr(s3_provider) + "\"") ) ; break ; }

	}	// END: switch (s3_provider)

	return r ;
}


/**********************************************************************************
  *	                                                                              *
  *	  get_default_endpoint
  *	                                                                              *
  **********************************************************************************/
string AwsS3Wrapper::get_default_endpoint(const Enum_S3Provider& s3_provider, const string& region) 
{
	string endpoint ; switch (s3_provider)
	{
		case S3P_AWS:	
		{ 
			endpoint = "" ; // Not needed ?
			break ; 
		}	
		 
		case S3P_WASABI:	
		{ 
			if      (region == "us-east-1")			{ endpoint = "https://s3.wasabisys.com" ; }					// Wasabi US East 1 (N. Virginia)
			else if (region == "us-east-2")			{ endpoint = "https://s3.us-east-2.wasabisys.com" ; }		// Wasabi US East 2 (N. Virginia)
			else if (region == "us-west-1")			{ endpoint = "https://s3.us-west-1.wasabisys.com" ; }		// Wasabi US West 1 (Oregon)
			else if (region == "eu-central-1")		{ endpoint = "https://s3.eu-central-1.wasabisys.com" ; }	// Wasabi EU Central 1 (Amsterdam)
			else									{ endpoint = "https://s3." + region + ".wasabisys.com" ; } 
			break ; 
		}
		case S3P_ORANGE:	
		{ 
			if (region == "eu-west-0")				{ endpoint = "https://oss.eu-west-0.prod-cloud-ocb.orange-business.com" ; }
			else									{ endpoint = "https://oss." + region + ".prod-cloud-ocb.orange-business.com" ; }
			break ; 
		}	
		case S3P_HUAWEI:	
		{
			if      (region == "af-south-1")		{ endpoint = "https://obs.af-south-1.myhuaweicloud.com" ; } 	// AF-Johannesburg
			else if (region == "ap-southeast-2")	{ endpoint = "https://obs.ap-southeast-2.myhuaweicloud.com" ; } // AP-Bangkok
			else if (region == "ap-southeast-1")	{ endpoint = "https://obs.ap-southeast-1.myhwclouds.com" ; }	// AP-Hong Kong 		 
			else if (region == "ap-southeast-3")	{ endpoint = "https://obs.ap-southeast-3.myhuaweicloud.com" ; }	// AP-Singapore 		 
			else if (region == "cn-east-3")			{ endpoint = "https://obs.cn-east-3.myhuaweicloud.com" ; }		// CN East-Shanghai1	 
			else if (region == "cn-east-2")			{ endpoint = "https://obs.cn-east-2.myhwclouds.com" ; }			// CN East-Shanghai2	 
			else if (region == "cn-north-1")		{ endpoint = "https://obs.cn-north-1.myhwclouds.com" ; } 		// CN North-Beijing1	 
			else if (region == "cn-north-4")		{ endpoint = "https://obs.cn-north-4.myhuaweicloud.com" ; }		// CN North-Beijing4	 
			else if (region == "cn-south-1")		{ endpoint = "https://obs.cn-south-1.myhwclouds.com" ; }		// CN South-Guangzhou	 
			else if (region == "cn-southwest-2")	{ endpoint = "https://obs.cn-southwest-2.myhuaweicloud.com" ; }	// CN Southwest-Guiyang1 
			else									{ endpoint = "https://obs." + region + ".myhuaweicloud.com" ; } 
			break ;
		}
		case S3P_CYNNYSPACE:	
		{ 
			endpoint = "http://cs3.cynnyspace.com" ; 
			break ; 
		}	
		case S3P_OVH:	
		{ 
			endpoint = "https://s3." + StringUtils::lowercase(region) + ".cloud.ovh.net" ; 
			break ; 
		}	
		case S3P_OVH_PERF:	
		{ 
			endpoint = "https://s3." + StringUtils::lowercase(region) + ".perf.cloud.ovh.net" ; 
			break ; 
		}	
		case S3P_MINIO:	
		{ 
			endpoint = "" ; 	//  e.g. http://example.com:9000
			break ; 
		}	
		default:									{ ACS_THROW ( exIllegalValueException("Usupported provider: \"" + toStr(s3_provider) + "\"") ) ; break ; }

	}	// END: switch (s3_provider)

	return endpoint ;
}


/**********************************************************************************
  *	                                                                              *
  *	  list_regions
  *	                                                                              *
  **********************************************************************************/
vector <string> AwsS3Wrapper::get_list_of_regions(const Enum_S3Provider& s3_provider) 
{
	vector <string> v ; switch (s3_provider)
	{
		case S3P_AWS:	
		{ 
            v.push_back( AwsS3Wrapper::stringFromRegion(Aws::S3::Model::BucketLocationConstraint::us_east_1) )  ;
			v.push_back( AwsS3Wrapper::stringFromRegion(Aws::S3::Model::BucketLocationConstraint::us_west_1) )  ;
			v.push_back( AwsS3Wrapper::stringFromRegion(Aws::S3::Model::BucketLocationConstraint::us_west_2) )  ;
			v.push_back( AwsS3Wrapper::stringFromRegion(Aws::S3::Model::BucketLocationConstraint::ap_northeast_1) )  ;
			v.push_back( AwsS3Wrapper::stringFromRegion(Aws::S3::Model::BucketLocationConstraint::ap_northeast_2) )  ;
			v.push_back( AwsS3Wrapper::stringFromRegion(Aws::S3::Model::BucketLocationConstraint::ap_south_1) )  ;
			v.push_back( AwsS3Wrapper::stringFromRegion(Aws::S3::Model::BucketLocationConstraint::ap_southeast_1) )  ;
			v.push_back( AwsS3Wrapper::stringFromRegion(Aws::S3::Model::BucketLocationConstraint::ap_southeast_2) )  ;
			v.push_back( AwsS3Wrapper::stringFromRegion(Aws::S3::Model::BucketLocationConstraint::sa_east_1) )  ;
			v.push_back( AwsS3Wrapper::stringFromRegion(Aws::S3::Model::BucketLocationConstraint::cn_north_1) )  ;
			v.push_back( AwsS3Wrapper::stringFromRegion(Aws::S3::Model::BucketLocationConstraint::eu_central_1) )  ;
			v.push_back( AwsS3Wrapper::stringFromRegion(Aws::S3::Model::BucketLocationConstraint::eu_west_1) )  ;
			break ; 
		}	
		 
		case S3P_WASABI:	
		{ 
            v.push_back( AwsS3Wrapper::stringFromRegion(Aws::S3::Model::BucketLocationConstraint::us_east_1) )  ;
			v.push_back( AwsS3Wrapper::stringFromRegion(Aws::S3::Model::BucketLocationConstraint::us_west_1) )  ;
			v.push_back( AwsS3Wrapper::stringFromRegion(Aws::S3::Model::BucketLocationConstraint::us_west_2) )  ;
			v.push_back( AwsS3Wrapper::stringFromRegion(Aws::S3::Model::BucketLocationConstraint::eu_central_1) )  ;
			break ; 
		}
		case S3P_ORANGE:	
		{ 
			v.push_back("eu-west-0")	;
			v.push_back("eu-west-1")	;
			break ; 
		}	
		case S3P_HUAWEI:	
		{
			v.push_back("af-south-1") ; v.push_back("ap-southeast-2") ; v.push_back("ap-southeast-1") ; v.push_back("ap-southeast-3")	; 
			v.push_back("cn-east-3") ; v.push_back("cn-east-2") ; 
			v.push_back("cn-north-1") ;  v.push_back("cn-north-4") ; v.push_back("cn-south-1") ; v.push_back("cn-southwest-2"); 
			break ;
		}
		case S3P_CYNNYSPACE:	
		{ 
			v.push_back("")	;
			break ; 
		}	
		case S3P_OVH:	
		{ 
			v.push_back("SERCO-DIAS1") ;
			break ; 
		}	
		case S3P_OVH_PERF:	
		{ 
			v.push_back("sbg") ;
			break ; 
		}	
		case S3P_MINIO:	
		{ 
			v.push_back("")	;
			break ; 
		}	
		default: { ACS_THROW ( exIllegalValueException("Usupported provider: \"" + toStr(s3_provider) + "\"") ) ; break ; }

	}	// END: switch (s3_provider)

	return v ;
}


/**********************************************************************************
  *	                                                                              *
  *	  regionFromString
  *	                                                                              *
  **********************************************************************************/
Aws::S3::Model::BucketLocationConstraint AwsS3Wrapper::regionFromString(const Enum_S3Provider &s3_provider, const string& region_string, bool emit_exception)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Method called. Region (as string): \"" + region_string + "\"" ) ;
	
	Aws::S3::Model::BucketLocationConstraint bc ; 
	if (region_string.empty())	{ bc = Aws::S3::Model::BucketLocationConstraint::NOT_SET ; }
	else
	{
		switch (s3_provider)
		{
			case S3P_ORANGE: 
			{
				bc = Aws::S3::Model::BucketLocationConstraintMapper::GetBucketLocationConstraintForName( 
					to_AwsString( "<LocationConstraint>" + region_string + "</LocationConstraint>") ) ; 

			
#if(0)		
				 if	("eu-west-0" == region_string)  				 { out = 0x4cf03dda ; }
				else if ("eu-west-1" == region_string)  				 { out = 0xffffffffd375f879 ;}
				else { 
					ACS_COND_THROW ( (true == emit_exception), exIllegalValueException("Invalid region: \"" + region_string + "\"") ) ; 
					out = static_cast<uint64_t>(Aws::S3::Model::BucketLocationConstraint::NOT_SET);	// Fallback (no exception)	
				}
#endif				
				break ;
			}
			default:
			{
				if (region_string.empty()) { bc = Aws::S3::Model::BucketLocationConstraint::NOT_SET; }
				else
				{ bc = Aws::S3::Model::BucketLocationConstraintMapper::GetBucketLocationConstraintForName( to_AwsString(region_string) ) ; }
	// FIXME HANDLE EXCEPTION
	#if(0)
				else if	(0 == strcmp(region_string.c_str(), Aws::Region::US_EAST_1))		{ bc = Aws::S3::Model::BucketLocationConstraint::us_east_1; }
				else if (0 == strcmp(region_string.c_str(), Aws::Region::US_WEST_1)) 		{ bc = Aws::S3::Model::BucketLocationConstraint::us_west_1; }
				else if (0 == strcmp(region_string.c_str(), Aws::Region::US_WEST_2))		{ bc = Aws::S3::Model::BucketLocationConstraint::us_west_2; }
				else if (0 == strcmp(region_string.c_str(), Aws::Region::AP_NORTHEAST_1))	{ bc = Aws::S3::Model::BucketLocationConstraint::ap_northeast_1; }
				else if (0 == strcmp(region_string.c_str(), Aws::Region::AP_NORTHEAST_2))	{ bc = Aws::S3::Model::BucketLocationConstraint::ap_northeast_2; }
				else if (0 == strcmp(region_string.c_str(), Aws::Region::AP_SOUTH_1))		{ bc = Aws::S3::Model::BucketLocationConstraint::ap_south_1; }
				else if (0 == strcmp(region_string.c_str(), Aws::Region::AP_SOUTHEAST_1))	{ bc = Aws::S3::Model::BucketLocationConstraint::ap_southeast_1; }
				else if (0 == strcmp(region_string.c_str(), Aws::Region::AP_SOUTHEAST_2))	{ bc = Aws::S3::Model::BucketLocationConstraint::ap_southeast_2; }
				else if (0 == strcmp(region_string.c_str(), Aws::Region::SA_EAST_1))		{ bc = Aws::S3::Model::BucketLocationConstraint::sa_east_1; }
				else if (0 == strcmp(region_string.c_str(), Aws::Region::CN_NORTH_1))		{ bc = Aws::S3::Model::BucketLocationConstraint::cn_north_1; }
				else if (0 == strcmp(region_string.c_str(), Aws::Region::EU_CENTRAL_1))		{ bc = Aws::S3::Model::BucketLocationConstraint::eu_central_1; }
				else if (0 == strcmp(region_string.c_str(), Aws::Region::EU_WEST_1))		{ bc = Aws::S3::Model::BucketLocationConstraint::eu_west_1; }
				else { 
					ACS_COND_THROW ( (true == emit_exception), exIllegalValueException("Invalid region: \"" + region_string + "\"") ) ; 
					bc = Aws::S3::Model::BucketLocationConstraint::NOT_SET ;	// Fallback (no exception)	
				}
	#endif
				break ;
			}
		}	// END: switch (s3_provider)
	}	// END: 

	return bc ; 
}


/**********************************************************************************
  *	                                                                              *
  *	  stringFromRegion
  *	                                                                              *
  **********************************************************************************/
string AwsS3Wrapper::stringFromRegion(const Aws::S3::Model::BucketLocationConstraint& bc, bool emit_exception)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Method called." ) ;

	string s ("") ; 
	if (bc != Aws::S3::Model::BucketLocationConstraint::NOT_SET)
	{
		s = to_string( Aws::S3::Model::BucketLocationConstraintMapper::GetNameForBucketLocationConstraint(bc) ) ; 
		if ( (s.length() >= 41)	&& (s.substr(0, 20) == "<LocationConstraint>") )
		{
			// Handle, for example: "<LocationConstraint>eu-west-0</LocationConstraint>" used by ORANGE
			s = s.substr(20, (s.length()-41)) ;
		}
	}

	// FIXME HANDLE EXCEPTION
			
#if(0)			
			else if (bc == Aws::S3::Model::BucketLocationConstraint::us_east_1)			{ s = Aws::Region::US_EAST_1 ; } 		
			else if (bc == Aws::S3::Model::BucketLocationConstraint::us_west_1)			{ s = Aws::Region::US_WEST_1 ; } 		
			else if (bc == Aws::S3::Model::BucketLocationConstraint::us_west_2)			{ s = Aws::Region::US_WEST_2 ; } 		
			else if (bc == Aws::S3::Model::BucketLocationConstraint::ap_northeast_1)	{ s = Aws::Region::AP_NORTHEAST_1 ; }   
			else if (bc == Aws::S3::Model::BucketLocationConstraint::ap_northeast_2)	{ s = Aws::Region::AP_NORTHEAST_2 ; }   
			else if (bc == Aws::S3::Model::BucketLocationConstraint::ap_south_1)		{ s = Aws::Region::AP_SOUTH_1 ; }	
			else if (bc == Aws::S3::Model::BucketLocationConstraint::ap_southeast_1)	{ s = Aws::Region::AP_SOUTHEAST_1 ; }   
			else if (bc == Aws::S3::Model::BucketLocationConstraint::ap_southeast_2)	{ s = Aws::Region::AP_SOUTHEAST_2 ; }   
			else if (bc == Aws::S3::Model::BucketLocationConstraint::sa_east_1)			{ s = Aws::Region::SA_EAST_1 ; } 		
			else if (bc == Aws::S3::Model::BucketLocationConstraint::cn_north_1)		{ s = Aws::Region::CN_NORTH_1 ; }	
			else if (bc == Aws::S3::Model::BucketLocationConstraint::eu_central_1)		{ s = Aws::Region::EU_CENTRAL_1 ; }  
			else if (bc == Aws::S3::Model::BucketLocationConstraint::eu_west_1)			{ s = Aws::Region::EU_WEST_1 ; } 		
			else { 
				ACS_COND_THROW( (true == emit_exception), exIllegalValueException("Invalid region") ) ; 
				s = "" ;	// Fallback (no exception)
			}
#endif

	return s ; 
}


/**********************************************************************************
  *	                                                                              *
  *	  createBucket
  *	                                                                              *
  **********************************************************************************/
void AwsS3Wrapper::createBucket(const string& region, const string& bucket_name_in) 
{
	/**
		Create a new bucket.
	*/

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. Region: \"" << region << "\"  Bucket name: \"" + bucket_name_in + "\"" ) ;

	ACS_COND_THROW( (true == isBucket(bucket_name_in)), AwsS3WrapperException("Bucket already existing: \"" + bucket_name_in + "\"" ) ) ; 

	const Aws::String bucket_name( to_AwsString(bucket_name_in) );

	Aws::S3::Model::CreateBucketConfiguration bucket_configuration;
	if ( (not region.empty()) && (region != get_unset_region()) ) {

		const Aws::S3::Model::BucketLocationConstraint bc ( static_cast<Aws::S3::Model::BucketLocationConstraint>( regionFromString(s3provider_, region)) ) ;
		bucket_configuration.WithLocationConstraint(bc); 
	}

	Aws::S3::Model::CreateBucketRequest bucket_request;
	bucket_request.WithBucket(bucket_name).WithCreateBucketConfiguration(bucket_configuration);

	const Aws::S3::Model::CreateBucketOutcome outcome ( s3_client_p_->CreateBucket(bucket_request) ) ;

	if (false == outcome.IsSuccess())
	{
		ostringstream o ; 
		o << "Error while creating bucket: \"" << bucket_name << "\".  Got exception: " << outcome.GetError().GetExceptionName() 
			<< "  Message: " << outcome.GetError().GetMessage() ;
		
		ACS_THROW( AwsS3WrapperException(o.str()) ) ; 
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Created bucket: \"" + bucket_name_in + "\"" ) ;

	return ; 
}


/**********************************************************************************
  *	                                                                              *
  *	  removeSingleObject
  *	                                                                              *
  **********************************************************************************/
void AwsS3Wrapper::removeSingleObject(const string& bucket_name_in, const string& object_in) 
{
	/**
		Remove a single object in a bucket. For example "file22" or "dir1/dir2" or "dir1/dir2/file33"
	*/

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. Object: \"" << object_in << "\"  Bucket name: \"" + bucket_name_in + "\"" ) ;

	isBucket(bucket_name_in, true/*always throw*/) ; // Bucket must exist !

	const bool removeLeadingSlash(true) ; const bool removeLeadingDot(false) ; 
	const string object_in_normalized (StringUtils::pathNormalize(object_in, removeLeadingSlash, removeLeadingDot)) ; 
	
	// Object must exist:
	isObject(bucket_name_in, object_in_normalized, true/*always throw*/) ;
	
	// Build the object name, taking into account if it's a file or a directory
	const Aws::String object( to_AwsString( object_in_normalized + (isDirectoryObject(bucket_name_in, object_in_normalized)? "/":"")) ) ;
	const Aws::String bucket_name( to_AwsString(bucket_name_in) );

	Aws::S3::Model::DeleteObjectRequest object_delete_request;
	object_delete_request.WithBucket(bucket_name).WithKey(object);

	const Aws::S3::Model::DeleteObjectOutcome outcome ( s3_client_p_->DeleteObject(object_delete_request) ) ;

	if (false == outcome.IsSuccess())
	{
		ostringstream o ; 
		o << "Error while removing object: \"" << object << "\" from bucket: \"" << bucket_name << "\".  Got exception: " << outcome.GetError().GetExceptionName() 
			<< "  Message: " << outcome.GetError().GetMessage() ;
		
		ACS_THROW( AwsS3WrapperException(o.str()) ) ; 
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Remove object: \"" + object_in + "\" from bucket: \"" + bucket_name_in + "\"" ) ;

	return ; 
}


/**********************************************************************************
  *	                                                                              *
  *	  multiRemoveObjects
  *	                                                                              *
  **********************************************************************************/
bool AwsS3Wrapper::multiRemoveObjects(const string& bucket_name, const string& remote_rel_path) 
{
	/**
		Recursively remove object from a path. E.G. "dir1/dir2" would remove: "dir1/dir2/dir3/file33" up to "dir1/dir2"
	*/

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. Remote relative path: \"" << remote_rel_path << "\"  Bucket name: \"" + bucket_name + "\"" ) ;

	isBucket(bucket_name, true/*always throw*/) ; // Bucket must exist !

	if ( false == isObject(bucket_name, remote_rel_path) )
	{
		ostringstream o ; o << "Object not to remove: " << remote_rel_path << "\" not found in bucket: \"" << bucket_name << "\"" ;
		ACS_LOG_WARNING (o.str()) ;
		
		return false ;
	}

	bool success (false) ; 
 	const bool removeLeadingSlash(true) ; const bool removeLeadingDot(true) ;
 	DirContent dc ;
	listContent(bucket_name, remote_rel_path, dc) ;
	
	// The root-path of the list of items to be removed, will be remote_rel_path itself, minus the latest level, since it's returned by getEntries
	// So, if the remote_rel_path is "dir1/dir2/dir3", the result of getEntries will begin with dir3.
	// For example, "dir3/dir4" or "dir3/file1"
	// In order to combine "dir1/dir2/dir3" and "dir3/file1" we need to remove the last level to the former (so: "dir1/dir2/")
	// and we will obtain: "dir1/dir2/dir3/file1"
	
	const string root_path (StringUtils::pathNormalize(File::getDirPath(remote_rel_path), removeLeadingSlash, removeLeadingDot));	// Used to remove the last level

	vector < pair <string, bool> > entries_V ( dc.getEntries(root_path) ) ;

	for (vector < pair <string, bool> >::const_reverse_iterator it = entries_V.rbegin() ; it != entries_V.rend() ; it ++)
	{
		const bool is_file (it->second)  ; 
		const string object ( it->first + ((true == is_file)? "" : "/") ) ; 	// Directories finish with "/"
		if ( (object != "/") && (true == isObject(bucket_name, object)) ) 
		{
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "About to remove " << (is_file? "file":"dir") << "-object: \"" << object << "\" from bucket: \"" << bucket_name << "\".") ;
			try
			{	
				removeSingleObject(bucket_name, object) ;		
				success = true ; // At least one			
			}
			catch (AwsS3WrapperException& ex)
			{
				ostringstream o ; o << "Could not to remove " << (is_file? "file":"dir") << "-object: \"" << object << "\" from bucket: \"" << bucket_name << "\":" ;
				ACS_LOG_WARNING (o.str() << ex) ;
			}
		}
		else if (true == is_file)
		{
			// The intermediate directory objects (like "dir1/dir2" if file "dir1/dir2/file3") MIGHT NOT EXIST. So no warning will be issued.
			ACS_LOG_INFO ("Unable to remove " << "file-object: \"" << object << "\" from bucket: \"" << bucket_name << "\". Object was not found.") ;
		}
	}
	

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Recursively pruned path: \"" + remote_rel_path + "\" from bucket: \"" + bucket_name + "\"" ) ;

	return success ; 
}


/**********************************************************************************
  *	                                                                              *
  *	  isFileObject
  *	                                                                              *
  **********************************************************************************/
bool AwsS3Wrapper::isFileObject(const string& bucket_name_in, const string& object_in, const bool emit_exception) const
{
	/**
		Check for the presence of a file (not terminating with "/") inside a Bucket
		Note that IF EXIT_EXCEPTION IS FALSE the method will return success/not success (bool T/F)
		IF EXIT_EXCEPTION IS TRUE the method will always return true, or an exception in case of insuccess (never return false)
	*/

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. Object: \"" << object_in << "\"  Bucket name: \"" + bucket_name_in + "\"" ) ;

	const bool removeLeadingSlash(true) ; const bool removeLeadingDot(false) ; 
	
	// Forge a file-name for the object. E.G. "mydir/myobject" (not terminating with "/", myobject must be a file)
	const string file_object ( StringUtils::pathNormalize(object_in, removeLeadingSlash, removeLeadingDot) ) ;	
	
	return internalIsObject(bucket_name_in, file_object, emit_exception) ; 
}

/**********************************************************************************
  *	                                                                              *
  *	  isDirectoryObject
  *	                                                                              *
  **********************************************************************************/
bool AwsS3Wrapper::isDirectoryObject(const string& bucket_name_in, const string& object_in, const bool emit_exception) const
{
	/**
		Check for the presence of an directory (terminating with "/") inside a Bucket
		Note that IF EXIT_EXCEPTION IS FALSE the method will return success/not success (bool T/F)
		IF EXIT_EXCEPTION IS TRUE the method will always return true, or an exception in case of insuccess (never return false)
	*/

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. Object: \"" << object_in << "\"  Bucket name: \"" + bucket_name_in + "\"" ) ;

	const bool removeLeadingSlash(true) ; const bool removeLeadingDot(false) ; 
	
	// Forge a directory-name for the object. E.G. "mydir/myobject/" (terminating with "/", myobject must be a directory)
	const string directory_object ( (StringUtils::pathNormalize(object_in, removeLeadingSlash, removeLeadingDot)) + "/") ;	
		
	return internalIsObject(bucket_name_in, directory_object, emit_exception) ; 
}

/**********************************************************************************
  *	                                                                              *
  *	  isObject
  *	                                                                              *
  **********************************************************************************/
bool AwsS3Wrapper::isObject(const string& bucket_name_in, const string& object_in, const bool emit_exception) const
{
	/**
		Check if the object is either a file or a directory, that is IF IT EXISTS. (return TRUE if one applies). False if not existing.
		Note that IF EXIT_EXCEPTION IS FALSE the method will return success/not success (bool T/F)
		IF EXIT_EXCEPTION IS TRUE the method will always return true, or an exception in case of insuccess (never return false)
	*/

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. Object: \"" << object_in << "\"  Bucket name: \"" + bucket_name_in + "\"" ) ;

	// During the check, emit no exception -- since at lease one of the two test will fail (a object cannot be both a file and a directory !)
	const bool success ( (isFileObject(bucket_name_in, object_in, false /* no exception */)) \
						 || (isDirectoryObject(bucket_name_in, object_in, false /* no exception */)) ) ;
		
	ACS_COND_THROW ( ((false == success) && (true == emit_exception)),				
						AwsS3WrapperNoSuchObjectException("Object not found: \"" + object_in + "\"  on bucket: \"" + bucket_name_in + "\"") ) ;	
		
	return success ; 
}

/**********************************************************************************
  *	                                                                              *
  *	  internalIsObject
  *	                                                                              *
  **********************************************************************************/
bool																									// OUT: True if object exists			 
		AwsS3Wrapper::internalIsObject(	const string& bucket_name_in, const string& object_in, 
												const bool emit_exception) const						// Throw exception (instead of return false) if object not found 
{
	/**
		Check for the presence of an object inside a Bucket
		Note: the object could be a file (conventionally, not terminating with "/") 
		or a directory (conventionally, terminating with "/").
		THIS IS A PRIVATE METHO. DO NOT PERFORM ANY PATH NORMALIZATION HERE.
		YOU DON'T WANT TO STRIP AWAY THE DIRECTORY MARKER ("/" at the end). 
		SEE: isFileObject, isDirectoryObject 
		
		Note that IF EXIT_EXCEPTION IS FALSE the method will return success/not success (bool T/F)
		IF EXIT_EXCEPTION IS TRUE the method will always return true, or an exception in case of insuccess (never return false)
	*/

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. Object: \"" << object_in << "\"  Bucket name: \"" + bucket_name_in + "\"" ) ;

	isBucket(bucket_name_in, true/*always throw*/) ; // Since the validation is on the object, the bucket MUST EXIST. O/W always exception !

	if (true == object_in.empty())
	{
		ACS_COND_THROW ( emit_exception, AwsS3WrapperNoSuchObjectException("Invalid object name: \"" + object_in + "\"  for bucket: \"" + bucket_name_in + "\"") ) ;
		return false ;
	}

	const Aws::String object( to_AwsString(object_in) );
	const Aws::String bucket_name( to_AwsString(bucket_name_in) );

	Aws::S3::Model::HeadObjectRequest object_head_request;
	object_head_request.WithBucket(bucket_name).WithKey(object);

	const Aws::S3::Model::HeadObjectOutcome outcome ( s3_client_p_->HeadObject(object_head_request) ) ;

	const bool success (outcome.IsSuccess()) ;

	ACS_COND_THROW ( (emit_exception && !success), AwsS3WrapperNoSuchObjectException("Object not found: \"" + object_in + "\"  on bucket: \"" + bucket_name_in + "\"") ) ;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Returning: " << (success?"T":"F") ) ;

	return success ; 
}


/**********************************************************************************
  *	                                                                              *
  *	  getHeadObjectResult
  *	                                                                              *
  **********************************************************************************/
Aws::S3::Model::HeadObjectResult																												 
		AwsS3Wrapper::getHeadObjectResult(	const string& bucket_name_in, const string& object_in) const		
{
	/**
		Return the metadata object of a file or a directory (the detection is automatic)
		Both the bucket and the object (either file or directory) MUST ALREADY EXISTS
	*/

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. Object: \"" << object_in << "\"  Bucket name: \"" + bucket_name_in + "\"" ) ;

	isBucket(bucket_name_in, true/*always throw*/) ; // Since the validation is on the object, the bucket MUST EXIST. O/W always exception !

	// Normalize the name of the input object
	const bool removeLeadingSlash(true) ; const bool removeLeadingDot(false) ; 
	string object_s (StringUtils::pathNormalize(object_in, removeLeadingSlash, removeLeadingDot)) ;

	isObject(bucket_name_in, object_s, true/*always throw*/) ; // The object MUST EXIST (either as directory or file). O/W always exception !

	if (true == isDirectoryObject(bucket_name_in, object_s)) { object_s += "/" ; } // Object name is turned to the one of a directory

	const Aws::String object( to_AwsString(object_s) );
	const Aws::String bucket_name( to_AwsString(bucket_name_in) );

	Aws::S3::Model::HeadObjectRequest object_head_request;
	object_head_request.WithBucket(bucket_name).WithKey(object);

	const Aws::S3::Model::HeadObjectOutcome outcome ( s3_client_p_->HeadObject(object_head_request) ) ;

	if (false == outcome.IsSuccess())
	{
		ostringstream o ; 
		o << "Error while getting object metadata: \"" << object << "\"  from bucket: \"" << bucket_name << "\".  Got exception: " << outcome.GetError().GetExceptionName() 
			<< "  Message: " << outcome.GetError().GetMessage() ;
		
		ACS_THROW( AwsS3WrapperException(o.str()) ) ; 
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished.") ;

	return outcome.GetResult() ; 
}


/**********************************************************************************
  *	                                                                              *
  *	  getObjectMetadata
  *	                                                                              *
  **********************************************************************************/
map<string, string> AwsS3Wrapper::getObjectMetadata(const string& bucket, const string& object) const 
{ 
	const Aws::Map< Aws::String, Aws::String > aws_map (getHeadObjectResult(bucket, object).GetMetadata()) ;
	map <string, string> m ;
	for (Aws::Map< Aws::String, Aws::String >::const_iterator it=aws_map.begin(); it != aws_map.end() ; it++)
	{ m.insert( pair<string, string> (to_string(it->first), to_string(it->second)) ) ; }
	return m ;
}


/**********************************************************************************
  *	                                                                              *
  *	  getObjectMetadataTags
  *	                                                                              *
  **********************************************************************************/
multimap <AwsS3Wrapper::Enum_S3MetadataTag, string> AwsS3Wrapper::getObjectMetadataTags(const string& bucket, const string& object) const 
{ 
	multimap <AwsS3Wrapper::Enum_S3MetadataTag, string> tags_M ;
	const map<string, string> mtd_entries_M(getObjectMetadata(bucket, object)) ;
	
	if (mtd_entries_M.end() != mtd_entries_M.find("x-amz-tagging") )
	{
		vector <string> entries_V ;
		StringUtils::Tokenize( mtd_entries_M.at("x-amz-tagging"), entries_V, "&" ) ;
		
		for (size_t i=0; i<entries_V.size(); i++)
		{
			const string value( entries_V.at(i) ) ;
			if ( (string::npos != value.find('=')) && (value.find('=') > 0) )
			{
				const string split_key (value.substr(0, value.find('='))) ;
				const AwsS3Wrapper::Enum_S3MetadataTag split_tag (to_S3MetadataTag(split_key)) ;
				const string split_value (value.substr(1+value.find('='))) ;

				if (S3TAG_CUSTOM_PAIR == split_tag) 
				{ 
					// Split_key not found, e.g. "Quality_Level=98"  (Split key: "Quality_Level" is not pre-defined)
					// The map will contain: S3TAG_CUSTOM_PAIR -> "Quality_Level=98"
					tags_M.insert( pair<AwsS3Wrapper::Enum_S3MetadataTag, string>(split_tag, value) ) ;
				}
				else
				{
					// Split_key found, e.g. "S3TAG_FORMAT1_APPL=2.0.1"  (Split key "S3TAG_FORMAT1_APPL" is pre-defined)
					// The map will contain the pair: S3TAG_FORMAT1_APPL -> "2.0.1"
					tags_M.insert( pair<AwsS3Wrapper::Enum_S3MetadataTag, string>(split_tag, split_value) ) ;
				}
			}	// END: valid entry
		}	// END: for (size_t i=0; i<entries_V.size(); i++)	
	}	// END: valid tag

	return tags_M ;
}


/**********************************************************************************
  *	                                                                              *
  *	  removeBucket
  *	                                                                              *
  **********************************************************************************/
void AwsS3Wrapper::removeBucket(const string& region, const string& bucket_name_in) 
{
	/**
		Remove a bucket.
	*/

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. Region: \"" << region << "\"  Bucket name: \"" + bucket_name_in + "\"" ) ;

	isBucket(bucket_name_in, true/*always throw*/) ; // Bucket must exist !

	const Aws::String bucket_name( to_AwsString(bucket_name_in) );

// FIXME UNABLE TO SPECIFY REGION -- Won't work if region is not the default one !!!
	Aws::S3::Model::DeleteBucketRequest bucket_request;
	bucket_request.WithBucket(bucket_name) ; // .WithDeleteBucketConfiguration(bucket_configuration_);

	const Aws::S3::Model::DeleteBucketOutcome outcome ( s3_client_p_->DeleteBucket(bucket_request) ) ;

	if (false == outcome.IsSuccess())
	{
		ostringstream o ; 
		o << "Error while removing bucket: \"" << bucket_name << "\".  Got exception: " << outcome.GetError().GetExceptionName() 
			<< "  Message: " << outcome.GetError().GetMessage() ;
		
		ACS_THROW( AwsS3WrapperException(o.str()) ) ; 
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Removed bucket: \"" + bucket_name_in + "\"" ) ;

	return ; 
}

 
/**********************************************************************************
  *	                                                                              *
  *	  listBuckets
  *	                                                                              *
  **********************************************************************************/
vector <string> AwsS3Wrapper::listBuckets() const
{
	/**
		Return a vector with the buckets, as string.
	*/
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called." ) ;

	vector <string> buckets_V ; 
	const Aws::S3::Model::ListBucketsOutcome outcome ( s3_client_p_->ListBuckets() ) ;

	if (true == outcome.IsSuccess())
	{
		const Aws::Vector<Aws::S3::Model::Bucket> bucket_list ( outcome.GetResult().GetBuckets() ) ;

		for (Aws::Vector<Aws::S3::Model::Bucket>::const_iterator i=bucket_list.begin() ; i!=bucket_list.end() ; i++)
		{
			buckets_V.push_back( to_string( i->GetName() ) ) ; 	
		}
	}
	else
	{
		ostringstream o ; 
		o << "Error while listing buckets.  Got exception: " << outcome.GetError().GetExceptionName() 
			<< "  Message: " << outcome.GetError().GetMessage() ;
		
		ACS_THROW( AwsS3WrapperException(o.str()) ) ; 
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Returning: " << buckets_V.size() << " values." ) ;

	return buckets_V ; 
}


/**********************************************************************************
  *	                                                                              *
  *	  listContent
  *	                                                                              *
  **********************************************************************************/
void AwsS3Wrapper::listContent(const std::string& bucket_name_in, const string& start_from_rel_path, AwsS3Wrapper::DirContent& dc) const
{
	/**
		Return a vector containg the content of the bucket.
	*/
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. Bucket name: \"" + bucket_name_in + "\"" ) ;

	isBucket(bucket_name_in, true/*always throw*/) ; // Bucket must exist !

	const Aws::String bucket_name( to_AwsString(bucket_name_in) );
	dc.clear() ;
	Aws::S3::Model::ListObjectsRequest request ;
	request.WithBucket(bucket_name) ;
	const Aws::S3::Model::ListObjectsOutcome outcome ( s3_client_p_->ListObjects(request) ) ;
	
		
	if (true == outcome.IsSuccess())
	{
		const bool removeLeadingSlash(true) ; const bool removeLeadingDot(true) ;
	
		const Aws::Vector<Aws::S3::Model::Object> object_list ( outcome.GetResult().GetContents() ) ;

		for (Aws::Vector<Aws::S3::Model::Object>::const_iterator i=object_list.begin() ; i!=object_list.end() ; i++)
		{ 
			string element ( to_string( i->GetKey() ))  ; 
			const bool is_file ( (element.length() > 1) && ('/' != element.at(element.length()-1)) ) ;
			element = StringUtils::pathNormalize(element, removeLeadingSlash, removeLeadingDot)	;

			if (true == is_file) { dc.addFile(element) ; } else { dc.addDirectory(element) ; } 
		}
	
		if (false == start_from_rel_path.empty())
		{
			DirContent tmp (dc) ;
			dc = tmp.at(start_from_rel_path) ; 
		}
	
	}
	else
	{
		ostringstream o ; 
		o << "Error while listing objects.  Got exception: " << outcome.GetError().GetExceptionName() 
			<< "  Message: " << outcome.GetError().GetMessage() ;
		
		ACS_THROW( AwsS3WrapperException(o.str()) ) ; 
	}


	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished." ) ;

	return ; 
}


/**********************************************************************************
  *	                                                                              *
  *	  getBucketRegion
  *	                                                                              *
  **********************************************************************************/
string AwsS3Wrapper::getBucketRegion(const std::string& bucket_name_in, const bool override_unset) const
{
	/**
		Return a region (e.g. "us-west-1") given a bucket name (e.g. "test123456").
		If a bucket as been created as "unset region", a valid region might be forced, if the bool <override_unset> is set.
	*/
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. Bucket name: \"" + bucket_name_in + "\"" ) ;

	const Aws::String bucket_name( to_AwsString(bucket_name_in) );
	string bucket_region ;
	Aws::S3::Model::GetBucketLocationRequest request ;
	request.WithBucket(bucket_name) ;
		
	const Aws::S3::Model::GetBucketLocationOutcome outcome( s3_client_p_->GetBucketLocation (request) ) ;  

	if (true == outcome.IsSuccess())
	{
		const Aws::S3::Model::BucketLocationConstraint bc ( outcome.GetResult().GetLocationConstraint() ) ;
		if (bc != Aws::S3::Model::BucketLocationConstraint::NOT_SET)	{ bucket_region = stringFromRegion(bc) ; }
		else if (true == override_unset)								{ bucket_region = get_unset_region() ; }
		else 															{ bucket_region = "<NOT SET>" ; }
	}
	else
	{
		ostringstream o ; 
		o << "Error while getting bucket region for bucket: \"" << bucket_name << "\".  Got exception: " << outcome.GetError().GetExceptionName() 
			<< "  Message: " << outcome.GetError().GetMessage() ;
		
		ACS_THROW( AwsS3WrapperException(o.str()) ) ; 
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Returning: \"" << bucket_region << "\"" ) ; 

	return bucket_region ; 
}


/**********************************************************************************
  *	                                                                              *
  *	  isBucket
  *	                                                                              *
  **********************************************************************************/
bool 
	AwsS3Wrapper::isBucket(	const std::string& bucket_name_in, 
								const bool emit_exception) const
{
	/**
		Return true if a bucket exists and is accessible
		Note that IF EXIT_EXCEPTION IS FALSE the method will return success/not success (bool T/F)
		IF EXIT_EXCEPTION IS TRUE the method will always return true, or an exception in case of insuccess (never return false)
	*/
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. Bucket name: \"" + bucket_name_in + "\"" ) ;

	bool success (false) ; 

	if ( false == bucket_name_in.empty() )
	{
		const Aws::String bucket_name( to_AwsString(bucket_name_in) );
		Aws::S3::Model::HeadBucketRequest request ;
		request.WithBucket(bucket_name) ;
// FIXME CHECK IF IT WORKS ALSO FOR DIFFERENT REGIONS THAT THE ONE USED AT THE BEGINNING OR THE DEFAULT ONE

		const Aws::S3::Model::HeadBucketOutcome outcome ( s3_client_p_->HeadBucket  (request) ) ;  	
// FIXME INVESTIGATE THE REASON OF FAILURE ON HeadBucketOutcome (E.G. ENDPOINT? BUCKET? )
		success = outcome.IsSuccess() ;
	}

	ACS_COND_THROW ( (emit_exception && (false == success)), AwsS3WrapperNoSuchBucketException("Invalid bucket name or not found: \"" + bucket_name_in + "\"") ) ;

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Returning: " << (success?"T":"F")) ; 

	return success ; 
}


/**********************************************************************************
  *	                                                                              *
  *	  uploadSmallObject
  *	                                                                              *
  **********************************************************************************/
std::string AwsS3Wrapper::uploadSmallObject(const string& local_object, const string& bucket_name_in, const string& s3key_in)
{
	/**
		Upload a local object to S3 pair: (bucket; s3key). 
		For example: local_object=/tmp/file1,  bucket="bucket2",  s3key="fold1/fold2/file88"
		will create the remote object  /bucket2/fold1/fold2/file88  with the content of S3:/tmp/file1
		SINCE IT DOES NOT USE the TransferManager paradigm, THE MEMORY FOOTPRINT MATCHES THE FILE TO TRANSFER !!!!
		THAT IS: A 2-GB FILE WILL ALLOCATE 2-GB OF MEMORY.
		THIS METHOD IS DEPRECATED AND PROBABLY WILL BE REMOVED.
		PLEASE KEEP IN MIND THAT IT SHOULD BE USED FOR EXTREMELY SHORT FILES, E.G. <= 128 MB.
		******   IT WILL CRASH FOR FILES > 2 GB EVEN IN CASE OF FREE MEMORY  *******
		SEE ALSO (AND USE ONLY) : AwsS3Wrapper::uploadObject
	*/
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.  local_object to read: \"" + local_object + "\"  Bucket name: \"" + bucket_name_in + \
		"\"  s3key to write: \"" + s3key_in + "\"" ) ;
		
	isBucket(bucket_name_in, true/*always throw*/) ; // Bucket must exist !
	ACS_COND_THROW( ((false == File::exists(local_object)) || (true == File::isDir(local_object))), \
					AwsS3WrapperNoSuchObjectException("Local object not found or invalid: \"" + local_object + "\"") ) ; 

	const Aws::String bucket_name( to_AwsString(bucket_name_in) );
	const bool removeLeadingSlash(true) ; const bool removeLeadingDot(false) ; 
	Aws::S3::Model::PutObjectRequest object_request;
    object_request.SetBucket(bucket_name);
    object_request.SetContentDisposition("attachment; filename=\"product-file.txt\"");
	
	const Aws::String s3_remote_key( to_AwsString( StringUtils::pathNormalize( s3key_in, removeLeadingSlash, removeLeadingDot) ) ) ; 
	ACS_COND_THROW( (true == s3_remote_key.empty()), AwsS3WrapperException("Remote S3 key resulted empty") ) ; 
    object_request.SetKey(s3_remote_key);

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Set PutObjectRequest for bucket name: \"" << to_string(bucket_name) << "\"  key: \"" << to_string(s3_remote_key) << "\"") ;

	const size_t to_read(1048576) ;
	unsigned char buffer[to_read] ;
	int fd ( open(local_object.c_str(), O_RDONLY) ) ; 

	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Local object: \"" << local_object << "\" open with fd: " << fd) ;

	ssize_t r ; 
	size_t r_tot(0) ; 
	size_t iter_tot(0) ; 
	if (fd < 0)
	{	
		ostringstream o ; o << "Unable to open local object: \"" << local_object << "\"  Errno: " << errno << "  (" << strerror(errno) << ")" ; 
		ACS_THROW( exIOException(o.str()) ) ; 
	}
	
	{
		acs::pattern::FDGuard fd_guard_in(fd) ;
	
		shared_ptr<Aws::IOStream> data = Aws::MakeShared<Aws::StringStream>("PutObjectInputStream", std::stringstream::in | std::stringstream::out | std::stringstream::binary);

		while ( (r = read(fd, buffer, to_read)) > 0 )
		{
			r_tot += r ; 
			ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "#" << iter_tot << "  to_read: " << to_read << " r: " << r << " so far: " << r_tot << "  <<<<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<<<<") ; 
    		data->write(reinterpret_cast<char*>(buffer), r);
			
			if (true == data->fail())
			{
				ostringstream o ; o <<" Unable to append to stream a buffer of length: " << r ; 
				ACS_THROW( exIOException(o.str()) ) ; 
			}
			
			iter_tot++ ;
		}	// END: while

		if (r <= 0) { ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "#" << iter_tot << "  to_read: " << to_read << " r: " << r << " so far: " << r_tot << "  <<<<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<f<<<<") ; }
		
		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "object_request: setting body") ;
		object_request.SetBody(data);
		
		if (r < 0)
		{		
			ostringstream o ; o << "Unable to write local object: \"" << local_object << "\"  Errno: " << errno << "  (" << strerror(errno) << ")" ;  
			ACS_THROW( exIOException(o.str()) ) ; 
		} 
		
	}	// END: Scope for fd_guard_in

	const Aws::S3::Model::PutObjectOutcome outcome ( s3_client_p_->PutObject(object_request) ) ;

    if( false == outcome.IsSuccess() )
	{
		ostringstream o ; 
		o << "Error while uploading local object: \"" + local_object + "\".  Got exception: " << outcome.GetError().GetExceptionName() 
			<< "  Message: " << outcome.GetError().GetMessage() ;
		
		ACS_THROW( AwsS3WrapperException(o.str()) ) ; 
    }
			
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Bucket name: \"" + bucket_name_in + "\"  local_object: \"" + local_object + "\"" \
		+ "  s3_remote_key: \"" << to_string(s3_remote_key) << "\"  bytes transferred: " << r_tot << "  over " << iter_tot << " iterations" ) ;

	return to_string(s3_remote_key) ; 
}


/**********************************************************************************
  *	                                                                              *
  *	  uploadObject
  *	                                                                              *
  **********************************************************************************/
std::string AwsS3Wrapper::uploadObject(	const string& local_object, const string& bucket_name_in, const string& s3key_in, \
											const multimap <Enum_S3MetadataTag, std::string>& tags_M) 
{
	/**
		Upload a local object to S3 pair: (bucket; s3key). 
		For example: local_object=/tmp/file1,  bucket="bucket2",  s3key="fold1/fold2/file88"
		will create the remote object  /bucket2/fold1/fold2/file88  with the content of S3:/tmp/file1
		Using the TransferManager paradigm, it is able to transfer extremely large files with low memory footprint
	*/
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.  local_object to read: \"" + local_object + "\"  Bucket name: \"" + bucket_name_in + \
		"\"  s3key to write: \"" + s3key_in + "\"" ) ;
		
	isBucket(bucket_name_in, true/*always throw*/) ; // Bucket must exist !
	ACS_COND_THROW( ((false == File::exists(local_object)) || (true == File::isDir(local_object))), \
					AwsS3WrapperNoSuchObjectException("Local object not found or invalid: \"" + local_object + "\"") ) ; 

	const Aws::String bucket_name( to_AwsString(bucket_name_in) );
	const bool removeLeadingSlash(true) ; const bool removeLeadingDot(false) ; 
	
	const Aws::String s3_remote_key( to_AwsString( StringUtils::pathNormalize( s3key_in, removeLeadingSlash, removeLeadingDot) ) ) ; 
	ACS_COND_THROW( (true == s3_remote_key.empty()), AwsS3WrapperException("Remote S3 key resulted empty") ) ; 
   
	// Setup the TransferManagerConfiguration:
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Creating a PooledThreadExecutor with " << get_upload_pool_size() << " threads") ; 

	shared_ptr<Aws::Utils::Threading::PooledThreadExecutor> pooledThreadExecutor = \
		Aws::MakeShared<Aws::Utils::Threading::PooledThreadExecutor>("executor", std::max(uint16_t(1),get_upload_pool_size()));

#if(0)	
/*
EC:: 2022-01-28
A problem was observer with a test machine using AWS C++ API version 1.18 (not yet used in production)
-------------------------------------------------------------------------------------------------------

The use of the DefaultExecutor (which worked perfectly up to API 1.17) coupled to the TransferManager
causes a memory corruption.
Due to the usual total lack of good documentation and any kind of example from Amazon, the problem could not be addressed.
Anyway it's worth noting that:
- The few known examples of TransferManager use the PooledThreadExecutor
- On several forums, the use of PooledThreadExecutor is recommended over DefaultExecutor (no example of the latter)
- The use of the (single-threaded) DefaultExecutor has always been deprecated since "slow".

As result, the DefaultExecutor is totally disabled in this code, too.
Even in the (unlikely ?) case of single-thread transfers, a PooledThreadExecutor configured with "1 thread" will be used instead.
No real side-effects, and in any case the code with DefaultExecutor seems not able to run with API >= 1.18
*/

	Aws::Utils::Threading::DefaultExecutor defaultExecutor ;
	Aws::Transfer::TransferManagerConfiguration transferConfig(
		(get_upload_pool_size()>1)? static_cast<Aws::Utils::Threading::Executor*>(pooledThreadExecutor.get())
								  :	static_cast<Aws::Utils::Threading::Executor*>(&defaultExecutor)
	);
#else
	// This portion of code always uses the PooledThreadExecutor -- recommended !
	Aws::Transfer::TransferManagerConfiguration transferConfig(pooledThreadExecutor.get()) ;
#endif


	transferConfig.s3Client = s3_client_p_ ;
	transferConfig.uploadProgressCallback = AwsS3Wrapper::uploadProgressCallback ;
	
	if (get_upload_pool_size() > 1)
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Setting the buffer size of PooledThreadExecutor to: " << get_upload_buffer_size() << " bytes") ; 
		transferConfig.bufferSize = get_upload_buffer_size() ;
		const uint64_t heap_size (1LL * get_upload_buffer_size() * get_upload_pool_size()) ; 
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Setting the heap size of PooledThreadExecutor to: " << \
			heap_size << " bytes") ; 
		transferConfig.transferBufferMaxHeapSize = heap_size ; 
	} else {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Pool size is 1:  Using DefaultExecutor") ; 
	}

	// Handle metadata and ContentDisposition
	Aws::Map<Aws::String, Aws::String> metadata_M ;
	string metadata, download_name_override ;
	
	for (multimap <AwsS3Wrapper::Enum_S3MetadataTag, string>::const_iterator it=tags_M.begin() ; it !=tags_M.end() ; it++)
	{ 
		const AwsS3Wrapper::Enum_S3MetadataTag tag( it->first ) ;
		const string value( it->second ) ; 	
		string value_pair ; 

		if (AwsS3Wrapper::S3TAG_CUSTOM_PAIR == tag)
		{
			value_pair = value ; // The tag_value itself is in the format: custom_key=custom_value ; 
		}
		else
		{
			// Not a custom tag: a value pair will be created:
			const string preformatted_key (toStr(tag)) ; 
			const string preformatted_value (value) ; 
			value_pair = preformatted_key + "=" + preformatted_value ;

			if (AwsS3Wrapper::S3TAG_TO_BE_RENAMED == tag) { download_name_override = value ; }
		}

		metadata += (metadata.empty()? "" : "&") + value_pair ;

	}	// END: for (multimap <AwsS3Wrapper::Enum_S3MetadataTag, string>::const_iterator it=tags_M.begin() ; it !=tags_M.end() ; it++)
	
	// Set the metadata tag
	if (false == metadata.empty()) { metadata_M.insert ( pair<Aws::String, Aws::String> ("x-amz-tagging", to_AwsString(metadata)) ) ; }
	
	// Set the ContentDisposition:
	if (false == download_name_override.empty())
	{
		Aws::S3::Model::PutObjectRequest putObj_req ;
		putObj_req.SetContentDisposition( to_AwsString("attachment; filename=\"" + download_name_override + "\"") );
		transferConfig.putObjectTemplate = putObj_req ;
		
		Aws::S3::Model::CreateMultipartUploadRequest multiPartObj_req ;			
		multiPartObj_req.SetContentDisposition( to_AwsString("attachment; filename=\"" + download_name_override + "\"") );
		transferConfig.createMultipartUploadTemplate = multiPartObj_req ;
		
	}
	
	// Create an instance of the TransferManager:
	std::shared_ptr<Aws::Transfer::TransferManager> transferManager (Aws::Transfer::TransferManager::Create(transferConfig)) ; 

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Starting TransferManager for bucket name: \"" << to_string(bucket_name) << "\"  key: \"" << to_string(s3_remote_key) << "\"") ;
	
	std::shared_ptr<Aws::Transfer::TransferHandle> transferHandle ( transferManager->UploadFile(to_AwsString(local_object), bucket_name, s3_remote_key, \
														"application/octet-stream", metadata_M) ) ;

	struct timespec w ; w.tv_sec = 0 ; w.tv_nsec = 125000000 ; /* 1/8 s -- quite smaller than the timing of any Internet cloud */ 

	while ( (transferHandle->GetStatus() == Aws::Transfer::TransferStatus::NOT_STARTED) \
			|| (transferHandle->GetStatus() == Aws::Transfer::TransferStatus::IN_PROGRESS) ) 
	{
		if (true == stopController_.requestedCancel()) { transferHandle->Cancel() ; }
		nanosleep (&w, NULL) ;
	}

	if ( transferHandle->GetStatus() != Aws::Transfer::TransferStatus::COMPLETED )
	{
		ostringstream o ; 
		o << "Error while uploading local object: \"" + local_object + "\".  Got Invalid Status: " << toStr(transferHandle->GetStatus()) ; 
		
		ACS_THROW( AwsS3WrapperException(o.str()) ) ; 
    }
			
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Bucket name: \"" + bucket_name_in + "\"  local_object: \"" + local_object + "\"" \
		+ "  s3_remote_key: \"" << to_string(s3_remote_key) << "\"" ) ;

	return to_string(s3_remote_key) ; 
}


/**********************************************************************************
  *	                                                                              *
  *	  downloadProgressCallback
  *	                                                                              *
  **********************************************************************************/
void AwsS3Wrapper::downloadProgressCallback (const Aws::Transfer::TransferManager* tm, const std::shared_ptr<const Aws::Transfer::TransferHandle>& th)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_INS_VERB,  "Method called." << "  Transferred: " << th->GetBytesTransferred() \
		<< "  Total size: " << th->GetBytesTotalSize() << endl ) ;
	
	// DO NOTHING
}

/**********************************************************************************
  *	                                                                              *
  *	  uploadProgressCallback
  *	                                                                              *
  **********************************************************************************/
void AwsS3Wrapper::uploadProgressCallback (const Aws::Transfer::TransferManager* tm, const std::shared_ptr<const Aws::Transfer::TransferHandle>& th)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_INS_VERB,  "Method called." << "  Transferred: " << th->GetBytesTransferred() \
		<< "  Total size: " << th->GetBytesTotalSize() << endl ) ;

	// DO NOTHING
}


/**********************************************************************************
  *	                                                                              *
  *	  downloadObject
  *	                                                                              *
  **********************************************************************************/
off64_t AwsS3Wrapper::downloadObject(const string& bucket_name_in, const string& s3key_in, const string& local_object)
{
	/**
		Download an object from the S3 pair: (bucket; s3key) saving it to the local file system.
		For example: ,  bucket="bucket2",  s3key="fold1/fold2/file88",  local_object=/tmp/file1
		will create the local file /tmp/file1 with the content of:  S3:/bucket2/fold1/fold2/file88   

		NOTE THAT THE DESTINATION LOCAL OBJECT MUST _NOT_ EXIST !
		To be sure, you might want to always issue a File::remove(local_object) before this call. 
		The remove method above does not return exception if not existing.
	*/
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.  Bucket name: \"" + bucket_name_in + "\"  s3key to read from: \"" \
								+ s3key_in + "\"  local_object to write: \"" + local_object + "\"" ) ;
		
	isBucket(bucket_name_in, true/*always throw*/) ; // Bucket must exist !
	ACS_COND_THROW( (true == File::exists(local_object)), \
					AwsS3WrapperException("Local object already existing: \"" + local_object + "\"") ) ; 

	const Aws::String bucket_name( to_AwsString(bucket_name_in) );
	const bool removeLeadingSlash(true) ; const bool removeLeadingDot(false) ; 
	Aws::S3::Model::GetObjectRequest object_request;
    object_request.SetBucket(bucket_name);
	
	const Aws::String s3_remote_key( to_AwsString( StringUtils::pathNormalize( s3key_in, removeLeadingSlash, removeLeadingDot) ) ) ; 
	ACS_COND_THROW( (true == s3_remote_key.empty()), AwsS3WrapperException("Remote S3 key resulted empty") ) ; 
	isFileObject(bucket_name_in, to_string(s3_remote_key), true) ; // This will emit exception if the remote object DOES NOT EXIST (or not a valid file, e.g. is a directory)

	const off64_t tot_size(getObjectSize(bucket_name_in, to_string(s3_remote_key))) ;

	// Setup the TransferManagerConfiguration:
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Creating a PooledThreadExecutor with " << get_download_pool_size() << " threads") ; 

	shared_ptr<Aws::Utils::Threading::PooledThreadExecutor> pooledThreadExecutor = \
		Aws::MakeShared<Aws::Utils::Threading::PooledThreadExecutor>("executor", std::max(uint16_t(1),get_upload_pool_size()));
	
#if(0)	
/*
EC:: 2022-01-28
A problem was observer with a test machine using AWS C++ API version 1.18 (not yet used in production)
-------------------------------------------------------------------------------------------------------

The use of the DefaultExecutor (which worked perfectly up to API 1.17) coupled to the TransferManager
causes a memory corruption.
Due to the usual total lack of good documentation and any kind of example from Amazon, the problem could not be addressed.
Anyway it's worth noting that:
- The few known examples of TransferManager use the PooledThreadExecutor
- On several forums, the use of PooledThreadExecutor is recommended over DefaultExecutor (no example of the latter)
- The use of the (single-threaded) DefaultExecutor has always been deprecated since "slow".

As result, the DefaultExecutor is totally disabled in this code, too.
Even in the (unlikely ?) case of single-thread transfers, a PooledThreadExecutor configured with "1 thread" will be used instead.
No real side-effects, and in any case the code with DefaultExecutor seems not able to run with API >= 1.18
*/
	
	Aws::Utils::Threading::DefaultExecutor defaultExecutor ;
	Aws::Transfer::TransferManagerConfiguration transferConfig(
		(get_download_pool_size()>1)? static_cast<Aws::Utils::Threading::Executor*>(pooledThreadExecutor.get())
								  :	static_cast<Aws::Utils::Threading::Executor*>(&defaultExecutor)
	);

#else
	// This portion of code always uses the PooledThreadExecutor -- recommended !
	Aws::Transfer::TransferManagerConfiguration transferConfig(pooledThreadExecutor.get()) ;
#endif

	transferConfig.s3Client = s3_client_p_ ;
	transferConfig.downloadProgressCallback = AwsS3Wrapper::downloadProgressCallback ;
	
	if (get_download_pool_size() > 1)
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Setting the buffer size of PooledThreadExecutor to: " << get_download_buffer_size() << " bytes") ; 
		transferConfig.bufferSize = get_download_buffer_size() ;
		const uint64_t heap_size (1LL * get_download_buffer_size() * get_download_pool_size()) ; 
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Setting the heap size of PooledThreadExecutor to: " << \
			heap_size << " bytes") ; 
		transferConfig.transferBufferMaxHeapSize = heap_size ; 

	} else {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Pool size is 1:  Using DefaultExecutor") ; 
	}

	// Create an empty Download Configuration:
	Aws::Transfer::DownloadConfiguration dc ;
	
	// Create an instance of the TransferManager:
	std::shared_ptr<Aws::Transfer::TransferManager> transferManager (Aws::Transfer::TransferManager::Create(transferConfig)) ; 


	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Starting TransferManager for bucket name: \"" << to_string(bucket_name) << "\"  key: \"" << to_string(s3_remote_key) << "\"") ;
	
	std::shared_ptr<Aws::Transfer::TransferHandle> transferHandle ( \
		transferManager->DownloadFile( bucket_name, s3_remote_key, to_AwsString(local_object), dc) \
	) ;

	struct timespec w ; w.tv_sec = 0 ; w.tv_nsec = 125000000 ; /* 1/8 s -- quite smaller than the timing of any Internet cloud */ 

	while ( (transferHandle->GetStatus() == Aws::Transfer::TransferStatus::NOT_STARTED) \
			|| (transferHandle->GetStatus() == Aws::Transfer::TransferStatus::IN_PROGRESS) ) 
	{
		if (true == stopController_.requestedCancel()) { transferHandle->Cancel() ; }
		nanosleep (&w, NULL) ;
	}

	if ( transferHandle->GetStatus() != Aws::Transfer::TransferStatus::COMPLETED )
	{
		ostringstream o ; 
		o << "Error while downloading local object: \"" + local_object + "\".  Got Invalid Status: " << toStr(transferHandle->GetStatus()) ; 
		
		ACS_THROW( AwsS3WrapperException(o.str()) ) ; 
    }
			
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Bucket name: \"" + bucket_name_in + "\"  local_object: \"" + local_object + "\"" \
		+ "  s3_remote_key: \"" << to_string(s3_remote_key) << "\"" ) ;

	const off64_t sz (File::size(local_object)) ; 

	if ((sz < tot_size) && (true == stopController_.requestedCancel()))
	{
		::unlink(local_object.c_str());
		ostringstream o ; 
		o << "Cancelled download from bucket: \"" << bucket_name<< "\"  s3key: \"" <<s3_remote_key 
			<< "\" to local file: \"" + local_object + "\".  Local file removed." ;
		ACS_THROW( AwsS3WrapperException(o.str()) ) ; 
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Bucket name: \"" + bucket_name_in + "\"  s3_remote_key: \"" << to_string(s3_remote_key) 
		<< "\"  destination local_object: \"" + local_object + "\"  bytes transferred: " << sz ) ;

	return sz ; 
}


/**********************************************************************************
  *	                                                                              *
  *	  copyObject
  *	                                                                              *
  **********************************************************************************/
string AwsS3Wrapper::copyObject(	const string&  bucket_name_in_s, const string&  s3key_in_s, \
									const string& bucket_name_out_s, const string& s3key_out_s) const
{	
	/**
		Copy object among buckets
	*/
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.  Input bucket name: \"" + bucket_name_in_s + "\"  input s3key: \"" \
		+ s3key_in_s + "\"  Output bucket name: \"" + bucket_name_out_s + "\"  output s3key: \"" + s3key_out_s + "\"" ) ;
 		
	isBucket(bucket_name_in_s, true/*always throw*/) ; // Input Bucket must exist !
	isBucket(bucket_name_out_s, true/*always throw*/) ; // Output Bucket must exist !

	const bool removeLeadingSlash(true) ; const bool removeLeadingDot(false) ; 
	const Aws::String  bucket_name_in( to_AwsString( StringUtils::pathNormalize(bucket_name_in_s, removeLeadingSlash, removeLeadingDot)) );
	const Aws::String bucket_name_out( to_AwsString(StringUtils::pathNormalize(bucket_name_out_s, removeLeadingSlash, removeLeadingDot)) );
	
	// Enforce checks on the input object
	isObject(	StringUtils::pathNormalize(bucket_name_in_s, removeLeadingSlash, removeLeadingDot), \
				StringUtils::pathNormalize(s3key_in_s, removeLeadingSlash, removeLeadingDot), true) ; // This will emit exception if the input remote object DOES NOT EXIST 

	// Enforce checks on the output object
	const Aws::String s3_remote_key_out( to_AwsString( StringUtils::pathNormalize( s3key_out_s, removeLeadingSlash, removeLeadingDot) ) ) ; 
	ACS_COND_THROW( (true == s3_remote_key_out.empty()), AwsS3WrapperException("Remote S3 out key out resulted empty") ) ; 

	// Create copy request object
	Aws::S3::Model::CopyObjectRequest copy_request;

	// Set source (as url: bucket_in+"/"+key_in) :
	const Aws::String s3_remote_url_in ( to_AwsString( \
		StringUtils::pathNormalize(bucket_name_in_s, removeLeadingSlash, removeLeadingDot) + "/" + \
		StringUtils::pathNormalize(s3key_in_s, removeLeadingSlash, removeLeadingDot)) ) ;
    copy_request.SetCopySource(s3_remote_url_in);

	// Set destination (as usual: 2 separate calls for bucket and key)
    copy_request.SetBucket(bucket_name_out);
    copy_request.SetKey(s3_remote_key_out);
  
	// Copy object
	Aws::S3::Model::CopyObjectOutcome outcome = s3_client_p_->CopyObject(copy_request);

    if (false == outcome.IsSuccess())
    {
		ostringstream o ; 
		o << "Error while copying from bucket: \"" << bucket_name_in_s << "\"  s3key: \"" <<s3key_in_s \
			<< "\"  to bucket: \"" << bucket_name_out << "\"  s3key: \"" <<s3_remote_key_out << "\"" \
			<< "  Message: " << outcome.GetError().GetMessage() ;

		ACS_THROW( AwsS3WrapperException(o.str()) ) ; 
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. From bucket: \"" << bucket_name_in_s << "\"  s3key: \"" <<s3key_in_s \
			<< "\"  to bucket: \"" << to_string(bucket_name_out) << "\"  s3key: \"" <<to_string(s3_remote_key_out) << "\"" ) ;

	return to_string(s3_remote_key_out) ; 
}


/**********************************************************************************
  *	                                                                              *
  *	  createDirectory
  *	                                                                              *
  **********************************************************************************/
string AwsS3Wrapper::createDirectory(const string& bucket_name_in, 
										const string& remote_path_in, 
										const bool recursive)
{
	/**
		Create a remote directory.
		If recursive is FALSE, the directory must be not-existent while all the inner path must be existing
		If recursive is TRUE, both the directory and the inner paths can be existing or not: no error will 
		be generated.
	*/	

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.  Bucket name: \"" + bucket_name_in 
		+ "\"  remote_rel_path: \"" + remote_path_in + "\" recursive: " << (recursive?"T":"F") ) ;
		
	const bool removeLeadingSlash(true) ; const bool removeLeadingDot(false) ; 
	const string remote_path(StringUtils::pathNormalize(remote_path_in, removeLeadingSlash, removeLeadingDot)) ; 	

	isBucket(bucket_name_in, true/*always throw*/) ; // Bucket must exist !
	ACS_COND_THROW( ((false == recursive) && (true == isObject(bucket_name_in, remote_path))), \
		exIllegalValueException("Object already existing: \"" + remote_path + "\"  in Bucket: \"" + bucket_name_in + "\"" ) ) ; 

	const Aws::String bucket_name( to_AwsString(bucket_name_in) );
	Aws::S3::Model::PutObjectRequest object_request;
    object_request.SetBucket(bucket_name);
	
	const vector <string> path_V (StringUtils::pathDisassemble(remote_path)) ;
	string out_path (remote_path + "/") ; 

	if ( (path_V.size() >=1) && (false == isDirectoryObject(bucket_name_in, remote_path)) )
	{
		for (size_t i=0; i<path_V.size(); i++)
		{
			// Create the intermediate directory component -- or the last level if not recursive
			const vector<string> tmp (path_V.begin(), path_V.begin()+(i+1)) ;

			// Create a directory path (it terminates with "/")
			const string intermed_path(StringUtils::pathAssemble(tmp)) ;
		
			if ( (true == recursive) || (i == (path_V.size()-1)) )
			{
				if ( (false == intermed_path.empty()) && (false == isDirectoryObject(bucket_name_in, intermed_path)) ) 
				{
					const Aws::String s3_remote_key( to_AwsString(intermed_path + "/") ) ; 
					ACS_COND_THROW( (true == s3_remote_key.empty()), AwsS3WrapperException("Remote S3 key resulted empty") ) ; 
					out_path = to_string(s3_remote_key) ; 
    				object_request.SetKey(s3_remote_key);

					ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Setting PutObjectRequest for bucket name: \"" << to_string(bucket_name) << "\"  key: \"" << to_string(s3_remote_key) << "\"") ;

					const Aws::S3::Model::PutObjectOutcome outcome ( s3_client_p_->PutObject(object_request) ) ;

    				if( false == outcome.IsSuccess() )
					{
						ostringstream o ; 
						o << "Error while creating remote s3 key: \"" + s3_remote_key + "\".  Got exception: " << outcome.GetError().GetExceptionName() 
							<< "  Message: " << outcome.GetError().GetMessage() ;

						ACS_THROW( AwsS3WrapperException(o.str()) ) ; 
    				}

					ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Creating a directory tree. Bucket name: \"" << bucket_name_in << "\"  s3_remote_key: \"" \
						<< to_string(s3_remote_key) << "\"" ) ;

				}	// END: if ( (false == intermed_path.empty()= && (false == isDirectoryObject(bucket_name_in, intermed_path)) ) 

			} else {
			
				const bool emit_exception (true) ; 
				
				// IF HERE -- NOT RECURSIVE AND NOT AT LAST LEVEL.
				// All intermediate level should be already existing.
				// Next statement will throw an exception if the intermediate leel is not existing:
				( (false == intermed_path.empty()) && (isDirectoryObject(bucket_name_in, intermed_path, emit_exception)) ) ;
			}

		}	// END: for (size_t i=i_start; i<v.size(); i++)

	}	// END:	if ( (path_V.size() >=1) && (false == isDirectoryObject(bucket_name_in, remote_path)) )

	return out_path ; 
}


/**********************************************************************************
  *	                                                                              *
  *	  diagnosticSelfTest
  *	                                                                              *
  **********************************************************************************/
bool AwsS3Wrapper::diagnosticSelfTest(const string& bucket_name, const bool emit_exception)
{
	bool success (false) ; 

	try 
	{
		/**
			Run a self test, entirely contained in a random directory, that will be removed.
		*/
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.  Bucket name: \"" + bucket_name + "\"" ) ;

		isBucket(bucket_name, true/*always throw*/) ; // Bucket must exist !

		srand(time(NULL)) ;
		
		string remote_root_dir ; bool got_root (false) ;
		
		for (size_t i=0; i<10; i++)
		{
			ostringstream o ; o << "remote-root-AwsS3Wrapper-" << hex << setw(8) << setfill('0') << rand() << rand() << rand() << rand() << dec ;
			remote_root_dir = o.str() ; 
			
			if (false == isObject(bucket_name, remote_root_dir)) { got_root = true ; break ; }
		}
		
		ACS_COND_THROW ( (false == got_root), exIllegalValueException("Unable to create a random root") ) ; 
	
		excerr << "Temporary remote directory: \"" << remote_root_dir << "\"  will be created on bucket: \"" + bucket_name + "\"" << endl ;
	
		set < pair<string, bool> > ctr ;
		char buffer [ 1048526 + (rand()%100) ] ; for (size_t i=0; i<sizeof(buffer); i++) { buffer[i] = (rand() % 0xff) ; }
		char buffer_out [ sizeof(buffer) ] ; 
		FileGuard  local_file_in_guard(File::tempname(  "/tmp/file_in" )) ;  
		FileGuard local_file_out_guard(File::tempname( "/tmp/file_out" )) ;  
		string  local_file_in ( local_file_in_guard) ; 
		string local_file_out (local_file_out_guard) ; 

		{
			ofstream local_file_s(local_file_in.c_str(), std::ios::binary|std::ios::trunc|std::ios::ate);
			ACS_COND_THROW ( ((not local_file_s.is_open()) || local_file_s.fail()), exIOException("Fatal error creating file: \"" + local_file_in + "\"") ) ;
			local_file_s.write( buffer, sizeof(buffer) ) ;
			ACS_COND_THROW ( (local_file_s.fail()), exIOException("Fatal error writing to file: \"" + local_file_in + "\"") ) ;
		}

		const string  dir1 ( StringUtils::pathJoin(remote_root_dir, "dir1") ) ; 
		const string file1 ( StringUtils::pathJoin(dir1, "file1") ) ; 
		const string  dir2 ( StringUtils::pathJoin(dir1, "dir2") ) ;
		const string file2 ( StringUtils::pathJoin(dir2, "file2") ) ; 
		const string  dir3 ( StringUtils::pathJoin(dir2, "dir3") ) ;
		const string file3 ( StringUtils::pathJoin(dir3, "file3") ) ; 
		const string file4 ( StringUtils::pathJoin(dir3, "file4") ) ; 
		const string file5 ( StringUtils::pathJoin(dir3, "file5") ) ; 
		
		
		// Create root directory
		createDirectory(bucket_name, remote_root_dir) ;

		// Create remote directories
		createDirectory(bucket_name, dir1) ; createDirectory(bucket_name, dir2) ; createDirectory(bucket_name, dir3) ;
		
		// Upload remote files (but not file3)
		uploadObject(local_file_in, bucket_name, file1) ; uploadObject(local_file_in, bucket_name, file2) ; 
		uploadObject(local_file_in, bucket_name, file4) ; uploadObject(local_file_in, bucket_name, file5) ; 
		
		// Remote-copy one of the files to file3 :
		copyObject(bucket_name, file1, bucket_name, file3) ;

		// Check some of the objects created so far
		isObject(bucket_name, file3, true/*always throw*/) ; 
		isObject(bucket_name, file4, true/*always throw*/) ; 
		isFileObject(bucket_name, file2, true/*always throw*/) ;
		isDirectoryObject(bucket_name, dir3, true/*always throw*/) ;
			
		// Obtain a current dir content and check it 
		{	
			DirContent dc ;
			listContent(bucket_name, remote_root_dir, dc) ;
			const vector< pair<string, bool> > v ( dc.getEntries()) ; 
			ACS_COND_THROW ( (9 != v.size()), exIllegalValueException("Invalid size for DirContent. Expected: 9") ) ; 
			excerr << "Correctly created " << v.size() << " objects on temporary directory: \"" << remote_root_dir << "\"" << endl ;
			for (size_t i=0; i<v.size(); i++) { ctr.insert ( pair<string, bool> (v.at(i)) ) ; } 
			
			ACS_COND_THROW ( (ctr.find(pair<string, bool> (remote_root_dir , false)) == ctr.end()), exIllegalValueException("Invalid DirContent: " +   remote_root_dir)) ; 
			ACS_COND_THROW ( (ctr.find(pair<string, bool> ( dir1, false)) == ctr.end()), exIllegalValueException("Invalid DirContent: " +   dir1)) ; 
			ACS_COND_THROW ( (ctr.find(pair<string, bool> ( dir2, false)) == ctr.end()), exIllegalValueException("Invalid DirContent: " +   dir2)) ; 
			ACS_COND_THROW ( (ctr.find(pair<string, bool> ( dir3, false)) == ctr.end()), exIllegalValueException("Invalid DirContent: " +   dir3)) ; 
			ACS_COND_THROW ( (ctr.find(pair<string, bool> (file1,  true)) == ctr.end()), exIllegalValueException("Invalid DirContent: " +  file1)) ; 
			ACS_COND_THROW ( (ctr.find(pair<string, bool> (file2,  true)) == ctr.end()), exIllegalValueException("Invalid DirContent: " +  file2)) ; 
			ACS_COND_THROW ( (ctr.find(pair<string, bool> (file3,  true)) == ctr.end()), exIllegalValueException("Invalid DirContent: " +  file3)) ; 
			ACS_COND_THROW ( (ctr.find(pair<string, bool> (file4,  true)) == ctr.end()), exIllegalValueException("Invalid DirContent: " +  file4)) ; 
			ACS_COND_THROW ( (ctr.find(pair<string, bool> (file5,  true)) == ctr.end()), exIllegalValueException("Invalid DirContent: " +  file5)) ; 
		}

		// Check the size of uploaded files:
		ACS_COND_THROW ( (static_cast<off64_t>(sizeof(buffer)) != getObjectSize(bucket_name, file1)), exIllegalValueException("Invalid size for file: " + file1)) ; 
		excerr << "Remote file: " << file1 << " has been uploaded with size: " << sizeof(buffer) << endl ;
		ACS_COND_THROW ( (static_cast<off64_t>(sizeof(buffer)) != getObjectSize(bucket_name, file2)), exIllegalValueException("Invalid size for file: " + file2)) ; 
		excerr << "Remote file: " << file2 << " has been uploaded with size: " << sizeof(buffer) << endl ;
		ACS_COND_THROW ( (static_cast<off64_t>(sizeof(buffer)) != getObjectSize(bucket_name, file3)), exIllegalValueException("Invalid size for file: " + file3)) ; 
		excerr << "Remote file: " << file3 << " has been copied with size: " << sizeof(buffer) << endl ;
		ACS_COND_THROW ( (static_cast<off64_t>(sizeof(buffer)) != getObjectSize(bucket_name, file4)), exIllegalValueException("Invalid size for file: " + file4)) ; 
		excerr << "Remote file: " << file4 << " has been uploaded with size: " << sizeof(buffer) << endl ;
		ACS_COND_THROW ( (static_cast<off64_t>(sizeof(buffer)) != getObjectSize(bucket_name, file5)), exIllegalValueException("Invalid size for file: " + file5)) ; 
		excerr << "Remote file: " << file5 << " has been uploaded with size: " << sizeof(buffer) << endl ;

		// Download a file uploaded. First check the size:
		::unlink(local_file_out.c_str()) ;
		ACS_COND_THROW ( (static_cast<off64_t>(sizeof(buffer)) != downloadObject(bucket_name, file3, local_file_out)), \
			exIllegalValueException("Invalid size obtained downloading file: " + file3) ) ; 
		excerr << "Remote file: " << file3 << " locally downloaded for comparison. Size: " << sizeof(buffer) << endl ;
		// Then check the content:
		{	
			ifstream local_file_s(local_file_out.c_str(), std::ios::binary|std::ios::in);
			ACS_COND_THROW ( ((not local_file_s.is_open()) || (not local_file_s.good())), exIOException("Fatal error opening file: \"" \
				+ local_file_in + "\"") ) ;
			local_file_s.read( buffer_out, sizeof(buffer_out) ) ;
			ACS_COND_THROW ( (memcmp(buffer_out, buffer, sizeof(buffer_out))), exIOException("Fatal comparing file: " + file3 + " after local download") ) ;
		}
		excerr << "Remote file: " << file3 << " was downloaded and matched the original memory buffer" << endl ;


		// Remove objects
		removeSingleObject(bucket_name, file2) ; ACS_COND_THROW ( (true == isObject(bucket_name, file2)), exIllegalValueException("Problems removing object: " +  file2)) ; 
		removeSingleObject(bucket_name, file4) ; ACS_COND_THROW ( (true == isObject(bucket_name, file4)), exIllegalValueException("Problems removing object: " +  file4)) ; 
		multiRemoveObjects(bucket_name, dir3) ;  
		
		// Obtain a current dir content and check it	
		{	
			DirContent dc ;
			listContent(bucket_name, remote_root_dir, dc) ;
			const vector< pair<string, bool> > v ( dc.getEntries()) ; 
			ACS_COND_THROW ( (4 != v.size()), exIllegalValueException("Invalid size for DirContent. Expected: 4") ) ; 
			excerr << "After partial removal, still " << v.size() << " objects left on temporary directory: \"" << remote_root_dir << "\"" << endl ;
			for (size_t i=0; i<v.size(); i++) { ctr.insert ( pair<string, bool> (v.at(i)) ) ; } 
			
			ACS_COND_THROW ( (ctr.find(pair<string, bool> (remote_root_dir , false)) == ctr.end()), exIllegalValueException("Invalid DirContent: " +   remote_root_dir)) ; 
			ACS_COND_THROW ( (ctr.find(pair<string, bool> ( dir1, false)) == ctr.end()), exIllegalValueException("Invalid DirContent: " +   dir1)) ; 
			ACS_COND_THROW ( (ctr.find(pair<string, bool> ( dir2, false)) == ctr.end()), exIllegalValueException("Invalid DirContent: " +   dir2)) ; 
			ACS_COND_THROW ( (ctr.find(pair<string, bool> (file1,  true)) == ctr.end()), exIllegalValueException("Invalid DirContent: " +  file1)) ; 
		}
	
		removeSingleObject(bucket_name, dir2) ; ACS_COND_THROW ( (true == isObject(bucket_name, dir2)), exIllegalValueException("Problems removing object: " +  dir2)) ; 
		multiRemoveObjects(bucket_name, remote_root_dir) ;  ACS_COND_THROW ( (true == isObject(bucket_name, remote_root_dir)), exIllegalValueException("Problems removing object: " +  remote_root_dir)) ;

		excerr << "Temporary directory: \"" << remote_root_dir << "\" successfully removed." << endl ;

		// Do not move from here
		success = true ;
	}
	catch (exException& ex)
	{
		ostringstream o ; o << "Fatal error running self test:" ;
		ACS_LOG_ERROR (o.str() << ex) ;
		
		if (true == emit_exception) { throw ; }
		success = false ; 
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Bucket name: \"" + bucket_name + "\"  returning: " << (success? "T":"F") ) ;

	return success ;
}


/**********************************************************************************
  *	                                                                              *
  *	  strip_s3protocol
  *	                                                                              *
  **********************************************************************************/
string AwsS3Wrapper::strip_s3protocol(const std::string& path) 
{
	/** Return the path WITHOUT optional S3 protocol, if present.
		So: "s3:/bucket1/dir2/file3" -> "/bucket1/dir2/file3" 	
			"S3:bucket1/file2" -> "bucket1/file2" 	
			"//bucket1/file2" -> "//bucket1/file2" 	
	 */

	// This comparison should be the fastest possible, single characters and matched befor longer substrings
	if ( (path.length() >= 3) && (':' == path.at(2)) && ("s3" == StringUtils::lowercase(path.substr(0,2))) )
	{ return path.substr(3) ; }

	return path ;
}


/**********************************************************************************
  *	                                                                              *
  *	  toStr
  *	                                                                              *
  **********************************************************************************/
string AwsS3Wrapper::toStr(const Enum_S3Provider& e)
{
	if		(e == S3P_AWS)			return "AWS" ;  	// Amazon
	else if (e == S3P_WASABI)		return "WASABI" ; 
	else if (e == S3P_ORANGE)		return "ORANGE" ; 
	else if (e == S3P_HUAWEI)		return "HUAWEI" ;   
	else if (e == S3P_CYNNYSPACE)	return "CYNNYSPACE" ;   
	else if (e == S3P_OVH)			return "OVH" ;   
	else if (e == S3P_OVH_PERF)		return "OVH_PERF" ;   
	else if (e == S3P_MINIO)		return "MINIO" ;   
	return "UNKNOWN" ;
}


/**********************************************************************************
  *	                                                                              *
  *	  to_S3Provider
  *	                                                                              *
  **********************************************************************************/
AwsS3Wrapper::Enum_S3Provider AwsS3Wrapper::to_S3Provider(const string& s)
{
	if		(s == "AWS")			return S3P_AWS ;	// Amazon
	else if (s == "WASABI")			return S3P_WASABI ;
	else if (s == "ORANGE")			return S3P_ORANGE ;
	else if (s == "HUAWEI")			return S3P_HUAWEI ;
	else if (s == "CYNNYSPACE")		return S3P_CYNNYSPACE ;
	else if (s == "OVH")			return S3P_OVH ;
	else if (s == "OVH_PERF")		return S3P_OVH_PERF ;
	else if (s == "MINIO")			return S3P_MINIO ;
	return S3P_UNKNOWN ;
}


/**********************************************************************************
  *	                                                                              *
  *	  toStr
  *	                                                                              *
  **********************************************************************************/
string AwsS3Wrapper::toStr(const Enum_S3MetadataTag& e)
{
	if      (e == S3TAG_UUID)				return "UUID" ; 
	else if (e == S3TAG_TO_BE_RENAMED)		return "TO_BE_RENAMED" ; 
	else if (e == S3TAG_ID1_APPL)			return "ID1_APPL" ;   
	else if (e == S3TAG_ID2_APPL)			return "ID2_APPL" ;   
	else if (e == S3TAG_ID3_APPL)			return "ID3_APPL" ;   
	else if (e == S3TAG_FORMAT1_APPL)		return "FORMAT1_APPL" ;   
	else if (e == S3TAG_FORMAT2_APPL)		return "FORMAT2_APPL" ;   
	else if (e == S3TAG_FORMAT3_APPL)		return "FORMAT3_APPL" ;   
	else if (e == S3TAG_VERSION_APPL)		return "VERSION_APPL" ;   
	return "CUSTOM_PAIR" ;
}


/**********************************************************************************
  *	                                                                              *
  *	  to_S3MetadataTag
  *	                                                                              *
  **********************************************************************************/
AwsS3Wrapper::Enum_S3MetadataTag AwsS3Wrapper::to_S3MetadataTag(const string& s)
{
	if      (s == "UUID")			  return S3TAG_UUID ;
	else if (s == "TO_BE_RENAMED")    return S3TAG_TO_BE_RENAMED ;
	else if (s == "ID1_APPL")		  return S3TAG_ID1_APPL ;
	else if (s == "ID2_APPL")		  return S3TAG_ID2_APPL ;
	else if (s == "ID3_APPL")		  return S3TAG_ID3_APPL ;
	else if (s == "FORMAT1_APPL")	  return S3TAG_FORMAT1_APPL ;
	else if (s == "FORMAT2_APPL")	  return S3TAG_FORMAT2_APPL ;
	else if (s == "FORMAT3_APPL")	  return S3TAG_FORMAT3_APPL ;
	else if (s == "VERSION_APPL")	  return S3TAG_VERSION_APPL ;
	return S3TAG_CUSTOM_PAIR ;
}


/**********************************************************************************
  *	                                                                              *
  *	  toStr
  *	                                                                              *
  **********************************************************************************/
string AwsS3Wrapper::toStr(const Aws::Transfer::TransferStatus t)
{
	string s ; switch (t)
	{
		case Aws::Transfer::TransferStatus::EXACT_OBJECT_ALREADY_EXISTS :
		{ s = "EXACT_OBJECT_ALREADY_EXISTS" ; break ; }
		case Aws::Transfer::TransferStatus::NOT_STARTED :
		{ s = "NOT_STARTED" ; break ; }
		case Aws::Transfer::TransferStatus::IN_PROGRESS :
		{ s = "IN_PROGRESS" ; break ; }
		case Aws::Transfer::TransferStatus::CANCELED :
		{ s = "CANCELED" ; break ; }
		case Aws::Transfer::TransferStatus::FAILED :
		{ s = "FAILED" ; break ; }
		case Aws::Transfer::TransferStatus::COMPLETED :
		{ s = "COMPLETED" ; break ; }
		case Aws::Transfer::TransferStatus::ABORTED :
		{ s = "ABORTED" ; break ; }
		default:
		{ s = "UNKNOWN" ; break ; }
	}
	return s ;
}

DateTime AwsS3Wrapper::to_DateTime(const Aws::Utils::DateTime& aws_epoch)	{ return DateTime(time_t(aws_epoch.Millis() / 1000LL)) ; }


/**********************************************************************************
  *	                                                                              *
  *	  << operators
  *	                                                                              *
  **********************************************************************************/

ostream &operator<<(ostream &os, const AwsS3Wrapper::AwsS3WrapperConfiguration &v) 	// output to stream
{
	os	<< "provider = \"" 			<< AwsS3Wrapper::toStr(v.get_aws_s3_provider())	<< "\"\n" \
		<< "key_ak = \"" 			<< v.get_aws_key_ak()  							<< "\"\n" \
		<< "key_sk = \"" 			<< v.get_aws_key_sk() 							<< "\"\n" \
		<< "uploadPoolSize=" 		<< v.get_upload_pool_size() 				    << '\n' \
		<< "uploadBufferSize=" 		<< v.get_upload_buffer_size() 				    << '\n' \
		<< "downloadPoolSize=" 		<< v.get_download_pool_size() 				    << '\n' \
		<< "downloadBufferSize="	<< v.get_download_buffer_size() 			    << '\n' \
		<< "region = \"" 			<< v.get_aws_region() 							<< "\"\n" \
		<< "endpoint = \"" 			<< v.get_aws_endpoint() 						<< "\"\n" \
		<< "endpoint_is_secure = "	<< (v.get_endpoint_is_secure()?"T":"F")			<< "\n" \
		<< "remoteBasepath = \""	<< v.get_aws_remote_basepath() 					<< "\"\n" \
 		<< "options = \"" 			<< v.get_options() 								<< "\"\n" ;	 
	
	return os ; // PRQA S 4028
}

acs::exostream &operator<<(acs::exostream &os, const AwsS3Wrapper::AwsS3WrapperConfiguration& v) 	// output to stream  
{
	os	<< "provider = \"" 			<< AwsS3Wrapper::toStr(v.get_aws_s3_provider())	<< "\"\n" \
		<< "key_ak = \"" 			<< v.get_aws_key_ak()  							<< "\"\n" \
		<< "key_sk = \"" 			<< v.get_aws_key_sk() 							<< "\"\n" \
		<< "uploadPoolSize=" 		<< v.get_upload_pool_size() 				    << '\n' \
		<< "uploadBufferSize=" 		<< v.get_upload_buffer_size() 				    << '\n' \
		<< "downloadPoolSize=" 		<< v.get_download_pool_size() 				    << '\n' \
		<< "downloadBufferSize="	<< v.get_download_buffer_size() 			    << '\n' \
		<< "region = \"" 			<< v.get_aws_region() 							<< "\"\n" \
		<< "endpoint = \"" 			<< v.get_aws_endpoint() 						<< "\"\n" \
		<< "endpoint_is_secure = "	<< (v.get_endpoint_is_secure()?"T":"F")			<< "\n" \
		<< "remoteBasepath = \""	<< v.get_aws_remote_basepath() 					<< "\"\n" \
		<< "options = \"" 			<< v.get_options() 								<< "\"\n" ;		 
	
	return os ; // PRQA S 4028
}

_ACS_END_NAMESPACE
