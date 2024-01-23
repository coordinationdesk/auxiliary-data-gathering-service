// PRQA S 1050 EOF
/*

	Copyright 2002-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.5  2013/12/09 14:01:13  giucas
	Library completely revisited.
	
	Revision 2.4  2011/06/07 07:19:00  giucas
	Slicing parameters made double
	
	Revision 2.3  2011/06/01 06:43:09  giucas
	Added slice parameters retrieval
	
	Revision 2.2  2009/12/11 13:19:18  giucas
	Added handling of ListOfFields
	
	Revision 2.1  2009/11/09 10:10:09  giucas
	Added
	

*/

#ifndef _Sentinel1L0ProductTypeDescriptor_H_
#define _Sentinel1L0ProductTypeDescriptor_H_ 

#include <acs_c++config.hpp>
#include <L0ProductTypeDescriptor.h>


_ACS_BEGIN_NAMESPACE(acs)

class Sentinel1L0ProductTypeDescriptor : public L0ProductTypeDescriptor // PRQA S 2153
{
public:

	/**
	 * \brief  Default Class constructor. Using this constructor the class
	 *		   data are read from database using Configuration Space
	 */	
	Sentinel1L0ProductTypeDescriptor();

	/**
	 * \brief  Class constructor
	 * \param  Satellite Name
	 */	
	explicit Sentinel1L0ProductTypeDescriptor(const std::string&);

	/**
	 * \brief  Class destructor 
	 */
	virtual ~Sentinel1L0ProductTypeDescriptor() throw();

private:

	/**
	 * \brief  Copy constructor
	 */
	Sentinel1L0ProductTypeDescriptor(const Sentinel1L0ProductTypeDescriptor & ); 


	/**
	 * \brief  Operator=
	 */
	Sentinel1L0ProductTypeDescriptor &operator=(const Sentinel1L0ProductTypeDescriptor &);

};

	
_ACS_END_NAMESPACE


#endif //_Sentinel1L0ProductTypeDescriptor_H_
