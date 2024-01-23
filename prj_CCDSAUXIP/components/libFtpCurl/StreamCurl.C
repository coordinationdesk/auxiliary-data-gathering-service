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
 *  $Prod: A.C.S. libFtpCurl Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.18  2016/05/09 14:31:31  francesco.avanzi
 *  performTransferRenameChmod() call modified accordingly with new method signature.
 *  default parameters are no longer alloed, in order to avoid unwanted cast among void * and bool
 *
 *  Revision 5.17  2015/04/23 17:45:55  marpas
 *  when putting a file to a remote system, in case o renaming, the latter is performed in the same connection that actually transfered the file.
 *  the concept is still valid for stream transfers where the endpoint is a file.
 *  Even the final chmod is done accordingly (valid only for FtpCurl transfers, not fo CirculationCurl ones).
 *  It does not applies to directory because their children have to be transferred BEFORE the renaming.
 *
 *  Revision 5.16  2015/04/16 13:21:41  marpas
 *  qa warnings fixed
 *
 *  Revision 5.15  2015/03/10 14:54:00  lucio.pulvirenti
 *  S2PDGS-984: streamget and streamput: CurlUnrecoverableException thrown if exception caught.
 *
 *  Revision 5.14  2014/06/20 14:52:34  marpas
 *  useless private variables removed
 *  statistics management (log) fixed
 *  statistics management (t_circulationstats) fixed
 *  operation are now an enum
 *
 *  Revision 5.13  2014/06/19 18:06:37  francesco.avanzi
 *  CURLOPT_NOPROGRESS now set to false (as it should be) in StreamCurl::streamget()
 *
 *  Revision 5.12  2014/06/04 17:29:21  marfav
 *  QA warning removed
 *  HTTP sessions managed without SSL layer
 *  CURL locking callback functions strategy revised
 *
 *  Revision 5.11  2014/02/26 10:37:41  lucio.pulvirenti
 *  BEGIN_TRANSACTION parameter added.
 *
 *  Revision 5.10  2014/02/24 20:49:07  marpas
 *  perfo and progress data rationalization
 *
 *  Revision 5.9  2014/02/21 16:41:36  lucio.pulvirenti
 *  No more derived by BandPriority: added to IF.
 *
 *  Revision 5.8  2014/02/21 09:46:53  lucio.pulvirenti
 *  typedef removed.
 *
 *  Revision 5.7  2014/02/14 13:54:32  marpas
 *  attempt to refactor band shaping mechanism
 *
 *  Revision 5.6  2013/10/28 11:25:23  marpas
 *  coding best practices applied
 *  encapsulation enforced
 *  timing improved
 *  compilation warnings removed
 *  qa warnings removed
 *  robustness improved
 *
 *  Revision 5.5  2013/10/24 18:28:00  marpas
 *  work in progress
 *
 *  Revision 5.4  2013/10/23 17:28:11  marpas
 *  work in progress
 *
 *  Revision 5.3  2013/10/22 17:31:51  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/10/11 11:57:09  marpas
 *  using conditions
 *
 *  Revision 5.1  2013/06/18 12:53:56  marpas
 *  debug macros ratioanlized
 *
 *  Revision 5.0  2013/06/06 18:08:22  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.10  2013/04/15 09:12:34  marpas
 *  coding best practices applied
 *  interfaces rationalization and simplification
 *
 *  Revision 2.9  2012/11/27 13:28:58  micmaz
 *  added quality tool directives.
 *
 *  Revision 2.8  2012/03/16 15:50:29  micmaz
 *  *** empty log message ***
 *
 *  Revision 2.7  2012/02/09 16:48:25  micmaz
 *  added ftps CCC  configuration parameter.
 *
 *  Revision 2.6  2012/02/07 10:25:47  micmaz
 *  removed warnings .
 *
 *  Revision 2.5  2012/02/02 17:19:58  micmaz
 *  IN PROGRESS - issue S1PDGS-2769: The Circulation Curl does not take into account the slow parameter
 *  http://jira.acsys.it/browse/S1PDGS-2769
 *
 *  Revision 2.4  2012/01/31 12:25:03  micmaz
 *  added a new delay method.
 *
 *  Revision 2.3  2012/01/20 16:11:07  micmaz
 *  OPEN - issue S1PDGS-2685: Circulation Agent Core in transferring large files
 *  http://jira.acsys.it/browse/S1PDGS-2685
 *
 *  Revision 2.2  2012/01/16 10:18:39  micmaz
 *  IN PROGRESS - issue S3PDGS-542: DC problem at explicit request
 *  http://jira.acsys.it/browse/S3PDGS-542
 *
 *  Revision 2.1  2012/01/11 18:46:33  micmaz
 *  IN PROGRESS - issue S1PDGS-2685: Circulation Agent Core in transferring large files
 *  http://jira.acsys.it/browse/S1PDGS-2685
 *
 *  Revision 2.0  2011/10/18 14:43:52  marpas
 *  new statistics adopted
 *
 *  Revision 1.20  2011/09/01 14:25:18  micmaz
 *  work in progress.
 *
 *  Revision 1.19  2011/09/01 08:52:38  micmaz
 *  removed excerr.
 *
 *  Revision 1.18  2011/08/31 15:31:54  micmaz
 *  IN PROGRESS - issue S1PDGS-1841: the CirculationAgent consumes too many CPU time
 *  http://jira.acsys.it/browse/S1PDGS-1841
 *
 *  Revision 1.17  2011/08/31 15:11:09  micmaz
 *  IN PROGRESS - issue S1PDGS-1841: the CirculationAgent consumes too many CPU time
 *  http://jira.acsys.it/browse/S1PDGS-1841
 *
 *  Revision 1.16  2011/08/30 15:47:03  micmaz
 *  removed excerr
 *
 *  Revision 1.15  2011/08/26 16:18:17  micmaz
 *  IN PROGRESS - issue S1PDGS-1808: Improve the CirculationCurl bytes transfer
 *  http://jira.acsys.it/browse/S1PDGS-1808
 *
 *  Revision 1.14  2011/08/26 15:52:05  micmaz
 *  IN PROGRESS - issue S1PDGS-1604: CirculationAgent does not use t_circulationtempnames
 *  http://jira.acsys.it/browse/S1PDGS-1604
 *  IN PROGRESS - issue S1PDGS-1808: Improve the CirculationCurl bytes transfer
 *  http://jira.acsys.it/browse/S1PDGS-1808
 *
 *  Revision 1.13  2011/08/12 14:49:04  micmaz
 *  work in progress..
 *
 *  Revision 1.12  2011/07/16 09:49:15  micmaz
 *  corrected the stream curl class: now it computes correctly the total amount of read and wrote bytes.
 *
 *  Revision 1.11  2011/05/30 15:24:45  micmaz
 *  added localhost as valid remote host
 *
 *  Revision 1.10  2011/05/12 17:22:13  micmaz
 *  work in progress
 *
 *  Revision 1.9  2011/05/10 10:25:13  micmaz
 *  added a while() loop to slow the reading stream in order to limit the amount of bytes in memory.
 *
 *  Revision 1.8  2011/05/05 17:52:34  micmaz
 *  work in progress...
 *
 *  Revision 1.7  2011/04/29 17:20:42  micmaz
 *  check style changes
 *
 *  Revision 1.6  2011/04/12 18:02:26  micmaz
 *  work in progress
 *
 *  Revision 1.5  2011/04/11 12:25:06  micmaz
 *  work in progress
 *
 *  Revision 1.4  2011/04/08 15:37:40  micmaz
 *  work in progress
 *
 *  Revision 1.3  2011/04/05 17:15:09  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.2  2011/02/24 11:41:10  micmaz
 *  Added:  CirculationCurl in order to make  dir-transfer using this server as bridge.
 *
 *  Revision 1.1  2011/02/23 14:39:26  micmaz
 *  Added: Ftpes support (but not tested) and CirculationCurl in order to make a file-transfer using this server as bridge.
 *
 *
 *
 *
 */

#include <StreamCurl.h>

#include <File.h>
#include <Thread.h>
#include <FtpProgressData.h>
#include <CurlConfigurer.h>
#include <CurlStatistics.h>

#include <PipedBytesStream.h>

#include <Thread.h>

#include <Curl.h>

#include <curl/easy.h> // PRQA S 1013

#include <unistd.h>
#include <boost/shared_ptr.hpp> // PRQA S 1013


_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) ;

using namespace acs::io;
using std::string ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(StreamCurl)

StreamCurl::StreamCurl(const StopController& stopController, CURLcode& res, const BandPriority &bp) :
    Curl(), 
    _stopController(stopController), // PRQA S 2528 2
    _res(res),
    band_prio_(bp)
{
    setCreateMissingDirs(true);
}

StreamCurl::StreamCurl(RemoteSite const& site, 
                       const StopController& stopController, 
                       CURLcode& res, 
                       const BandPriority &bp) :
    Curl(site), 
    _stopController(stopController), // PRQA S 2528 2
    _res(res),
    band_prio_(bp)
{
    setCreateMissingDirs(true);
}


CURLcode StreamCurl::streamget(const string &remoteFilename) {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "getting " << remoteFilename << " into stream.");
    _bytesWrote = 0;
    const string operation = "streamget" ; 
    CurlStatistics::log(getSite(), operation);
    boost::shared_ptr<FtpProgressData> progressDataHandle(new FtpProgressData(_stopController, band_prio_));
    progressDataHandle->setPerfMonitor(&_perfo) ;
    _perfo.setRemoteSite(getSite()) ;
    _perfo.setOperation(operation) ;
    _perfo.start();
    CURLcode res = _res;
    try {
        BEGIN_TRANSACTION(TrGet, true)  // PRQA S 4236
            curl_easy_setopt(getCurl(), CURLOPT_WRITEFUNCTION, _streamGetCallback);
            curl_easy_setopt(getCurl(), CURLOPT_PROGRESSFUNCTION, FtpProgressData::progress_callback);
            curl_easy_setopt(getCurl(), CURLOPT_PROGRESSDATA, progressDataHandle.get());
            curl_easy_setopt(getCurl(), CURLOPT_NOPROGRESS, false);

            res = performTransfer(true, remoteFilename, "", 0, this);
            if (res != CURLE_OK) {
                _res = res;
            }
			ACS_CLASS_EQ_DEBUG(123) // PRQA S 3050
				ACS_LOG_DEBUG(SimpleDebugSignature << "This is a TEST: Setting CURLcode to CURLE_FAILED_INIT") ;
				_res = CURLE_FAILED_INIT;
			ACS_CLASS_END_DEBUG
            LOG_TRANSACTION(TrGet, remoteFilename, _res);
            logError(_res, remoteFilename);
            _outStream->close();
        END_TRANSACTION;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "end transferring");
        _perfo.stop();
        updateStatistics(_res);
        deregisterProgressCallback();
    } 
    catch (std::exception const& ex) { // NOSONAR - any exception
		string msg("end error transferring");
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, msg);
        _outStream->close();
		ACS_THROW( CurlUnrecoverableException(ex, msg) );
    }
    _perfo.stop();
    deregisterProgressCallback();
    CurlStatistics::logResult(_res, getSite(), operation);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Returning " << int(_res) << " code"); // PRQA S 3081
    return _res;
}

CURLcode StreamCurl::streamput(const string &remoteFilename) {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "putting " << remoteFilename);
    const string operation = "streamput" ; 
    _bytesRead = 0;
    CurlStatistics::log(getSite(), operation);
    boost::shared_ptr<FtpProgressData> progressDataHandle(new FtpProgressData(_stopController, band_prio_));
    progressDataHandle->setPerfMonitor(&_perfo) ;
    _perfo.setRemoteSite(getSite()) ;
    _perfo.setOperation(operation) ;
    _perfo.start();
    CURLcode res = _res;
    try {
        BEGIN_TRANSACTION(TrPut, true) // PRQA S 4236
            curl_easy_setopt(getCurl(), CURLOPT_READFUNCTION, _streamPutCallback);
#if ( (LIBCURL_VERSION_MAJOR > 7) || ( (LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR > 19)) )
            curl_easy_setopt(getCurl(), CURLOPT_FTP_CREATE_MISSING_DIRS, CURLFTP_CREATE_DIR_RETRY);
#else
#warning libcurl does not allow parallel creation of remote directories, please upgrade it to a version >= 7.20
            curl_easy_setopt(getCurl(), CURLOPT_FTP_CREATE_MISSING_DIRS, 1);

#endif
            curl_easy_setopt(getCurl(), CURLOPT_PROGRESSFUNCTION, FtpProgressData::progress_callback);
            curl_easy_setopt(getCurl(), CURLOPT_PROGRESSDATA, progressDataHandle.get());
            curl_easy_setopt(getCurl(), CURLOPT_NOPROGRESS, false);
            mode_t permissions = 0 ;
            res = performTransfer(false, "", remoteFilename, permissions, this);
            if (res != CURLE_OK) {
                _res = res;
            }
            _inStream->close();
            LOG_TRANSACTION(TrPut, remoteFilename, _res);
            logError(_res, remoteFilename);
        END_TRANSACTION
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "end transferring");
        _perfo.stop();
        updateStatistics(_res);
        deregisterProgressCallback();
    } 
    catch (std::exception const& ex) { // NOSONAR - any exception
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "transfer error");
        _inStream->close();
		ACS_THROW( CurlUnrecoverableException(ex, "transfer error") );
    }
    _perfo.stop();
    deregisterProgressCallback();
    CurlStatistics::logResult(_res, getSite(), operation);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Returning " << int(_res) << " code"); // PRQA S 3081
    return _res;	
}

CURLcode StreamCurl::streamputRename(const string &remoteFilename ,const string &finalRemoteFilename) {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "putting " << remoteFilename << " final: " << finalRemoteFilename);
    const string operation = "streamput" ; 
    _bytesRead = 0;
    CurlStatistics::log(getSite(), operation);
    boost::shared_ptr<FtpProgressData> progressDataHandle(new FtpProgressData(_stopController, band_prio_));
    progressDataHandle->setPerfMonitor(&_perfo) ;
    _perfo.setRemoteSite(getSite()) ;
    _perfo.setOperation(operation) ;
    _perfo.start();
    CURLcode res = _res;
    try {
        BEGIN_TRANSACTION(TrPut, true) // PRQA S 4236
            curl_easy_setopt(getCurl(), CURLOPT_READFUNCTION, _streamPutCallback);
#if ( (LIBCURL_VERSION_MAJOR > 7) || ( (LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR > 19)) )
            curl_easy_setopt(getCurl(), CURLOPT_FTP_CREATE_MISSING_DIRS, CURLFTP_CREATE_DIR_RETRY);
#else
#warning libcurl does not allow parallel creation of remote directories, please upgrade it to a version >= 7.20
            curl_easy_setopt(getCurl(), CURLOPT_FTP_CREATE_MISSING_DIRS, 1);

#endif
            curl_easy_setopt(getCurl(), CURLOPT_PROGRESSFUNCTION, FtpProgressData::progress_callback);
            curl_easy_setopt(getCurl(), CURLOPT_PROGRESSDATA, progressDataHandle.get());
            curl_easy_setopt(getCurl(), CURLOPT_NOPROGRESS, false);
            mode_t permissions = 0 ;
			
#ifdef ACS_TEST_CURL_TRANSFER_ERROR
#warning "ACS_TEST_CURL_TRANSFER_ERROR compilation flag is set"
			ACS_LOG_DEBUG(SimpleDebugSignature << "This is a TEST: Setting CURLcode to CURLE_FAILED_INIT") ;
			res = CURLE_FAILED_INIT;
#else
            res = performTransferRenameChmod("", remoteFilename, finalRemoteFilename, permissions, \
												getRemoveTargetBeforeTransfer(), this);
#endif
            if (res != CURLE_OK) {
                _res = res;
            }
            _inStream->close();
            LOG_TRANSACTION(TrPut, remoteFilename, _res);
            LOG_TRANSACTION(TrRename, finalRemoteFilename, res);
            logError(_res, remoteFilename);
        END_TRANSACTION
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "end transferring");
        _perfo.stop();
        updateStatistics(_res);
        deregisterProgressCallback();
    } 
    catch (std::exception const& ex) { // NOSONAR - any exception
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "transfer/rename error");
        _inStream->close();
		ACS_THROW( CurlUnrecoverableException(ex, "transfer/rename error") );
    }
    _perfo.stop();
    deregisterProgressCallback();
    CurlStatistics::logResult(_res, getSite(), operation);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Returning " << int(_res) << " code"); // PRQA S 3081
    return _res;
}

string StreamCurl::humanSize(long long bytes) 
{
    std::ostringstream os;
                                        // PRQA S 1021 L1
    long long giga = bytes / Constants::Gigabyte ;
    long long mega = (bytes % Constants::Gigabyte)/ Constants::Megabyte ;
    long long kb = (mega % Constants::Megabyte) / Constants::Kilobyte ;
                                        // PRQA L:L1
    if (0 != giga) {
        os << giga << " Gb ";
    }
    if ((0 != giga) || (0 != mega)) {
        os << mega << " Mb ";
    }
    os << kb << " Kb ";
    return os.str();
}

void StreamCurl::printProgress(bool isGet, long long bytes, long long totalBytes) {
    //a debug method useful to print the trasfer file progress.
    ACS_CLASS_BGN_DEBUG(ACS_MID_VERB);
    if (totalBytes % 1000 == 0) { // PRQA S 4400
        if (isGet) {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Stream Get :" << humanSize(bytes) << " total: " << humanSize(totalBytes));
        } 
        else {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Stream Put :" << humanSize(bytes) << " total: " << humanSize(totalBytes));
        }
    }
    ACS_CLASS_END_DEBUG;
}

size_t StreamCurl::streamGetCallback(void *buffer, size_t size, size_t nmemb, void *stream)  // NOSONAR - callback
{
    StreamCurl* thisObj = reinterpret_cast<StreamCurl*>(stream);  // NOSONAR - callback
    if (thisObj->_res != CURLE_OK) {
        return 0;
    }
    acs::io::OutputStream* outStream = thisObj->_outStream;

    const PipedStream* pipedStream = reinterpret_cast<PipedStream*>(thisObj->_outStream); // NOSONAR 
    if (pipedStream) {
        thisObj->_bytesWrote = pipedStream->bytesWrote();
        thisObj->_bytesRead = pipedStream->bytesRead();
    }

    if (!outStream) {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "getCallback: cannot read: null stream");
        return 0; 
    }

    ssize_t length = ssize_t ( size * nmemb ) ; // NOSONAR
    ssize_t w = outStream->write(reinterpret_cast<char*>( buffer), length);  // NOSONAR - callback
    while ((w < length) && (thisObj->_res == CURLE_OK)) {
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Get waiting....");
        Timer::delay(10); // PRQA S 4400
        w += outStream->write(reinterpret_cast<char*>( buffer ) + w, length - w);  // NOSONAR - callback
    }
    if (thisObj->_res != CURLE_OK) {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Error encountered in put callback: " << int(thisObj->_res )); // PRQA S 3081
    }
    thisObj->_bytesWrote = outStream->bytesWrote();
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "bytesWrote : " << int(thisObj->_bytesWrote )); // PRQA S 3081
    printProgress(true, size * nmemb, thisObj->_bytesWrote); // PRQA S 3000, 3084
    return size * nmemb; // PRQA S 3084
}

size_t StreamCurl::streamPutCallback(void *buffer, size_t size, size_t nmemb, void *stream)  // NOSONAR - callback
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Put Callback: " << size << " * " << nmemb << " bytes.");
    StreamCurl* thisObj = reinterpret_cast<StreamCurl*>(stream);  // NOSONAR - callback
    acs::io::InputStream* inStream = thisObj->_inStream;

    const PipedStream* pipedStream = reinterpret_cast<PipedStream*>(thisObj->_outStream); // NOSONAR 
    if (pipedStream) {
        thisObj->_bytesWrote = pipedStream->bytesWrote();
        thisObj->_bytesRead = pipedStream->bytesRead();
    }

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "read " << thisObj->_bytesRead << " wrote " << thisObj->_bytesWrote);

    if (!inStream) {
        ACS_LOG_ERROR("streamPutCallback: cannot write: null stream");
        return 0;
    }
    size_t read = 0;
    while (!inStream->closed() && (thisObj->_res == CURLE_OK)) {
        read = inStream->read(reinterpret_cast<char*>( buffer ), size * nmemb); // NOSONAR
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "read " << read);
        if (!read) { // if read nothing 
            if (inStream->closed()) {
                break ;
            }
            else {
                // give it a chance.
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Stream still not available...retry...");
                Thread::yield();
                Timer::delay(10); // 10 ms // PRQA S 4400
            }
        }
        else 
        {
            break;
        }
    }
    if (thisObj->_res != CURLE_OK) {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Error encountered in get callback: " << int(thisObj->_res )); // PRQA S 3081
    }
    printProgress(false, read, thisObj->_bytesRead); // PRQA S 3000
    if (0 == read) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No Bytes read!" << ( thisObj->_res == CURLE_OK ? " Finish transfer." : "" ) );
    }
    if (thisObj->_res != CURLE_OK) {
        return CURL_READFUNC_ABORT;
    }
    return read;
}

void StreamCurl::setOutStream(acs::io::OutputStream* outStream) noexcept  // PRQA S 4121
{
    _outStream = outStream;
}

void StreamCurl::setInStream(acs::io::InputStream* inStream) noexcept  // PRQA S 4121
{
    _inStream = inStream;
}

//
//Circulation Curl Thread
//

StreamCurlThread::StreamCurlThread(const StopController& stopController, CURLcode& code, const BandPriority &bp) :
    StreamCurl(stopController, code, bp), 
    Thread("StreamCurlThread-Local")
{
    //empty
}

StreamCurlThread::StreamCurlThread(RemoteSite const& site,
                                   const StopController& stopController,
                                   CURLcode& code,
                                   const BandPriority& bp) :
  StreamCurl(site, stopController, code, bp),
  Thread("StreamCurlThread-" + site.getUser() + "@" + site.getHostAndPort())
{
    //empty.
}

void StreamCurlThread::requestFutureOperation(
    const string &op, 
    acs::io::InputStream* in, 
    acs::io::OutputStream* out,
    const string &remoteFilename,
    const string &finalRemoteFilename) 
{
    _operationRequested = op;
    setInStream(in) ;
    setOutStream(out) ;
    _remoteFilename = remoteFilename;
    _finalRemoteFilename = finalRemoteFilename;
}

CURLcode StreamCurlThread::completeAndGetResult() {
	// Wait until the thread completes
    while (running()) {
        // wait on condition, repeat ... 
        waitExitOrTimeout(100) ; // PRQA S 4400
    }
    // Get the run-exception if any has caused the run to exit
    auto theException = runException();
    // In case it exists, re-throw it to allow any caller to detect the issues
    if (theException)
    {
		ACS_LOG_NOTIFY_EX (*theException);
		_result = CURLE_FAILED_INIT;
	}
    
    // Ok no exceptions, a valid _result can be returned
    return _result;
}

void StreamCurlThread::run() 
{
	if (StringUtils::equalsNoCase(_operationRequested, "streamget")) {
		_result = streamget(_remoteFilename);
	} 
	else if (StringUtils::equalsNoCase(_operationRequested, "streamput")) {
		if (_finalRemoteFilename.empty() || _finalRemoteFilename == _remoteFilename) {
			_result = streamput(_remoteFilename);
		}
		else {
			_result = streamputRename(_remoteFilename,_finalRemoteFilename);
		}
	} 
	else {
		_result = CURLE_FUNCTION_NOT_FOUND;
		ACS_THROW(exIllegalValueException("Operation \"" + _operationRequested + "\" not found")); // PRQA S 3081
	}
}

_ACS_END_NESTED_NAMESPACE
