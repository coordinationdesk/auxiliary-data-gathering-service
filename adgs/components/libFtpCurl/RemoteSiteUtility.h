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
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.6  2015/03/06 16:45:56  lucio.pulvirenti
 *  S2PDGS-984: work in progress.
 *
 *  Revision 5.5  2014/10/10 10:49:01  lucio.pulvirenti
 *  S2PDGS-939: composeUrl method added to centralize url normalization w.r.t. body + filename.
 *
 *  Revision 5.4  2014/07/03 10:00:24  marpas
 *  method signature rationalized
 *
 *  Revision 5.3  2014/02/13 19:15:02  marpas
 *  test improved
 *
 *  Revision 5.2  2014/02/13 17:59:28  marpas
 *  Crypto class cleansed - btw a memory leak icrypting/decrypting passwords has been corrected
 *  test added
 *
 *  Revision 5.1  2013/10/28 13:42:14  marpas
 *  cleaning macros and curl versions.
 *
 *  Revision 5.0  2013/06/06 18:08:22  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.5  2013/05/29 10:53:59  marpas
 *  interface rationalization,
 *  coding best practices applied,
 *  qa rules
 *
 *  Revision 2.4  2013/04/05 09:44:15  marpas
 *  coding best practices applied
 *  qa warnings (partial)
 *  libDCUtility interface update
 *
 *  Revision 2.3  2012/11/29 16:28:20  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.2  2012/11/27 13:28:33  micmaz
 *  added quality tool directives.
 *
 *  Revision 2.1  2012/04/03 17:46:10  micmaz
 *  work in progress
 *
 *  Revision 2.0  2011/10/18 14:43:52  marpas
 *  new statistics adopted
 *
 *  Revision 1.4  2011/09/29 18:33:04  marpas
 *  fixing codecheck crashes
 *
 *  Revision 1.3  2011/09/29 16:46:53  marpas
 *  rules fixing
 *
 *  Revision 1.2  2011/07/14 15:19:32  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.1  2011/07/01 16:42:12  micmaz
 *  changed the temporary file name strategy.
 *
 *
 *
 *
 */

#ifndef _RemoteSiteUtility_H_
#define _RemoteSiteUtility_H_

#include <acs_c++config.hpp>

#include <FtpCurl.h>
#include <RemoteSite.h>
#include <StopController.h>
#include <vector>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) ;


class RemoteSiteUtility { // PRQA S 2109

public:

    exDECLARE_EXCEPTION(CryptException,exException) ;    // PRQA S 2131, 2502

    static bool isDir(RemoteSite const& site, long int &http_response_code, StopController const& stopController = StopController());
    static bool isDir(RemoteSite const& site, StopController const& stopController = StopController()) 
	{ long int http_response_code ; return isDir(site, http_response_code, stopController) ; }
    static RemoteResourceType  probeSite(RemoteSite* site,StopController const& stopController = StopController(), bool emitExc = false);
    static std::string encrypt(std::string const& password); // PRQA S 2502
    static std::string decrypt(std::string const& encrypted_password); // PRQA S 2502
	 /**
     * Compose url by removing exceeding '/' between url body and filename, e.g. ftp://hsm-dev/tmp/TEST_BASKET// + /IFR_WW3-GLOBAL-30MIN-20040724t03z-uss.nc -> ftp://hsm-dev/tmp/TEST_BASKET/IFR_WW3-GLOBAL-30MIN-20040724t03z-uss.nc
     * @param url body with no filename
     * @param filename
     * @return composed url
     */
	static std::string composeUrl(std::string const &, std::string const &); // PRQA S 2502

#ifdef ACS_TEST
public:
#else
private:
#endif

    static void encodeBASE64(unsigned const char* binary_string, 
                             unsigned int binary_string_length, 
                             std::vector<char> &base64_encrypt);

    static void decodeBASE64(const char* binary_string, 
                             size_t binary_string_length, 
                             std::vector<unsigned char> &base64_decrypt ) ;

private:
    RemoteSiteUtility();
    ~RemoteSiteUtility()throw();
    RemoteSiteUtility& operator=(RemoteSiteUtility const& other);
    RemoteSiteUtility(RemoteSiteUtility const& other);
private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(RemoteSiteUtility);
};


_ACS_END_NESTED_NAMESPACE;

#endif /* _RemoteSiteUtility_H_ */
