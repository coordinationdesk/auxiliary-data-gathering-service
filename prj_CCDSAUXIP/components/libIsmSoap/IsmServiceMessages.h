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

	$Id$

	$Author$

	$Log$
	Revision 1.19  2017/06/23 09:21:28  lucio.pulvirenti
	S3MPM-873: added new structs fields in methods.
	
	Revision 1.18  2017/05/18 10:33:59  lucio.pulvirenti
	Ism__getAttrRequest class added. Ism__listRequest and Ism__listResponse modified.
	
	Revision 1.17  2017/03/15 13:10:38  lucio.pulvirenti
	S3MPM-873: Ism__listRequest: user attribute removed. Ism__fileAttributes and Ism__getAttrResponse classes added.
	
	Revision 1.16  2017/03/08 11:28:03  lucio.pulvirenti
	S3MPM-873: methods to print Ism__accessRequest and Ism__accessResponse structs added.
	
	Revision 1.15  2017/02/24 14:08:56  lucio.pulvirenti
	S3MPM-873: Added classes and method to perform ls on given path.
	
	Revision 1.14  2014/06/27 09:13:03  marpas
	qa warnings fixed
	
	Revision 1.13  2012/02/22 09:47:15  marpas
	wrong comment fixed
	
	Revision 1.12  2012/02/22 09:42:10  marpas
	no more warning in compilation
	
	Revision 1.11  2011/02/23 11:19:15  lucio.pulvirenti
	Output of Ism__deleteRequest class added
	
	Revision 1.10  2011/02/22 17:00:37  lucio.pulvirenti
	work in progress
	
	Revision 1.9  2011/02/16 11:14:18  lucio.pulvirenti
	output of new structures added
	
	Revision 1.8  2011/02/11 14:53:55  lucio.pulvirenti
	work in progress
	
	Revision 1.7  2011/02/02 16:28:04  lucio.pulvirenti
	work in progress
	
	Revision 1.6  2011/02/01 15:56:04  lucio.pulvirenti
	work in progress
	
	Revision 1.5  2011/01/27 17:44:59  lucio.pulvirenti
	output of new classes added
	
	Revision 1.4  2011/01/25 18:02:22  lucio.pulvirenti
	work in progress
	
	Revision 1.3  2011/01/21 18:40:40  lucio.pulvirenti
	work in progress
	
	Revision 1.2  2011/01/20 17:40:55  lucio.pulvirenti
	work in progress
	
	Revision 1.1  2008/06/16 09:42:44  paoscu
	missing header
	
	
	
        
*/
#ifndef _IsmServiceMessages_H_
#define _IsmServiceMessages_H_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor" // PRQA S 1040 3
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wold-style-cast"

#include <acsH.h> // PRQA S 1013

#pragma GCC diagnostic pop

#include <exStream.h>


_ACS_BEGIN_NAMESPACE(acs)

// PRQA S 2072 EOF
std::ostream &operator<<(std::ostream &, const VectorOfInts &) ;

std::ostream &operator<<(std::ostream &, const VectorOfStrings &) ;

std::ostream &operator<<(std::ostream &, const VectorOfLongLongs &) ;

std::ostream &operator<<(std::ostream &, const VectorOfBooleans &) ;

std::ostream &operator<<(std::ostream &, const Ism__fileDefinition &) ;

std::ostream &operator<<(std::ostream &, const Ism__uploadSingleFileRequest &) ;

std::ostream &operator<<(std::ostream &, const Ism__uploadResponse &) ;

std::ostream &operator<<(std::ostream &, const Ism__DownloadFileRequest &) ;

std::ostream &operator<<(std::ostream &, const Ism__DownloadFileRequest_2 &) ;

std::ostream &operator<<(std::ostream &, const Ism__notifyDownloadRequest &) ;

std::ostream &operator<<(std::ostream &, const Ism__dirDefinition &) ;

std::ostream &operator<<(std::ostream &, const Ism__uploadDirectoryRequest &) ;

std::ostream &operator<<(std::ostream &, const Ism__dirUploadResponse &) ;

std::ostream &operator<<(std::ostream &, const Ism__notifyDirUploadRequest &) ;

std::ostream &operator<<(std::ostream &, const Ism__directoryTreeResponse &) ;

std::ostream &operator<<(std::ostream &, const Ism__downloadMultiFilesRequest &) ;

std::ostream &operator<<(std::ostream &, const Ism__downloadDirRequest &) ;

std::ostream &operator<<(std::ostream &, const Ism__downloadMultiFilesResponse &) ;

std::ostream &operator<<(std::ostream &, const Ism__deleteRequest &) ;

std::ostream &operator<<(std::ostream &, const Ism__listRequest &) ;

std::ostream &operator<<(std::ostream &, const Ism__listResponse&) ;

std::ostream &operator<<(std::ostream &, const Ism__accessRequest&) ;

std::ostream &operator<<(std::ostream &, const Ism__accessResponse&) ;

std::ostream &operator<<(std::ostream &, const Ism__fileAttributes&) ;

std::ostream &operator<<(std::ostream &, const Ism__getAttrRequest&) ;

std::ostream &operator<<(std::ostream &, const Ism__getAttrResponse&) ;

std::ostream &operator<<(std::ostream &, const Ism__setAttrRequest&) ;

std::ostream &operator<<(std::ostream &, const Ism__deleteRequestFs&) ;

std::ostream &operator<<(std::ostream &, const Ism__cacheInfo&) ;

std::ostream &operator<<(std::ostream &, const Ism__cacheInfoResponse&) ;

#ifdef EXPRIVIA_ISM_USE_AWS_S3  
std::ostream &operator<<(std::ostream &, const Ism__cacheConfiguration&) ;

std::ostream &operator<<(std::ostream &, const Ism__cacheConfigurationAdditionalParms&) ;
#endif

std::ostream &operator<<(std::ostream &, const Ism__giveupRequest&) ;

std::ostream &operator<<(std::ostream &, const Ism__onCacheRequest&) ;

std::ostream &operator<<(std::ostream &, const enum Ism__onCacheResponseCodeEnum&) ;


acs::exostream &operator<<(acs::exostream &, const VectorOfInts &) ;

acs::exostream &operator<<(acs::exostream &, const VectorOfStrings &) ;

acs::exostream &operator<<(acs::exostream &, const VectorOfLongLongs &) ;

acs::exostream &operator<<(acs::exostream &, const VectorOfBooleans &) ;

acs::exostream &operator<<(acs::exostream &, const Ism__fileDefinition &) ;

acs::exostream &operator<<(acs::exostream &, const Ism__uploadSingleFileRequest &) ;

acs::exostream &operator<<(acs::exostream &, const Ism__uploadResponse &) ;

acs::exostream &operator<<(acs::exostream &, const Ism__DownloadFileRequest &) ;

acs::exostream &operator<<(acs::exostream &, const Ism__DownloadFileRequest_2 &) ;

acs::exostream &operator<<(acs::exostream&, const Ism__notifyDownloadRequest &) ;

acs::exostream &operator<<(acs::exostream &, const Ism__dirDefinition &) ;

acs::exostream &operator<<(acs::exostream &, const Ism__uploadDirectoryRequest &) ;

acs::exostream &operator<<(acs::exostream &, const Ism__dirUploadResponse &) ;

acs::exostream &operator<<(acs::exostream &, const Ism__notifyDirUploadRequest &) ;

acs::exostream &operator<<(acs::exostream &, const Ism__directoryTreeResponse &) ;

acs::exostream &operator<<(acs::exostream &, const Ism__downloadMultiFilesRequest &) ;

acs::exostream &operator<<(acs::exostream &, const Ism__downloadDirRequest &) ;

acs::exostream &operator<<(acs::exostream &, const Ism__downloadMultiFilesResponse &) ;

acs::exostream &operator<<(acs::exostream &, const Ism__deleteRequest &) ;

acs::exostream &operator<<(acs::exostream &, const Ism__listRequest &) ;

acs::exostream &operator<<(acs::exostream &, const Ism__listResponse &) ;

acs::exostream &operator<<(acs::exostream &, const Ism__accessRequest&) ;

acs::exostream &operator<<(acs::exostream &, const Ism__accessResponse&) ;

acs::exostream &operator<<(acs::exostream &, const Ism__fileAttributes&) ;

acs::exostream &operator<<(acs::exostream &, const Ism__getAttrRequest&) ;

acs::exostream &operator<<(acs::exostream &, const Ism__getAttrResponse&) ;

acs::exostream &operator<<(acs::exostream &, const Ism__setAttrRequest&) ;

acs::exostream &operator<<(acs::exostream &, const Ism__deleteRequestFs&) ;

acs::exostream &operator<<(acs::exostream &, const Ism__cacheInfo&) ;

acs::exostream &operator<<(acs::exostream &, const Ism__cacheInfoResponse&) ;

#ifdef EXPRIVIA_ISM_USE_AWS_S3  
acs::exostream &operator<<(acs::exostream &, const Ism__cacheConfiguration&) ;

acs::exostream &operator<<(acs::exostream &, const Ism__cacheConfigurationAdditionalParms&) ;

#endif

acs::exostream &operator<<(acs::exostream &, const Ism__giveupRequest&) ;

acs::exostream &operator<<(acs::exostream &, const Ism__onCacheRequest&) ;

acs::exostream &operator<<(acs::exostream &, const enum Ism__onCacheResponseCodeEnum&) ;

_ACS_END_NAMESPACE

#endif
