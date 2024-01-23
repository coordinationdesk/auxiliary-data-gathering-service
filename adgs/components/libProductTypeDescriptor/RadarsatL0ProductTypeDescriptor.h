// PRQA S 1050 EOF
/*

	Copyright 2002-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 1.2  2013/12/09 14:01:12  giucas
	Library completely revisited.
	
	Revision 1.1  2009/01/21 11:18:17  giucas
	First issue
	

*/

#ifndef _RadarsatL0ProductTypeDescriptor_H_
#define _RadarsatL0ProductTypeDescriptor_H_ 

#include <acs_c++config.hpp>
#include <L0ProductTypeDescriptor.h>


_ACS_BEGIN_NAMESPACE(acs)

class RadarsatL0ProductTypeDescriptor : public L0ProductTypeDescriptor // PRQA S 2153
{
public:

	/*! class RadarsatL0ProductTypeDescriptorException declaration */
	exDECLARE_EXCEPTION(RadarsatL0ProductTypeDescriptorException,exException) ; // PRQA S 2131, 2502


	/**
	 * \brief  Default Class constructor. Using this constructor the class
	 *		   data are read from database using Configuration Space
	 */	
	RadarsatL0ProductTypeDescriptor();

	/**
	 * \brief  Class constructor
	 * \param  Satellite Name
	 */	
	explicit RadarsatL0ProductTypeDescriptor(const std::string&);


	/**
	 * \brief  Class destructor 
	 */
	virtual ~RadarsatL0ProductTypeDescriptor() throw();

private:

	/**
	 * \brief  Copy constructor
	 */
	RadarsatL0ProductTypeDescriptor(const RadarsatL0ProductTypeDescriptor & ); 


	/**
	 * \brief  Operator=
	 */
	RadarsatL0ProductTypeDescriptor &operator=(const RadarsatL0ProductTypeDescriptor &);

};

_ACS_END_NAMESPACE

#endif //_RadarsatL0ProductTypeDescriptor_H_
