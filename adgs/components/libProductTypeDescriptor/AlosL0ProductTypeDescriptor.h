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
	Revision 1.2  2013/12/09 14:01:10  giucas
	Library completely revisited.
	
	Revision 1.1  2008/10/22 16:20:13  giucas
	First issue
	

*/

#ifndef _AlosL0ProductTypeDescriptor_H_
#define _AlosL0ProductTypeDescriptor_H_ 

#include <acs_c++config.hpp>
#include <L0ProductTypeDescriptor.h>


_ACS_BEGIN_NAMESPACE(acs)

class AlosL0ProductTypeDescriptor : public L0ProductTypeDescriptor // PRQA S 2153
{
public:
	/*
	 * \brief  Default Class constructor. Using this constructor the class
	 *		   data are read from database using Configuration Space
	 */	
	AlosL0ProductTypeDescriptor();

	/**
	 * \brief  Class constructor
	 * \param  Satellite Name
	 */	
	explicit AlosL0ProductTypeDescriptor(const std::string&);


	/**
	 * \brief  Class destructor 
	 */
	virtual ~AlosL0ProductTypeDescriptor() throw();

private:

	/**
	 * \brief  Copy constructor
	 */
	AlosL0ProductTypeDescriptor(const AlosL0ProductTypeDescriptor & ); 


	/**
	 * \brief  Operator=
	 */
	AlosL0ProductTypeDescriptor &operator=(const AlosL0ProductTypeDescriptor &);


};

_ACS_END_NAMESPACE

#endif //_AlosL0ProductTypeDescriptor_H_
