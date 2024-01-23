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
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.4  2014/06/04 17:29:21  marfav
 *  QA warning removed
 *  HTTP sessions managed without SSL layer
 *  CURL locking callback functions strategy revised
 *
 *  Revision 5.3  2013/10/28 13:42:14  marpas
 *  cleaning macros and curl versions.
 *
 *  Revision 5.2  2013/10/24 18:27:59  marpas
 *  work in progress
 *
 *  Revision 5.1  2013/10/22 17:31:50  marpas
 *  work in progress
 *
 *  Revision 5.0  2013/06/06 18:08:20  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.3  2012/12/07 13:53:45  micmaz
 *  correct a core dump on put rollback
 *
 *  Revision 2.2  2012/11/29 16:28:21  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.1  2012/02/07 10:25:47  micmaz
 *  removed warnings .
 *
 *  Revision 2.0  2011/10/18 14:43:50  marpas
 *  new statistics adopted
 *
 *  Revision 1.9  2011/08/12 14:49:04  micmaz
 *  work in progress..
 *
 *  Revision 1.8  2011/05/11 13:52:46  micmaz
 *  corrected the rollback for put and get operations
 *
 *  Revision 1.7  2011/04/29 17:20:41  micmaz
 *  check style changes
 *
 *  Revision 1.6  2011/03/10 16:34:38  micmaz
 *  added new time out parameters. improved logs on failures.
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

#include "CurlTransaction.h"

#include<Curl.h>
#include<CurlConfigurer.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) 


using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL( CurlTransaction)

CurlTransaction::CurlTransaction(Curl& curl) :
    _curl(curl), // PRQA S 2528
    _partialPuts(),
    _partialGets(),
    _isRollingBack(false), 
    _isOpened(false), 
    _transactionType(TrUnset) 
{
    _curl.setTransaction(this);
}

CurlTransaction::~CurlTransaction() throw() 
{
    // PRQA S 4631 L1
    try {
        close();
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    // PRQA L:L1
}

void CurlTransaction::close() // PRQA S 4020
{
    //close a transaction : if it was not already opened, do nothing.
    if (!_isOpened) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Warning: cURL Connection already closed.");
        return;
    }
    //close the connection to the remote site, reset the transaction handle, clear the intermediate operations.
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Close Transaction.");
    _curl.closeConnection();
    _curl.setTransaction(0);
    _partialGets.clear();
    _partialPuts.clear();
    _isOpened = _isRollingBack = false;
    _transactionType = TrUnset;
}

CURL* CurlTransaction::begin() {
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    //start a new transaction (if already started exit with an exception).
    ACS_COND_THROW(_isOpened, Curl::CurlException("cURL transaction already opened.")); // PRQA S 3081
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Begin Transaction.");
    _isOpened = true;
    _curl.setTransaction(this);
    return _curl.openConnection();
}

void CurlTransaction::reconnect() {
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Reconnect to the server.");
    _curl.closeConnection();
    _curl.openConnection();
}

void CurlTransaction::prepareTransaction(TransactionType operationType) {
    //prepare a new transaction
    if (_transactionType == TrUnset) {
        _transactionType = operationType;
    } 
    else {
        //if we want to make a different operation on the same connection,
        //in order to keep cURL work, we must close and reopen a different connection.
        if (_transactionType != operationType) {
            reconnect();
            _transactionType = operationType;
        }
        //if we are making too many operations on the same connection, close and open a new one
        //otherwise an error will occur.
        if ((_partialPuts.size() + _partialGets.size()) % conf::CurlConfigurerSingleton::instance()->getMaxFilesToTransfer() == 0) // PRQA S 3084
        {
            reconnect();
        }
    }
}

void CurlTransaction::logTransaction(TransactionType operationType, const std::string &remoteFilename, CURLcode& responseCode) {
    vector < string > args;
    args.push_back(remoteFilename);
    logTransaction(operationType, args, responseCode);
}

void CurlTransaction::logTransaction(TransactionType operationType, std::vector<std::string>& args, CURLcode& responseCode) { // PRQA S 4020
    //log the operations executed on the same connection.
    ACS_COND_THROW(args.empty(), Curl::CurlException("At least one argument should be passed to the transaction logger!")); // PRQA S 3081
    std::string remoteFile = args.front();
    ACS_COND_THROW(not _isOpened, Curl::CurlException("cURL transaction not opened.")); // PRQA S 3081
    if (responseCode != 0) { return ; }
    
    
    switch(operationType) {
    case TrGet:
        {
            _partialGets.push_back(FtpOperation(TrGet, remoteFile));
            break ;
        } 
    case TrPut:
        {
            _partialPuts.push_back(FtpOperation(TrPut, remoteFile));
            break ;
        } 
    case TrRename:
        {
            //if the remote rename operation was made after an ftp put, then it is enough to
            //rollback the put operation with the renamed target file name.
            if (not _partialPuts.empty()) {
                FtpOperation lastPutOperation = _partialPuts.back();
                if (lastPutOperation.first == TrPut) {
                    string putFile = lastPutOperation.second;
                    //if we made a rename of a temporary file , then we remove only the final file name .
                    if (StringUtils::equals(putFile, args.back())) {
                        _partialPuts.pop_back();
                        _partialPuts.push_back(FtpOperation(TrPut, remoteFile));
                    }
                }
            }
            break ;
        } 
    case TrLocalRename:
        {
            //if the local rename operation was made after an ftp get, then it is enough to
            //rollback the get operation with the renamed target file name.
            if (not _partialGets.empty()) {
                FtpOperation lastGetOperation = _partialGets.back();
                if (lastGetOperation.first == TrGet) {
                    string getFile = lastGetOperation.second;
                    //if we made a rename of a temporary file , then we remove only the final file name .
                    if (StringUtils::equals(getFile, args.back())) {
                        _partialGets.pop_back();
                        _partialGets.push_back(FtpOperation(TrGet, remoteFile));
                    }
                }
            }
            break ;
        }
    default:
        { break ; /* nothing */ }
    }
}

void CurlTransaction::rollback() // PRQA S 4020
{
    //rollback a transaction: if it was not opened, exit and ignore.
    if (!_isOpened) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Warning: cURL Connection already closed.");
        return;
    }
    //close and open a new transaction.
    reconnect();
    _isRollingBack = true;
    //for every puts correctly done, delete the remote file.
    while (!_partialPuts.empty()) {
        FtpOperation op = _partialPuts.front();
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Rollback on: remote file " << op.second);
        Curl curl(_curl.getSite());
        curl.del(op.second);
        _partialPuts.pop_front();
    }
    //for every gets correctly done, delete the local file.
    while (!_partialGets.empty()) {
        FtpOperation op = _partialGets.front();
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Rollback on: local file " << op.second);
        Curl curl(_curl.getSite());
        curl.ldel(op.second);
        _partialGets.pop_front();
    }
    //close this new connection.
    close();
}

_ACS_END_NESTED_NAMESPACE 
