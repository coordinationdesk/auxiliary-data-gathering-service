// PRQA S 1050 EOF
/*

    Copyright 1995-2013, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod:  $

    $Id$

    $Author$

    $Log$
    Revision 1.16  2014/09/19 09:29:14  marfav
    Added configuration support to restart time

    Revision 1.15  2013/12/19 12:50:17  marpas
    coding best practices applied

    Revision 1.14  2013/05/12 14:37:10  marpas
    no longer master/slave check

    Revision 1.13  2013/03/14 13:22:42  marpas
    connection pool concept is demanded to class inheriting from Application

    Revision 1.12  2013/02/12 17:57:16  marpas
    minor changes, messages improved

    Revision 1.11  2013/02/07 17:58:24  marpas
    DistributorExecutor & CompletedOrderFinalizer now do most of the work (they are in libDistributorCreator)
    They are also aware about connection pools and stop predicates to maximize efficiency
    setting DistributorGenerator_DEBUG to 1357 allow the application to run even in case of problem loading configuration files for testing purposes

    Revision 1.10  2013/02/05 18:17:24  marpas
    listener re-added on t_temporarydistribution & t_completedorders
    logical flow with delay rewritten in main loop

    Revision 1.9  2013/01/24 19:21:29  marpas
    interface rationalization
    control flow simplified for easy maintenance
    coding best practices
    qa rules

    Revision 1.8  2013/01/11 13:54:56  marfav
    Addedd support to external appname definition

    Revision 1.7  2012/03/08 10:52:55  marpas
    task flow and messages revisited

    Revision 1.6  2012/02/09 15:12:44  marpas
    refactoring

    Revision 1.5  2011/10/20 10:08:41  crivig
    software status notification introduced

    Revision 1.4  2010/06/15 09:35:28  marpas
    HAS_MISSION_INCLUDES management added

    Revision 1.3  2008/12/11 13:17:17  marpas
    improvements

    Revision 1.2  2008/05/20 13:27:29  crivig
    create temporarydistribution from completed orders

    Revision 1.1.1.1  2007/03/15 15:01:03  crivig
    Imported source


*/


#ifndef _DistributorGenerator_H_
#define _DistributorGenerator_H_

#include <acs_c++config.hpp>
#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#if defined (HAS_MISSION_INCLUDES)
    #include <ProjectAppIncludes>
#elif GENERIC_STATION_APP
    #include <StationApp.h>
#elif COSMO
    #include <CosmoApp.h>
#else
        #include <CryosatApp.h>
#endif

#include <dbConnection.h>
#include <PluginLoader.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbSet ;
class DGListener ;

class DistributorGenerator : // PRQA S 2109, 2153

#if defined (HAS_MISSION_INCLUDES)
    public PROJECT_APP
#elif GENERIC_STATION_APP
    public StationApp
#elif  COSMO
    public CosmoApp
#else
    public CryosatApp

#endif
{
public:

    DistributorGenerator (const std::string &appName, const std::string &subSystem);
    virtual ~DistributorGenerator() throw() ;
    virtual int main(int argc, char const * const * argv, char const * const * env);

    virtual void customSigTermHandler() ;
    virtual void customSigIntHandler() ;
    virtual int usage ( const std::string & ) const ;
private:
    // declared but not implemented
    DistributorGenerator(const DistributorGenerator&);
    DistributorGenerator & operator=(const DistributorGenerator&);

    std::string loadPlugins() ;
    bool loadConf();
    void doWork() ;
    void mainLoop() ;
    void waitLoop(unsigned long secs, DGListener *listener=0) ;
    int loadFirstConf() ;
    void debugException() ;
    void writeMsgIfStopped() ;

private:

    PluginLoader _pluginLoader ;
    unsigned long _pollingTime ;
    unsigned int completed_orders_num_  ;
    bool _warning ;

    static const unsigned int default_completed_orders_num_ = 50 ;
    static const unsigned int default_pollingTime_ = 300 ;
    unsigned int _restartTime;
    static const unsigned int _defaultRestartTime;
public:
    static const std::string _distGenRootKey ; // PRQA S 2100

    // Configuration spaces keys for values
    static const std::string _pluginsPathKey ; // PRQA S 2100
    static const std::string _pluginRegExp ;   // PRQA S 2100
    static const std::string _restartTimeKey ;   // PRQA S 2100

private:
    pthread_t _mainThread ;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(DistributorGenerator) ;

};

_ACS_END_NAMESPACE

#endif /* _DistributorGenerator_H_ */
