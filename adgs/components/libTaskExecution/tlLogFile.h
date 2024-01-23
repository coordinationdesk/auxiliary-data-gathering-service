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

	$Prod: A.C.S. ThinLayer TaskTable Execution Library $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2011/01/24 15:48:47  marpas
	Mutex.h does not longer exixts
	
	Revision 2.0  2006/02/28 10:09:06  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 18:02:10  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/09/14 17:02:38  marfav
	Import libTaskExecution
	
	

*/


#ifndef _TL_LOG_FILE_H
#define _TL_LOG_FILE_H

#include <string>
#include <fstream>

#include <ThreadSafe.h>

// This class maps the description of a task as derived from the GEN_PROC icd.
class tlLogFile // PRQA S 2109
{
  public:
    tlLogFile();
    explicit tlLogFile(const std::string& aPath);
	~tlLogFile() throw() {} 

    /*
     * Since in principle more tlTaskLauncher can share the same log file,
     * logging shall be accomplished through this method, which is thread safe.
     * The output file is flushed after each write.
     */
    void logMessageString(const std::string& msg);

  private:
    // define but not implement next methods, to avoid their usage
    // (we don't actually need that, but acs::ThreadSafe cannot be copied, so...
    tlLogFile(const tlLogFile& f);
    tlLogFile& operator = (const tlLogFile& f);

  private: // attributes
    std::ofstream _log;
    acs::ThreadSafe _mutex;
};

#endif // _TL_LOG_FILE_H

