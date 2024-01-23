// PRQA S 1050 EOF
/*

 Copyright 1995-2011, Advanced Computer Systems , Inc.
 Via Della Bufalotta, 378 - 00139 Roma - Italy
 http://www.acsys.it

 All Rights Reserved.

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 the contents of this File may not be disclosed to third parties, copied or
 duplicated in any form, in whole or in part, without the prior written
 permission of Advanced Computer Systems, Inc.

 $Prod: A.C.S. String Utility Library $

 $Id$

 $Author$

 $Log$
 Revision 1.47  2017/05/25 09:37:13  marpas
 introducing urldecode method

 Revision 1.46  2015/12/15 14:53:47  francesco.avanzi
 uncapitalize() removed

 Revision 1.45  2013/03/25 20:00:02  marpas
 trimXXX template method added
 convenience predicate is_not_space added

 Revision 1.44  2013/02/18 10:59:40  enrcar
 EC:: new method pathExtractFirstLevel (to allow recursive navigation of paths). Test added.

 Revision 1.43  2013/02/06 12:15:16  marpas
 introduced findNoCase functions family (similar to those in std::string::find)
 best coding practices enforced
 coverage on new functions test added

 Revision 1.42  2013/01/25 13:09:23  marpas
 no debug in deprecated template

 Revision 1.41  2013/01/25 13:01:05  marpas
 missing include: fixed

 Revision 1.40  2012/12/06 19:09:38  marpas
 coding best practices
 strong performance optimization
 bug fixing
 test coverage improved

 Revision 1.39  2012/12/05 17:53:00  marpas
 refactoring, qa rules, efficiency and coding best practices ... still in progress

 Revision 1.38  2012/12/03 16:51:47  marpas
 refactoring, appliying qa rules and coding best practices
 pay attention to performances and other optimization

 Revision 1.37  2012/11/29 19:38:26  marpas
 qa rules partially adopted

 Revision 1.36  2012/02/27 10:45:12  marfav
 Added remove regexp methods

 Revision 1.35  2011/10/26 09:17:50  enrcar
 EC:: added static method ipv6Normalize. All IPv6 are automatically normalized before use.

 Revision 1.34  2011/10/25 09:26:59  enrcar
 EC:: Added static method: buildIncrementalTokens

 Revision 1.33  2011/10/24 13:03:32  enrcar
 EC:: expandPathToSubpaths static method added

 Revision 1.32  2011/09/21 10:01:56  tergem
 ExtractFromString template method added:Extracts values from a list of values (the list is considered as a string)

 Revision 1.31  2011/06/07 10:04:07  enrcar
 EC:: Added static method ipBelongsToSubnet (both IPv4 and IPv6)

 Revision 1.30  2011/05/05 17:52:55  micmaz
 added compare() and equals()

 Revision 1.29  2011/05/04 09:53:09  micmaz
 added trimBegin and trimEnd functions

 Revision 1.28  2011/04/07 13:33:19  micmaz
 added removeBegin and removeEnd methods

 Revision 1.27  2011/03/09 10:33:38  micmaz
 added reverse, endsWith and startsWith utility.

 Revision 1.26  2011/03/03 11:18:50  marpas
 method hash added

 Revision 1.25  2011/02/14 10:17:32  enrcar
 EC:: pathJoin (2 values): BUG FIXED. pathJoin (3 values): Method added

 Revision 1.24  2011/02/11 14:51:21  enrcar
 EC:: pathJoin: method added

 Revision 1.23  2011/02/11 10:13:36  enrcar
 EC:: pathNormalize and pathDisassemble: added optional parameters to remove leading slash and leading dot

 Revision 1.22  2011/02/10 11:31:14  enrcar
 EC:: added methods: pathNormalize pathDisassemble pathAssemble

 Revision 1.21  2010/11/16 14:29:31  micmaz
 substitued size_t with ssize_t

 Revision 1.20  2010/11/12 14:04:24  micmaz
 added a new method in order to replace characters in a std::string without change it.

 Revision 1.19  2010/11/12 13:17:01  micmaz
 added new Functionalities.

 Revision 1.18  2010/11/03 11:39:56  micmaz
 added a new function.

 Revision 1.17  2010/10/26 08:43:49  micmaz
 indexOf returns int.

 Revision 1.16  2010/10/25 16:09:34  micmaz
 vectors must be passed by reference.

 Revision 1.15  2010/10/25 15:50:37  micmaz
 added some new functions.

 Revision 1.14  2009/03/12 10:10:55  marpas
 rules violations fixed and or justified

 Revision 1.13  2008/07/08 13:24:58  marpas
 StringUtils::trim method added

 Revision 1.12  2008/04/16 00:39:31  marpas
 replace function overloaded to replace a char with a string

 Revision 1.11  2007/06/09 13:30:58  marpas
 replace method overloaded

 Revision 1.10  2007/06/09 13:23:15  marpas
 replace method added (useful to replace chars in string)

 Revision 1.9  2007/05/16 10:52:15  marpas
 lowercase, uppercase and capitalize methods implemented

 Revision 1.8  2006/06/22 14:44:40  marpas
 datetime dependency removed -

 Revision 1.7  2006/03/09 15:15:48  marfav
 Tokenize is now static

 Revision 1.6  2004/12/03 17:27:54  danalt
 Fixed ACS header

 Revision 1.5  2004/11/03 15:48:13  marfav
 Added DateTime support for generate filename parts

 Revision 1.4  2004/10/22 13:36:40  nicvac
 endl as a static constant string.

 Revision 1.3  2004/10/08 15:15:57  masdal
 added Tokenize method

 Revision 1.2  2004/09/22 09:28:06  marfav
 From String and To String template methods added

 Revision 1.1.1.1  2004/09/20 16:47:24  marfav
 Import libStringUtils


 */

#ifndef _StringUtils_H_
#define _StringUtils_H_

#include <acs_c++config.hpp>

#include <exException.h>

#include <vector>
#include <map>
#include <locale>
#include <algorithm>
#include <iterator>
#include <sstream>

_ACS_BEGIN_NAMESPACE(acs)


/** This is a static class: has no state variables
 *
 */
class StringUtils { // PRQA S 2109

public:



    static bool is_not_space(char c) { return ::isspace(c) == 0 ; } // convenience predicate: can be used in trimXXX template methods
	
    static int compareNoCase(const std::string&, const std::string&);
    static bool equalsNoCase(const std::string&, const std::string&);
    static int compareCase(const std::string&, const std::string&);
    static bool equals(const std::string&, const std::string&);

    static std::string::size_type findNoCase(const std::string&, const std::string&, std::string::size_type pos = 0) ;
    static std::string::size_type findNoCase(const std::string&, const char * s, std::string::size_type pos = 0) ;
    static std::string::size_type findNoCase(const std::string&, const char * s, std::string::size_type pos, std::string::size_type n) ;
    static std::string::size_type findNoCase(const std::string&, char c, std::string::size_type pos = 0) ;
    // splits the string and returns a vector of string plus the remainder
    // as return value.
    // Can use a user defaulted char as split marker
    static std::string split(const std::string&, std::vector<std::string>&, char = '\n');
    static std::string split(const std::string&, std::vector<std::string>&, bool allowEmptyTokens, char = '\n');
    static void Tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters);

	/**
 	 *  Given a vector of tokens, build a new vector, by incrementally adding the tokens each other.
	 *  E.G. (Delimiter "/")  IN: { "A" } OUT: { "A" } ;  IN: { "A" "B" "C" } OUT: { "A" "A/B" "A/B/C" } 
	 */
	static void buildIncrementalTokens(const std::vector<std::string>& tokens_in, 
                                      std::vector<std::string>& tokens_out, 
                                      const std::string &delimiters=" ");

// MP deprecated method to ber removed ASAP
    template<typename T> static std::string toString(const T&); // PRQA S 2502

    template<typename T> static T fromString(const std::string&);

    template<typename T> static std::vector<T> ExtractFromString(const std::string&);
 
    static std::string uppercase(const std::string &); // UppEr  -> UPPER // PRQA S 2502
    static std::string &uppercase(std::string&); // UppEr  -> UPPER      // PRQA S 2502
    static std::string lowercase(const std::string &); // LoWer  -> lower
    static std::string &lowercase(std::string&); // LoWer  -> lower
    static std::string capitalize(const std::string &); // CaPitaLize  -> Capitalize
    static std::string &capitalize(std::string&); // CaPitaLize  -> Capitalize

    static std::string trim(const std::string &, char c = ' ');
    static std::string trimBegin(const std::string &in, char c = ' ');
    static std::string trimEnd(const std::string &in, char c = ' ');

    template <typename UnaryPredicate>
    static std::string trimBegin(const std::string &in, UnaryPredicate pred) // PRQA S 4020
    {
        std::string::const_iterator f = find_if(in.begin(), in.end(), pred) ;
        if (f != in.end()) {
            return std::string(f, in.end()) ;
        }
        return in ;
    }
    
    template <typename UnaryPredicate>
    static std::string trimEnd(const std::string &in, UnaryPredicate pred) // PRQA S 4020
    {
        std::string::const_reverse_iterator r = find_if(in.rbegin(), in.rend(), pred) ;
        if (r != in.rend()) {
            std::string res = std::string(r, in.rend()) ;
            std::reverse(res.begin(), res.end()) ;
            return res ;
        }
        return in ;
    }
    
    
    template <typename UnaryPredicate>
    static std::string trim(const std::string &in, UnaryPredicate pred)
    {
        return trimEnd(trimBegin(in, pred), pred) ;
    }

    
    static std::string __attribute__((deprecated)) reverse(const std::string &source) ; // PRQA S 2502 3
    static bool startsWith(std::string const& source, std::string const& end);
    static bool endsWith(std::string const& source, std::string const& end);

    
    /**
     *  Removes all the chars in the string s that match the given regexp
     */
    static std::string removeRegExp (const std::string& s, const std::string& regexp);

    /**
     *  Removes all the spaces and punctuation from the given string, using removeRegExp
     */
    static std::string sanitizeDbName (const std::string& name);

    /** converts an encoded url into a string as     {
        '%7B%22value%22:30000%7D' -> '{"value":30000}' ;
     */
    static std::string urldecode(const std::string& ) ;

    /**
     *  Replace the first char with the second one  
     */
    static std::string transform(const std::string &str, const std::map<char, char> &replacements); // PRQA S 2502
    /**
     *  Replace the first char with the second string  
     */
    static std::string transform(const std::string &str, const std::map<char, std::string> &replacements); // PRQA S 2502
    /**
     *  Replace the "from" char to the "to" char copying the result in a new string (without changind the old one).
     */
    static std::string transform(const std::string &str, char from, char to); // PRQA S 2502
    /**
     *  Replace the "from" char to the "to" char in the "str" string.
     */
    static std::string transform(std::string const& str, std::string const& from, std::string const& to);  // PRQA S 2502
    /**
     *  Checks if a string is whitespace or empty ("").
     */
    static bool isBlank(std::string const& str);
    /**
     *  Checks if a string is not whitespace or empty ("").
     */
    static bool isNotBlank(std::string const& str);
    /**
     *  Joins the elements of the provided vector into a single string containing the provided elements.
     */
    static std::string join(const std::vector<std::string>& collection, char separator);
    /**
     *  Joins the elements of the provided vector into a single string containing the provided elements.
     */
    static std::string join(const std::vector<std::string>& collection, std::string const& separator);
    /**
     *   Gets the substring after the last occurrence of a separator.
     */
    static std::string substringAfterLast(std::string const& str, std::string const& separator);
    /**
     *   Gets the substring after the first occurrence of a separator.
     */
    static std::string substringAfter(std::string const& str, std::string const& separator);
    /**
     *   Gets the substring before the first occurrence of a separator.
     */
    static std::string substringBefore(std::string const& str, std::string const& separator);
    /**
     *    Gets the substring before the last occurrence of a separator.
     */
    static std::string substringBeforeLast(std::string const& str, std::string const& separator);
    /**
     *   Gets a substring from the specified String avoiding exceptions.
     */
    static std::string substring(std::string const& str, size_t start);
    /**
     *   Gets a substring from the specified String avoiding exceptions.
     */
    static std::string substring(std::string const& str, size_t start, size_t end);
    /**
     * Removes the first n chars
     */
    static std::string removeBegin(std::string const& str, size_t n = 1);
    /**
     * Removes the last n chars
     */
    static std::string removeEnd(std::string const& str, size_t n = 1);

    /**
     *   Join two paths (avoiding mistakes, like "//")
     */
    static std::string pathJoin(const std::string&, const std::string&, const char s = '/');

    /**
     *   Join three paths (avoiding mistakes, like "//")
     */
    static std::string pathJoin(const std::string&, const std::string&, const std::string&, char s = '/');

    /**
     *   Normalize a path (e.g. by removing double/useless slashes)
     */
    static std::string pathNormalize(const std::string&, char s = '/');

    /**
     *   Normalize a path, and also force any path to be relative or remove leading dot, if any
     */
    static std::string pathNormalize(const std::string&, bool removeLeadingSlash, bool removeLeadingDot, char s = '/');

    /**
     *   Disassemble a path (to a vector of strings)
     */
    static std::vector<std::string> pathDisassemble(const std::string&, char slash = '/');

    /**
     *   Disassemble a path (to a vector of strings) and also force any path to be relative or remove leading dot, if any
     */
    static std::vector<std::string> pathDisassemble(const std::string&, 
                                                   bool removeLeadingSlash, 
                                                   bool removeLeadingDot, 
                                                   char slash = '/');

	/**
     *    Extract the first level of a path (to a pair: first level, the remainder)
	 */ 
	static std::pair<std::string, std::string> pathExtractFirstLevel(const std::string& pathIN,  char slash = '/');


    /**
     *   Disassemble a path, building a vector of strings. Each row of the vector is the sum of all previous rows so far.
	 *   E.G. Input: "a/b/c/d"  ->  Output: { "a", "a/b", "a/b/c", "a/b/c/d" }
     */
	static void expandPathToSubpaths(const std::string&, std::vector <std::string>& v_out, char s = '/') ;

    /**
     *   Assemble a path (from a vector of strings)
     */
    static std::string pathAssemble(const std::vector<std::string>&, char s = '/');

    /**
     *  As method pathAssemble(vector, char) but also concat two vectors (useful for concat an absolute and relative path, e.g.)
     */
    static std::string pathAssemble(const std::vector<std::string>&, const std::vector<std::string>&, char s = '/');

	/**
	 *  Scan the provided IPv6 and replaces the missing groups (marked as "::"), if available, with actual zeros
	 *  Examples:
	 *  "1:2::3:4" -> "1:2:0:0:0:0:3:4" 
	 *  "::ffff" -> "0:0:0:0:0:0:0:ffff" 
	 *  "1::" -> "1:0:0:0:0:0:0:0" 
	 *  "::" -> "0:0:0:0:0:0:0:0" 
	 */
	static std::string ipv6Normalize (const std::string& ipv6) ;

    /**
     *  Check if given IP belongs to the subnet provided.
	 *  subnet is in the form: base_address \<SPACE\> netmask (e.g. 192.168.0.1 255.255.255.0)
	 *  -OR-
	 *  subnet is in the form: base_address [SPACE] \<SLASH\> CIDR (e.g. 192.168.0.1/24)
	 *  WORKS WITH IPv4 and IPv6 (uppercase and lowercase)
     */
	static bool ipBelongsToSubnet(const std::string& subnet, const std::string& ip) ;


    /**
     * Returns the hash value of the string corresponding to the string.
     * A hash for a string is a value that uniquely (see note) identifies the content of the string,
     * so that two strings with the same hash value, would compare equal by collate::compare,
     * and two strings with different hash values would compare not equal.
     * Thus, two strings can be easily compared for equality by simply comparing their hash values,
     * which are of an integer type.
     * \note The uniqueness is relative to the possible values the hash type can take (in this case numeric_limits<unsigned long>::max()).
     * Therefore, there is a very small probability that two strings with different contents have the same hash (generally, one in billions).
     */
    static long hash(const std::string &);

    /**
     * Convenience method to return a Container as a CSV String. E.g. useful for toString() implementations.
     */
    template<typename _Container>
    static std::string containerToCommaDelimitedString(_Container arr) {
        std::ostringstream os;
        for (typename _Container::const_iterator iter = arr.begin(); iter < arr.end(); ++iter) {
            os << *iter;
            os << ",";
        }
        std::string result = os.str();
        return StringUtils::substringBeforeLast(result, ",");
    }

    /**
     * Convenience method to return a String array as a delimited (e.g. CSV) String. E.g. useful for toString() implementations.
     */
    template<typename _Container>
    static std::string containerToDelimitedString(_Container arr, std::string const& delimiter) {
        std::ostringstream os;
        for (typename _Container::const_iterator iter = arr.begin(); iter < arr.end(); ++iter) {
            os << *iter;
            os << delimiter;
        }
        std::string result = os.str();
        return StringUtils::substringBeforeLast(result, delimiter);
    }

private:
    StringUtils();
    StringUtils(const StringUtils &);
    StringUtils &operator=(const StringUtils &);
    ~StringUtils();

private:
    static std::locale _loc; // the "C" locale
    static const std::collate<char>& _coll;
};

template<typename T> inline __attribute__((deprecated)) std::string StringUtils::toString(const T& value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

template<typename T> inline T __attribute__((deprecated)) StringUtils::fromString(const std::string& value) {
    std::istringstream is(value);
    T retval = T();
    is >> retval;
    return retval;
}

/** Extracts values from a list of values (the list is considered as a string) */
template <typename T> inline std::vector<T> __attribute__((deprecated)) StringUtils::ExtractFromString(const std::string& in)
{
	std::vector<std::string> out_str;
	Tokenize(in, out_str, " ");
	
	std::vector <T> out;
	std::back_insert_iterator< std::vector<T> > insert_it (out);
	std::transform(out_str.begin(), out_str.end(), insert_it, fromString<T>);

	return out;
}


_ACS_END_NAMESPACE

#endif /* _StringUtils_H_ */
