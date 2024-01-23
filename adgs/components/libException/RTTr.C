// PRQA S 1050 EOF
/*

    Copyright 2011-2018, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. Base Library $

    $Id$

    $Author$

    $Log$
    Revision 5.3  2013/09/26 18:09:35  marpas
    robustness strongly improved:
    singleton functions (with static variables inside) used
    some private members are now accessed only through the above technique
    exRTDebuggable classes do not longer unregister themselves from exRTDebug.
    it was a logic error: sooner or later the container class (exRTDebug) will throw away memory in any case.
    The above changes will lead in a more efficient process closure.
    moreover the exException class now uses the reentrant method strerror_t to obtain the errno explanation.

    Revision 5.2  2013/09/26 10:23:06  marpas
    debug strings now removes '(' and ')' to support anonymous namespace.

    Revision 5.1  2013/06/06 08:28:13  marpas
    coding best practices applied

    Revision 5.0  2013/06/04 15:34:59  marpas
    introducing new log macros, debug macros
    work in progress

    Revision 1.1.1.1  2011/05/26 17:02:39  marpas
    Creating libAcsBase



*/


#include <RTTr.hpp>
#include <algorithm>


_ACS_BEGIN_NESTED_NAMESPACE(acs,strings)

using std::string ;

namespace {
    string &replace_all(string &sf, const string &stbr, const string &str) // PRQA S 4020, 4284
    {
      if (stbr.empty()) { return sf ; } // PRQA S 4028
      size_t len = stbr.length() ;
      size_t f = 0 ;
      while((f = sf.find(stbr, f)) != string::npos) { sf.replace(f, len, str) ; } // PRQA S 3227, 3278, 3902, 4074, 4244, 4290, 4680, 4681, 4682, 4684
      return sf ; // PRQA S 4028
    }

}

bool RTTr::tr_(char c)
{
    return (
              (c==':') ||
              (c== '>') ||
              (c == '<') ||
              (c == ',') ||
              (c == '&') ||
              (c == '*')
           )  ;
}


bool RTTr::rm_(char c)
{
    return (
               (c==' ') ||
               (c== ')') ||
               (c== '(')
           ) ;
}
RTTr::RTTr(const std::string &s, const std::string &sfx) : t_(s)
{

    replace_all(t_,"acs::","") ;// remove any 'acs::' instance from the string
    std::replace_if(t_.begin(), t_.end(), tr_, '_'); // transform ":<>,&*" into '_'
    // we do not want ' ', '(' and ')' in order to set an env var.
    // note, ellipses are used from anonymous namespaces
    t_.erase(std::remove_if (t_.begin(), t_.end(), rm_), t_.end());
    if (not sfx.empty()) {
        size_t d_ = t_.rfind(sfx) ;
        if ((d_ == std::string::npos) || // avoid A_DEBUG_DEBUG // PRQA S 3231, 3278, 3084 2
            (d_ != (t_.length()-sfx.length()))) {
            t_ += sfx;
        }
    }
}

RTTr::operator const char *() const ACS_NOEXCEPT {return t_.c_str() ; }

_ACS_END_NESTED_NAMESPACE
