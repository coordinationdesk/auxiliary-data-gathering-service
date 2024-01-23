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
 *  Revision 5.1  2015/02/19 14:50:56  francesco.avanzi
 *  RemoteSite now takes in account Ciphers
 *
 *  Revision 5.0  2013/06/06 18:08:22  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.3  2013/05/29 10:53:59  marpas
 *  interface rationalization,
 *  coding best practices applied,
 *  qa rules
 *
 *  Revision 2.2  2012/06/18 12:36:30  micmaz
 *  the FtpCurl accepts url without user+password
 *
 *  Revision 2.1  2012/02/07 10:25:47  micmaz
 *  removed warnings .
 *
 *  Revision 2.0  2011/10/18 14:43:52  marpas
 *  new statistics adopted
 *
 *  Revision 1.8  2011/09/29 18:33:04  marpas
 *  fixing codecheck crashes
 *
 *  Revision 1.7  2011/06/28 08:42:37  micmaz
 *  added a new specific exception for malformed urls.
 *
 *  Revision 1.6  2011/06/24 16:28:41  micmaz
 *  work in progress
 *
 *  Revision 1.5  2011/06/14 13:36:26  micmaz
 *  work in progress.
 *
 *  Revision 1.4  2011/06/13 09:45:22  micmaz
 *  added comments to RemoteSiteFactory
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
 *
 *
 */
#ifndef _RemoteSiteFactory_H_
#define _RemoteSiteFactory_H_

#include <RemoteSite.h>
#include <StopController.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) 

/**
 * RemoteSite utility: it is used to decode/encode an url from/to the RemoteSite object.
 *
 * The url must respect the following formats:
 * <P>(s)ftp(es)://<user>:<password>@<host>:<port>/<url-path>
 * <P>or else:
 * (s)ftp(es)://<host>/<url-path>
 * <P>or else:
 * /<url-path>
 */
class RemoteSiteFactory { // PRQA S 2109

public:
    /**
     * Retrieve the RemoteSite class from the url string: (s)ftp(es)://<host>/<url-path>
     * @param url the url;
     * @param user the user name;
     * @param password the user password;
     * @param encrypted_password true if the password was encrypted, false otherwise.
     */
    static RemoteSite makeSite(std::string const& url, std::string const& user, std::string const& password, bool encrypted_password, const std::string &ciphers = "");
    /**
     * Retrieve the RemoteSite class from the url string: (s)ftp(es)://<user>:<password>@<host>:<port>/<url-path>
     * @param url the url;
     * @param encrypted_password true if the password was encrypted (and encoded in a textual format, f.e. base64), false otherwise.
     */
    static RemoteSite makeSite(std::string const& url, bool encrypted_password = false, const std::string &ciphers="");
    /**
     * Encode the the url string: (s)ftp(es)://<user>:<password>@<host>:<port>/<url-path>
     * from the RemoteSite site.
     * @param site the RemoteSite used to encode the url.
     */
    static std::string toURL(RemoteSite const& site,bool withEncryptedPassword=true);
    /**
     * Encode the the url string: (s)ftp(es)://<user>:<password>@<host>:<port>/<url-path>
     * from the values provided separately.
     * @param protocol the protocol used, it can be null, in this case the host is assumed to be localhost;
     * @param user the user credential, must be provided if the host is remote;
     * @param pswd the password (it is not mandatory);
     * @param host the host name (must be provided only if it is a remote host);
     * @param path the path where the resource is. This value must be provided.
     */
    static std::string toURL(std::string const& protocol, std::string const& user, std::string const& pswd, std::string const& host,
            std::string const& path);
private:
    static bool isLocalhost(std::string const& url);

private:
    RemoteSiteFactory();
    RemoteSiteFactory(RemoteSiteFactory const& other);
    RemoteSiteFactory& operator=(RemoteSiteFactory const& other);
    virtual ~RemoteSiteFactory();

private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(RemoteSiteFactory) ;

};

exDECLARE_EXCEPTION(URLMalformedException,acs::exException); // PRQA S 2131, 2502


_ACS_END_NESTED_NAMESPACE


#endif /* _RemoteSiteFactory_H_ */
