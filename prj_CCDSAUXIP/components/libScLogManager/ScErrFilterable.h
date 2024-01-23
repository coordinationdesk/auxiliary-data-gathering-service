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

	$Prod: A.C.S. Scheduler Log Manager Library $

	$Id$

	$Author$

	$Log$
	Revision 1.6  2009/06/26 10:49:15  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.5  2008/02/21 11:38:25  clanas
	Added detail methods.
	
	Revision 1.4  2006/11/03 05:52:47  marpas
	new method according the new exFilterable I/F
	
	Revision 1.3  2004/12/03 17:55:32  marfav
	Header fixed
	
	Revision 1.2  2004/09/23 15:17:19  marfav
	New object hierarchy structure
	
	Revision 1.1.1.1  2004/09/14 16:57:29  marfav
	Import libScLogManager
	


*/


#ifndef _ScErrFilterable_H_
#define _ScErrFilterable_H_

#include <acs_c++config.hpp>
#include <ScFilterable.h>


_ACS_BEGIN_NAMESPACE(acs)


/*! \class ScErrFilterable
	
	A filterable class to log processor tracking messages 
*/
class ScErrFilterable :
	public ScFilterable
{
public:
	ScErrFilterable() ;
	explicit ScErrFilterable(const std::string&);
	virtual ~ScErrFilterable() throw() ;
	
	/// convenience function doing
	//
	// ScErrFilterable m(originator) ;
	// m << msg ;
	// os << msg ;
	//
	static void notify(const std::string &msg, const std::string& originator="", exostream &o = excerr) ;
	
	virtual bool defaultNotify() const ;
	virtual std::string detail() const { return "" ;} // PRQA S 2131 2
	virtual void detail(const std::string &) {} 

private:
	// declared but not implemented to prevent their use
	ScErrFilterable(const ScErrFilterable &) ;
	ScErrFilterable &operator=(const ScErrFilterable &) ;
} ;

_ACS_END_NAMESPACE


#endif // _ScErrFilterable_H_


