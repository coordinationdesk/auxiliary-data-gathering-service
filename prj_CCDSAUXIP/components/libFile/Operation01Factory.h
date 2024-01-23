/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA
    
	$Prod: A.C.S. File Library $

	$Id$

	$Author$

	$Log$
	Revision 2.2  2012/11/29 19:31:29  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 2.1  2012/02/13 10:59:05  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 08:33:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/11/04 12:29:28  marpas
	newObject method name fixed
	
	Revision 1.1  2005/11/03 08:38:35  marpas
	new class added
	
	

*/


#ifndef _Operation01Factory_H_
#define _Operation01Factory_H_

#include <acs_c++config.hpp>

#include <string>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)

class Operation01 ;

//
//
//
// class Operation01Factory
//
//
//
class Operation01Factory    // PRQA S 2109
{
public:				  
	Operation01Factory(const Operation01Factory &) = delete ;
	Operation01Factory &operator=(const Operation01Factory &)  = delete ;

	virtual ~Operation01Factory() noexcept ;

	static Operation01 *newObject(const std::string &feature, const std::string &instance) ;

protected:
	Operation01Factory() ;
	virtual Operation01 *createOperation(const std::string &feature, const std::string &instance) = 0 ;

private:
	static Operation01Factory *_theFactory ;
} ;


_ACS_END_NAMESPACE

#endif // _Operation01Factory_H_
