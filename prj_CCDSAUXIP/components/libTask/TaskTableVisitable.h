//  PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer TaskTableVisitables Library $

	$Id$

	$Author$

	$Log$
	Revision 2.11  2017/09/07 15:59:47  marfav
	Added support for retrieving element lists using a smart container of pointers
	
	Revision 2.10  2017/01/13 09:55:14  marfav
	Fixed clear operation signature now it is virtual
	Fixed use of virtual operations in DTOR
	
	Revision 2.9  2017/01/12 16:32:34  marfav
	Fixed signatures const and not const of operator []
	
	Revision 2.8  2015/11/19 15:05:33  marpas
	adding warning
	
	Revision 2.7  2015/05/08 09:59:20  davide.tiriticco
	missing scoping namespace added
	
	Revision 2.6  2015/04/29 10:29:54  marfav
	Stack protected by a mutex. Warning removed
	
	Revision 2.5  2013/07/07 18:22:08  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.4  2012/06/11 15:49:54  marpas
	qac++ analisys fixed
	
	Revision 2.3  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.2  2009/03/04 20:58:52  marpas
	CODECHECK parsing strategy improved: no regressions
	
	Revision 2.1  2008/04/22 11:07:46  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	

*/

#ifndef _TASKTABLEVISITABLE_H_
#define _TASKTABLEVISITABLE_H_

#include <acs_c++config.hpp>
#include <exException.h>


#include <algorithm>
#include <functional>
#include <stack>

#include <boost/iterator/filter_iterator.hpp> // PRQA S 1013 3
#include <boost/iterator/indirect_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>




_ACS_BEGIN_NAMESPACE(acs)
	
/**
 *\brief A Class for defining the interface and the common implementation of task table visitable elements
 *       These elements can be "visited" by any ElementVisitor object
 *       The visitors are in charge to provide the implementation of execution 
 *       and read/write element using regular file or streams.
 */

class TaskTableVisitor;
class TaskTableVisitable;


typedef std::vector<TaskTableVisitable*>::const_iterator iterOnVectPTTV ;
	
template <class _itemtype, class Iter = iterOnVectPTTV>
class Deref_iter : public std::iterator_traits<Iter> // PRQA S 2109
{
public:
	friend bool operator ==(const Deref_iter& i, const Deref_iter& j)
	{
	    return i._curr == j._curr && i._end == j._end;
	}
	friend bool operator !=(const Deref_iter& i, const Deref_iter& j)
	{
	    return !(i==j);
	}

	Deref_iter(Iter p, Iter end) : _curr(p), _end(end)
	{
	    while (_curr!=_end && !dynamic_cast<_itemtype*>(*_curr))
	    {
		++_curr;
	    }
	}

	_itemtype& operator*() const
	{
	    return *dynamic_cast<_itemtype*>(*_curr);
	}

	_itemtype* operator->() const
	{
	    return dynamic_cast<_itemtype*>(*_curr);
	}

	Deref_iter& operator++() // prefix ++
	{
	    do
		++_curr;
	    while (_curr!=_end && !dynamic_cast<_itemtype*>(*_curr));
	    return *this;
	}

	Deref_iter operator++(int) // postfix ++
	{
	    Deref_iter tmp = *this;
	    ++ *this;
	    return tmp;
	}
private:
    Deref_iter() ; // not implemented
private:
    Iter _curr;
    Iter _end;
};

// some usefule predicates and helpers for aid in navigation (getList method)

template <class _itemtype> class always_true 
{
public:
	explicit always_true(const std::vector<_itemtype>&) {}
	bool operator() (const _itemtype&) const
	{
		return true;
	}

public:
	// Simplifying things 
    always_true() {} 
};

template <class _itemtype> always_true<_itemtype> always_true_pred(const std::vector<_itemtype>& v)
{
	return always_true<_itemtype>(v); // PRQA S 3081
}


// Template class needed to implement an STL algo 
// not provided wiht the current version of the GCC compiler
template<class In, class Out, class Pred>
Out copy_if(In first, In last, Out res, Pred p)
{
  while (first != last)
  {
    if (p(*first))
      *res++ = *first;
    ++first;
  }
  return res;
}



	template <typename _target> _target const *downcast(TaskTableVisitable const *p)
	{
	    // assumes the conversion has previously validated by is_downcastable
	    return static_cast<_target const *>(p);
	}


	template <typename _target> struct is_downcastable
	{
	    bool operator()(TaskTableVisitable const* p) { return dynamic_cast<_target const*>(p) != NULL; }
	};


class TaskTableVisitable // PRQA S 2109
{
public:

	/**
	 * Ctor and Dtor 
	 **/ 
	TaskTableVisitable(); 
	virtual ~TaskTableVisitable() throw() ;


	/**
	 * brief the clone method is used to copy the elements of the vector
	 *       The pointer are required in order to allow object polymorphism 
	 **/
	virtual TaskTableVisitable* clone() const = 0; // PRQA S 2502

	/**
	 * brief addElement implementation can take in charge the pointer 
	 *       creation for a new object using the specific clone method
	 *       This will help Element composition
	 **/
	void addElement (const TaskTableVisitable& e);
	void addElement (TaskTableVisitable* e);



	/**
	 *\brief This is the interface definition for
	 *       - reading from a resourceset
	 *       - writing to an XMLOstream
	 *       - executing an element
	 **/
	virtual bool acceptVisit(TaskTableVisitor& v);
	virtual bool acceptEnterVisit(TaskTableVisitor& v);
	virtual bool acceptExitVisit(TaskTableVisitor& v);
	virtual bool acceptVisitor (TaskTableVisitor& v);

	/**
	 * brief Retrieve the pointers to all the elements stored into the object
	 **/
	virtual std::vector<TaskTableVisitable*>& getElements ();
	virtual const std::vector<TaskTableVisitable*>& const_getElements () const;


	template <class _itemtype> Deref_iter<_itemtype> begin() const
	{
	    return Deref_iter<_itemtype>(_aggregates.begin(), _aggregates.end());
	}

	template <class _itemtype> Deref_iter<_itemtype> end() const
	{
	    return Deref_iter<_itemtype>(_aggregates.end(), _aggregates.end());
	}


template <class _itemtype> 
class TaskTableSmartVector
{

	typedef std::vector <_itemtype*> containerType;

	public:
		TaskTableSmartVector() :
			_container()
		{}

		void clear() throw()
		{
			typename containerType::iterator it;
			for (it = _container.begin(); it != _container.end(); ++it)
			{
				delete *it;
			}
			_container.clear();
		}

		void insert (_itemtype* ptr)
		{
			_container.push_back (ptr);
		}


		virtual ~TaskTableSmartVector () throw()
		{
			clear();
		}

		// Avoid external modification of the container, need to keep all the PTRs
		containerType const & container() const { return _container; }

	private:
		containerType _container;
};

/**
 * These methods gets a list of elements filtered by a predicate
 * It is possible to confine the search at the first level or
 * descend recursively
 */ 
template <class _itemtype, class _pred  > void getPtrList(
		TaskTableSmartVector< _itemtype >& aList,
		const _pred& aPred,
		bool recursive=false,
		bool erase=false
	) const
{

    if (erase) 
	{ 
		aList.clear(); 
    }

    std::vector<TaskTableVisitable*>::const_iterator p;
	std::vector<TaskTableVisitable*> recursiveList;

    for (p=_aggregates.begin(); p != _aggregates.end(); ++p)
    {
	    // if we're not recursive, but some of the item is a container, then recourse it
	    // (containers of TaskTableVistable are transparent)
	    // unless the to-be-recursed item has been already picked in the list

	    _itemtype const * itemPtr = dynamic_cast<_itemtype const *> (*p); // PRQA S 3081
	    bool have_we_peeked_it = false;

	    if (itemPtr) 
		{
	        have_we_peeked_it = aPred(*itemPtr);
			if (have_we_peeked_it)
			{
				// Store it in the safe container
				aList.insert ( dynamic_cast<_itemtype*> (itemPtr->clone()) );
			}
        }

	    if ( (recursive || (*p)->isContainer()) && !have_we_peeked_it ) 
		{
			recursiveList.push_back (*p);			
        }
    }

    std::vector<TaskTableVisitable*>::iterator it;

	for (it = recursiveList.begin(); it != recursiveList.end(); ++it)
	{
	        (*it)->getPtrList(aList, aPred, recursive, false);
	}	
}


	/**
	 * These methods gets a list of elements filtered by a predicate
	 * It is possible to confine the search at the first level or
	 * descend recursively
	 */ 
	template <class _itemtype, class _pred> void getList(std::vector<_itemtype>& aList,
							     const _pred& aPred,
							     bool recursive=false,
							     bool erase=false) const
	{
	    if (erase) { aList.clear(); 
    }



	    typedef boost::filter_iterator<is_downcastable<_itemtype>, std::vector<TaskTableVisitable *>::const_iterator>
	            downcast_iter;

#ifndef PRQA_ANALYZER
// NB qac++ is not able to analyze the following two lines of code.

	    downcast_iter begin_di = boost::make_filter_iterator<is_downcastable<_itemtype> >(_aggregates.begin(), _aggregates.end());
	    downcast_iter end_di   = boost::make_filter_iterator<is_downcastable<_itemtype> >(_aggregates.end(),   _aggregates.end());
#endif

#ifdef check_intermediate_steps
	    {
		std::vector<TaskTableVisitable const *> v;
		std::copy(begin_di, end_di, back_inserter(v));
	    }

	    {
		std::vector<_itemtype const *> v;
		std::copy(make_transform_iterator(begin_di, std::ptr_fun(downcast<_itemtype>)),
			  make_transform_iterator(end_di, std::ptr_fun(downcast<_itemtype>)),
			  back_inserter(v));
	    }

	    {
		std::vector<_itemtype> v;
		std::copy(make_indirect_iterator(make_transform_iterator(begin_di, std::ptr_fun(downcast<_itemtype>))),
			  make_indirect_iterator(make_transform_iterator(end_di, std::ptr_fun(downcast<_itemtype>))),
			  back_inserter(v));
	    }
#endif // check_intermediate_steps

	    {
#ifndef PRQA_ANALYZER
// NB qac++ is not able to analyze the following line of code.
		copy(make_filter_iterator(aPred,
					  make_indirect_iterator(make_transform_iterator(begin_di, std::ptr_fun(downcast<_itemtype>))),
					  make_indirect_iterator(make_transform_iterator(end_di, std::ptr_fun(downcast<_itemtype>)))),
		     make_filter_iterator(aPred,
					  make_indirect_iterator(make_transform_iterator(end_di, std::ptr_fun(downcast<_itemtype>))),
					  make_indirect_iterator(make_transform_iterator(end_di, std::ptr_fun(downcast<_itemtype>)))),
		     back_inserter(aList));
#endif
	    }



	    std::vector<TaskTableVisitable*>::const_iterator p;
	    for (p=_aggregates.begin(); p != _aggregates.end(); ++p)
	    {
		    // if we're not recursive, but some of the item is a container, then recourse it
		    // (containers of TaskTableVistable are transparent)
		    // unless the to-be-recursed item has been already picked in the list
		    // by the previous copy_if

		    _itemtype* itemPtr = dynamic_cast<_itemtype*> (*p); // PRQA S 3081
		    bool have_we_peeked_it = false;

		    if (itemPtr) {
		        have_we_peeked_it = aPred(*itemPtr);
            }

		    if ( (recursive || (*p)->isContainer()) && !have_we_peeked_it ) {
		        (*p)->getList(aList, aPred, recursive, false);
            }
	    }
	}


	virtual bool isContainer() const {return false;} // PRQA S 2131


	size_t size() const;
	virtual void clear();

	TaskTableVisitable const * operator[] (int idx) const {return _aggregates.at(idx);} // PRQA S 3000 2
	TaskTableVisitable * operator[] (int idx) {return _aggregates.at(idx);} // PRQA S 3000 2


	bool getEnterVisitResult() const;
	bool getVisitResult() const;
	bool getNavigationResult() const;


protected: // attributes  
	std::vector<TaskTableVisitable*> _aggregates; // PRQA S 2101
	/**
	 * Copy CTOR and assignment operator
	 **/
	TaskTableVisitable (const TaskTableVisitable&);
	TaskTableVisitable& operator= (const TaskTableVisitable&);


private: // attributes  
	struct StackData
	{
		bool enterVisitResult;
		bool visitResult;
		bool navigationResult;
	};

	std::stack<StackData > _visitStatusStack;

    Mutex _stackMutex;


private: 
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TaskTableVisitable) ;

};


_ACS_END_NAMESPACE

#endif //_TASKTABLEVISITABLE_H_
