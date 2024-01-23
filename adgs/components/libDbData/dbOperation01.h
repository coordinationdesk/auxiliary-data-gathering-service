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

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 2.4  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.3  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.2  2012/02/13 16:51:16  marpas
	refactoring in progress
	
	Revision 2.1  2009/05/14 11:49:16  marpas
	some rules fixing
	comments makeup
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/11/03 09:54:19  marpas
	dbOperation01Factory class added and used
	
	Revision 1.2  2005/11/02 17:56:05  marpas
	almost completed
	
	Revision 1.1  2005/10/28 17:36:02  marpas
	dbOperation01 class added
	

*/


#ifndef _dbOperation01_H_
#define _dbOperation01_H_

#include <acs_c++config.hpp>
#include <Operation01.h>
#include <exMacros.h>

#include <string>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)

//
//
//
// class dbOperation01
//
//
//
class dbOperation01: virtual public Operation01 // PRQA S 1703, 2109
{
public:

	dbOperation01(const std::string &feature,
				  const std::string &instance) ; // max 8 chars
				  
	virtual ~dbOperation01() throw() ;

	dbOperation01(const dbOperation01 &) ;
	dbOperation01 &operator=(const dbOperation01 &) ; // operator =

	// returns how much data have been set
	virtual size_t get(void *, size_t size, std::string &, const std::vector<std::string> &mods) const ;
	
	virtual void set(const void *, size_t size, const std::string &) const ;
	
	virtual bool remove(const std::string &mod) const ; // PRQA S 2502
private:
	dbOperation01() ; // declared but not implemented

private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbOperation01) ;
} ;


_ACS_END_NAMESPACE

#endif // _dbOperation01_H_
