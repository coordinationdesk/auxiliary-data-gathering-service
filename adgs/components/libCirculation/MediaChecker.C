// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2013, Advanced Computer Systems , Inc.
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
 *  Revision 7.0  2016/10/12 10:08:27  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.3  2014/08/06 16:16:00  lucio.pulvirenti
 *  S2PSGS-835: Exceptions notificated by the proper macro not to send stack on syslog.
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
 *  Revision 1.6  2012/12/07 17:47:50  micmaz
 *  updated media checker with new libFile.
 *
 *  Revision 1.5  2012/12/04 17:43:34  marpas
 *  getting rid of FilenameUtils class
 *
 *  Revision 1.4  2012/04/03 17:45:48  micmaz
 *  work in progress
 *
 *  Revision 1.3  2012/04/02 14:41:42  micmaz
 *  work in progress
 *
 *  Revision 1.2  2012/03/30 12:18:54  micmaz
 *  work in progress
 *
 *  Revision 1.1  2012/03/27 10:54:37  micmaz
 *  work in progress
 *
 *
 */

#include <MediaChecker.h>

#include <DateTime.h>

#include <vector>

#include <boost/bind.hpp> // PRQA S 1013 2
#include <boost/algorithm/string.hpp>


_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation)
using namespace std;
using namespace boost;

ACS_CLASS_DEFINE_DEBUG_LEVEL(MediaChecker);

MediaChecker::MediaChecker() {
    // empty
}

MediaChecker::~MediaChecker() throw () {
    // empty
}

bool MediaChecker::verifySignedFile(string const& absolutePath, SignedFile* signedFile) {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[verifySignedFileWithAbsolutePath: " << absolutePath << "]");
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Signed file name: " << signedFile->getName());
    string absFilename =
            (StringUtils::isBlank(absolutePath) || boost::equals(".", boost::trim_copy(absolutePath))) ? // PRQA S 3384
                    signedFile->getName() : absolutePath + "/" + signedFile->getName();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "absFilename : " << absFilename);
    ACS_COND_THROW(!File::exists(absFilename), // PRQA S 3081
            MediaCheckerException(string("The file to verify does not exist: ") + signedFile->getName()));
    signedFile->setVerificationDigest(MD5Signer::makeDigest(absFilename ));
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Digest : " << signedFile->getDigest());
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "verificationDigest : " << signedFile->getVerificationDigest());
    DateTime now;
    ostringstream time;
    now.asISO8601(time,0);
    signedFile->setVerificationDate(time.str());
    bool success = StringUtils::equals(signedFile->getVerificationDigest(), signedFile->getDigest());
    signedFile->setVerificationStatus(success ? SignedFile::verificationOk : SignedFile::verificationNok); // PRQA S 3380
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "verified file: " << *signedFile);
    return success;
}

bool MediaChecker::verify(string const& reportToCheck, string const& absolutePath, string const& outputReport) {
    bool success = true;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, 
            "VerifyWithReportToCheck: " << reportToCheck << " AndAbsolutePath: " << absolutePath << " AndOutputReport: "
                    << outputReport << "]");
    ACS_COND_THROW(!File::exists(reportToCheck)&& // PRQA S 3081
            File::isRegularFile(reportToCheck),
            MediaCheckerException(string("The report file to verify does not exist: ") + reportToCheck));
    ACS_COND_THROW(File::exists(outputReport) && File::isDir(outputReport), // PRQA S 3081
            MediaCheckerException(string("The output report file must not be folder!: ") + outputReport));
    ACS_COND_THROW(File::exists(outputReport) && // PRQA S 3081
            !StringUtils::equals(reportToCheck, outputReport),
            MediaCheckerException(string("The output report file already exists and it is not: ") + reportToCheck));
    vector<SignedFile> readFiles;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Reading " << reportToCheck);
    ifstream ifile;
    ifile.open(reportToCheck.c_str());
    MediaSerializer::fromStream(ifile, &readFiles);
    ifile.close();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "report read. Check each digest (" << readFiles.size() << " items).");
    try {
        for (vector<SignedFile>::iterator iter = readFiles.begin(); iter != readFiles.end(); ++iter) { // PRQA S 4238
            success = verifySignedFile(absolutePath, &(*iter)) && success ;
        }
    } catch (MediaCheckerException const& ex) {
		ACS_LOG_NOTIFY_EX(ex);
       	ACS_LOG_ERROR("An error occurred during media report checking; writing partial results to output report file.");
        success = false;
    }
    ofstream ofile;
    if (StringUtils::isBlank(outputReport)) {
        ofile.open(reportToCheck.c_str());
    }else{
        ofile.open(outputReport.c_str());
    }
    MediaSerializer::toStream(ofile, readFiles);
    ofile.close();

    return success;
}


_ACS_END_NESTED_NAMESPACE
