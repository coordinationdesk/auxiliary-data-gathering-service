// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2013, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
 *
 *  $Prod: A.C.S. libDataProvider Library $

 $Id$

 $Author:

 $Log$
 Revision 5.5  2014/05/05 19:10:20  nicvac
 S2PDGS-667: implementation.

 Revision 5.4  2013/11/25 17:34:05  nicvac
 Handled contingency in DownloadPackData::optimiser. concurrent downloading from checkDownloadedFile.

 Revision 5.3  2013/11/12 13:22:11  nicvac
 S2PDGS-518: Fixed checkDownloadedFile. fileName was I/O parameter without reason.

 Revision 5.2  2013/11/04 13:56:31  marpas
 coding best practices applied
 robustness improved
 performances improved
 compilation & qa warnings removed

 Revision 5.1  2013/09/30 12:37:13  marpas
 adopting libDbCryosat 4.x

 Revision 5.0  2013/06/06 18:51:20  marpas
 adopting libException 5.x standards

 Revision 1.15  2013/03/12 17:55:47  marpas
 dbConnectionPool is mandatory in file actions

 Revision 1.14  2012/10/08 13:59:19  micmaz
 http://jira.acsys.it/browse/S2GPP-611

 Revision 1.13  2012/09/19 15:57:11  micmaz
 ACCEPTED - issue S2GPP-611: Development of Distribution Formatter framework for Export
 http://jira.acsys.it/browse/S2GPP-611

 Revision 1.12  2012/09/14 16:31:32  micmaz
 S2GPP-611
 Development of Distribution Formatter framework for Export

 Revision 1.10  2012/09/11 16:42:31  micmaz
 removed compiling warning

 Revision 1.9  2012/02/13 16:59:44  marpas
 refactoring in progress

 Revision 1.8  2011/05/06 10:48:36  crivig
 modified for quality check; i++ modified into ++i; packFiles method simplified: no changedir

 Revision 1.7  2011/04/18 10:06:17  crivig
 cache filename methods moved in libFile

 Revision 1.6  2011/02/25 14:53:05  crivig
 bug on cartId fixed

 Revision 1.5  2010/12/17 10:24:20  crivig
 connection pool passed

 Revision 1.4  2010/12/03 15:00:49  crivig
 cache cleaning handled with Cache class

 Revision 1.3  2010/11/29 11:05:59  crivig
 changed Package to DataPackage

 Revision 1.2  2010/11/22 12:45:12  crivig
 modified disk cleaning algo

 Revision 1.1  2010/11/22 10:28:17  crivig
 imported new Class from libDownloadOptimiser

 */

#ifndef _DownloadPackData_H_
#define _DownloadPackData_H_

#include <acs_c++config.hpp>
#include <Thread.h>
#include <DataPackage.h>
#include <Cache.h>
#include <dbConnectionPool.h>


_ACS_BEGIN_NESTED_NAMESPACE(acs, dataprovider)

///////////////////////////////////////////////////
///////////////////////////////////////////////////
//
// DownloadPackData
//
///////////////////////////////////////////////////
///////////////////////////////////////////////////

/**
 *\brief A Class for downloading the files inventory from db
 *
 */
class DownloadPackData: public Thread { // PRQA S 2109
public:
    /*! class DownloadPackDataException declaration */
    exDECLARE_EXCEPTION(DownloadPackDataException,exException); // PRQA S 2131, 2502
    
    DownloadPackData(DataPackage& package, dbConnectionPool& connectionPool, Cache& cache);
    virtual ~DownloadPackData() throw ();
    
    const DataPackage &getPackage() const throw() ;
    bool isDone() const throw() ;
    virtual void exit(); // PRQA S 2502

private:
    DownloadPackData();
    DownloadPackData(const DownloadPackData&);
    DownloadPackData & operator=(const DownloadPackData&);

    virtual void run();

    /** Download the package's items considering the cache and link them in the Product Path */
    void downloadWithCacheAndLinkToProdPath();
    void findProductFileType(std::string*);

private:
    DataPackage _package;
    size_t _maxConnRetry;
    bool _done;
    dbConnectionPool& _connectionPool;
    Cache & _cache;
    bool _stop;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(DownloadPackData) ;

};


_ACS_END_NESTED_NAMESPACE

#endif // _DOWNLOADOPTIMSER_H_
