// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA - DFDA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.4  2018/09/04 16:18:49  marpas
	Debug strings are now stored into a multimap. This allows to have the same string point at different integer variables, hence allowing for linked debug variables.
	Although there is no reason to have in the data structure the same integer registered with different strings, this is harmless and invariant for the class job.
	
	Revision 5.3  2016/05/19 10:05:37  marpas
	useless and potentially dangerous unregistration methods removed
	
	Revision 5.2  2013/09/26 18:09:35  marpas
	robustness strongly improved:
	singleton functions (with static variables inside) used
	some private members are now accessed only through the above technique
	exRTDebuggable classes do not longer unregster themselves from exRTDebug.
	it was a logic error: sooner or later the container class (exRTDebug) will throw away memory in any case.
	The above changes will lead in a more efficient process closure.
	moreover the exException class now uses the reentrant method strerror_t to obtain the errno explaination.
	
	Revision 5.1  2013/06/04 17:23:46  marpas
	introducing ne RTDebuggable template
	
	Revision 5.0  2013/06/04 15:35:02  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.6  2013/05/14 17:56:51  marpas
	Singleton's release at exit for exRTDebug class
	
	Revision 4.5  2013/02/07 08:57:41  marpas
	pattern singleton used in debug support
	exception notification improved in debug subsystem support
	namespaces enforced
	
	Revision 4.4  2012/06/25 17:42:06  marpas
	qacpp instrumentation
	
	Revision 4.3  2012/06/14 17:16:41  marpas
	qacpp instrumentation
	
	Revision 4.2  2012/06/12 17:37:01  marpas
	qacpp instrumentation
	
	Revision 4.1  2012/06/06 17:12:20  marpas
	qa rules application in progress
	
	Revision 4.0  2011/10/18 11:22:00  marpas
	new statistics
	
	Revision 3.0  2009/10/08 10:48:10  marpas
	Statistics class added
	rules application improved
	
	Revision 2.4  2009/06/26 10:41:55  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.3  2009/03/11 17:09:02  marpas
	c++rules violation removed
	
	Revision 2.2  2008/12/15 15:43:47  marpas
	mode implementation in progress
	
	Revision 2.1  2006/02/28 08:24:37  marpas
	new release, exException inherits from std::exception
	
	Revision 1.3  2005/10/12 22:23:10  ivafam
	MP: robustness improved against threads, multiple initialization (shared objects)
	
	Revision 1.2  2005/10/11 10:36:38  marpas
	markers can be avoided from getDebugRefs(string &...)
	
	Revision 1.1  2005/10/06 10:30:24  marpas
	Runtime debug almost completed
		
*/

#ifndef _exRTDebug_H_
#define _exRTDebug_H_ 

#include <acs_c++config.hpp>
#include <Singleton.hpp>
#include <ThreadSafe.h>
#include <exRTDebuggable.h>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)

/*! \class exRTDebug

	\brief the runtime debugging class
	
	A singleton to register all runtime debuggable classes.
	Users generally do not need to use this class. 
	Anyway can be used at application level to change debug behaviour.
*/
class exRTDebug : public pattern::Singleton<exRTDebug> // PRQA S 2109,2153
{
// friendship needed in order to construct Statistics from a singleton see Singleton doc
friend exRTDebug *pattern::Singleton< exRTDebug>::instance(); // PRQA S 2107
public:
	virtual ~exRTDebug() ACS_NOEXCEPT ;
	typedef int * _v_ptr ;
	typedef std::multimap<std::string,  _v_ptr > _m_type ;
	
	void registerDebug(const std::string &, _v_ptr, const std::string &orig) ACS_NOEXCEPT ;
	void registerDebug(const std::string &, _v_ptr) ACS_NOEXCEPT ;
	
	const std::string &getDebugRefs(std::string &, bool markers=true) const ;
	const std::vector<std::string> &getDebugRefs(std::vector<std::string> &) const ;
	/*!
        \brief sets a debuglevel to specific class
    */
    void setDebug(const std::string &, int) ;
	size_t size() const ;
    
    /*! The method has a logn complexity, so do not abuse of it, 
        prefer getDebugRefs which returns a vector if you want to scan over it.
        \see getDebugRefs
    */ 
    std::string operator[](size_t) const ; // PRQA S 2141
	const std::string& mode() const ACS_NOEXCEPT { return _mode ; }
	void mode(const std::string &m) { _mode = m ; }
	
protected:
	exRTDebug() ;

private:
	// declared but not implemented to prevent their use
	exRTDebug(const exRTDebug &) ;
	exRTDebug &operator=(const exRTDebug &) ;

    /*! Fills the given vector with all elemnts having unique string. 
        There is no specific criteria in choosing an element from multiple entries 
        having the same associated string.
    */
    void get_unique(std::vector<std::pair < std::string, _v_ptr> > &) const ;
private:	
	_m_type map_ ;
	std::string _mode ;
	std::map<std::string, std::string> classlinks_;
private:
    // don't use macros here
	static int exCDebugLevel ;
} ;

_ACS_END_NAMESPACE


#endif // _exRTDebug_H_
