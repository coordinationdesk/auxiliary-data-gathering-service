// PRQA S 1050 EOF
/*

    Copyright 1995-2022, Exprivia SPA
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.com

    $Prod: A.C.S. Application Library $

    $Id$

    $Author$

    $Log$
    Revision 5.37  2017/11/15 10:46:19  chicas
    some message to PRIVINFO

    Revision 5.36  2017/09/29 09:43:13  lucio.pulvirenti
    S2PDGS-1843: configuration override filepath added to command line input. If set, it is loaded with the proper method of
    ConfigurationSingleton.

    Revision 5.35  2017/09/12 13:20:23  marpas
    support to application instance number and its persistence across a restart (phoenix)
    getInstance() method returns the actual instance (or -1 if no instance is known)
    setInstance() method to set a value (honored only if the actual instance is not known)

    Revision 5.34  2017/06/06 07:08:18  tergem
    S2PDGS-1641: getRotateLogXXX suppressed

    Revision 5.33  2017/06/05 15:31:36  marfav
    Message on missing log configuration issued only one time per process

    Revision 5.32  2017/02/09 13:05:33  marfav
    NotifyOnly replaced with FormattedOnly

    Revision 5.31  2017/02/09 10:30:49  marfav
    CSGACQ-113
    Added support to notifyOnly modificator in log streams

    Revision 5.30  2016/10/07 10:01:55  marfav
    S1PDGS-31809
    Log files permissions is in charge of exLogStream class
    (depends on libException >= 5.86)

    Revision 5.29  2016/07/04 14:06:50  marfav
    Do not use any default when trying to read the LRP from conf spaces

    Revision 5.28  2016/07/04 13:11:02  marfav
    QAC++ warning removed on the new code

    Revision 5.27  2016/07/04 12:56:19  marfav
    CSGACQ-84
    Adding support to application defined log rotation management
    Adding support to daily rotated logs (midnight to midnight)

    Revision 5.26  2016/03/29 12:41:52  marfav
    S1PDGS-31708 Added pthread_atfork handlers to serialize forking

    Revision 5.25  2016/03/09 10:03:11  marpas
    old logs are now into an unsigned int

    Revision 5.24  2016/01/11 15:48:07  marpas
    status notifier thread named: AppStatusNotifier

    Revision 5.23  2015/12/17 17:38:32  marpas
    removing from stderr/stdout re-emission of messages BEFORE log opening

    Revision 5.22  2015/11/27 15:14:57  marpas
    plugin load failure is logged with a warning, other than set the application status

    Revision 5.21  2015/10/29 12:16:49  marfav
    PDSEV-17 printing the stack of all the running threads in case of crash

    Revision 5.20  2015/10/22 17:51:42  marpas
    removing private info filterables from exostream::getDefaultOutLogger()

    Revision 5.19  2015/10/22 17:40:10  marpas
    debug message typo

    Revision 5.18  2015/10/22 16:10:54  marpas
    use of macros enforced
    prelog messages outputted as non-filterable class

    Revision 5.17  2015/08/25 12:21:44  marfav
    MC separate log put under ifdef ACS_CREATE_MC_LOG_FILES

    Revision 5.16  2015/06/18 07:33:05  tergem
    useless include of DateTime removed

    Revision 5.15  2015/06/12 08:22:00  tergem
    APF-267 Separate Log implementation for filterable events:
    _mclogRotationPolicy LogRotationPolicy defined and instantiated in the setLRPolicy function
    appendToLast set and get method defined in LogRotationPolicy class
    defineMCLogName virtual protected method added to allow its reimplementation
    forceRotation method added to implement the rotation on the mc log every time the rotation is Needed
    defaultMCLogPath defined

    Revision 5.14  2015/02/19 13:29:21  marpas
    log rotation policy characteristics message is issued only if the log rot pol changes

    Revision 5.13  2014/12/17 13:16:24  marpas
    timeToRestart do not retrigger the timer - once it elapsed, it continue returning true, unless the retrigger arg is set to true when calling

    Revision 5.12  2014/09/12 13:37:28  marfav
    timeToRestart can be forced to return always false by configuration

    Revision 5.11  2014/08/26 10:33:25  marfav
    S3PDGS-1970: adding possibility to reconfigure log files permission at compile time

    Revision 5.10  2014/07/11 14:01:17  lucio.pulvirenti
    S2PDGS-834: Lock in getCurrentStatus and setCurrentStatus to prevent crash in case of thread concurrency.

    Revision 5.9  2014/05/13 11:24:01  marpas
    Application::configurationLoaded virtual method added, it will be called after the loadConfiguration method. To be reimplemented in derived classes to know that the configuration is read, just in case they want to read some parameters from it.
    Application::StatusNotification::reloadConf virtual method added, to allow derived class to read parameters from configuration.

    Revision 5.8  2014/05/12 15:49:03  lucio.pulvirenti
    S2PDGS-760: StatusNotification::getActualNotificationInterval public method added to get notification interval from configuration, if occurring.

    Revision 5.7  2014/01/22 10:11:07  marpas
    robustness improved
    qa warning fixed

    Revision 5.6  2014/01/21 13:41:13  marfav
    setRotateLogParametersFromConf name mispell fixed

    Revision 5.5  2014/01/21 12:03:15  marfav
    ADC-224: log rotation policy read from configuration and applied in loadConfiguration

    Revision 5.4  2013/09/27 14:08:00  marpas
    streams have name

    Revision 5.3  2013/07/24 19:08:09  marpas
     Status stream inserter operator added - convenience for messages

    Revision 5.2  2013/07/16 13:36:23  marpas
    getDefaultConnectionPool is a const method from now on.

    Revision 5.1  2013/07/05 12:29:55  marpas
    adopting new libException 5.x standards
    using acs::base::defaultLockPath() to define application lock path

    Revision 5.0  2013/06/06 08:37:35  marpas
    new ibException 5.0 nterfaces

    Revision 4.24  2013/04/29 15:58:28  marpas
    Statistics are now emitted with exFPrivateInfo filterable
    test improved

    Revision 4.23  2013/04/23 13:49:19  marpas
    getActualRotationLogCompressOld, getActualRotationLogNumToSave, getActualRotationLogAgeSec, getActualRotationLogSize methods added,
    previous getDefaultLogSize, getDefaultLogAgeSec, getDefaultLogNumToSave, getDefaultLogCompressOld moved to getDefaultRotationXXX

    Revision 4.22  2013/04/23 11:10:26  marpas
    new methods added: getDefaultLogSize(), getDefaultLogAgeSec(), getDefaultLogNumToSave(), getDefaultLogCompressOld()

    Revision 4.21  2013/04/09 07:51:45  marpas
    new statistics namespace

    Revision 4.20  2013/03/14 15:25:14  marpas
    seDefaultConnectionPool and getDefaultConnectionPool method substitutes setConnectionPool and getConnectionPool
    interface rationalization

    Revision 4.19  2013/03/12 17:43:33  marpas
    registerWaitingCP method and related ones implemented to manage connection pool.
    When the connection pool is set, any registered will be called with a reference of the connection pool

    Revision 4.18  2013/02/19 15:23:14  marpas
    Application::daemonizeCalled method implemented

    Revision 4.17  2013/02/19 14:32:44  marpas
    Application::AppPluginLoaderNotifier class introduced
    this imply a dependency from libPlugin (added)

    Revision 4.16  2013/02/07 09:11:46  marpas
    namespaces enforced
    some interfaces modification in singleton's classes

    Revision 4.15  2013/02/05 18:06:41  marpas
    delayOrCondition signature changed

    Revision 4.14  2013/01/24 19:26:30  marpas
    connectionPool management improved for derived classes

    Revision 4.13  2012/12/01 17:25:27  marpas
    missing include

    Revision 4.12  2012/11/28 17:19:24  marpas
    qa rules and robustness

    Revision 4.11  2012/07/02 15:37:36  micmaz
    modified Critical message from Error to Critical

    Revision 4.10  2012/03/13 16:32:20  marpas
    help fixed

    Revision 4.9  2012/03/09 12:59:51  marpas
     no more empty messages from hooks

    Revision 4.8  2012/03/08 09:03:36  marpas
    default args are accepted with a single or double hyphen in front

    Revision 4.7  2012/03/06 11:08:17  marpas
    message about usage reimplementation improved

    Revision 4.6  2012/03/01 15:48:09  marpas
    using acs::base for default static paths

    Revision 4.5  2012/02/28 18:01:40  marpas
    SW status notification Critical maps to string ERROR

    Revision 4.4  2012/02/28 16:20:39  marpas
    Confspaces cannot be removed

    Revision 4.3  2012/02/24 14:30:30  marpas
    loadConfiguration emits warning or notifies exception now

    Revision 4.2  2012/02/24 11:32:10  marpas
    delayOrCondition method added

    Revision 4.1  2012/02/14 14:17:42  marpas
    refactoring

    Revision 4.0  2011/10/18 12:43:58  marpas
    new statistics adopted

    Revision 3.23  2011/10/13 14:10:05  marpas
    debug improved

    Revision 3.22  2011/10/13 13:53:23  marpas
    status notifications improvements (reason was sometime not notified)

    Revision 3.21  2011/10/13 10:58:48  marpas
    status is notified when notification thread exits (flush concept)
    setStatus has a debug message.

    Revision 3.20  2011/09/26 12:14:16  marpas
    isDaemon() method added,
    Notification class has virtual dtor

    Revision 3.19  2011/09/26 11:19:16  marpas
    SSF development in progress

    Revision 3.18  2011/09/22 18:26:56  marpas
    status is inherited from last status when restarting

    Revision 3.17  2011/09/22 17:31:56  marpas
    StatusNotification class improved

    Revision 3.16  2011/09/22 13:35:02  marpas
    no statistics if internalusage is called

    Revision 3.15  2011/09/22 11:49:04  marpas
    preLog deleted in dtor

    Revision 3.14  2011/09/22 10:28:17  marpas
    message improved when usage method is not correctly implemented

    Revision 3.13  2011/09/22 09:26:33  marpas
    usage method management standardized

    Revision 3.12  2011/09/21 17:39:18  marpas
    getParsedArgs and setParsedArgs method added

    Revision 3.11  2011/09/21 09:53:12  marpas
    now statistics are forced by default

    Revision 3.10  2011/09/20 17:34:52  marpas
    Software Status Notification (S1PDGS-1902) in progress

    Revision 3.9  2011/09/12 17:30:26  marpas
    setStatus development in progress (i/f creation)
    default for stack trace notification changed

    Revision 3.8  2011/09/12 12:46:34  marpas
    filterables used in nominal messages

    Revision 3.7  2011/08/08 12:18:04  marpas
    concept added:
    repository to store and retrieve a connection pool. NB: the application is responsible of:
    1) create and set a connection pool in an appropriate moment
    2) destroy and release the connection pool in an appropriate moment
    3) notify to any using class tree the change of the connection pool itself (if needed: e.g. any thread running and potentially using it)

    Revision 3.6  2011/04/27 09:22:17  marpas
    old log rotation can fail when application starts (not restart) and a .log was present: the latter was destroyed

    Revision 3.5  2011/03/18 18:02:13  marpas
    multithread robustness improved, now the log rotation policy uses the exLogStream::rotate method

    Revision 3.4  2011/02/28 17:07:56  giucas
    debug improved

    Revision 3.3  2010/09/15 10:19:13  marpas
    LogRotationPolicy has allowStackTrace(bool) method to set the stack trace into the log when notifying exceptions

    Revision 3.2  2010/05/26 10:36:11  marpas
    Application::allowDaemonize method added

    Revision 3.1  2009/10/08 12:58:50  marpas
    prelog added in order to have in the logfile all the messages issued BEFORE the log creation

    Revision 3.0  2009/10/08 10:48:56  marpas
    uses new Statistics from libException 3.x

    Revision 2.24  2009/06/19 10:13:55  tergem
    fixed for gcc version < 4

    Revision 2.23  2009/04/03 15:39:24  marpas
    some rules fixed

    Revision 2.22  2009/01/30 15:23:43  marpas
    the system mode exception list is no longer in output even if it was previously ignored

    Revision 2.21  2008/12/16 16:58:49  marpas
    VEN-54
    SystemMode final implementation

    Revision 2.20  2008/11/20 09:12:09  paoscu
    Added options to log

    Revision 2.19  2008/11/17 15:16:44  marpas
    using File::getCurrentPath() instead of just ::getcwd

    Revision 2.18  2008/11/14 18:01:15  marpas
    debug improved

    Revision 2.17  2008/11/14 14:26:15  marpas
    setHostnameSpaceMandatory method added

    Revision 2.16  2008/11/13 13:47:25  marpas
    Stop predicate introduced

    Revision 2.15  2008/11/13 12:39:38  marpas
    new ConfigurationSingleton approach for configuration. Convenience functions added.

    Revision 2.14  2008/05/06 16:17:44  marpas
    debug & test improved

    Revision 2.13  2008/01/21 11:38:20  clanas
    fixed cast to size_t which works on 32 and 64 bit

    Revision 2.12  2008/01/08 14:10:04  marpas
    bool Application::LogRotationPolicy::check() public method added

    Revision 2.11  2007/07/20 09:58:04  marpas
    error messages from logfile fixed

    Revision 2.10  2007/07/20 09:11:10  marpas
    Logfile is now readable and writable to everyone

    Revision 2.9  2007/06/19 10:37:55  marpas
    restarting not all file descriptors have to be closed (saving first 4)

    Revision 2.8  2007/05/21 08:37:54  marpas
    -logfile application argument management added

    Revision 2.7  2007/04/18 13:48:27  clanas
    reverted last modification on minor version

    Revision 2.6  2007/04/18 13:46:22  clanas
    added minor version

    Revision 2.5  2007/04/18 13:42:06  clanas
    added switch on gcc version

    Revision 2.4  2007/04/11 15:38:09  fracar
    closing file descriptors before execvp upon restart

    Revision 2.3  2007/02/15 15:16:15  marpas
    -dbstat argument management implemented

    Revision 2.2  2006/12/19 19:48:17  paoscu
    executableName modified to use /proc filesystem on Linux to get the real path

    Revision 2.1  2006/05/17 07:57:49  giucas
    Debug improved

    Revision 2.0  2006/02/28 08:35:39  marpas
    Exception 2.1 I/F adopted

    Revision 1.44  2006/02/03 14:58:25  marpas
    attachFilters method added
    log rotation with filters now works even in case the log file is already open

    Revision 1.43  2005/10/27 16:47:34  marpas
    log rotation policy fixed, size is checked even in append case

    Revision 1.42  2005/10/14 09:55:03  marpas
    debugMode method added

    Revision 1.41  2005/10/13 19:37:04  marpas
    new ApplicationDebug modes implemented - tested in libCryosatApp

    Revision 1.40  2005/10/06 19:41:32  marpas
    ApplicationDebug concept introduced, installs a SIGUSR2 handler in order to change debug values at runtime

    Revision 1.39  2005/09/14 17:50:31  marpas
    useless include removed

    Revision 1.38  2005/08/05 19:31:44  marpas
    post log hooks management improved

    Revision 1.37  2005/08/05 15:40:30  marpas
    debug improved

    Revision 1.36  2005/08/05 14:18:55  marpas
    LogRotationPolicy ctors now have a parameter more to signal the application to
    append this log to the last one. It is defaulted to false in order to not change
    the current behaviour.
    Moreover, log rotation, opening and closure now put a timestamp in the message.
    This in order to better investigate when a log was written as many policy do not
    print any time in their logs.
    to see how to use the append flag, please refer to the tPhoenix test of this module

    Revision 1.35  2005/08/05 10:58:17  marpas
    post log messages implemented

    Revision 1.34  2004/12/14 15:10:33  paoscu
    Messages  about log rotation are no longer sent as exFMessage but just as simple strign to avoid cluttering of MCF console.

    Revision 1.33  2004/09/28 11:57:47  paoscu
    decoupling between static Applications member and instance ones

    Revision 1.32  2004/09/10 09:01:03  paoscu
    all rotateLog flavours now return bool to notify the log was really rotated

    Revision 1.31  2004/04/14 11:12:29  paoscu
    Log rotation policy uses filters now and preserve them
    Thread safety implemented with ThreadSafe

    Revision 1.30  2003/12/22 19:15:11  paoscu
    More debug prints.

    Revision 1.29  2003/10/06 16:08:33  marpas
    message changed

    Revision 1.28  2003/10/01 16:25:51  marpas
    Application::isRestarted method added to know if the application was automatically restarted

    Revision 1.27  2003/09/29 15:21:30  marpas
    Application::restartInterval method added.
    Application::getLRPolicy methods added.
    Application::LogRotationPolicy::fullLogPathname method added
    Application::LogRotationPolicy::mode methods added
    Application::LogRotationPolicy::aging methods added
    Application::LogRotationPolicy::size methods added
    Now is possible to ask and changes mode, aging and size to the Application's current LogRotationPolicy

    Revision 1.26  2003/09/18 10:54:16  fracar
    modified signature for 		unsigned int timeToRestart(unsigned int & restartTime);

    Revision 1.25  2003/09/17 17:37:31  fracar
    added method                 void timeToRestart(unsigned int & restartTime);

    Revision 1.24  2003/09/12 13:59:23  paoscu
    executableName method fixed.

    Revision 1.23  2003/09/12 13:57:21  paoscu
    executableName method added.

    Revision 1.22  2003/08/19 14:51:00  paoscu
    Application::parseDate static method added

    Revision 1.21  2003/07/22 20:55:01  paoscu
    parseRevision method added.
    New ApplicationLock management.

    Revision 1.20  2003/07/21 10:28:40  paoscu
    bug fixed in restart path.

    Revision 1.19  2003/07/15 09:17:03  paoscu
    log compression for log.2 and log.3 fixed

    Revision 1.18  2003/07/14 22:43:13  paoscu
    zeroes in scalar values for log rotation are now checked

    Revision 1.17  2003/07/14 22:27:18  paoscu
    LogRotationPolicy  algorithm completed

    Revision 1.16  2003/07/14 19:22:23  paoscu
    static members for Application class separate in a different compilation
    unit in order to avoid useless dependencies

    Revision 1.15  2003/07/14 19:12:59  paoscu
    static method hostname() added

    Revision 1.14  2003/07/12 02:12:42  paoscu
    Log rotation policy partially implemented

    Revision 1.13  2003/07/11 12:58:26  paoscu
    restart implementation policy

    Revision 1.12  2003/06/26 10:13:31  paoscu
    lock implemented

    Revision 1.11  2003/05/14 09:57:31  crivig
    restartTime changed to unsigned int

    Revision 1.10  2003/05/14 08:56:03  crivig
    methods to restart a process added

    Revision 1.9  2003/04/30 16:10:45  paoscu
    using namespace std no longer into acs include

    Revision 1.8  2003/04/15 20:25:01  paoscu
    GCC3.2

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

#include <Application.h>
#include <ApplicationLock.h>
#include <ApplicationUuid.h>
#include <SystemMode.h>
#include <SystemModeFactory.h>
#include <Filterables.h>
#include <exRTDebug.h>
#include <ACS_SSL.h>
#include <File.h>
#include <Statistics.h>
#include <exMsgFmtFactory.h>
#include <exAdaptiveFilter.h>
#include <Uuid.h>

#include <rsPushNSpace.h>

#include <unistd.h>
#include <cstring>
#include <sys/resource.h> // PRQA S 1013

#include <MCDefaultFilter.h>

#include <SignalDispatcher.h>
#include <MQTTHelper.h>

#include <EventNotifyFactory.h>

// Uncomment this define to enable a separated log file capturing the MC messages
// #define ACS_CREATE_MC_LOG_FILES

_ACS_BEGIN_NAMESPACE(acs)
using namespace acs::math;
using namespace std;


Application::AppPluginLoaderNotifier::AppPluginLoaderNotifier(Application *app) :
    PluginLoaderNotifier(),
    warning_(false),
    app_(app)  // PRQA S 2528
{
}

Application::AppPluginLoaderNotifier::~AppPluginLoaderNotifier() ACS_NOEXCEPT
{
}

void Application::AppPluginLoaderNotifier::operator()(const string &plugin) const
{
    ACS_LOG_WARNING("Cannot load plugin: " << plugin) ;
    if (app_ != 0) {
        app_->setStatus(Warning, "Unable to load plugin: " + plugin) ;
    }
    warning_ = true ;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(Application::StatusNotification)

Application::StatusNotification::StatusNotification(Application &a, unsigned int interval) :
    Thread("AppStatusNotifier"),                           // PRQA S 4052
    app_(a),                            // PRQA S 2528
    currentStatus_(),
    reason_(),
    _notificationInterval(interval)
{
}


Application::StatusNotification::~StatusNotification() ACS_NOEXCEPT
{
}

void Application::StatusNotification::run() // PRQA S 4020
{
    Timer nt ;
    for(;;) {
        callnotify() ; // notify

        nt.start() ;
        for (;;) {
            // a small delay (100 ms)
            timeoutOrCancel(100) ;

            if (requestedCancel()) {// stop ?
                callnotify() ; // notify
                return ;
            }

            // some reason to notify ? note status changed MUST be call as first
            if (statusChanged() || timeToNotify(nt) || shouldNotify()) { break ; }
        }
    }
}

void Application::StatusNotification::notify() ACS_NOEXCEPT
{
}

void Application::StatusNotification::callnotify() ACS_NOEXCEPT
{
    statusChanged() ;
    notify() ;
}

bool Application::StatusNotification::shouldNotify() const ACS_NOEXCEPT
{
    return false ;
}


bool Application::StatusNotification::statusChanged() const ACS_NOEXCEPT // PRQA S 4020, 4214
{
    try {
        string r ;
        Status s = app_.getCurrentStatus(r) ;
        if ((currentStatus_ != s) || (reason_ != r)) {
            currentStatus_ = s ;
            reason_ = r ;
            return true ;
        }
        return false ;
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
        return true ;
    }
}

bool Application::StatusNotification::timeToNotify(Timer &t) const ACS_NOEXCEPT
{
    t.stop() ;
    return t.elapsed() >= _notificationInterval * 1000 ; // PRQA S 3084, 4400
}

unsigned int Application::StatusNotification::getActualNotificationInterval() const ACS_NOEXCEPT // PRQA S 4214
{
    unsigned int notificationInterval = _notificationInterval;
    rsResourceSet& resultSet = ConfigurationSingleton::instance()->get() ;
    const string tag = "StatusNotificationInterval";
    if (resultSet.hasKey(app_.applicationName()+ "." + tag)) {
        string key = app_.applicationName()+ "." + tag;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "key \"" << key << "\" found") ;
        resultSet.getValue(key, notificationInterval);
    }
    else if (resultSet.hasKey(app_.subSystem()+"." + tag)) {
        string key = app_.subSystem()+"." + tag;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "key \"" << key << "\" found") ;
        resultSet.getValue(key, notificationInterval);
    }
    else if (resultSet.hasKey(tag)) {
         ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "key \"" << tag << "\" found") ;
           resultSet.getValue(tag, notificationInterval);
    }
    else {
         ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "key \"" << tag << "\" not found in configuration") ;
    }

     ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Notification interval: " << notificationInterval << " s") ;

    return notificationInterval ;
}


Application::PostLogHooks *Application::PostLogHooks::_instance = nullptr ;


Application::PostLogHooks::PostLogHooks() : ThreadSafe(), _hooks()
{
}

Application::PostLogHooks::~PostLogHooks() ACS_NOEXCEPT
{
}


Application::PostLogHooks &Application::PostLogHooks::instance()
{
    if (!_instance) {
        _instance = new PostLogHooks() ;
    }
    return *_instance ;
}


size_t Application::PostLogHooks::size() const
{
    Lock l(*this) ;
    return _hooks.size() ;
}

void Application::PostLogHooks::registerHook(PostLogMessage f)
{
    Lock l(*this) ;
    if (f) {
        _hooks.insert(_hooks.begin(),f) ;
    }
}

bool Application::PostLogHooks::unregisterHook(PostLogMessage f) // PRQA S 4020
{
    // remove the given hooks from the list
    if (!f) {
        return false ; // not found
    }
    Lock l(*this) ;
    for (size_t i = 0; i < _hooks.size() ; ++i) { // PRQA S 4238, 4244
        if (_hooks[i]==f) { // found ?
            _hooks.erase(_hooks.begin()+i) ; // erase it // PRQA S 3000
            return true ; // just return the action was successful: there could be only one
        }
    }
    return false ; // not found
}


void Application::PostLogHooks::callHooks(std::string &msg)
{
    for (size_t i = 0; i < _hooks.size(); ++i) { // PRQA S 4238, 4244
        try {
            string m ;
            _hooks[i](m) ;
            msg += m ;
        }
        catch(exception &) {
            //ignore it
        }
    }
}

//**************** Application::Builder ************************
Application::Builder::Builder()
{
    ACS_COND_THROW(_instance!=nullptr,Application::Exception("Another Builder already exists.")); // PRQA S 3081
    _instance=this;
}

Application::Builder::~Builder()
{
    _instance=nullptr;
}

Application::Builder* Application::Builder::instance()
{
    ACS_COND_THROW(_instance==nullptr,Application::Exception("No application Builder has been built.")); // PRQA S 3081

    return _instance;
}





//**************** Application::LogRotationPolicy *********************************

Application::LogRotationPolicy::LogRotationPolicy(          // PRQA S 2017
    const std::string &fullLogPathName,
    Mode m,
    size_t seconds,
    std::streampos bytes,                                   // PRQA S 2010
    unsigned int saveLog,
    bool compressOld,
    bool append2Last) :

    ThreadSafe(),
    _currentLogName(),
    _logName (fullLogPathName),
    _forceLogName (false),
    _streams(),
    _mode(m),
    _seconds(seconds),
    _bytes(bytes),
    _saveLog(saveLog),
    _compressOld(compressOld),
    _appendToLast(append2Last),
    _timer(),
    _log(nullptr) ,
    _notifyDetailFromFilterable(false) ,
    _allowStackTrace(true),
    _formatter(nullptr),
    _logCreationDate(),
    _formattedOnly (false)
{
}


Application::LogRotationPolicy::LogRotationPolicy(          // PRQA S 2017
    const std::string &fullLogPathName,
    std::vector<exostream *> &streams,
    Mode m,
    size_t seconds,
    std::streampos bytes,                                   // PRQA S 2010
    unsigned int saveLog,
    bool compressOld,
    bool append2Last) :

    ThreadSafe(),
    _currentLogName (),
    _logName (fullLogPathName),
    _forceLogName (false),
    _streams(streams),
    _mode(m),
    _seconds(seconds),
    _bytes(bytes),
    _saveLog(saveLog),
    _compressOld(compressOld),
    _appendToLast(append2Last),
    _timer(),
    _log(nullptr) ,
    _notifyDetailFromFilterable(false) ,
    _allowStackTrace(true),
    _formatter(nullptr),
    _logCreationDate(),
    _formattedOnly (false)

{
}


Application::LogRotationPolicy::LogRotationPolicy(const LogRotationPolicy &r) :
    ThreadSafe(),
    _currentLogName (r._currentLogName),
    _logName (r._logName),
    _forceLogName (r._forceLogName),
    _streams(r._streams),
    _mode(r._mode),
    _seconds(r._seconds),
    _bytes(r._bytes),
    _saveLog(r._saveLog),
    _compressOld(r._compressOld),
    _appendToLast(r._appendToLast),
    _timer(),
    _log(nullptr),
    _notifyDetailFromFilterable(r._notifyDetailFromFilterable),
    _allowStackTrace(r._allowStackTrace),
    _formatter(r._formatter),
    _logCreationDate(r._logCreationDate),
    _formattedOnly (r._formattedOnly)
{
    // empty
}

Application::LogRotationPolicy& Application::LogRotationPolicy::operator=(const LogRotationPolicy &r)
{
    if (this != &r) { // check auto assignment
        // fill all the status variable
        // _currentLogName = r._currentLogName;
        _logName = r._logName;
        _forceLogName = r._forceLogName;
        _streams = r._streams ;
        _mode = r._mode ;
        _seconds = r._seconds ;
        _bytes = r._bytes ;
        _saveLog = r._saveLog ;
        _compressOld = r._compressOld ;
        _appendToLast = r._appendToLast ;
        _notifyDetailFromFilterable = r._notifyDetailFromFilterable ;
        _allowStackTrace = r._allowStackTrace ;
        _formatter = r._formatter ;
        _formattedOnly = r._formattedOnly;
    }

    return *this ;
}

Application::LogRotationPolicy::~LogRotationPolicy() ACS_NOEXCEPT
{
    // PRQA S 4631 L2
    try {
        Lock lock(*this) ;
        delete _log ;

        //Does not delete the _formatter!
        //delete _formatter ;
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    // PRQA L:L2
}


bool Application::LogRotationPolicy::behaveLike(const LogRotationPolicy &lrp) const // PRQA S 4020
{
    // only significant behavioural parameters are used.
    if ( computeLogName() != lrp.computeLogName() ) { return false ; }
    if (_mode != lrp._mode) { return false ; }
    if (_seconds != lrp._seconds) { return false ; }
    if (_saveLog != lrp._saveLog) { return false ; }
    if (_compressOld != lrp._compressOld) { return false ; }

    return true ;
}

Application::LogRotationPolicy::Mode Application::LogRotationPolicy::mode() const ACS_NOEXCEPT // PRQA S 4120
{
    return _mode ;
}

void Application::LogRotationPolicy::mode(Mode m) ACS_NOEXCEPT // PRQA S 4121
{
    _mode = m ;
}

size_t Application::LogRotationPolicy::aging() const ACS_NOEXCEPT // PRQA S 4120
{
    return _seconds ;
}

void Application::LogRotationPolicy::aging(size_t seconds) ACS_NOEXCEPT // PRQA S 4121
{
    _seconds = seconds ;
}

streampos Application::LogRotationPolicy::size() const ACS_NOEXCEPT // PRQA S 4120
{
    return _bytes ;
}

void Application::LogRotationPolicy::size(std::streampos bytes) ACS_NOEXCEPT // PRQA S 2010, 4121
{
    _bytes = bytes ;
}

void Application::LogRotationPolicy::saveLog(unsigned int logs) ACS_NOEXCEPT // PRQA S 4121
{
    _saveLog = logs;
}

unsigned int Application::LogRotationPolicy::saveLog() const  ACS_NOEXCEPT // PRQA S 4120
{
    return _saveLog;
}

void Application::LogRotationPolicy::compressOld(bool compress) ACS_NOEXCEPT // PRQA S 4121
{
    _compressOld = compress;
}

bool Application::LogRotationPolicy::compressOld() const ACS_NOEXCEPT // PRQA S 4120
{
    return _compressOld;
}

void Application::LogRotationPolicy::appendToLast(bool append) ACS_NOEXCEPT // PRQA S 4121
{
    _appendToLast = append;
}

bool Application::LogRotationPolicy::appendToLast() const ACS_NOEXCEPT // PRQA S 4120
{
    return _appendToLast;
}

void Application::LogRotationPolicy::notifyDetailFromFilterable(bool v) ACS_NOEXCEPT // PRQA S 4121
{
    _notifyDetailFromFilterable = v ;
}

void Application::LogRotationPolicy::formattedOnly(bool v) ACS_NOEXCEPT // PRQA S 4121
{
    _formattedOnly = v ;
}

void Application::LogRotationPolicy::allowStackTrace(bool v) ACS_NOEXCEPT // PRQA S 4121
{
    _allowStackTrace = v ;
}

void Application::LogRotationPolicy::setFormatter(exMsgFmt *f) ACS_NOEXCEPT
{
    delete _formatter ;
    _formatter = f ;
}


void Application::LogRotationPolicy::reset()
{
    _timer.start() ;
}

bool Application::LogRotationPolicy::check()
{
    vector<exFilter *> filters ;
    return check(filters) ;
}

bool Application::LogRotationPolicy::forceRotation(const vector<exFilter *> &nf) // PRQA S 4020
{
    // force the rotation
    Lock lock(*this) ;

    rotateOldLogFiles(nf) ; // rotate old logs
    reset() ;

       if (not nf.empty()) {
        attachFilters(nf); // attach the filters to the policy
    }

    return true ; // notify the rotation was done
}

bool Application::LogRotationPolicy::check(const vector<exFilter *> &nf) // PRQA S 4020
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    Lock lock(*this) ;
    if (rotationNeeded()) { // rotation needed - policy driven: size and/or aging or whatever
        rotateOldLogFiles(nf) ; // rotate old logs
        reset() ;
        return true ; // notify the rotation was done
    }

    if (not nf.empty()) {
        attachFilters(nf); // attach the filters to the policy
    }

    return false ; // no rotation
}

bool Application::LogRotationPolicy::rotationNeeded()   // PRQA S 4020
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_log: " << boolalpha << _log) ;

    if (!_log) {// never opened
        return true ;
    }
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_mode: " << _mode << " _seconds: " << _seconds << " _bytes: " << _bytes  // PRQA S 3050 2
            << " _log->size(): " << _log->size()) ;
     switch (_mode) {
    case Aging:
        {
            if (isLogOlder(_seconds*1000) ) { // aging check        // PRQA S 3084, 4400
                ACS_LOG_PRIVINFO("Rotating log because it lasted more than " << _seconds << " [sec]") ;
                return true ;
            }
            break ;
        }
    case Size:
        {
            if (( _bytes != streampos(0) ) && (_log->size() > _bytes) ) { // size check // PRQA S 3050, 3081
                ACS_LOG_PRIVINFO("Rotating log because it is greater than " << _bytes << " [bytes]") ; // PRQA S 3050
                return true ;
            }
            break ;
        }
    case FirstApplying:
        {
            if ( (_bytes != streampos(0)) && (_log->size() > _bytes) ) { // size overpassed ?   // PRQA S 3050, 3081
                ACS_LOG_PRIVINFO("Rotating log because it is greater than " << _bytes << " [bytes]") ; // PRQA S 3050
                return true ;
            }
            if ( ( 0 != _seconds ) && _timer.isOlder(_seconds*1000)) { // time elapsed ?    // PRQA S 3081, 3084, 4400
                ACS_LOG_PRIVINFO("Rotating log because it lasted more than " << _seconds << " [sec]") ;
                return true ;
            }
            break ;
        }
    case Daily:
        {
            struct timeval tv ;         // PRQA S 4102 3
            struct timezone tz ;
            struct tm theDate;
               gettimeofday(&tv, &tz);
            gmtime_r(&tv.tv_sec, &theDate);
            if ( (theDate.tm_mday != _logCreationDate.tm_mday) )
            {
                ACS_LOG_PRIVINFO("Rotating log because the date changed") ;
                return true ;
            }
            break;
        }
    }

    return false ;
}

bool Application::LogRotationPolicy::isLogOlder(Timer::Delay ms) // PRQA S 4020
{
// multiple return to improve readability, there are a number of independent conditions to
// exit with a false or true response
    if (0 == ms) { return false ; } // no timeout: no expiration

    if (_timer.isOlder(ms)) { return true ; }

    if ((nullptr != _log) && (_log->creationTime() + time_t(ms/1000) < ::time(nullptr))) {
        return true ;
    }

    return false ;
}

void Application::LogRotationPolicy::compress(const std::string &file) // PRQA S 4211
{
    // shoot and forget in order to avoid zombies until next Spawn
    string command = "gzip -f " ;
    command += file + " &" ;
    system(command.c_str()) ;
}


void Application::LogRotationPolicy::attachFilters(const std::vector<exFilter *> &f)  // PRQA S 4211
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "attaching " << f.size() << " filter(s)") ;
    if (_log && not f.empty()) { // is there any log ?
        _log->installFilters(f) ; // install the given filters
    }
}

bool Application::LogRotationPolicy::closeAndRotateLog() // PRQA S 4211
{
    bool ret = true ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "start rotation on " << _saveLog << " log files ") ;

    string previousLogName = _currentLogName;

    for (int i=_saveLog; i>=1 ; --i) {  // each log to save   // PRQA S 3000

        // move aaa.{x} to aaa.{x+1}
        // as in foo.1 -> foo.2
        ostringstream from ;
        from << _currentLogName << "." << i ;
        ostringstream to ;
        to << _currentLogName << "." << i+1 ;

        string fromFN = from.str() ;
        string toFN = to.str() ;

        try { // do it
            if (File::exists(fromFN)) { // uncompressed ?
                File::rename(fromFN, toFN) ;
                if (_compressOld) { // to be compressed ?
                    compress(toFN); // compress old file
                }
            }
            else { // was not found in uncompressed mode
                fromFN += ".gz" ;
                toFN += ".gz" ;

                if (File::exists(fromFN)) { // maybe it was compressed
                    File::rename(fromFN, toFN) ;
                }

            }
        }
        catch(exception &e) {
            ret = false ;
            ACS_LOG_TRYCATCHNOTIFY_EX(exIOException(e,"Log rotation: moving \""+ fromFN + "\" to \"" + toFN)) ; // PRQA S 3081
        }
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_log: " << _log << " calling rotate") ;
    if (_log) { // close and rotate
        struct timeval tv ;         // PRQA S 4102 2
        struct timezone tz ;
           gettimeofday(&tv, &tz);
        char timebuf[128] ;        // PRQA S 4403
        ostringstream ostime ;
        ostime << ctime_r(&tv.tv_sec,timebuf) ;
        ACS_LOG_PRIVINFO("Logfile closed at " << ostime.str()) ;
        ostringstream rmsg ;
        rmsg << "Old log rotation completed successfully at " << ostime.str() ;
        // Save information on the time when the current log has been opened
        gmtime_r(&tv.tv_sec, &_logCreationDate);

        // apply any pending change on the append mode
        _log->appendIfExists (_appendToLast);

        _currentLogName = computeLogName();

        if (previousLogName == _currentLogName)
        {
            // Must rotate AND the new name is the same of the previous
            // means that the application is trying to replace the log
            // use the configured rolling configuration to keep the number of foreseen logs
            _log->rotate(_currentLogName+".1", rmsg.str()) ;
        }
        else
        {
            // A new filename has been generated
            // Try to open it according to the current settings on the append flag
            _log->rotateOnNewFile (_currentLogName, rmsg.str()) ;
        }

    }
    else { // rotate the xxx.log (because was existent)
        if (File::exists(_currentLogName)) { // uncompressed ?
            File::rename(_currentLogName, _currentLogName + ".1") ;
        }
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "removing " << previousLogName << "." << _saveLog+1) ; // PRQA S 3084
    // remove the last logfile
    if (ret) {
        ostringstream to ;
        to << previousLogName << "." << _saveLog+1 ; // PRQA S 3084
        if (File::exists(to.str())) {
            try {
                File::unlink(to.str()) ;
            }
            catch(exception &e) {
                ret = false ;
                ACS_LOG_TRYCATCHNOTIFY_EX(exIOException(e,"Log rotation: removing \""+ to.str() )) ; // PRQA S 3081
            }
        }
        else if (File::exists(to.str()+".gz")) {
            try {
                File::unlink(to.str()+".gz") ;
            }
            catch(exception &e) {
                ret = false ;
                ACS_LOG_TRYCATCHNOTIFY_EX(exIOException(e,"Log rotation: removing \""+ to.str()+".gz" )) ; // PRQA S 3081
            }
        }
        else {
            // nothing
        }
    }

    if (!ret) {
        ACS_LOG_WARNING("Old log rotation had problems check old logs if any") ;
    }
    return ret ;
}

string Application::generateLogName(struct tm const & time) const
{
    // Use the default log name
    return getDefaultLogName();
}

void Application::LogRotationPolicy::setLogName (std::string const & name)
{
    Lock lock(*this);
    _logName = name;
}

std::string Application::LogRotationPolicy::getCurrentLogName () const ACS_NOEXCEPT
{
    Lock lock(*this);
    return _currentLogName;
}

void Application::LogRotationPolicy::forceLogName (bool f) ACS_NOEXCEPT
{
    Lock lock(*this);
    _forceLogName = f;
}

void Application::LogRotationPolicy::forceLogName (const std::string& logname) ACS_NOEXCEPT
{
    Lock lock(*this);
    _logName = logname;
    _forceLogName = true;
}

bool Application::LogRotationPolicy::forceLogName () const ACS_NOEXCEPT
{
    Lock lock(*this);
    return _forceLogName;
}

string Application::LogRotationPolicy::computeLogName () const
{
    Lock lock(*this);
    string retval("");
    if ( (not _forceLogName) && (Application::instance()->getLogNameFromApplication()) )
    {
        // Name is not forced AND application wants to decide the log name
        retval = Application::instance()->generateLogName (_logCreationDate);
    }
    else
    {
        // Name is forced OR application does not want to decide the log name
        retval = _logName;
    }
    return retval;
}

void Application::LogRotationPolicy::rotateOldLogFiles(const vector<exFilter *> &newFilters)
{
    Lock lock(*this) ;
    string tooBigSaveLog ;
    // if the number of log to save is too high, prepare a message to write
    if (_saveLog > _saveLogMax) {
        ostringstream os ;
        os << "Maximum number of log saved is " << _saveLogMax ;
        _saveLog = _saveLogMax ;
        tooBigSaveLog = os.str() ;
    }

    // set a flag to remember to log the open message, the flag is true only if there
    // was no log rotation until now from this process
    bool openMessage = (_log == nullptr) ;
    // bool oldlog = false ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_appendToLast: " << boolalpha << _appendToLast) ;

    // if there was no need to append this log to the last one the standard closure and
    // compression takes place -- note _appendToLast is true only for the very first log

    if (!_log) {
        // no log until now, means this is ther very first call
        // here the log is opened (again if applies)
        // and the append flag is passed through

        struct timeval tv ;     // PRQA S 4102 2
        struct timezone tz ;
           gettimeofday(&tv, &tz);
        gmtime_r(&tv.tv_sec, &_logCreationDate);

        _currentLogName = computeLogName();

        // if not requested to append must try the log rotation
        if (! _appendToLast)
        {
            closeAndRotateLog() ;
        }

        if (_streams.empty())  {
            _log = new exLogStream ("applog",_currentLogName, _appendToLast) ;
        }
        else {
            _log = new exLogStream ("applog",_currentLogName,_streams,_appendToLast) ;
        }

        _log -> notifyDetailFromFilterable(_notifyDetailFromFilterable) ;
        _log -> allowStackTrace(_allowStackTrace) ;

        if (_formattedOnly)
        {
            _log->formattedOnly();
        }

        if(_formatter)
        {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Formatter parameters are "
                    << " emitPid = " <<  _formatter ->emitPid()
                    << " emitTime = " <<  _formatter ->emitTime()
                    << " startTag = " <<  _formatter ->startTag()
                    << " endTag = " <<  _formatter ->endTag()
                    << " separator = " <<  _formatter ->separator()) ;
            _log->setFormatter( _formatter ) ;
        }
        // the log was successfully opened
        // install all filters
        if (not newFilters.empty()) {
            attachFilters(newFilters) ;
        }

        if (_appendToLast)
        {
            // Check if the just appended file needs to be rotated
            check (newFilters);
        }
    }
    else
    {
        // The log already exists.. just rotate it
        closeAndRotateLog();
    }

    // the openMessage flag is true only if this is the first time (see above)
    if (openMessage) {
        struct timeval tv ;     // PRQA S 4102 2
        struct timezone tz ;
           gettimeofday(&tv, &tz);
        char timebuf[128] ;     // PRQA S 4403
        ACS_LOG_PRIVINFO("Logfile opened at "<< ctime_r(&tv.tv_sec,timebuf)) ;
    }

    // a too big message is recorded if it was prepared, see at the beginning.
    if (!tooBigSaveLog.empty()) {
        ACS_LOG_WARNING(tooBigSaveLog) ;
    }

    // reset append to last flag
    // if application will restart and will set this flag, the reset has no effect,
    // else if it keep running, the reset will cause the standard log rotation policy

    // Using the log existence to detect if must rotate or not
    // _appendToLast = false ;
}













//**************** Application *********************************
bool Application::_allowPostLog = true ;

//    **** Public Members ****
Application::~Application() ACS_NOEXCEPT
{
// PRQA S 4631 L1
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

    delete stopManagerThread_;
    delete _preLog ;
    delete _applicationDebug ; // there was a debug ? Delete it

    _instance=nullptr; // no more application instances (ther can be only one at time)
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "exiting") ;
// PRQA L:L1
}

int Application::usage(const std::string &exename) const
{
    ACS_LOG_WARNING("\" virtual int " << appName() << "::usage ( const std::string & ) const\" method should be reimplemented.") ;
    return 1 ;
}


int Application::internalusage(const std::string &exename) const
{
    // remove statistics - they are boring in case of usage
    Statistics::instance()->enableStatistics(false) ;

    int ret = usage(exename) ;
    excerr << "\n\tgeneral options:\n" ;
    excerr << "\t--stat    : disable statistics output at the end of execution\n" ;
    excerr << "\t--logtime : emit time when logging (unless application overrides it)\n" ;
    excerr << "\t--nodaemon: no daemonize (for daemons)\n" ;
    excerr << "\t--instance <instance #>: sets the instance # for multi instance apps (0-relative)\n" ;
    excerr << "\t--confOverrideFpath <filepath>: load in configuration space the content of input file\n" ;
    excerr << "\t--help   : this help\n" ;
    excerr << "\n" ;
    return ret ;
}

bool Application::parseArgs(vector<string> &parsedArgs,  // PRQA S 4020
                           int argc,
                           char const * const * argv,
                           char const * const * env)
{
    parsedArgs.clear() ;

    const string stat =      "-stat" ;
    const string stat2 =     "--stat" ;
    const string logtime =   "-logtime" ;
    const string logtime2 =  "--logtime" ;
    const string nodaemon =  "-nodaemon" ;
    const string nodaemon2 = "--nodaemon" ;
    const string help =      "-help" ;
    const string help2 =     "--help" ;
    const string instance =  "--instance" ;
    const string conFpath =	 "--confOverrideFpath";

    if (argc > 1) { // check application command line arguments
        // check if -nodaemon or -autoRestart was given
        for (int i=1; i < argc; ++i) { // PRQA S 4235, 4236
            if (help == argv[i] || help2 == argv[i]) {
                return true ;
            }
            else if (nodaemon == argv[i] || nodaemon2 == argv[i]) { // -nodaemon
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "daemonize not allowed") ;
                _allowDaemonize = false ;
            }
            else if (autoRestart == string(argv[i]).substr(0,autoRestart.length()) ) { // -autoRestart (given if the application has been restarted -- see phoenix)  // PRQA S 3081
                // tricky part: cast to a Status
                restartStatus_ = Status(atoi(string(argv[i]).substr(autoRestart.length()).c_str())) ; // PRQA S 3013, 3081
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "restarted (implies no daemonize)"
                        << " restartStatus_: " << toString(restartStatus_)) ;
                _allowDaemonize = false ;
                _restarted = true ;
                setStatus(restartStatus_) ;
            }
            else if (stat == argv[i] || stat2 == argv[i]) { // -dbstat
                Statistics::instance()->enableStatistics(false) ;
            }
            else if (logtime == argv[i] || logtime2 == argv[i]) { // -logtime - will emit time when logging
                exMsgFmtFactory::instance()->emitTime(true) ;
            }
            else if (instance == argv[i]) {
                if (i < argc-1) {
                    char *endptr = nullptr ;
                    long instance_No = strtol(argv[++i], &endptr, 10) ;
                    if (*endptr != '\0' ) { // invalid number

                        ACS_LOG_WARNING("Unrecognized: " << argv[i]) ;
                    }
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "instance_No: " << instance_No) ;
                    setInstance(instance_No) ;
                    instanceNoArgToBeAdded_ = false ;
                }
                else { // no more args after instance
                    ACS_LOG_WARNING("No number after --instance: ignored arg") ;
                }
            }
            else if (conFpath == argv[i]) {
                if (i < argc-1) {
                    confOverrideFilepath_ = argv[++i];
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filepath to override configuration: \"" << confOverrideFilepath_ << '\"') ;
                }
                else { // no more args after instance
                    ACS_LOG_WARNING("No number after --confOverrideFpath: ignored arg") ;
                }
            }
           else {
                parsedArgs.push_back(argv[i]) ; // push args
            }
        }
    }
    return false ;
}


long Application::setInstance(long inst_no)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "setting instance no: " << inst_no << " actual: " << instanceNo_) ;
    if ((instanceNo_ == -1) && (inst_no >= 0) ) {
        instanceNo_ = inst_no ;
    }
    return instanceNo_ ;
}

void Application::createDefaultLRP ()
{

    // Prepare the project defined default LogRotationPolicy
    // Any app can override this settings by setting a different LRP on their main

    vector<exostream *> streams ;
    streams.push_back(&excerr) ;
    streams.push_back(&excout) ;

    _logRotationPolicy = LogRotationPolicy (
        getDefaultLogName(),
        streams,
        getDefaultRotationMode(),
        getDefaultRotationLogAgeSec(),
        getDefaultRotationLogSize(),
        getDefaultRotationLogNumToSave(),
        getDefaultRotationLogCompressOld(),
        getDefaultRotationAppend()
    );

#ifdef ACS_CREATE_MC_LOG_FILES

    // APF-267 - monitoring and control log rotation
    string mc_log = defineMCLogName(_logRotationPolicy.computeLogName()) ;

    LogRotationPolicy mc_lrp (
        mc_log,
        getDefaultRotationMode(),
        getDefaultRotationLogAgeSec(),
        getDefaultRotationLogSize(),
        getDefaultRotationLogNumToSave(),
        getDefaultRotationLogCompressOld(),
        getDefaultRotationAppend()
    );

    // do not use the logname passed by the application
    mc_lrp.forceLogName (true);
    _mclogRotationPolicy = mc_lrp ;
    _mclogRotationPolicy.reset() ;

#endif //ACS_CREATE_MC_LOG_FILES

}

int Application::run(const int argc, char const * const * argv, char const * const * env) // PRQA S 4020
{
    exAdaptiveFilter filter ; // default installs over exostream::getDefaultOutLogger() - see exAdaptiveFilter.h
    filter.allowPrivateInfo(false) ; // remove PrivateInfo from exostream::getDefaultOutLogger()


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to obtain instance pointer.. ") ;

    Application * app =dynamic_cast<Application*>( Application::instance() );   // PRQA S 3081
    ACS_COND_THROW(app == nullptr,Application::Exception("The Application has not been instantiated.")); // PRQA S 3081

    // create the default LRP according to the derivated application class
    // the actual log will be created at the first rotateLog call (by app itself or loadConfigration)
    app->createDefaultLRP();


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_lock " << app->_lock << " " << &app->_lock) ;

    Statistics::instance()->enableStatistics(true) ;

    vector<string> parsedArgs ;
    bool help = app->parseArgs(parsedArgs, argc, argv, env) ;

    if (help) {
        return app->internalusage(argv[0]) ;
    }

    app->setParsedArgs(parsedArgs) ;

    int argc_ = 0 ; // local
    if(not app->_restarted) // has not been restarted till now ?
    {
        // prepare args in case of restart (a -autoRestart arg will be added)
        argv_ = new  char *[ argc_ = (argc + 2 ) ] ; // ... reset status            // PRQA S 3000, 4403
        argv_[ argc_ - 1 ] = 0 ;
        ostringstream os ; os << int(restartStatus_) ;                              // PRQA S 3081
        argv_[ argc_ - 2 ] = strdup( (autoRestart+os.str()).c_str() ) ;             // PRQA S 4400
    }
    else // copies ... to use when restarting again
    {
        argv_ = new  char *[ argc_ = (argc + 1)] ;                                  // PRQA S 3000
        argv_[ argc_ - 1 ] = 0 ;
    }


    if (argv[0][0] != '/') //relative path or just executable name
    {
        string argvString(argv[0]);
        //relative path ?
        if( argvString.find('/')  != argvString.npos )
        {
            string abspath = File::getCurrentPath() + "/" + argv[0]  ;
            argv_[0] = strdup(abspath.c_str()) ;
        }
        else
        {
            argv_[0] = strdup(argv[0]) ;
        }
    }
    else {
        argv_[0] = strdup(argv[0]) ;
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "argv_[0]: " << argv_[0]) ;
    for (int i=1; i < argc; ++i) {
        argv_[i] = strdup(argv[i]) ;
    }

    //***************************************************

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "there are " << PostLogHooks::instance().size() << " hooks registered") ;

    // application debug stuff ;
    app->debugActivation(argc, argv, env) ;

    if( not app->confOverrideFilepath_.empty() ) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to override configuration with \"" << app->confOverrideFilepath_ << "\" file");
        ConfigurationSingleton::instance() -> addOverrideFilepath (app->confOverrideFilepath_, ConfigurationSingleton::LoadLazy);
    }


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "application pointer is " << app
            <<" . I'm going to run start method."
            << " app->_lock is " << app->_lock) ;
    // start: the real application CORE !!!!
    int ret = app ->start(argc, argv, env);
    restartStatus_ = app->status_ ; // store the status in case of restart


    if(not app->_restarted) // has not been restarted till now ?
    {
        // prepare args in case of --instance if it has to be added (not given from outside)
        bool add = app->instanceNoArgToBeAdded_ && (app->instanceNo_ != -1) ;
        if (add) {
            char ** nargv = new char *[ argc_ + 2 ] ; // ... reset status            // PRQA S 3000, 4403

            nargv[ argc_ + 1 ] = 0 ;
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "instanceNo_ " << app->instanceNo_ ) ;
            nargv[ argc_ - 1 ] = strdup("--instance") ;                     // PRQA S 4400
            ostringstream osi ; osi << int(app->instanceNo_) ;              // PRQA S 3081
            nargv[ argc_ ] = strdup( osi.str().c_str() ) ;             // PRQA S 4400
            for (int i = 0; i < argc_-1; ++i) {
                nargv[i] = strdup(argv_[i]) ;
            }
            delete [] argv_ ;
            // swap nargv & argv_
            argv_ = nargv ;
            argc_+= 2 ;

        }

    }

    // check argv_
    for (int i=0; argv_[i] ; ++i) { // PRQA S 4231, 4235
        if (autoRestart == string(argv_[i]).substr(0,autoRestart.length()) ) { // -autoRestart (given if the application has been restarted -- see phoenix)  // PRQA S 3081
            ostringstream os ; os << int(restartStatus_) ;  // PRQA S 3081
            free(argv_[i]) ;
            argv_[i] = strdup( (autoRestart+os.str()).c_str() ) ;
        }
    }


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "start returned " << ret <<" . Going to call postLogMessage. ") ;

    // something to log post execution ?
    postLogMessage() ;

    // Emit statistics in any case
    app->dumpStatistics();

    // Shutdown EventNotificationFactory (statistics collectors) 
    EventNotifyFactorySingleton::instance()->shutdown();

    // and finally return the value
    return ret ;
}

const vector<string> & Application::getParsedArgs() const ACS_NOEXCEPT { return args_ ; } // PRQA S 4120
void Application::setParsedArgs(const vector<string> &v ) { args_ = v ; } // PRQA S 4121

bool Application::mustRestart() const ACS_NOEXCEPT // PRQA S 4120
{
    return Application::PhoenixRestart == getCurrentStopMode();
}


void Application::restart()
{
    // opened file closure are left to the operating system
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
        ostringstream dbg ;
        dbg << "the process will be restarted with the following args:\n";
        char **a = argv_ ;
        for (int i=0;*a;++i,++a) { // PRQA S 4231
            dbg << "argv[" << i << "] \"" << *a << "\"\n" ;
        }
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, dbg.str()) ;
    ACS_CLASS_END_DEBUG
#if __GNUC__ < 4
    pthread_kill_other_threads_np() ;
    #warning "added pthread_kill_other_threads_np call not portable"
#endif
    {
        // close all open files
        struct rlimit rlim ; // PRQA S 4102
        getrlimit (RLIMIT_NOFILE, &rlim);

        rlim_t maxfd = max(rlim.rlim_cur, rlim.rlim_max) ;


        {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "closing " << maxfd << " descriptors") ;
            // close all opened file descriptors
            for (rlim_t i=3; i <= maxfd; ++i) {
                ::close(i) ; // PRQA S 3000, 3010
            }
        }
    }

    // hot to restart ?? exec family function of course
    if(execvp(argv_[0],argv_) == -1)
    {
        ACS_THROW(Application::Exception("Application::restart execvp returned with error" , errno)) ;
    }
}


string Application::appName()   // PRQA S 4020
{
    try {
        return Application::instance()->applicationName() ;
    }
    catch (std::exception &x) {
        return "Unnamed" ;
    }
}



const string &Application::applicationName() const    // PRQA S 4120
{
    return _appName ;
}


void Application::subSystem(const string &ss)     // PRQA S 4121
{
    _subSystem = ss ;
}


const string &Application::subSystem() const      // PRQA S 4120
{
    return _subSystem ;
}


void Application::setLRPolicy(const LogRotationPolicy &lrp)
{
    _logRotationPolicy = lrp ;
    _logRotationPolicy.reset() ;

#ifdef ACS_CREATE_MC_LOG_FILES

    // APF-267 - monitoring and control log rotation
    string mc_log = defineMCLogName(lrp.computeLogName()) ;

    LogRotationPolicy mc_lrp(mc_log, lrp.mode(), lrp.aging(), lrp.size(), lrp.saveLog(), lrp.compressOld(), lrp.appendToLast() );
    // do not use the logname passed by the application
    mc_lrp.forceLogName (true);
    _mclogRotationPolicy = mc_lrp ;
    _mclogRotationPolicy.reset() ;

#endif //ACS_CREATE_MC_LOG_FILES

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Calling rotateLog..");
    rotateLog() ;
}


Application::LogRotationPolicy & Application::getLRPolicy()      // PRQA S 4120
{
    return _logRotationPolicy ;     // PRQA S 4024
}

const Application::LogRotationPolicy & Application::getLRPolicy() const       // PRQA S 4120
{
    return _logRotationPolicy ;
}

Application::LogRotationPolicy & Application::getMCLRPolicy()      // PRQA S 4120
{
    return _mclogRotationPolicy ;     // PRQA S 4024
}

const Application::LogRotationPolicy & Application::getMCLRPolicy() const       // PRQA S 4120
{
    return _mclogRotationPolicy ;
}

void Application::updateDebug()  // PRQA S 4211
{
    if (_applicationDebug) {
        _applicationDebug->updateDebug() ;
    }
}

// returns the default log size when rotation occur
std::streampos Application::getDefaultRotationLogSize() const { return 1 << 20 ; } // 1 MegaByte // PRQA S 4400

// returns the default log age when rotation occur
size_t Application::getDefaultRotationLogAgeSec() const { return defaultLogAge ; }

// returns the default log num to save
unsigned int Application::getDefaultRotationLogNumToSave() const { return 3 ; } // PRQA S 4400

// returns the default log num to save
bool Application::getDefaultRotationLogCompressOld() const { return true ; }

// returns if logs appends by default
bool Application::getDefaultRotationAppend() const { return false; }

// returns if logs appends by default
Application::LogRotationPolicy::Mode Application::getDefaultRotationMode() const { return LogRotationPolicy::Aging; }

// returns if log name override is enabled by default
bool Application::getLogNameFromApplication() const { return false; }

// returns the overriden filename for the log file
std::string Application::getDefaultLogName() const
{
    ostringstream os;
    os << defaultLogPath << "/" << applicationName() << ".log";
    return os.str();
}




bool Application::rotateLog()
{
    vector <exFilter *> nf ;
    return rotateLog(nf) ;
}

bool Application::rotateLog(exFilter &f)
{
    vector <exFilter *> nf ;
    nf.push_back(&f) ;
    return rotateLog(nf) ;
}


bool Application::rotateLog(const vector<exFilter *> &nf)
{
    ACS_COND_THROW(_logRotationPolicy.computeLogName().empty(),exIllegalValueException("Logfile name cannot be the empty string")) ; // PRQA S 3081

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "with " << nf.size() << " filters") ;
    // check and apply filters

    string prelog ;
    try {
        if (_preLog) {
            prelog = _preLog->str().str() ;
            delete _preLog ;
            _preLog = nullptr ;
        }
    }
    catch(exception &) {
        // nothing to do ;
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to check _logRotationPolicy" );
    bool ret = _logRotationPolicy.check(nf) ;

#ifdef ACS_CREATE_MC_LOG_FILES
    if (ret)
    {
        vector <exFilter*> mc_nf;
        mc_nf.push_back (&_mcfilter);
        _mclogRotationPolicy.forceLogName ( defineMCLogName(_logRotationPolicy.getCurrentLogName()) );
        _mclogRotationPolicy.forceRotation(mc_nf);
    }
#endif //ACS_CREATE_MC_LOG_FILES
    bool silenced = false ;
    try {
        if ((&exostream::getDefaultOutLogger() == &excerr) || (&exostream::getDefaultOutLogger() == &excout)) {
            if (exostream::getDefaultOutLogger().mode() == exostream::Verbose) {
                exostream::getDefaultOutLogger().silent() ;
                silenced = true ;
            }
        }
        if (not prelog.empty()) {
            ACS_LOG_PRIVINFO("messages issued before log creation:") ;
            ACS_LOG_PRIVINFO("====================================") ;
            exostream::getDefaultOutLogger() << prelog ;
            ACS_LOG_PRIVINFO("====================================") ;
            ACS_LOG_PRIVINFO("end of messages issued before log creation:") ;
        }
        if (silenced) { exostream::getDefaultOutLogger().verbose() ; }
    }
    catch(exception &x) {
        if (silenced) { exostream::getDefaultOutLogger().verbose() ; }
        ACS_LOG_NOTIFY_EX(x) ;
    }

    return ret ;
}

string Application::defineMCLogName(const string& inName) const ACS_NOEXCEPT
{

    string oldName = File::getFileName(inName);

    string newName = defaultMCLogPath + "/" + File::removeExt(oldName) + ".mc.log";

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MC Default Log Name " << newName) ;

    return newName;
}

void Application::debugMode(int argc, char const * const * argv, char const * const * env, std::string &debugPath) const
{
    debugPath = "" ;
}


//     **** Configuration related Members ****

void Application::addSpace(                             // PRQA S 4214
    const ConfigurationSingleton::ConfSpace &c,
    ConfigurationSingleton::SpaceLoadMode mode,
    const ConfigurationSingleton::StopPredicate &stop
) const
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "adding space \"" << c.name() << "\"") ;
    ConfigurationSingleton::instance()->addSpace(c, mode, stop) ;
}


void Application::loadConfiguration(                    // PRQA S 4214
    bool notifyReplacementException,
    const ConfigurationSingleton::StopPredicate &stop
)
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    // load the configuration with the stop predicate - useful to stop the loop if a stop condition occurs
    ConfigurationSingleton::instance()->loadConfiguration(stop) ;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "replacements number: " << ConfigurationSingleton::instance()->getReplacements().size()) ;

    if (not ConfigurationSingleton::instance()->getReplacements().empty()) {
        ostringstream os ;
        os << "The following configuration spaces has been replaced:\n" ;
        for (unsigned long i =0; i < ConfigurationSingleton::instance()->getReplacements().size(); ++i)
        {
            os << "\"" << ConfigurationSingleton::instance()->getReplacements()[i].first
               << "\" with \""
               << ConfigurationSingleton::instance()->getReplacements()[i].second << "\"\n" ;
        }

        if (notifyReplacementException)
        {
            ACS_LOG_TRYCATCHNOTIFY_EX(Exception(os.str())) ; // PRQA S 3081
        }
        else {
            ACS_LOG_WARNING(os.str()) ;
        }
    }

    // ADC-224
    // Check if log rotation policies must be replaced after the configuration load
    if (rotateLogParametersFromConf_)
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Overriding log rotation policies from configuration") ;
        LogRotationPolicy theLRP = getLRPolicy();

        // Starting from CSGACQ-84 the application has its own default configuration
        // for what concerns the Log Rotation Policy
        // There is no need to prepare the LRP before trying to update it from the configuration spaces

        // Set the new LRP only if a suitable configuration has been found
        bool configurationChanged = false;

        static const string LRPTag ("DaemonLogRotationPolicy");

        rsResourceSet& rs = ConfigurationSingleton::instance()->get();

        // Application specific settings have the priority wrt the
        if (rs.hasKey (appName() + "." + LRPTag))
        {
            getLRPFromConfiguration(theLRP, appName() + "." + LRPTag);
            configurationChanged = true;
        }
        else if (rs.hasKey (LRPTag))
        {
            getLRPFromConfiguration(theLRP, LRPTag);
            configurationChanged = true;
        }
        else
        {
            // No configuration found.. it is not an error or a warning. Just notify it one time.
            static bool notified = false;
            if ( ! notified )
            {
                 ACS_LOG_PRIVINFO ("No rotation policy configuration found in conf spaces. Going with existing settings.");
                 notified = true;
            }
        }

        if (configurationChanged)
        {
            if (not theLRP.behaveLike(getLRPolicy())) {
                // Dump the log rotation policy configuration
                ACS_LOG_PRIVINFO ("Log rotation settings from configuration: mode=" << theLRP.mode() // PRQA S 3050
                           << " age=" << theLRP.aging()
                           << " size=" << theLRP.size()
                           << " oldLogs=" << theLRP.saveLog()
                           << " compressed=" << boolalpha << theLRP.compressOld());
            }
            // Apply new settings and then rotate if needed
            setLRPolicy (theLRP);
        }

    }

    loadConfigurationInternal();

    configurationLoaded() ;
}

void  Application::getLRPFromConfiguration(LogRotationPolicy& lrp, const std::string& rootTag) const // PRQA S 4214
{
    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
    rsPushNSpace myPusher (rs, rootTag);

    // LRP Mode section
    if ( rs.hasKey ("Mode") )
    {
        string mode;
        rs.getValue ("Mode", mode);
        if ("AGING" == mode)
        {
            lrp.mode( LogRotationPolicy::Aging );
        }
        else if ("SIZE" == mode)
        {
            lrp.mode( LogRotationPolicy::Size );
        }
        else if ("FIRST_APPLY" == mode)
        {
            lrp.mode( LogRotationPolicy::FirstApplying );
        }
        else if ("DAILY" == mode)
        {
            // Daily case introduced by CSGACQ-84
            // In case of daily logs must append to the current day log if any
            lrp.mode( LogRotationPolicy::Daily );
            lrp.appendToLast (true);
        }
        else
        {
            ACS_LOG_WARNING ("Unrecognized log rotation mode " << mode << " in namespace " << rootTag << ". Using FIRST_APPLY instead.");
            lrp.mode( LogRotationPolicy::FirstApplying );
        }
    }
    else
    {
        ACS_LOG_WARNING ("Log rotation mode not found in configuration. Using default.");
    }

    // LRP Aging section
    if ( rs.hasKey ("AgingThreshold") )
    {
        size_t age = 0 ;
        rs.getValue ("AgingThreshold", age);
        if (age > 0 ) {
            lrp.aging(age);
        }
        else {
            ACS_LOG_WARNING ("Wrong log rotation aging threshold (0). Using default.");
        }
    }
    else
    {
        ACS_LOG_WARNING ("Log rotation aging threshold not found in configuration. Using default.");
    }

    // LRP Aging section
    if ( rs.hasKey ("SizeThreshold") )
    {
        size_t sizets = 0 ;
        rs.getValue ("SizeThreshold", sizets); // read as unsigned
        if (sizets > 0 ) {
            lrp.size(sizets); // PRQA S 3000
        }
        else {
            ACS_LOG_WARNING ("Wrong log rotation sizing threshold (0). Using default.");
        }
    }
    else
    {
        ACS_LOG_WARNING ("Log rotation sizing threshold not found in configuration. Using default.");
    }

    // Save log section
    if ( rs.hasKey ("OldLogsToKeep") )
    {
        unsigned int num = 0 ;
        rs.getValue ("OldLogsToKeep", num);
        lrp.saveLog(num); // allows 0
    }
    else
    {
        ACS_LOG_WARNING ("Log rotation number of logs to save not found in configuration. Using default.");
    }

    // Compress log section
    if ( rs.hasKey ("CompressOldLogs") )
    {
        bool compress = false ;
        rs.getValue ("CompressOldLogs", compress);
        lrp.compressOld(compress);
    }
    else
    {
        ACS_LOG_WARNING ("Log rotation old logs compression setting not found in configuration. Using default.");
    }

}

bool Application::setHostnameSpaceMandatory(bool v) const   // PRQA S 4214
{
    return ConfigurationSingleton::instance()->setHostnameSpaceMandatory(v) ;
}

//     **** Protected Members ****


void Application::debugActivation(int argc, char const * const * argv, char const * const * env)
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

    if (_applicationDebug) { // any application debug class ?
        try {
            delete _applicationDebug ; // bye bye
        }
        // It is safe to catch ... because it is called by main thread
        catch(...) {
            ;
        }
        _applicationDebug = nullptr ;
    }
    string p ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "p: \"" << p << "\"") ;
    // ask for the debug mode
    debugMode(argc, argv, env, p) ;
    // the apply it
    if (p.empty()) {
        _applicationDebug = new ApplicationDebug() ; // a new one ...
    }
    else {
        _applicationDebug = new ApplicationDebug(p) ; // a new one ...
    }
}

void Application::loadConfigurationInternal()
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

// get the optional Salt configuration intended to overwrite possible compile time parameters. Currently it is only used by: ACS_SSL::ACS_RSA_encode_password  and 	ACS_SSL::ACS_RSA_decode_password
    const rsResourceSet rs ( ConfigurationSingleton::instance()->get() ) ;

    const string key_salt_string ("SaltCipher.SaltString") ;
    const string key_salt_random ("SaltCipher.SaltRandom") ;
    if( (true == rs.hasKey(key_salt_string) ) || ( true == rs.hasKey(key_salt_random) )  )
    {
        size_t saltRandom (0) ;
        string saltString ;

        if( true == rs.hasKey(key_salt_string) ) { rs.getValue(key_salt_string, saltString); }

        if( true == rs.hasKey(key_salt_random) ) {
            string tmp ; rs.getValue(key_salt_random, tmp); saltRandom = ( tmp.empty() ? 0 : atoi(tmp.c_str()) ) ;
        }

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to set salt parameters: SaltString=\"" << saltString << "\" SaltRandom=" << saltRandom );
        ACS_SSL::setSalt( saltRandom, saltString) ;
    }


}

Application::Status Application::getCurrentStatus() const ACS_NOEXCEPT
{
    Lock lock(*this) ;
    Status status = status_;
    return status;
}  // PRQA S 4120

Application::Status Application::getCurrentStatus(std::string &r) const
{
    Lock lock(*this) ;
    r= statusReason_ ;
    Status status = status_;
    return status ;
}

bool Application::delayOrCondition(Timer::Delay totDelay, volatile int *cond, int contCond, Timer::Delay slice) // PRQA S 4020
{
    Timer timer ;
    timer.start() ;
    if (nullptr!=cond) {
        for (;*cond == contCond;Timer::delay(slice)) {
            timer.stop() ;
            if (timer.elapsed() >= totDelay) { return false ; }
        }
        return true ; // here because cond

    }
    else {
       Timer::delay(totDelay) ;
       return false ;
    }
}



Application::Status Application::setCurrentStatus(Status s, const string &reason) ACS_NOEXCEPT
{
    Lock lock(*this) ;

    Status p = status_ ;
    status_ = s ;
    statusReason_ = reason ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Software status set to: " << toString(status_) << " " // PRQA S 3380, 3385
            << (not statusReason_.empty() ? statusReason_ : "" )) ;

    return p ;
}

string Application::toString(Status s) ACS_NOEXCEPT  // PRQA S 4020
{
    try {
        switch(s) {
            case Initializing:  { return "INITIALIZING" ; }
            case Ok:            { return "OK" ; }
            case Warning:       { return "WARNING" ; }
            case Critical:      { return "CRITICAL" ; }
            case Exiting:       { return "EXITING" ; }
            default:
                {
                    ostringstream os ;
                    os << "Invalid Status: " << int(s) << " must be in [0, 4]" ; // PRQA S 3081
                    ACS_THROW(exBoundsException(os.str())) ; // PRQA S 3081
                }
                break ;
        }
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
        return toString(Warning) ;
    }
}



Application::Application(const string &appName, const string &subSystem) :
    ThreadSafe(),
    _restarted(false),
    _appName(appName),
    _allowDaemonize(true),
    isDaemon_(false),
    daemonizeCalled_(false),
    _subSystem(subSystem),
    _restartTime(-1),
    _restartTimer(),
    _logRotationPolicy( "", LogRotationPolicy::Aging, defaultLogAge, 0),
    _mclogRotationPolicy( "", LogRotationPolicy::Aging, defaultLogAge, 0),
    _lock(0),
    _uuid(),
    _applicationDebug(nullptr),
    _preLog(new exLogStringStream("bcklog")),
    _mcfilter(0),
    _connPool(nullptr),
    status_(restartStatus_),
    statusReason_(),
    args_(),
    rotateLogParametersFromConf_ (true),
    instanceNo_(-1),
    instanceNoArgToBeAdded_(true),
    confOverrideFilepath_(),
    currentStopMode_ (Application::NoStop),
    stopModeChanged_ (false),
    stopManagerThread_ (nullptr),
    condition_(),
    lastStopModeChangeTime_()
{
    // CTOR time is the last mode change time
    ::gettimeofday (&lastStopModeChangeTime_, nullptr);

    _logRotationPolicy.reset() ; // reset the log rotation policy
    _mclogRotationPolicy.reset() ; // reset the mc log rotation policy
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_instance = " << _instance
            << " _lock:" << _lock << " " << &_lock
            << " _restart:" << boolalpha << mustRestart()) ;

    if(_instance!=nullptr) // any instance till now ?
    {
        // not a GOOD THING
        string err = "Application::Application(...) : ";
        err+=" appName is \"" + appName + "\"  \n Already another application has been instantiated. ";
        err+=" The existing application name is \""+ _instance->applicationName() + "\".";
        try {
            ACS_THROW(Exception(err)); // transform into SEP (Someone Else Problem) // PRQA S 3081
        }
        catch(exception &x) {
            ACS_LOG_NOTIFY_EX(x) ;
            throw ; // rethrow it
        }
    }
    _instance=this; // THIS is the unique instance !!!

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_instance = " << _instance
            << " _restart:" << boolalpha << mustRestart()
            << " _lock:" << _lock << " " << &_lock) ;

    // Set the MQTT topic infrastructure
    MQTTHelper::setOrganization ("XPR");
    MQTTHelper::setSubsystem (_subSystem);
    MQTTHelper::setSubsystemId (_subSystem + "_01");
    MQTTHelper::setEntity (_appName);
    MQTTHelper::setEntityId (_appName + "_01");

    stopManagerThread_ = new ApplicationStopManager();
    stopManagerThread_->startFICT();
}

void Application::setLock(ApplicationLock *l) // PRQA S 4020
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "l: " << l
            << " _lock: " << _lock) ;
    // already a lock && l is not null and they are different ?
    // means: not yet set ? can be set ?
    if (_lock && l && (_lock != l)) {
        return ; // bye bye
    }

    _lock = l ; // this is the app lock
}

ApplicationLock const * const Application::getLock() const // PRQA S 4120
{
    return _lock ;
}


bool Application::isRestarted() const  // PRQA S 4120
{
    return _restarted ;
}


dbConnectionPool *Application::getDefaultConnectionPool() const  // PRQA S 4211
{
    Lock l(*this) ;
    return _connPool ; // PRQA S 4628
}

void Application::setDefaultConnectionPool(dbConnectionPool *cp)
{
    Lock l(*this) ;
    _connPool = cp ;
}


void Application::registerPostLog(PostLogMessage f)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "registering hook: " << f) ;
    // register the hook - in case it should already exist - nothing
    PostLogHooks::instance().registerHook(f) ;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "there are " << PostLogHooks::instance().size() << " hooks registered") ;
}

bool Application::unregisterPostLog(PostLogMessage f)
{
    return PostLogHooks::instance().unregisterHook(f) ;
}

bool Application::allowPostLog()
{
    return _allowPostLog ;
}

void Application::allowPostLog(bool f)
{
    _allowPostLog = f ;
}

void  Application::allowDaemonize(bool v)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called with " << boolalpha << v) ;
    _allowDaemonize = v ;
}

void Application::daemonize()
{
    daemonizeCalled_ = true ;

#ifdef DISABLE_DAEMONIZE

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "daemonize is disabled - _lock: " << _lock ) ;
    return ;

#else // not DISABLE_DAEMONIZE: daemonize is normally processed

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "calling preDaemonize() - _lock: " << _lock ) ;
    // execute any pre-daemonization code
    preDaemonize() ;

    // is daemonize allowed ?
    if (_allowDaemonize) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "daemonizing()") ;

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Stopping and deleting the stop manager thread") ;

        // Stop and delete the exit status manager
        ApplicationStopManager* theManagerPtr = stopManagerThread_;
        stopManagerThread_ = nullptr;
        delete theManagerPtr;

        if(::daemon(0,0)==-1) // do it !!!
        {
            ACS_THROW(Exception("Daemonize error" , errno)); // PRQA S 3081
        }
        isDaemon_ = true ;

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Daemonized. Restarting the stop manager thread") ;

        // Restart the status manager after the fork
        stopManagerThread_ = new ApplicationStopManager();
        stopManagerThread_->startFICT();
    }

    if (_lock) { // a new pid should be written here
        _lock->writePid() ;
    }

    // call post daemonize to allow inherited classes to reinitialize themselves
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "calling postDaemonize()") ;

    // execute any post-daemonization code
    postDaemonize() ;
#endif // not DISABLE_DAEMONIZE
}

void Application::postDaemonize()
{
}

void Application::preDaemonize()
{
}




int Application::start(int argc, char const * const * argv, char const * const * env)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_lock is " << _lock) ;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "checking system mode for: " <<  appName() << "." << subSystem()) ;

    try {
        // the system mode -- see ACS wiki for details
        SystemMode *sm = SystemModeFactory::newItem(appName(),subSystem()) ;
        SystemModeScopeGuard sg(sm) ;
        exRTDebug::instance()->mode(sm->load()) ;

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "system mode is: " << exRTDebug::instance()->mode()) ;
        // force the refresh
        debugActivation(argc, argv, env) ;
        if (_applicationDebug)  { // .. and the reload ...
            _applicationDebug->updateDebug() ;
        }
    }
    catch(SystemMode::ModeException &x) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "*** " << x) ;
         ; // ignore it
    }
    catch(exception &x) {
        ACS_LOG_WARNING(x << "The above exception has been ignored") ;
    }

    // The pthread_atfork call was here, on File::globalmutex but has been removed because
    // fork is threadsafe. The only not threadsafe bad guy to guard is popen/pclose

    // Unblock all the signals that a thread blocks by default
    // this is needed to allow the reception of SIGINT and SIGTERM (at least) in
    // case of phoenix forced by a timeout that occurs in the SignalManager thread
    // the sigmanager thread shall not mess with the signal masks relying on the base class defaults
    sigset_t newmask ; // PRQA S 4102
    // This will return the list of signals blocked by any new started thread
    Thread::maskedSignals(newmask) ;
    // it is allowed to unblock a non-blocked signal
    ::pthread_sigmask(SIG_UNBLOCK, &newmask, nullptr) ;

    // Time to go now
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to start main method.") ;

    // start the main of the application
    int res=startMain(argc, argv, env);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "startMain returned "<<res << " _lock is " << _lock) ;

    return res; // the exit code
}

void Application::dumpStatistics() const
{
    try {
        string s ;
        Statistics::instance()->getStatistics(s) ;
        if (not s.empty()) {
            ACS_LOG_PRIVINFO(s) ;
        }
    }
    catch(exception &) {
        //ignore it
    }
}


void Application::postLogMessage() // PRQA S 4020
{
    // Then loop over the PostLogHooks to emit post log messages
    if (!_allowPostLog) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_allowPostLog is false") ;
        return ;
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "calling: " << PostLogHooks::instance().size() << " hooks") ;
    // call all hooks and collect all messages into a string
    string msg ;
    PostLogHooks::instance().callHooks(msg) ;

    if (not msg.empty()) {
        // write the string
        ACS_LOG_INFO(msg) ;
    }
}

int Application::startMain(int argc, char const * const * argv, char const * const * env)
{
    exStackTrace::addLiveThreadId (::pthread_self(), "Main");
    //Finally start the real application main method.
    // this should be implemented from the application derived class
    return main(argc, argv, env);
}

void Application::wantToRestart(const unsigned int & restartTime)
{
    _restartTime = restartTime;
    _restartTimer.start() ;
    return;
}

bool Application::restartEnabled() const // PRQA S 4214
{
    bool isEnabled = true;

    // Not mandatory. If not available the isDisabled value will be left unchanged
    ConfigurationSingleton::instance()->get().getValue("RestartEnabled", isEnabled, false);

    return isEnabled;
}

bool Application::timeToRestart(bool retrigger)   // PRQA S 4020
{
    // If restart is not enable will always return false
    if (restartEnabled())
    {
        // stop the restart Timer
        _restartTimer.stop() ;
        // check if it is time to restart
        if(_restartTimer.elapsed()/1000 > _restartTime) // PRQA S 3084, 4400
        {
            if (retrigger) {
                _restartTimer.start(); // restart the timer
            }
            return true; // signal it is time to restart
        }
    }
    return false; // not yet ... wait longer - do something !
}


unsigned int Application::timeToRestart(unsigned int & restartTime)
{
    _restartTimer.stop() ;
    if(_restartTimer.elapsed()/1000 < _restartTime)  // not yet time to restart  // PRQA S 3084, 4400
    {
        // compute the difference (in secs)
        restartTime = _restartTime - _restartTimer.elapsed()/1000;  // PRQA S 3010, 3084, 4400
    }
    else {
        restartTime = 0; // we have no more time ...
    }
    // return result
    return restartTime;
}

unsigned int Application::restartInterval() const // PRQA S 4120
{
    return _restartTime ;
}


void Application::doRestart()
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}


void Application::terminateProcess()
{
}

void Application::beforeRestart()
{
}

void Application::setRotateLogParametersFromConf (bool b) ACS_NOEXCEPT // PRQA S 4121
{
    rotateLogParametersFromConf_ = b;
}


bool Application::authOperation (const string& operation, string& message) const
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    return true;
}

std::string Application::getCurrentUuid() const
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    ostringstream base;
    base << Application::instance()->applicationName() << "UUID";

    if (!_uuid)
    {
        string currentUuid = base.str();
        _uuid = unique_ptr<ApplicationUuid>(new ApplicationUuid(currentUuid));
    }
    return _uuid->getUuid();
}

// Returns true if StopManagerThread is up and running
// in case of failures notifies (just once) with a message and the runException if any
bool Application::checkStopManagerThread() const
{
    if ( nullptr == stopManagerThread_ )
    {
        // There is no stop manager at all.. stop the process
        return false;
    }

    if ( ! stopManagerThread_->running() )
    {
        // Force the new status computation
        stopManagerThread_->managePendingSignals();

        // Emit the error message just once
        static bool errorEmitted = false;
        if (!errorEmitted)
        {
            ACS_LOG_ERROR ("ApplicationStopManager thread not running. Signal management is now in degraded mode.");
            errorEmitted = true;
            // If the thread exploded, ask him why
            if (nullptr != stopManagerThread_)
            {
                shared_ptr<const std::exception> e = stopManagerThread_->runException();
                if(e)
                {
                    ACS_LOG_ERROR ("The stop manager thread exited due to an exception.");
                    ACS_LOG_NOTIFY_EX(*e);
                }
            }
        }
    }

    // In any case it is possible to manage, at least synchronously, the signal reception
    return true;
}


Application::StopMode Application::getCurrentStopModeAttribute() const
{
    ThreadSafe::Lock lock (*this);
    return currentStopMode_;
}


Application::StopMode  Application::getCurrentStopMode() const
{

    if ( checkStopManagerThread() )
    {
        return getCurrentStopModeAttribute();
    }

    // Return immediate stop if there are problems with the status manager thread
    return Application::ImmediateStop;
}

const std::map<Application::StopMode, std::string> Application::StopModeToString = {
    {NoStop, "NoStop"},
    {PhoenixRestart, "PhoenixRestart"},
    {CleanStop, "CleanStop"},
    {ImmediateStop, "ImmediateStop"}
};

std::string Application::currentStopModeToString() const
{
    return StopModeToString.at(getCurrentStopMode());
}

void  Application::setCurrentStopMode (const Application::StopMode& m) const
{
    // Avoid to call broadcastCondition with the application mutex locked
    bool doBroadcast = false;
    
    // create a lock to manage internal class attributes
    {
        ThreadSafe::Lock lock (*this);
        if (currentStopMode_ != m)
        {
            // Update the stop mode and register the change time
            ::gettimeofday (&lastStopModeChangeTime_, nullptr);
            currentStopMode_ = m;
            switch ( currentStopMode_ )
            {
                case PhoenixRestart:
                case CleanStop:
                {
                    AppStopController().setHaltLevel(StopController::LAZY);
                    break;
                }
                case ImmediateStop:
                {
                    AppStopController().setHaltLevel(StopController::IMMEDIATE);
                    break;
                }
                default:
                {
                    // Nothing
                }
            }
            stopModeChanged_ = true;
            // Shall broadcast on the condition since the stop mode has just changed
            doBroadcast = true;
        }
    }
    
    // The application is unlocked here
    if (doBroadcast)
    {
        broadcastCondition();
    }
}

bool Application::hasStopModeChanged (bool resetFlag)
{
    if ( checkStopManagerThread () )
    {
        ThreadSafe::Lock lock (*this);
        bool retvalue = stopModeChanged_;
        if (resetFlag)
        {
            stopModeChanged_ = false;
        }
        return retvalue;
    }

    // Return true in case the stop manager thread is dead
    // signaling that the stop mode has changed... 
    // do you want to ask what the new stop mode is ??
    return true;
}

bool Application::hasStopModeChanged (const struct timeval& checkTime, struct timeval& lastChangeTime)
{
    if ( checkStopManagerThread () )
    {
        // Avoid stop mode changes during the computation
        ThreadSafe::Lock lock (*this);
        if (
             (checkTime.tv_sec > lastStopModeChangeTime_.tv_sec) ||
             ( (checkTime.tv_sec == lastStopModeChangeTime_.tv_sec) && (checkTime.tv_usec >= lastStopModeChangeTime_.tv_usec) )
           )
        {
            // No change
            return false;
        }
        else
        {
            // There was a change... update the lastChangeTime and return true
            lastChangeTime = lastStopModeChangeTime_;
            return true;
        }
    }

    // Return true in case the stop manager thread is dead
    // signaling that the stop mode has changed... 
    // do you want to ask what the new stop mode is ??
    return true;
}



std::ostream &operator<<(std::ostream &os, Application::LogRotationPolicy::Mode m)
{
    switch(m) {
    case Application::LogRotationPolicy::Aging:
        {
            os << "Aging" ; break ;
        }
    case Application::LogRotationPolicy::Size:
        {
            os << "Size" ; break ;
        }
    case Application::LogRotationPolicy::FirstApplying:
        {
            os << "FirstApplying" ; break ;
        }
    case Application::LogRotationPolicy::Daily:
        {
            os << "Daily" ; break ;
        }
    default:
        {
            os << "<unknown Application::LogRotationPolicy::Mode: " << int(m) << ">" ; break ; // PRQA S 3081
        }
    }
    return os ; // PRQA S 4028
}


exostream &operator<<(exostream &os, Application::LogRotationPolicy::Mode m) // PRQA S 4222
{
    switch(m) {
    case Application::LogRotationPolicy::Aging:
        {
           os << "Aging" ; break ;
        }
    case Application::LogRotationPolicy::Size:
        {
           os << "Size" ; break ;
        }
    case Application::LogRotationPolicy::FirstApplying:
        {
           os << "FirstApplying" ; break ;
        }
    case Application::LogRotationPolicy::Daily:
        {
            os << "Daily" ; break ;
        }
    default:
        {
            os << "<unknown Application::LogRotationPolicy::Mode: " << int(m) << ">" ; break ; // PRQA S 3081
        }
    }
    return os ; // PRQA S 4028
}

std::ostream &operator<<(std::ostream &os, Application::Status s)
{
    os << Application::toString(s) ;
    return os ; // PRQA S 4028
}


exostream &operator<<(exostream &os, Application::Status s) // PRQA S 4222
{
    os << Application::toString(s) ;
    return os ; // PRQA S 4028
}

unsigned int Application::getSigTermTimeoutSeconds () const
{
    // Default value, can be changed re-implementing the function in derived classes
    return 30;
}

void Application::sigTermTimeoutExpired ()
{
    // Default behaviour, can be changed re-implementing the function in derived classes
    // First of all disable the core dump emission
    SignalDispatcher::instance()->disableCoreEmission();
    // then raise the abort signal
    ::abort();
}

unsigned int Application::getPhoenixTimeoutSeconds () const
{
    // Default value, can be changed re-implementing the function in derived classes
    // Read the value from the configuration singleton.. can be changed until the phoenix has been triggered
    // There is a default (0) that chan be overriden by a global value or an application specific value
    // The priority is AppSpecificConfTag -> GlobalConfTag -> Default zero value
    string phxTimeoutKey = "PhoenixTimeoutSeconds";
    string appPhxTimeoutKey = appName() + '.' + "PhoenixTimeoutSeconds";
    int retval = 0;
    rsResourceSet& rs = ConfigurationSingleton::instance() -> get();
    if (rs.hasKey ( appPhxTimeoutKey ) )
    {
        // Application level override
        rs.getValue (appPhxTimeoutKey, retval);
    }
    else
    {
        // Use global value, if any availiable
        // Otherwise zero will be not modified by the next call
        rs.getValue (phxTimeoutKey, retval, false);
    }
    ACS_CLASS_WRITE_DEBUG ( ACS_LOW_VERB, "Phoenix timeout in seconds set to :" << retval);
    return retval;
}

void Application::phoenixTimeoutExpired ()
{
    // Default behaviour, can be changed re-implementing the function in derived classes
    // Should not return
    ACS_LOG_WARNING ("Forcing the application restart after a timeout. Expect status inconsistency.");
    Application::restart();
}


void Application::broadcastCondition() const
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

    ThreadSafe::Lock myLock (condition_);
    condition_.broadcast();
}

bool Application::waitStopOrTimeout(unsigned long msectimeout) const
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

    // This function return true immediately if an exit has already been scheduled
    if (NoStop != getCurrentStopMode())
    {
        return true;
    }
    // Waits a given time interval, unless a broadcast signal on condition attribute is performed
    else
    {
        // Lock the condition_ only if it is going to wait on it
        ThreadSafe::Lock myLock (condition_);
        return condition_.timedwait (msectimeout);
    }
}

bool Application::requestedStop() const
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

    return NoStop != getCurrentStopMode();
}


// INNER CLASS FOR STATE TRANSITION MANAGEMENT
ACS_CLASS_DEFINE_DEBUG_LEVEL(Application::ApplicationStopManager) ;

Application::ApplicationStopManager::ApplicationStopManager() :
    Thread("ApplicationStopManagerThread"),
    manageSignalMutex_()
{
}

Application::ApplicationStopManager::~ApplicationStopManager() ACS_NOEXCEPT
{
    // Join the thread if running
    if (running())
    {
        // Immediate stop of the thread with join
        stop();
    }
}

void Application::ApplicationStopManager::managePendingSignals()
{
    // This function can be called from outside this thread
    // Manage it as a big critical section
    
    ThreadSafe::Lock lock (manageSignalMutex_);
    
    // Static counters and timers

    // Initialize signal counters
    static int sigIntCounter = 0;
    static int sigTermCounter = 0;
    static int sigPhoenixCounter = 0;
    static int sigUsr1Counter = 0;

    // Keep track of the statistics emission times, do not start it now
    static Timer statisticsTimer;

    // This starts from zero and keeps a value in case the timeout timer is started
    static Timer::Delay sigTermTimeoutInMSecs = 0;
    static Timer sigTermTimeoutTimer;
    static bool termWatchdogSet = false;

    // Another timeout management for the phoenix. In case is set and expired it will force the process restart
    static Timer::Delay phoenixTimeoutInMSecs = 0;
    static Timer phoenixTimeoutTimer;
    static bool phoenixWatchdogSet = false;

    // Compute and detect if new signals arrived in the meantime
    bool gotSigInt = false;
    bool gotSigTerm = false;
    bool gotSigPhoenix = false;

    // Manage sigTermTimeouts if the watchdog has been fired
    if ( (true == termWatchdogSet) && ( sigTermTimeoutTimer.stopElapsed() >= sigTermTimeoutInMSecs ) )
    {
        ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "SIGTERM timeout reached. Firing the timeout handler now.");
        // Enter here just one time (running will be false from now on)
        Application::instance()->sigTermTimeoutExpired();
        // Avoid multiple signals of timeout expired
        termWatchdogSet = false;
    }

    // Manage phoenixTimeouts if the watchdog has been fired
    if ( (true == phoenixWatchdogSet) && ( phoenixTimeoutTimer.stopElapsed() >= phoenixTimeoutInMSecs ) )
    {
        ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "PHOENIX timeout reached. Firing the phoenix handler now.");
        // Enter here just one time (running will be false from now on)
        Application::instance()->phoenixTimeoutExpired();
        // Avoid multiple signals of timeout expired
        phoenixWatchdogSet = false;
    }


    // Emulate phoenix signal
    // This emulation will work only once and, after that, will be never again triggered
    // to avoid legacy issues with timeToRestart flag used in some applications
    if ( (Application::NoStop==Application::instance()->getCurrentStopModeAttribute()) && Application::instance()->timeToRestart( false ))
    {
        ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "TimeToRestart returned true -> simulating SigPhoenix");
        SignalDispatcher::instance()->catchSigPhoenix(0);
    }

    // Check new signals one by one
    // SIGINT
    if (SignalDispatcher::instance()->getSigInt() > sigIntCounter)
    {
        ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "new SIGINT detected");
        // OK new SIGINT received... keep track of it
        gotSigInt = true;
        // Inefficient.. hoping the compiler will optimize it ;)
        sigIntCounter = SignalDispatcher::instance()->getSigInt();
    }

    // SIGTERM
    if (SignalDispatcher::instance()->getSigTerm() > sigTermCounter)
    {
        ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "new SIGTERM detected");
        // OK new SIGTERM received... keep track of it
        gotSigTerm = true;
        // Inefficient.. hoping the compiler will optimize it ;)
        sigTermCounter = SignalDispatcher::instance()->getSigTerm();
    }

    // SIGUSR1
    if (SignalDispatcher::instance()->getSigUsr1() > sigUsr1Counter)
    {
        ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "new SIGUSR1 detected");
        // Inefficient.. hoping the compiler will optimize it ;)
        sigUsr1Counter = SignalDispatcher::instance()->getSigUsr1();
        // Emit the debug information now
        try
        {
            Application::instance()->updateDebug();
        }
        catch (exception& x)
        {
            // Notify and ignore it
            ACS_LOG_NOTIFY_EX (x);
        }

    }

    // SIGPHOENIX ... assuming it can be raised also from outside this loop
    // Otherwise it can be moved in the sigPhoenix simulation statements
    if (SignalDispatcher::instance()->getSigPhoenix() > sigPhoenixCounter)
    {
        ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "new SIGPHOENIX detected");
        // OK new SIGTERM received... keep track of it
        gotSigPhoenix = true;
        // Inefficient.. hoping the compiler will optimize it ;)
        sigPhoenixCounter = SignalDispatcher::instance()->getSigPhoenix();
    }

    // Just a convenience bool to simplify next if statement
    bool newSignalsReceived = (gotSigInt || gotSigTerm || gotSigPhoenix );
    bool statusChanged = false;

    // Here it comes!
    if (newSignalsReceived)
    {
        // Invoke the custom signal handlers for all the received signals, if needed
        if (gotSigInt)
        {
            ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Calling application custom SIGINT handler");
            try
            {
                Application::instance()->customSigIntHandler();
            }
            catch (exception& x)
            {
                // Notify and ignore it
                ACS_LOG_NOTIFY_EX (x);
            }
        }

        if (gotSigTerm)
        {
            ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Calling application custom SIGTERM handler");
            try
            {
                Application::instance()->customSigTermHandler();
            }
            catch (exception& x)
            {
                // Notify and ignore it
                ACS_LOG_NOTIFY_EX (x);
            }
        }

        // Apply the status transition map
        switch (Application::instance()->getCurrentStopModeAttribute())
        {
            case (Application::ImmediateStop):
            {
                // Nothing to do... can't exit from this status
                break;
            }
            case (Application::CleanStop):
            {
                // Can move to ImmediateStop if received a sigterm
                if (gotSigTerm)
                {
                    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Entering stop mode ImmediateStop now");
                    Application::instance()->setCurrentStopMode(Application::ImmediateStop);
                    statusChanged=true;
                }
                // Any other signal is ignored
                break;
            }
            case (Application::PhoenixRestart):
            {
                // Can move to ImmediateStop if received a sigterm
                if (gotSigTerm)
                {
                    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Entering stop mode ImmediateStop now");
                    Application::instance()->setCurrentStopMode(Application::ImmediateStop);
                    statusChanged=true;
                }
                // otherwise can move to CleanStop if received a sigint
                else if (gotSigInt)
                {
                    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Entering stop mode CleanStop now");
                    Application::instance()->setCurrentStopMode(Application::CleanStop);
                    statusChanged=true;
                }
                // Other signals are ignored
                break;
            }
            case Application::NoStop:
            default:
            {
                // Can move to ImmediateStop if received a sigterm
                if (gotSigTerm)
                {
                    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Entering stop mode ImmediateStop now");
                    Application::instance()->setCurrentStopMode(Application::ImmediateStop);
                    statusChanged=true;
                }
                // otherwise can move to CleanStop if received a sigint
                else if (gotSigInt)
                {
                    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Entering stop mode CleanStop now");
                    Application::instance()->setCurrentStopMode(Application::CleanStop);
                    statusChanged=true;
                }
                // otherwise can move to PhoenixRestart if received a sigphoenix
                else if (gotSigPhoenix)
                {
                    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Entering stop mode PhoenixRestart now");
                    Application::instance()->setCurrentStopMode(Application::PhoenixRestart);
                    statusChanged=true;
                }
                // Other signals??? - Ignore it
                break;
            }
        }
    }

    // Emit statistics if needed: after a timeout in case of signal reception or at any status change

    // Enter the following status change management code only if an actual status change has occurred
    if (statusChanged)
    {
        // Emit statistics at each change of the exit mode
        statisticsTimer.start();
        Application::instance()->dumpStatistics();

        // Disarm the phoenix timer. It will be set again if we are just entering the phoenix status
        // otherwise it will prevent phenix timeouts in case of migration from phoenix to clean or immediate stop
        phoenixWatchdogSet = false;

        // Start also the sigTermTimeout if entering the ImmediateStop sequence
        if (Application::ImmediateStop == Application::instance()->getCurrentStopModeAttribute() )
        {
            unsigned int sigTermTimeout = Application::instance()->getSigTermTimeoutSeconds();
            ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Mode changed to ImmediateStop. Application declared sigterm timeout is " << sigTermTimeout);
            if (sigTermTimeout != 0)
            {
                // Start the timer and mark that an exit watchdog has been set
                termWatchdogSet=true;
                sigTermTimeoutTimer.start();
                // Convert seconds in millisecond (Delay type)
                sigTermTimeoutInMSecs = sigTermTimeout * 1000;
                ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "SIGTERM timeout timer has been started");
            }
        }

        // Start also the phoenixTimeout if entering the phoenix sequence
        if (Application::PhoenixRestart == Application::instance()->getCurrentStopModeAttribute() )
        {
            unsigned int phoenixTimeout = Application::instance()->getPhoenixTimeoutSeconds();
            ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Mode changed to PhoenixRestart. Application declared phoenix timeout is " << phoenixTimeout);
            if (phoenixTimeout != 0)
            {
                // Start the timer and mark that an exit watchdog has been set
                phoenixWatchdogSet=true;
                phoenixTimeoutTimer.start();
                // Convert seconds in millisecond (Delay type)
                phoenixTimeoutInMSecs = phoenixTimeout * 1000;
                ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "PHOENIX timeout timer has been started");
                ACS_LOG_INFO ("Application restart has been scheduled. The restart will be forced in " << phoenixTimeout << " seconds from now.");
            }
        }
    }
    else if (newSignalsReceived)
    {
        // Got a signal... emit the statistics only if at least one minute passed
        if (statisticsTimer.elapsed() >= (60.0 * 1000))
        {
            // Reset the timer and emit statistics
            statisticsTimer.start();
            Application::instance()->dumpStatistics();
        }
    }

}



// Main loop thread implementation
void Application::ApplicationStopManager::run()
{
    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Thread started");

    // trapped into this loop until someone asks to exit
    while ( ! requestedCancel() )
    {
        // Manage signals and update application stop mode flag
        try
        {
            managePendingSignals();
        }
        catch (exception& x)
        {
            // Notify and ignore it
            ACS_LOG_NOTIFY_EX (x);
        }

        try
        {
            // Sleep for 100 msecs
            timeoutOrCancel (100);
        }
        catch (exception& x)
        {
            // Notify and ignore it
            ACS_LOG_NOTIFY_EX (x);
            Timer::delay (100);
        }
        // Use this debug level to simulate a crash in the stop manager thread
        ACS_CLASS_EQ_DEBUG (12345)
                ACS_THROW (Application::Exception("12345 debug level set for Application::ApplicationStopManager - commiting suicide"));
        ACS_CLASS_END_DEBUG

        // This will force the run to exit
        ACS_CLASS_EQ_DEBUG (12346)
            break;
        ACS_CLASS_END_DEBUG

    }
    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Thread exit");
}




_ACS_END_NAMESPACE
