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
 *  Revision 7.5  2017/10/19 16:18:04  marpas
 *  getting rid of EntityBean
 *
 *  Revision 7.4  2017/10/17 15:56:09  marpas
 *  getting rid of EntityBeam and rewriting all based on smart persistent
 *
 *  Revision 7.3  2017/10/16 16:49:00  marpas
 *  getting rid of libDCUtility
 *  using db::SmartQuery and rewriting persistent objects inheriting from dbSmartPersistent
 *
 *  Revision 7.2  2017/02/06 08:39:57  lucio.pulvirenti
 *  S2PDGS-1656: updateErrorStatus: boolean added (default true) to throw exception when updating record columns other than status.
 *
 *  Revision 7.1  2017/02/03 15:49:11  lucio.pulvirenti
 *  S2PDGS-1656: work in progress.
 *
 *  Revision 7.0  2016/10/12 10:08:26  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.1  2016/08/31 08:42:58  damdec
 *  PDSEV-24: Method fillPendingCirculation added.
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.11  2015/11/16 16:29:35  lucio.pulvirenti
 *  S2PDGS-1373: updateErrorStatus IF changed with addition of an output parameter (bool ptr) added to return if recoverable error hase been set.
 *
 *  Revision 5.10  2015/07/24 15:40:48  marfav
 *  S2PDGS-1247 Taking into accont the destination protocol id if available
 *
 *  Revision 5.9  2015/03/10 15:12:40  lucio.pulvirenti
 *  S2PDGS-984: updateErrorStatus: added a boolean parameter to IF to set possibly given t_circulationsqueue record to ERROR status (before it was always
 *  put to RECOVERABLEERROR.
 *
 *  Revision 5.8  2015/03/06 16:46:19  lucio.pulvirenti
 *  S2PDGS-984: wokr in progress.
 *
 *  Revision 5.7  2014/06/16 14:37:04  lucio.pulvirenti
 *  S2PDGS-764: findLocalhost: boolean paramaters added to IF to reload t_knownhost table every time and not to log exception.
 *
 *  Revision 5.6  2014/01/13 16:15:53  marfav
 *  S1PDGS-31349 Implementation completed
 *
 *  Revision 5.5  2013/11/21 14:49:59  marpas
 *  fixing debug variable and messages
 *
 *  Revision 5.4  2013/07/23 17:16:03  marpas
 *  getting rid of boost datetime in favour of DateTime
 *
 *  Revision 5.3  2013/07/22 13:19:18  marpas
 *  tyoes fixing
 *  coding best practices applied
 *  test fixed
 *
 *  Revision 5.2  2013/07/16 14:52:16  marpas
 *  coding best practices applied,
 *  performances and robustness improved
 *
 *  Revision 5.1  2013/07/15 17:21:11  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.0  2013/06/07 15:48:58  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.42  2013/04/22 11:00:24  marpas
 *  refactoring due NameIdAssociator and related classes using Singleton template pattern
 *
 *  Revision 1.41  2013/04/05 09:51:06  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.40  2013/03/14 15:56:14  marpas
 *  dbConnectionPool concept enforced
 *
 *  Revision 1.39  2012/11/29 16:55:05  micmaz
 *  work in progress
 *
 *  Revision 1.38  2012/11/28 12:36:42  micmaz
 *  IN PROGRESS - issue S1PDGS-30117: Duplicate insertion in table t_circulationsqueue by the CirculationService
 *  http://jira.acsys.it/browse/S1PDGS-30117
 *
 *  Revision 1.37  2012/11/27 15:24:33  micmaz
 *  IN PROGRESS - issue S1PDGS-30117: Duplicate insertion in table t_circulationsqueue by the CirculationService
 *  http://jira.acsys.it/browse/S1PDGS-30117
 *
 *  Revision 1.36  2012/11/14 10:32:17  micmaz
 *  now when a circualation reach  the final status , it will be updated in one shot.
 *
 *  Revision 1.35  2012/03/27 10:54:15  micmaz
 *  work in progress
 *
 *  Revision 1.34  2012/02/17 11:20:32  marpas
 *  namespaces fixed
 *  tests dependencies fixed
 *
 *  Revision 1.33  2012/02/07 10:25:28  micmaz
 *  removed warnings.
 *
 *  Revision 1.32  2012/02/01 16:05:00  micmaz
 *  removed loading local host from db.
 *
 *  Revision 1.31  2012/01/30 17:58:55  micmaz
 *  IN PROGRESS - issue S1PDGS-2736: CirculationService hangs in case of multiple policies installed on the same circulation node and using local baskets
 *  http://jira.acsys.it/browse/S1PDGS-2736
 *
 *  Revision 1.30  2012/01/20 15:58:41  micmaz
 *  OPEN - issue S1PDGS-2685: Circulation Agent Core in transferring large files
 *  http://jira.acsys.it/browse/S1PDGS-2685
 *
 *  Revision 1.29  2012/01/11 16:45:31  micmaz
 *  IN PROGRESS - issue S1PDGS-2685: Circulation Agent Core in transferring large files
 *  http://jira.acsys.it/browse/S1PDGS-2685
 *
 *  Revision 1.28  2011/11/03 16:02:27  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.27  2011/11/02 16:45:40  micmaz
 *  findLocalHost() must not return an object, instead it must return a pointer to avoid useless copies.
 *
 *  Revision 1.26  2011/10/28 16:41:18  micmaz
 *  IN PROGRESS - issue S1PDGS-2342: The Circulation Service does not update the end date in case of error and the start date.
 *  http://jira.acsys.it/browse/S1PDGS-2342
 *
 *  Revision 1.25  2011/10/28 13:13:24  micmaz
 *  IN PROGRESS - issue S1PDGS-2274: PolicyGenerator Resource not found
 *  http://jira.acsys.it/browse/S1PDGS-2274
 *
 *  Revision 1.24  2011/10/27 14:41:16  micmaz
 *  IN PROGRESS - issue S1PDGS-2276: CirculationService problema nella gestione di un dato gia' trasferito
 *  http://jira.acsys.it/browse/S1PDGS-2276
 *
 *  Revision 1.23  2011/09/19 15:43:26  micmaz
 *  OPEN - issue S1PDGS-1910: Wrong status changing in case of RECOVERABLE_ERROR
 *  http://jira.acsys.it/browse/S1PDGS-1910
 *
 *  Revision 1.22  2011/09/08 09:26:50  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.21  2011/08/31 14:18:27  micmaz
 *  RESOLVED - issue S1PDGS-1805: the CirculationAgent does not create a new consumer quickly when a single consumer finished.
 *  http://jira.acsys.it/browse/S1PDGS-1805
 *
 *  Revision 1.20  2011/08/09 11:26:26  micmaz
 *  IN PROGRESS - issue S1PDGS-1784: Remove the DataSource class in order to use the new getter/setter ConnectionPool in the libApp.
 *  http://jira.acsys.it/browse/S1PDGS-1784
 *
 *  Revision 1.19  2011/07/22 09:00:57  micmaz
 *  work in progress..
 *
 *  Revision 1.18  2011/07/21 17:44:40  micmaz
 *  updated Workspace, GeneralApplication and Entities.
 *
 *  Revision 1.17  2011/07/15 17:10:20  micmaz
 *  work in progress
 *
 *  Revision 1.16  2011/07/13 18:04:08  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.15  2011/07/13 17:06:03  micmaz
 *  REOPENED - issue S1PDGS-1655: CirculationAgent maxNumberOfRetries problem
 *  http://jira.acsys.it/browse/S1PDGS-1655
 *
 *  Revision 1.14  2011/07/12 17:22:33  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.13  2011/07/12 14:30:04  micmaz
 *  RESOLVED - issue S1PDGS-1655: CirculationAgent maxNumberOfRetries problem
 *  http://jira.acsys.it/browse/S1PDGS-1655
 *
 *  Revision 1.12  2011/06/22 17:14:10  micmaz
 *  work in progress
 *
 *  Revision 1.11  2011/06/20 16:01:25  micmaz
 *  work in progress.
 *
 *  Revision 1.10  2011/06/16 12:49:45  micmaz
 *  work in progress..
 *
 *  Revision 1.9  2011/06/14 15:02:29  micmaz
 *  correct date arithmetic
 *
 *  Revision 1.8  2011/06/10 16:26:52  micmaz
 *  work in progress..
 *
 *  Revision 1.7  2011/06/10 13:47:09  micmaz
 *  work in progress
 *
 *  Revision 1.6  2011/06/09 17:04:53  micmaz
 *  work in progress...
 *
 *  Revision 1.5  2011/06/09 15:38:16  micmaz
 *  work in progress
 *
 *  Revision 1.4  2011/06/07 17:20:32  micmaz
 *  work in progress
 *
 *  Revision 1.3  2011/04/12 18:00:29  micmaz
 *  work in progress...
 *
 *  Revision 1.2  2011/04/11 12:25:16  micmaz
 *  work in progress
 *
 *  Revision 1.1  2011/04/08 17:00:52  micmaz
 *  work in progress...
 *
 *
 *
 */

#ifndef _CirculationDbService_H_
#define _CirculationDbService_H_

#include <acs_c++config.hpp>
#include <ImplicitCirculationConfigurer.h>
#include <Status.h>
#include <KnownHost.h>
#include <SecurityZoneRelay.h>
#include <Circulation.h>
#include <exException.h>

#include <boost/smart_ptr.hpp> // PRQA S 1013

#include <vector>
#include <string>

_ACS_BEGIN_NAMESPACE(acs) ;
class dbConnection ;
_ACS_END_NAMESPACE;

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation) ;


exDECLARE_EXCEPTION(Exception,exException); // PRQA S 2131, 2502

class CirculationDBService { // PRQA S 2109
public:
    CirculationDBService();
    ~CirculationDBService() throw();
private:
    CirculationDBService(CirculationDBService const& other);
    CirculationDBService& operator=(CirculationDBService const& other);

public:

    /**
     * Creates a new query to find CirculationQueue items that were already created by a policy.
     * @param fromURL: the source url;
     * @param toURL: the target url;
     * @param stati: the std::vector of all stati;
     * @return the smart query.
     */
    void findAlreadyCreatedQueues(
		CirculationQuery &, dbConnection &conn, std::string const& fromURL, std::string const& toURL, 
		std::vector<int> const& stati, dbPersistent::IdType hostId ) const;
    
	void findDuplicatedQueues(
		CirculationQuery &, dbConnection &conn, std::string const& fromURL, 
		std::string const& toURL, dbPersistent::IdType hostId) const;
		
    KnownHost* findLocalhost(dbConnection &, bool reload = false, bool logException = true) const;
    size_t cleanupRecords(dbConnection &, size_t maxItems=0);
    void updateCirculationStatus(dbConnection &, std::string const& statusName, Circulation & circ) const;
    /**
     * Put t_circulationsqueue record status to ERROR/RECOVERABLEERROR. If status cannot be changed exception is always thrown.
     * @param emitExc: if false, exception in updating parameters other than status is not thrown.
     */
    void updateErrorStatus(dbConnection &, Circulation&, bool recoverableError = true, bool* recoverableErrorSet = 0, bool emitExcAfterUpdate = true) const;
    void updateToEndStatus(dbConnection &, std::string const& finalStatus, Circulation& circ) const;

    bool isOlderThan(time_t seconds, acs::DateTime &);
    /**
     * Get all status but not COMPLETED or ERRORED (not ended stati);
     * @param stati: out parameter where all status ids are stored.
     */
    std::vector<int> getNotEndStati(const std::string &dbkey) const;
    /**
     * Prepare the new circulation to be processed.
     * @param item: the circulation to be processed.
     */
    void startWorkingOnCirculation(dbConnection &, Circulation & item);

    bool checkHasNextRetryField (dbConnection&) const;
	bool checkHasProtocolIdField (dbConnection&) const;

    // Retrieve the current date (database time) 
    DateTime getCurrentDbDate (dbConnection &) const;

    // Compute next retry and update record on database
    void updateNextRetryDate (dbConnection &, const Circulation& circ) const;

    // Retry algo computation functions (can be moved to plugin/factory pattern)
    unsigned long linearRetryOffset (dbConnection&, const DateTime& currentDbTime, 
        const conf::ImplicitCirculationConfigurer::RetryConfiguration& conf, const Circulation& circ) const;

    unsigned long exp2LimitedRetryOffset (dbConnection&, const DateTime& currentDbTime, 
        const conf::ImplicitCirculationConfigurer::RetryConfiguration& conf, const Circulation& circ) const;

    unsigned long steppedRetryOffset (dbConnection&, const DateTime& currentDbTime, 
        const conf::ImplicitCirculationConfigurer::RetryConfiguration& conf, const Circulation& circ) const;

#ifdef ACS_TEST
public:
#else
private:
#endif
    size_t fillInInterruptedCirculations(dbConnection &, std::vector<Circulation> & results, size_t const maxItems = 0);

#ifdef ACS_TEST
public:
#else
private:
#endif
    acs::ThreadSafe _mutex;
    acs::DateTime _lastTimeFillInCirculations;
    mutable boost::shared_ptr<KnownHost> _localhost;

private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(CirculationDBService)

};

_ACS_END_NESTED_NAMESPACE;


#endif /* _CirculationDbService_H_ */
