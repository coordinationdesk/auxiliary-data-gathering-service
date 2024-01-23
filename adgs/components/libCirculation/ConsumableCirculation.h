// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2018, Advanced Computer Systems , Inc.
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
 *  $Prod: A.C.S. libCirculation Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 7.3  2017/10/19 16:18:04  marpas
 *  getting rid of EntityBean
 *
 *  Revision 7.2  2017/02/06 09:04:15  lucio.pulvirenti
 *  S2PDGS-1656: public method getItemToConsume added. _objectId string attribute added.
 *
 *  Revision 7.1  2017/02/03 15:49:11  lucio.pulvirenti
 *  S2PDGS-1656: work in progress.
 *
 *  Revision 7.0  2016/10/12 10:08:27  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.14  2015/11/16 16:33:21  lucio.pulvirenti
 *  S2PDGS-1373: _logExMsgAsError bool par added to IF with set/get method. In op() method check if proper flag is set to prevent log of recoverable errors
 *  as ERRORs: they will be logged ad INFO.
 *
 *  Revision 5.13  2015/08/17 13:05:46  marfav
 *  APF-273 adding messages for OUT and FAIL metrics
 *
 *  Revision 5.12  2015/03/10 15:15:24  lucio.pulvirenti
 *  S2PDGS-984: UnrecoverableErrorException defined.
 *
 *  Revision 5.11  2015/03/09 16:59:37  lucio.pulvirenti
 *  S2PDGS-984 work in progress.
 *
 *  Revision 5.10  2015/03/06 16:46:19  lucio.pulvirenti
 *  S2PDGS-984: wokr in progress.
 *
 *  Revision 5.9  2015/02/19 14:56:52  francesco.avanzi
 *  getFromNToCiphers() method added ./ConsumableCirculation.C ./ConsumableCirculation.h
 *
 *  Revision 5.8  2014/09/03 15:31:43  lucio.pulvirenti
 *  S3PDGS-1980: CannotRemoveTargetException added and verifyAndRemove IF changed: now void.
 *
 *  Revision 5.7  2014/06/24 10:58:44  marpas
 *  useless macros eviction in progress
 *
 *  Revision 5.6  2014/02/21 16:34:05  lucio.pulvirenti
 *  No more derived by BandPriority.
 *
 *  Revision 5.5  2014/02/14 14:00:51  marpas
 *  attempt to refactor band shaping mechanism
 *
 *  Revision 5.4  2013/11/21 14:49:13  marpas
 *  fixing debug variable
 *
 *  Revision 5.3  2013/10/28 17:33:52  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/07/22 13:19:18  marpas
 *  tyoes fixing
 *  coding best practices applied
 *  test fixed
 *
 *  Revision 5.1  2013/07/15 17:21:12  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.0  2013/06/07 15:48:59  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.20  2013/04/05 09:51:06  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.19  2013/03/14 15:56:14  marpas
 *  dbConnectionPool concept enforced
 *
 *  Revision 1.18  2012/04/04 15:31:36  micmaz
 *  work in progress
 *
 *  Revision 1.17  2012/03/30 12:50:19  micmaz
 *  work in progress
 *
 *  Revision 1.16  2012/03/27 10:54:16  micmaz
 *  work in progress
 *
 *  Revision 1.15  2012/03/08 13:20:45  micmaz
 *  added media functionality
 *
 *  Revision 1.14  2012/02/08 16:22:49  micmaz
 *  probe() method moved to FtpCurl class.
 *
 *  Revision 1.13  2012/02/07 10:25:29  micmaz
 *  removed warnings.
 *
 *  Revision 1.12  2012/02/02 17:19:57  micmaz
 *  IN PROGRESS - issue S1PDGS-2769: The Circulation Curl does not take into account the slow parameter
 *  http://jira.acsys.it/browse/S1PDGS-2769
 *
 *  Revision 1.11  2011/09/05 16:16:41  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.10  2011/08/09 11:26:26  micmaz
 *  IN PROGRESS - issue S1PDGS-1784: Remove the DataSource class in order to use the new getter/setter ConnectionPool in the libApp.
 *  http://jira.acsys.it/browse/S1PDGS-1784
 *
 *  Revision 1.9  2011/06/09 17:04:53  micmaz
 *  work in progress...
 *
 *  Revision 1.8  2011/06/07 17:20:32  micmaz
 *  work in progress
 *
 *  Revision 1.7  2011/04/29 17:22:14  micmaz
 *  if cannot create a log file, redirect to stderr
 *
 *  Revision 1.6  2011/04/15 18:02:54  micmaz
 *  work in progress...
 *
 *  Revision 1.5  2011/04/15 15:33:37  micmaz
 *  work in progress...
 *
 *  Revision 1.4  2011/04/14 16:32:41  micmaz
 *  work in progress...
 *
 *  Revision 1.3  2011/04/12 18:00:29  micmaz
 *  work in progress...
 *
 *  Revision 1.2  2011/04/11 12:25:16  micmaz
 *  work in progress
 *
 *  Revision 1.1  2011/04/05 17:14:24  micmaz
 *  work in progress...
 *
 *  Revision 1.4  2011/03/23 10:04:03  micmaz
 *  work in progress...
 *
 *  Revision 1.3  2011/03/11 17:55:46  micmaz
 *  reworked time outs.
 *
 *  Revision 1.2  2011/03/08 18:15:26  micmaz
 *  Added the ovewrite flag to the configurer
 *
 *  Revision 1.1  2011/01/21 18:04:34  micmaz
 *  added a copyable ftpCurl wrapper.
 *
 *
 *
 */

#ifndef ConsumableCirculation_H_
#define ConsumableCirculation_H_

#include <acs_c++config.hpp>
#include <CirculationDBService.h>
#include <FtpCurl.h>
#include <ConsumableItem.h>
#include <CirculationCurl.h>
#include <CirculationRemoveService.h>
#include <MediaSigner.h>


_ACS_BEGIN_NAMESPACE(acs) ;
class dbConnectionPool;
_ACS_END_NAMESPACE;

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation) ;

class KnownHost ;

class ConsumableCirculation: public dc::ConsumableItem { // PRQA S 2109, 2153

public:
	exDECLARE_EXCEPTION(CannotRemoveTargetException,exException); // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(UnrecoverableErrorException, FtpCurl::FtpCurlUnrecoverableErrorException); // PRQA S 2131, 2153, 2502
	exDECLARE_EXCEPTION(RecoverableErrorException, FtpCurl::FtpCurlRecoverableErrorException); // PRQA S 2131, 2153, 2502

    ConsumableCirculation(StopController& stopper, dbConnectionPool & connectionPool, Circulation const& entity);
    virtual ~ConsumableCirculation() throw();
private:
    ConsumableCirculation(); // not implemented
    ConsumableCirculation(ConsumableCirculation const& other); // not implemented
    ConsumableCirculation& operator=(ConsumableCirculation const& other); // not implemented
public:
    virtual std::string id() const;
    virtual std::string groupId() const;
    virtual std::string str() const;
    virtual int getPriority() const ;
    virtual void setPriority(int priority);
    std::string getSourceSite();
    std::string getTargetSite();
	void setLogExMsgAsError( bool v ) { _logExMsgAsError = v ; }
	bool getLogExMsgAsError() const ;
	virtual void getItemToConsume(Circulation &circ) const { circ = _itemToConsume; } // PRQA S 2131



    /**
     * Transfer the file(s) represented by the CirculationCurl class.
     */
    virtual void doWork();
	

#ifdef ACS_TEST
public:
#else
private:
#endif
    /**
     * This method includes all operations must be executed after transfer a product.
     */
    void postTransferOperations(curl::RemoteSite const& sourceSite, curl::RemoteSite const& targetSite, long long& transferSize);
    /**
     * Verify if  the current item should be transferred to the remote site, if not to be trasferred launch exception
     *
     * @param sourceSite: the source site;
     * @param targetSite: the target site;
     */
    void verifyAndRemove(curl::RemoteSite* sourceSite, curl::RemoteSite* targetSite);
    void cleanupRecords();
	void moveIntoBlockedCirculationsQueue( dbConnection &);
	
#ifdef ACS_TEST
public:
#else

private:

	void initializeObjectId();

#endif
    dbConnectionPool &pool_ ;
    curl::CirculationCurl _curl;
    CirculationDBService _manager;
    KnownHost* _localhost;
    mutable Circulation _itemToConsume;
    CirculationRemoveService _removeService;
    StopController &_stopper ;
	bool _logExMsgAsError;
	bool _gotRecoverableError;	// flag indicating recoverable error found
	std::string _objectId;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(ConsumableCirculation)
};

_ACS_END_NESTED_NAMESPACE;


#endif /* ConsumableCirculation_H_ */
