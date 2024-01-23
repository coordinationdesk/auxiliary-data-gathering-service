// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2020, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
 *
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.48  2017/12/04 09:43:08  marpas
 *  getHttpResponseCode method added to retrun the last http response code got
 *
 *  Revision 5.47  2017/11/23 09:54:10  marpas
 *  custom headers are now stored as strings and not pairs
 *  this allow to offer a method to set a custom header externally built
 *
 *  Revision 5.46  2017/11/22 14:02:41  marpas
 *  custom headers concept added (will be used when in transaction)
 *  httppost method added (allows user authentication before the host)
 *  custom headers can be added calling addCustomHeader
 *
 *  Revision 5.45  2017/11/22 11:38:52  marpas
 *  post method now accepts "username:password" as last argument (defaulted to empty string)
 *
 *  Revision 5.44  2017/10/19 15:14:20  marpas
 *  qa warning removed
 *  code best practice implemented
 *
 *  Revision 5.43  2017/09/25 14:11:48  marpas
 *  qa warning silenced
 *
 *  Revision 5.42  2017/07/26 07:48:19  lucio.pulvirenti
 *  S1PDGS-31936: setConnectionTimeout_ms: added optional bool parameter (default true) to adjust connection timeout.
 *
 *  Revision 5.41  2017/05/17 14:05:43  marpas
 *  httpget has a default parameter to set "user:pwd" during http request - NB THIS DO NOT AFFECT INTERNAL CLASS MEMBER
 *
 *  Revision 5.40  2017/05/05 16:14:06  lucio.pulvirenti
 *  S3PDGS-3137: CURLOPT_FTP_RESPONSE_TIMEOUT set by a dedicated timeout parameter got by configuration.
 *
 *  Revision 5.39  2017/04/06 15:35:52  marfav
 *  S2PDGS-1748
 *  Work In Progress
 *
 *  Revision 5.38  2017/02/14 16:15:57  marpas
 *  Curl::post and Curl::httpget signature is more robust now
 *
 *  Revision 5.37  2016/10/21 11:18:26  marpas
 *  boost include no longer needed
 *
 *  Revision 5.36  2016/09/26 16:17:25  francesco.avanzi
 *  Now curl::performTransferRenameChmod does a runtime version check to decide
 *  whether to use *-prefix or not on the quote remove commands
 *
 *  Revision 5.35  2016/05/09 14:27:25  francesco.avanzi
 *  performTransfer() and performTransferRenameChmod() no longer allow default parameters.
 *  this is to avoid unwanted casts between void * and bool
 *
 *  Revision 5.34  2016/05/05 13:10:26  marpas
 *  warning removed and doc updated
 *
 *  Revision 5.33  2016/04/21 13:24:42  francesco.avanzi
 *  S2PDGS-1431 Probing is now done using the new probeLs() function
 *  such functions stops the LS (therefore the data transfert) the very moment
 *  it finds out whether the site is passive or active.
 *
 *  Revision 5.32  2016/03/21 18:25:54  francesco.avanzi
 *  WORKINPROGRESS s2pdgs-1434
 *
 *  Revision 5.31  2016/03/18 13:17:12  francesco.avanzi
 *   S2PDGS-1434  Curl::put  modified:a bool is now taken in account to regulate target cleanup
 *
 *  Revision 5.30  2016/03/15 11:18:44  marfav
 *  Extending support to older libCurl version with specific defines
 *
 *  Revision 5.29  2015/04/23 17:45:55  marpas
 *  when putting a file to a remote system, in case o renaming, the latter is performed in the same connection that actually transfered the file.
 *  the concept is still valid for stream transfers where the endpoint is a file.
 *  Even the final chmod is done accordingly (valid only for FtpCurl transfers, not fo CirculationCurl ones).
 *  It does not applies to directory because their children have to be transferred BEFORE the renaming.
 *
 *  Revision 5.28  2015/04/16 13:22:51  marpas
 *  new putRename method implementation in progress
 *
 *  Revision 5.27  2015/04/09 15:55:38  marfav
 *  S1PDGS-31623 adding HTTP GET operation (on string)
 *
 *  Revision 5.26  2015/03/10 14:40:26  lucio.pulvirenti
 *  S2PDGS-984: CurlUnrecoverableException defined. probe IF changed.
 *
 *  Revision 5.25  2015/03/09 17:00:20  lucio.pulvirenti
 *  S2PDGS-984: work in progress.
 *
 *  Revision 5.24  2014/12/09 11:40:45  francesco.avanzi
 *  modified to use new ContentType
 *
 *  Revision 5.23  2014/11/03 17:08:33  marpas
 *  setting http response > 400 as error
 *
 *  Revision 5.22  2014/11/03 17:03:11  marpas
 *  simplified, init condition shall not be verified from external
 *
 *  Revision 5.21  2014/10/29 13:26:23  lucio.pulvirenti
 *  S2PDGS-955: setProxy(string) method removed. New setProxy method implemented setting proxy options if a proxy is configured for the current host.
 *
 *  Revision 5.20  2014/10/28 16:31:43  lucio.pulvirenti
 *  S2PDGS-955 work in progress.
 *
 *  Revision 5.19  2014/07/03 07:43:04  marpas
 *  useless method removed
 *
 *  Revision 5.18  2014/06/20 14:52:33  marpas
 *  useless private variables removed
 *  statistics management (log) fixed
 *  statistics management (t_circulationstats) fixed
 *  operation are now an enum
 *
 *  Revision 5.17  2014/06/05 10:50:50  marfav
 *  Replacing scoped_ptr with auto_ptr
 *
 *  Revision 5.16  2014/06/04 17:29:20  marfav
 *  QA warning removed
 *  HTTP sessions managed without SSL layer
 *  CURL locking callback functions strategy revised
 *
 *  Revision 5.15  2014/05/06 17:28:11  marfav
 *  Using ACS_SSL for ssl global initialization
 *
 *  Revision 5.14  2014/03/12 11:10:53  francesco.avanzi
 *  minor issue taken in account
 *
 *  Revision 5.13  2014/02/26 10:36:50  lucio.pulvirenti
 *  Timeout values now correctly managed.
 *  Band shaping effectively used in ftp get and put methods only (no used in rename, ls, etc.).
 *
 *  Revision 5.12  2014/02/21 16:40:19  lucio.pulvirenti
 *  Progress callback no more used: to remove next version,
 *
 *  Revision 5.11  2013/12/17 18:09:22  marpas
 *  minor changes
 *
 *  Revision 5.10  2013/12/10 13:44:26  lucio.pulvirenti
 *  MP: Work in progress.
 *
 *  Revision 5.9  2013/12/09 14:25:14  marpas
 *  triggering OPENSSL_VERSION_NUMBER is 0x010000000L for new OpenSSL thread safety.
 *  Before was conservatively set to 0x01000000fL
 *
 *  Revision 5.8  2013/11/21 14:44:45  marpas
 *  comments added
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
 *  Revision 5.5  2013/10/24 18:27:59  marpas
 *  work in progress
 *
 *  Revision 5.4  2013/10/23 17:28:11  marpas
 *  work in progress
 *
 *  Revision 5.3  2013/10/22 17:31:50  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/06/18 12:54:14  marpas
 *  debug macros rationalized
 *
 *  Revision 5.1  2013/06/14 10:54:15  marpas
 *  debug levels and messages improved
 *
 *  Revision 5.0  2013/06/06 18:08:19  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.12  2013/05/29 10:53:58  marpas
 *  interface rationalization,
 *  coding best practices applied,
 *  qa rules
 *
 *  Revision 2.11  2013/01/18 17:04:25  micmaz
 *  remove useless printouts
 *
 *  Revision 2.10  2013/01/15 11:22:52  micmaz
 *  work in progress
 *
 *  Revision 2.9  2012/12/20 16:12:47  lucio.pulvirenti
 *  openssl version management for thread id callbacks
 *
 *  Revision 2.8  2012/11/29 16:28:21  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.7  2012/11/28 18:16:23  marpas
 *  qa rules,
 *  optimization and robustness
 *  still work in progress
 *
 *  Revision 2.6  2012/11/27 13:28:11  micmaz
 *  corrected gcrypto call backs
 *
 *  Revision 2.5  2012/05/30 17:36:28  micmaz
 *  added local and remote chmod
 *
 *  Revision 2.4  2012/03/27 18:08:49  micmaz
 *  work in progress
 *
 *  Revision 2.3  2012/02/07 10:25:46  micmaz
 *  removed warnings .
 *
 *  Revision 2.2  2012/01/31 12:25:02  micmaz
 *  added a new delay method.
 *
 *  Revision 2.1  2012/01/20 16:04:59  micmaz
 *  OPEN - issue S1PDGS-2685: Circulation Agent Core in transferring large files
 *  http://jira.acsys.it/browse/S1PDGS-2685
 *
 *  Revision 2.0  2011/10/18 14:43:50  marpas
 *  new statistics adopted
 *
 *  Revision 1.24  2011/09/29 18:33:04  marpas
 *  fixing codecheck crashes
 *
 *  Revision 1.23  2011/08/31 14:08:40  micmaz
 *  IN PROGRESS - issue S1PDGS-1785: ssl thread safety
 *  http://jira.acsys.it/browse/S1PDGS-1785
 *
 *  Revision 1.22  2011/08/26 15:52:05  micmaz
 *  IN PROGRESS - issue S1PDGS-1604: CirculationAgent does not use t_circulationtempnames
 *  http://jira.acsys.it/browse/S1PDGS-1604
 *  IN PROGRESS - issue S1PDGS-1808: Improve the CirculationCurl bytes transfer
 *  http://jira.acsys.it/browse/S1PDGS-1808
 *
 *  Revision 1.21  2011/08/12 14:49:04  micmaz
 *  work in progress..
 *
 *  Revision 1.20  2011/05/11 13:52:46  micmaz
 *  corrected the rollback for put and get operations
 *
 *  Revision 1.19  2011/05/10 17:10:57  micmaz
 *  corrected bug of ProgressData callback: destroyed while working.
 *
 *  Revision 1.18  2011/04/29 17:20:41  micmaz
 *  check style changes
 *
 *  Revision 1.17  2011/04/01 14:43:55  micmaz
 *  implemented the Circulations WP
 *
 *  Revision 1.16  2011/03/23 10:04:03  micmaz
 *  work in progress...
 *
 *  Revision 1.15  2011/03/15 10:20:02  micmaz
 *  work in progress
 *
 *  Revision 1.14  2011/03/09 11:37:27  micmaz
 *  moved curle errors definition to the header file.
 *
 *  Revision 1.13  2011/03/08 10:44:15  marpas
 *  interface changed
 *
 *  Revision 1.12  2011/03/01 17:32:12  micmaz
 *  removed boost::thread for  added acs::Thread
 *
 *  Revision 1.11  2011/02/23 14:39:26  micmaz
 *  Added: Ftpes support (but not tested) and CirculationCurl in order to make a file-transfer using this server as bridge.
 *
 *  Revision 1.10  2011/02/10 15:53:32  micmaz
 *  added statistics.
 *
 *  Revision 1.9  2011/02/04 11:31:46  micmaz
 *  work in progress...
 *
 *  Revision 1.8  2011/02/02 17:33:32  micmaz
 *  work in  progress...
 *
 *  Revision 1.7  2011/02/01 15:12:14  micmaz
 *  work in  progress...
 *
 *  Revision 1.6  2011/01/28 18:55:55  micmaz
 *  work in  progress...
 *
 *  Revision 1.5  2011/01/27 16:13:28  micmaz
 *  adapted the library to old libcURL 1.15
 *
 *  Revision 1.4  2011/01/26 18:39:25  micmaz
 *  work in  progress...
 *
 *  Revision 1.3  2011/01/26 16:39:24  micmaz
 *  work in  progress...
 *
 *  Revision 1.2  2011/01/25 19:35:42  micmaz
 *  work in  progress...
 *
 *  Revision 1.1  2011/01/19 16:42:59  micmaz
 *  added the openConnection() and closeConnection() to cURL operations.
 *
 *
 *
 */

#ifndef _Curl_H_
#define _Curl_H_
#include <acs_c++config.hpp>
#include <CurlTransaction.h>
#include <CirculationStatistics.h>
#include <RemoteSite.h>
#include <CurlHeaderMaker.h>
#include <CurlFile.h>

#include <Statistics.h>
#include <StringUtils.h>

#include <exException.h>


#include <curl/curl.h> // PRQA S 1013
#include <boost/shared_ptr.hpp> // PRQA S 1013
#include <string>

// PRQA S 1020, 1023 L1
  #define BEGIN_TRANSACTION(operationCode, withCallback_)     \
      setOperationCode(operationCode) ;\
      CurlTransaction* t = getTransaction() ;\
      ACS_SMARTPTR <CurlTransaction> handle;\
	  ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "BGN_TRN withCallback_=" << boolalpha << (withCallback_));\
      if (t == 0) { \
          ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Opening a one-shot transaction");\
          t = new CurlTransaction(*this); \
          handle.reset(t);\
          setCurl(t->begin()) ; \
      } \
      int attempts=0; \
      ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "BGN_TRN Starting transaction loop");\
      while (attempts < 2) { \
      try { \
          t->prepareTransaction(operationCode); \
          prepareCustomOptions(withCallback_);


#define LOG_TRANSACTION(operationCode,remoteFile,returnCode) \
        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "LOG_TRN");\
        t->logTransaction(operationCode,remoteFile,returnCode);

#define LOG_RENAME_TRANSACTION(operationCode,remoteNewFilename,remoteOldFilename, returnCode) \
        {\
            ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "LOG_RN_TRN");\
            std::vector<std::string> args; \
            args.push_back(remoteNewFilename); \
            args.push_back(remoteOldFilename);\
            t->logTransaction(operationCode,args,returnCode);\
        }

#define END_TRANSACTION \
        break;\
        }catch(std::exception const& ex) { \
            ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "END_TRN Exception thrown in cURL transaction: " << ex.what());\
            if ( ++attempts < 2) { \
                if ( res == CURLE_COULDNT_CONNECT) { \
                    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "END_TRN Warning: Could not connect to remote server: try again."); \
                    t->reconnect();\
                    continue;\
                }\
            }\
            attempts = 2;\
            if ( getTransaction() == 0 ) { \
                t->rollback(); \
            } \
            else { \
                ACS_LOG_WARNING("END_TRN Warning: cURL Exception got. cURL connection needs rollback"); \
            } \
            throw; \
        }  \
    } /* end while */ \
    if (getTransaction() == 0 ) { \
        t->close(); \
    }\
    clearSlist();

// PRQA L:L1


#if ( (LIBCURL_VERSION_MAJOR < 7) || ( (LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR <= 15))  )
 #ifndef CURLE_REMOTE_FILE_NOT_FOUND
 #define CURLE_REMOTE_FILE_NOT_FOUND 78
 #endif
 #ifndef CURLE_SSH 
 #define CURLE_SSH 79
 #endif
 #ifndef CURLE_SSL_SHUTDOWN_FAILED
 #define CURLE_SSL_SHUTDOWN_FAILED 80
 #endif
 #if not defined(CURLE_REMOTE_DISK_FULL)
 #define CURLE_REMOTE_DISK_FULL CURLE_TFTP_DISKFULL
 #endif
 #if not defined(CURLE_QUOTE_ERROR)
 #define CURLE_QUOTE_ERROR CURLE_FTP_QUOTE_ERROR
 #endif
#endif

#if ( (LIBCURL_VERSION_MAJOR < 7) || ( (LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR < 24))  )
#ifndef CURLE_FTP_ACCEPT_TIMEOUT
#define CURLE_FTP_ACCEPT_TIMEOUT 12
#endif
#endif

#if ( (LIBCURL_VERSION_MAJOR < 7) || ( (LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR < 18)) || ( (LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR == 18) && (LIBCURL_PATCH < 2)) )
#ifndef CURLE_AGAIN
#define CURLE_AGAIN 81
#endif
#endif



_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) 
    
/**
 * Implementation of the Curl FTP/SFTP protocol.
 * This class implements 5 basic functions against a remote site:<P>
 * <ul>
 * <li>get: retrieves a file from the remote site;
 * <li>put: puts a file to the remote site;
 * <li>ls: lists the content of a remote directory;
 * <li>del: deletes a remote file;
 * <li>rename: renames a remote file.
 * </ul>
 * <p>
 * Every operation can be made using a unique connection to the remote site
 * calling openConnection() before the first command and closeConnection() after
 * the last command.
 * <p>
 * Internal callbacks are used to make cURL working.
 * @see acs::curl::CurlTransaction : the class responsible to use a unique transaction for different commands;
 * @see acs::curl::CurlHeaderMaker: the class responsible to make cURL working with its internal headers.
 */
class Curl { // PRQA S 2109
public:
    exDECLARE_EXCEPTION(CurlException, exException) ; // PRQA S 2131, 2502
    exDECLARE_EXCEPTION(CurlUnrecoverableException, CurlException) ; // PRQA S 2131, 2153, 2502
    exDECLARE_EXCEPTION(CurlCodingException, CurlException) ; // PRQA S 2131, 2153, 2502
	
	/**
	 * This class is used to wrap curl opt for post operations
	 */
	class CurlHttpOpt {
	public:
		/** Ctors/Dtors */
    CurlHttpOpt() :
      _uri(),
      _message(),
      _fileToUpload(),
      _contenttype(),
      _user(),
      _passwd(),
      _userpwd(),
      _secure(false),
      _strictAuth(true),
      _connectionTimeout_ms(),
      _connectionTimeout_s(),
      _responseTimeout_ms(),
      _responseTimeout_s(),
      _uriSet(false),
      _messageSet(false),
      _fileToUploadSet(false),
      _contenttypeSet(false),
      _userSet(false),
      _passwdSet(false),
      _userpwdSet(false),
      _timeoutSet(false)
    {
    }

    CurlHttpOpt(const CurlHttpOpt& opt) :
      _uri(opt._uri),
      _message(opt._message),
      _fileToUpload(opt._fileToUpload),
      _contenttype(opt._contenttype),
      _user(opt._user),
      _passwd(opt._passwd),
      _userpwd(opt._userpwd),
      _secure(opt._secure),
      _strictAuth(opt._strictAuth),
      _connectionTimeout_ms(opt._connectionTimeout_ms),
      _connectionTimeout_s(opt._connectionTimeout_s),
      _responseTimeout_ms(opt._responseTimeout_ms),
      _responseTimeout_s(opt._responseTimeout_s),
      _uriSet(opt._uriSet),
      _messageSet(opt._messageSet),
      _fileToUploadSet(opt._fileToUploadSet),
      _contenttypeSet(opt._contenttypeSet),
      _userSet(opt._userSet),
      _passwdSet(opt._passwdSet),
      _userpwdSet(opt._userpwdSet),
      _timeoutSet(opt._timeoutSet)
    { }
		
		virtual ~CurlHttpOpt() ACS_NOEXCEPT { }
		
		/** Operators overload */
		CurlHttpOpt& operator=(const CurlHttpOpt& opt) {
			if (this != &opt) {
				_uri = opt._uri;
				_message = opt._message;
				_fileToUpload = opt._fileToUpload;
				_contenttype = opt._contenttype;
				_user = opt._user;
				_passwd = opt._passwd;
				_userpwd = opt._userpwd;
				_secure = opt._secure;
				_strictAuth = opt._strictAuth;
        _connectionTimeout_ms = opt._connectionTimeout_ms;
        _connectionTimeout_s = opt._connectionTimeout_s;
        _responseTimeout_ms = opt._responseTimeout_ms;
        _responseTimeout_s = opt._responseTimeout_s;
        _uriSet = opt._uriSet;
				_messageSet = opt._messageSet;
				_fileToUploadSet = opt._fileToUploadSet;
				_contenttypeSet = opt._contenttypeSet;
				_userSet = opt._userSet;
				_passwdSet = opt._passwdSet;
				_userpwdSet = opt._userpwdSet;
        _timeoutSet = opt._timeoutSet;
      }
			return *this;
		}
		
		/** Setters */
		inline void setUri(const std::string& val) { _uri = val; _uriSet = true; } 
		inline void setMessage(const std::string& val) { _message = val; _messageSet = true; }
		inline void setFileToUpload(const std::string& file) { _fileToUpload = file; _fileToUploadSet = true; }
		inline void setContentType(const std::string& val) { _contenttype = val; _contenttypeSet = true; }
		inline void setUser(const std::string& val) { _user = val; _userSet = true; }
		inline void setPassword(const std::string& val) { _passwd = val; _passwdSet = true; }
		inline void setUserPassword(const std::string& val) { _userpwd = val; _userpwdSet = true; }
		inline void setSecureConnection(bool val) { _secure = val; }
		inline void setStrictAuthentication(bool val) { _strictAuth = val; }
    inline void setTimeouts(size_t ms, bool adjustResponseTimeout = true)
    {
      computeTimeouts(ms, adjustResponseTimeout, _connectionTimeout_ms, _connectionTimeout_s, _responseTimeout_ms,
                      _responseTimeout_s);
      _timeoutSet = true;
    }

    /** Getters */
		inline const std::string& getUri(bool* set = 0) const {
			if (0 != set) {
				*set = _uriSet;
			}
			return _uri;
		}
		
		inline const std::string& getMessage(bool* set = 0) const {
			if (0 != set) {
				*set = _messageSet;
			}
			return _message;
		}
		
		inline const std::string& getFileToUpload(bool* set = 0) const {
			if (0 != set) {
				*set = _fileToUploadSet;
			}
			return _fileToUpload;
		}
		
		inline const std::string& getContentType(bool* set = 0) const {
			if (0 != set) {
				*set = _contenttypeSet;
			}
			return _contenttype;
		}
		
		inline const std::string& getUser(bool* set = 0) const {
			if (0 != set) {
				*set = _userSet;
			}
			return _user;
		}
		
		inline const std::string& getPassword(bool* set = 0) const {
			if (0 != set) {
				*set = _passwdSet;
			}
			return _passwd;
		}
		
		inline const std::string& getUserPassword(bool* set = 0) const {
			if (0 != set) {
				*set = _userpwdSet;
			}
			return _userpwd;
		}
		
		inline bool getSecureConnection() const { return _secure; }
		inline bool getStrictAuthentication() const { return _strictAuth; }

    inline void getTimeouts(size_t& connectionTimeout_ms, size_t& connectionTimeout_s, size_t& responseTimeout_ms,
                            size_t& responseTimeout_s, bool* set = 0) const
    {
      if (0 != set)
      {
        *set = _timeoutSet;
      }
      connectionTimeout_ms = _connectionTimeout_ms;
      connectionTimeout_s = _connectionTimeout_s;
      responseTimeout_ms = _responseTimeout_ms;
      responseTimeout_s = _responseTimeout_s;
    }

  private:
		std::string _uri;			// The uri to be used for post
		std::string _message;		// message
		std::string _fileToUpload;	// File to upload. Used only by http PUT operations
		std::string _contenttype;	// Content type
		std::string _user;			// User
		std::string _passwd;		// Password
		std::string _userpwd;		// User:Password => noencoding
		bool _secure;				// If true, secure authentication will be used
		bool _strictAuth;			// If true, strict authentication is used. (Used only if _secure is true)
    size_t _connectionTimeout_ms;
    size_t _connectionTimeout_s;
    size_t _responseTimeout_ms;
    size_t _responseTimeout_s;

    bool _uriSet;
		bool _messageSet;
		bool _fileToUploadSet;
		bool _contenttypeSet;
		bool _userSet;
		bool _passwdSet;
		bool _userpwdSet;
    bool _timeoutSet;
  };

    typedef int (*ProgressCallback)(void *curl, double downloadTotal, double downloadNow, double uploadTotal, double uploadNow); // PRQA S 2502
    typedef int (*DebugCallback)(void *curl, int infotype, char *dbg, size_t size, void *data);
    typedef size_t (*ListCallback)(void *buffer, size_t size, size_t nmemb, void *stream);
    typedef size_t (*GetCallback)(void *buffer, size_t size, size_t nmemb, void *stream);
    typedef size_t (*PutCallback)(void *buffer, size_t size, size_t nmemb, void *stream);

    Curl();
    explicit Curl(const RemoteSite &site);
    virtual ~Curl() throw() ;
public:

    /**
     *Opens a new connection to cURL;
     */
    CURL* openConnection();
    /**
     *Close the connection (and commit) from cURL;
     */
    void closeConnection();
    /**
     *Close the connection with rollback;
     */
    void rollback();
    /**
     *Tests if there is a user-defined transaction opened.
     */
    bool isOpenedTransaction() const;
    /**
     * Puts a file to the remote site .
     */
    CURLcode put(const std::string &localFile, const std::string &remoteFile, mode_t permissions=0);
    /**
     * Puts a file to the remote site also handing a callback in the generic form: data_header / function pointer
     */
    CURLcode putWithCallback(const std::string &localFile, const std::string &remoteFile, mode_t permissions, void* data_pointer, PutCallback);

    /**
     * Puts a file to the remote site with the temporary name remoteFile and then rename it to finalRemoteFName .
     */
    CURLcode putRename(const std::string &localFile, 
                       const std::string &remoteFile, 
                       const std::string &finalRemoteFName, 
                       bool renameAlso,
                       mode_t permissions = 0 );
    /**
     * Puts a file to the remote site taking into account the temporary name. Also handing a callback in the generic form: data_header / function pointer
     */
    CURLcode putRenameWithCallback(	const std::string &localFile,
									const std::string &remoteFile, 
									const std::string &finalRemoteFName, 
									const bool renameAlso,
									const mode_t& permissions,
									void* data_pointer,
									PutCallback) ;

    /**
     * Gets a file from the remote site .
     * If the remoteFile is a directory, then in the localFile will be stored the "ls -la" output.
     */
    CURLcode get(const std::string &localFile, const std::string &remoteFile);
    /**
     * Gets a file from the remote site, taking into account the callback (the input stream is encapsulated in a CurlFile container)
     */
    CURLcode getWithCallback(const std::string &localFile, const std::string &remoteFile, CurlFile& targetFile, GetCallback get_func_pointer);
    /**
     * list a remote directory.
     * @param remoteDir : the remote directory to inspect.
     * @param output: the buffer where to put the "ls -la" output.
     */
    CURLcode ls(std::string const &, void* output = 0);
     /**
     * Tries to list a remote resource directory. 
     * If allowed, this triggers a transfer and can be used to check if the remote end is in passive or active mode depending how the remote site has been set-up (e.g. using Curl::setSite).
     * Because the listing can be very long in case the remote path would contain a very large number of files, the method is interrupted with an error as soon as the real transfer is on going - meaning that the site exists and we can trasfer data to/from it.
     * \see Curl::probe
     * @param dir : the remote directory to inspect.
     * @returns: CURLE_WRITE_ERROR if it was successful.
     */
    CURLcode probeLs(std::string const &dir);

     /**
     * Verifies if a file or a directory exists by getting the first byte of the file or of the directory listing
     * If allowed, this triggers a transfer and can be used to check if the remote end is in passive or active mode depending how the remote site has been set-up (e.g. using Curl::setSite).
     * @param isFile  : true if the element is a file or false in case of a directory
     * @param element : the remote element to probe
     * @returns: CURLE_OK if the file or the directory exists
     */
	CURLcode probeElement (bool isFile, std::string const& element);

    /**
     * Deletes a remote file.
     */
    CURLcode del(const std::string &remoteFile);
    /**
     * Deletes a remote directory (if empty).
     */
    CURLcode delDir(const std::string &remoteDir);
    /**
     * Makes an empty remote directory.
     */
    CURLcode mkDir(std::string const &remoteDir);
    /**
     * Change the remote file/directory permissions.
     */
    CURLcode chmod(const std::string &remoteFilename, mode_t permissions) ; // PRQA S 2502
    /**
     * Change the local file/directory permissions.
     */
    CURLcode lchmod(const std::string &localFilename, mode_t permissions) ; // PRQA S 2502
    /**
     * Change the local file/directory ownership.
     */
    CURLcode lchown(const std::string &localFilename, uid_t owner, gid_t group) ; // PRQA S 2502
    /**
     * Rename a remote file.
     */
    CURLcode rename(const std::string &remoteFilename, const std::string &newRemoteFilename);  // PRQA S 2502
    /**
     * Rename a local file.
     */
    CURLcode lrename(const std::string &oldFilename, const std::string &newFilename);
    /**
     * delete a Local file (local delete).
     */
    void ldel(const std::string &localFile);
    
    CURLcode post(const CurlHttpOpt& opt, std::string& output);
    CURLcode post(std::string const& uri, std::string const& data, std::string & output, const std::string& contentType=""); // PRQA S 4412
    CURLcode httppost(std::string const& uri, std::string const& data, std::string & output, const std::string& contentType="", const std::string &userpwd=""); // PRQA S 4412
    CURLcode httpget (const CurlHttpOpt& opt, std::string &output) ; // PRQA S 3081, 4412
    CURLcode httpget (std::string const& uri, std::string &output, const std::string &userpwd="") ; // PRQA S 3081, 4412
    CURLcode httpput(const CurlHttpOpt& opt, std::string& output);
    CURLcode httppatch(const CurlHttpOpt& opt, std::string& output);
    CURLcode httpdel(const CurlHttpOpt& opt, std::string& output);
    
    /**
    * String encoding
    * @param  inString : input string to encode
    * @returns: encoded string
    */
    static std::string encodeRFC398(const std::string & inString);
    /**
    * String decoding
    * @param  inString : input string to decode
    * @returns: decoded string
    */
    static std::string decodeRFC398(const std::string & inString);
     /**
     * Use WebDAV http(s) PROPFIND method to retrieve properties defined on given path of given host (see http://www.webdav.org/specs/rfc4918.html). _site attribute must be set properly in advance.
     * @param path  : the path to give properties of
     * @param depth : the depth level of requesting properties. Allowed values are 0: just the input path; 1: the contained entries if directory (i.e. collection); -1: infinity
     * @param output : the command output string
     * @param requestBody : the request body in xml format containing the properties to be returned. An empty body is treated as allprop request.
     * @param userpwd : the user and password formatted as "user:password"
     * @returns: CURLE_OK if executed successfully
     */
    CURLcode httpPropFind (std::string const& path, int depth, std::string &output, const std::string & requestBody=std::string(), const std::string &userpwd=std::string() ) ; // PRQA S 3081, 4412

    /**
     * Retrieves the cURL handle.
     */
    CURL* getCurlHandle();

    void deregisterProgressCallback();

    /**
     * Probes a remote site. Uses internally Curl::probeLs to optimize probe time.
     * \param: remotePath - the remote path to check for contents
     * \param: emitExc - if true this method throws an exception in case of failure
     * \param: curlCode - filled with the curl error code in case of failure
     * \returns true if the remote site was successfully probed
     */
    bool probe(std::string const &remotePath, bool emitExc = false, CURLcode* curlCode = 0);

    std::string str() const;

    struct Statistics { // PRQA S 2171, 2173, 2175, 2502
        // PRQA S 2100 L2
        double speed_download ; // bytes / sec   
        double speed_upload ;   // bytes / sec   
        double total_time ; // [s] - sum of the time values below
        double nlt;  // name lookup time [s]
        double ct;   // connection time [s]
        double act ; // app connection time [s]
        double ptt ; // pre transfer time [s]
        double stt ; // start stransfer time [s]
        double rt ;  // redirect time [s]
        double sizeUpload ; // bytes
        double sizeDownload; // bytes
        Statistics():
            speed_download(), 
            speed_upload(), 
            total_time(), 
            nlt(), 
            ct(), 
            act(), 
            ptt(), 
            stt(), 
            rt(), 
            sizeUpload(), 
            sizeDownload()
        {
            //empty.
        }
    private:
        Statistics(const Statistics &) ;             // not implemented
        Statistics & operator= (const Statistics &) ; // not implemented
        
        // PRQA L:L2
    };

public:
    //Getters and Setters
    void* getProgressCallbackData() const;

    void setSite(RemoteSite const& site);
    const RemoteSite &getSite() const throw() ;

    void setHttpContentType(const std::string &cT ) { httpContentType_ = cT; }
    const std::string &getHttpContentType( ) const throw(){ return httpContentType_;}
    void clearSlist();

    void setTransaction(CurlTransaction* transaction) throw() ;
    void setCreateMissingDirs(bool createMissingDirs) throw() ;
    bool getCreateMissingDirs() const throw() ;
    bool setProbeToken();
    size_t getConnectionTimeout_ms() const throw() ;
    /*- returns old value */
    size_t setConnectionTimeout_ms(size_t ms, bool adjustResponseTimeout = true) throw() ;
    static size_t computeTimeouts(const size_t ms, const bool adjustResponseTimeout, size_t& connectionTimeout_ms,
                                  size_t& connectionTimeout_s, size_t& response_timeout_ms,
                                  size_t& response_timeout_s) throw();
    const std::string &getLastErrorMessage() const throw() ;

protected:
    /**
     * Makes file transfer.
     * @param isGet: true if the operation to make is get, false if it is a put.
     * @param fileToTransfer: the file name to transfer;
     * @param targetFilename: the target file name (may be different from the source one);
     * @param readData: the pointer to the data to read (if null the Curl will read the fileToTransfer file on local disk).
     * @return the CURLCode, result of the operation;
     */
    CURLcode performTransfer(bool get, 
                             const std::string &fileToTransfer, 
                             const std::string &targetFilename, 
                             mode_t permissions, 
                             void* readData);
    /**
     * Makes file transfer and then rename it to finalTargetFName .
     * @param fileToTransfer: the file name to transfer;
     * @param targetFilename: the target file name (may be different from the source one);
     * @param finalTargetFName: the final Remote file Name.
     * @return the CURLCode, result of the operation;
     */
    CURLcode performTransferRenameChmod(const std::string &fileToTransfer, 
                                        const std::string &targetFilename, 
                                        const std::string &finalTargetFName,
                                        mode_t permissions,
                                        bool removeAlso,
                                        void* readData);
    /**
     * Logs the error (if any) of the operation.
     * @param res : it is the return code of the cURL operation;
     * @param fileToTransfer: it is the file name used to make ftp/sftp transfer. This name is only used to debug purpose.
     */
    void logError(CURLcode res, std::string const& fileToTransfer) const;
    /**
     * Prepare the cURL connection with custom values.
     * This method will be called before every operation, even if the connection was already
     * opened and initialized.
     */
    void prepareCustomOptions(bool use_progress_callback);
public:
    // Setter for custom callbacks
    /**
     * Setter for progress callback.
     * @param progressCallback: the new static function used to log the transfer progress.
     * @param progressCallbackData: the data used (a custom object) to work with the new progress callback.
     */
    void setProgressCallback(ProgressCallback progressCallback, void* progressCallbackData = 0);
    /**
     * Setter for list callback.
     * @param listCallback: the new static function used when making remote list
     * @param listCallbackData: the data used (a custom object) to work with the new list callback.
     */
    void setListCallback(ListCallback listCallback, void* listCallbackData = 0);
    /**
     * Reset the default list callback;
     */
    void setDefaultListCallback();

    void fillInCirculationStatistics(bool source, CirculationStatistics* statistics);
    static size_t dontWriteToConsole(void *buffer, size_t size, size_t nmemb, void *userp);
    void addCustomHeader(const std::string &name, const std::string &value) ; // compose name: value
    void addCustomHeader(const std::string &) ;
    void resetCustomHeaders() { _custhdrs.clear() ; }
    /*! returns the last code got from http calls \see httpget, httppost, post
    */
    long getHttpResponseCode() const throw() { return _http_response_code ; }
    
    void setHttpFailOnError(bool val) { _httpFailOnError = val; }
    bool httpFailOnError() const { return _httpFailOnError; }
public:
    static size_t istreamPutCallback(void *ptr_buffer_out, size_t size, size_t nmemb, void *ptr_istream_in ) ;
    static size_t ostreamGetCallback(void *ptr_buffer_in,  size_t size, size_t nmemb, void *ptr_ostream_out) ;
    // PRQA S 2100 3
    static const std::string ftpesAuthenticationType;
	
	static const std::string Confkey_verifycert_hostname ;
	static const std::string Confkey_verifycert_verifyhost ;
	static const std::string Confkey_verifycert_cacert ;
	static const std::string Confkey_verifycert_cert ;
	static const std::string Confkey_verifycert_key ;

protected:
    void updateStatistics(CURLcode res);

    TransactionType getOperationCode() const throw() ;
    void setOperationCode(TransactionType) throw() ;
    CurlTransaction* getTransaction() const throw() ; 
    void setCurl(CURL *) throw() ; 
    CURL * getCurl() const throw() ; 

    static int debugCallback(void *curl, int infotype, char *dbg, size_t dbgsize, void *dbgdata);

private:
    Curl(Curl const& ); // not implemented
    Curl& operator=(Curl const& ); // not implemented
    //Default Callbacks
    static size_t listCallback(void *buffer, size_t size, size_t nmemb, void *stream);
    static size_t probeCallBack(void *buffer, size_t size, size_t nmemb, void *curl);
    static size_t getCallback(void *buffer, size_t size, size_t nmemb, void *stream);
    static size_t putCallback(void *ptr, size_t size, size_t nmemb, void *stream);
    void setProxy() ;
	void checkFtpCommandResponseTimeout() throw();
  void setHTTPOpts(const CurlHttpOpt& opt);

private:
    CurlTransaction* _transaction;
    CURL* _curl;
    
    static const int _errorMessageSize = 500 ;
    
    char *_errorMessage ;
    RemoteSite _site;
    ProgressCallback _progressCallback;
    ListCallback _listCallback;
    GetCallback _getCallback;
    PutCallback _putCallback;
    DebugCallback _debugCallback;
    std::string _userPassword;
    std::string _ftpDefaultPort;
    bool _createMissingDirs;
    void* _progressCallbackData;
    void* _listCallbackData;
    std::vector<boost::shared_ptr<CurlHeaderMaker> > _ptrToHeaders;
    bool _isCleaning;
    size_t _connectionTimeout_ms ;
    size_t _connectionTimeout_s ;
    size_t _response_timeout_ms ;
    size_t _response_timeout_s ;
    size_t _ftp_command_response_timeout_s;
    mutable std::string _lastLoggedErrorMessage;
    TransactionType _operationCode;
    Statistics _stats;
    static size_t _delayPeriodCounter ;
    std::string tmp_proxy_ ;
    std::string tmp_proxyauth_ ;
    curl_slist *slist_ ;
    std::string httpContentType_;
    int probeToken_ ;
    std::vector< std::string > _custhdrs ;
    long _http_response_code ;
    bool _httpFailOnError;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(Curl) ;  
};

_ACS_END_NESTED_NAMESPACE

#endif /* _Curl_H_ */
