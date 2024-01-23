// PRQA S 1050 EOF
/*

	Copyright 1995-2022 Exprivia, SPA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.7  2018/09/04 16:19:36  marpas
	Debug strings are now stored into a multimap. This allows to have the same string point at different integer variables, hence allowing for linked debug variables.
	Although there is no reason to have in the data structure the same integer registered with different strings, this is harmless and invariant for the class job.
	
	Revision 5.6  2016/05/19 10:05:37  marpas
	useless and potentially dangerous unregistration methods removed
	
	Revision 5.5  2014/02/07 18:03:07  marpas
	introducing ACS_THROW and ACS_COND_THROW macros to substitute throwException and throwConditionedException
	documentation improved and tests aligned
	
	Revision 5.4  2013/09/26 18:09:35  marpas
	robustness strongly improved:
	singleton functions (with static variables inside) used
	some private members are now accessed only through the above technique
	exRTDebuggable classes do not longer unregster themselves from exRTDebug.
	it was a logic error: sooner or later the container class (exRTDebug) will throw away memory in any case.
	The above changes will lead in a more efficient process closure.
	moreover the exException class now uses the reentrant method strerror_t to obtain the errno explaination.
	
	Revision 5.3  2013/06/18 12:25:20  marpas
	some include files are now deprecated
	
	Revision 5.2  2013/06/05 15:55:51  marpas
	new RTDebuggable class in utility namespace to have a new approach to real-time debugging.
	previous exRTDebuggable left for backward compatibility, but it will sonn deprecated
	
	Revision 5.1  2013/06/04 17:23:46  marpas
	introducing ne RTDebuggable template
	
	Revision 5.0  2013/06/04 15:35:02  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.9  2013/05/14 17:56:51  marpas
	Singleton's release at exit for exRTDebug class
	
	Revision 4.8  2013/02/07 08:57:41  marpas
	pattern singleton used in debug support
	exception notification improved in debug subsystem support
	namespaces enforced
	
	Revision 4.7  2012/06/25 17:42:06  marpas
	qacpp instrumentation
	
	Revision 4.6  2012/06/22 13:41:47  marpas
	pr qa instrumentation
	
	Revision 4.5  2012/06/14 17:16:41  marpas
	qacpp instrumentation
	
	Revision 4.4  2012/06/12 17:37:01  marpas
	qacpp instrumentation
	
	Revision 4.3  2012/06/11 15:25:53  marpas
	qa fixing in progress
	
	Revision 4.2  2012/03/19 15:49:25  marpas
	improving debug and getting rid of std::e_n_d_l()
	
	Revision 4.1  2012/01/31 14:13:04  marpas
	getting rid of boost and compilation warning reduced
	
	Revision 4.0  2011/10/18 11:22:00  marpas
	new statistics
	
	Revision 3.1  2011/03/01 11:07:03  marpas
	GCC 4.4.x support
	
	Revision 3.0  2009/10/08 10:48:10  marpas
	Statistics class added
	rules application improved
	
	Revision 2.8  2009/06/26 10:41:55  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.7  2009/03/11 17:09:02  marpas
	c++rules violation removed
	
	Revision 2.6  2009/03/02 17:02:39  marpas
	CODECHECK parsing - no regressions
	
	Revision 2.5  2008/12/15 15:43:47  marpas
	mode implementation in progress
	
	Revision 2.4  2007/04/18 10:18:28  marpas
	global dtor behaviour enforced
	
	Revision 2.3  2007/04/18 10:02:07  marpas
	no lock performed after the static member is reset
	
	Revision 2.2  2006/05/05 08:15:33  giucas
	debug improved - now signals when the string is already assigned
	
	Revision 2.1  2006/02/28 08:24:37  marpas
	new release, exException inherits from std::exception
	
	Revision 1.7  2005/10/12 22:23:10  ivafam
	MP: robustness improved against threads, multiple initialization (shared objects)
	
	Revision 1.6  2005/10/12 17:07:46  marpas
	some improvements and debug
	
	Revision 1.5  2005/10/12 10:42:12  marpas
	dtor robustness improved
	
	Revision 1.4  2005/10/11 12:39:24  marpas
	messages improved
	
	Revision 1.3  2005/10/11 10:47:48  marpas
	debug info are written in order to be read by rsResourceSet class
	
	Revision 1.2  2005/10/11 10:36:38  marpas
	markers can be avoided from getDebugRefs(string &...)
	
	Revision 1.1  2005/10/06 10:30:23  marpas
	Runtime debug almost completed
	
*/

#include <exRTDebug.h>
#include <exStream.h>
#include <exRTDebuggable.h>
#include <Filterables.h>
#include <sstream>
#include <algorithm>
#include <limits>
#include <boost/bind.hpp> // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)

int exRTDebug::exCDebugLevel = 0 ;

exRTDebug::exRTDebug() : 
    pattern::Singleton<exRTDebug>()
{
	if (ACS_CLASS_GET_DEBUG() == 0) { // the very first time 
		ACS_CLASS_INIT_DEBUG(exRTDebug_DEBUG)
		if (ACS_CLASS_GET_DEBUG() == -1) { // was not set - try with the next
			ACS_CLASS_SET_DEBUG(0) ; // simulate nothig was done
			ACS_CLASS_INIT_DEBUG(RTDebug_DEBUG)
		}
	}
	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) 
		std::cerr << SimpleDebugSignature << "Debugging signatures will be written" << '\n' ; // NOSONAR
	ACS_CLASS_END_DEBUG
}

namespace {

    class equal_ptr {
    public:
        explicit equal_ptr(const std::pair < std::string, exRTDebug::_v_ptr> &e) : ptr(e.second) {}

        bool operator()(const std::pair < std::string, exRTDebug::_v_ptr> &e) const {
            return e.second == ptr ;
        }
    private:
    
        exRTDebug::_v_ptr ptr ;
    } ;
} 

void exRTDebug::registerDebug(const std::string &d, _v_ptr p, const std::string &src) noexcept // PRQA S 4020
{
    ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
        std::cerr << SimpleDebugSignature << d << " src " << src // NOSONAR
                  << " requested: " << std::hex << p << std::dec << '\n' ;
    ACS_CLASS_END_DEBUG	
    if (nullptr == p) { return ; }
    if (src.empty()) { registerDebug(d,p) ; return ;}
    
    Lock const lock(_M_mutex()) ;
    
    // Search if a classlink exists
    auto it = classlinks_.find(d);
    
    // A classlink is found for class d, hence 
    if (it != classlinks_.end()) {
        
        ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
            std::cerr << SimpleDebugSignature << it->first << " found " << src << '\n' ;  // NOSONAR
        ACS_CLASS_END_DEBUG	
		std::string xlink = it->second; // xlink in the name of the linked class
		        
		// The linked class variable must actually exist in the multimap
		registerDebug(xlink, p);
		
		// Check that the insertion of orig->xlink would not introduce cycles.
		// So check that starting from xlink, orig is unreachable
		bool acyclic = true;
		do {
			it = classlinks_.find(it->second);
			if (it != classlinks_.end()) {
				acyclic = (it->second != src);
			}
		} while (acyclic && it != classlinks_.end());
		
		// The graph is actually acyclic, can insert orig safetly
		// Create a link between orig (myself) and the linked class
		if (acyclic) {
			classlinks_[src] = xlink;
		}
        setDebug(d, get_max(d)) ;
		return;
	}
	
	registerDebug(d, p);
	
	// Cycles impossibles here, d does not exists in the classlinks map
	classlinks_[src] = d;
}

void exRTDebug::registerDebug(const std::string &d, _v_ptr p ) noexcept // PRQA S 4020
{
	if (nullptr == p) { return ; }

    try {
	    Lock const lock(_M_mutex()) ;

        std::pair<std::string, _v_ptr> el = make_pair(d,p) ;
        std::pair<_m_type::iterator, _m_type::iterator> range=map_.equal_range(d) ;
        if (( distance(range.first, range.second) == 0 ) || // empty range or ... 
            ( find_if(range.first, range.second, equal_ptr(el)) == range.second) ) { // not found
    	    _m_type::iterator id=map_.insert(el) ;
            ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
                using std::hex ;
                using std::dec ;
                std::cerr << SimpleDebugSignature << d   // NOSONAR
                          << " assigned to " << hex << id->second << dec << " (" << *id->second << ")"
                          << " requested: " << hex << p << dec << " (" << *p << ")" << '\n' ;
	        ACS_CLASS_END_DEBUG
        }
    }
    catch(const std::exception &) { /* nothing */ } // NOSONAR - no esception from here
}



const std::string & exRTDebug::getDebugRefs(std::string &refs, bool markers) const 
{
	Lock const lock(_M_mutex()) ;
	// prepare the output strings: the header
    // get unique debug strings - note is a multimap
    std::vector<std::pair < std::string, _v_ptr> > unique ;
    get_unique(unique) ;
    std::sort(unique.begin(), unique.end()) ;
	refs = "" ;
	refs.reserve(unique.size() * 80) ; // no too much realloc 
	if (!unique.empty()) { // only in case there are at least one in the map
		refs += "#DEBUG CLASSES " ;
		if (markers) {
			refs += "(* means debug )" ;
        }
		refs += "\n" ;
	}
	
	// each unique entry build a line like
    // EventNotifier_DEBUG=-1
    // if markers are required the line will be:
    // * MyClassXXX_DEBUG=20
    // with a '*' char when debug value is > 0
    std::ostringstream os ;
    for (const auto &i : unique) {
        os.str("") ; // reset previous 
		int const * const v = i.second ;
		if (!v) { continue ; } // no entries without a valid pointer to debug variable
        
		if (markers) {
			os << ((*v > 0) ? "* " : "  ") ; // PRQA S 3380
        }
		os << i.first << '=' << *v << '\n' ;
        
		refs+=os.str() ;
    }
 
	if (!refs.empty()) { // the footer
		refs += "#______________________________\n" ;
    }
	return refs ; // PRQA S 4028
}

namespace {
    bool equal_first(
        const std::pair < std::string, exRTDebug::_v_ptr> &a, 
        const std::pair < std::string, exRTDebug::_v_ptr> &b)
    { return a.first == b.first ; }
} 

void exRTDebug::get_unique(std::vector<std::pair < std::string, _v_ptr> > &v) const 
{
    v.clear() ;
	std::unique_copy( map_.begin(), map_.end(),
                      std::back_inserter(v),
                      equal_first );
}

const std::vector<std::string> &exRTDebug::getDebugRefs(std::vector<std::string> &v) const 
{
	v.clear() ;
	// prepare the output strings: the header
    // get unique debug strings - note is a multimap
    std::vector<std::pair < std::string, _v_ptr> > unique ;
    get_unique(unique) ;
    std::sort(unique.begin(), unique.end()) ;

	std::transform( unique.begin(), unique.end(),
                   std::back_inserter(v),
                   boost::bind(&std::pair < std::string, _v_ptr>::first,_1) );	
	return v ; // PRQA S 4028
}

void exRTDebug::setDebug(const std::string &d, int v)  // PRQA S 4020
{
	Lock const lock(_M_mutex()) ;
	std::pair<_m_type::iterator, _m_type::iterator> range=map_.equal_range(d) ;
    if (distance(range.first, range.second) == 0) { // the range is empty: 0 size
        // could it be a link ? 
        auto lnit = classlinks_.find(d);
        if (lnit == classlinks_.end()) { return ; } // no link
        range=map_.equal_range(lnit->second) ;
        // no need to check: if it is a link there should be a valid range
    } 
    
    for(_m_type::iterator i = range.first; i != range.second;  ++i) { // set the debug value to every integer ...
        if (i->second) { *(i->second) = v ; } // whose pointer is not null
    }
}


int exRTDebug::get_max(const std::string &d)  // PRQA S 4020
{
    int m = std::numeric_limits<int>::min() ;
	Lock const lock(_M_mutex()) ;
	std::pair<_m_type::iterator, _m_type::iterator> range=map_.equal_range(d) ;
    if (distance(range.first, range.second) == 0) { // the range is empty: 0 size
        // could it be a link ? 
        auto lnit = classlinks_.find(d);
        if (lnit == classlinks_.end()) { return m ; } // no link
        range=map_.equal_range(lnit->second) ;
        // no need to check: if it is a link there should be a valid range
    } 
    std::for_each(range.first,
                  range.second,
                  [&m](auto &i){ if (i.second) m = std::max(m,*(i.second));}) ;
    return m ;
}


size_t exRTDebug::size() const // PRQA S 4214
{
	Lock const lock(_M_mutex()) ;
    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) 
        std::cerr << SimpleDebugSignature << "mmap size: " << map_.size() << '\n' ; // NOSONAR
    ACS_CLASS_END_DEBUG
	// prepare the output strings: the header
    // get unique debug strings - note is a multimap
    std::vector<std::pair < std::string, _v_ptr> > unique ;
    get_unique(unique) ;
    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) 
        std::cerr << SimpleDebugSignature << "unique size: " << unique.size() << '\n' ; // NOSONAR
    ACS_CLASS_END_DEBUG
	return unique.size() ;
}


_ACS_END_NAMESPACE
