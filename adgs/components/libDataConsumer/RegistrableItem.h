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
 *  $Prod: A.C.S.libDataConsumer Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.16  2017/10/04 08:25:43  chicas
 *  message improvement
 *
 *  Revision 5.15  2017/02/03 15:29:51  lucio.pulvirenti
 *  S2PDGS-1656: error code initialized to NOT STARTED: set to SUCCESS_TRANSFER if all ok. If dbConnectionException caught, no error code is set.
 *
 *  Revision 5.14  2017/01/31 16:11:17  lucio.pulvirenti
 *  S2PDGS-1656: work in progress.
 *
 *  Revision 5.13  2016/02/09 14:42:08  lucio.pulvirenti
 *  S2PDGS-1412: consume(): if generic exception caught, ACS_LOG_NOTIFY_EX macro is called only if ConsumableItem::getLogExMsgAsError returns true.
 *
 *  Revision 5.12  2015/11/16 16:24:43  lucio.pulvirenti
 *  S2PDGS-1373: logError methos added to discriminate messages to be logged as ACS_LOG_INFO or ACS_LOG_ERROR.
 *
 *  Revision 5.11  2014/08/06 16:25:42  lucio.pulvirenti
 *  S2PDGS-835: Exceptions notificated by the proper macro not to send stack on syslog.
 *  S2PDGS-836: If specific exception caught no need to emit description. FtpCurlInitializationFailedException  and FtpCurlLocalWriteErrorException catch added.
 *
 *  Revision 5.10  2014/06/25 17:29:46  marpas
 *  Logger.C and Logger.h removed
 *
 *  Revision 5.9  2014/06/24 10:58:34  marpas
 *  useless macros eviction in progress
 *
 *  Revision 5.8  2014/06/18 13:45:03  lucio.pulvirenti
 *  S2PDGS-726: StopException managed.
 *
 *  Revision 5.7  2014/06/05 18:47:34  marpas
 *  getting rid of asserts
 *
 *  Revision 5.6  2014/04/15 16:18:38  marpas
 *  bad macro epuration in progress
 *
 *  Revision 5.5  2014/02/14 13:54:46  marpas
 *  attempt to refactor band shaping mechanism
 *
 *  Revision 5.4  2013/10/28 17:33:29  marpas
 *  work in progress
 *
 *  Revision 5.3  2013/07/22 13:18:15  marpas
 *  types and best practices fixed
 *
 *  Revision 5.2  2013/07/18 12:29:08  marpas
 *  coding best practices applied
 *  type mismatching fix in progress
 *
 *  Revision 5.1  2013/07/16 14:52:48  marpas
 *  coding best practices applied
 *  robustness and performances improved
 *
 *  Revision 5.0  2013/06/06 20:55:21  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.18  2013/04/05 09:47:16  marpas
 *  libDCUtility interface change
 *  flow control changed
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.17  2012/03/30 12:50:28  micmaz
 *  work in progress
 *
 *  Revision 1.16  2012/02/07 10:25:56  micmaz
 *  removed warnings.
 *
 *  Revision 1.15  2012/01/24 13:33:53  micmaz
 *  IN PROGRESS - issue S1PDGS-2740: Wrong Management of the FTPS Protocol
 *  http://jira.acsys.it/browse/S1PDGS-2740
 *
 *  Revision 1.14  2011/09/23 09:14:42  micmaz
 *  removed unused headers.
 *
 *  Revision 1.13  2011/09/13 16:31:05  micmaz
 *  IN PROGRESS - issue S1PDGS-1781: Problems with loading of the Configuration Parameters
 *  http://jira.acsys.it/browse/S1PDGS-1781
 *
 *  Revision 1.12  2011/05/09 14:39:36  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.11  2011/05/03 14:03:40  micmaz
 *  internal exceptions thrown with error number (where it is possible) and retrow original exception and not the catched one.
 *
 *  Revision 1.10  2011/04/15 15:33:20  micmaz
 *  work in progress...
 *
 *  Revision 1.9  2011/04/11 12:24:53  micmaz
 *  work in progress
 *
 *  Revision 1.8  2011/04/05 17:14:44  micmaz
 *  work in progress...
 *
 *  Revision 1.7  2011/03/15 17:26:49  micmaz
 *  implemented TERM signal
 *
 *  Revision 1.6  2011/03/10 16:34:09  micmaz
 *  added new paramters for connection and db  time out.
 *  Improved log in case of failure
 *
 *  Revision 1.5  2011/03/03 14:28:27  micmaz
 *  work in progress
 *
 *  Revision 1.4  2011/01/21 18:06:25  micmaz
 *  data consumer rework.
 *
 *  Revision 1.3  2010/12/17 10:29:20  micmaz
 *  Added MCF messages.
 *
 *  Revision 1.2  2010/12/03 14:23:12  micmaz
 *  The Registers now use the Stopper token in order to synch stopping / terminating events.
 *
 *  Revision 1.1  2010/11/30 16:41:49  micmaz
 *  added mail warning.
 *
 *
 *
 */

#ifndef REGISTRABLEITEM_H_
#define REGISTRABLEITEM_H_

#include <DataConsumerConfigurer.h>
#include <Indexed.h>
#include <FtpCurl.h>
#include <exStream.h>


_ACS_BEGIN_NESTED_NAMESPACE(acs, dc)

/**
 * @brief DataConsumerItem: is the item (created from the Packages) used from the DataConsumer in order to transfer files.
 * This class is a wrapper of the Item class,i.e. this is the indexed wrapper of the Item class.
 */
template<typename Item, typename KeyType>
class RegistrableItem: public acs::pc::Indexed<boost::shared_ptr<Item>, KeyType> {
public:
    /**
     * Empty ctor.
     */
    RegistrableItem() :
        acs::pc::Indexed<boost::shared_ptr<Item>, KeyType>(),
        _attempts(0), 
        _maxAttempts(3), // PRQA S 4400
        _errorCode(conf::NOT_STARTED), // PRQA S 1021
        _ex(),
        _lastAttempt(time(0))
    {

    }

    /**
     * Copy Ctor.
     */
    RegistrableItem(RegistrableItem const& rhl) : 
        acs::pc::Indexed<boost::shared_ptr<Item>, KeyType>(rhl),
        _attempts(rhl._attempts),
        _maxAttempts(rhl._maxAttempts),
        _errorCode(rhl._errorCode),
        _ex(rhl._ex),
        _lastAttempt(rhl._lastAttempt)
    {
        // empty
    }
    /**
     * Copy Assigner.
     */
    RegistrableItem& operator=(RegistrableItem const& rhl) 
    {
        if (this != &rhl) {
            acs::pc::Indexed<boost::shared_ptr<Item>, KeyType>::operator =(rhl);
            _attempts = rhl._attempts;
            _maxAttempts = rhl._maxAttempts;
            _errorCode = rhl._errorCode;
            _ex = rhl._ex;
            _lastAttempt = rhl._lastAttempt;
        }
        return *this;
    }
    virtual ~RegistrableItem() throw() {} // PRQA S 2131
	
	
    virtual std::string str() const {  return pItem()->str(); } // PRQA S 2131

    virtual std::string id() const { return pItem()->id(); }  // PRQA S 2131

    virtual std::string groupId() const { return pItem()->groupId(); }  // PRQA S 2131

    virtual int getPriority() const { return pItem()->getPriority(); } // PRQA S 2131

    virtual void setLastException(std::string const& ex) { _ex = ex; } // PRQA S 2131

    virtual std::string getLastException() const { return _ex; } // PRQA S 2131

    /**
     * Setter for the priority of this Item
     */
    void setPriority(int priority) {
        pItem()->setPriority(priority);
    }

    void reset(Item* item) {
        this->_item.reset(item);
    }

    /**
     * Getter for the Item.
     */
    Item* pItem() const {
        return this->_item.get();
    }

    /**
     * Consume the product.
     */
    virtual void consume() {
		
        try {
            ++_attempts;
            pItem()->doWork();
            _lastAttempt = time(0);
			setLastErrorCode(conf::SUCCESS_TRANSFER); // PRQA S 1021
        } catch (acs::FtpCurl::FtpCurlRemoteWriteErrorException const& ex) {
            setLastException(ex.what());
            setLastErrorCode(conf::ERROR_REMOTE_FILE_SYSTEM_ERROR); // PRQA S 1021
            _lastAttempt = time(0);
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, ex.what());
			std::string errMsg = groupId() + ". Access denied on remote file";
			logError(errMsg);
            throw;
        } catch (acs::FtpCurl::FtpCurlHostnameNotFoundException const& ex) {
            setLastException(ex.what());
            setLastErrorCode(conf::ERROR_HOST_UNKNOWN); // PRQA S 1021
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, ex.what());
            std::string errMsg = groupId() + ". Host name cannot be resolved";
			logError(errMsg);
            throw;
        } catch (acs::FtpCurl::FtpCurlConnectionFailedException const& ex) {
            setLastException(ex.what());
            setLastErrorCode(CURLE_COULDNT_CONNECT); // PRQA S 1021
 			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, ex.what());
          	std::string errMsg = groupId() + ". Connection failed." ;
			logError(errMsg);
           throw;
        } catch (acs::FtpCurl::FtpCurlInitializationFailedException const& ex) {
            setLastException(ex.what());
            setLastErrorCode(CURLE_FAILED_INIT); // PRQA S 1021
 			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, ex.what());
          	std::string errMsg = groupId() + ". Initialization failed. (curl error CURLE_FAILED_INIT) ";
			logError(errMsg);
           throw;
        } catch (acs::FtpCurl::FtpCurlLoginDeniedException const& ex) {
            setLastException(ex.what());
            setLastErrorCode(conf::ERROR_LOGIN); // PRQA S 1021
            _lastAttempt = time(0);
 			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, ex.what());
            std::string errMsg = groupId() + ". Login denied" ;
			logError(errMsg);
            throw;
        } catch (acs::FtpCurl::FtpCurlLocalWriteErrorException const& ex) {
            setLastException(ex.what());
            setLastErrorCode(CURLE_WRITE_ERROR); // PRQA S 1021
            _lastAttempt = time(0);
 			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, ex.what());
            std::string errMsg = groupId() + ". Write error on local file system" ;
			logError(errMsg);
            throw;
        } catch (ConsumableItem::StopException const& ex) {
            setLastException(ex.what());
            if (ex.errorNumber()) {
                setLastErrorCode(ex.errorNumber());
            } else {
                setLastErrorCode(conf::ERROR_UNKNOWN); // PRQA S 1021
            }
 			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, ex.what());
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, groupId() << ". Process interrupted due to stop signal." );
            throw;
        } catch (dbConnection::ConnectionException const& ex) {
            setLastException(ex.what());
			
 			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, ex.what());
			std::string errMsg = groupId() +  ". db Connection error occurred" ;
			logError(errMsg);
            throw;
        } catch (exException const& ex) {
            setLastException(ex.what());
            if (ex.errorNumber()) {
                setLastErrorCode(ex.errorNumber());
            } else {
                setLastErrorCode(conf::ERROR_UNKNOWN); // PRQA S 1021
            }
			
			std::string errMsg = groupId() +  ". Transferring error occurred" ;
			try {
				if( pItem()->getLogExMsgAsError() ) {
 					ACS_LOG_NOTIFY_EX(ex);
				}
				else {
					errMsg += (std::string(": ") + ex.what());
				}
			}
			catch( std::exception & )
			{
				errMsg += (std::string(": ") + ex.what());
			}
			logError(errMsg);
            throw;
        } catch (std::exception const& ex) {
            setLastException(ex.what());
            setLastErrorCode(conf::ERROR_UNKNOWN); // PRQA S 1021
			std::string errMsg = groupId() +  ". Transferring error occurred" ;
			try {
				if( pItem()->getLogExMsgAsError() ) {
 					ACS_LOG_NOTIFY_EX(ex);
				}
				else {
					errMsg += (std::string(": ") + ex.what());
				}
			}
			catch( std::exception & )
			{
				errMsg += (std::string(": ") + ex.what());
			}
			logError(errMsg);
             throw;
        }
    }

    /**
     * Getter for the attempts used to transfer the file.
     */
    size_t attempts() const {
        return _attempts;
    }

    /**
     * Getter for the last error code catched.
     */
    int lastErrorCode() const {
        return _errorCode;
    }

    /**
     * Setter for the last error code catched.
     */
    void setLastErrorCode(int errorCode) {
        _errorCode = errorCode;
        _lastAttempt = time(0);
    }

    /**
     * Setter for the maximum number of attempts to use.
     */
    void setMaxAttempts(unsigned int maxAttempts) {
        _maxAttempts = maxAttempts;
    }

    /**
     * Returns the Item's size.
     * This value is the directory size where the file is
     */
    size_t size() const {
        return pItem()->size();
    }

    /**
     * Returns last time of attempt.
     */
    time_t lastAttemptTime() const {
        return _lastAttempt;
    }
	
	void logError ( const std::string & errorMsg ) const throw() {
		
		try {
			if( pItem()->getLogExMsgAsError() ) {
            	ACS_LOG_ERROR(errorMsg);
			}
			else {
				ACS_LOG_INFO(errorMsg);
			}
		}
		catch( std::exception & )
		{
			ACS_LOG_ERROR(errorMsg);
		}
	
	}

	
protected:
    size_t _attempts; // PRQA S 2101 5
    size_t _maxAttempts;
    int _errorCode;
    std::string _ex;
    time_t _lastAttempt;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(RegistrableItem) ;


};

ACS_TEMPLATE_DEFINE_DEBUG_LEVEL2 (RegistrableItem, Item, KeyType) ; // PRQA S 2101

_ACS_END_NESTED_NAMESPACE

#endif /* REGISTRABLEITEM_H_ */
