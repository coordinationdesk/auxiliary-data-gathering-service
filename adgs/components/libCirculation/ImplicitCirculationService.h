// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2019, Advanced Computer Systems , Inc.
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
 *  Revision 7.1  2017/10/19 16:18:04  marpas
 *  getting rid of EntityBean
 *
 *  Revision 7.0  2016/10/12 10:08:27  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.22  2016/03/07 15:32:23  marfav
 *  S2PDGS-1428, PDSEV-28
 *  Filename scan algorithm completely rewritten to improve the speed
 *  Stop controller managed during directory contents scan (3 passes)
 *  Error messages in case of bad repositories issued only once
 *
 *  Revision 5.21  2016/01/19 15:54:16  lucio.pulvirenti
 *  S2PDGS-1384: localPoliciesDebugId_ string member added for debug purpose.
 *
 *  Revision 5.20  2015/09/04 15:01:48  lucio.pulvirenti
 *  S2PDGS-1247: INFO for M&C issued to warn operator that request to circulate items via media occurs.
 *
 *  Revision 5.19  2015/09/03 08:30:18  damdec
 *  Bug fixed in circInfo ctr
 *
 *  Revision 5.18  2015/08/27 16:03:32  damdec
 *  Log messages added to warn operator in case of media circulation.
 *
 *  Revision 5.17  2015/08/14 16:08:11  tergem
 *  APF-273: IN messages
 *
 *  Revision 5.16  2015/03/10 11:35:51  marfav
 *  S2PDGS-1110 Added the function to manage local and remote repositories in different controlling threads
 *
 *  Revision 5.15  2014/09/05 14:51:49  lucio.pulvirenti
 *  S1PDGS-31437: insertIntoCirculationQueue IF changed: returning bool if record inserted and added OUT strings msg to params.
 *
 *  Revision 5.14  2014/06/25 18:04:36  marpas
 *  adopting libDataConsumer Rev_5_23
 *  coding best practices application in progress
 *
 *  Revision 5.13  2014/06/16 14:45:54  lucio.pulvirenti
 *  S2PDGS-764: findHostToFilter: boolean parameter added to log exception.
 *
 *  Revision 5.12  2014/04/14 15:28:59  marfav
 *  In case of exception in the ItemService thread force a rescan of the affected local path
 *
 *  Revision 5.11  2014/04/14 12:58:00  lucio.pulvirenti
 *  stopImmediateService method added to IF.
 *  StopController ptr added to ItemService CTOR IF and class IF.
 *
 *  Revision 5.10  2014/04/11 17:21:28  lucio.pulvirenti
 *  Work in progress.
 *
 *  Revision 5.9  2014/03/11 17:42:56  marfav
 *  Added support to date based filtering
 *
 *  Revision 5.8  2014/02/04 15:33:24  lucio.pulvirenti
 *  removeIfNotMatch temporary removed: waiting for next version for definitive deletion.
 *
 *  Revision 5.7  2014/01/31 12:09:59  marpas
 *  Interface rationalization and robustness improved
 *
 *  Revision 5.6  2014/01/30 17:34:55  lucio.pulvirenti
 *  When inserting circulation queue from circulation policies, transaction performed on the same file instances and not on the whole set of policies.
 *
 *  Revision 5.5  2013/10/28 17:33:52  marpas
 *  work in progress
 *
 *  Revision 5.4  2013/07/23 17:35:01  marpas
 *  interface rationalization
 *
 *  Revision 5.3  2013/07/23 17:14:04  marpas
 *  type fixed
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
 *  Revision 5.0  2013/06/07 15:49:00  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.36  2013/04/08 16:29:08  micmaz
 *  corrected errors.
 *
 *  Revision 1.35  2013/04/05 09:51:06  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.34  2013/03/14 15:56:14  marpas
 *  dbConnectionPool concept enforced
 *
 *  Revision 1.33  2012/11/29 16:31:16  micmaz
 *  work in progress
 *
 *  Revision 1.32  2012/11/28 18:42:55  micmaz
 *  IN PROGRESS - issue S1PDGS-30117: Duplicate insertion in table t_circulationsqueue by the CirculationService
 *  http://jira.acsys.it/browse/S1PDGS-30117
 *
 *  Revision 1.31  2012/11/28 15:06:33  micmaz
 *  IN PROGRESS - issue S1PDGS-30117: Duplicate insertion in table t_circulationsqueue by the CirculationService
 *  http://jira.acsys.it/browse/S1PDGS-30117
 *
 *  Revision 1.30  2012/03/30 12:50:19  micmaz
 *  work in progress
 *
 *  Revision 1.29  2012/03/27 10:54:16  micmaz
 *  work in progress
 *
 *  Revision 1.28  2012/01/20 19:32:40  micmaz
 *  IN PROGRESS - issue S1PDGS-2734: Implicit Circulation: wrong management of local data
 *  http://jira.acsys.it/browse/S1PDGS-2734
 *
 *  Revision 1.27  2011/11/14 10:46:20  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.26  2011/11/10 15:59:03  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.25  2011/11/03 16:02:27  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.24  2011/11/02 16:45:40  micmaz
 *  findLocalHost() must not return an object, instead it must return a pointer to avoid useless copies.
 *
 *  Revision 1.23  2011/10/27 14:11:52  micmaz
 *  RESOLVED - issue S1PDGS-2312: CirculationService log illeggibile
 *  http://jira.acsys.it/browse/S1PDGS-2312
 *
 *  Revision 1.22  2011/09/16 15:50:55  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.21  2011/09/09 09:36:10  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.20  2011/08/09 11:26:26  micmaz
 *  IN PROGRESS - issue S1PDGS-1784: Remove the DataSource class in order to use the new getter/setter ConnectionPool in the libApp.
 *  http://jira.acsys.it/browse/S1PDGS-1784
 *
 *  Revision 1.19  2011/07/19 14:16:42  danalt
 *  removed reference to ApplicationStatus.h
 *
 *  Revision 1.18  2011/07/14 12:22:09  micmaz
 *  removed unused code.
 *
 *  Revision 1.17  2011/07/13 17:06:03  micmaz
 *  REOPENED - issue S1PDGS-1655: CirculationAgent maxNumberOfRetries problem
 *  http://jira.acsys.it/browse/S1PDGS-1655
 *
 *  Revision 1.15  2011/07/12 14:30:04  micmaz
 *  RESOLVED - issue S1PDGS-1655: CirculationAgent maxNumberOfRetries problem
 *  http://jira.acsys.it/browse/S1PDGS-1655
 *
 *  Revision 1.14  2011/07/12 12:45:19  micmaz
 *  RESOLVED - issue S1PDGS-1627: CirculationService problem in implicit mode
 *  http://jira.acsys.it/browse/S1PDGS-1627
 *
 *  Revision 1.13  2011/07/06 16:54:36  micmaz
 *  the implicit circolation service is now managed with smart pointers.
 *  When the application will stop, the main thread waits for the implicit circulation thread.
 *
 *  Revision 1.12  2011/07/06 14:55:27  micmaz
 *  IN PROGRESS - issue S1PDGS-1607: CirculationService destination URL Problem
 *  http://jira.acsys.it/browse/S1PDGS-1607
 *
 *  Revision 1.11  2011/07/01 16:41:20  micmaz
 *  changed the temporary file name strategy.
 *
 *  Revision 1.10  2011/06/28 17:27:20  micmaz
 *  wrong makefile
 *
 *  Revision 1.9  2011/06/28 17:04:59  micmaz
 *  Added the application status in order to verify when a policy must be reprocessed
 *
 *  Revision 1.1  2011/06/16 12:50:30  micmaz
 *  work in progress.
 *
 *
 *
 *
 */

#ifndef _ImplicitCirculationService_H_
#define _ImplicitCirculationService_H_

#include <acs_c++config.hpp>
#include <CirculationDBService.h>
#include <CirculationPolicy.h>
#include <KnownHost.h>
#include <FtpListItem.h>
#include <RemoteSite.h>


#include <Thread.h>


_ACS_BEGIN_NAMESPACE(acs)
class dbConnectionPool ;
class dbConnection ;


_ACS_END_NAMESPACE

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation) ;

class ImplicitCirculationService: public Thread { // PRQA S 2109
public:
    explicit ImplicitCirculationService(dbConnectionPool& connectionPool, bool enableFilter, bool localPolicies);
    virtual ~ImplicitCirculationService() throw ();

    void setApplicationName(std::string const& appName);
    void setApplicationVersion(std::string const& appVersion);
    void operator()(bool runForever = true);
    void makeSourcePath(CirculationPolicy const& policy, curl::RemoteSite& sourceSite) const ;
    void stopService();
    void stopImmediateService();
    virtual void run();

#ifdef ACS_TEST
public:
#else
private:
#endif
    void findCirculationPolicies(KnownHost const& hostToFilter, 
                                 std::vector<CirculationPolicy> & policies,
                                 bool filterWithApplicationStatus = false) const;
    KnownHost* findHostToFilter(bool logException) const;
	
	void checkWaitingCircQueueViaMedia( KnownHost const& hostToFilter ) const;
	void queryWaitingCircQueueViaMedia( KnownHost const& hostToFilter, std::map< std::string, size_t > & ) const;

private:
    ImplicitCirculationService(); // not implemented
    ImplicitCirculationService(ImplicitCirculationService const& other); // not implemented
    ImplicitCirculationService& operator=(ImplicitCirculationService const& other); // not implemented

private:
    CirculationDBService manager_;
    dbConnectionPool& connectionPool_;
    size_t policiesFound_;
    static const size_t millisecondsToWait_;
    static const time_t lastModificationTimeInSecs_;
    
    std::map < std::string, time_t> localPathScans_;


    // The following settings are used to create specialized threads
    // managing either local policies or remote policies
    bool enableFilter_;
    bool localPolicies_;
	std::string localPoliciesDebugId_;

#ifdef ACS_TEST
public:
#else
private:
#endif
    class ItemService: public Thread { // PRQA S 2109
    public:
        ItemService(dbConnectionPool &pool,
            CirculationPolicy const& policy, 
            curl::RemoteSite const& sourceSite, 
            CirculationDBService* manager,
            std::string const& sourcePath,
            bool filterByDate,
            const time_t&,
			StopController *) ;
            
        virtual ~ItemService() throw ();
        std::string getIds() const;
    public:
        virtual void run();
        void addPolicy(CirculationPolicy const& policy);
        const std::string &getSourcePath() const throw() { return sourcePath_; }
		void setLastRunLocalFiles( const std::set<std::string> & );
		void getCurrentRunLocalFiles( std::set<std::string> & ) const ;


#ifdef ACS_TEST
    public:
#else
    private:
#endif
        /**
         * Checks if the current source file/dir was already created by the policyId and it was still not completed.
         * @param policyId: the policy id;
         * @param fromURL: the source file/dir;
         * @return true if the file/dir must not be re-circulated.
         */
        bool alreadyCreated(curl::FtpListItem const& item, 
							dbPersistent::IdType policyId, 
							std::string const& fromURL, 
							std::string const& destinationURL,
							bool & allowRecordsInsertionForPolicy,
							dbPersistent::IdType hostId ) const;
                            
        void findCirculationPolicies(KnownHost const& hostToFilter, 
                                     std::vector<CirculationPolicy> & policies,
                                     bool filterWithApplicationStatus = true) const;
                                     
        void retrieveSourceFiles(std::vector<curl::FtpListItem> &sourceFiles) const;
        
        void makeSource(curl::FtpListItem const& item, curl::RemoteSite & site) const;
        
        void makeDestination(CirculationPolicy const& policy, 
                             curl::FtpListItem const& item, 
                             curl::RemoteSite & destination) const;
                             
        bool insertIntoCirculationQueue(dbConnection& conn, 
                                        curl::FtpListItem const& item, 
                                        CirculationPolicy const& policy, 
                                        curl::RemoteSite const& source, 
                                        curl::RemoteSite const& destination,
                                        bool removeSource,
										std::string & detectionMsg,
										std::string & insertionMsg,
										dbPersistent::IdType &,
										bool & allowRecordsInsertionForPolicy) const;
        void retrieveSourceLocalFiles(std::vector<curl::FtpListItem>& sourceFiles) const;
                                       
        bool matchFType( const curl::FtpListItem &file, const std::string &ftype, dbConnection &conn) ; 

    private:
        ItemService(); // not implemented
        ItemService(ItemService const& other);
        ItemService& operator=(ItemService const& other);
    private:
			
		// Converts a vector of FtpListItem to a map of vectors using the filename as key
		void accumulateListItems (std::vector <curl::FtpListItem> const&, std::map < std::string, std::vector <curl::FtpListItem> > &) const;

		// Detects stable files
		bool isItemStable (std::vector < curl::FtpListItem> const &) const;

    private:
        dbConnectionPool &pool_ ;
        std::vector<CirculationPolicy> policies_;
        CirculationDBService* manager_;
        curl::RemoteSite sourceSite_;
        std::string sourcePath_;
        bool filterByDate_;
        const time_t lastScan_;
		StopController *stopController_ ;
		std::set<std::string> lastRunLocalFiles_ ;
		std::set<std::string> currentRunLocalFiles_;

        
    private:
        ACS_CLASS_DECLARE_DEBUG_LEVEL(ItemService) ;
    };
private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(ImplicitCirculationService) ;

};


_ACS_END_NESTED_NAMESPACE;

#endif /* _ImplicitCirculationService_H_*/
