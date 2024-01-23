/*
	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA, Inc.

	$Prod: A.C.S. libIsmSoap $

	$Id$

	$Author$

	$Log$
	Revision 1.34  2017/11/08 16:51:46  lucio.pulvirenti
	S2PDGS-1843: method for cache management added.
	
	Revision 1.33  2017/06/23 09:19:36  lucio.pulvirenti
	S3MPM-873: Added flag to check permission (default true) in structure used to upload and download files/directories. Added group to structs used to download files/directories.
	IF added along with related structs to get user and group from userid and groupid. Struct to delete entry as in FS added (no full dirs).
	
	Revision 1.32  2017/06/13 13:59:58  lucio.pulvirenti
	S3MPM-873: string user replaced with unsigned int32 userId and groupId added to Ism__listRequest, Ism__accessRequest, Ism__fileAttributes, Ism__getAttrRequest classes.
	boolean isDir removed from Ism__accessRequest class.
	
	Revision 1.31  2017/05/22 10:02:38  lucio.pulvirenti
	S3MPM-873: Ism__deleteRequest: useless fields removed. Ism__deleteFileFsLike IF added.
	
	Revision 1.30  2017/05/18 10:32:13  lucio.pulvirenti
	S3MPM-873: Ism__getAttrRequest class introduced to be used as input of Ism__getFileAttributes method.
	user attr added to Ism__listRequest class and isDir attr added to Ism__listResponse;
	
	Revision 1.29  2017/03/15 13:08:58  lucio.pulvirenti
	S3MPM-873: Ism__listRequest: user attribute removed. Ism__fileAttributes and Ism__getAttrResponse classes added.
	Ism__getFileAttributes IF declared.
	
	Revision 1.28  2017/03/08 11:26:27  lucio.pulvirenti
	S3MPM-873: Ism__listRequest and Ism__listResponse size_t var types turned into int32_t; returnCode and errnumber var added to Ism__listResponse.
	Ism__accessRequest and Ism__accessResponse structs added and Ism__access method declared.
	
	Revision 1.27  2017/02/24 14:08:56  lucio.pulvirenti
	S3MPM-873: Added classes and method to perform ls on given path.
	
	Revision 1.26  2016/01/15 09:59:25  lucio.pulvirenti
	APF-270: totalItemsSize attribute turned into a pointer to guarantee compatibility between new/old Server version and old/new Client versions.
	
	Revision 1.25  2016/01/14 16:31:56  lucio.pulvirenti
	APF-270: totalItemsSize attribute (total size of directory files) added to Ism__dirDefinition and Ism__notifyDirUploadRequest classes.
	
	Revision 1.24  2012/09/13 10:28:51  lucio.pulvirenti
	updateRequestTime field (default false) added to Ism__DownloadFileRequest struct
	
	Revision 1.23  2012/05/24 12:03:46  lucio.pulvirenti
	SOAP_ISM_DATA_UNAVAILABLE error code added
	
	Revision 1.22  2011/03/07 17:09:22  lucio.pulvirenti
	Ism__downloadMultiFilesRequest: jobId added
	
	Revision 1.21  2011/03/03 17:10:57  lucio.pulvirenti
	work in progress
	
	Revision 1.20  2011/02/23 11:17:30  lucio.pulvirenti
	Ism__deleteRequest: added onlinesIds and removeUrls fields
	
	Revision 1.19  2011/02/22 17:00:37  lucio.pulvirenti
	work in progress
	
	Revision 1.18  2011/02/16 11:11:24  lucio.pulvirenti
	long long int turned into int64 to be independent from server arch
	bool isDir added to Ism__getFileIdResponse to recognize directory
	structures and methods to donwload directories added
	
	Revision 1.17  2011/02/11 14:53:55  lucio.pulvirenti
	work in progress
	
	Revision 1.16  2011/02/02 16:28:04  lucio.pulvirenti
	work in progress
	
	Revision 1.15  2011/02/01 15:56:04  lucio.pulvirenti
	work in progress
	
	Revision 1.14  2011/01/31 17:04:14  lucio.pulvirenti
	work in progress
	
	Revision 1.13  2011/01/27 17:43:08  lucio.pulvirenti
	class and methods to upload directories added
	
	Revision 1.12  2011/01/25 18:02:22  lucio.pulvirenti
	work in progress
	
	Revision 1.11  2011/01/21 18:40:40  lucio.pulvirenti
	work in progress
	
	Revision 1.10  2011/01/20 17:40:55  lucio.pulvirenti
	work in progress
	
	Revision 1.9  2009/03/19 18:29:35  lucio.pulvirenti
	book file method and related classes added
	
	Revision 1.8  2008/10/03 15:38:20  paoscu
	Changed download methods interface to add request id parameter
	
	Revision 1.7  2008/06/16 09:42:44  paoscu
	missing header
	
	Revision 1.6  2008/05/28 17:31:58  paoscu
	Added download methods
	
	Revision 1.5  2008/05/14 15:23:50  paoscu
	Delete function added
	
	Revision 1.4  2008/05/07 17:43:29  paoscu
	Interfaces changed
	
	Revision 1.3  2008/05/07 13:46:35  paoscu
	Work in progress
	
	Revision 1.2  2008/05/05 17:30:23  paoscu
	Work in progress on interface definition
	
	Revision 1.1.1.1  2008/05/05 16:54:47  paoscu
	Imported into CVS
	
	
        
*/






namespace acs{
//gsoap Ism service name: Ism
//gsoap Ism service style: rpc
//gsoap Ism service encoding: encoded
//gsoap Ism service namespace: http://www.acsys.it/ism.wsdl
//gsoap Ism service location: http://localhost/ism
//gsoap Ism service executable: Ism
//gsoap Ism schema namespace: urn:copy

#import "stlvector.h"

/* gSOAP error codes 600 to 999 are user definable */
#define SOAP_ISM_DATA_UNAVAILABLE						600
#define SOAP_ISM_DOWNLOAD_TEMPORARY_REFUSED				650
#define SOAP_ISM_DOWNLOAD_FILE_NEARLINE					660
#define SOAP_ISM_DOWNLOAD_FILE_OFFLINE					665
#define SOAP_ISM_DATA_NOT_REMOVED_FOR_MEDIUM_MIGRATION 	690

class VectorOfInts
{
	public:
		std::vector<int> numbers ;
		//void dump() ;

};


class VectorOfStrings
{
	public:
		std::vector<std::string> strings ;
		//void dump() ;

};


class VectorOfLongLongs
{
	public:
		std::vector<int64_t> numbers ;
		//void dump() ;

};

class VectorOfBooleans
{
	public:
		std::vector<bool> b ;
		//void dump() ;

};


class Ism__fileDefinition
{
	public:
		int         fileId ;
		std::string filePath ;
		bool        isDir ;
		int64_t     size ;
		std::string user ;
		std::string group ;
		short int   permissions ;
		std::string fileClass ;
		std::string packagingAlgo ;
		
		VectorOfStrings customKeys ;
		VectorOfStrings customValues ;

		//void dump() ;
} ;


class Ism__notifyUploadRequest
{
	public:
		int fileId ;
		std::string user ;
		bool checkPermissions = true;
		std::string uploadUrl ;

		//void dump() ;
} ;

class Ism__deleteRequest
{
	public:
		std::string filePath ;
		std::string user ;
} ;

class Ism__getFileIdRequest
{
	public:
		std::string filePath ;
		std::string user ;

} ;

class Ism__getFileIdResponse
{
	public:
		int fileId ;
		bool isDir ;

} ;



class Ism__DownloadFileRequest
{
	public:
		int fileId ;
		std::string user ;
		std::string group;
		int requestId ;
		bool updateRequestTime = false;	// optional parameter with default false
		bool checkPermissions = true;

} ;


class Ism__notifyDownloadRequest
{
	public:
		int fileId ;
		int cacheId ;

} ;

// Valid until Server Version 2.x
class Ism__DownloadFileResponse
{
	public:
		std::string downloadUrl ;
		int requestId ;
		std::string packagingAlgo ;

} ;

// Valid from Server Version 3.x on

class Ism__DownloadFileRequest_2
{
	public:
		int fileId ;
		std::string user ;
		std::string group;
		int requestId ;
		bool updateRequestTime ;
		bool checkPermissions ;
		std::string clientReference;
} ;

class Ism__DownloadFileResponse_2
{
	public:
		std::string downloadUrl ;
		int requestId ;
		std::string packagingAlgo ;
		int cacheId;

} ;

class Ism__notifyDownloadResponse
{

} ;


class Ism__bookFileRequest
{
	public:
		int fileId ;
		std::string user ;
		int hoursToDeadline; 
		float latencyDays;

} ;


class Ism__bookFileResponse
{

} ;


class Ism__dirDefinition
{
	public:
		std::string filePath ;
		std::string user ;
		std::string group ;
		short int   permissions ;
		std::string fileClass ;
		int64_t *totalItemsSize;
		
		VectorOfStrings customKeys ;
		VectorOfStrings customValues ;

		// input files data. They are parallel vectors.
		// It should be a map std::map<std::string, std::pair<int64_t, bool>> where the key is remote full path and value is <size, isDir?>
		// but maps not supported by soapcpp2 
		std::vector<std::string> fullpaths;
		std::vector<int64_t> sizes;
		std::vector<bool> isDir;
		std::vector<int> filesIds;

} ;

#import "../soapIsmAdd.h"

class Ism__uploadDirectoryRequest
{
	public:
		Ism__dirDefinition *dir ;
		bool recursive ;
		bool checkPermissions = true;
		bool extendDirPermissions = true; // If true extend by OR 0111
} ;

class Ism__dirUploadResponse
{
	public:
		Ism__dirDefinition *dir ;
		
		// output files data. They are parallel vectors.
		// It should be a map std::map<std::string, std::string> where the key is remote path and value is upload URL
		// but maps not supported by soapcpp2
		std::vector<std::string> uploadUrls; // for directories the element is a void string
		
		// ism_online file_id of files being uploaded
		std::vector<int> onlinesIds;
} ;


class Ism__notifyDirUploadRequest
{
	public:
		std::vector<int> filesIds;
		std::vector<int> onlinesIds;
		std::string user ;
		int64_t *totalItemsSize;
		bool checkPermissions = true;

} ;


class Ism__directoryTreeRequest
{
	public:
		int fileId;

} ;


class Ism__directoryTreeResponse
{
	public:
		std::vector<std::string> filePaths;
		std::vector<int> filesIds;
		std::vector<bool> isDir;

} ;


class Ism__downloadMultiFilesRequest
{

	public:
		std::vector<int> filesIds ;
		std::string user ;
		std::string group ;
		std::vector<int> requestsIds ;
		int jobId;
		bool updateRequestTime;
		bool filesDisappeared;
		bool checkPermissions = true;

} ;

class Ism__downloadDirRequest
{
	public:
		std::vector<int> dirsIds ;
		Ism__downloadMultiFilesRequest *filesRequest ;

} ;


class Ism__downloadMultiFilesResponse
{
	public:
		std::vector<std::string> downloadUrls ;
		std::vector<int> requestsIds ;
		bool downloadCompleted;
		int jobId;

} ;

class Ism__giveupRequest
{
	public:
		std::string filePath ;
		std::string user ;
		std::string clientReference ;
} ;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////               	Virtual File System methods                   /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Ism__listRequest
{
	public:
		std::string path;
		unsigned int32_t userId;
		unsigned int32_t groupId;
		int32_t offset;
		int32_t entries;
} ;

class Ism__listResponse
{
	public:
		int returnCode;
		std::vector<std::string> paths ;
		int32_t offset;
		int errnumber;
} ;

class Ism__accessRequest
{
	public:
		std::string path ;
		unsigned int32_t userId;
		unsigned int32_t groupId;
		unsigned int32_t mode;

} ;

class Ism__accessResponse
{
	public:
		int returnCode;
		int errnumber;

} ;

class Ism__fileAttributes
{
	public:
		int64_t     fileId ;
		std::string filePath ;
		bool        isDir ;
		int64_t     size ;
		unsigned int32_t userId ;
		unsigned int32_t groupId ;
		int   permissions ;
		time_t creationTime; 
		time_t modificationTime;
		time_t lastAccessTime;
} ;

class Ism__getAttrRequest
{
	public:
		std::string path ;
		unsigned int32_t userId;
		unsigned int32_t groupId;

} ;

class Ism__getAttrResponse
{
	public:
		int returnCode;
		int errnumber;
		Ism__fileAttributes attributes ;

} ;


class Ism__setAttrRequest
{
	public:
		std::string path ;
		unsigned int32_t userId;
		unsigned int32_t groupId;
		Ism__fileAttributes attributes ;

} ;


class Ism__userGroupRequest
{
	public:
		unsigned int32_t userId ;
		unsigned int32_t groupId ;
} ;

class Ism__userGroupResponse
{
	public:
		std::string user ;
		std::string group ;
} ;

class Ism__deleteRequestFs
{
	public:
		std::string filePath ;
		unsigned int32_t userId ;
		unsigned int32_t groupId ;
		bool recursive ;
};	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////               	Cache management methods             //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Ism__cacheInfo
{
	public:
		std::string cacheName;
		std::string fileClass;
		int64_t freeSize;
} ;


class Ism__cacheInfoResponse
{
	public:
		std::vector<Ism__cacheInfo> cacheInfo;
} ;

// Service methods declarations


// Upload Methods for files
Ism__uploadSingleFile(Ism__uploadSingleFileRequest *in, Ism__uploadResponse *out);
Ism__notifyUploadCompleted(Ism__notifyUploadRequest *in, struct Ism__notifyCompletedUploadResponse { } *out) ;
Ism__notifyUploading(Ism__notifyUploadRequest *in, struct Ism__notifyUploadingResponse { } *out) ;


// Upload Methods for directories
Ism__uploadDirectory(Ism__uploadDirectoryRequest *in, Ism__dirUploadResponse *out);
Ism__notifyDirUploadCompleted(Ism__notifyDirUploadRequest *in, struct Ism__notifyDirCompletedUploadResponse { } *out) ;
Ism__notifyDirUploading(Ism__notifyDirUploadRequest *in, struct Ism__notifyDirUploadingResponse { } *out) ;


Ism__deleteFile(Ism__deleteRequest *in, struct Ism__deleteResponse { } *out) ;


Ism__getFileId(Ism__getFileIdRequest *in, Ism__getFileIdResponse *out) ;


// download  Methods for files

//**************** Valid until Server Version 2.x
Ism__downloadFile(Ism__DownloadFileRequest *in, Ism__DownloadFileResponse *out) ;
Ism__notifyDownloading(Ism__DownloadFileRequest *in, Ism__notifyDownloadResponse *out) ;
Ism__notifyDownloadCompleted(Ism__DownloadFileRequest *in, Ism__notifyDownloadResponse *out) ;
//********************************************

//**************** Valid from Server Version 3.0 on
Ism__downloadFile_2(Ism__DownloadFileRequest_2 *in, Ism__DownloadFileResponse_2 *out) ;
Ism__notifyDownloading_2(Ism__notifyDownloadRequest *in, Ism__notifyDownloadResponse *out) ;
Ism__notifyDownloadCompleted_2(Ism__notifyDownloadRequest *in, Ism__notifyDownloadResponse *out) ;
//********************************************

Ism__abortDownload(Ism__DownloadFileRequest *in, Ism__notifyDownloadResponse *out) ;

Ism__bookFile(Ism__bookFileRequest *in, Ism__bookFileResponse *out);

// remove ism_ext_request_reference record corresponding to reference client associated to external request of given file and owner
Ism__giveupFileRequest(Ism__giveupRequest *in, struct Ism__giveupResponse { } *out) ;

// download  Methods for directory
Ism__getDirectoryTree(Ism__directoryTreeRequest *in, Ism__directoryTreeResponse *out) ;
Ism__downloadDirectory(Ism__downloadDirRequest *in, Ism__downloadMultiFilesResponse *out) ;

// download  Methods for multiple files: To Be Implemented
//Ism__downloadMultipleFiles(Ism__DownloadMultipleFilesRequest *in, Ism__downloadMultiFilesResponse *out) ;

Ism__notifyMultiFilesDownloading(Ism__downloadMultiFilesRequest *in, Ism__notifyDownloadResponse *out) ;
Ism__notifyMultiFilesDownloadCompleted(Ism__downloadMultiFilesRequest *in, Ism__notifyDownloadResponse *out) ;
Ism__abortMultiFilesDownload(Ism__downloadMultiFilesRequest *in, Ism__notifyDownloadResponse *out) ;

// methods used for VFS
Ism__getList(Ism__listRequest *in, Ism__listResponse *out) ;
Ism__access(Ism__accessRequest *in, Ism__accessResponse *out) ;
Ism__getFileAttributes(Ism__getAttrRequest *in, Ism__getAttrResponse *out) ;
Ism__setFileAttributes(Ism__setAttrRequest *in, Ism__accessResponse *out) ;
Ism__deleteFileFsLike(Ism__deleteRequestFs *in, int *errcode) ;

Ism__getUserGroup(Ism__userGroupRequest *in, Ism__userGroupResponse *out) ;


// methods used for caches management
Ism__getCacheStorageInfo(struct Ism__cacheInfoRequest { } *in, Ism__cacheInfoResponse *out) ;

// Server Version to discriminate IF names
Ism__getServerVersion( struct Ism__serverVersionRequest { } *in, int *serverVersion );

} //namespace
