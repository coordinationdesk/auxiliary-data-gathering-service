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
	Revision 2.2  2012/11/29 19:31:29  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 2.1  2012/02/07 17:57:44  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 08:33:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/11/03 08:38:17  marpas
	I/F slightly changed
	
	Revision 1.1  2005/11/02 17:07:12  marpas
	just added
	

*/


#include <Operation01.h>

using namespace std ;

using namespace acs ;

// constructor
Operation01::Operation01(const string &feature, 
						 const string &instance) :
	_feature(feature),
	_instance(instance)
{
	//empty 
}


// copy constructor
Operation01::Operation01(const Operation01 &rhs) :
	_feature(rhs._feature),
	_instance(rhs._instance)
{
	//empty 
}	


Operation01::~Operation01() throw() 
{
	//empty 
}


// operator =
Operation01 &Operation01::operator=(const Operation01 &rhs)
{
	if(this != &rhs)
	{
		_feature=rhs._feature;
		_instance = rhs._instance ;
	}
	return *this ;
}


const std::string &Operation01::getFeature() const throw() // PRQA S 4120
{
	return _feature ;
}

const std::string &Operation01::getInstance() const throw() // PRQA S 4120
{
	return _instance ;
}
