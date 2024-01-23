// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA
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
	Revision 5.2  2013/11/13 18:03:25  marpas
	introducing abstract method clear to clear filterable content
	
	Revision 5.1  2013/10/11 09:39:48  marpas
	streams graph traversal implemented to avoid multiple notification of the same filterable due to complex graphs
	
	Revision 5.0  2013/06/04 15:35:02  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.3  2013/04/29 15:39:01  marpas
	exFPrivateInfo filterable concept introduced and tested
	coding best practices applied
	
	Revision 4.2  2012/11/15 11:00:55  marpas
	qa rules enforced
	
	Revision 4.1  2012/06/14 17:16:41  marpas
	qacpp instrumentation
	
	Revision 4.0  2011/10/18 11:22:00  marpas
	new statistics
	
	Revision 3.0  2009/10/08 10:48:09  marpas
	Statistics class added
	rules application improved
	
	Revision 2.4  2009/06/26 10:41:55  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.3  2008/02/15 16:58:54  marpas
	each stream can have its own formatter setFormatter method added
	exception::what() has no longer the stack if enabled
	exception::stack() returns the stack (if enabled)
	exception stack are no longer notified if present unless the stream is instructed to do that via notifyStackFromEx method
	Filterables now hase details, they can be set via exFilterable::detail(...) method
	Filterables details are notified by the stream having the appropriate flag set via notifyDetailFromFilterable method
	exception inserted into a Filterable will fill its detail with their stack (if any)
	
	Revision 2.2  2006/10/31 20:41:52  marpas
	defaultNotify implementation almost done
	
	Revision 2.1  2006/02/28 08:24:36  marpas
	new release, exException inherits from std::exception
	
	Revision 1.5  2004/09/27 09:23:06  marpas
	exFilterable I/F changed
	
	Revision 1.4  2003/07/10 14:43:39  marpas
	header make-up
	
	Revision 1.3  2003/04/30 09:57:38  marpas
	using namespace std was removed from includes
	
	Revision 1.2  2002/11/26 10:31:18  marpas
	Thread safe implementation improved.
	
	Revision 1.1  2002/10/03 15:05:38  marpas
	exFilter and exFilterable added
	

*/

#ifndef _exFilterable_H_
#define _exFilterable_H_ 

#include <acs_c++config.hpp>
#include <string>
#include <list>

_ACS_BEGIN_NAMESPACE(acs)

class exostream ;

class StreamGraphTraversal ;

/*! \class exFilterable
	
	\brief Base class for filterable objects.
	
	It defines a common interface for all filters (exFilters and derivate classes)
	The only function to be implemented is the one returning the text to be logged.
	\see text()
*/
class exFilterable
{
public:
	exFilterable &operator=(const exFilterable &) = delete ;
	exFilterable(const exFilterable &) = delete ;
	exFilterable() ;
	virtual ~exFilterable() = default ;
	
    virtual void clear() = 0 ;
	virtual std::string text() = 0 ;
	virtual std::string message() const = 0 ;
	virtual std::string detail() const = 0 ;
	virtual void detail(const std::string &) = 0 ;
	
	/*! The abstract method is called when a Filterable is inserted into the stream. 
	    The implementation should take care of this, in order to freeze the content of the 
		filterable and remember the object has been already inserted
	 */
	virtual void insertionLock() = 0 ;
	
	virtual bool defaultNotify() const = 0 ;
    
    // this register the very first StreamGraphTraversal invoking this method
    void registerTraversal(StreamGraphTraversal *t) noexcept 
    {
        if (nullptr == traversalRegister_) { traversalRegister_ = t; }
    }
    
    // this unregister the registered StreamGraphTraversal
    void unregisterTraversal(StreamGraphTraversal const *s) noexcept 
    {
        if (traversalRegister_ == s) { traversalRegister_ = nullptr ; }
    }
    
    //- returns true if this filterable already traversed the given stream
    bool traversed(exostream const * const) noexcept ;

    StreamGraphTraversal *traversaleRegister() noexcept { return traversalRegister_ ; } // PRQA S 4211, 4627

private:
    StreamGraphTraversal *traversalRegister_ ;
} ;

/*- brief class to control exostream graph traversal during notify
*/
class StreamGraphTraversal { // PRQA S 2109
public:
    StreamGraphTraversal() = delete ;
    StreamGraphTraversal(const StreamGraphTraversal &) = delete ;
    StreamGraphTraversal& operator= (const StreamGraphTraversal &) = delete ;

    explicit StreamGraphTraversal(exFilterable &f) noexcept : f_(f) // PRQA S 2528
    { f_.registerTraversal(this) ; }
    
   	~StreamGraphTraversal() noexcept { f_.unregisterTraversal(this) ; }
    
    /*- brief - the tricky method
        returns true if the stream is already in the list
        else append it to the list and returns false (only the first time it will return false)
    */ 
    bool traversed(exostream const * const s) noexcept ;

private:
    exFilterable &f_ ;
    std::list<const exostream *> traversedStreams_ = {} ;
} ;
_ACS_END_NAMESPACE

#endif // _exFilterable_H_
