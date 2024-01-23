// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2013, Advanced Computer Systems , Inc.
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
 *  Revision 5.2  2013/10/28 13:42:14  marpas
 *  cleaning macros and curl versions.
 *
 *  Revision 5.1  2013/10/22 17:31:50  marpas
 *  work in progress
 *
 *  Revision 5.0  2013/06/06 18:08:20  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.5  2013/05/29 10:53:58  marpas
 *  interface rationalization,
 *  coding best practices applied,
 *  qa rules
 *
 *  Revision 2.4  2013/01/15 11:22:17  micmaz
 *  work in progress
 *
 *  Revision 2.3  2012/11/29 16:28:20  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.2  2012/05/30 17:36:28  micmaz
 *  added local and remote chmod
 *
 *  Revision 2.1  2012/02/07 10:25:47  micmaz
 *  removed warnings .
 *
 *  Revision 2.0  2011/10/18 14:43:50  marpas
 *  new statistics adopted
 *
 *  Revision 1.7  2011/05/11 13:52:47  micmaz
 *  corrected the rollback for put and get operations
 *
 *  Revision 1.6  2011/04/29 17:20:41  micmaz
 *  check style changes
 *
 *  Revision 1.5  2011/02/08 17:44:53  micmaz
 *  work in progress...
 *
 *  Revision 1.4  2011/02/04 11:31:46  micmaz
 *  work in progress...
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

#ifndef _CurlTransaction_H_
#define _CurlTransaction_H_

#include <exException.h>
#include <deque>
#include <utility>

#include <curl/curl.h> // PRQA S 1013

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) 


enum TransactionType {
    TrUnset = 0, 
    TrOther, 
    TrGet, 
    TrPut, 
    TrList, 
    TrRemove, 
    TrRename, 
    TrChmod,
    TrLocalRename, 
    TrLocalChmod,
    TrMkDir,
    TrHTTPPOST,
    TrHTTPGET,
    TrHTTPPROPFIND
} ;

class Curl;
/**
 * A simple transaction implementation.
 *
 * This class is responsible to open and close the connection to the remote site using the acs::curl::Curl
 * class.
 */
class CurlTransaction { // PRQA S 2109
public:
    /**
     * This class can be constructed only using the acs::curl::Curl reference.
     */
    explicit CurlTransaction(Curl& curl);
    /**
     * Closes the cURL connection (if it is still opened).
     */
    ~CurlTransaction() throw();
private:
    CurlTransaction();
    CurlTransaction(CurlTransaction const& rhs);
    CurlTransaction& operator=(CurlTransaction const& rhs);
public:

    /**
     * Create a new CURL handle using the internal acs::curl::Curl object.
     */
    CURL* begin();

    /**
     * Closes the cURL connection.
     */
    void close(); // PRQA S 2502

    /**
     * Rollbacks the current transaction.
     */
    void rollback();

    /**
     * This method must be called before an operation will be executed.
     * The purpose of this method is to close/reopen the cURL connection if the
     * user is making different operations in the current transaction.
     * If you want to make some puts and some gets in a single transaction, the
     * cURL connection must be reset (i.e. closed and re-opened) to avoid crashes.
     * If in the current connection we are always making the same operation, this method does
     * nothing.
     * @param operationType: the type of this operation.
     */
    void prepareTransaction(TransactionType operationType);

    /**
     * Logs the last operation just executed.
     * In transaction mode, if an error would occur, this log will used to make rollback.
     * @
     */
    void logTransaction(TransactionType operationType, std::vector<std::string>& args, CURLcode& responseCode);
    void logTransaction(TransactionType operationType, const std::string &remoteFilename, CURLcode& responseCode);
    /**
     * Reconnect to the ftp/sftp remote site closing and opening a new cURL connection handle.
     */
    void reconnect();

private:
    Curl& _curl;
    typedef std::pair<int, std::string> FtpOperation;

    std::deque<FtpOperation> _partialPuts;
    std::deque<FtpOperation> _partialGets;
    bool _isRollingBack;
    bool _isOpened;
    TransactionType _transactionType;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(CurlTransaction)    ;

};


_ACS_END_NESTED_NAMESPACE 

#endif /* _CurlTransaction_H_ */
