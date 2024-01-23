// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA
 *
 *  $Prod: A.C.S. libFtpCurl Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.8  2016/07/26 09:14:54  lucio.pulvirenti
 *  S1PDGS-31765: enum for local transfer mode introduced and relative flag added to IF along with set and get methods. In case of local to local
 *  circulation either hard link or copy performed depending on local transfer mode flag
 *
 *  Revision 5.7  2016/03/22 16:29:53  francesco.avanzi
 *  S2PDGS-1434: CirculationCurl modified to use new FtpCurl methods for Early and Late Target File Removal
 *
 *  Revision 5.6  2016/03/21 18:25:54  francesco.avanzi
 *  WORKINPROGRESS s2pdgs-1434
 *
 *  Revision 5.5  2014/02/14 13:54:31  marpas
 *  attempt to refactor band shaping mechanism
 *
 *  Revision 5.4  2013/11/04 15:41:51  marpas
 *  simulation mode added to conf
 *
 *  Revision 5.3  2013/10/24 18:27:59  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/10/22 17:31:50  marpas
 *  work in progress
 *
 *  Revision 5.1  2013/07/15 16:30:46  marpas
 *  qa warnings fixed
 *
 *  Revision 5.0  2013/06/06 18:08:19  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.11  2013/04/17 20:40:06  marpas
 *  interface robustness improved
 *
 *  Revision 2.10  2013/04/05 09:44:15  marpas
 *  coding best practices applied
 *  qa warnings (partial)
 *  libDCUtility interface update
 *
 *  Revision 2.9  2013/03/22 13:25:53  marpas
 *  mode is an integral and must be used as is,
 *  chmod static method is useless and has been removed
 *
 *  Revision 2.8  2012/11/29 16:28:20  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.7  2012/11/27 13:26:55  micmaz
 *  added quality tool directives.
 *
 *  Revision 2.6  2012/09/21 13:34:27  micmaz
 *  ACCEPTED - issue S2GPP-611: Development of Distribution Formatter framework for Export
 *  http://jira.acsys.it/browse/S2GPP-611
 *
 *  Revision 2.5  2012/03/27 18:08:49  micmaz
 *  work in progress
 *
 *  Revision 2.4  2012/02/13 15:03:57  micmaz
 *  added stats.
 *
 *  Revision 2.3  2012/02/07 10:25:46  micmaz
 *  removed warnings .
 *
 *  Revision 2.2  2012/02/02 17:19:58  micmaz
 *  IN PROGRESS - issue S1PDGS-2769: The Circulation Curl does not take into account the slow parameter
 *  http://jira.acsys.it/browse/S1PDGS-2769
 *
 *  Revision 2.1  2012/01/20 16:03:51  micmaz
 *  OPEN - issue S1PDGS-2685: Circulation Agent Core in transferring large files
 *  http://jira.acsys.it/browse/S1PDGS-2685
 *
 *  Revision 2.0  2011/10/18 14:43:49  marpas
 *  new statistics adopted
 *
 *  Revision 1.11  2011/09/29 18:33:04  marpas
 *  fixing codecheck crashes
 *
 *  Revision 1.10  2011/08/12 14:49:04  micmaz
 *  work in progress..
 *
 *  Revision 1.9  2011/05/13 08:55:50  micmaz
 *  work in progress
 *
 *  Revision 1.8  2011/05/12 17:22:13  micmaz
 *  work in progress
 *
 *  Revision 1.7  2011/05/05 17:52:34  micmaz
 *  work in progress...
 *
 *  Revision 1.6  2011/04/12 18:02:26  micmaz
 *  work in progress
 *
 *  Revision 1.5  2011/04/11 12:25:06  micmaz
 *  work in progress
 *
 *  Revision 1.4  2011/04/05 17:15:09  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.3  2011/04/01 14:43:55  micmaz
 *  implemented the Circulations WP
 *
 *  Revision 1.2  2011/02/24 11:41:10  micmaz
 *  Added:  CirculationCurl in order to make  dir-transfer using this server as bridge.
 *
 *  Revision 1.1  2011/02/23 14:39:26  micmaz
 *  Added: Ftpes support (but not tested) and CirculationCurl in order to make a file-transfer using this server as bridge.
 *
 *
 *
 *
 */

#ifndef CIRCULATIONCURL_H_
#define CIRCULATIONCURL_H_

#include <acs_c++config.hpp>

#include <RemoteSite.h>
#include <FtpPerformance.h>
#include <StreamCurl.h>
#include <BandPriority.h>

#include <StopController.h>
#include <exMacros.h>
#include <exStream.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) ;

class CirculationCurl: public BandPriority { // PRQA S 2109, 2153
public:
		
	enum LocalTransferMode // NOSONAR - old enum
    {
        COPY,
        HARDLINK,
        SYMLINK
    };

    /**
     * Empty ctor.
     */
    explicit CirculationCurl(const StopController& stopController);
    /**
     * Full ctor.
     * @param sourceSite: the source site where find the source file/directory to transfer.
     * @param targetSite: the destination site where to transfer the source file/directory.
     */
    CirculationCurl(RemoteSite const& sourceSite, RemoteSite const& targetSite, const StopController& stopController);

    CirculationCurl(const CirculationCurl &) = delete ;
    CirculationCurl& operator=(const CirculationCurl &) = delete ;

    ~CirculationCurl() override = default ;

    /**
     * The operator() function used by boost::thread to run the file/directory transfer
     * in a separate thread.
     * To use this method, set the source and target files and call operator()().
     * It is equivalent to call directly transfer() with the soruce and target as function
     * arguments.
     */
    void operator()();
    /**
     * The operator() function used by boost::thread to run the file/directory transfer
     * in a separate thread.
     * To use this method, set the source and target files and call operator()().
     * It is equivalent to call directly transfer() with the soruce and target as function
     * arguments.
     */
    void doTransfer();

    // Getters / Setters and to string method.
    const RemoteSite &getSourceSite() const noexcept ;
    const RemoteSite &getTargetSite() const noexcept ;
    void setSourceSite(RemoteSite const& site);
    void setTargetSite(RemoteSite const& site);
    void getStats(std::vector<curl::CirculationStatistics> &stats);

    std::string str() const;

    friend std::ostream& operator<<(std::ostream& stream, CirculationCurl& obj); // PRQA S 2107

    /*- set the transfer in simulation mode - noop.
     * \note if the transfer is in progress, simulation mode cannot be set.
     */
    void setSimulationMode(bool) noexcept ;
    bool isSimulated() const noexcept ;
	/**
		ENRCAR:: 
		Meaning of the flag: EarlyFinalRemoval [deduced by user-level applications]
		"Early" means: "At early stage", i.e. "Before calling Curl"
		"FinalRemoval" means: "Remove the final destination path, if existing"
		
		So:
		* If "EarlyFinalRemoval" is TRUE, the user-level-application is in charge of removing the existing path at destination
		(i.e. -> Curl will not remove the destination file)
		* If "EarlyFinalRemoval" is FALSE, the user-level-application will not remove the existing path at destination
		(i.e. -> Curl is in charge to remove the destination file, if already existing)
		
		So, as a conclusion, a flag like "OverwriteDestination" could be obtained via a simple negation:
		bool OverwriteDestination = (not EarlyFinalRemoval) ;
	*/
    void setEarlyFinalRemoval(bool v) noexcept { _earlyFinalRemoval = v ; } 
    bool getEarlyFinalRemoval() const noexcept { return _earlyFinalRemoval ; } 
	
	/**
		A new pair of get/set methods to handle the "EarlyFinalRemoval" flag has been introduced.
		The method name should be more comfortable.
	*/
	bool getRemoveTargetBeforeTransfer() const  		 { return (! getEarlyFinalRemoval()) ; }
	void setRemoveTargetBeforeTransfer(const bool b)	 { setEarlyFinalRemoval (! b) ; }

	
	void setStreamerId(dbPersistent::IdType id) noexcept { _streamerId = id; }
	dbPersistent::IdType getStreamerId() const noexcept { return _streamerId; }
	
	
	void setLocalTransferMode(LocalTransferMode v) { _localTransferMode = v; }
	LocalTransferMode getLocalTransferMode() const noexcept { return _localTransferMode ; }

    long getSourceHttpResponseCode() const noexcept { return _source_http_response_code ; }
    long getTargetHttpResponseCode() const noexcept { return _target_http_response_code ; }

#ifdef ACS_TEST
public:
#else
private:
#endif
    /**
     * Transfer method.
     * Used to transfer file/directory: <P>
     * <ul>
     * <li>from a local site to a remote site;
     * <li>from a remote site to a local site;
     * <li>from a local site to a local site (simple a File::copy() method will be invoked);
     * </ul>
     */
    void localTransfer();
    /**
     * Transfer method from and to a remote site.
     * Used to transfer file;
     */
    void transferFile();
    /**
     * Transfer method from and to a remote site.
     * Used to transfer all the tree directory (dir and all sub-dirs);
     */
    void transferTreeDir();
    
    /**
     * Returns the user specified at configuration or, if not configured, the current user.
     */
    std::string getUser() const;
    
    /**
     * Returns the group specified at configuration or, if not configured, the group associated to user
     */
    std::string getGroup(const std::string& user) const;
    
    bool changePermissions(const std::string& path) const;
    //Callbacks:
    static void visitRemoteFile(const std::string &remoteFile, const std::string &relativeRemoteFile, void* observer);
    static void visitRemoteDir(const std::string &remoteFile, const std::string &relativeRemoteFile, void* observer);
    
    static File::CompressionMode streamTypeToCompressionMode(const std::string& streamtype);
    
#ifdef ACS_TEST
public:
#else
private: // NOSONAR - for clarity
#endif
    static int chown(std::string const& userId, std::string const& groupId, std::string const& filename); // PRQA S 2502
    static int getUID(std::string const& userId, uid_t* uid) ;
    static int getGID(std::string const& groupId, gid_t* gid) ;

#ifdef ACS_TEST
public:
#else
private: // NOSONAR - for clarity
#endif
    RemoteSite _sourceSite = {} ;
    RemoteSite _targetSite = {} ;
    curl::FtpPerformance _perfo = {} ;
    //Statistics must be collected only if the curl transfer was ok.
    std::vector<curl::CirculationStatistics> _statistics = {} ;
    acs::ThreadSafe _mutex = {} ;
    const StopController & _stopController;
    bool _simulationMode = conf::CurlConfigurerSingleton::instance()->getSimulateCirculation() ;
    bool _earlyFinalRemoval = true  ; // default remove final file BEFORE the transfer
    dbPersistent::IdType _streamerId = 0 ;
	LocalTransferMode _localTransferMode = COPY ;
	long _source_http_response_code = 0 ;
	long _target_http_response_code = 0 ;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(CirculationCurl) ;


};

_ACS_END_NESTED_NAMESPACE

#endif /* CIRCULATIONCURL_H_ */
