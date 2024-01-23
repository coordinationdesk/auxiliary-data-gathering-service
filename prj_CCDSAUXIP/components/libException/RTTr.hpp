/*

	Copyright 2011-2022 Exprivia, SPA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. Base Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2013/09/26 10:23:06  marpas
	debug strings now removes '(' and ')' to support anonymous namespace.
	
	Revision 5.0  2013/06/04 15:34:59  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 1.4  2013/03/01 13:45:48  marpas
	pattern Noncopyable removed, prefer explicit declaration instead
	
	Revision 1.3  2011/12/23 15:58:13  marpas
	design enforced
	
	Revision 1.2  2011/06/01 14:28:49  marpas
	major changes
	coverage introduction
	
	Revision 1.1.1.1  2011/05/26 17:02:39  marpas
	Creating libAcsBase
	
	

*/


#ifndef _Strings_RTTr_H_
#define _Strings_RTTr_H_ 

#include <acs_c++config.hpp>
#include <string> 
#include <cstdlib> 

_ACS_BEGIN_NESTED_NAMESPACE(acs,strings)

/*! \addtogroup strings 
 *  @{
 */

/*! \class RTTr 
 * 
 * \brief support typename translator for RT debug
 * 
 * This class is used to translate typenames into strings that can be used as environment variables
 * to debug the typenames itself. 
 * 
 * The translation rules are the following:
 * 
 * - all the substrings "acs::" are removed
 * - all the chars ':', '<', '>' and ',' are translated into '_'
 * - all the ' ' are removed
 * Examples (with the default suffix):
 * \code
 *    C< B<complex<double> >,A> -> "C_B_std__complex_double___A__DEBUG"
 *    acs::stream::Stream_Init -> "stream__Stream_Init_DEBUG"
 *    acs::thread:.Thread -> "thread__Thread_DEBUG"
 * 
 * \endcode
*/
class RTTr { // PRQA S 2109
public:
	/*! 
	 * \brief ctor
	 * \param s the typename to be translated
	 * \param sfx the suffix to be added (default _DEBUG)
	 */
	explicit RTTr(const std::string &s, const std::string& sfx = "_DEBUG") ; // PRQA S 4634
    ~RTTr() = default ;

    RTTr() = delete ; 
    RTTr(const RTTr &)  = delete ;
    RTTr & operator = (const RTTr &) = delete ;
    RTTr(RTTr &&)  = delete ;
    RTTr & operator = (RTTr &&) = delete ;
    
	/*! this is the cast operator that translates the typename passed to the constructor */
	operator const char *() const noexcept { return t_.c_str() ; } // NOSONAR - implicit cast 
	/*! 
	 * this static method get the environment variable into the passed integer 
	 * if the environment variable is not found, the function sets it to -1
	 * \note inlined for perfo
	 * 
	 */
	static void envget_(const std::string &s_, int &pi_) // PRQA S 2133, 2134
    {
	    const char *ep_ = std::getenv(RTTr(s_)) ; // PRQA S 3050, 3081
	    pi_ = ep_ ? atoi(ep_) : -1 ; // PRQA S 2956, 3232, 3380, 3385
    }

private:
	
	std::string t_ ; ///< stores the translated typename
} ;

/*! @}*/

_ACS_END_NESTED_NAMESPACE

#endif // _Strings_RTTr_H_
