// PRQA S 1050,4401,4020 EOF
/*

 Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

 $Prod: A.C.S. libFtpCurl Library $
*/

#include <CurlErrorBehaviourSingleton.h>
#include <StringUtils.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

#if ( (LIBCURL_VERSION_MAJOR < 7) || ( (LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR <= 15))  )
#warning libcurl version < 7.15
#endif

namespace {
	CurlErrorBehaviourSingleton::CurlErrors const c_err[] = { // NOSONAR - array
		{ 0,"CURLE_OK", false, CurlErrorBehaviourSingleton::UNDEFINED},
		{ 1,"CURLE_UNSUPPORTED_PROTOCOL", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 2,"CURLE_FAILED_INIT", false, CurlErrorBehaviourSingleton::RECOVERABLE},                 // Not configurable
		{ 3,"CURLE_URL_MALFORMAT", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 4,"CURLE_NOT_BUILT_IN", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 5,"CURLE_COULDNT_RESOLVE_PROXY", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 6,"CURLE_COULDNT_RESOLVE_HOST", false, CurlErrorBehaviourSingleton::UNRECOVERABLE},      // Not configurable
		{ 7,"CURLE_COULDNT_CONNECT", false, CurlErrorBehaviourSingleton::RECOVERABLE},             // Not configurable
		{ 8,"CURLE_FTP_WEIRD_SERVER_REPLY", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 9,"CURLE_REMOTE_ACCESS_DENIED", false, CurlErrorBehaviourSingleton::UNRECOVERABLE},      // Not configurable
		{ 10,"CURLE_FTP_ACCEPT_FAILED", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 11,"CURLE_FTP_WEIRD_PASS_REPLY", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 12,"CURLE_FTP_ACCEPT_TIMEOUT", true, CurlErrorBehaviourSingleton::RECOVERABLE},
		{ 13,"CURLE_FTP_WEIRD_PASV_REPLY", true, CurlErrorBehaviourSingleton::RECOVERABLE},
		{ 14,"CURLE_FTP_WEIRD_227_FORMAT", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 15,"CURLE_FTP_CANT_GET_HOST", true, CurlErrorBehaviourSingleton::RECOVERABLE},
		{ 16,"CURLE_OBSOLETE16", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 17,"CURLE_FTP_COULDNT_SET_TYPE", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 18,"CURLE_PARTIAL_FILE", true, CurlErrorBehaviourSingleton::RECOVERABLE},
#ifdef JASONDC
		{ 19,"CURLE_FTP_COULDNT_RETR_FILE", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},     // Configurable - https://projectit.exprivia.it/browse/S6JCS-274
#else
		{ 19,"CURLE_FTP_COULDNT_RETR_FILE", false, CurlErrorBehaviourSingleton::UNRECOVERABLE},    // Not configurable - used by ISM
#endif
		{ 20,"CURLE_OBSOLETE20", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 21,"CURLE_QUOTE_ERROR", true, CurlErrorBehaviourSingleton::RECOVERABLE},
		{ 22,"CURLE_HTTP_RETURNED_ERROR", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 23,"CURLE_WRITE_ERROR", false, CurlErrorBehaviourSingleton::UNRECOVERABLE},               // Not configurable
		{ 24,"CURLE_OBSOLETE24", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 25,"CURLE_UPLOAD_FAILED", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 26,"CURLE_READ_ERROR", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 27,"CURLE_OUT_OF_MEMORY", true, CurlErrorBehaviourSingleton::RECOVERABLE},
		{ 28,"CURLE_OPERATION_TIMEDOUT", true, CurlErrorBehaviourSingleton::RECOVERABLE},
		{ 29,"CURLE_OBSOLETE29", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 30,"CURLE_FTP_PORT_FAILED", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 31,"CURLE_FTP_COULDNT_USE_REST", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 32,"CURLE_OBSOLETE32", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 33,"CURLE_RANGE_ERROR", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 34,"CURLE_HTTP_POST_ERROR", true, CurlErrorBehaviourSingleton::RECOVERABLE},
		{ 35,"CURLE_SSL_CONNECT_ERROR", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 36,"CURLE_BAD_DOWNLOAD_RESUME", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 37,"CURLE_FILE_COULDNT_READ_FILE", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 38,"CURLE_LDAP_CANNOT_BIND", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 39,"CURLE_LDAP_SEARCH_FAILED", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 40,"CURLE_OBSOLETE40", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 41,"CURLE_FUNCTION_NOT_FOUND", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 42,"CURLE_ABORTED_BY_CALLBACK", true, CurlErrorBehaviourSingleton::RECOVERABLE},
		{ 43,"CURLE_BAD_FUNCTION_ARGUMENT", true, CurlErrorBehaviourSingleton::RECOVERABLE},
		{ 44,"CURLE_OBSOLETE44", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 45,"CURLE_INTERFACE_FAILED", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 46,"CURLE_OBSOLETE46", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 47,"CURLE_TOO_MANY_REDIRECTS", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 48,"CURLE_UNKNOWN_OPTION", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 49,"CURLE_LDAP_SEARCH_FAILED", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 50,"CURLE_OBSOLETE50", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 51,"CURLE_PEER_FAILED_VERIFICATION", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 52,"CURLE_GOT_NOTHING", true, CurlErrorBehaviourSingleton::RECOVERABLE},
		{ 53,"CURLE_SSL_ENGINE_NOTFOUND", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 54,"CURLE_SSL_ENGINE_SETFAILED", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 55,"CURLE_SEND_ERROR", true, CurlErrorBehaviourSingleton::RECOVERABLE},
		{ 56,"CURLE_RECV_ERROR", true, CurlErrorBehaviourSingleton::RECOVERABLE},
		{ 57,"CURLE_OBSOLETE57", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 58,"CURLE_SSL_CERTPROBLEM", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 59,"CURLE_SSL_CIPHER", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 60,"CURLE_SSL_CACERT", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 61,"CURLE_BAD_CONTENT_ENCODING", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 62,"CURLE_LDAP_INVALID_URL", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 63,"CURLE_FILESIZE_EXCEEDED", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 64,"CURLE_USE_SSL_FAILED", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 65,"CURLE_SEND_FAIL_REWIND", true, CurlErrorBehaviourSingleton::RECOVERABLE},
		{ 66,"CURLE_SSL_ENGINE_INITFAILED", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 67,"CURLE_LOGIN_DENIED", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 68,"CURLE_TFTP_NOTFOUND", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 69,"CURLE_TFTP_PERM", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 70,"CURLE_REMOTE_DISK_FULL", true, CurlErrorBehaviourSingleton::RECOVERABLE},
		{ 71,"CURLE_TFTP_ILLEGAL", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 72,"CURLE_TFTP_UNKNOWNID", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 73,"CURLE_REMOTE_FILE_EXISTS", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 74,"CURLE_TFTP_NOSUCHUSER", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 75,"CURLE_CONV_FAILED", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 76,"CURLE_CONV_REQD", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 77,"CURLE_SSL_CACERT_BADFILE", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 78,"CURLE_REMOTE_FILE_NOT_FOUND", false, CurlErrorBehaviourSingleton::UNRECOVERABLE},      // Not configurable
		{ 79,"CURLE_SSH", true, CurlErrorBehaviourSingleton::RECOVERABLE},
		{ 80,"CURLE_SSL_SHUTDOWN_FAILED", true, CurlErrorBehaviourSingleton::RECOVERABLE},
		{ 81,"CURLE_AGAIN", true, CurlErrorBehaviourSingleton::RECOVERABLE},
		{ 82,"CURLE_SSL_CRL_BADFILE", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 83,"CURLE_SSL_ISSUER_ERROR", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 84,"CURLE_FTP_PRET_FAILED", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 85,"CURLE_RTSP_CSEQ_ERROR", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 86,"CURLE_RTSP_SESSION_ERROR", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 87,"CURLE_FTP_BAD_FILE_LIST", true, CurlErrorBehaviourSingleton::UNRECOVERABLE},
		{ 88,"CURLE_CHUNK_FAILED", true, CurlErrorBehaviourSingleton::RECOVERABLE},
	};

	CurlErrorBehaviourSingleton::CurlAliases const c_aliases[] = { // NOSONAR - array
		{"CURLE_OBSOLETE10","CURLE_FTP_ACCEPT_FAILED"},
		{"CURLE_OBSOLETE12","CURLE_FTP_ACCEPT_TIMEOUT"},
		{"CURLE_UNKNOWN_TELNET_OPTION","CURLE_UNKNOWN_OPTION"},
		{"CURLE_SSL_PEER_CERTIFICATE","CURLE_PEER_FAILED_VERIFICATION"},
		{"CURLE_OBSOLETE","CURLE_OBSOLETE50"},
		{"CURLE_BAD_PASSWORD_ENTERED","CURLE_OBSOLETE46"},
		{"CURLE_BAD_CALLING_ORDER","CURLE_OBSOLETE44"},
		{"CURLE_FTP_USER_PASSWORD_INCORRECT","CURLE_OBSOLETE10"},
		{"CURLE_FTP_CANT_RECONNECT","CURLE_OBSOLETE16"},
		{"CURLE_FTP_COULDNT_GET_SIZE","CURLE_OBSOLETE32"},
		{"CURLE_FTP_COULDNT_SET_ASCII","CURLE_OBSOLETE29"},
		{"CURLE_FTP_WEIRD_USER_REPLY","CURLE_OBSOLETE12"},
		{"CURLE_FTP_WRITE_ERROR","CURLE_OBSOLETE20"},
		{"CURLE_LIBRARY_NOT_FOUND","CURLE_OBSOLETE40"},
		{"CURLE_MALFORMAT_USER","CURLE_OBSOLETE24"},
		{"CURLE_SHARE_IN_USE","CURLE_OBSOLETE57"},
		{"CURLE_URL_MALFORMAT_USER","CURLE_NOT_BUILT_IN"},
		{"CURLE_FTP_ACCESS_DENIED","CURLE_REMOTE_ACCESS_DENIED"},
		{"CURLE_FTP_COULDNT_SET_BINARY","CURLE_FTP_COULDNT_SET_TYPE"},
		{"CURLE_FTP_QUOTE_ERROR","CURLE_QUOTE_ERROR"},
		{"CURLE_TFTP_DISKFULL","CURLE_REMOTE_DISK_FULL"},
		{"CURLE_TFTP_EXISTS","CURLE_REMOTE_FILE_EXISTS"},
		{"CURLE_HTTP_RANGE_ERROR","CURLE_RANGE_ERROR"},
		{"CURLE_FTP_SSL_FAILED","CURLE_USE_SSL_FAILED"},
		{"CURLE_OPERATION_TIMEOUTED","CURLE_OPERATION_TIMEDOUT"},
		{"CURLE_HTTP_NOT_FOUND","CURLE_HTTP_RETURNED_ERROR"},
		{"CURLE_HTTP_PORT_FAILED","CURLE_INTERFACE_FAILED"},
		{"CURLE_FTP_COULDNT_STOR_FILE","CURLE_UPLOAD_FAILED"},
		{"CURLE_FTP_PARTIAL_FILE","CURLE_PARTIAL_FILE"},
		{"CURLE_FTP_BAD_DOWNLOAD_RESUME","CURLE_BAD_DOWNLOAD_RESUME"}
	};
} // namespace end

ACS_CLASS_DEFINE_DEBUG_LEVEL(CurlErrorBehaviourSingleton);

std::string CurlErrorBehaviourSingleton::curlErrorBehaviour2String(CurlErrorBehaviourSingleton::CurlErrorBehaviour beh) {
	std::string tstr;
	switch (beh) {
	case CurlErrorBehaviourSingleton::UNDEFINED:
		tstr = "UNDEFINED";
		break;
	case CurlErrorBehaviourSingleton::RECOVERABLE:
		tstr = "RECOVERABLE";
		break;
	case CurlErrorBehaviourSingleton::UNRECOVERABLE:
		tstr = "UNRECOVERABLE";
		break;
	default:
		ACS_THROW(exIllegalValueException() << "Unrecognized CURL error behaviour having value " << static_cast<unsigned int>(beh)) ; // PRQA S 3081
	}

	return tstr;
}

CurlErrorBehaviourSingleton::CurlErrorBehaviour CurlErrorBehaviourSingleton::string2CurlErrorBehaviour(const std::string& behstr) {
	CurlErrorBehaviourSingleton::CurlErrorBehaviour beh = CurlErrorBehaviourSingleton::UNDEFINED;
	if (StringUtils::equalsNoCase(behstr, "RECOVERABLE")) {
		beh = CurlErrorBehaviourSingleton::RECOVERABLE;
	}
	else if (StringUtils::equalsNoCase(behstr, "UNRECOVERABLE")) {
		beh = CurlErrorBehaviourSingleton::UNRECOVERABLE;
	}
	else {
		ACS_THROW(exIllegalValueException("Unrecognized CURL error type " + behstr)); // PRQA S 3081
	}
	return beh;
}

CurlErrorBehaviourSingleton::CurlErrorBehaviourSingleton() noexcept :
	Singleton<CurlErrorBehaviourSingleton>()
{
	initErrorMaps();
}

CurlErrorBehaviourSingleton::CurlErrorBehaviour CurlErrorBehaviourSingleton::getBehaviour(int error) const {
	Lock lock(_mutex);

	CurlErrorBehaviour errType = UNDEFINED;
	if (error < 0) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "CURL error code " << error << " is not valid.");
	}
	else {
		auto it = _curlErrorsBehaviours.find(error);
		if (it != _curlErrorsBehaviours.end()) {
			errType = it->second.behaviour;
		} else {
			ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Curl error code " << error << " not found.");
		}
	}
	
	return errType;
}

bool CurlErrorBehaviourSingleton::setBehaviour(const std::string& errname, CurlErrorBehaviourSingleton::CurlErrorBehaviour beh) {
	Lock lock(_mutex);
	
	int errcode = 0;
	bool res = false;
	if (getErrorCode(errname, errcode)) {
		res = setBehaviour(errcode, beh);
	}
	return res;
}

bool CurlErrorBehaviourSingleton::setBehaviour(int error, CurlErrorBehaviourSingleton::CurlErrorBehaviour beh) {
	Lock lock(_mutex);
	bool res = false;
	
	if (error < 0) {
		ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "CURL error code " << error << " is not valid.");
		return res;
	}
	
	auto it = _curlErrorsBehaviours.find(error);
	if (it != _curlErrorsBehaviours.end()) {
		// Check error configurability
		if (it->second.configurable) {
			it->second.behaviour = beh;
			res = true;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Behaviour for CURL error " << it->second.errname << " set to " << curlErrorBehaviour2String(it->second.behaviour));
		}
		else {
			ACS_LOG_WARNING("The behaviour of CURL error " << it->second.errname << " cannot be overwritten. Its behaviour is fixed to " << curlErrorBehaviour2String(it->second.behaviour));
		}
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Curl error having code " << error << " not found.");
	}
	
	return res;
}

bool CurlErrorBehaviourSingleton::exists(const std::string& errname) const {
	Lock lock(_mutex);

	return (_curlErrors.find(errname) != _curlErrors.end());
}

bool CurlErrorBehaviourSingleton::exists(int error) const {
	Lock lock(_mutex);
	
	return ((error >= 0) && (_curlErrorsBehaviours.find(error) != _curlErrorsBehaviours.end()));
}

bool CurlErrorBehaviourSingleton::getErrorName(int error, std::string& errname) const {
	Lock lock(_mutex);
	
	bool res = false;
	auto it = _curlErrorsBehaviours.find(error);
	if (it != _curlErrorsBehaviours.end()) {
		errname = it->second.errname;
		res = true;
	}
	
	return res;
}

bool CurlErrorBehaviourSingleton::getErrorCode(const std::string& errname, int& errcode) const {
	Lock lock(_mutex);
	
	bool res = false;
	auto it = _curlErrors.find(errname);
	if (it != _curlErrors.end()) {
		errcode = it->second;
		res = true;
	}
	
	return res;
}

bool CurlErrorBehaviourSingleton::isConfigurable(const std::string& errname) const {
	Lock lock(_mutex);

	bool configurable = false;
	int errcode = 0;
	
	if (getErrorCode(errname, errcode)) {
		configurable = isConfigurable(errcode);
	}
	return configurable;
}

bool CurlErrorBehaviourSingleton::isConfigurable(int error) const {
	Lock lock(_mutex);

	if (error < 0) {
		ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "CURL error code " << error << " is not valid.");
		return false;
	}
	
	auto it = _curlErrorsBehaviours.find(error);
	return (it != _curlErrorsBehaviours.end() && it->second.configurable);
}

void CurlErrorBehaviourSingleton::initErrorMaps() {
	Lock lock(_mutex);

	unsigned int errl = sizeof(c_err)/sizeof(c_err[0]);
	
	// Init map with default behaviour (UNRECOVERABLE) 
	for (unsigned int i=0; i < errl; ++i) {
		_curlErrors[c_err[i].errname] = c_err[i].error;
		_curlErrorsBehaviours[c_err[i].error] = c_err[i];
	}
	
	// Handle aliases
	unsigned int aliasl = sizeof(c_aliases)/sizeof(c_aliases[0]);
	for (unsigned int i=0; i < aliasl; ++i) {
		_curlErrors[c_aliases[i].errname] = _curlErrors[c_aliases[i].alias];
	}
}

_ACS_END_NAMESPACE
