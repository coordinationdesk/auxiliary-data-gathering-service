// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2017, Advanced Computer Systems , Inc.
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
 *  Revision 7.4  2017/10/25 13:57:54  marpas
 *  getting rid of ThreadGroup and using Thread class' range functions
 *
 *  Revision 7.3  2017/10/19 16:18:04  marpas
 *  getting rid of EntityBean
 *
 *  Revision 7.2  2017/02/06 08:53:44  lucio.pulvirenti
 *  S2PDGS-1656: private methods manageInterruptedCirculations and fillInterruptedCirculationsFromChannel added. _interruptedCirculations attribute added.
 *
 *  Revision 7.1  2017/02/03 15:49:11  lucio.pulvirenti
 *  S2PDGS-1656: work in progress.
 *
 *  Revision 7.0  2016/10/12 10:08:26  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.2  2016/10/12 10:03:37  lucio.pulvirenti
 *  CSGACQ-86: _nextRetryDate attribute removed.
 *
 *  Revision 6.1  2016/08/31 08:39:27  damdec
 *  PDSEV-24: getManager method added.
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.9  2015/11/06 14:49:31  marfav
 *  In case host id is nullable in the queue, select also records with null host id
 *
 *  Revision 5.8  2015/07/21 10:46:23  damdec
 *  PDSEV-7: implementation finalized.
 *
 *  Revision 5.7  2015/07/16 15:26:57  damdec
 *  PDSEV-7: Selection loop efficiency improved.
 *
 *  Revision 5.6  2014/01/10 17:37:20  marfav
 *  S1PDGS-31349 Support of NextRetryDate id circulations queue dev in progress
 *
 *  Revision 5.5  2014/01/10 16:04:28  marfav
 *  Configuration management implementation completed
 *
 *  Revision 5.4  2013/10/28 17:33:52  marpas
 *  work in progress
 *
 *  Revision 5.3  2013/09/12 07:39:33  lucio.pulvirenti
 *  LoggingConvenience: log period now the same restitued from DataConsumerConfigurer class and configurable.
 *
 *  Revision 5.2  2013/08/08 10:48:07  lucio.pulvirenti
 *  static const _maxInitAttempts added.
 *
 *  Revision 5.1  2013/07/15 17:21:11  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.0  2013/06/07 15:48:58  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.31  2013/04/17 21:37:08  marpas
 *  messaging improved
 *
 *  Revision 1.30  2013/04/05 09:51:06  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.29  2013/03/21 20:16:22  marpas
 *  if localhost is not found, Circulation continue running in degraded state: ApplicationStatus is Critical
 *
 *  Revision 1.28  2013/03/14 15:56:14  marpas
 *  dbConnectionPool concept enforced
 *
 *  Revision 1.27  2013/02/19 11:33:15  micmaz
 *  added db broadcast listener support
 *
 *  Revision 1.26  2012/10/25 10:17:47  micmaz
 *  The removeSource flag must be read when the circulation has been completed.
 *
 *  Revision 1.25  2012/03/30 12:50:19  micmaz
 *  work in progress
 *
 *  Revision 1.24  2012/03/01 11:40:36  micmaz
 *  refactory Data circulator
 *
 *  Revision 1.23  2012/02/17 11:20:32  marpas
 *  namespaces fixed
 *  tests dependencies fixed
 *
 *  Revision 1.22  2012/02/16 16:00:12  micmaz
 *  IN PROGRESS - issue S3PDGS-518: Gestione thread paralleli di DC su base ricevitore
 *  http://jira.acsys.it/browse/S3PDGS-518
 *
 *  Revision 1.21  2012/02/07 10:25:28  micmaz
 *  removed warnings.
 *
 *  Revision 1.20  2012/01/17 16:50:52  micmaz
 *  OPEN - issue S3PDGS-542: DC problem at explicit request
 *  http://jira.acsys.it/browse/S3PDGS-542
 *
 *  Revision 1.19  2011/10/28 16:41:18  micmaz
 *  IN PROGRESS - issue S1PDGS-2342: The Circulation Service does not update the end date in case of error and the start date.
 *  http://jira.acsys.it/browse/S1PDGS-2342
 *
 *  Revision 1.18  2011/09/20 16:58:27  micmaz
 *  work in progress
 *
 *  Revision 1.17  2011/09/19 17:16:13  micmaz
 *  updated circulations  sort.
 *
 *  Revision 1.16  2011/09/19 15:43:26  micmaz
 *  OPEN - issue S1PDGS-1910: Wrong status changing in case of RECOVERABLE_ERROR
 *  http://jira.acsys.it/browse/S1PDGS-1910
 *
 *  Revision 1.15  2011/09/16 15:50:55  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.14  2011/09/13 16:30:42  micmaz
 *  RESOLVED - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *  IN PROGRESS - issue S1PDGS-1781: Problems with loading of the Configuration Parameters
 *  http://jira.acsys.it/browse/S1PDGS-1781
 *
 *  Revision 1.13  2011/09/09 09:36:10  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.12  2011/09/05 10:05:11  micmaz
 *  the application workspace now stops if the circulation consumer cannot correctly initialize.
 *
 *  Revision 1.11  2011/08/31 14:18:27  micmaz
 *  RESOLVED - issue S1PDGS-1805: the CirculationAgent does not create a new consumer quickly when a single consumer finished.
 *  http://jira.acsys.it/browse/S1PDGS-1805
 *
 *  Revision 1.10  2011/08/09 11:26:26  micmaz
 *  IN PROGRESS - issue S1PDGS-1784: Remove the DataSource class in order to use the new getter/setter ConnectionPool in the libApp.
 *  http://jira.acsys.it/browse/S1PDGS-1784
 *
 *  Revision 1.9  2011/06/09 17:04:53  micmaz
 *  work in progress...
 *
 *  Revision 1.8  2011/06/09 15:38:16  micmaz
 *  work in progress
 *
 *  Revision 1.7  2011/06/07 17:20:32  micmaz
 *  work in progress
 *
 *  Revision 1.6  2011/04/29 17:22:14  micmaz
 *  if cannot create a log file, redirect to stderr
 *
 *  Revision 1.5  2011/04/27 13:49:26  micmaz
 *  work in progress...
 *
 *  Revision 1.4  2011/04/27 12:46:17  micmaz
 *  corrected the stop procedure
 *
 *  Revision 1.3  2011/04/12 18:00:29  micmaz
 *  work in progress...
 *
 *  Revision 1.2  2011/04/07 15:02:41  micmaz
 *  work in progress...
 *
 *  Revision 1.1  2011/04/05 17:14:24  micmaz
 *  work in progress...
 *
 *
 *
 *
 */

#ifndef _CirculationConsumer_H_
#define _CirculationConsumer_H_

#include <acs_c++config.hpp>
#include <KnownHost.h>
#include <CirculationDBService.h>
#include <CirculationRegister.h>
#include <SyncPriorityConsumer.h>
#include <Timer.h>


_ACS_BEGIN_NAMESPACE(acs)
class dbConnectionPool ;
_ACS_END_NAMESPACE

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation) ;

class CirculationConsumer: public Thread { // PRQA S 2109
public:
    /**
     * Ctor with number of consumers.
     */
    explicit CirculationConsumer(dbConnectionPool & connectionPool, unsigned int numberOfConsumers = 0);
public:

    /**
     * Empty dtor.
     */
    virtual ~CirculationConsumer() throw ();

    unsigned int maxNumberOfConsumers() const;

    /**
     * Returns the number of items that are being consuming.
     */
    size_t sizeOfConsuming() const;
    void getConsumingItems(std::vector<CirculationItem*> & items) ;

    /**
     * Retrieves the reports: success items and errored items.
     */
    void getFinishedItems(std::vector<CirculationItem>& distributedPackages, std::vector<CirculationItem>& failedPackages);

    void discardFinishedItems();

    void verifyConfigurationFile();

    /**
     * Starts the consumer(s).
     * It executes to the following steps: <P>
     * <ul>
     * <li> verifies if the configuration file is present and correctly set.
     * <li> cleans up the records found in T_CIRCULATIONS table with dirty values (f.e.: PROCESSING status and assigned to this host);
     * <li> finds in the T_CIRCULATIONS table the items to be processed, and link them to this host;
     * <li> starts up all the consumers;
     * <li> terminats all when a TERM or SIGIN signal arrives.
     * </ul>
     */
    virtual void run();

    void joinAll(bool syncStart = true);
    /**
     * Stops the consumer.
     */
    void stopImmediate();

    /**
     * Terminates the consumer.
     */
    void stopGracefully();
    /**
     * Alias for stopGracefully() : from outside "stop" is intended as "do not accept
     * more packages but terminate all the remaining queue".
     *
     */
    virtual void stop(bool joinAlso = true);

    size_t threadsAlive() const;

    typedef boost::shared_ptr<Circulation> PtrCirculation;

    bool isAvailable() const throw() ;
    void setIsAvailable(bool) throw() ;
    const std::string &getLastErrorMessage() const throw() ;
    void setLastErrorMessage(std::string const&) ;
    void dbBroadcastSignalReceived() throw() ;
#ifdef ACS_TEST
public:
#else
protected:
#endif
    void init();
    /**
     * Returns the number of items that must be consumed.
     */
    size_t size();

    typedef acs::pc::SyncPriorityConsumer<CirculationItem> SyncCirculationConsumer;
    void terminateAll();
#ifdef ACS_TEST
public:
#else
private:
#endif
    void findCirculationsToConsume(std::vector<Circulation> & circs);
    bool fillInCirculations();
    void addCirculationFromRow(Circulation& );
    void updateDbMaxRetryConfiguration ();
    dbPersistent::IdType getDbParamId (dbConnection& conn, const std::string& param) const;
    void setDbParamId (dbConnection& conn, const dbPersistent::IdType& , const std::string& param, unsigned int value) const;
    void checkCirculationsQueueFields();
	void manageInterruptedCirculations();
	void fillInterruptedCirculationsFromChannel();

private:
    CirculationConsumer() ; // not implemented
    CirculationConsumer(CirculationConsumer const& ); // not implemented
    CirculationConsumer& operator=(CirculationConsumer const& ); // not implemented

    class LoggingConvenience { // PRQA S 2109
    public:
        LoggingConvenience() : freeConsumerT(), workingConsumerT(), waitingCandidateJobsT() 
        {
            freeConsumerT.start() ;
            workingConsumerT.start() ;
            waitingCandidateJobsT.start() ;
        }
        ~LoggingConvenience() throw () {} 

        bool isFreeConsumerTime() { // PRQA S 4020
            freeConsumerT.stop() ; 
            if (freeConsumerT.elapsed() > (conf::dcConfigurerSingleton::instance()->getLogInfoPeriod()*1000)) { // PRQA S 3084, 4400
                freeConsumerT.start() ;
                return true ;
            }
            
            return false ;
        } 

        bool isWorkingConsumerTime() {  // PRQA S 4020
            workingConsumerT.stop() ; 
            if (workingConsumerT.elapsed() > (conf::dcConfigurerSingleton::instance()->getLogInfoPeriod()*1000)) { // PRQA S 3084, 4400
                workingConsumerT.start() ;
                return true ;
            }
            
            return false ;
        } 

        bool isWaitingCandidateJobsTime() {  // PRQA S 4020
            waitingCandidateJobsT.stop() ; 
            if (waitingCandidateJobsT.elapsed() > (conf::dcConfigurerSingleton::instance()->getLogInfoPeriod()*1000)) {  // PRQA S 3084, 4400
                waitingCandidateJobsT.start() ;
                return true ;
            }
            
            return false ;
        } 
    public:
// to remove in next version!
        static const Timer::Delay freeConsumerTMinInterval_ms = 11000 ; // PRQA S 2100, 4400 3
        static const Timer::Delay workingConsumerTMinInterval_ms = 17000 ;
        static const Timer::Delay waitingCandidateJobsTMinInterval_ms = 13000 ;
    private:
        LoggingConvenience(const LoggingConvenience &) ; // not implemented
        LoggingConvenience& operator= (const LoggingConvenience &) ; // not implemented
    private:
        Timer freeConsumerT ;
        Timer workingConsumerT ;
        Timer waitingCandidateJobsT ;
    } ;

#ifdef ACS_TEST
public:
#else
private:
#endif
    CirculationDBService _manager;
    CirculationRegister _dataConsumerChannel;
    unsigned int _nConsumer;
    std::vector<Thread *> _group;
    bool _isAvailable;
    mutable Mutex _mutex;
    KnownHost _localhost;
    dbConnectionPool  &_connectionPool;
    bool _waitTime;
    acs::DateTime _lastTimeFillInCirculations;
    std::string _errorMessage;
    LoggingConvenience _logconv ;
	bool _hostIdNullable;
    
    std::map<int, std::pair<int, int> > _slotResource;	//key: dest_host_id, value: pair: max_upload_slots, progress_count
	std::list<Circulation> _interruptedCirculations;
	
	static int const _maxInitAttempts = 10; // the maximum number of attempts to retry init in run method
private:
	void initSlotResource();
    ACS_CLASS_DECLARE_DEBUG_LEVEL(CirculationConsumer) ;
};

_ACS_END_NESTED_NAMESPACE;

#endif /* _CirculationConsumer_H_ */
