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
 *  Revision 7.1  2017/10/16 16:49:00  marpas
 *  getting rid of libDCUtility
 *  using db::SmartQuery and rewriting persistent objects inheriting from dbSmartPersistent
 *
 *  Revision 7.0  2016/10/12 10:08:27  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.2  2014/02/07 18:53:36  marpas
 *  adopting ACS_THROW and ACS_COND_THROW macros
 *
 *  Revision 5.1  2013/07/15 17:21:12  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.0  2013/06/07 15:49:01  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.7  2013/04/22 11:00:25  marpas
 *  refactoring due NameIdAssociator and related classes using Singleton template pattern
 *
 *  Revision 1.6  2013/04/05 09:51:07  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.5  2013/03/14 15:56:14  marpas
 *  dbConnectionPool concept enforced
 *
 *  Revision 1.4  2013/02/13 17:06:30  micmaz
 *  executed regression tests.
 *
 *  Revision 1.3  2012/04/04 09:34:08  micmaz
 *  work in progress
 *
 *  Revision 1.2  2012/04/03 17:45:47  micmaz
 *  work in progress
 *
 *  Revision 1.1  2012/04/03 10:11:06  micmaz
 *  work in progress
 *
 *
 */

#include "MediaUpdater.h"
#include <dbConnectionPool.h>
#include <StringUtils.h>
#include <CirculationRemoveService.h>
#include <CirculationDBService.h>
#include <FtpCurl.h>
#include <RemoteSite.h>
#include <RemoteSiteUtility.h>
#include <map>


_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation) ;

using namespace std;

using namespace acs::db;
using namespace acs::curl;
using namespace acs::circulation;

ACS_CLASS_DEFINE_DEBUG_LEVEL(MediaUpdater);

MediaUpdater::MediaUpdater() :
    _circs(), 
    _service() 
{
    // empty
}

MediaUpdater::~MediaUpdater() throw () {
    // empty
}

bool MediaUpdater::update(dbConnection &conn, vector<SignedFile> const& files) {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Update " <<files.size() << " files");
    map<long, bool> results;
    for(vector<SignedFile>::const_iterator iter = files.begin(); iter != files.end(); ++iter) {
        int circId = atoi(iter->getCirculationId().c_str());
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Circ id " <<circId);
        map<long, bool>::iterator foundIter =results.find(circId);
        if (foundIter == results.end() || foundIter->second) {
            bool checked = checkFile(*iter);
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Checked " <<checked);
            results.insert(make_pair(circId, checked));
        }
    }
    for(map<long, bool>::iterator iter = results.begin(); iter != results.end(); ++iter) { // PRQA S 4238
        bool isValid = iter->second;
        Circulation* circ = findCirculation(conn, iter->first);
        
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "update for : " <<circ->getId());
        KnownHost* localhost =_service.findLocalhost(conn);
        bool null = false ;
        if (localhost->getId() != circ->getHostId(null,conn)) {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The host id specified in the circulation is not this localhost: " << localhost->str(conn, true));
            ostringstream message;
            message << "Circulation to update has not been circulated on this host: "+ circ->getId();
            ACS_THROW(MediaUpdaterException(message.str())); // PRQA S 3081
        }
        if (isValid) {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Set Status to completed");
            circ->setStatus(Status::instance(conn.getKey())->id(Status::completed), conn);
        } else {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Set Status to error");
            circ->setStatus(Status::instance(conn.getKey())->id(Status::recoverableError), conn);
        }
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Update: " << circ->str(conn, true));
        circ->update(conn);
        if (circ->getStatus(null, conn) == Status::instance(conn.getKey())->id(Status::completed)) {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Remove the sources if needed: " << circ->getRemoveSource(null, conn));
            if (circ->getRemoveSource(null, conn)) {
                ACS_LOG_INFO("Adding source  " << circ->getFromURL(null, conn) << " to remove queue.");
                RemoteSite sourceSite = RemoteSiteFactory::makeSite(circ->getFromURL(null, conn), true);
                RemoteResourceType rr = RemoteSiteUtility::probeSite(&sourceSite);
                if (rr == RRTNotExist) {
                    ACS_LOG_WARNING("The remote resource does not exist!");
                } else {
                    StopController stopper;
                    CirculationRemoveService removeService ;
                    removeService.removeSource(conn, *circ, sourceSite, stopper);
                }
            } else {
                ACS_LOG_INFO("The source must not be removed: " << circ->getFromURL(null, conn));
            }
        }
    }
    return true;
}

bool MediaUpdater::checkFile(SignedFile const& file) {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[fillInResultsWithSignedFile: " << file << "]");
    ACS_COND_THROW(StringUtils::isBlank(file.getVerificationStatus()), // PRQA S 3081
            MediaUpdaterException("The report file has not been validated."));
    bool isValid = (StringUtils::equalsNoCase(StringUtils::trim(file.getVerificationStatus()), SignedFile::verificationOk)) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Is valid:" << isValid);
    return isValid;
}

Circulation* MediaUpdater::findCirculation(dbConnection &conn, long circulationId) {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[updateWithCirculationId: " << circulationId << "]");
    boost::shared_ptr<Circulation> circ(new Circulation(conn));
    try {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Finding the circulation on db.." << circulationId);
        if (not circ->load(conn, circulationId)) {
            ostringstream os ; os << "Cannot load Circulation id: " << circulationId ;
            ACS_THROW(Circulation::ExecutionError(os.str())) ; // PRQA S 3081
        }
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Circ to Update: " << circ.get()->str(conn,true));
    } catch (std::exception const& ex) {
        ACS_LOG_ERROR("Cannot find the circulation on db");
        ostringstream message;
        message << "Circulation to update not found on db with id: " << circulationId;
        ACS_THROW(MediaUpdaterException(message.str())); // PRQA S 3081
    }
    _circs.insert(make_pair(circ.get()->getId(), circ));
    return circ.get();
}



_ACS_END_NESTED_NAMESPACE;
