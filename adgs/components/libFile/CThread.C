// PRQA S 1050 EOF
/*

	Copyright 1995-2005, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Thread Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2012/11/29 19:31:29  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 2.2  2012/02/07 17:57:43  marpas
	refactoring in progress
	
	Revision 2.1  2009/06/26 10:42:32  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.0  2006/02/28 08:33:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/11/03 09:39:03  marpas
	new class added
	

*/

#include <CThread.h>
#include <File.h>

using namespace acs;
using namespace std;

CThread::CThread(unsigned long msec) : 
    Thread(),                   // PRQA S 4052
	_msec(msec),
    _name()
{
}



CThread::~CThread() throw()
{
}

void CThread::run()
{
	nanosleep(_msec*1000000) ;  // PRQA S 3084, 4400

	doWork() ;
}

void CThread::doWork() throw() 
{
    // tricky part ... ask to MP for explainations
    // do not modify it even if it seems an error !!!!
	File::ifdstream	ifd(reinterpret_cast<unsigned long>(this)) ; // PRQA S 3000, 3010, 3081
	ifd >> _name ;
}

const string &CThread::name() const // PRQA S 4120
{
	return _name ;
}
