// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2016, Advanced Computer Systems , Inc.
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
 *  Revision 5.13  2016/12/14 10:45:30  marfav
 *  makesite uses toURL to detect local transfers and use correct URL syntax
 *
 *  Revision 5.12  2016/12/14 09:32:01  marfav
 *  Stripping username, password and host while making local URLs in toURL
 *
 *  Revision 5.11  2016/12/13 17:01:41  marfav
 *  CSGACQ-63 Local URL management rewritten
 *
 *  Revision 5.10  2016/10/21 11:20:18  marpas
 *  getting rid of boost when unnecessary
 *  ethod toURL rationalization
 *
 *  Revision 5.9  2016/10/19 08:28:29  francesco.avanzi
 *  OPMAN-195: now the path is kept absolute even if slashes are not duplicated
 *
 *  Revision 5.8  2015/08/04 08:44:41  lucio.pulvirenti
 *  S2PDGS-1247: media protocol no mre managed as localhost
 *
 *  Revision 5.7  2015/02/19 14:50:56  francesco.avanzi
 *  RemoteSite now takes in account Ciphers
 *
 *  Revision 5.6  2014/09/01 15:23:46  lucio.pulvirenti
 *  S2PDGS-841: toURL: if site password is encrypted and boolean parameter is set to false, password is logged as *** only if not an empty string.
 *
 *  Revision 5.5  2014/02/07 18:47:33  marpas
 *  adopting ACS_THROW and ACS_COND_THRO macros
 *
 *  Revision 5.4  2013/10/24 18:28:00  marpas
 *  work in progress
 *
 *  Revision 5.3  2013/10/23 17:28:11  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/10/22 17:31:51  marpas
 *  work in progress
 *
 *  Revision 5.1  2013/07/25 16:09:29  marpas
 *  use of old plain throwConditionedException
 *
 *  Revision 5.0  2013/06/06 18:08:22  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.22  2012/12/14 16:01:35  micmaz
 *  work in progress
 *
 *  Revision 2.21  2012/12/13 16:33:14  micmaz
 *  work in progress
 *
 *  Revision 2.20  2012/12/12 11:20:13  micmaz
 *  removed warnings
 *
 *  Revision 2.19  2012/12/12 10:42:54  micmaz
 *  removed warnings
 *
 *  Revision 2.18  2012/12/11 14:40:52  micmaz
 *  IN PROGRES: http://jira.acsys.it/browse/ADC-97
 *
 *  Revision 2.17  2012/12/07 17:58:13  micmaz
 *  IN PROGRES: http://jira.acsys.it/browse/ADC-97
 *
 *  Revision 2.16  2012/12/05 10:31:19  marpas
 *  getting rid of FilenameUtils
 *
 *  Revision 2.15  2012/12/03 16:56:48  marpas
 *  using new StringUtils interface
 *
 *  Revision 2.14  2012/06/19 13:27:31  micmaz
 *  libFtpCurl now can transfer files without password.
 *
 *  Revision 2.13  2012/06/18 12:36:30  micmaz
 *  the FtpCurl accepts url without user+password
 *
 *  Revision 2.12  2012/04/02 14:42:29  micmaz
 *  work in progress
 *
 *  Revision 2.11  2012/03/27 10:52:22  micmaz
 *  work in progress
 *
 *  Revision 2.10  2012/03/19 12:52:06  micmaz
 *  work in progress
 *
 *  Revision 2.9  2012/03/16 15:50:04  micmaz
 *  *** empty log message ***
 *
 *  Revision 2.8  2012/03/15 14:35:13  micmaz
 *  *** empty log message ***
 *
 *  Revision 2.7  2012/03/15 14:17:55  micmaz
 *  *** empty log message ***
 *
 *  Revision 2.6  2012/03/09 17:44:05  micmaz
 *  added media functionality
 *
 *  Revision 2.5  2012/03/08 13:11:41  micmaz
 *  added media functionality
 *
 *  Revision 2.4  2012/03/02 11:27:02  micmaz
 *  removed unused files and correct a check in RemoteSiteFactory
 *
 *  Revision 2.3  2012/01/24 17:02:49  micmaz
 *  IN PROGRESS - issue S1PDGS-2734: Implicit Circulation: wrong management of local data
 *  http://jira.acsys.it/browse/S1PDGS-2734
 *
 *  Revision 2.2  2012/01/24 13:33:53  micmaz
 *  IN PROGRESS - issue S1PDGS-2740: Wrong Management of the FTPS Protocol
 *  http://jira.acsys.it/browse/S1PDGS-2740
 *
 *  Revision 2.1  2012/01/20 19:31:46  micmaz
 *  IN PROGRESS - issue S1PDGS-2734: Implicit Circulation: wrong management of local data
 *  http://jira.acsys.it/browse/S1PDGS-2734
 *
 *  Revision 2.0  2011/10/18 14:43:52  marpas
 *  new statistics adopted
 *
 *  Revision 1.10  2011/09/15 12:46:41  micmaz
 *  when two files/directories are on the same localhost system, they are not copied but linked (hardlink).
 *
 *  Revision 1.9  2011/09/15 10:30:07  micmaz
 *  in case of localhost:// protocol, the url was not correctly formed.
 *
 *  Revision 1.8  2011/09/08 13:12:20  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.7  2011/07/01 16:42:12  micmaz
 *  changed the temporary file name strategy.
 *
 *  Revision 1.6  2011/06/28 08:42:37  micmaz
 *  added a new specific exception for malformed urls.
 *
 *  Revision 1.5  2011/06/24 16:28:41  micmaz
 *  work in progress
 *
 *  Revision 1.4  2011/06/14 13:36:26  micmaz
 *  work in progress.
 *
 *  Revision 1.3  2011/06/13 09:35:50  micmaz
 *  Added a new method to RemoteSiteFactory in order to decode an url in the following format:  protocol://host/path
 *
 *  Revision 1.2  2011/06/01 16:37:35  micmaz
 *  added crypto to libCurl
 *
 *  Revision 1.1  2011/05/11 15:48:38  micmaz
 *  rules modifications
 *
 *
 *
 */
#include <RemoteSiteFactory.h>

#include <FtpCurl.h>

#include <StringUtils.h>
#include <CurlConfigurer.h>
#include <HWScan.h>

#include <iostream>
#include <sstream>


_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) 

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(RemoteSiteFactory);

RemoteSiteFactory::RemoteSiteFactory() {
    // empty.
}

RemoteSiteFactory::~RemoteSiteFactory() {
    // empty.
}

RemoteSite RemoteSiteFactory::makeSite(std::string const& url, std::string const& user, std::string const& password,
        bool encrypted_password, const std::string &ciphers) {
    
    ACS_COND_THROW(url.empty(), URLMalformedException("The site url is empty.")); // PRQA S 3081 2
    ACS_COND_THROW(user.empty() && !password.empty(), URLMalformedException("The user name is unknown but password is set!"));

    size_t protocolSeparator = url.find(RemoteSite::protocolSeparator);
    ACS_COND_THROW(protocolSeparator == string::npos, URLMalformedException("Incorrect Site URL.")); // PRQA S 3081
    string protocol = StringUtils::substring(url, 0, protocolSeparator);
    string body = StringUtils::substring(url, protocolSeparator + 3, url.length()); // PRQA S 3084
    size_t remoteDirToken = body.find('/');
    string host;
    string path;
    if (remoteDirToken != string::npos) {
        host = StringUtils::substringBefore(body, "/");
        path = StringUtils::substringAfter(body, "/");
    } 
    else {
        host = RemoteSite::localhost;
        path = body;
    }

	// Build an URL that makes sense
	string completeURL = RemoteSiteFactory::toURL (protocol, user, password, host, path);
    return makeSite(completeURL, encrypted_password, ciphers);
}

RemoteSite RemoteSiteFactory::makeSite(std::string const& url, bool encrypted_password, const std::string &ciphers) {
    ACS_COND_THROW(url.empty(), URLMalformedException("The site url is empty.")); // PRQA S 3081
    RemoteSite site(RemoteSite::ftp, temp::unknownStrategy, ciphers);
    //if the url is a local url, then it starts with:
    // "/path/to/file"                         only absolute paths are allowed
    // "media://"                              media type
    // "fs://"                                 file system type
    // "file://"                               file system type (synonymous of fs://)
    //in this case there are not user and password fields.

    // "localhost//path/to/file"               file system on localhost -> HAS BEEN DISCONTINUED

    if (isLocalhost(url)) 
	{
        site.setProtocol(RemoteSite::file);
        site.setHost(RemoteSite::localhost);

        if (StringUtils::startsWith(url, RemoteSite::fileSystem) ||
                StringUtils::startsWith(url, RemoteSite::file) ) 
		{
            size_t protocolSeparator = url.find(RemoteSite::protocolSeparator);
            ACS_COND_THROW(protocolSeparator == string::npos, URLMalformedException("Incorrect Site URL."));  // PRQA S 3081

			// Here can be 
			// protocol://host/path -- (Case A)
			// or
			// protocol:///path     -- (Case B) this means no host explicit
			// or
			// protocol:/path       -- (Case C) this means no host implicit

			string absPath ("/");
			string tmpstr = url.substr(protocolSeparator + 1, 3); // PRQA S 3084, 4400
			if (tmpstr == "///")
			{
				// Case B
				// Keep the last slash and the absolute path
				// protocol:///abs/path
				//         0123  
				absPath = url.substr (protocolSeparator + 3); // PRQA S 3084, 4400
			}
			else if (tmpstr.substr(0,2) == "//") // PRQA S 4400
			{
				// Case A
				// find the next slash
				// protocol://host/path
				//         0123
				// protocolseparator + 3 is the start of the host
				size_t nextSlash = url.find ('/', protocolSeparator + 3); // PRQA S 3084, 4400
				ACS_COND_THROW ( nextSlash == string::npos, URLMalformedException ("No path detected after the hostname in url \"" + url + "\"")); // PRQA S 3081
				
				// Keep the next slash with the absolute path
				absPath = url.substr ( nextSlash );
			}
			else if (tmpstr.substr(0,1) == "/")
			{
				// Case C
				// protocol:/this/is/abs/path
				//         01
				absPath = url.substr (protocolSeparator + 1); // PRQA S 3084
			}
			else
			{
				ACS_THROW ( URLMalformedException ("Unable to decode the local URL \"" + url + "\"") ); // PRQA S 3081
			}

            site.setPath (absPath);
        } 
        else 
		{
			// This is an absolute path. Check it 
			ACS_COND_THROW ( url.empty() || (url[0] != '/'), URLMalformedException ("Can specify only absolute paths in local URL \"" + url + "\"")); // PRQA S 3081
			site.setPath (url);
        }
    } 
    else {
        size_t protocolSeparator = url.find(RemoteSite::protocolSeparator);
        if (protocolSeparator == string::npos) {
            ACS_LOG_ERROR("Wrong url: " << url);
        }
        ACS_COND_THROW(protocolSeparator == string::npos, URLMalformedException("Incorrect Site URL.")); // PRQA S 3081
        //Set the protocol type.
        string protocol = StringUtils::substring(url, 0, protocolSeparator);
        site.setProtocol(protocol);
        string body = url.substr(protocolSeparator + 3) ; // PRQA S 3084
        //takes the url body: user:password @ site / path.
        size_t annotationToken = body.find('@');
        if (annotationToken != string::npos) {
            //the url is with authentication
            string userAndPwd = StringUtils::substring(body, 0, annotationToken);
            size_t userAndPwdSeparator = userAndPwd.find(':');
            if (userAndPwdSeparator != string::npos) {
                string user = StringUtils::substring(userAndPwd, 0, userAndPwdSeparator);
                site.setUser(user);
                string password = userAndPwd.substr(userAndPwdSeparator + 1) ; // PRQA S 3084
                site.setPassword(password);
            } 
            else {
                site.setUser(userAndPwd);
            }
            body = body.substr(annotationToken + 1) ; // PRQA S 3084
        }
        size_t remoteUrlToken = body.find('/');
        if (remoteUrlToken != string::npos) {
            string host = StringUtils::substring(body, 0, remoteUrlToken);
            site.setHost(host);
            string path = body.substr( remoteUrlToken ) ; // PRQA S 3084

			if ( path.empty() || (path[0] != '/') ) {
				path = '/'+path;
			}
            site.setPath(path);
        } 
        else {
            site.setHost(body);
        }
        // the site will be probed later...up to now set the passive mode to true as default.
        site.setIsPassive(true);
        site.setIsPasswordEncrypted(encrypted_password);
    }
    return site;
}

string RemoteSiteFactory::toURL(string const& protocol, string const& user, string const& pswd, string const& host,
        string const& path) 
{
    ostringstream os;
    ACS_COND_THROW(path.empty(), URLMalformedException("The remote host path must be knwown.")); // PRQA S 3081
    if (protocol.empty()) 
	{
        ACS_LOG_WARNING("Warning: no protocol detected: using filesystem protocol!");
        os << RemoteSite::file << RemoteSite::protocolSeparator;
    } 
    else if (!isLocalhost(protocol))
	{
		// This is a remote protocol.. need to add user and password if any
        os << protocol << RemoteSite::protocolSeparator << user;
        if (not pswd.empty() && not user.empty()) {
            os << ":" << pswd;
        }
        if (not user.empty()) {
            os << "@";
        }
        os << host ;
    }
	else
	{
		// It is a local protocol
		// do not mind username, password and host but enforce the URL syntax
		// file:/absolute/path
		os << RemoteSite::file << RemoteSite::protocolSeparator;
	}
    
    // NB: path cannot be empty as per excecption above.
    if (path[0] != '/') { os << '/' ; }
    os << path ;
    return os.str();
}

std::string RemoteSiteFactory::toURL(RemoteSite const& site, bool withEncryptedPassword) {
    //URL format:
    ///<url-path>
    //URL format examples:
    //sftp://<user>:<password>@<host>:<port>/<url-path>
    //ftp://<user>@<host>:<port>/<url-path>
    //fs:///<url-path>
    //media:///<url-path>
    //ftps://<user>@<host>:<port>/<url-path>
    stringstream os;
    if (!site.isLocalhost()) {
        os << site.getProtocol();
        os << RemoteSite::protocolSeparator;
        os << site.getUser() ; 
        string password;
        if (site.isPasswordEncrypted()) {
						
			password = site.getEncryptedPassword();
            if( not withEncryptedPassword && not password.empty() ) {
				password = "***";
            }

        } 
        else {
            password = site.getPassword();
        }
        os << ((not password.empty() && not site.getUser().empty()) ? ":" + password : ""); // PRQA S 3380, 3385
        os << (not site.getUser().empty() ? "@" : ""); // PRQA S 3380
        os << site.getHost();
        if (site.getPort() >= 0) {
            os << ":" << site.getPort();
        }
    }
    const string &path = site.getPath() ;
    if (path.empty() || (path[0] != '/')) { os << '/' ; }
    os << path ;
    return os.str();
}

bool RemoteSiteFactory::isLocalhost(std::string const& url) {
    return (StringUtils::startsWith(url, "/")
            || StringUtils::startsWith(url, RemoteSite::fileSystem)
            || StringUtils::startsWith(url, RemoteSite::file) );
}

_ACS_END_NESTED_NAMESPACE
