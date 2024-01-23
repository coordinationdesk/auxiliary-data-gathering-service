/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.19  2018/10/04 15:42:24  marpas
	exLogStream::creationTime added()
	
	Revision 5.18  2018/09/21 15:05:04  marpas
	coding best practice
	
	Revision 5.17  2017/07/10 11:41:04  davide.tiriticco
	compiler warnings removed
	
	Revision 5.16  2017/02/09 13:04:21  marfav
	NotifyOnly replaced with FormattedOnly
	
	Revision 5.15  2017/02/09 09:09:03  marfav
	CSGACQ-113
	Addedd support to NotifyOnly mode to avoid free text insertions in streams
	
	Revision 5.14  2016/07/04 10:11:56  marfav
	CSGACQ-84
	Added functions to set/get the append flag in exLogStream
	Added function to rotate a log and open a new one in a different path
	
	Revision 5.13  2016/06/13 12:52:29  marpas
	exSyslog class allow to change facility from LOG_LOCAL0 (default) to any allowed values (see syslog(3) )for this a new method has been added: setFacility
	The private method open was never called: removed
	
	Revision 5.12  2014/06/27 09:14:23  marpas
	qa warning fixed
	
	Revision 5.11  2014/02/07 18:03:07  marpas
	introducing ACS_THROW and ACS_COND_THROW macros to substitute throwException and throwConditionedException
	documentation improved and tests aligned
	
	Revision 5.10  2013/11/19 17:53:10  marpas
	CoutLogger concept added
	doc updated
	debug improved
	
	Revision 5.9  2013/11/15 13:19:30  marpas
	introducing the isFilterable concept into an exostream.
	It is used when inserting an exception into a Filterable: in this case the formatter will be invoked with the forFilterable bool true, indicating that probably the text should not have an header that will be presumably applied to the filterabel where the exception has been inserted
	
	Revision 5.8  2013/11/13 18:02:44  marpas
	useless explicit removed
	
	Revision 5.7  2013/10/03 10:58:10  marpas
	coding best practices applied
	
	Revision 5.6  2013/09/27 14:05:17  marpas
	streams have names
	
	Revision 5.5  2013/09/26 18:17:44  marpas
	housekeeping, types renaming and cleanup
	
	Revision 5.4  2013/09/26 18:09:35  marpas
	robustness strongly improved:
	singleton functions (with static variables inside) used
	some private members are now accessed only through the above technique
	exRTDebuggable classes do not longer unregster themselves from exRTDebug.
	it was a logic error: sooner or later the container class (exRTDebug) will throw away memory in any case.
	The above changes will lead in a more efficient process closure.
	moreover the exException class now uses the reentrant method strerror_t to obtain the errno explaination.
	
	Revision 5.3  2013/09/26 11:03:28  marpas
	exostream_Init is friend of exostream
	
	Revision 5.2  2013/06/05 15:57:11  marpas
	new real time debugging macros adopted
	
	Revision 5.1  2013/06/04 17:23:46  marpas
	introducing ne RTDebuggable template
	
	Revision 5.0  2013/06/04 15:35:03  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.19  2013/03/11 16:27:08  lucio.pulvirenti
	HexDumper copy ctor provided
	
	Revision 4.18  2013/03/07 12:13:42  marpas
	qa warnings
	
	Revision 4.17  2013/03/07 12:03:32  marpas
	HexDumper helper class added
	
	Revision 4.16  2012/12/20 13:25:45  marpas
	coding best practices
	explain used instead of what()
	tests improved
	
	Revision 4.15  2012/12/02 15:28:25  marpas
	qa warning
	
	Revision 4.14  2012/11/29 12:47:36  marpas
	qa warning removed
	
	Revision 4.13  2012/11/15 11:00:55  marpas
	qa rules enforced
	
	Revision 4.12  2012/11/14 18:15:55  marpas
	qa rules enforced
	
	Revision 4.11  2012/10/29 14:29:52  marpas
	robustness improved
	
	Revision 4.10  2012/06/25 17:42:06  marpas
	qacpp instrumentation
	
	Revision 4.9  2012/06/14 17:16:41  marpas
	qacpp instrumentation
	
	Revision 4.8  2012/06/12 17:37:01  marpas
	qacpp instrumentation
	
	Revision 4.7  2012/06/11 15:25:53  marpas
	qa fixing in progress
	
	Revision 4.6  2012/06/06 17:12:21  marpas
	qa rules application in progress
	
	Revision 4.5  2012/06/05 17:07:00  marpas
	resolving some quality issues and documentation
	
	Revision 4.4  2012/05/30 16:20:51  marpas
	S3PDGS-741 - exception's format depends on exMsgFmt
	
	Revision 4.3  2012/04/23 14:01:44  marpas
	exSyslog class added
	
	Revision 4.2  2012/03/19 15:49:25  marpas
	improving debug and getting rid of std::e_n_d_l()
	
	Revision 4.1  2012/02/14 09:03:32  marpas
	operator<< overloaded for bool
	
	Revision 4.0  2011/10/18 11:22:00  marpas
	new statistics
	
	Revision 3.5  2011/09/29 11:32:52  marpas
	signature slightly changed
	
	Revision 3.4  2011/03/18 14:07:08  marpas
	interface and multithreading robustness improved
	
	Revision 3.3  2010/11/04 10:14:54  micmaz
	specified the namespace for standard exceptions.
	
	Revision 3.2  2010/09/15 10:20:52  marpas
	exostream::notifyStackFromEx  changed into exostream::allowStackTrace
	
	Revision 3.1  2009/10/08 12:38:17  marpas
	exLogStringStream class added
	
	Revision 3.0  2009/10/08 10:48:10  marpas
	Statistics class added
	rules application improved
	
	Revision 2.8  2009/06/26 10:41:55  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.7  2009/03/11 17:09:03  marpas
	c++rules violation removed
	
	Revision 2.6  2008/06/20 16:02:32  marpas
	exostream deletion check added
	
	Revision 2.5  2008/05/06 16:15:23  marpas
	locking during the whole exLogStream ctor was needed (APF-218)
	
	Revision 2.4  2008/02/15 16:58:54  marpas
	each stream can have its own formatter setFormatter method added
	exception::what() has no longer the stack if enabled
	exception::stack() returns the stack (if enabled)
	exception stack are no longer notified if present unless the stream is instructed to do that via notifyStackFromEx method
	Filterables now hase details, they can be set via exFilterable::detail(...) method
	Filterables details are notified by the stream having the appropriate flag set via notifyDetailFromFilterable method
	exception inserted into a Filterable will fill its detail with their stack (if any)
	
	Revision 2.3  2007/07/20 09:07:32  marpas
	logfile now supports permission setting via setPermission method
	
	Revision 2.2  2006/11/03 01:05:34  marpas
	changes for moving window's messages tree
	
	Revision 2.1  2006/02/28 08:24:37  marpas
	new release, exException inherits from std::exception
	
	Revision 1.29  2005/03/18 10:57:43  marpas
	loop check implemented, an exception is raised attempting to create stream loops.
	redirect(int) implemented
	
	Revision 1.28  2005/03/16 14:39:48  marpas
	bridging streams is now allowed
	
	Revision 1.27  2005/03/16 09:04:18  marpas
	exmcf changed into excmcf
	
	Revision 1.26  2005/03/16 08:46:52  marpas
	exmcf exostream object created.
	
	Revision 1.25  2005/03/16 08:43:11  marpas
	*** empty log message ***
	
	Revision 1.24  2005/03/01 18:17:32  marpas
	long double support added
	
	Revision 1.23  2004/09/20 15:26:06  marpas
	exBaseStream class introduced, is inside the inheritance tree between exLogStream and exStream
	
	Revision 1.22  2004/05/11 14:03:55  marpas
	checkFd changed
	
	Revision 1.21  2004/04/26 15:03:54  paoscu
	_threadSafe now is public.
	
	Revision 1.20  2004/04/14 11:06:13  marpas
	filter installation improved
	
	Revision 1.19  2004/04/14 09:05:09  marpas
	many changes
	
	Revision 1.18  2004/04/06 10:35:51  marpas
	cleanup
	
	Revision 1.17  2004/04/05 14:39:04  marpas
	unitBuffered & isUnitBuffered methods added
	
	Revision 1.16  2004/04/02 15:54:44  marpas
	Legacy code compilation fixed
	
	Revision 1.15  2004/04/02 14:15:14  marpas
	ThreadSafe class introduced and used inside
	
	Revision 1.14  2003/07/21 09:21:57  marpas
	mutex is unique for all chains
	
	Revision 1.13  2003/07/10 14:42:38  marpas
	LogStream::size method added
	
	Revision 1.12  2003/06/24 08:25:12  marpas
	long long types supported
	
	Revision 1.11  2003/04/30 09:57:38  marpas
	using namespace std was removed from includes
	
	Revision 1.10  2003/04/29 09:33:20  marpas
	new handler installed automatically
	
	Revision 1.9  2003/04/15 17:12:09  marpas
	GCC3.2
	
	Revision 1.8  2003/03/12 15:17:21  marpas
	exostream::TLock now is public.
	
	Revision 1.7  2002/12/06 16:11:44  marpas
	Manipulators fix for gcc 2.96
	
	Revision 1.6  2002/11/29 14:13:10  marpas
	notifyException now returns true and throws nothing.
	Now its even better
	
	Revision 1.5  2002/11/28 18:55:22  marpas
	now is better (look by yourself)
	
	Revision 1.4  2002/11/22 13:59:26  marpas
	Thread-Safe implementation
	
	Revision 1.3  2002/11/21 17:57:03  marpas
	- exostream modified for exFilterableManagement
	- exFEvent type added
	- exMsgFmtFactory added to provide tag and other values to the exFMessage
	class
	
	Revision 1.2  2002/10/03 15:05:38  marpas
	exFilter and exFilterable added
	
	Revision 1.1.1.1  2002/09/30 19:36:15  marpas
	Import libException
	

*/

#ifndef _exStream_H_
#define _exStream_H_ 

#include <acs_c++config.hpp>
#include <ThreadSafe.h>
#include <exMacros.h>
#include <exFilterable.h>

#include <string>
#include <vector>

#include <iostream>
#include <iomanip>
#include <fstream>

#include <sstream>

#include <sys/types.h> // PRQA S 1013

// PRQA S 1020, 1023, 1030 L3
#define CERR_LOG_DEBUG(message) { std::cerr << message << std::dec << '\n' ; } 
#define CERR_CLASS_WRITE_DEBUG(v,m)	\
ACS_CLASS_BGN_DEBUG(v) \
	CERR_LOG_DEBUG(SimpleDebugSignature << m ) ;\
ACS_CLASS_END_DEBUG
// PRQA L:L3

_ACS_BEGIN_NAMESPACE(acs)

class exException ;

class exFilterable ;
class exFilter ;
class exMsgFmt ;

class exostream ; // fwd

/*! \class exostream
	
	\brief base class for streams

	It supports streams chaining. The concept means that the parent stream will
	forward insertions and other operations to the children. Acting on a child 
	explicitely will not propagate to the parent or its siblings.
	Despite of nature children can have multiple parents.
*/
class exostream // PRQA S 2108,2109,2195
{
    friend class exostream_Init ; // PRQA S 2107
public:
	enum Mode { FormattedOnly, Verbose, Silent } ; // NOSONAR
    enum StreamFdEquiv {Cmcf = 0, StdCout = 1, StdCerr = 2, NullStream = -1 } ;  // NOSONAR
	
public: // NOSONAR
	exostream() = delete ;
	exostream(const exostream&) = delete ;
	exostream &operator=(const exostream&) = delete ;

	/*! Constructor.
	    Legal fd values are 0, 1 or 2 
	    1 binds the stream to cout
	    2 binds the stream to cerr
		0 binds the stream to a stream redirected on /dev/null
	    different values will raise an exception.
		It is designed only to chain leader (excerr and excout) so, do not use the numbers.
	*/
	explicit exostream(const std::string& name, StreamFdEquiv fd = StdCerr) ; 
	
	/*! This constructor append to the given stream the constructing one.
		The existing stream will be the parent of this and will forward 
		insertions and other operations to the children
	    Legal fd values are 0, 1 or 2 
	    1 binds the stream to cout
	    2 binds the stream to cerr
		0 binds the stream to a stream redirected on /dev/null
	    different values will raise an exception.
		It is designed only to chain leader (excerr and excout) so, do not use the numbers.
		Calling this constructor will register a reference to its parent.
	*/
	exostream(const std::string& name, exostream *,StreamFdEquiv fd = StdCerr) ; 
	
	/*! This constructor append to the given streams the constructing one.
		The existing vector of streams will be the parents of this and will forward 
		insertions and other operations to the children
	    Legal fd values are 0, 1 or 2 
	    1 binds the stream to cout
	    2 binds the stream to cerr
		0 binds the stream to a stream redirected on /dev/null
	    different values will raise an exception.
		It is designed only to chain leader (excerr and excout) so, do not use the numbers.
		Calling this constructor will register a reference to each of its parents.
	*/
	exostream(const std::string& name, const std::vector<exostream *> &, StreamFdEquiv fd = StdCerr) ; 
	
	/*! Destructor. If it is a child of someone, the destructor will remove
		references from its parents.
	*/
	virtual ~exostream() noexcept ; // NOSONAR

    template<typename T> 
    inline
    exostream &inserter(const T &t) noexcept
    {
        Lock const lock(globalMutex()) ;
        // am i destroying ?
        if (isDestroying()) {  return *this ; /* give up */  }


        if (Verbose == _mode) {
            try {
                stream() << t ;
            }
            catch(const std::exception &e) { // NOSONAR -- any exception
                std::cerr << explain(e) ; // NOSONAR - can't use streams here
            }      
        }

        for (auto i : _connected) { *i << t ; }

        return *this ;
    }


	//! Flushes the stream and invoke flush on all its children if any.
	exostream & flush() ; // PRQA S 2502
	
	//! Notifies the exception to itself and all its children if any.
	exostream &notify(const std::exception &) noexcept ;
	
	//! Notifies the filterable to itself and all its children if any.
	exostream &notify(exFilterable &) ;

	//! From now on the stream is silent, but continues to notify messages to the children.
	void silent() ;
	
	//! From now on the stream is no longer silent and continues to notify messages to the children.
	void verbose() ;

	//! From now on the stream will notify only exceptions or filterables, children will be notified on anything
	void formattedOnly() ;
	
	//! From now on the stream will use the given formatter NOTE: the formatter MUST exists until the end of the process
	void setFormatter(exMsgFmt *) ;
	
	//! If stack trace is enabled, the stream will notify it along with the exception what()
	void allowStackTrace(bool) ;

	//! If a filterable has details they will be notified too
	void notifyDetailFromFilterable(bool) ;
	
	//! returns the current stream mode
	//
	// \see Mode
	Mode mode() const ;
	
	void unitBuffered(bool) ;
	
	bool isUnitBuffered() ;
	
	//! append to the filter chain the given one
	void installFilter(exFilter *) ;
	
	//! append to the filter chain the given one
	void installFilters(const std::vector<exFilter *> &) ;
	
	//! removes from the filter chain the given one
	void uninstallFilter(exFilter *) ;
	
	/*! returns if the given filterable would be filtered in (true)
		or out (false)
	*/
	bool wouldFilter(exFilterable &) ;
	
	/*! to be used to get the filter list in order to reassign them from a
	    destroying filter to a new one. 
	*/
	void installedFilters(std::vector <exFilter *> &) const ;

	/*! to be used to append a child to notify on to an existing stream.
	*/
	void appendChild(exostream *) ;
	
	/*! to be used to remove a child to notify on from an existing stream.
	*/
	void removeChild(exostream *) ;
	
	/*! used internally to check if the given stream is a descendant
	    if a is a parent of b then b.isDescendant(a) is true
	*/
	bool isDescendant(const exostream *s) const ;
	
	
	/*! redirects insertion in cerr, cout, /dev/null respectively using 1,2 or 0
		returns old value 
	*/
	StreamFdEquiv redirect(StreamFdEquiv) ;
	
    const std::string &getName() const noexcept { return name_ ; }
    
    bool isFilterable() const noexcept { return isFilterable_ ; }
    
protected:
    void setIsFilterable(bool v) noexcept { isFilterable_ = v ; }
	/*! Used from derived classes to provide their own stream.
		 When the provided class should be destroyed, or in any other part of 
		 their life, if the old provided stream should be invalid, 
		 be sure to call this method with the null pointer first. 
	*/
	void set_stream(std::ostream *) noexcept ;

	/*! called at the beginning of a flush operation. 
	    To be reimplemented in derived classes.
		The function is called before children notification
		
		\see postflush
	*/
	virtual void preflush() { /* implement in derived if needed */ }

	/*! called at the end of a flush operation. 
	    To be reimplemented in derived classes.
		The function is called after children notification

		\see preflush
	*/
	virtual void postflush() { /* implement in derived if needed */ }
	
	/*! Receives an exception when it will be notified trough the 
		exostream::notify(const exException &) or the exostream::operator<<(const exException &) methods.
		After that, the exception is passed to the children if any. In case of failure in notifying the exception, 
		the method cannot raise an exception itself, but it must return false.
		Returning true means the work has been correctly done.
	*/
	virtual bool notifyException(const std::exception &) noexcept ;
	
	/*! Receives a filterable when it will be notified trough the 
		exostream::notify(exFilterable &) or the exostream::operator<<(exFilterable &) methods.
		After that, the filterable is passed to the children if any.
	*/
	virtual void notifyFilterable(exFilterable &) ;
	
	/*! The method applies filters (eventually installed on the stream)
		to the object in input.
		The first filter returning false will interrupt the cheain returning false.
		Returns ture only if every filter will not filter out the filterable. 
	*/
	virtual bool applyFilter(exFilterable &) ;

	/*! The method is used in conjunction with the exFilterable::defaultNotify.
		 in case the above mentioned method should return false, the filterable is handled according 
		 the value returned from this method. If it is true, then the filterable having defaultNotify set to false 
		 will be ignored, else it will handled
	 */
	virtual bool defaultNotify() const ;
	
	void destroying() noexcept ;

	void removeFromParents() ;

	bool isDestroying() const noexcept ;

private:

	void appendToParents() ;
	void checkFd() const ;
	void setDestroying() noexcept ;
	/*! Returns the output stream. 
	*/
	virtual std::ostream &stream() ;
	

public:
    //* public member to lock stream tree
	static Mutex &globalMutex() noexcept ;  
	
	// note the name is to be similar to cout and cerr ...
	static std::ofstream cmcf ;      // PRQA S 2100

    /*!
     * \brief the default cerr logger
     *
     * The method returns the default logger if set, else returns excerr.
     */
    static exostream &getDefaultErrLogger() ;

    /*!
     * \brief sets the default cerr logger
     *
     * The method sets the new logger and returns the old one.
     * It is responsibility of the caller do not destroy the given logger before setting a new one.
     */
    static exostream *setDefaultErrLogger(exostream *) noexcept ;
    
    /*!
     * \brief the default logger for macros
     *
     * The method returns the default logger if set, else returns excerr.
     */
    static exostream &getDefaultOutLogger() noexcept ;
    /*!
     * \brief sets the default logger for macros
     *
     * The method sets the new logger and returns the old one.
     * It is responsibility of the caller do not destroy the given logger before setting a new one.
     */
    static exostream *setDefaultOutLogger(exostream *) noexcept ;

    /*!
     * \brief the default cout logger
     *
     * The method returns the default logger if set, else returns excout.
     */
    static exostream &getDefaultCoutLogger() noexcept ;
    /*!
     * \brief sets the default logger for Info Debug and Warning
     *
     * The method sets the new logger and returns the old one.
     * It is responsibility of the caller do not destroy the given logger before setting a new one.
     */
    static exostream *setDefaultCoutLogger(exostream *) noexcept ;
    
private:
	Mode _mode = Verbose ;
	std::vector<exostream *> _connected = {} ;
	std::vector<exostream *> _parents = {} ;
	StreamFdEquiv _sysFd ;
	std::vector <exFilter *> _filters = {} ;
	exMsgFmt *_formatter = nullptr ;
	bool _allowStackTrace = false ;	
	bool _notifyDetailFromFilterable = false ;
	bool _destroying = false ;
	std::ostream *_stream = nullptr ;
	std::unique_ptr<std::ofstream> _nullStream = nullptr ;
    std::string name_ ;
    bool isFilterable_ = false ;
    static exostream *_defaultErrLogger ; ///< the default cerr 
    static exostream *_defaultOutLogger ; ///< the default logger for macros
    static exostream *_defaultCoutLogger ; ///< the default cout logger
    ACS_CLASS_DECLARE_DEBUG_LEVEL(exostream) 
} ;

// inserters
exostream & operator<<(exostream &, exFilterable &) ;
exostream & operator<<(exostream &, const std::exception &) noexcept ;
exostream & operator<<(exostream &, const std::string &) ;
exostream & operator<<(exostream &, const char *) ;

exostream & operator<<(exostream &, char) ;

exostream & operator<<(exostream &, bool) ;

exostream & operator<<(exostream &, short) ;

exostream & operator<<(exostream &, int) ;

exostream & operator<<(exostream &, long) ;

exostream & operator<<(exostream &, float) ;

exostream & operator<<(exostream &, double) ;

exostream & operator<<(exostream &, long double) ;

exostream & operator<<(exostream &, unsigned char) ;

exostream & operator<<(exostream &, unsigned short) ;

exostream & operator<<(exostream &, unsigned int) ;

exostream & operator<<(exostream &, unsigned long) ;

exostream & operator<<(exostream &, signed long long int ) ;
exostream & operator<<(exostream &, unsigned long long int ) ;

exostream & operator<<(exostream &, void *) ; // NOSONAR

exostream & operator<<(exostream &, const void *) ; // NOSONAR

exostream & operator<<(exostream &, std::streambuf *) ;

exostream & operator<<(exostream &, std::ostream & (*func)(std::ostream &)) ; // NOSONAR - stl
exostream & operator<<(exostream &, std::ios_base &(*func)(std::ios_base &)) ; // NOSONAR - stl
// linux manipulators
exostream & operator<<(exostream &, std::_Resetiosflags) ;
exostream & operator<<(exostream &, std::_Setiosflags) ;
exostream & operator<<(exostream &, std::_Setbase) ;
exostream & operator<<(exostream &, std::_Setfill<char>) ;
exostream & operator<<(exostream &, std::_Setprecision) ;
exostream & operator<<(exostream &, std::_Setw) ;

// PRQA S 2072 L1
std::ostream & operator <<(std::ostream &, exostream::Mode) noexcept ;
exostream & operator <<(exostream &, exostream::Mode) noexcept ;

std::ostream & operator <<(std::ostream &, exostream::StreamFdEquiv) noexcept ;
exostream & operator <<(exostream &, exostream::StreamFdEquiv) noexcept ;
// PRQA L:L1

/*! \class exLogStream

	\brief base class for logfiles

	This class is useful for logfiles. It can act as base class.

	\note To create a logfile:
	
	\code
	try {
		exLogStream log("filedilog.txt", argc > 1) ;

		// ...

		excerr << "It will write on cerr and on the logfile\n" ;
	
	}
	catch (std::exception &x) {
		// this will log the exception only on excerr, because the log dtor
		// is invoked by exiting the try block.
		excerr.notify(x) ; 
	}
	
	\endcode
	
*/
class exLogStream: public exostream // PRQA S 2109,2153
{
public:
	exLogStream() = delete ;
	exLogStream(const exLogStream&) = delete ;
	exLogStream &operator=(const exLogStream&) = delete ;
	
	/*! This constructor build a log stream and makes it child of excerr.
		
		\see exostream
	*/
	exLogStream(const std::string &sname, 
                const std::string &filename, 
                bool appendIfExists = false, 
                const Lock & = Lock(globalMutex())) ; // PRQA S 3081, 4636 # the temporary Lock ensure that during the ctor the global mutex is locked

	/*! This constructor build a log stream and makes it child of the given parent.
		
		\see exostream
	*/
	exLogStream(const std::string &sname, 
                const std::string &filename, 
                exostream *, 
                bool appendIfExists = false, 
                const Lock & = Lock(globalMutex())) ; // PRQA S 3081, 4636 # the temporary Lock ensure that during the ctor the global mutex is locked

	/*! This constructor build a log stream and makes it child of the given parents.
		
		\see exostream
	*/
	exLogStream(const std::string &sname, 
                const std::string &filename, 
				const std::vector<exostream *> &, 
				bool appendIfExists = false, 
				const Lock & = Lock(globalMutex())) ; // PRQA S 3081, 4636 # the temporary Lock ensure that during the ctor the global mutex is locked

	/*! Destructor.
	*/
	~exLogStream() noexcept override ; // NOSONAR
		
	virtual std::streampos size() ;
	time_t creationTime() ;
	
    /*!
    \note this method could technically speaking act even on const object, but because it changes 
    the permission of the logfile, is equivalent to change the 'state' of the object itself.
    */
	void setPermission(mode_t) ;

	void rotate(const std::string &fname, const std::string &msg="") ; // PRQA S 2502, 4634

	void rotateOnNewFile (const std::string &fname, const std::string &msg="") ; // PRQA S 2502, 4634

	void appendIfExists (bool);
	bool appendIfExists () const;

private: 
    // left open() private non-virtual
	void open() ;           // PRQA S 2502
	void close() noexcept ;  // PRQA S 2502

	std::unique_ptr<std::ofstream> _logfile = nullptr;
	std::string _fname = "" ;
	bool _append = false ;
} ;






/*! \class exBaseStream

	\brief base class for specialized streams 

	This is the base class for specialized base streams.
	All the insertion methods redirect their input to /dev/null.
*/
class exBaseStream: public exLogStream // PRQA S 2153
{
public:
	exBaseStream() = delete ;
	exBaseStream(const exBaseStream&) = delete ;
	exBaseStream &operator=(const exBaseStream&) = delete ;
	
	/*! This constructor build an handler and makes it child of excerr.
		
		\see exLogStream
	*/
	explicit exBaseStream(const std::string &n) ;
	
	/*! This constructor build an handler and makes it child of the given parent.
		
		\see exLogStream
	*/
	exBaseStream(const std::string &n, exostream *) ;

	/*! This constructor build an handler and makes it child of the given parent.
		
		\see exLogStream
	*/
	exBaseStream(const std::string &n, const std::vector<exostream *> &) ;

	/*! Destructor.
	*/
	~exBaseStream() noexcept override ; // NOSONAR
} ;


/*! \class exSyslog

	\brief class interface for syslog

	All the insertion methods redirect their input to /dev/null.
*/
class exSyslog: public exBaseStream // PRQA S 2109, 2153
{
public:
	exSyslog() = delete ;
	exSyslog(const exSyslog &) = delete ;
	exSyslog & operator=(const exSyslog &) = delete ;
	
	/*! This constructor build an handler and makes it child of excerr.
		
		\see exLogStream
	*/
	explicit exSyslog(const std::string &n) ;
	
	/*! This constructor build an handler and makes it child of the given parent.
		
		\see exLogStream
	*/
	exSyslog(const std::string &n, exostream *) ;

	/*! This constructor build an handler and makes it child of the given parent.
		
		\see exLogStream
	*/
	exSyslog(const std::string &n, const std::vector<exostream *> &) ;

	/*! Destructor.
	*/
	~exSyslog() noexcept override ;
    void setFacility(int f) noexcept { facility_ = f ; }
protected:
	void notifyFilterable(exFilterable &) override ;
	
private:
    int facility_ ; 
    ACS_CLASS_DECLARE_DEBUG_LEVEL(exSyslog) 
} ;





/*! \class exLogStringStream

	\brief class for logfiles into a stringstream

	\note To create a logstream:
	
	\code
	try {
		exLogStringStream log() ;

		// ...

		excerr << "It will write on cerr and on the logfile\n" ;
	
	}
	catch (std::exception &x) {
		// this will log the exception only on excerr, because the log dtor
		// is invoked by exiting the try block.
		excerr.notify(x) ; 
	}
	
	\endcode
	
*/
class exLogStringStream: public exBaseStream // PRQA S 2109, 2153
{
public:
	exLogStringStream() = delete ;
	exLogStringStream(const exLogStringStream&) = delete ;
	exLogStringStream &operator=(const exLogStringStream&) = delete ;

	/*! This constructor build a log stringstream and makes it child of excerr.
		
		\see exostream
	*/
	explicit exLogStringStream(const std::string &n) ;

	/*! This constructor build a log stringstream and makes it child of the given parent.
		
		\see exostream
	*/
	exLogStringStream(const std::string &n, exostream *) ;

	/*! This constructor build a log stringstream and makes it child of the given parents.
		
		\see exostream
	*/
	exLogStringStream(const std::string &n, const std::vector<exostream *> &) ;

	/*! Destructor.
	*/
	~exLogStringStream() noexcept override ;
	
	std::streampos size() override ;

	const std::ostringstream &str() const ;

private:
	std::ostringstream _stream ;
} ;







/*! \class exHandler

	\brief base class for specialized streams with exception handling support

	This is the base class for specialized exception handlers. 
	Only exceptions will have a special treatment.
	To use this, simply inherit from this and reimplement the notifyException method
*/
class exHandler: public exBaseStream // PRQA S 2153
{
public:
	exHandler() = delete ;
	exHandler(const exHandler&) = delete ;
	exHandler &operator=(const exHandler&) = delete ;

	/*! This constructor build an handler and makes it child of excerr.
		
		\see exBaseStream
	*/
	explicit exHandler(const std::string &n) ;
	
	/*! This constructor build an handler and makes it child of the given parent.
		
		\see exBaseStream
	*/
	exHandler(const std::string &n, exostream *) ;

	/*! This constructor build an handler and makes it child of the given parent.
		
		\see exBaseStream
	*/
	exHandler(const std::string &n, const std::vector<exostream *> & ) ;

	/*! Destructor.
	*/
	~exHandler() noexcept override ;
	
	
protected:
	/*! This method is the key one of the class. Reimplement it and each time an 
		exception is notified to the object or one of its ancestors the method is triggered 
		with the reference of the exception being notified.
	*/
	bool notifyException(const std::exception &) noexcept override ;

} ;



/*! \var excerr

	The default stream bound to cerr. It can be used as a standard ostream.
	In addition you can also use all the methods in exostream.
	It is also the parent for exLogStream and exHandler objects when no 
	specific parent was given to their constructors.
*/
extern exostream excerr ; // NOSONAR

/*! \var excout

	The default stream bound to cout. It can be used as a standard ostream.
	In addition you can also use all the methods in exostream.
	It can also be used as parent for exLogStream and exHandler objects but 
	it must explicitely passed as parent constructors as in
	exLogStream("logfile",&excout) ;
*/
extern exostream excout ; // NOSONAR


/*! \var excmcf

	The default stream bound to /dev/null. It can be used as a standard ostream.
	By default it sends all messages into /dev/null and its purpose is to be used 
	as root for a series of stream not linked to default one e.g. MCF
	In addition you can also use all the methods in exostream.
	It can also be used as parent for exLogStream and exHandler objects but 
	it must explicitely passed as parent constructors as in
	exLogStream("logfile",&excmcf) ;
*/
extern exostream excmcf ; // NOSONAR

/*! \brief helper class to dump buffers in hex */
class HexDumper // PRQA S 2109
{
public:
    HexDumper(void const * const buffer, size_t length) : buffer_(buffer), length_(length) // NOSONAR
    {
        ACS_COND_THROW((length_ != 0) && (0 == buffer_),
                                  exIllegalValueException("Invalid input buffer pointer")) ;
    }
    HexDumper(const HexDumper &rhs) = default ;
    HexDumper() = delete ;
    HexDumper& operator= (const HexDumper &) = delete ;

    ~HexDumper() = default ;

    template <typename Stream_>
    void dump(Stream_ &s) const 
    {
        const auto b = reinterpret_cast<unsigned char const *>(buffer_) ; // NOSONAR
        s << std::hex ;
        for (size_t i=0; i<length_; ++i)
	    {
		    s << std::setw(2) << std::setfill('0') << int(b[i]) ; 
	    }
        s << std::dec ;
    }
    
	
private:
    void const * const buffer_ ;
    size_t length_ ;
} ;

/*! \brief HexDumper inserter operator 
 *
 *  \relates HexDumper 
 */
template <typename Stream_>
inline
Stream_ &operator<<(Stream_ &s, const HexDumper &d) { d.dump(s) ; return s ; } // PRQA S 2072, 2134, 4028, 4222


inline
exostream &operator<<(exostream &s, const HexDumper &d) { d.dump(s) ; return s ; } // PRQA S 2072, 2134, 4028, 4222






/*! \brief convenience macro 
	
	NB is a MACRO !!! because we need __FILE__ and __LINE__ to be evaluated in the code using this macro
	The macro allows to write code like this:

	\code

	// ...
	
	tryCatchAndNotify(FileOpenException(string("Cannot open file: ") + filename, excout) ;
	
	// ...
	
	\endcode
	
	will result in:
	
	
	\code

	// ...
	
	tryCatchAndNotify(FileOpenException(string("Cannot open file: ") + filename, excout) ;
	
	// ...
	
	\endcode
		
*/
// PRQA S 1020,1023,1030 L2
#define tryCatchAndNotify(t,o) \
	try { \
		ACS_THROW(t) ; \
	} \
	catch (std::exception& x) { \
		o.notify(x) ; \
	}
// PRQA L:L2


/*! \class exostream_Init
	
	\brief static exostream initializer

	This class should not be used directly. Its only purpose is to initialize
	the excerr, excout & excmcf objects before the others. Only the first instance initializes 
	them. All the subsequent invocation will be simply ignored.
    The static declaration of this is to allow initialization of the symbols above.
    This tecnique is the same used to declare std::cerr, std::cout, std::cin and std::clog 
    as in iostream system include file.
*/
class exostream_Init // PRQA S 2109
{
public:
	/*! stream initialization, excerr, excout, excmcf will be initialized after the
		first constructor call
	*/
	exostream_Init() ;
	
	/*! class dtor */
	~exostream_Init() noexcept; // NOSONAR

	exostream_Init(const exostream_Init &) = delete ;
	exostream_Init &operator=(const exostream_Init &) = delete ;
	
private:
	static int _initcount ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(exostream_Init) 
	
} ;
static const exostream_Init exostreamInit ;
	
_ACS_END_NAMESPACE


#endif // _exStream_H_
