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
    Revision 2.4  2014/03/18 12:08:34  marpas
    Coding best practices applied
    warnings removed (g++ & qa)
    interface rationalization

    Revision 2.3  2013/12/03 14:21:08  giucas
    MP: Interface improved

    Revision 2.2  2013/11/14 14:42:20  marpas
    coding best practices in progress


*/

#ifndef _TransportTime_H_
#define _TransportTime_H_ 


#include <acs_c++config.hpp>
#include <exException.h>
#include <vector>
#include <TimeFormat.h>

_ACS_BEGIN_NAMESPACE(acs)

	
class TransportTime // PRQA S 2109
{
public:

	/**
	 * \brief  Default Class constructor 
	 *    
	 */	
	TransportTime(TimeFormat::TimeReference, TimeFormat::TransportTimeFormat) ;
    
	TransportTime(int days, int secs_msecs, int usecs,
                  TimeFormat::TimeReference, TimeFormat::TransportTimeFormat) ;
	/**
	 * \brief  Class constructor 
	 * 
	 * \param Time value representation expressed as a vector of integer values
	 * \param TimeFormat::TimeReference Time reference
	 * \param TimeFormat::TransportTimeFormat Ascii time format
	 */	
	TransportTime(const std::vector<long>&, 
                  TimeFormat::TimeReference, TimeFormat::TransportTimeFormat );


	/**
	 * \brief  Class destructor 
	 *    
	 */
	~TransportTime() throw() {} ;

	/**
	 * \brief  Class copy constructor 
	 *    
	 */ 
	TransportTime(const TransportTime & ); 

	/**
	 * \brief  Operator= 
	 *    
	 */
	TransportTime &operator=(const TransportTime &);

	/**
	 * \brief  Return the object time value as a vector of integer values
	 *    
	 */
	const std::vector<long> &getValue() const throw() { return _value; } ;

	/**
	 * \brief  Get the object time reference
	 * \retval Possible values are listed in TC_Time_ref_enum
	 */
	TimeFormat::TimeReference getRef() const throw() { return _reference; };

	/**
	 * \brief  Get the object time format
	 *    
	 */
	TimeFormat::TransportTimeFormat getFormat() const throw() { return _format; } ;
    void setValue(const std::vector<long>& value) throw()
    {	
	    if(_format != TimeFormat::TC_TRANS_UNDEF)
	    {
		    if( (_format == TimeFormat::TC_TRANS_CRYO_TM) || 
                (_format == TimeFormat::TC_TRANS_CRYO_TM_SIRAL) ) {
			    _dValue = value[0] + value[1] /86400000.0 + value[2] / 86400000000.0; // PRQA S 3011, 4400
            }
		    else {
			    _dValue = value[0] + value[1] /86400.0 + value[2] / 86400000000.0;// PRQA S 3011, 4400
            }
	    }
	    _value = value;
    }
private:
    TransportTime() ; // not implemented
    static long double computeValue(long days, long secs_msecs, long usecs, TimeFormat::TransportTimeFormat format) throw() // PRQA S 4020
    {	
	    switch(format) {
        case TimeFormat::TC_TRANS_UNDEF: { return 0.0 ; }
        case TimeFormat::TC_TRANS_CRYO_TM:
        case TimeFormat::TC_TRANS_CRYO_TM_SIRAL:
            {  // secs_msecs are msecs
			    return days + secs_msecs /86400000.0L + usecs / 86400000000.0L; // PRQA S 3011, 4400
            }
		default: // secs_msecs are secs 
            {
			    return days + secs_msecs /86400.0L + usecs / 86400000000.0L; // PRQA S 3011, 4400
            }
	    }
    }

    static long double computeValue(long days, long secs_msecs, long usecs, TimeFormat::TransportTimeFormat format, std::vector<long>&v) throw() // PRQA S 4020
    {	
	    switch(format) {
        case TimeFormat::TC_TRANS_UNDEF: { return 0.0 ; }
        case TimeFormat::TC_TRANS_CRYO_TM:
        case TimeFormat::TC_TRANS_CRYO_TM_SIRAL:
            {  // secs_msecs are msecs
                v.push_back(days) ;
                v.push_back(secs_msecs) ;                
                v.push_back(usecs ) ;
			    return days + secs_msecs /86400000.0L + usecs / 86400000000.0L; // PRQA S 3011, 4400
            }
		default: // secs_msecs are secs 
            {
                v.push_back(days) ;
                v.push_back(secs_msecs) ;                
                v.push_back(usecs ) ;
			    return days + secs_msecs /86400.0L + usecs / 86400000000.0L; // PRQA S 3011, 4400
            }
	    }
    }

private:

	std::vector<long>					_value;
	TimeFormat::TimeReference			_reference;
	TimeFormat::TransportTimeFormat		_format;
	long double							_dValue;
};


_ACS_END_NAMESPACE

#endif //_TransportTime_H_
