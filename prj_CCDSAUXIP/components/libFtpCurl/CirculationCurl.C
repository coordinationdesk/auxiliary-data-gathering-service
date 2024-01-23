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
 *
 */

#include <CirculationCurl.h>
#include <CurlStatistics.h>
#include <Application.h>
#include <dbSchemaMetaDataProvider.h>
#include <dbConnectionPool.h>
#include <NameIdAssociator.h>
#include <PipedBytesStream.h>
#include <RemoteDirVisitor.h>
#include <FtpProgressData.h>
#include <TemporaryFileService.h>
#include <RemoteSiteUtility.h>
#include <FtpCurl.h>
#include <CurlConfigurer.h>
#include <Password.h>
#include <File.h>
#include <Dir.h>
#include <StringUtils.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>


_ACS_BEGIN_NESTED_NAMESPACE(acs, curl)

using namespace acs::io;
using namespace acs::curl::temp;
using std::string ;
using std::vector ;

ACS_CLASS_DEFINE_DEBUG_LEVEL (CirculationCurl);

//
//Ctors and Dtor.
//

CirculationCurl::CirculationCurl(const StopController& stopController) : 
    _stopController(stopController)
{
}

CirculationCurl::CirculationCurl(RemoteSite const& sourceSite, 
                                 RemoteSite const& targetSite,
                                 const StopController& stopController) :
    _sourceSite(sourceSite), 
    _targetSite(targetSite), 
    _stopController(stopController)
{
}


//
//Public methods.
//
void CirculationCurl::operator ()() { 
    doTransfer() ;
}

void CirculationCurl::doTransfer() { // PRQA S 4020
    ACS_COND_THROW(_sourceSite.str() == _targetSite.str(), exIllegalValueException("The target and the source resources are the same")) ; // PRQA S 3081
    //this is a simple transfer: it is a put() or a get().
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Source site is local?: " << _sourceSite.isLocalhost());
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Target site is local?: " << _targetSite.isLocalhost());
    if (isSimulated()) {
        ACS_LOG_WARNING("Simulated circulation") ;
        Timer::delay(4) ; // 4 ms  // PRQA S 4400
        return ;
    }
    
    if (_targetSite.supportsActivePassive()) { // does the protocol support active/passive ? 
        // this to discover the target site mode (active/passive). In case it has already been discovered
        // in the current proc life, the value is cached by the FtpCurl statis data members
        FtpCurl target(*this, _targetSite);
        target.passiveActiveDiscover() ;
        _targetSite = target.getSite() ; // get back result
    }
    
    if (_sourceSite.supportsActivePassive()) { // does the protocol support active/passive ?
        // this to discover the source site mode (active/passive). In case it has already been discovered
        // in the current proc life, the value is cached by the FtpCurl statis data members
        FtpCurl source(*this, _sourceSite);
        source.passiveActiveDiscover() ;
        _sourceSite = source.getSite() ; // get back result
    }

    if (_sourceSite.isLocalhost() || _targetSite.isLocalhost()) {
		localTransfer();
    } 
    else {
        //this is a bridge-transfer: it is a stream put() and a stream get() from and to remote sites.
		_source_http_response_code = 0 ;
		_sourceSite.setIsDir(RemoteSiteUtility::isDir(_sourceSite, _source_http_response_code));

        if (_sourceSite.getIsDir()) {
            transferTreeDir();
        } 
        else {
            transferFile();
        }
    }
}

std::string CirculationCurl::getUser() const {
    std::string user;
    conf::CurlConfigurer::ChangeLocalPermissions const& permissions = conf::CurlConfigurerSingleton::instance()->getPermissions();
    if (!StringUtils::isBlank(permissions.getUserId())) {
        user = permissions.getUserId();
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "user "<< user);
    } 
    else {
        try {
            os::Password::passwd_w pw = os::Password::getPwUid (geteuid ());
            user = pw.pw_name;
        }
        catch(const std::exception &x) { // NOSONAR - any exception
            ACS_LOG_NOTIFY_EX(x) ;
        }
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "default user "<< user);
    }
    
    return user;
}

std::string CirculationCurl::getGroup(const std::string& user) const {
    std::string group ;
    conf::CurlConfigurer::ChangeLocalPermissions const& permissions = conf::CurlConfigurerSingleton::instance()->getPermissions();
    if (!StringUtils::isBlank(permissions.getGroupId())) {
        group = permissions.getGroupId();
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "group "<< group);
    } 
    else {
        uid_t uid ; // PRQA S 4101
        if (0 == getUID(user, &uid)) {
            try {
                os::Password::passwd_w pw = os::Password::getPwUid (uid) ;
                os::Password::group_w gw = os::Password::getGrGid(pw.pw_gid) ;
                group = gw.gr_name;
                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "default group "<< group);
            }
            catch(const std::exception &x) { // NOSONAR - any exception
                ACS_LOG_NOTIFY_EX(x) ;
            }
        }
    }
    
    return group;
}

bool CirculationCurl::changePermissions(const std::string& path) const // PRQA S 4214
{
    // http://jira.acsys.it/browse/S1PDGS-29906
    // Access conflicts to a common directories between two applications: solution to implement
    // It has been presented, in the document Installation  SD-55 Requirements Refinement
    // S1PD.TCN.00269.ASTR (see chap 3 and annex 2), the groups that shall be used for application and data accounts.
    // Therefore, the idea is to use a specific permission (SUID on folder) for the data group, the application belonging also to this group
    bool changedPermissions = false;
    if (conf::CurlConfigurerSingleton::instance()->changePermissions()) {
        conf::CurlConfigurer::ChangeLocalPermissions const& permissions =
          conf::CurlConfigurerSingleton::instance()->getPermissions();
          
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Must change the local permissions " << permissions.str());
        
        string user = getUser();
        string group = getGroup(user);
        mode_t perm = permissions.getMode();
        
        using std::oct ; 
        
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "mode " << oct << perm);
        if (!StringUtils::isBlank(user) && !StringUtils::isBlank(group)) {
            bool change_owner = conf::CurlConfigurerSingleton::instance()->isActivatedChangeOwner() ;
            if ((change_owner && (0 == chown(user, group, path))) || ! change_owner) { // PRQA S 3230
                bool change_mode = conf::CurlConfigurerSingleton::instance()->isActivatedChangeMode() ;
                if (change_mode) {
                    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "chmod: " << oct << perm << " on file " << path);
                    File::chmod(path, perm, File::SetPerm);
                }
                if (File::isDir(path)) {
                    DirTree folder(path, "",true);
                    folder.load();
                    size_t f_size = folder.size() ;
                    for ( size_t counter=0; counter < f_size; ++counter) {
                        string filename = folder[counter];
                        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "file to change permissions " << filename);
                        if ((change_owner && (0 == chown(user, group, filename))) || ! change_owner) { // PRQA S 3230
                            if (change_mode) {
                                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "chmod: " << oct << perm << " on file " << filename);
                                File::chmod(filename, perm, File::SetPerm);
                            }
                        } 
                        else {
                            break;
                            ACS_LOG_ERROR("Could not change permissions to " << filename);
                        }
                    }
                }
                changedPermissions = true;
            } 
            else {
                ACS_LOG_ERROR("Could not change permissions to " << path);
            }
        }
    } 
    else {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Must not change the local permissions to " << path);
    }
    return changedPermissions;
}

namespace {
bool endsWithSlashDot (const string& thePath)
{
	bool retval = false;
	if (! thePath.empty() )
	{
		string normalizedPath = StringUtils::trimEnd(thePath, '/');

		string lastFragment;

		// Search if any the first slash starting from searchpos
		size_t slashpos = normalizedPath.rfind ("/");
		if (slashpos != string::npos )
		{
			// Found.. start from it
			lastFragment = normalizedPath.substr (slashpos + 1, string::npos );
		}
		else
		{
			// Not found.. start from the begining up to the last known slash position
			lastFragment = normalizedPath;
		}
		// return true if last fragment is a dot
		retval = ( "." == lastFragment );
	}
	return retval;
}

};

void CirculationCurl::localTransfer() {

	_source_http_response_code = _target_http_response_code = 0 ; 

    //Case 1. both sites are local: make a simple copy (hardlink if possible).
    if (_sourceSite.getIsFileSystem() && _targetSite.getIsFileSystem()) {
     
        string sourceSitePath = _sourceSite.getPath();
        string targetSitePath = _targetSite.getPath();
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "localTransfer: both sites are on localhost.");
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "      From:" << sourceSitePath << " To: " << targetSitePath);
        string fullPath = File::getDirPath(targetSitePath);
        if (!StringUtils::isBlank(fullPath)) {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Making dest path:" << fullPath);
            File::mkdirtree(fullPath);
        }
        
        string temporaryTargetPath;
        const bool temporaryTargetNameDefined ( _targetSite.hasTemporaryTargetName() ) ;
        if( temporaryTargetNameDefined ) {
            temporaryTargetPath = _targetSite.getTemporaryPath();
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "temporary target path is defined: \"" << temporaryTargetPath << '\"');
            string temporaryTargetDir = File::getDirPath(temporaryTargetPath); 
            if( ! StringUtils::isBlank(temporaryTargetDir) && ! File::exists(temporaryTargetDir) ) {
                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Making temporary dest dir: \"" << temporaryTargetDir << '\"');
                File::mkdirtree(temporaryTargetDir);
            }
        }
        
        vector<File::PerfStatistics> lstats ;
        
        try {

			// Remove destination (including temporary)
			if (getRemoveTargetBeforeTransfer()) 
			{ 
				// Remove temporary destination (if existing -- no exception)
				if( temporaryTargetNameDefined ) { File::remove(temporaryTargetPath) ; }
				
				// Remove destination (if existing -- no exception)
				File::remove(targetSitePath) ; 
			}	

            // get temporary file name
            string targetFilename = targetSitePath;
            if( temporaryTargetNameDefined ) {
                targetFilename = temporaryTargetPath;
            }
            
            switch (_localTransferMode) {
            
            case HARDLINK: 
                { // NOSONAR - scope
                    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "going to transfer with hardlink");

                    bool copiedOnError = false;
                    File::hardlink(File::readlink(sourceSitePath, true), 
                                    targetFilename, 
                                    true,
                                    &copiedOnError,
                                    &lstats);

                    if( copiedOnError ) {
                        ACS_LOG_WARNING("Circulation from \"" << sourceSitePath << "\" to \"" << targetSitePath << "\" cannot be performed via hard link: copy executed");
                    }
                }
                break;
            case COPY: 
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "going to transfer with copy");	
                File::copy(File::readlink(sourceSitePath, true), targetFilename, &lstats);
                break;
            case SYMLINK: 
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "going to transfer with symbolic link");
            // add statistics in symlink method in next version.
                File::symlink(File::readlink(sourceSitePath, true), targetFilename);
                break;
            default:
                ACS_THROW( exIllegalValueException() << "Unrecognized local transfer mode: " << int(_localTransferMode)); 
            }
			
			// change permissions before renaming
            changePermissions(targetFilename);
			
            if( temporaryTargetNameDefined ) {
                // rename with the final name
                File::rename(targetFilename, targetSitePath);
                ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "temporary target filename \"" << targetFilename << "\" successfully renamed as \"" << targetSitePath << '\"');
            }
            
        }
        catch ( const std::exception &e )
        {
            ACS_THROW(FtpCurl::FtpCurlUnrecoverableErrorException(e, "Error got"))
        }
        if (conf::CurlConfigurerSingleton::instance()->getEnableStatistics()) {
            dbConnectionWrapper w(*Application::instance()->getDefaultConnectionPool()) ;
            dbConnection &conn = w ; // PRQA S 3050
            size_t lstats_size = lstats.size() ;
            double size = 0 ;
            double time = 0 ;
            for (size_t i=0; i < lstats_size; ++i) {
                size += lstats[i].size ;
                time += lstats[i].total_time ;
            }
            CirculationStatistics ds(conn) ;
            ds.setDestSite(_targetSite) ;
            ds.setSourceSite(_sourceSite) ;
            ds.setNameLookUpTime(0,conn);
            ds.setConnectionTime(0,conn);
            ds.setAppConnectionTime(0,conn);
            ds.setPreTransfTime(0,conn);
            ds.setStartTransfTime(0,conn);
            ds.setRedirectTime(0,conn);
            ds.setTotalTime(time,conn);
            ds.setDataSize(static_cast<long long>(size),conn); // PRQA S 3081
            _statistics.push_back(ds);
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "_statistics.size(): " << _statistics.size());
        }
        //Case 1. both sites are local: but the target is a media (copy).
    } 
    else if (_sourceSite.getIsFileSystem() && _targetSite.getIsMedia()) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "localTransfer: both sites are on localhost: the target is a media, make a simple copy.");
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "      From:" << _sourceSite.getPath() << " To: " << _targetSite.getPath());
        string fullPath = File::getDirPath(_targetSite.getPath());
        if (!StringUtils::isBlank(fullPath)) {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Making dest path:" << fullPath);
            File::mkdirtree(fullPath);
        }
        vector<File::PerfStatistics> lstats ;
        File::copy(_sourceSite.getPath(), _targetSite.getPath(), &lstats);
        if (conf::CurlConfigurerSingleton::instance()->getEnableStatistics()) {
            dbConnectionWrapper w(*Application::instance()->getDefaultConnectionPool()) ;
            dbConnection &conn = w ; // PRQA S 3050
            size_t lstats_size = lstats.size() ;
            double size = 0 ;
            double time = 0 ;
            for (size_t i=0; i < lstats_size; ++i) {
                size += lstats[i].size ;
                time += lstats[i].total_time ;
            }
            CirculationStatistics ds(conn) ;
            ds.setDestSite(_targetSite) ;
            ds.setSourceSite(_sourceSite) ;
            ds.setNameLookUpTime(0,conn);
            ds.setConnectionTime(0,conn);
            ds.setAppConnectionTime(0,conn);
            ds.setPreTransfTime(0,conn);
            ds.setStartTransfTime(0,conn);
            ds.setRedirectTime(0,conn);
            ds.setTotalTime(time,conn);
            ds.setDataSize(static_cast<long long>(size),conn); // PRQA S 3081
            _statistics.push_back(ds);
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "_statistics.size(): " << _statistics.size());
        }
    } 
    else 
	{
        if (_sourceSite.isLocalhost()) 
		{
        	// Case 2. the source site is a local host, target is not local !!! 
        	// then we have to make a put().
		
            bool sourceIsDir = File::isDir(_sourceSite.getPath());
            _targetSite.setIsDir(sourceIsDir);
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "localTransfer: the source site is a local host, thus we must make a put().");
            
            std::string localPath = _sourceSite.getPath();
            
            FtpCurl curl(*this, _targetSite);
            // Remove strange chars from the paths given to CURL (including any /. at the end)
            curl.setFullLocalFilename(StringUtils::pathNormalize(localPath));
            
            if (0 != _streamerId) {
                // Case 2.1 source must be packed into a tar object
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Case 2.1 on-the-fly tar transfer.");
                dbConnectionWrapper w(*Application::instance()->getDefaultConnectionPool()) ;
                dbConnection &conn = w ; // PRQA S 3050
                const std::string& dbKey = conn.getKey();

                ACS_COND_THROW(!dbSchemaMetaDataProvider::instance(dbKey)->existTable("t_streamers", conn), exCriticalException("Table t_streamers does not exist"));
                
                // Get the streamtype associated to streamer_id
                NameIdAssociator<dbPersistent::IdType> nia("t_streamers", "id", "streamtype", dbKey);
                nia.load();
                std::string streamtype = nia.name(_streamerId);
                
                File::CompressionMode cm = streamTypeToCompressionMode(streamtype);	// throws exception if streamtype is not valid
                // The source directory shall not be included in the tar file in the case 
                // the source path ends with a fragment stating "." i.e. the current directory
                // /my/source/path -> will include "path" in the tar package as a container
                // /my/source/path/. -> will not include "path" in the tar package
                bool includeRootDir = sourceIsDir;
                if (sourceIsDir && endsWithSlashDot(localPath) )
                {
                    // Is a directory end ends with a fragment named . -> shall not include the containing directory in the package
                    includeRootDir=false;
                }

				// Note that putAsTar has a simplified I/F such that the "overwriteDestination" flag is always set
				// (No need to route flag EarlyFinalRemoval)
                curl.putAsTar(cm, includeRootDir, 0, _stopController);	// include root dir if source is dir
            }
            else {
                // Case 2.2
                if (_targetSite.getIsDir()) {
                    //Case 2.2.1 we have to transfer a directory.
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Case 2.2.1 we must transfer a directory.");
                    curl.putDir(localPath, _targetSite.getPath(), FtpCurl::noFileFilter, _stopController);
                } 
                else {
                    using std::boolalpha ; 
                    
                    //Case 2.2.2 we must transfer a file.
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Case 2.2.2 we must transfer a file.");
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Early final removal set to : " << boolalpha << getEarlyFinalRemoval());
                    const curl_version_info_data *d = curl_version_info(CURLVERSION_NOW);

                    if( d->version_num >= 0x071800 ) // PRQA S 4400
                    {
                        curl.put(getRemoveTargetBeforeTransfer(), _stopController);
                    }
                    else
                    {
                        bool removeAlso = false ;
                        if (getRemoveTargetBeforeTransfer()) {
                            BandPriority no_priority ;
                            FtpCurl innercurl(no_priority, _targetSite, "", "");
                            removeAlso = innercurl.existRemoteFile(true, _stopController, false);
                            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Exist Remote File ? " << innercurl.str() << " : " << boolalpha << removeAlso);
                        }
                        curl.put(removeAlso, 0, _stopController);
                    }
                }
            }
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Transfer ended.");
            if (conf::CurlConfigurerSingleton::instance()->getEnableStatistics()) {
                curl.getStats(_statistics);
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "_statistics.size(): " << _statistics.size());
            }
        } 
        else if (_targetSite.isLocalhost()) 
		{
			// Case 3. The target site is a local host, source is not local !!! 
			// then we have to make a get().
		
		    if ( 0 != _streamerId ) {
                ACS_THROW(exCriticalException() << "Streamer id set to " << _streamerId << ". On-the-fly mode not supported for remote to local transfers." );
            }

            _sourceSite.setIsDir(RemoteSiteUtility::isDir(_sourceSite, _source_http_response_code));
			_source_http_response_code = 0 ;
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "localTransfer: the target site is a local host, thus we must make a get().");
            string localPath = _targetSite.getPath();
            FtpCurl curl(*this,_sourceSite);
            curl.setFullLocalFilename(localPath);
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "localTransfer: " << localPath);
            
            const conf::CurlConfigurer* configurer =  conf::CurlConfigurerSingleton::instance();
            FtpCurl::FtpCurlPermissions ftpCurlPerm;
            if (configurer->changePermissions()) {
                conf::CurlConfigurer::ChangeLocalPermissions const& confPerm = configurer->getPermissions();
                if (configurer->isActivatedChangeMode()) {
                    ftpCurlPerm.permissions = confPerm.getMode();
                }
                if (configurer->isActivatedChangeOwner()) {
                    std::string user = getUser();
                    std::string group = getGroup(user);
                    
                    getUID(user, &ftpCurlPerm.userId);
                    getGID(group, &ftpCurlPerm.groupId);
                }
            }
            
            if (_sourceSite.getIsDir()) {
                //Case 3.1.0. we have to transfer a directory.
				try
				{ 
            	    curl.getDir(localPath, ftpCurlPerm, _sourceSite.getPath(), _stopController);
				}
				catch(const std::exception& e) // NOSONAR - any exception 
				{
					_source_http_response_code = curl.getHttpResponseCode() ;
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Got exception while transferring " << e.what() << " - Source HTTPResponseCode set to " << _source_http_response_code);
					throw ;
				}
            } 
            else {
                //Case 3.2.0. we have to transfer a file to local destination.
				try
				{ 
					if (getRemoveTargetBeforeTransfer()) 
					{
						// Removal of temporary target name (if supported) :
						if (_targetSite.hasTemporaryTargetName()) { curl.ldel( _targetSite.getTemporaryPath() ) ; }

						// Removal of target name:
						curl.ldel(_targetSite.getPath()) ; 

					}	// END: if (getRemoveTargetBeforeTransfer()) 
				
					curl.get(ftpCurlPerm, _stopController);
				}
				catch(const std::exception& e)  // NOSONAR - any exception 
				{
					std::ostringstream os;
					os << "Got exception while transferring " << e.what();
					try
					{
						_source_http_response_code = curl.getHttpResponseCode() ;
						os << " - Source HTTPResponseCode set to " << _source_http_response_code;
					}
					catch (const exException &)
					{
						; //do nothing
					}
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, os.str());
					throw ;
				}
            }
            if (conf::CurlConfigurerSingleton::instance()->getEnableStatistics()) {
                curl.getStats(_statistics);
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "_statistics.size(): " << _statistics.size());
            }
        }
        else { /* nothing */ }
    }
}

void CirculationCurl::transferFile() {

    CURLcode res = CURLE_OK ;
	_source_http_response_code = _target_http_response_code = 0 ; 
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Actual band priority:" << getBandPriority());
    
    StreamCurlThread sourceCurl(_stopController, res, *this);
    StreamCurlThread targetCurl(_stopController, res, *this);
    
    RemoteSite tempTargetSite = _targetSite; // this SHALL copy active/passive flag
    TemporaryFileService tempService(_targetSite.getRenameStrategy(), _targetSite.getTempnamevalue() );
 
    //we manage the temp path in this class, and not in the Curl class.
	tempTargetSite.setRenameStrategy(noTemp);
    TempRenameStrategy strategy = tempTargetSite.getRenameStrategy();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Renaming strategy:" << strategy);
    if (tempService.hasTemporaryTargetName()) {
        tempTargetSite.setPath(tempService.getTemporaryPath(_targetSite.getPath()));
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[TransferFileWithSource: " << _sourceSite << " target:" << tempTargetSite << "]");
    } 
    else {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[TransferFileWithSource: " << _sourceSite << " target:" << _targetSite << "]");
    }

    CurlStatistics::log(_sourceSite, "transfer");

    sourceCurl.setSite(_sourceSite);
    targetCurl.setSite(tempTargetSite);
 	
	// Set the destination overwrite flag (note that the flag is negative)
	targetCurl.setRemoveTargetBeforeTransfer(getRemoveTargetBeforeTransfer()) ;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "PipedStream created: " << _sourceSite.getPath() << "\n\t" << tempTargetSite.getPath());
    Buffer buffer;
    auto handle = std::make_unique<PipedStream>(buffer) ;
    sourceCurl.requestFutureOperation("streamget", handle.get(), handle.get(), _sourceSite.getPath());
    sourceCurl.start();

/*
    if (tempService.hasTemporaryTargetName()) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Rename target file from:" << tempTargetSite.getPath() << " to: " << _targetSite.getPath());
        FtpCurl curl(*this, _targetSite);
        curl.rename(tempTargetSite.getPath(), _targetSite.getPath());
    }
*/
// the above commented code is useless: the renaming is performed by passing a (tempTargetSite.getPath() != _targetSite.getPath())
    targetCurl.requestFutureOperation("streamput", handle.get(), handle.get(), tempTargetSite.getPath(), _targetSite.getPath());
    targetCurl.start();

    CURLcode res1 = sourceCurl.completeAndGetResult();
    CURLcode res2 = targetCurl.completeAndGetResult();

	if (res1 == CURLE_HTTP_RETURNED_ERROR) { _source_http_response_code = sourceCurl.getHttpResponseCode(); }
	if (res2 == CURLE_HTTP_RETURNED_ERROR) { _target_http_response_code = targetCurl.getHttpResponseCode(); }

    CurlStatistics::logResult(res2, _sourceSite, "transfer");
	
// if the source curl result corresponds to FtpCurlRecoverableErrorException try target curl one.
	{ // NOSONAR - scope
		FtpCurl ftpCurl(*this, _sourceSite);
		string msg = "Error during stream transfer: src error: " + sourceCurl.getLastErrorMessage() + "tgt error: " + targetCurl.getLastErrorMessage();
		try {
			// Check for errors in source site
			ftpCurl.checkCURLcode(res1, msg, __PRETTY_FUNCTION__, __LINE__);
		}
		catch( const FtpCurl::FtpCurlRecoverableErrorException & )
		{
			// The source site error was a recoverable one, check for errors in target site (unrecoverable has the priority)
			ftpCurl.checkCURLcode(res2, msg, __PRETTY_FUNCTION__, __LINE__);
			throw;
		}
		
		//No errors in the source site, check directly the target
		ftpCurl.checkCURLcode(res2, msg, __PRETTY_FUNCTION__, __LINE__);
	}
	
    if (conf::CurlConfigurerSingleton::instance()->getEnableStatistics() && 
        Application::exists() && 
        Application::instance()->getDefaultConnectionPool()) 
    {
        
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Merging statistics" );
        dbConnectionWrapper w(*Application::instance()->getDefaultConnectionPool()) ;
        dbConnection &conn = w ; // PRQA S 3050
        CirculationStatistics source(conn) ;
        sourceCurl.fillInCirculationStatistics(true, &source);
        CirculationStatistics dest(conn) ;
        targetCurl.fillInCirculationStatistics(false, &dest);
        //take the worst statistics
        source.mergeStatistics(conn, dest, false);

        source.setSourceSite(_sourceSite);
        source.setDestSite(_targetSite);
        _statistics.push_back(source);
    }
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File successfully transferred!");
}

void CirculationCurl::transferTreeDir() {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "[TransferTreeDirWithSource: " << getSourceSite() << " target:" << getTargetSite() << "]");
    try {
        RemoteDirVisitor visitor(visitRemoteFile, visitRemoteDir, this);
        visitor.visit(_sourceSite, _sourceSite.getPath());
        if (getTargetSite().hasTemporaryTargetName()) {
            FtpCurl curl(*this, getTargetSite());
            curl.rename(getTargetSite().getTemporaryPath(), getTargetSite().getPath(), _stopController);
        }
    } 
    catch (std::exception const& ex) {
        ACS_LOG_ERROR("Error in transfer dir: " << ex.what());
        throw;
    }
}

void CirculationCurl::visitRemoteFile(const string &remoteFile, const string &relativeRemoteFile, void* observer) { // NOSONAR - observer has to be generic
    auto thisObj = reinterpret_cast<CirculationCurl *>(observer); // NOSONAR - is a callback

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Target Dir :" << thisObj->getTargetSite());

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Is Change Strategy: " << thisObj->getTargetSite().hasTemporaryTargetName());
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Temporary path: " << thisObj->getTargetSite().getTemporaryPath());
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Dest path: " << thisObj->getTargetSite().getPath());
    string destDir =
            thisObj->getTargetSite().hasTemporaryTargetName() ? // PRQA S 3384
                    thisObj->getTargetSite().getTemporaryPath() : thisObj->getTargetSite().getPath();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Transfer: " << remoteFile << " to " << destDir + "/" + relativeRemoteFile);
    RemoteSite source = thisObj->getSourceSite();
    RemoteSite target = thisObj->getTargetSite();
    source.setPath(remoteFile);
    target.setRenameStrategy(noTemp);
    target.setPath(StringUtils::trimEnd(destDir, '/') + "/" + relativeRemoteFile);
    CirculationCurl curl(source, target, thisObj->_stopController);
    curl.transferFile();

    if (conf::CurlConfigurerSingleton::instance()->getEnableStatistics()) {
        curl.getStats(thisObj->_statistics);
    }

}

void CirculationCurl::visitRemoteDir(const string &, const string & , void* ) // NOSONAR 
{
    //does nothing.
}

File::CompressionMode CirculationCurl::streamTypeToCompressionMode(const std::string& streamType) {
	File::CompressionMode cm;
	if (StringUtils::equalsNoCase(streamType, "src_tar")) {
		cm = File::Plain;
	}
	else {
		ACS_THROW(exIllegalValueException("Unrecognized streamtype \"" + streamType + "\""));
	}
	
	return cm;
}


//
//Getters and Setters
//

void CirculationCurl::setSourceSite(const RemoteSite & site) 
{
    ThreadSafe::Lock lock(_mutex);
    _sourceSite = site;
}

const RemoteSite &CirculationCurl::getSourceSite() const noexcept // PRQA S 4120
{
    return _sourceSite;
}

const RemoteSite &CirculationCurl::getTargetSite() const noexcept // PRQA S 4120
{
    return _targetSite;
}

void CirculationCurl::setTargetSite(const RemoteSite & site) 
{
    ThreadSafe::Lock lock(_mutex);
    _targetSite = site;
}

void CirculationCurl::getStats(std::vector<curl::CirculationStatistics>& stats) {
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Storing " << _statistics.size() << " statistics.");
    stats.insert(stats.end(),_statistics.begin(), _statistics.end()) ;
    _statistics.clear();
}

string CirculationCurl::str() const 
{
    std::ostringstream os;
    if (isSimulated()) {
        os << "SIMUL " ;
    }
    os << "CirculationCurl[sourceSite=" << _sourceSite.str() << ",targetSite=" << _targetSite.str() << "priority:"
            << getBandPriority() << "]";
#warning MP: here could be useful asking the band shaper the actual speed it should go.
    return os.str();
}

std::ostream& operator<<(std::ostream& stream, const CirculationCurl& obj) 
{
    stream << obj.str();
    return stream; // PRQA S 4028
}

int CirculationCurl::getUID(std::string const& userId, uid_t* uid) // PRQA S 4020
{
    if (!uid) { 
        ACS_LOG_TRYCATCHNOTIFY_EX(exIllegalValueException("null pointer passed")) ; // PRQA S 3081
        return -1 ; 
    }
    char *endptr = nullptr ;
    *uid = uid_t(strtol(userId.c_str(), &endptr, 10)); /* Allow a numeric string */ // PRQA S 3000, 3081, 4400
    if (*endptr != '\0') { /* Was not pure numeric string */
        try {
            os::Password::passwd_w pw = os::Password::getPwNam(userId) ;
            *uid = pw.pw_uid ;
        }
        catch(const std::exception &x) { // NOSONAR - any exception
            ACS_LOG_NOTIFY_EX(x) ;
            return -1 ;
        }
    }
    return 0;
}

int CirculationCurl::getGID(std::string const& groupId, gid_t* gid) // PRQA S 4020
{
    if (!gid) { 
        ACS_LOG_TRYCATCHNOTIFY_EX(exIllegalValueException("null pointer passed")) ; // PRQA S 3081
        return -1 ; 
    }
    char *endptr = nullptr ;
    *gid = uid_t(strtol(groupId.c_str(), &endptr, 10)); /* Allow a numeric string */ // PRQA S 3000, 3081, 4400
    if (*endptr != '\0') { /* Was not pure numeric string */
        try {
            os::Password::group_w pw = os::Password::getGrNam(groupId) ;
            *gid = pw.gr_gid ;
        }
        catch(const std::exception &x) { // NOSONAR - any exception
            ACS_LOG_NOTIFY_EX(x) ;
            return -1 ;
        }
    }
    return 0;
}

int CirculationCurl::chown(string const& userId, string const& groupId, string const& filename) // PRQA S 4020
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "[chown]");
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "user id: " << userId);
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "group id: " << groupId);
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "filename:  " << filename);
    char *endptr = nullptr ;
    uid_t uid = uid_t(strtol(userId.c_str(), &endptr, 10)); /* Allow a numeric string */ // NOSONAR
    if (*endptr != '\0') { /* Was not pure numeric string */
        try {
            os::Password::passwd_w pwd = os::Password::getPwNam(userId) ;
            uid = pwd.pw_uid;
        }
        catch(const std::exception &x) { // NOSONAR - any exception
            ACS_LOG_NOTIFY_EX(x) ;
            ACS_LOG_ERROR("Error in getPwNam for filename " << filename << " user and group :" << userId << " " << groupId);
            return -1;
        }
    }
    gid_t gid = gid_t(strtol(groupId.c_str(), &endptr, 10)); /* Allow a numeric string */ // NOSONAR
    if (*endptr != '\0') { /* Was not pure numeric string */
        try {
            os::Password::group_w grp = os::Password::getGrNam(groupId) ;/* Try getting GID for groupname */
            gid = grp.gr_gid;
        }
        catch(const std::exception &x) { // NOSONAR - any exception
            ACS_LOG_NOTIFY_EX(x) ;
            ACS_LOG_ERROR("Error in getGrNam for filename " << filename << " user and group :" << userId << " " << groupId);
            return -1;
        }
    }
    if (::chown(filename.c_str(), uid, gid) == -1) {
        ACS_LOG_ERROR("Error in chown for filename " << filename << " user and group :" << userId << " " << groupId);
        return -1;
    }
    return 0;
}


void CirculationCurl::setSimulationMode(bool v) noexcept // PRQA S 4121
{
    _simulationMode = v ;
}

bool CirculationCurl::isSimulated() const noexcept // PRQA S 4120
{
    return _simulationMode ;
}

_ACS_END_NESTED_NAMESPACE
