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
#include <acs_c++config.hpp>

_ACS_BEGIN_NAMESPACE(acs)

// constructor
Operation01::Operation01(const std::string &feature, 
						 const std::string &instance) :
	_feature(feature),
	_instance(instance)
{
	//empty 
}


const std::string &Operation01::getFeature() const noexcept // PRQA S 4120
{
	return _feature ;
}

const std::string &Operation01::getInstance() const noexcept // PRQA S 4120
{
	return _instance ;
}

_ACS_END_NAMESPACE
