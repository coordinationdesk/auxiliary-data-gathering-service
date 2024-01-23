// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. File Notify Library $

	$Id$

	$Author$

	$Log$
	Revision 5.5  2018/07/13 09:46:43  marfav
	Init of inotify structure conditioned to __flexarray logic
	
	Revision 5.4  2018/07/13 09:16:57  marfav
	inotify structure is not used by derivation but by encapsulation
	
	Revision 5.3  2014/02/12 12:35:27  marpas
	define INOTIFY_NOT_SUPPORTED for systems where inotify is not supported
	
	Revision 5.2  2013/07/02 17:31:26  marpas
	adoption of libException 5.x standards in progress, qa rules adn compilation warning fixed
	
	Revision 5.1  2013/06/21 10:31:40  marpas
	adoption of libException 5.x statndards in progress, qa rules adn compilation warning fixed
	
	Revision 5.0  2013/06/20 16:43:53  marpas
	adoption of libException 5.x in progress
	
	Revision 1.14  2013/03/04 15:25:23  enrcar
	EC:: warning removed
	
	Revision 1.13  2013/01/25 11:42:46  marpas
	using pattern::Singleton from libException
	
	Revision 1.12  2012/11/30 14:09:44  marpas
	qa rules and optimization
	
	Revision 1.11  2012/11/29 19:31:29  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 1.10  2012/10/24 14:24:39  lucio.pulvirenti
	EC:: unregister_userkey method added.
	
	Revision 1.9  2012/10/23 16:03:43  lucio.pulvirenti
	method flush_mismatched_cookies added
	threshold for map stored_events_ added
	map stored_events_ : vector turned to deque in order to allow faster removal of elements
	method check_stored_events_trasholds added
	
	Revision 1.8  2012/10/22 13:44:35  lucio.pulvirenti
	A parameter unique was added to the methods watch_directory_* . When set for a path, the path can be watched at maximum once per process (that is, multiple threads cannot share the same path)
	
	Revision 1.7  2012/10/17 15:44:23  lucio.pulvirenti
	generate_unique_userkey overloaded method added getting thread id as input and optional string
	
	Revision 1.6  2012/10/17 14:06:20  lucio.pulvirenti
	get_events_* methods: if wait_msec is zero, no poll is actually performed: the method just fills the list of queued events for the userkey, if any
	add_entry method: the return value was always set to TRUE. Fixed, now is evaluated for each userkey
	
	Revision 1.5  2012/10/16 15:42:26  lucio.pulvirenti
	EC:: improved and tested in multi-thread environment
	Added public method: get_events_map_originator_wd
	
	Revision 1.4  2012/10/12 15:05:03  enrcar
	EC:: Added method generate_unique_userkey
	
	Revision 1.3  2012/10/12 13:18:25  enrcar
	EC:: Several modifications. The userkey (which also works as threadid) has been added
	
	Revision 1.2  2012/10/05 16:10:53  lucio.pulvirenti
	Singleton<Notify> added.
	
	Revision 1.1  2012/10/04 13:30:05  lucio.pulvirenti
	Imported from PDS2 environment.

*/
 
#ifndef _Os_Notify_H_
#define _Os_Notify_H_

#ifndef INOTIFY_NOT_SUPPORTED

#include <acs_c++config.hpp>

#include <ThreadSafe.h>
#include <exException.h>
#include <Filterables.h>
#include <Singleton.hpp>


#include <sys/inotify.h>    // PRQA S 1013

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstring>
#include <vector>
#include <deque>
#include <map>

namespace {
	struct inotify_event MY_DEF_INOT_EVENT;
}; // unnamed namespace

_ACS_BEGIN_NESTED_NAMESPACE(acs, os)


using pattern::Singleton ;


/*! \addtogroup os 
 *  @{
 */
/*! \class Notify
	
	Utility class to manage changes on filesystem using inotify.
*/
class Notify                                                                 // PRQA S 2109
{
	
public:

	/* Generic exceptions: (derived directly from exException) */
	exDECLARE_EXCEPTION(NotifyException, exException) ;                     // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(NotifyCriticalException, exCriticalException) ;     // PRQA S 2131, 2153, 2502
	exDECLARE_EXCEPTION(NoSuchFileOrDirectoryException, NotifyException) ;  // PRQA S 2131, 2153, 2502
	exDECLARE_EXCEPTION(NotADirectoryException, NotifyException) ;          // PRQA S 2131, 2153, 2502

		
	
public:

	/** Mask for all possible notifying events */
	
	enum EventsMask { 
			
		NOTIFY_NOTHING 			= 0,				/* DO NOT NOTIFY EVENTS */		    		 
		NOTIFY_ALL	 			= IN_ALL_EVENTS,	/* NOTIFY ALL POSSIBLE EVENTS */		    		 

		NOTIFY_ACCESS 			= IN_ACCESS,		/* File was accessed */		    		 
		NOTIFY_MODIFY 			= IN_MODIFY,		/* File was modified */		    		 
		NOTIFY_ATTRIB 			= IN_ATTRIB,		/* Metadata changed */		    		 
		NOTIFY_CLOSE_WRITE		= IN_CLOSE_WRITE,	/* Writtable file was closed */  		 
		NOTIFY_CLOSE_NOWRITE	= IN_CLOSE_NOWRITE,	/* Unwrittable file closed */    		 
		NOTIFY_CLOSE 			= 					/* File closed (any kind) */
			(NOTIFY_CLOSE_WRITE | NOTIFY_CLOSE_NOWRITE),	
		NOTIFY_OPEN				= IN_OPEN,			/* File was opened */		    		 
		NOTIFY_MOVED_FROM		= IN_MOVED_FROM,	/* File was moved from self */		    		 
		NOTIFY_MOVED_TO			= IN_MOVED_TO,		/* File was moved to self */		    		 
		NOTIFY_MOVE 			=					/* File was moved (any kind) */	    		 
			(NOTIFY_MOVED_FROM | NOTIFY_MOVED_TO),	
		NOTIFY_CREATE 			= IN_CREATE,		/* File was created */  			   
		NOTIFY_DELETE 			= IN_DELETE,		/* File was deleted */	    		 
		NOTIFY_DELETE_SELF 		= IN_DELETE_SELF,	/* Self was deleted */		    		 
		NOTIFY_MOVE_SELF 		= IN_MOVE_SELF,		/* Self was moved */ 	
			
		NOTIFY_IGNORED 			= IN_IGNORED,		/* Entry is ignored */ 	
    		 
		NOTIFY_ISDIR 			= IN_ISDIR			/* Flag SET if entry is directory */ 		    		 

	} ;	/* enum EventsMask */
	
private :

	/** Features of each monitored entry */

	struct Entry_features // PRQA S 2173, 2175
	{
		Entry_features() : originator_wd(-1), mask(0), rec(false), full_path(""), rel_path("") { }
        // PRQA S 2100 6
		int 		originator_wd ; // Originator watch descriptor
		uint32_t 	mask ;			// Mask
		bool 		rec ; 			// Recursive (this one & future) subdirectories
		std::string full_path ; 	// Absolute, full-path
		std::string rel_path ; 		// Relative path (usually relative to a user's root)
	} ;


	/** The structure below extends the basic inotify_event in order to contain ALSO the optional filename payload.
		This allows copying into memory the base structure. See the documentation for inotify_event */
	
// the following pragma has been added to avoid - warning: ... a non-virtual destructor
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"       // PRQA S 1040 2
#pragma GCC diagnostic ignored "-Weffc++"


	struct Extended_inotify_event     // PRQA S 2116, 2153, 2173, 2175
	{
		Extended_inotify_event() : 
            stored_name(new char[stored_name_size]), 
            last_update(get_key_from_curtime()),
            ionot_event(MY_DEF_INOT_EVENT)
        {
	    ionot_event.wd=-1;
            ionot_event.len=0; 
        }
        
		explicit Extended_inotify_event(const inotify_event& p) : 
            stored_name(new char[stored_name_size]), 
            last_update(get_key_from_curtime()),
            ionot_event(p)
		{
			strncpy(stored_name, p.name, p.len) ; // copy only those with meaning
		    if (p.len < stored_name_size) {
                stored_name[p.len] = 0 ; // null terminate
            }
        }

		Extended_inotify_event(const Extended_inotify_event& p) : 
            stored_name(new char[stored_name_size]), 
            last_update(p.last_update),
            ionot_event(p.ionot_event)
		{
			strncpy(stored_name, p.stored_name, p.ionot_event.len) ; // copy only those with meaning
		    if (p.ionot_event.len < stored_name_size) {
                stored_name[p.ionot_event.len] = 0 ; // null terminate
            }
        }
        
        Extended_inotify_event& operator= (const Extended_inotify_event &p) 
        {
            if (this != &p) {
				ionot_event = p.ionot_event;
 			    strncpy(stored_name, p.stored_name, p.ionot_event.len) ; // copy only those with meaning
                last_update = p.last_update ;
		        if (p.ionot_event.len < stored_name_size) {
                    stored_name[p.ionot_event.len] = 0 ; // null terminate
                }
            } 
            return *this ;
        }

		virtual ~Extended_inotify_event() { delete [] stored_name ; } // PRQA S 2131

        static const unsigned int stored_name_size = 4096 ;
        // PRQA S 2100 3
		char *stored_name ; 
		uint64_t last_update ; 
		struct inotify_event ionot_event;
	} ;
	
// reactivate -Wnon-virtual-dtor
#pragma GCC diagnostic pop


	/** Map between the userkey and its monitored entries (the same entry might be monitored with different userkeys) */

	typedef std::map<std::string, Entry_features> Entry_feat_by_userkey ;	/* key of the map: user_key */

public:

	/** A containter of following structures will be returned as list of events */
	
	class Watch_Event // PRQA S 2109
	{	
	public:
		Watch_Event () : 
            src_absolute_path(""), 
            src_relative_path(""), 
            dst_absolute_path(""), 
            dst_relative_path(""), 
            event(), 
            dst_is_set(false), 
            src_wd(0), 
            cookie(0), 
            last_update(0) 
		{ }

        virtual ~Watch_Event() throw() {}                // PRQA S 2131

        Watch_Event(const Watch_Event &rhs) :
            src_absolute_path(rhs.src_absolute_path), 
            src_relative_path(rhs.src_relative_path), 
            dst_absolute_path(rhs.dst_absolute_path), 
			dst_relative_path(rhs.dst_relative_path), 
            event(rhs.event), 
            dst_is_set(rhs.dst_is_set), 
            src_wd(rhs.src_wd), 
            cookie(rhs.cookie), 
            last_update(rhs.last_update) 
        { }

		Watch_Event &operator=(const Watch_Event &rhs) {
			if (this != &rhs) {
				// copy parameters
				src_absolute_path = rhs.src_absolute_path ;
				src_relative_path = rhs.src_relative_path ;
				dst_absolute_path = rhs.dst_absolute_path ;
				dst_relative_path = rhs.dst_relative_path ;
				event = rhs.event ;
				dst_is_set = rhs.dst_is_set ;
				src_wd = rhs.src_wd ; 
				cookie = rhs.cookie ;
				last_update = rhs.last_update ;
			}
			return (*this) ; 
		}


	public:
		void build(const std::string& userkey, const struct inotify_event *pevent, const std::map <int, Notify::Entry_feat_by_userkey>& entry_map) ;
		void build(const std::string& userkey, const struct Extended_inotify_event &ext_inot_event , const std::map <int, Notify::Entry_feat_by_userkey>& entry_map) ;
		void dump() const ;	

		static Watch_Event merge(const Watch_Event& src, const Watch_Event& dst) ; // PRQA S 2502

	public:	
        // PRQA S 2100 L1
		std::string src_absolute_path ;
		std::string src_relative_path ;
		std::string dst_absolute_path ; 
		std::string dst_relative_path ;
		EventsMask event ;
		bool dst_is_set ;
		int src_wd ; 
		uint32_t cookie ; 
		uint64_t last_update ; 
		ACS_CLASS_DECLARE_DEBUG_LEVEL(Watch_Event) ;
        // PRQA L:L1 
	} ;


private:

	/** A sorted containter used to return events to the user (base class) */

	class Watch_Event_Container_BASE {
	public:
		Watch_Event_Container_BASE() {}
 		virtual ~Watch_Event_Container_BASE() throw() {}    // PRQA S 2131
		virtual void dump(const std::string&) const = 0 ;
		virtual void insert(const std::string& userkey, const Watch_Event&) = 0 ;
		virtual size_t size() const = 0 ;
		virtual bool empty() const throw() = 0 ;
    protected:
        Watch_Event_Container_BASE(const Watch_Event_Container_BASE &) {}
        Watch_Event_Container_BASE &operator= (const Watch_Event_Container_BASE &) { return *this ; } // PRQA S 4072
        
	} ;

	/** Sorted containters used to return events to the user (derived classes) */

	class Watch_Event_Container_Map_RelPath : public Watch_Event_Container_BASE {   // PRQA S 2109
	public:
		Watch_Event_Container_Map_RelPath(
                std::multimap<std::string, Watch_Event>& m,
                const std::map <int, Entry_feat_by_userkey>& inot_entries 
        ) : 
              Watch_Event_Container_BASE(),
              m_(m)     // PRQA S 2528
        {}

		virtual ~Watch_Event_Container_Map_RelPath() throw() {}                     // PRQA S 2131

		virtual void dump(const std::string& userkey) const {                         // PRQA S 2131
			ACS_LOG_DEBUG("Userkey: \"" << userkey << "\"") ;
			size_t j=0 ; 
            // PRQA S 4238 2
            for (std::multimap<std::string, Notify::Watch_Event>::const_iterator i=m_.begin(); i!=m_.end() ; ++i)
			{
				ACS_LOG_DEBUG("Event #" << (++j) << "  Key: \"" << i->first << "\"  :"); 
				(i->second).dump() ; 
				ACS_LOG_DEBUG(std::string(100,'-'));
			}
		}
        
		virtual void insert(const std::string& userkey, const Watch_Event& we)      // PRQA S 2131
        { 
            m_.insert( std::make_pair(we.src_relative_path, we)) ; 
        }
		
        virtual size_t size() const { return m_.size() ; }                          // PRQA S 2131 2
		virtual bool empty() const throw() { return m_.empty() ; }
    private:
        Watch_Event_Container_Map_RelPath() ; // not implemented 
        Watch_Event_Container_Map_RelPath(const Watch_Event_Container_Map_RelPath &) ; // not implemented 
        Watch_Event_Container_Map_RelPath& operator= (const Watch_Event_Container_Map_RelPath &) ; // not implemented 
	private:
		std::multimap<std::string, Watch_Event>& m_ ;
	} ;


	class Watch_Event_Container_Map_AbsPath : public Watch_Event_Container_BASE {   // PRQA S 2109
	public:
		Watch_Event_Container_Map_AbsPath(
                std::multimap<std::string, Watch_Event>& m, 
                const std::map <int, Entry_feat_by_userkey>& inot_entries 
        ) : 
            Watch_Event_Container_BASE(), 
            m_(m)      // PRQA S 2528
        {} 

		virtual ~Watch_Event_Container_Map_AbsPath() throw() {}                 // PRQA S 2131

		virtual void dump(const std::string& userkey) const {                     // PRQA S 2131
			ACS_LOG_DEBUG("Userkey: \"" << userkey << "\"") ;
			size_t j=0 ; 
            // PRQA S 4238 2
            for (std::multimap<std::string, Notify::Watch_Event>::const_iterator i=m_.begin(); 
                 i!=m_.end() ; 
                 ++i)
			{
				ACS_LOG_DEBUG("Event #" << (++j) << "  Key: \"" << i->first << "\"  :"); 
				(i->second).dump() ; 
				ACS_LOG_DEBUG(std::string(100,'-'));
			}
		}

        virtual void insert(const std::string& userkey, const Watch_Event& we)     // PRQA S 2131
        { 
            m_.insert( std::make_pair(we.src_absolute_path, we)) ; 
        }

        virtual size_t size() const { return m_.size() ; }                        // PRQA S 2131 2
		virtual bool empty() const throw() { return m_.empty() ; }

    private:
        Watch_Event_Container_Map_AbsPath() ; // not implemented 
        Watch_Event_Container_Map_AbsPath(const Watch_Event_Container_Map_AbsPath &) ; // not implemented 
        Watch_Event_Container_Map_AbsPath& operator= (const Watch_Event_Container_Map_AbsPath &) ; // not implemented 
	private:
		std::multimap<std::string, Watch_Event>& m_ ;  
	} ;
	
    
    
	class Watch_Event_Container_Map_WD : public Watch_Event_Container_BASE {  // PRQA S 2109
	public:
		Watch_Event_Container_Map_WD(
                std::multimap<int, Watch_Event>& m, 
                const std::map <int, Entry_feat_by_userkey>& inot_entries
        ) : 
            Watch_Event_Container_BASE(),
            m_(m)       // PRQA S 2528
        {}

		virtual ~Watch_Event_Container_Map_WD() throw() {}                      // PRQA S 2131

		virtual void dump(const std::string& userkey) const {                     // PRQA S 2131
			ACS_LOG_DEBUG("Userkey: \"" << userkey << "\"") ;
			size_t j=0 ; 
            // PRQA S 4238 2
            for (std::multimap<int, Notify::Watch_Event>::const_iterator i=m_.begin(); 
                 i!=m_.end() ; 
                 ++i)
			{
				ACS_LOG_DEBUG("Event #" << (++j) << "  Key: \"" << i->first << "\"  :") ; 
				(i->second).dump() ; 
				ACS_LOG_DEBUG(std::string(100,'-'));
			}
		}

		virtual void insert(const std::string& userkey, const Watch_Event& we)  // PRQA S 2131
        { 
            m_.insert( std::make_pair(we.src_wd, we)) ; 
        }

        virtual size_t size() const { return m_.size() ; }                      // PRQA S 2131 2
		virtual bool empty() const throw() { return m_.empty() ; }

    private:
        Watch_Event_Container_Map_WD() ; // not implemented 
        Watch_Event_Container_Map_WD(const Watch_Event_Container_Map_WD &) ; // not implemented 
        Watch_Event_Container_Map_WD& operator= (const Watch_Event_Container_Map_WD &) ; // not implemented 
	private:
		std::multimap<int, Watch_Event>& m_ ;  
	} ;
	
    
    
    
	class Watch_Event_Container_Map_Originator_WD : public Watch_Event_Container_BASE { // PRQA S 2109
	public:
		Watch_Event_Container_Map_Originator_WD(
                std::multimap<int, Watch_Event>& m, 
                const std::map <int, Entry_feat_by_userkey>& inot_entries
        ) : 
            Watch_Event_Container_BASE(),
			m_(m),                                                          // PRQA S 2528 2
            inot_entries_ref_(inot_entries) 
        {} 

        virtual ~Watch_Event_Container_Map_Originator_WD() throw() {}        // PRQA S 2131

		virtual void dump(const std::string& userkey) const {                  // PRQA S 2131
			ACS_LOG_DEBUG("Userkey: \"" << userkey << "\"") ;
			size_t j=0 ; 
            for (std::multimap<int, Notify::Watch_Event>::const_iterator i=m_.begin();  // PRQA S 4238
                 i!=m_.end() ; 
                 ++i)
			{
				ACS_LOG_DEBUG("Event #" << (++j) << "  Key: \"" << i->first << "\"  :") ; 
				(i->second).dump() ; 
				ACS_LOG_DEBUG(std::string(100,'-'));
			}
		}

		virtual void insert(const std::string& userkey, const Watch_Event& we)  // PRQA S 2131
        { 
			m_.insert( std::make_pair ( resolve_originator_wd(userkey, we.src_wd, inot_entries_ref_), we) ) ; 
		}

		virtual size_t size() const { return m_.size() ; }                    // PRQA S 2131 2
		virtual bool empty() const throw() { return m_.empty() ; }

    private:
        Watch_Event_Container_Map_Originator_WD() ; // not implemented 
        Watch_Event_Container_Map_Originator_WD(const Watch_Event_Container_Map_Originator_WD &) ; // not implemented 
        Watch_Event_Container_Map_Originator_WD& operator= (const Watch_Event_Container_Map_Originator_WD &) ; // not implemented 
	private:
		std::multimap<int, Watch_Event>& m_ ;  
		const std::map <int, Entry_feat_by_userkey>& inot_entries_ref_ ;
	} ;
    
    
	class Watch_Event_Container_Vector : public Watch_Event_Container_BASE { // PRQA S 2109
	public:
		Watch_Event_Container_Vector(
                std::vector<Watch_Event>& v, 
                const std::map <int, Entry_feat_by_userkey>& inot_entries
        ) : 
            Watch_Event_Container_BASE(),
            v_(v)       // PRQA S 2528
        {} 
		
        virtual ~Watch_Event_Container_Vector() throw() {}                     // PRQA S 2131

		virtual void dump(const std::string& userkey) const {                    // PRQA S 2131
			ACS_LOG_DEBUG("Userkey: \"" << userkey << "\"") ;
			size_t j=0 ; 
            // PRQA S 4238 2
            for (std::vector<Watch_Event>::const_iterator i=v_.begin(); 
                 i!=v_.end() ; 
                 ++i)
			{
				ACS_LOG_DEBUG("Event #" << (++j) << "  :") ; 
				i->dump() ; 
				ACS_LOG_DEBUG(std::string(100,'-'));
			}
		}
        
		virtual void insert(const std::string& userkey, const Watch_Event& we)   // PRQA S 2131
        { 
            v_.push_back(we) ; 
        }
        
		virtual size_t size() const { return v_.size() ; }                      // PRQA S 2131 2
		virtual bool empty() const throw() { return v_.empty() ; }
			
    private:
        Watch_Event_Container_Vector() ; // not implemented 
        Watch_Event_Container_Vector(const Watch_Event_Container_Vector &) ; // not implemented 
        Watch_Event_Container_Vector& operator= (const Watch_Event_Container_Vector &) ; // not implemented 
	private:
		std::vector<Watch_Event>& v_ ;  
	} ;


public:


	Notify() ;
	virtual ~Notify() throw() ;


	/*! 
 	 * \brief add a directory to the list of entries to watch
	 *
	 * \return TRUE if given entry was not already watched (so it's added to the list)
	 *         (Exceptions might be thrown, e.g. NoSuchFileOrDirectoryException )
	 *
	 * \sa watch_directory_rec
	 */
	bool watch_directory(const std::string& userkey, const std::string& abs_path, const std::string& rel_base_path, EventsMask m) ;


	/*! 
 	 * \brief add a directory to the list of entries to watch
	 *
	 * \return TRUE if given entry was not already watched (so it's added to the list). The wd is returned as reference.
	 *         (Exceptions might be thrown, e.g. NoSuchFileOrDirectoryException )
	 *
	 * \sa watch_directory_rec
	 */
	bool watch_directory(int& wd_out, const std::string& userkey, const std::string& abs_path, const std::string& rel_base_path, EventsMask m) ;

	
	/*! 
 	 * \brief add a directory to the list of entries to watch. This entry must be PROCESS-UNIQUE (it won't be added if already monitored by the process itself)
	 *
	 * \return TRUE if given entry was not already watched in the scope of the process (so it's added to the list). The wd is returned as reference.
	 *         (Exceptions might be thrown, e.g. NoSuchFileOrDirectoryException )
	 *
	 * \sa watch_directory_rec
	 */
	bool watch_directory_unique(int& wd_out, const std::string& userkey, const std::string& abs_path, const std::string& rel_base_path, EventsMask m) ;

	
	/*! 
 	 * \brief add a directory to the list of entries to watch. 
	 * All subdirectories (current and future) will be also automatically added
	 *
	 * \return TRUE if given entry was not already watched (so it's added to the list)
	 *         (Exceptions might be thrown, e.g. NoSuchFileOrDirectoryException )
	 *
	 * \sa watch_directory
	 */
	bool watch_directory_rec(const std::string& userkey, const std::string& abs_path, const std::string& rel_base_path, EventsMask m) ;


	/*! 
 	 * \brief add a directory to the list of entries to watch. 
	 * All subdirectories (current and future) will be also automatically added
	 *
	 * \return TRUE if given entry was not already watched (so it's added to the list). The wd is returned as reference.
	 *         (Exceptions might be thrown, e.g. NoSuchFileOrDirectoryException )
	 *
	 * \sa watch_directory
	 */
	bool watch_directory_rec(int& wd_out, const std::string& userkey, const std::string& abs_path, const std::string& rel_base_path, EventsMask m) ;


	/*! 
 	 * \brief add a directory to the list of entries to watch. This entry must be PROCESS-UNIQUE (it won't be added if already monitored by the process itself)
	 * All subdirectories (current and future) will be also automatically added
	 *
	 * \return TRUE if given entry was not already watched in the scope of the process (so it's added to the list). The wd is returned as reference.
	 *         (Exceptions might be thrown, e.g. NoSuchFileOrDirectoryException )
	 *
	 * \sa watch_directory
	 */
	bool watch_directory_rec_unique(int& wd_out, const std::string& userkey, const std::string& abs_path, const std::string& rel_base_path, EventsMask m) ;


	/*! 
 	 * \brief unregister a userkey. Meaning that from now on no more events will be notified for such userkey. Also, all the lists will be emptied for the userkey.
	 *
	 * \return TRUE if the userkey was successfully unregistered. FALSE if the userkey was not found.
	 *			Note that if emit_exception==true an exception will be thrown instead of returning FALSE
	 *
	 */
	bool unregister_userkey(const std::string& userkey, bool emit_exception=true) ;


	/*! 
 	 * \brief Return the occurred events as multimap: relative_path, Watch_Event
	 * if wait_msec is zero, no poll is actually performed: it only fills the list of queued events for the userkey, if any
	 *
	 * \return the multimap of occurred events
	 *
	 */
	size_t get_events_map_relpath(const std::string& userkey, std::multimap<std::string, Watch_Event>&, int wait_msec=10) ; // PRQA S 4400


	/*! 
 	 * \brief Return the occurred events as multimap: absolute_path, Watch_Event
	 * if wait_msec is zero, no poll is actually performed: it only fills the list of queued events for the userkey, if any
	 *
	 * \return the multimap of occurred events
	 *
	 */
	size_t get_events_map_abspath(const std::string& userkey, std::multimap<std::string, Watch_Event>&, int wait_msec=10) ; // PRQA S 4400


	/*! 
 	 * \brief Return the occurred events as multimap: source_wd, Watch_Event
	 * if wait_msec is zero, no poll is actually performed: it only fills the list of queued events for the userkey, if any
	 *
	 * \return the multimap of occurred events
	 *
	 */
	size_t get_events_map_wd(const std::string& userkey, std::multimap<int, Watch_Event>&, int wait_msec=10) ; // PRQA S 4400

	
	/*! 
 	 * \brief Return the occurred events as multimap: originator_source_wd, Watch_Event
	 * if wait_msec is zero, no poll is actually performed: it only fills the list of queued events for the userkey, if any
	 *
	 * \return the multimap of occurred events
	 *
	 */
	size_t get_events_map_originator_wd(const std::string& userkey, std::multimap<int, Watch_Event>&, int wait_msec=10) ; // PRQA S 4400


	/*! 
 	 * \brief Return the occurred events as vector
	 * if wait_msec is zero, no poll is actually performed: it only fills the list of queued events for the userkey, if any
	 *
	 * \return the vector of occurred events
	 *
	 */
	size_t get_events_vector(const std::string& userkey, std::vector <Watch_Event>&, int wait_msec=10) ; // PRQA S 4400


	/*! 
 	 * \brief Return the maximum number of events stored in stored_events_ map, for each user.
	 * If the threshold is exceeded, the stored_events_map will be shrinked to "lowlimit" entries
	 *
	 * \return max_stored_events_threshold_
	 *
	 */
	size_t get_max_stored_events_threshold() const { return max_stored_events_threshold_ ; }


	/*! 
 	 * \brief Return the lowlimit parameter, used to handle the size of stored_events_ map, for each user.
	 * If the threshold is exceeded, the stored_events_map will be shrinked to "lowlimit" entries
	 *
	 * \return max_stored_events_lowlimit_
	 *
	 */
	size_t get_max_stored_events_lowlimit() const { return max_stored_events_lowlimit_ ; }


	/*! 
 	 * \brief Check the size of stored_events_ map, for each user.
	 * If the threshold is exceeded, the stored_events_map will be shrinked to "lowlimit" entries
	 *
	 * \return max_stored_events_lowlimit_
	 *
	 */
	bool check_stored_events_trasholds() ;


	/*! 
 	 * \brief Set the maximum number of events stored in stored_events_ map and the lowlimit parameter, 
	 *			used to handle the size of stored_events_ map, for each user.
	 * If the threshold is exceeded, the stored_events_map will be shrinked to "lowlimit" entries
	 *
	 * \return max_stored_events_lowlimit_
	 *
	 */
	void set_max_stored_events_limits(const size_t& th, const size_t& ll) ;
	

	/*! 
 	 * \brief create a unique userkey. The key identifies the thread_id and an optional string label
	 */
	static std::string generate_unique_userkey(const std::string &label="") ;
	static std::string generate_unique_userkey(const pthread_t& tid, const std::string &label="") ;

	/*! 
 	 * \brief create a string with the list of all set Events in a given EventsMask
	 * \return a string with the list of all set Events in a given EventsMask
	 */
	static std::string eventmask_to_string(EventsMask m) ;

	/*! 
 	 * \brief create a string from integer, to be used in debug
	 */
	static std::string debug_to_string(int i) { std::ostringstream o ; o << i ; return o.str() ; }

	/*! 
 	 * \brief Task executed using just ONE "stat"
	 */
	static bool check_entry_exists_and_is_dir(const std::string& entry) ;

	/*! 
 	 * \brief Used in debug
	 */
	static void dump_entry_features(const Entry_features& e) ;

	/*! 
 	 * \brief Used in debug
	 */
	static void dump_entry_feat_by_userkey(const Entry_feat_by_userkey& e) ;

	/*! 
 	 * \brief obtain the key-time from current time
	 */
	static uint64_t get_key_from_curtime() ;
	
	/*! 
 	 * \brief create a string from the key-time, to be used in debug
	 */
	static std::string debug_time_to_string(const uint64_t& key_time) 
	{ 
		std::ostringstream o ; o << key_time ;
		const std::string s = o.str() ;
		return ( (s.length() >= 6)? (s.substr(0, s.length() - 6) + "." + s.substr(s.length() - 6)) : s ) ;  // PRQA S 3084, 3382, 4400
	}
	
	/*! 
 	 * \brief resolve the originator wd given a source wd, the specific userkey and the inot_entries map
	 */
	static int resolve_originator_wd(const std::string& userkey, 
                                    const int& wd_in, 
                                    const std::map <int, Entry_feat_by_userkey>& inot_entries)	;

private:	
	// declared but not used 
	Notify(const Notify &) ;
	Notify &operator=(const Notify &) ;

	bool add_entry(bool proc_unique, 
                   const std::string& userkey, 
                   const std::string& full_path, 
                   const std::string& rel_path, 
                   EventsMask m, 
                   bool rec, 
                   const int& wd_orig_in, 
                   int& wd_out) ;
                   
	bool add_directory_tree(bool unique, 
                            const std::string& userkey, 
                            const std::string& abs_path, 
                            const std::string& rel_base_path_in,
                            EventsMask m, 
                            const int& wd_orig_in, 
                            int& wd_out) ;

	void automatically_add_new_entries(bool proc_unique, 
                                       const std::string& userkey, 
                                       const struct inotify_event *pevent) ;

	int resolve_originator_wd (	const std::string& userkey, 
                                const int& wd_in, 
                                std::string& abs_base_path, 
                                std::string& rel_base_path, 
								uint32_t& mask_out, 
                                bool& recursive, 
                                bool emit_exception=true) const ;

	std::vector <std::string> get_userkeys_from_wd(const int& wd) const ;

	std::pair <bool, bool> archive_retrieve_incomplete_events(const struct inotify_event *pevent, Extended_inotify_event& matched_event) ;

	void merge_matched_events(const std::string& userkey, const Extended_inotify_event& matched_event_in, Watch_Event& ev) const ;

	void remove_merged_event(const Extended_inotify_event& matched_event_in) ;

	bool flush_mismatched_cookies() ;
	
	size_t add_events_to_container(const std::string &userkey, Watch_Event_Container_BASE* container, int wait_msec)  ;

	void dump_entry_map() const ;
	
	void dump_archived_entries_maps() const ;

	/*! 
 	 * \brief remove entry from map: inot_arch_events_by_date_
	 * \return TRUE if key was found.
	 */
	bool remove_from_inot_arch_events_by_date(const uint64_t& key, const uint32_t& value) ;
	
	/*! 
 	 * \brief remove entry from inot_entries
	 * \return TRUE if key was found.
	 */
	bool remove_from_inot_entries(const int& wd_in) ;
	bool remove_from_inot_entries_by_user_FIXME(const std::string& userkey, const int& wd_in) ;	/* FIXME NOT USEFUL */

private:

	int						inot_instance_ ;		/** inotify instance */
	unsigned char*			inot_buffer_ ;			/** pre-allocated memory buffer to read from inot_instance_ */
	static Mutex 			inot_entries_mutex_ ;		/** Mutex to access inot_entries_ map */

	std::map <int, Entry_feat_by_userkey>	inot_entries_ ;	/** Map to store the features of each watched entry. KEY: wd */
		
	std::map <std::string, std::deque <Watch_Event> >	stored_events_ ;	/** Map to store the the low-level events 
																				(one event is splitted for each possible userkey)  KEY: userkey */
	
	
	/* FIXME what happens if a WD is unregistered AND just one cookie was received ???? */
	
	std::map <uint32_t, Extended_inotify_event>	inot_arch_events_by_cookie_ ;	/** Map to store the archived events. KEY: inot. cookie */
	std::multimap <uint64_t, uint32_t>	inot_arch_events_by_date_ ;		/** Map to store the archived events. KEY: date of insertion */

	size_t max_stored_events_threshold_ ;
    size_t max_stored_events_lowlimit_ ;
	
private:

	static const size_t inot_max_events_ ;
    static const size_t buffer_size_ ;
    static const size_t filesystem_delay_usec ;
	static const size_t max_stored_events_threshold_default_ = 64*1024 ;
    static const size_t max_stored_events_lowlimit_default_ =1024 ;



private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(Notify) ;

} ;



class NotifySingleton : public Singleton<Notify> // PRQA S 2153
{
public:
    virtual ~NotifySingleton() throw() {} // PRQA S 2131
private:
    NotifySingleton() ;
    NotifySingleton(const NotifySingleton &) ;
    NotifySingleton& operator= (const NotifySingleton &) ;
};

		   
/*! @}*/

_ACS_END_NESTED_NAMESPACE

#endif // INOTIFY_NOT_SUPPORTED

#endif	// _Os_Notify_H_

