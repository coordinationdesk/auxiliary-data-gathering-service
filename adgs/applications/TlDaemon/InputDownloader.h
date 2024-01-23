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
	Revision 2.8  2016/10/11 07:56:09  marfav
	S3MPM-842
	CacheManager is now a pointer
	
	Revision 2.7  2015/12/01 17:44:45  marpas
	coding best practice application in progress
	
	Revision 2.6  2015/11/27 17:38:06  marpas
	coding best practice applicationin progress
	
	Revision 2.5  2015/04/30 15:49:17  marfav
	Compiler warning removed
	
	Revision 2.4  2012/02/09 17:26:24  marpas
	refactoring in progress
	
	Revision 2.3  2008/08/01 15:38:28  marfav
	PreConditionChecker plugins framework added
	
	Revision 2.2  2008/04/22 13:35:27  marfav
	Aligned to the task table conditioned visitor design pattern
	Exception management improved
	
	Revision 2.1  2006/03/06 15:59:19  marfav
	Using Task Tables having AtExit sequence support
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.18  2006/01/19 18:05:58  marfav
	Added support for mapping alternatives instead of filetypes when downloading inputs
	
	Revision 1.17  2005/03/09 09:30:43  marfav
	Order queue management changed
	now the orders are put in a waitlist when precondition fails
	the database will resume the waiting orders when the right input is inventoried
	
	Revision 1.16  2005/03/03 11:49:09  marfav
	Precondition check queries now use only one db connection per order
	
	Revision 1.15  2005/03/02 14:06:18  marfav
	Added cache support for downloaded files
	
	Revision 1.14  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.13  2004/10/19 08:14:50  marfav
	Added support for satellite and mission in queries
	
	Revision 1.12  2004/08/03 14:01:52  marfav
	Align to new version of libTask using TimeRange instead of dbGeoTimeRange
	
	Revision 1.11  2003/11/19 10:13:13  marfav
	Prepared for use new sortable SpecialQuery
	
	Revision 1.10  2003/09/23 14:02:42  fracar
	handling verbose execution mode in support to Thin Layer MMI interactive operations
	
	Revision 1.9  2003/05/13 08:45:19  marfav
	Fixed memory leak at cleanup
	
	Revision 1.8  2003/05/07 15:06:27  marfav
	Improved memory allocation management and exception handling
	
	Revision 1.7  2003/04/30 16:20:16  marfav
	Align to ICD v2.0 and some logging improvement
	
	Revision 1.6  2003/04/18 12:59:33  marfav
	Using namespaces removed from .h
	
	Revision 1.5  2003/03/11 18:25:50  marfav
	Patrol message handling added
	
	Revision 1.4  2003/02/10 18:50:19  fracar
	prepared for multi-interval management
	
	Revision 1.3  2003/02/07 08:30:48  fracar
	handling downloaded file split and improved messaging
	
	Revision 1.2  2003/02/04 09:58:34  marfav
	Added LOG field to headers
	


*/


#ifndef _INPUT_DOWNLOADER_H
#define _INPUT_DOWNLOADER_H


#include <CacheManager.h>
#include <PCC_Base.h>

#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class InputDownloader // PRQA S 2109
{
  public:
    // Exceptions
    exDECLARE_EXCEPTION (InvalidUse, exException);

  public:
    InputDownloader();
    ~InputDownloader() throw() ;

    InputDownloader (const PCC_Base::SymTableParams&, CacheManager*);
    InputDownloader (const InputDownloader& );
    void download(const std::string& destDir);
    void stopDownload();
    PCC_Base::SymTableParams getSymTableParams();

    InputDownloader& operator = (const InputDownloader& );


  protected: // methods

  protected: // attributes

private: // methods

private: // attributes
	  PCC_Base::SymTableParams _symtableParams;
	  bool _stopRequested;
	  CacheManager* _cacheManager;

  private: // static stuff...
    ACS_CLASS_DECLARE_DEBUG_LEVEL (InputDownloader)
};


_ACS_END_NAMESPACE

#endif // _INPUT_DOWNLOADER_H

