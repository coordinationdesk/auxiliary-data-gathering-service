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

	$Prod: A.C.S. Scheduler Abstract Commands Library $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2004/12/03 17:57:14  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:31:00  marfav
	Import libSchedCommands
	

*/

#include <ScAbstractCommand.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ScAbstractCommand::ScAbstractCommand () :
	_param (), _value ()
{}

ScAbstractCommand::ScAbstractCommand(const ScAbstractCommand &r) :
	_param (r._param), _value (r._value)
{}

ScAbstractCommand& ScAbstractCommand::operator= (const ScAbstractCommand &r)
{
    if (this != &r) {
	    _param = r._param ; 
        _value = r._value ;
    }
    return *this ;
}

ScAbstractCommand::~ScAbstractCommand() throw() 
{}


void ScAbstractCommand::param (const string &param) // PRQA S 4121
{
	_param = param;
}

const string &ScAbstractCommand::param() const throw()  // PRQA S 4120
{
	return _param;
}


void ScAbstractCommand::value (const string & value) // PRQA S 4121
{
	_value = value;
}

const string & ScAbstractCommand::value() const throw()  // PRQA S 4120
{
	return _value;
}


_ACS_END_NAMESPACE

