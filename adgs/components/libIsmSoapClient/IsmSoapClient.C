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

	$Prod: A.C.S. libIsmSoapClient$

	$Id$

	$Author$

	$Log$
	Revision 6.13  2018/02/21 12:00:38  chicas
	added #ifndefS2PDGSAI
	
	Revision 6.12  2017/12/20 10:52:48  lucio.pulvirenti
	ISM-113: as per request of Fabrizio Tringali check if Client and Server are the same host performed cutting possible Server dominion.
	clientAndServerSameHost public method added.
	
	Revision 6.11  2017/11/15 10:47:08  chicas
	some message to PRIVINFO
	
	Revision 6.10  2017/11/09 09:26:26  lucio.pulvirenti
	S2PDGS-1843: getCachesStorageInfo method added.
	
	Revision 6.9  2017/10/31 10:32:36  lucio.pulvirenti
	S2PDGS-1876: Exceptions caught during upload issued as ERROR messages on a single line.
	
	Revision 6.8  2017/06/23 09:32:06  lucio.pulvirenti
	S3MPM-873: Added flag to checkPermission (default true) to methods uploading and downloading files and directories.
	Added group string (default empty) to methods downloading files and directories. Added method to get user and group from the ids. Added method to remove entry like in FS.
	
	Revision 6.7  2017/06/13 14:08:19  lucio.pulvirenti
	S3MPM-873: user string input param replaced by uid_t and gid_t added to getList, access, getAttributes methods IFs to be used by invoked Server methods.
	boolean input pointer removed from getList method IF.
	
	Revision 6.6  2017/05/22 10:18:28  lucio.pulvirenti
	S3MPM-873: useless method internalremove removed: remove used instead.
	remove: is boolean parm set, Ism__deleteFileFsLike IF used to remove entry and errcode is returned.
	
	Revision 6.5  2017/05/18 10:43:33  lucio.pulvirenti
	S3MPM-873: added optional bool parameter to download method to remove local file is existing (default false).
	getList: added user parameter to be passed to getList Soap Server method; isDir boolean output valorized, if requested.
	getAttributes: added user parameter to valorize ISM__getAttrRequest obj.
	
	Revision 6.4  2017/03/15 13:21:27  lucio.pulvirenti
	S3MPM-873: convert method added to fill struct stat from Ism__fileAttributes. User input par removed from getList IF.
	getAttributes method added.
	
	Revision 6.3  2017/03/08 14:12:41  lucio.pulvirenti
	S3MPM-873: getList return logic changed.
	
	Revision 6.2  2017/03/08 12:58:52  lucio.pulvirenti
	S3MPM-873: access method added. getList returns an integer as error code and added a parameter with error number to IF.
	
	Revision 6.1  2017/02/27 16:16:43  lucio.pulvirenti
	S3MPM-873: ls method added.
	
	Revision 6.0  2016/11/14 10:53:52  lucio.pulvirenti
	ISM-113: If Client runs on the same Server host, LocalTransferMode tag searched in SMConfiguration node to override transfer modes. Possibility to override
	also if IsmLocalTransferMode node occurring in caller Application conf space.
	
	Revision 5.13  2016/05/09 09:21:16  lucio.pulvirenti
	PDSEV-34: WorkerThread _deleteLocalPath attribute and related method removed.
	
	Revision 5.12  2016/05/03 13:26:19  lucio.pulvirenti
	ISM-108: internalDirectoryUpload: bug fixed when trying to remove a directory that is a symbolic link after upload is successful if deleteUploadedFileFromDisk
	configuration flag is set.
	
	Revision 5.11  2016/04/12 12:19:35  marpas
	wrong exception inheritance fixed
	
	Revision 5.10  2016/01/15 10:36:47  lucio.pulvirenti
	APF-270: internalDirectoryUpload: Ism__dirDefinition totalItemsSize ptr allocated to be passed to uploadDirectory Server method request.
	If totalItemsSize ptr in the uploadDirectory Server method response valorized, then allocate Ism__notifyDirUploadRequest totalItemsSize ptr and valorize.
	
	Revision 5.9  2015/10/29 10:05:27  lucio.pulvirenti
	WorkerThread::ftp(): if remote url is empty and item is a directory, exception thrown no more: still to test deeply.
	
	Revision 5.8  2015/10/20 15:56:55  lucio.pulvirenti
	ARK-3: work in progress.
	
	Revision 5.7  2015/09/23 11:26:12  lucio.pulvirenti
	APF-312: Improved.
	
	Revision 5.6  2015/09/23 10:48:50  lucio.pulvirenti
	APF-312: end points now put in a static map that may have the t_storagemanager id as key. It can be used safely in a mutlithread context with n storage manager.
	
	Revision 5.5  2014/06/30 15:19:53  lucio.pulvirenti
	S1PDGS-31470 stopWorker method implemented in WorkerThread class to stop thread and wait until no more running (waitExitOrTimeout).
	If exception caught while worker thread is running, stopWorker method is called.
	
	Revision 5.4  2014/06/27 14:40:09  marpas
	removed deprecated FtpCurl methods (now uses RemoteSite class)
	WorkerThread and Enpoints internal classes removed from header file (are in the compilation unit now)
	codind best practices applied
	potential umask race conditions fixed: now uses ProcessStatus from libException
	
	Revision 5.3  2014/05/08 13:55:33  marfav
	S2PDGS-786 IsmSoapClient is not thread safe due to static _endpoint structure
	Lock guards added. To be evaluated if static structure can be avoided.
	
	Revision 5.2  2014/02/17 15:35:15  marpas
	new FtpCurl ctor interface w/ BandPriority
	
	Revision 5.1  2014/01/27 13:07:52  lucio.pulvirenti
	noChangingHardlinkedFilesMode configuration parameter introduced not to change permissions of files uploaded with hard link (after S2 Claudio Nasuti request).
	
	Revision 5.0  2013/07/22 18:15:54  marpas
	adopting libException 5.x standards
	
	Revision 1.73  2013/07/12 14:16:54  marpas
	work in progress
	
	Revision 1.72  2013/04/03 13:50:35  marpas
	run does not longer gets a void*
	
	Revision 1.71  2013/04/03 12:46:51  marpas
	Thread::run gets a void*
	
	Revision 1.70  2012/09/28 11:04:34  lucio.pulvirenti
	internalFileDownload: timer to renew downloading requests explicitely started; comment changed.
	
	Revision 1.69  2012/09/13 10:32:36  lucio.pulvirenti
	internalFileDownload: Ism__DownloadFileRequest::updateRequestTime periodically set to true
	
	Revision 1.68  2012/06/08 14:01:55  lucio.pulvirenti
	FtpCurl overloaded put and get method are over: put accepts permissions in octal format and
	get changes effectively permissions of downloaded files
	
	Revision 1.67  2012/06/01 10:39:31  lucio.pulvirenti
	WorkerThread::getumask method added
	Permissions of files uploaded in cache are set to 0444; new FtpCurl::put IF used
	Hard link now allowed in download: in this case no permissions change performed and files remain read-only
	Files downloaded by either ftp or copy will have permissions changed as follows: 0664 & ~umask; new FtpCurl::get IF used
	Downloaded directories are created with 0775 & ~umask permissions
	
	Revision 1.66  2012/05/31 16:27:47  lucio.pulvirenti
	work in progress
	
	Revision 1.65  2012/05/24 14:12:13  lucio.pulvirenti
	getValueFromFaultDetail method added
	When getting error in getFileIdInternal, internalFileDownload, directoryDownload, internalDirectoryDownload methods
	check if soap_fault_detail() ptr is valorized. If yes, extract error code and if equal to SOAP_ISM_DATA_UNAVAILABLE
	throw dbDownloadFile::SMUnknownFileException
	
	Revision 1.64  2012/04/27 13:01:45  lucio.pulvirenti
	WorkerThread::link(): before calling File::hardlink method, it is checked if the source file is a symbolic
	link. In that case the target file is considered for link.
	
	Revision 1.63  2012/04/24 14:52:23  lucio.pulvirenti
	internalDirectoryDownload: if error is returned by downloadDirectory Server method, Ism__downloadDirRequest
	job_id no more filled with Ism__downloadMultiFilesResponse job_id since in this case the returned structure is not valorized.
	Actually the Server soap_receiverfault is returned in case of error
	
	Revision 1.62  2012/04/16 15:24:59  lucio.pulvirenti
	dedicatedTransferMode method added to find in connection file particular transfer mode for the current Client, if any.
	
	Revision 1.61  2012/03/29 16:11:56  lucio.pulvirenti
	retryConnectionToServer: if the first attempt, re-start timer since it could have been
	waiting for request to be served for long time
	
	Revision 1.60  2012/02/22 09:50:21  marpas
	no more warnings in compilation
	
	Revision 1.59  2012/02/08 16:47:14  marpas
	removing compiler warning
	
	Revision 1.58  2011/07/20 10:24:43  lucio.pulvirenti
	If transfer mode is either copy or hard link, mode of file in cache changed into 666
	otherwise an error is caught when trying to write files on tape or to remove them from cache
	
	Revision 1.57  2011/05/12 10:08:10  lucio.pulvirenti
	Exceptions messages improved
	
	Revision 1.56  2011/05/11 13:32:50  lucio.pulvirenti
	No more used description method to insert new messages in exceptions. Information messages
	are logged instead and exception are notified and then thrown again
	
	Revision 1.55  2011/05/10 13:30:57  lucio.pulvirenti
	WorkerThread::ftp(): FtpCurl::setFullFilename and FtpCurl::setFullLocalFilename methods used instead of
	(setRemPath + setRemoteFile) and (setLocalPath + setLocalFile) respectively
	
	Revision 1.54  2011/05/09 15:22:45  lucio.pulvirenti
	download: check immediately if parent of local path exist: if not exception thrown
	
	Revision 1.53  2011/05/09 09:57:14  lucio.pulvirenti
	WorkerThread::link: deletion of local files performed anyway if deleteUploadedFileFromDisk == true
	When uploading files or directory, if cannot remove files from local disk (if required) ism_files recs
	set to REMOVED and ism_onlines and paths in cache deleted
	
	Revision 1.52  2011/05/05 16:56:18  lucio.pulvirenti
	work in progress
	
	Revision 1.51  2011/04/18 09:44:18  lucio.pulvirenti
	setConnParameters: input resourceSet now const
	
	Revision 1.50  2011/03/08 08:41:08  lucio.pulvirenti
	directoryDownload: insertion of external requests now got from jobId value
	internalDirectoryDownload: information to update ext requests timestamp sent periodically so Server;
	while loop checking if WorkerThread running now in try catch
	if FileToDownloadDisappearedExc caught, information to set ext req cache_id to null send to Server
	
	Revision 1.49  2011/03/03 17:11:53  lucio.pulvirenti
	work in progress
	
	Revision 1.48  2011/03/02 16:10:49  lucio.pulvirenti
	internalFileUpload and internalDirectoryUpload: update time set to 30 s (it was 60)
	
	Revision 1.47  2011/02/23 11:35:38  lucio.pulvirenti
	internalRemove private method added used by public remove. The former one allow to remove also ism_online records
	in particular cases (e.g. when exception is caught during upload of files/directories)
	
	Revision 1.46  2011/02/22 17:00:57  lucio.pulvirenti
	work in progress
	
	Revision 1.45  2011/02/16 15:10:26  lucio.pulvirenti
	upload methods IF changed so that input fileDefinition struct no more const. This allows the method itself
	to set the right value of fileDefinition::isDir flag depending on the input type (localpath file/directory)
	
	Revision 1.44  2011/02/16 12:24:17  lucio.pulvirenti
	WorkerThread::copy(): roll_back for DOWNLOAD operations added
	download: new field isDir of Ism_getFileIdResponse checked to discriminate download of either file or directory
	download code for file now in fileDownload method and internalDownload -> internalFileDownload
	directoryDownload and internalDirectoryDownload new methods added
	
	Revision 1.43  2011/02/11 14:54:54  lucio.pulvirenti
	work in progress
	
	Revision 1.42  2011/02/02 16:29:14  lucio.pulvirenti
	work in progress
	
	Revision 1.41  2011/02/01 15:56:38  lucio.pulvirenti
	work in progress
	
	Revision 1.40  2011/01/31 17:03:54  lucio.pulvirenti
	work in progress
	
	Revision 1.39  2011/01/28 11:51:43  lucio.pulvirenti
	Upload of directories introduced
	WorkerThread class modified to transfer more than one file; as to ftpCurl, new feature used: connection is opened just once
	Formed upload method turned into private internalFileUpload.
	Two public upload overloaded methods occur to get std::vector< std::pair<std::string , std::string> > from returned value or from IF as a &
	
	Revision 1.38  2011/01/26 16:39:38  lucio.pulvirenti
	work in progress
	
	Revision 1.37  2011/01/25 18:03:03  lucio.pulvirenti
	work in progress
	
	Revision 1.36  2011/01/21 18:39:56  lucio.pulvirenti
	work in progress
	
	Revision 1.35  2011/01/20 17:44:26  lucio.pulvirenti
	work in progress
	
	Revision 1.34  2011/01/18 15:33:13  lucio.pulvirenti
	FileToDownloadDisappearedExc added
	When file to download disappears in both ftp and copy case, FileToDownloadDisappearedExc is thrown so that,
	if caught, false is returned by internalDownload and download wait goes on.
	
	Revision 1.33  2011/01/17 12:11:37  lucio.pulvirenti
	Added to IF transfer mode parms for upload/download to be read from configuration file (setConnParameters)
	Added to IF _deleteUploadedFileFromDisk to be read from configuration file (setConnParameters) to delete
	succesfully uploaded file in case of hard link transfer mode (default for hard link)
	class FtpThread name turned into WorkerThread and added private methods to execute ftp, copy or hard link
	depending on configuration
	upload: if WorkerThread has been completed and WorkerThread::deleteLocalPath returns true, remove uploaded file
	
	Revision 1.32  2011/01/04 10:49:39  lucio.pulvirenti
	Refuse: sleep time forgot
	
	Revision 1.31  2011/01/04 10:36:28  lucio.pulvirenti
	internalDownload: bug fixed: if downloadURL on server not empty, file occurrence
	no more verified since it is not on a local repository
	download: when catching exception, ext request rec is removed only if id != -1
	
	Revision 1.30  2010/12/22 15:39:08  lucio.pulvirenti
	FtpThread::run(): on FtpCurl::FtpCurlRemoteRetrieveErrorException (file removed) just  thrown
	download method now called private internaldownload in a while loop exited by timeout and
	on exception ext request is removed
	internaldownload: on FtpCurl::FtpCurlRemoteRetrieveErrorException  by FtpThread, just exit to
	retry download later
	
	Revision 1.29  2010/11/29 14:47:40  lucio.pulvirenti
	ftpTimeout and ftpInterval attributes added to class IF to manage possible exception of ftpCurl
	Those parameters are expected to be taken from configuration blob: if they do not
	occur, warning is emitted the first time only.
	
	Revision 1.28  2010/11/12 11:01:52  marpas
	messages and make-up
	
	Revision 1.27  2010/11/11 18:07:16  marpas
	iallow at least 2 attempts on each port (then check timeout)
	if timeout is not elapsed the number of attempts can be greater
	
	Revision 1.26  2010/11/11 17:35:41  lucio.pulvirenti
	endpoints concept introduced for multithread
	test improved
	
	Revision 1.25  2010/11/03 10:28:06  lucio.pulvirenti
	Warnings substituted with errors
	
	Revision 1.24  2010/11/02 17:25:54  lucio.pulvirenti
	retryConnectionToServer: errors printed always (not only in DEBUG mode)
	_lastendpoint now static to memorize the last good value
	
	Revision 1.23  2009/12/16 11:44:05  lucio.pulvirenti
	retryConnectionToServer: connection failure message in DEBUG only
	
	Revision 1.22  2009/11/13 17:57:31  lucio.pulvirenti
	Connection timeout introduced: got from rs file
	
	Revision 1.21  2009/10/06 13:01:02  lucio.pulvirenti
	Ftp mode (passive/active) taken from connection parameters
	
	Revision 1.20  2009/06/26 10:44:17  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.19  2009/06/19 10:54:39  lucio.pulvirenti
	minor change
	
	Revision 1.18  2009/06/19 10:46:28  lucio.pulvirenti
	Possibility to set password encryption algo to decript password.
	If no algo set, password is assumed not encrypted
	
	Revision 1.17  2009/06/17 09:05:27  marpas
	Exception type narrowed
	
	Revision 1.16  2009/06/16 15:26:53  lucio.pulvirenti
	minor changes
	
	Revision 1.15  2009/06/16 14:53:11  marpas
	new client interface for parameters, older one still valid
	multiport(2) attempts to connect the server (if port2 set)
	.so dependencies changed (new types used)
	
	Revision 1.14  2009/03/19 18:33:07  lucio.pulvirenti
	book method added
	
	Revision 1.13  2009/03/18 18:27:33  lucio.pulvirenti
	bug fixed in computing notification time
	Error issued if download not completed
	
	Revision 1.12  2009/03/10 15:35:21  paoscu
	Added request id in all debug messages
	
	Revision 1.11  2009/03/10 15:02:58  paoscu
	Added time accounting
	
	Revision 1.10  2009/03/10 14:38:15  clanas
	sleep time changed
	
	Revision 1.9  2009/02/24 12:15:12  paoscu
	Download operation aborted in case of error from server
	
	Revision 1.8  2008/10/03 15:39:35  paoscu
	Changed download methods interface to add request id parameter
	
	Revision 1.7  2008/06/16 13:33:54  paoscu
	new interface
	
	Revision 1.6  2008/06/10 23:50:23  paoscu
	using strings instead of ids for user and group
	
	Revision 1.5  2008/06/06 18:07:40  paoscu
	Completed ftp curl integration
	
	Revision 1.4  2008/05/29 17:56:33  paoscu
	Added FTP support
	
	Revision 1.3  2008/05/28 17:32:25  paoscu
	Added download methods
	
	Revision 1.2  2008/05/14 15:23:27  paoscu
	Delete function added
	
	Revision 1.1.1.1  2008/05/07 17:48:45  paoscu
	Imported into CVS
	
        
*/ 

#include <IsmSoapClient.h>
#include <PackagingAlgoFactory.h>
#include <PackagingAlgoSingleton.h>
#include <IsmServerVersionManagerSingleton.h>
#include "IsmSoapClientV.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor" // PRQA S 1040 3
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wold-style-cast"

#include <acsIsmProxy.h>

#pragma GCC diagnostic pop

#include <IsmServiceMessages.h>

#include <dbDownloadFile.h>
#include <ProcessStatus.h>
#include <StringUtils.h>
#include <Application.h>
#include <HWScan.h>
#include <File.h>
#include <Dir.h>
#include <Timer.h>
#include <FtpCurl.h>
#include <RemoteSiteFactory.h>
#include <Filterables.h>
#include <DateTime.h>
#include <XMLIstream.h>
#include <XMLIstringstream.h>
#include <rsResourceSet.h>
#include <ftmpstream.h>


_ACS_BEGIN_NAMESPACE(acs) 

using namespace std;
namespace {

IsmSoapClientV version;

PackagingAlgoFactory algoFactory ;

class EndPoints { // PRQA S 2109, 2153
public:
	EndPoints() ;
	EndPoints &operator=(const EndPoints &) ;
	virtual ~EndPoints() throw() ;
	void addEndPoint(const std::string &) ;
	void clearEndPoints() ;
	unsigned short getLastGoodEndPoint() const ;
	unsigned short getNextEndPoint(unsigned short) const ;
	std::string operator[](unsigned short ) const ; // PRQA S 2141
	void setGoodEndPoint(unsigned short) ;
	unsigned short size() const ;
private:
	EndPoints(const EndPoints &) ; // not implemented

private:
	unsigned short _lastGoodEndPoint ;
	std::vector< std:: string > _endpoints ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(EndPoints) ;
} ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(EndPoints)

EndPoints::EndPoints() : _lastGoodEndPoint(0), _endpoints()
{
}

#if 0
EndPoints::EndPoints(EndPoints const &o): 
		  	_lastGoodEndPoint(o._lastGoodEndPoint),
		  	_endpoints(o._endpoints)
{
}
#endif

EndPoints & EndPoints::operator = (EndPoints const &o) 
{
	if( this != &o ) {
		_lastGoodEndPoint = o._lastGoodEndPoint;
		_endpoints = o._endpoints;
	}
	return (*this);
}

EndPoints::~EndPoints() throw() {}

void EndPoints::addEndPoint(const std::string &ep) 
{
	if (find(_endpoints.begin(),_endpoints.end(), ep) == _endpoints.end()) {
	  _endpoints.push_back(ep) ;
	  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "endpoint[" << _endpoints.size() -1 << "] is " << ep) ; // PRQA S 3084
	}
}

void EndPoints::clearEndPoints() 
{
	_endpoints.clear() ;
	_lastGoodEndPoint = 0 ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "endpoints cleared") ;
}

string EndPoints::operator[](unsigned short i) const  // PRQA S 4020
{
	if (i < _endpoints.size()) {
	  return _endpoints[i] ;
	}
	return ""  ;
}

unsigned short EndPoints::size() const 
{
	return _endpoints.size() ; // PRQA S 3010
}

unsigned short EndPoints::getLastGoodEndPoint() const 
{
	return _lastGoodEndPoint ;
}

void EndPoints::setGoodEndPoint(unsigned short v)  
{
	_lastGoodEndPoint = v ;
}

unsigned short EndPoints::getNextEndPoint(unsigned short v) const 
{
	return (v+1)%max<int>(_endpoints.size(),1) ; // PRQA S 3000, 3010
}
  
// THIS IS A STATIC VARIABLE used to map endpoints with a key that typically corresponds to t_storagemanager record id.
map<long long int, EndPoints> endpointsMap ;

 
map<long long int, EndPoints>::iterator getEndPointsFromMapIt(long long int key) // PRQA S 4211
{
	map<long long int, EndPoints>::iterator it = endpointsMap.find(key);
	if( it == endpointsMap.end() ) {
		ostringstream exMsg;
		exMsg << "Cannot find end points with key=" << key;
		ACS_THROW( IsmSoapClient::Exception( exMsg.str() ) );
	}
	return it;
}

#ifdef EXPRIVIA_ISM_USE_AWS_S3

void fillS3Handler( AwsS3Wrapper::AwsS3WrapperConfiguration & awsS3WrapperConfiguration, ACS_SMARTPTR<AwsS3Wrapper> &s3handler_p, const StopController & sc, IsmSoapClient::Operation *op = NULL) 
{
	AwsS3Wrapper::Enum_S3Provider S3Provider = awsS3WrapperConfiguration.get_aws_s3_provider() ;
	Aws::String key_ak, key_sk ;

	const bool credentials_set ( (false == awsS3WrapperConfiguration.get_aws_key_ak().empty()) && (false == awsS3WrapperConfiguration.get_aws_key_sk().empty() ) ) ;

	if (true == credentials_set)
	{
		// Set AK/SK
		key_ak = AwsS3Wrapper::to_AwsString( awsS3WrapperConfiguration.get_aws_key_ak() ) ; 
		key_sk = AwsS3Wrapper::to_AwsString( awsS3WrapperConfiguration.get_aws_key_sk() ) ; 
	}

	const Aws::Auth::AWSCredentials credentials ( (false == credentials_set)? Aws::Auth::AWSCredentials() : Aws::Auth::AWSCredentials(key_ak, key_sk) ) ;

	string aws_s3_region = awsS3WrapperConfiguration.get_aws_region();
	string aws_s3_endpoint = awsS3WrapperConfiguration.get_aws_endpoint();
	if ( false == aws_s3_endpoint.empty() ) {
		s3handler_p = ACS_SMARTPTR<AwsS3Wrapper>( new AwsS3Wrapper(S3Provider, credentials, aws_s3_region, aws_s3_endpoint, sc) ) ; 
		aws_s3_region = s3handler_p->get_region() ; 
	}
	else if ( false == aws_s3_region.empty() )  {	
		s3handler_p = ACS_SMARTPTR<AwsS3Wrapper>( new AwsS3Wrapper(S3Provider, credentials, aws_s3_region, sc) ) ; 
		aws_s3_endpoint = s3handler_p->get_endpoint() ; 
	}
	else {	
		s3handler_p = ACS_SMARTPTR<AwsS3Wrapper>( new AwsS3Wrapper(S3Provider, credentials, sc) ) ;
		
		aws_s3_endpoint = s3handler_p->get_endpoint() ; 
		aws_s3_region = s3handler_p->get_region() ; 
	}

			
	awsS3WrapperConfiguration.set_aws_endpoint( aws_s3_endpoint );
	awsS3WrapperConfiguration.set_aws_region( aws_s3_region );
	
	if( op != NULL ) {
		if( IsmSoapClient::UPLOAD == *op ) {

			if( awsS3WrapperConfiguration.get_upload_pool_size() != 0 ) {
				s3handler_p->set_upload_pool_size(awsS3WrapperConfiguration.get_upload_pool_size());
			}

			if( awsS3WrapperConfiguration.get_upload_buffer_size() != 0 ) {
				s3handler_p->set_upload_buffer_size(awsS3WrapperConfiguration.get_upload_buffer_size());
			}
		}
		else if ( IsmSoapClient::DOWNLOAD == *op ) {

			if( awsS3WrapperConfiguration.get_download_pool_size() != 0 ) {
				s3handler_p->set_download_pool_size(awsS3WrapperConfiguration.get_download_pool_size());
			}

			if( awsS3WrapperConfiguration.get_download_buffer_size() != 0 ) {
				s3handler_p->set_download_buffer_size(awsS3WrapperConfiguration.get_download_buffer_size());
			}

		}
	}

}	 

#endif

  class WorkerThread : public Thread // PRQA S 2109
  {
  public:
	  WorkerThread( IsmSoapClient &parent,
				 std::string  const & remoteUrl ,
				 std::string  const & localpath ,
				 IsmSoapClient::Operation op,
			  	 const IsmSoapClient::fileDefinition & = IsmSoapClient::fileDefinition());
		  		// string const & originalName = string()) ;
	  WorkerThread( IsmSoapClient &parent,
				 std::vector<std::string>  const & remoteUrls ,
				 std::vector<std::string>  const & localpaths ,
				 IsmSoapClient::Operation op,
			  	 const IsmSoapClient::fileDefinition & = IsmSoapClient::fileDefinition() );
			  	// vector<string> const & originalNames = vector<string>()) ;
	  virtual ~WorkerThread() throw() ;
	  virtual void run();
	  bool completed() const throw() ;
	  void stopWork() throw();
	  void setPackagingAlgoOnTheFly( const std::string & v ) throw() { _packagingAlgoOnTheFly = v ; }
  private:
      WorkerThread() ; // not implemented
      WorkerThread(const WorkerThread &) ; // not implemented
      WorkerThread& operator= (const WorkerThread &) ; // not implemented

	  IsmSoapClient::TransferMode getActualTransferMode() const ;
	  void ftp();
	  void copy(); // PRQA S 2502 2
	  void link();
#ifdef EXPRIVIA_ISM_USE_AWS_S3
	  void local_rollBack(const vector<string> & pathsToRemove ) ; 
	  void aws_s3();
	  void aws_s3_rollBack( AwsS3Wrapper *, const vector<string> & absPathsToRemove ) ; 
#endif
	  string createTemporaryDirectory( const string & rootPath );
	  void renameUntaredItem( const string & untaredItemDir, const string & localpath, mode_t fileMode, mode_t dirMode  ) ;
	  File::CompressionMode getCompressionMode (const string &cMode );

		
  private:
	  IsmSoapClient &_parent ;
	  std::vector<std::string> _remoteUrls ;
	  std::vector<std::string> _localpaths ;
	  IsmSoapClient::Operation _op ;
	  bool _completed ;
	  bool _stopRequested;
	  IsmSoapClient::fileDefinition _fileDefinition ;
	  std::string _packagingAlgoOnTheFly;

	  ACS_CLASS_DECLARE_DEBUG_LEVEL(WorkerThread) ;
  };	

  ACS_CLASS_DEFINE_DEBUG_LEVEL(WorkerThread)

const unsigned long stopTimeout_msecs = 60000;

WorkerThread::WorkerThread( 
                                     IsmSoapClient &parent ,
				                     string  const & remoteUrl ,
						             string  const & localpath ,
						             IsmSoapClient::Operation op,
									 const IsmSoapClient::fileDefinition &fd):
    Thread(),
	_parent(parent) , // PRQA S 2528
	_remoteUrls() ,
	_localpaths() ,
	_op(op) ,
	_completed(false),
	_stopRequested(false),
	_fileDefinition(fd),
	_packagingAlgoOnTheFly()
{
  _remoteUrls.push_back(remoteUrl);
  _localpaths.push_back(localpath);
}

WorkerThread::WorkerThread( IsmSoapClient &parent ,
				                     vector<string> const & remoteUrls ,
						             vector<string> const & localpaths ,
						             IsmSoapClient::Operation op,
									 const IsmSoapClient::fileDefinition &fd):
    Thread(),
	_parent(parent) , // PRQA S 2528
	_remoteUrls(remoteUrls) ,
	_localpaths(localpaths) ,
	_op(op) ,
	_completed(false),
	_stopRequested(false),
	_fileDefinition(fd),
	_packagingAlgoOnTheFly()
{
}

WorkerThread::~WorkerThread() throw() 
{
	stopWork();
}


void WorkerThread::run()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
			
	size_t remoteUrlsSize = _remoteUrls.size() ;
	if( remoteUrlsSize != _localpaths.size() ) {
		ostringstream exMsg;
		exMsg << "Remote urls number " << remoteUrlsSize << " and local paths " << _localpaths.size() << " do not match";
		ACS_THROW( exIllegalValueException( exMsg.str() ) ); // PRQA S 3081
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "packaging algo on the fly = \"" << _packagingAlgoOnTheFly << '\"') ;
	
	if( not _packagingAlgoOnTheFly.empty() && remoteUrlsSize > 1 ) { // it should not occur
		ostringstream exMsg;
		exMsg << "Internal error: \"" << _packagingAlgoOnTheFly << "\" packaging algo on the fly set but remote urls number is " << remoteUrlsSize << " (1 expected)";
		ACS_THROW( exIllegalValueException( exMsg.str() ) ); // PRQA S 3081
	}

	_completed = false ;
	
	IsmSoapClient::TransferMode tMode = getActualTransferMode();
		
	switch (tMode)  {
    case IsmSoapClient::FTP:
        {
			ftp();
			break;
        }
	case IsmSoapClient::COPY:
        {
			copy();
			break;
        }			
	case IsmSoapClient::LINK:
        {
			link();
			break;
        }			
	case IsmSoapClient::AWS_S3:
        {
#ifdef EXPRIVIA_ISM_USE_AWS_S3        	
			aws_s3();
#else
			ACS_THROW(exIllegalValueException("AWS_S3 was not enabled at compile-time and it is not managed. Please contact Exprivia technical support.")) ;
#endif			
			break;
        }			
	default:
        {
			ostringstream exMsg;
			exMsg << "Internal ERROR: transfer mode unrecognized (" << int(tMode) << ")" ; // PRQA S 3081 2
			ACS_THROW( exIllegalValueException( exMsg.str() ) );
        }			
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "worker thread completed") ;

	_completed = true ;
	
}

void WorkerThread::ftp()
{

	string ftpUser =  _parent.getFtpUser() ;
	if( ftpUser.empty()) {
		ftpUser =  _parent.getUser() ;
    }
	
	int ftpTimeout = _parent.getFtpTotalTimeout_s() ; 
	int ftpRetryingInt = _parent.getFtpRetryingInterval_s() ; 
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "host = \"" << _parent.getHost() << "\"") ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "ftp user = \"" << ftpUser << "\"") ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "ftp password  = \"" << _parent.getFtpPassword() << "\"") ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "ftp passive mode = \"" << boolalpha << _parent.getPassive() << "\"") ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "pwdEncryptionAlgo = 0x" << hex << int(_parent.getPwdEncryptionAlgo())) ; // PRQA S 3081
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "ftp total timeout " << ftpTimeout << " [s]") ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "ftp retrying interval " << ftpRetryingInt << " [s]") ;

	// decrypting passwd
 	string tmp = _parent.getFtpPassword() ;
	const char* encPassword = tmp.c_str() ;
	char decPassword [1024] ;  // PRQA S 4403
	memset( decPassword, 0, sizeof(decPassword) ) ;	/* clear buffer */
	ACS_SSL::EncryptionAlgo algo = _parent.getPwdEncryptionAlgo() ;
   
	// Get decPassword from encPassword  
	if ( encPassword  && (0 != encPassword[0]) ) // exists and is not empty
	{
		vector <void*> vin ;
        vector <void *> vout ;
		BF_KEY bfkey ; // PRQA S 4102
		unsigned char key_in[] = { 'm', 'a', 'n', 'u', 'e', 'l' } ;	/* Blowfish passphrase. Provided by RobRic */
		int key_len = 6 ;
		
		ACS_SSL::ACS_BLOWFISH_setkey( key_in, key_len, &bfkey) ;	/* Get Actual key */
		vin.push_back ( &bfkey ) ;
		
		ACS_COND_THROW( strlen(encPassword)>sizeof(decPassword),ThreadException("ftp password too long") ) ; // PRQA S 3081
   		
		size_t s = ACS_SSL::ACS_ALGO_decode( algo, 
                                             reinterpret_cast<const unsigned char*>(encPassword), // PRQA S 3030, 3081
                                             strlen(encPassword), 
                                             reinterpret_cast<unsigned char*>(decPassword),  // PRQA S 3030, 3081
                                             vin, 
                                             vout ) ;
		ACS_COND_THROW( s < 0, ThreadException("password decoding failure") ) ; // PRQA S 3081
   
    	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Password decrypted successfully.")  ;
#if(0)
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Decrypted password: \"" << decPassword << "\"")  ;
#endif  
	}
	else
	{
		// Nothing to do, since decPassword is zero-filled
	}
					
	string ftpPassword;
	if( 0 != decPassword[0] ) {
		ftpPassword = decPassword;	
    }

    curl::RemoteSite site ;
    site.setHost(_parent.getHost()) ;
    site.setUser(ftpUser) ;
	site.setPassword( ftpPassword ) ;	
    site.setRenameStrategy(curl::temp::noTemp) ;
    site.setIsPassive( _parent.getPassive()) ;
    
    curl::BandPriority no_priority ;
	FtpCurl ftpHandler(no_priority, site) ;				

	ftpHandler.openConnection() ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "going to transfer possibly " << _localpaths.size() << " files") ;

	File::CompressionMode compressionMode = getCompressionMode(_packagingAlgoOnTheFly);
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		if( compressionMode == File::UNDEFINED ) {
			ACS_LOG_DEBUG(SimpleDebugSignature << "Compression mode not set");
		}
		else {
			ACS_LOG_DEBUG(SimpleDebugSignature << "Compression mode: \"" << compressionMode << '\"');
		}
	ACS_CLASS_END_DEBUG

	const mode_t umask = os::ProcessStatus::instance()->getStartupUmask();
	mode_t mode = 0444; // PRQA S 4400
	if( _op == IsmSoapClient::DOWNLOAD ) {
		mode = 0664 & ~umask; // PRQA S 4400
	}
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "mode of " << ((_op == IsmSoapClient::DOWNLOAD) ? "downloaded" : "uploaded") << " files will be set to " << oct << mode) ; // PRQA S 3380
	
	size_t transferredFilesNr = 0;
    size_t lp_size = _localpaths.size(); 
	
	for( size_t i = 0; i < lp_size; ++i ) {
		
		if( _remoteUrls[i].empty() ) {
			if ( not File::isDir(_localpaths[i])) {
                ACS_LOG_WARNING("Local file \"" +  _localpaths[i] + "\" cannot be transferred because remote url is empty!");
			}
			continue;
		}
		
			
		ftpHandler.setFullFilename( _remoteUrls[i] ) ;	
		
		ACS_SMARTPTR <FileGuard> fg ;
		string localPath ;
		if( _op == IsmSoapClient::DOWNLOAD && compressionMode != File::UNDEFINED ) {
			
// *** WARNING *** in case of exception from this method no need to rollback FtpCurl obj since one item only occurs for tar-on-the-fly
			localPath = createTemporaryDirectory( File::getDirPath(_localpaths[i]) );
			fg = ACS_SMARTPTR<FileGuard>(new  FileGuard(localPath) );
			ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
				fg->detach();
			ACS_CLASS_END_DEBUG
			ftpHandler.setLocalPath( localPath );
		}
		else {
			localPath = _localpaths[i] ;
			ftpHandler.setFullLocalFilename( localPath ) ;
		}
	
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "remote full file name  = \"" << _remoteUrls[i] << "\" local full file name  = \"" << localPath << '\"') ;
		
		int currentRetryInt = 0	;	
		bool tryFtpOperation = true;	
		
		while( tryFtpOperation ) {

			int attempt = 1 + currentRetryInt / ftpRetryingInt;

			try {
				
				ACS_COND_THROW(haltImmediate(), ThreadException("Immediate stop requested")) ; 

				switch(_op)
				{
                case IsmSoapClient::UPLOAD:
                    {
						
						if( compressionMode == File::UNDEFINED ) {
					    	ftpHandler.put(true, mode, *this) ;
						}
						else {
							bool encapsulate_dir = false;
							if( File::isDir(localPath) ) {
								encapsulate_dir = true;
							}
        					ftpHandler.putAsTar(compressionMode, encapsulate_dir, mode, *this);
						}

					    tryFtpOperation = false;
                        break;
                    }
                case IsmSoapClient::DOWNLOAD:
                    {
					    if( compressionMode == File::UNDEFINED ) {
							ftpHandler.get(mode, *this) ;
						}
						else {
							vector<string> decompress_options ;
							decompress_options.push_back("-m"); // set item time to current time
							
							mode_t dirMode = 0775 & ~umask ;
							ftpHandler.getUnTar(compressionMode, dirMode, decompress_options, *this);
							
							renameUntaredItem( localPath, _localpaths[i], mode, dirMode) ;
							
						}
					    tryFtpOperation = false;
                        break;
                    }
				}

				transferredFilesNr++;
				if( !tryFtpOperation ) {
    				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "attempt number " << attempt << "; ftp operation successfull") ; 					
					break;
				}

			}
			catch( FtpCurl::FtpCurlRemoteRetrieveErrorException &e )
			{
				ostringstream msg;
				msg << "\"" << _remoteUrls[i] << "\" file to download from remote path no more occurring.";
				
				try {
					ftpHandler.rollbackConnection();
				}
				catch( exception &ee )
				{
					msg << "\n"
						<< "Even failed to rollback: " << ee.what() ;
				}
				
				ACS_THROW( IsmSoapClient::FileToDownloadDisappearedExc(e, msg.str()) ); // PRQA S 3081
			}
			catch( FtpCurl::FtpCurlConnectionFailedException &e )
			{
				ostringstream msg	;
				msg << "WorkerThread::ftp: " << __LINE__
					<< " exception caught: " << e.what() << "\n"
					<< " remote full file name  = \"" << _remoteUrls[i] << "\"\n"
					<< " local full file name  = \"" << localPath << "\"\n"
					<< " attempt number " << attempt << "; " ;
				
				if( (currentRetryInt+ftpRetryingInt) > ftpTimeout ) {
					
					msg << "TIMEOUT EXPIRED";
					try {
						ftpHandler.rollbackConnection();
					}
					catch( exception &ee )
					{						
						msg << "\n Even failed to rollback: " << ee.what() ;
					}
					ACS_LOG_WARNING(msg.str()) ;
					throw;
				}
				
				msg << "going to wait " << ftpRetryingInt << " seconds and then try again";
				ACS_LOG_WARNING(msg.str()) ;
				
				sleep(ftpRetryingInt); // PRQA S 3000
				currentRetryInt += ftpRetryingInt;
			}
			catch( exception &e )
			{
				try {
					ftpHandler.rollbackConnection();
				}
				catch( exception &ee )
				{
					ACS_LOG_ERROR(e.what() << "\n" << "Even failed to rollback: " << ee.what()) ;
				}
				throw;	
			}
		} // while( tryFtpOperation ) 
		
	} // for( size_t i = 0; i < _remoteUrl.size(); i++ )
	ftpHandler.closeConnection();
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, transferredFilesNr << " files successfully transferred") ;	
}


void WorkerThread::copy() // PRQA S 4211
{
	vector<string> sources;
	vector<string> dests;	
	mode_t mode = 0444;
	const mode_t umask = os::ProcessStatus::instance()->getStartupUmask();
	switch(_op)
	{
    case IsmSoapClient::UPLOAD:
        {
		    sources = _localpaths;
		    dests = _remoteUrls;
        	break;
        }
    case IsmSoapClient::DOWNLOAD:
        {
		    sources = _remoteUrls;
		    dests = _localpaths;
			mode = 0664 & ~umask; // PRQA S 4400
        	break;
        }
	}
	
	// needed for rollback
	vector<string> copiedFiles;
    size_t sources_size = sources.size() ;
	for( size_t i = 0; i < sources_size; ++i ) {
		if( dests[i].empty() ) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "source file \"" << sources[i] << "\" not to be copied because destination path is empty: skipping") ;
			continue;
		}	
		try {
			
			ACS_COND_THROW(haltImmediate(), ThreadException("Immediate stop requested")) ; 

			if( _packagingAlgoOnTheFly.empty() ) {
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "going to copy \"" << sources[i] << "\" into \"" << dests[i] << "\" path") ;
				File::copy(sources[i], dests[i]);
				
				copiedFiles.push_back(dests[i]);

				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "going to change permissions for file \"" << dests[i] << "\" to " << oct << mode) ;
				File::chmod(dests[i], mode);

			}
			else { // in this case one item only occurs for tar-on-the-fly
				PackagingAlgo *packagingAlgo = PackagingAlgoSingleton::instance()->newPackagingAlgo(StringUtils::lowercase(_packagingAlgoOnTheFly),  _parent.getPackageAlgoArchiveOptions(),   _parent.getPackageAlgoExtractOptions());
				ACS_COND_THROW( ! packagingAlgo, exIllegalValueException("Can't Instantiate object with \"" + _packagingAlgoOnTheFly + "\" key: got NULL pointer from PackagingAlgoFactory") );	 // PRQA S 3081
				
				switch(_op)
				{
    			case IsmSoapClient::UPLOAD:
        			{
						ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to pack \"" << sources[i] << "\" (size=" << _fileDefinition.size << " bytes) as \"" << dests[i] << "\" path with \"" << _packagingAlgoOnTheFly << "\" algorithm") ;
						packagingAlgo->pack( sources[i], dests[i], _parent.getPackageAlgoTimeout(), _fileDefinition.size ) ;
						
						copiedFiles.push_back(dests[i]);

						ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "going to change permissions for file \"" << dests[i] << "\" to " << oct << mode) ;
						File::chmod(dests[i], mode);

        				break;
        			}
    			case IsmSoapClient::DOWNLOAD:
        			{
						string localPath = createTemporaryDirectory( File::getDirPath( dests[i]) );
						FileGuard fg(localPath) ;
						ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
							fg.detach();
						ACS_CLASS_END_DEBUG
						ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to extract \"" << sources[i] << "\" in \"" << localPath << "\" path with \"" << _packagingAlgoOnTheFly << "\" algorithm") ;

						packagingAlgo->unpack( sources[i], localPath, _parent.getPackageAlgoTimeout() );
						
						mode_t dirMode = 0775 & ~umask;
						renameUntaredItem( localPath, dests[i], mode, dirMode ) ;
						copiedFiles.push_back(dests[i]);

        				break;
        			}
				}
			}
		}
		catch (exception &e)
    	{

			// rollback
			ostringstream exMsg;
			size_t j = 0;
			try {
                size_t copiedFiles_size = copiedFiles.size() ;
				for( j = 0; j < copiedFiles_size; ++j ) { // PRQA S 4230 
					File::remove(copiedFiles[j]);
                }
				exMsg << "All so far copied files (" << copiedFiles.size() << ") successfully removed";
			}
			catch (exception &ee)
    		{
				exMsg 	<< "Even error trying to remove \"" << copiedFiles[j] << "\" copied file:\n"
						<< ee.what() << "\n"
						<< "Already copied files [" << j << ":" << copiedFiles.size()-1 << "] will not be removed" ; // PRQA S 3084
			}
			
			if( (_op == IsmSoapClient::DOWNLOAD) && (getErrno(e) == ENOENT) ) {
				ostringstream msg;
				msg << "\"" << sources[i] << "\" file to copy into local path no more occurring: "
					<< e.what() << "\n" << exMsg.str();
        		ACS_THROW( IsmSoapClient::FileToDownloadDisappearedExc(e, msg.str()) ); // PRQA S 3081
			}
			
			ACS_THROW( ThreadException(e, exMsg.str()) ); // PRQA S 3081
    	}

	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, copiedFiles.size() << " files successfully copied") ;
}

void WorkerThread::link()
{
	ACS_COND_THROW( not _packagingAlgoOnTheFly.empty(), ThreadException("MISCONFIGURATION: cannot execute pack-on-the-fly with hard link transfer mode. Please fix!")) ; 

	vector<string> sources;
	vector<string> dests;	
		
	mode_t mode = 0444;
	switch(_op)
	{
    case IsmSoapClient::UPLOAD:
        {
		    sources = _localpaths;
		    dests = _remoteUrls;
        	break;
        }
    case IsmSoapClient::DOWNLOAD:
        {
		    sources = _remoteUrls;
		    dests = _localpaths;

    // ############### WARNING: this mode will be used for copy only i.e. if hard link fails!!		
		    mode_t umask = os::ProcessStatus::instance()->getStartupUmask();
		    mode = 0664 & ~umask; // PRQA S 4400
		
        	break;
        }

	}
	
	vector<string> copiedFiles;
	string source;
	try {
		
		const bool copyOnError = true;
		bool fileCopied = false;
		bool printWarning = true;
        size_t s_size = sources.size() ;
		for( size_t i = 0; i < s_size; ++i ) {
			
			ACS_COND_THROW(haltImmediate(), ThreadException("Immediate stop requested")) ; 

			if( dests[i].empty() ) {
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "source file \"" << sources[i] << "\" not to be hard-linked because destination path is empty: skipping") ;
				continue;
			}

			source = sources[i];
			if( File::isSymbolicLink( source, false ) ) {
				string tmp = File::readtargetlink( source, true );
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "source file \"" << source << "\" is a symbolic link; the target file is \"" << tmp << "\"") ;
				source = tmp;
			}

			ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "going to hard link \"" << source << "\" to \"" << dests[i] << "\" path") ;
					
			File::hardlink(source, dests[i], copyOnError, &fileCopied);
			if( fileCopied && printWarning ) {
				ACS_LOG_WARNING("WorkerThread::link: " << __LINE__ 
					<< " copy INSTEAD OF hard link performed in upload of \"" 
					<< source << "\" path into \"" << dests[i] 
					<< "\" path since they do not belong to the same file system.\n"
					<< "Next failed hard link attempts in current sessions, if any, will not be reported." ) ; 
				printWarning = false;
			}
			copiedFiles.push_back(dests[i]);

// ######## WARNING: if downloading file with hard link, no need of change permissions since they are 0444			
			if( ( (_op == IsmSoapClient::UPLOAD)   && !_parent.getNoChangingHardlinkedFilesMode() ) || 
                ( (_op == IsmSoapClient::DOWNLOAD) && fileCopied) ) {
				ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "going to change permissions for " << ( fileCopied ? "COPIED" : "HARD LINKED" ) << " \"" << dests[i] << "\" file in " // PRQA S 3380
							<< ( ( _op == IsmSoapClient::UPLOAD ) ? "IsmSoapClient::UPLOAD": "IsmSoapClient::DOWNLOAD" ) << " to " << oct << mode) ;
				File::chmod(dests[i], mode);
			}

		}

		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, copiedFiles.size() << " files successfully transferred") ;

	} // try
	catch(exception &e)
	{
	// rollback
		try {
            size_t copiedFiles_size = copiedFiles.size();
			for( size_t i = 0; i < copiedFiles_size; ++i ) {
				File::remove(copiedFiles[i]);
            }
		}
		catch (exception &ee)
    	{
			ACS_LOG_ERROR("Error trying to remove just transferred files: " << ee.what()) ;
		}

		if( (_op == IsmSoapClient::DOWNLOAD) && (getErrno(e) == ENOENT) ) {
			ostringstream msg;
			msg << "\"" << source << "\" file to copy into local path no more occurring" ;
        	ACS_THROW( IsmSoapClient::FileToDownloadDisappearedExc(e, msg.str()) );
		}
		
		throw;		
	}

}

#ifdef EXPRIVIA_ISM_USE_AWS_S3        	
void WorkerThread::aws_s3()
{
	

	int awsS3Timeout = _parent.getAwsS3TotalTimeout_s() ; 
	int awsS3RetryingInt = _parent.getAwsS3RetryingInterval_s() ; 
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "pwdEncryptionAlgo = 0x" << hex << int(_parent.getPwdEncryptionAlgo())) ; // PRQA S 3081
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "AWS S3 total timeout " << awsS3Timeout << " [s]") ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "AWS S3 retrying interval " << awsS3RetryingInt << " [s]") ;

	// decrypting passwd
 	string tmp = _parent.getFtpPassword() ;
	const char* encPassword = tmp.c_str() ;
	char decPassword [1024] ;  // PRQA S 4403
	memset( decPassword, 0, sizeof(decPassword) ) ;	/* clear buffer */
	ACS_SSL::EncryptionAlgo algo = _parent.getPwdEncryptionAlgo() ;
   
	// Get decPassword from encPassword  
	if ( encPassword  && (0 != encPassword[0]) ) // exists and is not empty
	{
		vector <void*> vin ;
        vector <void *> vout ;
		BF_KEY bfkey ; // PRQA S 4102
		unsigned char key_in[] = { 'm', 'a', 'n', 'u', 'e', 'l' } ;	/* Blowfish passphrase. Provided by RobRic */
		int key_len = 6 ;
		
		ACS_SSL::ACS_BLOWFISH_setkey( key_in, key_len, &bfkey) ;	/* Get Actual key */
		vin.push_back ( &bfkey ) ;
		
		ACS_COND_THROW( strlen(encPassword)>sizeof(decPassword),ThreadException("ftp password too long") ) ; // PRQA S 3081
   		
		size_t s = ACS_SSL::ACS_ALGO_decode( algo, 
                                             reinterpret_cast<const unsigned char*>(encPassword), // PRQA S 3030, 3081
                                             strlen(encPassword), 
                                             reinterpret_cast<unsigned char*>(decPassword),  // PRQA S 3030, 3081
                                             vin, 
                                             vout ) ;
		ACS_COND_THROW( s < 0, ThreadException("password decoding failure") ) ; // PRQA S 3081
   
    	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Password decrypted successfully.")  ;
#if(0)
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Decrypted password: \"" << decPassword << "\"")  ;
#endif  
	}
	else
	{
		// Nothing to do, since decPassword is zero-filled
	}
					
	string ftpPassword;
	if( 0 != decPassword[0] ) {
		ftpPassword = decPassword;	
    }
	
	AwsS3Wrapper::AwsS3WrapperConfiguration awsS3WrapperConfiguration = _parent.getAwsS3WrapperConfiguration() ; 
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "AwsS3WrapperConfiguration filled by parent:\n" << awsS3WrapperConfiguration );
	ACS_SMARTPTR<AwsS3Wrapper> s3handler_p ;
	fillS3Handler( awsS3WrapperConfiguration, s3handler_p, *this, &_op) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "AwsS3Wrapper download_pool_size=" << s3handler_p->get_download_pool_size() << " download_buffer_size=" << s3handler_p->get_download_buffer_size()
					<< " upload_pool_size=" << s3handler_p->get_upload_pool_size() << " upload_buffer_size=" << s3handler_p->get_upload_buffer_size() );

	_parent.setCache_config_endpoint(awsS3WrapperConfiguration.get_aws_endpoint());
	_parent.setCache_config_region(awsS3WrapperConfiguration.get_aws_region());
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Parent AwsS3WrapperConfiguration:\n" << _parent.getAwsS3WrapperConfiguration() );

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "going to transfer possibly " << _localpaths.size() << " files") ;	
	
	size_t transferredFilesNr = 0;
    size_t lp_size = _localpaths.size(); 
	vector<string> transferredItems ;

	for( size_t i = 0; i < lp_size; ++i ) {
		
		if( _remoteUrls[i].empty() ) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Local file \"" << _localpaths[i] << "\" not to be transferred because remote url is empty: skipping") ;
			continue;
		}
		
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "remote full file name  = \"" << _remoteUrls[i] << "\" "
				   << " local full file name  = \"" << _localpaths[i] << "\" original name = \"" << _fileDefinition.originalFilename << '\"') ;
		
		int currentRetryInt = 0	;	
		bool tryOperation = true;	
		
		while( tryOperation ) {

			int attempt = 1 + currentRetryInt / awsS3RetryingInt;

			try {
				
				ACS_COND_THROW(haltImmediate(), ThreadException("Immediate stop requested")) ; 
				
				string s3_bucket;
				string s3_key ;
				AwsS3Wrapper::absolute_to_relative_path(_remoteUrls[i], s3_bucket, s3_key) ;
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "bucket=\"" << s3_bucket << "\" s3_key=\"" << s3_key << '\"');

				switch(_op)
				{
                case IsmSoapClient::UPLOAD:
                    {
						ACS_COND_THROW( _fileDefinition.originalFilename.empty(), exIllegalValueException("Original filename in file definition is empty: wrong use, please check") );
						
						multimap <AwsS3Wrapper::Enum_S3MetadataTag, string> tagsMap;
						tagsMap.insert( pair<AwsS3Wrapper::Enum_S3MetadataTag, string>(AwsS3Wrapper::S3TAG_TO_BE_RENAMED, _fileDefinition.originalFilename) ) ;
						{
							ostringstream os;
							os << _fileDefinition.fileId;
							tagsMap.insert( pair<AwsS3Wrapper::Enum_S3MetadataTag, string>(AwsS3Wrapper::S3TAG_ID1_APPL, os.str()) ) ;
						}
						tagsMap.insert( pair<AwsS3Wrapper::Enum_S3MetadataTag, string>(AwsS3Wrapper::S3TAG_ID2_APPL, _fileDefinition.filePath) ) ;
						
						s3handler_p->uploadObject( _localpaths[i], s3_bucket, s3_key, tagsMap ) ;
						transferredItems.push_back(_remoteUrls[i]); 

					    tryOperation = false;
                        break;
                    }
                case IsmSoapClient::DOWNLOAD:
                    {
						
						const off64_t sz_download ( s3handler_p->downloadObject(s3_bucket, s3_key, _localpaths[i] ) ) ;
						ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "URL \"" << s3_key << "\" downloaded into \"" << _localpaths[i] << "\" path" );
						transferredItems.push_back(_localpaths[i]); 

					    tryOperation = false;
                        break;
                    }
				}

				transferredFilesNr++;
				if( !tryOperation ) {
    				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "attempt number " << attempt << "; operation successfull") ; 					
					break;
				}

			}
			catch( AwsS3Wrapper::AwsS3WrapperException &e )
			{
				ostringstream msg	;
				msg << SimpleDebugSignature
					<< " exception caught: " << e.what() << "\n"
					<< " remote full file name  = \"" << _remoteUrls[i] << "\"\n"
					<< " local full file name  = \"" << _localpaths[i] << "\"\n"
					<< " attempt number " << attempt << "; " ;
				
				if( (currentRetryInt+awsS3RetryingInt) > awsS3Timeout ) {
					
					if( IsmSoapClient::UPLOAD == _op ) {
						aws_s3_rollBack( s3handler_p.get(), transferredItems);
					}
					else {
						local_rollBack(transferredItems);
					}
					ACS_LOG_WARNING("TIMEOUT EXPIRED: " << msg.str()) ;
					throw;
				}
				
				msg << "going to wait " << awsS3RetryingInt << " seconds and then try again";
				ACS_LOG_WARNING(msg.str()) ;
				
				timeoutOrCancel( awsS3RetryingInt*1000 ) ; // PRQA S 3000
				currentRetryInt += awsS3RetryingInt;
			}
			catch( exception &e )
			{
				if( IsmSoapClient::UPLOAD == _op ) {
					aws_s3_rollBack( s3handler_p.get(), transferredItems);
				}
				else {
					local_rollBack(transferredItems);
				}
				throw;	
			}
		} // while( tryOperation ) 
		
	} // for( size_t i = 0; i < _remoteUrl.size(); i++ )

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, transferredFilesNr << " files successfully transferred") ;	

}

void WorkerThread::aws_s3_rollBack( AwsS3Wrapper* s3_handler_p,  const vector<string> & pathToRemove_V ) 
{
	const size_t itemsToRemoveSize = pathToRemove_V.size();
	for ( size_t i = 0; i < itemsToRemoveSize ; ++i ) {
		try {

			string bucket, s3KeyToRemove ;
			AwsS3Wrapper::absolute_to_relative_path(pathToRemove_V.at(i), bucket, s3KeyToRemove) ;

			// Remove from AWS S3, if the entry exists	
  			if ( true == s3_handler_p->isFileObject(bucket, s3KeyToRemove) ) 
  			{
  				s3_handler_p->removeSingleObject(bucket, s3KeyToRemove) ;
  			}

		}
		catch( exception &e)
		{
			ACS_LOG_NOTIFY_EX(e);
		    ACS_LOG_ERROR( "Failed to remove \"" << pathToRemove_V.at(i) << "\"; so far uploaded " << itemsToRemoveSize << " files" );

		}
	}
	
} 

#endif			



bool WorkerThread::completed() const throw() // PRQA S 4120
{
	return _completed ;
}


IsmSoapClient::TransferMode WorkerThread::getActualTransferMode() const
{
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ; 
	
	bool upload = true;
	IsmSoapClient::TransferMode configuredMode = IsmSoapClient::FTP ;		
	switch(_op)
	{
    case IsmSoapClient::UPLOAD:
        {
		    upload = true;
		    configuredMode = _parent.getUploadMode();
        	break;
        }
    case IsmSoapClient::DOWNLOAD:
        {
		    upload = false;
		    configuredMode = _parent.getDownloadMode();
        	break;
        }
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "configured transfer mode for " // PRQA S 3380 
			<< (upload ? "upload" : "download") << ": " << IsmSoapClient::getTransferModeString(configuredMode)) ;
	IsmSoapClient::TransferMode actualMode = configuredMode;

	switch (configuredMode)  {
    	
    case IsmSoapClient::FTP:
        {
			break;
		}
    case IsmSoapClient::COPY:
    case IsmSoapClient::LINK:
        {
		    // if Client host does not match with Server host
		    if( not _parent.clientAndServerSameHost( _parent.getHost() ) ) {

			    actualMode = IsmSoapClient::FTP;

			    ACS_LOG_WARNING(SimpleDebugSignature // PRQA S 3380
				    << " configured transfer mode " << IsmSoapClient::getTransferModeString(configuredMode) << " for "
				    << (upload ? "upload" : "download") << " cannot be accomplished since Client host ("
				    << HWScan::hostname() << ") does not match with Server host (" <<  _parent.getHost()
				    << "): transfer mode set to " << IsmSoapClient::getTransferModeString(actualMode)) ;
		    }
		    else {
			    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Client host (" << HWScan::hostname() << ") matches with Server host (" << _parent.getHost() << ")") ;
		    }
			
			break;	
		}
    case IsmSoapClient::AWS_S3:
        {
			break;
		}			
    default:
        {
			ACS_THROW( exIllegalValueException( "Internal ERROR: desired transfer mode unrecognized" ) ); // PRQA S 3081
        }
	}
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB,"Going to " << (upload ? "upload" : "download") << " files with the following transfer mode: " << StringUtils::uppercase(IsmSoapClient::getTransferModeString(actualMode)) );	
	return actualMode;
	
}		

void WorkerThread::stopWork() throw()
{
	try {
		if( !_stopRequested ) {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Going to stop worker thread" ) ;
			stop();
			_stopRequested = true;
			ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Going to wait thread exit with timeout of " << stopTimeout_msecs << " ms" ) ;
			if( !waitExitOrTimeout(stopTimeout_msecs) ) {
				ACS_LOG_WARNING("Timeout reached trying to stop worker thread (" << stopTimeout_msecs << " ms)");
			}
		}
		else {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Stop already requested" ) ;
		}
	}
	catch( exception &e) 
	{
		ACS_LOG_ERROR("Cannot stop worker thread: " << e.what() );
	}

}

#ifdef EXPRIVIA_ISM_USE_AWS_S3
void WorkerThread::local_rollBack(const vector<string> & pathsToRemove ) 
{
	const size_t itemsToRemoveSize = pathsToRemove.size();
	for ( size_t i = 0; i < itemsToRemoveSize ; ++i ) {
		try {
			File::remove(pathsToRemove[i]);
		}
		catch( exception &e)
		{
			ACS_LOG_NOTIFY_EX(e);
		    ACS_LOG_ERROR( "Failed to remove \"" << pathsToRemove[i] << "\"; so far downloaded " << itemsToRemoveSize << " files" );
		}
	}
	
}
#endif

string WorkerThread::createTemporaryDirectory( const string & rootPath )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ; 
	string dirPrefix = StringUtils::pathJoin(rootPath, "SoapClntDwnl_XXXXXX" );
	char tmpl[4096] ;
	strcpy(tmpl, dirPrefix.c_str()) ;
	char *res = mkdtemp(tmpl);
	if( not res ) {
		ostringstream exMsg;
		exMsg << "Cannot create temporary directory in \"" << rootPath << "\" root";
		ACS_THROW( exSystemException(exMsg.str(), errno) );
	}
	string tempDir = res;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << tempDir << "\" temporary directory just created");
	return tempDir ;
}

void WorkerThread::renameUntaredItem( const string & untaredItemDir, const string & localpath, mode_t fileMode, mode_t dirMode )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ; 
	Dir dir(untaredItemDir);
	if( dir.size() != 1 ) {
		ostringstream exMsg;
		exMsg << dir.size() << " items found in \"" << untaredItemDir << "\" downloaded directory: 1 expected";
		ACS_THROW( exIllegalValueException( exMsg.str() ) );
	}
	string oldPath = StringUtils::pathJoin( untaredItemDir, dir[0] ) ;
	
	// change mode
	if( File::isDir( oldPath ) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << oldPath << "\" is a directory");
		DirTree d (oldPath, "", true) ;
		size_t d_size = d.size() ;
		for (size_t i=0; i<d_size; ++i) {
			if( File::isDir(d[i]) ) {
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to change permissions for directory \"" << d[i] << "\" to " << oct << dirMode) ;
				File::chmod(d[i], dirMode) ;
			}
			else {
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to change permissions for file \"" << d[i] << "\" to " << oct << fileMode) ;
				File::chmod(d[i], fileMode) ;
			}
		}
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to change permissions for file \"" << oldPath << "\" to " << oct << fileMode) ;
		File::chmod(oldPath, fileMode) ;
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to rename \"" << oldPath << "\" as \"" << localpath << '\"');
	File::rename( oldPath, localpath );

}

File::CompressionMode WorkerThread::getCompressionMode (const string &cMode )  // PRQA S 4020
{
	
// This method has multiple returns to keep its complexity as low as possible
	if( cMode.empty() ) {
		return File::UNDEFINED ;
	}
	else if( StringUtils::equalsNoCase("tar", cMode ) ) {
		return File::Plain ;
	}
	else if( StringUtils::equalsNoCase("targz", cMode ) ) {
		return File::GZipped ;
	}
	else {
		ACS_THROW( exIllegalValueException( "Compression mode not recognized: \""+ cMode + '\"') );
	}
	
}	

pair<string, long long int> getStInfoEl(const Ism__cacheInfo & ci)
{
	return make_pair(ci.fileClass, ci.freeSize);
}


class packageAlgoOptionsMapFiller {
    public:
        packageAlgoOptionsMapFiller(map< string, string > &m) : 
            map_(m) // PRQA S 2528 3
        {}

        packageAlgoOptionsMapFiller(const packageAlgoOptionsMapFiller &r) : map_(r.map_) {} // PRQA S 2528
        ~packageAlgoOptionsMapFiller() ACS_NOEXCEPT {}
		void operator()(const IsmSoapClient::PackageAlgoOption &v) 
        {
            if (map_.find(v.key) == map_.end() ) {
				map_.insert ( make_pair(v.key, v.value) );
            }
            else {
			    // nothing to do
            }

        }
    private:
        packageAlgoOptionsMapFiller() ; // not implemented
        packageAlgoOptionsMapFiller& operator=(const packageAlgoOptionsMapFiller &) ; // not implemented

        map< string, string> & map_ ;
} ;

const int oldServerVersion = 1;

		

} // end unnamed namespace





ACS_CLASS_DEFINE_DEBUG_LEVEL(IsmSoapClient)
const unsigned int IsmSoapClient::_defaultConnectTimeout ; 
const int IsmSoapClient::_defaultFtpTotalTimeout_s ; 
const int IsmSoapClient::_defaultFtpRetryingInterval_s ;
const int IsmSoapClient::_transferOperationTimeout_s ;

bool IsmSoapClient::_emitWarning4FtpTimeout = true;
bool IsmSoapClient::_emitNoChangingHardlinkedFilesMode = true;
ThreadSafe IsmSoapClient::_endPointsMutex;
const int IsmSoapClient::_defaultSoapRecvTimeout ;
const int IsmSoapClient::_soapSendTimeout ;



void IsmSoapClient::convert(  IsmSoapClient::fileDefinition const & in ,   Ism__fileDefinition & out)
{
	out.fileId   =        in.fileId ;
	out.filePath =	 	  in.filePath ;
	out.isDir = 	 	  in.isDir ;
	out.size =		 	  in.size ;
	out.user =		 	  in.user ;
	out.group =		 	  in.group ;
	out.permissions =	  in.permissions ;
	out.fileClass =  	  in.fileClass ;
	out.packagingAlgo =   in.packagingAlgo ;

	vector<string> &customKeys(out.customKeys.strings) ;
	vector<string> &customValues(out.customValues.strings) ;

	for(  map<string , string>::const_iterator it=in.customAttributes.begin() ; it !=in.customAttributes.end() ; it ++)
	{
		customKeys.push_back(it->first) ;
		customValues.push_back(it->second) ;
	}
}


void IsmSoapClient::convert( IsmSoapClient::fileDefinition const & in ,  
							string const & localDirPath,
							DirTree const & localDirTreeIn,
							vector<string> & localFilepaths,
							Ism__dirDefinition & out )
{
	out.filePath =	 	  in.filePath ;
	out.user =		 	  in.user ;
	out.group =		 	  in.group ;
	out.permissions =	  in.permissions ;
	out.fileClass =  	  in.fileClass ;

	vector<string> &customKeys(out.customKeys.strings) ;
	vector<string> &customValues(out.customValues.strings) ;

	for(  map<string , string>::const_iterator it=in.customAttributes.begin() ; it !=in.customAttributes.end() ; it ++)
	{
		customKeys.push_back(it->first) ;
		customValues.push_back(it->second) ;
	}
	
	vector<string> &fullpaths(out.fullpaths) ;
	vector<int64_t> &sizes = out.sizes ;
	vector<bool> &isDir(out.isDir) ;
	
	DirTree localDirTree = localDirTreeIn;
	
	DirTree remoteDirTree = localDirTreeIn;
	remoteDirTree.relative(in.filePath);
    
    size_t ld_size = localDirTree.size() ;
				
	for (size_t i=0; i < ld_size; ++i) {
		
		string localPath( localDirTree[i] );
		localFilepaths.push_back( localPath );
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "local file \"" << localPath << "\" inserted into local paths vector") ;

		string remoteFilename( remoteDirTree[i] );
		// fill parallel vectors 
		fullpaths.push_back(remoteFilename);
		if( File::isDir(localPath) ) {
			isDir.push_back(true);
			sizes.push_back(0);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "file \"" << remoteFilename << "\" (directory) inserted into fileDefinition vectors") ;
		}
		else {
			isDir.push_back(false);
			sizes.push_back(File::size(localPath));
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "file \"" << remoteFilename << "\" (file) with size " << File::size(localPath) 
						<< " bytes inserted into fileDefinition vectors") ;
		}
		
	}
	
}

void IsmSoapClient::convert(const Ism__fileAttributes & attributes, struct stat & stbuf)
{
// initialize
	memset(&stbuf, 0, sizeof(struct stat));

	stbuf.st_ino = attributes.fileId;
	stbuf.st_uid = attributes.userId; 
	stbuf.st_gid = attributes.groupId;
	
	int protection = 0;
	{
		ostringstream os;
		os << attributes.permissions;
		istringstream is ( os.str() ); 
		is >> oct >> protection;  // e.g. "777" is turned in 0777 (i.e. decimal 511)
	}
	if( attributes.isDir ) {
		stbuf.st_mode = S_IFDIR | protection;
		stbuf.st_size = 0;
	}
	else {
		stbuf.st_mode = S_IFREG | protection;
		stbuf.st_size = attributes.size;
	}
	
	stbuf.st_atime = attributes.lastAccessTime;
	stbuf.st_mtime = attributes.modificationTime;
	stbuf.st_ctime = attributes.creationTime;
	
}

void IsmSoapClient::convert(const struct stat & stbuf, Ism__fileAttributes & attributes )
{
	 attributes.fileId = stbuf.st_ino ;
	 attributes.userId = stbuf.st_uid ;
	 attributes.groupId = stbuf.st_gid ;
	 
	 {
		ostringstream os;
		os << oct << (stbuf.st_mode & mode_t(0777));
		istringstream is ( os.str() );
		is >> attributes.permissions;
	}
	
	attributes.size = stbuf.st_size;
	attributes.lastAccessTime = stbuf.st_atime;
	attributes.modificationTime = stbuf.st_mtime ;
	attributes.creationTime = stbuf.st_ctime ;
	
	attributes.isDir = (stbuf.st_mode & S_IFDIR) ; 
}

void IsmSoapClient::convert(const Ism__DownloadFileRequest & in, const string & clientReference, Ism__DownloadFileRequest_2 & out)
{
	out.fileId = in.fileId;
	out.user = in.user ;
	out.group = in.group;
	out.requestId = in.requestId ;
	out.updateRequestTime = in.updateRequestTime ;
	out.checkPermissions = in.checkPermissions ;
	out.clientReference = clientReference ;

}

//*********************************************  IsmSoapClient   *********************************************



IsmSoapClient::IsmSoapClient(StopController const * sc) : 
    _host(),
	_port0(0),
	_port1(0), 
	_defaultTimeout(600), // PRQA S 4400
    _user(),
    _ftpUser(),
    _ftpPassword(),
	_pwdEncryptionAlgo(),
	_passive(true), 
	_sc(sc), // PRQA S 2528
	_connectTimeout_ms(_defaultConnectTimeout),
	_ftpTotalTimeout_s(_defaultFtpTotalTimeout_s),
	_ftpRetryingInterval_s(_defaultFtpRetryingInterval_s),
	_uploadMode(FTP),
	_downloadMode(FTP),
	_noChangingHardlinkedFilesMode(false),
	_endPointsKey(0),
#ifdef EXPRIVIA_ISM_USE_AWS_S3
	_awsS3TotalTimeout_s (_defaultFtpTotalTimeout_s),
    _awsS3RetryingInterval_s(_defaultFtpRetryingInterval_s),
	_awsS3WrapperConfiguration(),
#endif
	_packageAlgo(),
	_packageExt(),
	_packageAlgoTimeout(600),
	_packageAlgoArchiveOptions(),
	_packageAlgoExtractOptions(),
	_soapRecvTimeout(_defaultSoapRecvTimeout),
	_serverVersion(-1),
	_clientReference()
{
   _pwdEncryptionAlgo = ACS_SSL::ENC_NONE ;
   ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}



IsmSoapClient::~IsmSoapClient() throw() {
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ; // PRQA S 4631
}




void IsmSoapClient::setPort(int p)
{
	_port0 = p ;
	prepareEndpoint(0) ;
}

void IsmSoapClient::setPort2(int p)
{
	_port1 = p ;
	prepareEndpoint(1) ;
}


void IsmSoapClient::setHost(string const & h)
{
	_host = h ;
	prepareEndpoint() ;
}



void IsmSoapClient::setUser(std::string const &u) // PRQA S 4121
{
	_user = u ;
}

void IsmSoapClient::setFtpUser(std::string const &u) // PRQA S 4121
{
	_ftpUser = u ;
}


void IsmSoapClient::setFtpPassword(std::string const &p) // PRQA S 4121
{
	_ftpPassword = p ;
}

void IsmSoapClient::setEncryptionAlgo(int i) // PRQA S 4121
{
	_pwdEncryptionAlgo = static_cast<ACS_SSL::EncryptionAlgo>(i); // PRQA S 3013, 3081
}

void IsmSoapClient::setPassive(bool v) // PRQA S 4121
{
	_passive = v;
}

void IsmSoapClient::setConnectTimeout(unsigned int msec)
{
	_connectTimeout_ms = msec;
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_connectTimeout_ms = " << _connectTimeout_ms << " ms") ;
}

void IsmSoapClient::setConnParameters(rsResourceSet const &rset, long long int smId) 
{
	string host ;
	rset.getValue("SMConfiguration.ServerUrl",host);
	int port1 = 0 ;
	rset.getValue("SMConfiguration.port",port1);
	int port2 = 0  ;
	try {
		rset.getValue("SMConfiguration.port2",port2);
	}
	catch (rsResourceSet::NotFoundException &) {
		// do nothing .. no port2 
	}
	string user ;
	rset.getValue("SMConfiguration.user", user);
	string password ;
	rset.getValue("SMConfiguration.password", password);
	string ftpuser ;	
	rset.getValue("SMConfiguration.FtpUser", ftpuser);
	string ftppassword ;	
	rset.getValue("SMConfiguration.FtpPassword", ftppassword);
	int timeout = 0 ;
	rset.getValue("SMConfiguration.TapeRetrievalTimeOut", timeout);
	bool passive = true ;
	try {
		rset.getValue("SMConfiguration.Passive", passive);
	}
	catch (rsResourceSet::NotFoundException & )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Passive missing: ASSUMING: " << boolalpha << passive) ;
	}
	
	unsigned int connTimeout =  _defaultConnectTimeout;
	try {
		rset.getValue("SMConfiguration.ConnectionTimeOut", connTimeout);
	}
	catch (rsResourceSet::NotFoundException & )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ConnectionTimeOut missing: ASSUMING DEFAULT VALUE: "
					<<  connTimeout << " [msec]") ;
	}

	try
	{
		int i = 0 ;
		rset.getValue("SMConfiguration.passwordEncryptionAlgo", i) ; 
		setEncryptionAlgo(i);
	}
	catch (rsResourceSet::NotFoundException & )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "passwordEncryptionAlgo missing: ASSUMING UNENCRYPTED") ;
		_pwdEncryptionAlgo = ACS_SSL::ENC_NONE ;
	}
	
	bool ftpTimeoutParmsMissing = false;
	
	int ftpTotalTimeout = 0 ;
	try {
		rset.getValue("SMConfiguration.ftpTotalTimeOut", ftpTotalTimeout);
		if( ftpTotalTimeout < 0 ) {
			ACS_LOG_WARNING("ftpTotalTimeOut invalid value: " << ftpTotalTimeout
				<< " (>=0 expected). Setting default value: " << _defaultFtpTotalTimeout_s << " [s]") ;
			ftpTotalTimeout =  _defaultFtpTotalTimeout_s;
		}
		
	}
	catch (rsResourceSet::NotFoundException & )
	{
		ftpTimeoutParmsMissing = true;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ftpTotalTimeOut missing in configuration: ASSUMING DEFAULT VALUE: "
					<<  _defaultFtpTotalTimeout_s << " [s]") ;
		ftpTotalTimeout =  _defaultFtpTotalTimeout_s;
	}

	int ftpRetryingInterval = 0 ;
	try {
		rset.getValue("SMConfiguration.ftpRetryingInterval", ftpRetryingInterval);
		if( ftpRetryingInterval <= 0 ) {
			ACS_LOG_WARNING("ftpRetryingInterval invalid value: " << ftpRetryingInterval
				<< " (>0 expected). Setting default value: " << _defaultFtpRetryingInterval_s << " [s]") ;
			ftpRetryingInterval =  _defaultFtpRetryingInterval_s;
		}
	}
	catch (rsResourceSet::NotFoundException & )
	{
		ftpTimeoutParmsMissing = true;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ftpRetryingInterval missing in configuration: ASSUMING DEFAULT VALUE: "
					<<  _defaultFtpRetryingInterval_s << " [s]") ;
		ftpRetryingInterval =  _defaultFtpRetryingInterval_s;
	}
	
	if( ftpTimeoutParmsMissing && _emitWarning4FtpTimeout ) {
		ACS_LOG_WARNING("either ftpTotalTimeOut or ftpRetryingInterval parameters (or both) MISSING IN CONFIGURATION; "
			<< "SETTING VALUES " << ftpTotalTimeout << " [s], " << ftpRetryingInterval << " [s]") ;
		_emitWarning4FtpTimeout = false;
	}

		
	string uploadModality;
	try {
		rset.getValue("SMConfiguration.uploadModality", uploadModality);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SMConfiguration.uploadModality: " << uploadModality ) ;
	}
	catch (rsResourceSet::NotFoundException & )
	{}
	
	string downloadModality;
	try {
		rset.getValue("SMConfiguration.downloadModality", downloadModality);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "SMConfiguration.downloadModality: " << downloadModality) ;
	}
	catch (rsResourceSet::NotFoundException & )
	{}
	
	if( rset.hasKey("SMConfiguration.OnTheFlyPackage") ) {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "SMConfiguration.OnTheFlyPackage node found");
	
		try {
			string tmp ;
			rset.getValue("SMConfiguration.OnTheFlyPackage.algo", tmp);
			_packageAlgo = StringUtils::lowercase(tmp);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "SMConfiguration.OnTheFlyPackage.algo: \"" << _packageAlgo << '\"') ;
		}
		catch (rsResourceSet::NotFoundException & )
		{}

		try {
			rset.getValue("SMConfiguration.OnTheFlyPackage.ext", _packageExt);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "SMConfiguration.OnTheFlyPackage.ext: \"" << _packageExt << '\"') ;
		}
		catch (rsResourceSet::NotFoundException & )
		{}

		try {
			rset.getValue("SMConfiguration.OnTheFlyPackage.algoTimeout", _packageAlgoTimeout);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "SMConfiguration.OnTheFlyPackage.algoTimeout: " << _packageAlgoTimeout << " [s]") ;
		}
		catch (rsResourceSet::NotFoundException & )
		{}
	
	// fill here options maps
		vector<PackageAlgoOption> archiveOptions;
		size_t optionsElements = rset.getArray("SMConfiguration.OnTheFlyPackage.archiveOptions.option", archiveOptions); // PRQA S 4412
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "<" << optionsElements << "> archiveOptions elements just read from SMConfiguration.OnTheFlyPackage.archiveOptions node");
		if( optionsElements > 0 ) {
			packageAlgoOptionsMapFiller f(_packageAlgoArchiveOptions)  ;
			for_each(archiveOptions.begin(), archiveOptions.end(), f) ;
		}

		vector<PackageAlgoOption> extractOptions;
		optionsElements = rset.getArray("SMConfiguration.OnTheFlyPackage.extractOptions.option", extractOptions); // PRQA S 4412
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "<" << optionsElements << "> extractOptions elements just read from SMConfiguration.OnTheFlyPackage.extractOptions node");
		if( optionsElements > 0 ) {
			packageAlgoOptionsMapFiller f(_packageAlgoExtractOptions)  ;
			for_each(extractOptions.begin(), extractOptions.end(), f) ;
		}
	
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "SMConfiguration.OnTheFlyPackage node not found");
	}
	
	
	ACS_COND_THROW( ( not _packageAlgo.empty() && _packageExt.empty()) , exIllegalValueException("Misconfiguration: on the fly package algo set to \""+_packageAlgo+"\" but package extension not set. Please fix" ) );

	checkLocalTransferModes(host, rset, uploadModality, downloadModality) ;
	if( not uploadModality.empty() ) {
		_uploadMode = getTransferModeEnum(uploadModality);
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "uploadModality missing in configuration; setting default: " << getTransferModeString(_uploadMode) ) ;
	}
	if( not downloadModality.empty() ) {
		_downloadMode = getTransferModeEnum(downloadModality);
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "downloadModality missing in configuration; setting default: " << getTransferModeString(_downloadMode) ) ;
	}
	if( _uploadMode == LINK ) {
		try {
			rset.getValue("SMConfiguration.noChangingHardlinkedFilesMode", _noChangingHardlinkedFilesMode);
			if( _noChangingHardlinkedFilesMode && _emitNoChangingHardlinkedFilesMode) {
				ACS_LOG_WARNING("Permissions of files uploaded in cache with hard link WILL NOT be changed") ;
				_emitNoChangingHardlinkedFilesMode = false;
			}
		}
		catch (rsResourceSet::NotFoundException & )
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "noChangingHardlinkedFilesMode missing in configuration: ASSUMING DEFAULT VALUE: " << boolalpha << _noChangingHardlinkedFilesMode) ;
		}
	}
	_endPointsKey = smId;

#ifdef EXPRIVIA_ISM_USE_AWS_S3        	
	int awsS3TotalTimeout = _defaultFtpTotalTimeout_s ;
	try {
		rset.getValue("SMConfiguration.awsS3TotalTimeOut", awsS3TotalTimeout);
		if( awsS3TotalTimeout < 0 ) {
			ACS_LOG_WARNING("awsS3TotalTimeout invalid value: " << awsS3TotalTimeout << " (>=0 expected). Setting default value: " << _defaultFtpTotalTimeout_s << " [s]") ;
			awsS3TotalTimeout =  _defaultFtpTotalTimeout_s;
		}
	}
	catch (rsResourceSet::NotFoundException & )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "awsS3TotalTimeout missing in configuration: ASSUMING DEFAULT VALUE: " <<  awsS3TotalTimeout << " [s]") ;
	}

	int awsS3RetryingInterval = _defaultFtpRetryingInterval_s ;
	try {
		rset.getValue("SMConfiguration.awsS3RetryingInterval", awsS3RetryingInterval);
		if( awsS3RetryingInterval <= 0 ) {
			ACS_LOG_WARNING("awsS3RetryingInterval invalid value: " << awsS3RetryingInterval << " (>0 expected). Setting default value: " << _defaultFtpRetryingInterval_s << " [s]") ;
			awsS3RetryingInterval =  _defaultFtpRetryingInterval_s;
		}
	}
	catch (rsResourceSet::NotFoundException & )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "awsS3RetryingInterval missing in configuration: ASSUMING DEFAULT VALUE: " <<  awsS3RetryingInterval << " [s]") ;
	}
	
#endif

	try {
		rset.getValue("SMConfiguration.SoapRecvTimeout", _soapRecvTimeout);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SMConfiguration.SoapRecvTimeout: " << _soapRecvTimeout << " [ms]" ) ;
		if( _soapRecvTimeout <= 0 ) {
			ACS_LOG_WARNING("SoapRecvTimeout invalid value: " << _soapRecvTimeout << " (>0 expected). Setting default value: " << ((-1)*_defaultSoapRecvTimeout / 1000) << " [ms]") ;
			_soapRecvTimeout =  _defaultSoapRecvTimeout;
		}
		else {
			_soapRecvTimeout *= (-1000); // < 0 gives socket recv timeout in usec  
		}
	}
	catch (rsResourceSet::NotFoundException & )
	{}
	

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "host " << host) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "port " << port1) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "port2 " << port2) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "user " << user) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "password " << password) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ftpuser " << ftpuser) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ftppassword " << ftppassword) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ftp passive mode " << boolalpha << passive) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "passwordEncryptionAlgo 0x" << hex << int(_pwdEncryptionAlgo)) ; // PRQA S 3081
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "tape retrieval timeout " << timeout << " [s]") ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "connection timeout " << connTimeout << " [ms]") ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ftp total timeout " << ftpTotalTimeout << " [s]") ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ftp retrying interval " << ftpRetryingInterval << " [s]") ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "upload transfer mode " << getTransferModeString(_uploadMode)) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "download transfer mode " << getTransferModeString(_downloadMode)) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "not changing hard linked files permissions " << boolalpha << _noChangingHardlinkedFilesMode) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "storage manager Id (end points key) " << _endPointsKey ) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "on the fly package algo \"" << _packageAlgo << '\"' ) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "on the fly package extension \"" << _packageExt << '\"' ) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "on the fly package algo timeout " << _packageAlgoTimeout << " [s]") ;
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		if( _packageAlgoArchiveOptions.size() != 0 ) {
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << " package algo archive options map" );
			for (map<string,string>::const_iterator it=_packageAlgoArchiveOptions.begin(); it!=_packageAlgoArchiveOptions.end(); ++it) {
				ACS_LOG_DEBUG(SimpleDebugSignatureThread << it->first << " => " << it->second  );
			}
		}
		if( _packageAlgoExtractOptions.size() != 0 ) {
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << " package algo extract options map" );
			for (map<string,string>::const_iterator it=_packageAlgoExtractOptions.begin(); it!=_packageAlgoExtractOptions.end(); ++it) {
				ACS_LOG_DEBUG(SimpleDebugSignatureThread << it->first << " => " << it->second  );
			}
		}
	ACS_CLASS_END_DEBUG
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "soapRecvTimeout " << (-1)*_soapRecvTimeout << " [us]") ;

	ThreadSafe::Lock lock (_endPointsMutex);	
	
	setFtpUser(ftpuser) ;
	setFtpPassword(ftppassword) ;
	setUser(user) ;
	setPort(port1) ;
	setPort2(port2) ;
	setHost(host) ;
	setPassive(passive);
	_defaultTimeout = timeout ; 
	setConnectTimeout(connTimeout);	
	_ftpTotalTimeout_s = ftpTotalTimeout;
	_ftpRetryingInterval_s = ftpRetryingInterval;
	
#ifdef EXPRIVIA_ISM_USE_AWS_S3
	_awsS3TotalTimeout_s = awsS3TotalTimeout ;   
	_awsS3RetryingInterval_s = awsS3RetryingInterval ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "aws S3 total timeout " << _awsS3TotalTimeout_s << " [s]") ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "aws S3 retrying interval " << _awsS3RetryingInterval_s << " [s]") ;
#endif
	
	// ask Server/Singleton its version and set in class attribute
	setServerVersion();	
	
}

void IsmSoapClient::loadXmlConnParameters(std::string const &path, long long int key) 
{
	XMLIstream xmlparam( path );
	rsResourceSet params ;
	xmlparam >> params ;
	
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		oftmpstream tmppar("/tmp/ismcli_" , "par") ;
		tmppar.setAutoRemove(false) ;
		tmppar() << params ; // write it into a tmp file
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "params written in " << tmppar.path() ) ; 
	ACS_CLASS_END_DEBUG

	setConnParameters(params, key) ;
}


void IsmSoapClient::loadRsConnParameters(std::string const &path, long long int key) 
{
	ifstream  iparam( path.c_str() );
	rsResourceSet params ;
	iparam >> params ;

	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		oftmpstream tmppar("/tmp/ismcli_",".par") ;
		tmppar.setAutoRemove(false) ;
		tmppar() << params ; // write it into a tmp file
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "params written in " << tmppar.path()) ;
	ACS_CLASS_END_DEBUG

	setConnParameters(params, key) ;
}



void IsmSoapClient::prepareEndpoint(int which) // PRQA S 4211
{
	ThreadSafe::Lock lock (_endPointsMutex);
	
	map<long long int, EndPoints>::iterator it = endpointsMap.find(_endPointsKey);
	if( it == endpointsMap.end() ) {
		EndPoints ep;
		endpointsMap[_endPointsKey] = ep;
		it = endpointsMap.find(_endPointsKey);
	}
	
	if (which < 0) {
		it->second.clearEndPoints();
    }
		
	if( (which == 0 || which < 0) && not _host.empty() ) { // just for readability see if below
		ostringstream endpointStream ;
		endpointStream << "http://" << _host << ":" << _port0 ;
		it->second.addEndPoint(endpointStream.str()) ;
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "key=" << _endPointsKey << ": endpoint added (which: " << which << "): " << endpointStream.str()) ; 
	}

	if( (which == 1 || which < 0) && not _host.empty() ) { 
		ostringstream endpointStream ;
		endpointStream << "http://" << _host << ":" << _port1 ;
		it->second.addEndPoint(endpointStream.str()) ;
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "key=" << _endPointsKey << ": endpoint added (which: " << which << "): " << endpointStream.str()) ;
	}
	
}


void IsmSoapClient::internalFileUpload( fileDefinition const & f , string const &  localpath , bool recursive, bool checkPermissions, const long long int realFileSize, vector< pair<string , string> > & smEntryKeys )
{
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
	{
		ThreadSafe::Lock lock(_endPointsMutex);
		map<long long int, EndPoints>::iterator it = getEndPointsFromMapIt(_endPointsKey);
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "remote path is " << f.filePath  
				   << " ; local path is " << localpath << "; original name is \"" << f.originalFilename << "\""
				   << " ; endpoints are " << (it->second)[0] << " " << (it->second)[1]) ;
	}
	ACS_CLASS_END_DEBUG

	string  user = f.user ;
	

	IsmProxy q;
	q.send_timeout = _soapSendTimeout;
	q.recv_timeout = _soapRecvTimeout;  

	Ism__uploadResponse uploadResponse ;
	
	
	Timer timer ;
	timer.start() ;
	
	Ism__fileDefinition file ;

#ifdef EXPRIVIA_ISM_USE_AWS_S3        	
	Ism__cacheConfiguration cacheConfiguration ;
#endif
	
	{
		ThreadSafe::Lock lock (_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
		
		for (int counter = 0; ; counter++ ) {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry] << "\"") ;
			try {
				q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;

				convert(f , file) ;
				// in this case f.size is needed to book cache space and, if tar option is set, is estimated
				Ism__uploadSingleFileRequest uploadRequest ;
				uploadRequest.recursive = recursive ;
				uploadRequest.checkPermissions = checkPermissions ;
				uploadRequest.file = &file ;
#ifdef EXPRIVIA_ISM_USE_AWS_S3        	
				uploadRequest.cacheConfiguration = &cacheConfiguration ;
#endif				
				int soap_error_code = -255;
				if ( (soap_error_code = q.uploadSingleFile(&uploadRequest , &uploadResponse)) != SOAP_OK)
				{
					ostringstream error ;
					error 	<< '\"' << localpath << "\" upload SrvErr[ " << soap_error_code << "]: " ;

					if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
						error << "Soap message transfer TIMEOUT" ;
						ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
					}
					
					error << q.soap_fault_string();
					if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
						ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                	}
					string errString = error.str();
					ACS_LOG_ERROR( StringUtils::transform(errString, '\n', ' '));
					ACS_THROW( Exception(errString ) ) ; // PRQA S 3081
				}
				(mapIt->second).setGoodEndPoint(endpointToTry) ;
				break ; // succesfull
			}
			catch(ConnectionException &e) {
				retryConnectionToServer( e.what(), timer, counter+1, endpointToTry);
			}
		}
	} // 		ThreadSafe::Lock lock (_endPointsMutex);

	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Received : upload url "  << uploadResponse.uploadUrl.c_str() << " dumping response : "<<  "\n"
		       << uploadResponse) ;
	
	string uploadUrl = uploadResponse.uploadUrl ;
	int fileId = uploadResponse.file->fileId ;
#ifdef EXPRIVIA_ISM_USE_AWS_S3  
	if( ( _uploadMode == AWS_S3 ) && ( uploadResponse.cacheConfiguration != NULL ) ) {
	   
		_awsS3WrapperConfiguration.set_aws_s3_provider( AwsS3Wrapper::to_S3Provider( uploadResponse.cacheConfiguration->provider ) ); 
		_awsS3WrapperConfiguration.set_aws_key_ak( uploadResponse.cacheConfiguration->key_ak );
		_awsS3WrapperConfiguration.set_aws_key_sk( uploadResponse.cacheConfiguration->key_sk );
		_awsS3WrapperConfiguration.set_aws_region( uploadResponse.cacheConfiguration->region );
		_awsS3WrapperConfiguration.set_aws_endpoint( uploadResponse.cacheConfiguration->endpoint) ; 
		_awsS3WrapperConfiguration.set_endpoint_is_secure( uploadResponse.cacheConfiguration->endpointIsSecure );
		_awsS3WrapperConfiguration.set_options( uploadResponse.cacheConfiguration->options );
		
		
		switch (_serverVersion)  {
    		case 1: 
			{ 
				// Ism__getCacheConfigurationAdditionalParms method not implemented: pool and buffer size must not be set
				ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "No method for cache configuration additional parameters called" );
				_awsS3WrapperConfiguration.set_download_pool_size(0);
				_awsS3WrapperConfiguration.set_download_buffer_size(0);
				_awsS3WrapperConfiguration.set_upload_pool_size(0);
				_awsS3WrapperConfiguration.set_upload_buffer_size(0);
				break;
			}
    		case 2: 
			{ 
				Ism__cacheConfigurationAdditionalParms cacheConfigurationAddParms;
				getCacheConfigurationAddParamsInternal( uploadUrl , cacheConfigurationAddParms  ) ;

				_awsS3WrapperConfiguration.set_download_pool_size( cacheConfigurationAddParms.downloadPoolSize );
				_awsS3WrapperConfiguration.set_download_buffer_size( cacheConfigurationAddParms.downloadBufferSize ); 
				_awsS3WrapperConfiguration.set_upload_pool_size( cacheConfigurationAddParms.uploadPoolSize );
				_awsS3WrapperConfiguration.set_upload_buffer_size( cacheConfigurationAddParms.uploadBufferSize );
				
				break;
			}
			default:
        	{
		    	ostringstream exMsg;
		    	exMsg << "Internal ERROR: Server version value not recognized: " << _serverVersion; // PRQA S 3081 2
		    	ACS_THROW( exIllegalValueException( exMsg.str() ) ) ;
        	}				
		}
		
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "AwsS3WrapperConfiguration filled from Server methods:\n" << _awsS3WrapperConfiguration );

	}
#endif
	// see _uploadMode
	fileDefinition internalFileDefinition = f;
	internalFileDefinition.fileId = fileId ; 
	internalFileDefinition.size = realFileSize; // this is the real size (if tar option set, f.size was an estimate)
	
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "upload URL is " << uploadUrl) ;
			
	WorkerThread worker(*this, uploadUrl , localpath , UPLOAD, internalFileDefinition) ;
	worker.setPackagingAlgoOnTheFly( f.packagingAlgo ) ;
	worker.start() ;

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "worker thread started.") ;
	
	double updateInterval = 30./86400.;
	DateTime updateTime ;
	updateTime += updateInterval ;

	Ism__notifyUploadRequest notifyRequest ;
	notifyRequest.fileId = fileId ;
	notifyRequest.user = user ;
	notifyRequest.checkPermissions = checkPermissions;
	if( not f.packagingAlgo.empty() ) {
		notifyRequest.uploadUrl = uploadUrl ;
	}

	try {
		while( worker.running() )
		{
			if( isCancelRequested() ) {	worker.setHaltLevel(StopController::LAZY) ; }

			if( DateTime() > updateTime ) 
			{

				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Sending upload notification.") ;
				Ism__notifyUploadingResponse signalResponse ;

				timer.start() ;
				ThreadSafe::Lock lock (_endPointsMutex);
				map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
				unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
				
				for (int counter = 0; ; counter++ ) {

					ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry] << "\"") ;

					try {
						q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;
						int soap_error_code = -255;
						if ( (soap_error_code = q.notifyUploading( &notifyRequest, &signalResponse)) != SOAP_OK) 
						{
							ostringstream error ;
							error 	<< "Upl notify SrvErr[ " << soap_error_code << "]: " ;
							
							if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
								error << "Soap message transfer TIMEOUT" ;
								ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
							}
							
							error << q.soap_fault_string();
							if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
								ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                        	}
							ACS_THROW( Exception(error.str()) ) ; // PRQA S 3081
						}
						mapIt->second.setGoodEndPoint(endpointToTry) ;
						
						break;
					}
					catch(ConnectionException &e) {
						retryConnectionToServer( e.what(), timer, counter+1, endpointToTry );
					}
				}

				updateTime += updateInterval ;

			}

			Timer::delay(100) ; // 100 msec  // PRQA S 4400
		}

		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "worker thread exited.") ;
	} // try
	catch( exception &e) 
	{
		ACS_LOG_NOTIFY_EX(e);
		ostringstream err;
		err << '\"' << localpath << "\" Exception caught sending upload notification: " << e.what();
		ACS_LOG_ERROR( StringUtils::transform(err.str(), '\n', ' '));
		worker.stopWork();
	}
	
	string removeError;
	if( !worker.completed() )
	{
		//delete file
		string remoteFilePath = uploadResponse.file->filePath;
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "worker did not complete. Going to remove \"" << remoteFilePath
					<< "\" file from Server") ;
		
		try {
			remove( remoteFilePath , user) ;
		}
		catch( exception &e ) {
			ostringstream exMsg;
			exMsg << "Failed to remove  \"" << remoteFilePath << "\" file from Server:" << e.what();
		    removeError = exMsg.str();
		}


	}
	
	exception *workerException = 0 ;
	worker.runException(workerException) ;
	if(workerException)
	{
		ostringstream error ;
		error << '\"' << localpath << "\" error in uploading thread: " << workerException->what();
		ACS_LOG_ERROR( StringUtils::transform(error.str(), '\n', ' '));
		Exception ee ( *workerException, "Error in uploading thread." + removeError) ;
		delete workerException ;
		ACS_THROW( ee ) ;
	}	

	// Thread coul be still running if stop attempt, if any, not reached
	ACS_COND_THROW(worker.running(), Exception("Uploading thread still running: exiting anyway" ) );
	
	Ism__notifyCompletedUploadResponse signalResponse ;
	
	timer.start() ;
	{
		ThreadSafe::Lock lock (_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
		for (int counter = 0; ; counter++ ) {

			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry] << "\"") ;
			try {
				q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;
				int soap_error_code = -255;
				if ( (soap_error_code = q.notifyUploadCompleted( &notifyRequest, &signalResponse)) != SOAP_OK) 
				{
					ostringstream error ;
					error 	<< '\"' << localpath << "\" upl complete SrvErr[ " << soap_error_code << "]: " ;
					
					if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
						error << "Soap message transfer TIMEOUT" ;
						ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
					}
					
					error << q.soap_fault_string() ;
					if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
						ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                	}
					string errString = error.str();
					ACS_LOG_ERROR( StringUtils::transform(errString, '\n', ' '));
					ACS_THROW( Exception(errString ) ) ; // PRQA S 3081
				}
				mapIt->second.setGoodEndPoint(endpointToTry) ;
				
				break;
			}
			catch(ConnectionException &e) {
				retryConnectionToServer( e.what(), timer, counter+1, endpointToTry );
			}
		}
	} // ThreadSafe::Lock lock (_endPointsMutex);
	
	string smKey = f.filePath;
	if( _uploadMode == AWS_S3 ) {
#ifdef EXPRIVIA_ISM_USE_AWS_S3 		
		string bucket;
		string cachePath;
		AwsS3Wrapper::absolute_to_relative_path(uploadUrl, bucket, cachePath) ;
		smKey = StringUtils::pathJoin(_awsS3WrapperConfiguration.get_aws_endpoint(), bucket, cachePath);
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "AWS S3 smKey=\"" << smKey << '\"');
#else
		ACS_THROW(exIllegalValueException("AWS_S3 was not enabled at compile-time and it is not managed. Please contact Exprivia technical support.")) ;
#endif
	}
	smEntryKeys.push_back( pair<string , string>(localpath, smKey) );

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "upload completed.") ;

	
} 

void IsmSoapClient::internalDirectoryUpload( fileDefinition const & d , string const & localpath , bool recursive, vector< pair<string , string> > & smEntryKeys, \
												bool checkPermissions, bool extendDirPermissions )
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
	{
		ThreadSafe::Lock lock(_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "remote path is " << d.filePath  
				   << " ; local path is " << localpath 
				   << " ; endpoints are " << (mapIt->second)[0] << " " << (mapIt->second)[1]) ;
	}
	ACS_CLASS_END_DEBUG
			
	ACS_COND_THROW(_uploadMode == AWS_S3, Exception("Upload directory (\""+localpath+"\") with AWS_S3 mode not managed. Please contact Exprivia technical support.")) ;
	
	string  user = d.user ;
	string remoteDirPath = d.filePath;

	IsmProxy q;	
	Ism__dirUploadResponse uploadResponse ;
	Ism__dirDefinition directory ;
	
	Timer timer ;
	timer.start() ;
	
	vector<string> localFilepaths;
	
	DirTree localDirTree(localpath, "/", true);

	{ // _endPointsMutex lock scope guard	
		ThreadSafe::Lock lock (_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
		try {
			convert( d, localpath, localDirTree, localFilepaths, directory ) ;
			
			for (int counter = 0; ; counter++ ) {
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry] << "\"") ;
				try {
					q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;

					Ism__uploadDirectoryRequest uploadRequest ;
					uploadRequest.recursive = recursive ;
					uploadRequest.checkPermissions = checkPermissions ;
					uploadRequest.extendDirPermissions = extendDirPermissions ;
					uploadRequest.dir = &directory ;
					int64_t totalItemsSize = 0;
					uploadRequest.dir->totalItemsSize = &totalItemsSize;
					int soap_error_code = -255;
					if ( (soap_error_code = q.uploadDirectory(&uploadRequest , &uploadResponse)) != SOAP_OK)
					{
						ostringstream error ;
						error 	<< '\"' << localpath << "\" upload SrvErr[ " << soap_error_code << "]: "
								<< q.soap_fault_string();
						
						if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
							ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                    	}
						string errString = error.str();
						ACS_LOG_ERROR( StringUtils::transform(errString, '\n', ' '));
						ACS_THROW( Exception(errString ) ) ; // PRQA S 3081
					}
					mapIt->second.setGoodEndPoint(endpointToTry) ;
					
					break ; // succesfull
				}
				catch(ConnectionException &e) {
					retryConnectionToServer( e.what(), timer, counter+1, endpointToTry);
				}
			}

			remoteDirPath = uploadResponse.dir->fullpaths[0];

			// consistency checks
			if( uploadResponse.dir->isDir[0] != true ) { // PRQA S 3050
				ACS_THROW( CriticalException("First inserted record is not the wrapping directory") ) ; // PRQA S 3081
			}

			size_t directoryElementsNr = localFilepaths.size(); 	
			if( (uploadResponse.dir->fullpaths.size() 	!= directoryElementsNr)  ||
				(uploadResponse.dir->sizes.size() 		!= directoryElementsNr)  ||	
				(uploadResponse.dir->isDir.size() 	 	!= directoryElementsNr)  ||
				(uploadResponse.dir->filesIds.size() 	!= directoryElementsNr)  ||
				(uploadResponse.uploadUrls.size() 		!= directoryElementsNr)) {

				ostringstream exMsg;
				exMsg 	<< "Inconsistency of output data w.r.t. number of elements in remote path tree ("
						<< directoryElementsNr << "): the following sizes should match\n"
						<< " remote paths number " << uploadResponse.dir->fullpaths.size() << "\n"		
						<< " remote paths sizes number " << uploadResponse.dir->sizes.size() << "\n"		
						<< " remote paths dir ids number " << uploadResponse.dir->isDir.size() << "\n"		
						<< " ism_files pks number " << uploadResponse.dir->filesIds.size() << "\n"		
						<< " upload urls number " << uploadResponse.uploadUrls.size() ;		
				ACS_THROW( CriticalException(exMsg.str() ) ) ; // PRQA S 3081

			}


		} // try
		catch( TimeOutException &e ) {
			throw;
		}
		catch( CriticalException &e ) {

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to remove \"" << remoteDirPath << "\" directory from Server") ;

			try {	
				remove( remoteDirPath , user) ;
			}
			catch( exception &ee ) {
				ACS_THROW( CriticalException(e, "Even failed to remove \"" + remoteDirPath + "\" directory from Server:" + ee.what()) );
			}
			throw;

		}
	} // _endPointsMutex lock scope guard
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "input directory remote path  \"" << d.filePath  << "\"") ;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "path returned from db \"" <<  remoteDirPath << "\"") ;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "fileId of uploaded directory: " << uploadResponse.dir->filesIds[0]) ;

	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "dumping response: \n" <<  uploadResponse) ;
		
	// fill key pairs vector
    size_t localFilepaths_size = localFilepaths.size() ;
	for( size_t i = 0; i < localFilepaths_size; ++i ) {
		smEntryKeys.push_back( pair<string , string>(localFilepaths[i], uploadResponse.dir->fullpaths[i]) );
    }

	WorkerThread worker(*this, uploadResponse.uploadUrls , localFilepaths , UPLOAD) ;
	worker.start() ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "worker thread started.") ;
	
	double updateInterval = 30./86400.;
	DateTime updateTime ;


	Ism__notifyDirUploadRequest notifyRequest ;
	notifyRequest.filesIds = uploadResponse.dir->filesIds ;
	notifyRequest.onlinesIds = uploadResponse.onlinesIds ;
	notifyRequest.user = user ;
	if( uploadResponse.dir->totalItemsSize ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "totalItemsSize in uploadResponse valorized: " << *uploadResponse.dir->totalItemsSize );
		notifyRequest.totalItemsSize = new int64_t (*uploadResponse.dir->totalItemsSize);
	}
	notifyRequest.checkPermissions = checkPermissions ;
	
	try {
		while( worker.running() )
		{
			if( isCancelRequested() ) { worker.setHaltLevel(StopController::LAZY) ; }

			if( DateTime() > updateTime ) 
			{

				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Sending upload notification for \"" << remoteDirPath << "\" directory") ;
				Ism__notifyDirUploadingResponse signalResponse ;

				timer.start() ;
				ThreadSafe::Lock lock (_endPointsMutex);
				map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
				unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
				for (int counter = 0; ; counter++ ) {

					ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry] << "\"") ;

					try {
						q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;
						int soap_error_code = -255;

						if ( (soap_error_code = q.notifyDirUploading( &notifyRequest, &signalResponse)) != SOAP_OK) 
						{
							ostringstream error ;
							error 	<< "Upl notify SrvErr[ " << soap_error_code << "]: "
									<< q.soap_fault_string();
							
							if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
								ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                        	}
							ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
						}
						mapIt->second.setGoodEndPoint(endpointToTry) ;
						
						break;
					}
					catch(ConnectionException &e) {
						retryConnectionToServer( e.what(), timer, counter+1, endpointToTry );
					}
				}

				updateTime += updateInterval ;

			}

			Timer::delay(100) ; // 100 msec  // PRQA S 4400
		}

		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "worker thread exited.") ;
		
	}// try
	catch( exception &e) 
	{
		ACS_LOG_NOTIFY_EX(e);
		ostringstream err;
		err << '\"' << localpath << "\" Exception caught sending upload notification: " << e.what();
		ACS_LOG_ERROR( StringUtils::transform(err.str(), '\n', ' '));
		worker.stopWork();
	}
	

	string removeError;
	if( !worker.completed() )
	{
		//delete file
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "worker did not complete. Going to remove \"" << remoteDirPath
					<< "\" directory from Server") ;
				
		try {
			remove( remoteDirPath , user) ;
		}
		catch( exception &e ) {
			ostringstream exMsg;
			exMsg << "Failed to remove  \"" << remoteDirPath << "\" directory from Server:" << e.what();
		    removeError = exMsg.str();
		}

	}
	
	exception *workerException = 0 ;
	worker.runException(workerException) ;
	if(workerException)
	{
		ostringstream error ;
		error << '\"' << localpath << "\" error in uploading thread: " << workerException->what();
		ACS_LOG_ERROR( StringUtils::transform(error.str(), '\n', ' '));
		Exception ee ( *workerException, ("Error in uploading thread. " + removeError) ) ;
		delete workerException ;
		ACS_THROW( ee ) ;
	}	

	// Thread coul be still running if stop attempt, if any, not reached
	ACS_COND_THROW(worker.running(), Exception("Uploading thread still running: exiting anyway" ) );

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Sending upload complete notification for \"" << remoteDirPath << "\" directory") ;

	Ism__notifyDirCompletedUploadResponse signalResponse ;
	
	timer.start() ;
	{
		ThreadSafe::Lock lock (_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
		for (int counter = 0; ; counter++ ) {

			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry] << "\"") ;
			try {
				q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;
				int soap_error_code = -255;
				if ( (soap_error_code = q.notifyDirUploadCompleted( &notifyRequest, &signalResponse)) != SOAP_OK) 
				{
					ostringstream error ;
					error 	<< '\"' << localpath << "\" upl complete SrvErr[ " << soap_error_code << "]: "
							<< q.soap_fault_string() ;
					
					if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
						ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                	}
					string errString = error.str();
					ACS_LOG_ERROR( StringUtils::transform(errString, '\n', ' '));
					ACS_THROW( Exception(errString ) ) ; // PRQA S 3081
				}
				mapIt->second.setGoodEndPoint(endpointToTry) ;
				
				break;
			}
			catch(ConnectionException &e) {
				retryConnectionToServer( e.what(), timer, counter+1, endpointToTry );
			}
		}
	} // 	ThreadSafe::Lock lock (_endPointsMutex);
	
	delete notifyRequest.totalItemsSize;

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "upload completed.") ;
} 


void IsmSoapClient::upload( fileDefinition & f , string const &  localpath , bool recursive, vector< pair<string , string> > & smEntryKeys, \
							bool checkPermissions, bool extendDirPermissions)
{
	
	if( File::isDir(localpath) && _packageAlgo.empty() ) {
		f.isDir = true;
		internalDirectoryUpload(f , localpath , recursive, smEntryKeys, checkPermissions, extendDirPermissions);
	}
	else { // file OR _packageAlgo defined
		long long int realFileSize = f.size;
		if( not _packageAlgo.empty() ) {
			f.filePath = File::addExt(f.filePath, _packageExt );
			f.packagingAlgo = _packageAlgo;
			
			PackagingAlgo *packagingAlgo = PackagingAlgoSingleton::instance()->newPackagingAlgo(StringUtils::lowercase(f.packagingAlgo), _packageAlgoArchiveOptions, _packageAlgoExtractOptions );
			ACS_COND_THROW( ! packagingAlgo, Exception("Can't Instantiate object with \"" + f.packagingAlgo + "\" key: got NULL pointer from PackagingAlgoFactory") );	 // PRQA S 3081
			f.size = packagingAlgo->estimatePackageSize(f.size);
		}
		f.isDir = false;
		internalFileUpload(f , localpath , recursive, checkPermissions, realFileSize, smEntryKeys);
	}
	
}

vector< pair<string , string> > IsmSoapClient::upload( fileDefinition & f , string const &  localpath , bool recursive, \
														bool checkPermissions , bool extendDirPermissions)
{
	vector< pair<string , string> > smEntryKeys;
	upload( f , localpath , recursive, smEntryKeys, checkPermissions, extendDirPermissions);
	return smEntryKeys;
	
}


void IsmSoapClient::remove( string  const &  filePath , string  const & user )
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "file path is " << filePath  
			   << " ; user  is " << user ) ;
	
	// check protocol is http/https
	curl::RemoteSite remoteSite = curl::RemoteSiteFactory::makeSite(filePath) ;
	string remoteFilepath = filePath ;
	if( remoteSite.getIsHttp() ) {
#ifdef EXPRIVIA_ISM_USE_AWS_S3
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Path protocol is http(s)");
		
		string bucket ;
		string cachePath ;
		string fileUrl ;
		disassembleAWS_S3FilePath( filePath, bucket, cachePath, fileUrl ) ;
		
		Ism__cacheConfiguration cacheConfiguration ;
		multimap <AwsS3Wrapper::Enum_S3MetadataTag, string> objectsMap ;
		getAWS_S3ObjectMetadata( bucket, cachePath, fileUrl , objectsMap, cacheConfiguration ) ;
		multimap <AwsS3Wrapper::Enum_S3MetadataTag, string>::const_iterator mapIt = objectsMap.find(AwsS3Wrapper::S3TAG_ID2_APPL);
		ACS_COND_THROW( mapIt == objectsMap.end(), CriticalException("Cannot retrieve remote filepath from header data of \""+filePath+'\"') );
		remoteFilepath = mapIt->second ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Got remote filepath=\"" << remoteFilepath << "\" from header data of \"" << filePath << '\"') ;
#else
		ACS_THROW(exIllegalValueException("Path protocol is http(s):\""+filePath+"\". AWS_S3 was not enabled at compile-time and it is not managed. Please contact Exprivia technical support.")) ;
#endif
	}
	

	Timer timer ;
	timer.start() ;

	ThreadSafe::Lock lock (_endPointsMutex);
	map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
	unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
	for (int counter = 0; ; counter++ ) {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry]) ;
		try {
			IsmProxy q;
			q.send_timeout = _soapSendTimeout;
			q.recv_timeout = _soapRecvTimeout; 
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Soap recv_timeout=" << (-1)*q.recv_timeout << " [us]" );
			q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;

			Ism__deleteRequest deleteRequest ;
			deleteRequest.filePath = remoteFilepath ;
			deleteRequest.user = user ;

			Ism__deleteResponse deleteResponse;
			int soap_error_code = -255;
			if( ( soap_error_code = q.deleteFile(&deleteRequest , &deleteResponse) ) != SOAP_OK ) {
				ostringstream error ;
				error << "Delete SrvErr[ " << soap_error_code << "]: " ;

				if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
					error << "Soap message transfer TIMEOUT" ;
					ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
				}

				error << q.soap_fault_string();
				if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
					ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                }
				
				if( q.soap_fault_detail() ) {
					error << " Detail:" << endl << q.soap_fault_detail();
					
					int detailErrorCode = 0;
					getValueFromFaultDetail("errorcode", detailErrorCode, q.soap_fault_detail());
					if( detailErrorCode == SOAP_ISM_DATA_NOT_REMOVED_FOR_MEDIUM_MIGRATION ) { // PRQA S 1021
						ACS_THROW( FileToDeleteOnMigratingMediumExc(error.str()) ) ; // PRQA S 3081
                    }
				}
				
				ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
			}
			mapIt->second.setGoodEndPoint(endpointToTry) ;
						
			return ; // succesfull
		}
		catch(ConnectionException &e) {
			retryConnectionToServer( e.what(), timer, counter+1, endpointToTry );
		}

	}
}



void IsmSoapClient::getFileIdInternal( string  const &  filePath , string  const & user, Ism__getFileIdResponse &response ) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "file path is " << filePath  
			   << "; user is " << user) ;

	Timer timer ;
	timer.start() ;

	ThreadSafe::Lock lock (_endPointsMutex);
	map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
	unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
	for (int counter = 0; ; counter++ ) {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry]  << "\"") ;
		try {
			IsmProxy q;
			q.send_timeout = _soapSendTimeout;
			q.recv_timeout = _soapRecvTimeout;  
			q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;

			Ism__getFileIdRequest request ;
			request.filePath = filePath ;
			request.user = user ;

			int soap_error_code = -255;
			if ( (soap_error_code = q.getFileId(&request , &response)) != SOAP_OK)
			{
				ostringstream error ;
				error << "GetFileId SrvErr[ " << soap_error_code << "]: " ;
				if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
					error << "Soap message transfer TIMEOUT" ;
					ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
				}

				error << q.soap_fault_string();
				if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
					ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                }
				
				if( q.soap_fault_detail() ) {
					
					error << " Detail:" << endl << q.soap_fault_detail();
					
					int detailErrorCode = 0;
					getValueFromFaultDetail("errorcode", detailErrorCode, q.soap_fault_detail());
					if( detailErrorCode == SOAP_ISM_DATA_UNAVAILABLE ) { // PRQA S 1021
						ACS_THROW( dbDownloadFile::SMUnknownFileException(error.str()) ) ; // PRQA S 3081
                    }
				}
				ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
			}

			mapIt->second.setGoodEndPoint(endpointToTry) ;
			
			return ;
		}
		catch(ConnectionException &e) {
			retryConnectionToServer( e.what(), timer, counter+1, endpointToTry );
		}
	}

}





int IsmSoapClient::getFileId( string  const &  filePath , string  const & user )
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "file path is " << filePath  
			   << "; user is " << user) ;
	Ism__getFileIdResponse response ;
	getFileIdInternal(filePath,user, response) ;
	
	return response.fileId ;
}



void IsmSoapClient::download( string  const &  remotePath , string  const & user ,  string  const & localpath , int timeOut, bool removeLocalPathIfExisting, string const & group, bool checkPermissions )
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
	{
		ThreadSafe::Lock lock(_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "remote path is " << remotePath  
				   << " ; local path is " << localpath 
				   << " ; user  is " << user  
				   << " ; group is " << group  
				   << " ; timeout [s] is " << timeOut 
				   << " ; checkPermissions is " << boolalpha << checkPermissions  
				   << " ; endpoints are " << (mapIt->second)[0] << " " << (mapIt->second)[1]) ;
	}
	ACS_CLASS_END_DEBUG
			
	DateTime startDownload ; 
	
	if (timeOut < 0 ) { 
		timeOut = _defaultTimeout ;
    }
		
	if ( File::exists( localpath ) )
	{
		ACS_COND_THROW( not removeLocalPathIfExisting, Exception("Error: file \"" + localpath + "\" already exists.") ) ; // PRQA S 3081
		File::unlink( localpath );
	}
	
	
	{
		bool normalize = true;
		string parentPath = File::getDirPath(localpath, normalize);
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "parent path of local file \"" << parentPath << "\"") ;
		if ( !parentPath.empty() && !File::exists( parentPath ) )
		{
			ACS_THROW( Exception("Error: dir \"" + parentPath + "\" does not exist.") ) ; // PRQA S 3081
		}
	}

// check on AWS_S3 mode
	string filePath = remotePath ;
	int fileId = -1 ;
	bool isDir = false;
	if( _downloadMode == AWS_S3 ) {
#ifdef EXPRIVIA_ISM_USE_AWS_S3
		
		string fileUrl ;
		Ism__cacheConfiguration cacheConfiguration ;
		multimap <AwsS3Wrapper::Enum_S3MetadataTag, string> objectsMap ;
		
		{
			string bucket ;
			string cachePath ;
			disassembleAWS_S3FilePath( filePath, bucket, cachePath, fileUrl ) ;

			getAWS_S3ObjectMetadata( bucket, cachePath, fileUrl , objectsMap, cacheConfiguration ) ;
		}
		
		multimap <AwsS3Wrapper::Enum_S3MetadataTag, string>::const_iterator mapIt = objectsMap.find(AwsS3Wrapper::S3TAG_ID1_APPL);
		ACS_COND_THROW( mapIt == objectsMap.end(), CriticalException("Cannot retrieve fileId from header data of \""+filePath+'\"') );
		{
			istringstream is(mapIt->second) ;
			is >> fileId;
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Got file_id=" << fileId << " from header data of \"" << filePath << '\"') ;
		
// fill stuff needed by Worker Thread in download
		_awsS3WrapperConfiguration.set_aws_s3_provider( AwsS3Wrapper::to_S3Provider( cacheConfiguration.provider ) );
		_awsS3WrapperConfiguration.set_aws_key_ak( cacheConfiguration.key_ak );
		_awsS3WrapperConfiguration.set_aws_key_sk( cacheConfiguration.key_sk );
		_awsS3WrapperConfiguration.set_aws_region( cacheConfiguration.region );
		_awsS3WrapperConfiguration.set_aws_endpoint( cacheConfiguration.endpoint ); 
		_awsS3WrapperConfiguration.set_endpoint_is_secure( cacheConfiguration.endpointIsSecure );
		_awsS3WrapperConfiguration.set_options( cacheConfiguration.options );
		
		switch (_serverVersion)  {
    		case 1: 
			{ 
				// Ism__getCacheConfigurationAdditionalParms method not implemented: pool and buffer size must not be set
				ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "No method for cache configuration additional parameters called" );
				_awsS3WrapperConfiguration.set_download_pool_size(0);
				_awsS3WrapperConfiguration.set_download_buffer_size(0);
				_awsS3WrapperConfiguration.set_upload_pool_size(0);
				_awsS3WrapperConfiguration.set_upload_buffer_size(0);
				
				break;
			}
    		case 2: 
			{ 
				Ism__cacheConfigurationAdditionalParms cacheConfigurationAddParms;
				getCacheConfigurationAddParamsInternal( fileUrl , cacheConfigurationAddParms  ) ;

				_awsS3WrapperConfiguration.set_download_pool_size( cacheConfigurationAddParms.downloadPoolSize );
				_awsS3WrapperConfiguration.set_download_buffer_size( cacheConfigurationAddParms.downloadBufferSize );
				_awsS3WrapperConfiguration.set_upload_pool_size( cacheConfigurationAddParms.uploadPoolSize );
				_awsS3WrapperConfiguration.set_upload_buffer_size( cacheConfigurationAddParms.uploadBufferSize );
				
				break;
			}
			default:
        	{
		    	ostringstream exMsg;
		    	exMsg << "Internal ERROR: Server version value not recognized: " << _serverVersion; // PRQA S 3081 2
		    	ACS_THROW( exIllegalValueException( exMsg.str() ) ) ;
        	}				
		}	

		isDir = false;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "AwsS3WrapperConfiguration filled:\n" << _awsS3WrapperConfiguration );

#else
		ACS_THROW(exIllegalValueException("AWS_S3 was not enabled at compile-time and it is not managed. Please contact Exprivia technical support.")) ;
#endif
		
	}
	else {

		// get file id **** call internal method
		Ism__getFileIdResponse response ;
		getFileIdInternal(filePath, user, response) ;
		fileId = response.fileId ;
		isDir = response.isDir ;
	}
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "remote path \"" << filePath << "\" corresponds to file id " << fileId // PRQA S 3380
				<< " and is a " << (isDir ? "directory" : "file")) ;
	
	if( isDir ) {
		directoryDownload(filePath, fileId, user, localpath, timeOut, group, checkPermissions);
    }
	else {
		fileDownload(filePath, fileId, user, localpath, timeOut, group, checkPermissions, _clientReference);	
    }
    
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
        DateTime stopDownload ;
        int elapsedSecs = int((stopDownload.jd50() - startDownload.jd50())*86400. ); // PRQA S 3011  
	    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Total download time: " << elapsedSecs << " [s]") ;
    ACS_CLASS_END_DEBUG
}

void IsmSoapClient::fileDownload( string  const &  filePath , int fileId, string  const & user ,  string  const & localpath , int timeOut, string const & group, bool checkPermissions, string const & clientReference ) // PRQA S 4020
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
	{
		ThreadSafe::Lock lock(_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "remote path is " << filePath  
				   << " ; file id is " << fileId 
				   << " ; local path is " << localpath 
				   << " ; user  is " << user  
				   << " ; timeout [s] is " << timeOut  
				   << " ; group  is " << group  
				   << " ; checkPermissions is " << boolalpha << checkPermissions  
				   << " ; clientReference is \"" << clientReference << '\"'  
				   << " ; endpoints are " << (mapIt->second)[0] << " " << (mapIt->second)[1]) ;
	}
	ACS_CLASS_END_DEBUG

	// The file must be ready in local path before this deadline: 
	DateTime timeLimit ;
	timeLimit += timeOut / ( 3600. * 24. ) ; // in secs // PRQA S 3011, 4400

	// timeout for file transfer (ftp, copy, etc)
	const int transferTimeOut = _transferOperationTimeout_s; // 2 hours
	
	//Prepares here a request message to be sent to server
	Ism__DownloadFileRequest request ;
	request.fileId =fileId  ;
	request.user = user ;
	request.group = group ;
	request.checkPermissions = checkPermissions ;
	
	bool success = false ;
	int requestId = -1 ; //NOTE -1 means that this is the first time the download request is issued
	try {
		do {
			request.requestId = requestId ;  //NOTE: first time will be -1
			success = internalFileDownload( localpath, request, timeOut, transferTimeOut, clientReference );
			requestId = request.requestId;
			if( !success ) {
				if( DateTime() > timeLimit ) {
					ostringstream errMsg ;
					errMsg << "Timeout (" << timeOut << " s) expired trying to download file \""
							   << localpath << "\" (id=" << fileId << ") from remote client.";
					ACS_THROW(TimeOutException( errMsg.str() ) ) ; // PRQA S 3081
				}
				else {
					Timer::delay(500);  // 500 msec // PRQA S 4400
				}
			}
			
		} while ( !success ) ; 

		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB,'\"' << localpath << "\" file (\"" << filePath 
					<< "\" remote path) successfully downloaded") ;
				

	}
	catch ( exception &e ) {	

		string extReqMsg;
		if( request.requestId != -1 ) {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to remove external request with id=" <<  request.requestId) ;

			ThreadSafe::Lock lock (_endPointsMutex);
			map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
			unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry]  << "\"")  ;

			IsmProxy q;		
			q.send_timeout = _soapSendTimeout;
			q.recv_timeout = _soapRecvTimeout;  
			q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;

			Ism__notifyDownloadResponse abortResponse ;
			int soap_error_code = -255;
			if ((soap_error_code = q.abortDownload(&request , &abortResponse)) != SOAP_OK)
			{
				ostringstream error ;
				error 	<< "AbortDownload SrvErr[ " << soap_error_code << "]: " ;
				if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
					error << "Soap message transfer TIMEOUT" ;
				}
				else {
					error << q.soap_fault_string() ;
				} 
				extReqMsg = "Even failed to remove ext request: " + error.str() ;
			}
			else {			
				extReqMsg = "Ext request successfully removed";
            }

		}
		
		{
			ACS_LOG_ERROR(SimpleDebugSignature << e.what() << "\n" << extReqMsg);
		}
		
		if( dynamic_cast<StopException*> (&e) ) { // was a stop // PRQA S 3081
			return;
        }
		
		throw;
		
	} // catch

			
}


bool IsmSoapClient::internalFileDownload( string const & localpath, Ism__DownloadFileRequest & request, int downloadTimeout,  int transferTimeout, string const & clientReference ) // PRQA S 4020
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
	{
		ThreadSafe::Lock lock(_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "local path is " << localpath 
				   << " ; user  is " << request.user  
				   << " ; group  is " << request.group  
				   << " ; fileId  is " << request.fileId  
				   << " ; requestId  is " << request.requestId  
				   << " ; downloadTimeout [s] is " << downloadTimeout  
				   << " ; transferTimeout [s] is " << transferTimeout  
				   << " ; clientReference is \"" << clientReference << '\"'  
				   << " ; endpoints are " << (mapIt->second)[0] << " " << (mapIt->second)[1]) ;
	}
	ACS_CLASS_END_DEBUG

	// The file must be ready for ftp download before this deadline: 
	DateTime timeLimit = DateTime() + downloadTimeout / ( 3600. * 24. ) ; // in secs // PRQA S 3011
	
	IsmProxy q;
	q.send_timeout = _soapSendTimeout;
	q.recv_timeout = _soapRecvTimeout;  
	// This URL will point on the file to download
	string downloadUrl ;
	// ism_external_request pk
	int requestId = 0 ;
	// algo to decompress file if a package (e.g. tar, etc)
	string packagingAlgo;
	// cache where online has been downloaded
	int cacheId = -1;
	
	Timer::Delay updateReqInterval = 60 * 1000; // time interval in ms to update downloading request
	Timer downloadTimer ;
	downloadTimer.start();
	request.updateRequestTime = false;
	
	Ism__DownloadFileRequest_2 request_2 ;
	if( 2 == _serverVersion ) {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Ism__DownloadFileRequest:\n" << request );
		convert(request, clientReference, request_2);
		ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Ism__DownloadFileRequest_2 after conversion:\n" << request_2 );
	}

	// Start requesting the file to server
	// requests are over when
	// - the url field in the response message is not empty: the file is ready to download
	// - timeout has reached
	// - parent requests for cancellation
	while(true)
	{
		
		Timer timer ;
		timer.start() ;
		{
			ThreadSafe::Lock lock (_endPointsMutex);
			map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
			unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
			for (int counter = 0; ; counter++ ) {

				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry]  << "\"" ) ;
				try {

                    downloadTimer.stop() ;
					if ( downloadTimer.elapsed() > updateReqInterval ) {
						request.updateRequestTime = true;
						ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, downloadTimer.elapsed()*0.001 << " elapsed seconds (period " << updateReqInterval*0.001 // PRQA S 3011, 3084
								<< " seconds): downloading request time must be updated") ;
						downloadTimer.start();
					}

					q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;
					int soap_error_code = -255;
					
					switch (_serverVersion)  {
    					case 1: 
						{ 
							Ism__DownloadFileResponse downloadResponse ;
							soap_error_code = q.downloadFile(&request , &downloadResponse) ;
							if( SOAP_OK == soap_error_code ) {
								downloadUrl = downloadResponse.downloadUrl;
								requestId =  downloadResponse.requestId ; //get here the request id. This should be enough just the first time
								packagingAlgo = downloadResponse.packagingAlgo;
							}
							break;
						}
    					case 2: 
						{ 
							Ism__DownloadFileResponse_2 downloadResponse ;
							soap_error_code = q.downloadFile_2(&request_2 , &downloadResponse) ;
							if( SOAP_OK == soap_error_code ) {
								downloadUrl = downloadResponse.downloadUrl;
								requestId =  downloadResponse.requestId ; //get here the request id. This should be enough just the first time
								packagingAlgo = downloadResponse.packagingAlgo;
								cacheId = downloadResponse.cacheId ;
							}
							break;
						}
						default:
        				{
		    				ostringstream exMsg;
		    				exMsg << "Internal ERROR: Server version value not recognized: " << _serverVersion; // PRQA S 3081 2
		    				ACS_THROW( exIllegalValueException( exMsg.str() ) ) ;
        				}				
					}	
					
					if( soap_error_code != SOAP_OK )
					{
						ostringstream error ;
						error << "DownloadFile (server version " << _serverVersion << ") SrvErr[ " << soap_error_code << "]: " ;
								
						if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
							error << "Soap message transfer TIMEOUT" ;
							ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
						}

						error << q.soap_fault_string();
						if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
							ACS_THROW( ConnectionException(error.str()) ) ; // PRQA S 3081
                    	}

						if( q.soap_fault_detail() ) {
							int detailErrorCode = 0;
							getValueFromFaultDetail("errorcode", detailErrorCode, q.soap_fault_detail());
							if( detailErrorCode == SOAP_ISM_DATA_UNAVAILABLE ) { // PRQA S 1021
								error << " Detail:\n" << q.soap_fault_detail();
								ACS_THROW( dbDownloadFile::SMUnknownFileException(error.str()) ) ; // PRQA S 3081
                        	}
							else if( detailErrorCode == SOAP_ISM_DOWNLOAD_TEMPORARY_REFUSED ) { // PRQA S 1021
								ACS_LOG_WARNING("Error in downloading file with id " << request.fileId << ": " << error.str() << "\nKEEP TRYING...");
								return false;
							}
							else if( detailErrorCode == SOAP_ISM_DOWNLOAD_FILE_NEARLINE ) { // PRQA S 1021
								ACS_THROW( dbDownloadFile::FileToDownloadIsNearlineExc(error.str()) ) ; // PRQA S 3081
                        	}
							else if( detailErrorCode == SOAP_ISM_DOWNLOAD_FILE_OFFLINE ) { // PRQA S 1021
								ACS_THROW( dbDownloadFile::FileToDownloadIsOfflineExc(error.str()) ) ; // PRQA S 3081
                        	}
							else {} // empty
						}

						ACS_THROW( Exception(error.str()) ) ; // PRQA S 3081
					}

					mapIt->second.setGoodEndPoint(endpointToTry) ;
					
					break;
				}
				catch(ConnectionException &e) {
					retryConnectionToServer( e.what(), timer, counter+1, endpointToTry );
				}

			}
		} // ThreadSafe::Lock lock (_endPointsMutex);

		
		request.requestId = requestId ;   // update here the request message to contain the requestId
		request.updateRequestTime = false;

		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Server version " << _serverVersion << " received : requestId " << requestId << " ; download url \""  << downloadUrl << "\"; package algo \"" << packagingAlgo 
												<< "\"; cacheId " << cacheId << " .") ;
		
		// Check URL
		if(!downloadUrl.empty()) {
			break ;
        }

		// Check timeout or parent stop requested	
		DateTime Now;
		if( (Now > timeLimit) || isCancelRequested()  )
		{
			ostringstream errMsg ;
			if (Now > timeLimit) {  // was a timeout
				errMsg << "Timeout (" << downloadTimeout << " s) expired while waiting for file \""
					   << localpath << "\" (id=" << request.fileId << ") ready in cache.";
				ACS_THROW( TimeOutException( errMsg.str() ) ) ;  // PRQA S 3081
			}
			else {// was a parent stop
				errMsg << "Stop requested from parent"  ;
				ACS_THROW( StopException( errMsg.str() ) ) ; // PRQA S 3081
			}
		}
				
		Timer::delay(500) ; // 500 msec // PRQA S 4400
	} // while(true)
	
	if( ( cacheId < 1 ) && ( 1 != _serverVersion ) ) {
		ostringstream exMsg;
		exMsg << "Returned not valid cache id=" << cacheId ;
		ACS_THROW( Exception(exMsg.str() ) );
	}

	Ism__notifyDownloadRequest notifyRequest ;
	notifyRequest.fileId = request.fileId  ;
	notifyRequest.cacheId = cacheId;
	
	// The file must have been downloaded before this deadline: 
	timeLimit = DateTime() + transferTimeout / ( 3600. * 24. ) ; // in secs // PRQA S 3011, 4400

	WorkerThread worker(*this, downloadUrl , localpath , DOWNLOAD) ;
	worker.setPackagingAlgoOnTheFly( packagingAlgo ) ;
	worker.start() ;

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Worker thread started. Downloading online:" << notifyRequest) ;
	
	double updateInterval = 20./86400. ; // 20 s
	DateTime updateTime ;
	updateTime += updateInterval ;

	while( worker.running() )
	{
		if( isCancelRequested() ) { worker.setHaltLevel(StopController::LAZY) ; }

		if( DateTime() > updateTime ) 
		{
			
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, notifyRequest << ": Sending downloading notification.") ;
			Timer timer ;
			timer.start() ;
			ThreadSafe::Lock lock (_endPointsMutex);
			map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
			unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
			for (int counter = 0; ; counter++ ) {

				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry] << "\"") ;

				try {

					Ism__notifyDownloadResponse downloadingResponse ;
					q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;
					int soap_error_code = -255;
					switch (_serverVersion)  {
    					case 1: 
						{
							soap_error_code = q.notifyDownloading(&request , &downloadingResponse) ;
							break ;
						}
    					case 2: 
						{ 
							soap_error_code = q.notifyDownloading_2(&notifyRequest , &downloadingResponse) ;
							break ;
						}
						default:
        				{
		    				ostringstream exMsg;
		    				exMsg << "Internal ERROR: Server version value not recognized: " << _serverVersion; // PRQA S 3081 2
		    				ACS_THROW( exIllegalValueException( exMsg.str() ) ) ;
        				}				
					}	

					if ( soap_error_code != SOAP_OK)
					{
						ostringstream error ;
						error << "Downl notify (server version " << _serverVersion << ") SrvErr[ " << soap_error_code << "]: " ;
						if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
							error << "Soap message transfer TIMEOUT" ;
							ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
						}

						error << q.soap_fault_string();
						if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
							ACS_THROW( ConnectionException(error.str()) ) ; // PRQA S 3081
                        }
						ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
					}
					mapIt->second.setGoodEndPoint(endpointToTry) ;
					
					break;
				}
				catch(ConnectionException &e) {
					retryConnectionToServer( e.what(), timer, counter+1, endpointToTry );
				}
			}

			updateTime += updateInterval ;
		}

		{
			if( DateTime() > timeLimit )
			{
				ostringstream errMsg ;
				errMsg << "Timeout (" << transferTimeout << " s) expired trying to get file \""
						   << localpath << "\" (id=" << request.fileId << ") from Server.";
				ACS_THROW(TimeOutException( errMsg.str() ) ) ; // PRQA S 3081
			}
		}

		Timer::delay(100) ; // 100 msec // PRQA S 4400

	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, notifyRequest << ": worker thread exited.") ;

	exception *workerException = 0 ;
	worker.runException(workerException) ;
	if(workerException)
	{
		if( dynamic_cast<FileToDownloadDisappearedExc*> (workerException) ) { // PRQA S 3081
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Following exception caught: " << workerException->what()) ;
			delete workerException ;
			return false;
		}
		
		Exception ee ( *workerException, "Error in downloading thread.") ;
		delete workerException ;
		ACS_THROW( ee ) ;
		
	}	

	if(!worker.completed() )
	{
		ostringstream error ;
		error << "Download not completed: " << notifyRequest;
		ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
	}

	try {
		
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "downloading record" << notifyRequest << ": Sending download completed notification.") ;
		
		Timer timer ;
		timer.start() ;
		ThreadSafe::Lock lock (_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
		

		for (int counter = 0; ; counter++ ) {

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry] << "\"") ;

			try {

				Ism__notifyDownloadResponse downloadedResponse ;
				q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;
				int soap_error_code = -255;
				switch (_serverVersion)  {
    				case 1: 
					{
						soap_error_code = q.notifyDownloadCompleted(&request , &downloadedResponse) ;
						break;
					}
    				case 2: 
					{ 
						soap_error_code = q.notifyDownloadCompleted_2(&notifyRequest , &downloadedResponse) ;
						break;
					}
					default:
        			{
		    			ostringstream exMsg;
		    			exMsg << "Internal ERROR: Server version value not recognized: " << _serverVersion; // PRQA S 3081 2
		    			ACS_THROW( exIllegalValueException( exMsg.str() ) ) ;
        			}				
				}	

				if ( soap_error_code != SOAP_OK)
				{
					ostringstream error ;
					error << "Downl complete (server version " << _serverVersion << ") SrvErr[ " << soap_error_code << "]: " ;
					
					if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
						error << "Soap message transfer TIMEOUT" ;
						ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
					}
					
					error << q.soap_fault_string();
					if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
						ACS_THROW( ConnectionException(error.str()) ) ; // PRQA S 3081
                    }
					ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
				}
				mapIt->second.setGoodEndPoint(endpointToTry) ;
				
				break;
			}
			catch(ConnectionException &e) {
				retryConnectionToServer( e.what(), timer, counter+1, endpointToTry );
			}
		}
	}
	catch(exception &) {
		try {
			File::remove( localpath ) ;
		}
		catch( exception &ee )
		{
			ACS_LOG_ERROR(SimpleDebugSignature << "cannot even remove \"" << localpath << "\" file: " << ee) ;
		}
		throw ;
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, notifyRequest << '\"' << localpath << "\" file successfully downloaded") ;
			
	return true;

}


void IsmSoapClient::directoryDownload( string  const &  filePath , int fileId, string  const & user ,  string  const & localpath , int timeOut, string  const & group, bool checkPermissions ) // PRQA S 4020
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
	{
		ThreadSafe::Lock lock(_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "remote path is " << filePath  
				   << " ; file id is " << fileId 
				   << " ; local path is " << localpath 
				   << " ; user  is " << user  
				   << " ; timeout [s] is " << timeOut  
				   << " ; group  is " << group  
				   << " ; checkPermissions is " << boolalpha << checkPermissions  
				   << " ; endpoints are " << (mapIt->second)[0] << " " << (mapIt->second)[1]) ;
	}
	ACS_CLASS_END_DEBUG

	// The file must be ready in local path before this deadline: 
	DateTime timeLimit ;
	timeLimit += timeOut / ( 3600. * 24. ) ; // in secs // PRQA S 3011, 4400
		
	Timer timer ;
	timer.start() ;

	
	// get Directory Tree from ISM
	Ism__directoryTreeResponse dirTreeResponse ;
	{
		ThreadSafe::Lock lock (_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
		for (int counter = 0; ; counter++ ) {

			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry] << "\"") ;

			try {
				IsmProxy q;
				q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;

				Ism__directoryTreeRequest dirTreeRequest ;
				dirTreeRequest.fileId = fileId ;

				int soap_error_code = -255;
				if ( (soap_error_code = q.getDirectoryTree(&dirTreeRequest , &dirTreeResponse)) != SOAP_OK)
				{
					ostringstream error ;
					error << " upload SrvErr[ " << soap_error_code << "]: " << q.soap_fault_string();
					if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
						ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                	}

					if( q.soap_fault_detail() ) {

						error << " Detail:" << endl << q.soap_fault_detail();

						int detailErrorCode = 0;
						getValueFromFaultDetail("errorcode", detailErrorCode, q.soap_fault_detail());
						if( detailErrorCode == SOAP_ISM_DATA_UNAVAILABLE ) { // PRQA S 1021
							ACS_THROW( dbDownloadFile::SMUnknownFileException(error.str()) ) ; // PRQA S 3081
                    	}
					}
					ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
				}
				mapIt->second.setGoodEndPoint(endpointToTry) ;
				
				break ; // succesfull
			}
			catch(ConnectionException &e) {
				retryConnectionToServer( e.what(), timer, counter+1, endpointToTry);
			}
		}
	} // ThreadSafe::Lock lock (_endPointsMutex);
	
	// consistency checks
	if( dirTreeResponse.filesIds[0] != fileId ) {
		ostringstream exMsg;
		exMsg 	<< "Inconsistency in directory tree response data: the first returned record should be \"" << filePath << "\" but its file id is " 
				<< dirTreeResponse.filesIds[0] << " (" << fileId << " expected";		
		ACS_THROW( CriticalException(exMsg.str() ) ) ; // PRQA S 3081
	}

	size_t dirTreeElementsNr = dirTreeResponse.filePaths.size();	  
	if( (dirTreeElementsNr != dirTreeResponse.filesIds.size())  ||
		(dirTreeElementsNr != dirTreeResponse.isDir.size()) 		) {

		ostringstream exMsg;
		exMsg 	<< "Inconsistency in directory tree response data: the following sizes should match\n"
				<< " file paths number " << dirTreeElementsNr << "\n"		
				<< " file_id number " << dirTreeResponse.filesIds.size() << "\n"		
				<< " file paths dir ids number " << dirTreeResponse.isDir.size()  ;		
		ACS_THROW( CriticalException(exMsg.str() ) ) ; // PRQA S 3081
	}
	
	string remoteDirPath = dirTreeResponse.filePaths[0];

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "remote directory tree of file \"" << remoteDirPath  << "\" (file_id=" << fileId << ") succesfully retrieved") ;
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "dumping directory tree response : "<<  "\n"
		       	<<  dirTreeResponse) ;
		
	Ism__downloadMultiFilesRequest fRequest;	
	Ism__downloadDirRequest request; 
	request.filesRequest = &fRequest;
	request.filesRequest->user = user ;	
	request.filesRequest->group = group ;	
	request.filesRequest->checkPermissions = checkPermissions;
	// Build directory tree on local disk and vector of files to be downloaded
	vector<string> targetFilepaths; // target path of files to be downloaded
	for( size_t i = 0; i < dirTreeElementsNr; i++ ) {
		
		string remoteFilepath = dirTreeResponse.filePaths[i];
		string remoteRelFilepath = remoteFilepath.substr(remoteDirPath.length(), remoteFilepath.length()-1); // PRQA S 3084
		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "remote filepath \"" << remoteFilepath << "\" remote relative filepath \"" << remoteRelFilepath << "\"") ;
				
		string localFilepath = localpath + remoteRelFilepath;
		
		if( dirTreeResponse.isDir[i] ) { // PRQA S 3050
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "\"" << localFilepath << "\" is a directory: going to create it") ;
			File::mkdir(localFilepath);
			
			// changing permissions according to umask
			mode_t mode = File::getmod(localFilepath) & 0777775;
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "\"" << localFilepath << "\" current permissions " << oct << File::getmod(localFilepath) << "; going to set them to " << mode) ;
			File::chmod(localFilepath, mode);

			request.dirsIds.push_back(dirTreeResponse.filesIds[i]);
		}
		else {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "\"" << localFilepath << "\" is a file: going to insert into files to download") ;
            
			targetFilepaths.push_back(localFilepath);
			request.filesRequest->filesIds.push_back(dirTreeResponse.filesIds[i]);
			request.filesRequest->requestsIds.push_back(-1); //NOTE -1 means that this is the first time the download request is issued
		}
	}
			
	
	bool success = false ;
	int jobId = -1; //NOTE -1 means that this is the first time the download requests are issued
	bool filesDisappeared = false;
	try {
		do {
			request.filesRequest->jobId = jobId;
			request.filesRequest->filesDisappeared = filesDisappeared;
			success = internalDirectoryDownload( targetFilepaths, request, timeOut );
			jobId = request.filesRequest->jobId;
			filesDisappeared = request.filesRequest->filesDisappeared;
			if( !success && (DateTime() > timeLimit) ) {
				ostringstream errMsg ;
				errMsg << "Timeout (" << timeOut << " s) expired trying to download directory \""
						   << filePath << "\" (id=" << fileId << ") from remote client.";
				ACS_THROW(TimeOutException( errMsg.str() ) ) ; // PRQA S 3081
			}
			
		} while ( !success ) ; 

		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "\"" << localpath << "\" directory successfully downloaded from remote path \"" << filePath << "\"") ;
	}
	catch ( exception &e ) {	

		// remove the whole directory
		DirTree localDirTree(localpath, "/", true);
		localDirTree.reverse();
        size_t localDirTree_size = localDirTree.size() ;
		for (unsigned long i=0; i < localDirTree_size; i++) {
			try {
				bool isDir = false;
				if( File::isDir(localDirTree[i]) ) {
					isDir = true;
					File::rmdir(localDirTree[i]);
				}
				else {
					File::remove(localDirTree[i]);
                }

				ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "\"" << localDirTree[i] << "\" local " << (isDir ? "directory" : "file") << " successfully removed") ; // PRQA S 3380
			}
			catch( exception &ee )
			{
				ACS_LOG_ERROR(SimpleDebugSignature << " cannot remove \"" << localDirTree[i] << "\" path: " << ee ) ;
			}
		} // remove the whole directory

		string extReqMsg;
		if( request.filesRequest->jobId >= 0 ) {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to remove " << request.filesRequest->requestsIds.size() << " external requests (job_id=" 
					   <<  request.filesRequest->jobId << ")") ;

			ThreadSafe::Lock lock (_endPointsMutex);
			map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
			unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
			ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry]  << "\"") ;

			IsmProxy q;	
			q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;

			Ism__notifyDownloadResponse abortResponse ;
			int soap_error_code = -255;
			if ((soap_error_code = q.abortMultiFilesDownload(request.filesRequest , &abortResponse)) != SOAP_OK)
			{
				ostringstream error ;
				error 	<< "AbortDownload SrvErr[ " << soap_error_code << "]: "
						<< q.soap_fault_string() ;
				extReqMsg = "Even failed to remove ext requests: " + error.str() ;
			}
			else {
				extReqMsg = "Ext requests successfully removed";
            }
		}
		
		{
			ACS_LOG_ERROR(SimpleDebugSignature << " " << e.what() << "\n" << extReqMsg) ;
		}
		
		if( dynamic_cast<StopException*> (&e) )  { // stop requested // PRQA S 3081
			return;
		}
		
		throw;
		
	}


}


bool IsmSoapClient::internalDirectoryDownload( vector<string> const & targetFilepaths, Ism__downloadDirRequest & request, int downloadTimeout ) // PRQA S 4020
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_MID_VERB) ;

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Going to download directory with id " << request.dirsIds[0] << ", target files paths:") ;
	
		for( size_t i = 0; i < targetFilepaths.size(); i++ ) {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "# " << i << " \"" <<  targetFilepaths[i] << "\"") ;
        }
					
		ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "DownloadDirRequest: \n" << request) ;
	ACS_CLASS_END_DEBUG
	
	// The file must be ready for transfer on Client before this deadline: 
	DateTime timeLimit = DateTime() + downloadTimeout / ( 3600. * 24. ) ; // in secs // PRQA S 3011, 4400
	

	IsmProxy q;
	
	// These URLs will point on the file to download
	vector<string> downloadUrls ;
	
	// Start requesting the files to server
	// requests are over when
	// - the downloadCompleted field in the response message is true: files are ready to download
	// - timeout has reached
	// - parent requests for cancellation

	double updateReqInterval = 60./86400. ; // 60 s
	DateTime updateReqTime ;
	updateReqTime += updateReqInterval;
	request.filesRequest->updateRequestTime = false;
	while(true)
	{
		
		Timer timer ;
		timer.start() ;
		
		Ism__downloadMultiFilesResponse downloadResponse ;
		{
			ThreadSafe::Lock lock (_endPointsMutex);
			map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
			unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
			for (int counter = 0; ; counter++ ) {

				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry]  << "\"") ;

				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "counter=" << counter << " job_id for external requests is " << request.filesRequest->jobId) ;

				try {

					if( DateTime() > updateReqTime ) {
						request.filesRequest->updateRequestTime = true;
						updateReqTime += updateReqInterval;
						ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "downloading request time updated") ;
					}


					q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;
					int soap_error_code = -255;
					if ( (soap_error_code = q.downloadDirectory(&request , &downloadResponse)) != SOAP_OK)
					{
						downloadResponse.downloadCompleted = false;

						ostringstream error ;
						error << "DownloadFile SrvErr[ " << soap_error_code << "]: " 
								<< q.soap_fault_string();
						if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
							ACS_THROW( ConnectionException(error.str()) ) ; // PRQA S 3081
                    	}

						if( q.soap_fault_detail() ) {

							int detailErrorCode = 0;
							getValueFromFaultDetail("errorcode", detailErrorCode, q.soap_fault_detail());
							if( detailErrorCode == SOAP_ISM_DATA_UNAVAILABLE ) { // PRQA S 1021
								error << " Detail:" << endl << q.soap_fault_detail();
								ACS_THROW( dbDownloadFile::SMUnknownFileException(error.str()) ) ; // PRQA S 3081
                        	}
							else if( detailErrorCode == SOAP_ISM_DOWNLOAD_TEMPORARY_REFUSED ) { // PRQA S 1021
								ACS_LOG_WARNING("Error in downloading directory with id " << request.dirsIds[0] << ": " << error.str() << "\nKEEP TRYING...");
								return false;
							}
							else {} // empty
						}

						ACS_THROW( Exception(error.str()) ) ; // PRQA S 3081
					}

					mapIt->second.setGoodEndPoint(endpointToTry) ;
					
					break;
				}
				catch(ConnectionException &e) {
					retryConnectionToServer( e.what(), timer, counter+1, endpointToTry );
				}

			}
		} // ThreadSafe::Lock lock (_endPointsMutex);
		request.filesRequest->updateRequestTime = false;
		request.filesRequest->filesDisappeared = false; // just to remark it


		ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Response received : \n" << downloadResponse) ;
				
		request.filesRequest->jobId = downloadResponse.jobId;

// consistency checks
		size_t filesToDownload = targetFilepaths.size(); 		
		if( (request.filesRequest->filesIds.size() 		!= filesToDownload)  ||
			(request.filesRequest->requestsIds.size() 	!= filesToDownload)  ||
			(downloadResponse.downloadUrls.size() 		!= filesToDownload)  ||	
			(downloadResponse.requestsIds.size() 		!= filesToDownload) ) {

			ostringstream exMsg;
			exMsg 	<< "Inconsistency of input/output data w.r.t. the number of files to download ("
					<< filesToDownload << ") of directory with id " << request.dirsIds[0] << " The following sizes should match: \n"
					<< " input files ids number: " << request.filesRequest->filesIds.size() << "\n"		
					<< " input requests number: " << request.filesRequest->requestsIds.size() << "\n"		
					<< " output requests number: " << downloadResponse.requestsIds.size() << "\n"		
					<< " output urls number: " << downloadResponse.downloadUrls.size() ;		
			ACS_THROW( CriticalException(exMsg.str() ) ) ; // PRQA S 3081

		}

		downloadUrls = downloadResponse.downloadUrls;
		request.filesRequest->requestsIds = downloadResponse.requestsIds;

		if( downloadResponse.downloadCompleted ) {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "download completed: " << DateTime().getSqlString()) ;
			break ;
		}

		// Check timeout or parent stop requested	
		DateTime Now;
		if( (Now > timeLimit) || isCancelRequested() )
		{
			ostringstream errMsg ;
			errMsg 	<< "Downloading directory with id " << request.dirsIds[0] << " (" << downloadResponse.downloadUrls.size() << " files). ";
			if (Now > timeLimit) {  // was a timeout
				errMsg << "Timeout (" << downloadTimeout << " s) expired while waiting for files ready in caches.";
				ACS_THROW( TimeOutException( errMsg.str() ) ) ; // PRQA S 3081
			}
			else {// was a parent stop
				errMsg << "Stop requested from parent"  ;
				ACS_THROW( StopException( errMsg.str() ) ) ; // PRQA S 3081
			}
			
		}
				
		Timer::delay(200) ; // 200 msec // PRQA S 4400
	} // while(true)

	// The file must have been downloaded before this deadline: 
	timeLimit = DateTime() + _transferOperationTimeout_s / ( 3600. * 24. ) ; // in secs // PRQA S 4400
	
	WorkerThread worker(*this, downloadUrls , targetFilepaths , DOWNLOAD) ;
	worker.start() ;

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "directory with id " << request.dirsIds[0] << "(" <<  downloadUrls.size() << " files)" << " : worker thread started.") ;
	
	double updateInterval = 20./86400. ; // 20 s
	DateTime updateTime ;

	while( worker.running() )
	{
		if( isCancelRequested() ) { worker.setHaltLevel(StopController::LAZY) ; }

		if( DateTime() > updateTime ) 
		{
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "directory with id " << request.dirsIds[0] << "(" <<  downloadUrls.size() << " files)"
					   	<< " : Sending download notification") ;
			Timer timer ;
			timer.start() ;
			ThreadSafe::Lock lock (_endPointsMutex);
			map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
			unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
			for (int counter = 0; ; counter++ ) {

				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry] << "\"") ;

				try {
					Ism__notifyDownloadResponse downloadingResponse ;
					q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;
					int soap_error_code = -255;
					if ( (soap_error_code = q.notifyMultiFilesDownloading(request.filesRequest , &downloadingResponse)) != SOAP_OK)
					{
						ostringstream error ;
						error << "Downl notify SrvErr[ " << soap_error_code << "]: "
								<< q.soap_fault_string();
						if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
							ACS_THROW( ConnectionException(error.str()) ) ; // PRQA S 3081
                        }
						ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
					}
					mapIt->second.setGoodEndPoint(endpointToTry) ;
					
					break;
				}
				catch(ConnectionException &e) {
					retryConnectionToServer( e.what(), timer, counter+1, endpointToTry );
				}
			}

			updateTime += updateInterval ;
		}

		{
			if( DateTime() > timeLimit )
			{
				ostringstream errMsg ;
				errMsg 	<< "Timeout (" << _transferOperationTimeout_s << " s) expired trying to get directory with id " << request.dirsIds[0] 
						<< " (" << downloadUrls.size() << " files) from Server.";
				ACS_THROW(TimeOutException( errMsg.str() ) ) ; // PRQA S 3081
			}
		}

		Timer::delay(100) ; // 100 msec // PRQA S 4400

	} // while( worker.running() )

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "directory with id " << request.dirsIds[0] << " (" <<  downloadUrls.size() << " files): worker thread exited.") ;

	exception *workerException = 0 ;
	worker.runException(workerException) ;
	if(workerException)
	{
		if( dynamic_cast<FileToDownloadDisappearedExc*> (workerException) ) { // PRQA S 3081
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Following exception caught: \n"
						<< workerException->what());
			delete workerException ;
			request.filesRequest->filesDisappeared = true;
			return false;
		}
		
		Exception ee ( *workerException, "Error in downloading thread.") ;
		delete workerException ;
		ACS_THROW( ee ) ;
		
	}	

	if(!worker.completed() )
	{
		ostringstream error ;
		error << "Directory with id " << request.dirsIds[0] << "(" <<  downloadUrls.size() << " files): download not completed"  ;
		ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "directory with id " << request.dirsIds[0] << " (" <<  downloadUrls.size() << " files)"
				<< " : Sending download completed notification.") ;

	Timer timer ;
	timer.start() ;
	{
		ThreadSafe::Lock lock (_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
		for (int counter = 0; ; counter++ ) {

			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry] << "\"") ;

			try {
				Ism__notifyDownloadResponse downloadedResponse ;
				q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;
				int soap_error_code = -255;
				if ( (soap_error_code = q.notifyMultiFilesDownloadCompleted(request.filesRequest , &downloadedResponse)) != SOAP_OK)
				{
					ostringstream error ;
					error << "Downl complete SrvErr[ " << soap_error_code << "]: "
						<< q.soap_fault_string();
					if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
						ACS_THROW( ConnectionException(error.str()) ) ; // PRQA S 3081
                	}
					ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
				}
				mapIt->second.setGoodEndPoint(endpointToTry) ;
				
				break;
			}
			catch(ConnectionException &e) {
				retryConnectionToServer( e.what(), timer, counter+1, endpointToTry );
			}
		}
	} // ThreadSafe::Lock lock (_endPointsMutex);

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "directory with id " << request.dirsIds[0] << " (" <<  downloadUrls.size() << " files) successfully downloaded.") ;
	
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "The following files successfully downloaded");
		size_t targetFilepaths_size = targetFilepaths.size() ;
        for( size_t i = 0; i < targetFilepaths_size; ++i ) {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "# " << i << " \"" <<  downloadUrls[i] << "\" -> \"" <<  targetFilepaths[i] << "\"");
        }
	ACS_CLASS_END_DEBUG

	return true;

}

void IsmSoapClient::book( string  const &  filePath , string  const & user ,  int hoursToDeadline,  float latencyDays)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
	{
		ThreadSafe::Lock lock(_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
    	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "remote path is " << filePath  
				   << " ; user  is " << user  
				   << " : hours to deadline are " << hoursToDeadline  
				   << " ; latency days  are " << latencyDays  
				   << " ; endpoints are " << (mapIt->second)[0] << " " << (mapIt->second)[1]) ;
	}
	ACS_CLASS_END_DEBUG
	

	IsmProxy q;
	q.send_timeout = _soapSendTimeout;
	q.recv_timeout = _soapRecvTimeout;  
	int fileId = -1 ;

	
			
	// get file id **** call internal method
	Ism__getFileIdResponse response ;
	getFileIdInternal(filePath,user, response) ;
	fileId = response.fileId ;
	
	//Preapares here a request message to be sent to server
	Ism__bookFileRequest request ;
	request.fileId =fileId  ;
	request.user = user ;
	request.hoursToDeadline = hoursToDeadline; 
	request.latencyDays = latencyDays;
	
	
	// Book the file to server
	Timer timer ;
	timer.start() ;
	
	ThreadSafe::Lock lock (_endPointsMutex);
	map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
	unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
	for (int counter = 0; ; counter++ ) {

		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry]  << "\"") ;

		try {
			Ism__bookFileResponse bookResponse ;
			q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;
			int soap_error_code = -255;
			if ( (soap_error_code = q.bookFile(&request , &bookResponse)) != SOAP_OK)
			{
				ostringstream error ;
				error << "BookFile SrvErr[ " << soap_error_code << "]: " ;
				
				if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
					error << "Soap message transfer TIMEOUT" ;
					ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
				}

				error << q.soap_fault_string();
				if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
					ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                }
				ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
			}
			mapIt->second.setGoodEndPoint(endpointToTry) ;
			
			break;
		}
		catch(ConnectionException &e) {
			retryConnectionToServer( e.what(), timer, counter+1, endpointToTry );
		}
	}
	
}


void IsmSoapClient::giveupFileRequest( string  const &  filePath , string  const & user, string const & clientReference )
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "file path is " << filePath << " ; user  is " << user << " ; client reference is \"" << clientReference << '\"') ;
		
	Timer timer ;
	timer.start() ;

	ThreadSafe::Lock lock (_endPointsMutex);
	map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
	unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
	for (int counter = 0; ; counter++ ) {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry]) ;
		try {
			IsmProxy q;
			q.send_timeout = _soapSendTimeout;
			q.recv_timeout = _soapRecvTimeout; 
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Soap recv_timeout=" << (-1)*q.recv_timeout << " [us]" );
			q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;

			Ism__giveupRequest request ;
			request.filePath = filePath ;
			request.user = user ;
			request.clientReference = clientReference ;

			Ism__giveupResponse response;
			int soap_error_code = -255;
			if( ( soap_error_code = q.giveupFileRequest(&request , &response) ) != SOAP_OK ) {
				ostringstream error ;
				error << "Giveup file request SrvErr[ " << soap_error_code << "]: " ;

				if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
					error << "Soap message transfer TIMEOUT" ;
					ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
				}

				error << q.soap_fault_string();
				if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
					ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                }
				ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
			}
			mapIt->second.setGoodEndPoint(endpointToTry) ;
						
			return ; // succesfull
		}
		catch(ConnectionException &e) {
			retryConnectionToServer( e.what(), timer, counter+1, endpointToTry );
		}

	}
}



#ifdef EXPRIVIA_ISM_USE_AWS_S3 
void IsmSoapClient::getCacheConfigurationInternal( const string & pathInCache , Ism__cacheConfiguration & cacheConfiguration ) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Path in cache is \"" << pathInCache << '\"') ;

	Timer timer ;
	timer.start() ;

	ThreadSafe::Lock lock (_endPointsMutex);
	map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
	unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
	for (int counter = 0; ; counter++ ) {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry]  << "\"") ;
		try {
			IsmProxy q;
			q.send_timeout = _soapSendTimeout;
			q.recv_timeout = _soapRecvTimeout;  
			q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;

			int soap_error_code = -255;
			char path[4096];
			strcpy(path, pathInCache.c_str()) ;
			if ( (soap_error_code = q.getCacheConfiguration(path , &cacheConfiguration)) != SOAP_OK)
			{
				ostringstream error ;
				error << "GetCacheConfiguration SrvErr[ " << soap_error_code << "]: " ;
				if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
					error << "Soap message transfer TIMEOUT" ;
					ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
				}

				error << q.soap_fault_string();
				if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
					ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                }
				
				if( q.soap_fault_detail() ) {
					
					error << " Detail:" << endl << q.soap_fault_detail();
					
					int detailErrorCode = 0;
					getValueFromFaultDetail("errorcode", detailErrorCode, q.soap_fault_detail());
				}
				ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
			}

			mapIt->second.setGoodEndPoint(endpointToTry) ;
			
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "CacheConfiguration:\n" << cacheConfiguration) ;
			
			return ;
		}
		catch(ConnectionException &e) {
			retryConnectionToServer( e.what(), timer, counter+1, endpointToTry );
		}
	}

}

void IsmSoapClient::getCacheConfigurationAddParamsInternal( const string & pathInCache , Ism__cacheConfigurationAdditionalParms & cacheConfigurationAddParams ) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Path in cache is \"" << pathInCache << '\"') ;

	Timer timer ;
	timer.start() ;

	ThreadSafe::Lock lock (_endPointsMutex);
	map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
	unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
	for (int counter = 0; ; counter++ ) {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry]  << "\"") ;
		try {
			IsmProxy q;
			q.send_timeout = _soapSendTimeout;
			q.recv_timeout = _soapRecvTimeout;  
			q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;

			int soap_error_code = -255;
			char path[4096];
			strcpy(path, pathInCache.c_str()) ;
			if ( (soap_error_code = q.getCacheConfigurationAdditionalParms(path , &cacheConfigurationAddParams)) != SOAP_OK)
			{
				ostringstream error ;
				error << "GetCacheConfigurationAdditionalParms SrvErr[ " << soap_error_code << "]: " ;
				if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
					error << "Soap message transfer TIMEOUT" ;
					ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
				}

				error << q.soap_fault_string();
				if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
					ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                }
				
				if( q.soap_fault_detail() ) {
					
					error << " Detail:" << endl << q.soap_fault_detail();
					
					int detailErrorCode = 0;
					getValueFromFaultDetail("errorcode", detailErrorCode, q.soap_fault_detail());
				}
				ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
			}

			mapIt->second.setGoodEndPoint(endpointToTry) ;
			
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "CacheConfigurationAdditionalParms:\n" << cacheConfigurationAddParams) ;
			
			return ;
		}
		catch(ConnectionException &e) {
			retryConnectionToServer( e.what(), timer, counter+1, endpointToTry );
		}
	}

}

void IsmSoapClient::disassembleAWS_S3FilePath( const string & filePath, string & bucket, string & cachePath, string & fileUrl )
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Path in cache is \"" << filePath << '\"') ;
	
	string endPoint;	
	AwsS3Wrapper::disassemble_full_path(filePath, endPoint, bucket, cachePath) ;
	ACS_COND_THROW( endPoint.empty(), exIllegalValueException("Input path wrong formatted: \""+filePath+'\"')) ;

	AwsS3Wrapper::relative_to_absolute_path(bucket, cachePath, fileUrl, true) ;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "AWS S3 fileurl=\"" << fileUrl << '\"');
}


void IsmSoapClient::getAWS_S3ObjectMetadata( const string & bucket, const string & cachePath, const string & fileUrl, multimap <AwsS3Wrapper::Enum_S3MetadataTag, string> & objectsMap, Ism__cacheConfiguration & cacheConfiguration )
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_HIG_VERB) ;

	getCacheConfigurationInternal( fileUrl, cacheConfiguration );

	ACS_SMARTPTR<AwsS3Wrapper> s3handler_p ;
	StopController *stopController_p = NULL ; 
	if( _sc == NULL ) {
		stopController_p = new StopController() ;
		ACS_CLASS_WRITE_DEBUG_TH(ACS_INS_VERB, "StopController pointer instance"); 
	}
	else {
		stopController_p = const_cast<StopController*>(_sc) ;
		ACS_CLASS_WRITE_DEBUG_TH(ACS_INS_VERB, "StopController pointer now points to class attribute"); 
	}
	
	try {
		
		AwsS3Wrapper::AwsS3WrapperConfiguration awsS3WrapperConfiguration ;	
			
		awsS3WrapperConfiguration.set_aws_s3_provider( AwsS3Wrapper::to_S3Provider( cacheConfiguration.provider ) );
		awsS3WrapperConfiguration.set_aws_key_ak( cacheConfiguration.key_ak );
		awsS3WrapperConfiguration.set_aws_key_sk( cacheConfiguration.key_sk );
		awsS3WrapperConfiguration.set_aws_region( cacheConfiguration.region );
		awsS3WrapperConfiguration.set_aws_endpoint( cacheConfiguration.endpoint ); 
		awsS3WrapperConfiguration.set_endpoint_is_secure( cacheConfiguration.endpointIsSecure );
		awsS3WrapperConfiguration.set_options( cacheConfiguration.options );

		fillS3Handler( awsS3WrapperConfiguration, s3handler_p, *stopController_p) ;
		objectsMap = s3handler_p->getObjectMetadataTags(bucket, cachePath)  ;
	}
	catch( exception &e )
	{
		if(  _sc == NULL ) {
			delete stopController_p ;
		}
		throw ;
	}
	
	if(  _sc == NULL ) {
		delete stopController_p ;
		ACS_CLASS_WRITE_DEBUG_TH(ACS_INS_VERB, "StopController pointer deleted"); 
	}
	
}

#endif

void IsmSoapClient::retryConnectionToServer( string const & errMsg, Timer & timer, int attempt, unsigned short &endpointToTry) // PRQA S 4211
{

	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_HIG_VERB) ;
	unsigned short endpNo = 0 ;
	bool allPortsEqualNo = false ;
	// restart time
	if( attempt == 1 ) {
		timer.start() ;
    }
	{ // while in this scope - endpoint are locked
		ThreadSafe::Lock lock(_endPointsMutex) ;
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);

		endpNo = mapIt->second.size() ;
		allPortsEqualNo = (attempt >= endpNo*2) ; // number of attempts double the available port no // PRQA S 3000, 4400		
		timer.stop() ;

		ACS_LOG_WARNING(DebugSignatureThread << errMsg << ". Attempt #" << attempt << " (" << timer.elapsed() 
					<< " [ms]) url: \"" << (mapIt->second)[endpointToTry] << "\"")  ;

		if ( (timer.elapsed() > _connectTimeout_ms)  && allPortsEqualNo ) {
			ostringstream error ;
			error	<< "Timeout (" << _connectTimeout_ms << " ms) attempts: " << attempt
					<< " after " << timer.elapsed() << " [ms]. ERR: " << errMsg;
			ACS_THROW( TimeOutException(error.str() ) ) ; // PRQA S 3081
		}

		// change port
		if( endpNo > 1 ) { // PRQA S 3000
			endpointToTry = mapIt->second.getNextEndPoint(endpointToTry);

			ACS_LOG_WARNING(DebugSignatureThread << "retrying on \"" << (mapIt->second)[endpointToTry]  << "\"") ;
		}
	} // allow other threads 
	if (allPortsEqualNo) {
		int delayMsec = 500/max<int>(endpNo,1) ; // PRQA S 3000 2
		Timer::delay(delayMsec) ; 
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "elapsed time " << timer.elapsed() << " ms, timeout "
				   << _connectTimeout_ms << " ms, sleeping " << delayMsec << " [ms]") ;  
	}

}


IsmSoapClient::TransferMode IsmSoapClient::getTransferModeEnum(string const & tMode) // PRQA S 4020
{
	string localMode = StringUtils::uppercase(tMode);
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "going to set transfer mode corresponding to \"" << localMode << "\"") ;
	
	if( localMode == "FTP" ) {
		return FTP;
    }
	else if( localMode == "COPY" ) {
		return COPY;
    }
	else if( localMode == "LINK" ) {
		return LINK;
    }
	else if( localMode == "AWS_S3" ) {
		return AWS_S3;
    }
	else {
		ACS_LOG_WARNING(SimpleDebugSignature << "Unrecognized transfer mode: \"" << tMode << "\": setting default mode (ftp)") ;
		return FTP;
	}
}

string IsmSoapClient::getTransferModeString(IsmSoapClient::TransferMode const & tMode) // PRQA S 4020
{	
	switch (tMode)  {
    case FTP: { return "ftp"; }
	case COPY: { return "copy"; }
	case LINK: { return "hard link"; }
	case AWS_S3: { return "aws s3"; }
	default:
        {
		    ostringstream exMsg;
		    exMsg << "Internal ERROR: transfer (" << int(tMode) << ") mode unrecognized"; // PRQA S 3081 2
		    ACS_THROW( exIllegalValueException( exMsg.str() ) ) ;
        }				
	}	
}

bool IsmSoapClient::overrideTransferMode(rsResourceSet const & rset, string const & modeTag, string & transferMode)
{
	bool overridden = false;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Going to get \"" << modeTag << "\" value") ;
	try {
		string localTransferMode;
		rset.getValue(modeTag, localTransferMode);
		if( not localTransferMode.empty() ) {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Going to override " << transferMode << " transfer modality with " << localTransferMode) ;
			transferMode = localTransferMode;
			overridden = true;
		}
	}
	catch (rsResourceSet::NotFoundException &x ) {}
	
	return overridden;
	
}

void IsmSoapClient::checkLocalTransferModes(string const & serverHost, rsResourceSet const & rset, string & uploadModality, string & downloadModality) // PRQA S 4020
{
	
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_MID_VERB) ;
	
	if( clientAndServerSameHost( serverHost ) ) {
// search if local transfer mode is configured
	
		string keyField = "SMConfiguration.LocalTransferMode";
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "looking for \"" << keyField << "\" field occurrence") ;
		if( rset.hasKey(keyField) ) {

			string tag = keyField + ".uploadModality";
			overrideTransferMode(rset, tag, uploadModality);

	   		tag = keyField + ".downloadModality";
	   		overrideTransferMode(rset, tag, downloadModality);
		}
		
	// Then search if ISM local transfer mode is configured in application namespace
		try {
	// first of all application name must be defined!!!
			string applicationName = Application::instance()->applicationName() ;
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "current application name is \"" << applicationName << '\"') ;
			rsResourceSet &confRs = ConfigurationSingleton::instance()->get();
			keyField = applicationName + ".IsmLocalTransferMode";
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "looking for \"" << keyField << "\" field occurrence in configuration") ;
			if( confRs.hasKey(keyField) ) {

				string tag = keyField + ".uploadModality";
				if( overrideTransferMode(confRs, tag, uploadModality) ) {
					ACS_LOG_PRIVINFO("Upload transfer modality on ISM overridden by \"" << applicationName << "\" application configuration: " << uploadModality );
				}

	   			tag = keyField + ".downloadModality";
	   			if( overrideTransferMode(confRs, tag, downloadModality) ) {
					ACS_LOG_PRIVINFO("Download transfer modality from ISM overridden by \"" << applicationName << "\" application configuration: " << downloadModality );
				}
			}
		}
		catch( exception &e)
		{
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Cannot access to Client Application configuration space: " << e.what());
		}
	}
	else {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Client and Server are NOT the same host") ;
	}

		
}



void IsmSoapClient::getValueFromFaultDetail( // PRQA S 4020
    std::string const & field, 
    int & value, 
    const char * faultDetail)
{
	value = 0;
	if( !faultDetail ) {
		return;
    }
	
	try {
		rsResourceSet rset ;
		XMLIstringstream iss (faultDetail);
		iss >> rset;

		try{
			rset.getValue(field, value);
		}
		catch(rsResourceSet::NotFoundException &e)
		{}
		
	}
	catch( exception &e )
	{
		ACS_LOG_ERROR(e);
	}			
}

int IsmSoapClient::getList( string const & filePath , uid_t uid, gid_t gid, int32_t offset, size_t entries, vector<string> & listedPaths, int32_t & newOffset, int & errnum ) 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ThreadSafe::Lock lock(_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Input path is \"" << filePath << '\"'
					<< " ; userId is " << uid  
					<< " ; groupId is " << gid  
					<< " ; offset is " << offset
					<< " ; entries are " << entries
				   	<< " ; endpoints are " << (mapIt->second)[0] << " " << (mapIt->second)[1]) ;
	ACS_CLASS_END_DEBUG
		
	errnum = 0;
	int retCode = 1;
	const size_t maxEntries = 100;
	const size_t rem = entries % maxEntries;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Remainder: " << rem);
	size_t iterations = entries / maxEntries;
	if( rem != 0 ) {
		++iterations;
	}
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Number of iterations: " << iterations);
	if( iterations == 0 ) {
		retCode = 0;
	}
	
	size_t localOffset = offset;
	for( size_t i = 0; i < iterations && retCode == 1; ++i ) {
		
		size_t limit = maxEntries;
		if( i == (iterations - 1) && rem != 0 ) {
			limit = rem;
		}
		ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Iteration #" << i << " offset: " << localOffset << " limit: " << limit);
		// list Directory content
		Ism__listResponse listResponse;
		{
			ThreadSafe::Lock lock (_endPointsMutex);
			map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
			unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
			Timer timer ;
			timer.start() ;
			for (int counter = 0; ; counter++ ) {

				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry] << '\"') ;

				try {
					IsmProxy q;
					q.send_timeout = _soapSendTimeout;
					q.recv_timeout = _soapRecvTimeout;  
					q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;

					Ism__listRequest listRequest ;
					listRequest.path = filePath ;
					listRequest.userId = uid;
					listRequest.groupId = gid;
					listRequest.offset = localOffset;
					listRequest.entries = limit;

					int soap_error_code = -255;
					if ( (soap_error_code = q.getList(&listRequest , &listResponse)) != SOAP_OK)
					{
						ostringstream error ;
						error << "getList SrvErr[ " << soap_error_code << "]: " ;

						if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
							error << "Soap message transfer TIMEOUT" ;
							ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
						}

						error << q.soap_fault_string();
						if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
							ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                		}
						ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
					}
					mapIt->second.setGoodEndPoint(endpointToTry) ;

					break ; // succesfull
				}
				catch(ConnectionException &e) {
					retryConnectionToServer( e.what(), timer, counter+1, endpointToTry);
				}
			}
		} // ThreadSafe::Lock lock (_endPointsMutex);
	
		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Returned response: \n" << listResponse);
		errnum = listResponse.errnumber;
		retCode = listResponse.returnCode;
		if( retCode < 0 ) { // an error occurred.
			continue;
		}
		
		copy(listResponse.paths.begin(), listResponse.paths.end(), back_inserter(listedPaths));
		newOffset = listResponse.offset;
		localOffset = newOffset;
	} // for( size_t i = 0; i < iterations; ++i ) {
	
	return retCode;
	
}

int IsmSoapClient::access( string const & filePath , uid_t uid, gid_t gid, uint32_t mode, int & errnum) 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ThreadSafe::Lock lock(_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Input path is \"" << filePath << '\"'
					<< " ; userId is " << uid  
					<< " ; groupId is " << gid  
					<< " ; mode is " << mode
				   	<< " ; endpoints are " << (mapIt->second)[0] << " " << (mapIt->second)[1]) ;
	ACS_CLASS_END_DEBUG
			
	Ism__accessResponse accessResponse;
	{
		ThreadSafe::Lock lock (_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
		Timer timer ;
		timer.start() ;
		for (int counter = 0; ; counter++ ) {

			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry] << '\"') ;
			try {
				IsmProxy q;
				q.send_timeout = _soapSendTimeout;
				q.recv_timeout = _soapRecvTimeout;  
				q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;

				Ism__accessRequest accessRequest ;
				accessRequest.path = filePath ;
				accessRequest.userId = uid;
				accessRequest.groupId = gid;
				accessRequest.mode = mode;

				int soap_error_code = -255;
				if ( (soap_error_code = q.access(&accessRequest , &accessResponse)) != SOAP_OK)
				{
					ostringstream error ;
					error << "access SrvErr[ " << soap_error_code << "]: " ;
					if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
						error << "Soap message transfer TIMEOUT" ;
						ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
					}
					
					error << q.soap_fault_string();
					if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
						ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                	}

					ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
				}
				mapIt->second.setGoodEndPoint(endpointToTry) ;

				break ; // succesfull
			}
			catch(ConnectionException &e) {
				retryConnectionToServer( e.what(), timer, counter+1, endpointToTry);
			}
			
		}

	} // ThreadSafe::Lock lock (_endPointsMutex);
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Returned response: \n" << accessResponse);
	errnum = accessResponse.errnumber;
	return accessResponse.returnCode;

}	



int IsmSoapClient::getAttributes( std::string const & filePath, uid_t uid, gid_t gid,  struct stat &buf, int & errnum)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ThreadSafe::Lock lock(_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Input path is \"" << filePath << '\"'
					<< " ; userId is " << uid  
					<< " ; groupId is " << gid  
				   	<< " ; endpoints are " << (mapIt->second)[0] << " " << (mapIt->second)[1]) ;
	ACS_CLASS_END_DEBUG
	
	Ism__getAttrResponse attrResponse;
	{
		ThreadSafe::Lock lock (_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
		Timer timer ;
		timer.start() ;
		for (int counter = 0; ; counter++ ) {

			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry] << '\"') ;
			try {
				IsmProxy q;
				q.send_timeout = _soapSendTimeout;
				q.recv_timeout = _soapRecvTimeout;  
				q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;

				Ism__getAttrRequest getAttrRequest ;
				getAttrRequest.path = filePath ;
				getAttrRequest.userId = uid;
				getAttrRequest.groupId = gid;
				int soap_error_code = -255;
				if ( (soap_error_code = q.getFileAttributes(&getAttrRequest , &attrResponse)) != SOAP_OK)
				{
					ostringstream error ;
					error << "getFileAttributes SrvErr[ " << soap_error_code << "]: ";
					
					if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
						error << "Soap message transfer TIMEOUT" ;
						ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
					}
					
					error << q.soap_fault_string();
					if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
						ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                	}

					ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
				}
				mapIt->second.setGoodEndPoint(endpointToTry) ;

				break ; // succesfull
			}
			catch(ConnectionException &e) {
				retryConnectionToServer( e.what(), timer, counter+1, endpointToTry);
			}
			
		}

	} // ThreadSafe::Lock lock (_endPointsMutex);
	
// The following instruction causes core!!
 	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Returned response:\n" << attrResponse);
	if( attrResponse.returnCode == 0 ) {
		convert( attrResponse.attributes, buf );
	}
	errnum = attrResponse.errnumber;
	return attrResponse.returnCode;
	
}

int IsmSoapClient::setAttributes( std::string const & filePath, const uid_t& uid, const gid_t& gid,  const struct stat &buf, int & errnum)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ThreadSafe::Lock lock(_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Input path is \"" << filePath << '\"'
					<< " ; userId is " << uid  
					<< " ; groupId is " << gid  
				   	<< " ; endpoints are " << (mapIt->second)[0] << " " << (mapIt->second)[1]) ;
	ACS_CLASS_END_DEBUG
	
	Ism__accessResponse attrResponse;
	{
		ThreadSafe::Lock lock (_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
		Timer timer ;
		timer.start() ;
		for (int counter = 0; ; counter++ ) {

			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry] << '\"') ;
			try {
				IsmProxy q;
				q.send_timeout = _soapSendTimeout;
				q.recv_timeout = _soapRecvTimeout;  
				q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;

				Ism__setAttrRequest setAttrRequest ;
				setAttrRequest.path = filePath ;
				setAttrRequest.userId = uid;
				setAttrRequest.groupId = gid;
				
				convert( buf, setAttrRequest.attributes );
				
				int soap_error_code = -255;
				if ( (soap_error_code = q.setFileAttributes(&setAttrRequest , &attrResponse)) != SOAP_OK)
				{
					ostringstream error ;
					error << "setFileAttributes SrvErr[ " << soap_error_code << "]: ";
					
					if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
						error << "Soap message transfer TIMEOUT" ;
						ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
					}
					
					error << q.soap_fault_string();

					if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
						ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                	}

					ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
				}
				mapIt->second.setGoodEndPoint(endpointToTry) ;

				break ; // succesfull
			}
			catch(ConnectionException &e) {
				retryConnectionToServer( e.what(), timer, counter+1, endpointToTry);
			}
			
		}

	} // ThreadSafe::Lock lock (_endPointsMutex);
	
	errnum = attrResponse.errnumber;
	return attrResponse.returnCode;
	
}


void IsmSoapClient::getUserGroup( uid_t uid, gid_t gid, std::string & user, std::string & group)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ThreadSafe::Lock lock(_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << " userId is " << uid  
					<< " ; groupId is " << gid  
				   	<< " ; endpoints are " << (mapIt->second)[0] << " " << (mapIt->second)[1]) ;
	ACS_CLASS_END_DEBUG

	Ism__userGroupResponse response;
	{
		ThreadSafe::Lock lock (_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
		Timer timer ;
		timer.start() ;
		for (int counter = 0; ; counter++ ) {

			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry] << '\"') ;
			try {
				IsmProxy q;
				q.send_timeout = _soapSendTimeout;
				q.recv_timeout = _soapRecvTimeout;  
				q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;

				Ism__userGroupRequest request ;
				request.userId = uid;
				request.groupId = gid;
				int soap_error_code = -255;
				if ( (soap_error_code = q.getUserGroup(&request , &response)) != SOAP_OK)
				{
					ostringstream error ;
					error << "getUserGroup SrvErr[ " << soap_error_code << "]: ";

					if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
						error << "Soap message transfer TIMEOUT" ;
						ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
					}
					
					error << q.soap_fault_string();
					if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
						ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                	}

					ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
				}
				mapIt->second.setGoodEndPoint(endpointToTry) ;

				break ; // succesfull
			}
			catch(ConnectionException &e) {
				retryConnectionToServer( e.what(), timer, counter+1, endpointToTry);
			}
			
		}

	} // ThreadSafe::Lock lock (_endPointsMutex);
	
	user = response.user;
	group = response.group;
}


void IsmSoapClient::remove( string  const &  filePath , uid_t uid , gid_t gid , int & errnum, bool recursive)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "file path is " << filePath  
			   << " ; uid=" << uid 
			   << " ; gid=" << gid ) ;
	
	// check protocol is http/https
	curl::RemoteSite remoteSite = curl::RemoteSiteFactory::makeSite(filePath) ;
	string remoteFilepath = filePath ;
	if( remoteSite.getIsHttp() ) {
#ifdef EXPRIVIA_ISM_USE_AWS_S3
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Path protocol is http(s)");
		
		string bucket ;
		string cachePath ;
		string fileUrl ;
		disassembleAWS_S3FilePath( filePath, bucket, cachePath, fileUrl ) ;
		
		Ism__cacheConfiguration cacheConfiguration ;
		multimap <AwsS3Wrapper::Enum_S3MetadataTag, string> objectsMap ;
		getAWS_S3ObjectMetadata( bucket, cachePath, fileUrl , objectsMap, cacheConfiguration ) ;
		multimap <AwsS3Wrapper::Enum_S3MetadataTag, string>::const_iterator mapIt = objectsMap.find(AwsS3Wrapper::S3TAG_ID2_APPL);
		ACS_COND_THROW( mapIt == objectsMap.end(), CriticalException("Cannot retrieve remote filepath from header data of \""+filePath+'\"') );
		remoteFilepath = mapIt->second ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Got remote filepath=\"" << remoteFilepath << "\" from header data of \"" << filePath << '\"') ;

#else
		ACS_THROW(exIllegalValueException("Path protocol is http(s):\""+filePath+"\". AWS_S3 was not enabled at compile-time and it is not managed. Please contact Exprivia technical support.")) ;
#endif
	}
	
	Timer timer ;
	timer.start() ;

	ThreadSafe::Lock lock (_endPointsMutex);
	map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
	unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
	for (int counter = 0; ; counter++ ) {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry]) ;
		try {
			IsmProxy q;
			q.send_timeout = _soapSendTimeout;
			q.recv_timeout = _soapRecvTimeout;  
			q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;

			Ism__deleteRequestFs deleteRequest ;
			deleteRequest.filePath = remoteFilepath ;
			deleteRequest.userId = uid ;
			deleteRequest.groupId = gid ;
			deleteRequest.recursive = recursive ;

			int errnumber = 0;
			int soap_error_code = -255;
			if ( (soap_error_code = q.deleteFileFsLike(&deleteRequest , &errnumber)) != SOAP_OK) {
				ostringstream error ;
				error << "Delete SrvErr[ " << soap_error_code << "]: " ;
				if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
					error << "Soap message transfer TIMEOUT" ;
					ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
				}

				error << q.soap_fault_string();
				if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
					ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                }
				
				if( q.soap_fault_detail() ) {
					error << " Detail:" << endl << q.soap_fault_detail();
					
					int detailErrorCode = 0;
					getValueFromFaultDetail("errorcode", detailErrorCode, q.soap_fault_detail());
					if( detailErrorCode == SOAP_ISM_DATA_NOT_REMOVED_FOR_MEDIUM_MIGRATION ) { // PRQA S 1021
						ACS_THROW( FileToDeleteOnMigratingMediumExc(error.str()) ) ; // PRQA S 3081
                    }
				}
				
				ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
			}
			mapIt->second.setGoodEndPoint(endpointToTry) ;
			
			errnum = errnumber;
			
			return ; // succesfull
		}
		catch(ConnectionException &e) {
			retryConnectionToServer( e.what(), timer, counter+1, endpointToTry );
		}

	}
}

void IsmSoapClient::getCachesStorageInfo(vector< pair<string, long long int> > & stInfo) 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ThreadSafe::Lock lock(_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Endpoints are " << (mapIt->second)[0] << " " << (mapIt->second)[1]) ;
	ACS_CLASS_END_DEBUG

	Ism__cacheInfoResponse response;
	{
		ThreadSafe::Lock lock (_endPointsMutex);
		map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
		unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
		Timer timer ;
		timer.start() ;
		for (int counter = 0; ; counter++ ) {

			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry] << '\"') ;
			try {
				IsmProxy q;
				q.send_timeout = _soapSendTimeout;
				q.recv_timeout = _soapRecvTimeout;  
				q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;

				Ism__cacheInfoRequest request ;
				int soap_error_code = -255;
				if ( (soap_error_code = q.getCacheStorageInfo(&request , &response)) != SOAP_OK)
				{
					ostringstream error ;
					error << "getCachesStorageInfo SrvErr[ " << soap_error_code << "]: ";
					if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
						error << "Soap message transfer TIMEOUT" ;
						ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
					}
					
					error << q.soap_fault_string();

					if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
						ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                	}

					ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
				}
				mapIt->second.setGoodEndPoint(endpointToTry) ;

				break ; // succesfull
			}
			catch(ConnectionException &e) {
				retryConnectionToServer( e.what(), timer, counter+1, endpointToTry);
			}
			
		}

	} // ThreadSafe::Lock lock (_endPointsMutex);
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Returned response:\n" << response);

    transform(response.cacheInfo.begin(), response.cacheInfo.end(), back_inserter(stInfo), getStInfoEl);
	
}


bool IsmSoapClient::clientAndServerSameHost( const string & serverHost )
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_MID_VERB) ;

	string clientHost = HWScan::hostname(); // this return just hostname with no dominion
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Discover if Client host (\"" << clientHost << "\") and Server host (\"" << serverHost << "\") are the same" ) ;
	
	bool sameHost = false;
	if( StringUtils::equalsNoCase(serverHost, "localhost") || StringUtils::equals(serverHost, clientHost) ) {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "They are the same host") ;
		sameHost = true;
	}
	else { // find if a dominion occurs, and in that case compare the server host without dominion (e.g. hsm-dev.acs.it must be equal to hsm-dev)
		size_t pos = serverHost.find('.');
		if( pos != string::npos && StringUtils::equals( serverHost.substr(0, pos), clientHost) ) {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "They are the same host excluding dominion") ;
			sameHost = true;
		}
		else {
			// nothing
		}
	}
	
	return sameHost;
	

}


void IsmSoapClient::setSoapRecvTimeout( const int timeout_ms) 
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_MID_VERB) ;
	
	if( timeout_ms <= 0 ) {
		ostringstream exMsg;
		exMsg << "Not allowed value: " << timeout_ms << " ( > 0 expected)" ;
		ACS_THROW( Exception(exMsg.str() ) ) ; // PRQA S 3081
	}
	
	_soapRecvTimeout = timeout_ms * (-1000); // < 0 gives socket recv timeout in usec  
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "soapRecvTimeout " << (-1)*_soapRecvTimeout << " [us]") ;

	
}


void IsmSoapClient::setServerVersion()
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_MID_VERB) ;
	
	_serverVersion = IsmServerVersionManagerSingleton::instance()->getVersion(_host) ;
	if( -1 == _serverVersion ) {
		
		// going to ask to server
	
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ThreadSafe::Lock lock(_endPointsMutex);
			map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Endpoints are " << (mapIt->second)[0] << " " << (mapIt->second)[1]) ;
		ACS_CLASS_END_DEBUG

		{
			ThreadSafe::Lock lock (_endPointsMutex);
			map<long long int, EndPoints>::iterator mapIt = getEndPointsFromMapIt(_endPointsKey);
			unsigned short endpointToTry = mapIt->second.getLastGoodEndPoint() ;
			Timer timer ;
			timer.start() ;
			for (int counter = 0; ; counter++ ) {

				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "going to try on endpoint \"" << (mapIt->second)[endpointToTry] << '\"') ;
				try {
					IsmProxy q;
					q.send_timeout = _soapSendTimeout;
					q.recv_timeout = _soapRecvTimeout;  
					q.soap_endpoint = (mapIt->second)[endpointToTry].c_str() ;

					Ism__serverVersionRequest request;
					int soap_error_code = -255;
					if ( (soap_error_code =  q.getServerVersion(&request , &_serverVersion)) != SOAP_OK)
					{
						ostringstream error ;
						error << "getServerVersion SrvErr[ " << soap_error_code << "]: " ;
						if( ( SOAP_EOF == q.error ) && ( 0 == q.errnum ) ) {
							error << "Soap message transfer TIMEOUT" ;
							ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
						}

						const char *d = q.soap_fault_detail();
						error << q.soap_fault_string() << " detail: " << (d ? d : "(none)") ;

						if( soap_tcp_error_check(soap_error_code) ) { // PRQA S 3700
							ACS_THROW( ConnectionException(error.str() ) ) ; // PRQA S 3081
                		}
						else if( soap_soap_error_check(soap_error_code) ) {
							_serverVersion = oldServerVersion;
							ACS_LOG_WARNING( error.str() << ". Server version set to " << _serverVersion );
							break;
						}

						ACS_THROW( Exception(error.str() ) ) ; // PRQA S 3081
					}
					mapIt->second.setGoodEndPoint(endpointToTry) ;

					break ; // succesfull
				}
				catch(ConnectionException &e) {
					retryConnectionToServer( e.what(), timer, counter+1, endpointToTry);
				}

			}

			ACS_LOG_INFO( "Returned Server version is " << _serverVersion );
		} // ThreadSafe::Lock lock (_endPointsMutex);
		
		IsmServerVersionManagerSingleton::instance()->setVersion( _host, _serverVersion ) ;
	} // if( -1 == serverVersion )
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IsmSoapClient::PackageAlgoOption::PackageAlgoOption() :
    key(),
    value()
{
}


IsmSoapClient::PackageAlgoOption::PackageAlgoOption(IsmSoapClient::PackageAlgoOption const &o):
	key(o.key),
	value(o.value)
{
}
								
IsmSoapClient::PackageAlgoOption & IsmSoapClient::PackageAlgoOption::operator = (IsmSoapClient::PackageAlgoOption const &o)
{
	if( this != &o ) {
		key=o.key;
		value=o.value;
	}
	return (*this);
}								


ostream &operator<<(ostream &s, const IsmSoapClient::PackageAlgoOption & v)
{
	s	<< "****************** PackageAlgoOption ****************\n";
	s << "\tkey =\"" 	<< v.key 	<< "\"\n";
	s << "\tvalue =\"" 	<< v.value 	<< "\"\n";

	return s ; // PRQA S 4028
}

exostream &operator<<(exostream &s, const IsmSoapClient::PackageAlgoOption & v) // PRQA S 4222
{
	s	<< "****************** PackageAlgoOption ****************\n";
	s << "\tkey =\"" 	<< v.key 	<< "\"\n";
	s << "\tvalue =\"" 	<< v.value 	<< "\"\n";

	return s ; // PRQA S 4028

}

template<>
void rsResourceSet::getValue(const std::string &res, IsmSoapClient::PackageAlgoOption &v) const 
{
    Lock lock(_mutex) ;
	getValue(res+".key")->get(v.key) ;
	getValue(res+".value")->get(v.value) ;
} 



_ACS_END_NAMESPACE
