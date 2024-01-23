/*
    Copyright 1995-2008, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. libTLPreInventoryApp$

    $Id$

    $Author$

    $Log$
    Revision 1.7  2016/07/08 14:31:25  marfav
    CSGACQ-84
    New project/application based log rotation management strategy enforced

    Revision 1.6  2015/06/25 14:48:38  damdec
    Compilation warnings fixed.

    Revision 1.5  2013/03/14 14:58:51  marpas
    connection pool concept is left to class inheriting from Application

    Revision 1.4  2013/02/21 18:35:15  marpas
    dbConnectionPool used

    Revision 1.3  2012/02/14 15:18:48  marpas
    fixing dtor

    Revision 1.2  2009/04/20 15:25:04  nicvac
    Factories and Singleton moved in common subclass.

    Revision 1.1.1.1  2008/10/17 14:21:05  nicvac
    Import TLPreInventory


*/

#ifndef _TLPreInventoryApp_H_
#define _TLPreInventoryApp_H_

#if defined (HAS_MISSION_INCLUDES)
#include <ProjectAppIncludes>
#endif

#include <TlPreInventory.h>
#include <PreInventoryCommonApp.h>

#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

    /**
     * \brief
     * TL PreInventory Main application
     **/

   class TLPreInventoryApp: public PROJECT_APP, public TlPreInventory, public PreInventoryCommonApp {
   public:

      /** TLPreInventoryApp Exceptions */
      exDECLARE_EXCEPTION(exTLPreInventoryAppException, exException) ; // Base TLPreInventoryApp Exception.
      exDECLARE_EXCEPTION(exTLPreInventoryAppCriticalException, exTLPreInventoryAppException) ; // Critical Exception.


      /** Class constructor */
      TLPreInventoryApp(const std::string& app, const std::string& subsys);
      /** Destructor */
      virtual ~TLPreInventoryApp() throw() ;

      virtual std::string getDefaultLogName() const;

   private:  // copy constructor and operator= defined but not implemented
      /** Copy Constructor */
      TLPreInventoryApp(const TLPreInventoryApp & );
      /** Operator = */
      TLPreInventoryApp &operator=(const TLPreInventoryApp &);

   public:
      /** Entry point */
      virtual int main(int argc, char const * const * argv, char const * const * env);

   private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(TLPreInventoryApp)

   };

_ACS_END_NAMESPACE

#endif //_TLPreInventoryApp_H_

