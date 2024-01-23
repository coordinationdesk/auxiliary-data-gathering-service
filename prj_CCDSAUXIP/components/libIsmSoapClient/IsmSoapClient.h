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

	$Prod: A.C.S. libIsmSoapClient$

	$Id$

	$Author$

	$Log$
	Revision 6.9  2017/11/09 09:26:27  lucio.pulvirenti
	S2PDGS-1843: getCachesStorageInfo method added.
	
	Revision 6.8  2017/06/23 09:47:49  lucio.pulvirenti
	S3MPM-873: stdint.h include added.
	
	Revision 6.7  2017/06/23 09:29:06  lucio.pulvirenti
	S3MPM-873: Added flag to checkPermission (default true) to methods uploading and downloading files and directories.
	Added group string (default empty) to methods downloading files and directories. Added method to get user and group from the ids. Added method to remove entry like in FS.
	
	Revision 6.6  2017/06/13 14:05:01  lucio.pulvirenti
	S3MPM-873: user string input param replaced by uid_t and gid_t added to getList, access, getAttributes methods IFs.
	boolean input pointer removed from getList method IF.
	
	Revision 6.5  2017/05/22 10:08:53  lucio.pulvirenti
	S3MPM-873: parameters with default values added to remove IF: bool (File System like, default false), int ptr (if ptr valorized, error code like errno).
	private method internalremove removed.
	
	Revision 6.4  2017/05/18 10:38:06  lucio.pulvirenti
	S3MPM-873: added optional bool parameter to download method to remove local file is existing (default false). getList and getAttributes IFs changed.
	
	Revision 6.3  2017/03/15 13:19:27  lucio.pulvirenti
	S3MPM-873: getList IF changed (user input par removed). getAttributes and convert methods added.
	
	Revision 6.2  2017/03/08 12:55:26  lucio.pulvirenti
	S3MPM-873: getList returns an integer var; added errnum to IF. access method added.
	
	Revision 6.1  2017/02/27 16:16:43  lucio.pulvirenti
	S3MPM-873: ls method added.
	
	Revision 6.0  2016/11/14 10:53:52  lucio.pulvirenti
	ISM-113: If Client runs on the same Server host, LocalTransferMode tag searched in SMConfiguration node to override transfer modes. Possibility to override
	also if IsmLocalTransferMode node occurring in caller Application conf space.
	
	Revision 5.6  2016/05/09 09:19:34  lucio.pulvirenti
	PDSEV-34: _deleteUploadedFileFromDisk attribute and related get method removed.
	
	Revision 5.5  2015/10/29 10:06:52  lucio.pulvirenti
	ARK-3: setUploadMode and setDownloadMode public methods added. getTransferModeEnum method now public (it was private).
	
	Revision 5.4  2015/10/20 15:56:55  lucio.pulvirenti
	ARK-3: work in progress.
	
	Revision 5.3  2015/09/23 10:48:50  lucio.pulvirenti
	APF-312: end points now put in a static map that may have the t_storagemanager id as key. It can be used safely in a mutlithread context with n storage manager.
	
	Revision 5.2  2014/06/27 14:40:09  marpas
	removed deprecated FtpCurl methods (now uses RemoteSite class)
	WorkerThread and Enpoints internal classes removed from header file (are in the compilation unit now)
	codind best practices applied
	potential umask race conditions fixed: now uses ProcessStatus from libException
	
	Revision 5.1  2014/01/27 13:07:53  lucio.pulvirenti
	noChangingHardlinkedFilesMode configuration parameter introduced not to change permissions of files uploaded with hard link (after S2 Claudio Nasuti request).
	
	Revision 5.0  2013/07/22 18:15:54  marpas
	adopting libException 5.x standards
	
	Revision 1.39  2013/07/12 14:16:54  marpas
	work in progress
	
	Revision 1.38  2013/04/03 13:50:35  marpas
	run does not longer gets a void*
	
	Revision 1.37  2013/04/03 12:46:51  marpas
	Thread::run gets a void*
	
	Revision 1.36  2012/06/01 10:46:46  lucio.pulvirenti
	getumask method added to WorkerThread class
	
	Revision 1.35  2012/05/31 16:27:47  lucio.pulvirenti
	work in progress
	
	Revision 1.34  2012/05/24 14:04:19  lucio.pulvirenti
	getValueFromFaultDetail method added
	
	Revision 1.33  2012/04/16 15:21:12  lucio.pulvirenti
	dedicatedTransferMode private method added
	
	Revision 1.32  2012/02/08 16:47:14  marpas
	removing compiler warning
	
	Revision 1.31  2011/04/18 09:44:18  lucio.pulvirenti
	setConnParameters: input resourceSet now const
	
	Revision 1.30  2011/02/23 11:25:13  lucio.pulvirenti
	internalRemove private method added
	
	Revision 1.29  2011/02/22 17:00:57  lucio.pulvirenti
	work in progress
	
	Revision 1.28  2011/02/16 15:10:26  lucio.pulvirenti
	upload methods IF changed so that input fileDefinition struct no more const. This allows the method itself
	to set the right value of fileDefinition::isDir flag depending on the input type (localpath file/directory)
	
	Revision 1.27  2011/02/16 12:13:49  lucio.pulvirenti
	added methods IF for directories download
	
	Revision 1.26  2011/02/11 14:54:54  lucio.pulvirenti
	work in progress
	
	Revision 1.25  2011/02/02 16:29:14  lucio.pulvirenti
	work in progress
	
	Revision 1.24  2011/02/01 15:56:38  lucio.pulvirenti
	work in progress
	
	Revision 1.23  2011/01/31 17:03:54  lucio.pulvirenti
	work in progress
	
	Revision 1.22  2011/01/28 10:12:58  lucio.pulvirenti
	class WorkerThread: added a CTOR taking as input strings vectors; _remoteUrl and _localpath private attr turned into string vectors
	The formed upload method now returns std::vector< std::pair<std::string , std::string> >
	Added void upload method with std::vector< std::pair<std::string , std::string> > & in IF
	Added convert method to convert fileDefinition into Ism__dirDefinition
	The former void upload method now turned into internalFileUpload private method; added internalDirectoryUpload private method
	
	Revision 1.21  2011/01/26 16:39:38  lucio.pulvirenti
	work in progress
	
	Revision 1.20  2011/01/25 18:03:03  lucio.pulvirenti
	work in progress
	
	Revision 1.19  2011/01/20 17:44:26  lucio.pulvirenti
	work in progress
	
	Revision 1.18  2011/01/18 15:29:27  lucio.pulvirenti
	FileToDownloadDisappearedExc added
	
	Revision 1.17  2011/01/17 12:00:54  lucio.pulvirenti
	TransferMode enum added to class IF
	class FtpThread name turned into WorkerThread and added private methods
	getTransferModeString and getTransferModeEnum methods added
	_uploadMode and _downloadMode TransferMode private attributes added
	
	Revision 1.16  2010/12/22 15:32:26  lucio.pulvirenti
	internaldownload method and stopException added
	
	Revision 1.15  2010/11/29 14:47:40  lucio.pulvirenti
	ftpTimeout and ftpInterval attributes added to class IF to manage possible exception of ftpCurl
	Those parameters are expected to be taken from configuration blob: if they do not
	occur, warning is emitted the first time only.
	
	Revision 1.14  2010/11/12 11:01:52  marpas
	messages and make-up
	
	Revision 1.13  2010/11/11 17:35:41  lucio.pulvirenti
	endpoints concept introduced for multithread
	test improved
	
	Revision 1.12  2010/11/02 17:24:02  lucio.pulvirenti
	_lastendpoint now static
	
	Revision 1.11  2009/11/13 17:57:31  lucio.pulvirenti
	Connection timeout introduced: got from rs file
	
	Revision 1.10  2009/10/06 13:00:05  lucio.pulvirenti
	setPassive method added to set passive mode
	
	Revision 1.9  2009/06/26 10:44:17  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.8  2009/06/19 10:46:28  lucio.pulvirenti
	Possibility to set password encryption algo to decript password.
	If no algo set, password is assumed not encrypted
	
	Revision 1.7  2009/06/16 14:53:11  marpas
	new client interface for parameters, older one still valid
	multiport(2) attempts to connect the server (if port2 set)
	.so dependencies changed (new types used)
	
	Revision 1.6  2009/03/19 18:33:07  lucio.pulvirenti
	book method added
	
	Revision 1.5  2008/06/10 23:50:23  paoscu
	using strings instead of ids for user and group
	
	Revision 1.4  2008/05/29 17:56:33  paoscu
	Added FTP support
	
	Revision 1.3  2008/05/28 17:32:25  paoscu
	Added download methods
	
	Revision 1.2  2008/05/14 15:23:27  paoscu
	Delete function added
	
	Revision 1.1.1.1  2008/05/07 17:48:45  paoscu
	Imported into CVS
	
        
*/

#ifndef _IsmSoapClient_H_
#define _IsmSoapClient_H_

#ifdef EXPRIVIA_ISM_USE_AWS_S3
#include <AwsS3Wrapper.h>
#endif

#include <PackagingAlgo.h>
#include <dbDownloadFile.h>
#include <acs_c++config.hpp>
#include <Thread.h>
#include <Dir.h>
#include <exException.h>
#include <ACS_SSL.h>

#include <stdint.h>
#include <map>

_ACS_BEGIN_NAMESPACE(acs) 


class rsResourceSet ;
class Timer;
class Ism__fileDefinition ;
class Ism__getFileIdResponse ;
class Ism__DownloadFileRequest ;
class Ism__dirDefinition ;
class Ism__downloadDirRequest ;
class Ism__fileAttributes;
class Ism__DownloadFileRequest_2 ;
class Ism__downloadMultiFilesRequest;

#ifdef EXPRIVIA_ISM_USE_AWS_S3
class Ism__cacheConfiguration;
class Ism__cacheConfigurationAdditionalParms;
#endif
/**
 * \brief
 * <Insert Class Description> 
 **/

class IsmSoapClient // PRQA S 2109
{ 
public:

	struct fileDefinition // PRQA S 2171, 2173, 2175
	{
	public:
		fileDefinition () : 
            fileId(0), 
 			filePath(),
			isDir(),
			size(),
			user(),
			group(),
			permissions(),
			fileClass(),
			customAttributes(),
			originalFilename(),
			packagingAlgo()
        { }

		~fileDefinition () throw() {}
		
#if 0		
		fileDefinition(fileDefinition const &fd):
            fileId(fd,fileId), 
 			filePath(fd.filePath),
			isDir(fd.isDir),
			size(fd.size),
			user(fd.user),
			group(fd.group),
			permissions(fd.permissions),
			fileClass(fd.fileClass),
			customAttributes(fd.customAttributes),
			originalFilename(fd.originalFilename),
			packagingAlgo(fd.packagingAlgo)
		{
		}								

		fileDefinition & fileDefinition::operator = (fileDefinition const &fd)
		{
			if( this != &fd ) {
            	fileId = fd.fileId ; 
 				filePath = fd.filePath ;
				isDir = fd.isDir ;
				size = fd.size ;
				user = fd.user ;
				group = fd.group ;
				permissions = fd.permissions ;
				fileClass = fd.fileClass ;
				customAttributes = fd.customAttributes ;
				originalFilename = fd.originalFilename;
				packagingAlgo = fd.packagingAlgo;
			}
			return (*this);
		}								
#endif

		int         fileId ; // PRQA S 2100 10
		std::string filePath ;
		bool        isDir ;
		long long   size ;
		std::string user ;
		std::string group ;
		short int   permissions ;
		std::string fileClass ;

		std::map<std::string , std::string> customAttributes ;
		std::string originalFilename;
		std::string packagingAlgo;
	private:
//		fileDefinition(const fileDefinition & );
//		fileDefinition &operator=(const fileDefinition &);
	} ;

	enum TransferMode {
		FTP,
		COPY,
		LINK,
		AWS_S3 // Amazon S3 or compatible (Orange)
	};

	enum Operation {
		UPLOAD ,
		DOWNLOAD
	};
		
		
	struct PackageAlgoOption { // PRQA S 2173, 2175
		
		PackageAlgoOption() ; 
		~PackageAlgoOption() throw() {} 
		PackageAlgoOption(const PackageAlgoOption& ) ;
		PackageAlgoOption& operator=(const PackageAlgoOption& ) ;
		
		std::string key; // PRQA S 2100 2
		std::string value;
	} ;


public:

	/** IsmSoapClient Exceptions */
	exDECLARE_EXCEPTION(Exception, exException) ; // Base IsmSoapClient Exception. // PRQA S 2131, 2502 6
	exDECLARE_EXCEPTION(TimeOutException, Exception) ; // timeout Exception. // PRQA S 2153 5
	exDECLARE_EXCEPTION(CriticalException, Exception) ; // Critical Exception.
	exDECLARE_EXCEPTION(ConnectionException, Exception) ; // Connection refused Exception.
	exDECLARE_EXCEPTION(StopException, Exception) ; // Client has been stopped by parent Exception.
	exDECLARE_EXCEPTION(FileToDownloadDisappearedExc, Exception) ; // File to download has been disappeared (probably due to VFS concurrency)
	exDECLARE_EXCEPTION(FileToDeleteOnMigratingMediumExc, Exception) ; // File cannot be deleted since on migrating medium 
	exDECLARE_EXCEPTION(SoapReceiveTimeOutException, Exception) ; // timeout Exception. // PRQA S 2153 5

	/** Default Class constructor */
	explicit IsmSoapClient(StopController const *sc = 0 );
	/** Destructor */
	virtual ~IsmSoapClient() throw() ;

	void setPort(int) ; 
	void setPort2(int) ; 
	void setHost(std::string const &) ;
	void setUser(std::string const &) ;
	void setFtpUser(std::string const &) ;
	void setFtpPassword(std::string const &) ;
	void setEncryptionAlgo(int);
	void setPassive(bool);
	void setConnectTimeout(unsigned int msec);
	void setUploadMode(TransferMode const & v) { _uploadMode = v; }
	void setDownloadMode(TransferMode const & v) { _downloadMode = v; }

	void setConnParameters(rsResourceSet const &, long long int = 0) ;

	void loadXmlConnParameters(std::string const &, long long int = 0) ;
	void loadRsConnParameters(std::string const &, long long int = 0) ;

	std::vector< std::pair<std::string , std::string> > upload( fileDefinition & f , std::string  const & localpath , bool recursive, bool checkPermissions = true, bool extendDirPermissions = true) ;
	void upload( fileDefinition & f , std::string const &  localpath , bool recursive, std::vector< std::pair<std::string , std::string> > &, bool checkPermissions = true, bool extendDirPermissions = true ) ;

	void remove( std::string  const &  filePath , std::string  const & user ) ; // PRQA S 2502

	int getFileId( std::string  const &  filePath , std::string  const & user ) ;

 	void download( std::string  const &  filePath , std::string  const & user ,  std::string  const & localpath , int timeOut = -1, bool removeLocalPathIfExisting = false, std::string const & group = std::string(), bool checkPermissions = true ) ;

	void book( std::string  const &  filePath , std::string  const & user ,  int hoursToDeadline=-1,  float latencyDays=0.0 ) ;

	void giveupFileRequest( std::string  const &  filePath , std::string  const & user, std::string const & clientReference ) ; // PRQA S 2502
	
	void isFileOnCache( std::string  const &  filePath , std::string  const & user, std::string const & group, dbFileAction::CacheStatus & ) ; // PRQA S 2502

	void putFileOnCache( std::string  const &  filePath , std::string  const & user, std::string const & group, std::string const & clientReference, dbFileAction::CacheStatus & ) ; // PRQA S 2502

	void releaseCacheFile( std::string  const &  filePath , std::string  const & user, std::string const & group ) ; // PRQA S 2502

	/** mainly for debug purposes */
	static std::string getTransferModeString(TransferMode const &);
    
    const std::string &getFtpUser() const throw() { return _ftpUser ; }
    const std::string &getUser() const throw() { return _user ; }
    const std::string &getHost() const throw() { return _host ; }
    const std::string &getFtpPassword() const throw() { return _ftpPassword ; }
    bool getPassive() const throw() { return _passive ; }
    int getFtpTotalTimeout_s() const throw() { return _ftpTotalTimeout_s ; }
    int getFtpRetryingInterval_s() const throw() { return _ftpRetryingInterval_s ; }

#ifdef EXPRIVIA_ISM_USE_AWS_S3 
	int getAwsS3TotalTimeout_s() const throw() { return _awsS3TotalTimeout_s ; }
    int getAwsS3RetryingInterval_s() const throw() { return _awsS3RetryingInterval_s ; }
#endif
	
    ACS_SSL::EncryptionAlgo getPwdEncryptionAlgo() const throw() { return _pwdEncryptionAlgo ; }
	bool getNoChangingHardlinkedFilesMode() const throw() { return _noChangingHardlinkedFilesMode ; } 
	TransferMode getUploadMode() const throw() { return _uploadMode; }
	TransferMode getDownloadMode() const throw() { return _downloadMode; }
	TransferMode getTransferModeEnum(std::string const &);
	
#ifdef EXPRIVIA_ISM_USE_AWS_S3 
	
	AwsS3Wrapper::AwsS3WrapperConfiguration getAwsS3WrapperConfiguration() const throw() { return  _awsS3WrapperConfiguration ; }

	void setCache_config_endpoint ( const std::string & v) throw()  { _awsS3WrapperConfiguration.set_aws_endpoint(v); }
	void setCache_config_region ( const std::string & v) throw()  { _awsS3WrapperConfiguration.set_aws_region(v); }
	
#endif	
	
	int getPackageAlgoTimeout() const throw() { return _packageAlgoTimeout ; }
	std::map<std::string, std::string> getPackageAlgoArchiveOptions() const throw() { return _packageAlgoArchiveOptions ; }
	std::map<std::string, std::string> getPackageAlgoExtractOptions() const throw() { return _packageAlgoExtractOptions ; }
	PackagingAlgo::FilesChangeFixStrategy getPackageAlgoFilesChangeFixStrategy() const throw() { return _packageAlgoFilesChangeFixStrategy ; }

	
	int getList( std::string  const &  filePath , uid_t , gid_t, int32_t offset, size_t entries, std::vector<std::string> & listedPaths, int32_t & newOffset, int & errnum) ;
	
	int access( std::string  const &  filePath , uid_t , gid_t , uint32_t mode, int & errnum);
	
	int getAttributes( std::string const & filePath, uid_t , gid_t , struct stat &, int & errnum);

	int setAttributes( std::string const & filePath, const uid_t& , const gid_t& , const struct stat &, int & errnum);
	
	void getUserGroup( uid_t , gid_t , std::string & user, std::string & group );
	
	/**
	 * \brief Remove filePath from ISM by checking access as a real File System and not removing contained entries
	 * \param filePath The file full path
	 * \param The id of user executing the command
	 * \param The id of group of the user executing the command
	 * \param errnum It contains error number : 0 if successful, otherwise like errno
	 * \param recursive If set, entry is removed as well even if not empty.
	 */
	void remove( std::string  const &  filePath , uid_t , gid_t , int & errnum, bool recursive = false);
	
	/**
	 * \brief Get Caches storage info as available space
	 * \param Vector of storage info, every element being a pair (file class, free size [MB]). The same file class can occur more than once since associated to different caches
	 */
	void getCachesStorageInfo(std::vector< std::pair<std::string, long long int> >  &)  ; 

	/**
	 * \brief Check if client and server are the same host. Client name is got by HWScan::hostname() that uses C function gethostname
	 * \param serverHost The name of server host
	 * \retval True if client and server are the same host 
	 */
	bool clientAndServerSameHost( const std::string & serverHost );
	
	/**
	 * \brief Set _soapRecvTimeout in ms
	 */
	void setSoapRecvTimeout( const int timeout_ms) ;

	/**
	 * \brief Set _soapRecvTotalTimeout in ms
	 */
	void setSoapRecvTotalTimeout( const int timeout_ms) ;
	const int & getSoapRecvTotalTimeout() const throw() { return _soapRecvTotalTimeout ; }
		
    const std::string &getClientReference() const throw() { return _clientReference ; }
	void setClientReference ( const std::string & v) throw()  { _clientReference = v; }

protected:
	void prepareEndpoint(int which = -1) ;


private:
	/** Copy Constructor - not implemented */
	IsmSoapClient(const IsmSoapClient & );
	/** Operator = - not implemented */
	IsmSoapClient &operator=(const IsmSoapClient &);
	
#ifdef ACS_TEST
public:
#else
private:
#endif

	void retryConnectionToServer( std::string  const & errMsg, Timer & timer, int attempt, unsigned short &endpointToTry, bool soapRecvTimeout = false );
	static void convert(IsmSoapClient::fileDefinition const &, Ism__fileDefinition & ) ;
	static void convert(IsmSoapClient::fileDefinition const &,
						std::string const & localDirPath,
						DirTree const &, 
						std::vector<std::string> & localFilepaths,
						Ism__dirDefinition & ) ;
	static void convert(const Ism__fileAttributes &, struct stat &);

	static void convert(const struct stat &, Ism__fileAttributes &);
	static void convert(const Ism__DownloadFileRequest &, const std::string & clientReference, Ism__DownloadFileRequest_2 &);

	void getFileIdInternal( std::string  const &  filePath , std::string  const & user, Ism__getFileIdResponse & ) ;

	void internalFileUpload( fileDefinition const & f , std::string  const & localpath , bool recursive, bool checkPermissions, const long long int realFileSize, std::vector< std::pair<std::string , std::string> > &) ;
	void internalDirectoryUpload( fileDefinition const &, std::string const & localpath , bool recursive, std::vector< std::pair<std::string , std::string> > &, \
									bool checkPermissions, bool extendDirPermissions);

	void fileDownload( std::string  const & filePath , int fileId, std::string  const & user ,  std::string  const & localpath , int timeOut, std::string  const & group, bool checkPermissions, std::string const & clientReference );
	bool internalFileDownload( std::string const & localpath, Ism__DownloadFileRequest &, int downloadTimeout,  int transferTimeout, std::string const & clientRef );

	void directoryDownload( std::string  const & filePath , int fileId, std::string  const & user ,  std::string  const & localpath , int timeOut, std::string  const & group, bool checkPermissions );
	bool internalDirectoryDownload( std::vector<std::string> const & targetFilepaths, Ism__downloadDirRequest &, int downloadTimeout );

	/** Returns true if transfer mode for the present client has been specified */
	void checkLocalTransferModes(std::string const & serverHost, rsResourceSet const &, std::string & uploadModality, std::string & downloadModality) ;
	bool overrideTransferMode(rsResourceSet const &, std::string const & modeTag, std::string & transferMode);

	void getValueFromFaultDetail(std::string const & field, int & value, const char * faultDetail);
    bool isCancelRequested() const throw() { return _sc && _sc->requestedCancel() ; }
	
#ifdef EXPRIVIA_ISM_USE_AWS_S3 
	void getCacheConfigurationInternal( const std::string & pathInCache , Ism__cacheConfiguration &  );
	void getCacheConfigurationAddParamsInternal( const std::string & pathInCache , Ism__cacheConfigurationAdditionalParms &  ) ;
	void disassembleAWS_S3FilePath( const std::string & filePath, std::string & bucket, std::string & cachePath, std::string & fileUrl ) ;
	void getAWS_S3ObjectMetadata( const std::string & bucket, const std::string & cachePath, const std::string & fileUrl, std::multimap <AwsS3Wrapper::Enum_S3MetadataTag, std::string> & , Ism__cacheConfiguration &  ) ;
#endif
	
	// ask SOAP Server its version and set in attribute
	void setServerVersion();
	void abortDownload( Ism__DownloadFileRequest &, std::string & outmsg ) ;
	void abortMultiDownload( Ism__downloadMultiFilesRequest *, std::string const & localpath, std::string & outmsg ) ;

private:	
	std::string _host ;
	int _port0 ;
	int _port1 ;

	int _defaultTimeout ;
	std::string _user ;
	std::string _ftpUser ;
	std::string _ftpPassword ;
	ACS_SSL::EncryptionAlgo _pwdEncryptionAlgo ;
	bool _passive ;	
	StopController const *_sc ;
	unsigned int _connectTimeout_ms ; // msec
	static const unsigned int _defaultConnectTimeout = 3000 ; //  - 3 secs

	int _ftpTotalTimeout_s ; // sec
	static const int _defaultFtpTotalTimeout_s = 10 ; // secs

	int _ftpRetryingInterval_s ; // sec
	static const int _defaultFtpRetryingInterval_s = 5 ; // secs
	


	static bool _emitWarning4FtpTimeout;
	static bool _emitNoChangingHardlinkedFilesMode;
	static bool _emitWarning4GiveupFileRequest;
	static bool _emitWarning4IsFileOnCache;
	static bool _emitWarning4PutFileOnCache;
	static bool _emitWarning4ReleaseCacheFile;

	static const int _transferOperationTimeout_s = 7200 ; // secs

	TransferMode _uploadMode;
	TransferMode _downloadMode;

	// do not change files permissions when uploading files into cache with hard link
	bool _noChangingHardlinkedFilesMode;
	
	long long int _endPointsKey;

#ifdef EXPRIVIA_ISM_USE_AWS_S3        	
	int _awsS3TotalTimeout_s ;
    int _awsS3RetryingInterval_s ;
	
	AwsS3Wrapper::AwsS3WrapperConfiguration _awsS3WrapperConfiguration ;
#endif
	
	std::string _packageAlgo;
	std::string _packageExt;
	int _packageAlgoTimeout;
	std::map<std::string, std::string> _packageAlgoArchiveOptions ;
	std::map<std::string, std::string> _packageAlgoExtractOptions ;
	PackagingAlgo::FilesChangeFixStrategy _packageAlgoFilesChangeFixStrategy ;

	int _soapRecvTimeout; // to set proxy recv_timeout variable
	int _soapRecvTotalTimeout; // total timeout to wait before exiting with error in case of repeated SOAP timeout
	int _serverVersion ; // it is the Server Version to discriminate which Interfaces are to be used
	std::string _clientReference ;
	static const int _defaultSoapRecvTimeout = 30000; // ms -> 30 s
	static const int _defaultSoapRecvTimeoutAttempts = 3; // number of attempts to repeat request in case of SOAP timeout
	static const int _soapSendTimeout = 5; // s
	
	static ThreadSafe _endPointsMutex;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(IsmSoapClient) ;

};


_ACS_END_NAMESPACE

#endif //_IsmSoapClient_H_

