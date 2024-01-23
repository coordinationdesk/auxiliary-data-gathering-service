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
  Revision 1.4  2015/10/22 15:16:28  marpas
  new callback registration method added: it can choose to emit label with std info or pvt info filterable class.
  this version has been tested with no regressions and coverage test

  Revision 1.3  2015/10/22 11:35:17  marpas
  InitCallbackManagerSingleton class improved: more simple, documented and more secure in terms of wrong use
  test changed accordingly

  Revision 1.2  2015/05/27 13:24:06  damdec
  Some attributes made not static.

  Revision 1.1  2015/05/26 15:20:10  damdec
  Class added.


*/

#include <InitCallbackManagerSingleton.h>
#include <exception>
#include <Filterables.h>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(InitCallbackManagerSingleton)

InitCallbackManagerSingleton::InitCallbackManagerSingleton() :
  Singleton<InitCallbackManagerSingleton>()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

bool InitCallbackManagerSingleton::fire()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);

  // Immediately return if already fired
  if (_fired)
  {
    return true;
  }

  // Loop on priorities
  for (auto it = _callbackMap.rbegin(); it != _callbackMap.rend(); ++it)
  {
    // For each priority, cycle on callbacks
    for_each(it->second.begin(), it->second.end(), [this](const CallbackData& data) {
      ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Initializing " << data.label);
      try
      {
        // Execute callback
        data.callback();
        if (data.privateInfo)
        {
          ACS_LOG_PRIVINFO(data.label << " correctly initialized.");
        }
        else
        {
          ACS_LOG_INFO(data.label << " correctly initialized.");
        }
      }
      catch (const std::exception& e)  // NOSONAR - Want to be sure that all exception are catch
      {
        ACS_LOG_WARNING("Error occurred initializing " << data.label << ". Got exception: " << e.what());
        // Save the name of failed callback
        _failedCallback.push_back(data.label);
      }
    });
  }

  _fired = true;  // If not already set this mean nothing to initialize

  return _fired;
}

void InitCallbackManagerSingleton::addCallback(const FuncPtr& func, const std::string& label, const int& priority,
                                               bool pvt)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
  ACS_COND_THROW(_fired, exTooLateException("Too late attempt to add \"" + label + "\" to the initialization map."));
  bool found = false;

  using FuncRef = void (*const* const)();
  FuncRef callbackFunction(func.target<void (*)()>());
  // Cycle on priorities
  for (auto& callback : _callbackMap)
  {
    // Find if the callaback has been already inserted
    auto tbs = find_if(callback.second.begin(),
                       callback.second.end(),
                       [&callbackFunction](const CallbackData& p) {
                         FuncRef pF = p.callback.target<void (*)()>();
                         return (callbackFunction && pF && *callbackFunction == *pF);
                       });

    if (tbs != callback.second.end())  // callback already inserted: check priority
    {
      ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Just found: " << label);
      found = true;
      // Higher priority wins
      if (priority > callback.first)
      {
        // Replace the occurency
        callback.second.erase(tbs);
        _callbackMap[priority].emplace_back(func, label, pvt);
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, label << " replaced in the map for priority " << priority);
      }
      break;
    }
  }

  if (!found)  // New callback
  {
    _callbackMap[priority].emplace_back(func, label, pvt);
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Inserted " << label << " with priority " << priority);
  }
}

void InitCallbackManagerSingleton::reset()
{
  _fired = false;
  _callbackMap.clear();
  _failedCallback.clear();
}

_ACS_END_NAMESPACE
