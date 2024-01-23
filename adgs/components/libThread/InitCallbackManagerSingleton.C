// PRQA S 1050 EOF
/*
	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

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
using namespace std;
namespace
{
	// Helper class (supports checks on the occurency of callback duplications)
	class isTheSameCallback // PRQA S 2185
	{
	public:
        typedef InitCallbackManagerSingleton::funcPtr funcPtr ;
        typedef InitCallbackManagerSingleton::PvtMessage PvtMessage ;
        
		explicit isTheSameCallback(const std::pair<funcPtr, PvtMessage> & theCallback) : _callback(theCallback) {}; // PRQA S 2528 
		~isTheSameCallback() ACS_NOEXCEPT {};
		
		bool operator() (const std::pair<funcPtr, PvtMessage>& p) const
		{    
			// returns true if the callback is found 
 			// label neglected
 			return (p.first == _callback.first);

		}

		// Copy Constructor
		isTheSameCallback(const isTheSameCallback &s ) : _callback (s._callback) {};  // PRQA S 2528

	private:	
		// Operator =
		isTheSameCallback &operator=(const isTheSameCallback&);

		const std::pair<funcPtr, PvtMessage> & _callback;
	};
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(InitCallbackManagerSingleton)

InitCallbackManagerSingleton::InitCallbackManagerSingleton() : 
			Singleton <InitCallbackManagerSingleton>(),
            _callbackMap(),
            _fired (false),
			_failedCallback()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
}

InitCallbackManagerSingleton::~InitCallbackManagerSingleton() ACS_NOEXCEPT
{
}

bool InitCallbackManagerSingleton::fire()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
    if (not _fired) {
 	    // loop on priorities	
	    for (map<int, vector<pair<funcPtr, PvtMessage> > >::reverse_iterator it = _callbackMap.rbegin(); it != _callbackMap.rend(); ++it) // PRQA S 4238
	    {
		    // for each priority, cycle on callbacks
		    for (size_t k = 0; k < it->second.size(); ++k)
		    {
			    pair<funcPtr, PvtMessage> p = it->second[k];
			    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Initializing " << p.second.first );
			    try 
			    {
				    // execute callback
				    (*p.first)(); 
                    if (p.second.second) {					
				        ACS_LOG_PRIVINFO(p.second.first << " correctly initialized." );
                    }
                    else {
				        ACS_LOG_INFO(p.second.first << " correctly initialized." );
                    }
                    _fired = true ; // at least one has been called
			    }
			    catch(exception& e)
			    {
				    ACS_LOG_WARNING("Error occured initializing " << p.second.first << ". Got exception: " << e.what());
				    // save the name of failed callback
				    _failedCallback.push_back(p.second.first);
			    }
		    }
	    }
        _fired = true ;
    }
    
    return _fired ;
}

void InitCallbackManagerSingleton::addCallback(funcPtr f, const std::string & label, const int & priority, bool pvt)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	ACS_COND_THROW(_fired, exTooLateException("Too late attempt to add \"" +  label + "\" to the initialization map."));	// PRQA S 3081
	bool notFound = true ;
	
	map<int, vector<pair<funcPtr, PvtMessage> > >::iterator it = _callbackMap.begin();
	// cicle on priorities
	while (it != _callbackMap.end())
	{
		// find if the callaback has been alredy inserted 
		vector<pair<funcPtr, PvtMessage> >::iterator found = find_if(it->second.begin(), it->second.end(), isTheSameCallback(make_pair(f, make_pair(label, true)))); // PRQA S 3081
		if (found != it->second.end()) // callback already insered: check priority
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Just found: " << label);
			notFound = false;
			// higher priority wins
			if (priority > it->first)
			{
				// replace the occurency
				it->second.erase(found);
				if (it->second.empty())
				{
					_callbackMap.erase(it);
				}
					
				_callbackMap[priority].push_back(make_pair(f, make_pair(label,pvt)));				
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, label << " replaced in the map for priority " << priority );

			}
			it = _callbackMap.end();	// force loop exit
			
		}
		else
		{
			it++;
		}
	}
	if (notFound)	// new callback
	{
		_callbackMap[priority].push_back(make_pair(f, make_pair(label, pvt)));
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Inserted " << label << " with priority " << priority);
	}
}

void InitCallbackManagerSingleton::reset()
{
	_fired = false;
	_callbackMap.clear() ;
    _failedCallback.clear() ;
}



_ACS_END_NAMESPACE

