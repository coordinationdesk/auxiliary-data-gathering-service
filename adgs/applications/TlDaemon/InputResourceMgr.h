// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
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
	Revision 3.0  2016/10/11 08:27:16  marfav
	Major release due to cache management modifications
	
	Revision 2.10  2016/10/11 07:55:06  marfav
	S2MPM-842
	CacheManager is now a pointer
	
	Revision 2.9  2016/07/12 15:00:02  marfav
	Fixed connection management in waiting orders handling
	
	Revision 2.8  2015/11/27 17:38:06  marpas
	coding best practice applicationin progress
	
	Revision 2.7  2015/11/19 17:23:03  marpas
	coding best practices application in progress
	
	Revision 2.6  2015/04/30 15:49:17  marfav
	Compiler warning removed
	
	Revision 2.5  2013/11/19 17:50:24  marpas
	TaskTable has private copy ctor & operator=
	
	Revision 2.4  2012/02/09 17:26:24  marpas
	refactoring in progress
	
	Revision 2.3  2008/08/01 15:38:29  marfav
	PreConditionChecker plugins framework added
	
	Revision 2.2  2008/04/22 13:35:27  marfav
	Aligned to the task table conditioned visitor design pattern
	Exception management improved
	
	Revision 2.1  2006/03/06 15:59:19  marfav
	Using Task Tables having AtExit sequence support
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.12  2006/01/19 18:05:58  marfav
	Added support for mapping alternatives instead of filetypes when downloading inputs
	
	Revision 1.11  2005/03/09 09:30:43  marfav
	Order queue management changed
	now the orders are put in a waitlist when precondition fails
	the database will resume the waiting orders when the right input is inventoried
	
	Revision 1.10  2005/03/02 14:06:18  marfav
	Added cache support for downloaded files
	
	Revision 1.9  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.8  2004/10/19 08:17:07  marfav
	Support for orders satellite and mission fields added
	
	Revision 1.7  2003/09/23 14:02:42  fracar
	handling verbose execution mode in support to Thin Layer MMI interactive operations
	
	Revision 1.6  2003/05/13 08:45:19  marfav
	Fixed memory leak at cleanup
	
	Revision 1.5  2003/04/18 12:59:33  marfav
	Using namespaces removed from .h
	
	Revision 1.4  2003/03/11 18:25:50  marfav
	Patrol message handling added
	
	Revision 1.3  2003/02/04 14:11:17  marfav
	Using ConfigurationSingleton instead of parent config
	
	Revision 1.2  2003/02/04 09:58:34  marfav
	Added LOG field to headers
	


*/


#ifndef _INPUT_RESOURCE_MGR_H
#define _INPUT_RESOURCE_MGR_H

#include <TaskTable.h>
#include <InputDownloader.h>
#include <CacheManager.h>
#include <tlOrder.h>
#include <StringKey.h>
#include <PCC_Base.h>

#include <dbConnection.h>

#include <map>
#include <string>
#include <memory>

_ACS_BEGIN_NAMESPACE(acs)

class ThinLayer_PCC;

class InputResourceMgr // PRQA S 2109
{
public:

  InputResourceMgr (const StringKey&, const tlOrder&, const TaskTable&, CacheManager*);
  virtual ~InputResourceMgr () throw() ;

  bool checkInputSet(bool verboseFlag=false);
  bool downloadInputSet(const std::string& aDirPath);
  void abortDownload();
  bool aborted() const;

  bool getVisitedTaskTable(TaskTable&) const;
  bool getSymbolTable (PCC_Base::SymTable&) const;
  bool getDownloadedSymbolTable (PCC_Base::SymTable&) const;

  // public to avoid friend classes
  void updateSymbolTable (const InputAlternative&, const PCC_Base::SymTableParams&);
  void mapAlternative(std::pair<const InputAlternative, PCC_Base::SymTableParams>&);

protected: // methods

protected: // attributes

private: // methods
  double retrieveDeltaT() const;
  void mapAlternatives();

  void updateOrdersWaitTable (PCC_Base::SymTable, dbConnection&);

  InputResourceMgr& operator = (const InputResourceMgr& d); // not implemented
  InputResourceMgr (const InputResourceMgr& d); // not implemented
  InputResourceMgr (); // not implemented

private: // attributes
  std::map<InputAlternative, InputDownloader> _downloaders;
  bool _aborted;
  StringKey _theKey;
  tlOrder _order;
  ACS_SMARTPTR<TaskTable> _theTT;
  CacheManager* _cacheManager;
  ThinLayer_PCC* _precondChecker;
  PCC_Base::SymTable _symbolTable;
  PCC_Base::SymTable _downloadedSymbolTable;
  double _deltaT;

private: // static stuff...
  ACS_CLASS_DECLARE_DEBUG_LEVEL (InputResourceMgr)

public: // exceptions
  exDECLARE_EXCEPTION(InputResourceMgrException, exException) ; // PRQA S 2131, 2502
  exDECLARE_EXCEPTION(WaitingOrderException, InputResourceMgrException) ; // PRQA S 2131, 2502

};


_ACS_END_NAMESPACE


#endif // _INPUT_RESOURCE_MGR_H
