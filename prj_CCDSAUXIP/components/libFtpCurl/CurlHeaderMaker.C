// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA
 *
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.8  2016/03/22 16:29:20  francesco.avanzi
 *  S2PDGS-1434: debug enhanced
 *
 *  Revision 5.7  2015/04/28 09:30:42  marfav
 *  Do not disable Extended Passive Mode in FTP PUTs anymore
 *
 *  Revision 5.6  2015/04/16 13:07:07  marpas
 *  useless method removed
 *
 *  Revision 5.5  2014/11/03 17:01:56  marpas
 *  simplyfied
 *
 *  Revision 5.4  2014/06/19 17:58:30  francesco.avanzi
 *  better coding rules applied
 *
 *  Revision 5.3  2014/06/04 17:29:21  marfav
 *  QA warning removed
 *  HTTP sessions managed without SSL layer
 *  CURL locking callback functions strategy revised
 *
 *  Revision 5.2  2013/10/24 18:27:59  marpas
 *  work in progress
 *
 *  Revision 5.1  2013/10/22 17:31:50  marpas
 *  work in progress
 *
 *  Revision 5.0  2013/06/06 18:08:20  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.14  2012/12/05 10:31:19  marpas
 *  getting rid of FilenameUtils
 *
 *  Revision 2.13  2012/12/03 16:56:48  marpas
 *  using new StringUtils interface
 *
 *  Revision 2.12  2012/11/29 16:28:21  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.11  2012/11/28 18:16:23  marpas
 *  qa rules,
 *  optimization and robustness
 *  still work in progress
 *
 *  Revision 2.10  2012/07/24 13:47:08  micmaz
 *  Special characters as #,@,: in URLs have to be hex encoded.
 *
 *  Revision 2.9  2012/03/27 18:08:49  micmaz
 *  work in progress
 *
 *  Revision 2.8  2012/03/27 10:52:21  micmaz
 *  work in progress
 *
 *  Revision 2.7  2012/03/16 17:38:06  micmaz
 *  *** empty log message ***
 *
 *  Revision 2.6  2012/03/16 17:37:45  micmaz
 *  *** empty log message ***
 *
 *  Revision 2.5  2012/03/08 13:11:40  micmaz
 *  added media functionality
 *
 *  Revision 2.4  2012/02/07 15:49:26  micmaz
 *  When transferring a directory FtpCurl must not create immediately the destination dir.
 *
 *  Revision 2.3  2012/02/07 10:25:46  micmaz
 *  removed warnings .
 *
 *  Revision 2.2  2012/01/20 16:05:29  micmaz
 *  modified logs
 *
 *  Revision 2.1  2012/01/16 10:18:39  micmaz
 *  IN PROGRESS - issue S3PDGS-542: DC problem at explicit request
 *  http://jira.acsys.it/browse/S3PDGS-542
 *
 *  Revision 2.0  2011/10/18 14:43:50  marpas
 *  new statistics adopted
 *
 *  Revision 1.13  2011/09/29 16:46:53  marpas
 *  rules fixing
 *
 *  Revision 1.12  2011/08/12 14:49:04  micmaz
 *  work in progress..
 *
 *  Revision 1.11  2011/05/06 16:31:21  micmaz
 *  work in progress...
 *
 *  Revision 1.10  2011/05/05 17:52:33  micmaz
 *  work in progress...
 *
 *  Revision 1.9  2011/04/29 17:20:41  micmaz
 *  check style changes
 *
 *  Revision 1.8  2011/03/23 10:04:03  micmaz
 *  work in progress...
 *
 *  Revision 1.7  2011/03/15 13:25:18  micmaz
 *  added the TYPE I command in order to force binary trasmission (only ftp mode).
 *
 *  Revision 1.6  2011/02/23 14:39:26  micmaz
 *  Added: Ftpes support (but not tested) and CirculationCurl in order to make a file-transfer using this server as bridge.
 *
 *  Revision 1.5  2011/01/28 18:55:55  micmaz
 *  work in  progress...
 *
 *  Revision 1.4  2011/01/27 16:13:28  micmaz
 *  adapted the library to old libcURL 1.15
 *
 *  Revision 1.3  2011/01/26 13:38:21  micmaz
 *  work in  progress...
 *
 *  Revision 1.2  2011/01/25 19:35:42  micmaz
 *  work in  progress...
 *
 *  Revision 1.1  2011/01/19 16:42:58  micmaz
 *  added the openConnection() and closeConnection() to cURL operations.
 *
 *
 *
 */

#include <CurlHeaderMaker.h>

#include <StringUtils.h>
#include <File.h>
#include <Filterables.h>

#include <sstream>

#include <curl/easy.h> // PRQA S 1013

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) 

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(CurlHeaderMaker)

//CurlHeaderMaker::CurlHeaderMaker() :
//        _headerList(0), _fileHandler(0), _res(), _url(), _site(), _curl(0), _strings() {
//    // empty
//}

CurlHeaderMaker::CurlHeaderMaker(RemoteSite const& site, CURL* curl) : // NOSONAR - external type (CURL)
    _site(site), 
    _curl(curl)
{
}

CurlHeaderMaker::~CurlHeaderMaker() noexcept {
    close();
}

void CurlHeaderMaker::close() noexcept {
    //if the header list was allocated, free it.
    if (_headerList) {
        curl_slist_free_all(_headerList);
        _headerList = nullptr ;
    }
    
    // if the pre-command-list was allocated, free it
    if (_preCommandList) {
        curl_slist_free_all(_preCommandList);
        _preCommandList = nullptr ;
    }
    
    //if the file handler was opened, close it.
    if (_fileHandler) {
        fclose(_fileHandler);
        _fileHandler = nullptr ;
    }
}

const string &CurlHeaderMaker::getURL() const noexcept { // PRQA S 4120
    return _url;
}

void CurlHeaderMaker::setSite(RemoteSite const& site) // PRQA S 4121
{
    _site = site;
}

void CurlHeaderMaker::setCURL(CURL* curl)  // NOSONAR - external type (CURL)
{
    _curl = curl;
}

void CurlHeaderMaker::addHeader(string const& command) {

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Appending: \"" << command << "\"");
    _headerList = curl_slist_append(_headerList, command.c_str());
}

void CurlHeaderMaker::addPreCommand(string const& command) {
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Appending pre-command: \"" << command << "\"");
	_preCommandList = curl_slist_append(_preCommandList, command.c_str());
}

void CurlHeaderMaker::makeGet(string const& fileToTransfer, void* data, bool quoteOnly) { // NOSONAR - data to be passed to curl
    //construct the correct url.
    _url = makeURL(fileToTransfer);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Make Get: " << _url);
    curl_easy_setopt(_curl, CURLOPT_URL, _url.c_str());
    if (_site.getIsAnyFtp()) {
        //if the protocol is any FTP (ftp, ftps, ftpes), then force the binary transport (cURL should be do this but doesn't).
        _headerList = curl_slist_append(_headerList, "TYPE I");
    }
    //if some custom data were inserted, add them to the easy opts.
    if (data) {
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, data);
    }
	if (quoteOnly)
	{
		// Just execute quote commands without asking for the resource
		curl_easy_setopt(_curl, CURLOPT_NOBODY, 1L);
	}

}

string CurlHeaderMaker::makeProtocol() const {
    //decides which protocol must be used.
    string url;
    if (_site.getIsFtp() || _site.getIsFtpes()) {
        url += "ftp://"; //the same protocol for ftpes and ftp // NOSONAR 
    } 
    if (_site.getIsFtps()) {
        url += "ftps://"; //the same protocol for ftps and ftp
    }
    else if (_site.getIsSFTP())
    {
      url += "sftp://";
    }
    else if (_site.getIsHttps())
    {
      url += "https://";
    }
  else if( _site.getIsHttp() ) {
		url += "http://"; // NOSONAR
	}
    else { /* nothing */ }
    
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, _site.str());

    return url;
}

string CurlHeaderMaker::makeURL(string const& remoteFilename) const {
	string src;
	
	// Double slashes in ftp urls specifies that the remote path starts from the ROOT directory,
	// while single slash specifies that the remote path starts from the HOME directory.
	// The following if-else statement is used to let the function support also http url format
  if (_site.getIsHttp() || _site.getIsHttps())
  {
    src = StringUtils::transform(_site.getHostAndPort() + "/" + remoteFilename, "//", "/");
  }
  else
  {
    src = StringUtils::transform(_site.getHostAndPort() + "/" + remoteFilename, "/", "//");
  }

    // Replace special characters with hexadecimal code.
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Is ftp set protocol " << _site.isFtpSetProtocol());
    if ( _site.isFtpSetProtocol()) {
        map<char, string > tr ; 
        tr['#'] = "%23" ;
        tr[' '] = "%20" ;
        tr['@'] = "%40" ;
        src = StringUtils::transform(src, tr ) ;
    }
    string url = makeProtocol() + src;
    return url;
}

void* CurlHeaderMaker::makeReadData(string const& fileToTransfer, string const& ) { // NOSONAR 
    struct stat file_info; // PRQA S 4102
    /* get the file size of the local file */
    if (0 != stat(fileToTransfer.c_str(), &file_info)) {
        ACS_THROW(exIOException("Cannot open '" + fileToTransfer + "'", errno)) ; 
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Local file size: " << file_info.st_size << " bytes");

    /* get a FILE * of the same file */
    _fileHandler = fopen(fileToTransfer.c_str(), "rb");

    /* Set the size of the file to upload (optional).  If you give a *_LARGE
     option you MUST make sure that the type of the passed-in argument is a
     curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must
     make sure that to pass in a type 'long' argument. */
    curl_easy_setopt(_curl, CURLOPT_INFILESIZE_LARGE, curl_off_t(file_info.st_size)); // PRQA S 3081

    return _fileHandler;
}

void CurlHeaderMaker::makePut(string const& fileToTransfer, string const& targetFilename, void* handler) { // NOSONAR 
    if (!handler) {
        handler = makeReadData(fileToTransfer, targetFilename);
    }
    /* now specify which file to upload */
    curl_easy_setopt(_curl, CURLOPT_READDATA, handler);
#if ( (LIBCURL_VERSION_MAJOR > 7) || ( (LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR > 19)) )
    curl_easy_setopt(_curl, CURLOPT_FTP_CREATE_MISSING_DIRS, CURLFTP_CREATE_DIR_RETRY);
#else
#warning libcurl does not allow parallel creation of remote directories, please upgrade it to a version >= 7.20
    curl_easy_setopt(_curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
#endif

    curl_easy_setopt(_curl, CURLOPT_UPLOAD, 1);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Set create ftp flag to 1");
    if (_site.getIsAnyFtp()) {
        _headerList = curl_slist_append(_headerList, "TYPE I");
    }

    string uploadFileAs = targetFilename;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " remote file to upload: '" << uploadFileAs << "'");
    _url = makeURL(uploadFileAs);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Make Put: " << _url);
    curl_easy_setopt(_curl, CURLOPT_URL, _url.c_str());
}

void CurlHeaderMaker::makeHeader() // PRQA S 4211
{
    curl_easy_setopt(_curl, CURLOPT_NOSIGNAL, 1);
    if (_preCommandList) {
        curl_easy_setopt(_curl, CURLOPT_PREQUOTE, _preCommandList);
    }
    if (_headerList) {
        curl_easy_setopt(_curl, CURLOPT_POSTQUOTE, _headerList);
    }
}

_ACS_END_NESTED_NAMESPACE 
