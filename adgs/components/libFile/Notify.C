// PRQA S 1050 EOF
/*

	Copyright (C) 2011-2015, Advanced Computer Systems , S.p.A.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. File Notify Library $

	$Id$

	$Author$

	$Log$
	Revision 5.8  2017/11/15 11:39:58  chicas
	some message to PRIVINFO
	
	Revision 5.7  2016/09/02 13:23:24  marfav
	S1PDGS-31793
	No messages are issued if a starting notifier is requested to stop
	before completing the loop on the paths to monitor
	Flushing the pending events when stopping a DiskEventNotifier thread
	
	Revision 5.6  2015/05/25 14:55:36  marpas
	coding best practices applied
	
	Revision 5.5  2015/01/20 14:10:17  marfav
	close syscall exit code checked
	
	Revision 5.4  2014/02/12 12:35:27  marpas
	define INOTIFY_NOT_SUPPORTED for systems where inotify is not supported
	
	Revision 5.3  2014/02/07 18:42:30  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.2  2013/07/24 17:43:12  marpas
	messages fixed
	
	Revision 5.1  2013/06/21 10:31:40  marpas
	adoption of libException 5.x statndards in progress, qa rules adn compilation warning fixed
	
	Revision 5.0  2013/06/20 16:43:53  marpas
	adoption of libException 5.x in progress
	
	Revision 1.11  2012/11/30 14:09:44  marpas
	qa rules and optimization
	
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
	
	Revision 1.5  2012/10/16 15:42:25  lucio.pulvirenti
	EC:: improved and tested in multi-thread environment
	Added public method: get_events_map_originator_wd
	
	Revision 1.4  2012/10/12 15:05:03  enrcar
	EC:: Added method generate_unique_userkey
	
	Revision 1.3  2012/10/12 13:18:25  enrcar
	EC:: Several modifications. The userkey (which also works as threadid) has been added
	
	Revision 1.2  2012/10/08 09:28:42  lucio.pulvirenti
	get_key_from_curtime: removed dependency from libDateTime: used timeval struct filled by gettimeofday function.
	
	Revision 1.1  2012/10/04 13:30:05  lucio.pulvirenti
	Imported from PDS2 environment.
	
*/


#ifndef INOTIFY_NOT_SUPPORTED // PRQA S 1001


#include <Notify.h>
#include <File.h>
#include <Dir.h>
#include <RegExp.h>
#include <StringUtils.h>
#include <Filterables.h>

#include <algorithm> 
#include <cctype> 
#include <cstring> 
#include <ctime>
#include <sstream> 

#include <fcntl.h>
#include <poll.h>
#include <dirent.h>
#include <unistd.h>
#include <utime.h>
#include <sys/file.h> // PRQA S 1013 4
#include <sys/stat.h>
#include <sys/vfs.h>
#include <sys/time.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace acs::os ;
using namespace std ;


//****************************************************************************************************************************
//
//  CLASS Notify::Watch_Event
//
//****************************************************************************************************************************
ACS_CLASS_DEFINE_DEBUG_LEVEL(Notify::Watch_Event) ;

//
//		Notify::Watch_Event::build
//
//////////////////////////////////////////////////////////////////////////////////////////
void
		Notify::Watch_Event::build(const string& userkey, const struct inotify_event *pevent, const std::map <int, Notify::Entry_feat_by_userkey>& entry_map)
{
	
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;
	
	ACS_COND_THROW( !pevent, NotifyCriticalException("Null pointer received as input") ) ; // PRQA S 3081

	const Extended_inotify_event ext_inot_event(*pevent) ;	/* Also sets last_update to the current time */
	
	build (userkey, ext_inot_event, entry_map) ;
}


//
//		Notify::Watch_Event::build
//
//////////////////////////////////////////////////////////////////////////////////////////
void
		Notify::Watch_Event::build(const string& userkey, const Extended_inotify_event& ext_inot_event, const std::map <int, Notify::Entry_feat_by_userkey>& entry_map)
{
	ThreadSafe::Lock lock(inot_entries_mutex_) ;
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;
	
	const int wd = ext_inot_event.ionot_event.wd ;

	map <int, Notify::Entry_feat_by_userkey>::const_iterator i = entry_map.find(wd) ;
	if ( i == entry_map.end() ) {

		// wd not found in map
		ostringstream o ; o << "descriptor not found in map: " << wd ;
		ACS_THROW( NotifyCriticalException(o.str()) ) ; // PRQA S 3081

	}	// if ( i == entry_map.end() ) 
	
	// IF HERE, i->second is the map of Entry_feat_by_userkey
	
	Entry_feat_by_userkey::const_iterator j ( (i->second).find(userkey) ) ;
	
	if (j == (i->second).end()) {
		
		// userkey not found in map
		ostringstream o ; o << "descriptor with userkey: \"" << userkey << "\" not found in map" ;
		ACS_THROW( NotifyCriticalException(o.str()) ) ; // PRQA S 3081

	}

	const string entry_name = (ext_inot_event.ionot_event.len>0)? string(ext_inot_event.stored_name): string("") ;  // PRQA S 3081, 3384, 3385
					
	event = Notify::EventsMask(ext_inot_event.ionot_event.mask) ; // PRQA S 3013, 3081
	src_absolute_path = StringUtils::pathJoin( (j->second).full_path, entry_name) ; 
	src_relative_path = StringUtils::pathJoin ( (j->second).rel_path,  entry_name) ; 
	dst_is_set = false ; 
	src_wd = ext_inot_event.ionot_event.wd ;
	cookie = ext_inot_event.ionot_event.cookie ; 
	last_update = ext_inot_event.last_update ;	/* Copy last_update from the stored event to the watch event */

	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;
    
	return ; 
}
	
	
//
//		Notify::Watch_Event::merge
//
//////////////////////////////////////////////////////////////////////////////////////////
Notify::Watch_Event
		Notify::Watch_Event::merge(const Notify::Watch_Event& src, const Notify::Watch_Event& dst)
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;

	Notify::Watch_Event out ; 
	
	out.src_absolute_path = src.src_absolute_path ; out.src_relative_path = src.src_relative_path ; 
	out.dst_absolute_path = dst.src_absolute_path ; out.dst_relative_path = dst.src_relative_path ; 
	out.event = static_cast<EventsMask> (int(src.event) | int(dst.event)) ;  // PRQA S 3003, 3013, 3081
	out.dst_is_set = true ;
	out.src_wd = src.src_wd ;
	out.cookie = dst.cookie ;
	out.last_update = get_key_from_curtime() ;	
	
	return out ; 
}
			

//
//		Notify::Watch_Event::dump
//
//////////////////////////////////////////////////////////////////////////////////////////
void
		Notify::Watch_Event::dump() const
{
	ostringstream o ; 
	o << "\tsrc_absolute_path: \"" << src_absolute_path << "\"\n\tsrc_relative_path: \"" << src_relative_path<< "\"\n" ; 
	if (dst_is_set) {
        o << "\tdst_absolute_path: \"" << dst_absolute_path << "\n\tdst_relative_path: \"" << dst_relative_path << "\"\n" ;
    }
	o << "\tevent: 0x" << hex << static_cast<uint32_t>(event) << dec << "  (" << eventmask_to_string(event) << ")"  // PRQA S 3081
		<< "\tsrc_wd: " << src_wd << "\tcookie: " << cookie << "\tlast_update: " << debug_time_to_string(last_update) ;  
	
	ACS_LOG_DEBUG(SimpleDebugSignature << o.str()) ;
 

	return ; 
}


//****************************************************************************************************************************
//
//  CLASS Notify :
//
//****************************************************************************************************************************
ACS_CLASS_DEFINE_DEBUG_LEVEL(Notify) ;

Mutex Notify::inot_entries_mutex_ ;

const size_t Notify::inot_max_events_ = 1024 ; 
const size_t Notify::buffer_size_ = Notify::inot_max_events_ * (sizeof(inotify_event) + PATH_MAX ) ; // PRQA S 3084
const size_t Notify::filesystem_delay_usec = 2000000 ; // 2 sec  [unit: microsec = 1e-6 sec]
const size_t Notify::max_stored_events_threshold_default_, Notify::max_stored_events_lowlimit_default_ ;
		

//
//		Notify
//
//////////////////////////////////////////////////////////////////////////////////////////
Notify::Notify() :
	inot_instance_(-1), inot_buffer_(0), inot_entries_(), stored_events_(), inot_arch_events_by_cookie_(), inot_arch_events_by_date_(), \
		max_stored_events_threshold_(max_stored_events_threshold_default_), max_stored_events_lowlimit_(max_stored_events_lowlimit_default_)
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;
	
			
	/* Following parameters are set using the proper method in order to raise exception in case of misconfiguration */
	set_max_stored_events_limits(max_stored_events_threshold_default_, max_stored_events_lowlimit_default_) ;

			
	if ( (inot_instance_ = inotify_init()) < 0 ) {
		int err = errno;
		ostringstream o ; o << "inot: " << inot_instance_ << "  Unable to initialize inotify" ;
		ACS_THROW(NotifyException(o.str(), err)); // PRQA S 3081
	}
	else {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "inotify instance obtained with fd: " << inot_instance_) ;
	}
	
	
	if ( (inot_buffer_ = reinterpret_cast<unsigned char*>(malloc( buffer_size_))) == 0 ) { // PRQA S 3081
		int err = errno;
		ostringstream o ; o << "inot: " << inot_instance_ << "  Unable to allocate a buffer of " << buffer_size_ << " bytes" ;
		ACS_THROW(NotifyException(o.str(), err)); // PRQA S 3081
	}
	else {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Buffer successfully allocated (" << buffer_size_ << " at address: " <<  inot_buffer_ << ")") ; 
	}

}


//
//		~Notify
//
//////////////////////////////////////////////////////////////////////////////////////////
Notify::~Notify() throw()
{
    // PRQA S 4631 L1
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;

	if (inot_instance_ >= 0)
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "instance closed: " << inot_instance_ ) ;
				
		if (0 != close (inot_instance_) ) ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::close syscall failed", errno));
		inot_instance_ = -1 ;
	}
    // PRQA L:L1
}


//
//		Notify:watch_directory (simplified call)
//
//////////////////////////////////////////////////////////////////////////////////////////
bool Notify::watch_directory(const string& userkey, const string& abs_path, const string& rel_base_path, EventsMask m)
{
//	lock on inot_mutex_ not needed ;

	int discard = 0 ; 
	return watch_directory(discard, userkey, abs_path, rel_base_path, m) ;
}


//
//		Notify:watch_directory
//
//////////////////////////////////////////////////////////////////////////////////////////
bool Notify::watch_directory(int& wd_out, const string& userkey, const string& abs_path, const string& rel_base_path, EventsMask m)
{
//	lock on inot_mutex_ not needed ;

	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;
			
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "userkey: \"" << userkey << "\"  abs_path: \"" << abs_path << "\"  rel_base_path: \"" << rel_base_path << "  mask: " // PRQA S 3081
			<< int(m) << " ( " << eventmask_to_string(m) << " )") ;

	if (! File::exists(abs_path) ) {
		ACS_THROW(NoSuchFileOrDirectoryException("Fatal Error: Directory not found: \"" + abs_path + "\"" )) ; // PRQA S 3081
    }
	if (! File::isDir(abs_path) ) {
		ACS_THROW(NotADirectoryException("Fatal Error: Not a directory: \"" + abs_path + "\"" )) ; // PRQA S 3081
    }
	
	const bool rec = false ; 
	const bool unique = false ; 	/* The entry is NOT process unique (so several thread might watch the same path) */
	const int originator_wd = -1 ;
	return add_entry(unique, userkey, abs_path, rel_base_path, m, rec, originator_wd, wd_out) ;
}


//
//		Notify:watch_directory_unique
//
//////////////////////////////////////////////////////////////////////////////////////////
bool Notify::watch_directory_unique(int& wd_out, const string& userkey, const string& abs_path, const string& rel_base_path, EventsMask m)
{
//	lock on inot_mutex_ not needed ;

	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;
			
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "userkey: \"" << userkey << "\"  abs_path: \"" << abs_path << "\"  rel_base_path: \"" << rel_base_path << "  mask: "  // PRQA S 3081
			<< int(m) << " ( " << eventmask_to_string(m) << " )") ;

	if (! File::exists(abs_path) ) {
		ACS_THROW(NoSuchFileOrDirectoryException("Fatal Error: Directory not found: \"" + abs_path + "\"" )) ; // PRQA S 3081
    }
	if (! File::isDir(abs_path) ) {
		ACS_THROW(NotADirectoryException("Fatal Error: Not a directory: \"" + abs_path + "\"" )) ; // PRQA S 3081
    }
	
	const bool rec = false ; 
	const bool unique = true ;		/* This means that any entry is PROCESS UNIQUE */
	const int originator_wd = -1 ;
	return add_entry(unique, userkey, abs_path, rel_base_path, m, rec, originator_wd, wd_out) ;
}


//
//		Notify:watch_directory_rec (simplified call)
//
//////////////////////////////////////////////////////////////////////////////////////////
bool Notify::watch_directory_rec(const string& userkey, const string& abs_path, const string& rel_base_path, EventsMask m)
{
//	lock on inot_mutex_ not needed ;

	int discard = 0 ; 
	return watch_directory_rec(discard, userkey, abs_path, rel_base_path, m) ;
}


//
//		Notify:watch_directory_rec 
//
//////////////////////////////////////////////////////////////////////////////////////////
bool Notify::watch_directory_rec(int& wd_out, const string& userkey, const string& abs_path, const string& rel_base_path, EventsMask m)
{
//	lock on inot_mutex_ not needed ;

	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;
			
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "userkey: \"" << userkey << "\"  abs_path: \"" << abs_path << "\"  rel_base_path: \"" << rel_base_path << "  mask: "  // PRQA S 3081
			<< int(m) << " ( " << eventmask_to_string(m) << " )") ;

	if (! File::exists(abs_path) ) {
		ACS_THROW(NoSuchFileOrDirectoryException("Fatal Error: Directory not found: \"" + abs_path + "\"" )) ; // PRQA S 3081
    }
	if (! File::isDir(abs_path) ) {
		ACS_THROW(NotADirectoryException("Fatal Error: Not a directory: \"" + abs_path + "\"" )) ; // PRQA S 3081
	}
    
	const int originator_wd = -1 ;
	const bool unique = false ; 	/* The entry is NOT process unique (so several thread might watch the same path) */
	return add_directory_tree(unique, userkey, abs_path, rel_base_path, m, originator_wd, wd_out) ;
}


//
//		Notify:watch_directory_rec 
//
//////////////////////////////////////////////////////////////////////////////////////////
bool Notify::watch_directory_rec_unique(int& wd_out, const string& userkey, const string& abs_path, const string& rel_base_path, EventsMask m)
{
//	lock on inot_mutex_ not needed ;

	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;
			
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "userkey: \"" << userkey << "\"  abs_path: \"" << abs_path << "\"  rel_base_path: \"" << rel_base_path << "  mask: "  // PRQA S 3081
			<< int(m) << " ( " << eventmask_to_string(m) << " )") ;

	if (! File::exists(abs_path) ) {
		ACS_THROW(NoSuchFileOrDirectoryException("Fatal Error: Directory not found: \"" + abs_path + "\"" )) ; // PRQA S 3081
    }
	if (! File::isDir(abs_path) ) {
		ACS_THROW(NotADirectoryException("Fatal Error: Not a directory: \"" + abs_path + "\"" )) ; // PRQA S 3081
    }
	
	const int originator_wd = -1 ;
	const bool unique = true ;		/* This means that any entry is PROCESS UNIQUE */
	return add_directory_tree(unique, userkey, abs_path, rel_base_path, m, originator_wd, wd_out) ;
}


//
//		Notify:get_events_map_relpath
//
//////////////////////////////////////////////////////////////////////////////////////////
size_t Notify::get_events_map_relpath(const string& userkey, multimap<string, Notify::Watch_Event>& m, int wait_msec) 
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;

	m.clear() ;	// Empty the input map 
	
	Watch_Event_Container_Map_RelPath c(m, inot_entries_) ;	// Create a Watch_Event_Container, with a reference to the input map

	add_events_to_container( userkey, &c , wait_msec ) ;	// Fill Watch_Event_Container (thus, the input map)
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Returning " << m.size() << " events") ;

	return m.size() ;
}


//
//		Notify:get_events_map_abspath
//
//////////////////////////////////////////////////////////////////////////////////////////
size_t Notify::get_events_map_abspath(const string& userkey, multimap<string, Notify::Watch_Event>& m, int wait_msec) 
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;

	m.clear() ;	// Empty the input map 

	Watch_Event_Container_Map_AbsPath c(m, inot_entries_) ;	// Create a Watch_Event_Container, with a reference to the input map

	add_events_to_container( userkey, &c , wait_msec ) ;	// Fill Watch_Event_Container (thus, the input map)
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Returning " << m.size() << " events") ;

	return m.size() ;
}


//
//		Notify:get_events_map_wd
//
//////////////////////////////////////////////////////////////////////////////////////////
size_t Notify::get_events_map_wd(const string& userkey, multimap<int, Notify::Watch_Event>& m, int wait_msec) 
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;

	m.clear() ;	// Empty the input map 

	Watch_Event_Container_Map_WD c(m, inot_entries_) ;			// Create a Watch_Event_Container, with a reference to the input map

	add_events_to_container( userkey, &c , wait_msec ) ;	// Fill Watch_Event_Container (thus, the input map)
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Returning " << m.size() << " events") ;

	return m.size() ;
}


//
//		Notify:get_events_map_originator_wd
//
//////////////////////////////////////////////////////////////////////////////////////////
size_t Notify::get_events_map_originator_wd(const string& userkey, multimap<int, Notify::Watch_Event>& m, int wait_msec) 
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;

	m.clear() ;	// Empty the input map 

	Watch_Event_Container_Map_Originator_WD c(m, inot_entries_) ;			// Create a Watch_Event_Container, with a reference to the input map

	add_events_to_container( userkey, &c , wait_msec ) ;	// Fill Watch_Event_Container (thus, the input map)
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Returning " << m.size() << " events") ;

	return m.size() ;
}


//
//		Notify:get_events_map_vector
//
//////////////////////////////////////////////////////////////////////////////////////////
size_t Notify::get_events_vector(const string& userkey, vector <Notify::Watch_Event>& v, int wait_msec) 
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;

	v.clear() ;	// Empty the input vector 

	Watch_Event_Container_Vector c(v, inot_entries_) ;			// Create a Watch_Event_Container, with a reference to the input vector

	add_events_to_container( userkey, &c , wait_msec ) ;	// Fill Watch_Event_Container (thus, the input vector)

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Returning " << v.size() << " events") ;

	return v.size() ;
}


//
//		Notify:add_events_to_container
//
//////////////////////////////////////////////////////////////////////////////////////////
size_t Notify::add_events_to_container(const string& userkey, Watch_Event_Container_BASE* container, int wait_msec)
{
	ThreadSafe::Lock lock(inot_entries_mutex_) ; // Could be moved after the POLL -- to be checked 
 
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;
			
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "userkey: \"" << userkey << "\"  poll waiting time: " << wait_msec) ;
	
	struct pollfd plfd[1] ;
    plfd[0].fd = inot_instance_;
    plfd[0].events = POLLIN ;
	const EventsMask events_to_be_discarded = NOTIFY_IGNORED ;	// This event is produced EVEN AFTER A REMOVAL
	size_t loop = 5 ;
	
	/*
		Since the events read might contain the creation of new directories, these directories will be automatically watched, too.
		This may result in new events (e.g. the creation of nested subdirectories). In order to ensure faster discoveries, the reading loop
		is exectued more than one time, until:
		1) No new data is found
		-OR-
		2) Too many consecutive loops were executed (otherwise a infinite looping could be obtained).
	*/
	
	size_t new_events = 0 ; 
	bool new_event = false ;
	if (wait_msec != 0) {
        int p = 0 ;
		
		do
		{
			if ( (p = poll(plfd, sizeof(plfd)/sizeof(plfd[0]), wait_msec)) < 0) { // PRQA S 3084
				int err = errno;
				ostringstream o ; o << "inot: " << inot_instance_ << "  Error running command poll" ;
				ACS_THROW(NotifyException(o.str(), err)); // PRQA S 3081
			}

			new_event = ( (p>0) && (plfd[0].revents == POLLIN) ) ;

			if ( new_event )
			{
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Loops to do: " << loop << "  About to read data ... (buffer size: " << buffer_size_ << " bytes)") ;

				ssize_t r = 0 ; 
                ssize_t ptr = 0 ;

				if ( (r=read (plfd[0].fd, inot_buffer_, buffer_size_))  < 0) {
					int err = errno;
					ostringstream o ; o << "inot: " << inot_instance_ << "  Read error ! (" << r << ")" ;
					ACS_THROW(NotifyException(o.str(), err)); // PRQA S 3081
				}

				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Successfully read: " << r << "  bytes (buffer size: " << buffer_size_ << ")") ;
						
				if (r > 0)
				{
					ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Processing data buffer ...") ;
					ptr = 0 ; while (ptr < r)
					{
						struct inotify_event *pevent = reinterpret_cast<struct inotify_event *> (inot_buffer_ + ptr) ; // PRQA S 3081, 3030

						if ( not ((pevent->mask & events_to_be_discarded) == events_to_be_discarded) )
						{
							ACS_COND_THROW( (inot_entries_.find(pevent->wd) == inot_entries_.end()), NotifyCriticalException("Internal Error: wd not in map") ) ; // PRQA S 3081

							Extended_inotify_event matched_event ;

							const pair <bool, bool> r1 = archive_retrieve_incomplete_events(pevent, matched_event) ;

							const bool to_insert = r1.first ;
							const bool to_merge = r1.second ; 

							if (to_insert)
							{	
								/* Get the list of users registered to wd currently processed */
								const vector <string> userkeys = get_userkeys_from_wd( pevent->wd ) ;

								for (vector <string>::const_iterator k=userkeys.begin(); k!=userkeys.end(); k++)
								{
									/* For each registered user: Create the Watch_Event and add it to map: stored_events_ */

									const string uk = *k ; 

									const bool proc_unique = false ; 	/* Not relevant here -- anyway if the directory's parent had process_unique flag set, 
																			the insert will be performed once */
									if (0 != (pevent->mask & NOTIFY_ISDIR)) { automatically_add_new_entries(proc_unique, uk, pevent) ; }	// Watch new directories, if any

									ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "About to built a new WatchEvent for this entry: " // PRQA S 3013, 3081, 3380, 3385
											<< "userkey: " << uk << "  wd: " << pevent->wd << "  mask: 0x" 
											<< hex << pevent->mask << dec << "  (" << eventmask_to_string(static_cast<EventsMask>(pevent->mask)) // PRQA S 3081 2
											<< ")  entry: \"" << ( (pevent->len > 0)? string(pevent->name) : string("") ) << "\"" ) ;

									Watch_Event ev ; ev.build(uk, pevent, inot_entries_) ; 

									if (to_merge) { merge_matched_events(uk, matched_event, ev) ; }
									{
										/* Lock the stored_events_ map and add the event to the user's own list */
										ThreadSafe::Lock lock1(inot_entries_mutex_) ;
										stored_events_[ uk ].push_back(ev) ; 
									}

								}	// END: for

								if (to_merge)
								{
									/* 
										Since the matched event has been merged with ALL the userkeys, it should be removed from the list:
									*/
									remove_merged_event(matched_event) ;

								}

							}	// END: if (to_insert)
						}	// END: if ( not ((pevent->mask & events_to_be_discarded) == events_to_be_discarded) )

						if ( 0 != (pevent->mask & NOTIFY_DELETE_SELF) ) {

							ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Entry: wd: " << pevent->wd<< "  REMOVED_SELF. About to remove descriptor from map inot_entries_ ...") ;

							remove_from_inot_entries(pevent->wd) ; 
						}

						// Prepare the processing of next event :
						ptr += sizeof(struct inotify_event) + pevent->len; // PRQA S 3000, 3084

					}	// END: while (ptr < r)
				}	// END: if (r > 0)

				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Successfully processed: " << ptr << "  bytes (read: " << r << ")") ;
			}
			else {
				// (new_events == false)
				ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Loops to do: " << loop << "  No POLLIN event found") ;
			}

		} while (new_event && (--loop >0) ) ; // PRQA S 3230
	
	} else {
				
		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "inotify poll was NOT called since a null timeout was provided") ;

	}	// END: if (wait_msec != 0) 
		
	/* WHEN HERE: All the low-level events were stored in the stored_events_ persistent map */
	
	/* Scan the stored_events_ persistent map to add events to the specific userkey */
	
	map <string, deque <Watch_Event> >::iterator stored_event = stored_events_.find(userkey) ;
	
	
	if (stored_event != stored_events_.end())
	{	
		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Moving " << (stored_event->second).size() 
				<< " events from the map stored_events_ to the container of userkey: \"" << userkey << "\"") ;

		for (size_t i=0; i<(stored_event->second).size(); i++) {
			container->insert( stored_event->first, (stored_event->second)[i] ) ;	// Add to the container
		}
		
		stored_events_.erase(stored_event) ;
	
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "stored_events_ maps EMPTY for userkey: \"" << userkey << "\"") ;
	}

	
	flush_mismatched_cookies() ;	/* Check the cookies vector inot_arch_events_by_date_ , removing expired entries */
		
	
	check_stored_events_trasholds() ;	/* Check the size of map stored_events_ , shrinking it if needed */

	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		if ( new_events > 0 ) { dump_entry_map() ; }
	ACS_CLASS_END_DEBUG

	
	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
		if ( not container->empty() ) { container->dump(userkey) ; } 
	ACS_CLASS_END_DEBUG
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method finished. Returning: " << container->size() << " events") ;

#if(0) 
	ACS_LOG_DEBUG("****************************************  THIS DUMP IS AT THE END OF METHOD " << SimpleDebugSignature << "******************") ;	
	ACS_LOG_DEBUG(DebugSignatureThread << "\n\tinot_arch_events_by_cookie_.size: " << inot_arch_events_by_cookie_.size() 
			<< "  inot_arch_events_by_date_.size: " << inot_arch_events_by_date_.size()) ;
	
	size_t cnt = 0 ;
	for (map <string, deque <Watch_Event> >::const_iterator i=stored_events_.begin() ; i!=stored_events_.end() ; i++)
	{
        cnt++ ;  
        ACS_LOG_DEBUG("\tUserkey: \"" << i->first << "\"  size: " << (i->second).size()) ; 
    } ; 
	if (0 == cnt) {
        ACS_LOG_DEBUG("\tstored_events_ maps is EMPTY") ; 
    }
	ACS_LOG_DEBUG("\tinot_entries_.size() " << inot_entries_.size()) ;
	ACS_LOG_DEBUG("**********************************************************************************************************************") ;	
#endif
			
	return container->size() ;
}



//
//		Notify::set_max_stored_events_limits
//
//////////////////////////////////////////////////////////////////////////////////////////
void Notify::set_max_stored_events_limits(const size_t& th, const size_t& ll)
{
	ACS_COND_THROW( (ll > th), NotifyCriticalException("Invalid pair of threshold/lowlimit") ) ; // PRQA S 3081
			
	max_stored_events_threshold_ = th ;
	max_stored_events_lowlimit_ = ll ;	
	
	ACS_LOG_PRIVINFO(SimpleDebugSignature << "max_stored_events_threshold set to: " << max_stored_events_threshold_ ) ;
	
	if (max_stored_events_threshold_ > 0)
	{
		ACS_LOG_PRIVINFO(SimpleDebugSignature << "max_stored_events_lowlimit_ set to: " << max_stored_events_lowlimit_ ) ;
	}
}	


//
//		(PRIVATE) Notify::check_stored_events_trasholds
//
//////////////////////////////////////////////////////////////////////////////////////////
bool Notify::check_stored_events_trasholds()
{
	bool found = false ;
	
	if (max_stored_events_threshold_ > 0) 
	{			
		ThreadSafe::Lock lock(inot_entries_mutex_) ;
		
		for (map <string, deque <Watch_Event> >::iterator i=stored_events_.begin() ; i!=stored_events_.end() ; i++) // PRQA S 4238
		{	
			if ( (i->second).size() > max_stored_events_threshold_)
			{
				{
					ACS_LOG_WARNING(DebugSignatureThread << "userkey: \"" << i->first << "\" exceedes max_stored_events_threshold: " 
							<< (i->second).size() << " / " << max_stored_events_threshold_ << ". Shrinking number of events to: " <<  max_stored_events_lowlimit_) ;
				}

				found = true ;
				(i->second).erase( (i->second).begin(), (i->second).end()-max_stored_events_lowlimit_) ; // PRQA S 3000
			}
		}  
		
		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Method finished. Returning: " << boolalpha << found ) ;
	}
	else
	{	
		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "max_stored_events_threshold set to zero: nothing to do") ;
	}
					
	return found ; 
}	
	
	
//
//		(PRIVATE) Notify::flush_mismatched_cookies
//
//////////////////////////////////////////////////////////////////////////////////////////
bool 											/* OUT: True if at least one cookie was removed */
		Notify::flush_mismatched_cookies() 
{
	ThreadSafe::Lock lock(inot_entries_mutex_) ;
	size_t cnt = 0 ; 

	if ( ! inot_arch_events_by_date_.empty() )
	{
		const uint64_t curtime = get_key_from_curtime () ;
		const uint64_t threshold_time = curtime - filesystem_delay_usec ; // PRQA S 3084

		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Flushing incomplete events.  Current time: " << debug_time_to_string(curtime) 
				<< "  Threshold time: " << debug_time_to_string(threshold_time) << "  (threshold: " << filesystem_delay_usec << " usec)") ;

		multimap <uint64_t, uint32_t>::iterator i ; 
        multimap <uint64_t, uint32_t>::iterator lb = inot_arch_events_by_date_.lower_bound (threshold_time) ; 
		map <uint32_t, Extended_inotify_event>::iterator j ;

		if (lb != inot_arch_events_by_date_.begin())
		{
			for (i = inot_arch_events_by_date_.begin() ; (i != lb) ; i++)
			{
				++cnt ; 
				j = inot_arch_events_by_cookie_.find(i->second) ; 
				if (j != inot_arch_events_by_cookie_.end())
				{

					ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Flushing incomplete event #" << cnt << "  date: " << debug_time_to_string(i->first) 
							<< "  cookie: " << i->second) ;

					const vector <string> userkeys = get_userkeys_from_wd( j->second.ionot_event.wd ) ;

					for (vector <string>::const_iterator k=userkeys.begin(); k!=userkeys.end(); k++)
					{
						const string uk = *k ; 

						Watch_Event ev ; ev.build(uk, j->second, inot_entries_) ;

						stored_events_[ uk ].push_back(ev) ; 

					}

					inot_arch_events_by_cookie_.erase(j) ; 
				}
			}

			inot_arch_events_by_date_.erase( inot_arch_events_by_date_.begin(), lb ) ;
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "No incomplete events to flush. Oldest entry: "
					<< debug_time_to_string((inot_arch_events_by_date_.begin())->first)) ;
		}

	}	// END: if ( inot_arch_events_by_date_.begin() != inot_arch_events_by_date_.end() )

	return (cnt>0) ;	
}


//
//		Notify::unregister_userkey
//
//////////////////////////////////////////////////////////////////////////////////////////
bool Notify::unregister_userkey(const string& userkey, bool emit_exception) 
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called with userkey: \"" << userkey << "\"") ;
			
	ThreadSafe::Lock lock(inot_entries_mutex_) ;
	bool found = false ; 
			
	for (map <int, Entry_feat_by_userkey>::iterator it=inot_entries_.begin(); it!=inot_entries_.end(); ) // PRQA S 4238
	{
		// erase on items ivalidates the iterator 
		map <int, Entry_feat_by_userkey>::iterator i = it;
		// forward the loop iterator in advance to protect from erases
		it++;

		if( (i->second).erase(userkey) > 0 ) {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Userkey \"" << userkey << "\" found for wd " << i->first) ; 
			
			if ( (i->second).empty() )
			{
				if (inotify_rm_watch(inot_instance_, (i->first)) < 0)
				{
					ACS_LOG_WARNING(DebugSignatureThread << "couldn't execute method inotify_rm_watch with userkey \"" << userkey << "\" for wd: " << i->first) ;
				}

				// after removing the watch must remove also the pending events
				// that will belong to an unregistered notifier
				vector<Notify::Watch_Event> watchEventsV;
				this->get_events_vector(userkey, watchEventsV);
				
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "wd: " << i->first << " totally removed from map inot_entries_") ; 

				inot_entries_.erase(i) ;
			}
					
			found = true ;
		}
	}

	
	if (found)
	{
		if( stored_events_.erase(userkey) > 0 ) {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Stored events corresponding to key \"" << userkey << "\" successfully removed" ) ; 
		}
	}
	
	ACS_COND_THROW((!found && emit_exception), NotifyException("No path to watch for userkey \"" + userkey + "\"") ); // PRQA S 3081

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished with userkey: \"" << userkey << "\"  returning: " << boolalpha << found) ; 

	return found ;
}


//
//		Notify::generate_unique_userkey
//
//////////////////////////////////////////////////////////////////////////////////////////
string Notify::generate_unique_userkey(const string &label) 
{
	return generate_unique_userkey(pthread_self(), label) ; 
}


//
//		Notify::generate_unique_userkey
//
//////////////////////////////////////////////////////////////////////////////////////////
string Notify::generate_unique_userkey(const pthread_t& tid, const string &label) 
{
	ostringstream o ; o << tid << "_" << get_key_from_curtime() ;
	string s = o.str() ; 
	
	if (!label.empty()) { s += "_" + label ;  }

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Returning userkey: \"" << s << "\"") ;

	return s ; 
}

//
//		Notify::eventmask_to_string
//
//////////////////////////////////////////////////////////////////////////////////////////
string Notify::eventmask_to_string(EventsMask m) 
{
//FIXME ACS_CLASS_WRITE_DEBUG_TH ( ACS_HIG_VERB, "Method started. Input mask: " << hex << m << dec ) ;

	string ret ; 

	if (m == NOTIFY_NOTHING) { ret = "NOTIFY_NOTHING" ; } 
	else if (m == NOTIFY_ALL) { ret = "NOTIFY_ALL" ; }
    else { /* nothing */ }

    // PRQA S 3003 L2
	if ( ret.empty() )
	{
		if (0 != (m & NOTIFY_ISDIR)) { ret += "IS_DIRECTORY | " ; } 
	
		if (0 != (m & NOTIFY_ACCESS)) { ret += "ACCESS | " ; }
		if (0 != (m & NOTIFY_MODIFY)) { ret += "MODIFY | " ; }
		if (0 != (m & NOTIFY_ATTRIB)) { ret += "ATTRIB | " ; }

		if ( (m & NOTIFY_CLOSE) == NOTIFY_CLOSE) { ret += "CLOSE | " ; }
		else
		{
			if (0 != (m & NOTIFY_CLOSE_WRITE))  { ret += "CLOSE_WRITE | " ; }
			if (0 != (m & NOTIFY_CLOSE_NOWRITE))  { ret += "CLOSE_NOWRITE | " ; }
		}
		
		if ( (m & NOTIFY_MOVE) == NOTIFY_MOVE) { ret += "MOVE | " ; }
		else
		{
			if (0 != (m & NOTIFY_MOVED_TO)) { ret += "MOVED_TO | " ; } 
			if (0 != (m & NOTIFY_MOVED_FROM)) { ret += "MOVED_FROM | " ; } 
		}

		if (0 != (m & NOTIFY_OPEN)) { ret += "OPEN | " ; }  
		if (0 != (m & NOTIFY_CREATE)) { ret += "CREATE | " ; }  
		if (0 != (m & NOTIFY_DELETE)) { ret += "DELETE | " ; }  
		if (0 != (m & NOTIFY_DELETE_SELF)) { ret += "DELETE_SELF | " ; }  
		if (0 != (m & NOTIFY_MOVE_SELF)) { ret += "MOVE_SELF | " ; }  
		if (0 != (m & NOTIFY_IGNORED)) { ret += "IGNORED | " ; }  

		size_t l_ret = ret.length() ;
		if ( (l_ret>=3) && (ret.substr(l_ret-3)==" | ") ) { ret = ret.substr(0, l_ret-3) ; } // Beautify // PRQA S 3084, 4400

	}
    // PRQA L:L2
    ACS_CLASS_WRITE_DEBUG_TH ( ACS_HIG_VERB, "Method finished. Returning: \"" << ret << "\"" ) ;

	return ret ; 
}


//
//		(PRIVATE) Notify::get_userkeys_from_wd
//
//////////////////////////////////////////////////////////////////////////////////////////
vector <string> 
				Notify::get_userkeys_from_wd(const int& wd) const
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;

	ThreadSafe::Lock lock(inot_entries_mutex_) ;
	
	vector <string> ret ; 

	map <int, Entry_feat_by_userkey>::const_iterator i ( inot_entries_.find(wd) ) ;	

	if (i != inot_entries_.end())
	{
		// (i->second) is a map of type Entry_feat_by_userkey. It will be referenced below with the const_iterator j :

		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "wd: " << wd << "  Adding " << (i->second).size() << " userkeys") ;

		for (Entry_feat_by_userkey::const_iterator j=(i->second).begin() ; j!=(i->second).end(); j++)
		{
			/* Scan the map <userkey, Entry_features> , i.e. scan all the userkeys which registered the Entry_feature */

			const string uk = j->first ;

			ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "wd: " << wd << "  adding userkey: \"" << uk << "\"") ;

			ret.push_back( uk ) ;

		}	// END: for
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "No userkey registered for wd: " << wd) ;
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Returning: " << ret.size() << " userkeys") ;

	return ret ; 
}


//
//		(STATIC) Notify::resolve_originator_wd
//
//////////////////////////////////////////////////////////////////////////////////////////
int																	/* OUT: originator_wd  */ 														
		Notify::resolve_originator_wd(	const string& userkey, 		/* IN: input userkey */
										const int& wd_in, 			/* IN: input wd */
										const map <int, Entry_feat_by_userkey>&	inot_entries)	/* IN: map ofinot_entries  */
{	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called with userkey: \"" << userkey << "\"") ;
			
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "input wd: " << wd_in << "  userkey: \"" << userkey << "\"")  ;
	
	if ( wd_in < 0) {
		ostringstream o ; o << "invalid watch-descriptor passed as input: " << wd_in ;
		ACS_THROW( NotifyCriticalException(o.str()) ) ; // PRQA S 3081
	}

	map <int, Entry_feat_by_userkey>::const_iterator i ;
	bool finished = false ; 
    int wd = wd_in ; 
    do
	{
		// LOOP until the originator_wd is found
	
		i = inot_entries.find(wd) ;
		if ( i == inot_entries.end() ) {
			// Entry wd not found in map inot_entries
			ostringstream o ; o << "descriptor not found in map: " << wd << "  (input descriptor: " << wd_in << ")" ;
			ACS_THROW( NotifyCriticalException(o.str()) ) ; // PRQA S 3081
		}	// if ( i == inot_entries.end() ) 
	
		/* IF HERE: entry (of type Entry_feat_by_userkey) with key "wd" was found in map inot_entries */
	
		/* Now access map Entry_feat_by_userkey and look for key "userkey" */
		
		Entry_feat_by_userkey::const_iterator j ( (i->second).find(userkey) ) ;
						
		if (j == (i->second).end())
		{
			// Entry wd with userkey not found in map inot_entries
			ostringstream o ; o << "descriptor " << wd << " not found in map with userkey: \"" << userkey << "\"  (input descriptor: " << wd_in << ")" ;
			ACS_THROW( NotifyCriticalException(o.str()) ) ; // PRQA S 3081
		}
		
		int originator_wd = (j->second).originator_wd ;
		
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Processing wd: " <<  wd << "  found originator (so far) : " << originator_wd) ;

		if ( originator_wd == wd ) {
			// originator_id was found:
			finished = true ; 
		} else {
			wd = originator_wd ;
		}

	} while (! finished) ;

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "wd resolved: " << wd) ;

	return wd ; 
}

//
//		(PRIVATE) Notify::resolve_originator_wd
//
//////////////////////////////////////////////////////////////////////////////////////////
int																	/* OUT: originator_wd (-1 if not found) */ 														
		Notify::resolve_originator_wd(	const string& userkey, 		/* IN: input userkey */ // PRQA S 4020
										const int& wd_in, 			/* IN: input wd */
										string& abs_base_path, 		/* OUT: absolute base path for entry */
										string& rel_base_path, 		/* OUT: relative base path for entry */
										uint32_t& mask_out, 		/* OUT: mask of originator-id */
										bool& recursive, 			/* OUT: true if recursive */
										bool emit_exception) const	/* IN: TRUE to emit exception in case of error. If FALSE -1 will be returned */
{
	ThreadSafe::Lock lock(inot_entries_mutex_) ;
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called with userkey: \"" << userkey << "\"") ;
			
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "input wd: " << wd_in << "  userkey: \"" << userkey << "\"  emit_exception: " << boolalpha << emit_exception);
	
	if ( wd_in < 0) {
		ostringstream o ; o << "invalid watch-descriptor passed as input: " << wd_in ;
		ACS_THROW( NotifyCriticalException(o.str()) ) ; // PRQA S 3081
	}

	map <int, Entry_feat_by_userkey>::const_iterator i ;
	mask_out = 0 ;
	abs_base_path = "" ; 
    rel_base_path = "" ;

	bool finished = false ; 
    bool first_run = true ; 
    int wd = wd_in ; 
    do
	{
		// LOOP until the originator_wd is found
	
		i = inot_entries_.find(wd) ;
		if ( i == inot_entries_.end() ) {
		
			// Entry wd not found in map inot_entries_
			ostringstream o ; o << "descriptor not found in map: " << wd << "  (input descriptor: " << wd_in << ")" ;
			if (emit_exception) {
				ACS_THROW( NotifyCriticalException(o.str()) ) ; // PRQA S 3081
			} else {
				ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, o.str()) ;
				return -1 ; 
			}
		}	// if ( i == inot_entries_.end() ) 
	
		/* IF HERE: entry (of type Entry_feat_by_userkey) with key "wd" was found in map inot_entries_ */
	
		/* Now access map Entry_feat_by_userkey and look for key "userkey" */
		
		Entry_feat_by_userkey::const_iterator j ( (i->second).find(userkey) ) ;
				
		if (j == (i->second).end())
		{
			// Entry wd with userkey not found in map inot_entries_
			ostringstream o ; o << "descriptor " << wd << " not found in map with userkey: \"" << userkey << "\"  (input descriptor: " << wd_in << ")" ;
			if (emit_exception) {
				ACS_THROW( NotifyCriticalException(o.str()) ) ; // PRQA S 3081
			} else {
				ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, o.str()) ;
				return -1 ; 
			}
		}
		
		int originator_wd = (j->second).originator_wd ;
		if ( first_run ) {  
			abs_base_path = (j->second).full_path; 
			rel_base_path = (j->second).rel_path ; 
			first_run = false ; 
		}
		
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Processing wd: " <<  wd << "  found originator (so far) : " << originator_wd) ;

		if ( originator_wd == wd ) {
			// originator_id was found:
			
			mask_out = (j->second).mask ;	// Set mask as originator-wd mask
			recursive = (j->second).rec ; 
			finished = true ; 
		} else {
			wd = originator_wd ;
		}

	} while (! finished) ;

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "wd resolved: " << wd) ;

	return wd ; 
}


//
//		(PRIVATE) Notify::automatically_add_new_entries
//
//////////////////////////////////////////////////////////////////////////////////////////
void 														
		Notify::automatically_add_new_entries(bool proc_unique, const string& userkey, const struct inotify_event *pevent)	
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.  userkey: \"" << userkey << "\"") ;

	ACS_COND_THROW( !pevent, NotifyCriticalException("Null pointer received as input") ) ; // PRQA S 3081

	const int wd = pevent->wd ;
	const uint32_t mask_in = pevent->mask ;

	if (  (0 != (mask_in & NOTIFY_ISDIR)) && 
        ( (0 != (mask_in & NOTIFY_CREATE)) || (0 != (mask_in & NOTIFY_MOVED_TO)) ) 
       )
	{
		char filename [ PATH_MAX + 1024 ] ; // PRQA S 4403
		memset (filename, 0, sizeof(filename)) ;
		if (pevent->len > 0) { strncpy (filename, pevent->name, pevent->len) ; }
		
		uint32_t mask_out = 0; 
        string abs_base_path ;
        string rel_base_path ; 
		bool recursive = false ; 
		const bool emit_exception = true ; 
		int originator_wd = resolve_originator_wd (userkey, wd, abs_base_path, rel_base_path, mask_out, recursive, emit_exception) ;

		if (recursive)
		{
			string abs_path = StringUtils::pathJoin(abs_base_path, filename) ;

			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "wd: " << wd << "  mask_in: " << hex << mask_in << dec // PRQA S 3013, 3081
					<< "  (" << eventmask_to_string(static_cast<EventsMask>(mask_in)) << ")" // PRQA S 3081
					<< "  resolved originator_wd: " << originator_wd 
					<< "  adding entry abs_path: \"" << abs_path << "\"  rel_base_path: \"" << rel_base_path
					<< "\"  mask: " << hex << mask_out) ;

			int wd_out = 0; 
			bool b = add_directory_tree(proc_unique, userkey, abs_path, rel_base_path, static_cast<EventsMask>(mask_out), originator_wd, wd_out) ; // PRQA S 3013, 3081 

			if (b) {
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Directory added with wd: " << wd_out) ;
			} else {
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Directory not added (already monitored with wd: " << wd_out << ")") ;
			}
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "wd: " << wd << "  mask: " << hex << mask_in << dec  // PRQA S 3013, 3081
					<< "  (" << eventmask_to_string(static_cast<EventsMask>(mask_in)) << ")  originator_wd not recursive: SKIPPED") ; // PRQA S 3081
		}
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "wd: " << wd << "  mask: " << hex << mask_in << dec  // PRQA S 3013, 3081
				<< "  (" << eventmask_to_string(static_cast<EventsMask>(mask_in)) << ")  not the creation of a new directory: SKIPPED") ; // PRQA S 3081
	}
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished") ;

	return ; 
}


//
//		(PRIVATE) Notify::add_directory_tree
//
//////////////////////////////////////////////////////////////////////////////////////////
bool 															/* OUT: TRUE if inserted in watch list (FALSE if already monitored) */
		Notify::add_directory_tree(	bool unique,				/* IN: TRUE means that the entry is PROCESS UNIQUE */ // PRQA S 4020
									const string& userkey, 		/* IN: userkey */
									const string& abs_path, 	/* IN: absolute full path to be added */
									const string& rel_base_path_in,/* IN: relative base path */
									EventsMask m, 				/* IN: events to monitor */
									const int& wd_orig_in, 		/* IN: originator_wd (only if this call is due to recursion. Otherwise, -1) */
									int& wd_out)				/* OUT: wd (watch_descriptor) */
{
//	lock on inot_mutex_ not needed ; ????

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.  userkey: \"" << userkey << "\"") ;
			
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "abs_path: \"" << abs_path << "\"  rel_base_path_in: \"" << rel_base_path_in << "\"  mask: "  // PRQA S 3081, 3380, 3385
			<< int(m) << " ( " << eventmask_to_string(m) << " )  originator_wd: " << ((wd_orig_in < 0)? "N/A" : debug_to_string(wd_orig_in))
			<< "  process_unique: " << boolalpha << unique)	;
	
			
	// const string absolute_root_path = Path::extract_dirname(Path::normalize(abs_path)) ;
	const string absolute_root_path = File::getDirPath(abs_path, true) ;
	// const string entry_name = Path::extract_filename(Path::normalize(abs_path)) ;
	const string entry_name = File::getFileName(StringUtils::pathNormalize(abs_path)) ;

	try
	{
		const string dirEntry = "" ; // To format the directories; So e.g. "/dir1/dir2/." becomes "/dir/dir2"
		const bool hidden = true ; 
		DirTree d(abs_path, dirEntry, hidden) ;	
		d.mindepth(1) ;		// The path itself WON'T be added to the list (explicit add_entry below)
		d.relative("") ;	// Since path will be built using given rel_base_path
		d.hide_files() ;	// Only the directories will be shown

		// d is the list of all nested subdirectories inside the path (NOT including the path itself and all the files)

		const bool rec = true ; 
		int wd = wd_orig_in ;

		// the relative path is EXACTLY the one provided (if this call is not due to recursion). Otherwise, the entry name will be also added
		const string rel_base_path = ( (wd_orig_in > -1) ?  StringUtils::pathJoin(rel_base_path_in, entry_name) : rel_base_path_in) ;  // PRQA S 3384, 3385
		
		// Add the entry itself, in order to acquire the future originator_wd :
		if ( ! add_entry(unique, userkey, abs_path, rel_base_path, m, rec, wd, wd_out) )
		{
			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Main entry with path: \"" << abs_path << "\" already watched") ;
			return false ; 
		}


		// Add all nested subdirectories. If the originator_id was given, that one will be used.
		// Otherwise, the one obtained above will be used.
		
		const int originator_wd = ( (wd_orig_in<0)? wd_out : wd_orig_in) ; 	// PRQA S 3384

		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Entry: \"" << abs_path << "\" added.  Nested subdirectories will be processed with originator_wd: " << originator_wd) ;


        size_t dsize = d.size() ;
		for (size_t i=0; i<dsize; ++i)
		{
			string entry_abs_path = StringUtils::pathJoin(absolute_root_path, entry_name, d[i]) ; 
		
			string entry_rel_path;
			
			entry_rel_path = StringUtils::pathJoin(rel_base_path, d[i]) ; 

			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Processing entries of directory: \"" << abs_path << "\"  #" << (i+1) << " of " <<  d.size()  // PRQA S 3084
					<< "  : \"" << entry_abs_path << "\"  (\"" << entry_rel_path << "\"") ;

			bool b = false ;

			try
			{
				
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Trying to insert in map entry #" << (i+1) << " of " <<  d.size() << "  : \""  // PRQA S 3084
						<< entry_abs_path  << "\"  (rel.name: \"" << entry_rel_path << ")") ;
					 
				b = add_entry(unique, userkey, entry_abs_path, entry_rel_path, m, rec, originator_wd, wd) ;
			}
			catch(std::exception& e)
			{
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Error while inserting subdirectory: \"" 
						<< entry_abs_path << "\" : SKIPPED: " << e) ;

				continue ; // Skip this entry
			}

			if (b)
			{
				
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Subdirectory: \"" << entry_abs_path << "\"  rel.path: \"" 
						<< entry_rel_path << "\"  added with wd: " << wd 
						<< " (originator: " << originator_wd << ")") ;
			}
			else
			{
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Subdirectory: \"" << entry_abs_path << "\" already watched by wd: " << wd) ;
			}
			
		}
	}
	catch(std::exception& e)
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Error processing subdirectories of path: \"" 
				<< abs_path << "\" : IGNORED: " << e) ;
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		dump_entry_map() ; 
	ACS_CLASS_END_DEBUG

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished") ;
	return true ; 
}


//
//		(PRIVATE) Notify::add_entry
//
//////////////////////////////////////////////////////////////////////////////////////////
bool 														/* OUT: TRUE if inserted in watch list (FALSE if already monitored) */
		Notify::add_entry(	bool proc_unique,				/* IN: true if the entry must be process-unique: the entry won't be added if already watched by the process */
							const string& userkey,			/* IN: user key */
							const string& full_path, 		/* IN: path to be added */
							const string& rel_path,			/* IN: path to be added (as relative to a user's root directory) */
							EventsMask m, 					/* IN: events to monitor */
							bool rec, 						/* IN: recursive flag (used only to be stored inside the map) */
							const int& wd_orig_in, 			/* IN: originator_wd (only if this call is due to recursion. Otherwise, -1) */
							int& wd_out)					/* OUT: wd (watch_descriptor) */
{
	ThreadSafe::Lock lock(inot_entries_mutex_) ;
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;
			
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "userkey: \"" << userkey << "\"  full_path: \"" << full_path << "  rel_path: \"" << rel_path  // PRQA S 3081, 3380, 3385
			<< "\"  originator_wd: " << ((wd_orig_in < 0)? "N/A" : debug_to_string(wd_orig_in)) 
			<< "  mask: " << int(m) << " ( " << eventmask_to_string(m) << " )" << "  process_unique: " << boolalpha << proc_unique) ;

#if(0)		 
/* FIXME 
* ENRCAR::	This part was DISABLED after discovering that calling method inotify_add_watch of a directory ALREADY watched would result 
			in returning the current wd of the directory (NOT a new entry).
			Moreover, in case of deletion (and following creation of a new directory with the same name) the inotify_add_watch would
			return a NEW wd (that is what expected). While the code below would return a previous, misleading value.

*/
	wd_out = -1 ; for (map <int, Entry_features>::const_iterator i = inot_entries_.begin() ; i != inot_entries_.end() ; i++)
	{
		if ((i->second).full_path == full_path) { wd_out = i->first; break ; }
	}

	if (wd_out >= 0)
	{
		// Found !
		
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Userkey: \"" << userkey << "\"  Entry with path: \"" << full_path << "\" NOT INSERTED IN MAP since already watched with wd: " << wd_out) ;

		return false ; 
	}
#endif
	
	
	/**
		In case of directory RENAME (e.g. the monitoring entry "/old" becames "/new") this portion of code will be executed.
		Anyway, the SAME id will be returned in case of directory renaming.
		Suppose that inotify is watching these three directories:
		/dir (wd=1)
		/dir/old (wd=2)
		/dir/old/entry (wd=3)
		If "mv /dir/old /dir/new" is executed, the add_watch of "/dir/new" WOULD RETURN wd=2 (NOT wd=4).
		Also, .new/entry would be recursively processed and re-inserted in map with wd=3
		THIS IS PARTICULARLY USEFUL TO HANDLE THE MOVE COMMANDS !
	*/
	
	if ( (wd_out = inotify_add_watch(inot_instance_, full_path.c_str(), m)) < 0) // PRQA S 3000
	{
		int err = errno;
		ostringstream o ; o << "inot: " << inot_instance_ << "  Unable to add a watch to path: \""  << full_path << "\"" ; 
		ACS_THROW(NotifyException(o.str(), err)); // PRQA S 3081
	}
	
	bool to_insert = true ; 
	{
		/* Check if the entry is already registered for the current userkey */	
		
		map <int, Entry_feat_by_userkey>::const_iterator i = inot_entries_.find(wd_out) ;
		
		if (i != inot_entries_.end())
		{
			if (!proc_unique) {
				/* Not process unique: it will be inserted if not watched by the very same user */
				to_insert = ((i->second).find(userkey) == (i->second).end()) ;
			} else {
				to_insert = ((i->second).empty()) ; 
			}
		}
	}
	
	if (proc_unique && !to_insert)
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Entry with path: \"" << full_path << "\"  already monitored by this process with wd: " << wd_out << " So it won't be added again") ;
	}
	else
	{
		Entry_features ef ;
		ef.originator_wd = ( (wd_orig_in<0)? wd_out : wd_orig_in) ; // PRQA S 3380
		ef.mask = m ; // PRQA S 3000
		ef.full_path = full_path ; 
		ef.rel_path = rel_path ; 
		ef.rec = rec ;

		(inot_entries_ [wd_out])[userkey] = ef ;	// Insert entry in the map, or _REPLACE_ the current value

		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Userkey: \"" << userkey << "\"  Entry with path: \"" << full_path << "\"  rel.path: \"" << rel_path << "\"  INSERTED IN MAP with wd: " << wd_out 
				<< "  (originator_wd: " << ef.originator_wd << ")") ;
	}
			
	return to_insert ; 
}

			
//
//		(PRIVATE) Notify::archive_retrieve_incomplete_events
//
//////////////////////////////////////////////////////////////////////////////////////////
pair <bool, 																							/* OUT: to_insert */
		bool>																							/* OUT: to_merge */
				Notify::archive_retrieve_incomplete_events(	const struct inotify_event *pevent, 		/* IN: current event */
															Extended_inotify_event& matched_event)		/* OUT: matched event, if any */
{
	ThreadSafe::Lock lock(inot_entries_mutex_) ;
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;

	bool	to_insert = false ;
	bool	to_merge = false ;
	const EventsMask maskable_events = NOTIFY_MOVE ; 	// Maskable events are: MOVE (any kind)
	
	const EventsMask event_in = Notify::EventsMask(pevent->mask) ; // PRQA S 3013, 3081
		
	if ( 0 == (event_in & maskable_events) ) // PRQA S 3003
	{
		// Event is NOT maskable
	
		to_insert = true ;	// Event not maskable. It will be immediately inserted in the external list
		to_merge = false ;	// Since the event is not maskable, it shall not be merged.
		
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Rejecting NON-MASKABLE Event: 0x" << hex << int(event_in) << dec << "  ( "  // PRQA S 3081
				<< eventmask_to_string(event_in) << " )  -- it will be added to user list") ;
	}
	else 
	{

		// Event IS maskable:

		const uint32_t cookie = pevent->cookie ; 
		map <uint32_t, Extended_inotify_event>::iterator i = inot_arch_events_by_cookie_.find (cookie) ;

		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Processing cookie: " << cookie << "  for event: 0x" << hex << int(event_in) << dec  // PRQA S 3081
				<< "  ( " << eventmask_to_string(event_in) << " )") ;

		if ( (i == inot_arch_events_by_cookie_.end()) || ( (Notify::EventsMask((i->second).ionot_event.mask) & NOTIFY_MOVE) == (event_in & NOTIFY_MOVE)) )  { // PRQA S 3003, 3013, 3081
			/** EITHER: 
					Matching-cookie not found in list. 
				OR:
					Matching-cookie was found in list, but it belongs the same type of the input cookie (e.g. MOVE_FROM and MOVE_FROM). 
					This means that the cookie archived in list is misleading, and it will be overwritten with this one.
			*/

			to_insert = to_merge = false ; 	// Event is maskable but don't merge. It won't be inserted in the user's list

			Extended_inotify_event ee(*pevent) ; 	/* Current time is set as last_update */
			
			inot_arch_events_by_cookie_ [cookie] = ee ;	// Archive in internal list (replace the current value, if any, in the map)
			inot_arch_events_by_date_.insert( pair<uint64_t, uint32_t>(ee.last_update, cookie) ) ;	// Archive in internal list (adding to the multimap)
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Event ADDED to list.  Current size of map: " << inot_arch_events_by_cookie_.size()) ;
		}
		else
		{
			/* Matching-cookie found in list, and the type is compatible */
		
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Matching cookie: " << (i->second).ionot_event.cookie << "  found in map. Matching event: 0x" << hex << int((i->second).ionot_event.mask) << dec  // PRQA S 3013, 3081
					<< "  ( " << eventmask_to_string( Notify::EventsMask((i->second).ionot_event.mask) ) << " )") ;
		
			matched_event = (i->second) ; 	

			to_insert = to_merge = true ;	/* Event matched with an archived past event. */
			
		}
	
	}	// END: if ( ! (event_in & maskable_events) )

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Current size of cookie map: " << inot_arch_events_by_cookie_.size() 
			<< "  Current size of date map: " << inot_arch_events_by_date_.size()
			<< "  Returning:   to_insert: " << boolalpha << to_insert  << "  to_merge: " << boolalpha << to_merge) ;

	return pair <bool, bool>(to_insert, to_merge) ;
}


//
//		(PRIVATE) Notify::merge_matched_events
//
//////////////////////////////////////////////////////////////////////////////////////////
void Notify::merge_matched_events(	const string& userkey, 								/* IN: userkey */
										const Extended_inotify_event& matched_event_in, 	/* IN: matching event, fetched from the list */
										Watch_Event& ev) const								/* IN/OUT: current event, to be merged */
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. Matching cookies: " << matched_event_in.ionot_event.cookie << " and " << ev.cookie) ;
	

	/* Check the cookies */
	ACS_COND_THROW( (matched_event_in.ionot_event.cookie != ev.cookie), NotifyCriticalException("Internal error: Cookies don't match!") ) ; // PRQA S 3081

	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Matching cookie: " << ev.cookie << "  found in map. Matching event: 0x" << hex << int(ev.event) << dec // PRQA S 3081
			<< "  ( " << eventmask_to_string(ev.event) << " )") ;

	
	Watch_Event matched_event ; 

	matched_event.build(userkey, matched_event_in, inot_entries_) ;  

	if (ev.event == NOTIFY_MOVED_FROM) {

		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "DUMPING SOURCE Watch_Event for userkey: \"" << userkey << "\" :") ; 
			ev.dump() ; 
			ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "DUMPING DESTINATION Watch_Event for userkey: \"" << userkey << "\" :") ;  
			matched_event.dump() ; 
		ACS_CLASS_END_DEBUG

		ev = Watch_Event::merge(ev, matched_event) ;

	} else {

		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "DUMPING SOURCE Watch_Event for userkey: \"" << userkey << "\" :") ; 
			matched_event.dump() ; 
			ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "DUMPING DESTINATION Watch_Event for userkey: \"" << userkey << "\" :") ;  
			ev.dump() ; 
		ACS_CLASS_END_DEBUG

		ev = Watch_Event::merge(matched_event, ev) ;
	}
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished.") ;

	return ;
}


//
//		(PRIVATE) Notify::remove_merged_event
//
//////////////////////////////////////////////////////////////////////////////////////////
void Notify::remove_merged_event(const Extended_inotify_event& matched_event_in)
{
	ThreadSafe::Lock lock(inot_entries_mutex_) ;
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;

	const uint32_t cookie = matched_event_in.ionot_event.cookie ; 
	map <uint32_t, Extended_inotify_event>::iterator i = inot_arch_events_by_cookie_.find (cookie) ;

	if (i == inot_arch_events_by_cookie_.end())
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Couldn't find cookie: " << cookie) ;
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "About to remove cookie: " << cookie) ;

		bool r = remove_from_inot_arch_events_by_date( matched_event_in.last_update, cookie) ;
		inot_arch_events_by_cookie_.erase(i) ; 

		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Cookie " << cookie << " REMOVED from cookie_ map. Current size of map: " 
				<< inot_arch_events_by_cookie_.size() << "  Removed from date_ map ? " << boolalpha << r) ;
	}
		
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished.") ;

	return ;
}


//
//		(PRIVATE) Notify::remove_from_inot_arch_events_by_date
//
//////////////////////////////////////////////////////////////////////////////////////////
bool 
		Notify::remove_from_inot_arch_events_by_date(const uint64_t& key, const uint32_t& value) 
{
	ThreadSafe::Lock lock(inot_entries_mutex_) ;
	
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "key: " << key << "  value: " << value) ;
	bool found = false ; 
	
	pair < multimap<uint64_t, uint32_t>::iterator, multimap<uint64_t, uint32_t>::iterator > p = inot_arch_events_by_date_.equal_range(key) ;
	
	if ( p.first != inot_arch_events_by_date_.end() )
	{
		for ( multimap<uint64_t, uint32_t>::iterator it=p.first ; (!found && (it != p.second)) ; )
		{
			// Pre increment and protect from erases
			multimap<uint64_t, uint32_t>::iterator i = it;
			it++;
			
			if (i->second == value)
			{
				inot_arch_events_by_date_.erase(i) ; 
				found = true ;
			}
		}	// END: for
	
	}	// END: if ( p.first != inot_arch_events_by_date_.end() )

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Returning: " << boolalpha << found) ;

	return found ; 
}


//
//		(PRIVATE) Notify::remove_from_inot_entries
//
//////////////////////////////////////////////////////////////////////////////////////////
bool 
		Notify::remove_from_inot_entries(const int& wd_in) 
{
	ThreadSafe::Lock lock(inot_entries_mutex_) ;
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "wd: " << wd_in) ;
				
	map <int, Entry_feat_by_userkey>::iterator i = inot_entries_.find(wd_in) ;	

	bool found = (i != inot_entries_.end()) ;
	
	if (found)
	{
		inot_entries_.erase(i) ; 
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Returning: " << boolalpha << found) ;

	return found ; 
}


//
//		(PRIVATE) Notify::dump_entry_map
//
//////////////////////////////////////////////////////////////////////////////////////////
void	Notify::dump_entry_map() const
{
	ThreadSafe::Lock lock(inot_entries_mutex_) ;
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_VLO_VERB) ;
	
	for ( map<int, Entry_feat_by_userkey>::const_iterator i=inot_entries_.begin() ; i != inot_entries_.end() ; i++)
	{
		ACS_LOG_DEBUG("DUMPING KEY. wd: " << i->first) ;
		dump_entry_feat_by_userkey(i->second) ; 
	}
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Method finished") ;

	return ; 
}


//
//		(PRIVATE) Notify::dump_archived_entries_maps
//
//////////////////////////////////////////////////////////////////////////////////////////
void	Notify::dump_archived_entries_maps() const
{
	ThreadSafe::Lock lock(inot_entries_mutex_) ;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "****  Method called  ***********************************************************") ;
	
	ACS_LOG_DEBUG("Dumping map: inot_arch_events_by_cookie_") ;
	for ( map<uint32_t, Extended_inotify_event>::const_iterator i=inot_arch_events_by_cookie_.begin() ; i != inot_arch_events_by_cookie_.end() ; i++)
	{
		ACS_LOG_DEBUG("\tkey: " << i->first << "n\tvalue: ") ;
		
		const vector <string> userkeys = get_userkeys_from_wd( (i->second).ionot_event.wd ) ;

		for (vector <string>::const_iterator k=userkeys.begin(); k!=userkeys.end(); k++)
		{
			const string uk = *k ; 
			Watch_Event ev ; ev.build(uk, (i->second), inot_entries_) ; 
			ACS_LOG_DEBUG("Dumping Watch Event for userkey: \"" << uk << "\"") ; 
			ev.dump() ;
		}				
	}
	
	ACS_LOG_DEBUG("Dumping map: inot_arch_events_by_date_") ;
	for ( multimap<uint64_t, uint32_t>::const_iterator i=inot_arch_events_by_date_.begin() ; i != inot_arch_events_by_date_.end() ; i++)
	{
		ACS_LOG_DEBUG("\tkey: " << i->first << "n\tvalue: ") ;
	}
	
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "****  Method finished  ***********************************************************") ;

	return ; 
}


//
//		(STATIC) Notify::get_key_from_curtime
//
//////////////////////////////////////////////////////////////////////////////////////////
uint64_t
			Notify::get_key_from_curtime()
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_MID_VERB) ;

	struct timeval t ; // PRQA S 4102
				   
   	if (0 != gettimeofday(&t, 0) ) {
		ACS_THROW(NotifyException("gettimeofday bad return value", errno)); // PRQA S 3081
    }
	uint64_t ret = ( (uint64_t(t.tv_sec) * 1000000) + uint64_t(t.tv_usec) ) ; // SSSSSSSSSSUUUUUU (s=second, u=microsecs) // PRQA S 3081, 3084, 4400
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Returning: " << ret) ;

	return ret ; 
}

//
//		(STATIC) Notify::dump_entry_feat_by_userkey
//
//////////////////////////////////////////////////////////////////////////////////////////
void	Notify::dump_entry_feat_by_userkey(const Entry_feat_by_userkey& e)
{
	for ( Entry_feat_by_userkey::const_iterator i=e.begin() ; i != e.end() ; i++)
	{
		ACS_LOG_DEBUG("\tDUMPING KEY. userkey: \"" << i->first << "\"") ;
		dump_entry_features(i->second) ; 
	}
	
	return ; 
}


//
//		(STATIC) Notify::dump_entry_features
//
//////////////////////////////////////////////////////////////////////////////////////////
void	Notify::dump_entry_features(const Entry_features& e)
{
	ACS_LOG_DEBUG(SimpleDebugSignature << "\t\toriginator_wd: " << e.originator_wd << "\n\tmask: 0x" << hex << e.mask << dec << "  ( " << eventmask_to_string( EventsMask(e.mask) )  // PRQA S 3013, 3081
		<< " )\n\tabs_path: \"" << e.full_path << "\"\n\trel_path: \"" << e.rel_path << "\"\n\trecursive? " << boolalpha << e.rec) ;
	
	return ; 
}


//
//		(STATIC) Notify::check_entry_exists_and_is_dir
//
//////////////////////////////////////////////////////////////////////////////////////////
/**
		Obtain this information using only one stat 
*/
bool 														
		Notify::check_entry_exists_and_is_dir(const std::string& entry) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. Entry: \"" << entry << "\"") ;

	int status = 0 ; 
	struct stat s ; // PRQA S 4102
	
	if ( (status=stat(entry.c_str(), &s)) != 0)
	{
		int err = errno;
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Unable to stat. exit code: " << status << "  errno: " << err) ;

		return false ; 
	}
	
	bool ret = S_ISDIR (s.st_mode) ;
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Entry: \"" << entry << "\"  Returning: " << boolalpha << ret) ;

	return ret ; 
}

_ACS_END_NAMESPACE

#endif // INOTIFY_NOT_SUPPORTED
