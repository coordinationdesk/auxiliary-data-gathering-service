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
	Revision 2.1  2013/10/22 06:20:42  giucas
	New logging macros adopted
	
	Revision 2.0  2006/02/28 09:14:47  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2004/12/03 09:33:26  giucas
	Header fixed
	
	Revision 1.2  2004/09/17 13:59:33  giucas
	Added ability to handle counter in double format
	
	Revision 1.1  2003/09/03 14:41:34  giucas
	First release
	


*/

#ifndef _OnBoardTime_H_
#define _OnBoardTime_H_ 


#include <acs_c++config.hpp>
#include <exException.h>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)

class OnBoardTime 
{
public:

	/*! class OnBoardTimeException declaration */
	exDECLARE_EXCEPTION(OnBoardTimeException,exException) ; // PRQA S 2131, 2502

	/**
	 * \brief  Default Class constructor 
	 *    
	 */	
	OnBoardTime();

	/**
	 * \brief  Class constructor 
	 * 
	 * \param The OBT counter value expressed as an unsigned long vector
	 */	
	explicit OnBoardTime(const std::vector<unsigned long int>&);

	/**
	 * \brief  Class constructor 
	 * 
	 * \param The OBT counter value expressed as a double
	 */	
	explicit OnBoardTime(const double&);


	/**
	 * \brief  Class destructor 
	 *    
	 */
	~OnBoardTime();

	/**
	 * \brief  Class copy constructor 
	 *    
	 */ 
	OnBoardTime(const OnBoardTime & ); 

	/**
	 * \brief  Operator= 
	 *    
	 */
	OnBoardTime &operator=(const OnBoardTime &);


	/**
	 * \brief  Set Counter value as a vector of unsigned long int
	 *    
	 */
	void setValue(const std::vector<unsigned long int>& v) { _valueVec = v; }

	/**
	 * \brief  Set Counter value as a double
	 *    
	 */
	void setValue(const double& v) throw() { _value = v; }

	/**
	 * \brief  Return the object Counter value as a vector of unsigned long integer values
	 *    
	 */
	const std::vector<unsigned long int> & getValue() const throw() { return _valueVec; }

	/**
	 * \brief  Return the object Counter value as a double
	 *    
	 */
	double getValueInDouble() const throw() { return _value; }

private:

	std::vector<unsigned long int>		_valueVec;
	double								_value;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(OnBoardTime) ;

};
	

_ACS_END_NAMESPACE


#endif //_OnBoardTime_H_
