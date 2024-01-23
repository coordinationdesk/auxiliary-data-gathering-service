/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

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
#include <chrono>
#include <thread>

_ACS_BEGIN_NAMESPACE(acs)

CThread::CThread(unsigned long msec) : 
    Thread(),
	_msec(msec)
{
}

void CThread::run()
{
    std::this_thread::sleep_for (std::chrono::milliseconds(_msec)) ;

	doWork() ;
}

void CThread::doWork() noexcept 
{
    // tricky part ... ask to MP for explainations
    // do not modify it even if it seems an error !!!!
	File::ifdstream	ifd(reinterpret_cast<unsigned long>(this)) ; // NOSONAR
	ifd >> _name ;
}

const std::string &CThread::name() const 
{
	return _name ;
}

_ACS_END_NAMESPACE
