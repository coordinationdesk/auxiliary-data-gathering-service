// PRQA S 1050 EOF
/*
	Copyright 1995-2022, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA, Inc.

	$Prod: A.C.S. libIsmSoap $
        
*/ 

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor" // PRQA S 1040 3
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wold-style-cast"

#include <acsIsmService.h>

#pragma GCC diagnostic pop
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs) 

using namespace std;

int IsmService::uploadSingleFile(Ism__uploadSingleFileRequest *in, Ism__uploadResponse *out)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::notifyUploadCompleted(Ism__notifyUploadRequest *in, struct Ism__notifyCompletedUploadResponse *out)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}


int IsmService::notifyUploading(Ism__notifyUploadRequest *in, struct Ism__notifyUploadingResponse  *out) 
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}


int IsmService::deleteFile(Ism__deleteRequest *in, struct Ism__deleteResponse  *out) 
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}


int IsmService::getFileId(Ism__getFileIdRequest *in, Ism__getFileIdResponse *out) 
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}


// download  Methods

//**************** Valid until Server Version 2.x
int IsmService::downloadFile(Ism__DownloadFileRequest *in, Ism__DownloadFileResponse *out) 
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::notifyDownloading(Ism__DownloadFileRequest *in, Ism__notifyDownloadResponse *out) 
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::notifyDownloadCompleted(Ism__DownloadFileRequest *in, Ism__notifyDownloadResponse *out) 
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

//********************************************


//**************** Valid from Server Version 3.0 on
int IsmService::downloadFile_2(Ism__DownloadFileRequest_2 *in, Ism__DownloadFileResponse_2 *out) 
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::notifyDownloading_2(Ism__notifyDownloadRequest *in, Ism__notifyDownloadResponse *out) 
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::notifyDownloadCompleted_2(Ism__notifyDownloadRequest *in, Ism__notifyDownloadResponse *out) 
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

//********************************************

int IsmService::abortDownload(Ism__DownloadFileRequest *in, Ism__notifyDownloadResponse *out) 
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}


int IsmService::bookFile(Ism__bookFileRequest *in, Ism__bookFileResponse *out)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::giveupFileRequest(Ism__giveupRequest *in, struct Ism__giveupResponse  *out) 
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::isFileOnCache(Ism__onCacheRequest *, enum Ism__onCacheResponseCodeEnum *)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::putFileOnCache(Ism__onCacheRequest *, enum Ism__onCacheResponseCodeEnum *)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::releaseCacheFile(Ism__onCacheRequest *, struct Ism__releaseCacheResponse *)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::deleteFileFsLike(acs::Ism__deleteRequestFs*, int*)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::getDirectoryTree(acs::Ism__directoryTreeRequest*, acs::Ism__directoryTreeResponse*)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::notifyDirUploadCompleted(acs::Ism__notifyDirUploadRequest*, acs::Ism__notifyDirCompletedUploadResponse*)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::notifyDirUploading(acs::Ism__notifyDirUploadRequest*, acs::Ism__notifyDirUploadingResponse*)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::uploadDirectory(acs::Ism__uploadDirectoryRequest*, acs::Ism__dirUploadResponse*)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::getFileAttributes(acs::Ism__getAttrRequest*, acs::Ism__getAttrResponse*)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::setFileAttributes(acs::Ism__setAttrRequest*, acs::Ism__accessResponse*)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::notifyMultiFilesDownloading(acs::Ism__downloadMultiFilesRequest*, acs::Ism__notifyDownloadResponse*)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::getList(acs::Ism__listRequest*, acs::Ism__listResponse*)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::getUserGroup(acs::Ism__userGroupRequest*, acs::Ism__userGroupResponse*)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::downloadDirectory(acs::Ism__downloadDirRequest*, acs::Ism__downloadMultiFilesResponse*)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::notifyMultiFilesDownloadCompleted(acs::Ism__downloadMultiFilesRequest*, acs::Ism__notifyDownloadResponse*)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::getCacheStorageInfo(acs::Ism__cacheInfoRequest*, acs::Ism__cacheInfoResponse*)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::abortMultiFilesDownload(acs::Ism__downloadMultiFilesRequest*, acs::Ism__notifyDownloadResponse*)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::access(acs::Ism__accessRequest*, acs::Ism__accessResponse*)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

#ifdef EXPRIVIA_ISM_USE_AWS_S3
int IsmService::getCacheConfiguration(char *, Ism__cacheConfiguration *) 
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int IsmService::getCacheConfigurationAdditionalParms(char *, Ism__cacheConfigurationAdditionalParms *) 
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

#endif

int IsmService::getServerVersion( acs::Ism__serverVersionRequest *, int * )
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

_ACS_END_NAMESPACE
