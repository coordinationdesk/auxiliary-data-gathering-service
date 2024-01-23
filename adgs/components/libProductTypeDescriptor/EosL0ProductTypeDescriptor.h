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
	Revision 1.2  2013/12/09 14:01:11  giucas
	Library completely revisited.
	
	Revision 1.1  2006/10/05 12:08:47  giucas
	Added
	


*/

#ifndef _EosL0ProductTypeDescriptor_H_
#define _EosL0ProductTypeDescriptor_H_ 

#include <acs_c++config.hpp>
#include <L0ProductTypeDescriptor.h>


_ACS_BEGIN_NAMESPACE(acs)

class EosL0ProductTypeDescriptor : public L0ProductTypeDescriptor // PRQA S 2153
{
public:

	/**
	 * \brief  Default Class constructor. Using this constructor the class
	 *		   data are read from database using Configuration Space
	 */	
	EosL0ProductTypeDescriptor();

	/**
	 * \brief  Class constructor
	 * \param  Satellite Name
	 */	
	explicit EosL0ProductTypeDescriptor(const std::string&);


	/**
	 * \brief  Class destructor 
	 */
	virtual ~EosL0ProductTypeDescriptor() throw();

private:

	/**
	 * \brief  Copy constructor
	 */
	EosL0ProductTypeDescriptor(const EosL0ProductTypeDescriptor & ); 


	/**
	 * \brief  Operator=
	 */
	EosL0ProductTypeDescriptor &operator=(const EosL0ProductTypeDescriptor &);


};

_ACS_END_NAMESPACE

#endif //_EosL0ProductTypeDescriptor_H_
