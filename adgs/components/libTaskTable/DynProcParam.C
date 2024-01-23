/*
	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. TaskTable library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2014/06/10 15:49:47  marfav
	Added support to DynProcParams and SensingTimeFlag
	

        
*/ 

#include <DynProcParam.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(DynProcParam)
using namespace std;


const std::string DynProcParam::STRING("string");
const std::string DynProcParam::STRINGCAP("String");
const std::string DynProcParam::NUMBER("number");
const std::string DynProcParam::DATENUMBER("datenumber");


// class DynProcParam

DynProcParam::DynProcParam(const string& aParamName, const string& aParamType, const string& aParamDefault) :
	_paramName(aParamName), 
	_paramNameSet(false),
	_paramType(aParamType), 
	_paramTypeSet(false),
	_paramDefault(aParamDefault), 
	_paramDefaultSet(false)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    checkParamType();
}

DynProcParam::~DynProcParam() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

DynProcParam::DynProcParam(const DynProcParam& d) :
	_paramName(d._paramName), 
	_paramNameSet(d._paramNameSet),
	_paramType(d._paramType), 
	_paramTypeSet(d._paramTypeSet),
	_paramDefault(d._paramDefault), 
	_paramDefaultSet(d._paramDefaultSet)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    checkParamType();
}

DynProcParam::DynProcParam() :
	_paramName(), 
	_paramNameSet(false),
	_paramType(DynProcParam::STRING), 
	_paramTypeSet(false),
	_paramDefault(), 
	_paramDefaultSet(false)
{
    checkParamType();
}

DynProcParam&
DynProcParam::operator =(const DynProcParam& d)
{
	// Definition of = operator
	if (this != &d)
	{
		_paramName    = d._paramName;
		_paramType    = d._paramType;
		_paramDefault    = d._paramDefault;
		_paramNameSet = d._paramNameSet;
		_paramTypeSet = d._paramTypeSet;
		_paramDefaultSet = d._paramDefaultSet;
	}
	return *this;
}


bool
DynProcParam::isSetParamName () const throw()
{
	return _paramNameSet;
}

void
DynProcParam::setParamName(const string& aParamName)
{
	_paramName = aParamName;
	_paramNameSet = true;
}

const string&
DynProcParam::getParamName() const throw()
{
	return _paramName;
}


bool
DynProcParam::isSetParamType () const throw()
{
	return _paramTypeSet;
}

void
DynProcParam::setParamType(const string& aParamType)
{
	_paramType = aParamType;
	_paramTypeSet = true;
}

void DynProcParam::checkParamType() const
{
	if (_paramType != STRING && _paramType != STRINGCAP &&_paramType != NUMBER && _paramType != DATENUMBER)
	{
		exDynProcParamException e(string("Dynamic Parameter Type = [ ") + _paramType + string(" ] not allowed "));
		ACS_THROW (e) ;
	}
}

const string&
DynProcParam::getParamType() const throw()
{
	return _paramType;
}

bool
DynProcParam::isSetParamDefault () const throw()
{
	return _paramDefaultSet;
}

void
DynProcParam::setParamDefault(const string& aParamDefault)
{
	_paramDefaultSet = true;
	_paramDefault = aParamDefault;
}

const string&
DynProcParam::getParamDefault() const throw()
{
	return _paramDefault;
}


_ACS_END_NAMESPACE
