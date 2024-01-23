/*
 *
 *  Copyright 1995-2012, Advanced Computer Systems , Inc.
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
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.2  2013/10/28 17:33:52  marpas
 *  work in progress
 *
 *  Revision 5.1  2013/07/15 17:21:12  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.0  2013/06/07 15:49:01  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.4  2012/11/29 16:55:05  micmaz
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

#ifndef _MediaChecker_H_
#define _MediaChecker_H_

#include <acs_c++config.hpp>
#include <MediaSigner.h>
#include <string>

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation) ;

/*
 *
 */
class MediaChecker { // PRQA S 2109
public:
    MediaChecker();
    ~MediaChecker() throw ();
private:
    MediaChecker& operator=(MediaChecker const& other);
    MediaChecker(MediaChecker const& other);
public:
    /**
     * Checks if the istream contains a correct report of file(s).
     * @param reportToCheck: the report to check;
     * @param absolutePath: the directory where the file is located;
     * @param checkedReport: the output file where to the checked report. If checkedReport is the empty string, then the Checker;
     * will overwrite the reportToCheck file;
     * @return true if all the files checked are correct, false otherwise.
     */
    bool verify(std::string const& reportToCheck, std::string const& absolutePath, std::string const& outputReport = "");

#ifdef ACS_TEST
public:
#else
private:
#endif
    /**
     * Verifies the signed file.
     * @param absolutePath: the directory where the file is located;
     * @param signedFile: in/out parameter: the Checker will insert into the SignedFile the <verification> tags;
     * @return true if success , false otherwise.
     */
    bool verifySignedFile(std::string const& absolutePath, SignedFile* signedFile);
    ACS_CLASS_DECLARE_DEBUG_LEVEL(MediaChecker) ;
};

exDECLARE_EXCEPTION(MediaCheckerException,exException); // PRQA S 2131, 2502

_ACS_END_NESTED_NAMESPACE;

#endif /* _MediaChecker_H_ */
