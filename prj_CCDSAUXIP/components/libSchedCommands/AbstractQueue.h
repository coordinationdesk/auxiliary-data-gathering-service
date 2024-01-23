// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Scheduler Abstract Commands Library $

	$Id$

	$Author$

	$Log$
	Revision 1.5  2011/03/01 16:26:43  marpas
	GCC 4.4.x support
	
	Revision 1.4  2011/01/24 15:50:33  marpas
	Mutex.h does not longer exixts
	
	Revision 1.3  2005/10/12 15:02:04  marpas
	GCC 3.4.4 fixed
	
	Revision 1.2  2004/12/03 17:57:14  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:31:01  marfav
	Import libSchedCommands
	

*/


#ifndef _AbstractQueue_H_
#define _AbstractQueue_H_

#include <ThreadSafe.h>
#include <queue>
#include <vector>
#include <memory>

_ACS_BEGIN_NAMESPACE(acs)


template <class T>
class AbstractQueue : protected std::queue<T*>
{
public:
	AbstractQueue () : std::queue<T*>(), _mutex() {}
	virtual ~AbstractQueue () throw() {}

	AbstractQueue (const AbstractQueue &r) : std::queue<T*>(r), _mutex() {}
	AbstractQueue& operator= (const AbstractQueue& r)
    {
	    if (this != &r)
	    {
		    Lock g1(r._mutex);
		    Lock g2(_mutex);
		    std::queue<T*>::operator=(r) ; // PRQA S 3072
	    }
	    return *this;
    }


	void clear();
	void push (T*);
	void push (std::vector<T*>&);
	bool pop (ACS_SMARTPTR<T>&);
	bool empty();

protected:
	virtual void pushElement(T*);

private:
	acs::ThreadSafe _mutex;

};

template <class T> void
AbstractQueue<T>::push (T* anElement)
{
	Lock guard(_mutex);
	pushElement (anElement);
}

template <class T> void
AbstractQueue<T>::push (std::vector<T*>& anElementVector)
{
	Lock guard(_mutex);
	for (typename std::vector<T*>::const_iterator it=anElementVector.begin(); it!=anElementVector.end(); ++it) {
		pushElement (*it);
    }
}

template <class T> void
AbstractQueue<T>::clear ()
{
	Lock guard(_mutex);
	std::queue<T*>::clear() ;
}

template <class T> bool
AbstractQueue<T>::pop (ACS_SMARTPTR<T>& theElement)
{
	Lock guard(_mutex);
	if (!std::queue<T*>::empty())
	{
		theElement.reset(std::queue<T*>::front());
		std::queue<T*>::pop();
		return true;
	}
	else
		return false;
}

template <class T> bool
AbstractQueue<T>::empty ()
{
	Lock guard(_mutex);
	return std::queue<T*>::empty();
}

// PRIVATE METHOD - this can not be guarded by mutex and there is no need to do that
template <class T> void
AbstractQueue<T>::pushElement (T* anElement)
{
	std::queue<T*>::push (anElement);
}



_ACS_END_NAMESPACE


#endif //_AbstractQueue_H_
