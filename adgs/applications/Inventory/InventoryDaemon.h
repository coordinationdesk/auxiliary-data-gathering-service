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

    $Prod: A.C.S. Inventory Application running as Daemon $

    $Id$

    $Author$

    $Log$
    Revision 5.1  2014/09/19 10:21:11  marfav
    Added support to configurable restart time

    Revision 5.0  2013/06/20 07:51:40  marpas
    implementing SENT-37 about messaging
    rationalization, streams redirection and chaining fixed
    adopting libException 5.x standards
    coding best practices applied
    qa warnings removed
    compilation warnings removed

    Revision 1.5  2013/03/14 15:29:54  marpas
    connection pool concept is left to class inheriting from Application

    Revision 1.4  2013/02/21 19:20:30  marpas
    dbConnectionPool used

    Revision 1.3  2012/02/09 17:45:40  marpas
    refactoring in progress

    Revision 1.2  2010/06/14 15:27:40  marpas
    HAS_MISSION_INCLUDES management added

    Revision 1.1  2008/08/27 14:24:59  davcas
    Added, derived from InventoryApp



*/

#ifndef _InventoryDaemon_H_
#define _InventoryDaemon_H_

#if defined (HAS_MISSION_INCLUDES)
        #include <ProjectAppIncludes>
#elif GENERIC_STATION_APP
    #include <StationApp.h>
    #define PROJECT_APP StationApp
#elif COSMO
    #include <CosmoApp.h>
    #define PROJECT_APP	CosmoApp
#else
    #include <CryosatApp.h>
    #define PROJECT_APP	CryosatApp
#endif
#include <exException.h>

#include <string>


_ACS_BEGIN_NAMESPACE(acs)

class InventoryDaemon : // PRQA S 2153
            public PROJECT_APP
{
public:

    /*! class InventoryAppException declaration */
    exDECLARE_EXCEPTION(InventoryDaemonException,exException) ; // PRQA S 2131, 2502

    /**
     * \brief  Default Class constructor
     *
     */
    InventoryDaemon(const std::string &appName, const std::string &subSystem);

    /**
     * \brief  Class destructor
     *
     */
    virtual ~InventoryDaemon() throw() ;

    virtual int main(int argc, char const * const * argv, char const * const * env);



private:
    // copy constructor and operator= defined but not implemented
    InventoryDaemon();
    InventoryDaemon(const InventoryDaemon & );
    InventoryDaemon &operator=(const InventoryDaemon &);


    //methods

    void startInventory();
    void getConfiguration();
    virtual void customSigTermHandler() ;
    virtual void customSigIntHandler();

    //attributes
    pthread_t _mainThread ;
    std::string _failedOrders ;
    std::string _targetDir;
    std::string _backupDir;
    unsigned int _restartTime;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(InventoryDaemon) ;
};

_ACS_END_NAMESPACE

#endif //_InventoryDaemon_H_
