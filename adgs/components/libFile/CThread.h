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
	
	Revision 2.2  2009/06/26 10:42:32  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.1  2009/03/13 15:26:44  marpas
	some rule violations fixed
	
	Revision 2.0  2006/02/28 08:33:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/11/03 09:39:03  marpas
	new class added
	

*/


#ifndef _CThread_H_
#define _CThread_H_

#include <acs_c++config.hpp>
#include <Thread.h>

_ACS_BEGIN_NAMESPACE(acs)

//////////////////////////
//                      //
// CThread class        //
//                      //
//////////////////////////

// note virtual inheritance is harmless
class CThread : public Thread // PRQA S 2109
{
public:
	explicit CThread(unsigned long msec = 100) ;
	virtual ~CThread() throw() ;

private:
	//declared but not used to prevent their use
	CThread(const CThread &) ;
	CThread& operator=(const CThread &) ;
public:
	virtual void run() ;
	const std::string &name() const ;
		
private:

	void doWork() throw() ;
		
private:
	unsigned long _msec;
	std::string _name ;
};	


_ACS_END_NAMESPACE

#endif /* _CThread_H_ */

