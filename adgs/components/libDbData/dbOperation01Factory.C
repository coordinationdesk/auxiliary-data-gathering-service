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


#include <dbOperation01Factory.h>
#include <dbOperation01.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


// constructor
dbOperation01Factory::dbOperation01Factory() :
    Operation01Factory()  
{
}


dbOperation01Factory::~dbOperation01Factory() throw() {}

Operation01 *dbOperation01Factory::createOperation(const std::string &feature, const std::string &instance)
{
	return new dbOperation01(feature,instance) ;
}

_ACS_END_NAMESPACE
