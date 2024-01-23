// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$

    $Log$
*/

#include <ExtraInfo.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;


//Class constructors
ExtraInfo::ExtraInfo() :
	_modelDependent(),
	_modelIndependent()
{
} 

//Class destructor
ExtraInfo::~ExtraInfo() throw()
{
}



//copy constructor
ExtraInfo::ExtraInfo(const ExtraInfo & e) :
	_modelDependent(e._modelDependent),
	_modelIndependent(e._modelIndependent)
{
}

//operator = 
ExtraInfo&  ExtraInfo::operator=(const ExtraInfo & e)
{
	if (this != &e) 
	{
		_modelDependent		= e._modelDependent;
		_modelIndependent	= e._modelIndependent;
	}
	return *this ;
}

string ExtraInfo::show() const
{
	ostringstream info;
	info << "ExtraInfo::show() : " << '\n';
	for(size_t i=0; i<_modelDependent.size(); i++)
	{
		info << "ModelDependent[" << i << "] = " 
			 << setprecision(17) << _modelDependent[i] << '\n'; // PRQA S 4400
	}
	for(size_t i=0; i<_modelIndependent.size(); i++)
	{
		info << "ModelIndependent[" << i << "] = " 
			 << setprecision(17) << _modelIndependent[i] << '\n'; // PRQA S 4400
	}
	info << "-----------------------------------------------------" << '\n';	
	return info.str();
}
_ACS_END_NAMESPACE
