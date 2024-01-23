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
 *  Revision 7.0  2016/10/12 10:08:26  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.4  2016/02/03 15:36:28  damdec
 *  removeURL is now public. isToRemove public method added.
 *
 *  Revision 5.3  2013/11/21 14:49:14  marpas
 *  fixing debug variable
 *
 *  Revision 5.2  2013/07/18 12:29:51  marpas
 *  coding best practices application in progress
 *  type mismatch fix in progress
 *
 *  Revision 5.1  2013/07/15 17:21:11  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.0  2013/06/07 15:48:59  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.14  2013/04/05 09:51:06  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.13  2013/03/14 15:56:14  marpas
 *  dbConnectionPool concept enforced
 *
 *  Revision 1.12  2013/01/18 13:23:24  micmaz
 *  removed race conditions on remove queue.
 *
 *  Revision 1.11  2012/04/03 17:45:48  micmaz
 *  work in progress
 *
 *  Revision 1.10  2012/03/30 12:50:19  micmaz
 *  work in progress
 *
 *  Revision 1.9  2012/03/19 11:35:29  micmaz
 *  work in progress
 *
 *  Revision 1.8  2012/02/20 15:11:10  marpas
 *  refactoring
 *
 *  Revision 1.7  2012/02/07 10:25:28  micmaz
 *  removed warnings.
 *
 *  Revision 1.6  2012/01/27 20:44:52  micmaz
 *  work in progress..
 *
 *  Revision 1.5  2012/01/24 17:02:15  micmaz
 *  IN PROGRESS - issue S1PDGS-2734: Implicit Circulation: wrong management of local data
 *  http://jira.acsys.it/browse/S1PDGS-2734
 *
 *  Revision 1.4  2011/09/09 15:13:47  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.3  2011/09/08 09:26:50  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.2  2011/09/06 09:39:02  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.1  2011/09/05 16:16:41  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *
 *
 */

#ifndef _CirculationRemoveService_H_
#define _CirculationRemoveService_H_

#include <acs_c++config.hpp>
#include <CirculationRemoveQueue.h>
#include <ImplicitCirculationConfigurer.h>
#include <CirculationDBService.h>
#include <RemoteSite.h>

#include <boost/ptr_container/ptr_vector.hpp> // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)
class StopController ;
_ACS_END_NAMESPACE

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation) ;

class CirculationRemoveService: public ThreadSafe { // PRQA S 2109, 2153
public:
    CirculationRemoveService();
    virtual ~CirculationRemoveService() throw() ;
private:
    CirculationRemoveService(CirculationRemoveService const& other);
    CirculationRemoveService& operator=(CirculationRemoveService const& other);
public:
    void removeSource(dbConnection &, Circulation const& circ, curl::RemoteSite const& sourceSite, StopController& stopper);
    void removeSourceAfterComplete(dbConnection &, Circulation const& circ, curl::RemoteSite const& sourceSite,
            StopController& stopper) ;
    void removeURL(dbConnection&, curl::RemoteSite const& site, std::string const& url, StopController& stopper) throw();
    void removeURL(dbConnection&, curl::RemoteSite const& site, std::string const& url, StopController& stopper, bool rethrow);
    bool isToRemove(dbConnection &conn, std::string const& url, dbPersistent::IdType circulationId);
    
    inline void setEnqueueOnFailure(bool v) { enqueueOnFailure_ = v; }
    inline void setUnlinkFailureAction(acs::conf::ImplicitCirculationConfigurer::UnlinkFailureAction action) { unlinkFailureAction_ = action; }
    inline void setUnlinkFailureRenameStrategy(acs::conf::ImplicitCirculationConfigurer::UnlinkFailureRenameStrategy strat) { unlinkFailureRenameStrategy_ = strat; }
    inline void setLocalUnlinkFailureTrashPath(const std::string& trashPath) { localUnlinkFailureTrashPath_ = trashPath; }
    
#ifdef ACS_TEST
public:
#else
private:
#endif
    dbPersistent::IdType getHostId(dbConnection& conn) const;
    void addToFailedRemoveQueue(dbConnection& conn, const std::string& remoteUrl);
    void addToRemoveQueue(dbConnection &, std::string const& remoteUrl);
    void remove(dbConnection &, curl::RemoteSite const& site, dbPersistent::IdType circulationId, StopController& stopper); // PRQA S 2502
    /**
     * tests if an url is removable or not. the circulation id is -1 if the test must not exclude any circulation,
     * otherwise it will exclude the circulation with the circulationId key.
     * @url: the source file/dir to test if removable;
     * @circulationId: the circulation to exclude from this test, -1 if no circulations must be excluded.
     * @return true if the url can be deleted, false otherwise.
     */
    bool isRemovable(dbConnection &, std::string const& url,dbPersistent::IdType circulationId, std::string & outMsg);
    unsigned long findByURL(dbConnection &, std::string const& url, std::vector<CirculationRemoveQueue> & results,size_t limit=0) ;
	bool deleteByUrl(dbConnection &, std::string const& url);
    /**
     * Extract the local path to the url if the url is local, returns the url itself otherwise.
     */
    std::string extractLocalPath(std::string const& url);
    
    bool handleUnlinkFailures(const std::string& source);


#ifdef ACS_TEST
public:
#else
private:
#endif
    
    static Mutex mutex_;

    CirculationDBService manager_;
    bool enqueueOnFailure_;

    acs::conf::ImplicitCirculationConfigurer::UnlinkFailureAction unlinkFailureAction_;
    acs::conf::ImplicitCirculationConfigurer::UnlinkFailureRenameStrategy unlinkFailureRenameStrategy_;
    std::string localUnlinkFailureTrashPath_;
    
    static bool emitHostIdWarning_;
private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(CirculationRemoveService)
};

_ACS_END_NESTED_NAMESPACE;


#endif /* _CirculationRemoveService_H_ */
