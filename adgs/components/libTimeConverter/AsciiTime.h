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
    Revision 2.2  2014/03/18 12:08:34  marpas
    Coding best practices applied
    warnings removed (g++ & qa)
    interface rationalization


*/

#ifndef _AsciiTime_H_
#define _AsciiTime_H_ 


#include <acs_c++config.hpp>
#include <exException.h>
#include <vector>
#include <TimeFormat.h>

_ACS_BEGIN_NAMESPACE(acs)

	
class AsciiTime 
{
public:


    /**
	 * \brief  Class constructor 
	 * 
	 * \param Time value representation expressed in string format
	 * \param TimeFormat::TimeReference Time reference
	 * \param TimeFormat::AsciiTimeFormat Ascii time format
	 */	
	explicit AsciiTime(const std::string & v = "", TimeFormat::TimeReference r =TimeFormat::TC_TIME_UNDEF, TimeFormat::AsciiTimeFormat f=TimeFormat::TC_ASCII_UNDEF );

	/**
	 * \brief  Class destructor 
	 *    
	 */
	~AsciiTime() throw() ;

	/**
	 * \brief  Class copy constructor 
	 *    
	 */ 
	AsciiTime(const AsciiTime & ); 

	/**
	 * \brief  Operator= 
	 *    
	 */
	AsciiTime &operator=(const AsciiTime &);

	/**
	 * \brief  Set the object time value
	 *    
	 */
	void setValue(const std::string& value) { _value = value; };

	/**
	 * \brief  Set the object time reference
	 *    
	 */
	void setRef(TimeFormat::TimeReference ref) throw() { _reference = ref; };

	/**
	 * \brief  Set the object time format
	 *    
	 */
	void setFormat(TimeFormat::AsciiTimeFormat format) throw() { _format = format; };

	/**
	 * \brief  Return the object time value as a string value
	 *    
	 */
	const std::string &getValue() const throw() { return _value; };

	/**
	 * \brief  Get the object time reference
	 * \retval Possible values are listed in TimeFormat::TimeReference
	 */
	TimeFormat::TimeReference getRef() const throw() { return _reference; };

	/**
	 * \brief  Get the object time format
	 *    
	 */
	TimeFormat::AsciiTimeFormat getFormat() const throw() { return _format;};



private:

	std::string						_value;
	TimeFormat::TimeReference		_reference;
	TimeFormat::AsciiTimeFormat		_format;
};
_ACS_END_NAMESPACE


#endif //_AsciiTime_H_
