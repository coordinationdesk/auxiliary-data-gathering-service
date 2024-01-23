/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

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


#include <sys/inotify.h>    

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstring>
#include <vector>
#include <deque>
#include <map>


_ACS_BEGIN_NESTED_NAMESPACE(acs, os)


/*! \addtogroup os 
 *  @{
 */
/*! \class Notify
	
	Utility class to manage changes on filesystem using inotify.
*/
class Notify                                                                 
{
	
public:

	/* Generic exceptions: (derived directly from exException) */
	exDECLARE_EXCEPTION(NotifyException, exException) ;                     
	exDECLARE_EXCEPTION(NotifyCriticalException, exCriticalException) ;     
	exDECLARE_EXCEPTION(NoSuchFileOrDirectoryException, NotifyException) ;  
	exDECLARE_EXCEPTION(NotADirectoryException, NotifyException) ;          

	/** Mask for all possible notifying events */
	
	enum EventsMask { // NOSONAR - old enum
			
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

	struct Entry_features
	{
		int 		originator_wd = -1 ; // Originator watch descriptor
		uint32_t 	mask = 0 ;			 // Mask
		bool 		rec = false ;        // Recursive (this one & future) subdirectories
		std::string full_path = "" ; 	 // Absolute, full-path
		std::string rel_path = "" ;      // Relative path (usually relative to a user's root)
	} ;


	/** The structure below extends the basic inotify_event in order to contain ALSO the optional filename payload.
		This allows copying into memory the base structure. See the documentation for inotify_event */
	
// the following pragma has been added to avoid - warning: ... a non-virtual destructor
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"       
#pragma GCC diagnostic ignored "-Weffc++"


	struct Extended_inotify_event     
	{
		Extended_inotify_event() 
        {
	        ionot_event.wd=-1;
            ionot_event.len=0; 
        }
        
		explicit Extended_inotify_event(const inotify_event& p) : 
            ionot_event(p)
		{
			strncpy(stored_name.data(), p.name, p.len) ; // NOSONAR - size is ok, copy only those with meaning
		    if (p.len < stored_name_size) {
                stored_name[p.len] = 0 ; // null terminate
            }
        }

		Extended_inotify_event(const Extended_inotify_event& p) : 
            last_update(p.last_update),
            ionot_event(p.ionot_event)
		{
			strncpy(stored_name.data(), p.stored_name.data(), p.ionot_event.len) ; // NOSONAR - size is ok, copy only those with meaning
		    if (p.ionot_event.len < stored_name_size) {
                stored_name[p.ionot_event.len] = 0 ; // null terminate
            }
        }
        
        Extended_inotify_event& operator= (const Extended_inotify_event &p) 
        {
            if (this != &p) {
				ionot_event = p.ionot_event;
 			    strncpy(stored_name.data(), p.stored_name.data(), p.ionot_event.len) ; // NOSONAR - size is ok, copy only those with meaning
                last_update = p.last_update ;
		        if (p.ionot_event.len < stored_name_size) {
                    stored_name[p.ionot_event.len] = 0 ; // null terminate
                }
            } 
            return *this ;
        }

		~Extended_inotify_event() = default ;

        static const unsigned int stored_name_size = 4096 ;
        
		std::array<char, stored_name_size > stored_name ; 
		uint64_t last_update = get_key_from_curtime() ; 
		struct inotify_event ionot_event = {} ;
	} ;
	
// reactivate -Wnon-virtual-dtor
#pragma GCC diagnostic pop


	/** Map between the userkey and its monitored entries (the same entry might be monitored with different userkeys) */

	using Entry_feat_by_userkey = std::map<std::string, Entry_features, std::less<> > ;	/* key of the map: user_key */

public:

	/** A containter of following structures will be returned as list of events */
	
	class Watch_Event 
	{	
	public:
		Watch_Event () = default ;
        ~Watch_Event() = default ;

        Watch_Event(const Watch_Event &rhs) = default ;
		Watch_Event &operator=(const Watch_Event &rhs) = default ;

		void build(const std::string& userkey, const struct inotify_event *pevent, const std::map <int, Notify::Entry_feat_by_userkey>& entry_map) ;
		void build(const std::string& userkey, const struct Extended_inotify_event &ext_inot_event , const std::map <int, Notify::Entry_feat_by_userkey>& entry_map) ;
		void dump() const ;	

		static Watch_Event merge(const Watch_Event& src, const Watch_Event& dst) ; 
        
		std::string src_absolute_path = "" ;
		std::string src_relative_path = "" ;
		std::string dst_absolute_path = "" ; 
		std::string dst_relative_path = "" ;
		EventsMask event = {} ;
		bool dst_is_set = false ;
		int src_wd = 0 ; 
		uint32_t cookie = 0  ; 
		uint64_t last_update = 0 ; 
		ACS_CLASS_DECLARE_DEBUG_LEVEL(Watch_Event) ;
        
	} ;


private:

	/** A sorted containter used to return events to the user (base class) */

	class Watch_Event_Container_BASE {
	public:
		Watch_Event_Container_BASE() = default ;
 		virtual ~Watch_Event_Container_BASE() = default ;   
		virtual void dump(const std::string&) const = 0 ;
		virtual void insert(const std::string& userkey, const Watch_Event&) = 0 ;
		virtual size_t size() const = 0 ;
		virtual bool empty() const noexcept = 0 ;
    protected:
        Watch_Event_Container_BASE(const Watch_Event_Container_BASE &) = default ;
        Watch_Event_Container_BASE &operator= (const Watch_Event_Container_BASE &) = default ;
        
	} ;

	/** Sorted containters used to return events to the user (derived classes) */

	class Watch_Event_Container_Map_RelPath : public Watch_Event_Container_BASE {   
	public:
        Watch_Event_Container_Map_RelPath(const Watch_Event_Container_Map_RelPath &) = delete ;
        Watch_Event_Container_Map_RelPath& operator= (const Watch_Event_Container_Map_RelPath &) = delete ;

		Watch_Event_Container_Map_RelPath(
                std::multimap<std::string, Watch_Event>& m,
                const std::map <int, Entry_feat_by_userkey>& ) : 
              Watch_Event_Container_BASE(),
              m_(m)     
        {}

		~Watch_Event_Container_Map_RelPath() override = default ;                     

		void dump(const std::string& userkey) const override 
        {                         
			ACS_LOG_DEBUG("Userkey: \"" << userkey << "\"") ;
			size_t j=0 ; 
            
            for (auto i=m_.begin(); i!=m_.end() ; ++i)
			{
				ACS_LOG_DEBUG("Event #" << (++j) << "  Key: \"" << i->first << "\"  :"); 
				(i->second).dump() ; 
				ACS_LOG_DEBUG(std::string(100,'-'));
			}
		}
        
		void insert(const std::string& userkey, const Watch_Event& we) override
        { 
            m_.emplace(we.src_relative_path, we) ; 
        }
		
        size_t size() const override { return m_.size() ; }                          
		bool empty() const noexcept override { return m_.empty() ; }
	private:
		std::multimap<std::string, Watch_Event>& m_ ;
	} ;


	class Watch_Event_Container_Map_AbsPath : public Watch_Event_Container_BASE {   
	public:
		Watch_Event_Container_Map_AbsPath(
                std::multimap<std::string, Watch_Event>& m, 
                const std::map <int, Entry_feat_by_userkey>& ) : 
            Watch_Event_Container_BASE(), 
            m_(m)      
        {} 

        Watch_Event_Container_Map_AbsPath(const Watch_Event_Container_Map_AbsPath &) = delete ;
        Watch_Event_Container_Map_AbsPath& operator= (const Watch_Event_Container_Map_AbsPath &) = delete ;
		~Watch_Event_Container_Map_AbsPath() override = default ;                

		void dump(const std::string& userkey) const override 
        {                     
			ACS_LOG_DEBUG("Userkey: \"" << userkey << "\"") ;
			size_t j=0 ; 
            
            for (auto i=m_.begin(); i!=m_.end() ; ++i)
			{
				ACS_LOG_DEBUG("Event #" << (++j) << "  Key: \"" << i->first << "\"  :"); 
				(i->second).dump() ; 
				ACS_LOG_DEBUG(std::string(100,'-'));
			}
		}

        void insert(const std::string& userkey, const Watch_Event& we) override
        { 
            m_.emplace(we.src_absolute_path, we) ; 
        }

        size_t size() const override { return m_.size() ; }                        
		bool empty() const noexcept override { return m_.empty() ; }

	private:
		std::multimap<std::string, Watch_Event>& m_ ;  
	} ;
	
    
    
	class Watch_Event_Container_Map_WD : public Watch_Event_Container_BASE {  
	public:
		Watch_Event_Container_Map_WD(
                std::multimap<int, Watch_Event>& m, 
                const std::map <int, Entry_feat_by_userkey>& ) : 
            Watch_Event_Container_BASE(),
            m_(m)       
        {}

        Watch_Event_Container_Map_WD(const Watch_Event_Container_Map_WD &) = delete ;
        Watch_Event_Container_Map_WD& operator= (const Watch_Event_Container_Map_WD &) = delete ;
		~Watch_Event_Container_Map_WD() override = default ;                      

		void dump(const std::string& userkey) const override 
        {                     
			ACS_LOG_DEBUG("Userkey: \"" << userkey << "\"") ;
			size_t j=0 ; 
            
            for (auto i=m_.begin(); i!=m_.end() ; ++i)
			{
				ACS_LOG_DEBUG("Event #" << (++j) << "  Key: \"" << i->first << "\"  :") ; 
				(i->second).dump() ; 
				ACS_LOG_DEBUG(std::string(100,'-'));
			}
		}

		void insert(const std::string& userkey, const Watch_Event& we) override 
        { 
            m_.emplace(we.src_wd, we) ; 
        }

        size_t size() const override { return m_.size() ; }                      
		bool empty() const noexcept override { return m_.empty() ; }

	private:
		std::multimap<int, Watch_Event>& m_ ;  
	} ;
	
    
    
    
	class Watch_Event_Container_Map_Originator_WD : public Watch_Event_Container_BASE { 
	public:
		Watch_Event_Container_Map_Originator_WD(
                std::multimap<int, Watch_Event>& m, 
                const std::map <int, Entry_feat_by_userkey>& inot_entries
        ) : 
            Watch_Event_Container_BASE(),
			m_(m),                                                          
            inot_entries_ref_(inot_entries) 
        {} 

        ~Watch_Event_Container_Map_Originator_WD() override = default ;      

        Watch_Event_Container_Map_Originator_WD(const Watch_Event_Container_Map_Originator_WD &) = delete ;
        Watch_Event_Container_Map_Originator_WD& operator= (const Watch_Event_Container_Map_Originator_WD &) = delete ;

		void dump(const std::string& userkey) const override 
        {                  
			ACS_LOG_DEBUG("Userkey: \"" << userkey << "\"") ;
			size_t j=0 ; 
            for (auto i=m_.begin(); i!=m_.end() ; ++i)
			{
				ACS_LOG_DEBUG("Event #" << (++j) << "  Key: \"" << i->first << "\"  :") ; 
				(i->second).dump() ; 
				ACS_LOG_DEBUG(std::string(100,'-'));
			}
		}

		void insert(const std::string& userkey, const Watch_Event& we) override
        { 
			m_.emplace(resolve_originator_wd(userkey, we.src_wd, inot_entries_ref_), we ) ; 
		}

		size_t size() const override { return m_.size() ; }                    
		bool empty() const noexcept override { return m_.empty() ; }

	private:
		std::multimap<int, Watch_Event>& m_ ;  
		const std::map <int, Entry_feat_by_userkey>& inot_entries_ref_ ;
	} ;
    
    
	class Watch_Event_Container_Vector : public Watch_Event_Container_BASE { 
	public:
		Watch_Event_Container_Vector(
                std::vector<Watch_Event>& v, 
                const std::map <int, Entry_feat_by_userkey>& ) : 
            Watch_Event_Container_BASE(),
            v_(v)       
        {} 
		
        ~Watch_Event_Container_Vector() override = default ;                   

        Watch_Event_Container_Vector(const Watch_Event_Container_Vector &) = delete ; 
        Watch_Event_Container_Vector& operator= (const Watch_Event_Container_Vector &) = delete ; 
		void dump(const std::string& userkey) const override
        {                    
			ACS_LOG_DEBUG("Userkey: \"" << userkey << "\"") ;
			size_t j=0 ; 
            
            for (auto const & i : v_ )
			{
				ACS_LOG_DEBUG("Event #" << (++j) << "  :") ; 
				i.dump() ; 
				ACS_LOG_DEBUG(std::string(100,'-'));
			}
		}
        
		void insert(const std::string& userkey, const Watch_Event& we) override
        { 
            v_.push_back(we) ; 
        }
        
		size_t size() const override { return v_.size() ; }                      
		bool empty() const noexcept override { return v_.empty() ; }
			
	private:
		std::vector<Watch_Event>& v_ ;  
	} ;


public:


	Notify() ;
	virtual ~Notify() noexcept ;


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
	size_t get_events_map_relpath(const std::string& userkey, std::multimap<std::string, Watch_Event>&, int wait_msec=10) ; 


	/*! 
 	 * \brief Return the occurred events as multimap: absolute_path, Watch_Event
	 * if wait_msec is zero, no poll is actually performed: it only fills the list of queued events for the userkey, if any
	 *
	 * \return the multimap of occurred events
	 *
	 */
	size_t get_events_map_abspath(const std::string& userkey, std::multimap<std::string, Watch_Event>&, int wait_msec=10) ; 


	/*! 
 	 * \brief Return the occurred events as multimap: source_wd, Watch_Event
	 * if wait_msec is zero, no poll is actually performed: it only fills the list of queued events for the userkey, if any
	 *
	 * \return the multimap of occurred events
	 *
	 */
	size_t get_events_map_wd(const std::string& userkey, std::multimap<int, Watch_Event>&, int wait_msec=10) ; 

	
	/*! 
 	 * \brief Return the occurred events as multimap: originator_source_wd, Watch_Event
	 * if wait_msec is zero, no poll is actually performed: it only fills the list of queued events for the userkey, if any
	 *
	 * \return the multimap of occurred events
	 *
	 */
	size_t get_events_map_originator_wd(const std::string& userkey, std::multimap<int, Watch_Event>&, int wait_msec=10) ; 


	/*! 
 	 * \brief Return the occurred events as vector
	 * if wait_msec is zero, no poll is actually performed: it only fills the list of queued events for the userkey, if any
	 *
	 * \return the vector of occurred events
	 *
	 */
	size_t get_events_vector(const std::string& userkey, std::vector <Watch_Event>&, int wait_msec=10) ; 


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
		return ( (s.length() >= 6)? (s.substr(0, s.length() - 6) + "." + s.substr(s.length() - 6)) : s ) ;  
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

private: // NOSONAR - for clarity 
	static const size_t inot_max_events_ ;
    static const size_t buffer_size_ ;
    static const size_t filesystem_delay_usec ;
	static const size_t max_stored_events_threshold_default_ = 64*1024 ;
    static const size_t max_stored_events_lowlimit_default_ =1024 ;


	int             inot_instance_ = -1 ;		/** inotify instance */
	unsigned char*  inot_buffer_ = nullptr ;			/** pre-allocated memory buffer to read from inot_instance_ */
	static Mutex     inot_entries_mutex_ ;		/** Mutex to access inot_entries_ map */

	std::map <int, Entry_feat_by_userkey> inot_entries_ = {} ;	/** Map to store the features of each watched entry. KEY: wd */
		
	std::map <std::string, std::deque <Watch_Event>, std::less<> > stored_events_ = {} ;	/** Map to store the the low-level events 
																				(one event is splitted for each possible userkey)  KEY: userkey */
	
	
	/* FIXME what happens if a WD is unregistered AND just one cookie was received ???? */
	
	std::map <uint32_t, Extended_inotify_event>	inot_arch_events_by_cookie_ = {} ;	/** Map to store the archived events. KEY: inot. cookie */
	std::multimap <uint64_t, uint32_t>	inot_arch_events_by_date_ = {} ;		/** Map to store the archived events. KEY: date of insertion */

	size_t max_stored_events_threshold_ = max_stored_events_threshold_default_ ;
    size_t max_stored_events_lowlimit_ =  max_stored_events_lowlimit_default_ ;
	
// private data members 
	ACS_CLASS_DECLARE_DEBUG_LEVEL(Notify) ;

} ;



class NotifySingleton : public pattern::Singleton<Notify> 
{
public:
    ~NotifySingleton() override = default ; 
private:
    NotifySingleton() = default ;
};

		   
/*! @}*/

_ACS_END_NESTED_NAMESPACE

#endif // INOTIFY_NOT_SUPPORTED

#endif	// _Os_Notify_H_

