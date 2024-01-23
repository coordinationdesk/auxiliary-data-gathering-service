// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2014, Advanced Computer Systems , Inc.
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
 *  $Prod: A.C.S. libFtpCurl Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.14  2017/02/14 12:53:35  marpas
 *  ctor added
 *
 *  Revision 5.13  2015/04/23 17:42:37  marpas
 *  interface rationalization
 *
 *  Revision 5.12  2015/02/19 14:50:56  francesco.avanzi
 *  RemoteSite now takes in account Ciphers
 *
 *  Revision 5.11  2014/11/04 17:19:47  marpas
 *  Proxy protocol management added
 *
 *  Revision 5.10  2014/11/03 17:03:11  marpas
 *  simplified, init condition shall not be verified from external
 *
 *  Revision 5.9  2014/10/30 10:32:32  marpas
 *  qa warnings removed
 *
 *  Revision 5.8  2014/10/29 11:59:17  lucio.pulvirenti
 *  S2PDGS-955: Class Proxy added.
 *
 *  Revision 5.7  2014/10/28 16:31:43  lucio.pulvirenti
 *  S2PDGS-955 work in progress.
 *
 *  Revision 5.6  2014/02/11 10:07:26  marpas
 *  useless typedef removed
 *
 *  Revision 5.5  2013/10/28 13:42:14  marpas
 *  cleaning macros and curl versions.
 *
 *  Revision 5.4  2013/10/24 18:28:00  marpas
 *  work in progress
 *
 *  Revision 5.3  2013/10/22 17:31:51  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/06/18 12:54:14  marpas
 *  debug macros rationalized
 *
 *  Revision 5.1  2013/06/11 14:36:01  lucio.pulvirenti
 *  _tempnamevalue string attribute added to IF corresponding to transfer strategy value (e.g. .LOCK).
 *  Input string parameter added to setRenameStrategy method (default empty string) to set _tempnamevalue.
 *  getTempnamevalue() method added to IF to get _tempnamevalue attr.
 *
 *  Revision 5.0  2013/06/06 18:08:22  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.15  2013/05/29 10:53:59  marpas
 *  interface rationalization,
 *  coding best practices applied,
 *  qa rules
 *
 *  Revision 2.14  2013/04/05 09:44:15  marpas
 *  coding best practices applied
 *  qa warnings (partial)
 *  libDCUtility interface update
 *
 *  Revision 2.13  2013/02/14 16:28:31  micmaz
 *  executed regression tests.
 *
 *  Revision 2.12  2012/12/07 17:58:13  micmaz
 *  IN PROGRES: http://jira.acsys.it/browse/ADC-97
 *
 *  Revision 2.11  2012/11/29 16:28:20  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.10  2012/07/25 10:44:21  micmaz
 *  Special characters as #,@,: in URLs have to be hex encoded.
 *
 *  Revision 2.9  2012/06/18 12:36:30  micmaz
 *  the FtpCurl accepts url without user+password
 *
 *  Revision 2.8  2012/04/02 14:42:30  micmaz
 *  work in progress
 *
 *  Revision 2.7  2012/03/27 10:52:22  micmaz
 *  work in progress
 *
 *  Revision 2.6  2012/03/16 15:49:55  micmaz
 *  *** empty log message ***
 *
 *  Revision 2.5  2012/03/08 14:22:46  micmaz
 *  updated tests to cppunit
 *
 *  Revision 2.4  2012/03/08 13:11:41  micmaz
 *  added media functionality
 *
 *  Revision 2.3  2012/03/02 11:27:02  micmaz
 *  removed unused files and correct a check in RemoteSiteFactory
 *
 *  Revision 2.2  2012/02/07 10:25:47  micmaz
 *  removed warnings .
 *
 *  Revision 2.1  2012/01/20 19:31:46  micmaz
 *  IN PROGRESS - issue S1PDGS-2734: Implicit Circulation: wrong management of local data
 *  http://jira.acsys.it/browse/S1PDGS-2734
 *
 *  Revision 2.0  2011/10/18 14:43:52  marpas
 *  new statistics adopted
 *
 *  Revision 1.20  2011/09/29 18:33:04  marpas
 *  fixing codecheck crashes
 *
 *  Revision 1.19  2011/09/29 16:46:53  marpas
 *  rules fixing
 *
 *  Revision 1.18  2011/09/16 13:20:59  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.17  2011/08/26 15:52:05  micmaz
 *  IN PROGRESS - issue S1PDGS-1604: CirculationAgent does not use t_circulationtempnames
 *  http://jira.acsys.it/browse/S1PDGS-1604
 *  IN PROGRESS - issue S1PDGS-1808: Improve the CirculationCurl bytes transfer
 *  http://jira.acsys.it/browse/S1PDGS-1808
 *
 *  Revision 1.16  2011/07/01 16:42:12  micmaz
 *  changed the temporary file name strategy.
 *
 *  Revision 1.15  2011/06/24 16:28:41  micmaz
 *  work in progress
 *
 *  Revision 1.14  2011/06/13 17:33:14  micmaz
 *  work in progress
 *
 *  Revision 1.13  2011/06/01 16:37:35  micmaz
 *  added crypto to libCurl
 *
 *  Revision 1.12  2011/05/11 15:47:53  micmaz
 *  rules modifications
 *
 *  Revision 1.11  2011/05/06 16:31:22  micmaz
 *  work in progress...
 *
 *  Revision 1.10  2011/05/05 17:52:34  micmaz
 *  work in progress...
 *
 *  Revision 1.9  2011/05/03 14:01:34  micmaz
 *  corrected some circulation bugs and add CurlStatistics class
 *
 *  Revision 1.8  2011/04/29 17:20:42  micmaz
 *  check style changes
 *
 *  Revision 1.7  2011/04/26 17:33:23  micmaz
 *  work in progress..
 *
 *  Revision 1.6  2011/04/15 15:33:22  micmaz
 *  work in progress...
 *
 *  Revision 1.5  2011/03/07 11:17:36  micmaz
 *  Added RemoteSiteFactory class.
 *
 *  Revision 1.4  2011/02/23 14:39:26  micmaz
 *  Added: Ftpes support (but not tested) and CirculationCurl in order to make a file-transfer using this server as bridge.
 *
 *  Revision 1.3  2011/01/26 13:38:21  micmaz
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

#ifndef _RemoteSite_H_
#define _RemoteSite_H_

#include <TemporaryFileService.h>
#include <rsResourceSet.h>

#include <exMacros.h>
#include <exException.h>

#include <string>
#include <memory>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) 

/**
 * Value object representing an ftp/sftp remote site.
 */
class RemoteSite { // PRQA S 2109
public:
    /**
     * Ctors & dtor.
     */
    explicit RemoteSite(const std::string &protocol=ftp, temp::TempRenameStrategy strategy=temp::unknownStrategy, const std::string &ciphers="");
    RemoteSite(const std::string &protocol, const std::string &host, int port) ;
    RemoteSite(const std::string &protocol, 
               const std::string &host, 
               const std::string &user, 
               const std::string &password, temp::TempRenameStrategy strategy = temp::unknownStrategy, const std::string &ciphers="");
    RemoteSite& operator=(RemoteSite const& );
    RemoteSite(RemoteSite const& );
    ~RemoteSite() ACS_NOEXCEPT;

    bool isLocalhost() const;
    bool isNetworkLocalhost() const;
    void setIsLocalhost(bool isLocalhost);
    bool isFtpSetProtocol() const ACS_NOEXCEPT ;

    /**
     * Getter for the host property.
     */
    const std::string &getHost() const ACS_NOEXCEPT ;
    /**
     * Getter for the isSecure property (sftp/ftp).
     */
    bool getIsSecure() const;
    /**
     * Getter for the password property.
     */
    std::string getPassword() const;
    /**
     * Getter for the user id property.
     */
    const std::string &getUser() const ACS_NOEXCEPT ;
    /**
     * Setter for the host property.
     */
    void setHost(const std::string &);
    /**
     * Setter for the is secure property (sftp/ftp).
     */
    void setIsSecure(bool) ACS_NOEXCEPT ;
    /**
     * Setter for the password property.
     */
    void setPassword(const std::string &);
    /**
     * Setter for the user id property.
     */
    void setUser(const std::string &);
    /**
     * Getter for the is passive property (PASV or PORT).
     */
    bool getIsPassive() const ACS_NOEXCEPT;
    /**
     * Setter for the is passive property (PASV or PORT).
     */
    void setIsPassive(bool) ACS_NOEXCEPT ;

    /**
     * Getter for the verify certificate property from the whole parameters map
     */
    bool getVerifyCertificate() const ACS_NOEXCEPT;
    /**
     * Setter for the verify certificate parameters 
     */
    void setVerifyCertificateParameters(const std::map<std::string, std::string>&) ACS_NOEXCEPT ;
    /**
     * Getter for the verify certificate parameters (the whole map of parameters)
     */
    std::map<std::string, std::string> getVerifyCertificateParameters() const ACS_NOEXCEPT;
    /**
     * Getter for the verify certificate parameter (one single parameter). The boolean is set if the param has been found
     */
    bool getVerifyCertificateParameter(const std::string& par_name, std::string& par_value) const ACS_NOEXCEPT;


    bool getIsFtpes() const ACS_NOEXCEPT;
    void setIsFtpes() ACS_NOEXCEPT ;

    bool getIsMedia() const ACS_NOEXCEPT;

    bool getIsFtp() const ACS_NOEXCEPT ;
    void setIsFtp() ACS_NOEXCEPT ;

    bool getIsFtps() const ACS_NOEXCEPT;
    void setIsFtps() ACS_NOEXCEPT ;

    bool getIsFileSystem() const ACS_NOEXCEPT ;
	bool getIsHttp() const ACS_NOEXCEPT ;
	bool getIsSecureHttp() const ACS_NOEXCEPT ;
    
    bool getIsAnyFtp() const ACS_NOEXCEPT ;
    /**
     * Setter/getter for port.
     */
    void setPort(int port) ACS_NOEXCEPT ;
    int getPort() const ACS_NOEXCEPT ;
    /**
     * Setter/getter for path.
     */
    void setPath(std::string const& path);
    const std::string &getPath() const ACS_NOEXCEPT ;

    void setCiphers(const std::string &c);
    const std::string & getCipher()const ACS_NOEXCEPT;
    
    void setIsDir(bool isDir) ACS_NOEXCEPT ;
    bool getIsDir() const ACS_NOEXCEPT ;

    void setProtocol(std::string const& protocol);
    const std::string &getProtocol() const ACS_NOEXCEPT ;

    std::string getRemoteDir() const;
    std::string getRemoteFileName() const;

    bool isPasswordEncrypted() const ACS_NOEXCEPT ;
    void setIsPasswordEncrypted(bool isPasswordEncrypted);
    const std::string &getEncryptedPassword() const ACS_NOEXCEPT ;
    void setEncryptedPassword(std::string const& encrypted);
    temp::TempRenameStrategy getRenameStrategy() const ACS_NOEXCEPT ;
    void setRenameStrategy(temp::TempRenameStrategy s, std::string const & = "" )  ;
    bool hasTemporaryTargetName() const ;
    std::string getTemporaryPath() const ;
    std::string getTemporaryPath(std::string const& path) const ;
    std::string getTemporaryDir() const ;
    std::string getTemporaryDir(std::string const& path) const ;
    std::string getTemporaryFile() const ;
    std::string getTemporaryFile(std::string const& path) const ;
    void resetStrategyState();    
	const std::string &getTempnamevalue() const ACS_NOEXCEPT ;

    /*
     * Setter/getter/adder and reset for custom headers
     */
    void setCustomHeaders(const std::vector<std::string>& newCustomHeaders);
    void addCustomHeader(const std::string& newCustomHeader);
    void resetCustomHeaders();
    std::vector<std::string> getCustomHeaders() const;

    std::string str() const;

    bool supportsActivePassive() const ACS_NOEXCEPT { return protocolSupportsActivePassive(_protocol) ; }
    bool supportsDirectory() const ACS_NOEXCEPT { return protocolSupportsDirectory(_protocol) ; }
    bool supportsRemove() const ACS_NOEXCEPT { return protocolSupportsRemove(_protocol) ; }
    bool supportsSource() const ACS_NOEXCEPT { return protocolSupportsSource(_protocol) ; }
    bool supportsDestination() const ACS_NOEXCEPT { return protocolSupportsDestination(_protocol) ; }
public:
    // PRQA S 2100 L1
    static std::string const localhost; 
    static std::string const fileSystem;
    static std::string const file;
    static std::string const sftp;
    static std::string const ftp;
    static std::string const ftps;
    static std::string const ftpes;
    static std::string const media;
    static std::string const http;
    static std::string const https;
    static std::string const protocolSeparator;
    // PRQA L:L1
    static bool protocolSupportsActivePassive(const std::string &) ACS_NOEXCEPT ; 
    static bool protocolSupportsDirectory(const std::string &) ACS_NOEXCEPT ; 
    static bool protocolSupportsRemove(const std::string &) ACS_NOEXCEPT ; 
    static bool protocolSupportsSource(const std::string &) ACS_NOEXCEPT ; 
    static bool protocolSupportsDestination(const std::string &) ACS_NOEXCEPT ; 

private:
    void initRenameStrategy() ;

private:
    std::string _host;
    std::string _user;
    std::string _password;
    int _port;
    std::string _path;
    bool _isPassive;
    std::map<std::string, std::string> _verifyCertParams ;
    bool _isDir;
    bool _isPasswordEncrypted;
    std::vector<std::string> _customHeaders;

    acs::curl::temp::TemporaryFileService _tempService;

    temp::TempRenameStrategy _tempStrategy;
    std::string _protocol;
	std::string _tempnamevalue;
    std::string _ciphers ;
};

std::ostream& operator<<(std::ostream& , const RemoteSite &) ;
acs::exostream& operator<<(acs::exostream& , const RemoteSite &) ; // PRQA S 2072


class Proxy {
    friend void rsResourceSet::getValue<Proxy>(const std::string &res, Proxy &v) const ;
public:
    Proxy() ;
    Proxy(const std::string &h, const std::string &pr, const std::string &u, const std::string &p, unsigned short port=0) ;
    Proxy(const Proxy &) ;
    ~Proxy() ACS_NOEXCEPT ;
    Proxy& operator=(const Proxy &) ;
    
    const std::string &host() const ACS_NOEXCEPT { return host_ ; } /*! empty host means no proxy */
    const std::string &protocol() const ACS_NOEXCEPT { return protocol_ ; } /*! empty host means no proxy */
    const std::string &user() const ACS_NOEXCEPT { return user_ ; } 
    const std::string &password() const ACS_NOEXCEPT { return password_ ; } 
    unsigned short port() const ACS_NOEXCEPT { return port_ ; } 
    
private:
    std::string host_ ;
    std::string protocol_ ;
    std::string user_ ;
    std::string password_ ;
    unsigned short port_ ;

} ;

const Proxy & no_proxy() ;

bool operator==(const Proxy &, const Proxy &) ACS_NOEXCEPT ;

inline 
bool operator!=(const Proxy &a, const Proxy &b) ACS_NOEXCEPT { return not (a == b) ; } // PRQA S 2134


_ACS_END_NAMESPACE
std::ostream& operator<<(std::ostream& , const curl::Proxy &) ;
acs::exostream& operator<<(acs::exostream& , const curl::Proxy &) ; // PRQA S 2072

// in acs namespace because templates

template<>
inline
void rsResourceSet::setValue<curl::Proxy>(const std::string &res, const curl::Proxy &v) // PRQA S 2133, 2134
{
	Lock lock(_mutex) ;
	findOrInsertValue(res+".host")->set(v.host()) ;
	findOrInsertValue(res+".protocol")->set(v.protocol()) ;
	findOrInsertValue(res+".user")->set(v.user()) ;
	findOrInsertValue(res+".password")->set(v.password()) ;
	findOrInsertValue(res+".port")->set(v.port()) ;
}

template<>
inline
void rsResourceSet::getValue(const std::string &res, curl::Proxy &v) const // PRQA S 2133, 2134
{
    Lock lock(_mutex) ;
 	getValue(res+".host")->get(v.host_) ;
    if (not v.host_.empty()) { 
        if (hasKey(res+".protocol")) { getValue(res+".protocol")->get(v.protocol_) ; }
        else { v.protocol_ = "" ; }
        if (hasKey(res+".user")) { getValue(res+".user")->get(v.user_) ; }
        else { v.user_ = "" ; }
	    if (hasKey(res+".password")) { getValue(res+".password")->get(v.password_) ; }
        else { v.password_ = "" ; }
	    if (hasKey(res+".port")) { getValue(res+".port")->get(v.port_) ; }
        else { v.port_ = 0 ; }
    }
    else {
        v = curl::no_proxy() ;
    }
} 

_ACS_END_NAMESPACE

#endif /* _RemoteSite_H_ */
