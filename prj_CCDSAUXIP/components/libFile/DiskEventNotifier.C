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
	Revision 5.9  2017/03/01 15:35:51  marfav
	Addedd support to external condition notifications
	
	Revision 5.8  2016/10/20 08:34:16  marpas
	coding best practice applied, qa warnings removed
	
	Revision 5.7  2016/09/02 13:23:24  marfav
	S1PDGS-31793
	No messages are issued if a starting notifier is requested to stop
	before completing the loop on the paths to monitor
	Flushing the pending events when stopping a DiskEventNotifier thread
	
	Revision 5.6  2015/05/25 14:55:36  marpas
	coding best practices applied
	
	Revision 5.5  2014/06/16 14:33:46  lucio.pulvirenti
	S2PDGS-764: Added flag to IF not to log exception stack in case of not existing path.
	
	Revision 5.4  2014/02/12 12:35:27  marpas
	define INOTIFY_NOT_SUPPORTED for systems where inotify is not supported
	
	Revision 5.3  2014/02/07 18:42:30  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.2  2013/10/25 13:00:11  marpas
	empty method added
	
	Revision 5.1  2013/06/21 10:31:40  marpas
	adoption of libException 5.x statndards in progress, qa rules adn compilation warning fixed
	
	Revision 5.0  2013/06/20 16:43:52  marpas
	adoption of libException 5.x in progress
	
	Revision 1.12  2013/05/17 08:49:32  lucio.pulvirenti
	addPathToWatch: specific exception from Notify class is caught to warn about not existing path.
	
	Revision 1.11  2013/05/03 10:17:10  chicas
	Bug fixed in Dtor: stop instead of exit call
	
	Revision 1.10  2013/01/17 13:18:10  chicas
	Bug fixed: added exit call in Dtor
	
	Revision 1.9  2012/12/02 11:53:08  marpas
	CompressionMode insertion operators added
	
	Revision 1.8  2012/11/29 19:31:29  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 1.7  2012/11/19 16:23:55  lucio.pulvirenti
	EventElementsList::clear method added to clear list.
	
	Revision 1.6  2012/10/24 14:27:02  lucio.pulvirenti
	Notify::unregister_userkey method invoked both in DTOR and in run method after while loop.
	AddFilePath public method now does the job just if thread has not been requested to stop.
	
	Revision 1.5  2012/10/23 10:53:07  lucio.pulvirenti
	Relative paths removed by EventElement struct.
	DiskEventNotifier CTOR accepts now a boolean parameter that if set, uses Notify class not to duplicate events on all watched
	paths: events are retrieved per process.
	addPathToWatch both private and public methods added to centralize Notify invocation to watch paths.
	Events no more separated by relative path, but all events relative to the thread (same key) are retrieved.
	
	Revision 1.4  2012/10/17 17:01:26  lucio.pulvirenti
	work in progress
	
	Revision 1.3  2012/10/12 15:55:03  lucio.pulvirenti
	EventElementsList::getData implemented.
	
	Revision 1.2  2012/10/05 16:12:37  lucio.pulvirenti
	work in progress.
	
	Revision 1.1  2012/10/04 16:24:03  lucio.pulvirenti
	First issue: work in progress.
	
	
        
*/

#ifndef INOTIFY_NOT_SUPPORTED 

#include <DiskEventNotifier.h>

#include <File.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using os::Notify ;
using os::NotifySingleton ;
using std::string ;
using std::vector ;
using std::list ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(DiskEventNotifier)
ACS_CLASS_DEFINE_DEBUG_LEVEL(DiskEventNotifier::EventElementsList)
		
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DiskEventNotifier::DiskEventNotifier(string const &pathToWatch, 
                                     bool recursive, 
                                     Notify::EventsMask m, 
                                     EventElementsList &el, 
                                     bool processUnique):
    Thread(),
    _pathsToWatch(vector<string>(1, pathToWatch)),
    _recursive(recursive),
    _registeredEvents(m),
    _eventsList(el), 
    _processUnique(processUnique)
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;
}

DiskEventNotifier::DiskEventNotifier(vector<string> const &pathsToWatch, 
                                     bool recursive, 
                                     Notify::EventsMask m, 
                                     EventElementsList &el, 
                                     bool processUnique):
    Thread(),
    _pathsToWatch(pathsToWatch),
    _recursive(recursive),
    _registeredEvents(m),
    _eventsList(el), 
    _processUnique(processUnique)
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DiskEventNotifier::~DiskEventNotifier() noexcept
{
    
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;
	
    Thread::stop();        

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Now thread is stopped.") ;
					
	if( !_notifyUniqueKey.empty() ) {
		bool emitExc = false;
		bool unregistered = NotifySingleton::instance()->unregister_userkey( _notifyUniqueKey, emitExc );
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
            if (! unregistered) {
                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "notify key \"" << _notifyUniqueKey << "\" already unregistered") ;
	        }
        ACS_CLASS_END_DEBUG
    }	
    
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiskEventNotifier::run()
{

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "method called with threadId " << ::pthread_self()) ;

	size_t ptwsize = _pathsToWatch.size() ;
	for( size_t i = 0; i < ptwsize; ++i ) 
	{
		addPathToWatch(_pathsToWatch[i], false);
	}

	while( !requestedCancel() ) 
	{

		vector<Notify::Watch_Event> watchEventsV;

		if( 0 != NotifySingleton::instance()->get_events_vector(_notifyUniqueKey, watchEventsV) ) 
		{

			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, watchEventsV.size() << " events occurred on paths to watch identified by key \"" << _notifyUniqueKey << "\"") ;

			size_t evenCnt = 0; // used for debug	
			size_t wesize = watchEventsV.size()	;
			for( size_t i = 0 ; i < wesize ; ++i) 
			{

				ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
				ACS_LOG_DEBUG(DebugSignatureThread << "Event #" << evenCnt) ;
				watchEventsV[i].dump();
				ACS_CLASS_END_DEBUG

				_eventsList.push( EventElement(watchEventsV[i]) ); 
				evenCnt++;
			}

		}
		else
		{
			// S1PDGS-31890
			// Delay the loop to avoid starvation when multiple DiskEventNotifier 
			// are concurrently executed and also to save some CPU power
			Timer::delay (10);
		}

	} // while( !requestedCancel() )

	NotifySingleton::instance()->unregister_userkey( _notifyUniqueKey );
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Notify key \"" << _notifyUniqueKey << "\" unregistered") ;

}

void DiskEventNotifier::addPathToWatch(std::string const & path)
{
	addPathToWatch(path, true);			
}

//
// private methods
//

std::string DiskEventNotifier::getThreadKey()
{
	Lock guard(_keyMutex);
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "method called.") ;
			
	if( _notifyUniqueKey.empty() ) {
		_notifyUniqueKey = Notify::generate_unique_userkey( threadId() ) ;
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "unique key set: \"" << _notifyUniqueKey << "\"") ;
	}
			
	return _notifyUniqueKey;
}


void DiskEventNotifier::addPathToWatch(std::string const & path, bool insertIntoVector) 
{
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "method called.") ;

	if (!running())
	{
		// Should it throw the exception here?
		ACS_LOG_TRYCATCHNOTIFY_EX ( DiskEventNotifierException () << "Will not watch to path \"" << path << "\" since DiskEventNotifier thread is not running" ); 
		return;
	}
			
	if( requestedCancel() ) {
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Path \"" << path << "\" not added to watch list since thread requested to stop") ;
		return;
	}
			
	getThreadKey() ; // When this method returns, _notifyUniqueKey is certainly set !

	try {
		int wd = 0  ;
		bool firstWatch = false;
		if( _recursive ) {
			if( _processUnique ) {
				firstWatch = NotifySingleton::instance()->watch_directory_rec_unique(wd, _notifyUniqueKey, File::getAbsolutePath(path), "", _registeredEvents);
			}
            else {
				firstWatch = NotifySingleton::instance()->watch_directory_rec(wd, _notifyUniqueKey, File::getAbsolutePath(path), "", _registeredEvents);
            }
		}
		else {
			if( _processUnique ) {
				firstWatch = NotifySingleton::instance()->watch_directory_unique(wd, _notifyUniqueKey, File::getAbsolutePath(path), "", _registeredEvents) ;
            }
			else {
				firstWatch = NotifySingleton::instance()->watch_directory(wd, _notifyUniqueKey, File::getAbsolutePath(path), "", _registeredEvents) ;
            }
		}

		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Path to watch \"" << path << "\" with key \"" << _notifyUniqueKey << "\" (" << (firstWatch ? "never" : "already") 
				<< " watched [id=" << wd << "], process unique: " << std::boolalpha << _processUnique << ")" ) ;

		if( insertIntoVector ) {
			_pathsToWatch.push_back(path);
        }
	}
	catch( const Notify::NoSuchFileOrDirectoryException &e ) {
		ACS_LOG_ERROR('\"' << path << "\" path does not exists");
		if( _logExcStack ) {
			ACS_LOG_NOTIFY_EX(e);
		}
	}
// Other exceptions are thrown outside
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


DiskEventNotifier::EventElement::EventElement(Notify::Watch_Event const & we) :
								src_absolute_path(we.src_absolute_path),
								dst_absolute_path(we.dst_absolute_path), 
								event(we.event),
								dst_is_set(we.dst_is_set)
{}



void DiskEventNotifier::EventElement::dump() const
{
    
	string s;
	if (0 != (event & Notify::NOTIFY_ISDIR)) { s += "Directory" ; }
    else { s += "Entry" ; }

	s += ( " \"" + src_absolute_path + "\" " );

	string e ;

	if ((event & Notify::NOTIFY_ALL) == Notify::NOTIFY_ALL) {
		e += "has all flags set !" ;
	} else {
		if (0 != (event & Notify::NOTIFY_ACCESS)) { e += "accessed" ; }
		if (0 != (event & Notify::NOTIFY_MODIFY)) { e += "modified" ; }
		if (0 != (event & Notify::NOTIFY_ATTRIB)) { e += "metadata changed" ; }
		if ((event & Notify::NOTIFY_CLOSE) == Notify::NOTIFY_CLOSE) { e += "closed (mode ro)" ; }
        else {
			if (0 != (event & Notify::NOTIFY_CLOSE_WRITE)) { e += "closed (mode rw)" ; }
			if (0 != (event & Notify::NOTIFY_CLOSE_NOWRITE)) { e += "closed (mode ro)" ; }
		}
		if (0 != (event & Notify::NOTIFY_OPEN)) { e += "opened" ; }

		if ((event & Notify::NOTIFY_MOVE) == Notify::NOTIFY_MOVE) { e += "moved to:" ; }
        else {
			if (0 != (event & Notify::NOTIFY_MOVED_FROM)) { e += "moved away" ; }
			if (0 != (event & Notify::NOTIFY_MOVED_TO)) { e += "moved here" ; }
		}

		if (0 != (event & Notify::NOTIFY_CREATE)) { e += "created" ; }
		if (0 != (event & Notify::NOTIFY_DELETE)) { e += "removed" ; }
		if (0 != (event & Notify::NOTIFY_DELETE_SELF)) { e += "removed (self)" ; }
		if (0 != (event & Notify::NOTIFY_MOVE_SELF)) { e += "moved (self)" ; }

	}

	if ( e.empty() ) {
		std::ostringstream o ; o << "0x" << std::hex << uint32_t(event) << std::dec ; 
		e = "  " + o.str() + "  ( " +  Notify::eventmask_to_string(event) + " )" ;
	}

	s += e ; 

	if ( dst_is_set ) {
		s += ( " \"" + dst_absolute_path + "\"" ) ;
	}
    
	ACS_LOG_DEBUG(s) ;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void DiskEventNotifier::EventElementsList::setListMaxSize(size_t maxSize)
{
	if( 0 == maxSize ) {
		ACS_THROW( exIllegalValueException("Maximum list size must be >0") ); 
    }
	
	Lock guard(_mutex);
	_listMaxSize = maxSize;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "events list max size set to " << _listMaxSize) ;
	
}

void DiskEventNotifier::EventElementsList::setNotificationCondition (Condition* c)
{
	Lock guard(_mutex);
	_externalCondition = c;
}

void DiskEventNotifier::EventElementsList::push(EventElement const & ee)
{
	
	Lock guard(_mutex);
	if( _list.size() >= _listMaxSize ) {
		
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "events list is full (" << _list.size() << " elements contained): going to remove the first element") ;
		// remove the first element
		_list.pop_front();
		
	}
		
	_list.push_back(ee);
	if (_externalCondition)	{ _externalCondition->broadcast(); }
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "events list contains " << _list.size() << " elements") ;
}

void DiskEventNotifier::EventElementsList::pop(EventElement & ee)
{
	
	Lock guard(_mutex);
    ACS_COND_THROW(_list.empty(), NoDataFound("Events list is empty") ); 
	
	ee = *_list.begin();
// remove the first element
	_list.pop_front();
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "event popped and removed: now events list contains " << _list.size() << " elements") ;
}


void DiskEventNotifier::EventElementsList::getData(list<EventElement> & wl)
{
	if( _list.empty() ) {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "events list is empty") ;
	}
	else {	
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "events list contains " << _list.size() << " elements") ;
		try {
			Timer timer ;
			timer.start();
			for(;;) {
				EventElement event;
				pop(event);
				wl.push_back(event);
				if( timer.stopElapsed() > _getDataTimeout ) {
					ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "timeout (" << _getDataTimeout << " ms) excedeed, going to exit") ;
					break;
				}
				
			}
		}
		catch(const NoDataFound &) 
		{
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "events list now is empty: returning " << wl.size() << " elements list") ;
		}
	}
	
}

size_t DiskEventNotifier::EventElementsList::getSize() const
{
	Lock guard(_mutex);
	return _list.size();
}


bool DiskEventNotifier::EventElementsList::empty() const
{
	Lock guard(_mutex);
	return _list.empty();
}


void DiskEventNotifier::EventElementsList::dump(list<EventElement> const & wl) const
{
	for( auto const &event : wl ) {	event.dump(); }
	
}


void DiskEventNotifier::EventElementsList::clear()
{
	Lock guard(_mutex);
	
	_list.clear();
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "events list cleared: now events list contains " << _list.size() << " elements") ;
}

_ACS_END_NAMESPACE

#endif // INOTIFY_NOT_SUPPORTED
