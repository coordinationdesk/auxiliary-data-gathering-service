// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2015, Advanced Computer Systems , Inc.
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
 *  Revision 5.11  2016/03/21 18:26:16  francesco.avanzi
 *  WORKINPROGRESS s2pdgs-1434
 *
 *  Revision 5.10  2016/03/21 10:20:08  francesco.avanzi
 *  rollback to previous state, WORKINPROGRESS
 *
 *  Revision 5.8  2015/03/10 15:04:07  lucio.pulvirenti
 *  S2PDGS-984: boolean _logError member added to IF together with related put and set method. Used to be passed to FtpCurl methods to avoid to log exceptions.
 *
 *  Revision 5.7  2015/03/06 16:48:03  lucio.pulvirenti
 *  S2PDGS-984: work in progress.
 *
 *  Revision 5.6  2014/06/23 13:38:50  marpas
 *  useless data removed
 *  using FtpCurl 5.47 interface
 *
 *  Revision 5.5  2014/04/14 09:27:21  marpas
 *  management of filters fixed and improved. test added
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
 *  Revision 5.2  2013/07/22 13:18:15  marpas
 *  types and best practices fixed
 *
 *  Revision 5.1  2013/07/15 16:48:06  marpas
 *  qa warnings fixed
 *  robusteness improved
 *
 *  Revision 5.0  2013/06/06 20:55:19  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.15  2013/04/05 09:47:15  marpas
 *  libDCUtility interface change
 *  flow control changed
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.14  2013/02/14 16:28:22  micmaz
 *  executed regression tests.
 *
 *  Revision 1.13  2012/11/08 16:52:47  micmaz
 *  updated to branch
 *
 *  Revision 1.12  2012/03/30 12:50:27  micmaz
 *  work in progress
 *
 *  Revision 1.11  2012/03/13 13:19:56  marpas
 *  abstract signature fixed
 *
 *  Revision 1.10  2012/02/07 10:25:56  micmaz
 *  removed warnings.
 *
 *  Revision 1.9  2011/07/01 16:40:44  micmaz
 *  changed the temporary file name strategy.
 *
 *  Revision 1.8  2011/06/14 09:36:05  micmaz
 *  corrected the temp file removing
 *
 *  Revision 1.7  2011/05/30 18:01:00  micmaz
 *  products with more files are sent in the same transaction
 *
 *  Revision 1.6  2011/05/12 09:09:16  micmaz
 *  work in progress
 *
 *  Revision 1.5  2011/04/15 15:33:20  micmaz
 *  work in progress...
 *
 *  Revision 1.4  2011/04/15 10:48:16  micmaz
 *  correct the verify and remove method in order to return a boolean: true if we must transfer the file / dir, false if we must skip.
 *
 *  Revision 1.3  2011/04/14 16:32:26  micmaz
 *  work in progress...
 *
 *  Revision 1.2  2011/04/11 12:24:53  micmaz
 *  work in progress
 *
 *  Revision 1.1  2011/04/05 17:14:43  micmaz
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

#ifndef ConsumableFtpCurl_H_
#define ConsumableFtpCurl_H_

#include <acs_c++config.hpp>
#include <FtpCurl.h>
#include <ConsumableItem.h>
#include <RemoteSite.h>

#include <boost/utility.hpp> // PRQA S 1013 2
#include <boost/shared_ptr.hpp>

#include <ostream>
#include <map>

_ACS_BEGIN_NESTED_NAMESPACE(acs, dc)

class ConsumableFtpCurl: public ConsumableItem { // PRQA S 2109

public:
    /**
     * Empty ctor.
     */
    explicit ConsumableFtpCurl(StopController& stopper);

    /**
     * Empty dtor.
     */
    virtual ~ConsumableFtpCurl()throw();
    /**
     * Transfer the file represented by the FtpCurl class.
     */
    virtual void doWork();
    /**
     * Getter for the primary key.
     */
    virtual std::string id() const;
    /**
     * Getter for the aggregate group.
     */
    virtual std::string groupId() const;
    /**
     * Getter for the directory size.
     * This size declare if this ftpcurl operation is a part of a set of ftpcurls (and groupid is the key of the set).
     */
    size_t size() const throw() ;
    /**
     * Setter for the directory size.
     */
    void setDirSize(size_t dirSize) throw() ;
    /**
     * Getter for the index field.
     */
    size_t index() const throw() ; // PRQA S 2502
    /**
     * Setter for the index field.
     */
    void setIndex(size_t index) throw() ;
    /**
     * Getter for the max number of attempts field.
     */
    size_t attempts() const throw() ;
    /**
     * Setter for the max number of attempts field.
     */
    void setAttempts(size_t attempts) throw() ;
    /**
     * Getter for the package id field (directory name).
     */
    const std::string &packageId() const throw() ;
    /**
     * Setter for the package id field (directory name).
     */
    void setPackageId(const std::string &packageId);
    /**
     * Initialize the interal state after the creation of the object.
     */
    void initFtpCurl(const curl::BandPriority &);

    static bool fileFilter(std::string const& );

    /**
     * To string method used for debug.
     */
    virtual std::string str() const;

    /**
     * Getter for the name attribute.
     */
    const std::string &name() const throw() ;

    /**
     * Getter for the FtpCurl pointer.
     */
    FtpCurl* cURL() ;

    /**
     * Getter for the FtpCurl pointer.
     */
    const FtpCurl* cURL() const ;
    /**
     * Rename target file name / directory name.
     */
    void rename(std::string const &srcFilename, std::string const &destFilename); // PRQA S 2502

    /**
     * Deletes the remote temporary file (in the remote temporary directory).
     */
    void deleteRemoteTempFile(bool silent);
    /**
     * Deletes the remote temporary file (in the remote temporary directory).
     */
    void deleteRemoteFinalFile(bool silent);

    /**
     * Deletes the remote file (if already exists) in the temporary directory.
     * This method is called before transferring the file.
     */
    void deleteRemoteFile();

    bool probe(std::string const &remotePath) const;
    /**
     *  Does exist the destination file in the destination directory?
     *  @return true if the destination  file in the destination  directory exits.
     */
    bool existRemoteFile();
    /**
     *  Does exist the destination directory?
     *  @return true if the destination directory exits.
     */
    bool existRemoteDir();

    /**
     * Verify if the remote file or directory already exists, in this case it will be removed.
     * @return true if the item must be transferred, false otherwise;
     * @throws Exception: if we cannot remove the destination file/directory.
     */
    bool verifyAndRemove(bool isDirectory, bool &removeLater);

    /**
     * Getter for the local path.
     */
    const std::string &localPath() const throw() ;
    /**
     * Setter for the local path.
     */
    void setLocalPath(const std::string &localPath);

    /**
     * Getter for the local file.
     */
    const std::string &localFile() const throw() ;
    /**
     * Setter for the local file.
     */
    void setLocalFile(const std::string &localFile);
    /**
     * Getter for the priority field.
     */
    virtual int getPriority() const ;
    
    /**
     * Setter for the priority field.
     */
    virtual void setPriority(int priority);

    const curl::RemoteSite &getSite() const throw() ;

    void setSite(curl::RemoteSite const& site);
    void setFtpSiteRepo(std::string const& ftpSiteRepo) ;
	
	bool getLogError() const;
	void setLogError(bool);

protected:

    void makeRemoteList(std::string const &remoteDir, std::map<std::string, std::string> &);

    void remoteTempDir(std::map<std::string, std::string> &);

private:

    void deleteRemoteFileInternal(const std::string &rempath, bool silent) ;

    ConsumableFtpCurl(); // not implemented
    ConsumableFtpCurl(ConsumableFtpCurl const& );
    ConsumableFtpCurl& operator=(ConsumableFtpCurl const& );
private:
    /**
     * @brief the internal FtpCurl holder.
     *
     *The internal FtpCurl must be unique because it is resposible to transfer the file, to handle
     *the statistics and to throw the exceptions. thus, even if the ConsumableFtpCurl object can be copied
     *anywhere, the internal FtpCurl will not be copied, but shared using this holder.
     */
    boost::shared_ptr<FtpCurl> _holder;

    curl::RemoteSite _site;
    std::string _localPath;
    std::string _localFile;
    int _priority;

    size_t _dirSize;
    size_t _index;
    size_t _attempts;
    std::string _packageId;
    std::string _ftpSiteRepo;
    StopController &_stopper ;
	bool _logError;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(ConsumableFtpCurl) ;
};
_ACS_END_NESTED_NAMESPACE

#endif /* ConsumableFtpCurl_H_ */
