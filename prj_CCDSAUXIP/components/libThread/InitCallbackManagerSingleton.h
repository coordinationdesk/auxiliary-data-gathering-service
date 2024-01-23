/*
  Copyright 1995-2022, Exprivia SpA.
  Via Della Bufalotta, 378 - 00139 Roma - Italy
  http://www.exprivia.com

  All Rights Reserved.

  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
  the contents of this file may not be disclosed to third parties, copied or
  duplicated in any form, in whole or in part, without the prior written
  permission of Exprivia SpA.

  $Prod: A.C.S. libThread$

  $Id$

  $Author$

  $Log$
  Revision 1.3  2015/10/22 11:35:17  marpas
  InitCallbackManagerSingleton class improved: more simple, documented and more secure in terms of wrong use
  test changed accordingly

  Revision 1.2  2015/05/27 13:24:06  damdec
  Some attributes made not static.

  Revision 1.1  2015/05/26 15:20:10  damdec
  Class added.
*/

#ifndef _InitCallbackManagerSingleton_H_
#define _InitCallbackManagerSingleton_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <Singleton.hpp>
#include <map>
#include <vector>
#include <functional>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton;

/*!
 * \brief
 * Singleton implementing the controlled initialization of callbacks (not thread-safe)
 **/

class InitCallbackManagerSingleton : public Singleton<InitCallbackManagerSingleton>
{
  // SINGLETON
  friend InitCallbackManagerSingleton* Singleton<InitCallbackManagerSingleton>::instance();

public:
  /*! \brief shortcut for pointer to function getting no args and returning void */
  using FuncPtr = std::function<void()>;

  // InitCallbackManagerSingleton Exceptions
  /*! \brief class' exception family */
  exDECLARE_EXCEPTION(exInitCallbackManagerSingletonException, exException);
  /*! \brief exception to signal is too late for adding other callbacks */
  exDECLARE_EXCEPTION(exTooLateException, exInitCallbackManagerSingletonException);

  /*! \brief std dtor */
  ~InitCallbackManagerSingleton() noexcept override = default;

  /*! \brief convenience static method to call non-static one \see addCallback */
  static void addInitCallback(const FuncPtr& f, const std::string& lbl, const int& prio, bool pvt = false)
  {
    InitCallbackManagerSingleton::instance()->addCallback(f, lbl, prio, pvt);
  }

  /*! \brief method to add callbacks
   \param f the function callback to be called when fired
   \param label the label emitted once successfully called
   \param priority is the priority that will sort the callback sequence
   \param pvt shall be true if the message shall be emitted with a private info filterable

   When the fire method will be called, all the callbacks will be called in reverse priority sort order (the grater the
   priority the sooner the call wrt lower priorities) and after a successful call, meaning no exception from it, the
   label will be emitted with an Info or a PvtInfo filterable class
   \see fire
  */
  void addCallback(const FuncPtr& f, const std::string& label, const int& priority, bool pvt);
  /*! \return true if the init callback have been called */
  bool fired() const noexcept { return _fired; }
  /*! \returns the list of failed callback */
  const std::vector<std::string>& getFailed() const noexcept { return _failedCallback; }
  /*! \return true if all callbacks have been successfully executed */
  bool areAllOk() const noexcept { return _failedCallback.empty(); }  // no failed means all ok

protected:
  /*- protected ctor: is a Singleton */
  InitCallbackManagerSingleton();
#ifdef TEST_THREAD
public:
#else
private:
#endif
  /*- \brief reset the class status
   This method resets the class as it had never fired any callback
  */
  void reset();

  /*! \brief class Thread can call fire*/
  friend class Thread;
  /*! \returns true if initialization have been done */
  bool fire();

#ifdef TEST_THREAD
private:
#endif

  /*! \brief convenience type definition */
  struct CallbackData
  {
    FuncPtr callback;
    std::string label;
    bool privateInfo;

    CallbackData(const FuncPtr& cb, const std::string& lbl, const bool pv) :
      callback(cb),
      label(lbl),
      privateInfo(pv)
    {
    }
  };

  // Key: priority, Value: vector of CallbackData
  std::map<int, std::vector<CallbackData> > _callbackMap = {};
  // flag indicating if initialization has been performed
  bool _fired = false;
  // vector containing the list of labels describing callbacks for which initialization has failed
  std::vector<std::string> _failedCallback = {};

  ACS_CLASS_DECLARE_DEBUG_LEVEL(InitCallbackManagerSingleton)
};

_ACS_END_NAMESPACE

#endif  //_InitCallbackManagerSingleton_H_
