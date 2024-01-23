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

*/

#ifndef _ExtraInfo_H_
#define _ExtraInfo_H_ 


#include <acs_c++config.hpp>
#include <exMacros.h>


_ACS_BEGIN_NAMESPACE(acs)
	
class ExtraInfo 
{
public:


	/**
	 * \brief  Default Class constructor 
	 *    
	 */	
	ExtraInfo();


	/**
	 * \brief  Class destructor 
	 *    
	 */
	~ExtraInfo() throw();

	/**
	 * \brief  Class copy constructor 
	 *    
	 */ 
	ExtraInfo(const ExtraInfo & ); 

	/**
	 * \brief  Operator= 
	 *    
	 */
	ExtraInfo &operator=(const ExtraInfo &);

	/**
	 * \brief  Set the Model dependent field value
	 *    
	 */
	void modelDependent(const std::vector<double>& value) { _modelDependent = value; };

	/**
	 * \brief  Set the Model independent field value
	 *    
	 */
	void modelIndependent(const std::vector<double>& value) { _modelIndependent = value; };


	/**
	 * \brief  Return the Model Dependent vector
	 *    
	 */
	std::vector<double> modelDependent() const { return _modelDependent; };

	/**
	 * \brief  Return the Model independent vector
	 */
	std::vector<double> modelIndependent() const { return _modelIndependent; };

	/**
	 * \brief  Return the Model independent vector
	 */
	std::string show() const ;


private:

	std::vector<double> _modelDependent;
	std::vector<double> _modelIndependent;

};
_ACS_END_NAMESPACE

#endif //_ExtraInfo_H_
