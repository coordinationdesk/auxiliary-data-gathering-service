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
 *  Revision 7.10  2018/06/06 12:29:04  lucio.pulvirenti
 *  S2PDGS-2004: messages improved.
 *
 *  Revision 7.9  2018/06/06 09:38:21  lucio.pulvirenti
 *  S2PDGS-2004: message improved.
 *
 *  Revision 7.8  2018/02/28 09:35:34  chicas
 *  message improvement
 *
 *  Revision 7.7  2018/02/27 16:44:54  lucio.pulvirenti
 *  S2PDGS-1948: warning added.
 *
 *  Revision 7.6  2018/02/27 16:10:47  lucio.pulvirenti
 *  S2PDGS-1948: extractLocalPath: RemoteSite::isLocalhost method used instead of isLocalPath (removed) class method considering
 *  file protocol too.
 *
 *  Revision 7.5  2018/01/09 09:31:47  chicas
 *  S2PDGS-1898 : added notification in case of error during local url delete
 *
 *  Revision 7.4  2017/11/15 13:55:48  chicas
 *  some message to PRIVINFO
 *
 *  Revision 7.3  2017/11/15 10:28:33  chicas
 *  some message to PRIVINFO
 *
 *  Revision 7.2  2017/10/19 16:18:04  marpas
 *  getting rid of EntityBean
 *
 *  Revision 7.1  2017/10/17 15:56:09  marpas
 *  getting rid of EntityBeam and rewriting all based on smart persistent
 *
 *  Revision 7.0  2016/10/12 10:08:26  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.13  2016/03/26 21:32:47  marpas
 *  using new db I/F
 *
 *  Revision 5.12  2016/02/03 15:36:28  damdec
 *  removeURL is now public. isToRemove public method added.
 *
 *  Revision 5.11  2014/08/06 16:09:32  lucio.pulvirenti
 *  S2PDGS-835: exceptions notificated by the proper macro not to send stack on syslog.
 *
 *  Revision 5.10  2014/06/20 14:53:41  marpas
 *  adopting FtpCurl 5.47 interface
 *
 *  Revision 5.9  2014/02/14 14:00:51  marpas
 *  attempt to refactor band shaping mechanism
 *
 *  Revision 5.8  2014/01/31 12:12:45  marpas
 *  useless warning removed
 *
 *  Revision 5.7  2014/01/28 11:02:04  lucio.pulvirenti
 *  S2PDGS-611 Messages and debug improved.
 *
 *  Revision 5.6  2014/01/27 16:15:48  lucio.pulvirenti
 *  Message and debug improved.
 *
 *  Revision 5.5  2013/11/21 14:49:38  marpas
 *  fixing debug variable
 *
 *  Revision 5.4  2013/07/23 17:15:34  marpas
 *  compilation warnings fixed
 *
 *  Revision 5.3  2013/07/18 12:29:50  marpas
 *  coding best practices application in progress
 *  type mismatch fix in progress
 *
 *  Revision 5.2  2013/07/15 17:21:11  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.1  2013/07/12 09:06:55  marpas
 *  porting circulation business intelligence into the library
 *
 *  Revision 5.0  2013/06/07 15:48:59  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.22  2013/04/22 11:00:24  marpas
 *  refactoring due NameIdAssociator and related classes using Singleton template pattern
 *
 *  Revision 1.21  2013/04/05 09:51:06  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.20  2013/03/14 15:56:14  marpas
 *  dbConnectionPool concept enforced
 *
 *  Revision 1.19  2013/01/18 13:45:29  micmaz
 *  removed race conditions on remove queue.
 *
 *  Revision 1.18  2013/01/18 13:23:24  micmaz
 *  removed race conditions on remove queue.
 *
 *  Revision 1.17  2013/01/18 12:21:51  micmaz
 *  removed race conditions on remove queue.
 *
 *  Revision 1.16  2013/01/18 11:44:01  micmaz
 *  removed race conditions on remove queue.
 *
 *  Revision 1.15  2012/04/03 17:45:48  micmaz
 *  work in progress
 *
 *  Revision 1.14  2012/03/30 12:50:19  micmaz
 *  work in progress
 *
 *  Revision 1.13  2012/03/27 10:54:16  micmaz
 *  work in progress
 *
 *  Revision 1.12  2012/03/19 11:35:29  micmaz
 *  work in progress
 *
 *  Revision 1.11  2012/02/20 15:11:10  marpas
 *  refactoring
 *
 *  Revision 1.10  2012/01/30 17:58:55  micmaz
 *  IN PROGRESS - issue S1PDGS-2736: CirculationService hangs in case of multiple policies installed on the same circulation node and using local baskets
 *  http://jira.acsys.it/browse/S1PDGS-2736
 *
 *  Revision 1.9  2012/01/27 20:44:52  micmaz
 *  work in progress..
 *
 *  Revision 1.8  2012/01/24 17:02:15  micmaz
 *  IN PROGRESS - issue S1PDGS-2734: Implicit Circulation: wrong management of local data
 *  http://jira.acsys.it/browse/S1PDGS-2734
 *
 *  Revision 1.7  2012/01/20 15:59:07  micmaz
 *  modified logs
 *
 *  Revision 1.6  2011/11/03 16:02:27  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.5  2011/09/09 15:13:47  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.4  2011/09/08 14:51:48  micmaz
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

#include <CirculationRemoveService.h>
#include <Circulation.h>
#include <dbConnectionPool.h>
#include <RemoteSiteFactory.h>
#include <RemoteSiteUtility.h>
#include <FtpCurl.h>

#include <dbQueryParameters.h>


_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation) ;
using namespace std;
using namespace acs::curl;
using namespace acs::conf;
ACS_CLASS_DEFINE_DEBUG_LEVEL (CirculationRemoveService);

Mutex CirculationRemoveService::mutex_;
bool CirculationRemoveService::emitHostIdWarning_ = true;

CirculationRemoveService::CirculationRemoveService() : 
    ThreadSafe(),
    manager_(),
    enqueueOnFailure_(true),
    unlinkFailureAction_(CirculationConfigurerSingleton::instance()->getUnlinkFailureAction()),
    unlinkFailureRenameStrategy_(CirculationConfigurerSingleton::instance()->getUnlinkFailureRenameStrategy()),
    localUnlinkFailureTrashPath_(CirculationConfigurerSingleton::instance()->getLocalUnlinkFailureTrashPath())
{
}

CirculationRemoveService::~CirculationRemoveService() throw () {
}

string CirculationRemoveService::extractLocalPath(string const& url) { // PRQA S 4020

	string returnedUrl = url;
	RemoteSite site = RemoteSiteFactory::makeSite(url);
#warning "Local path cannot start with localhost"
// Old code isLocalPath method foresaw that an URL started with localhost:
// return ( StringUtils::startsWith(url, "/") || StringUtils::startsWith(url, RemoteSite::fileSystem)
//			|| StringUtils::startsWith(url, RemoteSite::localhost))
// but it is not correct since localhost occurs ONLY IF url starts with protocol (e.g. file://localhost/home/lucpul/prova).
	if( site.isLocalhost() ) {
		returnedUrl = site.getPath();
	}
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Returning \"" << returnedUrl << '\"');
	return returnedUrl;
}

dbPersistent::IdType CirculationRemoveService::getHostId(dbConnection& conn) const {
    // The manager caches the host, so the query is executed only once as soon as the manager lives.
    KnownHost* host = manager_.findLocalhost(conn);
    ACS_COND_THROW ((0 == host), exIllegalValueException("No hostname found for current host in t_knownhosts"));
    return host->getId();
}

void CirculationRemoveService::addToFailedRemoveQueue(dbConnection& conn, const std::string& remoteUrl) {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[addFailedToRemoveQueueWithUrl: " << remoteUrl << "]");
	
	dbPersistent::IdType hostId = getHostId(conn);
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Inserting url \"" << remoteUrl << "\" into \"t_circulation_failed_remove_queue\"");
	dbSmartPersistent sp("t_circulation_failed_remove_queue", conn);
	sp.set<std::string>("url", remoteUrl, conn);
	sp.set<dbPersistent::IdType>("host_id", hostId, conn);
	sp.save(conn);
	
}
void CirculationRemoveService::addToRemoveQueue(dbConnection &conn, string const& remoteUrl) {
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "[addToRemoveQueueWithURL: " << remoteUrl << "]");
    string url = extractLocalPath(remoteUrl);
    vector<CirculationRemoveQueue> elements;

    unsigned long size = findByURL(conn, url, elements, 1);
    if (0 == size) {
        CirculationRemoveQueue removeQueue(conn);
        removeQueue.setURL(url, conn);
        
        if (dbSchemaMetaDataProvider::instance(conn.getKey())->hasField("t_circulation_remove_queue", "host_id", conn)) {
            dbPersistent::IdType hostId = getHostId(conn);
            removeQueue.setHostId(hostId, conn);
        }
        else {
            Lock l(mutex_);
            if (emitHostIdWarning_) {
                ACS_LOG_WARNING("No host_id field found on table t_circulation_remove_queue. Consider to update your db schema.");
                emitHostIdWarning_ = false;
            }
        }
        
        removeQueue.save(conn);
        ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "URL added to to remove queue: " << removeQueue.str(conn));
    } else {
        ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "URL already present in remove queue, skipped : " << url);
    }
}

bool CirculationRemoveService::isRemovable(dbConnection &conn, string const& url, dbPersistent::IdType circulationId, string & outMsg) { // PRQA S 4211
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "[isRemovableWithURL: " << url << "]");
    bool toRemove = true;
    const std::string &key = conn.getKey() ;
    {
        dbQueryParameters parameter(key) ;
        parameter.addParameter(
                dbParam(key, CirculationRemoveQueue::table(), "url",
                        dbParam::EQUAL_TO, dbConv(key,  url))); // PRQA S 3050
        CirculationRemoveQueueQuery query(key);
        query.addParameter(parameter, dbGeneralQueryIF::AND);

        unsigned long size = query.doQuery(conn);
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Url is present in the remove queue: " << size);
        toRemove = (size > 0) && toRemove;
    }
	
	string urlNoPassword = url; // just needed for INFO messages
	try {
		urlNoPassword = RemoteSiteFactory::toURL(RemoteSiteFactory::makeSite(url, true), false);
	}
	catch( std::exception const& xx) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Error getting url with no password: " << xx.what() );
	} 

    if (toRemove) {
		
        toRemove = isToRemove(conn, url, circulationId);
		ostringstream os;
		if( toRemove ) {
			os << '\"' << urlNoPassword << "\" url is removable";
		}
		else {
			os << '\"' << urlNoPassword << "\" url is not removable: used by other not COMPLETED circulations (the current one has id=" << circulationId << ')' ;
		}
		outMsg = os.str();
			
    }
	else {
		outMsg ='\"'+urlNoPassword+"\" url is not removable: already removed";
	}
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "url is " << ( toRemove ? "" : "not ") << "removable: \"" << url << '\"');
    return toRemove;
}
bool CirculationRemoveService::isToRemove(dbConnection &conn, string const& url, dbPersistent::IdType circulationId) // PRQA S 4211
{
    const std::string &key = conn.getKey() ;
    vector<int> allStatusButCompleted = manager_.getNotEndStati(key);
    allStatusButCompleted.push_back(Status::instance(key)->id(Status::error));
    allStatusButCompleted.push_back(Status::instance(key)->id(Status::suspended));
    dbQueryParameters parameter(key);
    parameter.addParameter(dbParam(key, Circulation::table(), "status_id", dbParam::IN, allStatusButCompleted));
    parameter.addParameter(dbParam(key, Circulation::table(), "fromurl", dbParam::EQUAL_TO, dbConv(key,url))); // PRQA S 3050
    if (circulationId > -1) {
        parameter.addParameter(
                dbParam(key, Circulation::table(), "id", dbParam::DIFFERENT_FROM, dbConv(key, circulationId))); // PRQA S 3050
    }
    CirculationQuery query(key);
    query.addParameter(parameter, dbGeneralQueryIF::AND);
    query.doQuery(conn);

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Circulations found: " << query.size());
    return query.empty() ;
}

bool CirculationRemoveService::handleUnlinkFailures(const std::string& source) {
	std::string newname;
	std::string trashPath;
	std::string srcpath = File::getDirPath(source);
	bool failureHandled = false;
	try {

		switch (unlinkFailureAction_) {
		// The action associated to the unlink failure is to move the source product into the configured trash folder
		case ImplicitCirculationConfigurer::ON_UNLINK_FAILURE_DO_MOVE:
			if (localUnlinkFailureTrashPath_[0] == '/') { // Path is absolute
				trashPath = localUnlinkFailureTrashPath_;
			} else { // Path is relative
				// The trash path is a relative path, it must be considered as relative wrt the source file
				trashPath = StringUtils::pathJoin(File::getDirPath(source), localUnlinkFailureTrashPath_);
			}
			
			if (!File::exists(trashPath)) {
				ACS_LOG_WARNING("Source \"" << source << "\" couldn't be unlinked. It cannot be moved into trash path \"" << trashPath << "\" since it doesn't exists.");
			} else {
				trashPath = StringUtils::pathJoin(trashPath, File::getFileName(source));
				ACS_LOG_WARNING("Source \"" << source << "\" couldn't be unlinked. It will be moved into trash path " << trashPath << ".");
				File::rename(source, trashPath);
				failureHandled = true;
			}
			break;
		// The action associated to the unlink failure is to rename the source product according to the configured strategy
		case ImplicitCirculationConfigurer::ON_UNLINK_FAILURE_DO_RENAME:
			if (unlinkFailureRenameStrategy_.mode == ImplicitCirculationConfigurer::PREFIX) {
				newname = StringUtils::pathJoin(File::getDirPath(source), unlinkFailureRenameStrategy_.value + File::getFileName(source));
			}
			else if (unlinkFailureRenameStrategy_.mode == ImplicitCirculationConfigurer::SUFFIX) {
				newname = source + unlinkFailureRenameStrategy_.value;
			}
			else {
				ACS_LOG_WARNING("Unknown failure rename strategy " + std::to_string(static_cast<unsigned int>(unlinkFailureRenameStrategy_.mode)));
			}
			
			if (File::exists(newname)) {
				ACS_LOG_WARNING("Cannot rename file " << source << " into " << newname << " since destination already exists.");
			}
			else {
				ACS_LOG_WARNING("Source \"" << source << "\" couldn't be unlinked. Renaming source file " << source << " into " << newname);
				File::rename(source, newname);
				failureHandled = true;
			}
			break;
		default:
			break; // do nothing
		}
	} catch (std::exception& e) {
		ACS_LOG_WARNING("Error handling local unlink failure. Reason was: " << e.what());
	}
    
    return failureHandled;
}

void CirculationRemoveService::remove(dbConnection& conn, RemoteSite const& site, dbPersistent::IdType circulationId, StopController& stopper) {
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "[removeWithSite: " << site.str() << "]");
    string url = RemoteSiteFactory::toURL(site);
	bool localhost = false;
    if (site.isLocalhost()) {
		localhost = true;
        url = site.getPath();
    }
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "url to remove: " << url);
	
	string infoMsg;
    if (isRemovable(conn, url, circulationId, infoMsg)) {
        ThreadSafe::Lock l(*this);
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "url is removable: \"" << url << "\"");
		string urlNoPassword = url; // just needed for INFO messages
		if( deleteByUrl(conn, url) ) { 
			try
			{
				if (localhost) {
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "unlink: " << url);
					std::string source = StringUtils::pathNormalize(site.getPath());
					try {
						File::unlink(source, false, true);
					} catch (std::exception& e) {
						if (!handleUnlinkFailures(source)) {
							throw;
						}
					}
				} else {
		    		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Removing remote file/dir " << url);
		    		bool isDir = RemoteSiteUtility::isDir(site);
		    		BandPriority no_priority ;
		    		FtpCurl curl(no_priority, site);
		    		if (isDir) {
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "del dir tree" << url);
					curl.delDirTree(site.getPath(), stopper);
		    		} else {
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "del " << url);
					curl.del(stopper);
		    		}
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Removed remote file/dir ");
				}
				
				try {
					urlNoPassword = RemoteSiteFactory::toURL(RemoteSiteFactory::makeSite(url, true), false);
				}
				catch( std::exception const& xx) 
				{
					ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Error getting url with no password: " << xx.what() );
				} 
				ACS_LOG_PRIVINFO("[" << __FUNCTION__ << "] url removed: \"" << urlNoPassword << "\"");
			}
			catch (exception&)
			{

				bool throwExc = true;
				try {
					if( localhost && not File::exists(url) ) { // if localhost only check on file existence performed: for remote hosts it could be time expensive
						ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Could not remove the local url \"" << url << "\" since not existing");
						throwExc = false;
					}
				}
				catch (std::exception const& ee)
				{
					ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
						ACS_LOG_NOTIFY_EX (ee);
					ACS_CLASS_END_DEBUG
					ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Cannot state if local url \"" << url << "\" exists");
				}

				if( throwExc ) {
					try {
						// Insert url into failed remove queue
						if (enqueueOnFailure_) {
							if (dbSchemaMetaDataProvider::instance(conn.getKey())->existTable("t_circulation_failed_remove_queue", conn)) {
								addToFailedRemoveQueue(conn, url);
							} else {
								ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Table t_circulation_failed_remove_queue not found in schema. Url " + urlNoPassword + " cannot be registered.");
							}
						}
						else {
							ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Enqueue on failure not requested");
						}
					} catch (std::exception& e) {
						ACS_LOG_WARNING("Could not insert url " << urlNoPassword << " into t_circulation_failed_remove_queue. Reason was: " + std::string(e.what())); 
					}
					throw;
				}
			}
		}
		else {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Circulation remove queue elements already removed: path will not be removed.");
		}
    } else {
        ACS_LOG_INFO(infoMsg);
    }
}
unsigned long CirculationRemoveService::findByURL(dbConnection& conn, std::string const& url, std::vector<CirculationRemoveQueue> &results, // PRQA S 4211
        size_t limit) {
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "[findByURLWithURL: " << url << "]");
    
    dbPersistent::IdType hostId = 0;
    if (dbSchemaMetaDataProvider::instance(conn.getKey())->hasField("t_circulation_remove_queue", "host_id", conn)) {
        RemoteSite site = RemoteSiteFactory::makeSite(url);
        // The host id must be considered only if url refers to a local resource, i.e., the protocol is file or fs or the hostname is localhost
        if (site.isNetworkLocalhost()) {
            hostId = getHostId(conn);
        }
    } 
    else {
        Lock l(mutex_);
        if (emitHostIdWarning_) {
            ACS_LOG_WARNING("No host_id field found on table t_circulation_remove_queue. Consider to update your db schema.");
            emitHostIdWarning_ = false;
        }
    }
    
    const string &key = conn.getKey() ;
    CirculationRemoveQueueQuery q(key) ;
    dbQueryParameters parameter(key) ;
    parameter.addParameter( dbParam(key, CirculationRemoveQueue::table(), "url", dbParam::EQUAL_TO, dbConv(key, url))); // PRQA S 3050
    if (hostId != 0) {
        parameter.addParameter( dbParam(key, CirculationRemoveQueue::table(), "host_id", dbParam::EQUAL_TO, dbConv(key, hostId))); // PRQA S 3050
    }
    q.addParameter(parameter, dbGeneralQueryIF::AND);
    q.maxItems(limit) ; // PRQA S 3000
    q.doQuery(conn) ;
    q.fill(results) ;

    return results.size();
}

void CirculationRemoveService::removeURL(dbConnection& conn, RemoteSite const& site, std::string const& url, StopController& stopper, bool rethrowExc) {
	
	bool localhost = false;

	string urlNoPassword = url; // just for INFO messages
	try {
		urlNoPassword = RemoteSiteFactory::toURL(RemoteSiteFactory::makeSite(url, true), false);
	}
	catch( std::exception const& xx) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Error getting url with no password: " << xx.what() );
	} 
    try {
        if (site.isLocalhost()) {
            localhost = true;
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "unlink: " << url);
            std::string source = StringUtils::pathNormalize(site.getPath());
            try {
                // The following exception is thrown only to simulate an
                // unlink failure caused by unstable connections to NFS path.
                if (ACS_CLASS_GET_DEBUG() == 24680) {
                    ACS_THROW(exIllegalValueException("Test exception for unstable NFS connection."));
                }
                File::unlink(source, false, true);
            } catch (std::exception& e) {
                if (!handleUnlinkFailures(source)) {
                    throw;
                }
            }
        } else {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Removing remote file/dir " << url);
            bool isDir = RemoteSiteUtility::isDir(site);
            BandPriority no_priority ; 
            FtpCurl curl(no_priority, site);
            if (isDir) {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "del dir tree" << url);
                curl.delDirTree(site.getPath(), stopper);
            } else {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "del " << url);
                curl.del(stopper);
            }
        }
		ACS_LOG_PRIVINFO("[" << __FUNCTION__ << "] url removed: \"" << urlNoPassword << "\"");

    } catch (std::exception const& ex) {
		bool notifyExc = true;
		try {
			if( localhost && not File::exists(site.getPath()) ) { // if localhost only check on file existence performed: for remote hosts it could be time expensive
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Could not remove the local url \"" << url << "\" since not existing");
				notifyExc = false;
			}
		}
		catch (std::exception const& ee)
		{
			ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
				ACS_LOG_NOTIFY_EX (ee);
			ACS_CLASS_END_DEBUG
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Cannot state if local url \"" << url << "\" exists");
		}

		if( notifyExc ) {
			ACS_LOG_NOTIFY_EX (ex);
    		ACS_LOG_ERROR ("Could not remove the url \"" << urlNoPassword << "\"");
            
			string errorCause;
			if( dynamic_cast<const exException*> (&ex) )	{
				string description = dynamic_cast<const exException*>(&ex)->description();
				errorCause = ": " + description.substr(0, description.find('\n'));
			}	
			
			try {
				// Insert url into failed remove queue
				if (enqueueOnFailure_) {
					if (dbSchemaMetaDataProvider::instance(conn.getKey())->existTable("t_circulation_failed_remove_queue", conn)) {
						addToFailedRemoveQueue(conn, url);
					} else {
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Table t_circulation_failed_remove_queue not found in schema. Url " + urlNoPassword + " cannot be registered.");
					}
				}
				else {
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Enqueue on failure not requested");
				}
			} catch (std::exception& e) {
				ACS_LOG_WARNING("Could not insert url " << urlNoPassword << " into t_circulation_failed_remove_queue. Reason was: " + std::string(e.what())); 
			}
			
			if (rethrowExc) {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Rethrow of exception requested");
				throw;
			}
		}
	}
}

void CirculationRemoveService::removeURL(dbConnection& conn, RemoteSite const& site, std::string const& url, StopController& stopper) throw() {
	removeURL(conn, site, url, stopper, false);
}

void CirculationRemoveService::removeSource(dbConnection &conn, Circulation const& circ, RemoteSite const& sourceSite, StopController& stopper) {
    ThreadSafe::Lock l(*this);
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "[removeSourceWithCirc: " << circ.str(conn) <<" AndSourceSite: " << sourceSite << "]"); // PRQA S 3050
    bool null = false ;
    string fromurl = circ.getFromURL(null, conn) ;
    addToRemoveQueue(conn, fromurl);
	string urlNoPassword = fromurl; // just for INFO messages
	try {
		urlNoPassword = RemoteSiteFactory::toURL(RemoteSiteFactory::makeSite(fromurl, true), false);
	}
	catch( std::exception const& xx) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Error getting url with no password: " << xx.what() );
	} 
    try {
        ACS_LOG_PRIVINFO("[" << __FUNCTION__ << "] Going to remove source: " << urlNoPassword);
        remove(conn, sourceSite, circ.getId(), stopper);
    } catch (std::exception const& ex) {
		ACS_LOG_NOTIFY_EX(ex);
        ACS_LOG_ERROR("Could not remove the url \"" << urlNoPassword << '\"');
    }
}

void CirculationRemoveService::removeSourceAfterComplete(dbConnection &conn, // PRQA S 4020
                                                         Circulation const& circ, 
                                                         RemoteSite const& sourceSite,
                                                         StopController& stopper) {
//this remove method will called after the circulation record, this method is like the removeSource()
//and it has added in order to solve a race condition problem.
//This method will:
//1. select into the circulation remove table the current source file; if it is not present exit;
//2. try to remove the file ( unlink or remote remove ), if some error occurred ignore it (probably because the
//file was already canceled);
//3. remove this file (from_url) from the t_circulation_remove_queue.
    ThreadSafe::Lock l(*this);
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "[removeSourceWithCirc: " << circ.str(conn) <<" AndSourceSite: " << sourceSite<< "]"); // PRQA S 3050
    vector<CirculationRemoveQueue> elements;
    string url = RemoteSiteFactory::toURL(sourceSite);
    
    unsigned long size = findByURL(conn, url, elements, 1);
    if (size == 0) {
        return;
    }
	
	string urlNoPassword = url; // just for INFO messages
	try {
		urlNoPassword = RemoteSiteFactory::toURL(RemoteSiteFactory::makeSite(url, true), false);
	}
	catch( std::exception const& xx) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Error getting url with no password: " << xx.what() );
	} 
	
    try {
        ACS_LOG_PRIVINFO("[" <<__FUNCTION__ << "] circulation remove queue not empty; going to check if source: " << urlNoPassword << " can be removed");
        ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "[removeWithSite: " << sourceSite.str() << "]");
        if (sourceSite.isLocalhost()) {
            url = sourceSite.getPath();
        }
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "url to remove: " << url << " numb of rows to cancel: " << elements.size());
		string infoMsg;
        if (isRemovable(conn, url, circ.getId(), infoMsg)) {
			if( deleteByUrl(conn, url) ) { 
        		ACS_LOG_INFO("Going to remove url: \"" << urlNoPassword << "\"; same url used by a group of concurrent circulations some of them having removeSource set.");
               	removeURL(conn, sourceSite, url, stopper);
			}
			else {
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Circulation remove queue element already removed: path will not be removed.");
			}
        }
		else {
            ACS_LOG_INFO(infoMsg);
    	}
    } catch (std::exception const& ex) {
		ACS_LOG_NOTIFY_EX(ex);
        ACS_LOG_ERROR("Could not remove the url \"" << urlNoPassword << '\"');
    }
}

bool CirculationRemoveService::deleteByUrl(dbConnection& conn, std::string const& url) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << url << '\"');
    
    const string &key = conn.getKey() ;
	SQLString sqlString(key, SQLString::DELETE) ;
	sqlString.addTableList(CirculationRemoveQueue::table()) ;

    dbPersistent::IdType hostId = 0;
    RemoteSite site = RemoteSiteFactory::makeSite(url);
    if (dbSchemaMetaDataProvider::instance(conn.getKey())->hasField("t_circulation_remove_queue", "host_id", conn)) {
        // The host id must be considered only if url refers to a local resource, i.e., the protocol is file or fs or the hostname is localhost
        if (site.isNetworkLocalhost()) {
            hostId = getHostId(conn);
        }
    } 
    else {
        Lock l(mutex_);
        if (emitHostIdWarning_) {
            ACS_LOG_WARNING("No host_id field found on table t_circulation_remove_queue. Consider to update your db schema.");
            emitHostIdWarning_ = false;
        }
    }

	{
    	dbQueryParameters parameter(key) ;
        parameter.addParameter( dbParam(key, CirculationRemoveQueue::table(), "url", dbParam::EQUAL_TO, dbConv(key, url))); // PRQA S 3050
        if (hostId != 0) {
            parameter.addParameter( dbParam(key, CirculationRemoveQueue::table(), "host_id", dbParam::EQUAL_TO, dbConv(key, hostId))); // PRQA S 3050
        }
		sqlString.addANDWhereParameter(parameter);
	}
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to execute the following SQL statement: "  << sqlString.getSQL() );
	dbQuery query(conn, sqlString, "Removal from " + CirculationRemoveQueue::table() + " table") ;
	if (not query.execForStatus() ) {
		string ErrorString ;
		ErrorString += "Query Execution error SQL command is \"" ;
		ErrorString += sqlString.getSQL() ;
		ErrorString += "\". Error is \"" + string(query.errorString())  + "\".";
		ACS_THROW (dbQuery::QueryExecuteException (ErrorString) ) ;
	}
	size_t deletedRows =  query.getAffectedRows();
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, deletedRows << " records removed from " << CirculationRemoveQueue::table() + " table");
	return ( deletedRows > 0 );
}

_ACS_END_NESTED_NAMESPACE;
