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
 *  Revision 5.4  2015/04/16 13:07:07  marpas
 *  useless method removed
 *
 *  Revision 5.3  2014/11/03 17:01:56  marpas
 *  simplyfied
 *
 *  Revision 5.2  2013/10/28 13:42:14  marpas
 *  cleaning macros and curl versions.
 *
 *  Revision 5.1  2013/10/22 17:31:50  marpas
 *  work in progress
 *
 *  Revision 5.0  2013/06/06 18:08:20  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.4  2013/05/29 10:53:58  marpas
 *  interface rationalization,
 *  coding best practices applied,
 *  qa rules
 *
 *  Revision 2.3  2012/11/29 16:28:20  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.2  2012/03/08 13:11:40  micmaz
 *  added media functionality
 *
 *  Revision 2.1  2012/02/07 10:25:46  micmaz
 *  removed warnings .
 *
 *  Revision 2.0  2011/10/18 14:43:50  marpas
 *  new statistics adopted
 *
 *  Revision 1.6  2011/04/29 17:20:41  micmaz
 *  check style changes
 *
 *  Revision 1.5  2011/02/23 14:39:25  micmaz
 *  Added: Ftpes support (but not tested) and CirculationCurl in order to make a file-transfer using this server as bridge.
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

#ifndef _CurlHeaderMaker_H_
#define _CurlHeaderMaker_H_

#include <RemoteSite.h>

#include <exException.h>

#include <curl/curl.h> // PRQA S 1013

#include <vector>

#include<sstream>
#include <string>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) 

/**
 * This class is responsible to manage the internal's cURL headers.
 *
 * cURL headers are used to make the ftp/sftp commands like get or put.
 */
class CurlHeaderMaker { // PRQA S 2109
public:
    /**
     * Constructs the object using the remote site and an already created CURL* handler.
     * @param site: the remote site to connect;
     * @param curl: the CURL handler.
     */
    CurlHeaderMaker(RemoteSite const& site, CURL* curl);
    CurlHeaderMaker(CurlHeaderMaker const& ) = delete ;
    CurlHeaderMaker& operator=(CurlHeaderMaker const& ) = delete ;

    ~CurlHeaderMaker() noexcept;

    /**
     * Makes the get command's headers.
     * @param fileToTransfer: the local filename to transfer.
	 * @param data: void* casted FILE* used for the data transfer. If not set STDOUT will be used by CURL
	 * @param quoteOnly: send pre and post quote commands avoiding the actual file transfer
     */
    void makeGet(std::string const& fileToTransfer, void* data = nullptr, bool quoteOnly = false);
    /**
     * Makes the put command's headers.
     * @param fileToTransfer: the local filename to transfer.
     * @param targetFilename: the remote new filename.
     * @param handler: the put data handler (null if it must be created by default (i.e. read a file on disk in
     * order to transfer it).
     */
    void makePut(std::string const& fileToTransfer, std::string const& targetFilename, void* handler = nullptr);

    /**
     * Adds a custom header.
     * @param: commad: the command to add in the internal headers list.
     */
    void addHeader(std::string const& command);
    
    /**
     * Adds a command will be executed before the transfer
     * @param: commad: the command to add in the pre-command list.
     */
    void addPreCommand(std::string const& command);

    /**
     * Getter for the remote URL.
     * @return the URL.
     */
    const std::string &getURL() const noexcept ;

    /**
     * Makes the remote url from the internal RemoteSite and the remoteFilename file.
     * @param remoteFilename: the remote file name used to make some operation.
     * @return the correct url string used by cURL to make some operation (f.e.: "sftp://user@remotesite//path/to/file").
     */
    std::string makeURL(std::string const& remoteFilename) const;

    /**
     * Setter for the remote site.
     * @param site: the remote site.
     */
    void setSite(RemoteSite const& site);
    /**
     * Setter for the CURL handler.
     * @param curl: the CURL handler.
     */
    void setCURL(CURL* curl);
    /**
     * Compiles the headers and set the cURL connection able to execute the command (get or put).
     */
    void makeHeader();
    /**
     * Closes the headers list (must called after the command).
     */
    void close() noexcept ; // PRQA S 2502

protected:
    std::string makeProtocol() const;

    void* makeReadData(std::string const& fileToTransfer, std::string const& targetFilename);

private:
    curl_slist* _headerList = nullptr ;
    curl_slist* _preCommandList = nullptr ;
    FILE* _fileHandler = nullptr ;
    CURLcode _res = {} ;
    std::string _url = "" ;
    RemoteSite _site;
    CURL* _curl; // NOSONAR - external type CURL

    ACS_CLASS_DECLARE_DEBUG_LEVEL(CurlHeaderMaker) ;

};

_ACS_END_NESTED_NAMESPACE 

#endif /* _CurlHeaderMaker_H_ */
