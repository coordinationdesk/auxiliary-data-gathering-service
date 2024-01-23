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

    $Prod:

    $Id$

    $Author$

    $Log$
    Revision 5.3  2015/02/04 14:58:58  marfav
    S2PDGS-1072 adding support to options noactions and restoreonsync

    Revision 5.2  2015/02/02 16:08:50  marfav
    S2PDGS-1072 upload without triggers implemented

    Revision 5.1  2015/01/30 17:23:06  marfav
    S2PDGS-976 managing restore on sync storage managers

    Revision 5.0  2013/06/20 07:51:40  marpas
    implementing SENT-37 about messaging
    rationalization, streams redirection and chaining fixed
    adopting libException 5.x standards
    coding best practices applied
    qa warnings removed
    compilation warnings removed

    Revision 2.9  2013/03/14 15:29:54  marpas
    connection pool concept is left to class inheriting from Application

    Revision 2.8  2013/02/21 19:20:30  marpas
    dbConnectionPool used

    Revision 2.7  2012/03/06 13:30:25  marpas
    usage fixed

    Revision 2.6  2012/02/09 17:45:40  marpas
    refactoring in progress

    Revision 2.5  2011/06/20 10:26:13  marpas
    -relaxfncheck && --relaxfncheck arguments can be used to relax the check on files number for directory products.

    Revision 2.4  2011/03/21 18:28:55  marpas
    VEN-1993 management in progress: no critical exception if a duplicate was found and -noCriticalOnDuplicates arg passed

    Revision 2.3  2010/06/14 15:27:40  marpas
    HAS_MISSION_INCLUDES management added

    Revision 2.2  2006/05/05 08:27:23  giucas
    final version

    Revision 2.1  2006/05/04 17:59:54  marpas
    builder used instead of global variable

    Revision 2.0  2006/02/28 11:52:50  paoscu
    Exception 2.1 I/F adopted

    Revision 1.9  2005/09/29 16:33:39  marpas
    make up and list of failed orders (if any)

    Revision 1.8  2004/12/03 11:18:45  giucas
    Header fixed

    Revision 1.7  2004/10/14 15:02:01  seralb
    overwrite option added

    Revision 1.6  2004/09/07 13:49:09  marpas
    Makefile.Station removed
    supported projects Cosmo, Cryosat, Envisat

    Revision 1.5  2004/09/06 08:40:50  danalt
    Cosmo MultiMission Environment Support Added

    Revision 1.4  2003/09/03 14:02:15  seralb
    Upgrade

    Revision 1.3  2003/07/09 12:26:36  seralb
    Improvement

    Revision 1.2  2003/03/03 15:04:27  marpas
    boh (makefile fixed)

    Revision 1.1.1.1  2003/01/20 13:57:10  seralb
    Import Inventory




*/

#ifndef _InventoryApp_H_
#define _InventoryApp_H_

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

class InventoryApp :
            public PROJECT_APP
{
public:

    /*! class InventoryAppException declaration */
    exDECLARE_EXCEPTION(InventoryAppException,exException) ; // PRQA S 2131, 2502

    /**
     * \brief  Default Class constructor
     *
     */
    InventoryApp(const std::string &appName, const std::string &subSystem);

    virtual std::string getDefaultLogName() const;

    /**
     * \brief  Class destructor
     *
     */
    virtual ~InventoryApp() throw() ;

    virtual int main(int argc, char const * const * argv, char const * const * env);

    virtual int usage ( const std::string & ) const ;

private:
    // copy constructor and operator= defined but not implemented
    InventoryApp();
    InventoryApp(const InventoryApp & );
    InventoryApp &operator=(const InventoryApp &);


    //methods

    void startInventory();
    void getConfiguration();

    //attributes
    std::string _option;
    std::string _failedOrders ;

    bool _noCriticalOnDuplicates ;
    bool _relaxFnCheck ;
    bool _restoreOnSync;
    bool _noActions;
    bool _noBackup ;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(InventoryApp) ;
};

_ACS_END_NAMESPACE

#endif //_InventoryApp_H_
