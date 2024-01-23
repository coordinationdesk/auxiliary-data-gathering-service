// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$

    $Log$
    Revision 2.3  2014/03/18 12:08:34  marpas
    Coding best practices applied
    warnings removed (g++ & qa)
    interface rationalization

    Revision 2.2  2013/11/14 14:42:19  marpas
    coding best practices in progress


*/

#ifndef _ProcessingTime_H_
#define _ProcessingTime_H_ 


#include <acs_c++config.hpp>
#include <exException.h>
#include <vector>
#include <TimeFormat.h>

_ACS_BEGIN_NAMESPACE(acs)
	
class ProcessingTime 
{
public:

	/*! class ProcessingTimeException declaration */
	exDECLARE_EXCEPTION(ProcessingTimeException,exException) ; // PRQA S 2131, 2502

	/**
	 * \brief  Class constructor 
	 * 
	 * \param Time value representation expressed in double format
	 * \param TimeFormat::TimeReference Time reference
	 * \param TimeFormat::ProcessingTimeFormat Processing time format
	 */	
	explicit ProcessingTime(long double v= 0.0, 
            TimeFormat::TimeReference r= TimeFormat::TC_TIME_UNDEF, 
            TimeFormat::ProcessingTimeFormat f = TimeFormat::TC_PROC_UNDEF);


	/**
	 * \brief  Class destructor 
	 *    
	 */
	~ProcessingTime();
	/**
	 * \brief  Class copy constructor 
	 *    
	 */ 
	ProcessingTime(const ProcessingTime & ); 

	/**
	 * \brief  Operator= 
	 *    
	 */
	ProcessingTime &operator=(const ProcessingTime &);

	/**
	 * \brief  Set the object time value
	 *    
	 */
	void setValue(long double value) throw()  { _value = value; };

	/**
	 * \brief  Set the object time reference
	 *    
	 */
	void setRef(TimeFormat::TimeReference ref) throw() { _reference = ref; };

	/**
	 * \brief  Set the object time format
	 *    
	 */
	void setFormat(TimeFormat::ProcessingTimeFormat format) throw() { _format = format; };

	/**
	 * \brief  Return the object time value as a double value
	 *    
	 */
	long double getValue() const throw()  { return _value; };

	/**
	 * \brief  Get the object time reference
	 * \retval Possible values are listed in TC_Time_ref_enum
	 */
	TimeFormat::TimeReference getRef() const throw()  { return _reference; };

	/**
	 * \brief  Get the object time format
	 *    
	 */
	TimeFormat::ProcessingTimeFormat getFormat() const throw()  { return _format; };



private:

	long double							_value;
	TimeFormat::TimeReference			_reference;
	TimeFormat::ProcessingTimeFormat	_format;
};


_ACS_END_NAMESPACE


#endif //_ProcessingTime_H_
