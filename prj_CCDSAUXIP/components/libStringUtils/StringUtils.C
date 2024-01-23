/*

 Copyright 1995-2022, Exprivia SpA - DFDA-AS
 Via Della Bufalotta, 378 - 00139 Roma - Italy
 http://www.exprivia.com

 All Rights Reserved.

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
 the contents of this file may not be disclosed to third parties, copied or
 duplicated in any form, in whole or in part, without the prior written
 permission of Exprivia SpA

 $Prod: A.C.S. String Utility Library $

 $Id$

 $Author$

 $Log$
 Revision 1.54  2017/05/25 09:37:13  marpas
 introducing urldecode method

 Revision 1.53  2015/12/15 14:53:47  francesco.avanzi
 uncapitalize() removed

 Revision 1.52  2015/04/23 14:12:28  marpas
 pathJoin("a/","/b") fixed: now returns "a/b" instead of "a//b"

 Revision 1.51  2014/02/13 12:34:44  marpas
 test improved
 adopting ACS_THROW and ACS_COND_THROW macros

 Revision 1.50  2013/02/19 17:21:26  marpas
 qa rules

 Revision 1.49  2013/02/18 10:59:40  enrcar
 EC:: new method pathExtractFirstLevel (to allow recursive navigation of paths). Test added.

 Revision 1.48  2013/02/06 12:15:16  marpas
 introduced findNoCase functions family (similar to those in std::string::find)
 best coding practices enforced
 coverage on new functions test added

 Revision 1.47  2012/12/06 19:09:38  marpas
 coding best practices
 strong performance optimization
 bug fixing
 test coverage improved

 Revision 1.46  2012/12/05 17:52:59  marpas
 refactoring, qa rules, efficiency and coding best practices ... still in progress

 Revision 1.45  2012/12/03 16:51:47  marpas
 refactoring, appliying qa rules and coding best practices
 pay attention to performances and other optimization

 Revision 1.44  2012/02/27 10:45:12  marfav
 Added remove regexp methods

 Revision 1.43  2012/02/14 18:15:10  micmaz
 IN PROGRESS - issue S3PDGS-518: Gestione thread paralleli di DC su base ricevitore
 http://jira.acsys.it/browse/S3PDGS-518

 Revision 1.42  2012/01/31 16:40:52  marpas
 removing compiler warning

 Revision 1.41  2012/01/23 11:53:58  micmaz
 removed LOG(...)

 Revision 1.40  2011/10/26 09:17:50  enrcar
 EC:: added static method ipv6Normalize. All IPv6 are automatically normalized before use.

 Revision 1.39  2011/10/25 09:26:59  enrcar
 EC:: Added static method: buildIncrementalTokens

 Revision 1.38  2011/10/24 15:25:35  enrcar
 EC:: bug fixed converting a char to string (GCC 4.4.4)

 Revision 1.37  2011/10/24 13:03:32  enrcar
 EC:: expandPathToSubpaths static method added

 Revision 1.36  2011/06/07 13:30:54  giapas
 fixed missed include cstring

 Revision 1.35  2011/06/07 10:04:07  enrcar
 EC:: Added static method ipBelongsToSubnet (both IPv4 and IPv6)

 Revision 1.34  2011/05/16 13:22:37  enrcar
 EC:: pathJoin: misleading behaviour fixed: pathJoin("", "dir") now returns "dir" (instead of "/dir")

 Revision 1.33  2011/05/05 17:52:55  micmaz
 added compare() and equals()

 Revision 1.32  2011/05/04 09:53:09  micmaz
 added trimBegin and trimEnd functions

 Revision 1.31  2011/04/26 15:35:01  enrcar
 EC:: pathDisassemble: incorrent handling of hidden (.) files. FIXED.

 Revision 1.30  2011/04/07 13:33:19  micmaz
 added removeBegin and removeEnd methods

 Revision 1.29  2011/03/09 10:33:38  micmaz
 added reverse, endsWith and startsWith utility.

 Revision 1.28  2011/03/03 11:18:50  marpas
 method hash added

 Revision 1.27  2011/03/01 11:25:56  marpas
 GCC 4.4.x support added

 Revision 1.26  2011/02/14 10:17:32  enrcar
 EC:: pathJoin (2 values): BUG FIXED. pathJoin (3 values): Method added

 Revision 1.25  2011/02/11 14:51:21  enrcar
 EC:: pathJoin: method added

 Revision 1.24  2011/02/11 10:13:36  enrcar
 EC:: pathNormalize and pathDisassemble: added optional parameters to remove leading slash and leading dot

 Revision 1.23  2011/02/10 11:31:14  enrcar
 EC:: added methods: pathNormalize pathDisassemble pathAssemble

 Revision 1.22  2011/02/01 16:11:36  micmaz
 correct substring() function.

 Revision 1.21  2010/12/17 10:30:22  micmaz
 Changed substringBefore methods when the token is not found.

 Revision 1.20  2010/11/16 14:29:31  micmaz
 substitued size_t with ssize_t

 Revision 1.19  2010/11/12 14:04:24  micmaz
 added a new method in order to replace characters in a std::string without change it.

 Revision 1.18  2010/11/12 13:17:01  micmaz
 added new Functionalities.

 Revision 1.17  2010/10/26 08:43:49  micmaz
 indexOf returns int.

 Revision 1.16  2010/10/25 16:55:20  micmaz
 correct the substringAfter*() method: the last character must not be included.

 Revision 1.15  2010/10/25 16:09:34  micmaz
 vectors must be passed by reference.

 Revision 1.14  2010/10/25 15:50:37  micmaz
 added some new functions.

 Revision 1.13  2009/03/12 10:10:55  marpas
 rules violations fixed and or justified

 Revision 1.12  2009/03/02 14:06:38  marpas
 CODECHECK parsing - no regressions

 Revision 1.11  2008/07/08 13:24:58  marpas
 StringUtils::trim method added

 Revision 1.10  2008/04/16 00:39:31  marpas
 replace function overloaded to replace a char with a string

 Revision 1.9  2007/06/09 13:30:58  marpas
 replace method overloaded

 Revision 1.8  2007/06/09 13:23:15  marpas
 replace method added (useful to replace chars in string)

 Revision 1.7  2007/05/16 10:52:15  marpas
 lowercase, uppercase and capitalize methods implemented

 Revision 1.6  2006/06/22 14:44:40  marpas
 datetime dependency removed -

 Revision 1.5  2004/12/03 17:27:54  danalt
 Fixed ACS header

 Revision 1.4  2004/11/03 15:48:13  marfav
 Added DateTime support for generate filename parts

 Revision 1.3  2004/10/22 13:36:40  nicvac
 endl as a static constant string.

 Revision 1.2  2004/10/08 15:15:51  masdal
 added Tokenize method

 Revision 1.1.1.1  2004/09/20 16:47:24  marfav
 Import libStringUtils


 */

#include <acs_c++config.hpp>

#include <StringUtils.h>
#include <RegExp.h>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <suV.h>

_ACS_BEGIN_NAMESPACE(acs)


namespace {
    const suV version; // NOSONAR - versioning 
}

using namespace std;



locale StringUtils::_loc; // the C locale
const std::collate<char>& StringUtils::_coll = use_facet < collate<char> > (_loc);


int StringUtils::compareNoCase(const std::string& s1, const std::string& s2) // PRQA S 4020
{
    auto p1 = s1.begin();
    auto p2 = s2.begin();

    while (p1 != s1.end() && p2 != s2.end()) {
        if (toupper(*p1) != toupper(*p2)) {
            return (toupper(*p1) < toupper(*p2)) ? -1 : 1; // PRQA S 3382
        }
        ++p1;
        ++p2;
    }
    // here if the comparisons are good till now.
    
    if (s2.length() == s1.length()) 
        return 0 ; // same length - every char equal (case unsensitive)
        
    return s1.length() < s2.length() ?  -1 : 1 ;
}

int StringUtils::compareCase(const std::string& s1, const std::string& s2) 
{
    return s1.compare(s2) ;
}

bool StringUtils::equalsNoCase(const std::string& s1, const std::string& s2) {
    return (StringUtils::compareNoCase(s1, s2) == 0);
}

bool StringUtils::equals(const std::string& s1, const std::string& s2) {
    return s1 == s2 ;
}


string::const_iterator findNoCase_internal(const std::string& str, const std::string& s, std::string::size_type pos, string::size_type n) // PRQA S 4020
{
    size_t len = min(s.length(), n) ;
    if (len > 0) {
        return search (str.begin()+pos, str.end(), // PRQA S 3000 2
                       s.begin(), s.begin()+len,
                       [](char c1,  char c2){ return toupper(c1) == toupper(c2) ; }) ;
    }
    return str.end() ; 
}


string::size_type StringUtils::findNoCase(const std::string& str, const std::string& s, std::string::size_type pos )  // PRQA S 4020
{

    if (pos >= str.length()) {
        return string::npos ;
    }
    string::const_iterator p = findNoCase_internal(str, s, pos, s.length()) ; 
    
    if (p == str.end()) {
        return string::npos ;
    }
    else {
        return p - str.begin() ; // PRQA S 3000
    }
}

string::size_type StringUtils::findNoCase(const std::string &str, const char * s, std::string::size_type pos ) // PRQA S 4020
{
    if (s) { return findNoCase(str, string(s), pos) ; } // PRQA S 3081
    else { return string::npos ; }
}

string::size_type StringUtils::findNoCase(const std::string&str, const char * s, std::string::size_type pos, std::string::size_type n)  // PRQA S 4020
{
    if ((pos >= str.length()) || !s) {
        return string::npos ;
    }
    string::const_iterator p = findNoCase_internal(str, s, pos, n) ; 
    if (p == str.end()) {
        return string::npos ;
    }
    else {
        return p - str.begin() ; // PRQA S 3000
    }
}


string::size_type StringUtils::findNoCase(const std::string&str, char c, std::string::size_type pos)  // PRQA S 4020
{
    
    if (pos >= str.length()) {
        return string::npos ;
    }
    const auto uc = char(toupper(c)) ;
    auto p = std::find_if(str.begin()+pos, str.end(), [uc](char v){return uc == toupper(v) ; }) ; // PRQA S 3000, 3081
    if (p == str.end()) { 
        return string::npos ;
    }
    else {
        return p - str.begin() ; // PRQA S 3000
    }
}

std::string StringUtils::split(const std::string& input, std::vector<std::string>& result, bool allowEmptyTokens, char marker) {
    result.clear();

    size_t firstIndex = 0 ;
    size_t lastIndex = 0;

    while ((lastIndex = input.find(marker, firstIndex)) != std::string::npos) {
        const string &temp = input.substr(firstIndex, lastIndex - firstIndex); // PRQA S 3084

        // push empty tokens if requested
        if (!temp.empty() || allowEmptyTokens) {
            result.push_back(temp);
        }
        firstIndex = lastIndex + 1 ;    // PRQA S 3084
    }
    
    string remainder;
    if (firstIndex < input.size()) {
        remainder = input.substr(firstIndex, std::string::npos);
    }

    return remainder;
}

string StringUtils::split(const std::string& input, std::vector<std::string>& result, char marker) 
{
    return split(input, result, false, marker);
}

void StringUtils::Tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = " ") {
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);

    while ((std::string::npos != pos) || (std::string::npos != lastPos)) {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos)); // PRQA S 3084
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

// PRQA S 2171, 2173, 2175, 2185 L1
struct add_tokens {
	explicit add_tokens(const std::string& delim) : delim_(delim) {}
	std::string operator()(const std::string& a, const std::string& b) const
    { 
        return (a.empty()? b : (a + delim_ + b) );  // PRQA S 3382, 3385
    }

private:
	std::string delim_ ; 	
};
// PRQA L:L1


void StringUtils::buildIncrementalTokens(
    const std::vector<std::string>& tokens_in, 
    std::vector<std::string>& tokens_out,
    const std::string &delimiters) 
{
    tokens_out.clear();
    // PRQA S 3084 2
    tokens_out.resize(tokens_in.size() + 1); // Increase the size by one, since also the (N+1)th element will be filled

    // Merge the output vector with the input. Store the result into the (i+1)th element of the output.
    std::transform(tokens_out.begin(), 
                   tokens_out.end() - 1, 
                   tokens_in.begin(), 
                   tokens_out.begin() + 1,
                   add_tokens(delimiters)); // PRQA S 3081

    // First component is always empty:
    tokens_out.erase(tokens_out.begin());
}

void StringUtils::expandPathToSubpaths(const std::string& pathIN, std::vector<std::string>& v_out, const char slash) // PRQA S 2017
{
    string path = pathIN;
    bool root = false;
    v_out.clear();
    string delim(1, slash);

    string last ;
    while (path.length() > 0) {
        size_t ps = path.find(slash);
        string s = path.substr(0, ps);

        if (!((v_out.size() >= 1) && s == ".")) {

            if (!s.empty()) {

                last = ((last.empty() && ! root) ? s : (last + delim + s)); // PRQA S 3380, 3385
                v_out.push_back(last); // Extract [portion of] path before slash. Also works if ps==npos
                root = false;
            } else {
                root = true;
            }
        }

        if (ps != string::npos) {
            // Skip this slash, and also the next, if any (e.g. '////')
            path = path.substr(ps);
            do {
                path = path.substr(1);
            } while (!path.empty() && (path.at(0) == slash));
        } else {
            path = "";
        }

    }; // while (path.length() > 0)

    if (root) {
        v_out.push_back(delim);
    }
    
    return;
}

string &StringUtils::uppercase(std::string &s1) 
{
    std::transform(s1.begin(), s1.end(), s1.begin(), [](char c){ return toupper(c); });
    return s1; // PRQA S 4028
}

string StringUtils::uppercase(const std::string &s) 
{
    string s1 (s);
    std::transform(s1.begin(), s1.end(), s1.begin(), [](char c){ return toupper(c); });
    return s1; 
}

string &StringUtils::lowercase(std::string &s1) 
{
    std::transform(s1.begin(), s1.end(), s1.begin(), [](char c){ return tolower(c); });
    return s1; // PRQA S 4028
}

string StringUtils::lowercase(const std::string &s) 
{
    string s1 (s);
    std::transform(s1.begin(), s1.end(), s1.begin(), [](char c){ return tolower(c); });
    return s1;
}



// PRQA S 2173, 2175 L4

template<class T> struct uf_capitalize { // PRQA S 2171
    T operator()(T x) 
    {
        if (! _first) {
            return T(tolower(x)) ;
        }
        _first = false ; 
        return T(toupper(x)) ;
    }
private:
    bool _first = true ;
};

// PRQA L:L4


string &StringUtils::capitalize(std::string &s1) 
{
    std::transform(s1.begin(), s1.end(), s1.begin(), uf_capitalize<char>());
    return s1; // PRQA S 4028
}

string StringUtils::capitalize(const std::string &s) {
    string s1 (s);
    std::transform(s1.begin(), s1.end(), s1.begin(), uf_capitalize<char>());
    return s1;
}

string StringUtils::trim(const std::string &in, char c) // PRQA S 4020
{
    size_t start = in.find_first_not_of(c);
    if (start == std::string::npos) { return ""; }
    return in.substr(start, in.find_last_not_of(c) - start + 1); // PRQA S 3084
}

string StringUtils::trimBegin(const std::string &in, char c) // PRQA S 4020
{
    size_t start = in.find_first_not_of(c);
    if (start == std::string::npos) { return ""; }
    return in.substr(start);
}

string StringUtils::trimEnd(const std::string &in, char c) {
    return in.substr(0, in.find_last_not_of(c) + 1); // PRQA S 3084
}

std::string StringUtils::reverse(const std::string &source) 
{
    return string(source.rbegin(), source.rend()) ;
}

bool StringUtils::startsWith(std::string const& source, std::string const& end)  // PRQA S 4020
{
    if (end.length() > source.length()) {
        return false;
    }
    return 0 == source.compare(0, end.length(), end);
}

bool StringUtils::endsWith(std::string const& source, std::string const& end)  // PRQA S 4020
{
    if (end.length() > source.length()) {
        return false;
    }
    return 0 == source.compare(source.length() - end.length(), source.length(), end); // PRQA S 3084
}


// PRQA S 2173, 2175, 2185 L5

struct uf_replace_mchar {
    explicit uf_replace_mchar(const map<char, char> &tr) :tr_(tr) {} // PRQA S 2528
    char operator()(char x) const // PRQA S 4020
    {
        map<char, char>::const_iterator r = tr_.find(x) ;
        if (r != tr_.end()) {
            return r->second ;
        }
        return x ;
    }

    const map<char, char> &tr_ ; // PRQA S 2100
};

// PRQA L:L5

string StringUtils::urldecode(const std::string &s ) 
{
    string r ;
    r.reserve(s.length()) ;
    const char *src = s.c_str() ;
    while (*src) {
        char a ; 
        char b ;
        // note: as soon as *src or src[1] or src[2] are 0, the condition fails
        // so the collateral effect are needed to avoid buffer overflow 
        if ((*src == '%') && // NOSONAR
            ((a = src[1]) && (b = src[2])) && // NOSONAR
            (isxdigit(a) && isxdigit(b))) {
                if (a >= 'a') { a -= 'a'-'A'; }
                if (a >= 'A') { a -= ('A' - 10); }
                else { a -= '0'; }
                if (b >= 'a') { b -= 'a'-'A'; }
                if (b >= 'A') { b -= ('A' - 10); }
                else { b -= '0'; }
            char c = 16*a+b ;
            r += c ;
            src+=3;
        } else if (*src == '+') {
            r += ' ';
            src++;
        } else {
            char c = *src++ ;
            r += c ;
        }
    }
    return r ;
}


std::string StringUtils::transform(const std::string &str, const std::map<char, char> &replacements)
{
    string s(str.length(),0) ;
    std::transform(str.begin(), str.end(), s.begin(), uf_replace_mchar(replacements)); // PRQA S 3081
    return s ;
}


// PRQA S 2173, 2175, 2185 L6

struct uf_add_mchar { 
    uf_add_mchar(const map<char, string> &tr, string &s) :tr_(tr), s_(s) {} // PRQA S 2528
    void operator()(char x) // PRQA S 4020, 4214
    {
        map<char, string>::const_iterator r = tr_.find(x) ;
        if (r != tr_.end()) {
            s_ += r->second ;
        }
        else {
            s_ += x ;
        }
    }

    // PRQA S 2100 3
    const map<char, string> &tr_ ;
    string &s_ ;
};

// PRQA L:L6

std::string StringUtils::transform(const std::string &str, const std::map<char, std::string> &replacements)
{
    string s ;
    for_each(str.begin(), str.end(), uf_add_mchar(replacements, s)) ;
    return s ;    
}

std::string StringUtils::transform(const std::string &str, char from, char to) {
    string s(str.length(),0) ;
    replace_copy(str.begin(), str.end(), s.begin(), from, to);
    return s;
}

std::string StringUtils::transform(std::string const& str, std::string const& from, std::string const& to) {
    string result(str);
    size_t index = result.find( from);
    while (index != string::npos) {
        result.replace(index, from.length(), to);
        index = result.find( from, index + to.length()); // PRQA S 3084
    }
    return result;
}

bool StringUtils::isBlank(std::string const& str) 
{
    return str.find_first_not_of(' ') == string::npos ;
}

bool StringUtils::isNotBlank(std::string const& str) 
{
    return str.find_first_not_of(' ') != string::npos ;
}

std::string StringUtils::join(const std::vector<std::string>& collection, std::string const& separator) 
{
    ostringstream os ;
    if (! collection.empty()) {
        ostream_iterator<string> it (os,separator.c_str());
        copy ( collection.begin(), collection.end()-1, it );
        os << collection[collection.size() - 1];    // PRQA S 3084
    }
    return os.str();
}

std::string StringUtils::join(const std::vector<std::string>& collection, char separator) 
{
    return join(collection, string()+separator) ;
}

std::string StringUtils::substringAfterLast(std::string const& str, std::string const& separator) // PRQA S 4020 
{
    size_t lastIndexOf = str.rfind(separator);
    if (lastIndexOf != string::npos) {
        return str.substr(lastIndexOf + separator.length());    // PRQA S 3084
    } else {
        return "";
    }
}

std::string StringUtils::substringAfter(std::string const& str, std::string const& separator)  // PRQA S 4020 
{
    size_t sep_pos = str.find(separator);
    if (sep_pos != string::npos) {
        return str.substr(sep_pos + separator.length());    // PRQA S 3084
    } else {
        return "";
    }
}

std::string StringUtils::substringBefore(std::string const& str, std::string const& separator) 
{
    return str.substr(0, str.find(separator));
}

std::string StringUtils::substringBeforeLast(std::string const& str, std::string const& separator) 
{
    return str.substr(0, str.rfind(separator));
}

std::string StringUtils::substring(std::string const& str, size_t start, size_t end)  // PRQA S 4020 
{
    size_t len = str.length();
    if (end > len) {
        end = len;
    }
    
    if ((start > len) || (end < start)) {
        return "";
    }
    
    return str.substr(start, (end - start));    // PRQA S 3084
}

std::string StringUtils::substring(std::string const& str, size_t start) 
{
    return StringUtils::substring(str, start, string::npos);
}

std::string StringUtils::removeBegin(std::string const& str, size_t n) // PRQA S 4020
{
    if (n >= str.length()) { return "" ; }
    return str.substr(n) ;
}

std::string StringUtils::removeEnd(std::string const& str, size_t n)  // PRQA S 4020
{
    if (n >= str.length()) { return "" ; }
    return str.substr(0, str.length()-n) ; // PRQA S 3084
}

/**
 *   Disassemble a path (to a vector of strings)
 *   Examples:
 *     myfile			->  { 'myfile' }
 *     mydir/myfile		->  { 'mydir','myfile' }
 *     /mydir/myfile	->  { '', 'mydir','myfile' }
 *     .				->  { '.' }
 *     /				->  { '' }	<Special Case #1>
 *     ""				->  { }		<Special Case #2>
 */
std::vector<std::string> StringUtils::pathDisassemble(const std::string& pathIN, 
                                            bool removeLeadingSlash, 
                                            bool removeLeadingDot,
                                            char slash) 
{
    vector < string > v;
    string path = pathIN;

    while (path.length() > 0) {
        size_t ps = path.find(slash);
        string s = path.substr(0, ps);

        if (!(removeLeadingSlash && (s == "")) && 
            !((removeLeadingDot || (v.size() >= 1) ) && s == ".")) {
            v.push_back(s); // Extract [portion of] path before slash. Also works if ps==npos
        }
        
        if (ps != string::npos) {
            // Skip this slash, and also the next, if any (e.g. '////')
            path = path.substr(ps);
            do {
                path = path.substr(1);
            }
            while (!path.empty() && (path.at(0) == slash));
        } else {
            path = "";
        }
    }; // while (path.length() > 0)

    return v;
}


/**
 *   Extract the first level of a path 
 *   Examples:
 *     myfile			->  { 'myfile', '' }
 *     mydir/myfile		->  { 'mydir','/myfile' }
 *     /mydir/myfile	->  { '/mydir','/myfile' }
 *     /myd1/myd2/file	->  { '/myd1','/myd2/file' }
 *     .				->  { '.', '' }
 *     /				->  { '/', '' }	
 *     ""				->  { '', '' }	
 */
std::pair<std::string, std::string> StringUtils::pathExtractFirstLevel(const std::string& pathIN, 
                                           				 char slash) 
{
    pair < string, string > p;
    const string path = pathNormalize(pathIN);

    size_t ps = path.find(slash);

	if (ps == string::npos)
	{
		p.first = path ; 
		p.second = "" ; 
	
	}
	else
	{
		if (ps == 0) {
			ps = path.find_first_not_of(slash) ;
			if (ps != string::npos) { ps = path.find(slash, ps+1) ; } // PRQA S 3084
		}
		
		if (ps == string::npos) 
		{
			p.first = path ; 
			p.second = "" ; 
		}
		else
		{
			p.first = path.substr(0, ps) ; 
			p.second = path.substr(ps) ; 
		}
		
	}
	
    return p;
}


/**
 *   Join two paths (avoiding mistakes, like "//")
 */
std::string StringUtils::pathJoin(const std::string& path1, const std::string& path2, char s) // PRQA S 4020
{

    if (path1.empty() || path2.empty()) {
        return (path1 + path2); // At least one path empty
    }
    
    return trimEnd(path1,s) + s + trimBegin(path2,s) ;
}

/**
 *   Join three paths (avoiding mistakes, like "//")
 */
std::string StringUtils::pathJoin(const std::string& path1, const std::string& path2, const std::string& path3, char s) 
{
    return pathJoin(pathJoin(path1, path2, s), path3, s);
}

/**
 *   Disassemble a path (to a vector of strings)
 *   Simplified c'tor
 */
std::vector<std::string> StringUtils::pathDisassemble(const std::string& pathIN, char slash) 
{
    return pathDisassemble(pathIN, false, false, slash);
}

long StringUtils::hash(const std::string &s) 
{
    return _coll.hash(s.data(), s.data() + s.length()); // PRQA S 3084
}

/**
 *   Assemble a path (from a vector of strings)
 *   Examples:
 *     { 'myfile' }				-> myfile
 *     { 'mydir','myfile' }		-> mydir/myfile
 *     { '', 'mydir','myfile' }	-> /mydir/myfile
 *     { '.' }					-> .
 *     { '' }					-> /    <Special Case #1>
 *     { }						-> ""   <Special Case #2>
 */
std::string StringUtils::pathAssemble(const std::vector<std::string>&v, char s) {
    ostringstream o;
    o << s; // Char-to-string conversion
    const string slash = o.str();
    string ret = "";

    for (size_t i = 0; i < v.size(); i++) {
        if (0 == i) {
            ret = ((! v[0].empty()) ? v[0] : slash); // PRQA S 3380
        }
        else if ((v[i] != "") && (v[i] != ".")) {
            ret = pathJoin(ret, v[i]);
        }
        else {
            // nothing
        }
    }

    return ret;
}

/**
 *  As method pathAssemble(vector, char) but also concat two vectors (useful for concat an absolute and relative path, e.g.)
 */
std::string StringUtils::pathAssemble(const std::vector<std::string>&v1, const std::vector<std::string>&v2, char s) 
{
    vector < string > v = v1;
    v.insert(v.end(), v2.begin(), v2.end());
    return pathAssemble(v, s);
}

/**
 *   Normalize a path (e.g. by removing double/useless slashes)
 */
std::string StringUtils::pathNormalize(const std::string& str, char s) 
{
    return pathNormalize(str, false, false, s);
}

/**
 *   Normalize a path (e.g. by removing double/useless slashes)
 *   Simplified c'tor
 */
std::string StringUtils::pathNormalize(const std::string& str,  // PRQA S 4020
                                  bool removeLeadingSlash, 
                                  bool removeLeadingDot, 
                                  char s) 
{
    if (str.empty()) {
        return str; // Nothing to do !
    }
    vector < string > v;
    v = pathDisassemble(str, removeLeadingSlash, removeLeadingDot, s);
    return pathAssemble(v, s);
}

/**
 *  Scan the provided IPv6 and replaces the missing groups (marked as "::"), if available, with actual zeros
 *  Examples:
 *  "1:2::3:4" -> "1:2:0:0:0:0:3:4" 
 *  "::ffff" -> "0:0:0:0:0:0:0:ffff" 
 *  "1::" -> "1:0:0:0:0:0:0:0" 
 *  "::" -> "0:0:0:0:0:0:0:0" 
 */
std::string StringUtils::ipv6Normalize(const std::string& ipv6)// PRQA S 4020
{
    string ip_out1 ;
    string ip_out2 ; 
    string ip_outz ;
    size_t z = 0 ;

    if ((z = ipv6.find("::")) != string::npos) {

        size_t ng1 = 0 ;

        // Count groups BEFORE :::"
        size_t f_cur = 0 ;
        for(;;) {

            size_t f = ipv6.find(':', f_cur); 

            if ((f != string::npos) && (f <= z)) {
                if (f - f_cur > 0) { // PRQA S 3084
                    ng1++; // Do not move from here
                    ip_out1 = ip_out1 + (ipv6.substr(f_cur, (f - f_cur)) + ((ng1 < 8) ? ":" : "")); // PRQA S 3084, 3380, 4400
                }

                f_cur = (f + 1); // PRQA S 3084
            } else {
                break;
            }

        }; // while (!0)

        size_t ng2 = 0;
        // Count groups AFTER "::"
        f_cur = (z + 1); // PRQA S 3084
        for(;;) {

            size_t f = ipv6.find_first_of(':', f_cur);

            if (f != string::npos) {
                if (f - f_cur > 0) { // PRQA S 3084
                    ip_out2 = ip_out2 + (((0 != ng2) ? ":" : "") + ipv6.substr(f_cur, (f - f_cur))); // PRQA S 3084, 3380
                    ng2++; // Do not move from here
                }

                f_cur = (f + 1); // PRQA S 3084
            } else {
                if ((ipv6.length() - f_cur) > 0) { // PRQA S 3084
                    ip_out2 += (((0!=ng2) ? ":" : "") + ipv6.substr(f_cur, (ipv6.length() - f_cur))); // PRQA S 3084, 3380
                    ng2++; // Do not move from here
                }
                break;
            }

        }; // while (!0)

        // Add the first separator, if needed
        if ((ng2 > 0) && (ng2 < 8)) { // PRQA S 4400
            ip_out2 = ":" + ip_out2;
        }
        
        // Add omitted groups
        if ((ng1 + ng2) < 8) { // PRQA S 3084, 4400
            for (size_t i = 0; i < (8 - ng1 - ng2); ++i) { // PRQA S 3084, 4400
                ip_outz = ip_outz + ((0!=i) ? ":0000" : "0000"); // PRQA S 3380
            }
        }
    } else {
        return ipv6; // No :: was found
    }
    return (ip_out1 + ip_outz + ip_out2);
}

namespace {
   void stoV4(const std::string &aom, unsigned char bytes[16])  // NOSONAR
   {
       unsigned int s[4]; // NOSONAR
       int status = sscanf(aom.c_str(), "%u.%u.%u.%u", &s[0], &s[1], &s[2], &s[3]); // PRQA S 4412
       ACS_COND_THROW( // PRQA S 3081
               (status != 4),
               exIllegalValueException(
                       "Invalid IPv4 (address or mask): \"" + aom + "\""));
       for (size_t i = 0; i < 4; i++) {
           ACS_COND_THROW( // PRQA S 3081
                   s[i] > 0xff,
                   exIllegalValueException(
                           "Invalid IPv4 (address or mask) overflow: \""
                                   + aom + "\""));
           bytes[i] = static_cast<unsigned char>(s[i]); // PRQA S 3081
       }
   }

   void stoV6(const std::string &aom, unsigned char bytes[16])  // NOSONAR
   {
       unsigned short h[8]; // NOSONAR
       int status = sscanf(StringUtils::ipv6Normalize(aom).c_str(), 
                           "%hx:%hx:%hx:%hx:%hx:%hx:%hx:%hx", 
                           &h[0], &h[1], &h[2], &h[3], &h[4], &h[5], &h[6], &h[7]);
       ACS_COND_THROW( // PRQA S 3081
               (status != 8),
               exIllegalValueException(
                       "Invalid IPv6 (address or mask): \"" + aom + "\""));
       for (size_t i = 0; i < 16; i++) { // PRQA S 4400
           bytes[i] = ((i & 0x1) ? ((h[i / 2]) & 0x00ff) : ((h[i / 2] >> 8) & 0x00ff)); // PRQA S 3000, 3007, 3010, 3054, 3084, 3380, 4400
       }
   }


}

/**
 *  Check if given IP belongs to the subnet provided.
 *  subnet is in the form: base_address \<SPACE\> netmask (e.g. 192.168.0.1 255.255.255.0)
 *  -OR-
 *  subnet is in the form: base_address [SPACE] \<SLASH\> CIDR (e.g. 192.168.0.1/24)
 *  WORKS WITH IPv4 and IPv6 (uppercase and lowercase)
 */
bool StringUtils::ipBelongsToSubnet(const std::string& subnet, const std::string& ip) 
{
    // Normalize inputs
    string wsubnet = lowercase(trim(subnet));// Remove leading/trailing spaces and force to lowercase
    string wip = lowercase(trim(ip)) ;// Remove leading/trailing spaces and force to lowercase

    // Check inputs are not empty :
    //////////////////////////////////////////
    ACS_COND_THROW(wsubnet.empty(), exIllegalValueException("Subnet is empty")); // PRQA S 3081 2
    ACS_COND_THROW(wip.empty(), exIllegalValueException("IP is empty"));



    //  Scan for subnet_address
    //////////////////////////////////////////
    size_t pos = wsubnet.find(' ');
    if (pos == std::string::npos) {
        pos = wsubnet.find('/');
        ACS_COND_THROW((pos == std::string::npos), // PRQA S 3081
                exIllegalValueException("Invalid Subnet format: \"" + wsubnet + "\""));
    }

    string subnet_address = wsubnet.substr(0, pos);

    //  Scan for subnet_address
    //////////////////////////////////////////
    pos = wsubnet.rfind("/");
    if (pos == std::string::npos) {
        pos = wsubnet.rfind(" ");
        ACS_COND_THROW((pos == std::string::npos), // PRQA S 3081
                exIllegalValueException("Invalid Subnet format: \"" + wsubnet + "\""));
    }

    string subnet_mask = wsubnet.substr(pos + 1); // PRQA S 3084

    // Check if subnet_address is a valid IPv4 or IPv6 address:
    bool has_colon = (subnet_address.find(":") != std::string::npos) ;
    bool has_point = (subnet_address.find(".") != std::string::npos) ;

    // Check if subnet_address is a valid IPv4 or IPv6 address:
    // !has_colon == !has_point -> both present or absent
    ACS_COND_THROW(!has_colon == !has_point, exIllegalValueException("Invalid Subnet address: \"" + subnet_address + "\""));

    bool use_ipv6 = has_colon ;

    // If subnet_mask is not an IP address (no separators) so it's a CIDR. And viceversa.
    const bool has_cidr = ((subnet_mask.find(".") == std::string::npos) && (subnet_mask.find(":") == std::string::npos));

    //  Check if provided ip matches the version (4 or 6) of subnet_address
    ///////////////////////////////////////////////////////////////////////////////////
    if ((use_ipv6 && (wip.find(".") != std::string::npos)) || (!use_ipv6 && (wip.find(":") != std::string::npos))) {
        ACS_THROW( // PRQA S 3081
                exIllegalValueException("IP Address \"" + wip
                                + "\" invalid or not matching the version of Subnet Address: \"" + subnet_address
                                + "\""));
    }

    unsigned char subnet_mask_bytes[16] ; // NOSONAR  - they're not strings
    memset(subnet_mask_bytes, 0, sizeof(subnet_mask_bytes));

    if (has_cidr) {
        //  Compute bitfield for CIDR (both IPv4 and IPv6)
        ///////////////////////////////////////////////////////////////////////////////////

        unsigned int cidr = 0 ;
        int status = sscanf(subnet_mask.c_str(), "%u", &cidr); // PRQA S 4412
        ACS_COND_THROW((status != 1), // PRQA S 3081
                exIllegalValueException("Invalid CIDR: \"" + subnet_mask + "\""));

        ACS_COND_THROW((( ! use_ipv6 && (cidr > 32)) || (use_ipv6 && (cidr > 128))), // PRQA S 3081
                exIllegalValueException("CIDR too large: " + subnet_mask));

        unsigned char r = 0x00;
        size_t i = 0 ;
        for (i = 0; i < size_t(cidr % 8); i++) { // PRQA S 3081, 3084, 4230, 4400
            r >>= 1;
            r |= 0x80; // PRQA S 4400
        };

        for (i = 0; i < size_t(cidr / 8); i++) { // PRQA S 3081, 3084, 4230, 4400
            
            subnet_mask_bytes[i] = 0xff; // PRQA S 4400
        }
        if (i < sizeof(subnet_mask_bytes)) { // 16 
            subnet_mask_bytes[i] = r;
        }
    } else {
        //  Compute bitfield for subnet_mask
        ///////////////////////////////////////////////////////////////////////////////////

        if (!use_ipv6) {
            stoV4(subnet_mask, subnet_mask_bytes) ;
        } else {
            stoV6(subnet_mask, subnet_mask_bytes) ;
        }
    }

    //  Compute bitfield for subnet_address
    //  Compute bitfield for IP
    ///////////////////////////////////////////////////////////////////////////////////

    unsigned char subnet_address_bytes[16] ; // NOSONAR - they're not strings
    memset(subnet_address_bytes, 0, sizeof(subnet_address_bytes));
    unsigned char ip_bytes[16];; // NOSONAR 
    memset(ip_bytes, 0, sizeof(ip_bytes));


    size_t field_number ;
    if (!use_ipv6) {
        field_number = 4 ;
        stoV4(subnet_address, subnet_address_bytes) ;
        stoV4(wip, ip_bytes) ;
    } else {
        field_number = 16 ;
        stoV6(subnet_address, subnet_address_bytes) ;
        stoV6(wip, ip_bytes) ;
    }

    //  Check if IP belongs to the subnet
    ///////////////////////////////////////////////////////////////////////////////////

    for (size_t i = 0; i < field_number; i++) {
        unsigned char v = (subnet_mask_bytes[i] & (subnet_address_bytes[i] ^ ip_bytes[i])); // PRQA S 3000, 3007, 3010

        // DEBUG: excerr << "i: " << i << "  subnet_mask_bytes: " << hex << (int)subnet_mask_bytes[i] << dec << "  subnet_address_bytes: " << hex << (int)subnet_address_bytes[i] << dec << "  ip_bytes: " << hex << (int)ip_bytes[i] << dec << "  v:" << hex << (int)v << dec << endl ;
        
        if (v != 0) return false ; 

    }

    return true ;
}
// PRQA S 2173, 2175, 2185, 2631, 2632, 2633 L7

// Inner functor class
struct uf_find_regexp
{
    explicit uf_find_regexp(const std::string&s) : _myRegExp (s) { }

    bool operator() (char c) const { return _myRegExp.match (string()+c); } 

    RegExp _myRegExp; // PRQA S 2100
};

// PRQA L:L7

std::string StringUtils::removeRegExp (const std::string& s, const std::string& regexp) // PRQA S 4020
{
    if (regexp.empty()) { return s ; }
    string newString = s;
	newString = std::string (newString.begin(), 
                             remove_if (newString.begin(), 
                                        newString.end(),
                                        uf_find_regexp(regexp)   // PRQA S 3081
                                       )
                             );
    return newString;
}

std::string StringUtils::sanitizeDbName (const std::string& name)
{
    return removeRegExp (name, "[[:punct:][:space:]]");
}


_ACS_END_NAMESPACE
