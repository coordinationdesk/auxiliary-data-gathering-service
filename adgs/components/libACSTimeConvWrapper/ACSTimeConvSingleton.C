// PRQA S 1050 EOF
/* 

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: $

	$Id$

	$Author$

	$Log $

 */


#include <ACSTimeConvSingleton.h>
#include <ACSTimeConvWrap.h>
#include <acsTimeConvWrapV.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

namespace {
	acsTimeConvWrapV vers;
}


ACSTimeConvWrap* ACSTimeConvSingleton::_converter = 0;

//constructor
ACSTimeConvSingleton::ACSTimeConvSingleton() :
    TimeConverterSingleton(),
    _mutex()
{	
	TimeConverterSingleton::Register(this);
}

//destructor
ACSTimeConvSingleton::~ACSTimeConvSingleton() throw() 
{
}


TimeConverter* ACSTimeConvSingleton::getConverter()
{
	if(_converter == 0)
	{
		acs::ThreadSafe::Lock guard(_mutex);
		{
			if(_converter == 0)
			{
				_converter = new ACSTimeConvWrap ;
			}
		}		
	}
	return _converter;	
}

_ACS_END_NAMESPACE

