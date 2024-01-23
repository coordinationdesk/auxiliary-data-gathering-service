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
	Revision 2.1  2013/02/21 14:44:20  marfav
	Compiler warnings removed
	
	Revision 2.0  2006/02/28 10:09:06  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 18:02:10  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/09/14 17:02:38  marfav
	Import libTaskExecution
	
	

*/


#include <tlLogFile.h>

using namespace std;

tlLogFile::tlLogFile() :
	_log(),
	_mutex()
{}

tlLogFile::tlLogFile(const string& aPath) :
	_log(aPath.c_str()),
	_mutex()
{
}

void
tlLogFile::logMessageString(const string& msg)
{
  acs::Lock guard(_mutex); //automaticaly acquire the lock

  _log << msg << endl;

  // force message writing to logfile
  _log.flush();
}
