// PRQA S 1050 EOF
/*

    Copyright 1995-2020, Exprivia SPA
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.com

    $Prod: A.C.S. Application Library $

    $Id$

    $Author$

    $Log$
    Revision 5.25  2018/09/27 14:10:02  marpas
    compilation warning fixed

    Revision 5.24  2017/11/20 13:56:52  marpas
    no default args inside friend declarations

    Revision 5.23  2017/11/20 13:48:55  marpas
    friend declaration now matches the ctor signature

    Revision 5.22  2017/09/29 09:26:00  lucio.pulvirenti
    S2PDGS-1843: confOverrideFilepath_ string private attr added.

    Revision 5.21  2017/09/12 13:20:23  marpas
    support to application instance number and its persistence across a restart (phoenix)
    getInstance() method returns the actual instance (or -1 if no instance is known)
    setInstance() method to set a value (honored only if the actual instance is not known)

    Revision 5.20  2017/06/06 07:08:18  tergem
    S2PDGS-1641: getRotateLogXXX suppressed

    Revision 5.19  2017/02/09 13:05:33  marfav
    NotifyOnly replaced with FormattedOnly

    Revision 5.18  2017/02/09 10:30:49  marfav
    CSGACQ-113
    Added support to notifyOnly modificator in log streams

    Revision 5.17  2016/07/04 13:11:02  marfav
    QAC++ warning removed on the new code

    Revision 5.16  2016/07/04 12:56:19  marfav
    CSGACQ-84
    Adding support to application defined log rotation management
    Adding support to daily rotated logs (midnight to midnight)

    Revision 5.15  2016/03/09 10:03:11  marpas
    old logs are now into an unsigned int

    Revision 5.14  2015/06/12 08:22:00  tergem
    APF-267 Separate Log implementation for filterable events:
    _mclogRotationPolicy LogRotationPolicy defined and instantiated in the setLRPolicy function
    appendToLast set and get method defined in LogRotationPolicy class
    defineMCLogName virtual protected method added to allow its reimplementation
    forceRotation method added to implement the rotation on the mc log every time the rotation is Needed
    defaultMCLogPath defined

    Revision 5.13  2015/02/19 13:29:21  marpas
    log rotation policy characteristics message is issued only if the log rot pol changes

    Revision 5.12  2014/12/17 13:16:24  marpas
    timeToRestart do not retrigger the timer - once it elapsed, it continue returning true, unless the retrigger arg is set to true when calling

    Revision 5.11  2014/09/12 13:37:28  marfav
    timeToRestart can be forced to return always false by configuration

    Revision 5.10  2014/05/13 11:24:01  marpas
    Application::configurationLoaded virtual method added, it will be called after the loadConfiguration method. To be reimplemented in derived classes to know that the configuration is read, just in case they want to read some parameters from it.
    Application::StatusNotification::reloadConf virtual method added, to allow derived classes to read parameters from configuration.

    Revision 5.9  2014/05/12 15:47:48  lucio.pulvirenti
    S2PDGS-760: StatusNotification:_notificationInterval no more const. setNotificationInterval, getNotificationInterval and getActualNotificationInterval public methods added to IF.

    Revision 5.8  2014/01/22 10:11:07  marpas
    robustness improved
    qa warning fixed

    Revision 5.7  2014/01/21 13:41:13  marfav
    setRotateLogParametersFromConf name mispell fixed

    Revision 5.6  2014/01/21 12:03:15  marfav
    ADC-224: log rotation policy read from configuration and applied in loadConfiguration

    Revision 5.5  2013/11/19 16:29:00  marpas
    doc updated

    Revision 5.4  2013/07/24 19:08:09  marpas
     Status stream inserter operator added - convenience for messages

    Revision 5.3  2013/07/16 13:36:23  marpas
    getDefaultConnectionPool is a const method from now on.

    Revision 5.2  2013/07/15 16:18:14  marpas
    qa warning fixed

    Revision 5.1  2013/07/05 12:29:55  marpas
    adopting new libException 5.x standards
    using acs::base::defaultLockPath() to define application lock path

    Revision 5.0  2013/06/06 08:37:35  marpas
    new ibException 5.0 nterfaces

    Revision 4.17  2013/04/23 13:49:19  marpas
    getActualRotationLogCompressOld, getActualRotationLogNumToSave, getActualRotationLogAgeSec, getActualRotationLogSize methods added,
    previous getDefaultLogSize, getDefaultLogAgeSec, getDefaultLogNumToSave, getDefaultLogCompressOld moved to getDefaultRotationXXX

    Revision 4.16  2013/04/23 11:10:26  marpas
    new methods added: getDefaultLogSize(), getDefaultLogAgeSec(), getDefaultLogNumToSave(), getDefaultLogCompressOld()

    Revision 4.15  2013/04/17 20:38:54  marpas
    instance() method has now explicit exception listed

    Revision 4.14  2013/03/14 15:25:14  marpas
    seDefaultConnectionPool and getDefaultConnectionPool method substitutes setConnectionPool and getConnectionPool
    interface rationalization

    Revision 4.13  2013/03/12 17:43:33  marpas
    registerWaitingCP method and related ones implemented to manage connection pool.
    When the connection pool is set, any registered will be called with a reference of the connection pool

    Revision 4.12  2013/02/19 15:23:14  marpas
    Application::daemonizeCalled method implemented

    Revision 4.11  2013/02/19 14:32:44  marpas
    Application::AppPluginLoaderNotifier class introduced
    this imply a dependency from libPlugin (added)

    Revision 4.10  2013/02/14 09:57:20  marpas
    qa warnings removed

    Revision 4.9  2013/02/14 09:55:22  marpas
    header rationalization & coding best practices applied

    Revision 4.8  2013/02/05 18:06:41  marpas
    delayOrCondition signature changed

    Revision 4.7  2013/01/24 19:26:30  marpas
    connectionPool management improved for derived classes

    Revision 4.6  2012/11/28 17:19:24  marpas
    qa rules and robustness

    Revision 4.5  2012/03/13 16:32:20  marpas
    help fixed

    Revision 4.4  2012/03/01 15:48:09  marpas
    using acs::base for default static paths

    Revision 4.3  2012/02/28 16:20:39  marpas
    Confspaces cannot be removed

    Revision 4.2  2012/02/24 11:32:10  marpas
    delayOrCondition method added

    Revision 4.1  2012/02/14 14:17:42  marpas
    refactoring

    Revision 4.0  2011/10/18 12:43:58  marpas
    new statistics adopted

    Revision 3.13  2011/09/26 12:14:16  marpas
    isDaemon() method added,
    Notification class has virtual dtor

    Revision 3.12  2011/09/26 11:19:16  marpas
    SSF development in progress

    Revision 3.11  2011/09/22 18:26:56  marpas
    status is inherited from last status when restarting

    Revision 3.10  2011/09/22 17:31:56  marpas
    StatusNotification class improved

    Revision 3.9  2011/09/22 09:26:33  marpas
    usage method management standardized

    Revision 3.8  2011/09/21 17:39:18  marpas
    getParsedArgs and setParsedArgs method added

    Revision 3.7  2011/09/20 17:34:52  marpas
    Software Status Notification (S1PDGS-1902) in progress

    Revision 3.6  2011/09/12 17:30:26  marpas
    setStatus development in progress (i/f creation)
    default for stack trace notification changed

    Revision 3.5  2011/08/08 12:18:04  marpas
    concept added:
    repository to store and retrieve a connection pool. NB: the application is responsible of:
    1) create and set a connection pool in an appropriate moment
    2) destroy and release the connection pool in an appropriate moment
    3) notify to any using class tree the change of the connection pool itself (if needed: e.g. any thread running and potentially using it)

    Revision 3.4  2011/03/18 18:00:13  marpas
    interface changed, multithread robustness improved

    Revision 3.3  2010/09/15 10:19:13  marpas
    LogRotationPolicy has allowStackTrace(bool) method to set the stack trace into the log when notifying exceptions

    Revision 3.2  2010/05/26 10:36:11  marpas
    Application::allowDaemonize method added

    Revision 3.1  2009/10/08 12:58:50  marpas
    prelog added in order to have in the logfile all the messages issued BEFORE the log creation

    Revision 3.0  2009/10/08 10:48:56  marpas
    uses new Statistics from libException 3.x

    Revision 2.8  2009/04/03 15:39:24  marpas
    some rules fixed

    Revision 2.7  2008/11/20 09:12:09  paoscu
    Added options to log

    Revision 2.6  2008/11/14 14:26:15  marpas
    setHostnameSpaceMandatory method added

    Revision 2.5  2008/11/13 13:47:25  marpas
    Stop predicate introduced

    Revision 2.4  2008/11/13 12:39:38  marpas
    new ConfigurationSingleton approach for configuration. Convenience functions added.

    Revision 2.3  2008/01/08 14:10:04  marpas
    bool Application::LogRotationPolicy::check() public method added

    Revision 2.2  2006/12/19 19:48:17  paoscu
    executableName modified to use /proc filesystem on Linux to get the real path

    Revision 2.1  2006/03/16 18:05:24  paoscu
    Added methods getDistributionVersion and getDistributionRelease to obtain
    the version and release under which the application has been distributed.
    Current implementation performs the task
    calling an external application called ACS_Wh3r3RUFr0m

    Revision 2.0  2006/02/28 08:35:39  marpas
    Exception 2.1 I/F adopted

    Revision 1.36  2006/02/03 14:57:05  marpas
    checkMisuse and logMisuse dead methods removed
    attachFilters added

    Revision 1.35  2005/10/14 09:55:03  marpas
    debugMode method added

    Revision 1.34  2005/10/13 19:37:04  marpas
    new ApplicationDebug modes implemented - tested in libCryosatApp

    Revision 1.33  2005/10/06 19:41:32  marpas
    ApplicationDebug concept introduced, installs a SIGUSR2 handler in order to change debug values at runtime

    Revision 1.32  2005/08/05 19:31:44  marpas
    post log hooks management improved

    Revision 1.31  2005/08/05 14:18:55  marpas
    LogRotationPolicy ctors now have a parameter more to signal the application to
    append this log to the last one. It is defaulted to false in order to not change
    the current behaviour.
    Moreover, log rotation, opening and closure now put a timestamp in the message.
    This in order to better investigate when a log was written as many policy do not
    print any time in their logs.
    to see how to use the append flag, please refer to the tPhoenix test of this module

    Revision 1.30  2005/08/05 10:58:17  marpas
    post log messages implemented

    Revision 1.29  2004/09/10 09:01:03  paoscu
    all rotateLog flavours now return bool to notify the log was really rotated

    Revision 1.28  2004/07/20 17:12:36  paoscu
    rotateLog(vector<...>) is now virtual

    Revision 1.27  2004/04/14 11:12:29  paoscu
    Log rotation policy uses filters now and preserve them
    Thread safety implemented with ThreadSafe

    Revision 1.26  2004/04/06 10:16:55  paoscu
    exStream.h included

    Revision 1.25  2004/01/14 14:12:19  paoscu
    Change in documentation.

    Revision 1.24  2003/10/01 16:25:51  marpas
    Application::isRestarted method added to know if the application was automatically restarted

    Revision 1.23  2003/09/29 15:21:30  marpas
    Application::restartInterval method added.
    Application::getLRPolicy methods added.
    Application::LogRotationPolicy::fullLogPathname method added
    Application::LogRotationPolicy::mode methods added
    Application::LogRotationPolicy::aging methods added
    Application::LogRotationPolicy::size methods added
    Now is possible to ask and changes mode, aging and size to the Application's current LogRotationPolicy

    Revision 1.22  2003/09/18 10:54:16  fracar
    modified signature for 		unsigned int timeToRestart(unsigned int & restartTime);

    Revision 1.21  2003/09/17 17:37:31  fracar
    added method                 void timeToRestart(unsigned int & restartTime);

    Revision 1.20  2003/09/12 13:59:23  paoscu
    executableName method fixed.

    Revision 1.19  2003/09/12 13:57:21  paoscu
    executableName method added.

    Revision 1.18  2003/08/19 14:51:00  paoscu
    Application::parseDate static method added

    Revision 1.17  2003/07/22 20:55:01  paoscu
    parseRevision method added.
    New ApplicationLock management.

    Revision 1.16  2003/07/14 22:27:19  paoscu
    LogRotationPolicy  algorithm completed

    Revision 1.15  2003/07/14 19:22:23  paoscu
    static members for Application class separate in a different compilation
    unit in order to avoid useless dependencies

    Revision 1.14  2003/07/14 19:12:59  paoscu
    static method hostname() added

    Revision 1.13  2003/07/14 17:29:22  paoscu
    logPath removed

    Revision 1.12  2003/07/12 02:12:42  paoscu
    Log rotation policy partially implemented

    Revision 1.11  2003/07/11 12:58:26  paoscu
    restart implementation policy

    Revision 1.10  2003/05/14 09:57:31  crivig
    restartTime changed to unsigned int

    Revision 1.9  2003/05/14 08:56:03  crivig
    methods to restart a process added

    Revision 1.8  2003/04/30 16:10:45  paoscu
    using namespace std no longer into acs include

    Revision 1.7  2003/04/09 13:04:20  paoscu
    Application::Builder and Application::GenericBuilder classes added.

    Revision 1.6  2003/03/20 18:20:25  marpas
    subSystem parameter added

    Revision 1.5  2003/03/18 13:15:31  marpas
    -nodaemon supported on the command line
    debug improved
    fixed a bug between pre/post daemonize

    Revision 1.4  2003/03/12 17:15:36  marpas
    preDaemonize protected method added

    Revision 1.3  2003/02/27 14:31:52  marpas
    postDaemonize virtual protected method added

    Revision 1.2  2003/02/24 11:11:15  marpas
    rotateLog method added

    Revision 1.1.1.1  2002/11/27 10:23:36  paoscu
    Import libApp


*/

#ifndef _Application_H_
#define _Application_H_

#include <acs_c++config.hpp>
#include <ApplicationDebug.h>
#include <ApplicationUuid.h>
#include <ConfigurationSingleton.h>
#include <PluginLoader.h>
#include <exException.h>
#include <exStream.h>
#include <Timer.h>
#include <MCDefaultFilter.h>

#include <ctime>

#include <Thread.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbConnectionPool ;

class ApplicationLock ;

/**
 * Base class for applications.
 * No more than one application can exists in a program. If a second application is instantiated
 * an Application::Exception is thrown.
 */
// PRQA S 1703 L1
class Application : virtual public ThreadSafe // PRQA S 2109, 2153
{
public:
    /**
     * Application related thread that listens to signal activities and manages the
     * application StopMode state transitions to implement a standard scheme
     * for exit signals related procedures
     */

    class ApplicationStopManager : public Thread
    {
    public:
        // CTOR and DTOR
        ApplicationStopManager();
        virtual ~ApplicationStopManager() ACS_NOEXCEPT ;

        // This class cannot be copied or assigned or moved
        ApplicationStopManager(const ApplicationStopManager&) = delete;
        ApplicationStopManager& operator= (const ApplicationStopManager&) = delete;
        ApplicationStopManager(const ApplicationStopManager&&) = delete;
        ApplicationStopManager& operator= (const ApplicationStopManager&&) = delete;

        // Will manage signals and update the application status
        void managePendingSignals();

        // Thread specialization... maybe useless
        virtual void run();

    private:
        Mutex manageSignalMutex_;
        ACS_CLASS_DECLARE_DEBUG_LEVEL(ApplicationStopManager) ;
    };

public:
    /**
     * a convenience typedef for a pointer to a function that fills the given string and
     * returns void
     */
    typedef void (*PostLogMessage)(std::string &) ;

    class PostLogHooks : public ThreadSafe {                            // PRQA S 2109, 2153
    public:
        virtual ~PostLogHooks() ACS_NOEXCEPT ;
        static PostLogHooks &instance() ;

        void callHooks(std::string &) ;
        void registerHook(PostLogMessage) ;
        bool unregisterHook(PostLogMessage) ;
        size_t size() const ;
    private:
        PostLogHooks() ;
        PostLogHooks(const PostLogHooks &) ;
        PostLogHooks & operator= (const PostLogHooks &) ;
    private:
        std::vector<PostLogMessage> _hooks ;
        static PostLogHooks *_instance ;
    } ;

    class AppPluginLoaderNotifier : public PluginLoaderNotifier // PRQA S 2109, 2153
    {
    public:
        explicit AppPluginLoaderNotifier(Application *app) ;
        virtual ~AppPluginLoaderNotifier() ACS_NOEXCEPT; // PRQA S 2131

        void virtual operator()(const std::string &plugin) const ;
        bool warning() const ACS_NOEXCEPT { return warning_ ; }
    private:
        AppPluginLoaderNotifier() ; // not implemented
        AppPluginLoaderNotifier(const AppPluginLoaderNotifier &) ; // not implemented
        AppPluginLoaderNotifier& operator=(const AppPluginLoaderNotifier &) ; // not implemented
    private:
        mutable bool warning_ ;
        Application *app_ ;
    } ;

public:

    /*! this enum is used to define a type for the Stop Mode */
    enum StopMode { NoStop = 0, PhoenixRestart, CleanStop, ImmediateStop } ; // PRQA S 2502

    /*! this map is used to have the Stop Mode as string */
    static const std::map<StopMode, std::string> StopModeToString;

    /*! this enum is to be passed to setStatus method */
    enum Status { Ok = 0, Warning, Critical, Initializing, Exiting } ; // PRQA S 2502
    static std::string toString(Status) ACS_NOEXCEPT ;                       // PRQA S 2502
    class Builder                                                       // PRQA S 2109, 2502
    {
    public:
        Builder();
        virtual ~Builder() ;
        virtual void build()=0;
        static  Builder* instance();
    private:
        Builder(const Builder &) ;
        Builder &operator= (const Builder &) ;
    private:
        static Builder* _instance;
    };


    template <class T>
    class GenericBuilder : public Builder                                 // PRQA S 2109
    {
    public:
        GenericBuilder(const std::string &appName, const std::string &subSystem) :
            Builder(),
            _appName(appName) ,
            _subSystem(subSystem)
        {}

        virtual ~GenericBuilder() {}
        virtual void build() { new T(_appName,_subSystem); }

    private:
        GenericBuilder() ; // not implemented
        GenericBuilder(const GenericBuilder &) ; // not implemented
        GenericBuilder& operator= (const GenericBuilder &) ;    // not implemented
    private:
        std::string _appName;
        std::string _subSystem;
    };


    /*! class Exception declaration */
    exDECLARE_EXCEPTION(Exception,exException) ;                        // PRQA S 2131, 2502

    // declared here because frienship
    /**
     * To be called to rotate log or create it the very first time adding a set of filter to it
     * returns true if the log was rotated (even created)
     */
    virtual bool rotateLog(const std::vector<exFilter *> &) ;
    class LogRotationPolicy ; // forward declaration for next line
    void setLRPolicy(const LogRotationPolicy &) ;
    LogRotationPolicy & getLRPolicy() ;
    const LogRotationPolicy & getLRPolicy() const ;
    LogRotationPolicy & getMCLRPolicy() ;
    const LogRotationPolicy & getMCLRPolicy() const ;

protected:

    /*
     * Provide default parameters for the application log rotation policy
     * This should be overriden by the project specific application class
     */
    virtual void createDefaultLRP ();

    /**
     * Set Application name.
     * No more than one application can exists in a program. If a second application is instantiated
     * an Application::Exception is thrown.
     */
    explicit Application(const std::string &appName, const std::string &subSystem="");


    /**
     * Set MC Default Log Name
     * Specialised application can be reimplemented to set a new MC Log Name
    */
    virtual std::string defineMCLogName(const std::string& ) const ACS_NOEXCEPT ;


    /**
     * \brief Custom signal handler called in user space by an external thread when
     * a SIGINT is detected
     */
     virtual void customSigIntHandler() {}

    /**
     * \brief Custom signal handler called in user space by an external thread when
     * a SIGTERM is detected
     */
     virtual void customSigTermHandler() {}


public:

    class StatusNotification: public Thread                                 // PRQA S 2109
    {
        public:
            explicit StatusNotification(Application &, unsigned int interval = defaultNotifyInterval) ;
            virtual ~StatusNotification() ACS_NOEXCEPT ;
            virtual void run() ;
            void setNotificationInterval(unsigned int nv) { _notificationInterval = nv; }
            unsigned int getNotificationInterval() const { return _notificationInterval; }
            // returns the notification interval in seconds inspecting the configuration
            unsigned int getActualNotificationInterval() const ACS_NOEXCEPT ;
            virtual void reloadConf() {} // PRQA S 2131
        protected:
            virtual void notify() ACS_NOEXCEPT ;
             /*! to be reimplemented to force a notification.
             * \returns false
             */
            virtual bool shouldNotify() const ACS_NOEXCEPT ;
        private:
            StatusNotification() ;  // not implemented
            StatusNotification(const StatusNotification &) ;
            StatusNotification&operator =(const StatusNotification &) ;
            bool statusChanged() const ACS_NOEXCEPT ;
            void callnotify() ACS_NOEXCEPT ;
            bool timeToNotify(Timer &) const ACS_NOEXCEPT ;
        public:
            static const unsigned int defaultNotifyInterval = 10 ; // sec
        protected:                                 // PRQA S 2101 5
            const Application &app_ ;
            mutable Status currentStatus_ ;
            mutable std::string reason_ ;
        private:
            unsigned int _notificationInterval ;
            ACS_CLASS_DECLARE_DEBUG_LEVEL(Application::StatusNotification) ;
    } ;


    class LogRotationPolicy : public ThreadSafe {                       // PRQA S 2109, 2153
        public:
            enum Mode {
                Aging,			// based on file aging
                Size,			// based on files size
                FirstApplying, 	// combination of the above
                Daily           // a log every day as per CSGACQ-84
            } ;

            LogRotationPolicy(const std::string &fullLogPathName,
                Mode,
                size_t seconds,
                std::streampos bytes,
                unsigned int saveLog=1,
                bool compressOld=true,
                bool appendToLast=false) ;
            LogRotationPolicy(const std::string &fullLogPathName,
                  std::vector<exostream *> &streams,
                  Mode,
                  size_t seconds,
                  std::streampos bytes,
                  unsigned int saveLog=1,
                  bool compressOld=true,
                  bool appendToLast=false) ;
            LogRotationPolicy(const LogRotationPolicy &) ;
            LogRotationPolicy& operator=(const LogRotationPolicy &) ;
            virtual ~LogRotationPolicy() ACS_NOEXCEPT ;

            bool behaveLike(const LogRotationPolicy &) const ;
            Mode mode() const ACS_NOEXCEPT ;

            /**
             * Set log rotation mode.
             * Will be effective next time the rotationNeeded method is invoked.
             */

            void mode(Mode) ACS_NOEXCEPT ;
            size_t aging() const ACS_NOEXCEPT ;
            /**
             * Set aging period.
             * It applies only if mode is Aging or FirstApplying and will be effective next time the
             * rotationNeeded method is invoked.
             */
            void aging(size_t seconds) ACS_NOEXCEPT ;
            std::streampos size() const ACS_NOEXCEPT ;
            /**
             * Set size condition.
             * It applies only if mode is Size or FirstApplying and will be effective next time the
             * rotationNeeded method is invoked.
             */
            void size(std::streampos bytes) ACS_NOEXCEPT ;

            /**
             * Set number of old logs to be kept when rotating.
             */
            void saveLog(unsigned int logs) ACS_NOEXCEPT ;
            unsigned int saveLog() const ACS_NOEXCEPT ;

            /**
             * Set compression option for old logs
             */
            void compressOld(bool compress) ACS_NOEXCEPT ;
            bool compressOld() const ACS_NOEXCEPT ;


            /**
             * Set appendToLast option for old logs
             */
            void appendToLast(bool append) ACS_NOEXCEPT ;
            bool appendToLast() const ACS_NOEXCEPT ;
            /**
             * Set if details from filterable (e.g. exceptions stack whee exceptions are notified into the filterable) must be shown in log
             */
            void notifyDetailFromFilterable(bool) ACS_NOEXCEPT ;

            /**
             * Set if the log shall discard free text and allow notify of filterables and exception only
             */
            void formattedOnly (bool) ACS_NOEXCEPT ;

            /** sets if stack trace should be notified along exceptions notification. Just to remember exception notification
                   is invoked only using operator << over an exception or calling the method notify with an exception.
             */
            void allowStackTrace(bool) ACS_NOEXCEPT ;

            /**
             * Set a new formatter for the log
             */
            void setFormatter(exMsgFmt *) ACS_NOEXCEPT ;

            /**
             * calls the private check function passing an empty vector of filters
            */
            bool check() ;

            /**
             * set the logname to be used when the application override is disabled
            **/
            void setLogName (std::string const &);


            /**
             * returns the currently opened log file
            **/
            std::string getCurrentLogName () const ACS_NOEXCEPT ;

            /**
             * set and get the force log name flag
             * when the logname is forced the name passed in the ctor or in the setLogName
             * will be used to create and rotate the logs
            **/
            void forceLogName (bool) ACS_NOEXCEPT ;
            void forceLogName (const std::string& ) ACS_NOEXCEPT;
            bool forceLogName () const ACS_NOEXCEPT ;


        protected:
            /**
                * Generates the log filename
                * It can use the internal attribute of the desired filename
            */
            virtual std::string computeLogName () const;

        private:

            // PRQA S 2107 4
            friend bool Application::rotateLog(const std::vector<exFilter *> &) ;
            friend void Application::setLRPolicy(const LogRotationPolicy &) ;
            friend void Application::createDefaultLRP() ;
            friend Application::Application(const std::string &appName, const std::string &subSystem);

            void rotateOldLogFiles(const std::vector<exFilter *> &) ;
            void reset() ;
            bool check(const std::vector<exFilter *> &) ;
            bool forceRotation(const std::vector<exFilter *> &) ;
            bool rotationNeeded() ;
            bool closeAndRotateLog() ;
            static void compress(const std::string &) ;
            void attachFilters(const std::vector<exFilter *> &) ;
            bool isLogOlder(Timer::Delay ms) ;

            LogRotationPolicy() ; // not implemented
        public:
            const static unsigned short _saveLogMax = 128 ; // max log saved is 128
        private:
            std::string _currentLogName ;
            std::string _logName ;
            bool _forceLogName;
            std::vector<exostream *> _streams ;
            Mode _mode ;
            size_t _seconds ;
            std::streampos _bytes ;
            unsigned int _saveLog ;
            bool _compressOld ;
            bool _appendToLast ;
            Timer _timer ; // not copied ;
            exLogStream *_log ; // not copied
            bool _notifyDetailFromFilterable ;
            bool _allowStackTrace ;
            exMsgFmt *_formatter ;
            struct tm _logCreationDate; // not copied
            bool _formattedOnly;
    } ;


    virtual ~Application() ACS_NOEXCEPT ;

    const std::vector<std::string> & getParsedArgs() const ACS_NOEXCEPT ;
    void setParsedArgs(const std::vector<std::string> &) ;

    virtual int usage(const std::string &exename) const ;

    /**
     * To be called to rotate log or create it the very first time adding a filter to it.
     * returns true if the log was rotated, even created
     */
    bool rotateLog(exFilter &) ;

    // returns the default log size when rotation occur
    virtual std::streampos getDefaultRotationLogSize() const ;

    // returns the default log age when rotation occur
    virtual size_t getDefaultRotationLogAgeSec() const ;

    // returns the default log num to save
    virtual unsigned int getDefaultRotationLogNumToSave() const ;

    // returns the default log num to save
    virtual bool getDefaultRotationLogCompressOld() const ;

    // returns if default is to append or not
    virtual bool getDefaultRotationAppend() const;

    // returns the default log mode
    virtual Application::LogRotationPolicy::Mode getDefaultRotationMode() const;

    // returns if override the filename or not
    // see setLogName and forceLogName also
    virtual bool getLogNameFromApplication() const;
    virtual std::string getDefaultLogName() const;


    /**
     * The debugPath param can be used to set a path different from the default one where to search
     * for debug files. Set a path into the string to change default.
     */
    virtual void debugMode(int argc, char const * const * argv, char const * const * env, std::string &debugPath) const ;


    /**
     * To be called to rotate log or create it the very first time.
     * returns true if the log was rotated, even created
     */
    bool rotateLog() ;


    /**
     * This is the method that actually does the work.
     */
    virtual int main(int argc, char const * const * argv, char const * const * env) =0;


    /*! method to be called each time the status changes
     * \returns previous status
     */
    static Status setStatus(Status, const std::string & reason = "") ;

    /**
     * \Return the instance of the unique instance of Application class.
     * \note Dislike other singleton implementations instance method doesn't instace
     * the class itself.
     * \throw Application::Exception is thrown No Application has been built.
     */
    static Application * instance() ; // throw(Application::Exception) - no more need to declare
    static std::string appName() ;

    static bool exists();
    /**
     * \Return the string from a standard RCS Revision: XXX tag.
     *	$ Revision: 1.3.4.5 $ -> "1.3.4.5"
     *
     * but
     * 1.3-4.5 $ -> ""
     * "" -> ""
     * all -> ""
     *
     */
    static std::string parseRevision(const std::string &) ;

    /**
     * \Return the string from a standard RCS Date: XXX tag.
     *	$ Date: 2003/08/18 16:25:51 $ -> "2003/08/18"
     *
     */
    static std::string parseDate(const std::string &) ;

    /**
     * This method can be used in program main function.
     * It obtains application instance pointer and invokes start method.
     */
    static int run(int argc, char const * const * argv, char const * const * env);


    /**
     * Retrieve distribution version of the application
     * The operation is probably performed executing some external program
     * The external program is executed only the first time the method is called
     * \param applicationExecutable absolute path of application
     * \throw Application::exception if it is not possible to retrieve version and/or release
     */
    static std::string const & getDistributionVersion(std::string const &applicationExecutable) ;


    /**
     * Retrieve distribution release of the application
     * The operation is probably performed executing some external program
     * The external program is executed only the first time the method is called
     * \param applicationExecutable absolute path of application
     * \throw Application::exception if it is not possible to retrieve version and/or release
     */
    static std::string const & getDistributionRelease(std::string const &applicationExecutable) ;


    /**
     * Overloaded method. Calls getDistributionVersion(std::string const &applicationExecutable)
     * passing executableName() as an argument
     */
    std::string const & getDistributionVersion() ;


    /**
     * Overloaded method. Calls getDistributionRelease(std::string const &applicationExecutable)
     * passing executableName() as an argument
     */
    std::string const & getDistributionRelease() ;

    /**
     * If application executes doRestart() it will returns true.
     */
    bool mustRestart() const ACS_NOEXCEPT ;


    /**
     * This method restart the process. Must be executed by main
     * function,
     */
    static void restart();


    const std::string &applicationName() const ;

    void subSystem(const std::string &) ;

    const std::string &subSystem() const ;

    /**
     * returns the local hostname, just a convenience function calling HWScan::hostname in libFile
     */
    static std::string hostname() ;

    /**
     * Tries to return the real path of the executable
     * On Linux this is done using the /proc filesystem
     * On other system for the moment just returns argv[0]
     */
    static const std::string executableName()  ;

    void setLock(ApplicationLock *) ;
    ApplicationLock const * const getLock() const ;
    bool isRestarted() const ;

    /**
     * the method returns the actual application instance (set from external or discovered)
     * once set, each restart will retain that value
     */
    long getInstance() const ACS_NOEXCEPT { return instanceNo_ ; }


    /**
     * sets the instance if it has not set
     * \returns the value set (the old one if it was already set)
     * The instance set, if it is >=0 will be inherited again after the restar (phoenix).
     * This means that, once set, it will no more modifiable.
     */
    long setInstance(long inst_no) ;

    /**
     * the method can be invoked on a const Application to get a valid connectionPool.
     */
    dbConnectionPool *getDefaultConnectionPool() const ;
    void setDefaultConnectionPool(dbConnectionPool *) ;

    /**
     * the method is used to patch the log filename adding time informations
     * and is aimed to manage the mission specific requirements on the file naming convention
     */
    virtual std::string generateLogName (struct tm const &) const;

    /**
     * hook function needed to support project level password
     * authentication according to a operation -> password specific map
     * the password will be required inside the derived specialization.
     * Default implementation will return always true.
     * \param operation is the operation that is going to be performed
     * \param message reports any diagnostic message useful to identify the reason of failed authentication
     * \return true if auth is ok or false otherwise, specifying details in message
     */
     virtual bool authOperation (const std::string& operation, std::string& message) const;

    /**
    * Get application Uuid
    */
    std::string getCurrentUuid() const;
public:

    /*!
     * \brief Returns the current stop mode according to the received signals and events
     * \returns the current StopMode of the application
     */
     Application::StopMode getCurrentStopMode() const ;

    /*!
     * \brief Returns the current stop mode according to the received signals and events
     * \returns the current StopMode of the application
     */
     std::string currentStopModeToString() const ;


    /**
     * registers a post log method, which signature will be void f(std::string &)
     * all functions registered will be called following the order in which they registered.
     * The strings returned are then posted in the excerr and related streams.
     * No exception will be notified, all their occurrence will be cached and silently ignored.
     * The same method can be registered many times and will be called for each registration.
     */
    static void registerPostLog(PostLogMessage) ;

    /**
     * unregisters a post log method if found (the last occurrence).
     * returns true if it was unregistered.
     * In case a method was registered more times, it will takes the same number of calls
     * to this function to get rid of every occurrence.
     */
    static bool unregisterPostLog(PostLogMessage) ;

    /**
     * Returns the status of the postlog flag.
     *
     */
    static bool allowPostLog() ;

    /**
     * Changes the status of the postlog flag.
     *
     */
    static void allowPostLog(bool) ;

    // configuration issues ==========
    // add a configuration space
    void addSpace(const ConfigurationSingleton::ConfSpace &,
                  ConfigurationSingleton::SpaceLoadMode mode=ConfigurationSingleton::LoadLazy,
                  const ConfigurationSingleton::StopPredicate &stop = ConfigurationSingleton::StopPredicate()) const ;

    void loadConfiguration(bool notifyReplacementException=true,
                           const ConfigurationSingleton::StopPredicate &stop = ConfigurationSingleton::StopPredicate());

    bool setHostnameSpaceMandatory(bool v) const ;

    void setRotateLogParametersFromConf (bool) ACS_NOEXCEPT ;

    Status getCurrentStatus() const ACS_NOEXCEPT ;
    Status getCurrentStatus(std::string &r) const ;

    /*-
     * let's say you have an int variable (cond) that a signal handler will change from its value to
     * signal an interrupt. Nominally the value is contCond, if it change from this value,
     * the signal has been received.
     * If you want to perform a delay, but still be reactive to the signal you can use the method:
     * \param totDelay is the total delay in ms the method will wait in absence of signal
     * \param cond the variable changed asynchronously
     * \param contCond the variable value in absence of signal
     * \param slice the interval to check if it has been changed - default 1000 ms e.g. 1 sec
     * \returns true if condition was signaled, false if timeout expired
     */
    __attribute__((deprecated)) static bool delayOrCondition(Timer::Delay totDelay, volatile int *cond, int contCond, Timer::Delay slice=1000) ;

    /*-
     * This method waits a given time interval, unless a broadcast signal on condition attribute is performed to return immediately
     * \param msectimeout is the total timeout in ms to expect in absence of broadcast
     * \returns true if the signal has been received and false in case of timeouts
     */
    bool waitStopOrTimeout(unsigned long msectimeout) const ;

    /*-
     * This method check if a stop was requested or if the phoenix restart is engaged
     * \returns true if the stop mode is different from NoStop
     */
    bool requestedStop() const ;

protected:

    /**
     * To be called to update debug information.
     */
    void updateDebug() ;


    virtual void configurationLoaded() {} // PRQA S 2131

    /**
     * useful to force demonization flag (true - force daemonize, false not).
     */
    void allowDaemonize(bool);

    virtual int internalusage(const std::string &exename) const ;

    /**
     * Turn the application into a daemon.
     */
    void daemonize();

    /**
     * This method can  be reimplemented by derived classes.
     * Its aim is to provide some common initialization actions.
     * Reimplemented  procedures must always invoke Application::start
     * to start main method
     */
    virtual int start(int argc, char const * const * argv, char const * const * env);

    /**
     * This method loops over the registered PostLogMessage functions \see registerPostLog and
     * unregisterPostLog and logs their returned strings into excerr.
     * If the postlog flag is false, no loop is performed \see allowPostLog.
     */
    static void postLogMessage() ;

    /**
     * Invokes the statistics collector to emit the report on a private info filterable
     * Statistics can be enabled or disabled by configuration
     */
    void dumpStatistics() const;

    /**
     * called by successfull daemonize()
     */
    virtual void preDaemonize();

    /**
     * called by successfull daemonize()
     */
    virtual void postDaemonize();


    /**
     * called by applications that want to restart
     * in order to register; restartTime -> seconds
     */
    void wantToRestart(const unsigned int & restartTime);

    /**
     * used internally and managed by the resourceset
     * if not enabled timeToRestart will always return false
     */
    bool restartEnabled() const;

    /**
     * called by applications that want to restart
     * in order to see if it is time (true -> restart)
     */
    bool timeToRestart(bool retrigger = false );


    /**
     * called by applications that want to know
     * how long does it take for next restart time
     * (returns 0 if timer expired)
     */
    unsigned int timeToRestart(unsigned int & restartTime);


    /**
     * Returns the restart interval currently set (the returned value is in seconds)
     */
    unsigned int restartInterval() const;


    /**
     * called by applications when want to restart
     *
     * This method is deprecated because now the application will restart automatically
     * (using getCurrentStopMode == PhoenixRestart) if wantToRestart was called and the timeout has been reached
     */
    __attribute__((deprecated)) void doRestart();

    void debugActivation(int argc, char const * const * argv, char const * const * env);


    Status setCurrentStatus(Status, const std::string &reason="") ACS_NOEXCEPT ;

    /*!
     *  \returns true if the application is currently daemonized
     */
    bool isDaemon() const ACS_NOEXCEPT { return isDaemon_ ; }

    /*!
     *  \returns true if the application called daemonize even if allowDaemonize was true
     * this is useful when applications start with --nodaemon but we want to simulate completely its behaviour
     */
    bool daemonizeCalled() const ACS_NOEXCEPT { return daemonizeCalled_ ; }

    /*!
     * set the curent StopMode of the application
     * \param the new stop mode to set
     */
     void setCurrentStopMode (const Application::StopMode&) const ;

    /*!
     * \returns a flag indicating if the stop mode has changed in the meantime
     * \param resetFlag can be used to force flag reset or not
     */
    bool hasStopModeChanged (bool resetFlag = true) ;

    /*!
     * \returns a flag indicating if the stop mode has changed since the given timestamp
     * \param checkTime, is the time to use to detect if a mode change happened in the meantime
     * \param lastChangeTime, is the time of the last stop mode change that occurred
     */
    bool hasStopModeChanged (const struct timeval& checkTime, struct timeval& lastChangeTime) ;


    /*!
     * Used by the stop manager to ask the application if a timeout shall be enforced
     * in case a sigterm has been received.
     * 0 means that no timeout is foreseen and the application is willing to wait forever
     * upon timeout expiration the current sigTermTimeoutHandler() is invoked (just once)
     * \returns the number of seconds to wait (0 means forever, default 30)
     */
    virtual unsigned int getSigTermTimeoutSeconds () const;

    /*!
     * Used by the stop manager to ask the application if a timeout shall be enforced
     * during the phoenix phase
     * 0 means that no timeout is foreseen and the application is willing to wait forever
     * upon timeout expiration the current phoenixTimeoutHandler() is invoked (just once)
     * \returns the number of seconds to wait (0 means forever, default 0)
     */
    virtual unsigned int getPhoenixTimeoutSeconds () const;

    /*!
     * Invoked by the stop manager in case
     * - a sigterm has been received
     * - the application wants to apply a timeout for the forced exit
     * - the timeout has expired
     * It is expected to terminate the application and do not return.
     * It is called once.
     * Default behavior is to call abort() system call.
     */
    virtual void sigTermTimeoutExpired();

    /*!
     * Invoked by the stop manager in case
     * - the application in in the phoenix phase
     * - the application wants to apply a timeout for the process exit
     * - the timeout has expired
     * It is expected to restart the application and do not return.
     * It is called once.
     * Default behavior is to call Application::restart() function
     */
    virtual void phoenixTimeoutExpired();

    /**
     * Returns a copy of the internal attribute keeping the current stop mode
     * it can be used only from inside the routine that computes the new stop mode
     * according to the received signals
     */
    StopMode getCurrentStopModeAttribute() const;

private:
    //Not implemented to prevent their use
    Application();
    Application(Application const &);
    Application & operator=(Application const &);

    /**
     * Chec if the stop manager thread is up and running
     * @returns false if the thread is unavailable
     */
    bool checkStopManagerThread() const;

    void getLRPFromConfiguration(LogRotationPolicy&, const std::string& rootTag) const;

    /**
     * called by applications that want to restart
     * in order to finish the process in a clean way
     * each application will implement this method
     */
    virtual void terminateProcess();

    /**
     * called by applications that want to restart just after the terminate process method
     * This method is intended to be reimplemented in complex class hierarchies
     */
    virtual void beforeRestart();

    /**
     * Start main method.
     */
    int startMain(const int argc, char const * const * argv, char const * const * env);
    static void getDistributionVersionAndRelease(std::string const &applicationExecutable) ;

    bool parseArgs(std::vector<std::string> &parsedArgs,
        int argc,
        char const * const * argv,
        char const * const * env) ;

    void loadConfigurationInternal();

    void broadcastCondition() const;

public:
    static const std::string defaultLogPath ; // defaulted to base::defaultLogPath() // PRQA S 2100
    static const std::string defaultMCLogPath ; // defaulted to base::defaultMCLogPath() // PRQA S 2100
    static const size_t defaultLogAge = 86400 ; // a log a day keeps the doctor away
    static const std::string autoRestart;       // PRQA S 2100
private:
    bool _restarted ;
    std::string _appName ;
    bool _allowDaemonize ;
    bool isDaemon_ ;
    bool daemonizeCalled_ ;
    std::string _subSystem ;

    unsigned int _restartTime; /* units -> seconds */
    Timer _restartTimer ;

    static char ** argv_ ;
    static Status restartStatus_ ;
    static Application *_instance;
    LogRotationPolicy _logRotationPolicy ;
    LogRotationPolicy _mclogRotationPolicy ;

    static ThreadSafe  _hNameMutex ;
    static std::string _exeName ;
    static const std::string _distributionVersionApplication;
    static std::string _distributionVersion ;
    static std::string _releaseVersion ;
    ApplicationLock * _lock ;
    mutable std::unique_ptr<ApplicationUuid> _uuid;
    static bool _allowPostLog ;
    ApplicationDebug *_applicationDebug ;
    exLogStringStream *_preLog ;
    MCDefaultFilter _mcfilter ;
    dbConnectionPool *_connPool ;
    Status status_ ;
    std::string statusReason_ ;
    std::vector<std::string> args_ ;
    bool rotateLogParametersFromConf_;
    long instanceNo_ ;
    bool instanceNoArgToBeAdded_ ;
    std::string confOverrideFilepath_;

    // The stop mode attribute
    mutable StopMode currentStopMode_;
    mutable bool stopModeChanged_;
    ApplicationStopManager* stopManagerThread_;
    Condition condition_;
    mutable struct timeval lastStopModeChangeTime_;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(Application) ;

};
// PRQA L:L1



std::ostream & operator<<(std::ostream &, Application::LogRotationPolicy::Mode) ;
exostream & operator<<(exostream &, Application::LogRotationPolicy::Mode) ;     // PRQA S 2072
std::ostream & operator<<(std::ostream &, Application::Status) ;
exostream & operator<<(exostream &, Application::Status) ;     // PRQA S 2072


_ACS_END_NAMESPACE

#endif /* _Application_H_ */
