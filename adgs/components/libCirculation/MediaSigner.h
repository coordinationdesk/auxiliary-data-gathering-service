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
 *  Revision 5.4  2014/06/05 18:48:09  marpas
 *  getting rid of asserts
 *
 *  Revision 5.3  2013/10/28 17:33:52  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/07/16 14:52:16  marpas
 *  coding best practices applied,
 *  performances and robustness improved
 *
 *  Revision 5.1  2013/07/15 17:21:12  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.0  2013/06/07 15:49:01  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.13  2013/04/05 09:51:07  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.12  2013/03/14 15:56:14  marpas
 *  dbConnectionPool concept enforced
 *
 *  Revision 1.11  2012/12/07 17:47:50  micmaz
 *  updated media checker with new libFile.
 *
 *  Revision 1.10  2012/12/04 17:43:34  marpas
 *  getting rid of FilenameUtils class
 *
 *  Revision 1.9  2012/11/29 16:55:05  micmaz
 *  work in progress
 *
 *  Revision 1.8  2012/04/03 17:45:47  micmaz
 *  work in progress
 *
 *  Revision 1.7  2012/04/02 14:41:42  micmaz
 *  work in progress
 *
 *  Revision 1.6  2012/03/30 12:18:54  micmaz
 *  work in progress
 *
 *  Revision 1.5  2012/03/27 10:54:16  micmaz
 *  work in progress
 *
 *  Revision 1.4  2012/03/09 17:43:33  micmaz
 *  added media functionality
 *
 *  Revision 1.3  2012/03/08 15:12:03  micmaz
 *  added media functionality
 *
 *
 *
 */

#ifndef _MediaSigner_H_
#define _MediaSigner_H_

#include <acs_c++config.hpp>
#include <Circulation.h>

#include <File.h>
#include <Dir.h>
#include <FileMD5Th.h>
#include <DateTime.h>
#include <StringUtils.h>
#include <RemoteSiteFactory.h>
#include <RemoteSite.h>
#include <CirculationDBService.h>
#include <dbConnection.h>


#include <boost/algorithm/string.hpp> // PRQA S 1013 3 
#include <boost/utility/enable_if.hpp>
#include <boost/tokenizer.hpp>

#include <vector>
#include <string>


_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation)

//
//Signed file report.
//
class SignedFile { // PRQA S 2109
public:
    SignedFile();
    ~SignedFile() throw () {
    }
    explicit SignedFile(rsResourceSet& resultSet, std::string const& tagName = mediaTag);
    SignedFile(SignedFile const& other);
    SignedFile& operator=(SignedFile const& other);
    bool operator==(SignedFile const& other); // PRQA S 2070
    std::string str(bool full = false) const;
private:
    std::string _sourceHostId;
    std::string _destinationHostId;
    std::string _circulationHost;
    std::string _circulationId;
    std::string _creationDate;
    std::string _lastModificationDate;
    std::string _name;
    std::string _digest;
    std::string _digestType;
    std::string _verificationDigest;
    std::string _verificationStatus;
    std::string _verificationDate;
public:
    std::string getCirculationHost() const;
    std::string getCirculationId() const;
    std::string getCreationDate() const;
    std::string getDestinationHostId() const;
    std::string getDigest() const;
    std::string getDigestType() const;
    std::string getLastModificationDate() const;
    std::string getName() const;
    std::string getSourceHostId() const;
    std::string getVerificationDate() const;
    std::string getVerificationDigest() const;
    std::string getVerificationStatus() const;
    void setCirculationHost(std::string const& circulationHost);
    void setCirculationId(std::string const& circulationId);
    void setCreationDate(std::string const& creationDate);
    void setDestinationHostId(std::string const& destinationHostId);
    void setDigest(std::string const& digest);
    void setDigestType(std::string const& digestType);
    void setLastModificationDate(std::string const& lastModificationDate);
    void setName(std::string const& name);
    void setSourceHostId(std::string const& sourceHostId);
    void setVerificationDate(std::string const& verificationDate);
    void setVerificationDigest(std::string const& verificationDigest);
    void setVerificationStatus(std::string const& verificationStatus);

    // PRQA S 2100 L1
    static std::string const nameKey;
    static std::string const digestKey;
    static std::string const circulationHostKey;
    static std::string const circulationIdKey;
    static std::string const creationDateKey;
    static std::string const lastModificationDateKey;
    static std::string const mediaTag;
    static std::string const reportTag;
    static std::string const sourceHostIdKey;
    static std::string const destinationHostIdKey;
    static std::string const listOfFilesKey;
    static std::string const verificationKey;
    static std::string const verificationDateKey;
    static std::string const verificationDigestKey;
    static std::string const verificationStatusKey;
    static std::string const verificationOk;
    static std::string const verificationNok;
    // PRQA L:L1
};

std::ostream& operator<<(std::ostream& stream, SignedFile const& obj);
exostream& operator<<(exostream& stream, SignedFile const& obj); // PRQA S 2072
/**
 * MediaSerializer: Helper used to serialize/deserialize the SignedFile files.
 */
class MediaSerializer {
public:
    MediaSerializer() throw () {}
    ~MediaSerializer() throw () {}
    MediaSerializer(const MediaSerializer &) throw() {}
    MediaSerializer& operator= (const MediaSerializer &) throw() { return *this ; } // PRQA S 4072
    
    static std::ostream& toStream(std::ostream& stream, std::vector<SignedFile> const& obj);
    static std::istream& fromStream(std::istream& stream, std::vector<SignedFile>* files);
private:

    static void readStream(std::istream& stream, std::ostringstream* fileContent);
};

/**
 * Signs a single file or a complete directory with a given signer algorithm.
 * This Template is used to sign Media transfers.
 * The generic type SignAlgorithm implements the algorithm used (MD5, SHA-1, etc...) to sign the files.
 */
template<typename SignAlgorithm>
class MediaSigner { // PRQA S 2109
public:
    MediaSigner() throw() {}
    ~MediaSigner() throw () {}
#ifdef ACS_TEST
public:
#else
private:
#endif
    MediaSigner(MediaSigner const& other); // not implemented
    MediaSigner& operator=(MediaSigner const& other); // not implemented

    /**
     * Signs a single file.
     * @param circ: the circulation queue element;
     * @param filename: the file name. If it was a relative path it is transformed in absolute path;
     * @param signedFile: output parameter: the signed file.
     */
    void signFile(dbConnection &conn, circulation::Circulation const& circ, std::string const& filename, SignedFile* signedFile) {
        ACS_COND_THROW(StringUtils::isBlank(filename), exIllegalValueException("file name is blank")) ; // PRQA S 3081
        ACS_COND_THROW(not File::exists(filename) ||  // PRQA S 3081
                       not File::isReadable(filename) || 
                       not File::isRegularFile(filename),
                exIOException("File to sign does not exist/not readable/not file")) ;
        ACS_LOG_INFO("[signFileWithCirculation: " << circ.str(conn,false) << " AndFilename:" << filename << " ]");
        std::string absoluteFilename = File::getAbsolutePath(filename);
        ACS_LOG_INFO("Absolute Filename :" << absoluteFilename);
        std::stringstream hostId;
        bool null = false ;
        hostId << circ.getHostId(null, conn);
        signedFile->setCirculationHost(hostId.str());
        std::stringstream circId;
        circId << circ.getId();
        signedFile->setCirculationId(circId.str());
        {
            std::ostringstream time;
            circ.getCreationDate(null, conn).asISO8601(time, 0);
            signedFile->setCreationDate(time.str());
        }
        {
            std::ostringstream time;
            circ.getModificationDate(null, conn).asISO8601(time, 0);
            signedFile->setLastModificationDate(time.str());
        }
        std::string destPath = StringUtils::substringAfter(circ.getToURL(null, conn),
                curl::RemoteSite::media + curl::RemoteSite::protocolSeparator);
        ACS_LOG_INFO("Dest path: " << destPath);
        if (File::isRegularFile(destPath)) {
            signedFile->setName(File::getFileName(absoluteFilename));
        } else {
            //Write the filename into the report check using the circulation dest url: the path must be
            //relative to the ToURL value.
            //Retrieve the destination path from the circulation queue:
            //removing trailing slash.
            destPath = StringUtils::trimEnd(destPath, '/');
            //Get the parent directory.
            destPath = StringUtils::substringBeforeLast(destPath, "/");
            ACS_COND_THROW(destPath.empty(), exIllegalValueException("Error in url path" + circ.getToURL(null, conn)) ); // PRQA S 3081
            std::string relativeName = StringUtils::substringAfter(absoluteFilename, destPath);
            relativeName = StringUtils::trimBegin(relativeName, '/');
            ACS_LOG_INFO("Relative Path: " << relativeName);
            signedFile->setName(relativeName);
        }
        ACS_LOG_INFO("Name Set: " << signedFile->getName());
        signedFile->setDigestType(SignAlgorithm::getSignerName());
        signedFile->setDigest(makeDigest(absoluteFilename));
        std::stringstream destHostId;
        destHostId << circ.getDestHostId(null, conn);
        signedFile->setDestinationHostId(destHostId.str());

        curl::RemoteSite from = curl::RemoteSiteFactory::makeSite(circ.getFromURL(null, conn));
        if (!from.isLocalhost()) {
            ACS_LOG_INFO("Find from host id . ");
            const std::string &key = conn.getKey() ;
            KnownHostQuery hosts(key) ;
            {
                dbQueryParameters pp(key) ;
                pp.addParameter(dbParam(key, "t_knownhosts","host_name", dbParam::EQUAL_TO, from.getHost())) ;
                hosts.addParameter(pp) ;
            }
            hosts.doQuery(conn);
            if (not hosts.empty()) {
                std::ostringstream host_id ; 
                host_id << hosts[0]->getId() ;
                signedFile->setSourceHostId(host_id.str());
                ACS_LOG_INFO("From host id found: " << hosts[0]->getId());
            } else {
                ACS_LOG_WARNING("From host id not found: leave it blank " << from.getHost());
            }
        } else {
            signedFile->setSourceHostId(signedFile->getCirculationHost());
        }
        if (circ.nullDestHostId(conn)) {
            curl::RemoteSite to = curl::RemoteSiteFactory::makeSite(circ.getToURL(null, conn));
            if (!to.isLocalhost()) {
                ACS_LOG_INFO("Find TO host id . ");
                const std::string &key = conn.getKey() ;
                KnownHostQuery hosts(key) ;
                {
                    dbQueryParameters pp(key) ;
                    pp.addParameter(dbParam(key, "t_knownhosts","host_name", dbParam::EQUAL_TO, to.getHost())) ;
                    hosts.addParameter(pp) ;
                }
                hosts.doQuery(conn);
                
                if (not hosts.empty()) {
                    std::ostringstream host_id ; 
                    host_id << hosts[0]->getId() ;
                    signedFile->setDestinationHostId(host_id.str());
                    ACS_LOG_INFO("TO host id found: " << hosts[0]->getId());
                } else {
                    ACS_LOG_WARNING("To host id not found: " << to.getHost());
                }
            } else {
                signedFile->setDestinationHostId(signedFile->getCirculationHost());
            }
        }
    }
    void signFolder(dbConnection &conn, circulation::Circulation const& circ, std::string const& foldername,
            std::vector<SignedFile>* signedFiles) {
        ACS_COND_THROW(StringUtils::isBlank(foldername), exIllegalValueException("dir name is blank")); // PRQA S 3081
        ACS_COND_THROW(not File::exists(foldername) ||// PRQA S 3081
                       not File::isReadable(foldername) || 
                       not File::isDir(foldername), 
                exIOException("File to sign does not exist/not readable/not dir"));
        DirTree dir(foldername, false);
        size_t dir_size = dir.size() ;
        for (size_t counter = 0; counter < dir_size; ++counter) {
            SignedFile sf;
            signFile(conn, circ, dir[counter], &sf);
            signedFiles->push_back(sf);
        }
    }
public:
    /**
     * Signs a file/folder.
     * @param circ: the circulation queue element;
     * @param path: the file/folder absolute path name;
     * @param signedFile: output parameter:  the signed file(s).
     */
    void sign(dbConnection &conn, circulation::Circulation const& circ, std::string const& path, std::vector<SignedFile>* signedFiles) {
        if (File::isDir(path)) {
            signFolder(conn, circ, path, signedFiles);
        } else {
            SignedFile signedFile;
            signFile(conn, circ, path, &signedFile);
            signedFiles->push_back(signedFile);
        }
    }
    static std::string makeDigest(std::string const& filename) {
        SignAlgorithm signer(filename, 0);
        signer.start();
        return signer.getResult();
    }

    std::ostream& toStream(std::ostream& stream, std::vector<SignedFile> const& obj) {
        return MediaSerializer::toStream(stream, obj);
    }
    std::istream& fromStream(std::istream& stream, std::vector<SignedFile>* obj) {
        return MediaSerializer::fromStream(stream, obj);
    }

private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(MediaSigner) ;
};

ACS_TEMPLATE_DEFINE_DEBUG_LEVEL(MediaSigner,SignAlgorithm) ;
//
//File MD5 Signer.
//

class FileMD5Signer: public FileMD5Th { // PRQA S 2153
public:
    FileMD5Signer(std::string const & filename, size_t safeThr) :
            FileMD5Th(filename, safeThr) 
    {}
    
    virtual ~FileMD5Signer() throw () {} // PRQA S 2131

    static std::string getSignerName() { return "MD5"; }
private:
    FileMD5Signer() ; // not implemented 
    FileMD5Signer(const FileMD5Signer &) ; // not implemented 
    FileMD5Signer& operator= (const FileMD5Signer &) ; // not implemented

};

//
//MD5 Signer.
//
typedef MediaSigner<FileMD5Signer> MD5Signer;

_ACS_END_NESTED_NAMESPACE

#endif /* _MediaSigner_H_ */
