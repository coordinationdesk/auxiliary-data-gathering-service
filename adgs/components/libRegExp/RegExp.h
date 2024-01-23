// PRQA S 1050 EOF
/*

	Copyright 1995-2012, Advanced Computer Systems , Inc.
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
	Revision 2.6  2012/11/29 13:37:30  marpas
	removing qa warnings
	
	Revision 2.5  2012/06/27 11:27:30  marpas
	qac++ instrumentation
	
	Revision 2.4  2012/03/21 13:39:25  marpas
	coverage integration in progress
	
	Revision 2.3  2012/02/08 16:26:15  marpas
	refactoring
	
	Revision 2.2  2009/11/25 12:28:03  marpas
	verify method added
	
	Revision 2.1  2006/09/19 14:52:23  danalt
	fixed for GCC 4.1.0
	
	Revision 2.0  2006/02/28 08:25:52  marpas
	Exception 2.1 I/ adopted
	
	Revision 1.8  2005/05/19 17:58:59  marpas
	headers fixed
	
	Revision 1.7  2004/10/11 16:05:04  nicvac
	Method match overloaded. String matching delimited expression in brackets are retrivable by Key.
	
	Revision 1.6  2004/09/14 08:09:21  nicvac
	Method match overloaded. Allow to get all the matches related to brackets delimited regular expression.
	
	Revision 1.5  2003/07/10 15:03:29  marpas
	exException::clone implemented
	
	Revision 1.4  2003/04/30 09:35:07  marpas
	using namespace std was removed from includes
	
	Revision 1.3  2003/01/30 19:45:23  marpas
	method to get largest matching substring position.
	
	Revision 1.2  2002/11/21 16:14:38  paoscu
	Copy constructor and operator= added .
	
	Revision 1.1.1.1  2002/11/21 15:00:23  paoscu
	Import libRegExp
	
	

*/

#ifndef _RegExp_H_
#define _RegExp_H_

#include <exException.h>
#include <map>

#include <sys/types.h> // PRQA S 1013
#include <regex.h>

namespace acs{


class RegExp // PRQA S 2109
{
public:


  /*! class acs::BadRegularExpression declaration */
  exDECLARE_EXCEPTION(BadRegularExpression,exException) ; // PRQA S 2131, 2502

  RegExp();

  /// Set the regular expression
  explicit RegExp(std::string const & regexp);

  ~RegExp() ACS_NOEXCEPT ;

  RegExp(const RegExp &) ;

  RegExp &operator=(const RegExp &) ; 


  void setReg(std::string const & regexp);
  const std::string &getReg() const ACS_NOEXCEPT ;

  /**
  * Match a string against the regular expression.
  * First time analize the regular expression.
  * If matchBegin or matchEnd are not null, they will be filled respectively with 
  * the start and end position of the largest substring matching the expression.
  * \throw BadRegularExpression The regular expression is not good.
  */
  bool match(std::string const &) const ;
  /**
  * Match a string against the regular expression.
  * First time analize the regular expression.
  * \returns into matchBegin and matchEnd with 
  * the start and end position of the largest substring matching the expression.
  * e.g. mat+ matches the "to be matched" string with 6-9.
  * \throw BadRegularExpression The regular expression is not good.
  */
  bool match(std::string const &, regoff_t &matchBegin, regoff_t &matchEnd) const;

  /**
  * A single Match.
  * matched: substring element matching the regular expression
  * begin: start position of the largest substring matching the expression.
  * end: end position of the largest substring matching the expression.
  */
  class Match {
  public:
    Match();
    Match(const std::string& matched, const bool& empty, const regoff_t& begin, const regoff_t& end);
    virtual ~Match() ACS_NOEXCEPT ;
    Match(const Match&);
    Match& operator = (const Match&);

    /** substring element matching the regular expression */
    const std::string &getMatched() const ACS_NOEXCEPT ;
    /** true if getMatched contains a string matched with the expression  */
    bool getEmpty() const ACS_NOEXCEPT ;
    /** start position of the largest substring matching the expression */
    regoff_t getBegin() const ACS_NOEXCEPT ;
    /** end position of the largest substring matching the expression */
    regoff_t getEnd() const ACS_NOEXCEPT ;
  private:
    std::string _matched;
    bool _empty;
    regoff_t _begin;
    regoff_t _end;
  };

  /** Matches object returned by overloaded match */
  typedef std::vector<Match> Matches;
  /** Key of the Match */
  typedef const unsigned int MatchKey;
  /** Matches object returned by overloaded match */
  typedef std::map<MatchKey, Match> KeyMatches;

  /**
  * Match a string against the regular expression.
  * Analize the regular expression.
  * Fill a vector of Match with all the matches found.
  * e.g. ^Dummy(InterestingString)Dummy(.*)EndOfString$ regular expression
  *      matches the "DummyInterestingStringDummyAnotherInterestingStringEndOfString" string with
  *      "InterestingString" and "AnotherInterestingString"
  * \throw BadRegularExpression The regular expression is not good.
  */
  bool match(std::string const &, Matches &matches) const;

  /**
  * Match a string against the regular expression.
  * Analize the regular expression.
  * Fill a map of Match with all the matches found.
  * e.g. ^Dummy(InterestingString)?Dummy(.*)EndOfString$ regular expression
  *      matches the "DummyDummyAnotherInterestingStringEndOfString" string with
  *      matches with key 0: "DummyDummyAnotherInterestingStringEndOfString"
  *      matches with key 1: "". Note: Match::getEmpty=true;
  *      matches with key 2: "AnotherInterestingString"
  * \throw BadRegularExpression The regular expression is not good.
  */
  bool match(std::string const &, KeyMatches &matches) const;

  /** 
  * Verifies the given string against the regular expression.
  * If the string does not match, it returns the position of the first character not matching.
  * if the boolean ex is true, an exception is thrown.
  */
  bool verify(std::string const &, bool ex = true) const ;
private:
  /** 
   * Computes the size of vector that contains the matches 
   */
  size_t getDimOfMatch() const ;

  void compile() const ;	

private:
  mutable bool _re_compiled ;
  std::string _regExp;	
  mutable regex_t _preg;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(RegExp) ;

};


}  //close namespace



#endif //_RegExp_H_
