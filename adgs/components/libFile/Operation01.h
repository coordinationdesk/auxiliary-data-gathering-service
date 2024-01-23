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

	$Prod: A.C.S. File Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2012/11/29 19:31:29  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 2.2  2012/02/07 17:57:44  marpas
	refactoring in progress
	
	Revision 2.1  2009/05/14 13:43:46  enrcar
	EC:: c++ rules
	
	Revision 2.0  2006/02/28 08:33:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/11/03 08:38:17  marpas
	I/F slightly changed
	
	Revision 1.1  2005/11/02 17:07:12  marpas
	just added
	
	Revision 1.1  2005/10/28 17:36:02  marpas
	Operation01 class added
	

*/


#ifndef _Operation01_H_
#define _Operation01_H_


#include <string>
#include <vector>

namespace acs {

//
//
//
// class Operation01
//
//
//
class Operation01   // PRQA S 2109
{
public:

	Operation01(const std::string &feature, const std::string &instance) ; // max 8 chars
	virtual ~Operation01() throw() ;

	// returns how much data have been set
	virtual size_t get(void *, size_t size, std::string &, const std::vector<std::string> &mods) const = 0 ;
	
	virtual void set(const void *, size_t size, const std::string &) const = 0 ;
	
	virtual bool remove(const std::string &mod) const = 0 ;             // PRQA S 2502
protected:
	Operation01(const Operation01 &) ;

	Operation01 &operator=(const Operation01 &) ; // operator =

	const std::string &getFeature() const throw() ;
	const std::string &getInstance() const throw() ;
private:
	Operation01() ;	/* Unimplemented */
private:
	std::string _feature ;
	std::string _instance ;
} ;


} // namespace

#endif // _Operation01_H_
