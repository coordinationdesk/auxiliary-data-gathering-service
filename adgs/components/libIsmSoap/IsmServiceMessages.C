// PRQA S 1050 EOF
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
	Revision 1.25  2017/06/23 09:21:28  lucio.pulvirenti
	S3MPM-873: added new structs fields in methods.
	
	Revision 1.24  2017/06/13 13:59:58  lucio.pulvirenti
	S3MPM-873: string user replaced with unsigned int32 userId and groupId added to Ism__listRequest, Ism__accessRequest, Ism__fileAttributes, Ism__getAttrRequest classes.
	boolean isDir removed from Ism__accessRequest class.
	
	Revision 1.23  2017/05/22 10:03:19  lucio.pulvirenti
	S3MPM-873: Ism__deleteRequest: useless fields removed.
	
	Revision 1.22  2017/05/18 10:33:59  lucio.pulvirenti
	Ism__getAttrRequest class added. Ism__listRequest and Ism__listResponse modified.
	
	Revision 1.21  2017/03/15 13:10:38  lucio.pulvirenti
	S3MPM-873: Ism__listRequest: user attribute removed. Ism__fileAttributes and Ism__getAttrResponse classes added.
	
	Revision 1.20  2017/03/08 11:28:03  lucio.pulvirenti
	S3MPM-873: methods to print Ism__accessRequest and Ism__accessResponse structs added.
	
	Revision 1.19  2017/02/24 14:08:56  lucio.pulvirenti
	S3MPM-873: Added classes and method to perform ls on given path.
	
	Revision 1.18  2016/01/15 09:59:25  lucio.pulvirenti
	APF-270: totalItemsSize attribute turned into a pointer to guarantee compatibility between new/old Server version and old/new Client versions.
	
	Revision 1.17  2016/01/14 16:31:56  lucio.pulvirenti
	APF-270: totalItemsSize attribute (total size of directory files) added to Ism__dirDefinition and Ism__notifyDirUploadRequest classes.
	
	Revision 1.16  2014/06/27 09:13:03  marpas
	qa warnings fixed
	
	Revision 1.15  2011/03/07 17:09:22  lucio.pulvirenti
	Ism__downloadMultiFilesRequest: jobId added
	
	Revision 1.14  2011/03/03 17:10:56  lucio.pulvirenti
	work in progress
	
	Revision 1.13  2011/02/23 11:19:15  lucio.pulvirenti
	Output of Ism__deleteRequest class added
	
	Revision 1.12  2011/02/22 17:00:37  lucio.pulvirenti
	work in progress
	
	Revision 1.11  2011/02/16 11:14:18  lucio.pulvirenti
	output of new structures added
	
	Revision 1.10  2011/02/11 14:53:55  lucio.pulvirenti
	work in progress
	
	Revision 1.9  2011/02/02 16:28:04  lucio.pulvirenti
	work in progress
	
	Revision 1.8  2011/02/01 15:56:04  lucio.pulvirenti
	work in progress
	
	Revision 1.7  2011/01/27 17:44:59  lucio.pulvirenti
	output of new classes added
	
	Revision 1.6  2011/01/25 18:02:22  lucio.pulvirenti
	work in progress
	
	Revision 1.5  2011/01/21 18:40:40  lucio.pulvirenti
	work in progress
	
	Revision 1.4  2011/01/20 17:40:55  lucio.pulvirenti
	work in progress
	
	Revision 1.3  2008/06/16 09:42:44  paoscu
	missing header
	
	Revision 1.2  2008/05/07 17:43:29  paoscu
	Interfaces changed
	
	Revision 1.1  2008/05/07 13:46:35  paoscu
	Work in progress
	
	Revision 1.2  2008/05/05 17:30:23  paoscu
	Work in progress on interface definition
	
	Revision 1.1.1.1  2008/05/05 16:54:47  paoscu
	Imported into CVS
	
	
        
*/




#include <IsmSoapV.h>

#include <IsmServiceMessages.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


namespace {
    IsmSoapV Version;
}


ostream &operator<<(ostream &s, const VectorOfInts & v)
{
	for( size_t i = 0 ; i<v.numbers.size() ; ++i )
	{
		s << "VectorOfInts.number[" << i << "] = " << v.numbers[i] << '\n' ;
	}
	return s ; // PRQA S 4028
}



ostream &operator<<(ostream &s, const VectorOfStrings &v) 
{
	for( size_t i = 0 ; i<  v.strings.size() ; ++i )
	{
		s << "VectorOfStrings.string[" << i << "] = " << v.strings[i].c_str() << '\n' ;
	}
	return s ; // PRQA S 4028
}


ostream &operator<<(ostream &s, const VectorOfLongLongs & v)
{
	for( size_t i = 0 ; i< v.numbers.size() ; ++i )
	{
		s<< "VectorOfLongLongs.number[" << i << "] = " << v.numbers[i] << '\n' ;
	}
	return s ; // PRQA S 4028
}



ostream &operator<<(ostream &s, const VectorOfBooleans &v) 
{
    ostream::fmtflags fl = s.flags() ;
	for( size_t i = 0 ; i< v.b.size() ; ++i )
	{
		s<< "VectorOfBooleans.b[" << i << "] = "
		     << boolalpha << v.b[i] << '\n' ;
	}
    s.flags(fl) ; // reset flags (boolalpha)
	return s ; // PRQA S 4028
}


ostream &operator<<(ostream &s, const Ism__fileDefinition &fd) 
{
	s 	<< "fileId = " << fd.fileId << '\n' 
	    << "filePath = " << fd.filePath << '\n' 
		<< "isDir  = "   << fd.isDir  << '\n' 
		<< "size  = "	 << fd.size  << '\n' 
		<< "user  = "	<< fd.user.c_str()  << '\n' 
		<< "group  = "	 << fd.group.c_str()  << '\n' 
		<< "permissions  = " << fd.permissions  << '\n' 
		<< "fileClass  = "   << fd.fileClass  << '\n'
		<< "packaging algo  = "   << fd.packagingAlgo  << '\n' ;

		s << fd.customKeys << '\n' ;
		s << fd.customValues << '\n' ;

	return s ; // PRQA S 4028
}


ostream &operator<<(ostream &s, const Ism__uploadSingleFileRequest &ur)
{
    ostream::fmtflags fl = s.flags() ;

	s << "recursive = " << boolalpha << ur.recursive << '\n' ;
	s << "checkPermissions = " << boolalpha << ur.checkPermissions << '\n' ;
    s.flags(fl) ; // reset flags (boolalpha)
	s << *(ur.file) << '\n';
#ifdef EXPRIVIA_ISM_USE_AWS_S3        	
	s << *(ur.cacheConfiguration) << '\n';
#endif
	return s ; // PRQA S 4028
}


ostream &operator<<(ostream &s, const Ism__uploadResponse &ur)
{
	s << "uploadUrl = " << ur.uploadUrl << '\n' ;
	s << *(ur.file) << '\n' ;
#ifdef EXPRIVIA_ISM_USE_AWS_S3        	
	s << *(ur.cacheConfiguration) << '\n';
#endif

	return s ; // PRQA S 4028
} 

ostream &operator<<(ostream &s, const Ism__DownloadFileRequest &dr)
{
	s 	<< "fileId = " 				<< dr.fileId 							<< '\n'
	 	<< "user = \"" 				<< dr.user 								<< "\"\n"
		<< "group = \"" 			<< dr.group 							<< "\"\n"
		<< "requestId = " 			<< dr.requestId 						<< '\n' 
		<< "updateRequestTime = " 	<< boolalpha << dr.updateRequestTime 	<< '\n' 
		<< "checkPermissions = " 	<< dr.checkPermissions 					<< '\n' ; 

	return s ; // PRQA S 4028
} 

ostream &operator<<(ostream &s, const Ism__DownloadFileRequest_2 &dr)
{
	s 	<< "fileId = " 				<< dr.fileId 							<< '\n'
	 	<< "user = \"" 				<< dr.user 								<< "\"\n"
		<< "group = \"" 			<< dr.group 							<< "\"\n"
		<< "requestId = " 			<< dr.requestId 						<< '\n' 
		<< "updateRequestTime = " 	<< boolalpha << dr.updateRequestTime 	<< '\n' 
		<< "checkPermissions = " 	<< dr.checkPermissions 					<< '\n' 
		<< "clientReference = \"" 	<< dr.clientReference 					<< "\"\n" ;

	return s ; // PRQA S 4028
} 

ostream &operator<<(ostream &s, const Ism__notifyDownloadRequest & r)
{
	s << " [file_id = " << r.fileId << ", " ;
	s << "cache_id = " << r.cacheId << "] " ;

	return s ; // PRQA S 4028
} 



ostream &operator<<(ostream &s, const Ism__dirDefinition &dd) 
{
	s 	<< "filePath = " << dd.filePath << '\n' 
		<< "user  = "	<< dd.user.c_str()  << '\n' 
		<< "group  = "	 << dd.group.c_str()  << '\n' 
		<< "permissions  = " << dd.permissions  << '\n' 
		<< "fileClass  = "   << dd.fileClass  << '\n' ;
	if( dd.totalItemsSize ) {
		s << "totalItemsSize = " << *(dd.totalItemsSize) << '\n' ;
	}

		s << dd.customKeys << '\n' ;
		s << dd.customValues << '\n' ;
		
		
		for( size_t i = 0 ; i< dd.fullpaths.size() ; ++i ) {
			 s<< "fullpaths[" << i << "] = \"" << dd.fullpaths[i] << "\"\n" ;
        }
		for( size_t i = 0 ; i< dd.sizes.size() ; ++i ) {
			 s<< "sizes[" << i << "] = " << dd.sizes[i] << '\n' ;
        }
        
    ostream::fmtflags fl = s.flags() ;
		for( size_t i = 0 ; i< dd.isDir.size() ; ++i ) {
			 s<< "isDir[" << i << "] = " << boolalpha << dd.isDir[i] << '\n' ;
        }
    s.flags(fl) ; // reset flags (boolalpha)
        
		for( size_t i = 0 ; i< dd.filesIds.size() ; ++i ) {
			 s<< "filesIds[" << i << "] = " << dd.filesIds[i] << '\n' ;
        }
		

	return s ; // PRQA S 4028
}



ostream &operator<<(ostream &s, const Ism__uploadDirectoryRequest &ur)
{
    ostream::fmtflags fl = s.flags() ;
	s << "recursive = " << boolalpha << ur.recursive << '\n' ;
	s << "checkPermissions = " << boolalpha << ur.checkPermissions << '\n' ;
	s << "extendDirPermissions = " << boolalpha << ur.extendDirPermissions << '\n' ;
	s << *(ur.dir) << '\n' ;
    s.flags(fl) ; // reset flags (boolalpha)

	return s ; // PRQA S 4028
}


ostream &operator<<(ostream &s, const Ism__dirUploadResponse &ur)
{
	s << *(ur.dir) << '\n' ;

	for( size_t i = 0 ; i< ur.uploadUrls.size() ; ++i ) {
		 s<< "uploadUrls[" << i << "] = \"" << ur.uploadUrls[i] << "\"\n" ;
    }
	for( size_t i = 0 ; i< ur.onlinesIds.size() ; ++i ) {
		 s<< "onlinesIds[" << i << "] = " << ur.onlinesIds[i] << '\n' ;
    }
	
	return s ; // PRQA S 4028
} 


ostream &operator<<(ostream &s, const Ism__notifyDirUploadRequest &ur)
{
	for( size_t i = 0 ; i< ur.filesIds.size() ; ++i ) {
		 s<< "filesIds[" << i << "] = " << ur.filesIds[i] << '\n' ;
    }
	for( size_t i = 0 ; i< ur.onlinesIds.size() ; ++i ) {
		 s<< "onlinesIds[" << i << "] = " << ur.onlinesIds[i] << '\n' ;
    }
	
	s << "user = " << ur.user << '\n';
	if( ur.totalItemsSize ) {
		s << "totalItemsSize = " << *(ur.totalItemsSize) << '\n' ;
	}
	s << "checkPermissions = " << boolalpha << ur.checkPermissions << '\n';
	
	
	return s ; // PRQA S 4028
} 


ostream &operator<<(ostream &s, const Ism__directoryTreeResponse &dr)
{
	
	for( size_t i = 0 ; i< dr.filePaths.size() ; ++i )  {
		 s<< "filePaths[" << i << "] = \"" << dr.filePaths[i] << "\"\n" ;
    }
	for( size_t i = 0 ; i< dr.filesIds.size() ; ++i ) {
		 s<< "filesIds[" << i << "] = " << dr.filesIds[i] << '\n' ;
    }
	for( size_t i = 0 ; i< dr.isDir.size() ; ++i ) {
		 s<< "isDir[" << i << "] = " << boolalpha << dr.isDir[i] << noboolalpha << '\n' ;
    }
	
	return s ; // PRQA S 4028
} 


ostream &operator<<(ostream &s, const Ism__downloadMultiFilesRequest &dr) 
{

	for( size_t i = 0 ; i< dr.filesIds.size() ; ++i ) {
		 s<< "filesIds[" << i << "] = " << dr.filesIds[i] << '\n' ;
    }
	s << "user = " << dr.user << '\n';
	s << "group = " << dr.group << '\n';
	for( size_t i = 0 ; i< dr.requestsIds.size() ; ++i ) {
		 s<< "requestsIds[" << i << "] = " << dr.requestsIds[i] << '\n' ;
    }
	s << "jobId = " << dr.jobId << '\n';
	s << "updateRequestTime = " << boolalpha << dr.updateRequestTime << noboolalpha << '\n' ;
	s << "filesDisappeared = " << boolalpha << dr.filesDisappeared << noboolalpha << '\n' ;
	s << "checkPermissions = " << boolalpha << dr.checkPermissions << noboolalpha << '\n' ;
	
	return s ; // PRQA S 4028
}


ostream &operator<<(ostream &s, const Ism__downloadDirRequest &dr)
{
	
	for( size_t i = 0 ; i< dr.dirsIds.size() ; ++i ) {
		 s<< "dirsIds[" << i << "] = " << dr.dirsIds[i] << '\n' ;
    }
	s << *(dr.filesRequest) << '\n' ;	 
	
	return s ; // PRQA S 4028
} 



ostream &operator<<(ostream &s, const Ism__downloadMultiFilesResponse &dr)
{
		
	for( size_t i = 0 ; i< dr.downloadUrls.size() ; ++i ) {
		 s<< "downloadUrls[" << i << "] = \"" << dr.downloadUrls[i] << "\"\n" ;
    }
	for( size_t i = 0 ; i< dr.requestsIds.size() ; ++i ) {
		 s<< "requestsIds[" << i << "] = " << dr.requestsIds[i] << '\n' ;
    }
		 
	s << "downloadCompleted = " << boolalpha << dr.downloadCompleted << noboolalpha << '\n' ;
	s << "jobId = " << dr.jobId << '\n';
	
	return s ; // PRQA S 4028
} 

ostream &operator<<(ostream &s, const Ism__deleteRequest &dr)
{

	s 	<< "filePath = \"" << dr.filePath << "\"\n"
		<< "user = " << dr.user << '\n';	
	
	return s ; // PRQA S 4028
} 

ostream &operator<<(ostream &s, const Ism__listRequest &lr)
{

	s 	<< "path = \"" << lr.path << "\"\n"
		<< "userId = " << lr.userId << '\n'	
		<< "groupId = " << lr.groupId << '\n'	
		<< "offset = " << lr.offset << '\n'	
		<< "entries = " << lr.entries << '\n';	
	
	return s ; // PRQA S 4028
} 

ostream &operator<<(ostream &s, const Ism__listResponse &lr)
{

	s << "returnCode = " << lr.returnCode << '\n';
	{
		size_t pathsSize = lr.paths.size();
		for( size_t i = 0; i < pathsSize; ++i ) {
			s << "path[" << i << "] = \"" << lr.paths[i] << "\"\n";
		}
	}
	s 	<< "offset = " << lr.offset << '\n'	
		<< "errno = " << lr.errnumber << '\n';	
	
	return s ; // PRQA S 4028
} 

ostream &operator<<(ostream &s, const Ism__accessRequest &ar)
{
	s 	<< "path = \"" << ar.path << "\"\n"
		<< "userId = " << ar.userId << '\n'	
		<< "groupId = " << ar.groupId << '\n'	
		<< "mode = " << ar.mode << '\n'	;
	
	return s ; // PRQA S 4028
} 

ostream &operator<<(ostream &s, const Ism__accessResponse &ar)
{
	s 	<< "returnCode = " << ar.returnCode << '\n'
		<< "errno = " << ar.errnumber << '\n';
	
	return s ; // PRQA S 4028
} 

ostream &operator<<(ostream &s, const Ism__fileAttributes &attr)
{
	s 	<< "fileId = " << attr.fileId << '\n' 
	    << "filePath = \"" << attr.filePath << "\"\n" 
		<< "isDir  = "   << boolalpha << attr.isDir  << '\n' 
		<< "size  = "	 << attr.size  << '\n' 
		<< "userId  = "	<< attr.userId  << '\n' 
		<< "groupId  = " << attr.groupId  << '\n' 
		<< "permissions  = " << attr.permissions  << '\n' 
		<< "creationTime = " 		<< attr.creationTime << '\n'
		<< "modificationTime = " 	<< attr.modificationTime << '\n'
		<< "lastAccessTime = " 		<< attr.lastAccessTime << '\n';
	
	return s ; // PRQA S 4028
} 


ostream &operator<<(ostream &s, const Ism__getAttrRequest &ar)
{
	s 	<< "path = \"" << ar.path << "\"\n"
		<< "userId = " << ar.userId << '\n'	
		<< "groupId = " << ar.groupId << '\n';	
	
	return s ; // PRQA S 4028
} 


ostream &operator<<(ostream &s, const Ism__getAttrResponse &ar)
{
	s 	<< "returnCode = " << ar.returnCode << '\n'
		<< "errno = " << ar.errnumber << '\n'
		<< ar.attributes ;	 
	
	return s ; // PRQA S 4028
} 


ostream &operator<<(ostream &s, const Ism__setAttrRequest &ar)
{
	s 	<< "path = \"" << ar.path << "\"\n"
		<< "userId = " << ar.userId << '\n'	
		<< "groupId = " << ar.groupId << '\n'
		<< ar.attributes ;	 
	
	return s ; // PRQA S 4028
} 


ostream &operator<<(ostream &s, const Ism__deleteRequestFs &ar)
{
	s 	<< "path = \"" << ar.filePath << "\"\n"
		<< "userId = " << ar.userId << '\n'	
		<< "groupId = " << ar.groupId << '\n'
		<< "recursive = " << boolalpha << ar.recursive << '\n';	
	
	return s ; // PRQA S 4028
} 


ostream &operator<<(ostream &s, const Ism__cacheInfo& c)
{
	s	<< "cacheName = \"" << c.cacheName <<  "\"\n"
		<< "fileClass = \"" << c.fileClass <<  "\"\n"
		<< "freeSize = " << c.freeSize <<  " [MB]\n";
	
	return s ; // PRQA S 4028
}

ostream &operator<<(ostream &s, const Ism__cacheInfoResponse& cr) 
{
	size_t cSize = cr.cacheInfo.size();
	for( size_t i = 0; i < cSize; ++i ) {
		s << "cacheInfo[" << i << "]\n" << cr.cacheInfo[i] << '\n';
	}
	return s ; // PRQA S 4028
}

#ifdef EXPRIVIA_ISM_USE_AWS_S3        	
ostream &operator<<(ostream &s, const Ism__cacheConfiguration &v)
{
	s	<< "configurationKey = \"" 	<< v.configurationKey 				<< "\"\n"
	  	<< "provider = \"" 			<< v.provider 						<< "\"\n"
		<< "key_ak = \"" 			<< v.key_ak  						<< "\"\n"
		<< "key_sk = \"" 			<< v.key_sk 						<< "\"\n"
		<< "region = \"" 			<< v.region 						<< "\"\n"
		<< "endpoint = \"" 			<< v.endpoint 						<< "\"\n"
		<< "endpointIsSecure = " 	<< boolalpha << v.endpointIsSecure 	<< "\n"
		<< "options = \"" 			<< v.options 						<< "\"\n";
	
	return s ; // PRQA S 4028
} 

ostream &operator<<(ostream &s, const Ism__cacheConfigurationAdditionalParms &v)
{
	s	<< "downloadPoolSize=" 		<< v.downloadPoolSize 				<< '\n'
		<< "downloadBufferSize=" 	<< v.downloadBufferSize 			<< '\n'
		<< "uploadPoolSize=" 		<< v.uploadPoolSize 				<< '\n'
		<< "uploadBufferSize=" 		<< v.uploadBufferSize 				<< '\n';
	
	return s ; // PRQA S 4028
} 

#endif

ostream &operator<<(ostream &s, const Ism__giveupRequest &dr)
{

	s 	<< "filePath = \"" 			<< dr.filePath 			<< "\"\n"
		<< "user = \"" 				<< dr.user 				<< "\"\n"
		<< "clientReference = \"" 	<< dr.clientReference 	<< "\"\n" ;
	
	return s ; // PRQA S 4028
} 


exostream &operator<<(exostream &s, const VectorOfInts & v) // PRQA S 4222
{
	for( size_t i = 0 ; i< v.numbers.size() ; ++i )
	{
		s<< "VectorOfInts.number[" << i << "] = " << v.numbers[i] << '\n' ;
	}
	return s ; // PRQA S 4028
}



exostream &operator<<(exostream &s, const VectorOfStrings &v) // PRQA S 4222 
{
	for( size_t i = 0 ; i< v.strings.size() ; ++i )
	{
		s<< "VectorOfStrings.string[" << i << "] = " << v.strings[i].c_str() << '\n' ;
	}
	return s ; // PRQA S 4028
}


exostream &operator<<(exostream &s, const VectorOfLongLongs & v) // PRQA S 4222
{
	for( size_t i = 0 ; i< v.numbers.size() ; ++i )
	{
		s<< "VectorOfLongLongs.number[" << i << "] = " << v.numbers[i] << '\n' ;
	}
	return s ; // PRQA S 4028
}



exostream &operator<<(exostream &s, const VectorOfBooleans &v)  // PRQA S 4222
{
	for( size_t i = 0 ; i< v.b.size() ; ++i )
	{
		s<< "VectorOfBooleans.b[" << i << "] = " << boolalpha << v.b[i] << noboolalpha << '\n' ;
	}
	return s ; // PRQA S 4028
}

exostream &operator<<(exostream &s, const Ism__fileDefinition &fd)  // PRQA S 4222
{
	s 	<< "fileId = " << fd.fileId << '\n' 
	    << "filePath = " << fd.filePath << '\n' 
		<< "isDir  = "   << fd.isDir  << '\n' 
		<< "size  = "	 << fd.size  << '\n' 
		<< "user  = "	<< fd.user  << '\n' 
		<< "group  = "	 << fd.group  << '\n' 
		<< "permissions  = " << fd.permissions  << '\n' 
		<< "fileClass  = "   << fd.fileClass  << '\n' 
		<< "packaging algo  = "   << fd.packagingAlgo  << '\n' ;

		s << fd.customKeys << '\n' ;
		s << fd.customValues << '\n' ;

	return s ; // PRQA S 4028
}


exostream &operator<<(exostream &s, const Ism__uploadSingleFileRequest &ur) // PRQA S 4222
{
	s << "recursive = " << boolalpha << ur.recursive << noboolalpha << '\n' ;
	s << "checkPermissions = " << boolalpha << ur.checkPermissions << '\n' ;
	s << *(ur.file) << '\n' ;
#ifdef EXPRIVIA_ISM_USE_AWS_S3        	
	s << *(ur.cacheConfiguration) << '\n';
#endif

	return s ; // PRQA S 4028
}




exostream &operator<<(exostream &s, const Ism__uploadResponse &ur) // PRQA S 4222
{
	s << "uploadUrl = " << ur.uploadUrl << '\n' ;
	s << *(ur.file) << '\n' ;
#ifdef EXPRIVIA_ISM_USE_AWS_S3        	
	s << *(ur.cacheConfiguration) << '\n';
#endif

	return s ; // PRQA S 4028
} 

exostream &operator<<(exostream &s, const Ism__DownloadFileRequest &dr)
{
	s 	<< "fileId = " 				<< dr.fileId 							<< '\n'
	 	<< "user = \"" 				<< dr.user 								<< "\"\n"
		<< "group = \"" 			<< dr.group 							<< "\"\n"
		<< "requestId = " 			<< dr.requestId 						<< '\n' 
		<< "updateRequestTime = " 	<< boolalpha << dr.updateRequestTime 	<< '\n' 
		<< "checkPermissions = " 	<< dr.checkPermissions 					<< '\n' ; 

	return s ; // PRQA S 4028
} 

exostream &operator<<(exostream &s, const Ism__DownloadFileRequest_2 &dr)
{
	s 	<< "fileId = " 				<< dr.fileId 							<< '\n'
	 	<< "user = \"" 				<< dr.user 								<< "\"\n"
		<< "group = \"" 			<< dr.group 							<< "\"\n"
		<< "requestId = " 			<< dr.requestId 						<< '\n' 
		<< "updateRequestTime = " 	<< boolalpha << dr.updateRequestTime 	<< '\n' 
		<< "checkPermissions = " 	<< dr.checkPermissions 					<< '\n'  
		<< "clientReference = \"" 	<< dr.clientReference 					<< "\"\n" ;

	return s ; // PRQA S 4028
} 

exostream &operator<<(exostream &s, const Ism__notifyDownloadRequest & r)
{
	s << " [file_id = " << r.fileId << ", " ;
	s << "cache_id = " << r.cacheId << "] " ;


	return s ; // PRQA S 4028
} 



exostream &operator<<(exostream &s, const Ism__dirDefinition &dd)  // PRQA S 4222
{
	s 	<< "filePath = " << dd.filePath << '\n' 
		<< "user  = "	<< dd.user.c_str()  << '\n' 
		<< "group  = "	 << dd.group.c_str()  << '\n' 
		<< "permissions  = " << dd.permissions  << '\n' 
		<< "fileClass  = "   << dd.fileClass  << '\n';
	if( dd.totalItemsSize ) {
		s << "totalItemsSize = " << *(dd.totalItemsSize) << '\n' ;
	}

		s << dd.customKeys << '\n' ;
		s << dd.customValues << '\n' ;
		
		
		for( size_t i = 0 ; i< dd.fullpaths.size() ; ++i ) {
			 s<< "fullpaths[" << i << "] = \"" << dd.fullpaths[i] << "\"\n" ;
        }
		for( size_t i = 0 ; i< dd.sizes.size() ; ++i ) {
			 s<< "sizes[" << i << "] = " << dd.sizes[i] << '\n' ;
        }
		for( size_t i = 0 ; i< dd.isDir.size() ; ++i ) {
			 s<< "isDir[" << i << "] = " << boolalpha << dd.isDir[i] << noboolalpha << '\n' ;
        }
		for( size_t i = 0 ; i< dd.filesIds.size() ; ++i ) {
			 s<< "filesIds[" << i << "] = " << dd.filesIds[i] << '\n' ;
        }
		

	return s ; // PRQA S 4028
}



exostream &operator<<(exostream &s, const Ism__uploadDirectoryRequest &ur) // PRQA S 4222
{
	s << "recursive = " << boolalpha << ur.recursive << noboolalpha << '\n' ;
	s << "checkPermissions = " << boolalpha << ur.checkPermissions << '\n' ;
	s << "extendDirPermissions = " << boolalpha << ur.extendDirPermissions << '\n' ;
	s << *(ur.dir) << '\n' ;

	return s ; // PRQA S 4028
}


exostream &operator<<(exostream &s, const Ism__dirUploadResponse &ur) // PRQA S 4222
{
	s << *(ur.dir) << '\n' ;


	for( size_t i = 0 ; i< ur.uploadUrls.size() ; ++i ) {
		 s<< "uploadUrls[" << i << "] = \"" << ur.uploadUrls[i] << "\"\n" ;
    }
	for( size_t i = 0 ; i< ur.onlinesIds.size() ; ++i ) {
		 s<< "onlinesIds[" << i << "] = " << ur.onlinesIds[i] << '\n' ;
    }
	
	return s ; // PRQA S 4028
} 


exostream &operator<<(exostream &s, const Ism__notifyDirUploadRequest &ur) // PRQA S 4222
{
	for( size_t i = 0 ; i< ur.filesIds.size() ; ++i ) {
		 s<< "filesIds[" << i << "] = " << ur.filesIds[i] << '\n' ;
    }
	for( size_t i = 0 ; i< ur.onlinesIds.size() ; ++i ) {
		 s<< "onlinesIds[" << i << "] = " << ur.onlinesIds[i] << '\n' ;
    }
	
	s << "user = " << ur.user << '\n';
	if( ur.totalItemsSize ) {
		s << "totalItemsSize = " << *(ur.totalItemsSize) << '\n' ;
	}
	s << "checkPermissions = " << boolalpha << ur.checkPermissions << '\n';
	
	return s ; // PRQA S 4028
} 


exostream &operator<<(exostream &s, const Ism__directoryTreeResponse &dr) // PRQA S 4222
{
	
	for( size_t i = 0 ; i< dr.filePaths.size() ; ++i ) {
		 s<< "filePaths[" << i << "] = \"" << dr.filePaths[i] << "\"\n" ;
    }
	for( size_t i = 0 ; i< dr.filesIds.size() ; ++i ) {
		 s<< "filesIds[" << i << "] = " << dr.filesIds[i] << '\n' ;
    }
	for( size_t i = 0 ; i< dr.isDir.size() ; ++i ) {
		 s<< "isDir[" << i << "] = " << boolalpha << dr.isDir[i] << noboolalpha << '\n' ;
    }
	
	return s ; // PRQA S 4028
} 


exostream &operator<<(exostream &s, const Ism__downloadMultiFilesRequest &dr)  // PRQA S 4222
{

	for( size_t i = 0 ; i< dr.filesIds.size() ; ++i ) {
		 s<< "filesIds[" << i << "] = " << dr.filesIds[i] << '\n' ;
    }
	s << "user = " << dr.user << '\n';
	s << "group = " << dr.group << '\n';
	for( size_t i = 0 ; i< dr.requestsIds.size() ; ++i ) {
		 s<< "requestsIds[" << i << "] = " << dr.requestsIds[i] << '\n' ;
    }
	s << "jobId = " << dr.jobId << '\n';
	s << "updateRequestTime = " << boolalpha << dr.updateRequestTime << noboolalpha << '\n' ;
	s << "filesDisappeared = " << boolalpha << dr.filesDisappeared << noboolalpha << '\n' ;
	s << "checkPermissions = " << boolalpha << dr.checkPermissions << noboolalpha << '\n' ;
	
	return s ; // PRQA S 4028
}


exostream &operator<<(exostream &s, const Ism__downloadDirRequest &dr) // PRQA S 4222
{
	
	for( size_t i = 0 ; i< dr.dirsIds.size() ; ++i ) {
		 s<< "dirsIds[" << i << "] = " << dr.dirsIds[i] << '\n' ;
    }
	s << *(dr.filesRequest) << '\n' ;	 
	
	return s ; // PRQA S 4028
} 



exostream &operator<<(exostream &s, const Ism__downloadMultiFilesResponse &dr) // PRQA S 4222
{
		
	for( size_t i = 0 ; i< dr.downloadUrls.size() ; ++i ) {
		 s<< "downloadUrls[" << i << "] = \"" << dr.downloadUrls[i] << "\"\n" ;
    }
	for( size_t i = 0 ; i< dr.requestsIds.size() ; ++i ) {
		 s<< "requestsIds[" << i << "] = " << dr.requestsIds[i] << '\n' ;
    }
		 
	s << "downloadCompleted = " << boolalpha << dr.downloadCompleted << noboolalpha << '\n' ;
	s << "jobId = " << dr.jobId << '\n';
	
	return s ; // PRQA S 4028
} 



exostream &operator<<(exostream &s, const Ism__deleteRequest &dr) // PRQA S 4222
{

	s 	<< "filePath = \"" << dr.filePath << "\"\n"
		<< "user = " << dr.user << '\n' ;
	
	return s ; // PRQA S 4028
} 

exostream &operator<<(exostream &s, const Ism__listRequest &lr)
{

	s 	<< "path = \"" << lr.path << "\"\n"
		<< "userId = " << lr.userId << '\n'	
		<< "groupId = " << lr.groupId << '\n'	
		<< "offset = " << lr.offset << '\n'	
		<< "entries = " << lr.entries << '\n';	
	
	return s ; // PRQA S 4028
} 

exostream &operator<<(exostream &s, const Ism__listResponse &lr)
{

	s << "returnCode = " << lr.returnCode << '\n';
	{
		size_t pathsSize = lr.paths.size();
		for( size_t i = 0; i < pathsSize; ++i ) {
			s << "path[" << i << "] = \"" << lr.paths[i] << "\"\n";
		}
	}
	s 	<< "offset = " << lr.offset << '\n'	
		<< "errno = " << lr.errnumber << '\n';	
	
	return s ; // PRQA S 4028
} 

exostream &operator<<(exostream &s, const Ism__accessRequest &ar)
{
	s 	<< "path = \"" << ar.path << "\"\n"
		<< "userId = " << ar.userId << '\n'	
		<< "groupId = " << ar.groupId << '\n'	
		<< "mode = " << ar.mode << '\n'	;
	
	return s ; // PRQA S 4028
} 

exostream &operator<<(exostream &s, const Ism__accessResponse &ar)
{
	s 	<< "returnCode = " << ar.returnCode << '\n'
		<< "errno = " << ar.errnumber << '\n';
	
	return s ; // PRQA S 4028
} 

exostream &operator<<(exostream &s, const Ism__fileAttributes &attr)
{
	s 	<< "fileId = " << attr.fileId << '\n' 
	    << "filePath = \"" << attr.filePath << "\"\n" 
		<< "isDir  = "   << boolalpha << attr.isDir  << '\n' 
		<< "size  = "	 << attr.size  << '\n' 
		<< "userId  = "	<< attr.userId  << '\n' 
		<< "groupId  = "	 << attr.groupId  << '\n' 
		<< "permissions  = " << attr.permissions  << '\n' 
		<< "creationTime = " 		<< attr.creationTime << '\n'
		<< "modificationTime = " 	<< attr.modificationTime << '\n'
		<< "lastAccessTime = " 		<< attr.lastAccessTime << '\n';
	
	return s ; // PRQA S 4028
} 

exostream &operator<<(exostream &s, const Ism__getAttrRequest &ar)
{
	s 	<< "path = \"" << ar.path << "\"\n"
		<< "userId = " << ar.userId << '\n'	
		<< "groupId = " << ar.groupId << '\n';	
	
	return s ; // PRQA S 4028
} 


exostream &operator<<(exostream &s, const Ism__getAttrResponse &ar)
{
	s 	<< "returnCode = " << ar.returnCode << '\n'
		<< "errno = " << ar.errnumber << '\n'
	    << ar.attributes ;	 
	
	return s ; // PRQA S 4028
} 

exostream &operator<<(exostream &s, const Ism__setAttrRequest &ar)
{
	s 	<< "path = \"" << ar.path << "\"\n"
		<< "userId = " << ar.userId << '\n'	
		<< "groupId = " << ar.groupId << '\n'
		<< ar.attributes ;	 
	
	return s ; // PRQA S 4028
} 

exostream &operator<<(exostream &s, const Ism__deleteRequestFs &ar)
{
	s 	<< "path = \"" << ar.filePath << "\"\n"
		<< "userId = " << ar.userId << '\n'	
		<< "groupId = " << ar.groupId << '\n'	
		<< "recursive = " << boolalpha << ar.recursive << '\n';	
	
	return s ; // PRQA S 4028
} 

exostream &operator<<(exostream &s, const Ism__cacheInfo& c)
{
	s	<< "cacheName = \"" << c.cacheName <<  "\"\n"
		<< "fileClass = \"" << c.fileClass <<  "\"\n"
		<< "freeSize = " << c.freeSize <<  " [MB]\n";
	return s ; // PRQA S 4028
}

exostream &operator<<(exostream &s, const Ism__cacheInfoResponse& cr) 
{
	
	size_t cSize = cr.cacheInfo.size();
	for( size_t i = 0; i < cSize; ++i ) {
		s << "cacheInfo[" << i << "]\n" << cr.cacheInfo[i] << '\n';
	}
	return s ; // PRQA S 4028
}

#ifdef EXPRIVIA_ISM_USE_AWS_S3
exostream &operator<<(exostream &s, const Ism__cacheConfiguration &v)
{
	s	<< "configurationKey = \"" 	<< v.configurationKey 				<< "\"\n"
	  	<< "provider = \"" 			<< v.provider 						<< "\"\n"
		<< "key_ak = \"" 			<< v.key_ak  						<< "\"\n"
		<< "key_sk = \"" 			<< v.key_sk 						<< "\"\n"
		<< "region = \"" 			<< v.region 						<< "\"\n"
		<< "endpoint = \"" 			<< v.endpoint 						<< "\"\n"
		<< "endpointIsSecure = " 	<< boolalpha << v.endpointIsSecure 	<< "\n"
		<< "options = \"" 			<< v.options 						<< "\"\n";
	
	return s ; // PRQA S 4028
} 

exostream &operator<<(exostream &s, const Ism__cacheConfigurationAdditionalParms &v)
{
	s	<< "downloadPoolSize=" 		<< v.downloadPoolSize 				<< '\n'
		<< "downloadBufferSize=" 	<< v.downloadBufferSize 			<< '\n'
		<< "uploadPoolSize=" 		<< v.uploadPoolSize 				<< '\n'
		<< "uploadBufferSize=" 		<< v.uploadBufferSize 				<< '\n';
	
	return s ; // PRQA S 4028
} 

#endif

exostream &operator<<(exostream &s, const Ism__giveupRequest &dr)
{

	s 	<< "filePath = \"" 			<< dr.filePath 			<< "\"\n"
		<< "user = \"" 				<< dr.user 				<< "\"\n"
		<< "clientReference = \"" 	<< dr.clientReference 	<< "\"\n" ;
	
	return s ; // PRQA S 4028
} 

_ACS_END_NAMESPACE
