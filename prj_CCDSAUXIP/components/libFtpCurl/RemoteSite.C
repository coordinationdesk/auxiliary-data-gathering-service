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
 *  Revision 5.21  2017/10/19 15:14:20  marpas
 *  qa warning removed
 *  code best practice implemented
 *
 *  Revision 5.20  2017/02/14 12:53:35  marpas
 *  ctor added
 *
 *  Revision 5.19  2016/12/13 17:01:41  marfav
 *  CSGACQ-63 Local URL management rewritten
 *
 *  Revision 5.18  2016/10/04 14:42:11  marfav
 *  OPMAN-196
 *  Management of basenames and dirnames now relies on libFile
 *  Class FtpDirUtility removed from the library
 *
 *  Revision 5.17  2015/08/04 08:42:05  lucio.pulvirenti
 *  S2PDGS-1247: media protocol no more managed as localhost.
 *
 *  Revision 5.16  2015/04/23 17:42:37  marpas
 *  interface rationalization
 *
 *  Revision 5.15  2015/02/19 14:50:56  francesco.avanzi
 *  RemoteSite now takes in account Ciphers
 *
 *  Revision 5.14  2014/11/04 17:19:47  marpas
 *  Proxy protocol management added
 *
 *  Revision 5.13  2014/11/03 17:03:11  marpas
 *  simplified, init condition shall not be verified from external
 *
 *  Revision 5.12  2014/10/30 10:32:32  marpas
 *  qa warnings removed
 *
 *  Revision 5.11  2014/10/29 11:59:17  lucio.pulvirenti
 *  S2PDGS-955: Class Proxy added.
 *
 *  Revision 5.10  2014/10/28 16:31:43  lucio.pulvirenti
 *  S2PDGS-955 work in progress.
 *
 *  Revision 5.9  2014/06/20 14:52:34  marpas
 *  useless private variables removed
 *  statistics management (log) fixed
 *  statistics management (t_circulationstats) fixed
 *  operation are now an enum
 *
 *  Revision 5.8  2014/06/04 17:29:21  marfav
 *  QA warning removed
 *  HTTP sessions managed without SSL layer
 *  CURL locking callback functions strategy revised
 *
 *  Revision 5.7  2014/02/13 17:26:36  marpas
 *  Crypto class cleansed
 *
 *  Revision 5.6  2014/02/10 17:22:30  lucio.pulvirenti
 *  Warning added.
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
 *  Revision 5.1  2013/06/11 14:38:28  lucio.pulvirenti
 *  init(): the proper value of _tempnamevalue is passed to TemporaryFileService CTOR.
 *  Input string parameter added to setRenameStrategy method (default empty string) to set _tempnamevalue.
 *  getTempnamevalue() method added to IF to get _tempnamevalue attr.
 *
 *  Revision 5.0  2013/06/06 18:08:22  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.15  2013/04/15 09:12:34  marpas
 *  coding best practices applied
 *  interfaces rationalization and simplification
 *
 *  Revision 2.14  2013/02/14 16:28:31  micmaz
 *  executed regression tests.
 *
 *  Revision 2.13  2012/12/07 17:58:13  micmaz
 *  IN PROGRES: http://jira.acsys.it/browse/ADC-97
 *
 *  Revision 2.12  2012/12/05 10:31:19  marpas
 *  getting rid of FilenameUtils
 *
 *  Revision 2.11  2012/07/25 10:44:21  micmaz
 *  Special characters as #,@,: in URLs have to be hex encoded.
 *
 *  Revision 2.10  2012/06/18 12:36:30  micmaz
 *  the FtpCurl accepts url without user+password
 *
 *  Revision 2.9  2012/04/02 14:42:30  micmaz
 *  work in progress
 *
 *  Revision 2.8  2012/03/27 10:52:22  micmaz
 *  work in progress
 *
 *  Revision 2.7  2012/03/19 14:40:48  micmaz
 *  work in progress
 *
 *  Revision 2.6  2012/03/16 15:49:55  micmaz
 *  *** empty log message ***
 *
 *  Revision 2.5  2012/03/09 17:44:05  micmaz
 *  added media functionality
 *
 *  Revision 2.4  2012/03/08 13:11:40  micmaz
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
 *  Revision 1.21  2011/09/29 18:33:04  marpas
 *  fixing codecheck crashes
 *
 *  Revision 1.20  2011/09/16 13:20:59  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.19  2011/08/26 15:52:05  micmaz
 *  IN PROGRESS - issue S1PDGS-1604: CirculationAgent does not use t_circulationtempnames
 *  http://jira.acsys.it/browse/S1PDGS-1604
 *  IN PROGRESS - issue S1PDGS-1808: Improve the CirculationCurl bytes transfer
 *  http://jira.acsys.it/browse/S1PDGS-1808
 *
 *  Revision 1.18  2011/08/12 14:49:04  micmaz
 *  work in progress..
 *
 *  Revision 1.17  2011/07/14 15:19:37  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.16  2011/07/01 16:42:12  micmaz
 *  changed the temporary file name strategy.
 *
 *  Revision 1.15  2011/06/24 16:28:41  micmaz
 *  work in progress
 *
 *  Revision 1.14  2011/06/16 18:27:12  micmaz
 *  work in progress
 *
 *  Revision 1.13  2011/06/13 17:33:14  micmaz
 *  work in progress
 *
 *  Revision 1.12  2011/06/01 16:37:35  micmaz
 *  added crypto to libCurl
 *
 *  Revision 1.11  2011/05/30 18:01:14  micmaz
 *  correct putDir()
 *
 *  Revision 1.10  2011/05/11 15:47:53  micmaz
 *  rules modifications
 *
 *  Revision 1.9  2011/05/06 16:31:21  micmaz
 *  work in progress...
 *
 *  Revision 1.8  2011/05/05 17:52:34  micmaz
 *  work in progress...
 *
 *  Revision 1.7  2011/04/29 17:20:42  micmaz
 *  check style changes
 *
 *  Revision 1.6  2011/04/26 17:33:23  micmaz
 *  work in progress..
 *
 *  Revision 1.5  2011/04/15 15:33:22  micmaz
 *  work in progress...
 *
 *  Revision 1.4  2011/03/07 11:17:36  micmaz
 *  Added RemoteSiteFactory class.
 *
 *  Revision 1.3  2011/02/23 14:39:26  micmaz
 *  Added: Ftpes support (but not tested) and CirculationCurl in order to make a file-transfer using this server as bridge.
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

#include <RemoteSite.h>
#include <CurlConfigurer.h>
#include <RemoteSiteUtility.h>
#include <File.h>

#include <StringUtils.h>

#include <sstream>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl)

using std::string;

string const RemoteSite::localhost = "localhost";
string const RemoteSite::fileSystem = "fs";
string const RemoteSite::file = "file";
string const RemoteSite::sftp = "sftp";
string const RemoteSite::ftp = "ftp";
string const RemoteSite::ftps = "ftps";
string const RemoteSite::ftpes = "ftpes";
string const RemoteSite::media = "media";
string const RemoteSite::http = "http";
string const RemoteSite::https = "https";

string const RemoteSite::protocolSeparator = "://";
string const RemoteSite::authSeparator = "@";
string const RemoteSite::userPasswordSeparator = ":";
string const RemoteSite::portSeparator = ":";
string const RemoteSite::pathSeparator = "/";

RemoteSite::RemoteSite(const std::string& protocol, temp::TempRenameStrategy strategy, const std::string& ciphers) :
  _tempStrategy(strategy),
  _protocol(protocol),
  _ciphers(ciphers)
{
  initRenameStrategy();
}

RemoteSite::RemoteSite(const std::string& protocol, const std::string& host, int port) :
  _port(port),
  _protocol(protocol)
{
  setHost(host);
  ACS_COND_THROW(port != _port,
                 exIllegalValueException() << "Host '" << host << "' contain a different port than input one (" << port
                                           << ")!");
  initRenameStrategy();
}

RemoteSite::RemoteSite(const std::string& protocol,
                       const std::string& host,
                       const std::string& user,
                       const std::string& password, temp::TempRenameStrategy strategy, const std::string& ciphers) :
  _user(user),
  _password(password),
  _tempStrategy(strategy),
  _protocol(protocol),
  _ciphers(ciphers)
{
  setHost(host);
  ACS_COND_THROW(user != _user,
                 exIllegalValueException() << "Host '" << host << "' contain a different user than input one ('" << user
                                           << "')!");
  ACS_COND_THROW(password != _password,
                 exIllegalValueException() << "Host '" << host << "' contain a different password than input one ('"
                                           << password << "')!");
  initRenameStrategy();
}

RemoteSite::RemoteSite(const std::string& protocol,
                       const std::string& host,
                       const int port,
                       const std::string& user,
                       const std::string& password, temp::TempRenameStrategy strategy, const std::string& ciphers) :
  _user(user),
  _password(password),
  _port(port),
  _tempStrategy(strategy),
  _protocol(protocol),
  _ciphers(ciphers)
{
  setHost(host);
  ACS_COND_THROW(port != _port,
                 exIllegalValueException() << "Host '" << host << "' contain a different port than input one (" << port
                                           << ")!");
  ACS_COND_THROW(user != _user,
                 exIllegalValueException() << "Host '" << host << "' contain a different user than input one ('" << user
                                           << "')!");
  ACS_COND_THROW(password != _password,
                 exIllegalValueException() << "Host '" << host << "' contain a different password than input one ('"
                                           << password << "')!");
  initRenameStrategy();
}

void RemoteSite::initRenameStrategy()
{
  if (_tempStrategy == temp::unknownStrategy)
  {
    _tempStrategy = temp::TemporaryFileService::makeStrategy(conf::CurlConfigurerSingleton::instance()->getTempNameStrategy());
    if (_tempStrategy == temp::unknownStrategy)
    {
      _tempStrategy = temp::TemporaryFileService::makeDefaultStrategy();
    }
    _tempnamevalue = "";
  }
  _tempService = temp::TemporaryFileService(_tempStrategy, _tempnamevalue);
}

string RemoteSite::str() const
{
  std::ostringstream os;
  string passwordString;
  if (isPasswordEncrypted())
  {
    passwordString = ", pwd:**** ";  // NOSONAR - fake pwd
  }
  else
  {
    passwordString = ", pwd:" + _password;
  }
  os << "RemoteSite{host:" << _host << ", user:" << _user << passwordString
     << ", protocol:" << _protocol << " (" << (_isPassive ? "passive" : "active") << ") "
     << ", verify certificate: " << (getVerifyCertificate() ? "true" : "false")
     << ", verification certificate parameters: { ";
  for (auto it = _verifyCertParams.begin(); it != _verifyCertParams.end(); it++)  // NOSONAR
  {
    os << "\"" << it->first << "\": \"" << it->second << "\" ; ";
  }
  os << "}, path:" << _path
     << ", temp:" << _tempStrategy << ", tempnamevalue: \"" << _tempnamevalue << "\"";

  if (!_customHeaders.empty())
  {
    os << ", customHeader: [";
    for (std::vector<std::string>::const_iterator it = _customHeaders.begin(); it != _customHeaders.end(); ++it)
    {  // NOSONAR
      if (it != _customHeaders.begin())
      {
        os << ", ";
      }
      os << "\"" << *it << "\"";
    }
    os << "]";
  }

  os << "}";

  return os.str();
}

string RemoteSite::getTemporaryPath() const
{
  return _tempService.getTemporaryPath(getPath());
}

string RemoteSite::getTemporaryPath(string const& path) const
{
  return _tempService.getTemporaryPath(path);
}

string RemoteSite::getTemporaryDir() const
{
  return _tempService.getTemporaryDir(getPath());
}

string RemoteSite::getTemporaryDir(string const& path) const
{
  return _tempService.getTemporaryDir(path);
}

string RemoteSite::getTemporaryFile() const
{
  return _tempService.getTemporaryFile(getPath());
}

string RemoteSite::getTemporaryFile(string const& path) const
{
  return _tempService.getTemporaryFile(path);
}

void RemoteSite::resetStrategyState()
{
  _tempService.resetStrategyState();
}

string RemoteSite::getPassword() const
{
  if (_password.empty() || !isPasswordEncrypted())
  {
    return _password;
  }

  return RemoteSiteUtility::decrypt(_password);
}

void RemoteSite::setIsLocalhost(bool isLocalhost)
{
  if (isLocalhost)
  {
    _host = localhost;
  }
}

bool RemoteSite::isLocalhost() const
{
  return (_host.empty() ||
          StringUtils::equalsNoCase(_protocol, fileSystem) ||
          StringUtils::equalsNoCase(_protocol, file));
}

bool RemoteSite::isNetworkLocalhost() const
{
  return (isLocalhost() || StringUtils::equalsNoCase(_host, localhost));
}

//
// Getters/Setters.
//

const string& RemoteSite::getHost() const noexcept
{
  return _host;
}

std::string RemoteSite::getHostAndPort() const noexcept
{
  std::ostringstream os;
  os << _host;

  if (_port > 0 && !_host.empty())
  {
    os << portSeparator << _port;
  }

  return os.str();
}

bool RemoteSite::getIsMedia() const noexcept
{
  return StringUtils::equalsNoCase(_protocol, media);
}

bool RemoteSite::getIsSFTP() const noexcept
{
  return StringUtils::equalsNoCase(_protocol, sftp);
}

const string& RemoteSite::getUser() const noexcept
{
  return _user;
}

void RemoteSite::setHost(const string& host)
{
  ACS_COND_THROW(host.find(protocolSeparator) != string::npos,
                 exIllegalValueException() << "Host '" << host << "' contain a protocol separator")

  ACS_COND_THROW(host.find(pathSeparator) != string::npos,
                 exIllegalValueException() << "Host '" << host << "' contain a path")

  static bool toWarnUserPwd = true;
  _host = host;

  const size_t authPos = _host.find(authSeparator);
  if (authPos != string::npos)
  {
    if (toWarnUserPwd)
    {
      ACS_LOG_WARNING("Host '" << host << "' contain authentication data. Use setUser and setPassword");
      toWarnUserPwd = false;
    }

    // the url is with authentication
    string userAndPwd = StringUtils::substring(_host, 0, authPos);
    const size_t userPasswordPos = userAndPwd.find(userPasswordSeparator);
    if (userPasswordPos != string::npos)
    {
      setUser(Curl::decodeRFC398(StringUtils::substring(userAndPwd, 0, userPasswordPos)));
      setPassword(Curl::decodeRFC398(userAndPwd.substr(userPasswordPos + userPasswordSeparator.size())));
    }
    else
    {
      setUser(Curl::decodeRFC398(userAndPwd));
    }
    _host = _host.substr(authPos + authSeparator.size());
  }

  static bool toWarnPort = true;

  size_t portPos = _host.find(portSeparator);
  if (portPos != string::npos)
  {
    if (toWarnPort)
    {
      ACS_LOG_WARNING("Host '" << host << "' contain port. Use setPort");
      toWarnPort = false;
    }

    const string portStr = StringUtils::substring(_host, portPos + portSeparator.size());
    size_t portConv;
    const int port = stoi(portStr, &portConv);

    ACS_COND_THROW(portConv != portStr.size(), exIllegalValueException() << "Port '" << portStr << "' on host '" << host
                                                                         << "' is not valid");
    ACS_COND_THROW(port < 0 || port > 65535, exIllegalValueException() << "Port '" << portStr << "' on host '"
                                                                       << host << "' is out of the range 0-65535");

    setPort(port);

    _host = StringUtils::substring(_host, 0, portPos);
  }
}

void RemoteSite::setIsSFTP() noexcept
{
  _protocol = sftp;
}

void RemoteSite::setPassword(const string& password)
{
  _password = password;
  // This will check if any encrypted password is good or not
  getPassword();
}

void RemoteSite::setUser(const string& user)
{
  _user = user;
}

bool RemoteSite::getIsPassive() const noexcept
{
  return _isPassive;
}

void RemoteSite::setIsPassive(bool isPassive) noexcept
{
  _isPassive = isPassive;
}

bool RemoteSite::getVerifyCertificate() const noexcept
{
  string par_value;
  const bool to_verify = ((true == getVerifyCertificateParameter(Curl::Confkey_verifycert_verifyhost, par_value)) &&
                          (Curl::Confkey_verifycert_verifyhost == par_value));

  return to_verify;
}

void RemoteSite::setVerifyCertificateParameters(const std::map<std::string, std::string>& m) noexcept  // NOSONAR - map no transparent comp
{
  _verifyCertParams = m;
}

std::map<std::string, std::string> RemoteSite::getVerifyCertificateParameters() const noexcept  // NOSONAR - map no transparent comp
{
  // Getter for the verify certificate parameters (the whole map of parameters)
  return _verifyCertParams;
}

bool RemoteSite::getVerifyCertificateParameter(const std::string& par_name, std::string& par_value) const noexcept
{
  // Getter for the verify certificate parameter (one single parameter)

  bool found;

  if ((found = (_verifyCertParams.find(par_name) != _verifyCertParams.end())))
  {
    par_value = _verifyCertParams.at(par_name);
  }
  else
  {
    par_value = "";
  }

  return found;
}

bool RemoteSite::getIsFtpes() const noexcept
{
  return StringUtils::equalsNoCase(_protocol, ftpes);
}

bool RemoteSite::getIsFtps() const noexcept
{
  return StringUtils::equalsNoCase(_protocol, ftps);
}

bool RemoteSite::getIsFtp() const noexcept
{
  return StringUtils::equalsNoCase(_protocol, ftp);
}

bool RemoteSite::getIsAnyFtp() const noexcept
{
  return getIsFtp() || getIsFtps() || getIsFtpes();
}

bool RemoteSite::getIsFileSystem() const noexcept
{
  return StringUtils::equalsNoCase(_protocol, fileSystem) || StringUtils::equalsNoCase(_protocol, file);
}

bool RemoteSite::getIsHttp() const noexcept
{
  return StringUtils::equalsNoCase(_protocol, http) || StringUtils::equalsNoCase(_protocol, https);
}

bool RemoteSite::getIsHttps() const noexcept
{
  return StringUtils::equalsNoCase(_protocol, https);
}

void RemoteSite::setIsFtpes() noexcept
{
  _protocol = ftpes;
}

void RemoteSite::setIsFtps() noexcept
{
  _protocol = ftps;
}

void RemoteSite::setIsFtp() noexcept
{
  _protocol = ftp;
}

void RemoteSite::setPort(int port) noexcept
{
  _port = port;
}

int RemoteSite::getPort() const noexcept
{
  return _port;
}

void RemoteSite::setPath(std::string const& path)
{
  _path = StringUtils::trimEnd(path, '/');
  if (_path.empty())
  {
    _path = '/';
  }
}

const string& RemoteSite::getPath() const noexcept
{
  return _path;
}

void RemoteSite::setIsDir(bool isDir) noexcept
{
  _isDir = isDir;
}

bool RemoteSite::getIsDir() const noexcept
{
  return _isDir;
}

string RemoteSite::getRemoteDir() const
{
  return File::getDirPath(getPath(), true);
}

string RemoteSite::getRemoteFileName() const
{
  return File::getFileName(getPath());
}

bool RemoteSite::isPasswordEncrypted() const noexcept
{
  return _isPasswordEncrypted;
}

void RemoteSite::setIsPasswordEncrypted(bool isPasswordEncrypted)
{
  _isPasswordEncrypted = isPasswordEncrypted;
  // This will check if any encrypted password is good or not.
  // will throw if the password is not decryptable
  getPassword();
}

const string& RemoteSite::getEncryptedPassword() const noexcept
{
  return _password;
}

const string& RemoteSite::getTempnamevalue() const noexcept
{
  return _tempnamevalue;
}

temp::TempRenameStrategy RemoteSite::getRenameStrategy() const noexcept
{
  return _tempStrategy;
}

void RemoteSite::setRenameStrategy(temp::TempRenameStrategy s, string const& tempnamevalue)
{
  _tempStrategy = s;
  _tempnamevalue = tempnamevalue;
  initRenameStrategy();
}
void RemoteSite::setCiphers(const std::string& c)
{
  _ciphers = c;
}
const std::string& RemoteSite::getCipher() const noexcept
{
  return _ciphers;
}

bool RemoteSite::hasTemporaryTargetName() const
{
  return _tempService.hasTemporaryTargetName();
}

void RemoteSite::setProtocol(string const& protocol)
{
  const string lp = StringUtils::lowercase(protocol);
  ACS_COND_THROW(
    !(StringUtils::equals(lp, fileSystem) ||
      StringUtils::equals(lp, file) ||
      StringUtils::equals(lp, sftp) ||
      StringUtils::equals(lp, ftp) ||
      StringUtils::equals(lp, ftpes) ||
      StringUtils::equals(lp, ftps) ||
      StringUtils::equals(lp, http) ||
      StringUtils::equals(lp, https) ||
      StringUtils::equals(lp, media)),

    exIllegalValueException("Unrecognized protocol: " + protocol));
  _protocol = protocol;
}

const string& RemoteSite::getProtocol() const noexcept
{
  return _protocol;
}

bool RemoteSite::isFtpSetProtocol() const noexcept
{
  return getIsAnyFtp() || getIsSFTP();
}

std::ostream& operator<<(std::ostream& stream, const RemoteSite& obj)
{
  return stream << obj.str();
}

acs::exostream& operator<<(acs::exostream& stream, const RemoteSite& obj)
{
  stream << obj.str();
  return stream;
}

bool RemoteSite::protocolSupportsActivePassive(const std::string& protocol) noexcept
{
  const string lp = StringUtils::lowercase(protocol);
  return StringUtils::equals(lp, ftp) ||
         StringUtils::equals(lp, ftpes) ||
         StringUtils::equals(lp, ftps);
}

bool RemoteSite::protocolSupportsDirectory(const std::string& protocol) noexcept
{
  const string lp = StringUtils::lowercase(protocol);
  // http & https do not support directories
  return !StringUtils::equals(lp, http) && !StringUtils::equals(lp, https);
}

bool RemoteSite::protocolSupportsRemove(const std::string& protocol) noexcept
{
  const string lp = StringUtils::lowercase(protocol);
  // http & https do not support erasure
  return !StringUtils::equals(lp, http) && !StringUtils::equals(lp, https);
}

bool RemoteSite::protocolSupportsSource(const std::string& protocol) noexcept
{
  const string lp = StringUtils::lowercase(protocol);
  // any protocol can be used as source
  return (StringUtils::equals(lp, fileSystem) ||
          StringUtils::equals(lp, file) ||
          StringUtils::equals(lp, sftp) ||
          StringUtils::equals(lp, ftp) ||
          StringUtils::equals(lp, ftpes) ||
          StringUtils::equals(lp, ftps) ||
          StringUtils::equals(lp, http) ||
          StringUtils::equals(lp, https) ||
          StringUtils::equals(lp, media));
}

bool RemoteSite::protocolSupportsDestination(const std::string& protocol) noexcept
{
  const string lp = StringUtils::lowercase(protocol);
  // http & https do not support erasure
  return !StringUtils::equals(lp, http) && !StringUtils::equals(lp, https);
}

void RemoteSite::setCustomHeaders(const std::vector<std::string>& newCustomHeaders)
{
  _customHeaders = newCustomHeaders;
}

void RemoteSite::addCustomHeader(const std::string& newCustomHeader)
{
  _customHeaders.push_back(newCustomHeader);
}

void RemoteSite::resetCustomHeaders()
{
  _customHeaders.clear();
}

std::vector<std::string> RemoteSite::getCustomHeaders() const
{
  return _customHeaders;
}

//
// End of Getters/Setters.
//

Proxy::Proxy(const std::string& h, const std::string& pr, const std::string& u, const std::string& p, unsigned short port) :
  host_(h),
  protocol_(pr),
  user_(u),
  password_(p),
  port_(port)
{
}

const Proxy& no_proxy()
{
  static Proxy noProxy_("", "", "", "", 0);
  return noProxy_;
}

bool operator==(const Proxy& a, const Proxy& b) noexcept
{
  if (a.host() == b.host())
  {
    if (a.host().empty())
    {
      return true;
    }  // empty host on both: no_proxy
    return (a.protocol() == b.protocol()) && (a.user() == b.user()) && (a.password() == b.password()) && (a.port() == b.port());
  }
  return false;
}
_ACS_END_NAMESPACE

std::ostream& operator<<(std::ostream& os, const curl::Proxy& obj)
{
  if (obj == curl::no_proxy())
  {
    os << "[NO-PROXY]";
  }
  else
  {
    os << "[Host: " << obj.host() << " Protocol: " << obj.protocol() << " User: " << obj.user()
       << " Password: " << obj.password() << " Port: " << obj.port() << ']';
  }

  return os;
}

acs::exostream& operator<<(acs::exostream& os, const curl::Proxy& obj)
{
  if (obj == curl::no_proxy())
  {
    os << "[NO-PROXY]";
  }
  else
  {
    os << "[Host: " << obj.host() << " Protocol: " << obj.protocol() << " User: " << obj.user()
       << " Password: " << obj.password() << " Port: " << obj.port() << ']';
  }
  return os;
}

_ACS_END_NAMESPACE
