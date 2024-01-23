/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. fileOperation01 Handler Factory $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2012/11/29 19:31:30  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 2.2  2012/02/13 10:59:05  marpas
	refactoring in progress
	
	Revision 2.1  2008/06/04 15:11:24  enrcar
	EC:: fileOperation01, fileOperation01Factory added
	
	
*/


#include <fileOperation01Factory.h>
#include <fileOperation01.h>

_ACS_BEGIN_NAMESPACE(acs)

void ACS_fileOperation01Factory_Instantiate() 
{
	_theDbOperationFactory2 = new fileOperation01Factory ;
}



Operation01 *fileOperation01Factory::createOperation(const std::string &feature, const std::string &instance)
{
	return new fileOperation01(feature,instance) ;
}

_ACS_END_NAMESPACE
