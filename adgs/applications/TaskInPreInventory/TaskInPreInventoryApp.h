// PRQA S 1050 EOF
/*
    Copyright 1995-2014, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: $

    $Id$

    $Author$

    $Log$
    Revision 1.4  2014/02/17 13:48:01  marpas
    coding best practices applied

    Revision 1.3  2013/03/14 13:56:31  marpas
    connection pool concept is left to class inheriting from Application

    Revision 1.2  2013/02/21 18:44:44  marpas
    dbConnectionPool used

    Revision 1.1.1.1  2012/11/30 12:44:54  chicas
    Imported sources

    Revision 1.1.1.1  2012/10/17 10:07:58  chicas
    Imported sources


*/

#ifndef _TaskInPreInventoryApp_H_
#define _TaskInPreInventoryApp_H_

#if defined (HAS_MISSION_INCLUDES)
#include <ProjectAppIncludes>
#endif

#include <TaskInPreInventory.h>
#include <PreInventoryCommonApp.h>

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief
 *  TaskInPreInventory Main application
 **/

class TaskInPreInventoryApp: // PRQA S 2109, 2153
    public PROJECT_APP,
    public TaskInPreInventory,
    public PreInventoryCommonApp {
public:

  /** TaskInPreInventoryApp Exceptions */
  exDECLARE_EXCEPTION(exTaskInPreInventoryAppException, exException) ; // Base TaskInPreInventoryApp Exception. // PRQA S 2131, 2502 2
  exDECLARE_EXCEPTION(exTaskInPreInventoryAppCriticalException, exTaskInPreInventoryAppException) ; // Critical Exception. // PRQA S 2153


  /** Class constructor */
  TaskInPreInventoryApp(const std::string& subsys, const std::string& app);
  /** Destructor */
  virtual ~TaskInPreInventoryApp() throw() ;

private:

  TaskInPreInventoryApp();// not implemented
  TaskInPreInventoryApp(const TaskInPreInventoryApp & );// not implemented
  TaskInPreInventoryApp &operator=(const TaskInPreInventoryApp &);// not implemented

public:
  /** Entry point */
  virtual int main(int argc, char const * const * argv, char const * const * env);

private:
  ACS_CLASS_DECLARE_DEBUG_LEVEL(TaskInPreInventoryApp)

};

_ACS_END_NAMESPACE

#endif //_TaskInPreInventoryApp_H_

