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

  $Prod: A.C.S. Exception Library $

*/

#ifndef _Pattern_Observer_H_
#define _Pattern_Observer_H_

#include <vector>
#include <algorithm>
#include <memory>

#include <exException.h>
#include <ThreadSafe.h>
#include <Filterables.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, pattern)
template<class T>
class Observer;

// PRQA S 1703 EOF
template<class T>
class Observable
{
public:
  Observable();
  virtual ~Observable() ACS_NOEXCEPT;

  /**
    * Register the observer.
    * Take care not to register the same observer more than once.
    * \return True if the registration went well.
    */
  bool registerObserver(std::shared_ptr<Observer<T>> observer);

  /**
    * Unregister the observer.
    */
  void unregisterObserver(std::shared_ptr<Observer<T>> observer);

  /**
    * Clean the list of registered observers.
    */
  void unregisterAllObservers() ACS_NOEXCEPT;

protected:
  /**
    * Invokes handleMessage(message) for every observer that registered itself.
    */
  void notify(T message);

private:
  Observable(const Observable<T>&);
  Observable<T>& operator=(const Observable<T>&);

  std::vector<std::weak_ptr<Observer<T>>> _observers;
  ThreadSafe _mutex;

  ACS_TEMPLATE_DECLARE_DEBUG_LEVEL(Observable, T);
};

ACS_TEMPLATE_DEFINE_DEBUG_LEVEL(Observable, T);

template<class T>
Observable<T>::Observable() :
  _observers(),
  _mutex()
{
  ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "me: " << std::hex << this << "; ctor call"); //LCOV_EXCL_BR_LINE
}

template<class T>
Observable<T>::~Observable() ACS_NOEXCEPT
{
  ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "me: " << std::hex << this << "; dtor call"); //LCOV_EXCL_BR_LINE

  unregisterAllObservers();
}

template<class T>
bool Observable<T>::registerObserver(std::shared_ptr<Observer<T>> observer)
{
  Lock guard(_mutex);

  //LCOV_EXCL_BR_START
  ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "me: " << std::hex << this << "; registring observer: " << std::hex 
                                             << observer.get());
  //LCOV_EXCL_BR_STOP

  typename std::vector<std::weak_ptr<Observer<T>>>::iterator it =
    std::find_if(_observers.begin(), _observers.end(), [&observer](const std::weak_ptr<Observer<T>>& ptr1) {
    return ptr1.lock() == observer;
  });

  if (_observers.end() == it)
  {
    std::weak_ptr<Observer<T>> weakObserver(observer);
    _observers.push_back(weakObserver);

    //LCOV_EXCL_BR_START
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "me: " << std::hex << this << "; observer " << std::hex << observer.get() 
                                               << " registred");
    //LCOV_EXCL_BR_STOP
    return true;
  }
  else
  {
    //LCOV_EXCL_BR_START
    ACS_LOG_WARNING("me: " << std::hex << this << "; observer: " << std::hex << observer.get() << " already registred");
    //LCOV_EXCL_BR_STOP
  }

  return false;
}

template<class T>
void Observable<T>::unregisterObserver(std::shared_ptr<Observer<T>> observer)
{
  Lock guard(_mutex);

  //LCOV_EXCL_BR_START
  ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "me: " << std::hex << this << "; unregistring observer " << std::hex 
                                             << observer.get());
  //LCOV_EXCL_BR_STOP

  typename std::vector<std::weak_ptr<Observer<T>>>::iterator it =
    std::find_if(_observers.begin(), _observers.end(), [&observer](const std::weak_ptr<Observer<T>>& ptr1) {
    return ptr1.lock() == observer;
  });
  if (_observers.end() != it)
  {
    //LCOV_EXCL_BR_START
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "me: " << std::hex << this << "; observer " << std::hex << observer.get()
                                               << " unregistered");
    //LCOV_EXCL_BR_STOP
    _observers.erase(it);
  }
}

template<class T>
void Observable<T>::unregisterAllObservers() ACS_NOEXCEPT
{
  Lock guard(_mutex);

  ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "me: " << std::hex << this << "; unregistring all observers"); //LCOV_EXCL_BR_LINE

  _observers.clear();
}

template<class T>
void Observable<T>::notify(T message)
{
  Lock guard(_mutex);

  ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "me: " << std::hex << this << " notifying a new message"); //LCOV_EXCL_BR_LINE

  for (typename std::vector<std::weak_ptr<Observer<T>>>::iterator it = _observers.begin(); it != _observers.end();)
  {

    std::shared_ptr<Observer<T>> observer = (*it).lock();
    if (observer)
    {
      //LCOV_EXCL_BR_START
      ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "me: " << std::hex << this << "; calling observer: " << std::hex
                                                 << observer.get());
      //LCOV_EXCL_BR_STOP

      observer->handleMessage(message);
      it++;
    }
    else
    {
      it = _observers.erase(it);

       //LCOV_EXCL_BR_START
      ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "me: " << std::hex << this
                                                 << "; removed an expired observer while notifying a new message");
       //LCOV_EXCL_BR_STOP
    }
  }
}

template<class T>
class Observer
{
  template<class U>
  friend void Observable<U>::notify(U message);

public:
  Observer();
  virtual ~Observer() ACS_NOEXCEPT;

protected:
  /**
    * Handle a message from an observable
    */
  virtual void handleMessage(T msg) = 0;

  ACS_TEMPLATE_DECLARE_DEBUG_LEVEL(Observer, T);
};

ACS_TEMPLATE_DEFINE_DEBUG_LEVEL(Observer, T);

template<class T>
Observer<T>::Observer() {
  ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "me: " << std::hex << this << "; ctor call"); //LCOV_EXCL_BR_LINE
}

template<class T>
Observer<T>::~Observer() ACS_NOEXCEPT
{
  ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "me: " << std::hex << this << "; dtor call"); //LCOV_EXCL_BR_LINE
}

_ACS_END_NESTED_NAMESPACE

#endif  // _Pattern_Observer_H_
