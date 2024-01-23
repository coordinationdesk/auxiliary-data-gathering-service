// PRQA S 1050 EOF
/*

    Copyright 1995-2015, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. ThinLayer Daemon $

    $Id$

    $Author$

    $Log$
    Revision 2.2  2015/11/27 17:38:06  marpas
    coding best practice application in progress

    Revision 2.1  2015/11/20 11:10:55  marpas
    coding best practices application in progress

    Revision 2.0  2006/02/28 17:02:07  paoscu
    Exception 2.1 I/F adopted

    Revision 1.8  2005/03/18 11:22:03  marfav
    Calval environment merged to standard one

    Revision 1.7  2004/12/03 17:24:47  marfav
    Header Fixed

    Revision 1.6  2004/05/05 14:22:05  marfav
    Patch for SUSPENDED order state management added (CalVal environment only)

    Revision 1.5  2004/03/11 19:10:29  marfav
    Preparing to manage external commands
    Align to new version of Scheduler Engine (libScheduler)

    Revision 1.4  2003/05/07 15:06:27  marfav
    Improved memory allocation management and exception handling

    Revision 1.3  2003/04/18 12:59:33  marfav
    Unsing namespaces removed from .h

    Revision 1.2  2003/03/24 16:35:26  marfav
    Included JobOrderFactory support

    Revision 1.1  2003/03/11 18:25:50  marfav
    Patrol message handling added


*/



#include <TlStatusMgrSingleton.h>
#include <FEConvenienceValues.h>


using namespace std;
using namespace acs;



//   **********   class TlStatusMgrSingleton *************//

//
//
//	Singleton Implementation
//
//

// TlStatusMgrSingleton* TlStatusMgrSingleton::_instance = NULL;


// -------------------------------------------------------------------------------------
//                       getTlStatusMgr
// -------------------------------------------------------------------------------------
TlStatusMgrSingleton::MgrType *
TlStatusMgrSingleton::buildMgr()
{
    map<tl_status,string> statusMap;
    statusMap[TL_ERROR]=EXFE_ERROR;
    statusMap[TL_WARNING]=EXFE_WARNING;
    statusMap[TL_PROCESSING_ORDER]=EXFE_PROCESSING_ORDER;
    statusMap[TL_CHECKING_QUEUE]=EXFE_CHECKING_QUEUE;
    statusMap[TL_IDLE]=EXFE_IDLE;
    return new TlStatusMgrSingleton::MgrType(disp_functor<tl_status>("TL_PROCESS_STATUS", statusMap), TL_IDLE) ;
}



//   **********   class ProcessorStatusMgrSingleton *************//

//
//
//	Singleton Implementation
//
//

// ProcessorStatusMgrSingleton* ProcessorStatusMgrSingleton::_instance = NULL;


// -------------------------------------------------------------------------------------
//                       getProcessorStatusMgr
// -------------------------------------------------------------------------------------
ProcessorStatusMgrSingleton::MgrType *
ProcessorStatusMgrSingleton::buildMgr()
{

    map<processor_status,string> statusMap;
    statusMap[PR_ERROR]=EXFE_ERROR;
    statusMap[PR_WARNING]=EXFE_WARNING;
    statusMap[PR_RUNNING]=EXFE_RUNNING;
    statusMap[PR_IDLE]=EXFE_IDLE;
    statusMap[PR_SUSPENDED]=EXFE_SUSPENDED;

    return new ProcessorStatusMgrSingleton::MgrType(disp_functor<processor_status> ("PRODUCTION_STATUS",statusMap),PR_IDLE) ;
}



//   **********   class DownloadStatusMgrSingleton *************//

//
//
//	Singleton Implementation
//
//

// DownloadStatusMgrSingleton* DownloadStatusMgrSingleton::_instance = NULL;


// -------------------------------------------------------------------------------------
//                       getDownloadStatusMgr
// -------------------------------------------------------------------------------------
DownloadStatusMgrSingleton::MgrType *
DownloadStatusMgrSingleton::buildMgr()
{
    map<download_status, string> statusMap;
    statusMap[DL_ERROR]=EXFE_ERROR;
    statusMap[DL_BUSY]=EXFE_BUSY;
    statusMap[DL_IDLE]=EXFE_IDLE;

    return new DownloadStatusMgrSingleton::MgrType (disp_functor<download_status> ("DOWNLOAD_INPUT_FILES_STATUS",statusMap),DL_IDLE) ;
}
