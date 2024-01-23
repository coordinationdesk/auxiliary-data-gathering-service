// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA_AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.2  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.1  2012/02/13 16:51:16  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/11/03 09:54:19  marpas
	dbOperation01Factory class added and used
	
	

*/


#ifndef _dbOperation01Factory_H_
#define _dbOperation01Factory_H_

#include <acs_c++config.hpp>
#include <Operation01Factory.h>


_ACS_BEGIN_NAMESPACE(acs)

//
//
//
// class dbOperation01Factory
//
//
//
class dbOperation01Factory : virtual public Operation01Factory // PRQA S 1703
{
public:				  
	dbOperation01Factory() ;
	dbOperation01Factory(const dbOperation01Factory &) = delete ;
	dbOperation01Factory &operator=(const dbOperation01Factory &) = delete ;
	dbOperation01Factory( dbOperation01Factory &&) = delete ;
	dbOperation01Factory &operator=( dbOperation01Factory &&) = delete ;
	virtual ~dbOperation01Factory() noexcept ;

protected:
	virtual Operation01 *createOperation(const std::string &feature, const std::string &instance) ;

} ;

_ACS_END_NAMESPACE

#endif // _dbOperation01Factory_H_
