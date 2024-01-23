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
 *  Revision 5.15  2016/03/22 16:34:35  francesco.avanzi
 *  debug enhanced
 *
 *  Revision 5.14  2016/03/21 18:26:16  francesco.avanzi
 *  WORKINPROGRESS s2pdgs-1434
 *
 *  Revision 5.13  2016/03/21 10:20:08  francesco.avanzi
 *  rollback to previous state, WORKINPROGRESS
 *
 *  Revision 5.11  2015/03/10 15:04:07  lucio.pulvirenti
 *  S2PDGS-984: boolean _logError member added to IF together with related put and set method. Used to be passed to FtpCurl methods to avoid to log exceptions.
 *
 *  Revision 5.10  2015/03/09 16:59:58  lucio.pulvirenti
 *  S2PDGS-984 work in progress.
 *
 *  Revision 5.9  2015/03/06 16:48:03  lucio.pulvirenti
 *  S2PDGS-984: work in progress.
 *
 *  Revision 5.8  2014/06/23 13:38:50  marpas
 *  useless data removed
 *  using FtpCurl 5.47 interface
 *
 *  Revision 5.7  2014/05/21 09:25:15  marfav
 *  Removing useless boost foreach dependency
 *
 *  Revision 5.6  2014/04/14 09:27:21  marpas
 *  management of filters fixed and improved. test added
 *
 *  Revision 5.5  2014/02/26 14:40:01  lucio.pulvirenti
 *  String equlity now safer.
 *
 *  Revision 5.4  2014/02/14 13:54:46  marpas
 *  attempt to refactor band shaping mechanism
 *
 *  Revision 5.3  2013/10/30 17:50:45  marpas
 *  coding best practices applied
 *  robustness improved
 *  useless classes removed
 *  compilation and qa warnings removed
 *
 *  Revision 5.2  2013/10/28 17:33:29  marpas
 *  work in progress
 *
 *  Revision 5.1  2013/07/22 13:18:15  marpas
 *  types and best practices fixed
 *
 *  Revision 5.0  2013/06/06 20:55:19  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.31  2013/04/05 09:47:15  marpas
 *  libDCUtility interface change
 *  flow control changed
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.30  2013/02/14 16:28:22  micmaz
 *  executed regression tests.
 *
 *  Revision 1.29  2013/01/25 13:38:53  marpas
 *  using pattern::Singleton from libException
 *
 *  Revision 1.28  2012/12/05 10:33:56  marpas
 *  getting rid of FilenameUtils
 *
 *  Revision 1.27  2012/11/08 16:52:47  micmaz
 *  updated to branch
 *
 *  Revision 1.26  2012/03/30 12:50:27  micmaz
 *  work in progress
 *
 *  Revision 1.25  2012/03/29 10:47:49  micmaz
 *  work in progress
 *
 *  Revision 1.24  2012/03/13 13:19:56  marpas
 *  abstract signature fixed
 *
 *  Revision 1.23  2012/03/08 17:06:42  micmaz
 *  added media functionality
 *
 *  Revision 1.22  2012/02/07 10:25:55  micmaz
 *  removed warnings.
 *
 *  Revision 1.21  2011/12/15 10:49:27  micmaz
 *  removed unused import
 *
 *  Revision 1.20  2011/12/13 11:21:57  micmaz
 *  removed compilation warning
 *
 *  Revision 1.19  2011/09/23 09:14:42  micmaz
 *  removed unused headers.
 *
 *  Revision 1.18  2011/07/13 14:00:07  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.17  2011/07/05 11:47:40  micmaz
 *  work in progress
 *
 *  Revision 1.16  2011/07/01 17:30:54  micmaz
 *  correct the error_log test
 *
 *  Revision 1.15  2011/07/01 16:40:44  micmaz
 *  changed the temporary file name strategy.
 *
 *  Revision 1.14  2011/06/14 09:36:05  micmaz
 *  corrected the temp file removing
 *
 *  Revision 1.13  2011/05/30 18:01:00  micmaz
 *  products with more files are sent in the same transaction
 *
 *  Revision 1.12  2011/05/12 14:07:59  micmaz
 *  correct the directory transfer
 *
 *  Revision 1.11  2011/05/12 09:09:16  micmaz
 *  work in progress
 *
 *  Revision 1.10  2011/05/09 14:39:36  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.9  2011/05/03 14:03:40  micmaz
 *  internal exceptions thrown with error number (where it is possible) and retrow original exception and not the catched one.
 *
 *  Revision 1.8  2011/04/26 17:32:49  micmaz
 *  added logs.
 *
 *  Revision 1.7  2011/04/26 14:42:15  micmaz
 *  Updated logs and added removing of sources (if required)
 *
 *  Revision 1.6  2011/04/15 15:33:20  micmaz
 *  work in progress...
 *
 *  Revision 1.5  2011/04/15 10:48:16  micmaz
 *  correct the verify and remove method in order to return a boolean: true if we must transfer the file / dir, false if we must skip.
 *
 *  Revision 1.4  2011/04/14 16:32:26  micmaz
 *  work in progress...
 *
 *  Revision 1.3  2011/04/11 12:24:53  micmaz
 *  work in progress
 *
 *  Revision 1.2  2011/04/07 13:28:54  micmaz
 *  work in progress
 *
 *  Revision 1.1  2011/04/05 17:14:44  micmaz
 *  work in progress...
 *
 *  Revision 1.5  2011/03/23 10:04:03  micmaz
 *  work in progress...
 *
 *  Revision 1.4  2011/03/11 17:55:46  micmaz
 *  reworked time outs.
 *
 *  Revision 1.3  2011/03/08 18:15:26  micmaz
 *  Added the ovewrite flag to the configurer
 *
 *  Revision 1.2  2011/03/08 15:04:08  micmaz
 *  improved logs.
 *
 *  Revision 1.1  2011/01/21 18:04:34  micmaz
 *  added a copyable ftpCurl wrapper.
 *
 *
 *
 */

#include <ConsumableFtpCurl.h>
#include <CurlConfigurer.h>

#include <exMacros.h>

#include <DataConsumerConfigurer.h>

#include <File.h>

#include <exMacros.h>
#include <algorithm>
#include <boost/algorithm/string.hpp> // PRQA S 1013 3
#include <boost/tokenizer.hpp>

_ACS_BEGIN_NESTED_NAMESPACE(acs, dc)
using namespace std;
using namespace acs::curl;

ACS_CLASS_DEFINE_DEBUG_LEVEL( ConsumableFtpCurl);


ConsumableFtpCurl::ConsumableFtpCurl(StopController& stopper) :
    ConsumableItem(),
    _holder(),
    _site(RemoteSite::ftp),
    _localPath(),
    _localFile(),
    _priority(),
    _dirSize(1), 
    _index(0), 
    _attempts(0),
    _packageId (),
    _ftpSiteRepo(),
    _stopper(stopper),
	_logError(true) // PRQA S 2528
{
}

ConsumableFtpCurl::~ConsumableFtpCurl() throw() {}

bool ConsumableFtpCurl::fileFilter(string const& file) // PRQA S 4020
{
    const string filename = File::getFileName(file) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[fileFilter: " << file << "]");
    
    const string &mail_warning_style = conf::dcConfigurerSingleton::instance()->getMailWarningStyle() ;
    const string &by_mail_ext = conf::dcConfigurerSingleton::instance()->getByEmailExtension() ;
    if (not by_mail_ext.empty() ) {
        if (StringUtils::equalsNoCase(mail_warning_style, conf::DataConsumerConfigurer::XmlSlipEmailStyle)) {
            if (filename.find(by_mail_ext) != string::npos) {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "not kept: " << filename << " BY_MAIL_EXTENSION " << by_mail_ext);
                return false; // discard it
            }
        }
    }

    if (conf::dcConfigurerSingleton::instance()->getLogFilename() == filename) {
        return false ; // discard it
    }

    const string &filterList = conf::dcConfigurerSingleton::instance()->getFilterList() ;
    if ( not filterList.empty() ) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ignore list: " << filterList);
        using namespace boost;
        char_separator<char> sep ( ", " );
        tokenizer< char_separator<char> > tokens ( filterList, sep );
        for (tokenizer< char_separator<char> >::iterator tok_iter = tokens.begin();
             tok_iter != tokens.end(); ++tok_iter) {
            if (filename == *tok_iter) {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "not kept: " << filename << " filtered by : " << *tok_iter );
                return false; // discard it
            }
        }
    }
    return true ; // keep the file
}


void ConsumableFtpCurl::doWork() {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "file/dir to transfer:" << _localPath << "/" << _localFile);
    bool isDirectory = File::isDir(_localPath + "/" + _localFile); // PRQA S 4412
    bool removeLater ;
    if (verifyAndRemove(isDirectory, removeLater)) {
        if (isDirectory) {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Transferring local directory: " << id());
            cURL()->putDir(_localPath + "/" + _localFile, _site.getPath(), fileFilter, _stopper);
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "*****************");
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "*Directory successfully transferred: ");
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "*Id: " << id());
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "*****************");
        } else {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Transferring file (in the temporary directory): " << id());
            cURL()->put(removeLater, _stopper);
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "*****************");
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "*File successfully transferred: ");
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "*Id: " << id());
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "*****************");
        }
    }
}

bool ConsumableFtpCurl::verifyAndRemove(bool isDirectory, bool &removeLater) { // PRQA S 4020
    removeLater = false ;
    bool checkPreTransfer = conf::dcConfigurerSingleton::instance()->getCheckPreTransfer();
    bool overwrite_completed = conf::dcConfigurerSingleton::instance()->getOverwriteCompletedFiles() ;
    if (!isDirectory) {
        bool exists = false ;
        if (!_site.isLocalhost()) {
			// If enabled, this function checks the existence of the remote file and, if requested removes it (current behaviour)
			if (checkPreTransfer) {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Delete the temporary file: " << id());
				deleteRemoteTempFile(true); // no exceptions
				try {
					exists = existRemoteFile() ;
				}
				catch(exception &ex) {
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Cannot decide if the remote file already exist...try to transfer." << id() <<"\n" << ex.what() );
					return true ;
				}
				if (overwrite_completed) { // transfer MUST overwrite existing file
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Overwrite existing file (if any) true " << id());
					if(conf::dcConfigurerSingleton::instance()->getEarlyFinalRemoval()) { // should I remove existing file now ?
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Delete the destination file: " << id());
						if (exists) {
							try {
								deleteRemoteFinalFile(false); // exceptions
							}
							catch (exception &ex) {
								ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,
									"Could not delete destination file, " << "cannot start re-send of file in overwrite mode." << id() <<"\n"
									<< ex.what() );
								throw;
							}
						}
						else { // do not exists: nop
						}
					}
					else { // I could remove it, just before the rename if existed
						removeLater = exists ;
					}
				}
				else { // do not overwrite, so if a file exists it is the goal
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Do not overwrite existing file (if any): keep it " << id());
					if (exists) { // it exists: already transferred
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Skip transfer file: the package was already transferred:" << id() << " " << groupId());
						return false;
					}
					else { // it does not exist
						return true ;
					}
				}
			}
			// If disabled, removal will be performed directly by curl pre-quote/post-quote commands in a single connection
			else {
				removeLater = true;
				if (!overwrite_completed) {
					try {
						exists = existRemoteFile() ;
					} catch(exception &ex) {
						ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Cannot decide if the remote file already exist...try to transfer." << id() <<"\n" << ex.what() );
						return true ;
					}
					
					if (exists) { // it exists: already transferred
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Skip transfer file: the package was already transferred:" << id() << " " << groupId());
						return false;
					}
					else { // it does not exist
						return true ;
					}
				}
			}
        }
    } else {
        string path = cURL()->getSite().getPath();
        string temporaryPath = cURL()->getSite().getTemporaryPath ( path );
        try {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Delete temporary directory : " << temporaryPath);
            cURL()->delDirTree ( temporaryPath, _stopper, false );
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Deleted temporary directory : " << temporaryPath << " for id: " << id() );
        } catch ( std::exception const& ex ) {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Ignored..cannot delete the product temp dir." );
        }
        if ( existRemoteDir() ) {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "The remote dir exists for id: " << id() <<"\n\t path : " << path );
            if ( overwrite_completed ) {
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "And the user wants to Overwrite Already Completed Files.");
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Remove the remote dir (if any) for id: " << id() <<"\n\t path : " << path );
                try {
                    cURL()->delDirTree (path, _stopper, false );
                } catch ( std::exception const& ex ) {
                    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Ignored..cannot delete the product directory into temp dir." );
                }
            } else {
                //test if the destination file already exists in the destination directory.
                //this means that the file (and the whole package) was already transferred.
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Skip transfer dir: the package was already transferred:" << id() << " " << groupId() );
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "And the user does not want to Overwrite Already Completed Files.");
                return false;
            }
        }else{
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "The target dir does not exist for id: " << id() <<"\n\t path : " << path );
        }
    }
    return true ;
}

bool ConsumableFtpCurl::existRemoteFile() // PRQA S 4211
{
    BandPriority no_priority ;
    FtpCurl curl(no_priority, _site, _localPath, _localFile);
    bool result = curl.existRemoteFile(true, _stopper, _logError);
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Exist Remote File ? " << curl.str() << " : " << boolalpha << result);
    return result;
}

bool ConsumableFtpCurl::existRemoteDir() // PRQA S 4211
{
    BandPriority no_priority ;
    FtpCurl curl(no_priority, _site, _localPath, _localFile);
    bool result = curl.existRemoteFile(false, _stopper, _logError);
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Exist Remote Dir ? " << curl.str() << " : " << boolalpha << result);
    return result;
}


void ConsumableFtpCurl::deleteRemoteTempFile(bool silent) 
{
    deleteRemoteFileInternal(cURL()->getSite().getTemporaryDir()+cURL()->getSite().getTemporaryFile(), silent) ;
}

void ConsumableFtpCurl::deleteRemoteFinalFile(bool silent) 
{
    deleteRemoteFileInternal(cURL()->getSite().getTemporaryDir()+cURL()->getSite().getRemoteFileName(), silent) ;
}

void ConsumableFtpCurl::deleteRemoteFileInternal(const string &rempath, bool silent) 
{
    try {
        RemoteSite site = cURL()->getSite();
        site.setPath(rempath);
        BandPriority no_priority ;
        FtpCurl curl(no_priority, site);
        curl.del();
    } catch (std::exception const& ex) {
        if (!silent ) { throw ; }
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Cannot delete the path " << rempath << " in the temporary directory " << ex.what());
    }
}

void ConsumableFtpCurl::deleteRemoteFile() 
{
    try {
        cURL()->del();
    } 
    catch (std::exception const& ex) {
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Cannot delete the destination file in the temporary directory " << ex.what());
    }
}

void ConsumableFtpCurl::initFtpCurl(const BandPriority &bp) 
{
    FtpCurl* curl = new FtpCurl(bp, _site, _localPath, _localFile);
    _holder.reset(curl);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Created cURL: " << curl->str());
}

FtpCurl* ConsumableFtpCurl::cURL() // PRQA S 4211
{
    return _holder.get();
}

const FtpCurl* ConsumableFtpCurl::cURL() const 
{
    return _holder.get();
}

bool ConsumableFtpCurl::probe(string const &remotePath) const 
{
    curl::Curl curl;
    curl.setSite(_site);
    return curl.probe(remotePath);
}

void ConsumableFtpCurl::makeRemoteList(string const &remoteDir, map<string, string> &result) // PRQA S 4211
{
    result.clear() ;
    string str ;
    try {
        BandPriority no_priority ;
        FtpCurl curl(no_priority, _site, _localPath, _localFile);
        str = curl.list();
    } catch (acs::FtpCurl::FtpCurlException const& ex) {
        //empty.
    }
    typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
    boost::char_separator<char> sep(" \n");
    Tokenizer tokens(str, sep);
    for (Tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter) {
        result[*tok_iter] = *tok_iter;
    }
}

void ConsumableFtpCurl::remoteTempDir(map<string, string> &m) 
{
    makeRemoteList(_site.getTemporaryPath(), m);
}

void ConsumableFtpCurl::rename(string const &srcFilename, string const &destFilename) 
{
    cURL()->rename(srcFilename, destFilename);
}

const string & ConsumableFtpCurl::name() const throw() // PRQA S 4120
{
    return _localFile;
}

const string &ConsumableFtpCurl::localPath() const throw() // PRQA S 4120
{
    return _localPath;
}

void ConsumableFtpCurl::setLocalPath(const string & localPath) // PRQA S 4121
{
    _localPath = localPath;
}

const string &ConsumableFtpCurl::localFile() const throw() // PRQA S 4120
{
    return _localFile;
}

void ConsumableFtpCurl::setLocalFile(const string &localFile)   // PRQA S 4121
{
    _localFile = localFile;
}

int ConsumableFtpCurl::getPriority() const // PRQA S 4120
{
    return _priority;
}

void ConsumableFtpCurl::setPriority(int priority) 
{
    int maxPrio = acs::conf::dcConfigurerSingleton::instance()->getMaxPriority() ;
    _priority = max(0, min(priority, maxPrio));
}

const curl::RemoteSite &ConsumableFtpCurl::getSite() const throw() // PRQA S 4120
{
    return _site;
}

void ConsumableFtpCurl::setSite(curl::RemoteSite const& site) // PRQA S 4121
{
    _site = site;
}

string ConsumableFtpCurl::str() const 
{
    std::ostringstream os;
    os << "[" << _packageId << "] From: " << _localPath << "/" << _localFile << " To: " << _site.getHost() << _site.getPath();
    return os.str();
}

string ConsumableFtpCurl::id() const  // PRQA S 4120
{
    return str();
}

string ConsumableFtpCurl::groupId() const // PRQA S 4120
{
    return _packageId;
}

size_t ConsumableFtpCurl::size() const throw()  // PRQA S 4120
{
    return _dirSize;
}

void ConsumableFtpCurl::setDirSize(size_t dirSize)  throw()  // PRQA S 4121
{
    _dirSize = dirSize;
}

size_t ConsumableFtpCurl::index() const throw()  // PRQA S 4120
{
    return _index;
}

void ConsumableFtpCurl::setIndex(size_t index) throw()  // PRQA S 4121
{
    _index = index;
}

size_t ConsumableFtpCurl::attempts() const throw()  // PRQA S 4120
{
    return _attempts;
}

void ConsumableFtpCurl::setAttempts(size_t attempts) throw() // PRQA S 4121
{
    _attempts = attempts;
}

const string &ConsumableFtpCurl::packageId() const throw() // PRQA S 4120
{
    return _packageId;
}

void ConsumableFtpCurl::setPackageId(const string &packageId)  // PRQA S 4121
{
    _packageId = packageId;
}

void ConsumableFtpCurl::setFtpSiteRepo(string const& ftpSiteRepo) // PRQA S 4121
{
    _ftpSiteRepo = ftpSiteRepo;
}

bool ConsumableFtpCurl::getLogError() const // PRQA S 4121
{
    return _logError;
}

void ConsumableFtpCurl::setLogError(bool v) // PRQA S 4121
{
    _logError = v;
}


_ACS_END_NESTED_NAMESPACE
