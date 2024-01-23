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

    $log: $
*/

#include <TransportTime.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

TransportTime::TransportTime(int days, int secs_msecs, int usecs,
              TimeFormat::TimeReference reference, 
              TimeFormat::TransportTimeFormat format) :
    _value(),	
	_reference(reference),
	_format(format),
	_dValue(computeValue(days,secs_msecs,usecs,_format,_value ))
{
}

TransportTime::TransportTime(
              TimeFormat::TimeReference reference, 
              TimeFormat::TransportTimeFormat format) :
    _value(),	
	_reference(reference),
	_format(format),
	_dValue()
{
}

TransportTime::TransportTime(const vector<long>& value,
						     TimeFormat::TimeReference reference, 
					 		 TimeFormat::TransportTimeFormat  format) :
	_value(value),
	_reference(reference),
	_format(format),
	_dValue(computeValue(_value[0],_value[1],_value[2],_format)) // PRQA S 4400
{
} 




//copy constructor
TransportTime::TransportTime(const TransportTime & e) :
	_value(e._value),	
	_reference(e._reference),
	_format(e._format),
	_dValue(e._dValue)
{
}


/////////////////////////////////////////////////////////////////////////////////////////
// operators 
/////////////////////////////////////////////////////////////////////////////////////////

//operator = 
TransportTime&  TransportTime::operator=(const TransportTime & e)
{
	if (this != &e) {
		_value		= e._value;
		_reference	= e._reference;
		_format		= e._format;
		_dValue		= e._dValue;
	}
	return *this ;
}


/////////////////////////////////////////////////////////////////////////////////////////
// Public methods 
/////////////////////////////////////////////////////////////////////////////////////////

/*
void TransportTime::setFormat(TimeFormat::TransportTimeFormat format)
{
	if( (_format != TimeFormat::TC_TRANS_UNDEF) && (_value.size() > 0) )
	{
		if( (_format == TimeFormat::TC_TRANS_CRYO_TM) || 
            (_format == TimeFormat::TC_TRANS_CRYO_TM_SIRAL) ) {
			_dValue = _value[0] + _value[1] /86400000.0 + _value[2] / 86400000000.0;
        }
		else {
			_dValue = _value[0] + _value[1] /86400.0 + _value[2] / 86400000000.0;
        }
	}
	_format = format;
}
*/
_ACS_END_NAMESPACE
