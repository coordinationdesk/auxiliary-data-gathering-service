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
 *  $Prod: A.C.S. libFtpCurl  $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.11  2015/04/23 17:45:55  marpas
 *  when putting a file to a remote system, in case o renaming, the latter is performed in the same connection that actually transfered the file.
 *  the concept is still valid for stream transfers where the endpoint is a file.
 *  Even the final chmod is done accordingly (valid only for FtpCurl transfers, not fo CirculationCurl ones).
 *  It does not applies to directory because their children have to be transferred BEFORE the renaming.
 *
 *  Revision 5.10  2014/06/20 14:52:34  marpas
 *  useless private variables removed
 *  statistics management (log) fixed
 *  statistics management (t_circulationstats) fixed
 *  operation are now an enum
 *
 *  Revision 5.9  2014/02/21 16:41:36  lucio.pulvirenti
 *  No more derived by BandPriority: added to IF.
 *
 *  Revision 5.8  2014/02/14 13:54:32  marpas
 *  attempt to refactor band shaping mechanism
 *
 *  Revision 5.7  2013/10/28 13:42:14  marpas
 *  cleaning macros and curl versions.
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
 *  Revision 5.2  2013/07/16 14:51:26  marpas
 *  coding best practices applied
 *
 *  Revision 5.1  2013/06/18 12:53:56  marpas
 *  debug macros ratioanlized
 *
 *  Revision 5.0  2013/06/06 18:08:22  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.6  2013/04/05 09:44:15  marpas
 *  coding best practices applied
 *  qa warnings (partial)
 *  libDCUtility interface update
 *
 *  Revision 2.5  2012/11/29 16:28:21  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.4  2012/11/27 13:29:13  micmaz
 *  added quality tool directives.
 *
 *  Revision 2.3  2012/02/07 10:25:47  micmaz
 *  removed warnings .
 *
 *  Revision 2.2  2012/02/02 17:19:58  micmaz
 *  IN PROGRESS - issue S1PDGS-2769: The Circulation Curl does not take into account the slow parameter
 *  http://jira.acsys.it/browse/S1PDGS-2769
 *
 *  Revision 2.1  2012/01/20 16:11:07  micmaz
 *  OPEN - issue S1PDGS-2685: Circulation Agent Core in transferring large files
 *  http://jira.acsys.it/browse/S1PDGS-2685
 *
 *  Revision 2.0  2011/10/18 14:43:52  marpas
 *  new statistics adopted
 *
 *  Revision 1.8  2011/09/07 12:12:34  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *  RESOLVED - issue S1PDGS-1808: Improve the CirculationCurl bytes transfer
 *  http://jira.acsys.it/browse/S1PDGS-1808
 *
 *  Revision 1.7  2011/07/16 09:49:15  micmaz
 *  corrected the stream curl class: now it computes correctly the total amount of read and wrote bytes.
 *
 *  Revision 1.6  2011/04/29 17:20:42  micmaz
 *  check style changes
 *
 *  Revision 1.5  2011/04/12 18:02:26  micmaz
 *  work in progress
 *
 *  Revision 1.4  2011/04/11 12:25:06  micmaz
 *  work in progress
 *
 *  Revision 1.3  2011/04/08 15:37:40  micmaz
 *  work in progress
 *
 *  Revision 1.2  2011/04/05 17:15:09  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.1  2011/02/23 14:39:26  micmaz
 *  Added: Ftpes support (but not tested) and CirculationCurl in order to make a file-transfer using this server as bridge.
 *
 *
 *
 *
 */

#ifndef _StreamCurl_H_
#define _StreamCurl_H_

#include <acs_c++config.hpp>

#include <Curl.h>
#include <BandPriority.h>
#include <FtpPerformance.h>
#include <StopController.h>
#include <CurlStream.h>

#include <Thread.h>

#include<string>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) ;

class BandPriority ;

class StreamCurl: public Curl { // PRQA S 2109, 2153
public:
    StreamCurl(const StopController&, CURLcode&, const BandPriority &);
    StreamCurl(RemoteSite const&, const StopController&, CURLcode&, const BandPriority & );
    StreamCurl(StreamCurl const& ) = delete ;
    StreamCurl& operator=(StreamCurl const& ) = delete ;

    ~StreamCurl() override = default ;

    CURLcode streamput( const std::string &);
    CURLcode streamputRename( const std::string &remoteFilename, const std::string &finalRemoteFilename);
    CURLcode streamget( const std::string &);

    void setOutStream(acs::io::OutputStream* ) noexcept ;
    void setInStream(acs::io::InputStream* ) noexcept ;

	bool getRemoveTargetBeforeTransfer() const  		 { return _removeTargetBeforeTransfer ; }
	void setRemoveTargetBeforeTransfer(const bool b)	 { _removeTargetBeforeTransfer = b ; }

private:
    static void printProgress(bool isGet, long long bytes, long long totalBytes);

    //Default Callbacks
    static size_t streamGetCallback(void *buffer, size_t size, size_t nmemb, void *stream);
    static size_t streamPutCallback(void *buffer, size_t size, size_t nmemb, void *stream);

    static std::string humanSize(long long bytes) ;

private: // NOSONAR - for clarity 
    GetCallback _streamGetCallback = &streamGetCallback ;
    PutCallback _streamPutCallback = &streamPutCallback;
    long long  _bytesWrote = 0 ;
    long long  _bytesRead = 0 ;
    acs::io::InputStream* _inStream = nullptr ;
    acs::io::OutputStream* _outStream = nullptr ;
    FtpPerformance _perfo = {} ;
    const StopController & _stopController;
    CURLcode& _res;
    const BandPriority &band_prio_ ;
	bool _removeTargetBeforeTransfer = false ;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(StreamCurl) ;  

};

class StreamCurlThread: public StreamCurl, public Thread { // PRQA S 2109, 2153
public:
    StreamCurlThread(const StopController &, CURLcode&, const BandPriority & );
    StreamCurlThread(RemoteSite const&, const StopController &, CURLcode&, const BandPriority &);
    StreamCurlThread(StreamCurlThread const& ) = delete ;
    StreamCurlThread& operator=(StreamCurlThread const& ) = delete ;

    ~StreamCurlThread() override = default ;

    void requestFutureOperation(const std::string &op, 
                                acs::io::InputStream* in, 
                                acs::io::OutputStream* out, 
                                const std::string &remoteFilename,
                                const std::string &finalRemoteFilename="");
    CURLcode completeAndGetResult();

protected:
    void run() override ;

private:
    std::string _remoteFilename = "" ;
    std::string _finalRemoteFilename = "" ;
    std::string _operationRequested = "" ;
    CURLcode _result = {} ;
};

_ACS_END_NESTED_NAMESPACE

#endif /* _StreamCurl_H_ */
