// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Regular Expression Library $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2014/02/07 18:05:18  marpas
	adopting ACS_THROW and ACS_COND_THRO macros
	
	Revision 5.1  2013/06/18 12:26:23  marpas
	deprecated include changed
	
	Revision 5.0  2013/06/10 15:52:41  marpas
	adopting libException 5.x standards
	
	Revision 2.9  2012/06/27 11:27:30  marpas
	qac++ instrumentation
	
	Revision 2.8  2012/03/29 16:06:03  marpas
	minor changes
	
	Revision 2.7  2012/03/21 13:39:25  marpas
	coverage integration in progress
	
	Revision 2.6  2012/02/08 16:26:15  marpas
	refactoring
	
	Revision 2.5  2012/01/31 15:10:03  marpas
	removing comiler warnings
	
	Revision 2.4  2009/12/03 13:05:14  marpas
	diagnostic improved
	
	Revision 2.3  2009/11/25 12:56:25  marpas
	bug fixed
	
	Revision 2.2  2009/11/25 12:28:03  marpas
	verify method added
	
	Revision 2.1  2009/03/11 19:10:43  marpas
	rule violations fixed
	
	Revision 2.0  2006/02/28 08:25:52  marpas
	Exception 2.1 I/ adopted
	
	Revision 1.10  2005/05/19 17:58:59  marpas
	headers fixed
	
	Revision 1.9  2004/10/11 16:05:04  nicvac
	Method match overloaded. String matching delimited expression in brackets are retrivable by Key.
	
	Revision 1.8  2004/09/14 08:09:21  nicvac
	Method match overloaded. Allow to get all the matches related to brackets delimited regular expression.
	
	Revision 1.7  2003/07/10 15:03:29  marpas
	exException::clone implemented
	
	Revision 1.6  2003/04/30 09:35:07  marpas
	using namespace std was removed from includes
	
	Revision 1.5  2003/01/30 19:45:23  marpas
	method to get largest matching substring position.
	
	Revision 1.4  2002/11/29 10:22:56  paoscu
	Fixed a bug in ~RegExp() and in setReg .
	
	Revision 1.3  2002/11/21 16:14:37  paoscu
	Copy constructor and operator= added .
	
	Revision 1.2  2002/11/21 15:47:41  paoscu
	Line  "regfree(&_preg);" added in ~RegExp()
	
	Revision 1.1.1.1  2002/11/21 15:00:23  paoscu
	Import libRegExp
	
	

*/

#include <RegExp.h>
#include <Filterables.h>
#include <lregx.h>

using namespace acs;
using namespace std;

namespace {
    lregx Version;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(RegExp) 


RegExp::RegExp() : 
    _re_compiled(false), 
    _regExp(), 
    _preg()
{
}



// PRQA S 4207 L3
// The above suppression is due to a problem qac++ has with this kind of POD initialization
RegExp::RegExp(string const & regexp) : 
    _re_compiled(false), 
    _regExp(), 
    _preg()
{
	setReg(regexp) ;
}
// PRQA L:L3

RegExp::~RegExp() ACS_NOEXCEPT 
{
    // regfree cannot throw
	if(_re_compiled) { regfree(&_preg) ; } // PRQA S 4631
}

// PRQA S 4207 L2
// The above suppression is due to a problem qac++ has with this kind of POD initialization
RegExp::RegExp(const RegExp & rhs) :
    _re_compiled(false),
    _regExp(),
    _preg()
{
    setReg(rhs._regExp) ;
}
// PRQA L:L2


RegExp & RegExp::operator=(const RegExp & rhs)
{
	if ( this !=  &rhs ) { setReg(rhs._regExp) ; }
	return *this;
} 


void RegExp::setReg(string const & regexp)
{
	if( _regExp != regexp)
	{
		if(_re_compiled)
		{
			regfree(&_preg);
		}
		_re_compiled=false;	
		_regExp=regexp;
	}
}


const string &RegExp::getReg() const ACS_NOEXCEPT // PRQA S 4120
{
	return _regExp;
}

void RegExp::compile() const
{
	if(not _re_compiled)
	{
		int compRes=regcomp ( &_preg , _regExp.c_str() , REG_EXTENDED | REG_NEWLINE) ;
		if(compRes != 0)
		{
			char errorString[1024]; // PRQA S 4403
			regerror( compRes, &_preg , errorString , sizeof(errorString) );
			ACS_THROW(BadRegularExpression(string(errorString) + " - regexp was \"" + _regExp + "\"")); // PRQA S 3081
		}
		
		_re_compiled=true;
	}
}



bool RegExp::match(string const & st) const 
{
	compile() ;

	int res=regexec ( &_preg , st.c_str() , 0 ,0 ,0 ); 
	
	return res == 0 ; // true if ok
}

bool RegExp::match(string const & st, regoff_t &matchBegin, regoff_t &matchEnd) const // PRQA S 4020
{
	compile() ;
		
	
	regmatch_t match[1] ;
	int res=regexec ( &_preg , st.c_str(), 1 ,match ,0 ); 
		
	if (res == 0) {
		if (match[0].rm_so != -1) {
			matchBegin = match[0].rm_so ;
			matchEnd = match[0].rm_eo ;
		}
		return true ;
	}
	
	return false ;	
}


RegExp::Match::Match(): _matched(""), _empty(true), _begin(-1), _end(-1) {
}
RegExp::Match::Match(const string& matched, const bool& empty, const regoff_t& begin, const regoff_t& end):  
                    _matched(matched), _empty(empty), _begin(begin), _end(end) {
}
RegExp::Match::~Match() ACS_NOEXCEPT { }

RegExp::Match::Match(const RegExp::Match& rhs) :
    _matched(rhs._matched),
    _empty(rhs._empty),
    _begin(rhs._begin),
    _end(rhs._end)
{
}

RegExp::Match& RegExp::Match::operator = (const RegExp::Match& aMatch) {
  if (this != &aMatch) {
      _matched = aMatch._matched;
      _empty =   aMatch._empty;
      _begin =   aMatch._begin;
      _end =     aMatch._end;
  }
  return *this;
}


const string &RegExp::Match::getMatched() const ACS_NOEXCEPT { return _matched; } // PRQA S 4120
bool RegExp::Match::getEmpty() const ACS_NOEXCEPT { return _empty; }         // PRQA S 4120
regoff_t RegExp::Match::getBegin() const ACS_NOEXCEPT { return _begin; }     // PRQA S 4120
regoff_t RegExp::Match::getEnd() const ACS_NOEXCEPT { return _end; }         // PRQA S 4120

bool RegExp::match(string const & st, Matches &matches) const // PRQA S 4020
{
	matches.clear();
    compile();
	
    size_t numMatch( getDimOfMatch() );
    
    regmatch_t match[numMatch];
	int res=regexec ( &_preg, st.c_str(), numMatch, match, 0 ); 

    if (res == 0) {
        for ( size_t i=0; i<numMatch; i++ ) {
            if (match[i].rm_so != -1) { // PRQA S 3000,3051 2
                matches.push_back( Match( st.substr(match[i].rm_so, match[i].rm_eo-match[i].rm_so), false, match[i].rm_so, match[i].rm_eo) );
            }
        }
        return true;
    }
	return false ;	
}

bool RegExp::match(string const & st, KeyMatches &matches) const // PRQA S 4020 
{
    matches.clear();
    compile();
	
    size_t numMatch( getDimOfMatch() );

    regmatch_t match[numMatch];
	int res=regexec ( &_preg, st.c_str(), numMatch, match, 0 ); 

    if (res == 0) {
        for ( size_t i=0; i<numMatch; i++ ) {

            bool empty(true); string matchedString("");
            MatchKey key(i); // PRQA S 3010,3051
            if ( match[i].rm_so != -1 ) {
                empty = false;
                // PRQA S 3000,3051 2
                matchedString = st.substr(match[i].rm_so, match[i].rm_eo-match[i].rm_so);
            }
            
            matches.insert( make_pair(key , Match( matchedString, 
                                                   empty, 
                                                   match[i].rm_so, 
                                                   match[i].rm_eo)) );
        }
        return true;
    }
	return false ;	
}

bool RegExp::verify(string const &v, bool ex) const
{
  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " verifying: \"" << v << "\"" << "(size: " << v.length() << ")") ;

  regoff_t beg = -1 ;
  regoff_t end = -1 ;
  if (!match(v, beg, end)) {
    ACS_COND_THROW(ex, // PRQA S 3081
      exIllegalValueException("Regexp match error: \"" + v + "\" does not match \"" + _regExp + "\"")
    ) ;		
  }
	
  if (size_t(end) != v.length()) { // PRQA S 3081
    ostringstream os ;
    os << "Unexpected char at pos: " << end+1 << " for " << '\n' 
       << "\"" << v << "\"\n" << ' ' ;
    for (regoff_t i=0; i < end+1 ; ++i) { os << '.' ; }
    os << "^\n" ;
    ACS_COND_THROW(ex,exIllegalValueException(os.str())) ; // PRQA S 3081		
  }
	
  return true ;
}

// PRQA S 3054,3084,3380,4231 L1
// Note that qac++ does not recognizes correctly the expression in the for
// that's why there are many suppressions
size_t RegExp::getDimOfMatch() const 
{
    //The string to be stored are al least the number of "(" +1
    size_t pos(0); 
    size_t numMatch(0);
    //first time (!(numMatch|pos)) pos has to be 0. In the next steps pos has to be pos+1. (pos cannot be init to -1)
    for (; (pos=_regExp.find("(", ( !(numMatch|pos))?pos: (pos+1) ) ) != string::npos ; ++numMatch ) {} //number of "("...
    numMatch++; //...+1
    return numMatch;
}
// PRQA L:L1
