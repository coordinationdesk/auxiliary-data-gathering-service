// PRQA S 1050 EOF
/*

	Copyright 2002-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 1.3  2013/12/09 14:01:11  giucas
	Library completely revisited.
	
	Revision 1.2  2008/07/25 16:38:20  giucas
	Fixed erroneous call
	
	Revision 1.1  2006/11/08 10:35:40  giucas
	Added
	

*/

#ifndef _ErsL0ProductTypeDescriptor_H_
#define _ErsL0ProductTypeDescriptor_H_ 

#include <acs_c++config.hpp>
#include <L0ProductTypeDescriptor.h>


_ACS_BEGIN_NAMESPACE(acs)

class ErsL0ProductTypeDescriptor : public L0ProductTypeDescriptor // PRQA S 2153
{
public:

	/**
	 * \brief  Default Class constructor. Using this constructor the class
	 *		   data are read from database using Configuration Space
	 */	
	ErsL0ProductTypeDescriptor();

	/**
	 * \brief  Class constructor
	 * \param  Satellite Name
	 */	
	explicit ErsL0ProductTypeDescriptor(const std::string&);


	/**
	 * \brief  Class destructor 
	 */
	virtual ~ErsL0ProductTypeDescriptor() throw();

private:

	/**
	 * \brief  Copy constructor
	 */
	ErsL0ProductTypeDescriptor(const ErsL0ProductTypeDescriptor & ); 


	/**
	 * \brief  Operator=
	 */
	ErsL0ProductTypeDescriptor &operator=(const ErsL0ProductTypeDescriptor &);


};
	
_ACS_END_NAMESPACE

#endif //_ErsL0ProductTypeDescriptor_H_
