/*
	Copyright 2012-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2014/06/16 14:33:46  lucio.pulvirenti
	S2PDGS-764: Added flag to IF not to log exception stack in case of not existing path.
	
	Revision 5.2  2014/02/12 12:35:27  marpas
	define INOTIFY_NOT_SUPPORTED for systems where inotify is not supported
	
	Revision 5.1  2013/10/25 13:00:11  marpas
	empty method added
	
	Revision 5.0  2013/06/20 16:43:52  marpas
	adoption of libException 5.x in progress
	
	Revision 1.7  2012/11/29 19:31:29  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 1.6  2012/11/19 16:23:55  lucio.pulvirenti
	EventElementsList::clear method added to clear list.
	
	Revision 1.5  2012/10/23 10:53:07  lucio.pulvirenti
	Relative paths removed by EventElement struct.
	DiskEventNotifier CTOR accepts now a boolean parameter that if set, uses Notify class not to duplicate events on all watched
	paths: events are retrieved per process.
	addPathToWatch both private and public methods added to centralize Notify invocation to watch paths.
	Events no more separated by relative path, but all events relative to the thread (same key) are retrieved.
	
	Revision 1.4  2012/10/17 17:01:26  lucio.pulvirenti
	work in progress
	
	Revision 1.3  2012/10/12 15:53:13  lucio.pulvirenti
	EventElementsList now is a container of WatchEventExtended objects.
	
	Revision 1.2  2012/10/05 16:12:37  lucio.pulvirenti
	work in progress.
	
	Revision 1.1  2012/10/04 16:24:03  lucio.pulvirenti
	First issue: work in progress.
	
	
        
*/

#ifndef _DiskEventNotifier_H_
#define _DiskEventNotifier_H_

#ifndef INOTIFY_NOT_SUPPORTED

#include <acs_c++config.hpp>
#include <Notify.h>

#include <Thread.h>
#include <exException.h>
#include <Timer.h>

#include <list>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief 
 **/
     
class DiskEventNotifier : public Thread { 
		
public:

    struct EventElement 
    {
        
	    std::string src_absolute_path = "" ; // source absolute path
	    std::string dst_absolute_path = "" ; // destination absolute path (if dst_is_set is true)
	    os::Notify::EventsMask event = {} ;	// event type
	    bool dst_is_set = false ;

	    // Ctor
	    EventElement() = default ;
	    explicit EventElement(os::Notify::Watch_Event const &);

	    void dump() const;		
    };

    /**
     * \brief It is a container of EventElement objects
     **/
	class EventElementsList // FIFO
	{
		
    public:

        exDECLARE_EXCEPTION(NoDataFound, exException) ; 

        // Ctor
        EventElementsList() = default ;
        ~EventElementsList() = default ;

        EventElementsList(EventElementsList const &) = default ;
        EventElementsList & operator = (EventElementsList const &) = default ;
        EventElementsList(EventElementsList &&) = default ;
        EventElementsList & operator = (EventElementsList &&) = default ;

        void push(EventElement const &);
        /**
         * \brief Pop the first inserted EventElement object of list
         **/
        void pop(EventElement &); // returns exception if list is empty
        /**
         * \brief Get the list of all the EventElement objects by insertion order
         **/
        void getData(std::list<EventElement> &) ;
        /**
         * \brief Dump all list elements
         **/
        void dump(std::list<EventElement> const &) const ;
        /**
         * \brief Clear list elements
         **/
        void clear() ;

        size_t getSize() const;
        bool empty() const;
        void setListMaxSize(size_t);

		/**
	 	 *  Set a condition that will be notified when new elements are pushed into the events list
		**/
		void setNotificationCondition (Condition*);

    private:		
        static const size_t _listDefaultMaxSize = 64*1024;
        static const Timer::Delay _getDataTimeout = 5 * 1000; // timeout in ms 
        std::list<EventElement> _list = {} ;
        ThreadSafe _mutex = {};
        size_t _listMaxSize = _listDefaultMaxSize ;

		Condition* _externalCondition = nullptr ;

        ACS_CLASS_DECLARE_DEBUG_LEVEL(EventElementsList) ;
		
    };


    exDECLARE_EXCEPTION(DiskEventNotifierException, exException) ;  

    /**
    * \brief  Class constructor 
    * \param IN pathToWatch path to watch for desired events
    * \param recursive, if true nested paths are watched too
    * \param events mask enum (see Notify.h)
    * \param OUT data structure filled with occurred events
    * \param if true, events are watched per process i.e. for different threads watching the same directories events will not be duplicated
    */
    DiskEventNotifier(std::string const & pathToWatch, bool recursive, os::Notify::EventsMask, EventElementsList &, bool = false );

    /**
    * \brief  Class constructor 
    * \param IN pathsToWatch paths to watch for desired events
    * \param recursive, if true nested paths are watched too
    * \param events mask enum (see Notify.h)
    * \param OUT data structure filled with occurred events
    * \param if true, events are watched per process i.e. for different threads watching the same directories events will not be duplicated
    */
    DiskEventNotifier(std::vector<std::string> const & pathsToWatch, bool recursive, os::Notify::EventsMask, EventElementsList &, bool = false );

    /**
    * \brief Class destructor 
    */
    ~DiskEventNotifier() noexcept override ;

	DiskEventNotifier(const DiskEventNotifier &) = delete ;
	DiskEventNotifier &operator=(const DiskEventNotifier &) = delete ;
	
    /**
    * \brief Add a new path to watch. It must be called after invocation of start method, otherwise exception is thrown 
    */
    void addPathToWatch(std::string const &);

    /**
    * \brief Returns paths to watch
    */
    void getPathsToWatch(std::vector<std::string> & w) const { w = _pathsToWatch; } 
	
	void setLogExcStack(bool v) { _logExcStack = v; }

protected:
		   
	void run() override ;
		   

private:

	/**
	* \brief Add a new path to watch.
	* \param The new path
	* \param If true, insert the new path into _pathsToWatch vector
	*/
	void addPathToWatch(std::string const &, bool insertIntoVector);

	/**
	* \brief Get thread notify unique key
	*/
	std::string getThreadKey();
	
	
// data
private: // NOSONAR - for clarity
		   
	std::vector<std::string> _pathsToWatch;
   	bool _recursive;
	os::Notify::EventsMask _registeredEvents;
	EventElementsList &_eventsList;
	std::string _notifyUniqueKey = "" ;
	Mutex _keyMutex = {} ;
	bool _processUnique;
	bool _logExcStack = true ;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(DiskEventNotifier) ;
     
};

_ACS_END_NAMESPACE

#endif // INOTIFY_NOT_SUPPORTED

#endif	/*_DiskEventNotifier_H_ */

